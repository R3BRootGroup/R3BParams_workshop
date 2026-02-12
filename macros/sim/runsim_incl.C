/******************************************************************************
 *   Copyright (C) 2026 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2026 Members of R3B Collaboration                          *
 *                                                                            *
 *             This software is distributed under the terms of the            *
 *                 GNU General Public Licence (GPL) version 3,                *
 *                    copied verbatim in the file "LICENSE".                  *
 *                                                                            *
 * In applying this license GSI does not waive the privileges and immunities  *
 * granted to it by virtue of its status as an Intergovernmental Organization *
 * or submit itself to any jurisdiction.                                      *
 ******************************************************************************/

#include <TStopwatch.h>
#include <TString.h>
#include <TSystem.h>
#include <memory>

void runsim_incl(const TString fInclFile = "/path_to/inclabla/build/p_Sn132_700.root",
                 const int nbevents = 100)
{
    // Timer
    TStopwatch timer;
    timer.Start();

    // Logging
    auto logger = FairLogger::GetLogger();
    logger->SetLogVerbosityLevel("low");
    logger->SetLogScreenLevel("info");
    logger->SetColoredLog(true);

    // System paths
    const TString workDirectory = getenv("VMCWORKDIR");
    gSystem->Setenv("GEOMPATH", workDirectory + "/geometry");
    gSystem->Setenv("CONFIG_DIR", workDirectory + "/gconfig");

    // Output files
    const TString simufile = "sim.root";
    const TString parafile = "par.root";

    // Store tracks for visualization
    Bool_t fVis = true;

    // Input GLAD geometry
    const TString fGladGeo = "glad_v2025.1.geo.root";

    // Input CALIFA geometry
    const TString fCalifaGeo = "califa_full.geo.root";

    // Input TofD geometry
    const TString fTofDGeo = "tofd_v2025.6.geo.root";

    // Basic simulation setup
    auto run = std::make_unique<FairRunSim>();
    run->SetName("TGeant4");
    run->SetStoreTraj(fVis);
    run->SetMaterials("media_r3b.geo");

    auto config = std::make_unique<FairGenericVMCConfig>();
    run->SetSimulationConfig(std::move(config));
    run->SetSink(std::make_unique<FairRootFileSink>(simufile.Data()));

    // Get run time data base
    auto rtdb = run->GetRuntimeDb();

    // INCL generator
    auto* genINCL = new R3BINCLRootGenerator(fInclFile.Data());
    genINCL->SetXYZ(0., 0., 0.);       // cm
    genINCL->SetDxDyDz(0.5, 0.5, 0.5); // cm

    // FairRoot Primary Generator manager
    auto primGen = new FairPrimaryGenerator();
    primGen->AddGenerator(genINCL);
    run->SetGenerator(primGen);

    // Geometry: Cave
    auto cave = new R3BCave("CAVE");
    cave->SetGeometryFileName("r3b_cave_vacuum.geo");
    run->AddModule(cave);

    // Geometry: Califa
    auto calsim = new R3BCalifa(fCalifaGeo.Data(), { 0., 0., 0. });
    calsim->SelectGeometryVersion(0);
    run->AddModule(calsim);

    // Califa Digitizer
    auto califaDig = new R3BCalifaDigitizer();
    run->AddTask(califaDig);

    // Califa Task: CrystalCal to Cluster
    auto califaCal2Cluster = new R3BCalifaCrystalCal2Cluster();
    califaCal2Cluster->SetCrystalThreshold(0.1); // 100 keV
    run->AddTask(califaCal2Cluster);

    // Geometry: GLAD
    run->AddModule(new R3BGladMagnet(fGladGeo.Data()));

    // GLAD Filed
    auto* GladField = new R3BGladFieldMap("R3BGladMap");
    GladField->SetFieldfromCurrent(2600.); // Current in Amperes
    run->SetField(GladField);

    // Geometry TofD
    auto* rtof = new TGeoRotation("Tofrot");
    rtof->RotateY(-18.);
    auto tofdsim = new R3BTofD(fTofDGeo.Data(), { -177., 0., 900., rtof });
    run->AddModule(tofdsim);

    // NeuLAND detector with 13 DP
    run->AddModule(new R3BNeuland(13, { 0., 0., 1574. }));

    // Init
    run->Init();

    // Save field parameters
    auto* fieldPar = dynamic_cast<R3BFieldPar*>(rtdb->getContainer("R3BFieldPar"));
    fieldPar->SetParameters(GladField);
    fieldPar->setChanged();

    auto parFileIO = new FairParRootFileIo(true);
    parFileIO->open(parafile);
    rtdb->setOutput(parFileIO);

    // Simulate
    run->Run(nbevents);

    // Save parameters
    rtdb->saveOutput();

    // Report
    timer.Stop();
    std::cout << "Real time: " << timer.RealTime() << "s, CPU time: " << timer.CpuTime() << "s" << std::endl;
    std::cout << "Macro finished successfully." << std::endl;
}

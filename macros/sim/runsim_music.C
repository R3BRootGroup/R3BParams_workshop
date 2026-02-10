/******************************************************************************
 *   Copyright (C) 2023 GSI Helmholtzzentrum für Schwerionenforschung GmbH    *
 *   Copyright (C) 2023 Members of R3B Collaboration                          *
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

void runsim_music(int nbevents = 100)
{
    // Timer
    TStopwatch timer;
    timer.Start();

    // Logger
    auto fLogger = FairLogger::GetLogger();
    fLogger->SetLogVerbosityLevel("low");
    fLogger->SetLogScreenLevel("info");
    fLogger->SetColoredLog(true);

    // Output files
    const TString simufile = "sim.root";
    const TString parafile = "par.root";

    // Store tracks for visualization
    Bool_t fVis = true;

    // System paths
    const TString workDirectory = getenv("VMCWORKDIR");
    gSystem->Setenv("GEOMPATH", workDirectory + "/geometry");
    gSystem->Setenv("CONFIG_DIR", workDirectory + "/gconfig");

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

    // Primary particle generator
    auto ionGen = new FairIonGenerator(82, 208, 82, 1, 0., 0., 1.09, 0., 0., 0.);
    auto primGen = new FairPrimaryGenerator();
    primGen->AddGenerator(ionGen);
    run->SetGenerator(primGen);

    // Geometry: Cave
    auto cave = new R3BCave("CAVE");
    cave->SetGeometryFileName("r3b_cave.geo");
    run->AddModule(cave);

    // Geometry: Music
    run->AddModule(new R3BMusic("music_v2023.2.geo.root", { 0., 0., 60. }));

    run->SetField(nullptr);

    // Init tasks
    run->Init();

    // Connect runtime parameter file
    auto parFileIO = new FairParRootFileIo(true);
    parFileIO->open(parafile);
    rtdb->setOutput(parFileIO);

    // Run the simulation
    run->Run(nbevents);

    // Save parameters
    rtdb->saveOutput();

    // Report
    timer.Stop();
    std::cout << "Real time: " << timer.RealTime() << " s, CPU time: " << timer.CpuTime() << " s" << std::endl;
    std::cout << "Macro finished successfully." << std::endl;
}

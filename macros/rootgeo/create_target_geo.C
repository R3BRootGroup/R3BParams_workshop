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

//
//    General macro to generate a ROOT target geometry
//
//    Author: Jose Luis <j.l.rodriguez.sanchez@udc.es>
//

#include "TGeoManager.h"
#include "TMath.h"
#include <iomanip>
#include <iostream>

void create_target_geo(TString geoTag = "v2023.2") {
  // --------------------------------------------------------------------------
  //
  // Execute macro:  root -l
  //                 .L create_target_geo.C
  //                 create_target_geo()
  // --------------------------------------------------------------------------

  TGeoManager *gGeoMan = NULL;

  // -------   Load media from media file   -----------------------------------
  auto geoLoad = new FairGeoLoader("TGeo", "FairGeoLoader");
  FairGeoInterface *geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media_r3b.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;

  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/target_";
  geoFileName = geoFileName + geoTag + ".geo.root";

  // -----------------   Get and create the required media    -----------------
  FairGeoMedia *geoMedia = geoFace->getMedia();
  FairGeoBuilder *geoBuild = geoLoad->getGeoBuilder();

  FairGeoMedium *mVac = geoMedia->getMedium("vacuum");
  if (!mVac)
    Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(mVac);
  TGeoMedium *pMedVac = gGeoMan->GetMedium("vacuum");
  if (!pMedVac)
    Fatal("Main", "Medium vacuum not found");

  FairGeoMedium *mC = geoMedia->getMedium("carbon");
  if (!mC)
    Fatal("Main", "FairMedium carbon not found");
  geoBuild->createMedium(mC);
  TGeoMedium *pMedC = gGeoMan->GetMedium("carbon");
  if (!pMedC)
    Fatal("Main", "Medium carbon not found");

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = dynamic_cast<TGeoManager*>(gROOT->FindObject("FAIRGeom"));
  gGeoMan->SetName("TARGETgeom");
  TGeoVolume *top = new TGeoVolumeAssembly("TOP");
  gGeoMan->SetTopVolume(top);

  // Positions -----------------------------------------------------------------
  double dx = 0., dy = 0., dz = 0.;
  // Target dimensions
  double targetWorldSize = 3.1; // 3.1 x 3.1 cm^2
  double targetWorldL = 0.51; // cm
  double targetSize = 3.0; // 3 x 3 cm^2
  double targetL = 0.5; // cm
  
  // World definition ----------------------------------------------------------
  auto vWorld = new TGeoBBox("WBox",targetWorldSize/2.,targetWorldSize/2.,targetWorldL/2.);
  auto World = new TGeoVolume("TargetWorld", vWorld, pMedVac);
  TGeoRotation *fRefRot = NULL; // unitary rotation
  auto fMatrix1 = new TGeoCombiTrans("", dx, dy, dz, fRefRot);
  top->AddNode(World, 1, fMatrix1);

  // Target --------------------------------------------------------------------
  TGeoVolume *targetbox = gGeoManager->MakeBox("C_Target", 
                                             pMedC, 
                                             targetSize / 2.,
                                             targetSize / 2., 
                                             targetL / 2.
                                             );
  targetbox->SetVisLeaves(kTRUE);
  targetbox->SetLineColor(2); // red
  World->AddNode(targetbox,0);

  // ---------------   Check Overlaps   ----------------------------------------
  gGeoMan->CloseGeometry();
  gGeoMan->CheckOverlaps(0.001); // precision in cm
  gGeoMan->PrintOverlaps();
  gGeoMan->Test();

  // ---------------   Save geometry   -----------------------------------------
  auto geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();

  std::cout << "\033[34m Creating geometry:\033[0m "
            << "\033[33m" << geoFileName << " \033[0m" << std::endl;
  std::cout << "Macro finished succesfully." << std::endl;
}

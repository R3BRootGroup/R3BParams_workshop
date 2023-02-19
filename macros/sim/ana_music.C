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

#include <TCanvas.h>
#include <TF2.h>
#include <TH1.h>
#include <TMath.h>
#include <TStopwatch.h>
#include <TString.h>
#include <TSystem.h>
#include <memory>

void ana_music(const float nev = -1) 
{
  // Timer
  TStopwatch timer;
  timer.Start();

  // INPUT FILE
  TFile *file = TFile::Open("music.sim.root");

  // READING TREE
  TTree *tree = dynamic_cast<TTree *>(file->Get("evt"));

  // TREE ENTRIES
  Int_t nevents = tree->GetEntries();
  if (nev > -1)
    nevents = nev;
  std::cout << "Events: " << nevents << std::endl;

  // MCTrack Data
  auto MCTrackCA = new TClonesArray("R3BMCTrack");
  TBranch *branchMCTrack = dynamic_cast<TBranch *>(tree->GetBranch("MCTrack"));
  branchMCTrack->SetAddress(&MCTrackCA);

  // MUSIC Point Data
  auto musicpoint = new TClonesArray("R3BMusicPoint");
  TBranch *branchMusicData =
      dynamic_cast<TBranch *>(tree->GetBranch("MusicPoint"));
  if (branchMusicData)
    branchMusicData->SetAddress(&musicpoint);

  // Histograms
  auto h2_theta_ek =
      new TH2F("h2_Theta_Ek", "Theta vs Ek", 1000, 0., 150., 1200, 0., 1200.);

  // LOOP IN THE EVENTS
  for (int ie = 0; ie < nevents; ie++) {
    // Print current event in the analysis
    printf("Processed: %d of %d (%.2f of 100) \r", ie + 1, nevents,
           100. * (ie + 1.0) / nevents);
    fflush(stdout);

    // Clean the TClonesArrays
    musicpoint->Clear();
    MCTrackCA->Clear();

    // Fill the TClonesArrays
    tree->GetEntry(ie);

    // Read MCTrack Data
    int MCtracksPerEvent = MCTrackCA->GetEntriesFast();
    if (MCtracksPerEvent > 0) {
      for (int j = 0; j < MCtracksPerEvent; j++) {
        auto track = dynamic_cast<R3BMCTrack *>(MCTrackCA->At(j));
        auto mom = track->GetMomentumMass() * 1000.; // MeV
        auto ekin = (mom.E() - mom.M());
        auto ekin_per_n = ekin / track->GetMass();
        //  cout <<ekin_per_n << endl;
        h2_theta_ek->Fill(mom.Theta() * TMath::RadToDeg(), ekin_per_n);
      }
    }
  }

  // Define TCanvas and draw histograms, ...
  auto c1 = new TCanvas("c1");
  h2_theta_ek->Draw("col");

  // Report
  timer.Stop();
  std::cout << "Real time: " << timer.RealTime()
            << " s, CPU time: " << timer.CpuTime() << " s" << std::endl;
  std::cout << "Macro finished successfully." << std::endl;
}

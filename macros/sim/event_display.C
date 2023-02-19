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

void event_display() 
{
  // Logging
  auto fLogger = FairLogger::GetLogger();
  fLogger->SetLogVerbosityLevel("low");
  fLogger->SetLogScreenLevel("info");
  fLogger->SetColoredLog(true);

  // Open files with FairRunAna
  auto fRun = new FairRunAna();
  fRun->SetSource(new FairFileSource("music.sim.root"));
  fRun->SetSink(new FairRootFileSink("test.root"));

  // Load parameters
  auto fRtdb = dynamic_cast<FairRuntimeDb *>(fRun->GetRuntimeDb());
  auto parIo1 = new FairParRootFileIo();
  parIo1->open("music.par.root");
  fRtdb->setFirstInput(parIo1);
  fRtdb->print();

  // Create event display
  auto fMan = new R3BEventManager();
  auto fTrack = new R3BMCTracks("Monte-Carlo Tracks");
  fMan->AddTask(fTrack);

  fMan->Init();
  gEve->GetDefaultGLViewer()->SetClearColor(0);
}

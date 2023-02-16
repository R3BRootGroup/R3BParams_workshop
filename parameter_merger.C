
void parameter_merger() {

  // ---------------------- Parameter Version List ------------------
  // Put here last parameter version for each detector at the end of the list

  std::vector<TString> califaCalParList{"califa/califacalibparams_v1.par"};

  std::vector<TString> califaMapParList{"califa/califamappingparams_v1.par"};

  std::vector<TString> califaGeoParList{"califa/califageoparams_v1.par"};

  std::vector<TString> amsMapParList{"ams/amsmappingparams_v1.par"};

  std::vector<TString> amsCalParList{"ams/amscalibparams_v1.par"};

  std::vector<TString> amsGeoParList{"ams/amsgeoparams_v1.par"};


  // -------------------- Setting Parameters --------------------
  R3BCalifaCrystalCalPar *califaCalPar;
  R3BCalifaMappingPar *califaMapPar;
  R3BTGeoPar *califaGeoPar;

  R3BAmsStripCalPar *amsStripCalPar;
  R3BAmsMappingPar *amsMapPar;
  R3BTGeoPar *amsGeoPar;

  FairParAsciiFileIo *inputAscii = new FairParAsciiFileIo();

  FairRuntimeDb *rtdb = FairRuntimeDb::instance();

  // ------------------- Setting Outputs ----------------------------
  // Here Run Id's do not matter. We only need versions of each parameter

  Bool_t kParameterMerged = kFALSE;
  FairParRootFileIo *parOut = new FairParRootFileIo(kParameterMerged);
  parOut->open("workshop_expParams.root", "RECREATE");
  rtdb->setOutput(parOut);
  rtdb->addRun(1);

  for (Int_t k = 0; k < califaCalParList.size(); k++) {

    califaCalPar =
        (R3BCalifaCrystalCalPar *)rtdb->getContainer("califaCrystalCalPar");

    inputAscii->open(califaCalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < califaMapParList.size(); k++) {

    califaMapPar =
        (R3BCalifaMappingPar *)rtdb->getContainer("califaMappingPar");

    inputAscii->open(califaMapParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < califaGeoParList.size(); k++) {

    califaGeoPar = (R3BTGeoPar *)rtdb->getContainer("CalifaGeoPar");

    inputAscii->open(califaGeoParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < amsMapParList.size(); k++) {

    amsMapPar = (R3BAmsMappingPar *)rtdb->getContainer("amsMappingPar");

    inputAscii->open(amsMapParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }

  for (Int_t k = 0; k < amsCalParList.size(); k++) {

    amsStripCalPar = (R3BAmsStripCalPar *)rtdb->getContainer("amsStripCalPar");

    inputAscii->open(amsCalParList.at(k), "in");
    rtdb->setFirstInput(inputAscii);
    rtdb->initContainers(k + 1);
    rtdb->saveOutput();
  }


}

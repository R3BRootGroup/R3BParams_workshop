void insert_parameter(){


    /* Here goes your parameter file */
    TString calFile = "../../califa/califacalibparams_v1.par";


    /* Declare an object to store mapping parameters */
    R3BCalifaCrystalCalPar *califaCalPar;

    FairParAsciiFileIo *inputAscii = new FairParAsciiFileIo();

    /* This is what we use to manage parameters in R3BRoot */
    FairRuntimeDb *rtdb = FairRuntimeDb::instance();

    califaCalPar = (R3BCalifaCrystalCalPar*)rtdb->getContainer("califaCrystalCalPar");

    TList* parList = new TList();
     parList->Add(new TObjString(calFile));


    inputAscii->open(parList);

    rtdb->setFirstInput(inputAscii);

    rtdb->initContainers(1);

    Int_t crystalNumber = 3; // Starts at CryID = 1

    Int_t offsetPosition = califaCalPar->GetNumParametersFit()*(crystalNumber-1) + 1;
    Int_t slopePosition  = califaCalPar->GetNumParametersFit()*(crystalNumber-1);

    Float_t newOffset = 0.1;
    Float_t newSlope  = 2.0;

    califaCalPar->SetCryCalParams(newOffset,offsetPosition);
    califaCalPar->SetCryCalParams(newSlope,slopePosition);

    FairParAsciiFileIo *parOut = new FairParAsciiFileIo();
     parOut->open("corrected_file.par", "out");

    rtdb->setOutput(parOut);

    rtdb->saveOutput();

}

void get_crystal_info(Int_t crystal){


    /* Here goes your parameter file */
    TString calFile = "../../califa/califacalibparams_v1.par";
    TString mapFile = "../../califa/califamappingparams_v1.par";


    /* Declare an object to store mapping parameters */
    R3BCalifaMappingPar *califaMapPar;
    R3BCalifaCrystalCalPar *califaCalPar;

    FairParAsciiFileIo *inputAscii = new FairParAsciiFileIo();

    /* This is what we use to manage parameters in R3BRoot */
    FairRuntimeDb *rtdb = FairRuntimeDb::instance();


    califaMapPar = (R3BCalifaMappingPar *)rtdb->getContainer("califaMappingPar");
    califaCalPar = (R3BCalifaCrystalCalPar *)rtdb->getContainer("califaCrystalCalPar");

    TList* parList = new TList();
    parList->Add(new TObjString(calFile));
    parList->Add(new TObjString(mapFile));


    inputAscii->open(parList);

    rtdb->setFirstInput(inputAscii);

    rtdb->initContainers(1);

    Int_t half,ring,preamp,channel,type,apd_number,febex_slot,febex_mod,febex_channel,lab,mrcc_mod,mrcc_bus,mrcc_preamp;
    Float_t voltage;
    Float_t offset,slope;


    if(!califaMapPar->GetInUse(crystal)){

     cout<<"Crystal "<<crystal<<" was not installed in this experiment!!"<<endl;
     return;

    }

    TArrayF * cry_cal_params;

    half          = califaMapPar->GetHalf( crystal);
    ring          = califaMapPar->GetRing( crystal);
    preamp        = califaMapPar->GetPreamp( crystal);
    channel       = califaMapPar->GetChannel( crystal);
    type          = califaMapPar->GetCrystalType( crystal);
    apd_number    = califaMapPar->GetApdNumber( crystal);
    voltage       = califaMapPar->GetVoltage( crystal);
    febex_slot    = califaMapPar->GetFebexSlot( crystal);
    febex_mod     = califaMapPar-> GetFebexMod( crystal);
    febex_channel = califaMapPar->GetFebexChannel( crystal);
    lab           = califaMapPar->GetLab( crystal);
    mrcc_mod      = califaMapPar->GetMrccModule( crystal);
    mrcc_bus      = califaMapPar->GetMrccBus( crystal);
    mrcc_preamp   = califaMapPar->GetMrccPreamp( crystal);


    cry_cal_params = califaCalPar->GetCryCalParams();

    offset = cry_cal_params->GetAt(2*(crystal - 1));
    slope = cry_cal_params->GetAt(2*(crystal - 1) + 1);



   cout<<"=================================="<<endl;
   cout<<"Information for Crystal "<<crystal<<" : "<<endl;
   cout<<"Half : "<<half<<endl;
   cout<<"Ring : "<<ring<<endl;
   cout<<"Preamp : "<<preamp<<endl;
   cout<<"Channel : "<<channel<<endl;
   cout<<"Type : "<<type<<endl;
   cout<<"Apd Number : "<<apd_number<<endl;
   cout<<"Voltage : "<<voltage<<endl;
   cout<<"Febex Slot : "<<febex_slot<<endl;
   cout<<"Febex Mod : "<<febex_mod<<endl;
   cout<<"Febex Channel : "<<febex_channel<<endl;
   cout<<"Lab : "<<lab<<endl;
   cout<<"Mrcc Mod : "<<mrcc_mod<<endl;
   cout<<"Mrcc Bus : "<<mrcc_bus<<endl;
   cout<<"Mrcc Preamp : "<<mrcc_preamp<<endl;

   cout<<endl;
   cout<<"Calibration Parameters : "<<endl;
   cout<<"Offset : "<<offset<<endl;
   cout<<"Slope : "<<slope<<endl;



   vector<Double_t> cryIdVec;
   vector<Double_t> slopeVec;
   vector<Double_t> constantVec;

   for(int i = 0 ; i < califaCalPar->GetNumCrystals() ; i++) {

    if(califaMapPar->GetInUse(i+1)) {

     cryIdVec.push_back(i+1);
     slopeVec.push_back(cry_cal_params->GetAt(2*(i) + 1));
     constantVec.push_back(cry_cal_params->GetAt(2*(i)));

    }
  }


 TGraph *constantGraph = new TGraph(cryIdVec.size(), &(cryIdVec.at(0)), &(constantVec.at(0)));
 TGraph *slopeGraph    = new TGraph(cryIdVec.size(), &(cryIdVec.at(0)), &(slopeVec.at(0)));

 constantGraph->SetTitle("Offset");
 slopeGraph->SetTitle("Slope");

 constantGraph->GetXaxis()->SetTitle("Crystal ID");
 slopeGraph->GetXaxis()->SetTitle("Crystal ID");

  TCanvas *myCanvas = new TCanvas("myCanvas","myCanvas");
    myCanvas->Divide(2,1);

  myCanvas->cd(1);
  constantGraph->Draw("A*");

  myCanvas->cd(2);
  slopeGraph->Draw("A*");














}

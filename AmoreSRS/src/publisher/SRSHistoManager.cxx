//Author: Kondo GNANVO 18/08/2010  

#define NTIMEBINS 30
#define NDATA 4000
#include "SRSHistoManager.h"

ClassImp(SRSHistoManager);

//============================================================================================
SRSHistoManager::SRSHistoManager(const char * histoCfgname, Int_t minClustSize, Int_t maxClustSize, Int_t maxClustMult, Int_t minPadClustSize, Int_t maxPadClustSize, Int_t maxPadClustMult) {
  printf("SRSHistoManager::SRSHistoManager() ==> Enter: maxPadClustMult=%d \n", maxPadClustMult);
  fMapping = SRSMapping::GetInstance();

  fOutputFile = "toto";
  fOutputDir = "toto";
  fRunType = "RAWDATA";

  fAmoreAgentID = "0";

  fPreviousAdcCount = 99999;

  fMaxClustMult   = maxClustMult;
  fMinClustSize   = minClustSize;
  fMaxClustSize   = maxClustSize;

  fMaxPadClustMult   = maxPadClustMult;
  fMinPadClustSize   = minPadClustSize;
  fMaxPadClustSize   = maxPadClustSize;

  fHistosRatioCut = 1;
  f1DHistNBin     = 21;
  f1DHistMinRange = 0;
  f1DHistMaxRange = 20;

  fNBin     = 256;
  fRangeMin = -0.5;
  fRangeMax = 255.5;

  fNBin2     = 14;
  fRangeMin2 = -0.5;
  fRangeMax2 = 13.5;

  fNBinMap2    = 256;
  fNBinMap     = 256;
  fRangeMinMap = -0.5;
  fRangeMaxMap = 255.5;
  fRangeMinMap2 = -0.5;
  fRangeMaxMap2 = 255.5;

  fNBinPlane     = 256;
  fRangeMinPlane = -51.2;
  fRangeMaxPlane =  51.2; 

  fNBinTime     = 30;
  fRangeMinTime = -0.5;
  fRangeMaxTime = 29.5;

  fNBinSpectrum     = 100;
  fRangeMinSpectrum = -0.5;
  fRangeMaxSpectrum = 3999.5;

  fIsFirstEvent = kTRUE;
  fEventNumber = 0;

  fNtupleSizeX = 100.0;
  fNtupleSizeY = 100.0;
  fNtupleSizeZ = 400.0;

  ResetHistos();
  RefreshHistos();

  fEventNumber = 0;

  //Set initial values...(in case fit fails)
  fRChi2      = -100;
  fMean       = -100;
  fMeanError  = -100;
  fSigma      = -100;
  fSigmaError = -100;
  printf("SRSHistoManager::SRSHistoManager() ==> Exit \n");
}

//============================================================================================
SRSHistoManager::~SRSHistoManager(){

  ClearMaps();
  ClearMapOfHistos(fResidualHistos);
  ClearMapOfHistos(fCalibrationHistos);

  ClearMapOfVectors(fFittedHistos);
  ClearMapOfVectors(fHistoTypeToDetectorMap);
  DeleteHistos();
  DeleteROOTObjs();

  delete fNtuple;
  delete fOffsetFile;
 
 }

//============================================================================================
void SRSHistoManager::SetRunParameters(TString amoreAgentId, TString runtype, TString outputdir, TString outputfile) {
  printf("\nSRSHistoManager::SetRunParameters() => Enter: \n");
  fAmoreAgentID = amoreAgentId;
  fRunType = runtype;
  fOutputDir =  outputdir;
  fOutputFile = outputfile;
  //printf("Seems like the code gfoe here?\n");
  printf("SRSHistoManager::SetRunParameters() => Exit: fOutputDir = %s:\n",fOutputDir.Data());
  printf("SRSHistoManager::SetRunParameters() => Exit: fRunType=%s, fAmoreAgentID = %s, fOutputFile = %s\n",runtype.Data(), amoreAgentId.Data(), fOutputFile.Data());
}

//====================================================================================================================
void SRSHistoManager::ClearMaps() {
  fObjTitle.clear();
  fVarType.clear();
  fObjType.clear();
  fDetector.clear();
  fDetectorPlane1.clear();
  fDetectorPlane2.clear();
  fAPVIDStr.clear();
  fNbOfEvents.clear();
  fFitParam1Str.clear();
  fFitParam2Str.clear();
  fFitFunction.clear();
  fDividedHistos.clear();
  fNameAPVIDMap.clear();
}

//====================================================================================================================
void SRSHistoManager::ClearMapOfVectors( map<TString, vector <TString> > mapOfVectors) {
  map<TString, vector <TString> >::const_iterator itr;
  for(itr = mapOfVectors.begin(); itr != mapOfVectors.end(); ++itr) {
    vector <TString > vect =  (*itr).second;
    vect.clear();
  }
  mapOfVectors.clear();
}
//====================================================================================================================
void SRSHistoManager::ClearMapOfHistos( map<TString,  TH1* > mapOfHistos) {
  map<TString, TH1*>::const_iterator hist_itr;
  for(hist_itr = mapOfHistos.begin(); hist_itr != mapOfHistos.end(); ++hist_itr){
    TString hist = (*hist_itr).first;
    delete mapOfHistos[hist];
  }
  mapOfHistos.clear();
}
//====================================================================================================================
void SRSHistoManager::DeleteHistos() {
  while (!fHist1dToReset.empty()) {
    TH1 * h = fHist1dToReset.back();
    delete h;
    fHist1dToReset.pop_back();
  }
  while (!fHist2dToReset.empty()){
    TH2 * h = fHist2dToReset.back();
    delete h;
    fHist2dToReset.pop_back();
  }
}

//====================================================================================================================
void SRSHistoManager::DeleteROOTObjs() {
  map<TString, TObject*>::const_iterator itr;
  for (itr = fObjToDraw.begin(); itr != fObjToDraw.end(); ++itr) {
    TNamed * obj = (TNamed*) (*itr).second;
    delete obj;
    printf("SRSHistoManager::(DeleteROOTObjs) == delete all ROOTs objects \n");
  }
}

//====================================================================================================================
void SRSHistoManager::ResetHistos() {
  map<TString,TObject*>::const_iterator itr;
  for (itr = fObjToDraw.begin(); itr != fObjToDraw.end(); ++itr){
    TString objName = (*itr).first;
    if (fObjType[objName]=="1D") {
      ((TH1*)fObjToDraw[objName])->Reset();
    }
    else if (fObjType[objName]=="2D") {
      ((TH2*)fObjToDraw[objName])->Reset();
    } 
    else if (fObjType[objName]=="Ntuple") {
      ((TNtuple*)fObjToDraw[objName])->Reset();
    } 
    else Error("SRSHistoManager","ResetHistos:=> Wrong histo name: %s", objName.Data());
  }

  fIsFirstEvent = kTRUE;
}

//====================================================================================================================
void SRSHistoManager::RefreshHistos() {
  while (!fHist1dToReset.empty()) {
    TH1 * h = fHist1dToReset.back();
    h->Reset();
    fHist1dToReset.pop_back();
  }
  while (!fHist2dToReset.empty()){
    TH2 * h = fHist2dToReset.back();
    h->Reset();
    fHist2dToReset.pop_back();
  }
}

//============================================================================================
void SRSHistoManager::Reset2DHistBinning(TString type, Int_t nbin, Float_t min, Float_t max, Int_t nbin2, Float_t min2, Float_t max2) {
  //    printf("  SRSHistoManager::Reset2DHistBinning()=> type=%s, nbin=%d, min=%f, max=%f, nbin=%d, min2=%f, max2=%f \n",type.Data(), nbin, min, max, nbin2, min2, max2);
  if (type == "CLUSTCOR" )  {
    f1DHistNBin     = nbin;
    f1DHistMinRange = min;
    f1DHistMaxRange = max;
    fNBinSpectrum     = nbin2;
    fRangeMinSpectrum = min2;
    fRangeMaxSpectrum = max2;
  }

  else if (type == "CHARGES_RATIODIST")  {
    fNBinMap = nbin;
    fNBinMap2= nbin2;
  }
  
  else if ( (type == "HITMAP") || (type == "CORRELATION") || (type == "PADHITPEDOFFSET") || (type == "PADRAWHITS") || (type == "PADHITDIST")  || (type == "PADCLUSTERDIST") ) {
    fNBinMap      = nbin;
    fNBinMap2     = nbin2;
    fRangeMinMap  = min;
    fRangeMaxMap  = max;
    fRangeMinMap2 = min2;
    fRangeMaxMap2 = max2;
    //    printf("  SRSHistoManager::Reset2DHistBinning()=> type=%s,fNBin =%d, fRangeMin=%f, fRangeMax=%f, fNBin2=%d, fRangeMin2=%f, fRangeMax2=%f \n",type.Data(), fNBinMap, fRangeMinMap, fRangeMaxMap, fNBinMap2, fRangeMinMap2, fRangeMaxMap2);
  }
  else Error("SRSHistoManager", "Reset2DHistBinning:=> Unknown type %s", type.Data());
}

//============================================================================================
void SRSHistoManager::Reset1DHistBinning(TString type, Int_t nbin, Float_t min, Float_t max){
  //  printf("  SRSHistoManager::ResetBinning()=> type=%s\n",type.Data());
  if ((type == "ADCDIST") || (type == "CHARGES_SH") || (type == "CHARGES_RATIO") || (type == "SPECTRUM") ) {
    fNBinSpectrum     = nbin;
    fRangeMinSpectrum = min;
    fRangeMaxSpectrum = max;
  }

  else if ((type == "CLUSTERDIST") || (type == "CLUSTMULT") || (type == "CLUSTSIZE")  ) {
    fNBinMap      = nbin;
    fRangeMinMap  = min;
    fRangeMaxMap  =  max;
  }

  else if ( (type == "PLANETIMEBIN") || (type == "CLUSTERTIMEBIN") ){
    fNBinTime     = nbin;
    fRangeMinTime = min;
    fRangeMaxTime = max;
  }

  else if ((type == "TIMESLICE") || (type == "OCCUPANCY") || (type == "CHARGECROSSTALK") || (type == "CLUSTSTAT") || (type == "NOISERESO")) {
    f1DHistNBin     = nbin;
    f1DHistMinRange = min;
    f1DHistMaxRange = max;
  }

  else Error("SRSHistoManager", "Reset1DHistBinning:=> Unknown type %s", type.Data());
}

//============================================================================================
void SRSHistoManager::SetBinning(TString type, TString detPlane1, TString detPlane2) {
  //  printf("SRSHistoManager::SetBinning()  ==> Type=%s, detPlane1=%s, detPlane2=%s \n", type.Data(), detPlane1.Data(), detPlane2.Data());

  if ((type == "ADCDIST") || (type == "CHARGES_SH") || (type == "CHARGES_RATIO") || (type == "SPECTRUM")) {
    fNBin      = fNBinSpectrum;
    fRangeMin  = fRangeMinSpectrum;
    fRangeMax  = fRangeMaxSpectrum;
    fNBin2     = fNBinSpectrum;
    fRangeMin2 = fRangeMinSpectrum;
    fRangeMax2 = fRangeMaxSpectrum;
    //    printf("  SRSHistoManager::SetBinning()  ==> Type=%s, detPlane=%s, nbin=%d, min=%.1f, max=%.1f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax);
  }

  else if ((type == "HITDIST") || (type == "HITZEROSUP") || (type == "CLUSTERHITS") || (type == "HITPEDOFFSET") || (type == "RAWHITS") || (type == "HITCOMMODE") || (type == "HITRAWDATA")) {
 
    if(fMapping->GetReadoutBoardFromDetector(detPlane1) == "PADPLANE") {
      fNBin      =  (Int_t) fMapping->GetPadDetectorMap(detPlane1) [2];
      fRangeMin  = -0.5 * fNBin * fMapping->GetPadDetectorMap(detPlane1) [0];
      fRangeMax  =  0.5 * fNBin* fMapping->GetPadDetectorMap(detPlane1) [0];
    }
    else {
      fNBin      =  NCH * fMapping->GetNbOfAPVs(detPlane1);
      fRangeMin  = -0.5 * fMapping->GetSizeOfPlane(detPlane1);
      fRangeMax  =  0.5 * fMapping->GetSizeOfPlane(detPlane1);
    }    
    //   printf("  SRSHistoManager::SetBinning()  ==> Type=%s, detPlane=%s, nbin=%d, min=%.1f, max=%.1f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax);
  }

  else if (type == "CHARGES_RATIODIST") {
    fNBin       =  fNBinMap;
    fNBin2      =  fNBinMap;
    fRangeMin   = -0.5 * fMapping->GetSizeOfPlane(detPlane1);
    fRangeMax   =  0.5 * fMapping->GetSizeOfPlane(detPlane1);
    fRangeMin2  = -0.5 * fMapping->GetSizeOfPlane(detPlane2);
    fRangeMax2  =  0.5 * fMapping->GetSizeOfPlane(detPlane2);
   //    printf("  SRSHistoManager::SetBinning()  ==> Type=%s, detPlane1=%s, nbin=%d, min=%.1f, max=%.1f, DetPlane2=%s, nBin2=%d, min2=%.1f, max2=%.1f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax, detPlane1.Data(), fNBin2, fRangeMin2, fRangeMax2);
  }

  else if ( (type == "HITMAP") || (type == "CORRELATION") || (type == "PADHITPEDOFFSET") || (type == "PADRAWHITS") || (type == "PADHITZEROSUP") || (type == "PADCLUSTERHITS") || (type == "PADHITDIST") ) {

    if ( (type == "PADHITZEROSUP") || (type == "PADCLUSTERHIT") ) {
      fNBin      =  (Int_t) fMapping->GetPadDetectorMap(detPlane1) [2]; 
      fRangeMin  = -0.5 * fNBin * fMapping->GetPadDetectorMap(detPlane1) [0];
      fRangeMax  =  0.5 * fNBin* fMapping->GetPadDetectorMap(detPlane1) [0];
      fNBin2     =  (Int_t) fMapping->GetPadDetectorMap(detPlane1) [3]; 
      fRangeMin2 = -0.5 * fNBin2 * fMapping->GetPadDetectorMap(detPlane1) [1];
      fRangeMax2 =  0.5 * fNBin2 * fMapping->GetPadDetectorMap(detPlane1) [1];
    }

    else if  ( (type == "PADHITPEDOFFSET") || (type == "PADHITDIST") || (type == "PADRAWHITS")  ) {
      fNBin       = (Int_t) fMapping->GetPadDetectorMap(detPlane1) [2]; 
      fRangeMin   = -0.5;
      fRangeMax   =  fNBin - 0.5;
      fNBin2      = (Int_t) fMapping->GetPadDetectorMap(detPlane1) [2]; 
      fRangeMin2  = -0.5;
      fRangeMax2  =  fNBin - 0.5;
    }

    else {
      if (fNBinMap == 99099)        fNBin = 256;
      else                          fNBin = fNBinMap;
      if (fNBinMap2 == 99099)       fNBin2 = 256;
      else                          fNBin2 = fNBinMap2;
      if (fRangeMinMap == 99099.0)  fRangeMin = -0.5 * fMapping->GetSizeOfPlane(detPlane1);
      else                          fRangeMin = fRangeMinMap;
      if (fRangeMinMap2 == 99099.0) fRangeMin2 = -0.5 * fMapping->GetSizeOfPlane(detPlane2);
      else                          fRangeMin2 = fRangeMinMap2;
      if (fRangeMaxMap == 99099.0)  fRangeMax = 0.5 * fMapping->GetSizeOfPlane(detPlane1);
      else                          fRangeMax = fRangeMaxMap;
      if (fRangeMaxMap2 == 99099.0) fRangeMax2 = 0.5 * fMapping->GetSizeOfPlane(detPlane2);
      else                          fRangeMax2 = fRangeMaxMap2;
    }
    // printf("  SRSHistoManager::SetBinning()  ==> Type=%s, detPlane1=%s, nbin=%d, min=%.1f, max=%.1f, DetPlane2=%s, nBin2=%d, min2=%.1f, max2=%.1f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax, detPlane1.Data(), fNBin2, fRangeMin2, fRangeMax2);
  }

  else if (type == "CMSHITMAP") {
    fNBin = 8;
    fRangeMin = 0;
    fRangeMax = 1000;

    fNBin2 = 250;
    fRangeMin2 = 0;
    fRangeMax2 = 500;
    //    printf("  SRSHistoManager::SetBinning()  ==> Type=%s, lane1=%s, nbin=%d, min=%.1f, max=%.1f, plane2=%s, nBin2=%d, min2=%.1f, max2=%.1f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax, detPlane1.Data(), fNBin2, fRangeMin2, fRangeMax2);
  }

  else if (type == "CLUSTERDIST") {
    if (fNBinMap == 99099)        fNBin = NCH *  fMapping->GetNbOfAPVs(detPlane1);
    else                          fNBin = fNBinMap;
    if (fRangeMaxMap == 99099.0)  fRangeMax = 0.5 * fMapping->GetSizeOfPlane(detPlane1);
    else                          fRangeMax = fRangeMaxMap;
    if (fRangeMinMap == 99099.0)  fRangeMin = -0.5 * fMapping->GetSizeOfPlane(detPlane1);
    else                          fRangeMin = fRangeMinMap;
    //    printf("  SRSHistoManager::SetBinning()  ==> Type=%s, detPlane=%s, nbin=%d, min=%.1f, max=%.1f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax);
  }

  else if (type == "HITCROSSTALK") {
    fNBin      = 128;
    fRangeMin  = -0.5;
    fRangeMax  = 127.5;
    //    printf("  SRSHistoManager::SetBinning()  ==> Type=%s, detPlane=%s, nbin=%d, min=%.1f, max=%.1f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax);
  }
  else if (type == "CLUSTSIZE") { 
    fNBin     = fNBinMap;
    fRangeMin = fRangeMinMap; 
    fRangeMax = fRangeMaxMap;
  }

  else if (type == "CLUSTMULT") { 
    fNBin     = fMaxClustMult + 1;
    fRangeMin = -0.5;
    fRangeMax = fMaxClustMult + 0.5;

    if(fMapping->GetReadoutBoardFromDetector(detPlane1) == "PADPLANE") {
      fNBin     = fMaxPadClustMult +1 ;
      fRangeMin = -0.5;
      fRangeMax = fMaxPadClustMult + 0.5;
    }
    printf("  SRSHistoManager::SetBinning()  ==> Type=%s, detPlane=%s, nbin=%d, min=%f, max=%f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax);
  }

  else if ((type == "TIMESLICE") || (type == "OCCUPANCY") || (type == "CHARGECROSSTALK") || (type == "CLUSTSTAT") || (type == "NOISERESO")) { 
    fNBin     = f1DHistNBin;
    fRangeMin = f1DHistMinRange;
    fRangeMax = f1DHistMaxRange;
    //    printf("  SRSHistoManager::SetBinning()  ==> Type=%s, detPlane=%s, nbin=%d, min=%.1f, max=%.1f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax);
  }

  else if (type == "CLUSTCOR") {
    fNBin2     = f1DHistNBin;
    fRangeMin2 = f1DHistMinRange;
    fRangeMax2 = f1DHistMaxRange;

    fNBin     = 64;
    fRangeMin = -0.5;
    fRangeMax = 4095.5;
    //    printf("  SRSHistoManager::SetBinning()  ==> Type=%s, detPlane1=%s, nbin=%d, min=%.1f, max=%.1f, DetPlane2=%s, nBin2=%d, min2=%.1f, max2=%.1f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax, detPlane1.Data(), fNBin2, fRangeMin2, fRangeMax2);
  }

  else if (type == "PLANETIMEBIN") {
    fNBin2     = fNBinTime;
    fRangeMin2 = fRangeMinTime;
    fRangeMax2 = fRangeMaxTime;

    fNBin     =  NCH  * fMapping->GetNbOfAPVs(detPlane1)  + 1;
    fRangeMin = -0.5 * fMapping->GetSizeOfPlane(detPlane1);
    fRangeMax =  0.5 * fMapping->GetSizeOfPlane(detPlane1);
  }

  else if (type == "CLUSTERTIMEBIN") {
    fNBin      = fNBinTime;
    fRangeMin  = fRangeMinTime;
    fRangeMax  = fRangeMaxTime;
    //    printf("  SRSHistoManager::SetBinning()  ==> Type=%s, detPlane=%s, nbin=%d, min=%.1f, max=%.1f \n", type.Data(), detPlane1.Data(), fNBin, fRangeMin, fRangeMax);
  }
  else Error("\n   SRSHistoManager","SetBinning:=> Unknown type %s", type.Data());
}

//=========================================================================================================================
void SRSHistoManager::Book1DHistos(TString histoName, TString title, TString type, TString detPlane1, TString detPlane2){
  //printf("\nSRSHistoManager::Book1DHistos() ==> Enter: Histo=%s, Type=%s, DetPlane1=%s, DetPlane2=%s \n", histoName.Data(), type.Data(),  detPlane1.Data(), detPlane1.Data());
  SetBinning(type, detPlane1, detPlane2);
  TH1F * h = new TH1F(histoName, title, fNBin, fRangeMin, fRangeMax);
  fObjToDraw[histoName]      = h;
  fDetectorPlane1[histoName] = detPlane1;
  fDetectorPlane2[histoName] = detPlane2;
  fObjType[histoName]        = "1D";
  fVarType[histoName]        = type;
  fObjTitle[histoName]       = title;
  fNbOfEvents[histoName]     = 0;
  fHistoTypeToDetectorMap[detPlane1].push_back(histoName);
  fHistoTypeToDetectorMap[detPlane2].push_back(histoName);
  //  printf("SRSHistoManager::Book1DHistos() ==> Exit: fNBin=%d, fRangeMin=%f, fRangeMax=%f \n", fNBin, fRangeMin, fRangeMax);
}

//=========================================================================================================
void SRSHistoManager::Book1DHistos(TString histoName, TString title, TString type, TString detPlane){
  printf("\nSRSHistoManager::Book1DHistos() ==> Enter: Histo=%s, Type=%s, DetPlane=%s \n", histoName.Data(), type.Data(),  detPlane.Data());
  SetBinning(type, detPlane, detPlane);
  TH1F * h = new TH1F(histoName, title, fNBin, fRangeMin, fRangeMax);
  fObjToDraw[histoName]      = h;
  fDetector[histoName]       = detPlane;
  fDetectorPlane1[histoName] = detPlane;
  fDetectorPlane2[histoName] = detPlane;
  fObjType[histoName]        = "1D";
  fVarType[histoName]        = type;
  fObjTitle[histoName]       = title;
  fNbOfEvents[histoName]     = 0;
  fHistoTypeToDetectorMap[detPlane].push_back(histoName);
  printf("SRSHistoManager::Book1DHistos() ==> Exit: fNBin=%d, fRangeMin=%f, fRangeMax=%f \n", fNBin, fRangeMin, fRangeMax);
}

//====================================================================================================================
void SRSHistoManager::Book2DHistos(TString histoName, TString title, TString type, TString detector) {
  printf("\nSRSHistoManager::Book2DHisto() ==> Enter: Histo=%s, Type=%s, Detector=%s \n", histoName.Data(), type.Data(),  detector.Data());
  SetBinning(type, detector, detector);
  TH2F * h  = new TH2F(histoName, title, fNBin, fRangeMin, fRangeMax, fNBin2, fRangeMin2, fRangeMax2);
  fObjToDraw[histoName]      = h;
  fDetector[histoName]       = detector;
  fDetectorPlane1[histoName] = detector;
  fDetectorPlane2[histoName] = detector;
  fObjType[histoName]        = "2D";
  fVarType[histoName]        = type;
  fObjTitle[histoName]       = title;
  fNbOfEvents[histoName]     = 0;
  fHistoTypeToDetectorMap[detector].push_back(histoName);
  printf("SRSHistoManager::Book2DHistos() ==> Exit: fNBin=%d, fRangeMin=%f, fRangeMax=%f, fNBin=%d, fRangeMin2=%f, fRangeMax2=%f \n", fNBin, fRangeMin, fRangeMax, fNBin2, fRangeMin2, fRangeMax2);
}

//====================================================================================================================
void SRSHistoManager::Book2DHistos(TString histoName, TString title, TString type, TString detPlane1, TString detPlane2) {
  printf("\n  SRSHistoManager::Book2DHisto() ==> Enter: Histo=%s, Type=%s, DetPlane1=%s, DetPlane2=%s \n", histoName.Data(), type.Data(),  detPlane1.Data(), detPlane1.Data());
  SetBinning(type, detPlane1, detPlane2);
  TH2F * h  = new TH2F(histoName, title, fNBin, fRangeMin, fRangeMax, fNBin2, fRangeMin2, fRangeMax2);
  fObjToDraw[histoName]      = h;
  fDetectorPlane1[histoName] = detPlane1;
  fDetectorPlane2[histoName] = detPlane2;
  fObjType[histoName]        = "2D";
  fVarType[histoName]        = type;
  fObjTitle[histoName]       = title;
  fNbOfEvents[histoName]     = 0;
  fHistoTypeToDetectorMap[detPlane1].push_back(histoName);
  fHistoTypeToDetectorMap[detPlane2].push_back(histoName);
  printf("SRSHistoManager::Book2DHistos()=> fNBinMap=%d, fRangeMinMap=%f, fRangeMaxMap=%f, fNBinMap2=%d, fRangeMinMap2=%f, fRangeMaxMap2=%f \n",fNBinMap, fRangeMinMap, fRangeMaxMap, fNBinMap2, fRangeMinMap2, fRangeMaxMap2);
}

//=========================================================================================================================
void SRSHistoManager::BookHitTimingHistos(){
  if (fRunType == "HITTIMING") {
    for (Int_t i =0; i < 20; i++) {
      stringstream hitno;
      hitno << i;
      TString  hitNoStr = hitno.str();
      TString histoName = "timingHitNo_" + i; 
      TString title = histoName;
      TString type = "HITTIMING";
      TH1F * h = new TH1F(histoName, title, 30, 0, 29);
      fObjToDraw[histoName]  = h;
      fObjType[histoName]    = "1D";
      fVarType[histoName]    = type;
      fObjTitle[histoName]   = title;
      fNbOfEvents[histoName] = 0;
      //      printf("  SRSHistoManager::BookHitTimingHistos() ==> histo = %s booked \n",histoName.Data() );
    }
  } 
}

//=========================================================================================================================
//     RAW DATA
//=========================================================================================================================
void SRSHistoManager::BookRawDataHistos() {
    // printf("  SRSHistoManager::BookRawDataHistos() ==> ENTER \n");
  if (fRunType == "RAWDATA") {
    map <Int_t, TString> listOfAPVs = fMapping->GetAPVFromIDMap();
    map <Int_t, TString>::const_iterator apv_itr;
    printf("  SRSHistoManager::BookRawDataHisto() ==> \n");
    for (apv_itr =listOfAPVs.begin(); apv_itr != listOfAPVs.end(); ++ apv_itr) { 
      Int_t apvID = (*apv_itr).first;
      TString histoName = fMapping->GetAPVFromID(apvID);
      TString title = histoName;
      TString type = "RAWDATA";
      TH1F * h = new TH1F(histoName, title, 2000, 0, 1999);
      fAPVIDStr[histoName] = apvID;
      fObjToDraw[histoName]  = h;
      fObjType[histoName]    = "1D";
      fVarType[histoName]    = type;
      fObjTitle[histoName]   = title;
      fNbOfEvents[histoName] = 0;
            // printf("  SRSHistoManager::BookRawDataHistos() ==> histo = %s booked \n",histoName.Data() );
    }
  } 
}

//=========================================================================================================================
void SRSHistoManager::RawDataRun(SRSEventBuilder * eventbuilder) { 

  Int_t eventNumber = eventbuilder->GetEventNumber();

    printf("  SRSHistoManager::FillRawDataHistos() ==> eventNumber = %d \n", eventNumber );

    map<TString,TObject*>::const_iterator objToDraw_itr; // issue

    for (objToDraw_itr = fObjToDraw.begin(); objToDraw_itr != fObjToDraw.end(); ++objToDraw_itr) {

    TString histoName = (*objToDraw_itr).first;  
    cout << histoName << endl;
//cout<<"SDKGAFHDSFDJKSAGFJKHGJKHAGDSFKGSAHK"<<endl;

    if (!fObjToDraw[histoName]) {cout<< "HERE\n"; }continue; 
	cout<<"1"<<endl;
    if (fVarType[histoName]=="RAWDATA") {
      fHist1dToReset.push_back((TH1F*)(fObjToDraw[histoName]));
		cout << "test 01" << endl;
      Int_t apvID = fAPVIDStr[histoName];
		cout << "test 02" << endl;
      if(!fMapping->IsAPVIDMapped(apvID)) {
	Warning("FillHistos","=> APVID=%d not connected, skip histo %s\n", apvID, histoName.Data());
	continue;
      }
      SRSAPVEvent * apvEvent = eventbuilder->GetAPVEventFromAPVKey(fMapping->GetAPVNoFromID(apvID));
      apvEvent->ComputeRawData16bits(); 
      vector <UInt_t> rawdata16bitsVect = apvEvent->GetRawData16bits();
      Int_t size = ((Int_t) (rawdata16bitsVect.size()));
      vector <UInt_t>::const_iterator  rawdata16_itr;
      Int_t stripNo = 0;
      Int_t apvEventNb = apvEvent->GetEventNumber();

      stringstream out;
      out << apvEventNb; 
      TString apvEventNbStr = out.str();
      TString newTitle = fObjTitle[histoName];
      ((TH1*) fObjToDraw[histoName])->SetTitle(newTitle);
      for(rawdata16_itr = rawdata16bitsVect.begin(); rawdata16_itr != rawdata16bitsVect.end(); rawdata16_itr++ ) {
	UInt_t rawdata16  =  * rawdata16_itr;
	((TH1*) fObjToDraw[histoName])->SetBins(size,0,size);
	((TH1*) fObjToDraw[histoName])->Fill(stripNo, rawdata16);
	//	printf("  SRSHistoManager::FillRawDataHistos() ==> apvID = %d, StripNo = %d, rawdata = %d\n", apvID, stripNo, rawdata16 );
	stripNo++;
      }
cout<<"2"<<endl;

    }
	cout<<"3"<<endl;
  }
}

//====================================================================================================================
//        RESIDUALS
//====================================================================================================================
void SRSHistoManager::BookTrackingHistos(SRSTrack * track) {
  printf("  SRSHistoManager::BookTrackingHistos() ==> ENTER \n");
  GetStyle(1);
  map <TString, TString > detectorList =  track->GetDetectorList();
  map <TString, TString >::const_iterator det_itr;
  for(det_itr = detectorList.begin(); det_itr != detectorList.end(); ++det_itr) {

    TString detName = (*det_itr).first;

    if ((detName == "GEM1") || (detName == "GEM2") || (detName == "GEM3")) continue ;
    TString planeX = (fMapping->GetDetectorPlaneListFromDetector(detName)).front();
    TString planeY = (fMapping->GetDetectorPlaneListFromDetector(detName)).back();

    Int_t   xnbin = track->GetXNBinResiduals(detName);
    Float_t xmin  = track->GetXRangeMinResiduals(detName);
    Float_t xmax  = track->GetXRangeMaxResiduals(detName);

    TString xresName = "residual_" + detName +"X";
    TString xresTitle = detName  + ": Residuals on x-axis";
    TH1S * hX  = new TH1S(xresName, xresTitle, xnbin, xmin, xmax);
    printf("  SRSHistoManager::BookTrackingHistos()=> x_residual histo = %s \n", xresName.Data());
    hX->SetXTitle("xFit - xMeas. (mm)");
    hX->SetYTitle("Counts");
    fResidualHistos[xresName] = hX;

    Int_t   ynbin = track->GetYNBinResiduals(detName);
    Float_t ymin  = track->GetYRangeMinResiduals(detName);
    Float_t ymax  = track->GetYRangeMaxResiduals(detName); 
    TString yresName = "residual_" + detName + "Y";
    TString yresTitle = detName  + ": Residuals on y-axis"; 
    TH1S * hY  = new TH1S(yresName, yresTitle, ynbin, ymin, ymax);
    printf("  SRSHistoManager::BookTrackingHistos()=> y_residual histo = %s \n", yresName.Data());
    hY->SetXTitle("yFit - yMeas. (mm)");
    hY->SetYTitle("Counts");
    fResidualHistos[xresName] = hX;    fResidualHistos[yresName] = hY;

    //    if(detName.Contains("PADGEM")) {

      /**
      TString xRespLin = "respLin2DCOLZ_" + detName + "X";
      TString xResLinTitle = detName  + ": Linearity on x";
      TH2S* hRespLinX = new TH2S(xRespLin, xResLinTitle, 200, -20.0, 20.0,  200, -20.0, 20.0);
      hRespLinX->SetXTitle("x [Track] (mm)");
      hRespLinX->SetYTitle("x [Meas.] (mm)");
      fResidualHistos[xRespLin] = hRespLinX;

      TString xDiffNonLin = "diffNonLin2DCOLZ_" + detName + "X";
      TString xDiffNonLinTitle = detName  + ": DNL on x";
      TH2F* hDiffNonLinX   = new TH2F(xDiffNonLin, xDiffNonLinTitle, 200, -20.0, 20.0, 200, ymin, ymax);
      hDiffNonLinX->SetXTitle("x [Track] (mm)");
      hDiffNonLinX->SetYTitle("x [Track] - x[Meas.] (mm)");
      fResidualHistos[xDiffNonLin] = hDiffNonLinX;
      */
      /**
      TString xresNameCorr = "residual_" + detName +"XCorr";
      TString xresTitleCorr = detName  + ": Residuals on x-axis after correction";
      TH1S * hXCorr  = new TH1S(xresNameCorr, xresTitleCorr, xnbin, xmin, xmax);
      printf("  SRSHistoManager::BookTrackingHistos()=> x_residual histo = %s \n", xresNameCorr.Data());
      hXCorr->SetXTitle("xFit - xMeas. (mm)");
      hXCorr->SetYTitle("Counts");
      fResidualHistos[xresNameCorr] = hXCorr;

      TString xRespLinCorr = "respLin2DCOLZ_" + detName + "XCorr";
      TString xResLinTitleCorr = detName  + ": Linearity on x after correction";
      TH2S* hRespLinXCorr = new TH2S(xRespLinCorr, xResLinTitleCorr, 200, -30.0, 30.0,  200, -30.0, 30.0);
      hRespLinXCorr->SetXTitle("x [Track] (mm)");
      hRespLinXCorr->SetYTitle("x [Meas.] (mm)");
      fResidualHistos[xRespLinCorr] = hRespLinXCorr;

      TString xDiffNonLinCorr = "diffNonLin2DCOLZ_" + detName + "XCorr";
      TString xDiffNonLinTitleCorr = detName  + ": DNL on x after Correction";
      TH2F* hDiffNonLinXCorr   = new TH2F(xDiffNonLinCorr, xDiffNonLinTitleCorr, 200, -30.0, 30.0, 200, ymin, ymax);
      hDiffNonLinXCorr->SetXTitle("x [Track] (mm)");
      hDiffNonLinXCorr->SetYTitle("x [Track] - x[Meas.] (mm)");
      fResidualHistos[xDiffNonLinCorr] = hDiffNonLinXCorr;
  
      TString xDiffNonLin1D = "diffNonLin_" + detName + "X";
      TProfile* hDiffNonLinX1D = new TProfile(xDiffNonLin1D, xDiffNonLinTitle, 180, -30.0, 30.0);
      fResidualHistos[xDiffNonLin1D] = hDiffNonLinX1D;   
      */
      /**
      TString yRespLin = "respLin2DCOLZ_" + detName + "Y";
      TString yResLinTitle = detName  + ": Response Linearity on y-axis";
      TH2S* hRespLinY = new TH2S(yRespLin, yResLinTitle, 100, -24.0, -12.0,  100, -24.0, -12.0);
      hRespLinY->SetXTitle("y [Track] (mm)");
      hRespLinY->SetYTitle("y [Meas.] (mm)");
      fResidualHistos[yRespLin] = hRespLinY;

      TString yDiffNonLin = "diffNonLin2DCOLZ_" + detName + "Y";
      TString yDiffNonLinTitle = detName  + ": Diff Non Linearity on y-axis";
      TH2F* hDiffNonLinY   = new TH2F(yDiffNonLin, yDiffNonLinTitle, 100, -24.0, -12.0, 200, ymin, ymax);
      hDiffNonLinY->SetXTitle("y [Track] (mm)");
      hDiffNonLinY->SetYTitle("y [Track] - y[Meas.] (mm)");
      fResidualHistos[yDiffNonLin] = hDiffNonLinY;
      */

      /**
      TH1S* hXresRange1 = new TH1S("xResRange1", "xResRange1", xnbin, xmin, xmax); 
      TH1S* hXresRange2 = new TH1S("xResRange2", "xResRange2", xnbin, xmin, xmax); 
      TH1S* hXresRange3 = new TH1S("xResRange3", "xResRange3", xnbin, xmin, xmax); 
      TH1S* hXresRange4 = new TH1S("xResRange4", "xResRange4", xnbin, xmin, xmax); 
      TH1S* hXresRange5 = new TH1S("xResRange5", "xResRange5", xnbin, xmin, xmax); 
      TH1S* hXresRange6 = new TH1S("xResRange6", "xResRange6", xnbin, xmin, xmax); 
      TH1S* hXresRange7 = new TH1S("xResRange7", "xResRange7", xnbin, xmin, xmax); 
      TH1S* hXresRange8 = new TH1S("xResRange8", "xResRange8", xnbin, xmin, xmax); 
      TH1S* hXresRange9 = new TH1S("xResRange9", "xResRange9", xnbin, xmin, xmax); 
      TH1S* hXresRange10 = new TH1S("xResRange10", "xResRange10", xnbin, xmin, xmax); 
      TH1S* hXresRange11 = new TH1S("xResRange11", "xResRange11", xnbin, xmin, xmax); 
      TH1S* hXresRange12 = new TH1S("xResRange12", "xResRange12", xnbin, xmin, xmax); 
      TH1S* hXresRange13 = new TH1S("xResRange13", "xResRange13", xnbin, xmin, xmax); 
      TH1S* hXresRange14 = new TH1S("xResRange14", "xResRange14", xnbin, xmin, xmax); 
      TH1S* hXresRange15 = new TH1S("xResRange15", "xResRange15", xnbin, xmin, xmax); 
      TH1S* hXresRange16 = new TH1S("xResRange16", "xResRange16", xnbin, xmin, xmax); 
      TH1S* hXresRange17 = new TH1S("xResRange17", "xResRange17", xnbin, xmin, xmax); 
      TH1S* hXresRange18 = new TH1S("xResRange18", "xResRange18", xnbin, xmin, xmax); 
      TH1S* hXresRange19 = new TH1S("xResRange19", "xResRange19", xnbin, xmin, xmax); 
      TH1S* hXresRange20 = new TH1S("xResRange20", "xResRange20", xnbin, xmin, xmax); 
      TH1S* hXresRange21 = new TH1S("xResRange21", "xResRange21", xnbin, xmin, xmax); 
      TH1S* hXresRange22 = new TH1S("xResRange22", "xResRange22", xnbin, xmin, xmax); 
      TH1S* hXresRange23 = new TH1S("xResRange23", "xResRange23", xnbin, xmin, xmax); 
      TH1S* hXresRange24 = new TH1S("xResRange24", "xResRange24", xnbin, xmin, xmax); 
      TH1S* hXresRange25 = new TH1S("xResRange25", "xResRange25", xnbin, xmin, xmax); 
      TH1S* hXresRange26 = new TH1S("xResRange26", "xResRange26", xnbin, xmin, xmax); 
      TH1S* hXresRange27 = new TH1S("xResRange27", "xResRange27", xnbin, xmin, xmax); 
      TH1S* hXresRange28 = new TH1S("xResRange28", "xResRange28", xnbin, xmin, xmax); 
      TH1S* hXresRange29 = new TH1S("xResRange29", "xResRange29", xnbin, xmin, xmax); 
      TH1S* hXresRange30 = new TH1S("xResRange30", "xResRange30", xnbin, xmin, xmax); 
      TH1S* hXresRange31 = new TH1S("xResRange31", "xResRange31", xnbin, xmin, xmax); 
      TH1S* hXresRange32 = new TH1S("xResRange32", "xResRange32", xnbin, xmin, xmax); 
      TH1S* hXresRange33 = new TH1S("xResRange33", "xResRange33", xnbin, xmin, xmax); 
      TH1S* hXresRange34 = new TH1S("xResRange34", "xResRange34", xnbin, xmin, xmax); 
      TH1S* hXresRange35 = new TH1S("xResRange35", "xResRange35", xnbin, xmin, xmax); 
      TH1S* hXresRange36 = new TH1S("xResRange36", "xResRange36", xnbin, xmin, xmax);

      fResidualHistos["xResRange1"] = hXresRange1;
      fResidualHistos["xResRange2"] = hXresRange2;
      fResidualHistos["xResRange3"] = hXresRange3;
      fResidualHistos["xResRange4"] = hXresRange4;
      fResidualHistos["xResRange5"] = hXresRange5;
      fResidualHistos["xResRange6"] = hXresRange6;
      fResidualHistos["xResRange7"] = hXresRange7;
      fResidualHistos["xResRange8"] = hXresRange8;
      fResidualHistos["xResRange9"] = hXresRange9;
      fResidualHistos["xResRange10"] = hXresRange10;
      fResidualHistos["xResRange11"] = hXresRange11;
      fResidualHistos["xResRange12"] = hXresRange12;
      fResidualHistos["xResRange13"] = hXresRange13;
      fResidualHistos["xResRange14"] = hXresRange14;
      fResidualHistos["xResRange15"] = hXresRange15;
      fResidualHistos["xResRange16"] = hXresRange16;
      fResidualHistos["xResRange17"] = hXresRange17;
      fResidualHistos["xResRange18"] = hXresRange18;
      fResidualHistos["xResRange19"] = hXresRange19;
      fResidualHistos["xResRange20"] = hXresRange20;
      fResidualHistos["xResRange21"] = hXresRange21;
      fResidualHistos["xResRange22"] = hXresRange22;
      fResidualHistos["xResRange23"] = hXresRange23;
      fResidualHistos["xResRange24"] = hXresRange24;
      fResidualHistos["xResRange25"] = hXresRange25;
      fResidualHistos["xResRange26"] = hXresRange26;
      fResidualHistos["xResRange27"] = hXresRange27;
      fResidualHistos["xResRange28"] = hXresRange28;
      fResidualHistos["xResRange29"] = hXresRange29;
      fResidualHistos["xResRange30"] = hXresRange30;
      fResidualHistos["xResRange31"] = hXresRange31;
      fResidualHistos["xResRange32"] = hXresRange32;
      fResidualHistos["xResRange33"] = hXresRange33;
      fResidualHistos["xResRange34"] = hXresRange34;
      fResidualHistos["xResRange35"] = hXresRange35;
      fResidualHistos["xResRange36"] = hXresRange36;

      TH1S* hYresRange1 = new TH1S("yResRange1", "yResRange1", ynbin, ymin, ymax); 
      TH1S* hYresRange2 = new TH1S("yResRange2", "yResRange2", ynbin, ymin, ymax); 
      TH1S* hYresRange3 = new TH1S("yResRange3", "yResRange3", ynbin, ymin, ymax); 
      TH1S* hYresRange4 = new TH1S("yResRange4", "yResRange4", ynbin, ymin, ymax); 
      TH1S* hYresRange5 = new TH1S("yResRange5", "yResRange5", ynbin, ymin, ymax); 
      TH1S* hYresRange6 = new TH1S("yResRange6", "yResRange6", ynbin, ymin, ymax); 
      TH1S* hYresRange7 = new TH1S("yResRange7", "yResRange7", ynbin, ymin, ymax); 
      TH1S* hYresRange8 = new TH1S("yResRange8", "yResRange8", ynbin, ymin, ymax); 
      TH1S* hYresRange9 = new TH1S("yResRange9", "yResRange9", ynbin, ymin, ymax); 
      TH1S* hYresRange10 = new TH1S("yResRange10", "yResRange10", ynbin, ymin, ymax); 
      TH1S* hYresRange11 = new TH1S("yResRange11", "yResRange11", ynbin, ymin, ymax); 
      TH1S* hYresRange12 = new TH1S("yResRange12", "yResRange12", ynbin, ymin, ymax); 
      TH1S* hYresRange13 = new TH1S("yResRange13", "yResRange13", ynbin, ymin, ymax); 
      TH1S* hYresRange14 = new TH1S("yResRange14", "yResRange14", ynbin, ymin, ymax); 
      TH1S* hYresRange15 = new TH1S("yResRange15", "yResRange15", ynbin, ymin, ymax); 
      TH1S* hYresRange16 = new TH1S("yResRange16", "yResRange16", ynbin, ymin, ymax); 
      TH1S* hYresRange17 = new TH1S("yResRange17", "yResRange17", ynbin, ymin, ymax); 
      TH1S* hYresRange18 = new TH1S("yResRange18", "yResRange18", ynbin, ymin, ymax); 
      TH1S* hYresRange19 = new TH1S("yResRange19", "yResRange19", ynbin, ymin, ymax); 
      TH1S* hYresRange20 = new TH1S("yResRange20", "yResRange20", ynbin, ymin, ymax); 
      TH1S* hYresRange21 = new TH1S("yResRange21", "yResRange21", ynbin, ymin, ymax); 
      TH1S* hYresRange22 = new TH1S("yResRange22", "yResRange22", ynbin, ymin, ymax); 
      TH1S* hYresRange23 = new TH1S("yResRange23", "yResRange23", ynbin, ymin, ymax); 
      TH1S* hYresRange24 = new TH1S("yResRange24", "yResRange24", ynbin, ymin, ymax); 
      TH1S* hYresRange25 = new TH1S("yResRange25", "yResRange25", ynbin, ymin, ymax); 
      TH1S* hYresRange26 = new TH1S("yResRange26", "yResRange26", ynbin, ymin, ymax); 
      TH1S* hYresRange27 = new TH1S("yResRange27", "yResRange27", ynbin, ymin, ymax); 
      TH1S* hYresRange28 = new TH1S("yResRange28", "yResRange28", ynbin, ymin, ymax); 
      TH1S* hYresRange29 = new TH1S("yResRange29", "yResRange29", ynbin, ymin, ymax); 
      TH1S* hYresRange30 = new TH1S("yResRange30", "yResRange30", ynbin, ymin, ymax); 
      TH1S* hYresRange31 = new TH1S("yResRange31", "yResRange31", ynbin, ymin, ymax); 
      TH1S* hYresRange32 = new TH1S("yResRange32", "yResRange32", ynbin, ymin, ymax); 
      TH1S* hYresRange33 = new TH1S("yResRange33", "yResRange33", ynbin, ymin, ymax); 
      TH1S* hYresRange34 = new TH1S("yResRange34", "yResRange34", ynbin, ymin, ymax); 
      TH1S* hYresRange35 = new TH1S("yResRange35", "yResRange35", ynbin, ymin, ymax); 
      TH1S* hYresRange36 = new TH1S("yResRange36", "yResRange36", ynbin, ymin, ymax);

      fResidualHistos["yResRange1"] = hYresRange1;
      fResidualHistos["yResRange2"] = hYresRange2;
      fResidualHistos["yResRange3"] = hYresRange3;
      fResidualHistos["yResRange4"] = hYresRange4;
      fResidualHistos["yResRange5"] = hYresRange5;
      fResidualHistos["yResRange6"] = hYresRange6;
      fResidualHistos["yResRange7"] = hYresRange7;
      fResidualHistos["yResRange8"] = hYresRange8;
      fResidualHistos["yResRange9"] = hYresRange9;
      fResidualHistos["yResRange10"] = hYresRange10;
      fResidualHistos["yResRange11"] = hYresRange11;
      fResidualHistos["yResRange12"] = hYresRange12;
      fResidualHistos["yResRange13"] = hYresRange13;
      fResidualHistos["yResRange14"] = hYresRange14;
      fResidualHistos["yResRange15"] = hYresRange15;
      fResidualHistos["yResRange16"] = hYresRange16;
      fResidualHistos["yResRange17"] = hYresRange17;
      fResidualHistos["yResRange18"] = hYresRange18;
      fResidualHistos["yResRange19"] = hYresRange19;
      fResidualHistos["yResRange20"] = hYresRange20;
      fResidualHistos["yResRange21"] = hYresRange21;
      fResidualHistos["yResRange22"] = hYresRange22;
      fResidualHistos["yResRange23"] = hYresRange23;
      fResidualHistos["yResRange24"] = hYresRange24;
      fResidualHistos["yResRange25"] = hYresRange25;
      fResidualHistos["yResRange26"] = hYresRange26;
      fResidualHistos["yResRange27"] = hYresRange27;
      fResidualHistos["yResRange28"] = hYresRange28;
      fResidualHistos["yResRange29"] = hYresRange29;
      fResidualHistos["yResRange30"] = hYresRange30;
      fResidualHistos["yResRange31"] = hYresRange31;
      fResidualHistos["yResRange32"] = hYresRange32;
      fResidualHistos["yResRange33"] = hYresRange33;
      fResidualHistos["yResRange34"] = hYresRange34;
      fResidualHistos["yResRange35"] = hYresRange35;
      fResidualHistos["yResRange36"] = hYresRange36;
      */
    //    } 

    if(detName.Contains("EIC")) {
      Int_t   rnbin = track->GetRNBinResiduals(detName);
      Float_t rmin  = track->GetRRangeMinResiduals(detName);
      Float_t rmax  = track->GetRRangeMaxResiduals(detName);
      Int_t   phinbin = track->GetPHINBinResiduals(detName);
      Float_t phimin  = track->GetPHIRangeMinResiduals(detName);
      Float_t phimax  = track->GetPHIRangeMaxResiduals(detName);

      xresName = "residual_" + detName + "X" ;
      yresName = "residual_" + detName + "Y" ;
      xresTitle = detName  + ": Residuals on x-strips";
      yresTitle = detName  + ": Residuals on y-strips";

      TString topresName = "residual_" + planeX;
      TString botresName = "residual_" + planeY;
      TString topresTitle = detName  + ": Residuals on x-axis";
      TString botresTitle = detName  + ": Residuals on y-axis"; 
      TH1S * hTopY  = new TH1S(topresName, topresTitle, xnbin, xmin, xmax);
      TH1S * hBotY  = new TH1S(botresName, botresTitle, ynbin, ymin, ymax);

      hTopY->SetYTitle("Counts");
      hBotY->SetYTitle("Counts");
      hTopY->SetXTitle("xFit - xMeas. (mm)");
      hBotY->SetXTitle("yFit - yMeas. (mm)");
      fResidualHistos[topresName] = hTopY ;
      fResidualHistos[botresName] = hBotY;
      printf("  SRSHistoManager::BookTrackingHistos()=> Top strips residual histo = %s \n", topresName.Data());
      printf("  SRSHistoManager::BookTrackingHistos()=> Bot strips residual histo = %s \n", botresName.Data());

      TString rname    = "residual_" +  detName + "R";
      TString phiname  = "residual_" +  detName + "PHI";
      TString rtitle   = rname + " Residuals";
      TString phititle = phiname + " Residuals";
      TH1S * hR    = new TH1S(rname, rtitle, rnbin, rmin, rmax);
      TH1S * hPhi  = new TH1S(phiname, phititle, phinbin, phimin, phimax);
      hR->SetYTitle("Counts");
      hPhi->SetYTitle("Counts");
      hR->SetXTitle("xFit - xMeas.(mm)");
      hPhi->SetXTitle("yFit - yMeas. (murad)"); 
      fResidualHistos[rname] = hR;
      fResidualHistos[phiname] = hPhi;
      printf("  SRSHistoManager::BookTrackingHistos()=> r_residual histo   = %s \n", rname.Data());
      printf("  SRSHistoManager::BookTrackingHistos()=> phi_residual histo = %s \n", phiname.Data());
    }

    /**   
    if(!fResidualHistos["hXtrackAngle"]) {
      TH1S * hXtrackAngle = new TH1S("hXtrackAngle", "hXtrackAngle", 200, -8, -6);
      fResidualHistos["hXtrackAngle"] = hXtrackAngle;
    }
    if(!fResidualHistos["hYtrackAngle"] ) {
      TH1S * hYtrackAngle = new TH1S("hYtrackAngle", "hYtrackAngle", 100, 2, 3);
      fResidualHistos["hYtrackAngle"] = hYtrackAngle;
    }
    if(!fResidualHistos["hXtrackOffset"]) {
      TH1S * hXtrackOffset = new TH1S("hXtrackOffset", "hXtrackOffset", 200, -50, 50);
      fResidualHistos["hXtrackOffset"] = hXtrackOffset;
    }
    if(!fResidualHistos["hYtrackOffset"]) {
      TH1S * hYtrackOffset = new TH1S("hYtrackOffset", "hYtrackOffset", 80, -40, 0);
      fResidualHistos["hYtrackOffset"] = hYtrackOffset;
    }
    */
  }
  printf("  SRSHistoManager::BookTrackingHistos() ==> EXIT \n");  
}

//====================================================================================================================
void SRSHistoManager::TrackingRun(SRSTrack * track, SRSEventBuilder * eventbuilder) {
  //  printf("====SRSHistoManager::TrackingRun() ==> ENTER \n");

  if(!eventbuilder->IsAGoodEvent()) return;
  if(!track->IsAGoodTrack(eventbuilder)) return;

  map <TString, vector<Float_t> > rawData          = track->GetRawDataSpacePoints();
  //  map <TString, vector<Float_t> > eicClustRawDataY = track->GetEICstripClusterRawDataY();
  map <TString, vector<Float_t> > fittedData       = track->GetFittedSpacePoints();

  /**
  Float_t angle_x = track->GetFitParameters()["xDirection"];
  Float_t angle_y = track->GetFitParameters()["yDirection"];

  if ( ( track->GetFitParameters()["xDirection"] < track->GetAngleCutMinX() ) ||  ( track->GetFitParameters()["xDirection"] > track->GetAngleCutMaxX() ) ) return;
  if ( ( track->GetFitParameters()["yDirection"] < track->GetAngleCutMinY() ) ||  ( track->GetFitParameters()["yDirection"] > track->GetAngleCutMaxY() ) ) return;
    
  ((TH1S*) fResidualHistos["hXtrackAngle"]->Fill(track->GetFitParameters()["xDirection"]));
  ((TH1S*) fResidualHistos["hYtrackAngle"]->Fill(track->GetFitParameters()["yDirection"]));
  ((TH1S*) fResidualHistos["hXtrackOffset"]->Fill(track->GetFitParameters()["xOffset"]));
  ((TH1S*) fResidualHistos["hYtrackOffset"]->Fill(track->GetFitParameters()["yOffset"]));
  */

  vector <Float_t> referencePoint =  rawData["GEM1"];
  map < TString, vector<Float_t> >::const_iterator detector_itr;
  for(detector_itr = rawData.begin(); detector_itr !=  rawData.end(); ++detector_itr) {
    TString detector = (*detector_itr).first;
    vector <Float_t> rawPoint =  rawData[detector];
    vector <Float_t> fittedPoint = fittedData[detector];
    Int_t size    = rawPoint.size();
    Int_t sizeRef = referencePoint.size();
    Int_t sizeFit = fittedPoint.size();

    if ( (size != 3) || (sizeRef != 3) || (sizeFit != 3)) {
      //      printf("  ==SRSHistoManager::TrackingRun() ==> size=%d, sizeRef=%d, sizeFit=%d \n", size, sizeRef, sizeFit);
      //      printf("  ==SRSHistoManager::TrackingRun() ==> xFitPt[%s]=%f,  yfitPt[%s]=%f,  zFitPt[%s]=%f \n", detector.Data(), fittedPoint[0], detector.Data(), fittedPoint[1], detector.Data(), fittedPoint[2]);
      rawPoint.clear();
      fittedPoint.clear();
      continue;
    }
   
    //============================================
    // Cartesian coordinates
    //============================================

    TString planeX = "residual_" + detector + "X";
    TString planeY = "residual_" + detector + "Y";
    Float_t residualX = fittedPoint[0] - rawPoint[0];
    Float_t residualY = fittedPoint[1] - rawPoint[1];

    if ((detector == "GEM1") ||(detector == "GEM2") || (detector == "GEM3")) {
      rawPoint.clear();
      fittedPoint.clear();
      continue;
    }
    if ( fabs(fittedPoint[0]) > 20) {
      rawPoint.clear();
      fittedPoint.clear();
      continue;
    }
    if ( fabs(fittedPoint[1]) > 19) {
      rawPoint.clear();
      fittedPoint.clear();
      continue;
    }

    ((TH1S*) fResidualHistos[planeX])->Fill(residualX);
    ((TH1S*) fResidualHistos[planeY])->Fill(residualY);

    //    if(detector.Contains("PADGEM")) {      
      /**
      TString xRespLin = "respLin2DCOLZ_" + detector + "X";
      TString xDiffNonLin = "diffNonLin2DCOLZ_" + detector + "X"; 
      ((TH2F*) fResidualHistos[xDiffNonLin]->Fill(fittedPoint[0], residualX));
      ((TH2S*) fResidualHistos[xRespLin]->Fill(fittedPoint[0], rawPoint[0]));
      */

      /**
      TString yRespLin = "respLin2DCOLZ_" + detector + "Y";
      TString yDiffNonLin = "diffNonLin2DCOLZ_" + detector + "Y";
      ((TH2F*) fResidualHistos[yDiffNonLin]->Fill(fittedPoint[1], residualY)); 
      ((TH2S*) fResidualHistos[yRespLin]->Fill(fittedPoint[1], rawPoint[1]));
      
      Float_t a = 0.03  ;
      Float_t b = 0.9; 
      if( (fittedPoint[0] > -30) && (fittedPoint[0] < -20) ) b = 0.6;
      if( (fittedPoint[0] > -20) && (fittedPoint[0] < -10) ) b = 0.3;
      if( (fittedPoint[0] > -10) && (fittedPoint[0] < 0) ) b = 0.0;
      if( (fittedPoint[0] > -0) && (fittedPoint[0] < 10) ) b = -0.3;
      if( (fittedPoint[0] > 10) && (fittedPoint[0] < 20) ) b = -0.6;
      if( (fittedPoint[0] > 20) && (fittedPoint[0] < 30) ) b = -0.9;
      Float_t rawPointCorr = rawPoint[0] + (a*fittedPoint[0] + b);
      Float_t residualXCorr = fittedPoint[0] - rawPointCorr;
      TString planeXCorr = "residual_" + detector + "XCorr";
      TString xRespLinCorr = "respLin2DCOLZ_" + detector + "XCorr";
      TString xDiffNonLinCorr = "diffNonLin2DCOLZ_" + detector + "XCorr";
      ((TH1S*) fResidualHistos[planeXCorr])->Fill(residualXCorr);
      ((TH2F*) fResidualHistos[xDiffNonLinCorr]->Fill(fittedPoint[0], residualXCorr));
      ((TH2S*) fResidualHistos[xRespLinCorr]->Fill(fittedPoint[0], rawPointCorr)); 
      */
    //    }
   

    /**
    if (detector.Contains("PADGEM")) && ( fabs(fittedPoint[0]) < 30) ) {
     if( (fittedPoint[0] > -45) && (fittedPoint[0] < -42.5) ) { 
      ((TH1S*) fResidualHistos["xResRange1"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange1"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -42.5) && (fittedPoint[0] < -40) ) { 
      ((TH1S*) fResidualHistos["xResRange2"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange2"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -40) && (fittedPoint[0] < -37.5) ) { 
      ((TH1S*) fResidualHistos["xResRange3"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange3"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -37.5) && (fittedPoint[0] < -35) ) { 
      ((TH1S*) fResidualHistos["xResRange4"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange4"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -35) && (fittedPoint[0] < -32.5) ) { 
      ((TH1S*) fResidualHistos["xResRange5"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange5"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -32.5) && (fittedPoint[0] < -30) ) { 
      ((TH1S*) fResidualHistos["xResRange6"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange6"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -30) && (fittedPoint[0] < -27.5) ) { 
      ((TH1S*) fResidualHistos["xResRange7"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange7"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -27.5) && (fittedPoint[0] < -25) ) { 
      ((TH1S*) fResidualHistos["xResRange8"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange8"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -25) && (fittedPoint[0] < -22.5) ) { 
      ((TH1S*) fResidualHistos["xResRange9"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange9"])->Fill(residualY); 
    }
   
    if( (fittedPoint[0] > -22.5) && (fittedPoint[0] < -20) ) { 
      ((TH1S*) fResidualHistos["xResRange10"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange10"])->Fill(residualY); 
    }

    if( (fittedPoint[0] > -20) && (fittedPoint[0] < -17.5) ) { 
      ((TH1S*) fResidualHistos["xResRange11"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange11"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -17.5) && (fittedPoint[0] < -15) ) { 
      ((TH1S*) fResidualHistos["xResRange12"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange12"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -15) && (fittedPoint[0] < -12.5) ) { 
      ((TH1S*) fResidualHistos["xResRange13"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange13"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -12.5) && (fittedPoint[0] < -10) ) { 
      ((TH1S*) fResidualHistos["xResRange14"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange14"])->Fill(residualY);  
    }
    if( (fittedPoint[0] > -10) && (fittedPoint[0] < -7.5) ) { 
      ((TH1S*) fResidualHistos["xResRange15"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange15"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -7.5) && (fittedPoint[0] < -5) ) { 
      ((TH1S*) fResidualHistos["xResRange16"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange16"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -5) && (fittedPoint[0] < -2.5) ) { 
      ((TH1S*) fResidualHistos["xResRange17"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange17"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -2.5) && (fittedPoint[0] < 0) ) { 
      ((TH1S*) fResidualHistos["xResRange18"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange18"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > -0) && (fittedPoint[0] < 2.5) ) { 
      ((TH1S*) fResidualHistos["xResRange19"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange19"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 2.5) && (fittedPoint[0] < 5) ) { 
      ((TH1S*) fResidualHistos["xResRange20"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange20"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 5) && (fittedPoint[0] < 7.5) ) { 
      ((TH1S*) fResidualHistos["xResRange21"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange21"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 7.5) && (fittedPoint[0] < 10) ) { 
      ((TH1S*) fResidualHistos["xResRange22"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange22"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 10) && (fittedPoint[0] < 12.5) ) { 
      ((TH1S*) fResidualHistos["xResRange23"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange23"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 12.5) && (fittedPoint[0] < 15) ) { 
      ((TH1S*) fResidualHistos["xResRange24"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange24"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 15) && (fittedPoint[0] < 17.5) ) { 
      ((TH1S*) fResidualHistos["xResRange25"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange25"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 17.5) && (fittedPoint[0] < 20) ) { 
      ((TH1S*) fResidualHistos["xResRange26"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange26"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 20) && (fittedPoint[0] < 22.5) ) { 
      ((TH1S*) fResidualHistos["xResRange27"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange27"])->Fill(residualY); 
    }
   
    if( (fittedPoint[0] > 22.5) && (fittedPoint[0] < 25) ) { 
      ((TH1S*) fResidualHistos["xResRange28"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange28"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 25) && (fittedPoint[0] < 27.5) ) { 
      ((TH1S*) fResidualHistos["xResRange29"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange29"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 27.5) && (fittedPoint[0] < 30) ) { 
      ((TH1S*) fResidualHistos["xResRange30"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange30"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 30) && (fittedPoint[0] < 32.5) ) { 
      ((TH1S*) fResidualHistos["xResRange31"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange31"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 32.5) && (fittedPoint[0] < 35) ) { 
      ((TH1S*) fResidualHistos["xResRange32"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange32"])->Fill(residualY);  
    }
    if( (fittedPoint[0] > 35) && (fittedPoint[0] < 37.5) ) { 
      ((TH1S*) fResidualHistos["xResRange33"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange33"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 37.5) && (fittedPoint[0] < 40) ) { 
      ((TH1S*) fResidualHistos["xResRange34"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange34"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 40) && (fittedPoint[0] < 42.5) ) { 
      ((TH1S*) fResidualHistos["xResRange35"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange35"])->Fill(residualY); 
    }
    if( (fittedPoint[0] > 42.5) && (fittedPoint[0] < 45) ) { 
      ((TH1S*) fResidualHistos["xResRange36"])->Fill(residualX);
      ((TH1S*) fResidualHistos["yResRange36"])->Fill(residualY);  
    }
    */
    rawPoint.clear();
    fittedPoint.clear();
  }

  referencePoint.clear();

  map <TString, vector<Float_t> >::const_iterator raw_itr;
  for(raw_itr =  rawData.begin(); raw_itr !=  rawData.end(); ++raw_itr) {
    vector<Float_t> rawdata = (*raw_itr).second;
    rawdata.clear();
  }
  rawData.clear();

  map <TString, vector<Float_t> >::const_iterator fit_itr;
  for(fit_itr = fittedData.begin(); fit_itr != fittedData.end(); ++fit_itr) {
    vector<Float_t> fittedTrack = (*fit_itr).second;
    fittedTrack.clear();
  }
  fittedData.clear();
  /**
  map <TString, vector<Float_t> >::const_iterator clustY_itr;
  for(clustY_itr = eicClustRawDataY.begin(); clustY_itr != eicClustRawDataY.end(); ++clustY_itr) {
    vector<Float_t> eicClustRawDataY = (*clustY_itr).second;
    eicClustRawDataY.clear();
  }
  eicClustRawDataY.clear();
  */
  //  printf("====SRSHistoManager::TrackingRun() ==> EXIT \n");
}

//==================================================================================
void SRSHistoManager::SaveTrackingROOTFile(SRSTrack * track) {
  printf("   SRSHistoManager::SaveTrackingROOTFile()=> Entering ===== \n" );
  //  TString fileName = fOutputDir + "/trackingDir/tracking_" + fOutputFile + ".root";
  TString fileName = fOutputDir + "/trackingDir/tracking_" + fOutputFile + "_SRS" + fAmoreAgentID + ".root";
  TFile * file = new TFile(fileName,"recreate"); 

  if (!file->IsOpen()){
    Error("SRSHistoManager","SaveTrackingROOTFile()=> Cannot open file %s", fileName.Data());
  }

  //=== Create Folder
  if (! file->GetDirectory("Tracking")) file->mkdir("Tracking");
  map<TString, TH1*>::const_iterator itr; 
  for (itr = fResidualHistos.begin(); itr != fResidualHistos.end(); ++itr) {
    TString name = (*itr).first;
    if ((fResidualHistos[name])->GetEffectiveEntries() < 100 ) {
      printf("   SRSHistoManager::SaveTrackingROOTFile()=> Statistic of histo = %s is too low (<100) for the DoubleGaussFit \n", name.Data());
      continue;
    }

    if( (name.Contains("residual_")) || (name.Contains("ResRange"))) DoubleGaussFit( (TH1S*) fResidualHistos[name], 2, 3, kTRUE);
    file->cd();
    gDirectory->cd("Tracking");
    fResidualHistos[name]->Write();
  }
  file->Close();
}

//====================================================================================================================
//        RESIDUALS FOR TRACK BASED EFFICIENCY
//====================================================================================================================
void SRSHistoManager::BookTrackBasedEfficiencyHistos(SRSTrackBasedEfficiency * track) { 
  printf("  SRSHistoManager::BookEfficiencyHistos() ==> ENTER \n");
  GetStyle(1);

  map <Int_t, TString> listOfDetectors = fMapping->GetDetectorFromIDMap();
  map <Int_t, TString>::const_iterator det_itr;
  for (det_itr = listOfDetectors.begin(); det_itr != listOfDetectors.end(); det_itr++) {

    TString detName = (*det_itr).second;
    if (!track->IsTracker(detName)) continue;

    TString planeX = (fMapping->GetDetectorPlaneListFromDetector(detName)).front();
    TString planeY = (fMapping->GetDetectorPlaneListFromDetector(detName)).back();
    printf("  SRSHistoManager::BookEfficiencyHistos()=> det name = %s, x-plane = %s, y-plane = %s, \n", detName.Data(), planeX.Data(), planeX.Data());
    Int_t   nbin = 60; 
    Float_t min  = -15;
    Float_t max  = 15;

    TString xresName = "effRes_" + planeX;
    TString xresTitle = detName  + ": Residuals on x";
    printf("  SRSHistoManager::BookEfficiencyHistos()=> x_residual: histo Name = %s, histo title = %s, \n", xresName.Data(), xresTitle.Data());

    TString yresName = "effRes_" + planeY;
    TString yresTitle = detName  + ": Residuals on y"; 
    printf("  SRSHistoManager::BookEfficiencyHistos()=> y_residual: histo Name = %s, histo title = %s, \n", yresName.Data(), yresTitle.Data());

    TString resName = "effRes_" + detName;
    TString resTitle = detName  + ": Residuals"; 
    printf("  SRSHistoManager::BookEfficiencyHistos()=> residual: histo Name = %s, histo title = %s, \n", resName.Data(), resTitle.Data());

    /**
    TH1F * hX     = new TH1F(xresName, xresTitle, nbin, min, max);
    TH1F * hY     = new TH1F(yresName, yresTitle, nbin, min, max);
    TH1F * hBoth  = new TH1F(resName, resTitle, nbin, min, max);
    */

    TH1S * hX     = new TH1S(xresName, xresTitle, nbin, min, max);
    TH1S * hY     = new TH1S(yresName, yresTitle, nbin, min, max);
    TH1S * hBoth  = new TH1S(resName, resTitle, nbin, min, max);

    hX->SetXTitle("xFit - xMeas. (mm)");
    hX->SetYTitle("Counts");
    hY->SetXTitle("yFit - yMeas. (mm)");
    hY->SetYTitle("Counts");

    fEfficiencyHistos[xresName] = hX;
    fEfficiencyHistos[yresName] = hY;
    fEfficiencyHistos[resName]  = hBoth;
  }
  printf("  SRSHistoManager::BookEfficiencyHistos() ==> EXIT \n");  
}

//====================================================================================================================
void SRSHistoManager::TrackBasedEfficiencyRun(SRSTrackBasedEfficiency * track, SRSEventBuilder * eventbuilder) {
  printf("SRSHistoManager::TrackBasedEfficiencyRun() ==> ENTER IN \n");
  if(eventbuilder->IsAGoodEvent()) {
    map <TString, Float_t > fitParameters = track->GetFitParameters();
    Int_t fitSize =  fitParameters.size();
    Int_t minRange = 5;

    map <Int_t, TString> listOfDetectors = fMapping->GetDetectorFromIDMap();
    map <Int_t, TString>::const_iterator det_itr;
    for (det_itr = listOfDetectors.begin(); det_itr != listOfDetectors.end(); det_itr++) {
      Float_t min_xRes = 1000, min_yRes = 1000;

      TString detName = (* det_itr).second; 
      if (!track->IsTracker(detName)) continue;

      Float_t z = track->GetDetZPos()[detName];
      TString detPlaneX    = (fMapping->GetDetectorPlaneListFromDetector(detName)).front();
      TString detPlaneY    = (fMapping->GetDetectorPlaneListFromDetector(detName)).back();
      TList * clusterListX = eventbuilder->GetClustersInDetectorPlane()[detPlaneX.Data()];
      TList * clusterListY = eventbuilder->GetClustersInDetectorPlane()[detPlaneY.Data()];

      Int_t clustMultInX = clusterListX->GetSize();
      for (Int_t clustNb = 0; clustNb < clustMultInX; clustNb++) {
 	Float_t xRaw = ((SRSCluster *) clusterListX->At(clustNb) )->GetClusterPosition();
	Float_t offsetX = fitParameters["offsetX"];
	Float_t directionX = fitParameters["directionX"];
	Float_t xFit = (offsetX + (directionX * z));
	Float_t xRes = xRaw - xFit; 
	if(fabs(min_xRes) > fabs(xRes)) min_xRes = xRes;
      }

      if(fabs(min_xRes) < minRange) {
	TString planeX = "effRes_" + (fMapping->GetDetectorPlaneListFromDetector(detName)).front();
	//	((TH1F*) fEfficiencyHistos[planeX])->Fill(min_xRes); 	
	((TH1S*) fEfficiencyHistos[planeX])->Fill(min_xRes); 	
      }

      Int_t clustMultInY = clusterListY->GetSize();
      for (Int_t clustNb = 0; clustNb < clustMultInY; clustNb++) {
 	Float_t yRaw = ((SRSCluster *) clusterListY->At(clustNb) )->GetClusterPosition();
	Float_t offsetY = fitParameters["offsetY"];
	Float_t directionY = fitParameters["directionY"];
	Float_t yFit = (offsetY + (directionY * z));
	Float_t yRes = yRaw - yFit; 
	if(fabs(min_yRes) > fabs(yRes)) min_yRes = yRes;
      }

      if(fabs(min_yRes) < minRange) {
	TString planeY = "effRes_" + (fMapping->GetDetectorPlaneListFromDetector(detName)).back();
	//	((TH1F*) fEfficiencyHistos[planeY])->Fill(min_yRes);
	((TH1S*) fEfficiencyHistos[planeY])->Fill(min_yRes);
      }
      if ( (fabs(min_yRes) < minRange) && (fabs(min_xRes) < minRange)) {
	TString det = "effRes_" + detName;
	//	((TH1F*) fEfficiencyHistos[det])->Fill(min_yRes);
	((TH1S*) fEfficiencyHistos[det])->Fill(min_yRes);
      }
    }
  }  
  //  printf("SRSHistoManager::TrackingRun() ==> EXIT \n");
}

//==================================================================================
void SRSHistoManager::SaveTrackBasedEfficiencyROOTFile(SRSTrackBasedEfficiency * track) {
  printf("   SRSHistoManager::SaveTrackingROOTFile()=> Entering ===== \n" );  
  TString fileName = fOutputDir + "/results/" + fOutputFile + "_residuals.root";
  TFile * file = new TFile(fileName,"recreate"); 

  if (!file->IsOpen()){
    Error("SRSHistoManager","SaveEfficiencyROOTFile()=> Cannot open file %s", fileName.Data());
  }

  //=== Create Folder
  if (! file->GetDirectory("TrackResidual")) file->mkdir("TrackResidual");
  map<TString, TH1*>::const_iterator itr; 
  for (itr = fEfficiencyHistos.begin(); itr != fEfficiencyHistos.end(); ++itr) {
    TString name = (*itr).first;
    if (!fEfficiencyHistos[name] ) continue;
    if ((fEfficiencyHistos[name])->GetEffectiveEntries() < 100 ) {
      printf("   SRSHistoManager::SaveEfficiencyROOTFile()=> Statistic of histo = %s is too low (<100) for the DoubleGaussFit \n", name.Data());
      continue;
    }

    DoubleGaussFit( (TH1S*) fEfficiencyHistos[name], 2, 3, kTRUE);
    //    DoubleGaussFit( (TH1F*) fEfficiencyHistos[name], 2, 3, kTRUE);
    file->cd();
    gDirectory->cd("Efficiency");
    fEfficiencyHistos[name]->Write();
  }
  file->Close();
}

//====================================================================================================================
vector <Float_t> SRSHistoManager::CartesianToCylindricalCoordinates(Float_t offsetx, Float_t offsety,  vector <Float_t> cartesianCoordinates ) {
  vector <Float_t> cylindricalCoordinates;
  cylindricalCoordinates.clear();
  Float_t xnew = cartesianCoordinates[0] + offsetx;
  Float_t ynew = cartesianCoordinates[1] + offsety;
  Float_t r = sqrt( (xnew * xnew) + (ynew * ynew) );
  Float_t phy = atan (ynew / xnew);
  cylindricalCoordinates.push_back(r);
  cylindricalCoordinates.push_back(phy);
  cylindricalCoordinates.push_back(cartesianCoordinates[2]);
  return cylindricalCoordinates;
}

//====================================================================================================================
Float_t SRSHistoManager::PlaneRotationAngle(vector <Float_t> referencePoint,  vector <Float_t> rotationPoint) {
  Float_t num = (rotationPoint[0] * referencePoint[1]) - (referencePoint[0] * rotationPoint[1]);
  Float_t denom = (referencePoint[0] * referencePoint[0]) + referencePoint[1] * referencePoint[1];
  return asin (num / denom);
}

//====================================================================================================================
//       3D TRACKING EVENT DSPLAY
//====================================================================================================================
void SRSHistoManager::BookEvent3DDisplayNtuple(SRSTrack * track) {
  fNtuple = new TNtuple("eventDisplayNtuple", track->GetNtupleTitle(), "x:y:z:parameter");
  fNtupleSizeX  = track->GetNtupleSizeX();
  fNtupleSizeY  = track->GetNtupleSizeY(); 
  fNtupleSizeZ  = track->GetNtupleSizeZ(); 
  fNtuple->Fill(-fNtupleSizeX/2, -fNtupleSizeY/2, -1.0, 0.0);
  fNtuple->Fill(-fNtupleSizeX/2, -fNtupleSizeY/2, fNtupleSizeZ, 0.0);
  fNtuple->Fill(-fNtupleSizeX/2, fNtupleSizeY/2, -1.0, 0.0);
  fNtuple->Fill(-fNtupleSizeX/2, fNtupleSizeY/2, fNtupleSizeZ, 0.0);
  fNtuple->Fill(fNtupleSizeX/2, -fNtupleSizeY/2, -1.0, 0.0);
  fNtuple->Fill(fNtupleSizeX/2, -fNtupleSizeY/2, fNtupleSizeZ, 0.0);
  fNtuple->Fill(fNtupleSizeX/2, fNtupleSizeY/2,  0.0, 0.0);
  fNtuple->Fill(fNtupleSizeX/2, fNtupleSizeY/2, fNtupleSizeZ, 0.0);
  printf("  SRSHistoManager::BookNTuple() ==> ntuple = %s booked; fNtupleSizeX=%f, fNtupleSizeY=%f, fNtupleSizeZ=%f\n", track->GetNtupleName().Data(), fNtupleSizeX, fNtupleSizeY, fNtupleSizeZ);
}   

//====================================================================================================================
void SRSHistoManager::Event3DDisplay(SRSTrack * track, SRSEventBuilder * eventBuilder) {
  //   printf("  SRSHistoManager::EventDisplay() ==> ENTER IN \n");
  if(eventBuilder->IsAGoodEvent()) {
    fEventNumber++;

    map <TString, vector<Float_t> > fittedData = track->GetFittedSpacePoints();
    map <TString, vector<Float_t> > rawData = track->GetRawDataSpacePoints();
    map <TString, vector<Float_t> >::const_iterator spacePoint_itr;
    Int_t color = 1;
    Int_t nbPoints = fittedData.size();
    //    for(spacePoint_itr =  rawData.begin(); spacePoint_itr !=  rawData.end(); ++spacePoint_itr) {
    for(spacePoint_itr =  fittedData.begin(); spacePoint_itr !=  fittedData.end(); ++spacePoint_itr) {
      TString detName = (*spacePoint_itr).first;    

      vector <Float_t> spacePoint = (*spacePoint_itr).second;

      if(detName.Contains("BNLTRK1")) color = 0;
      if(detName.Contains("BNLTRK2")) color = 1;
      if(detName.Contains("BNLTRK3")) color = 3;
      if(detName.Contains("BNLTRK4")) color = 4;
      if(detName.Contains("uRWELL")) color = 2;

      if ((fabs(spacePoint[0]) > fNtupleSizeX/2) || (fabs(spacePoint[1]) > fNtupleSizeY/2) || (fabs(spacePoint[2]) > fNtupleSizeZ)) continue;
      if (fEventNumber > 10000) continue;
      fNtuple->Fill(spacePoint[0], spacePoint[1], spacePoint[2], color);
      //      printf("  SRSHistoManager::EventDisplay() ==> spacePoint[%s] = %d \n", detName.Data(), spacePoint.size());
    }

    map <TString, vector<Float_t> >::const_iterator raw_itr;
    for(raw_itr =  rawData.begin(); raw_itr !=  rawData.end(); ++raw_itr) {
      vector<Float_t> rawdata = (*raw_itr).second;
      rawdata.clear();
    }
    rawData.clear();
  }
}

//============================================================================================
static Bool_t CompareHitADCs( TObject *obj1, TObject *obj2) {
  Bool_t compare;
  if ( ( (SRSHit*) obj1 )->GetHitADCs() > ( ( SRSHit*) obj2 )->GetHitADCs()) compare = kTRUE;
  else compare = kFALSE;
  return compare;
}

//============================================================================================
static Bool_t CompareClusterADCs( TObject *obj1, TObject *obj2) {
  Bool_t compare;
  if ( ( (SRSCluster*) obj1 )->GetClusterADCs() > ( ( SRSCluster*) obj2 )->GetClusterADCs()) compare = kTRUE;
  else compare = kFALSE;
  return compare;
}

//====================================================================================================================
void SRSHistoManager::SetHistoTitle(TString histoName,  Int_t goodEvents,  Int_t eventNumber) {
  // printf("SRSHistoManager::SetHistoTitle(): => entering the SetHistoTitle \n");
  stringstream out, out2, out3;

  out2 <<  goodEvents;
  TString  goodEventstr = out2.str();

  out3 << eventNumber;
  TString eventNumberStr = out3.str();

   TString newTitle = fObjTitle[histoName] +  " ("  + goodEventstr + " / " + eventNumberStr + ")";
  ((TH1F*) fObjToDraw[histoName])->SetTitle(newTitle);
  ((TH1F*) fObjToDraw[histoName])->SetTitleSize(0.05,"xy");
}

//====================================================================================================================
void SRSHistoManager::SetHistoTitle(TString histoName) {
  // printf("SRSHistoManager::ZeroSupAnalysis(): => entering the ZeroSupAnalysis \n");
  stringstream out, out2;
  out << fNbOfEvents[histoName];
  TString  nbOfEventsStr = out.str();

  TString newTitle = fObjTitle[histoName];
  ((TH1F*) fObjToDraw[histoName])->SetTitle(newTitle);
  ((TH1F*) fObjToDraw[histoName])->SetTitleSize(0.05,"xy");
}

//=========================================================================================================================
//                                   CALIBRATION RUN: TRACKING OFFSETS & ROTATION                                                        
//========================================================================================================================
void SRSHistoManager::BookCalibrationHistos(SRSTrack * track) {
  printf("  SRSHistoManager::BookCalibrationHistos() ==> ENTER \n");

  map <TString, TString > detectorList =  track->GetDetectorList();

  if(fRunType == "FINETUNING") {
    TString offsetFileName = fOutputDir + "offsetDir/coarseOffsetCalibration.root";
    fOffsetFile = new TFile(offsetFileName,"read");
    if (!fOffsetFile->IsOpen()){
      printf("  SRSHistoManager::BookCalibrationHistos() **** ERROR ERROR *** Cannot open file %s \n", offsetFileName.Data()); 
    }
    printf("  SRSHistoManager::BookCalibrationHistos() Calibration ROOT file opened %s \n", offsetFileName.Data());
  }

  map <TString, TString >::const_iterator ref_itr = detectorList.begin();
  TString refTracker = (*ref_itr).first;
  TString xRefPlane  = fMapping->GetDetectorPlaneListFromDetector(refTracker).front();
  TString yRefPlane  = fMapping->GetDetectorPlaneListFromDetector(refTracker).back();
  printf("SRSHistoManager::CalibrationRun(): => First Tracker = %s xplane = %s, yplane = %s \n", refTracker.Data(), xRefPlane.Data(), yRefPlane.Data());

  map <TString, TString >::const_iterator det_itr;
  for(det_itr = detectorList.begin(); det_itr != detectorList.end(); ++det_itr) {
    TString detector = (*det_itr).first;
    if (detector == refTracker) continue;
    TString planeX = (fMapping->GetDetectorPlaneListFromDetector(detector)).front();
    TString planeY = (fMapping->GetDetectorPlaneListFromDetector(detector)).back();

    Int_t   xnbin =  1281;
    Float_t xmin  = -51.2;
    Float_t xmax  =  51.2;
    Int_t   ynbin =  1281;
    Float_t ymin  = -51.2;
    Float_t ymax  =  51.2;

    if (detector.Contains("EICGEM")) {
      xnbin =  5001;
      xmin  = -500;
      xmax  =  500;
      ynbin =  2701;
      ymin  = -270;
      ymax  =  270;
    }

    TString xOffsetName  = "offset_" + xRefPlane + "_" + planeX;
    TString yOffsetName  = "offset_" + yRefPlane + "_" + planeY;

    if(fRunType == "FINETUNING") {
      xnbin =  200;
      xmin  = -2.0;
      xmax  =  2.0;
      ynbin =  200;
      ymin  = -2.0;
      ymax  =  2.0;

      if (detector.Contains("EICGEM")) {
	xnbin =  1001;
	xmin  = -5;
	xmax  =  5;
	ymin  = -2;
	ymax  =  2;
      }

      xOffsetName  = "fine_offset_" + xRefPlane + "_" + planeX;
      yOffsetName  = "fine_offset_" + yRefPlane + "_" + planeY;

      TString xyRotationName  = "rotation_" + refTracker + "_" + detector;
      TH1F * hXYRotation = new TH1F(xyRotationName, xyRotationName, 250, -0.25, 0.25);
      hXYRotation->SetYTitle("Frequency");
      fCalibrationHistos[xyRotationName] = hXYRotation;
      printf("  SRSHistoManager::BookCalibrationHistos() => Rotation histo = %s \n", xyRotationName.Data());
    }

    TH1F * hXOffset = new TH1F(xOffsetName, xOffsetName, xnbin, xmin, xmax);
    hXOffset->SetYTitle("Frequency");
    fCalibrationHistos[xOffsetName] = hXOffset;
    printf("  SRSHistoManager::BookCalibrationHistos() => X Offset histo = %s \n", xOffsetName.Data());

    TH1F * hYOffset = new TH1F(yOffsetName, yOffsetName, ynbin, ymin, ymax);
    hYOffset->SetYTitle("Frequency");
    fCalibrationHistos[yOffsetName] = hYOffset;
    printf("  SRSHistoManager::BookCalibrationHistos() => Y Offset histo = %s \n", yOffsetName.Data());  
  }
  printf("  SRSHistoManager::BookCalibrationHistos() ==> EXIT \n");
}

//====================================================================================================================
void SRSHistoManager::CalibrationRun(SRSTrack * track, SRSEventBuilder * eventbuilder, const eventHeaderStruct * eventHeader) {
  //  printf("SRSHistoManager::CalibrationRun(): => ENTER \n");

  Int_t eventType    = eventHeader->eventType;
  Int_t eventNumber = eventbuilder->GetEventNumber();

  map < TString, TList * > hitsInDetectorPlaneMap = eventbuilder->GetHitsInDetectorPlane();
  map < TString, TList * > clustersInDetectorPlaneMap = eventbuilder->GetClustersInDetectorPlane();

  Int_t size = clustersInDetectorPlaneMap.size();
  if (size == 0) return;
  if(!(eventbuilder->IsAGoodEvent())) return;

  map<TString, TString> detectorList = track->GetDetectorList();
  map<TString, TString>::const_iterator ref_itr = detectorList.begin();

  TString refTracker = (*ref_itr).first;
  TString xRefPlane  = fMapping->GetDetectorPlaneListFromDetector(refTracker).front();
  TString yRefPlane  = fMapping->GetDetectorPlaneListFromDetector(refTracker).back();

  map < Int_t, vector <Float_t > > refCluster = eventbuilder->GetDetectorCluster(refTracker);
  Int_t refClusterMult = refCluster.size();
  Float_t xRef = refCluster[0][0];
  Float_t yRef = refCluster[0][1];

  map<TString, TString>::const_iterator det_itr;
  for (det_itr = detectorList.begin(); det_itr != detectorList.end(); ++det_itr) {
    TString detector = (*det_itr).first;  
    if (detector == refTracker) continue;
    TString planeX = fMapping->GetDetectorPlaneListFromDetector(detector).front();
    TString planeY = fMapping->GetDetectorPlaneListFromDetector(detector).back();
    map < Int_t, vector <Float_t > > cluster = eventbuilder->GetDetectorCluster(detector);
    Int_t clusterMult = cluster.size();

    if((refClusterMult * clusterMult) == 0) {;
      refCluster.clear();
      cluster.clear();
      continue;
    }

    // Position offset
    TString xOffsetName  = "offset_" + xRefPlane + "_" + planeX;
    TString yOffsetName  = "offset_" + yRefPlane + "_" + planeY;

    Float_t x = cluster[0][0];
    Float_t y = cluster[0][1];

    if(fRunType == "FINETUNING") {

      xOffsetName  = "fine_offset_" + xRefPlane + "_" + planeX;
      yOffsetName  = "fine_offset_" + yRefPlane + "_" + planeY;

      TString coarseOffsetX  = "offset_" + xRefPlane + "_" + planeX;
      TString coarseOffsetY  = "offset_" + yRefPlane + "_" + planeY;

      x = x - ((TH1F *) fOffsetFile->Get(coarseOffsetX))->GetFunction("fitFunction")->GetParameter("Mean");
      y = y - ((TH1F *) fOffsetFile->Get(coarseOffsetY))->GetFunction("fitFunction")->GetParameter("Mean");   

      TString xyRotationName  = "rotation_" + refTracker + "_" + detector;
      Float_t rotation = asin ( ( ( x * yRef ) - ( xRef * y) ) / ( ( x * y ) + ( xRef * yRef) ) );
      ((TH1F*) fCalibrationHistos[xyRotationName])->Fill(rotation);
    } 

    ((TH1F*) fCalibrationHistos[xOffsetName])->Fill( x - xRef);   
    ((TH1F*) fCalibrationHistos[yOffsetName])->Fill( y - yRef);  

    // Clear everything
    for (Int_t k = 0; k < clusterMult; k++)    cluster[k].clear();
    cluster.clear();
  }
  //  printf("SRSHistoManager::CalibrationRun(): => EXIT \n");

 }

//==================================================================================
void SRSHistoManager::SaveCalibrationROOTFile(SRSTrack * track) {
  printf("   SRSHistoManager::SaveCalibrationROOTFile=> Entering ======== \n" );
  TString offsetfileName = fOutputDir + "offsetDir/coarseOffsetCalibration.root";

  if(fRunType == "FINETUNING") {
    offsetfileName = fOutputDir + "offsetDir/fineOffsetCalibration.root";
    printf("   SRSHistoManager::SaveCalibrationROOTFile()=> output data file = %s \n", offsetfileName.Data());
  }

  TFile * offsetfile = new TFile(offsetfileName,"recreate");
  if (!offsetfile->IsOpen()){
    Error("SRSHistoManager","SaveCalibrationROOTFile()=> Cannot open file %s", offsetfileName.Data());
  }

  //=== Create Folder
  map<TString, TH1*>::const_iterator itr; 
  for (itr = fCalibrationHistos.begin(); itr != fCalibrationHistos.end(); ++itr) {
    TString name = (*itr).first;
    if ((fCalibrationHistos[name])->GetEffectiveEntries() > 100 ) {
      printf("   SRSHistoManager::SaveCalibrationROOTFile()=> Enough statistic to fit this histo = %s  \n", name.Data());
      GaussFit( (TH1F*) fCalibrationHistos[name]);
      //      ((TH1F*) fCalibrationHistos[name])->Fit("gaus");
    }

    offsetfile->cd();
    fCalibrationHistos[name]->Write();
  }
  offsetfile->Close();
}

//====================================================================================================================
void SRSHistoManager::ZeroSupAnalysis(SRSTrackBasedEfficiency * track, SRSEventBuilder * eventbuilder, const eventHeaderStruct * eventHeader) {
	// VVV WAS COMMENTED OUT
   //printf("   SRSHistoManager::ZeroSupAnalysis():\n" );
  
  eventbuilder->SetTriggerList(track->GetTriggerList());
  eventbuilder->ComputeClusters();

  if(!eventbuilder->IsAGoodEvent()) { 
      printf("   SRSHistoManager::ZeroSupAnalysis(): => NO GOOD EVENT \n" );
    return ;
  }

  Int_t eventType    = eventHeader->eventType;
  Int_t eventNumber = eventbuilder->GetEventNumber();

  map < TString, TList * > hitsInDetectorPlaneMap     = eventbuilder->GetHitsInDetectorPlane();
  map < TString, TList * > clustersInDetectorPlaneMap = eventbuilder->GetClustersInDetectorPlane();

  if (clustersInDetectorPlaneMap.size() == 0)    return;
    
  // START FILLING HISTOGRAMS
  map<TString,TObject*>::const_iterator objToDraw_itr;
  for (objToDraw_itr = fObjToDraw.begin(); objToDraw_itr != fObjToDraw.end(); ++objToDraw_itr) {

    TString histoName = (*objToDraw_itr).first;    
    if (!fObjToDraw[histoName]) {
      continue;
    }    

    fNbOfEvents[histoName] += 1;
    SetHistoTitle(histoName, fNbOfEvents[histoName], fEventNumber);

    TString detector     = fMapping->GetDetectorFromPlane(fDetectorPlane1[histoName].Data());
    TString detectorType = fMapping->GetDetectorTypeFromDetector(detector);
    TString readoutBoard = fMapping->GetReadoutBoardFromDetector(detector);

    TString planeX = (fMapping->GetDetectorPlaneListFromDetector(fDetector[histoName])).front();
    TString planeY = (fMapping->GetDetectorPlaneListFromDetector(fDetector[histoName])).back();
    //    printf("  SRSHistoManager::ZeroSupAnalysis(): => %s with detector %s for histo %s \n",fVarType[histoName].Data(), fDetectorPlane1[histoName].Data(), histoName.Data() );
   
    // Histograms to be reset at each event
    if ( (fVarType[histoName]=="HITZEROSUP") || (fVarType[histoName]=="CLUSTERHITS") /**|| (fVarType[histoName]=="RAWHITS") || (fVarType[histoName]=="HITRAWDATA") || (fVarType[histoName]=="HITCOMMODE")*/ || (fVarType[histoName]=="CLUSTERTIMEBIN") ) {
        fHist1dToReset.push_back((TH1F*)(fObjToDraw[histoName]));
    }

    if ( (fVarType[histoName]=="PLANETIMEBIN") || (fVarType[histoName]=="PADHITZEROSUP") || /**(fVarType[histoName]=="PADHITPEDOFFSET") || (fVarType[histoName]=="PADRAWHITS") || */(fVarType[histoName]=="PADCLUSTERHITS") ){
      fHist2dToReset.push_back((TH2F*)(fObjToDraw[histoName]));
    }
  
    //=========================================================================================================================//
    //                                                      ZERO SUPPRESSION DATA                                              //
    //=========================================================================================================================//

    //=========================================================================================================================//
    if ( (fVarType[histoName]=="ADCDIST") || (fVarType[histoName]=="PLANETIMEBIN") || (fVarType[histoName]=="CLUSTERDIST") || (fVarType[histoName]=="CLUSTSIZE") || (fVarType[histoName]=="CLUSTMULT") || (fVarType[histoName]=="HITCROSSTALK") || (fVarType[histoName]=="CHARGECROSSTALK") || (fVarType[histoName]=="OCCUPANCY") || (fVarType[histoName]=="NOISERESO")   || (fVarType[histoName]=="TIMESLICE") || (fVarType[histoName]=="CLUSTERTIMEBIN") || (fVarType[histoName]=="PADHITZEROSUP") || (fVarType[histoName]=="HITZEROSUP") || /**(fVarType[histoName]=="PADRAWHITS") ||*/ (fVarType[histoName]=="CLUSTERHITS") ) { 

      if(!eventbuilder->IsAGoodClusterEvent(fDetectorPlane1[histoName].Data())) continue;

      TList * listOfClusters = clustersInDetectorPlaneMap[fDetectorPlane1[histoName].Data()];      
      Int_t clustMult = listOfClusters->GetSize();
      
      if(clustMult == 0) {
	listOfClusters->Clear();
	continue;
      }
      
      //      printf("   SRSHistoManager::ZeroSupAnalysis(): => %s with detector %s for histo %s \n",fVarType[histoName].Data(), fDetectorPlane1[histoName].Data(), histoName.Data() );

      if (fVarType[histoName]=="ADCDIST") {
	if(eventbuilder->GetClustMaxOrTotalADCs() == "totalADCs") ((TH1F*) fObjToDraw[histoName])->SetXTitle("Cluster total charges (ADC counts)");
	else                                                        ((TH1F*) fObjToDraw[histoName])->SetXTitle("Cluster peak height (ADC counts)");
	((TH1F*) fObjToDraw[histoName])->SetYTitle("Counts");
      }

      if (fVarType[histoName]=="OCCUPANCY") {
	((TH1F*) fObjToDraw[histoName])->SetXTitle("Strip Occupancy");
	((TH1F*) fObjToDraw[histoName])->SetYTitle("Counts");
      }

      if (fVarType[histoName]=="CLUSTSIZE") {
	((TH1F*) fObjToDraw[histoName])->SetXTitle("Cluster size");
	((TH1F*) fObjToDraw[histoName])->SetYTitle("Counts");
      }

      if( (fVarType[histoName]=="HITZEROSUP") || (fVarType[histoName]=="CLUSTERHITS") || ((fVarType[histoName]=="CLUSTERDIST") ) ) {        
	if((fVarType[histoName]=="HITZEROSUP") || (fVarType[histoName]=="CLUSTERHITS") || ((fVarType[histoName]=="CLUSTERDIST")  && ((histoName.Contains("clusterADCDist")) || (histoName.Contains("clusterADCDist")) || (histoName.Contains("clusterAdcDist")) || (histoName.Contains("clusterAdcDist")) ))) {
	  ((TH1F*) fObjToDraw[histoName])->SetYTitle("ADC Counts");
	}
	if ( (fVarType[histoName]=="CLUSTERDIST")  && (histoName.Contains("distanceToStrip")) ) {
	  ((TH1F*) fObjToDraw[histoName])->SetXTitle("diff cluster Position cluster central strip");
	  ((TH1F*) fObjToDraw[histoName])->SetYTitle("Counts");
	}
      }

      if (fVarType[histoName]=="PLANETIMEBIN") {
	((TH2F*) fObjToDraw[histoName])->SetYTitle("time bin number (25 ns)");
	((TH2F*) fObjToDraw[histoName])->SetZTitle("ADC Counts");
	Int_t nbBins = NCH * fMapping->GetNbOfAPVs(fDetectorPlane1[histoName]);
	Int_t nmax   = NCH * fMapping->GetNbOfAPVs(fDetectorPlane1[histoName]);
	((TH2F*) fObjToDraw[histoName])->SetXTitle("hit position (strip number)");
	((TH2F*) fObjToDraw[histoName])->SetBins(nbBins,0,nmax,fNBinTime,fRangeMinTime,fRangeMaxTime);
      }

      Int_t stripOccupancy = 0;
      TIter nextCluster(listOfClusters);
      
      while( SRSCluster * cluster = ( (SRSCluster *) nextCluster() ) ) {
	Int_t clusterSize     = cluster->GetNbOfHitsInCluster();
	Int_t clusterSizePadX = cluster->GetPadXClusterSize();
	Int_t clusterSizePadY = cluster->GetPadYClusterSize();
	if (cluster->IsCrossTalkCluster()) {
	  clustMult--;
	  continue;
	}

	// Purely Cluster info
	Float_t clusterADCcounts     = cluster->GetClusterADCs();
	Float_t clusterCentralStrip  = cluster->GetClusterCentralStrip();
       	Float_t clusterPosition      = cluster->GetClusterPosition();
        stripOccupancy += clusterSize;

	Float_t padPosX = cluster->GetPadClustPosX();
	Float_t padPosY = cluster->GetPadClustPosY();

	// hit related Cluster info
	for(Int_t i = 0; i < clusterSize; i++ ) {
          SRSHit * hit = cluster->GetHitFromCluster(i);
          Float_t stripNb = cluster->GetStripPosition(hit->GetStripNo());
          Float_t hitADCcounts= hit->GetHitADCs();
	  Float_t normalizedCharge = hitADCcounts / clusterADCcounts;

	  if (readoutBoard == "PADPLANE") {
	    Int_t padNo  = hit->GetPadNo();
	    Int_t padXNo = hit->GetPadXNo();
	    Int_t padYNo = hit->GetPadYNo();
	    padPosX = cluster->GetPadPosX(padXNo);
	    padPosY = cluster->GetPadPosY(padYNo);
 	  }
	  //printf("   SRSHistoManager::ZeroSupAnalysis():Plane =%s, position = %f, adc = %f\n",  fDetectorPlane1[histoName].Data(), clusterPosition,  clusterADCcounts);

          if(((fVarType[histoName]=="PADHITZEROSUP") || (fVarType[histoName]=="PADCLUSTERHITS")) && (readoutBoard == "PADPLANE")) ((TH2F*) fObjToDraw[histoName])->Fill(padPosX, padPosY, hitADCcounts);
          if ( (fVarType[histoName]=="HITZEROSUP") || (fVarType[histoName]=="CLUSTERHITS") ) {
	    if (readoutBoard == "PADPLANE") {
	      if ( histoName.Contains("PadX"))  ((TH1F*) fObjToDraw[histoName])->Fill(padPosX, hitADCcounts); 
	      if ( histoName.Contains("PadY")) 	((TH1F*) fObjToDraw[histoName])->Fill(padPosY, hitADCcounts);
	    }
	    else {
	      ((TH1F*) fObjToDraw[histoName])->Fill(stripNb,hitADCcounts);
	      //   printf("   SRSHistoManager::ZeroSupAnalysis():fill histo %s for %s with hits position = %f and ADCs = %f \n",  histoName.Data(), fDetectorPlane1[histoName].Data(), stripNb, hitADCcounts );
	    }
	  }
       
 	  if ( fVarType[histoName]=="PLANETIMEBIN" ) {
	    SetHistoTitle(histoName, fNbOfEvents[histoName], eventNumber);
            vector<Float_t > timebinADCs = hit->GetTimeBinADCs();
            Int_t timebin = 0;
            vector<Float_t >::const_iterator timebinCharge_itr;
            for (timebinCharge_itr = timebinADCs.begin(); timebinCharge_itr != timebinADCs.end(); ++timebinCharge_itr) {
              Float_t charges = * timebinCharge_itr;
	      ((TH2F*) fObjToDraw[histoName])->Fill(stripNb,timebin,charges);
              timebin++;
            }
            timebinADCs.clear();
          }
	}

	// Purely Cluster info
	padPosX = cluster->GetPadClustPosX();
	padPosY = cluster->GetPadClustPosY();
        
	if ( (fVarType[histoName] ==  "TIMESLICE" ) && (histoName.Contains("cluster"))) {
	  Int_t timeSlice = cluster->GetClusterPeakTimeBin();
	  if (histoName.Contains("clusterPeakTimeBin")) timeSlice = cluster->GetClusterPeakTimeBin();
	  if (histoName.Contains("clusterTimeBin"))     timeSlice = cluster->GetClusterTimeBin();

	  ((TH1F*) fObjToDraw[histoName])->SetXTitle("Time slice (25 ns)");
	  ((TH1F*) fObjToDraw[histoName])->SetYTitle("counts");
	  ((TH1F*) fObjToDraw[histoName])->Fill(timeSlice);
	}
       
	if (fVarType[histoName]=="ADCDIST") {
	  if (clusterADCcounts != 0) ((TH1F*) fObjToDraw[histoName])->Fill(cluster->GetClusterADCs());
	}

	if (fVarType[histoName]=="CLUSTSIZE") {
	  if (clusterADCcounts == 0) continue;

	  if (readoutBoard == "PADPLANE") {
	    if ((histoName.Contains("PADX")) || (histoName.Contains("PADY"))) {
	      if (histoName.Contains("PADX")) ((TH1F*) fObjToDraw[histoName])->Fill(clusterSizePadX);
	      if (histoName.Contains("PADY")) ((TH1F*) fObjToDraw[histoName])->Fill(clusterSizePadY);
	    }    
	    else {
	      ((TH1F*)  fObjToDraw[histoName])->Fill(clusterSize);
	    }
	  }
	  else  ((TH1F*) fObjToDraw[histoName])->Fill(clusterSize);	  	  
	}

	if (fVarType[histoName]=="CLUSTERDIST" ) {
	  if ( (histoName.Contains("clusterDist")) || (histoName.Contains("ClusterDist")) || (histoName.Contains("ClustDist")) || (histoName.Contains("clustDist")) ) {
	    if (readoutBoard == "PADPLANE") {
	      if ( histoName.Contains("PadX")) ((TH1F*) fObjToDraw[histoName])->Fill(padPosX);
	      if ( histoName.Contains("PadY")) ((TH1F*) fObjToDraw[histoName])->Fill(padPosY);
	    }
	    else {
	      ((TH1F*) fObjToDraw[histoName])->Fill(clusterPosition);
	    }
	  }	
	  if ( histoName.Contains("distanceToStrip") ) ((TH1F*) fObjToDraw[histoName])->Fill(clusterPosition - clusterCentralStrip);	  
	  if ( (histoName.Contains("clusterADCDist")) || (histoName.Contains("clusterAdcDist"))) {
	    if (readoutBoard == "PADPLANE") {
	      if ( histoName.Contains("PadX")) ((TH1F*) fObjToDraw[histoName])->Fill(padPosX, clusterADCcounts);
	      if ( histoName.Contains("PadY")) ((TH1F*) fObjToDraw[histoName])->Fill(padPosY, clusterADCcounts);
	    }
	    else {
	      ((TH1F*) fObjToDraw[histoName])->Fill(clusterPosition, clusterADCcounts);
	    }
	  }
	}

	if (fVarType[histoName]=="CLUSTERTIMEBIN") {
	  SetHistoTitle(histoName, fNbOfEvents[histoName], eventNumber);
	  vector<Float_t > timebinCharges = cluster->GetClusterTimeBinADCs();
	  Int_t timebin = 0;
	  vector<Float_t >::const_iterator timebinCharge_itr;

	  ((TH1F*) fObjToDraw[histoName])->SetXTitle("Time sample (25ns) of the cluster");
	  ((TH1F*) fObjToDraw[histoName])->SetYTitle("sum ADCs of the cluster");
	  for (timebinCharge_itr = timebinCharges.begin(); timebinCharge_itr != timebinCharges.end(); ++timebinCharge_itr) {
	    Float_t charges = * timebinCharge_itr;
	    if(charges < 200) charges = 0;
	    ((TH1F*) fObjToDraw[histoName])->Fill(timebin,charges);
	    timebin++;
	  }
	  timebinCharges.clear();
	}
      }

      if (fVarType[histoName]=="OCCUPANCY") ((TH1F*) fObjToDraw[histoName])->Fill(stripOccupancy);
      //============   CLUSTER IBFORMATION

      if (fVarType[histoName]=="CLUSTMULT") {
	((TH1F*) fObjToDraw[histoName])->SetXTitle("Cluster Multiplicity");
        ((TH1F*) fObjToDraw[histoName])->SetYTitle("Counts");
	((TH1F*) fObjToDraw[histoName])->Fill(clustMult);
      }
    }

    //=========   HIT INFORMATION
    //    if ( (fVarType[histoName]=="RAWHITS") || (fVarType[histoName]=="HITPEDOFFSET") || (fVarType[histoName]=="HITDIST") || (fVarType[histoName]=="PADHITPEDOFFSET") || (fVarType[histoName]=="PADRAWHITS") || (fVarType[histoName]=="PADHITDIST") || (fVarType[histoName]=="TIMESLICE")  ) { 
    if ( (fVarType[histoName]=="HITDIST") || (fVarType[histoName]=="PADHITDIST") || (fVarType[histoName]=="TIMESLICE")  ) { 

      TList  * listOfHits = hitsInDetectorPlaneMap[fDetectorPlane1[histoName].Data()];
      if(listOfHits->GetSize() == 0) {
	listOfHits->Clear();
	continue;
      }

      Int_t padNo   = 0;
      Int_t padXNo  = 0;
      Int_t padYNo  = 0;
      Int_t nbPadX  = 0;
      Int_t nbPadY  = 0;
      Int_t padPosX = 0; 
      Int_t padPosY = 0;

      //      if ( (fVarType[histoName]=="RAWHITS") || (fVarType[histoName]=="HITPEDOFFSET") || (fVarType[histoName]=="HITDIST") ) { 
      if  (fVarType[histoName]=="HITDIST") { 
	if(readoutBoard =="PADPLANE") { 
	  nbPadX = (Int_t) (fMapping->GetPadDetectorMap(detector) [2]);
	  nbPadY = (Int_t) (fMapping->GetPadDetectorMap(detector) [3]);
	  if ( histoName.Contains("PadX")) ((TH1F*) fObjToDraw[histoName])->SetBins(nbPadX, -0.5, (nbPadX - 0.5)); 
	  if ( histoName.Contains("PadY")) ((TH1F*) fObjToDraw[histoName])->SetBins(nbPadY, -0.5, (nbPadY - 0.5)); 
	} 
	else {
	  Int_t nbBins = NCH * fMapping->GetNbOfAPVs(fDetectorPlane1[histoName]);
	  Int_t nmax  =  NCH * fMapping->GetNbOfAPVs(fDetectorPlane1[histoName]) - 1; 
	  ((TH1F*) fObjToDraw[histoName])->SetBins(nbBins,0, nmax);
	  ((TH1F*) fObjToDraw[histoName])->SetXTitle("Strip number");
	  //	  printf("   SRSHistoManager::ZeroSupAnalysis(): 3 => %s with detector %s for histo %s \n",fVarType[histoName].Data(), fDetector[histoName].Data(), histoName.Data() );
	}
      }

      TIter nextHit(listOfHits);
      while( SRSHit * hit = ( (SRSHit *) nextHit() ) ) {
	Int_t stripNb      = hit->GetStripNo();
	Float_t hitADCcounts = hit->GetHitADCs();

	if(readoutBoard == "PADPLANE") {
	  padNo  = hit->GetPadNo();
	  padXNo = hit->GetPadXNo();
	  padYNo = hit->GetPadYNo();
  
	  //	  if ((fVarType[histoName]=="PADHITPEDOFFSET" ) || (fVarType[histoName]=="PADRAWHITS") ) ((TH2F*) fObjToDraw[histoName])->Fill(padXNo, padYNo, hitADCcounts);
	  if ((fVarType[histoName]=="PADHITDIST" ) && ( histoName.Contains("HitDistPadRO")))     ((TH2F*) fObjToDraw[histoName])->Fill(padXNo, padYNo);
	  if ((fVarType[histoName]=="PADHITDIST" ) && ( histoName.Contains("ADCDistPadRO")))     ((TH2F*) fObjToDraw[histoName])->Fill(padXNo, padYNo, hitADCcounts);
	}

	if ( (fVarType[histoName] ==  "TIMESLICE" ) && (histoName.Contains("hitTiming"))) {
	  Int_t timeSlice  = hit->GetSignalPeakBinNumber();
	  ((TH1F*) fObjToDraw[histoName])->SetXTitle("Time bin signal's peak");
	  ((TH1F*) fObjToDraw[histoName])->SetYTitle("Counts");
	  ((TH1F*) fObjToDraw[histoName])->Fill(timeSlice);
	}

	/**
	if ( (fVarType[histoName] ==  "HITPEDOFFSET" ) || (fVarType[histoName]=="RAWHITS") ){
	  ((TH1F*) fObjToDraw[histoName])->SetYTitle("ADC Counts");
ss
	  if (readoutBoard == "PADPLANE") {
	    if ( histoName.Contains("PadX")) ((TH1F*) fObjToDraw[histoName])->Fill(padXNo, hitADCcounts);
	    if ( histoName.Contains("PadY")) ((TH1F*) fObjToDraw[histoName])->Fill(padYNo, hitADCcounts);
	  }
	  else {
	    ((TH1F*) fObjToDraw[histoName])->Fill(stripNb,hitADCcounts);
	  }
	}
	*/

	if (fVarType[histoName]=="HITDIST") {
	  if ( (histoName.Contains("hitDist"))  || (histoName.Contains("hitdist")) || (histoName.Contains("HitDist")) ) {
	    ((TH1F*) fObjToDraw[histoName])->SetYTitle("Counts");

	    if (readoutBoard == "PADPLANE") {
	      if ( histoName.Contains("PadX")) ((TH1F*) fObjToDraw[histoName])->Fill(padXNo);
	      if ( histoName.Contains("PadY")) ((TH1F*) fObjToDraw[histoName])->Fill(padYNo);
	    }
	    else {
	      ((TH1F*) fObjToDraw[histoName])->Fill(stripNb);
	    }
	  }

	  if ( (histoName.Contains("HitADCDist")) || (histoName.Contains("hitADCDist")) || (histoName.Contains("HitAdcDist")) || (histoName.Contains("hitAdcDist")) ) {
	    ((TH1F*) fObjToDraw[histoName])->SetYTitle("ADC Counts");

	    if (readoutBoard == "PADPLANE") {
	      if ( histoName.Contains("PadX")) ((TH1F*) fObjToDraw[histoName])->Fill(padXNo,hitADCcounts);
	      if ( histoName.Contains("PadY")) ((TH1F*) fObjToDraw[histoName])->Fill(padYNo,hitADCcounts);
	    }
	    else {
	      ((TH1F*) fObjToDraw[histoName])->Fill(stripNb,hitADCcounts);
	    }
	  }
	}	
      }
    }

    //=========================================================================================================================//
    //                                                  HITMAP, CHARGE SHARING                                                 //
    //=========================================================================================================================//
    if ( (fVarType[histoName]=="CHARGES_SH") ||  (fVarType[histoName]=="CHARGES_RATIO") || (fVarType[histoName]=="CHARGES_RATIODIST") || (fVarType[histoName]=="CLUSTCOR") || (fVarType[histoName]=="HITMAP") || (fVarType[histoName]=="CMSHITMAP") ) {
      //      printf("   SRSHistoManager::ZeroSupAnalysis(): => %s with detector %s for histo %s \n",fVarType[histoName].Data(), fDetector[histoName].Data(), histoName.Data() );

      map < Int_t, vector <Float_t > > detectorEvent = eventbuilder->GetDetectorCluster(fDetector[histoName]);
      Int_t clusterMult = detectorEvent.size();

      if (clusterMult == 0) {
	detectorEvent.clear();
	continue;
      }   

      //*******************************************************************************
      // Make sense to use only ONE CLUSTER in a multi-cluster event for this analysis
      // The selection is on the cluster with maximum charge
      //*******************************************************************************
      Float_t pos1 = detectorEvent[0][0];
      Float_t pos2 = detectorEvent[0][1];

      Float_t adcCount1 = detectorEvent[0][2];
      Float_t adcCount2 = detectorEvent[0][3];
      Float_t totalCharge = (adcCount1 + adcCount2)/2;

      Float_t timing1 = detectorEvent[0][4];
      Float_t timing2 = detectorEvent[0][5];

      if(fPreviousAdcCount == 0) fPreviousAdcCount = detectorEvent[0][3];
      
      ((TH2F*) fObjToDraw[histoName])->SetXTitle(" x(mm)"); 
      ((TH2F*) fObjToDraw[histoName])->SetYTitle(" y (mm)");

      if(planeX != planeY) {
	((TH2F*) fObjToDraw[histoName])->SetXTitle(" x (mm)");
	((TH2F*) fObjToDraw[histoName])->SetYTitle(" y (mm)");
      }

      if ( fVarType[histoName]=="CHARGES_SH") {
	((TH2F*) fObjToDraw[histoName])->SetXTitle("Cluster charge (ADC counts) in X");
	((TH2F*) fObjToDraw[histoName])->SetYTitle("Cluster charge (ADC counts) in Y");

	if(planeX != planeY) {
	  if(eventbuilder->GetClustMaxOrTotalADCs() == "totalADCs") {
	    ((TH2F*) fObjToDraw[histoName])->SetXTitle("Cluster ADCs (X-plane)");
	    ((TH2F*) fObjToDraw[histoName])->SetYTitle("Cluster ADCs (Y-plane)");
	  }
	  else {
	    ((TH2F*) fObjToDraw[histoName])->SetXTitle("Cluster peak ADC (X-plane)");
	    ((TH2F*) fObjToDraw[histoName])->SetYTitle("Cluster peak ADC (Y-plane)");
	  }
	}
	((TH2F*) fObjToDraw[histoName])->SetZTitle("Events");

	if ( (fPreviousAdcCount == 99999) && (histoName.Contains("UnCorr")))  {
	  fPreviousAdcCount = adcCount2;
	  continue;
	}
	
	if(histoName.Contains("UnCorr")) ((TH2F*) fObjToDraw[histoName])->Fill(adcCount1, fPreviousAdcCount);
	else ((TH1F*) fObjToDraw[histoName])->Fill(adcCount1, adcCount2 );
	fPreviousAdcCount = adcCount2;
      }
     
      if (fVarType[histoName]=="CHARGES_RATIO") {	
	if ( (fPreviousAdcCount == 99999) && (histoName.Contains("UnCorr")))  {
	  fPreviousAdcCount = adcCount2;
	  continue;
	}
	
	if(eventbuilder->GetClustMaxOrTotalADCs() == "totalADCs") {
	  ((TH1F*) fObjToDraw[histoName])->SetXTitle("Cluster total charge ratio X/Y ");
	}
	else {
	  ((TH1F*) fObjToDraw[histoName])->SetXTitle("Cluster peak height ratio X/Y ");
	}
	((TH1F*) fObjToDraw[histoName])->SetYTitle("Counts");

	if (histoName.Contains("UnCorr")) ((TH1F*) fObjToDraw[histoName])->Fill(adcCount1 /  fPreviousAdcCount);
	else ((TH1F*) fObjToDraw[histoName])->Fill(adcCount1 / adcCount2 );
	 fPreviousAdcCount = adcCount2;
      }


      if (fVarType[histoName]=="CHARGES_RATIODIST") {
	((TH2F*) fObjToDraw[histoName])->SetZTitle("total charge ratio X/Y"); 
	Float_t ratio = adcCount1 / adcCount2;
	((TH2F*) fObjToDraw[histoName])->Fill(pos1, pos2, ratio);
      }

      if (fVarType[histoName]=="HITMAP") {

	if ( (histoName.Contains("Hit2D")) || (histoName.Contains("hit2D")) || (histoName.Contains("HIT2D")) ) {
	  ((TH2F*) fObjToDraw[histoName])-> SetZTitle("Counts");
	  ((TH2F*) fObjToDraw[histoName])->Fill(pos1, pos2);
	}

	if ( (histoName.Contains("adc2D")) || (histoName.Contains("ADC2D")) || (histoName.Contains("Adc2D")) ) {
	  ((TH2F*) fObjToDraw[histoName])->SetZTitle("ADC channels (Cluster)");
	  if(histoName.Contains("XPLANE") ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(pos1, pos2, adcCount1);
	  }
	  else if(histoName.Contains("YPLANE") ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(pos1, pos2, adcCount2);
	  }

	  else{
	    Float_t totalCharge = adcCount1 + adcCount2;
	    ((TH2F*) fObjToDraw[histoName])->Fill(pos1, pos2, totalCharge);
	  }
	}

	if ( (histoName.Contains("timing2D")) || (histoName.Contains("Timing2D")) || (histoName.Contains("timeBin2D")) ) {
	  ((TH2F*) fObjToDraw[histoName])->SetZTitle("Time bin (25 ns)");

	  if(histoName.Contains("XPLANE") ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(pos1, pos2, timing1);
	  }

	  else if(histoName.Contains("YPLANE") ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(pos1, pos2, timing2);
	  }
	  else {
	    Float_t timebin = (timing1 + timing2) / 2;
	    ((TH2F*) fObjToDraw[histoName])->Fill(pos1, pos2, timebin);

	  }
	}
	if ( (histoName.Contains("chargeRatio2D")) || (histoName.Contains("ChargeRatio2D")) ) {
	  if(eventbuilder->GetClustMaxOrTotalADCs() == "totalADCs") {
	    ((TH2F*) fObjToDraw[histoName])->SetZTitle("Cluster total ADCs ratio X/Y ");
	  }
	  else {
	    ((TH2F*) fObjToDraw[histoName])->SetZTitle("Cluster peak height ratio X/Y ");
	  }
	  Float_t ratio = adcCount1 / adcCount2;
	  ((TH2F*) fObjToDraw[histoName])->Fill(pos1, pos2, ratio);
	}
      }
    
      for (Int_t k = 0; k < clusterMult; k++) {
	detectorEvent[k].clear();
      }
       detectorEvent.clear();     
}
    //=========================================================================================================================//
//                                POSITION CORRELATION BETWEEN 2 DETECTORS                                                 //
    //=========================================================================================================================//
    if (fVarType[histoName]=="CORRELATION")  {
      //      printf("   SRSHistoManager::ZeroSupAnalysis(): => %s with plane1 %s and plane2 %s for histo %s \n",fVarType[histoName].Data(), fDetectorPlane1[histoName].Data(), fDetectorPlane1[histoName].Data(), histoName.Data() );
    
      TString detector1 = fMapping->GetDetectorFromPlane(fDetectorPlane1[histoName]);
      TString detector2 = fMapping->GetDetectorFromPlane(fDetectorPlane2[histoName]);

      map < Int_t, vector <Float_t > > detectorEvent1 = eventbuilder->GetDetectorCluster(detector1);
      map < Int_t, vector <Float_t > > detectorEvent2 = eventbuilder->GetDetectorCluster(detector2);
      Int_t clusterMult1 = detectorEvent1.size();
      Int_t clusterMult2 = detectorEvent2.size();
      if(clusterMult1 != clusterMult2) {
	detectorEvent1.clear();
	detectorEvent2.clear();
	continue;
      }

      for (Int_t k = 0; k < clusterMult1; k++) {
	if( (histoName.Contains("timing")) || (histoName.Contains("Timing")) ) {
	  ((TH1F*) fObjToDraw[histoName])->SetXTitle("x-strips: Time bin signal's peak");
	  ((TH1F*) fObjToDraw[histoName])->SetYTitle("y-strips: Time bin signal's peak");
	  ((TH2F*) fObjToDraw[histoName])-> SetZTitle("Counts");

	  Float_t x1t = detectorEvent1[k][4];
	  Float_t y1t = detectorEvent1[k][5];
	  Float_t x2t = detectorEvent2[k][4];
	  Float_t y2t = detectorEvent2[k][5];

	  if( (fDetectorPlane1[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector1)).front() ) && (fDetectorPlane2[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector2)).front()) ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(x1t, x2t);
	  }
	  if( (fDetectorPlane1[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector1)).front() ) && (fDetectorPlane2[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector2)).back()) ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(x1t, y2t);
	  }
	  if( (fDetectorPlane1[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector1)).back() ) && (fDetectorPlane2[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector2)).front()) ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(y1t, x2t);
	  }
	  if( (fDetectorPlane1[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector1)).back() ) && (fDetectorPlane2[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector2)).back()) ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(y1t, y2t);
	  }
	  detectorEvent1[k].clear();
	  detectorEvent2[k].clear();
	}

	else {
	  Float_t x1 = detectorEvent1[k][0];
	  Float_t y1 = detectorEvent1[k][1];
	  Float_t x2 = detectorEvent2[k][0];
	  Float_t y2 = detectorEvent2[k][1];

	  TString readoutBoard = fMapping->GetReadoutBoardFromDetector(fDetector[histoName].Data());
	  TString detectorType = fMapping->GetDetectorTypeFromDetector(fDetector[histoName].Data());
	  if ( (readoutBoard == "UV_ANGLE") &&  (detectorType == "EICPROTO1") ) ((TH2F*) fObjToDraw[histoName])->SetBins(250, -500, 500, 150, -300, 300);	  
	  ((TH2F*) fObjToDraw[histoName])-> SetZTitle("Counts");

	  if(histoName.Contains("xCorr")) ((TH2F*) fObjToDraw[histoName])->Fill(x1, (x1-x2));
	  if(histoName.Contains("yCorr")) ((TH2F*) fObjToDraw[histoName])->Fill(y1, (y1-y2));

	  /**
	  if( (histoName.Contains("xCorr")) ((TH2F*) fObjToDraw[histoName])->Fill(x1, x2);
	  if( (fDetectorPlane1[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector1)).front() ) && (fDetectorPlane2[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector2)).front()) ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(x1, x2);
	  }
	  if( (fDetectorPlane1[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector1)).front() ) && (fDetectorPlane2[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector2)).back()) ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(x1, y2);
	  }
	  if( (fDetectorPlane1[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector1)).back() ) && (fDetectorPlane2[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector2)).front()) ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(y1, x2);
	  }
	  if( (fDetectorPlane1[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector1)).back() ) && (fDetectorPlane2[histoName] == (fMapping->GetDetectorPlaneListFromDetector(detector2)).back()) ) {
	    ((TH2F*) fObjToDraw[histoName])->Fill(y1, y2);
	  }
	  */
	  detectorEvent1[k].clear();
	  detectorEvent2[k].clear();
	}
      }
      detectorEvent1.clear();
      detectorEvent2.clear();
    }
  }
}

void SRSHistoManager::PedSubAnalysis(SRSEventBuilder * eventbuilder, const eventHeaderStruct * eventHeader) {
  //  printf("SRSHistoManager::PedSubAnalysis(): => entering the PedSubAnalysis \n");
  if(!eventbuilder->IsAGoodEvent()) return;

  Int_t eventType   = eventHeader->eventType;
  Int_t eventNumber = eventbuilder->GetEventNumber();
  map < TString, TList * > hitsInDetectorPlaneMap = eventbuilder->GetHitsInDetectorPlane();

  Int_t size1 = hitsInDetectorPlaneMap.size();

  map < TString, TList * >::const_iterator plane_itr;
  for (plane_itr = hitsInDetectorPlaneMap.begin(); plane_itr != hitsInDetectorPlaneMap.end(); ++plane_itr) {
    TString plane  = (* plane_itr).first;
  }

  // START FILLING HISTOGRAMS
  map<TString,TObject*>::const_iterator objToDraw_itr;
  for (objToDraw_itr = fObjToDraw.begin(); objToDraw_itr != fObjToDraw.end(); ++objToDraw_itr) {

    TString histoName = (*objToDraw_itr).first;    
    if (!fObjToDraw[histoName]) {
      continue;
    }

    // Histograms to be reset at each event
    if ( (fVarType[histoName]=="HITPEDOFFSET") || (fVarType[histoName]=="RAWHITS") || (fVarType[histoName]=="HITRAWDATA") || (fVarType[histoName]=="HITCOMMODE") || (fVarType[histoName]=="PADHITPEDOFFSET") || (fVarType[histoName]=="PADRAWHITS") ) {      fHist1dToReset.push_back((TH1F*)(fObjToDraw[histoName]));
    }
    else continue;

    fNbOfEvents[histoName] += 1;
    SetHistoTitle(histoName,  fEventNumber, fNbOfEvents[histoName]);

    TString detector     = fMapping->GetDetectorFromPlane(fDetectorPlane1[histoName].Data());
    TString detectorType = fMapping->GetDetectorTypeFromDetector(detector);
    TString readoutBoard = fMapping->GetReadoutBoardFromDetector(detector);

    TList  * listOfHits = hitsInDetectorPlaneMap[fDetectorPlane1[histoName].Data()];
  
    Int_t  size = listOfHits->GetSize() ;

    if(size == 0) {
      //     printf("SRSHistoManager::PedSubAnalysis(): => after TList \n");
     listOfHits->Clear();
      continue;
    }

    //=========   HIT INFORMATION
    if ( (fVarType[histoName]=="HITPEDOFFSET") || (fVarType[histoName]=="PADHITPEDOFFSET") || (fVarType[histoName]=="RAWHITS") || (fVarType[histoName]=="PADRAWHITS") ) { 
      Int_t padNo  = 0;
      Int_t padXNo = 0;
      Int_t padYNo = 0;
      Int_t nbPadX = (Int_t) (fMapping->GetPadDetectorMap(detector) [2]);
      Int_t nbPadY = (Int_t) (fMapping->GetPadDetectorMap(detector) [3]);

      if ( (fVarType[histoName]=="HITPEDOFFSET") || (fVarType[histoName]=="RAWHITS") || (fVarType[histoName]=="HITDIST") ) { 
	if(readoutBoard =="PADPLANE") {
	  if ( histoName.Contains("PadX")) ((TH1F*) fObjToDraw[histoName])->SetBins(nbPadX, -0.5, (nbPadX - 0.5)); 
	  if ( histoName.Contains("PadY")) ((TH1F*) fObjToDraw[histoName])->SetBins(nbPadY, -0.5, (nbPadY - 0.5)); 
	} 
	else {
	  //	  printf("SRSHistoManager::PedSubAnalysis(): => set Hist \n");;
	  Int_t nbBins = NCH * fMapping->GetNbOfAPVs(fDetectorPlane1[histoName]); 
	  Int_t nmax  =  NCH * fMapping->GetNbOfAPVs(fDetectorPlane1[histoName]); 
	  ((TH1F*) fObjToDraw[histoName])->SetBins(nbBins,0, nmax);
	  ((TH1F*) fObjToDraw[histoName])->SetXTitle("Strip number");
	}
      }

      TIter nextHit(listOfHits);
      while( SRSHit * hit = ( (SRSHit *) nextHit() ) ) {
	Int_t stripNb        = hit->GetStripNo();
	Float_t hitADCcounts = hit->GetHitADCs();

	if(readoutBoard == "PADPLANE") {
	  padNo  = hit->GetPadNo();
	  padXNo = hit->GetPadXNo();
	  padYNo = hit->GetPadYNo();
	}

	if ( (fVarType[histoName]=="PADHITPEDOFFSET" ) || (fVarType[histoName]=="PADRAWHITS") ) ((TH2F*) fObjToDraw[histoName])->Fill(padXNo, padYNo, hitADCcounts);
   
	if ( (fVarType[histoName] ==  "HITPEDOFFSET" ) || (fVarType[histoName]=="RAWHITS") ) {
	  ((TH1F*) fObjToDraw[histoName])->SetYTitle("ADC Counts");
	  if (readoutBoard == "PADPLANE") {
	    if ( histoName.Contains("PadX")) ((TH1F*) fObjToDraw[histoName])->Fill(padXNo, hitADCcounts);
	    if ( histoName.Contains("PadY")) ((TH1F*) fObjToDraw[histoName])->Fill(padYNo, hitADCcounts);
	  }
	  else {
	    ((TH1F*) fObjToDraw[histoName])->Fill(stripNb,hitADCcounts);
	  }
	}

      }
    }
  }
//	 VVVV WAS COMMENTED OUT
    //printf("SRSHistoManager::PedSubAnalysis(): => Exit \n");
}



//===============================================================================================================================
void SRSHistoManager::ReadHistoCfgFile(const char * histoCfgname){
  printf("\nSRSHistoManager::ReadHistoCfgFile() => Enter: Opening Histo Cfg file: %s\n", histoCfgname);
  ifstream file (histoCfgname, ifstream::in);

  Int_t iline = 0;
  TString detector1, detector2, plane1, plane2, fecNoStr, fecChannelStr, type, line, name, title, fitParam1Str, fitParam2Str, fitFunction;
  TString histoNameGraph, SizeX, SizeY, SizeZ;
  TString xOffset, yOffset, zOffset, invertCoordinate;

  while (line.ReadLine(file)) {
    //=== reset flags
    Bool_t is1dHisto = kFALSE, is2dHisto = kFALSE, isRawData = kFALSE, isTimeBin = kFALSE, isFit = kFALSE, isCMSGEM = kFALSE;
    Bool_t isPlaneTimeBin = kFALSE, isSpectrum = kFALSE, isOneDetPlane = kFALSE, isTwoPlanes = kFALSE,  isPadPlane = kFALSE, isTwoDetectors = kFALSE, is2dDetector = kFALSE;

    iline++;
    Int_t column = 0;
    line.Remove(TString::kLeading, ' ');   // strip leading spaces 
    if (line.BeginsWith("#")) continue;   // and skip comments
    if (line.BeginsWith("@")) continue;   // and skip comments

    //    printf("   SRSHistoManager::ReadHistoCfgFile() ==> Scanning the histo cfg file %s\n",line.Data());
    TObjArray * tokens = line.Tokenize(","); // Create an array of the tokens separated by "," in the line; lines without "," are skipped
    TIter myiter(tokens);                    // iterator on the tokens array 

    while (TObjString * st = (TObjString*) myiter.Next()) {  // inner loop (loop over the line)
      TString s = st->GetString().Remove(TString::kLeading, ' ' ); // remove leading spaces
      s.Remove(TString::kTrailing, ' ' );                          // remove trailing spaces 
  
      //      printf("     SRSHistoManager::ReadHistoCfgFile() ==> Data ==> %s\n",s.Data());
      if (column == 0) {

	if (s == "BINNING") { // Reset the binning of the histos
	  TString bintype = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );

	  if (bintype != "CLUSTERDIST" ) {
	    //	    printf("  SRSHistoManager::ReadHistoCfgFile() ==> binType ==> %s\n", bintype.Data());
	    Int_t nbin      = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	    Float_t min     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	    Float_t max     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	    Reset1DHistBinning(bintype, nbin, min, max);
	  }
	  else {
	    //	    printf("  SRSHistoManager::ReadHistoCfgFile() ==> binType ==> %s\n", bintype.Data());
	    TString nbinStr = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	    TString minStr  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	    TString maxStr  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
   
	    Int_t nbin  = 99099;
	    Float_t min = 99099.0;
	    Float_t max = 99099.0;

	    if(nbinStr != "INF") nbin = nbinStr.Atoi();
	    if(minStr  != "INF") min = minStr.Atof();
	    if(maxStr  != "INF") max = maxStr.Atof();
	    Reset1DHistBinning(bintype, nbin, min, max);
	  }
	  continue;
	}

	else if (s == "BINNING2D") { // Reset the binning of the histos
          TString bintype  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
          TString nbinStr  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
          TString minStr   = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' ); 
          TString maxStr   = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
          TString nbin2Str = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
          TString min2Str  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
          TString max2Str  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );

	  Int_t nbin   = 99099;
	  Int_t nbin2  = 99099;
          Float_t min  = 99099.0;
          Float_t max  = 99099.0;
          Float_t min2 = 99099.0;
          Float_t max2 = 99099.0;

	  if(nbinStr  != "INF") nbin  = nbinStr.Atoi();
	  if(nbin2Str != "INF") nbin2 = nbin2Str.Atoi();
          if(minStr   != "INF") min   = minStr.Atof();
          if(min2Str  != "INF") min2  = min2Str.Atof();
          if(maxStr   != "INF") max   = maxStr.Atof();
          if(max2Str  != "INF") max2  = max2Str.Atof();
          Reset2DHistBinning(bintype, nbin, min, max, nbin2, min2, max2);
          continue;
        }

	else if (s == "HITTIMING") {
	  BookHitTimingHistos();
	}

        else if ( (s == "HITPEDOFFSET") || (s == "RAWHITS") || (s == "HITCOMMODE") ||  (s == "NOISERESO") || (s == "HITRAWDATA") || (s == "CLUSTSTAT") || (s == "HITDIST")  || (s == "CLUSTERDIST") || (s == "HITZEROSUP") || (s == "CLUSTERHITS") || (s == "CLUSTSIZE") || (s == "CLUSTMULT") ) {
          is1dHisto     = kTRUE;
          isOneDetPlane = kTRUE;
	} 

        else if ( (s == "ADCDIST") || (s == "TIMESLICE") || (s == "OCCUPANCY") || (s == "CLUSTERTIMEBIN") || (s == "HITCROSSTALK") || (s == "CHARGECROSSTALK") ||  (s == "SPECTRUM") ) {
          is1dHisto     = kTRUE;
          isOneDetPlane = kTRUE;
	} 

      	else if ( (s == "HITMAP") || (s == "CHARGES_SH") || (s == "CHARGES_RATIODIST") ){
	  is2dHisto    = kTRUE;
          is2dDetector = kTRUE;
        }

     	else if (s == "CORRELATION") {
	  is2dHisto   = kTRUE;
          isTwoPlanes = kTRUE;
        }

     	else if ( (s == "PADHITPEDOFFSET") || (s == "PADHITDIST") || (s == "PADHITZEROSUP") || (s == "PADRAWHITS") || (s == "PADCLUSTERHITS") ){
	  is2dHisto  = kTRUE;
          isPadPlane = kTRUE;
        }

    	else if ( s == "CMSHITMAP"){
	  isCMSGEM = kTRUE;
        }

	else if (s == "CHARGES_RATIO")  {
          is1dHisto          = kTRUE;
	  is2dDetector = kTRUE; 
	} 
 
      	else if (s == "PLANETIMEBIN" )  {
	  is2dHisto           = kTRUE;
	  isPlaneTimeBin = kTRUE;
	  isOneDetPlane  = kTRUE;
	}

      	else if (s == "DIVIDEDHISTOS") {
	  TString firstHisto  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  TString secondHisto = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  fDividedHistos[firstHisto] = secondHisto;
	  fHistosRatioCut   = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	}

 	else if (s == "FIT") {
	  TString histo = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  TString typeOfFit = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  TString firstParam = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  TString secondParam = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  fFittedHistos[histo].push_back(typeOfFit);
	  fFittedHistos[histo].push_back(firstParam);
	  fFittedHistos[histo].push_back(secondParam);
	}

	else {
	  Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration line %s: unknown command", line.Data());
	  continue;
	}
	// Get the Type
	type = s;
      }

      //=== CONFIG FILE ERRORS
      if ( (isCMSGEM) && (column > 3)) {
	Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line not the right length", line.Data());
	continue;
      }
      if ( (isOneDetPlane || isPlaneTimeBin) && (column > 4)) {
	Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line not the right length", line.Data());
	continue;
      }

      if (isTwoPlanes && (column > 5)) {
        Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line too long", line.Data());
        continue;
      }

      if (is2dDetector && (column > 4)) {
        Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line too long", line.Data());
        continue;
      }

      if (isPadPlane && (column > 4)) {
        Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line too long", line.Data());
        continue;
      }

      if (isFit && (column > 6)) {
        Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line too long", line.Data());
        continue;
      }

      switch (column) {
      case 1:
	name = s;
	break;

      case 2:
	title = s;
	break;

      case 3:
	if  (isTimeBin)          fecNoStr  = s;
        else if (isFit)          fitParam1Str = s;
	else if (is2dDetector)   detector1 = s;
 	else if (isPadPlane)     detector1 = s;
        else                     plane1 = s;
	break;

      case 4:
        if (isTimeBin)           fecChannelStr = s;
        else if (isFit)          fitParam2Str  = s;
	else                     plane2 = s;
	break;

      case 5:
        fitFunction = s;
	break;
      }
      column++;
    } // End While : Inner loop (loop over the line)

    //=== CONFIG FILE ERRORS      
    if ( (isCMSGEM) && (column != 3)) { 
      Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line not the right length", line.Data());
      continue;
    } 
    if ( (isOneDetPlane || isPlaneTimeBin) && (column != 4)) {
      Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line not the right length", line.Data());
      continue; 
    }

    if ( (isTwoPlanes|| isTwoDetectors)  && (column != 5)) {
        Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line too long", line.Data());
        continue; 
    }

    if (isPadPlane && (column != 4)) {
        Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line too long", line.Data());
        continue; 
    }

    if (is2dDetector && (column != 4)) {
        Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line too long", line.Data());
        continue; 
    }

    if (isFit && (column != 6)) {
        Error("SRSHistoManager","ReadHistoCfgFile:=> Error in configuration %s: histo line too long", line.Data());
        continue; 
    }
    tokens->Delete();

    //===================================================================================================
    // BOOKING THE HISTOGRAMS AND UPDATING THE RECORDS
    //******************************************************************  
    if (is1dHisto && isOneDetPlane)  Book1DHistos(name, title, type, plane1);
    if (is1dHisto && isTwoPlanes)    Book1DHistos(name, title, type, plane1, plane2);
    if (is2dHisto && isTwoPlanes)    Book2DHistos(name, title, type, plane1, plane2);
    if (is2dHisto && isPadPlane)     Book2DHistos(name, title, type, detector1);
    if (is1dHisto && is2dDetector)   Book1DHistos(name, title, type, detector1);
    if (is2dHisto && is2dDetector)   Book2DHistos(name, title, type, detector1);
  }
  printf("SRSHistoManager::ReadHistoCfgFile() ==> Exit\n");
}

//==================================================================================
void SRSHistoManager::SaveAllHistos() {
  //  printf("   SRSHistoManager::SaveAllHistos() => Entering ======== \n" );
  TString fileName = fOutputDir + "/results/" + fOutputFile + ".root";
  //  TString fileName = fOutputDir + "/results/" + fOutputFile + "_SRS" + fAmoreAgentID + ".root";

  TFile * file = new TFile(fileName,"recreate"); 
  if (!file->IsOpen()){
    Error("SRSHistoManager","SaveHistos:=> Cannot open file %s", fileName.Data());
  }

  map<TString, TString>::const_iterator ratio_itr;
  for (ratio_itr = fDividedHistos.begin(); ratio_itr != fDividedHistos.end(); ++ratio_itr) {

    TString firsthistoname = (*ratio_itr).first;
    TString secondhistoname = (*ratio_itr).second;

    TH2F* histo1 =((TH2F*) fObjToDraw[firsthistoname]);
    TH2F* histo2 =((TH2F*) fObjToDraw[secondhistoname]);

    Int_t nbOfBinsX = histo1->GetNbinsX();
    Int_t nbOfBinsY = histo1->GetNbinsY();

    if (! histo1) {
      printf("   SRSHistoManager::SaveAllHistos() WARNING !!!! histo1 = %s  do not exist for fit \n", firsthistoname.Data());
      printf("   SRSHistoManager::SaveAllHistos() WARNING !!!! PLEASE CHECK YOUR HISTO CONFIG FILE \n");
      continue;
    }

    if (! histo2) {
      printf("   SRSHistoManager::SaveAllHistos() WARNING !!!! histo2 = %s  do not exist for fit \n", secondhistoname.Data());
      printf("   SRSHistoManager::SaveAllHistos() WARNING !!!! PLEASE CHECK YOUR HISTO CONFIG FILE \n");
      continue;
    }

    for (Int_t ii = 0; ii < nbOfBinsX; ii++) {
      for (Int_t jj = 0; jj < nbOfBinsY; jj++) {
	Int_t binx = ii + 1;
	Int_t biny = jj + 1; 
	Int_t binContent = ((Int_t ) histo2->GetBinContent(binx,biny));
	if (binContent < fHistosRatioCut ) histo1->SetBinContent(binx,biny,0);
      }
    }
    histo1->Divide(histo2);
  } 

  map<TString, vector<TString> >::const_iterator fit_itr;
  for (fit_itr = fFittedHistos.begin(); fit_itr != fFittedHistos.end(); ++fit_itr) {
    TString histoname = (*fit_itr).first;

    TH1F* histo =((TH1F*) fObjToDraw[histoname]);
    if (! histo) {
      printf("   SRSHistoManager::SaveAllHistos() WARNING !!!! histo = %s  do not exist for fit \n", histoname.Data());
      printf("   SRSHistoManager::SaveAllHistos() WARNING !!!! PLEASE CHECK YOUR HISTO CONFIG FILE \n");
      continue;
    }

    vector<TString> fitParameter = (*fit_itr).second;
    TString typeOfFit = fitParameter[0];
    Float_t start = fitParameter[1].Atof();
    Float_t end = fitParameter[2].Atof();
    printf("   SRSHistoManager::SaveAllHistos() typeOfFit  = %s, start = %f, end = %f \n", typeOfFit.Data(), start, end);

    if (histo->GetEffectiveEntries() < 1000) { 
      printf("   SRSHistoManager::SaveAllHistos(): Statistic of histo = %s is too low for the %s fit \n", histoname.Data(), typeOfFit.Data());
      continue;
    }

    if ((typeOfFit == "Gauss")  || (typeOfFit == "gauss") || (typeOfFit == "Gaussian")  || (typeOfFit == "gaussian")) {
      GaussFit (histo);
    }
    if ((typeOfFit == "Landau") || (typeOfFit == "landau") ) {
      LandauFit (histo);
    }
    if ((typeOfFit == "DoubleGauss") || (typeOfFit == "doubleGauss") || (typeOfFit == "doublegauss")) {
      DoubleGaussFit(histo, 2, 3., kTRUE);
    }
  }

  map<TString, TObject*>::const_iterator itr;
  for (itr = fObjToDraw.begin(); itr != fObjToDraw.end(); ++itr) {
    TString name = (*itr).first;

    if (fObjToDraw[name]) {
      if ((fVarType[name]=="HITMAP") ||(fVarType[name]=="CMSHITMAP") || (fVarType[name]=="CHARGESMAP")) {
	if (! file->GetDirectory("HITMAP") ) file->mkdir("HITMAP");
	file->cd();
	gDirectory->cd("HITMAP");
	fObjToDraw[name]->Write();
      }

      if (fVarType[name]=="RAWDATA")  {
	if (! file->GetDirectory("RawData") ) file->mkdir("RawData");
        file->cd();
        gDirectory->cd("RawData");
        fObjToDraw[name]->Write();
      }

      if ( (fVarType[name]=="HITCROSSTALK") || (fVarType[name]=="CHARGECROSSTALK") ) {
	if (! file->GetDirectory("Crostalk") ) file->mkdir("Crostalk");
	file->cd();
	gDirectory->cd("Crostalk");
	fObjToDraw[name]->Write();
      }

      if (fVarType[name]=="PEDESTALS") {
	if (! file->GetDirectory("Pedestals") ) file->mkdir("Pedestals");
	file->cd();
        gDirectory->cd("Pedestals");
	Info ("SRSHistoManager","SaveHistos:=> Histo %s saved", name.Data());
	fObjToDraw[name]->Write();
      }

      if ( (fVarType[name]=="HITDIST") || (fVarType[name]=="PADHITDIST") ) {
	if (! file->GetDirectory("HitDist") ) file->mkdir("HitDist");
	file->cd();
        gDirectory->cd("HitDist");
	((TH1F*) fObjToDraw[name])->Scale( 1/ ((TH1F*) fObjToDraw[name])->GetMaximum());
	fObjToDraw[name]->Write();
      } 

      if (fVarType[name]=="CLUSTERDIST") {
	if (! file->GetDirectory("ClusterDist") ) file->mkdir("ClusterDist");
	file->cd(); 
        gDirectory->cd("ClusterDist");
	((TH1F*) fObjToDraw[name])->Scale( 1/ ((TH1F*) fObjToDraw[name])->GetMaximum());
	fObjToDraw[name]->Write();
      }

      if ( (fVarType[name]=="CLUSTSIZE") || (fVarType[name]=="CLUSTMULT") || (fVarType[name]=="OCCUPANCY")){
	if (! file->GetDirectory("Cluster") ) file->mkdir("Cluster");
	file->cd();
	gDirectory->cd("Cluster");
	((TH1F*) fObjToDraw[name])->Scale( 1/ ((TH1F*) fObjToDraw[name])->GetMaximum());
	fObjToDraw[name]->Write();
      }

      if (fVarType[name]=="TIMESLICE") {
	if (! file->GetDirectory("Timing") ) file->mkdir("Timing");
	file->cd();
	gDirectory->cd("Timing");
	((TH1F*) fObjToDraw[name])->Scale( 1/ ((TH1F*) fObjToDraw[name])->GetMaximum());
	fObjToDraw[name]->Write();
      }

      if (fVarType[name]=="CORRELATION") {
	if (! file->GetDirectory("Correlation") ) file->mkdir("Correlation");
	file->cd();
	gDirectory->cd("Correlation");
	fObjToDraw[name]->Write();
      }

       if ((fVarType[name]=="CHARGES_SH") || (fVarType[name]=="CHARGES_RATIO") ||  (fVarType[name]=="CHARGES_RATIODIST")) {
	if (! file->GetDirectory("ChargeSharing") ) file->mkdir("ChargeSharing"); 
	file->cd(); 
        gDirectory->cd("ChargeSharing");
	//	if ( fVarType[name]=="CHARGES_RATIO") ((TH1F*) fObjToDraw[name])->Scale( 1/ ((TH1F*) fObjToDraw[name])->GetMaximum());
	fObjToDraw[name]->Write();
      }

      if (fVarType[name]=="NOISERESO") {
	if (! file->GetDirectory("NoiseReso") ) file->mkdir("NoiseReso");
	file->cd();
        gDirectory->cd("NoiseReso");
	fObjToDraw[name]->Write();
      }

      if ( (fVarType[name]=="ADCDIST") || (fVarType[name]=="SPECTRUM")) {
	if (! file->GetDirectory("adcDist") ) file->mkdir("adcDist");
	file->cd();
        gDirectory->cd("adcDist");
	//	((TH1F*) fObjToDraw[name])->Scale( 1/ ((TH1F*) fObjToDraw[name])->GetMaximum());
	fObjToDraw[name]->Write();
      }
    }
  }
  file->Close(); 
}

//==================================================================================
void SRSHistoManager::SaveNTuple() {
  printf("   SRSHistoManager::SaveTrackingHistos() => Entering ======== \n" );
  TString fileName = fOutputDir + fOutputFile + "_nTuple.root";
  TFile * file = new TFile(fileName,"recreate"); 
  if (!file->IsOpen()){
    Error("SRSHistoManager","SaveTrackingHistos:=> Cannot open file %s", fileName.Data());
  }  
  file->mkdir("plot3D");
  file->cd();
  gDirectory->cd("plot3D");

  GetStyle(kTRUE); 
  TString histoname = fNtuple->GetName();
  TString picturename  = fOutputDir + fOutputFile + "_" + histoname + ".png";
  TCanvas c("c1", "c1", 80,80,3080,2080);
  c.cd();
  fNtuple->Draw("y:x:z:parameter","","colz");
  fNtuple->UseCurrentStyle();
  c.SaveAs(picturename);

  fNtuple->Write(); 
  file->Close();
} 

//==================================================================================
void SRSHistoManager::GetStyle(Bool_t setStats) { 

  gStyle->SetOptStat(0);
  if(setStats) {
    gStyle->SetOptStat(1100);
    gStyle->SetStatColor(0);
    gStyle->SetStatX(0.9);
    gStyle->SetStatY(0.9);
    gStyle->SetStatW(0.25);
    gStyle->SetStatH(0.25);
  }

  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetLabelFont(62,"xyz");
  gStyle->SetLabelSize(0.045,"xyz");
  gStyle->SetLabelColor(1,"xyz");
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleSize(0.06,"xyz");
  gStyle->SetTitleOffset(.8,"xyz");
  gStyle->SetPalette(1);
  gStyle->SetMarkerStyle(1);
  gStyle->SetMarkerSize(1);

  const Int_t NRGBs = 5;
  const Int_t NCont = 99;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
}

//==================================================================================
void SRSHistoManager::DumpList() {
  map<TString,TObject*>::const_iterator itr;
  for (itr = fObjToDraw.begin(); itr != fObjToDraw.end(); ++itr){
    cout << itr->first <<" " << itr->second << endl;
  }
}

//==================================================================================
void SRSHistoManager::GaussFit( TH1F* h) {

  gStyle->SetOptFit(1110);
  gStyle->SetStatFontSize(0.02);
  gStyle->SetHistFillColor(kRed);

  Float_t fwhm = h->FindLastBinAbove(h->GetMaximum()/2) - h->FindFirstBinAbove(h->GetMaximum()/2); 
  printf("\n   SRSHistoManager::GaussFit() => FWHM = %.2f\n", fwhm);

  Float_t mean = h->GetMean();
  Float_t start = mean - fwhm;
  Float_t end   = mean + fwhm;

  printf("\n   SRSHistoManager::GaussFit() => FWHM = %.2f, mean =  %.2f, start =  %.2f, end = %.2f \n", fwhm, mean, start, end);

  Double_t par[3];
  TF1 * gauss = new TF1("fitFunction","gaus", start, end);
  gauss->GetParameters(&par[0]);
  gauss->SetLineColor(kBlack);
  gauss->SetLineWidth(2);
  h->Fit(gauss,"R+"); 
}

//==================================================================================
void SRSHistoManager::GaussFit( TH1S* h) {

  gStyle->SetOptFit(1110);
  gStyle->SetStatFontSize(0.02);
  gStyle->SetHistFillColor(kRed);

  Float_t fwhm = h->FindLastBinAbove(h->GetMaximum()/2) - h->FindFirstBinAbove(h->GetMaximum()/2); 
  printf("\n   SRSHistoManager::GaussFit() => FWHM = %.2f\n", fwhm);

  Float_t mean = h->GetMean();
  Float_t start = mean - fwhm;
  Float_t end   = mean + fwhm;

  printf("\n   SRSHistoManager::GaussFit() => FWHM = %.2f, mean =  %.2f, start =  %.2f, end = %.2f \n", fwhm, mean, start, end);

  Double_t par[3];
  TF1 * gauss = new TF1("fitFunction","gaus", start, end);
  gauss->GetParameters(&par[0]);
  gauss->SetLineColor(kBlack);
  gauss->SetLineWidth(2);
  h->Fit(gauss,"R+"); 
}
//================================================================================================//
void SRSHistoManager::LandauFit( TH1F* h) { 

  gStyle->SetOptFit(1110);
  gStyle->SetStatFontSize(0.02);
  gStyle->SetHistFillColor(kGreen);

  Double_t MostProb =  h->GetMaximum();
  Double_t spread = h->GetRMS();
  Float_t fwhm = h->FindFirstBinAbove(h->GetMaximum()/2) - h->FindLastBinAbove(h->GetMaximum()/2); 

  Double_t from = MostProb - 0.2 * spread;
  Double_t to   = MostProb + 3.0 * spread;

  Double_t par[3];
  TF1 * landau = new TF1("fitFunction","landau",from,to);
  landau->SetParameters(1,MostProb,spread);
  landau->SetParNames("const","MPV","Lambda");
  landau->GetParameters(&par[0]);
  landau->SetRange(from, to);
  landau->SetLineColor(kBlack);
  landau->SetLineWidth(2);
  h->Fit(landau,"R+");
}

//================================================================================================//
void SRSHistoManager::LandauFit( TH1S* h) { 

  gStyle->SetOptFit(1110);
  gStyle->SetStatFontSize(0.02);
  gStyle->SetHistFillColor(kGreen);

  Double_t MostProb =  h->GetMaximum();
  Double_t spread = h->GetRMS();
  Float_t fwhm = h->FindFirstBinAbove(h->GetMaximum()/2) - h->FindLastBinAbove(h->GetMaximum()/2); 

  Double_t from = MostProb - 0.2 * spread;
  Double_t to   = MostProb + 3.0 * spread;

  Double_t par[3];
  TF1 * landau = new TF1("fitFunction","landau",from,to);
  landau->SetParameters(1,MostProb,spread);
  landau->SetParNames("const","MPV","Lambda");
  landau->GetParameters(&par[0]);
  landau->SetRange(from, to);
  landau->SetLineColor(kBlack);
  landau->SetLineWidth(2);
  h->Fit(landau,"R+");
}

//================================================================================================//
void SRSHistoManager::DoubleGaussFit(TH1S *htemp, Int_t N_iter,  Float_t N_sigma_range, Bool_t ShowFit) { 

  gStyle->SetHistLineColor(kBlue);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1100);

  //---------Basic Histo Peak Finding Parameters----------
  Int_t binMaxCnt   = htemp->GetMaximumBin();  
  Int_t binMaxCnt_counts = (Int_t) htemp->GetBinContent(binMaxCnt); 
  Double_t binMaxCnt_value  = (Double_t) htemp->GetXaxis()->GetBinCenter(binMaxCnt); 

  Int_t NPeaks;
  Float_t * Peak;           
  Float_t * PeakAmp;   

  Float_t peak_pos = 0;
  Float_t peak_pos_amp = 0;  //Initial value, assuming positive going peaks
  Int_t   peak_pos_bin = 0;  

  Int_t Nbins = 0;
  Int_t zero_value_bin = 0;
  Float_t low_limit = 0;
  Float_t high_limit = 0;
  Float_t peak_pos_count = 0;
  Float_t zero_bin_value = 0;
  Float_t max_bin_value = 0;

  TSpectrum * s = new TSpectrum();
  //  printf("   SRSHistoManager::DoubleGaussianFit() After TSpectrum \n");
  
  //---------TSpectrum Peak Finding Parameters--------
  if (ShowFit) NPeaks = s->Search(htemp, 2, "goff", 0.5); //opens a canvas (one time in a loop), even with:  s->Search(htemp, 2, "nodraw", 0.9);
  Peak = s->GetPositionX();
  PeakAmp = s->GetPositionY();

  for ( Int_t i = 0; i < NPeaks; i++)     {
    if (peak_pos_amp < PeakAmp[i]) 	{
      peak_pos_amp = PeakAmp[i];   //TSpectrum finds peak counts
      peak_pos = Peak[i];          //TSpectrum finds pos. of peak in x-axis units
      printf("   SRSHistoManager::DoubleGaussianFit(): i = %d,  peak_pos = %f, peak_pos_amp = %f \n", i, peak_pos, peak_pos_amp  );
    }
  }

  peak_pos_bin = htemp->GetXaxis()->FindBin(peak_pos);
  peak_pos_count = htemp->GetBinContent(peak_pos_bin);
  zero_value_bin = htemp->GetXaxis()->FindBin(0.0);
  Nbins = htemp->GetSize() - 2;
  zero_bin_value =  htemp->GetXaxis()->GetBinCenter(0);
  max_bin_value = htemp->GetXaxis()->GetBinCenter(Nbins);
 
  TF1 *func;
  TF1 *func1;
  TF1 *func2;
  TF1 *func3;

  Float_t Chi2;
  Int_t NDF = 1;

  Float_t RChi2 = 1; 
  Float_t peak  = 1;  

  Float_t const1     = 1;
  Float_t mean1      = 1;
  Float_t mean_err1  = 1;
  Float_t sigma1     = 1;
  Float_t sigma_err1 = 1;

  Float_t const2     = 1;
  Float_t mean2      = 1;
  Float_t mean_err2  = 1;
  Float_t sigma2     = 1;
  Float_t sigma_err2 = 1;
  
  int TS = 0;
  func = new TF1("func", "gaus");

  //Make sure that TSpectrum peak is within histo range if not, use Par initial values from Basic Histo Peak Find
  if (peak_pos >= zero_bin_value  &&  peak_pos <= max_bin_value)  {
    TS=1;
    low_limit  = peak_pos - (0.1 * abs(max_bin_value-zero_bin_value));
    high_limit = peak_pos + (0.1 * abs(max_bin_value-zero_bin_value));
    func->SetParameter(0, peak_pos_count);
    func->SetParameter(1, peak_pos);
  }

  else {
   low_limit = binMaxCnt_value - (0.1 * abs(max_bin_value-zero_bin_value)); 
   high_limit = binMaxCnt_value + (0.1 * abs(max_bin_value-zero_bin_value)); 
   func->SetParameter(0, binMaxCnt_counts);
   func->SetParameter(1, binMaxCnt_value);
  }

  //low_limit, high_limit); //  To Show fit: htemp->Fit("gaus"); //better fit?-> Fit("gaus", "MQ", "", "", ""); 
  htemp->Fit("gaus", "Q0", "", low_limit, high_limit);
  func  = htemp->GetFunction("gaus");
  Chi2 = func->GetChisquare();
  NDF = func->GetNDF();
  if (NDF != 0) RChi2 = Chi2/NDF;
  const1 = func->GetParameter(0);
  mean1 = func->GetParameter(1);
  mean_err1 = func->GetParError(1);
  sigma1 = func->GetParameter(2);
  sigma_err1 = func->GetParError(2);

  printf("\n   SRSHistoManager::DoubleGaussianFit(): ===== BEFORE  FIRST LOOP \n");

  for (int i=0; i< 2; i++) {
    htemp->Fit("gaus", "Q0", "",(mean1 - (N_sigma_range*sigma1)), (mean1 + (N_sigma_range*sigma1) )); //don't show fit
    func  = htemp->GetFunction("gaus");
    Chi2 = func->GetChisquare();
    NDF = func->GetNDF();

    if (NDF != 0) RChi2 = Chi2/NDF;
    const1     = func->GetParameter(0);
    mean1      = func->GetParameter(1);
    mean_err1  = func->GetParError(1);
    sigma1     = func->GetParameter(2);  
    sigma_err1 = func->GetParError(2);
  }

  //Amplitude
  peak =  func->GetParameter(0); 

  //background height ~ bgd_h*gaus_amp
  Float_t bgd_h = 0.25;  

  func1 = new TF1("func1", "gaus");
  func2 = new TF1("func2", "gaus");

  func3 = new TF1("fitFunction", "func1 + func2", (mean1 - 10*sigma1), (mean1 + 10*sigma1) );
  func3->SetLineColor(kBlack);

  //-----------Fit Parameter constraints:
  func3->SetParameters(const1, mean1, sigma1, const1/5, mean1, 20*sigma1); //Set Initial Valules
  func3->SetParLimits(3, 0, (0.1*peak_pos_amp) ); 

  htemp->Fit("fitFunction", "Q0"); 
  func  = htemp->GetFunction("fitFunction");
  Chi2 = func3->GetChisquare();
  NDF = func->GetNDF();

  if (NDF != 0) RChi2 = Chi2/NDF;
  const1     = func3->GetParameter(0);
  mean1      = func3->GetParameter(1);
  mean_err1  = func3->GetParError(1);
  sigma1     = func3->GetParameter(2);
  sigma_err1 = func3->GetParError(2);
  const2     = func3->GetParameter(3);
  mean2      = func3->GetParameter(4);
  mean_err2  = func3->GetParError(4);
  sigma2     = func3->GetParameter(5);
  sigma_err2 = func3->GetParError(5);
  func3->SetParNames("Const", "Mean", "Sigma", "Bkg Const", "Bkg Mean", "Bkg Sigma");

  for (Int_t j=0; j<4; j++)  {
    func3->SetParameters(const1, mean1, sigma1, const2, mean2, sigma2);
    //----------------Show or don't show fit----------------- 
    if (ShowFit) htemp->Fit("fitFunction", "R+"); 
    else htemp->Fit("fitFunction", "Q0");        
    //-------------------------------------------------------     

    func3 = htemp->GetFunction("fitFunction");
    func3->SetLineColor(kRed);
    Chi2 = func3->GetChisquare();
    NDF = func3->GetNDF();
    if (NDF != 0) RChi2 = Chi2/NDF;

    const1     = func3->GetParameter(0);
    mean1      = func3->GetParameter(1);
    mean_err1  = func3->GetParError(1);
    sigma1     = func3->GetParameter(2);
    sigma_err1 = func3->GetParError(2);
    const2     = func3->GetParameter(3);
    mean2      = func3->GetParameter(4);
    mean_err2  = func3->GetParError(4);
    sigma2     = func3->GetParameter(5);
    sigma_err2 = func3->GetParError(5);
  }

  if (abs(const1 - peak) < abs(const2 - peak)) {
    fRChi2      = RChi2;
    fMean       = mean1;
    fMeanError  = mean_err1;
    fSigma      = abs(sigma1);
    fSigmaError = sigma_err1;
  }
  else {
    fRChi2      = RChi2;
    fMean       = mean2;
    fMeanError  = mean_err2;
    fSigma      = abs(sigma2);
    fSigmaError = sigma_err2;
  }
  delete s;
  delete func; 
  delete func1;
  delete func2;
  //  delete func3; 
 }

//================================================================================================//
void SRSHistoManager::DoubleGaussFit(TH1F *htemp, Int_t N_iter,  Float_t N_sigma_range, Bool_t ShowFit) { 

  gStyle->SetHistLineColor(kBlue);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1100);

  //---------Basic Histo Peak Finding Parameters----------
  Int_t binMaxCnt   = htemp->GetMaximumBin();  
  Int_t binMaxCnt_counts = (Int_t) htemp->GetBinContent(binMaxCnt); 
  Double_t binMaxCnt_value  = (Double_t) htemp->GetXaxis()->GetBinCenter(binMaxCnt); 

  Int_t NPeaks;
  Float_t * Peak;           
  Float_t * PeakAmp;   

  Float_t peak_pos = 0;
  Float_t peak_pos_amp = 0;  //Initial value, assuming positive going peaks
  Int_t   peak_pos_bin = 0;  

  Int_t Nbins = 0;
  Int_t zero_value_bin = 0;
  Float_t low_limit = 0;
  Float_t high_limit = 0;
  Float_t peak_pos_count = 0;
  Float_t zero_bin_value = 0;
  Float_t max_bin_value = 0;

  TSpectrum * s = new TSpectrum();
  //  printf("   SRSHistoManager::DoubleGaussianFit() After TSpectrum \n");
  
  //---------TSpectrum Peak Finding Parameters--------
  if (ShowFit) NPeaks = s->Search(htemp, 2, "goff", 0.5); //opens a canvas (one time in a loop), even with:  s->Search(htemp, 2, "nodraw", 0.9);
  Peak = s->GetPositionX();
  PeakAmp = s->GetPositionY();

  for ( Int_t i = 0; i < NPeaks; i++)     {
    if (peak_pos_amp < PeakAmp[i]) 	{
      peak_pos_amp = PeakAmp[i];   //TSpectrum finds peak counts
      peak_pos = Peak[i];          //TSpectrum finds pos. of peak in x-axis units
      printf("   SRSHistoManager::DoubleGaussianFit(): i = %d,  peak_pos = %f, peak_pos_amp = %f \n", i, peak_pos, peak_pos_amp  );
    }
  }

  peak_pos_bin = htemp->GetXaxis()->FindBin(peak_pos);
  peak_pos_count = htemp->GetBinContent(peak_pos_bin);
  zero_value_bin = htemp->GetXaxis()->FindBin(0.0);
  Nbins = htemp->GetSize() - 2;
  zero_bin_value =  htemp->GetXaxis()->GetBinCenter(0);
  max_bin_value = htemp->GetXaxis()->GetBinCenter(Nbins);
 
  TF1 *func;
  TF1 *func1;
  TF1 *func2;
  TF1 *func3;

  Float_t Chi2;
  Int_t NDF = 1;

  Float_t RChi2 = 1; 
  Float_t peak  = 1;  

  Float_t const1     = 1;
  Float_t mean1      = 1;
  Float_t mean_err1  = 1;
  Float_t sigma1     = 1;
  Float_t sigma_err1 = 1;

  Float_t const2     = 1;
  Float_t mean2      = 1;
  Float_t mean_err2  = 1;
  Float_t sigma2     = 1;
  Float_t sigma_err2 = 1;
  
  int TS = 0;
  func = new TF1("func", "gaus");

  //Make sure that TSpectrum peak is within histo range if not, use Par initial values from Basic Histo Peak Find
  if (peak_pos >= zero_bin_value  &&  peak_pos <= max_bin_value)  {
    TS=1;
    low_limit  = peak_pos - (0.1 * abs(max_bin_value-zero_bin_value));
    high_limit = peak_pos + (0.1 * abs(max_bin_value-zero_bin_value));
    func->SetParameter(0, peak_pos_count);
    func->SetParameter(1, peak_pos);
  }

  else {
   low_limit = binMaxCnt_value - (0.1 * abs(max_bin_value-zero_bin_value)); 
   high_limit = binMaxCnt_value + (0.1 * abs(max_bin_value-zero_bin_value)); 
   func->SetParameter(0, binMaxCnt_counts);
   func->SetParameter(1, binMaxCnt_value);
  }

  //low_limit, high_limit); //  To Show fit: htemp->Fit("gaus"); //better fit?-> Fit("gaus", "MQ", "", "", ""); 
  htemp->Fit("gaus", "Q0", "", low_limit, high_limit);
  func  = htemp->GetFunction("gaus");
  Chi2 = func->GetChisquare();
  NDF = func->GetNDF();
  if (NDF != 0) RChi2 = Chi2/NDF;
  const1 = func->GetParameter(0);
  mean1 = func->GetParameter(1);
  mean_err1 = func->GetParError(1);
  sigma1 = func->GetParameter(2);
  sigma_err1 = func->GetParError(2);

  printf("\n   SRSHistoManager::DoubleGaussianFit(): ===== BEFORE  FIRST LOOP \n");

  for (int i=0; i< 2; i++) {
    htemp->Fit("gaus", "Q0", "",(mean1 - (N_sigma_range*sigma1)), (mean1 + (N_sigma_range*sigma1) )); //don't show fit
    func  = htemp->GetFunction("gaus");
    Chi2 = func->GetChisquare();
    NDF = func->GetNDF();

    if (NDF != 0) RChi2 = Chi2/NDF;
    const1     = func->GetParameter(0);
    mean1      = func->GetParameter(1);
    mean_err1  = func->GetParError(1);
    sigma1     = func->GetParameter(2);  
    sigma_err1 = func->GetParError(2);
  }

  //Amplitude
  peak =  func->GetParameter(0); 

  //background height ~ bgd_h*gaus_amp
  Float_t bgd_h = 0.25;  

  func1 = new TF1("func1", "gaus");
  func2 = new TF1("func2", "gaus");

  func3 = new TF1("fitFunction", "func1 + func2", (mean1 - 10*sigma1), (mean1 + 10*sigma1) );
  func3->SetLineColor(kBlack);

  //-----------Fit Parameter constraints:
  func3->SetParameters(const1, mean1, sigma1, const1/5, mean1, 20*sigma1); //Set Initial Valules
  func3->SetParLimits(3, 0, (0.1*peak_pos_amp) ); 

  htemp->Fit("fitFunction", "Q0"); 
  func  = htemp->GetFunction("fitFunction");
  Chi2 = func3->GetChisquare();
  NDF = func->GetNDF();

  if (NDF != 0) RChi2 = Chi2/NDF;
  const1     = func3->GetParameter(0);
  mean1      = func3->GetParameter(1);
  mean_err1  = func3->GetParError(1);
  sigma1     = func3->GetParameter(2);
  sigma_err1 = func3->GetParError(2);
  const2     = func3->GetParameter(3);
  mean2      = func3->GetParameter(4);
  mean_err2  = func3->GetParError(4);
  sigma2     = func3->GetParameter(5);
  sigma_err2 = func3->GetParError(5);
  func3->SetParNames("Const", "Mean", "Sigma", "Bkg Const", "Bkg Mean", "Bkg Sigma");

  for (Int_t j=0; j<4; j++)  {
    func3->SetParameters(const1, mean1, sigma1, const2, mean2, sigma2);
    //----------------Show or don't show fit----------------- 
    if (ShowFit) htemp->Fit("fitFunction", "R+"); 
    else htemp->Fit("fitFunction", "Q0");        
    //-------------------------------------------------------     

    func3 = htemp->GetFunction("fitFunction");
    func3->SetLineColor(kRed);
    Chi2 = func3->GetChisquare();
    NDF = func3->GetNDF();
    if (NDF != 0) RChi2 = Chi2/NDF;

    const1     = func3->GetParameter(0);
    mean1      = func3->GetParameter(1);
    mean_err1  = func3->GetParError(1);
    sigma1     = func3->GetParameter(2);
    sigma_err1 = func3->GetParError(2);
    const2     = func3->GetParameter(3);
    mean2      = func3->GetParameter(4);
    mean_err2  = func3->GetParError(4);
    sigma2     = func3->GetParameter(5);
    sigma_err2 = func3->GetParError(5);
  }

  if (abs(const1 - peak) < abs(const2 - peak)) {
    fRChi2      = RChi2;
    fMean       = mean1;
    fMeanError  = mean_err1;
    fSigma      = abs(sigma1);
    fSigmaError = sigma_err1;
  }
  else {
    fRChi2      = RChi2;
    fMean       = mean2;
    fMeanError  = mean_err2;
    fSigma      = abs(sigma2);
    fSigmaError = sigma_err2;
  }
  delete s;
  delete func; 
  delete func1;
  delete func2;
  //  delete func3; 
 }

//============================================================================================
void SRSHistoManager::DeleteListOfClusters(TList * listOfClusters) {
  listOfClusters->Clear();
  delete listOfClusters;
}


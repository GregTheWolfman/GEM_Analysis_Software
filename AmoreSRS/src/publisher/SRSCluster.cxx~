#include "SRSCluster.h"
ClassImp(SRSCluster);

//====================================================================================================================
SRSCluster::SRSCluster(Int_t clustMinSize, Int_t clustMaxSize,  Int_t clustMinADCs, Int_t padClustMinSize, Int_t padClustMaxSize,  Int_t padClustMinADCs, TString isMaximumOrTotalADCs) {
  fNbOfHitsInCluster = 0;  
  fstrip = 0;

  fNbOfPadX = 11;
  fNbOfPadY = 11;
  fPadSizeX = 9.0;
  fPadSizeY = 9.0;

  fPadXClusterSize = 0;
  fPadYClusterSize = 0;

  fPadClustPosX = 0.0;
  fPadClustPosY = 0.0;

  fPlaneSize = 102.4;
  fTrapezoidDetInnerRadius = 220.0;
  fTrapezoidDetOuterRadius = 430.0;

  fClusterSumADCs = 0; 
  fClusterPeakADCs = 0;
  fIsClusterMaxOrSumADCs = isMaximumOrTotalADCs;

  fClusterTimeBin     = 0;
  fClusterTimeBinADC  = 0;
  fClusterPeakTimeBin = 0;

  fClusterPosition     = 0;
  fClusterCentralStrip = 0;

  fPadClusterMinADCs = padClustMinADCs;
  fPadClusterMinSize = padClustMinSize;
  fPadClusterMaxSize = padClustMaxSize;

  fClusterMinADCs = clustMinADCs;
  fClusterMinSize = clustMinSize;
  fClusterMaxSize = clustMaxSize;

  fPlane        = "NOGEMX";
  fDetector     = "NOGEM";
  fReadoutBoard = "CARTESIAN";
  fDetectorType = "STANDARD";

  fNbAPVsOnPlane   = 6;
  fAPVOrientation  = 0;
  fApvIndexOnPlane = 0;

  fListOfHitsInCluster = new TList;
  fCrossTalkFlag = kFALSE;

  //  printf("=======SRSCluster\n");
}

//====================================================================================================================
SRSCluster::~SRSCluster() {
  fListOfHitsInCluster->Clear();
  fClusterTimeBinADCs.clear();
}

//============================================================================================
void SRSCluster::DeleteListOfHits(TList * listOfHits) { 
  TIter nextHit(listOfHits);
  while( SRSHit * hit = ( (SRSHit *) nextHit() ) ) delete hit ;
  listOfHits->Clear();
  delete listOfHits;
}

//============================================================================================
void SRSCluster::Timing() {
  Int_t nBins = fClusterTimeBinADCs.size();
  TH1F * timeBinHist = new TH1F("timeBinHist", "timeBinHist", nBins, 0, (nBins-1) );
  for (Int_t k = 0; k < nBins; k++) {
    timeBinHist->Fill(k,fClusterTimeBinADCs[k]);
  }
  fClusterTimeBin = timeBinHist->GetMaximumBin(); 
  delete timeBinHist;
}

//====================================================================================================================
Int_t SRSCluster::GetClusterTimeBin() {  
  for (int i = 0; i < fListOfHitsInCluster->GetSize(); i++) {	
    vector< Float_t > timeBinADCs =  ((SRSHit *) (fListOfHitsInCluster->At(i)))->GetTimeBinADCs();
    Int_t nbOfTimeBins = timeBinADCs.size();
    fClusterTimeBinADCs.resize(nbOfTimeBins);
    for (int k = 0; k < nbOfTimeBins; k++) { 
      fClusterTimeBinADCs[k] += timeBinADCs[k];
    }
    timeBinADCs.clear();
  }
  Timing();
  return fClusterTimeBin;
}

//====================================================================================================================
void SRSCluster::ClusterPosition() {  // Calculate the fposition and the total fClusterSumADCs
  //  printf("SRSCluster::ClusterPosition => clusterSize[%s] = %d \n", fPlane.Data(), fListOfHitsInCluster->GetSize());
  for (int i = 0; i < fListOfHitsInCluster->GetSize(); i++) {
    SRSHit * hit = ((SRSHit *) fListOfHitsInCluster->At(i));
    float q = hit->GetHitADCs();
    float hitposition = GetStripPosition(hit->GetStripNo());
    fClusterSumADCs += q;
    fClusterPosition += q * hitposition;
    //    if(fPlane == "TRKGEM3Y") printf("SRSCluster::ClusterPosition => clusterPosition[%s] = %f, fClusterADCs[%s] = %f\n",  fPlane.Data(), hitposition,  fPlane.Data(), hitposition);
    if (q > fClusterPeakADCs) {
      fClusterPeakTimeBin = ((SRSHit *) (fListOfHitsInCluster->At(i)))->GetSignalPeakBinNumber();
      fClusterPeakADCs = q;
    }
  }
  fClusterPosition /= fClusterSumADCs;
  //  printf("SRSCluster::ClusterPosition => clusterPosition[%s] = %f, fClusterSumADCs[%s] = %f, fClusterPeakADCs[%s] = %f\n",  fPlane.Data(), fClusterPosition,  fPlane.Data(), fClusterSumADCs, fPlane.Data(),  fClusterPeakADCs);
}

//====================================================================================================================
void SRSCluster::PadClustPosition() {  // Calculate the fposition and the total fClusterSumADCs
  Int_t clusterSize = fListOfHitsInCluster->GetSize();
  if ( ( clusterSize > fPadClusterMaxSize) || ( clusterSize < fPadClusterMinSize) ) {
    return;
  }
  if ( fListOfHitsInCluster->GetSize() == 1) {
    fClusterSumADCs  = ((SRSHit *) (fListOfHitsInCluster->At(0)))->GetHitADCs();
    fClusterPeakADCs = ((SRSHit *) (fListOfHitsInCluster->At(0)))->GetHitADCs();
    fPadClustPosX = GetCentralPadPosX();
    fPadClustPosY = GetCentralPadPosY();
    return;
  }
  else {
    Int_t currentPadX = -1;
    Int_t currentPadY = -1;
    map<Int_t, Int_t> padXClusterSizeMap;
    map<Int_t, Int_t> padYClusterSizeMap;
    for (int i = 0; i < fListOfHitsInCluster->GetSize(); i++) {
      Float_t q = ((SRSHit *) (fListOfHitsInCluster->At(i)))->GetHitADCs();
      Int_t padXNo =((SRSHit *) (fListOfHitsInCluster->At(i)))->GetPadXNo();
      Int_t padYNo =((SRSHit *) (fListOfHitsInCluster->At(i)))->GetPadYNo();
      padXClusterSizeMap[padXNo] += 1;
      padYClusterSizeMap[padYNo] += 1;
      Float_t padPosX = GetPadPosX(padXNo);
      Float_t padPosY = GetPadPosY(padYNo);
      fClusterSumADCs += q;
      fPadClustPosX   += q * padPosX;
      fPadClustPosY   += q * padPosY;
      if (q > fClusterPeakADCs) {
        fClusterPeakTimeBin = ((SRSHit *) (fListOfHitsInCluster->At(i)))->GetSignalPeakBinNumber();
        fClusterPeakADCs = q;
      }
    }
    fPadXClusterSize = padXClusterSizeMap.size();
    fPadYClusterSize = padYClusterSizeMap.size();
    fPadClustPosX /= fClusterSumADCs;
    fPadClustPosY /= fClusterSumADCs;
    padXClusterSizeMap.clear();
    padYClusterSizeMap.clear();
  }
}

//====================================================================================================================
void SRSCluster::ClusterCentralStrip() {   
  for (int i = 0; i < fListOfHitsInCluster->GetSize(); i++) {
    float p = GetStripPosition(((SRSHit *) (fListOfHitsInCluster->At(i)))->GetStripNo());
    float dp = fabs(fClusterPosition - p); 
    float dpmin = 9999.0;
    if (dp <= dpmin) {
      fClusterCentralStrip = p;
      dpmin = dp;
    }
  }
}

//====================================================================================================================
void SRSCluster::ComputeClusterPosition() {
  if(fReadoutBoard == "PADPLANE") PadClustPosition();
  else ClusterPosition();
  ClusterCentralStrip();
}

//====================================================================================================================
SRSHit * SRSCluster::GetHitWithLargestADCs() {
  SRSHit * hit;
  for (int i = 0; i < fListOfHitsInCluster->GetSize(); i++) {
    Float_t largestAdc = 0.0;
    Int_t largestAdcStripId = 0;
    Float_t adc = ((SRSHit *) (fListOfHitsInCluster->At(i)))->GetHitADCs();
    if (largestAdc < adc) {
      largestAdc = adc;
      largestAdcStripId = i;
      hit = ((SRSHit *) (fListOfHitsInCluster->At(largestAdcStripId)));
    }
  }
  return hit;
}

//============================================================================================
Float_t SRSCluster::GetStripPosition(Int_t stripNo) {
  Float_t pitch = fPlaneSize / (NCH * fNbAPVsOnPlane);
  Float_t stripPosition = 0;
  Float_t OpeningAngle = 30.1;

  if ((fReadoutBoard == "UV_ANGLE_OLD") &&  (fDetectorType == "EICPROTO1") ) {
    pitch = 2 * fTrapezoidDetOuterRadius  / (NCH * fNbAPVsOnPlane);
    stripPosition = -0.5 * (fTrapezoidDetOuterRadius - pitch) + ( (fTrapezoidDetOuterRadius / ( (NCH / 2 )* fNbAPVsOnPlane) ) * stripNo);
  }
  else if (fReadoutBoard ==  "SBS_UV_STRIPS") {
    pitch = 0.46;
    stripPosition =  - 0.5 * (fPlaneSize - pitch) + ((fPlaneSize / (NCH * fNbAPVsOnPlane)) * stripNo);
  }
  else if (fReadoutBoard == "UV_ANGLE")  {
    stripPosition = -0.5 * (fTrapezoidDetOuterRadius - pitch) + ((fTrapezoidDetOuterRadius / ( NCH * fNbAPVsOnPlane) ) * stripNo);
  }
  else if (fReadoutBoard == "FITZZAPV3") {
    stripPosition = -0.5*OpeningAngle + (stripNo*OpeningAngle)/(NCH-1);
    //std::cout << "strip number: " << stripNo << std::endl;
    //std::cout << "strip position in degrees: " << stripPosition << std::endl;
  }
  else {
    stripPosition = -0.5 * (fPlaneSize - pitch) + ((fPlaneSize / (NCH * fNbAPVsOnPlane)) * stripNo);
    //   if(fDetector == "RESHGEM") printf("   SRSCluster::GetStripPosition => det = %s, detType = %s, readout = %s, stripNo = %d, planeSize = %.1f, pitch = %.2f, nbAPVs = %d,  stripPos = %.2f \n", fDetector.Data(), fDetectorType.Data(), fReadoutBoard.Data(), stripNo, fPlaneSize, pitch, fNbAPVsOnPlane, stripPosition);
  }   

  return stripPosition;
}

//============================================================================================
Float_t SRSCluster::GetPadPosX(Int_t padXNo) {
  Float_t posX = ( (padXNo + 0.5) *  fPadSizeX) - 0.5 * (fPadSizeX * fNbOfPadX);
  //  if(fDetector=="CAPA9MM") printf("SRSCluster::GetPadPosX():=> fPadXNo[%s] = %d, posX[%s]  = %f\n", fDetector.Data(), padXNo, fDetector.Data(), posX);
  return posX;
}

//============================================================================================
Float_t SRSCluster::GetPadPosY(Int_t padYNo) {
  Float_t posY = ( (padYNo + 0.5) *  fPadSizeY) - 0.5 * (fPadSizeY * fNbOfPadY);
  // if(fDetector=="CAPA9MM") printf("SRSCluster::GetPadPosY():=>  fPadYNo[%s] = %d, posY[%s]  = %f\n\n", fDetector.Data(), padYNo, fDetector.Data(), posY);
  return posY;
}

//============================================================================================
Int_t SRSCluster::GetCentralPadNo() {
   Int_t padNo =((SRSHit *) (fListOfHitsInCluster->At(0)))->GetPadNo();
   return padNo;
}

//============================================================================================
Float_t SRSCluster::GetCentralPadPosX() {
   Int_t padXNo =((SRSHit *) (fListOfHitsInCluster->At(0)))->GetPadXNo();
   Float_t padPosX = GetPadPosX(padXNo);
   //printf("SRSCluster::GetCentralPadPosX(): => padXNo=%d, posX = %f, adc = %f \n",padXNo, padPosX, q);
   return padPosX;
}

//============================================================================================
Float_t SRSCluster::GetCentralPadPosY() {
  Int_t padYNo =((SRSHit *) (fListOfHitsInCluster->At(0)))->GetPadYNo();
  Float_t padPosY = GetPadPosY(padYNo);
  // printf("SRSCluster::GetCentralPadPosY(): => padYNo=%d, posY = %f, adc = %f \n\n",padYNo, padPosY, q);
  return padPosY;
}

//====================================================================================================================
Float_t SRSCluster::GetClusterADCs() {
  Float_t adcs = 0;
  if (fIsClusterMaxOrSumADCs == "maximumADCs") {
    adcs = fClusterPeakADCs;
  }
  else {
    adcs = fClusterSumADCs;
  }
  return adcs;
}

//====================================================================================================================
void SRSCluster::Dump() {
  cout << "*** APV Cluster dump ***" << endl;
  TObject::Dump();
  cout << endl;
}

//====================================================================================================================
void SRSCluster::AddHitToCluster(SRSHit *h) {
  fListOfHitsInCluster->AddLast(h); 
}

//====================================================================================================================
void SRSCluster::ClearArrayOfHits() {
  fListOfHitsInCluster->Clear();
}

//====================================================================================================================
void SRSCluster::printHits() {
  for (int i = 0; i < fListOfHitsInCluster->GetSize(); i++) {
    Float_t q = ((SRSHit *) (fListOfHitsInCluster->At(i)))->GetHitADCs();
    Int_t padXNo =((SRSHit *) (fListOfHitsInCluster->At(i)))->GetPadXNo();
    Int_t padYNo =((SRSHit *) (fListOfHitsInCluster->At(i)))->GetPadYNo();

    Float_t padPosX = GetPadPosX(padXNo);
    Float_t padPosY = GetPadPosY(padYNo);
    if(fDetector=="CAPA9MM")  printf("   SRSCluster::printHits => padXNo=%d, padYNo=%d, padPosX=%f, padPosY=%f ADCs=%f \n", padXNo, padYNo, padPosX, padPosX, q);
  }
}

//====================================================================================================================
int SRSCluster::Compare(const TObject *obj) const {
  int compare = (fClusterSumADCs < ((SRSCluster*)obj)->GetClusterADCs()) ? 1 : -1;
  return compare;
}

//====================================================================================================================
Bool_t SRSCluster::IsClusterSizeTooSmall() {
  Bool_t isClusterSizeTooSmall = kFALSE;
  Int_t clustMinSize = fClusterMinSize;
  if (fReadoutBoard == "PADPLANE") clustMinSize = fPadClusterMinSize;
  if (fNbOfHitsInCluster < clustMinSize) isClusterSizeTooSmall = kTRUE;
  return  isClusterSizeTooSmall;
}

//====================================================================================================================
Bool_t SRSCluster::IsClusterSizeTooBig() {
  Bool_t isClusterSizeTooBig = kFALSE;
  Int_t clustMaxSize = fClusterMaxSize;
  if (fReadoutBoard == "PADPLANE") clustMaxSize = fPadClusterMaxSize;
  if (fNbOfHitsInCluster > clustMaxSize ) isClusterSizeTooBig = kTRUE;
  return  isClusterSizeTooBig;
}

//====================================================================================================================
Bool_t SRSCluster::IsClusterADCsTooSmall() {
  Bool_t isClusterADCsTooSmall = kFALSE;
  Int_t minADCs = fClusterMinADCs;
  if (fReadoutBoard == "PADPLANE") minADCs = fPadClusterMinADCs;
  if (GetClusterADCs() < minADCs) { 
    isClusterADCsTooSmall = kTRUE; 
  }
  return  isClusterADCsTooSmall;
}

//====================================================================================================================
void  SRSCluster::SetCrossTalkFlag(Bool_t flag) {
   fCrossTalkFlag = flag;
}

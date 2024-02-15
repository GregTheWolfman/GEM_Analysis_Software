#include "SRSHit.h"
ClassImp(SRSHit);

//============================================================================================
SRSHit::SRSHit(Int_t apvId, Int_t index, Int_t nbAPVs, Int_t orient, TString det, TString readout, TString plane, TString detType, Int_t nbPadX, Int_t nbPadY, Int_t zeroSup, TString isMaxOrTotADCs,  Bool_t isHit,  Bool_t isActive, Int_t stripNo, Float_t adcs) {

  //  printf("SRSHit::SRSHit() \n") ;
  fTimeBinADCs.clear();

  fapvID   = apvId ;
  fHitADCs = 0 ;
  fStripNo = stripNo ;
  fPadNo   = stripNo ;
  fHitADCs = adcs ;

  fPadXNo   = 0 ;
  fPadYNo   = 0 ;
  fNbofPadX = nbPadX;
  fNbofPadY = nbPadY;

  fDetector     = det ;
  fPlane        = plane;
  fReadoutBoard = readout;
  fDetectorType = detType;

  fIsHitMaxOrTotalADCs = isMaxOrTotADCs ;

  fapvIndexOnPlane = index ;
  fNbAPVsOnPlane   = nbAPVs ;
  fAPVOrientation  = orient ;
  fSignalPeakBinNumber = 0 ;

  fIsHit = isHit ;
  fIsActiveChannel = isActive ;

  SetStripNo(stripNo);

  //  printf(" === SRSHit::SetStripNo(): => fDetector=%s, fReadoutBoard=%s,  fStripNo=%d, fAbsoluteStripNo=%d, fNbAPVsOnPlane=%d, fAPVOrientation=%d\n", fDetector.Data(), fReadoutBoard.Data(), fStripNo,fAbsoluteStripNo, fNbAPVsOnPlane, fAPVOrientation);


}

//============================================================================================
void SRSHit::Timing() {
  Bool_t timingStatus = kTRUE ;
  Int_t nBins = fTimeBinADCs.size() ;
  TH1F * timeBinHist = new TH1F("timeBinHist", "timeBinHist", nBins, 0, (nBins-1) ) ;
  for (Int_t k = 0; k < nBins; k++) {
    timeBinHist->Fill(k,fTimeBinADCs[k]) ;
  }
  fSignalPeakBinNumber = timeBinHist->GetMaximumBin() ; 
  delete timeBinHist ;
}

//============================================================================================
void SRSHit::SetHitADCs(Float_t zeroSupCut, Float_t charges, TString isHitMaxOrTotalADCs) {
  fIsHitMaxOrTotalADCs = isHitMaxOrTotalADCs ;

  if(zeroSupCut > 0.0 ) {
    Float_t totalADCs = 0, minADCs = 0, maxADCs = 0 ;
    Int_t nBins = fTimeBinADCs.size() ;
    for (Int_t k = 0; k < nBins; k++) {
      totalADCs += fTimeBinADCs[k] ;
     if(fTimeBinADCs[k] > maxADCs) maxADCs = fTimeBinADCs[k] ;
    }
    totalADCs /= nBins ;
    if (fIsHitMaxOrTotalADCs == "signalPeak") fHitADCs = maxADCs ;
    else                                      fHitADCs = totalADCs ;
  }
  else {
    fHitADCs = charges ;
  }
}

//============================================================================================
void SRSHit::SetStripNo(Int_t stripNo) {
  fAbsoluteStripNo = stripNo ;
  //std::cout << "fReadoutBoard: " << fReadoutBoard << std::endl; 
  //std::cout << "fDetectorType: " << fDetectorType << std::endl; 
  if ((fReadoutBoard == "UV_ANGLE_OLD") &&  (fDetectorType == "EICPROTO1") ) {
    Int_t apvIndexOnPlaneEICBOT =  0 ;
    if (fapvIndexOnPlane < 4 ) apvIndexOnPlaneEICBOT = fapvIndexOnPlane + 8 ;
    else                       apvIndexOnPlaneEICBOT = fapvIndexOnPlane - 4;

    if ((fAPVOrientation == 0) && ((fPlane.Contains("TOP"))  && (fapvIndexOnPlane > 3)) || ((fPlane.Contains("BOT"))  && ( apvIndexOnPlaneEICBOT < 8))) {
      stripNo = 63 - stripNo ;
    }

    if (fPlane.Contains("TOP")) fStripNo = stripNo + (64 * (fapvIndexOnPlane % fNbAPVsOnPlane)) ;
    else                        fStripNo = stripNo + (64 * (apvIndexOnPlaneEICBOT % fNbAPVsOnPlane)) ;
  }

  else if ( (fReadoutBoard == "FITZZAPV3") || (fReadoutBoard == "1DSTRIPS") || (fReadoutBoard == "CARTESIAN") || (fReadoutBoard == "CMSGEM") || (fReadoutBoard == "UV_ANGLE") ) {
    if(fAPVOrientation == 0) stripNo = 127 - stripNo ;
    if (fapvIndexOnPlane > fNbAPVsOnPlane) fStripNo = -1000000 ;
    else fStripNo = stripNo + (NCH * (fapvIndexOnPlane % fNbAPVsOnPlane)) ;
  }

  else if (fReadoutBoard == "SBS_UV_STRIPS") {
    if(fAPVOrientation == 0) stripNo = 127 - stripNo ;
    if (fapvIndexOnPlane > fNbAPVsOnPlane) fStripNo = -1000000 ;
    else fStripNo = stripNo + (NCH * (fapvIndexOnPlane % fNbAPVsOnPlane)) ;
  }
  
  else if (fReadoutBoard == "PADPLANE") {
    if(fAPVOrientation == 1) stripNo = 127 - stripNo ;
    fStripNo = stripNo ;
    fPadNo   = stripNo;
    Int_t period =  (Int_t) fNbofPadX / fNbAPVsOnPlane ;
    fPadXNo = (fPadNo % period) + (period * fapvIndexOnPlane);
    fPadYNo = ((Int_t) fPadNo / period); 

    // SPECIAL CASE FOR CAPA9MM
    if(fDetector=="CAPA9MM") {

      if ( stripNo <= 7 ) {
	fPadNo  = stripNo;
	fPadXNo = stripNo + 2; 
	fPadYNo = 0; 
	//        printf(" === SRSHit::SetStripNo(): => fPadNo[%d]=%d, fPadXNo[%d]=%d, fPadYNo[%d]=%d \n", stripNo, fPadNo, stripNo, fPadXNo, stripNo, fPadYNo);
      }

      else if ( (stripNo >= 8) && (stripNo <= 15) ) {
	fPadNo  = stripNo;
	fPadXNo = stripNo - 6; 
	fPadYNo = 1; 
      }

      else if ( (stripNo >=16) && (stripNo <= 111) ) {
	fPadNo  = stripNo + 8;
	fPadXNo = (fPadNo % period) + (period * fapvIndexOnPlane);
	fPadYNo = ((Int_t) (fPadNo / period)); 
      }

      else if ( (stripNo >= 112) && (stripNo <= 119) ) {
       	fPadNo  = stripNo;
	fPadXNo = stripNo - 110; 
	fPadYNo = 10; 
      }

      else {
      	fPadNo  = stripNo;
	fPadXNo = stripNo - 118; 
	fPadYNo = 11; 
      }
      //   printf(" === SRSHit::SetStripNo(): => fPadNo[%d]=%d, fPadXNo[%d]=%d, fPadYNo[%d]=%d \n", stripNo, fPadNo, stripNo, fPadXNo, stripNo, fPadYNo);
    }
    
  }

  else {
    if(fAPVOrientation == 0) stripNo = 127 - stripNo ;
    fStripNo = stripNo ;
  }
  //     if(fDetector=="TRKGEM1") printf(" === SRSHit::SetStripNo(): => fDetector=%s, fReadoutBoard=%s, stripNo=%d, fStripNo=%d, fNbAPVsOnPlane=%d, fAPVOrientation=%d\n", fDetector.Data(),fReadoutBoard.Data(),stripNo, fStripNo, fNbAPVsOnPlane, fAPVOrientation);

}

//====================================================================================================================
int SRSHit::Compare(const TObject *obj) const {
  int compare = 0;
  if (fReadoutBoard == "PADPLANE")  compare = (fHitADCs < ((SRSHit*)obj)->GetHitADCs()) ? 1 : -1;
  else                              compare = (fStripNo > ((SRSHit*)obj)->GetStripNo()) ? 1 : -1;
  return compare ;
}

//============================================================================================
SRSHit::~SRSHit(){
  fTimeBinADCs.clear() ;
}

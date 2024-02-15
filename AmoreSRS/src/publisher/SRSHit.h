#ifndef SRSHIT_H
#define SRSHIT_H
/*******************************************************************************
*  AMORE FOR SRS - SRS                                                         *
*  SRSHit                                                                      *
*  SRS Module Class                                                            *
*  Author: Kondo GNANVO 18/08/2010                                             *
*******************************************************************************/

#if !defined(__CINT__) || defined(__MAKECINT__)

//#include "SRSAPVSignalFit.h"

#include <map>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include "TList.h"
#include "TObject.h"
#include "TMath.h"
#include "TGraph.h"
#include "TVector.h"
#include "TH1.h"
#include "TF1.h"
#include <stdlib.h>
#endif

#define NCH 128
using namespace std;

class SRSHit : public TObject { 

 public:

  ~SRSHit() ;
  //  SRSHit() ;

  SRSHit(Int_t apvId, Int_t index, Int_t nbAPVs, Int_t orient, TString det, TString readout, TString plane, TString detType, Int_t nbPadX, Int_t nbPadY, Int_t zeroSup, TString isMaxOrTotADCs, Bool_t isHit, Bool_t isActive, Int_t stripNo, Float_t adcs) ;

  Bool_t IsSortable() const { return kTRUE; }

  void IsHitFlag(Bool_t hitOrNoise) { fIsHit = hitOrNoise ;}
  Bool_t IsHit() {return fIsHit ;}

  void SetChannelActiveOrNot(Bool_t isActiveChannel) {  fIsActiveChannel = isActiveChannel;}
  Bool_t IsActiveChannel() {return fIsActiveChannel ;}

  void Timing() ;

  //=== Sort hit according to the strip number
  Int_t Compare(const TObject *obj) const ;

  void AddTimeBinADCs(Float_t charges) {fTimeBinADCs.push_back(charges);}

  void SetTimeBinADCs(vector<Float_t> timebinCharges) {
    fTimeBinADCs.clear() ;
    fTimeBinADCs = timebinCharges ;
  }

  void ClearTimeBinADCs() {fTimeBinADCs.clear() ;}

  vector<Float_t> GetTimeBinADCs() { return fTimeBinADCs ;}

  Int_t GetAPVID()            {return fapvID;}
  //  void  SetAPVID(Int_t apvID) {fapvID = apvID;}
  void  SetHitADCs(Float_t sigmaLevel, Float_t charges, TString isHitMaxOrTotalADCs) ;
  Float_t GetHitADCs()   {return fHitADCs;}

  Int_t GetSignalPeakBinNumber()   {
    Timing() ;
    return fSignalPeakBinNumber;
  }

  TString GetHitMaxOrTotalADCs() { return fIsHitMaxOrTotalADCs ;}

  Int_t GetAPVIndexOnPlane() {return fapvIndexOnPlane;}
  Int_t GetNbAPVsFromPlane() {return fNbAPVsOnPlane;}
  Int_t GetAPVOrientation()  {return fAPVOrientation;}

  //  void  SetAPVIndexOnPlane(Int_t index) {fapvIndexOnPlane = index;}
  //  void  SetNbAPVsFromPlane(Int_t nb)    {fNbAPVsOnPlane = nb;}
  //  void  SetAPVOrientation(Int_t nb)     {fAPVOrientation = nb;}

  TString GetPlane() {return fPlane;}
  //  void SetPlane(TString plane) {fPlane = plane;}

  TString GetDetector() {return fDetector;}
  //  void SetDetector(TString detector) {fDetector = detector;}

  TString GetReadoutBoard() {return fReadoutBoard;}
  //  void SetReadoutBoard(TString readoutBoard) {fReadoutBoard = readoutBoard;}

  TString GetDetectorType() {return fDetectorType;}
  //  void SetDetectorType(TString detectorType) {fDetectorType = detectorType;}

  void  SetStripNo(Int_t stripNo) ;
  Int_t GetStripNo()         {return fStripNo;}
  Int_t GetAbsoluteStripNo() {return fAbsoluteStripNo;}

  //  void SetPadReadoutParameters(Int_t padNo, Int_t nbofPadX, Int_t nbofPadY);
  Int_t GetPadNo()  {return fPadNo;}
  Int_t GetPadXNo() {return fPadXNo;}
  Int_t GetPadYNo() {return fPadYNo;}

 private:

  Bool_t fIsHit, fIsActiveChannel ;
  Int_t fapvID, fStripNo, fAbsoluteStripNo, fapvIndexOnPlane, fNbAPVsOnPlane, fAPVOrientation; 
  Int_t fSignalPeakBinNumber;   
  Int_t fPadNo, fPadYNo, fPadXNo, fNbofPadX, fNbofPadY;   
  Float_t fHitADCs, fStripPosition;
  TString fPlane, fReadoutBoard, fDetectorType, fDetector, fIsHitMaxOrTotalADCs;

  vector<Float_t> fTimeBinADCs ;
  ClassDef(SRSHit,1) 
};

#endif

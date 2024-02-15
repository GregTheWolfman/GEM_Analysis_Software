#ifndef SRSCLUSTER_H
#define SRSCLUSTER_H
/*******************************************************************************
*  AMORE FOR SRS - SRS                                                         *
*  SRSCluster                                                                  *
*  SRS Module Class                                                            *
*  Author: Kondo GNANVO 18/08/2010                                             *
*******************************************************************************/

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include "SRSHit.h"

#include <TMath.h>
#include <TObject.h>
#include <TObjArray.h>
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#endif 

using namespace std;

class SRSCluster : public TObject {

 public:
  SRSCluster(Int_t minClustSize, Int_t maxClustSize,  Int_t clustMinADCs, Int_t minPadClustSize, Int_t maxPadClustSize,  Int_t padClustMinADCs, TString isMaximumOrTotalCharges);
 ~SRSCluster();

  Bool_t IsSortable() const {return kTRUE;}

  void DeleteListOfHits(TList * listOfHits);
  TList * GetArrayOfHitsInCluster() {return fListOfHitsInCluster;}

  SRSHit * GetHitFromCluster(Int_t i) {return ((SRSHit *) (fListOfHitsInCluster->At(i)));  }

  SRSHit * GetHitWithLargestADCs();

  void SetClusterMinSize(Int_t min) {fClusterMinSize = min; }
  void SetClusterMaxSize(Int_t max) {fClusterMaxSize = max; }
  void SetPadClusterMinSize(Int_t min) {fPadClusterMinSize = min; }
  void SetPadClusterMaxSize(Int_t max) {fPadClusterMaxSize = max; }

  void AddHitToCluster(SRSHit * h);
  void Timing();

  int Compare(const TObject *obj) const; 

  void SetPlane(TString planename) {fPlane = planename;}
  void SetDetector(TString detname) {fDetector = detname;}
  void SetDetectorType(TString dettype) {fDetectorType = dettype;}
  void SetReadoutBoard(TString readoutboard) {fReadoutBoard = readoutboard;}

  TString GetPlane() {return fPlane;}
  TString GetDetector() {return fDetector;}
  TString GetReadoutBoard() {return fReadoutBoard;}

  Float_t & GetClusterPosition()     {return fClusterPosition;}
  Float_t & GetClusterCentralStrip() {return fClusterCentralStrip;}
  vector< Float_t > GetClusterTimeBinADCs () {return fClusterTimeBinADCs; } 

  Float_t  GetPadClustPosX() {return fPadClustPosX;}
  Float_t  GetPadClustPosY() {return fPadClustPosY;}

  Float_t GetStripPosition(Int_t stripNo);
  Float_t GetPadPosX(Int_t padNo);
  Float_t GetPadPosY(Int_t padNo);

  Int_t GetCentralPadNo();
  Float_t GetCentralPadPosX();
  Float_t GetCentralPadPosY();

  Int_t GetClusterTimeBin() ;
  Int_t GetClusterPeakTimeBin() {return fClusterPeakTimeBin;}

  Float_t GetClusterADCs();

  Int_t GetPadXClusterSize() {return fPadXClusterSize; }
  Int_t GetPadYClusterSize() {return fPadYClusterSize; }
  Int_t GetClusterSize() {return  fNbOfHitsInCluster; }

  Int_t GetNbOfHitsInCluster() {return  fNbOfHitsInCluster; }
  void SetNbOfHitsInCluster() {fNbOfHitsInCluster = fListOfHitsInCluster->GetSize();}

  void Dump();
  void printHits();
  void ClearArrayOfHits();

  Bool_t IsClusterSizeTooBig();
  Bool_t IsClusterSizeTooSmall();
  Bool_t IsClusterADCsTooSmall();

  void SetCrossTalkFlag(Bool_t flag); 
  Bool_t IsCrossTalkCluster() { return fCrossTalkFlag;}

  void ClusterCentralStrip();
  void ClusterPosition();
  void PadClustPosition();
  void ComputeClusterPosition();

  void SetPlaneSize(Float_t planesize)    {fPlaneSize = planesize;}
  void SetNbOfAPVsOnPlane(Int_t nbOfAPVs) {fNbAPVsOnPlane = nbOfAPVs;}
  void SetAPVOrientation(Int_t apvOrient) {fAPVOrientation = apvOrient;}
  void SetApvIndexOnPlane(Int_t apvIndex) {fApvIndexOnPlane = apvIndex;}

  void SetTrapezoidDetRadius(Float_t innerRadius, Float_t outerRadius) {fTrapezoidDetInnerRadius = innerRadius; fTrapezoidDetOuterRadius = outerRadius;  }
  void SetPadReadoutParameters(Int_t nbPadX, Int_t nbPadY, Float_t sizeX, Float_t sizeY) {fNbOfPadX=nbPadX; fNbOfPadY=nbPadY, fPadSizeX=sizeX; fPadSizeY=sizeY;}

 private:

  Int_t fNbOfHitsInCluster;     // numbers of strips with hit

  TList * fListOfHitsInCluster;

  Bool_t fCrossTalkFlag;
  TString fIsClusterMaxOrSumADCs, fPlane,  fReadoutBoard, fDetectorType, fDetector;

  Float_t fPlaneSize, fTrapezoidDetInnerRadius, fTrapezoidDetOuterRadius;
  Float_t fClusterPeakADCs, fClusterTimeBinADC, fClusterSumADCs, fClusterPosition, fClusterCentralStrip, fstrip;
  Float_t fPadSizeX, fPadSizeY, fPadClustPosX, fPadClustPosY;

  Int_t fNbAPVsOnPlane, fApvIndexOnPlane, fAPVOrientation;   
  Int_t fNbOfPadX, fNbOfPadY, fPadClusterMinSize, fPadClusterMaxSize, fPadClusterMinADCs;   
  Int_t fClusterMinSize, fClusterMaxSize, fClusterMinADCs;
  Int_t fClusterPeakTimeBin, fClusterTimeBin, fStripwithHighestADC;
  Int_t fPadXClusterSize, fPadYClusterSize;

  vector< Float_t > fClusterTimeBinADCs;

  ClassDef(SRSCluster,1) 
}; 
#endif

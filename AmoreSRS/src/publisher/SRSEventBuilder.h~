#ifndef __SRSEVENTBUILDER__
#define __SRSEVENTBUILDER__
/*******************************************************************************
*  AMORE FOR SRS - SRS                                                         *
*  SRSEventBuilder                                                             *
*  SRS Module Class                                                            *
*  Author: Kondo GNANVO 18/08/2010                                             *
*******************************************************************************/

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <map>
#include <list>
#include <vector>
#include <iostream>

#include "TList.h"
#include "TObject.h"
#include "SRSMapping.h"
#include "SRSPedestal.h"
#include "SRSCluster.h"
#include "TFile.h"
#endif

#define PI 3.14159265359 

using namespace std;
class SRSEventBuilder : public TObject {

public:

  SRSEventBuilder(Int_t evtNb, TString clustMaxMult, TString clustMaxSize, TString clustMinSize, TString clustMaxADCs, TString padClustMaxMult, TString padClustMaxSize, TString padClustMinSize, TString padClustMaxADCs,  TString runType);
  ~SRSEventBuilder();
 
  template <typename M> void ClearVectorMap( M & amap );

  void AddHitInDetectorPlane(SRSHit* hit);
  void AddHit(SRSHit* hit ) {fListOfHits->Add(hit);}

  void ComputeClusters();
  SRSCluster * PadReadoutCluster() ;

  void AddAPVEvent(SRSAPVEvent * apvEvent) {fListOfAPVEvents->AddAt(apvEvent, apvEvent->GetAPVKey()); }
  TString GetRunType() {return fRunType;}

  TList * GetListOfHits() {return fListOfHits;}

  map < TString, TList * > GetHitsInDetectorPlane() {return fHitsInDetectorPlaneMap;}
  map < TString, TList * > GetClustersInDetectorPlane() {return fClustersInDetectorPlaneMap;}

  void SetTriggerList(map<TString, TString> triggerList);
  Int_t GetEventNumber() {return fEventNo;}

  Bool_t IsAGoodEvent();
  Bool_t IsAGoodClusterEvent(TString detPlaneName);
  Bool_t IsAGoodEventInDetector(TString detName);

  void CrossTalkFlag( TList *  listOfClusters );

  SRSAPVEvent * GetAPVEventFromAPVKey(Int_t apvKey) {return (SRSAPVEvent*) (fListOfAPVEvents->At(apvKey));}

  void DeleteListOfAPVEvents(TList * listOfAPVEvents);

  void DeleteListOfHits(TList * listOfHits);
  void DeleteListOfClusters(TList * listOfClusters);

  void DeleteClustersInDetectorMap( map < TString, TList *> & stringListMap);
  void DeleteHitsInDetectorMap( map < TString, TList * >  & stringListMap);

  map < Int_t, vector <Float_t > > GetDetectorCluster(TString detector);

  void SetHitMaxOrTotalADCs(TString isHitMaxOrTotalADCs) {fIsHitMaxOrTotalADCs = isHitMaxOrTotalADCs ;}
  TString GetHitMaxOrTotalADCs() {return fIsHitMaxOrTotalADCs;}

  void SetClustMaxOrTotalADCs(TString isClustMaxOrTotalADCs) { fIsClustMaxOrTotalADCs = isClustMaxOrTotalADCs ;}
  TString GetClustMaxOrTotalADCs() {return fIsClustMaxOrTotalADCs;}

  void SetEventNumber(Int_t evtNo) { fEventNo = evtNo;}

  void SetCrosstalRemovalParameters(TString isCrossTalkRemoved, TString crossTalkMaxClustSize) { 
    fIsCrossTalkRemoved    = kFALSE;
    if (isCrossTalkRemoved == "YES") fIsCrossTalkRemoved = kTRUE;
    fCrossTalkMaxClustSize = crossTalkMaxClustSize.Atoi();
  }

  void SetAPVSignalTimeBinWindow(TString signalPeakMinTimeBin, TString signalPeakMaxTimeBin) {
    fSignalPeakMinTimeBin = signalPeakMinTimeBin.Atoi();
    fSignalPeakMaxTimeBin = signalPeakMaxTimeBin.Atoi();
  }

private:

  SRSMapping * fMapping ;

  Int_t fEventNo, fNbOfAPVs, fCrossTalkMaxClustSize;
  Int_t fSignalPeakMinTimeBin, fSignalPeakMaxTimeBin;
  Int_t fClustMinADCs, fClustMinSize, fClustMaxSize, fClustMaxMult;
  Int_t fPadClustMinADCs, fPadClustMinSize, fPadClustMaxSize, fPadClustMaxMult, fNbOfPadX, fNbOfPadY;

  TString fDetector, fPlane, /* fPlaneX, fPlaneY,*/ fReadoutBoard, fDetectorType;
  TString fIsClustMaxOrTotalADCs, fIsHitMaxOrTotalADCs, fRunType ;
  Bool_t fIsGoodEvent, fIsGoodCluster, fIsGoodClusterEvent, fIsGoodEventInDetector, fIsCrossTalkRemoved;

  Float_t fDetSizeX, fDetSizeY, fPadSizeX, fPadSizeY;
  Float_t fPlaneSize, fTrapezoidLength, fTrapezoidInRadius, fTrapezoidOutRadius;

  map < TString, TString >  fTriggerList;
  map < TString, TList * >  fHitsInDetectorPlaneMap;
  map < TString, TList * >  fClustersInDetectorPlaneMap;

  TList * fListOfAPVEvents, * fListOfHits, * fListOfHitsInPlane;

  const char * fClusterPositionCorrectionRootFile;

  ClassDef(SRSEventBuilder, 1)
};
#endif

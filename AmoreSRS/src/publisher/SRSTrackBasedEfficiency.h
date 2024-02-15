#ifndef __SRSTRACKBASEDEFFICIENCY__
#define __SRSTRACKBASEDEFFICIENCY__
/*******************************************************************************
*  AMORE FOR SRS - SRS                                                         *
*  SRSTrackBasedEfficiency                                                                    *
*  SRS Module Class                                                            *
*  Author: Kondo GNANVO, Mike Staib 01/05/2011                                 *
*******************************************************************************/
#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "TObject.h"
#include "TString.h"
#include "SRSCluster.h"
#include "SRSMapping.h"
#include "SRSConfiguration.h"
#include "SRSEventBuilder.h"
#include "SRSTrackFit.h"

#include "event.h"
#include "TRandom2.h"

#endif
#define PI 3.14159265359 

using namespace std ;

class SRSTrackBasedEfficiency : public TObject {

 public:

  SRSTrackBasedEfficiency(const char * cfgname, TString offsetDir, TString amoreAgentId) ;
  ~SRSTrackBasedEfficiency();

  void DoTracking(SRSEventBuilder * eventbuilder) ;

  Bool_t IsAGoodTrack(SRSEventBuilder * eventbuilder) ;
  Bool_t IsTracker(TString detName) ;
  Bool_t IsTrigger(TString detName) ;
  Bool_t IsTrackingEnabled();

  void SetDetectorConfig(TString detName, TString triggerType, TString trackerType, Float_t z);

  void ReadCfg(const char * cfgname); 

  TString GetRunFilePrefix() { return fRunFilePrefix; }
  TString GetRunFileValue()  { return fRunFileValue; }

  map<TString, TString> GetTriggerList()  {return fTriggerList; }
  map<TString, TString> GetTrackerList()  {return fTrackerList; }
  map<TString, TString> GetDetectorList() {return fDetectorList; }

  map<TString, Float_t> GetFitParameters() { return fFitParameters ; }
  map<TString, Float_t> GetDetZPos()    {return fDetZPos; }

//============================================================================================
 private:

  SRSMapping * fMapping ;

  TString fRunFilePrefix, fRunFileValue, fOffsetFilename, fAmoreAgentID;

  map<TString, TString> fTriggerList, fDetectorList, fTrackerList;
  map<TString, Float_t> fFitParameters;
  map<TString, Float_t> fDetZPos ;

  Bool_t fIsGoodEvent,  fIsGoodTrack ;

  ClassDef(SRSTrackBasedEfficiency,1)
};

#endif


#ifndef AMORE_SRS_PUBLISHERSRSPUBLISHER_H
#define AMORE_SRS_PUBLISHERSRSPUBLISHER_H

/*******************************************************************************
*  AMORE FOR SRS - SRS                                                         *
*  SRSPublisher                                                                *
*  SRS Module Class                                                            *
*  Author: Kondo GNANVO 18/08/2010                                             *
*******************************************************************************/
#include <map>
#include <list>
#include <fstream>
#include <iostream>

#include <MonitorObject.h>
#include <PublisherModule.h>

#include <Event.h>
#include <TDATEEventParser.h>

#include "TFile.h"

#include "SRSTrack.h"
#include "SRSCommon.h"
#include "SRSMapping.h" 
#include "SRSPedestal.h"
#include "SRSOutputROOT.h"
#include "SRSRawPedestal.h"
#include "SRSHistoManager.h"
#include "SRSConfiguration.h"
#include "SRSFECEventDecoder.h"
#include "SRSFECPedestalDecoder.h"
#include "SRSTrackBasedEfficiency.h"

//class amore::core::Event;
using namespace std;

namespace amore {

  namespace SRS {

    namespace publisher {

      class SRSPublisher: public amore::publisher::PublisherModule, public amore::SRS::common::SRSCommon {
	
      public:
 
	SRSPublisher();
	~SRSPublisher();

	virtual void BookMonitorObjects();
	virtual void EndOfCycle();
	virtual void StartOfCycle();
	virtual void MonitorEvent(amore::core::Event& event);
	virtual void EndOfRun(const amore::core::Run& run);
	virtual void StartOfRun(const amore::core::Run& run);
	virtual void EndOfSession(const amore::core::Session& session){};
	virtual void StartOfSession(const amore::core::Session& session){};

	virtual std::string Version(); // the version of the module

        void ResetMonitors(void) ;
	void ObjectsToBePublished(void) ;

      	void LoadPedestalRootFile(const char * filename, Int_t nbOfAPVs) ;
      	void LoadRawPedestalRootFile(const char * filename, Int_t nbOfAPVs) ;
	void LoadAPVGainCalibrationRootFile(const char * filename, Int_t nbOfAPVs) ;

      private:

	Int_t fEvent;
	Int_t fEventType;
	Int_t fEventRunNb;
	Int_t fEquipmentSize;

	TFile * fPedRootFile ;
	TFile * fRawPedRootFile ;
	TFile * fClusterPositionCorrectionRootFile ;

	TString fRunType, fCFGFile, fCycleWait, fStartEventNumber, fEventFrequencyNumber, fAmoreAgentID, fOutputFile; 
	TString fPadZeroSupCut, fPadClusterMinSize, fPadClusterMaxSize, fPadClusterMinADCs, fPadClusterMaxMult;
	TString fZeroSupCut, fMaskedChannelCut, fClusterMinSize, fClusterMaxSize, fClusterMinADCs, fClusterMaxMult;

      	SRSTrack                * fTrack;
	SRSTrackBasedEfficiency * fTrackBasedEfficiency;

	SRSHistoManager  * fHMan;
 	SRSConfiguration * fSRSConf;
	SRSOutputROOT    * fROOT ;

	SRSPedestal      * fSavePedFromThisRun;
	SRSPedestal      * fLoadPedToUse;

	SRSRawPedestal   * fSaveRawPedFromThisRun;
	SRSRawPedestal   * fLoadRawPedToUse;

	SRSMapping       * fMapping ;

	Int_t fMinNbOfEquipments ;
	Int_t fMaxNbOfEquipments ;

	map <Int_t, Int_t > fApvNoFromApvIDMap ;
	map <Int_t, TString > fApvNameFromIDMap ;
	TH1F ** fEtaFunctionHistos ;       

	ClassDef(SRSPublisher, 1); // SRS Module Base Class
      };
    };
  };
};

#endif

// Author: Kondo GNANVO 18/08/2010
#include "SRSPublisher.h"

//#define DEBUG
ClassImp(amore::SRS::publisher::SRSPublisher)

namespace amore {
  namespace SRS {
    namespace publisher {
      using amore::publisher::Publish;

      //-------------------------------------------------------------------------------------------------
      SRSPublisher::SRSPublisher() {
      	fEvent         = 0;
	fEquipmentSize = 0;
	fStartEventNumber = "0" ;
	fEventFrequencyNumber = "1";
	fMinNbOfEquipments = 1;
	fMaxNbOfEquipments = 8;
	fEventType     = 0;
	fEventRunNb    = -1;
	fRunType       = "RAWDATA";
	fZeroSupCut    = "0";
	fPadZeroSupCut = "0";

	fMaskedChannelCut  = "0"; 
	fClusterMinSize    = "2";
	fClusterMaxSize    = "10";
	fClusterMaxMult    = "10";

	fPadClusterMinSize    = "3";
	fPadClusterMaxSize    = "20";
	fClusterMinADCs = "200";
	fPadClusterMinADCs = "200";

	fApvNoFromApvIDMap.clear();
	fApvNameFromIDMap.clear();
	fMapping = SRSMapping::GetInstance();
      }

      //-------------------------------------------------------------------------------------------------
      SRSPublisher::~SRSPublisher() { 
      }

      //------------------------------------------------------------------------------------------------- 
      void SRSPublisher::StartOfRun(const amore::core::Run & run) {
       	ResetMonitors();
	printf("SRSPublisher::StartOfRun() ==> run type = %s; run number = %d\n",fRunType.Data(), run.RunNumber());
      }

      //-------------------------------------------------------------------------------------------------
      void SRSPublisher::EndOfRun(const amore::core::Run& run) {
	printf("SRSPublisher::EndOfRun()==> Entering ========= \n");

	if (fRunType == "PEDESTAL")  {
	  fSavePedFromThisRun->ComputePedestalData();
	  fSavePedFromThisRun->Write();
	  fPedRootFile->Close();
	  printf("SRSPublisher::EndOfRun()==> Leaving ========= \n");
	}

	if (fRunType == "RAWPEDESTAL")  {
	  fSaveRawPedFromThisRun->ComputeRawPedestalData();
	  fSaveRawPedFromThisRun->Write();
	  fRawPedRootFile->Close();
	  printf("SRSPublisher::EndOfRun()==> Leaving ========= \n");
	}

        if (fRunType == "PHYSICS") {
	  fHMan->SaveAllHistos();
	  //	  if(fTrackBasedEfficiency->IsTrackingEnabled() ) fHMan->SaveTrackBasedEfficiencyROOTFile(fTrackBasedEfficiency);
	  printf("SRSPublisher::EndOfRun()==> Leaving ========= \n");
	}

        if ( (fRunType == "CALIBRATION") || (fRunType == "FINETUNING") ) {
	  fHMan->SaveCalibrationROOTFile(fTrack);
	  printf("SRSPublisher::EndOfRun()==> Leaving ========= \n");
	}

        if (fRunType == "TRACKING") {
	  fHMan->SaveTrackingROOTFile(fTrack);
	  printf("SRSPublisher::EndOfRun()==> Leaving ========= \n");
	}

        if (fRunType == "ROOTFILE") {
	  fROOT->WriteRootFile();
	  printf("SRSPublisher::EndOfRun()==> Leaving ========= \n");
	}
      }  
 
      //-------------------------------------------------------------------------------------------------
      void SRSPublisher::StartOfCycle() {
	//	printf("===> StartOfcycle()\n");
      } 

      //-------------------------------------------------------------------------------------------------
      void SRSPublisher::EndOfCycle() {
	//     	printf("===> endOfCycle()\n");
	if (fEvent >  fStartEventNumber.Atoi()) {
	  Int_t cycleWait = (fCycleWait.Atoi()) * 1000000 ;
	  usleep(cycleWait);
	}
      } 

      //-------------------------------------------------------------------------------------------------
      void SRSPublisher::BookMonitorObjects() {
	printf("=========================================== SRSPublisher::BookMonitorObjects()==> LOAD CONFIG FILE  \n");

	std::cout << "AgentName = " << gPublisher->AgentName() << std::endl;
	TString configFileNameAgent = "$AMORESRS/configFileDir/" + gPublisher->AgentName() + "amore.cfg";
	TString configFileName      = "$AMORESRS/configFileDir/amore.cfg";

	fAmoreAgentID = gPublisher->AgentName();
	fAmoreAgentID = ((TString) (gPublisher->AgentName())).Remove(0,3);
	printf(" SRSPublisher::BookMonitorObjects()==> amoreAgent Id = %s \n",fAmoreAgentID.Data());

	if (fSRSConf->FileExists(configFileNameAgent.Data())) fCFGFile = configFileNameAgent;
	else fCFGFile = configFileName;
       
	cout << __PRETTY_FUNCTION__ << "==>LOAD CONFIG FILE to be used " << fCFGFile << " : " << endl;

	//=== Initialize the configuration file
	fSRSConf = new SRSConfiguration(fCFGFile.Data());

	//=== Initialize 
	fCycleWait            = fSRSConf->GetCycleWait();
	fStartEventNumber     = fSRSConf->GetStartEventNumber();
	fEventFrequencyNumber = fSRSConf->GetEventFrequencyNumber();

	fMaskedChannelCut = fSRSConf->GetMaskedChannelCut();

	fZeroSupCut     = fSRSConf->GetZeroSupCut();
	fClusterMinSize = fSRSConf->GetClusterMinSize();
	fClusterMaxSize = fSRSConf->GetClusterMaxSize();
	fClusterMinADCs = fSRSConf->GetClusterMinADCs();
	fClusterMaxMult = fSRSConf->GetClusterMaxMultiplicity();

	fPadZeroSupCut     = fSRSConf->GetPadZeroSupCut();
	fPadClusterMinSize = fSRSConf->GetPadClusterMinSize();
	fPadClusterMaxSize = fSRSConf->GetPadClusterMaxSize();
	fPadClusterMinADCs = fSRSConf->GetPadClusterMinADCs();
	fPadClusterMaxMult = fSRSConf->GetPadClusterMaxMultiplicity();

	fOutputFile = fSRSConf->GetOutputFile() ; 
       	TString  maxOrSum = fSRSConf->GetClusterMaxOrTotalADCs() ;
	//	fOutputFile += "_zeroSup-" + fPadZeroSupCut + "_minClustS-" + fPadClusterMinSize + "_maxClustS-" + fPadClusterMaxSize + "_maxADCs-" + fPadClusterMinADCs + "_clustMult-" + fPadClusterMaxMult + "_" + maxOrSum;
	//	fOutputFile += "_zeroSup-" + fPadZeroSupCut + "_minClustS-" + fPadClusterMinSize + "_maxClustS-" + fPadClusterMaxSize + "_maxADCs-" + fPadClusterMinADCs + "_clustMult-" + fPadClusterMaxMult + "_" + maxOrSum;

        if(fStartEventNumber.Atoi() < 1 ) {
	  printf(" SRSPublisher::BookMonitorObjects()==> before fStartEventNumber = %s \n",fStartEventNumber.Data());
	  fStartEventNumber = "0" ;
	  printf(" SRSPublisher::BookMonitorObjects()==> after  fStartEventNumber = %s \n",fStartEventNumber.Data());

	}
	if(fEventFrequencyNumber.Atoi() < 1 ) {;
	  printf(" SRSPublisher::BookMonitorObjects()==> before fEventFrequency = %s \n",fEventFrequencyNumber.Data());
	  fEventFrequencyNumber = "1" ;
	  printf(" SRSPublisher::BookMonitorObjects()==> after  fEventFrequency = %s \n",fEventFrequencyNumber.Data());
	}

	if( fSRSConf->GetRunType() == "") fRunType = "RAWDATA" ;
	else                              fRunType = fSRSConf->GetRunType();
	printf("========= SRSPublisher::BookMonitorObjects() ==>  CONFIG FILE LOADED - RUNTYPE: %s\n\n", fRunType.Data()); 

	printf("========= SRSPublisher::BookMonitorObjects() ==>  LOAD MAPPING started \n"); 
	//=== Get an instance of the mapping class object and load the mapping
	fMapping->LoadDefaultMapping(fSRSConf->GetMappingFile());
	fMapping->PrintMapping();
	fMapping->SaveMapping(fSRSConf->GetSavedMappingFile());

	fApvNoFromApvIDMap = fMapping->GetAPVNoFromIDMap();
	fApvNameFromIDMap  = fMapping->GetAPVFromIDMap();
	fMaxNbOfEquipments = fMapping->GetNbOfFECs();
	printf("========= SRSPublisher::BookMonitorObjects()==> MAPPING LOADED \n\n");

	printf("========= SRSPublisher::BookMonitorObjects() ==>  INITIALISE HISTO TOOL \n"); 
	fHMan = new SRSHistoManager(fSRSConf->GetHistoCfgName(), fClusterMinSize.Atoi(), fClusterMaxSize.Atoi(), fClusterMaxMult.Atoi(), fPadClusterMinSize.Atoi(), fPadClusterMaxSize.Atoi(), fPadClusterMaxMult.Atoi());
	printf("========= SRSPublisher::BookMonitorObjects() ==>  histo tool initialized \n\n"); 

        //=== RAW DATA RUN
	if (fRunType == "RAWDATA") {
	  fHMan->SetRunParameters(fAmoreAgentID, fSRSConf->GetRunType(), fSRSConf->GetOutputDir(), fSRSConf->GetOutputFile());
	  fHMan->BookRawDataHistos();
	  printf("SRSPublisher::BookMonitorObjects()==> Book raw data Histos\n");
	} 

	//=== For the raw pedestal runs
	if (fRunType == "RAWPEDESTAL") {
	  fRawPedRootFile = new TFile(fSRSConf->GetRawPedestalFile(), "recreate");
	  fSaveRawPedFromThisRun = new SRSRawPedestal(fMapping->GetNbOfAPVs());
	  fSaveRawPedFromThisRun->SetOutputDir(fSRSConf->GetOutputDir());
	  fSaveRawPedFromThisRun->SetRunType(fSRSConf->GetRunType());
	  printf("SRSPublisher::BookMonitorObjects()==> fSaveRawPedFromThisRun() initialised\n");
	}

	//=== For pedestal runs
	if (fRunType == "PEDESTAL") {
	  LoadRawPedestalRootFile(fSRSConf->GetRawPedestalFile(), fMapping->GetNbOfAPVs());
	  fPedRootFile = new TFile(fSRSConf->GetPedestalFile(), "recreate");
	  fSavePedFromThisRun = new SRSPedestal(fMapping->GetNbOfAPVs());
	  fSavePedFromThisRun->SetOutputDir(fSRSConf->GetOutputDir());
	  fSavePedFromThisRun->SetRunType(fSRSConf->GetRunType());
	  printf("SRSPublisher::BookMonitorObjects()==>  fSavePedFromThisRun() initialised \n"); 
	}

        //===PHYSICS  RUN
	if (fRunType == "PHYSICS")  {
	  LoadPedestalRootFile(fSRSConf->GetPedestalFile(), fMapping->GetNbOfAPVs());
	  fTrackBasedEfficiency = new SRSTrackBasedEfficiency(fSRSConf->GetHistoCfgName(), fSRSConf->GetTrackingOffsetDir(), fAmoreAgentID);
	  fHMan->SetRunParameters(fAmoreAgentID, fSRSConf->GetRunType(), fSRSConf->GetOutputDir(), fOutputFile);
	  fHMan->ReadHistoCfgFile(fSRSConf->GetHistoCfgName());
	  
      	  if(fTrackBasedEfficiency->IsTrackingEnabled() ) { 
	    fHMan->BookTrackBasedEfficiencyHistos(fTrackBasedEfficiency);
	    printf("========= SRSPublisher::BookMonitorObjects()==> Tracking initialised \n\n");
	  }	 
	}

        //===ROOTFILE  RUN
	if (fRunType == "ROOTFILE" ) {
	  LoadPedestalRootFile(fSRSConf->GetPedestalFile(), fMapping->GetNbOfAPVs());
	  TString rootData  = fSRSConf->GetROOTDataType();
	  fTrack = new SRSTrack(fSRSConf->GetHistoCfgName(), fSRSConf->GetTrackingOffsetDir(), fAmoreAgentID);
	  if(rootData == "TRACKING_ONLY") fTrack->LoadAlignementParamRootFile(fSRSConf->GetOutputDir());
	  fROOT = new SRSOutputROOT(fSRSConf->GetZeroSupCut(), fSRSConf->GetROOTDataType() );
	  fROOT->SetRunParameters(fAmoreAgentID, fSRSConf->GetRunType(), fSRSConf->GetOutputDir(), fOutputFile);
	  fROOT->InitRootFile();
	  printf("================================================ SRSPublisher::BookMonitorObjects()==> ROOT output initialised \n\n");
	}

        //===CALIBRATION  RUN
	if ( (fRunType == "CALIBRATION") || (fRunType == "FINETUNING") ) {
	  LoadPedestalRootFile(fSRSConf->GetPedestalFile(), fMapping->GetNbOfAPVs());
	  fTrackBasedEfficiency = new SRSTrackBasedEfficiency(fSRSConf->GetHistoCfgName(), fSRSConf->GetTrackingOffsetDir(), fAmoreAgentID);
	  fHMan->SetRunParameters(fAmoreAgentID, fSRSConf->GetRunType(), fSRSConf->GetOutputDir(), fOutputFile);
	  fHMan->BookCalibrationHistos(fTrack);
	}

        //===TRACKING  RUN
	if (fRunType == "TRACKING") {
	  LoadPedestalRootFile(fSRSConf->GetPedestalFile(), fMapping->GetNbOfAPVs());
	  fTrack = new SRSTrack(fSRSConf->GetHistoCfgName(), fSRSConf->GetTrackingOffsetDir(), fAmoreAgentID);
	  fTrack->LoadAlignementParamRootFile(fSRSConf->GetOutputDir());
	  fHMan->SetRunParameters(fAmoreAgentID, fSRSConf->GetRunType(), fSRSConf->GetOutputDir(), fOutputFile);
	  fHMan->BookTrackingHistos(fTrack);
	  //	  fHMan->BookEvent3DDisplayNtuple(fTrack);
	}
	printf("========= SRSPublisher::BookMonitorObjects()==> RUNTYPE: %s initialised with zeroSup=%s, clustMaxS=%s, clustMinS=%s, clustMaxMult=%s,  padZeroSup=%s, padClustMaxS=%s, padClustMinS=%s, padClustMaxMult=%s\n", fRunType.Data(), fZeroSupCut.Data(), fClusterMaxSize.Data(), fClusterMinSize.Data(), fClusterMaxMult.Data(), fPadZeroSupCut.Data(), fPadClusterMaxSize.Data(), fPadClusterMinSize.Data(), fPadClusterMaxMult.Data());
	  printf("================================================ SRSPublisher::BookMonitorObjects()==> Tracking initialised \n\n");


	//=== Objects to be published
	ObjectsToBePublished();
      }

      //-------------------------------------------------------------------------------------------------
      void SRSPublisher::ResetMonitors(void) {
	printf("=========================================== SRSPublisher::ResetMonitors()==> Enter \n");
      	if(fHMan) fHMan->ResetHistos();
 	printf("=========================================== SRSPublisher::ResetMonitors()==> Exit \n");
     }

      //-------------------------------------------------------------------------------------------------
      void SRSPublisher::ObjectsToBePublished(void) {
	printf("=========================================== SRSPublisher::ObjectsToBePublished()==> Enter \n");
	//=== Objects to be published
        if ( (fRunType == "PHYSICS") || (fRunType == "RAWDATA") ) {
	  map<TString, TObject*> allObjectsToBePublished = fHMan->GetAllObjectsToBePublished();
	  Int_t size = allObjectsToBePublished.size();
	  if(size !=0) {
            map<TString, TObject*>::const_iterator objectToBePublished_itr ;
            for(objectToBePublished_itr = allObjectsToBePublished.begin(); objectToBePublished_itr != allObjectsToBePublished.end(); objectToBePublished_itr++) {
             TNamed * obj = (TNamed*) (*objectToBePublished_itr).second ;
              Publish(obj, obj->GetName());
     	      if( (fRunType != "PHYSICS") && (fHMan->GetVarType(obj->GetName()) != "RAWDATA")) continue ;
	      printf("SRSPublisher::ObjectsToBePublished()==> Publishing monitoring object %s\n", obj->GetName());
            }
          }
	  map<TString, TH1*> toBePublished = fHMan->GetEfficiencyHistosToBePublished();
	  Int_t totoSize = toBePublished.size();
	  if(totoSize !=0) {
	    map<TString, TH1*>::const_iterator toBePublished_itr ;
            for(toBePublished_itr = toBePublished.begin(); toBePublished_itr != toBePublished.end(); toBePublished_itr++) {
              TNamed * obj = (TNamed*) (*toBePublished_itr).second ;
              Publish(obj, obj->GetName());
	      printf("SRSPublisher::ObjectsToBePublished()==> Publishing monitoring object %s\n", obj->GetName());
            }
          }
          else { 
	    printf("SRSPublisher::ObjectsToBePublished()==> No Efficiency object to be published\n");
	  }
	}

	if ( (fRunType == "CALIBRATION") || (fRunType == "FINETUNING") ) {
	  map<TString, TH1*> toBePublished = fHMan->GetCalibrationHistosToBePublished();
	  Int_t size = toBePublished.size();
	  if(size !=0) {
	    map<TString, TH1*>::const_iterator toBePublished_itr ;
            for(toBePublished_itr = toBePublished.begin(); toBePublished_itr != toBePublished.end(); toBePublished_itr++) {
              TNamed * obj = (TNamed*) (*toBePublished_itr).second ;
              Publish(obj, obj->GetName());
	      printf("SRSPublisher::ObjectsToBePublished()==> Publishing monitoring object %s\n", obj->GetName());
            }
          }
          else printf("SRSPublisher::ObjectsToBePublished()==> No object to be published\n"); 
	}

	if (fRunType == "TRACKING") {
	  map<TString, TH1*> toBePublished = fHMan->GetResidualHistosToBePublished();
	  Int_t size = toBePublished.size();
	  if(size !=0) {
	    map<TString, TH1*>::const_iterator toBePublished_itr ;
            for(toBePublished_itr = toBePublished.begin(); toBePublished_itr != toBePublished.end(); toBePublished_itr++) {
              TNamed * obj = (TNamed*) (*toBePublished_itr).second ;
              Publish(obj, obj->GetName());
	      printf("SRSPublisher::ObjectsToBePublished()==> Publishing monitoring object %s\n", obj->GetName());
            }
          }
	  /**
	  if(fHMan->GetNtupleToBePublished()) {
	    TNamed * obj = (TNamed*) fHMan->GetNtupleToBePublished();
	    Publish(obj, obj->GetName());
	    printf("SRSPublisher::ObjectsToBePublished()==> Publishing monitoring object %s\n", obj->GetName());
	  }
	  */
          else printf("SRSPublisher::ObjectsToBePublished()==> No object to be published\n"); 
	}


	printf("=========================================== SRSPublisher::ObjectsToBePublished()==> Exit \n");
      }

      //-------------------------------------------------------------------------------------------------
      void SRSPublisher::MonitorEvent(amore::core::Event& event) {

	//	if( (event.DATEEvent()->eventSize != 72) && (event.DATEEvent()->eventSize != 68) && (event.DATEEvent()->eventSize != 80) && (event.DATEEvent()->eventSize == 76744) ) { // eventsize == 72 is an End of Run event
	//	if( (event.DATEEvent()->eventSize != 72) && (event.DATEEvent()->eventSize != 68) && (event.DATEEvent()->eventSize != 80) && (event.DATEEvent()->eventSize == 32296) ) { // eventsize == 72 is an End of Run event
	if( (event.DATEEvent()->eventSize != 72) && (event.DATEEvent()->eventSize != 68) && (event.DATEEvent()->eventSize != 80)) { // eventsize == 72 is an End of Run event
	  fEvent++ ;
	  //printf("SRSPublisher::MonitorEvent() ==> Enter %s\n", fRunType.Data());

      	  if (fEvent >  fStartEventNumber.Atoi() ) {
	    if ( (fEvent % fEventFrequencyNumber.Atoi()) == 0 ) {
	      UInt_t * buffer = 0;
	      TDATEEventParser * parser = event.DATEEventParser();
	      parser->Parse();

	      eventLdcIdType * firstLdcId = const_cast<eventLdcIdType*>(parser->FirstLDC());
	      eventLdcIdType * lastLdcId  = const_cast<eventLdcIdType*>(parser->LastLDC());

	      SRSEventBuilder * eventBuilder = new SRSEventBuilder(fEvent, fClusterMaxMult, fClusterMaxSize, fClusterMinSize, fClusterMinADCs, fPadClusterMaxMult, fPadClusterMaxSize, fPadClusterMinSize, fPadClusterMinADCs, fRunType);
	      eventBuilder->SetHitMaxOrTotalADCs(fSRSConf->GetHitMaxOrTotalADCs());
	      eventBuilder->SetClustMaxOrTotalADCs(fSRSConf->GetClusterMaxOrTotalADCs());
	      eventBuilder->SetAPVSignalTimeBinWindow(fSRSConf->GetSignalPeakMinTimeBin(), fSRSConf->GetSignalPeakMinTimeBin());
	      eventBuilder->SetCrosstalRemovalParameters(fSRSConf->GetCrossTalkRemovalFlag(), fSRSConf->GetCrossTalkMaxClustSize());

	      for (eventLdcIdType * ldcId = firstLdcId; ldcId < lastLdcId; ldcId++) {
		const eventHeaderStruct * ldcHeader = parser->GetEventByLDCId(*ldcId);
		if (ldcHeader == 0) {
		  std::cerr << "invalid LDC" << std::endl ;
		  continue;
		}

		const equipmentIdType * firstEqId = parser->FirstEquipment(*ldcId);
		const equipmentIdType * lastEqId  = parser->LastEquipment(*ldcId);
	
		for (const equipmentIdType * eqId = firstEqId; eqId < lastEqId; eqId++) {
		  const equipmentHeaderStruct * const equipmentHeader = parser->GetEquipmentById(*eqId);
		  if (equipmentHeader) { 
		    buffer = (UInt_t*)equipmentHeader ;
		    fEquipmentSize = equipmentHeader->equipmentSize ;

		    if ((fRunType == "PEDESTAL") || (fRunType == "RAWPEDESTAL")) {
		      SRSFECPedestalDecoder * pedestalDecoder = new SRSFECPedestalDecoder(fEquipmentSize, buffer);
		      if (fRunType == "PEDESTAL")    fSavePedFromThisRun->FillPedestalHistos(pedestalDecoder, fLoadRawPedToUse);
		      if (fRunType == "RAWPEDESTAL") fSaveRawPedFromThisRun->FillRawPedestalHistos(pedestalDecoder);
		      delete pedestalDecoder ;
		    }
		    else {
		      Float_t sigmaCut    = ((TString) fSRSConf->GetZeroSupCut()).Atof();
		      Float_t padSigmaCut = ((TString) fSRSConf->GetPadZeroSupCut()).Atof();
		      if (fRunType == "RAWDATA") {
			SRSFECEventDecoder * fecDecoder = new SRSFECEventDecoder(fEquipmentSize,buffer,eventBuilder);
			delete fecDecoder ;
		      }
		      else {
			SRSFECEventDecoder * fecDecoder = new SRSFECEventDecoder(fEquipmentSize,buffer,fLoadPedToUse,eventBuilder,sigmaCut, padSigmaCut);
			delete fecDecoder ;
		      }
		    }
		  }
		}

		// RAWDATA RUN
		if (fRunType == "RAWDATA") {
		  fHMan->RefreshHistos();
		  fHMan->RawDataRun(eventBuilder);
		}

		// PHYSICS RUN
		if(fRunType == "PHYSICS")  {
		  fHMan->SetEventNumber(fEvent);
		  if (fZeroSupCut.Atof() == 0) {
		    fHMan->RefreshHistos();
		    fHMan->PedSubAnalysis(eventBuilder, ldcHeader);
		  }
		  else {
		    fHMan->RefreshHistos();
		    //		    eventBuilder->SetTriggerList(fTrackBasedEfficiency->GetTriggerList());
		    // fHMan->PedSubAnalysis(eventBuilder, ldcHeader);
		    //		    eventBuilder->ComputeClusters();
		    fHMan->PedSubAnalysis(eventBuilder, ldcHeader);
		    fHMan->ZeroSupAnalysis(fTrackBasedEfficiency, eventBuilder, ldcHeader) ;
		    /**		    	    
		    if ( fTrackBasedEfficiency->IsTrackingEnabled() && fTrackBasedEfficiency->IsAGoodTrack(eventBuilder) ) {
		      fHMan->TrackBasedEfficiencyRun(fTrackBasedEfficiency, eventBuilder);
		    }
		    */
		  }
	         }           

		// ROOT FILE RUN
		if (fRunType == "ROOTFILE") {
		  if (!eventBuilder->IsAGoodEvent()) continue ;
		  if ( fTrack->IsTrackingEnabled() ) {
		    eventBuilder->SetTriggerList(fTrack->GetTriggerList());
		    fROOT->FillRootFile(eventBuilder, fTrack);
		  }		
		  else {
		    fROOT->FillRootFile(eventBuilder);
		  }
		}		

		// TRACKING
		if (fRunType == "TRACKING") {
		  if (fZeroSupCut.Atof() == 0.0) {
		    printf("SRSPublisher::MonitorEvent No Zero Suppression Applied  \n");
		    continue ;
		  }
		  fHMan->RefreshHistos();
	       	  eventBuilder->SetTriggerList(fTrack->GetTriggerList());
		  eventBuilder->ComputeClusters();
		  fHMan->SetEventNumber(fEvent);
		  fHMan->TrackingRun(fTrack, eventBuilder);
		  //	fHMan->Event3DDisplay(fTrack, eventBuilder);
		}

		//CALIBRATION  RUN
		if ( (fRunType == "CALIBRATION") || (fRunType == "FINETUNING") ) {
		  if (fZeroSupCut.Atof() == 0.0) continue ;
		  if (!eventBuilder->IsAGoodEvent()) continue ;
		  fHMan->RefreshHistos();
		  eventBuilder->ComputeClusters();
		  fHMan->SetEventNumber(fEvent);
		  fHMan->CalibrationRun(fTrack, eventBuilder, ldcHeader);
		}
	      }
	      // Delete the event decoder and event builder object class
	      delete eventBuilder ; 
	    }
	  }
	}
      }

      //------------------------------------------------------------------------------------------------- 
      string SRSPublisher::Version()  {
	return "$MAKEFILE_VERSION";
      }

      //-------------------------------------------------------------------------------------------------
      void SRSPublisher::LoadPedestalRootFile(const char * filename, Int_t nbOfAPVs) {
	printf("SRSPublisher::LoadPedestalRootFile() ==> load pedestal root file %s \n",filename);
	fLoadPedToUse = SRSPedestal::GetPedestalRootFile(filename);
	if(fLoadPedToUse) {
	  fLoadPedToUse->Init();
	  fLoadPedToUse->LoadPedestalData(filename); 
	  printf("SRSPublisher::LoadPedestalRootFile() ==> Pedestal loaded \n")  ;
	} 
	else { 	
	  printf("SRSPublisher::LoadPedestalRootFile() ==> cannot load pedestal root file %s \n",filename); 
	}
      }

      //-------------------------------------------------------------------------------------------------
      void SRSPublisher::LoadRawPedestalRootFile(const char * filename, Int_t nbOfAPVs) {
	printf("SRSPublisher::LoadRawPedestalRootFile() ==> load pedestal root file %s \n",filename);
	fLoadRawPedToUse = SRSRawPedestal::GetRawPedestalRootFile(filename);
	if(fLoadRawPedToUse) {
	  fLoadRawPedToUse->Init(nbOfAPVs);
	  fLoadRawPedToUse->LoadRawPedestalData(filename); 
	  printf("SRSPublisher::LoadRawPedestalRootFile() ==> raw pedestal loaded \n")  ;
	}
	else { 	
	  printf("SRSPublisher::LoadPedestalRootFile() ==> cannot load raw pedestal root file %s \n",filename); 
	}
      }

    }// SRSPublisher 
  }
}

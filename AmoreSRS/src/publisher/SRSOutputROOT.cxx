#include "SRSOutputROOT.h"
ClassImp(SRSOutputROOT);

SRSOutputROOT::SRSOutputROOT(TString zeroSupCutStr, TString rootDataType) {
  printf(" ===>>>  SRSOutputROOT::SRSOutputROOT() \n");
  fOutputDir = "toto";
  fRunType = "PHYSICS";
  fAmoreAgentID = "0";
  fZeroSupCut = zeroSupCutStr.Atoi();
  fROOTDataType = rootDataType;
}

//====================================================================================================================
SRSOutputROOT::~SRSOutputROOT() {
  fFile->Close();
  if (fZeroSupCut == 0) {
    DeleteHitsTree();
   }

  else {
    if (fROOTDataType == "HITS_ONLY") {
      DeleteHitsTree();
    }
    else if (fROOTDataType == "CLUSTERS_ONLY") {
      DeleteClustersTree();
    } 
    else if (fROOTDataType == "TRACKING_ONLY") {
      DeleteTrackingTree();
    } 
    else {
      DeleteHitsTree(); 	
      DeleteClustersTree(); 
    }
  }

}

//====================================================================================================================
void SRSOutputROOT::DeleteHitsTree() {
  if (m_adc0)  delete[] m_adc0;
  if (m_adc1)  delete[] m_adc1;
  if (m_adc2)  delete[] m_adc2;
  if (m_adc3)  delete[] m_adc3;
  if (m_adc4)  delete[] m_adc4;
  if (m_adc5)  delete[] m_adc5;
  if (m_adc6)  delete[] m_adc6;
  if (m_adc7)  delete[] m_adc7;
  if (m_adc8)  delete[] m_adc8;
  if (m_adc9)  delete[] m_adc9;
  if (m_adc10)  delete[] m_adc10;
  if (m_adc11)  delete[] m_adc11;
  if (m_adc12)  delete[] m_adc12;
  if (m_adc13)  delete[] m_adc13;
  if (m_adc14)  delete[] m_adc14;
  if (m_adc15)  delete[] m_adc15;
  if (m_adc16)  delete[] m_adc16;
  if (m_adc17)  delete[] m_adc17;
  if (m_adc18)  delete[] m_adc18;
  if (m_adc19)  delete[] m_adc19;
  if (m_adc20)  delete[] m_adc20;
  if (m_adc21)  delete[] m_adc21;
  if (m_adc22)  delete[] m_adc22;
  if (m_adc23)  delete[] m_adc23;
  if (m_adc24)  delete[] m_adc24;
  if (m_adc25)  delete[] m_adc25;
  if (m_adc26)  delete[] m_adc26;
  if (m_adc27)  delete[] m_adc27;
  if (m_adc28)  delete[] m_adc28;
  if (m_adc29)  delete[] m_adc29;

  if (m_hit_timeBin)   delete[] m_hit_timeBin;
  if (m_strip)         delete[] m_strip;
  if (m_hit_planeID)   delete[] m_hit_planeID;
  if (m_hit_detID)     delete[] m_hit_detID;
  if (m_hit_etaSector) delete[] m_hit_etaSector;
  if (fHitTree)        delete fHitTree;
}

//====================================================================================================================
void SRSOutputROOT::DeleteClustersTree() {
  if(m_clustTimeBin) delete[] m_clustTimeBin;
  if(m_clustPosX)    delete[] m_clustPosX;
  if(m_clustPosY)    delete[] m_clustPosY;
  if(m_clustSize)    delete[] m_clustSize;
  if(m_clustADCs)    delete[] m_clustADCs;
  if(m_planeID)      delete[] m_planeID;
  if(m_detID)        delete[] m_detID;
  if(m_etaSector)    delete[] m_etaSector;
  if(fClusterTree)   delete fClusterTree;
}

//====================================================================================================================
void SRSOutputROOT::DeleteTrackingTree() {

  if(m_rawData1) delete[] m_rawData1;
  if(m_rawData2) delete[] m_rawData2;
  if(m_rawData3) delete[] m_rawData3;
  if(m_rawData4) delete[] m_rawData4;
  //  if(m_rawData5) delete[] m_rawData5;

  if(m_fitData1) delete[] m_fitData1;
  if(m_fitData2) delete[] m_fitData2;
  if(m_fitData3) delete[] m_fitData3;
  if(m_fitData4) delete[] m_fitData4;
  //  if(m_fitData5) delete[] m_fitData5;
  if(fTrackingTree) delete fTrackingTree;
}

//====================================================================================================================
void SRSOutputROOT::FillRootFile(SRSEventBuilder * eventbuilder) {
  //  printf("  SRSOutputROOT::FillRootFile() ==> Enter \n");
  m_evtID++;
   
  if (eventbuilder->IsAGoodEvent()) {
    if (fZeroSupCut == 0) {
      FillHitsTree(eventbuilder);
    }
    else {
	//std::cout << fROOTDataType << std::endl;
      if (fROOTDataType == "HITS_ONLY") {
	FillHitsTree(eventbuilder);
      }
      else if (fROOTDataType == "CLUSTERS_ONLY") {
	//printf("Clustersonly?\n");
	FillClustersTree(eventbuilder);
      }
      else {
        //std::cout << m_evtID << std::endl;
	FillHitsAndClustersTree(eventbuilder);
      }
    }
  }
}

//====================================================================================================================
void SRSOutputROOT::FillRootFile(SRSEventBuilder * eventbuilder, SRSTrack * track) {
  //   printf("  SRSOutputROOT::FillRootFile() ==> Enter \n");
  m_evtID++;

  if (eventbuilder->IsAGoodEvent()) {
    if (fZeroSupCut == 0) {
      FillHitsTree(eventbuilder);
    }
    else  if (fROOTDataType == "TRACKING_ONLY") {
	FillTrackingTree(eventbuilder, track);
      }
    else {
      //std::cout << m_evtID << std::endl;
      FillHitsAndClustersTree(eventbuilder);
    }
  }
}

//============================================================================================
static Bool_t CompareStripNo( TObject *obj1, TObject *obj2) {
  Bool_t compare;
  if ( ( (SRSHit*) obj1 )->GetStripNo() < ( ( SRSHit*) obj2 )->GetStripNo() ) compare = kTRUE;
  else compare = kFALSE;
  return compare;
}

//====================================================================================================================
void SRSOutputROOT::FillHitsTree(SRSEventBuilder * eventbuilder) {
  //  printf("  SRSOutputROOT::FillHitsTree() ==> Enter \n");
  m_chID = 0;

  SRSMapping * mapping = SRSMapping::GetInstance();

  Int_t NbADCTimeFrames = 40;
  vector<Float_t > timebinCharges;
  timebinCharges.resize(NbADCTimeFrames,0);

  map < TString, TList * >  hitsInDetectorPlaneMap = eventbuilder->GetHitsInDetectorPlane();
  map < TString, TList * >::const_iterator detPlane_itr;

  for (detPlane_itr = hitsInDetectorPlaneMap.begin(); detPlane_itr != hitsInDetectorPlaneMap.end(); detPlane_itr++) {
    TString planeName = (* detPlane_itr).first;

    // Assign Detector ID
    Int_t detID = 0;
    Int_t  planeID = 0; 
    Float_t etaSector = 0.0;

    TString detName      = mapping->GetDetectorFromPlane(planeName);
    TString readoutBoard = mapping->GetReadoutBoardFromDetector(detName);

    detID = mapping->GetDetectorIDFromDetector(detName);
    planeID = ((Int_t) mapping->GetPlaneIDorEtaSector(planeName));
    etaSector = mapping->GetPlaneIDorEtaSector(planeName);

    //  printf("  SRSOutputROOT::Fill Hits     ==> det[%s] = %s, detID[%s] = %d, planeID[%s] = %d \n", planeName.Data(), detName.Data(),  planeName.Data(), detID, planeName.Data(), planeID);


    TList * listOfHits = hitsInDetectorPlaneMap[planeName]; 
    TIter nextHit(listOfHits);
    listOfHits->Sort(CompareStripNo);
    while( SRSHit * hit = ( (SRSHit *) nextHit() ) ) {

      m_strip[m_chID]       = hit->GetStripNo();
      m_hit_timeBin[m_chID] = hit->GetSignalPeakBinNumber();

      m_hit_detID[m_chID]     = detID;
      m_hit_planeID[m_chID]   = planeID;
      m_hit_etaSector[m_chID] = (Short_t) etaSector;
      //std::cout << "etaSector: " << etaSector << std::endl;

      Int_t nbOfTimeBin = hit->GetTimeBinADCs().size();
      if (nbOfTimeBin <= NbADCTimeFrames) {
	for(Int_t t = 0; t < nbOfTimeBin; t++) {
	  timebinCharges[t] = hit->GetTimeBinADCs()[t];
	}
      }
      else {
	for(Int_t t = 0; t < NbADCTimeFrames; t++) {
	  timebinCharges[t] = hit->GetTimeBinADCs()[t];
	}
      }

      m_adc0[m_chID] = (Short_t) timebinCharges[0];
      m_adc1[m_chID] = (Short_t) timebinCharges[1];
      m_adc2[m_chID] = (Short_t) timebinCharges[2];
      m_adc3[m_chID] = (Short_t) timebinCharges[3];
      m_adc4[m_chID] = (Short_t) timebinCharges[4];
      m_adc5[m_chID] = (Short_t) timebinCharges[5];
      m_adc6[m_chID] = (Short_t) timebinCharges[6];
      m_adc7[m_chID] = (Short_t) timebinCharges[7];
      m_adc8[m_chID] = (Short_t) timebinCharges[8];
      m_adc9[m_chID] = (Short_t) timebinCharges[9];
      m_adc10[m_chID] = (Short_t) timebinCharges[10];
      m_adc11[m_chID] = (Short_t) timebinCharges[11];
      m_adc12[m_chID] = (Short_t) timebinCharges[12];
      m_adc13[m_chID] = (Short_t) timebinCharges[13];
      m_adc14[m_chID] = (Short_t) timebinCharges[14];
      m_adc15[m_chID] = (Short_t) timebinCharges[15];
      m_adc16[m_chID] = (Short_t) timebinCharges[16];
      m_adc17[m_chID] = (Short_t) timebinCharges[17];
      m_adc18[m_chID] = (Short_t) timebinCharges[18];
      m_adc19[m_chID] = (Short_t) timebinCharges[19];
      m_adc20[m_chID] = (Short_t) timebinCharges[20];
      m_adc21[m_chID] = (Short_t) timebinCharges[21];
      m_adc22[m_chID] = (Short_t) timebinCharges[22];
      m_adc23[m_chID] = (Short_t) timebinCharges[23];
      m_adc24[m_chID] = (Short_t) timebinCharges[24];
      m_adc25[m_chID] = (Short_t) timebinCharges[25];
      m_adc26[m_chID] = (Short_t) timebinCharges[26];
      m_adc27[m_chID] = (Short_t) timebinCharges[27];
      m_adc28[m_chID] = (Short_t) timebinCharges[28];
      m_adc29[m_chID] = (Short_t) timebinCharges[29];
      m_chID++;
      //std::cout << m_chID << std::endl;
      timebinCharges.clear();

    }
    listOfHits->Clear();
  }
  DeleteHitsInDetectorPlaneMap(hitsInDetectorPlaneMap);
  fHitTree->Fill();
}

//====================================================================================================================
void SRSOutputROOT::FillClustersTree(SRSEventBuilder * eventbuilder) {
  //  printf("  SRSOutputROOT::FillClustersTree() ==> \n");
  m_nclust = 0;
  eventbuilder->ComputeClusters();
  SRSMapping * mapping = SRSMapping::GetInstance();

  map < TString, TList * >  clustersInDetectorPlaneMap = eventbuilder->GetClustersInDetectorPlane();
  map < TString, TList * >::const_iterator detPlane_itr;
  for (detPlane_itr = clustersInDetectorPlaneMap.begin(); detPlane_itr != clustersInDetectorPlaneMap.end(); detPlane_itr++) {
    TString planeName = (* detPlane_itr).first;

    //Assign Detector ID
    Int_t detID = 0;
    Int_t  planeID = 0; 
    Float_t etaSector = 0.0;

    TString detName      = mapping->GetDetectorFromPlane(planeName);
    //std::cout << "DetName is: " << detName << std::endl;

    TString readoutBoard = mapping->GetReadoutBoardFromDetector(detName);
    detID = mapping->GetDetectorIDFromDetector(detName);

    planeID   = (Int_t) (mapping->GetPlaneIDorEtaSector(planeName));
    etaSector = mapping->GetPlaneIDorEtaSector(planeName);

    TList * listOfClusters = (* detPlane_itr).second;
    Int_t nbOfClusters = listOfClusters->GetSize();
    TIter nextCluster(listOfClusters);
    while( SRSCluster * cluster = ( (SRSCluster *) nextCluster() ) ) {

      m_clustTimeBin[m_nclust] = cluster->GetClusterPeakTimeBin();
      m_clustSize[m_nclust]    = cluster->GetNbOfHitsInCluster();
      if(readoutBoard =="PADPLANE") {
	m_clustPosX[m_nclust] = cluster->GetPadClustPosX();
	m_clustPosY[m_nclust] = cluster->GetPadClustPosY();
      }
      else if(readoutBoard == "FITZZAPV3"){
      //printf("Hello");
      
      }

      else {
	if (planeName.Contains("X")) m_clustPosX[m_nclust] = cluster->GetClusterPosition();
      	if (planeName.Contains("Y")) m_clustPosY[m_nclust] = cluster->GetClusterPosition();
      }
      m_clustADCs[m_nclust]    = cluster->GetClusterADCs();
      m_planeID[m_nclust]   = planeID;
      m_detID[m_nclust]     = detID;
      m_etaSector[m_nclust] = (Short_t) etaSector;
      m_nclust++;
    }
    listOfClusters->Clear();
  }
  DeleteClustersInDetectorPlaneMap(clustersInDetectorPlaneMap);
  fClusterTree->Fill();
}


//====================================================================================================================
void SRSOutputROOT::FillHitsAndClustersTree(SRSEventBuilder * eventbuilder) {
  //  printf("  SRSOutputROOT::FillHitsAndClustersTree() ==> Enter \n");
  m_chID = 0;
  //std::cout << "Here??" << std::endl;

  SRSMapping * mapping = SRSMapping::GetInstance();

  Int_t NbADCTimeFrames = 40;
  vector<Float_t > timebinCharges;
  timebinCharges.resize(NbADCTimeFrames,0);

  map < TString, TList * >  hitsInDetectorPlaneMap = eventbuilder->GetHitsInDetectorPlane();
  map < TString, TList * >::const_iterator detPlane_itr;

  for (detPlane_itr = hitsInDetectorPlaneMap.begin(); detPlane_itr != hitsInDetectorPlaneMap.end(); detPlane_itr++) {
    TString planeName = (* detPlane_itr).first;

    // Assign Detector ID
    Int_t detID = 0;
    Int_t  planeID = 0; 
    Float_t etaSector = 0.0;

    TString detName      = mapping->GetDetectorFromPlane(planeName);
    //std::cout << detName << std::endl;
    TString readoutBoard = mapping->GetReadoutBoardFromDetector(detName);

    detID = mapping->GetDetectorIDFromDetector(detName);
    planeID = ((Int_t) mapping->GetPlaneIDorEtaSector(planeName));
    etaSector = mapping->GetPlaneIDorEtaSector(planeName);

    //  printf("  SRSOutputROOT::Fill Hits     ==> det[%s] = %s, detID[%s] = %d, planeID[%s] = %d \n", planeName.Data(), detName.Data(),  planeName.Data(), detID, planeName.Data(), planeID);

    TList * listOfHits = hitsInDetectorPlaneMap[planeName]; 
    TIter nextHit(listOfHits);
    listOfHits->Sort(CompareStripNo);
    while( SRSHit * hit = ( (SRSHit *) nextHit() ) ) {
      //std::cout << "SRSHit next hit: " << *hit << std::endl;      
      m_strip[m_chID] = hit->GetStripNo();
      //std::cout << "Next Event" << std::endl;
      //std::cout << "strip: " << m_strip[m_chID] << std::endl;
      m_hit_timeBin[m_chID] = hit->GetSignalPeakBinNumber();
      //std::cout << "Max timebin is at: " << m_hit_timeBin[m_chID] << std::endl;
      m_hit_detID[m_chID]     = detID;
      //std::cout << "detID: " << m_hit_detID[m_chID] << std::endl; 
      m_hit_planeID[m_chID]   = planeID;
      //std::cout << "planeID: " << m_hit_planeID[m_chID] << std::endl;
      m_hit_etaSector[m_chID] = (Short_t) etaSector;

      Int_t nbOfTimeBin = hit->GetTimeBinADCs().size();
      if (nbOfTimeBin <= NbADCTimeFrames) {
	for(Int_t t = 0; t < nbOfTimeBin; t++) {
	  timebinCharges[t] = hit->GetTimeBinADCs()[t];
	}
      }
      else {
	for(Int_t t = 0; t < NbADCTimeFrames; t++) {
	  timebinCharges[t] = hit->GetTimeBinADCs()[t];
	}
      }

      m_adc0[m_chID] = (Short_t) timebinCharges[0];
      //std::cout << "adc0 value" << m_adc0[m_chID] << std::endl;
      m_adc1[m_chID] = (Short_t) timebinCharges[1];
      m_adc2[m_chID] = (Short_t) timebinCharges[2];
      m_adc3[m_chID] = (Short_t) timebinCharges[3];
      m_adc4[m_chID] = (Short_t) timebinCharges[4];
      m_adc5[m_chID] = (Short_t) timebinCharges[5];
      m_adc6[m_chID] = (Short_t) timebinCharges[6];
      m_adc7[m_chID] = (Short_t) timebinCharges[7];
      m_adc8[m_chID] = (Short_t) timebinCharges[8];
      m_adc9[m_chID] = (Short_t) timebinCharges[9];
      m_adc10[m_chID] = (Short_t) timebinCharges[10];
      m_adc11[m_chID] = (Short_t) timebinCharges[11];
      m_adc12[m_chID] = (Short_t) timebinCharges[12];
      m_adc13[m_chID] = (Short_t) timebinCharges[13];
      m_adc14[m_chID] = (Short_t) timebinCharges[14];
      m_adc15[m_chID] = (Short_t) timebinCharges[15];
      m_adc16[m_chID] = (Short_t) timebinCharges[16];
      m_adc17[m_chID] = (Short_t) timebinCharges[17];
      m_adc18[m_chID] = (Short_t) timebinCharges[18];
      m_adc19[m_chID] = (Short_t) timebinCharges[19];
      m_adc20[m_chID] = (Short_t) timebinCharges[20];
      m_adc21[m_chID] = (Short_t) timebinCharges[21];
      m_adc22[m_chID] = (Short_t) timebinCharges[22];
      m_adc23[m_chID] = (Short_t) timebinCharges[23];
      m_adc24[m_chID] = (Short_t) timebinCharges[24];
      m_adc25[m_chID] = (Short_t) timebinCharges[25];
      m_adc26[m_chID] = (Short_t) timebinCharges[26];
      m_adc27[m_chID] = (Short_t) timebinCharges[27];
      m_adc28[m_chID] = (Short_t) timebinCharges[28];
      m_adc29[m_chID] = (Short_t) timebinCharges[29];
      m_chID++;
      //std::cout << "m_chID " << m_chID << std::endl;
      timebinCharges.clear();

    }
    //    listOfHits->Clear();
    //for (uint i = 0; i <= m_chID;i++){
    //	std::cout << "strip " << m_strip[i] << std::endl;	
    //}
  }
 
	
		
  //std::cout << "strip type " << typeid(m_strip).name() << std::endl;
  fHitTree->Fill();

  m_nclust = 0;
  eventbuilder->ComputeClusters();

  map < TString, TList * >  clustersInDetectorPlaneMap = eventbuilder->GetClustersInDetectorPlane();
  map < TString, TList * >::const_iterator plane_itr;
  for (plane_itr = clustersInDetectorPlaneMap.begin(); plane_itr != clustersInDetectorPlaneMap.end(); plane_itr++) {
    TString planeName = (* plane_itr).first;

    //Assign Detector ID
    Int_t detID = 0;
    Int_t  planeID = 0; 
    Float_t etaSector = 0.0;

    TString detName      = mapping->GetDetectorFromPlane(planeName);
    TString readoutBoard = mapping->GetReadoutBoardFromDetector(detName);

    detID = mapping->GetDetectorIDFromDetector(detName);
    planeID   = (Int_t) (mapping->GetPlaneIDorEtaSector(planeName));
    etaSector = mapping->GetPlaneIDorEtaSector(planeName);
    //std::cout << etaSector << std::endl; 
    //if (){}

    //    printf("  SRSOutputROOT::Fill Clusters  ==> det[%s] = %s, detID[%s] = %d, planeID[%s] = %d \n", planeName.Data(), detName.Data(),  planeName.Data(), detID, planeName.Data(), planeID);

    TList * listOfClusters = (* plane_itr).second;
    Int_t nbOfClusters = listOfClusters->GetSize();
    TIter nextCluster(listOfClusters);
    while( SRSCluster * cluster = ( (SRSCluster *) nextCluster() ) ) {

      m_clustTimeBin[m_nclust] = cluster->GetClusterPeakTimeBin();
      m_clustSize[m_nclust]    = cluster->GetNbOfHitsInCluster();
      	
      if(readoutBoard =="PADPLANE") {
	m_clustPosX[m_nclust] = cluster->GetPadClustPosX();
	m_clustPosY[m_nclust] = cluster->GetPadClustPosY();
      }
      else {
	if (planeName.Contains("X")) m_clustPosX[m_nclust] = cluster->GetClusterPosition();
      	if (planeName.Contains("Y")) m_clustPosY[m_nclust] = cluster->GetClusterPosition();
      }
      m_clustADCs[m_nclust] = cluster->GetClusterADCs();
      m_planeID[m_nclust]   = planeID;
      m_detID[m_nclust]     = detID;
      m_etaSector[m_nclust] = (Short_t) etaSector;
      m_nclust++;
    }
    //    listOfClusters->Clear();
  }
  fClusterTree->Fill();

  DeleteHitsInDetectorPlaneMap(hitsInDetectorPlaneMap);
  DeleteClustersInDetectorPlaneMap(clustersInDetectorPlaneMap); 
}

//====================================================================================================================
void SRSOutputROOT::FillTrackingTree(SRSEventBuilder * eventbuilder, SRSTrack * track) {
  //  printf("  SRSOutputROOT::FillResidualHistos() ==> ENTER IN \n");

  SRSMapping * mapping = SRSMapping::GetInstance(); 
  if(!eventbuilder->IsAGoodEvent()) return ;
  if(!track->IsAGoodTrack(eventbuilder)) return ;
  map <TString, TString >         detectorList = track->GetDetectorList();

  map <TString, vector<Float_t> > rawData = track->GetRawDataSpacePoints();
  map <TString, vector<Float_t> > fitData = track->GetFittedSpacePoints();

  map <TString, TString>::const_iterator detector_itr;
  for(detector_itr =  detectorList.begin(); detector_itr !=  detectorList.end(); ++detector_itr) {

    TString detector = (*detector_itr).first;
    Int_t detID = mapping->GetDetectorIDFromDetector(detector);
    vector <Float_t> rawPoint =  rawData[detector];
    vector <Float_t> fitPoint =  fitData[detector];

    Int_t size = rawPoint.size(); 
    if(size == 0) continue;


    if(detID == 0)  {
      for(Int_t i = 0; i < 3; i++) {
	m_rawData1[i] = rawPoint[i];
	m_fitData1[i] = fitPoint[i];
      }
    }
    if(detID == 1)  {
      for(Int_t i = 0; i < 3; i++) {
	m_rawData2[i] = rawPoint[i];
	m_fitData2[i] = fitPoint[i];
      }
    }
    if(detID == 2)  {
      for(Int_t i = 0; i < 3; i++) {
	m_rawData3[i] = rawPoint[i];
	m_fitData3[i] = fitPoint[i];
      }
    }
    if(detID == 3)  {
      for(Int_t i = 0; i < 3; i++) {
	if (!rawPoint[i]) continue ;
	m_rawData4[i] = rawPoint[i];
	m_fitData4[i] = fitPoint[i];
      }
    }
  }

  fTrackingTree->Fill();
  detectorList.clear(); 

  map <TString, vector<Float_t> >::const_iterator raw_itr;
  for(raw_itr =  rawData.begin(); raw_itr !=  rawData.end(); ++raw_itr) {
    vector<Float_t> rawdata = (*raw_itr).second;
    rawdata.clear();
  }
  rawData.clear();

  map <TString, vector<Float_t> >::const_iterator fit_itr;
  for(fit_itr =  fitData.begin(); fit_itr !=  fitData.end(); ++fit_itr) {
    vector<Float_t> fitdata = (*fit_itr).second;
    fitdata.clear();
  }
  fitData.clear();
} 

//====================================================================================================================
void SRSOutputROOT::InitRootFile() { 
  printf(" ===>>>  SRSOutputROOT::InitRootFile() \n");

  TString fileName = fOutputDir + "/results/" + fOutputFile + "_dataTree" + fAmoreAgentID + ".root";
  printf("  SRSOutputROOT::InitRootFile() ==> Init Root file %s \n",fileName.Data());

  m_evtID = 0;
  m_chID = 0;
  m_nclust = 0;
  fFile = TFile::Open(fileName,"RECREATE");

  if (fZeroSupCut == 0) {
    printf("  SRSOutputROOT::InitRootFile() ==> Creating the Hit Tree: fHitTree \n");
    fHitTree = new TTree("THit","GEM Hit Data Rootfile");
    fHitTree->SetDirectory(fFile);
    
    //printf("Both in zerosup=0");
    m_strip         = new Int_t[10000];
    m_hit_detID     = new Int_t[10000];
    m_hit_planeID   = new Int_t[10000];
    m_hit_timeBin   = new Int_t[10000];
    m_hit_etaSector = new Short_t[10000];

    m_adc0  = new Short_t[10000];
    m_adc1  = new Short_t[10000];
    m_adc2  = new Short_t[10000];
    m_adc3  = new Short_t[10000];
    m_adc4  = new Short_t[10000];
    m_adc5  = new Short_t[10000];
    m_adc6  = new Short_t[10000];
    m_adc7  = new Short_t[10000];
    m_adc8  = new Short_t[10000];
    m_adc9  = new Short_t[10000];
    m_adc10 = new Short_t[10000];
    m_adc11 = new Short_t[10000];
    m_adc12 = new Short_t[10000];
    m_adc13 = new Short_t[10000];
    m_adc14 = new Short_t[10000];
    m_adc15 = new Short_t[10000];
    m_adc16 = new Short_t[10000];
    m_adc17 = new Short_t[10000];
    m_adc18 = new Short_t[10000];
    m_adc19 = new Short_t[10000];
    m_adc20 = new Short_t[10000];
    m_adc21 = new Short_t[10000];
    m_adc22 = new Short_t[10000];
    m_adc23 = new Short_t[10000];
    m_adc24 = new Short_t[10000];
    m_adc25 = new Short_t[10000];
    m_adc26 = new Short_t[10000];
    m_adc27 = new Short_t[10000];
    m_adc28 = new Short_t[10000];
    m_adc29 = new Short_t[10000];

    printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fHitTree \n");
    fHitTree->Branch("evtID",&m_evtID,"evtID/I");
    fHitTree->Branch("nch",&m_chID,"nch/I");
    fHitTree->Branch("strip", m_strip, "strip[nch]/I");
    fHitTree->Branch("hitTimebin", m_hit_timeBin, "hitTimeBin[nch]/I");

    fHitTree->Branch("adc0", m_adc0, "adc0[nch]/S");
    fHitTree->Branch("adc1", m_adc1, "adc1[nch]/S");
    fHitTree->Branch("adc2", m_adc2, "adc2[nch]/S");
    fHitTree->Branch("adc3", m_adc3, "adc3[nch]/S");
    fHitTree->Branch("adc4", m_adc4, "adc4[nch]/S");
    fHitTree->Branch("adc5", m_adc5, "adc5[nch]/S");
    fHitTree->Branch("adc6", m_adc6, "adc6[nch]/S");
    fHitTree->Branch("adc7", m_adc7, "adc7[nch]/S");
    fHitTree->Branch("adc8", m_adc8, "adc8[nch]/S");
    fHitTree->Branch("adc9", m_adc9, "adc9[nch]/S");
    fHitTree->Branch("adc10", m_adc10, "adc10[nch]/S");
    fHitTree->Branch("adc11", m_adc11, "adc11[nch]/S");
    fHitTree->Branch("adc12", m_adc12, "adc12[nch]/S");
    fHitTree->Branch("adc13", m_adc13, "adc13[nch]/S");
    fHitTree->Branch("adc14", m_adc14, "adc14[nch]/S");
    fHitTree->Branch("adc15", m_adc15, "adc15[nch]/S");
    fHitTree->Branch("adc16", m_adc16, "adc16[nch]/S");
    fHitTree->Branch("adc17", m_adc17, "adc17[nch]/S");
    fHitTree->Branch("adc18", m_adc18, "adc18[nch]/S");
    fHitTree->Branch("adc19", m_adc19, "adc19[nch]/S");
    fHitTree->Branch("adc20", m_adc20, "adc20[nch]/S");
    fHitTree->Branch("adc21", m_adc21, "adc21[nch]/S");
    fHitTree->Branch("adc22", m_adc22, "adc22[nch]/S");
    fHitTree->Branch("adc23", m_adc23, "adc23[nch]/S");
    fHitTree->Branch("adc24", m_adc24, "adc24[nch]/S");
    fHitTree->Branch("adc25", m_adc25, "adc25[nch]/S");
    fHitTree->Branch("adc26", m_adc26, "adc26[nch]/S");
    fHitTree->Branch("adc27", m_adc27, "adc27[nch]/S");
    fHitTree->Branch("adc28", m_adc28, "adc28[nch]/S");
    fHitTree->Branch("adc29", m_adc29, "adc29[nch]/S");

    fHitTree->Branch("detID", m_hit_detID, "detID[nch]/I");
    fHitTree->Branch("planeID", m_hit_planeID, "planeID[nch]/I");
    fHitTree->Branch("etaSector", m_hit_etaSector, "etaSector[nch]/S");
  }

  else {
    if (fROOTDataType == "HITS_ONLY") {
      printf("  SRSOutputROOT::InitRootFile() ==> Creating the Hit Tree: fHitTree \n");
      //printf("Both in Hits");
      fHitTree = new TTree("THit","GEM Hit Data Rootfile");
      fHitTree->SetDirectory(fFile);

      m_hit_detID     = new Int_t[2000];
      m_hit_planeID   = new Int_t[2000];
      m_hit_timeBin   = new Int_t[2000];
      m_hit_etaSector = new Short_t[2000];

      m_adc0  = new Short_t[2000];
      m_adc1  = new Short_t[2000];
      m_adc2  = new Short_t[2000];
      m_adc3  = new Short_t[2000];
      m_adc4  = new Short_t[2000];
      m_adc5  = new Short_t[2000];
      m_adc6  = new Short_t[2000];
      m_adc7  = new Short_t[2000];
      m_adc8  = new Short_t[2000];
      m_adc9  = new Short_t[2000];
      m_adc10 = new Short_t[2000];
      m_adc11 = new Short_t[2000];
      m_adc12 = new Short_t[2000];
      m_adc13 = new Short_t[2000];
      m_adc14 = new Short_t[2000];
      m_adc15 = new Short_t[2000];
      m_adc16 = new Short_t[2000];
      m_adc17 = new Short_t[2000];
      m_adc18 = new Short_t[2000];
      m_adc19 = new Short_t[2000];
      m_adc20 = new Short_t[2000];
      m_adc21 = new Short_t[2000];
      m_adc22 = new Short_t[2000];
      m_adc23 = new Short_t[2000];
      m_adc24 = new Short_t[2000];
      m_adc25 = new Short_t[2000];
      m_adc26 = new Short_t[2000];
      m_adc27 = new Short_t[2000];
      m_adc28 = new Short_t[2000];
      m_adc29 = new Short_t[2000];//std::cout << m_evtID << std::endl;
      m_strip   = new Int_t[2000];

      printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fHitTree \n");
      fHitTree->Branch("evtID",&m_evtID,"evtID/I");
      fHitTree->Branch("nch",&m_chID,"nch/I");
      fHitTree->Branch("strip", m_strip, "strip[nch]/I");
      fHitTree->Branch("hitTimebin", m_hit_timeBin, "hitTimeBin[nch]/I");

      fHitTree->Branch("adc0", m_adc0, "adc0[nch]/S");
      fHitTree->Branch("adc1", m_adc1, "adc1[nch]/S");
      fHitTree->Branch("adc2", m_adc2, "adc2[nch]/S");
      fHitTree->Branch("adc3", m_adc3, "adc3[nch]/S");
      fHitTree->Branch("adc4", m_adc4, "adc4[nch]/S");
      fHitTree->Branch("adc5", m_adc5, "adc5[nch]/S");
      fHitTree->Branch("adc6", m_adc6, "adc6[nch]/S");
      fHitTree->Branch("adc7", m_adc7, "adc7[nch]/S");
      fHitTree->Branch("adc8", m_adc8, "adc8[nch]/S");
      fHitTree->Branch("adc9", m_adc9, "adc9[nch]/S");
      fHitTree->Branch("adc10", m_adc10, "adc10[nch]/S");
      fHitTree->Branch("adc11", m_adc11, "adc11[nch]/S");
      fHitTree->Branch("adc12", m_adc12, "adc12[nch]/S");
      fHitTree->Branch("adc13", m_adc13, "adc13[nch]/S");
      fHitTree->Branch("adc14", m_adc14, "adc14[nch]/S");
      fHitTree->Branch("adc15", m_adc15, "adc15[nch]/S");
      fHitTree->Branch("adc16", m_adc16, "adc16[nch]/S");
      fHitTree->Branch("adc17", m_adc17, "adc17[nch]/S");
      fHitTree->Branch("adc18", m_adc18, "adc18[nch]/S");
      fHitTree->Branch("adc19", m_adc19, "adc19[nch]/S");
      fHitTree->Branch("adc20", m_adc20, "adc20[nch]/S");
      fHitTree->Branch("adc21", m_adc21, "adc21[nch]/S");
      fHitTree->Branch("adc22", m_adc22, "adc22[nch]/S");
      fHitTree->Branch("adc23", m_adc23, "adc23[nch]/S");
      fHitTree->Branch("adc24", m_adc24, "adc24[nch]/S");
      fHitTree->Branch("adc25", m_adc25, "adc25[nch]/S");
      fHitTree->Branch("adc26", m_adc26, "adc26[nch]/S");
      fHitTree->Branch("adc27", m_adc27, "adc27[nch]/S");
      fHitTree->Branch("adc28", m_adc28, "adc28[nch]/S");
      fHitTree->Branch("adc29", m_adc29, "adc29[nch]/S");

      fHitTree->Branch("detID", m_hit_detID, "detID[nch]/I");
      fHitTree->Branch("planeID", m_hit_planeID, "planeID[nch]/I");
      fHitTree->Branch("etaSector", m_hit_etaSector, "etaSector[nch]/S");
    }

    else if (fROOTDataType == "CLUSTERS_ONLY") {
      printf("  SRSOutputROOT::InitRootFile() ==> Creating the Cluster tree: fClusterTree \n");
      //printf("Both in Cluster \n \n \n \n ");
      //When rootdatatype is cluster only in config file, the code does enter this section
      fClusterTree = new TTree("TCluster","GEM Cluster Data Rootfile");
      fClusterTree->SetDirectory(fFile);

      m_clustSize    = new Int_t[200];
      m_clustTimeBin = new Int_t[200];
      m_clustPosX    = new Float_t[200];
      m_clustPosY    = new Float_t[200];
      m_clustADCs    = new Float_t[200];
      m_detID        = new Int_t[200];
      m_planeID      = new Int_t[200];
      m_etaSector    = new Short_t[200];

      printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fClusterTree \n");
      fClusterTree->Branch("evtID",&m_evtID,"evtID/I");
      fClusterTree->Branch("nclust",&m_nclust,"nclust/I");
      fClusterTree->Branch("clustPosX", m_clustPosX, "clustPosX[nclust]/F");
      fClusterTree->Branch("clustPosY", m_clustPosY, "clustPosY[nclust]/F");
      fClusterTree->Branch("clustSize", m_clustSize, "clustSize[nclust]/I");
      fClusterTree->Branch("clustADCs", m_clustADCs, "clustADCs[nclust]/F");
      fClusterTree->Branch("clustTimebin", m_clustTimeBin, "clustTimebin[nclust]/I");

      fClusterTree->Branch("detID", m_detID, "detID[nclust]/I");
      fClusterTree->Branch("planeID", m_planeID, "planeID[nclust]/I");
      fClusterTree->Branch("etaSector", m_etaSector, "etaSector[nclust]/S");
    }

    else if (fROOTDataType == "TRACKING_ONLY") {
      
      printf("  SRSOutputROOT::InitRootFile() ==> Creating the Tracking tree: fTrackingTree\n");
      printf("\n \n \n \n Both in tracking \n \n \n \n ");
      fTrackingTree = new TTree("TTracking","Data from FLYSUB Test Beam @ FNAL: ");
      fTrackingTree ->SetDirectory(fFile);

      printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fTrackingTree\n");

      m_rawData1 = new Float_t[3];
      m_rawData2 = new Float_t[3];
      m_rawData3 = new Float_t[3];
      m_rawData4 = new Float_t[3];
      //      m_rawData5 = new Float_t[3];

      m_fitData1 = new Float_t[3];
      m_fitData2 = new Float_t[3];
      m_fitData3 = new Float_t[3];
      m_fitData4 = new Float_t[3];
      //      m_fitData5 = new Float_t[3];


      fTrackingTree->Branch("evtID", &m_evtID, "evtID/I");
      fTrackingTree->Branch("RAW1", m_rawData1, "RAW1[3]/F");
      fTrackingTree->Branch("RAW2", m_rawData2, "RAW2[3]/F");
      fTrackingTree->Branch("RAW3", m_rawData3, "RAW3[3]/F");
      fTrackingTree->Branch("RAW4", m_rawData4, "RAW4[3]/F");
      //      fTrackingTree->Branch("RAW5", m_rawData5, "RAW5[3]/F");

      fTrackingTree->Branch("FIT1", m_fitData1, "FIT1[3]/F");
      fTrackingTree->Branch("FIT2", m_fitData2, "FIT2[3]/F");
      fTrackingTree->Branch("FIT3", m_fitData3, "FIT3[3]/F");
      fTrackingTree->Branch("FIT4", m_fitData4, "FIT4[3]/F");
      //      fTrackingTree->Branch("FIT5", m_fitData5, "FIT5[3]/F");

    }

    else {
      printf("  SRSOutputROOT::InitRootFile() ==> Creating the Hit Tree: fHitTree \n");
      //printf("\n \n \n \n Both in final else \n \n \n \n");
      fHitTree = new TTree("THit","GEM Hit Data Rootfile");
      fHitTree->SetDirectory(fFile);
       
      m_hit_detID     = new Int_t[2000];
      m_hit_planeID   = new Int_t[2000];
      m_hit_etaSector = new Short_t[2000];
      m_hit_timeBin   = new Int_t[2000];

      m_adc0  = new Short_t[2000];
      m_adc1  = new Short_t[2000];
      m_adc2  = new Short_t[2000];
      m_adc3  = new Short_t[2000];
      m_adc4  = new Short_t[2000];
      m_adc5  = new Short_t[2000];
      m_adc6  = new Short_t[2000];
      m_adc7  = new Short_t[2000];
      m_adc8  = new Short_t[2000];
      m_adc9  = new Short_t[2000];
      m_adc10 = new Short_t[2000];
      m_adc11 = new Short_t[2000];
      m_adc12 = new Short_t[2000];
      m_adc13 = new Short_t[2000];
      m_adc14 = new Short_t[2000];
      m_adc15 = new Short_t[2000];
      m_adc16 = new Short_t[2000];
      m_adc17 = new Short_t[2000];
      m_adc18 = new Short_t[2000];
      m_adc19 = new Short_t[2000];
      m_adc20 = new Short_t[2000];
      m_adc21 = new Short_t[2000];
      m_adc22 = new Short_t[2000];
      m_adc23 = new Short_t[2000];
      m_adc24 = new Short_t[2000];
      m_adc25 = new Short_t[2000];
      m_adc26 = new Short_t[2000];
      m_adc27 = new Short_t[2000];
      m_adc28 = new Short_t[2000];
      m_adc29 = new Short_t[2000];
      m_strip = new Int_t[2000];

      printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fHitTree \n");
      fHitTree->Branch("evtID",&m_evtID,"evtID/I");
      fHitTree->Branch("nch",&m_chID,"nch/I");
      fHitTree->Branch("strip", m_strip, "strip[nch]/I");
      fHitTree->Branch("hitTimebin", m_hit_timeBin, "hitTimeBin[nch]/I");

      fHitTree->Branch("adc0", m_adc0, "adc0[nch]/S");
      fHitTree->Branch("adc1", m_adc1, "adc1[nch]/S");
      fHitTree->Branch("adc2", m_adc2, "adc2[nch]/S");
      fHitTree->Branch("adc3", m_adc3, "adc3[nch]/S");
      fHitTree->Branch("adc4", m_adc4, "adc4[nch]/S");
      fHitTree->Branch("adc5", m_adc5, "adc5[nch]/S");
      fHitTree->Branch("adc6", m_adc6, "adc6[nch]/S");
      fHitTree->Branch("adc7", m_adc7, "adc7[nch]/S");
      fHitTree->Branch("adc8", m_adc8, "adc8[nch]/S");
      fHitTree->Branch("adc9", m_adc9, "adc9[nch]/S");
      fHitTree->Branch("adc10", m_adc10, "adc10[nch]/S");
      fHitTree->Branch("adc11", m_adc11, "adc11[nch]/S");
      fHitTree->Branch("adc12", m_adc12, "adc12[nch]/S");
      fHitTree->Branch("adc13", m_adc13, "adc13[nch]/S");
      fHitTree->Branch("adc14", m_adc14, "adc14[nch]/S");
      fHitTree->Branch("adc15", m_adc15, "adc15[nch]/S");
      fHitTree->Branch("adc16", m_adc16, "adc16[nch]/S");
      fHitTree->Branch("adc17", m_adc17, "adc17[nch]/S");
      fHitTree->Branch("adc18", m_adc18, "adc18[nch]/S");
      fHitTree->Branch("adc19", m_adc19, "adc19[nch]/S");
      fHitTree->Branch("adc20", m_adc20, "adc20[nch]/S");
      fHitTree->Branch("adc21", m_adc21, "adc21[nch]/S");
      fHitTree->Branch("adc22", m_adc22, "adc22[nch]/S");
      fHitTree->Branch("adc23", m_adc23, "adc23[nch]/S");
      fHitTree->Branch("adc24", m_adc24, "adc24[nch]/S");
      fHitTree->Branch("adc25", m_adc25, "adc25[nch]/S");
      fHitTree->Branch("adc26", m_adc26, "adc26[nch]/S");
      fHitTree->Branch("adc27", m_adc27, "adc27[nch]/S");
      fHitTree->Branch("adc28", m_adc28, "adc28[nch]/S");
      fHitTree->Branch("adc29", m_adc29, "adc29[nch]/S");

      fHitTree->Branch("detID", m_hit_detID, "detID[nch]/I");
      fHitTree->Branch("planeID", m_hit_planeID, "planeID[nch]/I");
      fHitTree->Branch("etaSector", m_hit_etaSector, "etaSector[nch]/S");

      printf("  SRSOutputROOT::InitRootFile() ==> Creating the Cluster Tree: fClusterTree \n");
      fClusterTree = new TTree("TCluster","GEM Cluster Data Rootfile");
      fClusterTree->SetDirectory(fFile);

      m_clustSize    = new Int_t[200];
      m_clustTimeBin = new Int_t[200];
      m_clustPosX    = new Float_t[200];
      m_clustPosY    = new Float_t[200];
      m_clustADCs    = new Float_t[200];
      m_detID        = new Int_t[200];
      m_planeID      = new Int_t[200];
      m_etaSector    = new Short_t[200];

      printf("  SRSOutputROOT::InitRootFile() ==> Initialising the branches for fClusterTree \n");
      fClusterTree->Branch("evtID",&m_evtID,"evtID/I");
      fClusterTree->Branch("nclust",&m_nclust,"nclust/I");
      fClusterTree->Branch("clustPosX", m_clustPosX, "clustPosX[nclust]/F");
      fClusterTree->Branch("clustPosY", m_clustPosY, "clustPosY[nclust]/F");
      fClusterTree->Branch("clustSize", m_clustSize, "clustSize[nclust]/I");
      fClusterTree->Branch("clustADCs", m_clustADCs, "clustADCs[nclust]/F");
      fClusterTree->Branch("clustTimebin", m_clustTimeBin, "clustTimeBin[nclust]/I");

      fClusterTree->Branch("detID", m_detID, "detID[nclust]/I");
      fClusterTree->Branch("planeID", m_planeID, "planeID[nclust]/I");
      fClusterTree->Branch("etaSector", m_etaSector, "etaSector[nclust]/S");
    }
  }
}

//============================================================================================
void SRSOutputROOT::SetRunParameters(TString amoreAgentId, TString runtype, TString outputdir, TString outputfile) {
  printf("\nSRSOutputROOT::SetRunParameters() => Enter: \n");
  fAmoreAgentID = amoreAgentId;
  fRunType = runtype;
  fOutputDir =  outputdir;
  fOutputFile = outputfile;
  printf("SRSOutputROOT::SetRunParameters() => Exit: fOutputDir=%s:\n",fOutputDir.Data());
  printf("SRSOutputROOT::SetRunParameters() => Exit: fRunType=%s, fAmoreAgentID=%s, fOutputFile=%s\n",runtype.Data(), amoreAgentId.Data(), fOutputFile.Data());
}

//====================================================================================================================
void SRSOutputROOT::WriteRootFile() {
  if (fZeroSupCut == 0) {
    fHitTree->Write();
  }

  else {
    if (fROOTDataType == "HITS_ONLY") {
    fHitTree->Write();
    }
    else if (fROOTDataType == "CLUSTERS_ONLY") {
      fClusterTree->Write(); 
    } 
    else if (fROOTDataType == "TRACKING_ONLY") {
      fTrackingTree->Write(); 
    } 
    else {
      fHitTree->Write();
      fClusterTree->Write(); 
    }
  }
  fFile->Write();
}

//============================================================================================
void SRSOutputROOT::DeleteHitsInDetectorPlaneMap( map<TString, TList * > & stringListMap) {
  map < TString, TList* >::const_iterator itr;
  for (itr = stringListMap.begin(); itr != stringListMap.end(); itr++) {
    TList * listOfHits = (*itr).second;
    TIter nextHit(listOfHits);
    while( SRSHit * hit = ( (SRSHit *) nextHit() ) ) delete hit;
    listOfHits->Clear();
  }
  stringListMap.clear();
}

//============================================================================================
void SRSOutputROOT::DeleteClustersInDetectorPlaneMap( map<TString, TList * > & stringListMap) {
  map < TString, TList* >::const_iterator itr;
  for (itr = stringListMap.begin(); itr != stringListMap.end(); itr++) {
    TList * listOfClusters = (*itr).second;
    TIter nextCluster(listOfClusters);
    while( SRSCluster * cluster = ( (SRSCluster *) nextCluster() ) ) delete cluster;
    listOfClusters->Clear();
  }
  stringListMap.clear();
}

#include "SRSTrackBasedEfficiency.h"
ClassImp(SRSTrackBasedEfficiency);

SRSTrackBasedEfficiency::SRSTrackBasedEfficiency(const char * cfgname, TString offsetDir, TString amoreAgentId) {
  printf("\nSRSTrackBasedEfficiency::SRSTrackBasedEfficiency() ==> Enter:\n");

  fMapping = SRSMapping::GetInstance();

  fIsGoodEvent = kFALSE;
  fIsGoodTrack = kFALSE;

  fRunFilePrefix = "std";
  fRunFileValue = "0";
  fAmoreAgentID = amoreAgentId;

  ReadCfg(cfgname);
  Int_t nbOfDetectors  = fDetectorList.size();
  Int_t nbOfTriggers   = fTriggerList.size();
  Int_t nbOfTrackers   = fTrackerList.size();
  //  printf("SRSTrackBasedEfficiency::SRSTrackBasedEfficiency() ==> Exit: zeroSupCut = %d, minClustSize = %d,  maxClustSize = %d, nbOfTrackers = %d \n",fZeroSupCut, fMinClusterSize, fMaxClusterSize, nbOfTrackers);
}

//=============================================================================================================
SRSTrackBasedEfficiency::~SRSTrackBasedEfficiency() {
  fDetZPos.clear();
  fFitParameters.clear();
  fTriggerList.clear();
  fTrackerList.clear();
  fDetectorList.clear();
}

//============================================================================================
void SRSTrackBasedEfficiency::ReadCfg(const char * cfgname) { 
  printf("  SRSTrackBasedEfficiency::ReadCfg() ==> Loading SRS Tracking Configuration from: %s\n", cfgname);

  fTriggerList.clear();
  fTrackerList.clear();
  fDetectorList.clear();

  ifstream file (cfgname, ifstream::in);
  Int_t iline = 0;
  TString line;

  while (line.ReadLine(file)) {
    iline++;
    line.Remove(TString::kLeading, ' ');   // strip leading spaces 
    if (line.BeginsWith("#")) continue;   // and skip comments

    TObjArray * tokens = line.Tokenize(","); // Create an array of the tokens separated by "," in the line; lines without "," are skipped
    TIter myiter(tokens);                    // iterator on the tokens array 
    while (TObjString * st = (TObjString*) myiter.Next()) {  // inner loop (loop over the line);

      TString s = st->GetString().Remove(TString::kLeading, ' ' ); // remove leading spaces
      s.Remove(TString::kTrailing, ' ' );                          // remove trailing spaces 

      if (!s.Contains("@")) continue;
    

      if (s == "@TRACKING2D") {
	TString name         = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	TString triggerType  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	TString trackerType  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	Float_t zPos = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	Int_t  xnbin = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	Float_t xmin = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	Float_t xmax = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	Int_t  ynbin = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	Float_t ymin = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	Float_t ymax = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	SetDetectorConfig(name, triggerType, trackerType, zPos);
	continue;
      }

      if (s == "@TRACKING1D") {
	TString name         = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	TString triggerType  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	TString trackerType  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	Float_t zPos = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	Int_t  xnbin = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	Float_t xmin = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	Float_t xmax = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
  	SetDetectorConfig(name, triggerType, trackerType, zPos);
	continue;
      }
    }
    tokens->Delete();
  } 
  printf("  SRSTrackBasedEfficiency::ReadCfg() ==> Exit SRS Tracking Configuration =======================================================\n");
}   

//============================================================================================
void SRSTrackBasedEfficiency::SetDetectorConfig(TString detName, TString triggerType, TString trackerType, Float_t zPos) {
  fDetZPos[detName] = zPos;
  fDetectorList[detName] = detName;
  if((trackerType == "isTracker") || (trackerType == "IsTracker")) fTrackerList[detName] = trackerType;
  if((triggerType == "isTrigger") || (triggerType == "IsTrigger")) fTriggerList[detName] = triggerType;
  printf("  SRSTrackBasedEfficiency::SetDetectorConfig():  %s ==> %s, %s, z=%f \n", detName.Data(), trackerType.Data(), triggerType.Data(), fDetZPos[detName] );
 }

//============================================================================================
Bool_t SRSTrackBasedEfficiency::IsTrackingEnabled() {
  Bool_t isTrackingEnabled = kFALSE;
  if (fTrackerList.size() != 0) isTrackingEnabled = kTRUE;
  return  isTrackingEnabled;
}

//============================================================================================
Bool_t SRSTrackBasedEfficiency::IsTracker(TString detName) {
  map < TString, TString > ::const_iterator it = fTrackerList.find(detName);
  return it != fTrackerList.end();
}

//============================================================================================
Bool_t SRSTrackBasedEfficiency::IsTrigger(TString detName) {
  map < TString, TString > ::const_iterator it = fTriggerList.find(detName);
  return it != fTriggerList.end();
}

//============================================================================================
void SRSTrackBasedEfficiency::DoTracking(SRSEventBuilder * eventbuilder) {
  //  printf("------ SRSTrackBasedEffic0iency::DoTracking() ==> Enter\n");
  if(eventbuilder->IsAGoodEvent()) {
    SRSMapping * mapping = SRSMapping::GetInstance();
    fFitParameters.clear();
    Float_t sumx=0, sumy =0,sumz = 0;
    Float_t squareSumz = 0;
    Float_t crossSumxz = 0, crossSumyz = 0;
    Int_t n = fTrackerList.size();

    map<TString, TString>::const_iterator trk_itr;
    for (trk_itr = fTrackerList.begin(); trk_itr != fTrackerList.end(); trk_itr++) {
      TString detName = (*trk_itr).first;
      TString detPlaneX = (mapping->GetDetectorPlaneListFromDetector(detName)).front();
      TString detPlaneY = (mapping->GetDetectorPlaneListFromDetector(detName)).back();
      TList * clusterListX = eventbuilder->GetClustersInDetectorPlane() [detPlaneX.Data()];
      TList * clusterListY = eventbuilder->GetClustersInDetectorPlane() [detPlaneY.Data()];
      Float_t x = ((SRSCluster *) clusterListX->At(0))->GetClusterPosition();
      Float_t y = ((SRSCluster *) clusterListY->At(0))->GetClusterPosition();
      Float_t z = fDetZPos[detName];
      //      printf("------ SRSTrackBasedEfficiency::DoTracking() ==> Good Event => nb of track = %d, det=%s, detPlaneX=%s, detPlaneY=%s\n", n, detName.Data(), detPlaneX.Data(), detPlaneY.Data());

      sumx += x;
      sumy += y;
      sumz += z;
      squareSumz += z * z;
      crossSumxz += x * z;
      crossSumyz += y * z;
    }

    fFitParameters["offsetX"]    = (sumz*crossSumxz - sumx*squareSumz) / (sumz*sumz - n*squareSumz);
    fFitParameters["offsetY"]    = (sumz*crossSumyz - sumy*squareSumz) / (sumz*sumz - n*squareSumz) ; 
    fFitParameters["directionX"] = (sumz*sumx - n*crossSumxz) / (sumz*sumz - n*squareSumz);
    fFitParameters["directionY"] = (sumz*sumy - n*crossSumyz) / (sumz*sumz - n*squareSumz);
  }
  fIsGoodTrack = kTRUE;
  //  printf("------ SRSTrackBasedEfficiency::DoTracking() ==> Exit:\n");
}

//====================================================================================================================
Bool_t  SRSTrackBasedEfficiency::IsAGoodTrack(SRSEventBuilder * eventbuilder) {
  Int_t nbOfTrackers = fTrackerList.size();
  Int_t nbOfTrackersInEvents = 0;
  SRSMapping * mapping = SRSMapping::GetInstance();

  map<TString, TString>::const_iterator trk_itr;
  for (trk_itr = fTrackerList.begin(); trk_itr != fTrackerList.end(); trk_itr++) {
    TString detName = (*trk_itr).first;
    TString detPlaneX = (mapping->GetDetectorPlaneListFromDetector(detName)).front();
    TString detPlaneY = (mapping->GetDetectorPlaneListFromDetector(detName)).back();
    TList * clusterListX = eventbuilder->GetClustersInDetectorPlane() [detPlaneX.Data()];
    TList * clusterListY = eventbuilder->GetClustersInDetectorPlane() [detPlaneY.Data()];
    Int_t xSize = clusterListX->GetSize();
    Int_t ySize = clusterListY->GetSize();
    if ( (xSize == 1) && (ySize == 1));
    nbOfTrackersInEvents++;
  }
  if  (nbOfTrackersInEvents > 1) DoTracking(eventbuilder);
  return fIsGoodTrack;
}

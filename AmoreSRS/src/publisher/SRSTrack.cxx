#include "SRSTrack.h"
ClassImp(SRSTrack);

SRSTrack::SRSTrack(const char * cfgname, TString offsetDir, TString amoreAgentId) {
  //  printf("\nSRSTrack::SRSTrack() ==> Enter:\n");

  fMapping = SRSMapping::GetInstance();

  fIsGoodEvent = kFALSE;
  fIsGoodTrack = kFALSE;

  fRunFilePrefix = "std";
  fRunFileValue = "0";
  fAmoreAgentID = amoreAgentId;

  fAngleCutMinX = -7;
  fAngleCutMinY = 2.3;
  fAngleCutMaxX = -6.7;
  fAngleCutMaxY = 2.7;

  fNtupleSizeX = 100; 
  fNtupleSizeY = 100; 
  fNtupleSizeZ = 4000;
  fNtupleTitle = "noNtuple"; 

  ReadCfg(cfgname);
  Int_t nbOfDetectors  = fDetectorList.size();
  Int_t nbOfTriggers   = fTriggerList.size();
  Int_t nbOfTrackers   = fTrackerList.size();
  //  printf("SRSTrack::SRSTrack() ==> Exit: zeroSupCut = %d, minClustSize = %d,  maxClustSize = %d nbDetectors = %d, nbOfTriggers = %d, nbOfTrackers = %d \n",fZeroSupCut, fMinClusterSize, fMaxClusterSize, nbOfDetectors, nbOfTriggers, nbOfTrackers);
  //  printf("SRSTrack::SRSTrack() ==> Exit: padZeroSupCut = %d, padMinClustSize = %d,  padMaxClustSize = %d nbDetectors = %d, nbOfTriggers = %d, nbOfTrackers = %d \n",fPadZeroSupCut, fPadMinClusterSize, fPadMaxClusterSize, nbOfDetectors, nbOfTriggers, nbOfTrackers);
}

//=============================================================================================================
SRSTrack::~SRSTrack() {
  fDetXOffset.clear();
  fDetYOffset.clear();
  fDetZPos.clear();

  fFitParameters.clear();

  fXRangeMaxResiduals.clear();
  fYRangeMaxResiduals.clear();
  fRRangeMaxResiduals.clear();
  fPHIRangeMaxResiduals.clear();

  fXRangeMinResiduals.clear();
  fYRangeMinResiduals.clear();
  fRRangeMinResiduals.clear();
  fPHIRangeMinResiduals.clear();

  fYNBinResiduals.clear();
  fXNBinResiduals.clear();
  fRNBinResiduals.clear();
  fPHINBinResiduals.clear();

  fTriggerList.clear();
  fTrackerList.clear();
  fDetectorList.clear();
  fDetPlaneRotationCorrection.clear();

  ClearSpacePoints(fTrackSpacePointMap);
  ClearSpacePoints(fFittedSpacePointMap);
  ClearSpacePoints(fRawDataSpacePointMap);
  ClearSpacePoints(fEICstripClusterRawDataYMap);
}

//============================================================================================
void SRSTrack::ClearSpacePoints( map<TString, vector<Float_t> > & spacePointMap) {
  map<TString, vector<Float_t> >::const_iterator map_itr;
  for (map_itr = spacePointMap.begin(); map_itr != spacePointMap.end(); map_itr++) { 
    vector<Float_t> hit = (*map_itr).second;
    hit.clear();
  }
  spacePointMap.clear();
}

//============================================================================================
void SRSTrack::ReadCfg(const char * cfgname) { 
  printf("  SRSTrack::ReadCfg() ==> Loading SRS Tracking Configuration from: %s\n", cfgname);

  fTriggerList.clear();
  fTrackerList.clear();
  fDetectorList.clear();

  ClearSpacePoints(fTrackSpacePointMap);
  ClearSpacePoints(fRawDataSpacePointMap);
  ClearSpacePoints(fFittedSpacePointMap);

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
    
      if (s == "@RUNFILE") {
	fRunFilePrefix = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	fRunFileValue = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
      }

      if (s == "@TRACKANGLECUT") {
	fAngleCutMinX = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	fAngleCutMaxX = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	fAngleCutMinY = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	fAngleCutMaxY = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();

        printf("  SRSTrack::ReadCfg() ==> Min angle cut in x = %f [deg]; max angle cut in x = %f [deg] \n", fAngleCutMinX, fAngleCutMaxX);
        printf("  SRSTrack::ReadCfg() ==> Min angle cut in y = %f [deg]; max angle cut in y = %f [deg] \n", fAngleCutMinY, fAngleCutMaxY);
	continue;
      }

      if (s == "@EVENT3DDISPLAY") {
	fNtupleTitle = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	fNtupleSizeX = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	fNtupleSizeY = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	fNtupleSizeZ = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	printf("  SRSTrack::ReadCfg() %s ==>  fNtupleSizeX = %f mm, fNtupleSizeY = %f mm, fNtupleSizeZ = %f mm  \n", s.Data(), fNtupleSizeX, fNtupleSizeY, fNtupleSizeZ);
	continue;
      }

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
	SetDetectorConfig(name, triggerType, trackerType, zPos, xnbin, xmin, xmax, ynbin, ymin, ymax); 
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
  	SetDetectorConfig(name, triggerType, trackerType, zPos, xnbin, xmin, xmax, xnbin, xmin, xmax); 
	continue;
      }
    }
    tokens->Delete();
  } 
  printf("  SRSTrack::ReadCfg() ==> Exit SRS Tracking Configuration =======================================================\n");
}   

//-------------------------------------------------------------------------------------------------
void SRSTrack::LoadAlignementParamRootFile(TString runname) {
  map <TString, TString >::const_iterator ref_itr = fDetectorList.begin();
  TString refTracker = (*ref_itr).first;
  TString xRefPlane  = fMapping->GetDetectorPlaneListFromDetector(refTracker).front();
  TString yRefPlane  = fMapping->GetDetectorPlaneListFromDetector(refTracker).back();
  printf("SRSTrack::LoadAlignementParamRootFile():=> 1st Tracker = %s, xplane = %s, yplane = %s\n",refTracker.Data(), xRefPlane.Data(), yRefPlane.Data());

  TString xOffsetName = "offset_" + xRefPlane;
  TString yOffsetName = "offset_" + yRefPlane;
  TString xyRotationName = "rotation_" + refTracker;

  // COARSE OFFSET CORRECTION PARAM
  TString coarseOffset = runname + "offsetDir/coarseOffsetCalibration.root";
  printf("  SRSTrack::LoadAlignementParamRootFile() ==> load coarse offsets ROOT file %s\n", coarseOffset.Data());

  TFile * coarseOffsetFile = new TFile(coarseOffset,"read");
  if (!coarseOffsetFile->IsOpen()) {
    printf("  SRSTrack::LoadAlignementParamRootFile() **** ERROR ERROR *** Cannot open ROOT file %s \n", coarseOffset.Data()); 
    map<TString, TString>::const_iterator det_itr;
    for (det_itr = fDetectorList.begin(); det_itr != fDetectorList.end(); det_itr++) {
      TString detName = (*det_itr).first;
      fDetXOffset[detName] = 0;
      fDetYOffset[detName] = 0;
      fDetPlaneRotationCorrection[detName] = 0;
    }
  }

  else {
    map<TString, TString>::const_iterator det_itr;
    for (det_itr = fDetectorList.begin(); det_itr != fDetectorList.end(); det_itr++) {
      TString detName = (*det_itr).first;
      if (detName == refTracker) continue;

      TString planeX = (fMapping->GetDetectorPlaneListFromDetector(detName)).front();
      TString planeY = (fMapping->GetDetectorPlaneListFromDetector(detName)).back();

      xOffsetName  = "offset_" + xRefPlane + "_" + planeX;
      yOffsetName  = "offset_" + yRefPlane + "_" + planeY;
 
      fDetXOffset[detName] = ((TH1F *) coarseOffsetFile->Get(xOffsetName))->GetFunction("fitFunction")->GetParameter("Mean");
      fDetYOffset[detName] = ((TH1F *) coarseOffsetFile->Get(yOffsetName))->GetFunction("fitFunction")->GetParameter("Mean"); 
      fDetPlaneRotationCorrection[detName] = 0;
      printf("  SRSTrack::LoadAlignementParamRootFile() => offxetX[ %s ] = %f  offsetY[ %s ] = %f, rot[ %s ] = %f \n", detName.Data(), fDetXOffset[detName.Data()], detName.Data(), fDetYOffset[detName.Data()], detName.Data(), fDetPlaneRotationCorrection[detName.Data()]);
   }
  }
  coarseOffsetFile->Close();


  // FINE OFFSET CORRECTION PARAM
  TString fineOffset   = runname + "offsetDir/fineOffsetCalibration.root";
  printf("  SRSTrack::LoadAlignementParamRootFile() ==> load fine offsets ROOT file %s\n", fineOffset.Data());

  TFile * fineOffsetFile = new TFile(fineOffset,"read");
  if (!fineOffsetFile->IsOpen()) {
    printf("  SRSTrack::LoadAlignementParamRootFile() **** ERROR ERROR *** Cannot open ROOT file %s \n", fineOffset.Data()); 
  }

  else {
    map<TString, TString>::const_iterator det_itr;
    for (det_itr = fDetectorList.begin(); det_itr != fDetectorList.end(); det_itr++) {
      TString detName = (*det_itr).first;

      if (detName == refTracker) continue;
      TString planeX = (fMapping->GetDetectorPlaneListFromDetector(detName)).front();
      TString planeY = (fMapping->GetDetectorPlaneListFromDetector(detName)).back();

      xOffsetName  = "fine_offset_" + xRefPlane + "_" + planeX;
      yOffsetName  = "fine_offset_" + yRefPlane + "_" + planeY;
      xyRotationName  = "rotation_"    + refTracker + "_" + detName;

      fDetXOffset[detName] = fDetXOffset[detName] + ((TH1F *) fineOffsetFile->Get(xOffsetName))->GetFunction("fitFunction")->GetParameter("Mean");
      fDetYOffset[detName] = fDetYOffset[detName] + ((TH1F *) fineOffsetFile->Get(yOffsetName))->GetFunction("fitFunction")->GetParameter("Mean"); 
      fDetPlaneRotationCorrection[detName] =+ ((TH1F *) fineOffsetFile->Get(xyRotationName))->GetFunction("fitFunction")->GetParameter("Mean");;

      printf("  SRSTrack::LoadAlignementParamRootFile() => offsetX[%s]=%f  offsetY[%s]=%f, rot[%s]=%f \n", detName.Data(), fDetXOffset[detName.Data()], detName.Data(), fDetYOffset[detName.Data()], detName.Data(), fDetPlaneRotationCorrection[detName.Data()]);
    }
  }
  fineOffsetFile->Close();
  printf("  SRSTrack::LoadAlignementParamRootFile()==> Exit \n\n");
}

//============================================================================================
void SRSTrack::SetDetectorConfig(TString detName, TString triggerType, TString trackerType, Float_t zPos, Int_t xnbin, Float_t xmin, Float_t xmax, Int_t ynbin, Float_t ymin, Float_t ymax) { 

  if (detName.Contains("EIC")) {
    trackerType = "noTracker";
    triggerType = "noTrigger";
  } 

  printf("  SRSTrack::SetDetectorConfig() ==> %s,  %s,  %s,  %f \n", detName.Data(), triggerType.Data(), trackerType.Data(), zPos);

  fDetZPos[detName]      = zPos;
  fDetectorList[detName] = detName;

  if((trackerType == "isTracker") || (trackerType == "IsTracker")) fTrackerList[detName] = trackerType;
  if((triggerType == "isTrigger") || (triggerType == "IsTrigger")) fTriggerList[detName] = triggerType;

  fXNBinResiduals[detName]     = xnbin; 
  fXRangeMinResiduals[detName] = xmin; 
  fXRangeMaxResiduals[detName] = xmax; 

  fYNBinResiduals[detName]     = ynbin; 
  fYRangeMinResiduals[detName] = ymin; 
  fYRangeMaxResiduals[detName] = ymax;

  fRNBinResiduals[detName]     = xnbin; 
  fRRangeMinResiduals[detName] = xmin; 
  fRRangeMaxResiduals[detName] = xmax;

  fPHINBinResiduals[detName]     = ynbin; 
  fPHIRangeMinResiduals[detName] = ymin; 
  fPHIRangeMaxResiduals[detName] = ymax;
}

//============================================================================================
Bool_t SRSTrack::IsTracker(TString detName) {
  map < TString, TString > ::const_iterator it = fTrackerList.find(detName);
  return it != fTrackerList.end();
}

//============================================================================================
Bool_t SRSTrack::IsTrigger(TString detName) {
  map < TString, TString > ::const_iterator it = fTriggerList.find(detName);
  return it != fTriggerList.end();
}

//============================================================================================
Bool_t SRSTrack::IsTrackingEnabled() {
  Bool_t isTrackingEnabled = kFALSE;
  if (fTrackerList.size() != 0) isTrackingEnabled = kTRUE;
  return  isTrackingEnabled;
}

//============================================================================================
void SRSTrack::BuildRawDataSpacePoints(SRSEventBuilder * eventbuilder) {
  //  printf("\n------ SRSTrack::BuildRawDataSpacePoints() ==> Enter\n");

  ClearSpacePoints(fRawDataSpacePointMap);
  ClearSpacePoints(fEICstripClusterRawDataYMap);
  if(eventbuilder->IsAGoodEvent()) {

    map < Int_t, vector <Float_t > > detectorEvent ;
    SRSMapping * mapping = SRSMapping::GetInstance();
    map<TString, TString>::const_iterator det_itr;
    for (det_itr = fDetectorList.begin(); det_itr != fDetectorList.end(); det_itr++) {

      TString detName = (*det_itr).first;

      if (eventbuilder->IsAGoodEventInDetector(detName) == kFALSE) continue;

      detectorEvent.clear() ;
      detectorEvent = eventbuilder->GetDetectorCluster(detName);
      Int_t clusterMult = detectorEvent.size();

      if (clusterMult == 0) {
	detectorEvent.clear();
	continue;
      }

      if(detName.Contains("GEM1")) {
	fDetXOffset[detName] =  0.0375; 
	fDetYOffset[detName] = -0.006;
      }
      if(detName.Contains("GEM2")) {
	fDetXOffset[detName] =  -0.056; 
	fDetYOffset[detName] = -0.01;
      }
      if(detName.Contains("GEM3")) {
	fDetXOffset[detName] =  0.0187; 
	fDetYOffset[detName] = -0.003;
      }
      if(detName.Contains("PADGEM")) {
	fDetXOffset[detName] = -0.4; 
	fDetYOffset[detName] =  0.65; 
      }
      fRawDataSpacePointMap[detName].push_back(detectorEvent[0][0] - fDetXOffset[detName]);
      fRawDataSpacePointMap[detName].push_back(detectorEvent[0][1] - fDetYOffset[detName]);
      fRawDataSpacePointMap[detName].push_back(fDetZPos[detName]);
      PlaneRotationCorrection(fDetPlaneRotationCorrection[detName], fRawDataSpacePointMap[detName]);
      fEICstripClusterRawDataYMap[detName].push_back(detectorEvent[0][6]);
      fEICstripClusterRawDataYMap[detName].push_back(detectorEvent[0][7]);
      fEICstripClusterRawDataYMap[detName].push_back(fDetZPos[detName]);

      for (Int_t k = 0; k <detectorEvent.size() ; k++) 	detectorEvent[k].clear();
      detectorEvent.clear();
    }
  }
  //  printf("------ SRSTrack::BuildRawDataSpacePoints() ==> Exit: Nb Of good Event Det = %d \n\n", fRawDataSpacePointMap.size());
}

//============================================================================================
void SRSTrack::BuildTrack() {
  //  printf("-------- SRSTrack::BuildTrack() ==> Enter: Nb Of Good Tracks = %d, Nb Of Good Events Det = %d \n", fTrackSpacePointMap.size(), fRawDataSpacePointMap.size());
  ClearSpacePoints(fTrackSpacePointMap);
  map <TString, vector<Float_t> >::const_iterator tracker_itr;
  for(tracker_itr = fRawDataSpacePointMap.begin(); tracker_itr != fRawDataSpacePointMap.end(); ++tracker_itr) {
    TString detName = (*tracker_itr).first;
    if (!IsTrigger(detName)) continue;
    if (!IsTracker(detName)) continue;
    fTrackSpacePointMap[detName].clear();
    fTrackSpacePointMap[detName] = fRawDataSpacePointMap[detName];
    fIsGoodTrack = kTRUE;
  }
  if(fTrackSpacePointMap.size() != fTrackerList.size()) {
    fIsGoodTrack = kFALSE;
    return;
  }
  //  printf("-------- SRSTrack::BuildTrack() ==> Exit:  Nb Of Good Tracks = %d, Nb Of Good Events Det = %d \n", fTrackSpacePointMap.size(), fRawDataSpacePointMap.size());
}

//====================================================================================================================
void SRSTrack::FitTrack() {
  //  printf("---- SRSTrack::FitTrack() ==> Enter\n");
  ClearSpacePoints(fFittedSpacePointMap);
  if(fIsGoodTrack == kFALSE) return;
  SRSTrackFit * trackFit = new SRSTrackFit(fTrackSpacePointMap, fRawDataSpacePointMap);
  fFittedSpacePointMap = trackFit->GetTrackFittedData();

  fFitParameters["xOffset"] = trackFit->GetFitParameters()["xOffset"];
  fFitParameters["yOffset"] = trackFit->GetFitParameters()["yOffset"];
  fFitParameters["xDirection"] = trackFit->GetFitParameters()["xDirection"] * 180 / PI;
  fFitParameters["yDirection"] = trackFit->GetFitParameters()["yDirection"] * 180 / PI; 
}

//====================================================================================================================
Bool_t  SRSTrack::IsAGoodTrack(SRSEventBuilder * eventbuilder) {
  //  printf("---- SRSTrack::IsAGoodTrack() ==> ENTER \n");
  Int_t nbOfTrackers = fTrackerList.size();
  if (nbOfTrackers > 1) {
    BuildRawDataSpacePoints(eventbuilder);
    BuildTrack();
    FitTrack();
  }
  //  printf("---- SRSTrack::IsAGoodTrack() ==>fIsGoodTrack=%d \n", fIsGoodTrack );
  return fIsGoodTrack;
}

//=========================================================================================================================//
const vector<Float_t> SRSTrack::directionVectorFrom2Points(const vector<Float_t> u, const vector<Float_t> v) {
  int sizeV =  v.size();
  int sizeU =  u.size();
  vector<Float_t> dir;
  dir.resize(3,0);
  if( (sizeU == 3) && (sizeV == 3) ) {
    vector<Float_t> sub;
    sub.push_back(v[0] - u[0]);
    sub.push_back(v[1] - u[1]);
    sub.push_back(v[2] - u[2]);
    Float_t norm = normVec(sub);
    dir.push_back(sub[0]/norm);
    dir.push_back(sub[1]/norm);
    dir.push_back(sub[2]/norm);
    //    printf("SRSTrack::directionVectorFrom2Points(): sub[0]=%f, sub[1]=%f, sub[2]=%f, norm=%f \n",sub[0],sub[1],sub[2],norm);
  }
  return dir;
}

//=========================================================================================================================//
Float_t SRSTrack::getAngleTo(const vector<Float_t> u, const vector<Float_t> v) {
  Float_t angle = 0.00;
  int sizeV =  v.size();
  int sizeU =  u.size();
  if( (sizeU == 3) && (sizeV == 3) ) {
    Float_t argument = dotVec(u,v)/(normVec(u)*normVec(v));
    angle = acos(argument);    
    if(angle != angle) angle = 0.0;
    //    printf("SRSTrack::getAngleTo()=> angle = %f \n",angle);
  }
  return angle;
}

//=========================================================================================================================//
Float_t SRSTrack::getAngleAmplitude(const vector<Float_t> u, const vector<Float_t> v) {

  Float_t angle = 0.00;
  int sizeV =  v.size();
  int sizeU =  u.size();

  if( (sizeU == 3) && (sizeV == 3) ) {
    Float_t argument = abs(dotVec(u,v))/(normVec(u)*normVec(v));
    angle = acos(argument);    
    if(angle != angle) angle = 0.0;
    //    printf("SRSTrack::getAngleTo()=> angle = %f \n",angle);
  }
  return angle;
}

//=========================================================================================================================//
Float_t SRSTrack::normVec(const vector<Float_t> u) {
  int size =  u.size();
  if(size != 3) printf("SRSTrack::normVec(): WARNING ==> point vector's size = %d,!= 3  \n",size); 
  return sqrt(dotVec(u,u));
}

//=========================================================================================================================//
Float_t SRSTrack::dotVec(const vector<Float_t> u, const vector<Float_t> v) {
  int sizeV =  v.size();
  int sizeU =  u.size();
  if(sizeU != 3)  printf("SRSTrack::dotVec(): WARNING ==> U point vector's size = %d,!= 3  \n",sizeU);
  if(sizeV != 3)  printf("SRSTrack::dotVec(): WARNING ==> V point vector's size = %d,!= 3  \n",sizeV);
  return v[0]*u[0] + v[1]*u[1] + v[2]*u[2];  
}

//=========================================================================================================================//
const vector<Float_t> SRSTrack::subVec(const vector<Float_t> u, const vector<Float_t> v) {
  int sizeV =  v.size();
  int sizeU =  u.size();
  if(sizeU != 3)  printf("SRSTrack::subVec(): WARNING ==> U point vector's size = %d,!= 3  \n",sizeU);
  if(sizeV != 3)  printf("SRSTrack::subVec(): WARNING ==> V point vector's size = %d,!= 3  \n",sizeV);
  vector<Float_t> sub;
  sub.push_back(v[0] - u[0]);
  sub.push_back(v[1] - u[1]);
  sub.push_back(v[2] - u[2]);
  return sub;
}

//=========================================================================================================================//
const vector<Float_t> SRSTrack::addVec(const vector<Float_t> u, const vector<Float_t> v) {
  int sizeV =  v.size();
  int sizeU =  u.size();
  if(sizeU != 3)  printf("SRSTrack::addVec(): WARNING ==> U point vector's size = %d,!= 3  \n",sizeU);
  if(sizeV != 3)  printf("SRSTrack::addVec(): WARNING ==> V point vector's size = %d,!= 3  \n",sizeV);
  vector<Float_t> add;
  add.push_back(u[0] + v[0]);
  add.push_back(u[1] + v[1]);
  add.push_back(u[2] + v[2]);
  return add;
}

//=========================================================================================================================//
const vector<Float_t> SRSTrack::prodVec(Float_t a, const vector<Float_t> u) {
  int sizeU =  u.size();
  if(sizeU != 3)  printf("SRSTrack::prodVec(): WARNING ==> U point vector's size = %d,!= 3  \n",sizeU);
  vector<Float_t> prod;
  prod.push_back(a*u[0]);
  prod.push_back(a*u[1]);
  prod.push_back(a*u[2]);
  return prod;
}

//=========================================================================================================================//
void SRSTrack::PlaneRotationCorrection(Float_t alpha, vector<Float_t> & u) {
  int sizeU =  u.size();
  if(sizeU != 3)  printf("SRSTrack::PlaneRotationCorrection: WARNING ==> U point vector's size = %d,!= 3  \n",sizeU);
  alpha = -1 * alpha; 
  u[0] = u[0] * TMath::Cos(alpha) + u[1] * TMath::Sin(alpha);
  u[1] = u[1] * TMath::Cos(alpha) - u[0] * TMath::Sin(alpha);
  u[2] = u[2];
}

//=========================================================================================================================//
Float_t SRSTrack::projectedAngleXY(const vector<Float_t> u, TString xORy) {
  vector<Float_t> v = getDirection(u);
  Float_t vz=v[2];
  Float_t vproj=v[0];

  if((xORy == "X") || (xORy == "x")) vproj=v[0];
  if((xORy == "Y") || (xORy == "y")) vproj=v[1];

  Float_t vplane = sqrt(vz*vz+vproj*vproj);
  Float_t projAngle = 0.;
  if(vplane > 0.) {
      vz /= vplane;
      projAngle = acos(-vz);
      if(vproj < 0.) projAngle *= -1.;
  }
  return projAngle;
}

//=========================================================================================================================//
Float_t SRSTrack::projectedAngleXZ(const vector<Float_t> u, TString xORz) {
  vector<Float_t> v = getDirection(u);
  Float_t vy=v[1];
  Float_t vproj=v[0];
  if((xORz == "X") || (xORz == "x")) vproj=v[0];
  if((xORz == "Z") || (xORz == "z")) vproj=v[2];
  Float_t vplane = sqrt(vy*vy+vproj*vproj);
  Float_t projAngle = 0.;
  if(vplane > 0.) {
    vy /= vplane;
    projAngle = acos(-vy);
    if(vproj < 0.) projAngle *= -1.;
  }
  return projAngle;
}

//=========================================================================================================================//
const vector<Float_t> SRSTrack::getXandYKnowingZ(const vector<Float_t> w, const vector<Float_t> v, Float_t z0) {
  vector<Float_t> d = directionVectorFrom2Points(v,w);
  vector<Float_t> p0;
  Float_t t0 = (w[2] - z0)/d[2];
  Float_t x0 = w[0] - d[0]*t0;
  Float_t y0 = w[1] - d[1]*t0;
  p0.push_back(x0);
  p0.push_back(y0);
  p0.push_back(z0);
  return p0;
}

//=========================================================================================================================//
Float_t SRSTrack::abs(Float_t x) {
  return ((x) >= 0 ? (x):(-x)); 
}

//=========================================================================================================================//
const vector<Float_t> SRSTrack::getDirection(const vector<Float_t> u) {
  int size =  u.size();
  if(size != 3)  printf("SRSTrack::getDirection(): WARNING ==> U point vector's size = %d,!= 3  \n",size);
  vector<Float_t> direction;
  Float_t x = u[0]/normVec(u);
  Float_t y = u[1]/normVec(u);
  Float_t z = u[2]/normVec(u);
  direction.push_back(x);
  direction.push_back(y);
  direction.push_back(z);
  return direction;
}

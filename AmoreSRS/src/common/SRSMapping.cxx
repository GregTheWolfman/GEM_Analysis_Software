#include "SRSMapping.h"

SRSMapping * SRSMapping::fInstance = 0 ;

//======================================================================================================================================
void SRSMapping::SetCartesianStripsReadoutMap(TString readoutBoard, TString detectorType, TString detector, Int_t detID, TString planeX,  Float_t sizeX, Int_t connectorsX, Int_t orientX, TString planeY,  Float_t sizeY, Int_t connectorsY, Int_t orientY) {

  printf("  SRSMapping::SetCartesianStripsReadoutMap() => readout=%s, detType=%s, det=%s, detID=%d, planeX=%s, SizeX=%.1f, connectorsX=%d, orientationX=%d, planeY=%s, SizeY=%.1f, connectorsY=%d, orientationY=%d \n", readoutBoard.Data(), detectorType.Data(), detector.Data(), detID, planeX.Data(), sizeX, connectorsX, orientX, planeY.Data(), sizeY, connectorsY, orientY) ;

  fDetectorFromIDMap[detID]     = detector ;
  fReadoutBoardFromIDMap[detID] = readoutBoard ; 

  fDetectorIDFromDetectorMap[detector] = detID ;
  fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
  fDetectorTypeFromDetectorMap[detector] = detectorType ;

  fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
  fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;

  fPlaneIDFromPlaneMap[planeX] = 0 ;
  fPlaneIDFromPlaneMap[planeY] = 1 ;

  fDetectorFromPlaneMap[planeX] = detector ;
  fDetectorFromPlaneMap[planeY] = detector ;

  fDetectorPlaneListFromDetectorMap[detector].push_back(planeX) ;
  fDetectorPlaneListFromDetectorMap[detector].push_back(planeY) ;

  fCartesianPlaneMap[planeX].push_back(0) ;
  fCartesianPlaneMap[planeX].push_back(sizeX) ;
  fCartesianPlaneMap[planeX].push_back(connectorsX) ;
  fCartesianPlaneMap[planeX].push_back(orientX) ;
  fCartesianPlaneMap[planeY].push_back(1) ;
  fCartesianPlaneMap[planeY].push_back(sizeY) ;
  fCartesianPlaneMap[planeY].push_back(connectorsY) ;
  fCartesianPlaneMap[planeY].push_back(orientY) ;
}
//===========================================================================================
// Implimenting Straight strip and large ZZ sectors (WIP)[Jared Hadley 2021] VVVV
//===========================================================================================
void SRSMapping::SetFITZZAPV3ReadoutMap(TString readoutBoard, TString detectorType, TString detector, Int_t detID, TString plane,  Float_t size, Int_t connectors, Int_t orient) {

printf("  SRSMapping::SetFITZZAPV3StripsReadoutMap() => readout=%s, detType=%s, det=%s, detID=%d, plane=%s, Size=%.1f, connectors=%d, orientation=%d \n", readoutBoard.Data(), detectorType.Data(), detector.Data(), detID, plane.Data(), size, connectors, orient) ;

  fDetectorFromIDMap[detID]     = detector ;
  fReadoutBoardFromIDMap[detID] = readoutBoard ; 

  fDetectorIDFromDetectorMap[detector] = detID ;
  fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
  fDetectorTypeFromDetectorMap[detector] = detectorType ;

  fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
  fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;

  fPlaneIDFromPlaneMap[plane] = 0 ;
  
  fDetectorFromPlaneMap[plane] = detector ;
  
  fDetectorPlaneListFromDetectorMap[detector].push_back(plane) ;
  
  fFITZZAPV3ReadoutMap[plane].push_back(0) ;
  fFITZZAPV3ReadoutMap[plane].push_back(size) ;
  fFITZZAPV3ReadoutMap[plane].push_back(connectors) ;
  fFITZZAPV3ReadoutMap[plane].push_back(orient) ;
}  
//===========================================================================================
// Implimenting Straight strip and large ZZ sectors (WIP)[Jared Hadley 2021] ^^^^
//===========================================================================================

//======================================================================================================================================
void SRSMapping::SetTrapezoid_UV_ReadoutMap(TString readoutBoard, TString detectorType, TString detector, Int_t detID,  Float_t length,  Float_t innerR,  Float_t outerR, TString planeTop, Int_t connectTop, Int_t orientTop, TString planeBot, Int_t connectBot, Int_t orientBot) {

  printf("  SRSMapping::SetUVStripsReadoutMap() => readout=%s, detType=%s, det=%s, detID=%d, innerR=%.1f, outerR=%.1f, planeTop=%s, connectTop=%d, orientTop=%d, planeBot=%s, connectBot=%d, orientBot=%d \n", readoutBoard.Data(), detectorType.Data(), detector.Data(), detID, innerR, outerR,   planeTop.Data(), connectTop, orientTop, planeBot.Data(), connectBot, orientBot) ;

  orientTop = 1 ;
  orientBot = 1 ;

  fDetectorFromIDMap[detID]     = detector ;
  fReadoutBoardFromIDMap[detID] = readoutBoard ; 

  fDetectorIDFromDetectorMap[detector] = detID ;
  fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
  fDetectorTypeFromDetectorMap[detector] = detectorType ;

  fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
  fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;

  fPlaneIDFromPlaneMap[planeTop] = 0 ;
  fPlaneIDFromPlaneMap[planeBot] = 1 ;

  fDetectorFromPlaneMap[planeTop] = detector ;
  fDetectorFromPlaneMap[planeBot] = detector ;

  fDetectorPlaneListFromDetectorMap[detector].push_back(planeTop) ;
  fDetectorPlaneListFromDetectorMap[detector].push_back(planeBot) ;

  fUVangleReadoutMap[detector].push_back(length) ;
  fUVangleReadoutMap[detector].push_back(innerR) ;
  fUVangleReadoutMap[detector].push_back(outerR) ;

  fUVangleReadoutMap[planeTop].push_back(0) ;
  fUVangleReadoutMap[planeTop].push_back(outerR) ;
  fUVangleReadoutMap[planeTop].push_back(connectTop) ;
  fUVangleReadoutMap[planeTop].push_back(orientTop) ;
  fUVangleReadoutMap[planeBot].push_back(1) ;
  fUVangleReadoutMap[planeBot].push_back(outerR) ;
  fUVangleReadoutMap[planeBot].push_back(connectBot) ;
  fUVangleReadoutMap[planeBot].push_back(orientBot) ;
}

//======================================================================================================================================
void SRSMapping::SetRectangle_UV_ReadoutMap(TString readoutBoard, TString detectorType, TString detector, Int_t detID,  Float_t length,  Float_t width,  Float_t uvangle, TString planeTop, Int_t connectTop, Int_t orientTop, TString planeBot, Int_t connectBot, Int_t orientBot) {

  printf("  SRSMapping::SetUVStripsReadoutMap() => readout=%s, detType=%s, det=%s, detID=%d, length=%.1f, width=%.1f, uv_angle=%.1f, planeTop=%s, connectTop=%d, orientTop=%d, planeBot=%s, connectBot=%d, orientBot=%d \n", readoutBoard.Data(), detectorType.Data(), detector.Data(), detID, length,  width, uvangle, planeTop.Data(), connectTop, orientTop, planeBot.Data(), connectBot, orientBot) ;

  orientTop = 1 ;
  orientBot = 1 ;

  fDetectorFromIDMap[detID]     = detector ;
  fReadoutBoardFromIDMap[detID] = readoutBoard ; 

  fDetectorIDFromDetectorMap[detector] = detID ;
  fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
  fDetectorTypeFromDetectorMap[detector] = detectorType ;

  fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
  fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;

  fPlaneIDFromPlaneMap[planeTop] = 0 ;
  fPlaneIDFromPlaneMap[planeBot] = 1 ;

  fDetectorFromPlaneMap[planeTop] = detector ;
  fDetectorFromPlaneMap[planeBot] = detector ;

  fDetectorPlaneListFromDetectorMap[detector].push_back(planeTop) ;
  fDetectorPlaneListFromDetectorMap[detector].push_back(planeBot) ;

  fUVangleReadoutMap[detector].push_back(length) ;
  fUVangleReadoutMap[detector].push_back(width) ;
  fUVangleReadoutMap[detector].push_back(uvangle) ;

  fUVangleReadoutMap[planeTop].push_back(0) ;
  fUVangleReadoutMap[planeTop].push_back(length) ;
  fUVangleReadoutMap[planeTop].push_back(connectTop) ;
  fUVangleReadoutMap[planeTop].push_back(orientTop) ;

  fUVangleReadoutMap[planeBot].push_back(1) ;
  fUVangleReadoutMap[planeBot].push_back(length) ;
  fUVangleReadoutMap[planeBot].push_back(connectBot) ;
  fUVangleReadoutMap[planeBot].push_back(orientBot) ;
}


//======================================================================================================================================
void SRSMapping::Set1DStripsReadoutMap(TString readoutBoard, TString detectorType, TString detector, Int_t detID, TString plane,  Float_t size, Int_t connectors, Int_t orient) {
  orient = 1 ;
  printf("  SRSMapping::Set1DStripsReadoutMap() => readout=%s, detType=%s, det=%s, detID=%d, plane=%s, Size=%.1f, connectors=%d, orientation=%d \n", readoutBoard.Data(), detectorType.Data(), detector.Data(), detID, plane.Data(), size, connectors, orient) ;

  fDetectorFromIDMap[detID]     = detector ;
  fReadoutBoardFromIDMap[detID] = readoutBoard ; 

  fDetectorIDFromDetectorMap[detector] = detID ;
  fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
  fDetectorTypeFromDetectorMap[detector] = detectorType ;

  fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
  fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;

  fPlaneIDFromPlaneMap[plane] = 0 ;
  fDetectorFromPlaneMap[plane] = detector ;
  fDetectorPlaneListFromDetectorMap[detector].push_back(plane) ;

  f1DStripsPlaneMap[plane].push_back(0) ;
  f1DStripsPlaneMap[plane].push_back(size) ;
  f1DStripsPlaneMap[plane].push_back(connectors) ;
  f1DStripsPlaneMap[plane].push_back(orient) ;
}

//======================================================================================================================================
void SRSMapping::SetCMSGEMReadoutMap(TString readoutBoard, TString detectorType,  TString detector, Int_t detID, TString EtaSector,  Float_t etaSectorPos, Float_t etaSectorSize, Float_t nbOfSectorConnectors, Int_t apvOrientOnEtaSector) {
  printf("  SRSMapping::SetCMSGEMReadoutMap() =>readout=%s, detType=%s, det=%s, detID=%d, EtaSector=%s, etaSectorSize=%.1f, nbOSectorfConnectors=%.1f, apvOrientOnEtaSector=%d \n", readoutBoard.Data(), detectorType.Data(), detector.Data(), detID, EtaSector.Data(), etaSectorSize, nbOfSectorConnectors, apvOrientOnEtaSector) ;

  fDetectorFromIDMap[detID]     = detector ;
  fReadoutBoardFromIDMap[detID] = readoutBoard ; 

  fDetectorIDFromDetectorMap[detector] = detID ;
  fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
  fDetectorTypeFromDetectorMap[detector] = detectorType ;
  fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
  fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;

  fDetectorFromPlaneMap[EtaSector] = detector ;
  fDetectorPlaneListFromDetectorMap[detector].push_back(EtaSector) ;

  fCMSGEMDetectorMap[EtaSector].push_back(etaSectorPos) ;
  fCMSGEMDetectorMap[EtaSector].push_back(etaSectorSize) ;
  fCMSGEMDetectorMap[EtaSector].push_back(nbOfSectorConnectors) ;
  fCMSGEMDetectorMap[EtaSector].push_back(apvOrientOnEtaSector) ;
} 

//======================================================================================================================================
void SRSMapping::SetPadsReadoutMap(TString readoutBoard, TString detectorType,  TString detector, Int_t detID, Float_t padSizeX,  Float_t padSizeY, Float_t nbOfPadX, Float_t nbOfPadY, Float_t nbOfConnectors) {
  printf("  SRSMapping::SetPadsReadoutMap() =>readout=%s, detType=%s, det=%s, detID=%d, nbOfPadX=%f, padSizeX=%.1f, nbOfPadY=%f, padSizeY=%f, nbOfConnectors=%f \n", readoutBoard.Data(), detectorType.Data(), detector.Data(), detID, nbOfPadX, padSizeX, nbOfPadY, padSizeY, nbOfConnectors) ;

  fDetectorFromIDMap[detID]     = detector ;
  fReadoutBoardFromIDMap[detID] = readoutBoard ; 

  fDetectorIDFromDetectorMap[detector]   = detID ;
  fReadoutBoardFromDetectorMap[detector] = readoutBoard ;
  fDetectorTypeFromDetectorMap[detector] = detectorType ;

  fDetectorListFromDetectorTypeMap[detectorType].push_back(detector) ;
  fDetectorListFromReadoutBoardMap[readoutBoard].push_back(detector) ;

  fDetectorFromPlaneMap[detector] = detector ;
  fDetectorPlaneListFromDetectorMap[detector].push_back(detector) ;
  fPlaneIDFromPlaneMap[detector]  = 0 ;

  fPadDetectorMap[detector].push_back(padSizeX) ;
  fPadDetectorMap[detector].push_back(padSizeY) ;
  fPadDetectorMap[detector].push_back(nbOfPadX) ;
  fPadDetectorMap[detector].push_back(nbOfPadY) ;
  fPadDetectorMap[detector].push_back(nbOfConnectors); 
} 

//=========================================================================================================================
TString SRSMapping::GetAPV(TString detPlane, Int_t fecId, Int_t adcCh, Int_t apvNo, Int_t apvIndex, Int_t apvID) {
  stringstream out ;
  out << apvID ;
  TString apvIDStr = out.str();
  out.str("") ;
  out << fecId ;
  TString fecIDStr = out.str();
  out.str("") ;
  out <<  adcCh;
  TString adcChStr = out.str();
  out.str("") ;
  out <<  apvNo ;
  TString apvNoStr = out.str();
  out.str("") ;
  out <<  apvIndex ;
  TString apvIndexStr = out.str();
  out.str("") ;
  TString apvName = "apvNo" + apvNoStr + "_apvId" + apvIDStr + "_adcCh" + adcChStr + "_FecId" + fecIDStr + "_" + detPlane ;
  return apvName ;
}

//======================================================================================================================================
void  SRSMapping::SetAPVMap(TString detPlane, Int_t fecId, Int_t adcCh, Int_t apvNo, Int_t apvOrient, Int_t apvIndex, Int_t apvHdr) {
  //  printf("SRSMapping::SetAPVMap() ==> Enter: \n") ;
  // std::cout << "Code Enters this function!" << std::endl;
  Int_t apvID = (fecId << 4) | adcCh ;
  TString apvName = GetAPV(detPlane, fecId, adcCh, apvNo, apvIndex, apvID) ;
	
  std::cout << "adcCh: " << adcCh << std::endl;
  std::cout << "apvID: " << apvID << std::endl;
  std::cout << "apvNo: " << apvNo << std::endl;

  fAPVNoFromIDMap[apvID]           = apvNo ;//apvNo
  fAPVIDFromAPVNoMap[apvNo]        = apvID ;//apvID
  fAPVFromIDMap[apvID]             = apvName ;
  fAPVHeaderLevelFromIDMap[apvID]  = apvHdr ;
  fAPVOrientationFromIDMap[apvID]  = apvOrient ;
  fAPVIndexOnPlaneFromIDMap[apvID] = apvIndex ;

  fAPVIDFromNameMap[apvName]       = adcCh ;//apvID
  fDetectorPlaneFromAPVIDMap[apvID] = detPlane ;

  fAPVIDListFromFECIDMap[fecId].push_back(apvID);
  fFECIDListFromDetectorPlaneMap[detPlane].push_back(fecId);
  fAPVIDListFromDetectorPlaneMap[detPlane].push_back(apvID);

  TString detector = GetDetectorFromPlane(detPlane) ;
  fAPVIDListFromDetectorMap[detector].push_back(apvID);
  // printf("SRSMapping::SetAPVMap() ==> detector[%s]=%s,  fecId=%d, adcCh=%d, apvNod=%d, apvIDd=%d\n", detPlane.Data(), detector.Data(), fecId, adcCh, apvNo, apvID) ;
}

//======================================================================================================================================
/**
void  SRSMapping::SetAPVtoPadMapping(Int_t fecId, Int_t adcCh, Int_t padId, Int_t apvCh) {
  Int_t apvID      = (fecId << 4) | adcCh ;
  Int_t apvChPadCh = (padId << 8) | apvCh ;
  fAPVToPadChannelMap[apvID].push_back(apvChPadCh) ;
}
*/

//======================================================================================================================================
void SRSMapping::PrintMapping() {
  printf("SRSMapping::PrintMapping() ==> Enter: \n") ;

  map<TString, list<TString> >::const_iterator det_itr ;
  for(det_itr = fDetectorPlaneListFromDetectorMap.begin(); det_itr != fDetectorPlaneListFromDetectorMap.end(); ++det_itr) {
    TString detector = det_itr->first ;
    //    printf("  ======================================================================================================================\n") ;
    printf("  SRSMapping::PrintMapping() ==> Detector = %s \n",detector.Data()) ;

    list<TString> detPlaneList = det_itr->second ;
    list<TString>::const_iterator plane_itr ;
    for(plane_itr = detPlaneList.begin(); plane_itr != detPlaneList.end(); ++plane_itr) {
      TString detPlane = * plane_itr ;
      list <Int_t> fecList = GetFECIDListFromDetectorPlane(detPlane) ;
      list<Int_t>::const_iterator fec_itr ;
      for(fec_itr = fecList.begin(); fec_itr != fecList.end(); ++fec_itr) { 
	Int_t fecId = * fec_itr ;
	printf("  SRSMapping::PrintMapping()     Plane = %s,        FEC = %d \n", detPlane.Data(), fecId) ;

	list <Int_t> apvList = GetAPVIDListFromDetectorPlane(detPlane) ;
	list<Int_t>::const_iterator apv_itr ;
	for(apv_itr = apvList.begin(); apv_itr != apvList.end(); ++apv_itr) { 
	  Int_t apvID       = * apv_itr ;
	  Int_t apvNo       = GetAPVNoFromID(apvID);
	  Int_t apvIndex    = GetAPVIndexOnPlane(apvID);
	  Int_t apvOrient   = GetAPVOrientation(apvID);
	  Int_t fecID       = GetFECIDFromAPVID(apvID);
	  Int_t adcCh       = GetADCChannelFromAPVID(apvID);
	  Int_t apvHdrLevel = GetAPVHeaderLevelFromID(apvID);
	  TString  apvName  = GetAPVFromID(apvID) ;
	  if(fecID == fecId) printf("  SRSMapping::PrintMapping() ==> adcCh=%d,  apvName=%s,  apvID=%d, apvNo=%d,  apvIndex=%d,  apvOrientation=%d,  apvHdr=%d \n", adcCh, apvName.Data(), apvID, apvNo, apvIndex, apvOrient, apvHdrLevel) ;
	}
      }
      printf("\n") ;
    } 
  }
  printf("SRSMapping::PrintMapping() Exit: ==> Mapping of %d detectors, %d planes, %d FECs, %d APVs\n", GetNbOfDetectors(), GetNbOfDetectorPlane(), GetNbOfFECs(), GetNbOfAPVs());
  //  printf("======================================================================================================================\n") ;
}

//======================================================================================================================================
void SRSMapping::SaveMapping(const char * file) {
  printf("SRSMapping::SaveMapping() ==> Enter: Saving Mapping Config into file [%s],\n", file) ;
  FILE * f = fopen(file, "w");  

  fprintf(f,"#################################################################################################\n") ;
  fprintf(f,"         readoutType  Detector    Plane  DetNo   Plane   size (mm)  connectors  orientation\n");
  fprintf(f,"#################################################################################################\n") ;

  map<TString, list<TString> >::const_iterator det_itr ;
  for(det_itr = fDetectorPlaneListFromDetectorMap.begin(); det_itr != fDetectorPlaneListFromDetectorMap.end(); ++det_itr) {
    TString detector      = det_itr->first ;
    TString readoutBoard = GetReadoutBoardFromDetector(detector) ;
    TString detectorType = GetDetectorTypeFromDetector(detector) ;

    if ( (readoutBoard == "CARTESIAN") || (readoutBoard == "UV_ANGLE_OLD") || (readoutBoard == "UV_ANGLE") ){
      list<TString> detPlaneList = det_itr->second ;
      TString planeX    = detPlaneList.front() ;
      vector <Float_t> cartesianPlaneX = GetCartesianReadoutMap(planeX) ;
       Float_t sizeX     = cartesianPlaneX[1] ;
      Int_t connectorsX = (Int_t) (cartesianPlaneX[2]) ;
      Int_t orientX     = (Int_t) (cartesianPlaneX[3]) ;

      TString planeY    = detPlaneList.back() ;
      vector <Float_t> cartesianPlaneY = GetCartesianReadoutMap(planeY) ;
      Float_t sizeY     = cartesianPlaneY[1] ;
      Int_t connectorsY = (Int_t) (cartesianPlaneY[2]) ;
      Int_t orientY     = (Int_t) (cartesianPlaneY[3]) ;
      fprintf(f,"DET,  %s,   %s,   %s,   %s,  %f,   %d,   %d,   %s,   %f,   %d,   %d \n", readoutBoard.Data(), detectorType.Data(), detector.Data(), planeX.Data(), sizeX, connectorsX, orientX, planeY.Data(), sizeY, connectorsY, orientY ) ;
}    
//===========================================================================================
// Implimenting Straight strip and large ZZ sectors (WIP)[Jared Hadley 2021] VVVV
//===========================================================================================
  else if ( (readoutBoard == "FITZZAPV3")){
      list<TString> detPlaneList = det_itr->second ;
      TString plane    = detPlaneList.front() ;
      vector <Float_t> FITZZAPV3Plane = GetFITZZAPV3ReadoutMap(plane) ;
       Float_t size     = FITZZAPV3Plane[1] ;
      Int_t connectors = (Int_t) (FITZZAPV3Plane[2]) ;
      Int_t orient     = (Int_t) (FITZZAPV3Plane[3]) ;

      fprintf(f,"DET,  %s,   %s,   %s,   %s,  %f,   %d,   %d \n", readoutBoard.Data(), detectorType.Data(), detector.Data(), plane.Data(), size, connectors, orient) ;
}
//===========================================================================================
// Implimenting Straight strip and large ZZ sectors (WIP)[Jared Hadley 2021] /\/\/\/\/
//===========================================================================================


//=======================================================================================
// trying to impliment 1D strips; I have no idea what I'm doing (WIP) [Jared Hadley 2021]
//=======================================================================================
//    else if (readoutBoard == "1DSTRIPS" ) {
//      list<TString> detPlaneList = det_itr->second ;
//      TString plane    = detPlaneList.front() ;
//      vector <Float_t> 1DPlane = Get1dStripsReadoutMap(plane) ;
//      Float_t size     = 1DPlane[1] ;
//      Int_t connectors = (Int_t) (1DPlane[2]) ;
//      Int_t orient     = (Int_t) (1DPlane[3]) ;
//    else if (readoutBoard == "1DSTRIPS" ) { 
//    printf("SRSMapping::SaveMapping() ==> 1DSTRIPS is really broken, I'm working on it\n Warmest regards, Best wishes\n Jared", readoutBoard.Data()) ;
//    }
//========================================================================================
// YOLO
//========================================================================================
    else if (readoutBoard == "PADPLANE") {
      list<TString> detPlaneList = det_itr->second ;
      TString padPlane = detPlaneList.back() ;
      Float_t padSizeX = GetPadDetectorMap(detector) [0];
      Float_t padSizeY = GetPadDetectorMap(detector) [1];
      Float_t nbOfPadX = GetPadDetectorMap(detector) [2];
      Float_t nbOfPadY = GetPadDetectorMap(detector) [3];
      Float_t nbOfCons = GetPadDetectorMap(detector) [4]; 
      fprintf(f,"DET,  %s,   %s,   %s,  %s,  %f,   %f,   %f,   %f,  %f\n", readoutBoard.Data(), detectorType.Data(), detector.Data(), padPlane.Data(), padSizeX, nbOfPadX,  padSizeY, nbOfPadY, nbOfCons) ;
    }

    else if (readoutBoard == "CMSGEM") {
      list<TString> detPlaneList = det_itr->second ;
    }

    else {
      printf("SRSMapping::SaveMapping() ==> detector readout board type %s is not yet implemented ==> PLEASE MOVE ON \n", readoutBoard.Data()) ;
      continue ;
    }
  }

  fprintf(f,"###############################################################\n") ;
  fprintf(f,"#     fecId   adcCh   detPlane  apvOrient  apvIndex    apvHdr #\n");
  fprintf(f,"###############################################################\n") ;
  map<Int_t, TString>::const_iterator apv_itr;
  for(apv_itr = fAPVFromIDMap.begin(); apv_itr != fAPVFromIDMap.end(); ++apv_itr){
    Int_t apvID      = apv_itr->first;
    Int_t fecId      = GetFECIDFromAPVID(apvID);
    Int_t adcCh      = GetADCChannelFromAPVID(apvID);
    TString detPlane = GetDetectorPlaneFromAPVID(apvID) ;
    Int_t apvOrient  = GetAPVOrientation(apvID);
    Int_t apvIndex   = GetAPVIndexOnPlane(apvID);
    Int_t apvHdr     = GetAPVHeaderLevelFromID(apvID);
    fprintf(f,"APV,   %d,     %d,     %s,     %d,    %d,   %d\n", fecId, adcCh, detPlane.Data(), apvOrient, apvIndex, apvHdr);
  }
  fclose(f);
  printf("SRSMapping::SaveMapping() ==> Exit: Saving Mapping Config into file [%s],\n", file) ;
  //  printf("======================================================================================================================\n") ;

}

//============================================================================================
void SRSMapping::LoadDefaultMapping(const char * mappingCfgFilename) {

  Clear() ;
  printf("  SRSMapping::LoadDefaultMapping() ==> Loading Mapping from %s \n", mappingCfgFilename) ;
  Int_t apvNo = 0 ;
  Int_t detID = 0 ;

  ifstream filestream (mappingCfgFilename, ifstream::in); 
  TString line;
  while (line.ReadLine(filestream)) {

    line.Remove(TString::kLeading, ' ');   // strip leading spaces
    if (line.BeginsWith("#")) continue ;   // and skip comments
    //printf("   SRSMapping::LoadDefaultMapping() ==> Scanning the mapping cfg file %s\n",line.Data()) ;

    //=== Create an array of the tokens separated by "," in the line;
    TObjArray * tokens = line.Tokenize(","); 

    //=== iterator on the tokens array
    TIter myiter(tokens); 
    while (TObjString * st = (TObjString*) myiter.Next()) {

      //== Remove leading and trailer spaces
      TString s = st->GetString().Remove(TString::kLeading, ' ' );
      s.Remove(TString::kTrailing, ' ' );                         

      //      printf("    SRSMapping::LoadDefaultMapping() ==> Data ==> %s\n",s.Data()) ;
      if(s == "DET") {
	TString readoutBoard = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	TString detectorType = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	TString detector     = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );

	printf("  SRSMapping::LoadDefaultMapping() ==> detectorType=%s, readoutBoard=%s, detector=%s \n", detectorType.Data(), readoutBoard.Data(), detector.Data()) ;

if (readoutBoard == "CARTESIAN")  {
	  TString planeX           = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Float_t sizeX            = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Int_t   nbOfConnectorsX  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Int_t   orientationX     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();

	  TString planeY           = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Float_t sizeY            = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Int_t   nbOfConnectorsY  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Int_t   orientationY     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  SetCartesianStripsReadoutMap(readoutBoard, detectorType, detector, detID, planeX, sizeX, nbOfConnectorsX, orientationX, planeY, sizeY, nbOfConnectorsY, orientationY) ;
	}

//===========================================================================================
// Implimenting Straight strip and large ZZ sectors (WIP)[Jared Hadley 2021] VVVV
//===========================================================================================

	else if (readoutBoard == "FITZZAPV3")  {
	  TString plane           = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Float_t size            = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Int_t   nbOfConnectors  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Int_t   orientation     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();

	  SetFITZZAPV3ReadoutMap(readoutBoard, detectorType, detector, detID, plane, size, nbOfConnectors, orientation) ;
	}
//===========================================================================================
// Implimenting Straight strip and large ZZ sectors (WIP)[Jared Hadley 2021] /\/\/\/
//===========================================================================================

	else if (readoutBoard == "1DSTRIPS")  {
	  TString plane           = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Float_t size            = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Int_t   nbOfConnectors  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Int_t   orientation     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Set1DStripsReadoutMap(readoutBoard, detectorType, detector, detID, plane, size, nbOfConnectors, orientation);
	}

        else if (readoutBoard == "UV_ANGLE_OLD") {
	  TString planeX           = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Float_t sizeX            = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Int_t   nbOfConnectorsX  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Int_t   orientationX     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();

	  TString planeY           = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Float_t sizeY            = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Int_t   nbOfConnectorsY  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Int_t   orientationY     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  SetCartesianStripsReadoutMap(readoutBoard, detectorType, detector, detID, planeX, sizeX, nbOfConnectorsX, orientationX, planeY, sizeY, nbOfConnectorsY, orientationY) ;
	}
	
        else if (readoutBoard == "UV_ANGLE") {
	  Float_t length           = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Float_t outerRadius      = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Float_t innerRadius      = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();

	  TString planeTop           = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Int_t   nbOfConnectorsTop  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Int_t   orientationTop     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();

	  TString planeBot           = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Int_t   nbOfConnectorsBot  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Int_t   orientationBot     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  //	  SetUVStripsReadoutMap(readoutBoard, detectorType, detector, detID, length, innerRadius, outerRadius, planeTop, nbOfConnectorsTop, orientationTop, planeBot, nbOfConnectorsBot, orientationBot) ;
	  SetTrapezoid_UV_ReadoutMap(readoutBoard, detectorType, detector, detID, length, innerRadius, outerRadius, planeTop, nbOfConnectorsTop, orientationTop, planeBot, nbOfConnectorsBot, orientationBot) ;
	}

        else if (readoutBoard == "SBS_UV_STRIPS") {
	  Float_t length           = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Float_t width      = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Float_t angle      = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();

	  TString planeTop           = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Int_t   nbOfConnectorsTop  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Int_t   orientationTop     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();

	  TString planeBot           = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Int_t   nbOfConnectorsBot  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  Int_t   orientationBot     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  //	  SetUVStripsReadoutMap(readoutBoard, detectorType, detector, detID, length, innerRadius, outerRadius, planeTop, nbOfConnectorsTop, orientationTop, planeBot, nbOfConnectorsBot, orientationBot) ;
	  SetRectangle_UV_ReadoutMap(readoutBoard, detectorType, detector, detID, length, width, angle, planeTop, nbOfConnectorsTop, orientationTop, planeBot, nbOfConnectorsBot, orientationBot) ;
	}

	else if (readoutBoard == "PADPLANE") {
	  Float_t padSizeX     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  printf("    ==> SRSMapping::LoadDefaultMapping() ==>  ==> padSizeX = %f \n", padSizeX ) ;
	  Int_t nbPadX       = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  printf("    ==> SRSMapping::LoadDefaultMapping() ==>  ==> nbPadX = %d \n", nbPadX) ;
	  Float_t padSizeY     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  printf("    ==> SRSMapping::LoadDefaultMapping() ==>  ==> padSizeY = %f \n", padSizeY ) ;
	  Int_t nbPadY       = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  printf("    ==> SRSMapping::LoadDefaultMapping() ==>  ==> nbPadY = %d \n", nbPadY) ;
	  Int_t nbConnectors = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  printf("    ==> SRSMapping::LoadDefaultMapping() ==>  ==> nbConnectors = %d \n", nbConnectors ) ;
	  SetPadsReadoutMap(readoutBoard, detectorType, detector, detID, padSizeX, padSizeY, nbPadX, nbPadY, nbConnectors) ;
	}

	else if (readoutBoard == "CMSGEM") {
	  TString etaSector     = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
	  Float_t etaSectorPos = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Float_t etaSectorSize = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Float_t nbConnectors        = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atof();
	  Int_t orientation     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	  SetCMSGEMReadoutMap(readoutBoard, detectorType, detector, detID, etaSector, etaSectorPos, etaSectorSize, nbConnectors, orientation) ;
	}

	else {
	  printf("XXXXXXX SRSMapping::LoadDefaultMapping()==> detector with this readout board type %s is not yet implemented ==> PLEASE MOVE ON XXXXXXXXXXX \n", readoutBoard.Data()) ;
	  continue ;
	}
	detID++ ;
      }

      if(s == "APV") {
	Int_t   fecId     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	Int_t   adcCh     = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
        TString detPlane  = ((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' );
        Int_t   apvOrient = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	Int_t   apvIndex  = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	Int_t   apvheader = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	if (detPlane == "NULL") continue ;
	SetAPVMap(detPlane, fecId, adcCh, apvNo, apvOrient, apvIndex, apvheader) ;
	apvNo++ ;
      }
    }
   tokens->Delete();
  }
  printf("================================\n") ;
}

//============================================================================================
/**
void SRSMapping::LoadAPVtoPadMapping(const char * mappingCfgFilename) {
  //  Clear() ;
  printf("  SRSMapping::LoadAPVtoPadMapping() ==> Loading Mapping from %s \n", mappingCfgFilename) ;
  ifstream filestream (mappingCfgFilename, ifstream::in); 
  TString line;
  while (line.ReadLine(filestream)) {

    line.Remove(TString::kLeading, ' ');   // strip leading spaces
    if (line.BeginsWith("#")) continue ;   // and skip comments
    //    printf("   SRSMapping::LoadAPVtoPadMapping ==> Scanning the mapping cfg file %s\n",line.Data()) ;

    //=== Create an array of the tokens separated by "," in the line;
    TObjArray * tokens = line.Tokenize(","); 

    //=== iterator on the tokens array
    TIter myiter(tokens); 
    while (TObjString * st = (TObjString*) myiter.Next()) {

      //== Remove leading and trailer spaces
      TString s = st->GetString().Remove(TString::kLeading, ' ' );
      s.Remove(TString::kTrailing, ' ' );                         
      if(s == "PAD") {
	Int_t apvCh      = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	Int_t padId      = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )) .Atoi();;
	Int_t fecId      = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	Int_t adcCh      = (((TObjString*) myiter.Next())->GetString().Remove(TString::kLeading, ' ' )).Atoi();
	SetAPVtoPadMapping(fecId, adcCh, padId, apvCh) ;
      }
    }
   tokens->Delete();
  }
  printf("\n  ======================================================================================================================\n") ;
}
*/

//============================================================================================
void SRSMapping::Clear() {
  printf("  SRSMapping::Clear() ==> Clearing Previous Mapping \n") ;

  fAPVIDFromAPVNoMap.clear() ; 
  fAPVIDFromNameMap.clear() ; 
  fAPVIDListFromDetectorMap.clear() ; 
  fAPVIDListFromDetectorPlaneMap.clear() ; 
  fAPVNoFromIDMap.clear() ; 
  fAPVFromIDMap.clear() ; 
  fAPVHeaderLevelFromIDMap.clear() ;

  fPlaneIDFromPlaneMap.clear() ;
  fDetectorIDFromDetectorMap.clear() ; 
  fDetectorFromIDMap.clear() ; 
  fDetectorFromAPVIDMap.clear() ;  
  fDetectorFromPlaneMap.clear() ;

  fDetectorPlaneFromAPVIDMap.clear() ;

  fReadoutBoardFromIDMap.clear() ;
  fReadoutBoardFromDetectorMap.clear() ;

  fNbOfAPVsFromDetectorMap.clear() ;

  fAPVOrientationFromIDMap.clear() ;
  fAPVIndexOnPlaneFromIDMap.clear() ;

  printf("  SRSMapping::Clear() ==> Previous Mapping cleared \n") ;
}

//============================================================================================
template <typename M> void ClearMapOfList( M & amap ) {
  for ( typename M::iterator it = amap.begin(); it != amap.end(); ++it ) {
    ((*it).second).clear();
  }
  amap.clear() ;
}


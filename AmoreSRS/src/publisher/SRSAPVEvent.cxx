#include "SRSAPVEvent.h"

ClassImp(SRSAPVEvent);

//=====================================================
SRSAPVEvent::SRSAPVEvent(Int_t fec_no, Int_t fec_channel, Int_t apv_id, Float_t zeroSupCut, Int_t EventNb, Int_t packetSize) {
  //  printf("SRSAPVEvent::SRSAPVEvent \n" );
  Clear();
  SRSMapping * mapping = SRSMapping::GetInstance();

  fPlane        = "NOGEMX";
  fDetector     = "NOGEM";
  fReadoutBoard = "CARTESIAN";
  fDetectorType = "STANDARD";

  fDetSizeX = 100.0;
  fDetSizeY = 100.0;

  fPadSizeX = 10.0;
  fPadSizeY = 10.0;
  fNbOfPadX = 10;
  fNbOfPadY = 10;

  fPlaneSize       = 102.4;
  fAPVIndexOnPlane = 2;
  fAPVOrientation  = 0;
  fAPVHeaderLevel  = 1300;
  fEtaSectorPos = 0;

  fTrapezoidDetOuterRadius = 430.0;
  fTrapezoidDetInnerRadius = 220.0;

  fPedSubFlag           = kFALSE;
  fCommonModeFlag       = kFALSE;
  fIsCosmicRunFlag      = kFALSE;
  fIsPedestalRunFlag    = kFALSE;
  fIsRawPedestalRunFlag = kFALSE;
  fIsActiveChannel      = kTRUE;

  fFECNo = fec_no;
  fAPVID = apv_id;

  fADCChannel = fec_channel;
  fPacketSize = packetSize;

  fEventNb = EventNb;
  fZeroSupCut = zeroSupCut;

  fAPVKey          = mapping->GetAPVNoFromID(apv_id);
  fAPV             = mapping->GetAPVFromID(apv_id);
  fAPVHeaderLevel  = mapping->GetAPVHeaderLevelFromID(apv_id);
  fAPVIndexOnPlane = mapping->GetAPVIndexOnPlane(apv_id);
  fAPVOrientation  = mapping->GetAPVOrientation(apv_id);

  fPlane          = mapping->GetDetectorPlaneFromAPVID(apv_id);
  fDetector       = mapping->GetDetectorFromPlane(fPlane);
  fReadoutBoard   = mapping->GetReadoutBoardFromDetector(fDetector);

	//std::cout << "Should be FITZZAPV3: " << fReadoutBoard << std::endl;

  fDetectorType   = mapping->GetDetectorTypeFromDetector(fDetector);
	//std::cout << "Should be STANDARD: " << fDetectorType << std::endl;

   if(fReadoutBoard == "CARTESIAN") {
    fPlaneSize         = (mapping->GetCartesianReadoutMap(fPlane))[1];  
    fNbOfAPVsFromPlane = (Int_t) ((mapping->GetCartesianReadoutMap(fPlane)) [2]);
    //    printf("SRSAPVEvent::SRSAPVEvent  fPlane=%s, planeSize=%.1f, fNbOfAPVsFromPlane=%, fAPVOrientation=%d\n", fPlane.Data(), fPlaneSize, fNbOfAPVsFromPlane, fAPVOrientation);
  }

//===========================================================================================
// Implimenting Straight strip and large ZZ sectors (WIP)[Jared Hadley 2021] VVVV
//===========================================================================================


  else if(fReadoutBoard == "FITZZAPV3") {
    fPlaneSize         = (mapping->GetFITZZAPV3ReadoutMap(fPlane))[1]; 
    fNbOfAPVsFromPlane = (Int_t) ((mapping->GetFITZZAPV3ReadoutMap(fPlane)) [2]);
    //printf("SRSAPVEvent::SRSAPVEvent  fPlane=%s, planeSize=%.1f, fNbOfAPVsFromPlane=%, fAPVOrientation=%d\n", fPlane.Data(), fPlaneSize, fNbOfAPVsFromPlane, fAPVOrientation);
   }

//==========================================================================================
// Implimenting Straight strip and large ZZ sectors (WIP)[Jared Hadley 2021] /\/\/\/\
//===========================================================================================

  else if(fReadoutBoard == "UV_ANGLE") {
    fTrapezoidDetLength      = (mapping->GetUVangleReadoutMap(fDetector)) [0];
    fTrapezoidDetInnerRadius = (mapping->GetUVangleReadoutMap(fDetector)) [1];
    fTrapezoidDetOuterRadius = (mapping->GetUVangleReadoutMap(fDetector)) [2];
    fNbOfAPVsFromPlane       = (Int_t) ((mapping->GetUVangleReadoutMap(fPlane)) [2]);
    //printf("SRSAPVEvent::SRSAPVEvent => fDetector =%s,  fPlane=%s, detLength=%.1f, innerRadius=%.1f, outerRadius=%.1f, fNbOfAPVsFromPlane=%d \n",fDetector.Data(), fPlane.Data(), fTrapezoidDetLength, fTrapezoidDetInnerRadius, fTrapezoidDetOuterRadius, fNbOfAPVsFromPlane); 
  }

  else if(fReadoutBoard == "SBS_UV_STRIPS") {
    fDetectorLength   = (mapping->GetUVangleReadoutMap(fDetector)) [0];
    fDetetectorWidth  = (mapping->GetUVangleReadoutMap(fDetector)) [1];
    fDetectorUVangle  = (mapping->GetUVangleReadoutMap(fDetector)) [2];
    fNbOfAPVsFromPlane = (Int_t) ((mapping->GetUVangleReadoutMap(fPlane)) [2]);
    //    printf("SRSAPVEvent::SRSAPVEvent => fDetector =%s,  fPlane=%s, detLength=%.1f, detWidthth=%.1f, detUVangle=%.1f, fNbOfAPVsFromPlane=%d \n",fDetector.Data(), fPlane.Data(), fDetectorLength, fDetetectorWidth, fDetectorUVangle, fNbOfAPVsFromPlane); 
  }

  else if(fReadoutBoard == "1DSTRIPS") {
    fPlaneSize = (mapping->Get1DStripsReadoutMap(fPlane)) [1];
    fNbOfAPVsFromPlane = (Int_t) ((mapping->Get1DStripsReadoutMap(fPlane)) [2]);
    //    printf("SRSAPVEvent::SRSAPVEvent  fPlane=%s, planeSize=%.1f, fNbOfAPVsFromPlane=%d \n", fPlane.Data(), fPlaneSize, fNbOfAPVsFromPlane); 
  }

  else if(fReadoutBoard == "CMSGEM") {
    fEtaSectorPos = (mapping->GetCMSGEMReadoutMap(fPlane)) [0]; 
    fPlaneSize    = (mapping->GetCMSGEMReadoutMap(fPlane)) [1];
    fNbOfAPVsFromPlane = (Int_t) ((mapping->GetCMSGEMReadoutMap(fPlane)) [1]);
    //    printf("SRSAPVEvent::SRSAPVEvent   fPlane=%s, etaSectorPos=%.1f, planeSize=%.1f, fNbOfAPVsFromPlane=%d, \n", fPlane.Data(), fEtaSectorPos, fPlaneSize, fNbOfAPVsFromPlane); 
  }

  else if (fReadoutBoard == "PADPLANE") {
    fPadSizeX          = (mapping->GetPadDetectorMap(fDetector)) [0];
    fPadSizeY          = (mapping->GetPadDetectorMap(fDetector)) [1];
    fNbOfPadX          = (Int_t) (mapping->GetPadDetectorMap(fDetector)) [2];
    fNbOfPadY          = (Int_t) (mapping->GetPadDetectorMap(fDetector)) [3];
    fNbOfAPVsFromPlane = (Int_t) (mapping->GetPadDetectorMap(fDetector)) [4];

    fDetSizeX = fPadSizeX * fNbOfPadX;
    fDetSizeY = fPadSizeY * fNbOfPadY;

    Int_t detId = mapping->GetDetectorIDFromDetector(fDetector);
    //    printf("SRSAPVEvent::SRSAPVEvent => nbPadX[%s]=%d, nbPadY[%s]=%d, padSizeX[%s]=%.1f, padSizeY[%s]=%.1f, detSizeX[%s]=%.1f, detSizeY[%s]=%.1f, nbCon[%s]=%d\n",fDetector.Data(), fNbOfPadX, fDetector.Data(), fNbOfPadY, fDetector.Data(), fPadSizeX, fDetector.Data(), fPadSizeY, fDetector.Data(), fDetSizeX, fDetector.Data(), fDetSizeY, fDetector.Data(), fNbOfAPVsFromPlane); 

    /**
    vector< Int_t> apvChPadChannel = mapping->GetPadChannelsMapping(detId);
    vector< Int_t>::const_iterator apvChPadCh_itr;
    for (apvChPadCh_itr = apvChPadChannel.begin(); apvChPadCh_itr != apvChPadChannel.end(); ++apvChPadCh_itr) { 
      Int_t apvChPadCh = * apvChPadCh_itr; 
      Int_t padCh =  (apvChPadCh >> 8) & 0xffff;
      Int_t apvCh = apvChPadCh & 0xff;
      //      printf("SRSAPVEvent::SRSAPVEvent APV to Pad mapping \n"); 
    }
    apvChPadChannel.clear(); 
    */
  }
  else {
      printf("Doesnt Get Past SRSAPVEvent::SRSAPVEvent Non Identified Readout Board Type \n"); 
  }
  //  if (fDetector.Contains("TrkGEM")) fZeroSupCut = 5;
  //  printf("SRSAPVEvent::SRSAPVEvent   fPlane=%s, etaSectorPos=%.1f, planeSize=%.1f, fNbOfAPVsFromPlane=%d, \n", fPlane.Data(), fEtaSectorPos, fPlaneSize, fNbOfAPVsFromPlane);
}

//=====================================================
void SRSAPVEvent::Clear() {
  ClearListOfHits();
  fRawData32bits.clear();
  fRawData16bits.clear();
  fapvTimeBinDataMap.clear();

  fPedestalData.clear();
  fRawPedestalData.clear();

  fPedestalNoises.clear();
  fRawPedestalNoises.clear();

  fPedestalOffsets.clear();
  fRawPedestalOffsets.clear();

  fCommonModeOffsets.clear();
  fCommonModeOffsets_odd.clear();
  fCommonModeOffsets_even.clear();
  fComModeFluctData.clear();
  //  printf(" === SRSAPVEvent::Clear()\n");
}

//=====================================================
SRSAPVEvent::~SRSAPVEvent(){
  Clear();
}

//=====================================================
void SRSAPVEvent::ClearListOfHits() {
  //  list <SRSHit * >::const_iterator itr;
  // for (itr = fListOfHits.begin(); itr != fListOfHits.end(); itr++) delete * itr  ;
  fListOfHits.clear();
}
//=====================================================

void SRSAPVEvent::Add32BitsRawData(UInt_t rawData32bits) {
  //  printf("SRSAPVEvent::Add32BitsRawData()\n"); 
  fRawData32bits.push_back(rawData32bits); 
}

//=====================================================
void SRSAPVEvent::Set32BitsRawData(vector<UInt_t> rawData32bits) {
  //  printf("SRSAPVEvent::Set32BitsRawData()\n"); 
  fRawData32bits.clear();
  fRawData32bits = rawData32bits; 
}

//=====================================================
Int_t  SRSAPVEvent::APVchannelCorrection(Int_t chNo) {
  //std::cout << "chNo before: " << chNo << std::endl; 
  chNo = (32 * (chNo%4)) + (8 * (Int_t)(chNo/4)) - (31 * (Int_t)(chNo/16));
  //std::cout << "chNo after: " << chNo << std::endl;
  return chNo;
}

//=====================================================
Int_t SRSAPVEvent::NS2StripMapping(Int_t chNo) { 
  if((chNo%2)==1){
    chNo=((chNo-1)/2)+64;
  }
  else{
    chNo=(chNo/2);
  }
  return chNo;
}

//=====================================================
Int_t SRSAPVEvent::CMSStripMapping(Int_t chNo) { 
  if((chNo%2)==1){
    chNo= 127 -((chNo-1)/2);
  }
  else{
    chNo = (chNo/2);
  }
  return chNo;
}

//=====================================================
Int_t SRSAPVEvent::MMStripMappingAPV1(Int_t chNo){
  if((chNo%2)==1){
    chNo=((chNo-1)/2) + 32;
  }
  else{
    chNo=(chNo/2);
    if (chNo < 32) chNo = 31 - chNo;
    else if (chNo > 37) chNo = 159 - chNo;
    else chNo += 90;
  }
  return chNo;
}

//=====================================================
Int_t SRSAPVEvent::MMStripMappingAPV2(Int_t chNo){
  if((chNo%2)==1){
    chNo=((chNo-1)/2) + 27;
  }
  else{
    chNo=(chNo/2);
    if (chNo < 27) chNo = 26 - chNo;
    else if (chNo > 38) chNo = 154 - chNo;
    else chNo += 89;
  }
  return chNo;
}

//=====================================================
Int_t SRSAPVEvent::MMStripMappingAPV3(Int_t chNo){
  if((chNo%2)==1){
    chNo=((chNo-1)/2) + 26;
  }
  else{
    chNo=(chNo/2);
    if (chNo < 26) chNo = 25 - chNo;
    else if (chNo > 31) chNo = 153 - chNo;
    else chNo += 96;
  }
  return chNo;
}

//=====================================================
Int_t SRSAPVEvent::StandardMapping(Int_t chNo) {
  
  //cout << "APV comes from: " << fPlane << endl;
  //cout << "strip is: " << chNo << endl;
  return chNo;
}

//=====================================================
Int_t SRSAPVEvent::EICStripMapping(Int_t chNo) { 
  if(chNo % 2 == 0) chNo = chNo / 2;
  else              chNo = 64 + (chNo - 1) / 2;
  return chNo;
}

//=====================================================
Int_t SRSAPVEvent::SBSUVStripMapping(Int_t chNo) { 
  if (chNo % 2 == 0)    chNo = ( chNo / 2) + 32 ;
  else 
    if (chNo < 64) chNo = (63 - chNo) / 2 ;
    else chNo = 127 + (65 - chNo) / 2 ;
  //   printf("SRSAPVEvent::SBSUVStripMapping ==>  APVID=%d, chNo=%d, stripNo=%d, \n",fAPVID, chno, chNo) ;
  return chNo ;
}

//=====================================================
Int_t SRSAPVEvent::HMSStripMapping(Int_t chNo) { 
  if(chNo % 4 == 0)      chNo = chNo + 2;
  else if(chNo % 4 == 1) chNo = chNo - 1;
  else if(chNo % 4 == 2) chNo = chNo + 1;
  else if(chNo % 4 == 3) chNo = chNo - 2;
  else chNo = chNo;
  return chNo;
}

//=====================================================
//Int_t SRSAPVEvent::FITZZAPV3StripMapping(Int_t chNo) { 
//  if(chNo%2==0) chNo = chNo/2;
//  else  chNo = (chNo-1)/2 + 64;
//  return chNo ;
//}

//=====================================================
Int_t SRSAPVEvent::FITZZMIDStripMapping(Int_t chNo) { 
  if(chNo < 64) {chNo = 2*chNo;}
  else  {chNo = 2*(chNo-64)+1;}
  return chNo ;
}
//======================================================
//Int_t SRSAPVEvent::FITZZMIDEOStripMapping(Int_t chNo) {
//	int chNo2 = 0
//  if(chNo < 64) {chNo2 = 2*chNo;}
//  else  {chNo2 = 2*(chNprintf("Does FITZZ go in here?");o-64)+1;}
//
//  if(chNo2%2==0) chNo = chNo2/2;
//  else  {chNo = (chNo2-1)/2 + 64;}
//
//  return chNo ;
//}

//====================================================================================================
// Attempting to impliment FIT EIC ZZ with APVs as seperate "detector types" (WIP) [Jared Hadley 2021]
// strip mapping functions are named for the apv/panasonic connector location on Aiwu's Altium ROB file
// >FITZZ mapping update Begin
//====================================================================================================
Int_t SRSAPVEvent::FITZZAPV1StripMapping(Int_t chNo) {
	//std::cout << "chNo: " << chNo << std::endl;
	if((chNo%2)==0) chNo = 64 + (chNo/2);
	else  	        chNo = (chNo-1)/2;
	//std::cout << "new chNo: " << chNo << std::endl;
	return chNo;
}

Int_t SRSAPVEvent::FITZZAPV2StripMapping(Int_t chNo) {
	if((chNo%2)==0) chNo = chNo + 1;
	else            chNo = chNo - 1;
	return chNo;
} 

Int_t SRSAPVEvent::FITZZAPV3StripMapping(Int_t chNo) {
	if((chNo%2)==0) chNo = 64 + (chNo/2);
	else            chNo = (chNo-1)/2;
	return chNo;
}

Int_t SRSAPVEvent::FITZZAPV4StripMapping(Int_t chNo) {
	return chNo;
} 

Int_t SRSAPVEvent::FITZZAPV5StripMapping(Int_t chNo) { 
	return chNo;
}

Int_t SRSAPVEvent::FITZZAPV6StripMapping(Int_t chNo) { 
	return chNo;
}

Int_t SRSAPVEvent::FITZZAPV7StripMapping(Int_t chNo) { 
	if((chNo%2)==0) chNo = 64 + (chNo/2);
	else            chNo = (chNo-1)/2;
	return chNo;
}

Int_t SRSAPVEvent::FITZZAPV8StripMapping(Int_t chNo) { 
	if((chNo%2)==0) chNo = chNo + 1;
	else            chNo = chNo - 1;
	return chNo;
} 

Int_t SRSAPVEvent::FITZZAPV9StripMapping(Int_t chNo) { 
	if((chNo%2)==0) chNo = chNo/2;
	else            chNo = ((chNo-1)/2) + 64;
	return chNo;
}
// >FITZZ mapping update END
//====================================================================================================
Int_t SRSAPVEvent::StripMapping(Int_t chNo) {
  chNo = APVchannelCorrection(chNo);
  if      (fDetectorType == "CMSGEM")    chNo = CMSStripMapping(chNo);
  else if (fDetectorType == "ZIGZAG")    chNo = ZigZagStripMapping(chNo);
  else if (fDetectorType == "NS2")       chNo = NS2StripMapping(chNo);
  else if (fDetectorType == "EICPROTO1") chNo = EICStripMapping(chNo);
  else if (fDetectorType == "HMSGEM")    chNo = HMSStripMapping(chNo);
  else if (fDetectorType == "SBS_UVGEM") chNo = SBSUVStripMapping(chNo);
  else if (fDetectorType == "FITEO") chNo = FITZZAPV3StripMapping(chNo);
  else if (fDetectorType == "FITMID") chNo = FITZZMIDStripMapping(chNo);
  else if (fDetectorType == "FITEOMID") chNo = FITZZMIDStripMapping(FITZZAPV3StripMapping(chNo));
  else if (fDetectorType == "FITMIDEO") chNo = FITZZAPV3StripMapping(FITZZMIDStripMapping(chNo));
//FITZZ Implimentation Begin
  else if (fDetectorType == "FITZZAPV1") chNo = FITZZAPV1StripMapping(chNo);
  else if (fDetectorType == "FITZZAPV2") chNo = FITZZAPV2StripMapping(chNo);
  else if (fDetectorType == "FITZZAPV3") chNo = FITZZAPV3StripMapping(chNo);
  else if (fDetectorType == "FITZZAPV4") chNo = FITZZAPV4StripMapping(chNo);
  else if (fDetectorType == "FITZZAPV5") chNo = FITZZAPV5StripMapping(chNo);
  else if (fDetectorType == "FITZZAPV6") chNo = FITZZAPV6StripMapping(chNo);
  else if (fDetectorType == "FITZZAPV7") chNo = FITZZAPV7StripMapping(chNo);
  else if (fDetectorType == "FITZZAPV8") chNo = FITZZAPV8StripMapping(chNo);
  else if (fDetectorType == "FITZZAPV9") chNo = FITZZAPV9StripMapping(chNo);
//FITZZ Implimentation End
  else                                   chNo = StandardMapping(chNo);
  return chNo;
}

//========================================================================================================================
static Bool_t usingGreaterThan(Float_t u, Float_t v) {
  return u > v ;
}

//=====================================================
void SRSAPVEvent::Print() {
  //  printf("SRSAPVEvent::Print() => Printing APV Data: -1 == No Data, -2 == Underflow, -3 == Overflow\n"); 
  Int_t Size = fRawData32bits.size(); 
  Int_t Capacity = fRawData32bits.capacity(); 
  Int_t MaxSize =  fRawData32bits.max_size(); 
  vector<UInt_t>::const_iterator rawdata_itr; 
  for (rawdata_itr = fRawData32bits.begin(); rawdata_itr != fRawData32bits.end(); ++rawdata_itr) { 
    Int_t rawdata = * rawdata_itr; 
    printf("SRSAPVEvent::Print() ====> 32 bits raw data [0x%x]\n",rawdata); 
  }
}

//=====================================================
void SRSAPVEvent::ComputeRawData16bits() {
  //  printf("SRSAPVEvent::ComputeRawData()==> fRawData32bits.size() = %d \n",(Int_t) (fRawData32bits.size()));
  fRawData16bits.clear();
  vector<UInt_t>::const_iterator rawData_itr;
  for (rawData_itr = fRawData32bits.begin(); rawData_itr != fRawData32bits.end(); ++rawData_itr) {
    UInt_t word32bit = * rawData_itr;
    if (((word32bit >> 8) & 0xffffff) != 0x414443) {
      UInt_t data1 = (word32bit>> 24) & 0xff;
      UInt_t data2 = (word32bit >> 16) & 0xff;
      UInt_t data3 = (word32bit >> 8)  & 0xff;
      UInt_t data4 = word32bit  & 0xff;
      fRawData16bits.push_back(((data2 << 8) | data1));
      fRawData16bits.push_back(((data4 << 8) | data3));
    }
  }
}

//=====================================================
void SRSAPVEvent::ComputeTimeBinCommonMode() {
  // printf("  SRSAPVEvent::ComputeTimeBinCommonMode()==>enter \n");

  Int_t idata = 0 ;
  Bool_t startDataFlag = kFALSE;

  fapvTimeBinDataMap.clear();
  UInt_t apvheaderlevel = (UInt_t)  fAPVHeaderLevel;
  Int_t size = fRawData16bits.size();

  if (size != fPacketSize) {
    printf("SRSAPVEvent::ComputeTimeBinCommonMode() XXXX ERROR XXXXX ==> Packet size %d different from expected %d, header=%d \n",size,fPacketSize, fAPVHeaderLevel );
  }

  std::list<Float_t> commonModeOffset,  commonModeOffset_odd, commonModeOffset_even;
  Float_t apvBaseline = 4096 - TMath::Mean(fPedestalOffsets.begin(), fPedestalOffsets.end());

  while(idata < size) {
    //    printf("    SRSAPVEvent::ComputeTimeBinCommonMode()  ==> idata=%d, apvBaseline=%.1f \n",idata, apvBaseline);
    //===============================================================//
    // If 3 consecutive words satisfy this condition below => it is  //
    // an APV header so we could take meaninfull data                //
    //===============================================================//
    if (fRawData16bits[idata] < apvheaderlevel ) {
      idata++;
      if (fRawData16bits[idata] < apvheaderlevel ) {
        idata++;
        if (fRawData16bits[idata] < apvheaderlevel ) {
          idata += 10;
          startDataFlag = kTRUE;
	  continue;
        }
      }
    }

    //===============================================================//
    // That's where the meaninfull data are taken                    //
    // 128 analog word for each apv strip and each timebin           //
    //===============================================================//
    if (startDataFlag == kTRUE) {
      commonModeOffset.clear();
      commonModeOffset_odd.clear();
      commonModeOffset_even.clear();

      Float_t commMode      = 0;
      Float_t commMode_odd  = 0;
      Float_t commMode_even = 0;

      for(Int_t chNo = 0; chNo < NCH; ++chNo) {

	Int_t stripNo = StripMapping(chNo);

	Float_t rawdata = ((Float_t) fRawData16bits[idata]);
	Float_t comMode = rawdata;

	Float_t thresohld = 500;

	if ((fReadoutBoard == "UV_ANGLE_OLD") &&  (fDetectorType == "EICPROTO1") ) thresohld = 500 	;
	if (fabs(comMode - apvBaseline) > thresohld) {
	  comMode = apvBaseline;
	}

	rawdata = 4096 - rawdata;
	comMode = 4096 - comMode;

	if (fIsCosmicRunFlag) {
	  if ((fReadoutBoard == "UV_ANGLE_OLD") &&  (fDetectorType == "EICPROTO1") ) {
	    if(stripNo < 64) commonModeOffset_even.push_back(comMode - fPedestalOffsets[stripNo]);
	    else             commonModeOffset_odd.push_back(comMode - fPedestalOffsets[stripNo]);
	  }
	  else {
	    commonModeOffset.push_back(comMode - fPedestalOffsets[stripNo]);
	  }
	}
   
	if (fIsPedestalRunFlag) {
	  if ((fReadoutBoard == "UV_ANGLE_OLD") &&  (fDetectorType == "EICPROTO1") ) {
	    if(stripNo < 64) commonModeOffset_even.push_back(comMode - fRawPedestalOffsets[stripNo]);
	    else             commonModeOffset_odd.push_back(comMode - fRawPedestalOffsets[stripNo]);
	  }
	  else {
	    commonModeOffset.push_back(comMode - fRawPedestalOffsets[stripNo]);
	  }
	}
	fapvTimeBinDataMap.insert(pair<Int_t, Float_t>(stripNo, rawdata));
	idata++;
      }

      if ((fReadoutBoard == "UV_ANGLE_OLD") &&  (fDetectorType == "EICPROTO1") ) {
	commonModeOffset_odd.sort();
	commonModeOffset_even.sort();

	commMode_odd  = TMath::Mean(commonModeOffset_odd.begin(),commonModeOffset_odd.end());
	commMode_even = TMath::Mean(commonModeOffset_even.begin(),commonModeOffset_even.end());
	commonModeOffset_odd.clear();
	commonModeOffset_even.clear();
      }
      else {
	commonModeOffset.sort();
	commMode = TMath::Mean(commonModeOffset.begin(),commonModeOffset.end());
	commonModeOffset.clear();
      }

      if (fIsRawPedestalRunFlag) {
	if ((fReadoutBoard == "UV_ANGLE_OLD") &&  (fDetectorType == "EICPROTO1") ) {
	  fCommonModeOffsets_odd.push_back(0);
	  fCommonModeOffsets_even.push_back(0);
	}
	else {
	  fCommonModeOffsets.push_back(0);
	}
      }
      else {
	if ((fReadoutBoard == "UV_ANGLE_OLD") &&  (fDetectorType == "EICPROTO1") ) {
	  fCommonModeOffsets_odd.push_back(commMode_odd);
	  fCommonModeOffsets_even.push_back(commMode_even);
	}
	else { 
	  fCommonModeOffsets.push_back(commMode);
	  //	  fCommonModeOffsets.push_back(0);
	  //	      printf("  SRSAPVEvent::ComputeTimeBinCommonMode()==> Exit \n");
	}
      }
      startDataFlag = kFALSE;
      continue;
    }
    idata++;
  }

  commonModeOffset.clear();
  commonModeOffset_odd.clear();
  commonModeOffset_even.clear();

  // printf("  SRSAPVEvent::ComputeTimeBinCommonMode()==>exit \n");
}

//===================================================================
void SRSAPVEvent::ComputeListOfAPVHits() {
  //  printf("SRSAPVEvent::ComputeListOfAPVHits()==> enter \n");
  //std::cout << "Rawpeds? \n \n \n \n" << endl;
  fIsActiveChannel = kTRUE;
  fIsCosmicRunFlag = kTRUE;
  fIsPedestalRunFlag = kFALSE;
  fIsRawPedestalRunFlag = kFALSE;

  ComputeRawData16bits();
  ComputeTimeBinCommonMode();

  fListOfHits.clear();
  Int_t apvTimeBinDataMapSize = fapvTimeBinDataMap.size();
  Int_t padNo = 0;  
  TString plane = fPlane;

  if (apvTimeBinDataMapSize != 0) {
    std::vector <Float_t> stripPedestalNoise;
    pair<multimap<Int_t, Float_t>::iterator, multimap<Int_t, Float_t>::iterator> stripSetOfTimeBinRawData;

    for (Int_t stripNo = 0;  stripNo < NCH; stripNo++) {
      vector<Float_t> timeBinADCs;
      Int_t timebin = 0;

      stripSetOfTimeBinRawData = fapvTimeBinDataMap.equal_range(stripNo);

      multimap <Int_t, Float_t>::iterator timebin_it;
      for (timebin_it = stripSetOfTimeBinRawData.first; timebin_it != stripSetOfTimeBinRawData.second; ++timebin_it) {
	Float_t rawdata =  timebin_it->second;

	// BASELINE CORRECTION
	if (fCommonModeFlag ) {
          //printf("Does FIT&& fDetectorType != "FITZZAPV3") {ZZ go in here?");
	  if ((fReadoutBoard == "UV_ANGLE_OLD") &&  (fDetectorType == "EICPROTO1") ) {
	    //printf("Does FITZZ go in here?");
	    if(stripNo < 64)  rawdata -= fCommonModeOffsets_even[timebin];
	    else              rawdata -= fCommonModeOffsets_odd[timebin];
	  }
	  else  {
            //printf("Does FITZZ take the else statement?
	    rawdata -= fCommonModeOffsets[timebin];
	  }
	}

	// Pedestal Offset Suppression
	if(fPedSubFlag){// && fPlaneType != "FITZZ*") {

            rawdata -= fPedestalOffsets[stripNo];
	    //printf(fDetectorType);
            } 
        timeBinADCs.push_back(rawdata);
	timebin++;               
      }

      /**
      if(fReadoutBoard == "PADPLANE") {
	padNo = stripNo;
      }
      */

      Int_t stripNb = stripNo;
      if ((fReadoutBoard == "UV_ANGLE_OLD") &&  (fDetectorType == "EICPROTO1") ) {
	if(stripNo > 63) {
	  stripNb = stripNo - 64;
	  plane = fDetector + "BOT";
	}
	else    {
	  plane = fDetector + "TOP";
	}
      }

      // ZERO SUPPRESSION
      if (fZeroSupCut > 0.0){// && fDetectorType != "FITZZAPV3") {
        //printf("Does FITZZ go into zerosup?");
	//cout << fDetectorType << endl;	
	Float_t zsCut = fZeroSupCut;
	if( TMath::Mean(timeBinADCs.begin(), timeBinADCs.end()) < zsCut * fPedestalNoises[stripNo] ) {
	  stripPedestalNoise.push_back(TMath::Mean(timeBinADCs.begin(), timeBinADCs.end()));
	  timeBinADCs.clear();
	  continue;
	}

	if ( (Int_t) (timeBinADCs.size()) != 0 ) { 
	  Float_t adcs = * (TMath::LocMax(timeBinADCs.begin(), timeBinADCs.end()));
	  if(adcs < 0) adcs = 0;
	  SRSHit * apvHit = new SRSHit(fAPVID, fAPVIndexOnPlane, fNbOfAPVsFromPlane, fAPVOrientation, fDetector, fReadoutBoard, fPlane, fDetectorType, fNbOfPadX, fNbOfPadY, fZeroSupCut, fIsHitMaxOrTotalADCs, kTRUE, fIsActiveChannel, stripNb, adcs) ;
	  apvHit->SetTimeBinADCs(timeBinADCs);
	  fListOfHits.push_back(apvHit);
	}
      }

      else {
	Float_t adcs = * (TMath::LocMax(timeBinADCs.begin(), timeBinADCs.end()));
	if( TMath::Mean(timeBinADCs.begin(), timeBinADCs.end()) < 5 * fPedestalNoises[stripNo] ) {
	  adcs = TMath::Mean(timeBinADCs.begin(), timeBinADCs.end());
	}
	SRSHit * apvHit = new SRSHit(fAPVID, fAPVIndexOnPlane, fNbOfAPVsFromPlane, fAPVOrientation, fDetector, fReadoutBoard, fPlane, fDetectorType, fNbOfPadX, fNbOfPadY, fZeroSupCut, fIsHitMaxOrTotalADCs, kFALSE, fIsActiveChannel, stripNb, adcs) ;
	apvHit->SetTimeBinADCs(timeBinADCs);
	fListOfHits.push_back(apvHit);
      }
      //	printf("SRSAPVEvent::ComputeListOfAPVHits()==> Det=%s, DetType=%s, Readout=%s zeroSupCut=%.1f\n", fDetector.Data(), fDetectorType.Data(), fReadoutBoard.Data(), fZeroSupCut);
      //	printf(" == SRSAPVEvent::ComputeListOfAPVHits()==> DetPlane=%s, StripNb=%d, adcs=%.1f \n",plane.Data(), stripNb, adcs);     
      timeBinADCs.clear();
    }
    stripPedestalNoise.clear();
  }
  fapvTimeBinDataMap.clear();
  //  printf("SRSAPVEvent::ComputeListOfAPVHits()==> exit \n");
}

//========================================================================================================================
void SRSAPVEvent::ComputeMeanTimeBinRawPedestalData() { 
  //  printf("SRSAPVEvent::ComputeMeanTimeBinRawPedestalData() \n");

  fPedSubFlag = kFALSE;
  fIsCosmicRunFlag = kFALSE;

  fIsPedestalRunFlag = kFALSE;
  fIsRawPedestalRunFlag = kTRUE;

  ComputeRawData16bits();
  ComputeTimeBinCommonMode();
  fRawPedestalData.clear();

  vector<Float_t>  meanTimeBinRawPedestalDataVect;
  pair<multimap<Int_t, Float_t>::iterator, multimap<Int_t, Float_t>::iterator> stripSetOfTimeBinRawData;
  for (Int_t stripNo = 0;  stripNo < NCH; stripNo++) {
    stripSetOfTimeBinRawData = fapvTimeBinDataMap.equal_range(stripNo);
    multimap<Int_t, Float_t>::iterator timebin_it;
    Int_t timebin = 0;
    for (timebin_it = stripSetOfTimeBinRawData.first; timebin_it != stripSetOfTimeBinRawData.second; ++timebin_it) {
       Float_t rawdata = timebin_it->second;

       if ((fReadoutBoard == "UV_ANGLE") &&  (fDetectorType == "EICPROTO1") ) {
	 if(stripNo < 64) meanTimeBinRawPedestalDataVect.push_back(rawdata - fCommonModeOffsets_even[timebin]);
	 else             meanTimeBinRawPedestalDataVect.push_back(rawdata - fCommonModeOffsets_odd[timebin]); 
       }
       else {
	 meanTimeBinRawPedestalDataVect.push_back(rawdata - fCommonModeOffsets[timebin]);
       }
       timebin++;
    }
    fRawPedestalData.push_back(TMath::Mean(meanTimeBinRawPedestalDataVect.begin(),meanTimeBinRawPedestalDataVect.end()));
    meanTimeBinRawPedestalDataVect.clear();  
  }
  meanTimeBinRawPedestalDataVect.clear();  
}

//========================================================================================================================
void SRSAPVEvent::ComputeMeanTimeBinPedestalData() {
  //  printf("SRSAPVEvent::ComputeMeanTimeBinPedestalData() \n");

  fPedSubFlag        = kFALSE;
  fIsCosmicRunFlag   = kFALSE;

  fIsPedestalRunFlag = kTRUE;
  fIsRawPedestalRunFlag = kFALSE;

  ComputeRawData16bits();
  ComputeTimeBinCommonMode();
  fPedestalData.clear();

  vector<Float_t>  meanTimeBinPedestalDataVect;
  pair<multimap<Int_t, Float_t>::iterator, multimap<Int_t, Float_t>::iterator> stripSetOfTimeBinRawData;
  for (Int_t stripNo = 0;  stripNo < NCH; stripNo++) {
    stripSetOfTimeBinRawData = fapvTimeBinDataMap.equal_range(stripNo);
    multimap<Int_t, Float_t>::iterator timebin_it;
    Int_t timebin = 0;
    for (timebin_it = stripSetOfTimeBinRawData.first; timebin_it != stripSetOfTimeBinRawData.second; ++timebin_it) {
      Float_t rawdata = timebin_it->second;

      //=========================== TEMPORARY
      //XXXXXXXXXXXXXXXXXXXXXXX
      //XXXXXXXXXXXXXXXXXXXXXXX
      //XXXXXXXXXXXXXXXXXXXXXXX
      //XXXXXXXXXXXXXXXXXXXXXXX
      //    fCommonModeOffsets[timebin] = 0 ;
      //XXXXXXXXXXXXXXXXXXXXXXX
      //XXXXXXXXXXXXXXXXXXXXXXX
      //XXXXXXXXXXXXXXXXXXXXXXX
      //XXXXXXXXXXXXXXXXXXXXXXX
      //===========================

      if ((timebin != 1) && (stripNo == 0)) fComModeFluctData.push_back(fCommonModeOffsets[timebin] - fCommonModeOffsets[1]);


      if ((fReadoutBoard == "UV_ANGLE") &&  (fDetectorType == "EICPROTO1") ) {
	if(stripNo < 64) meanTimeBinPedestalDataVect.push_back(rawdata - fCommonModeOffsets_even[timebin]);
	else             meanTimeBinPedestalDataVect.push_back(rawdata - fCommonModeOffsets_odd[timebin]);
      }
      else {
	meanTimeBinPedestalDataVect.push_back(rawdata - fCommonModeOffsets[timebin]);
      }
      timebin++;
    }
    fPedestalData.push_back(TMath::Mean(meanTimeBinPedestalDataVect.begin(),meanTimeBinPedestalDataVect.end()));
     meanTimeBinPedestalDataVect.clear();
  }
  meanTimeBinPedestalDataVect.clear();
}

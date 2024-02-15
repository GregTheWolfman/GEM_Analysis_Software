#include "SRSFECEventDecoder.h"

ClassImp(SRSFECEventDecoder);

SRSFECEventDecoder::SRSFECEventDecoder(Int_t nwords, UInt_t * buf, SRSEventBuilder * eventBuilder) {
 
  fBuf = buf;
  fNWords = nwords;
  fEventNb = -1 ;
  fIsNewPacket = kFALSE ;
  fPacketSize = 4000 ;

  fActiveFecChannelsMap.clear() ;
  SRSMapping * mapping = SRSMapping::GetInstance();
  map <Int_t, Int_t> apvNoFromApvIDMap = mapping->GetAPVNoFromIDMap();
  map <Int_t, Int_t >::const_iterator adcChannel_itr ;
  for(adcChannel_itr = apvNoFromApvIDMap.begin(); adcChannel_itr != apvNoFromApvIDMap.end(); ++adcChannel_itr) {
    Int_t apvid = (* adcChannel_itr).first ;
    Int_t activeChannel = apvid & 0xF;
    Int_t fecId = (apvid >> 4 ) & 0xF;
    fActiveFecChannelsMap[fecId].push_back(activeChannel) ;
    //    printf("  SRSFECEventDecoder => List of  fecNo=%d, activeChannel = %d\n", fecId, activeChannel) ;
  }
  apvNoFromApvIDMap.clear();

  //==========================================================================// 
  // Needed as the key to link apvID (or adcChannel) to the apvEvent in the TList  //
  // Should be < to 15 (max 16 APV channel in the FEC)                        //
  //==========================================================================//

  Int_t current_offset = 0, fecID = 0 ;
  Int_t adcChannel = 0, apvID = 0;
  UInt_t currentAPVPacketHdr;
  Int_t previousAPVPacketSize = 0 ;

  vector<UInt_t> data32BitsVector ;
  data32BitsVector.clear() ;

  //===============================================================================//
  // Dealing with the 7 Equipment header words. We just skip the first 2 words     //
  // and go straight to the 3rd word  where we extract the FEC no (Equip Id)       //
  //===============================================================================//
  current_offset += 2 ;
  UInt_t eqHeaderWord = fBuf[current_offset] ;
  fecID = eqHeaderWord & 0xff ;
  fActiveFecChannels.clear();
  fActiveFecChannels = fActiveFecChannelsMap[fecID] ;

  //  printf("  SRSFECEventDecoder => List of  fecNo=%d \n", fecID) ;

  //=== The next 4 words are Equip word, we dont care
  current_offset += 5 ;

  //================================================================================//
  // Start looking at the APV data word from here                                   //
  //================================================================================//
  while (current_offset < fNWords) {

    UInt_t rawdata = fBuf[current_offset] ;
    //=============================================================================//
    // end of event ==> break: add the data from the last sample here              //
    //=============================================================================//

    if (rawdata == 0xfafafafa) {
      //===================================================================================================//  
      // last word of the previous packet added for Filippo in DATE to count the eventNb x 16 UDP packets  // 
      // We dont need it here, will just skip it We remove it from the vector of data                      //   
      //===================================================================================================// 
      if(!data32BitsVector.empty()) {
	apvID = (fecID << 4) | adcChannel ;
	BuildRawAPVEvents(data32BitsVector, fecID, adcChannel, eventBuilder) ;
	Int_t datasize = data32BitsVector.size() ;
      }

      adcChannel = 0 ;
      data32BitsVector.clear() ;
      current_offset++ ;
      break ;
    }

    //==========================================================================================//
    // Word with the event number (trigger count) and the packet size information               // 
    //                                     size of APV packet                                    // 
    //==========================================================================================//
    if (fIsNewPacket) { 
      fPacketSize = (rawdata & 0xffff) ;
      data32BitsVector.clear() ;
      fIsNewPacket = kFALSE ; 
      current_offset++ ;
      continue ; 
    }


    //=========================================================================================================//
    //         New packet (or frame) FEC channel data in the equipment                                         //
    //=========================================================================================================//
    if (((rawdata >> 8) & 0xffffff) == 0x414443) {  
      data32BitsVector.pop_back() ;
      if(!data32BitsVector.empty()) {
	apvID = (fecID << 4) | adcChannel ;
	BuildRawAPVEvents(data32BitsVector, fecID, adcChannel, eventBuilder) ;
      }
  
      currentAPVPacketHdr = rawdata  ;
      adcChannel = currentAPVPacketHdr & 0xff ;

      //=== REINITIALISE EVERYTHING 
      if(adcChannel > 15) {
	printf("  SRSFECEventDecoder => ERROR #### fecID=%d, ADC Channel=%d, apvID=%d, \n",fecID, adcChannel, apvID) ;
	break ;
      } 

      data32BitsVector.clear() ;
      fIsNewPacket = kTRUE ;

      current_offset++ ;
      continue ; 
    }

    //=========================================================================================================//
    //         apv data in the packet (frame)                                                                 //
    //========================================================================================================//
    if (!fIsNewPacket) {
      data32BitsVector.push_back(rawdata) ;
      current_offset++ ;
      continue ;
    }
  }
  data32BitsVector.clear() ;
}

//======================================================================================================================================
SRSFECEventDecoder::SRSFECEventDecoder(Int_t nwords, UInt_t * buf, SRSPedestal * ped, SRSEventBuilder * eventBuilder, Float_t zeroSupCut, Float_t padZeroSupCut) {

  fBuf = buf;
  fNWords = nwords;
  fEventNb = -1 ;
  fIsNewPacket = kFALSE ;
  fPacketSize = 4000 ;

  fActiveFecChannelsMap.clear() ;
  SRSMapping * mapping = SRSMapping::GetInstance();
  map <Int_t, Int_t> apvNoFromApvIDMap = mapping->GetAPVNoFromIDMap();
  map <Int_t, Int_t >::const_iterator adcChannel_itr ;
  for(adcChannel_itr = apvNoFromApvIDMap.begin(); adcChannel_itr != apvNoFromApvIDMap.end(); ++adcChannel_itr) {
    Int_t apvid = (* adcChannel_itr).first ;
    Int_t activeChannel = apvid & 0xF;
    Int_t fecId = (apvid >> 4 ) & 0xF;
    fActiveFecChannelsMap[fecId].push_back(activeChannel) ;
  }
  apvNoFromApvIDMap.clear();

  //==========================================================================// 
  // Needed as the key to link apvID (or adcChannel) to the apvEvent in the TList  //
  // Should be < to 15 (max 16 APV channel in the FEC)                        //
  //==========================================================================//

  Int_t current_offset = 0, fecID = 0 ;
  Int_t adcChannel = 0, apvID = 0;
  UInt_t currentAPVPacketHdr;
  Int_t previousAPVPacketSize = 0 ;

  vector<UInt_t> data32BitsVector ;
  data32BitsVector.clear() ;

  //===============================================================================//
  // Dealing with the 7 Equipment header words. We just skip the first 2 words     //
  // and go straight to the 3rd word  where we extract the FEC no (Equip Id)       //
  //===============================================================================//
  current_offset += 2 ;
  UInt_t eqHeaderWord = fBuf[current_offset] ;
  fecID = eqHeaderWord & 0xff ;
  fActiveFecChannels.clear();
  fActiveFecChannels = fActiveFecChannelsMap[fecID] ;

  //=== The next 4 words are Equip word, we dont care
  current_offset += 5 ;

  //================================================================================//
  // Start looking at the APV data word from here                                   //
  //================================================================================//
  while (current_offset < fNWords) {

    UInt_t rawdata = fBuf[current_offset] ;
    //=============================================================================//
    // end of event ==> break: add the data from the last sample here              //
    //=============================================================================//

    if (rawdata == 0xfafafafa) {
      //===================================================================================================//  
      // last word of the previous packet added for Filippo in DATE to count the eventNb x 16 UDP packets  // 
      // We dont need it here, will just skip it We remove it from the vector of data                      //   
      //===================================================================================================// 
      if(!data32BitsVector.empty()) {
	apvID = (fecID << 4) | adcChannel ;
        TString plane = mapping->GetDetectorPlaneFromAPVID(apvID);;
        TString detector = mapping->GetDetectorFromPlane(plane) ;
        TString readoutBoard = mapping->GetReadoutBoardFromDetector(detector) ;
        Float_t zeroSup = zeroSupCut ;
	if (readoutBoard == "PADPLANE") zeroSup = padZeroSupCut;
	BuildHits(data32BitsVector, fecID, adcChannel, ped, eventBuilder, zeroSup) ;
	Int_t datasize = data32BitsVector.size() ;
      }

      adcChannel = 0 ;
      data32BitsVector.clear() ;
      current_offset++ ;
      break ;
    }

    //==========================================================================================//
    // Word with the event number (trigger count) and the packet size information               // 
    //                                     size of APV packet                                    // 
    //==========================================================================================//
    if (fIsNewPacket) { 
      fPacketSize = (rawdata & 0xffff) ;
      data32BitsVector.clear() ;
      fIsNewPacket = kFALSE ; 
      current_offset++ ;
      continue ; 
    }

    //=========================================================================================================//
    //         New packet (or frame) FEC channel data in the equipment                                         //
    //=========================================================================================================//
    if (((rawdata >> 8) & 0xffffff) == 0x414443) {  
      data32BitsVector.pop_back() ;
      if(!data32BitsVector.empty()) {
	apvID = (fecID << 4) | adcChannel ;
        TString plane = mapping->GetDetectorPlaneFromAPVID(apvID);;
        TString detector = mapping->GetDetectorFromPlane(plane) ;
        TString readoutBoard   = mapping->GetReadoutBoardFromDetector(detector) ;
        Float_t zeroSup = zeroSupCut ;
        if (readoutBoard == "PADPLANE") zeroSup = padZeroSupCut;
	BuildHits(data32BitsVector, fecID, adcChannel, ped, eventBuilder, zeroSup) ;
      }
  
      currentAPVPacketHdr = rawdata  ;
      adcChannel = currentAPVPacketHdr & 0xff ;

      //=== REINITIALISE EVERYTHING 
      if(adcChannel > 15) {
	printf("  SRSFECEventDecoder => ERROR #### fecID=%d, ADC Channel=%d, apvID=%d, \n",fecID, adcChannel, apvID) ;
	break ;
      } 

      data32BitsVector.clear() ;
      fIsNewPacket = kTRUE ;

      current_offset++ ;
      continue ; 
    }

    //=========================================================================================================//
    //         apv data in the packet (frame)                                                                 //
    //========================================================================================================//
    if (!fIsNewPacket) {
      data32BitsVector.push_back(rawdata) ;
      current_offset++ ;
      continue ;
   }
  }
  data32BitsVector.clear() ;
}

//======================================================================================================================================
void SRSFECEventDecoder::BuildHits(vector<UInt_t> data32bits, Int_t fec_id, Int_t adc_channel, SRSPedestal * ped, SRSEventBuilder * eventBuilder, Float_t zeroSupCut) {
  SRSMapping * mapping = SRSMapping::GetInstance();
  Int_t apvID = (fec_id << 4) | adc_channel ;
  // printf("\n  SRSFECEventDecoder::BuildHits => fecID=%d, ADCCh=%d, apvID=%d \n",fec_id, adc_channel, apvID) ;
  if (find(fActiveFecChannels.begin(), fActiveFecChannels.end(), adc_channel) != fActiveFecChannels.end() ) {
    SRSAPVEvent * apvEvent = new SRSAPVEvent(fec_id, adc_channel, apvID, zeroSupCut, fEventNb, fPacketSize) ;
    apvEvent->SetHitMaxOrTotalADCs(eventBuilder->GetHitMaxOrTotalADCs()) ;
    vector <UInt_t >::const_iterator data_itr ;
    for(data_itr = data32bits.begin(); data_itr != data32bits.end(); ++data_itr) {
      UInt_t data = (* data_itr) ;
      apvEvent->Add32BitsRawData(data) ;
    }
    apvEvent->SetAllFlags(kTRUE, kTRUE) ;
    apvEvent->SetPedestals(ped->GetAPVNoises(apvEvent->GetAPVID()), ped->GetAPVOffsets(apvEvent->GetAPVID())) ; 
    apvEvent->ComputeListOfAPVHits();
    list <SRSHit*> listOfHits = apvEvent->GetListOfHits();
    list <SRSHit*>::const_iterator  hit_itr ;
    for (hit_itr = listOfHits.begin(); hit_itr != listOfHits.end(); ++hit_itr) {
      SRSHit * hit = * hit_itr ;
      //      printf("  SRSFECEventDecoder::BuildHits =>    PadNo[%d]=%d,  x-Pad[%d]=%d, y-Pad[%d]=%d  adc[%d]=%f  \n",hit->GetStripNo(), hit->GetPadNo(), hit->GetStripNo(), hit->GetPadXNo(), hit->GetStripNo(), hit->GetPadYNo(), hit->GetStripNo(), hit->GetHitADCs()) ;
      eventBuilder->AddHitInDetectorPlane(hit)  ;
    }
    data32bits.clear();
    listOfHits.clear() ;
    apvEvent->ClearListOfHits(); 
    delete apvEvent ;
  }
} 

//======================================================================================================================================
void SRSFECEventDecoder::BuildRawAPVEvents(vector<UInt_t> data32bits, Int_t fec_id, Int_t adc_channel, SRSEventBuilder * eventBuilder) {
  SRSMapping * mapping = SRSMapping::GetInstance();
  Int_t apvID = (fec_id << 4) | adc_channel ;
  if (find(fActiveFecChannels.begin(), fActiveFecChannels.end(), adc_channel) != fActiveFecChannels.end() ) {
    SRSAPVEvent * apvEvent = new SRSAPVEvent(fec_id, adc_channel, apvID, 0, fEventNb, fPacketSize) ;
    vector <UInt_t >::const_iterator data_itr ;
    for(data_itr = data32bits.begin(); data_itr != data32bits.end(); ++data_itr) {
      UInt_t data = (* data_itr) ;
      apvEvent->Add32BitsRawData(data) ;
    }
    eventBuilder->AddAPVEvent(apvEvent) ;
  }
}

//=====================================================
SRSFECEventDecoder::~SRSFECEventDecoder() {
  map<Int_t, vector <Int_t> >::const_iterator activeChannel_itr ;
  for(activeChannel_itr = fActiveFecChannelsMap.begin(); activeChannel_itr != fActiveFecChannelsMap.end(); ++activeChannel_itr) {
    vector <Int_t> activeCh = (* activeChannel_itr).second ;
    activeCh.clear() ;
  }
  fActiveFecChannelsMap.clear() ;
}


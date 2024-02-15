
#include "SRSPedestal.h"
ClassImp (SRSPedestal);

SRSPedestal::SRSPedestal(Int_t nbOfAPVs) {
  fNbOfAPVs = nbOfAPVs ;
  fNbOfChannels = NCH ;
  fEventNb = -1 ;

  Init();
  fIsFirstEvent = kTRUE;
  fIsPedestalComputed = kFALSE;
}

//==================================================================================
SRSPedestal::SRSPedestal() {
  fIsFirstEvent = kTRUE;
  fIsPedestalComputed = kFALSE;
}

//==================================================================================
SRSPedestal::~SRSPedestal() {
  Clear();
  ClearMaps();
}

//==================================================================================
void SRSPedestal::Clear() {
  if (!fNoises.empty())          fNoises.clear();
  if (!fOffsets.empty())         fOffsets.clear();
  if (!fRMSDist.empty())         fRMSDist.clear();
  if (!fPedHistos.empty())       fPedHistos.clear();
  if (!fPed2DHistos.empty())     fPed2DHistos.clear();
  if (!fComModeFluctDist.empty()) fComModeFluctDist.clear();
}

//=========================================================================================================================
TString SRSPedestal::GetHistoName(Int_t apvKey, TString dataType, TString dataNb) {
  SRSMapping * mapping = SRSMapping::GetInstance();
  Int_t apvID = mapping->GetAPVIDFromAPVNo(apvKey);
  Int_t fecID = mapping->GetFECIDFromAPVID(apvID);
  Int_t adcCh = mapping->GetADCChannelFromAPVID(apvID);
  TString apvName = mapping->GetAPVFromID(apvID);
  stringstream out;
  out << apvID;
  TString apvIDStr = out.str();
  out.str("");
  out << fecID;
  TString fecIDStr = out.str();
  out.str("");
  out << adcCh;
  TString adcChStr = out.str();
  out.str("");
  out << apvKey;
  TString apvNoStr = out.str();
  out.str("");
  TString histoName = dataType + dataNb + "" + apvName ;
  return histoName;
}

//==================================================================================
void SRSPedestal::ClearMaps() {
  fPedestalData.clear();
}

//==================================================================================
void SRSPedestal::Reset() {
  Clear();
}

//==================================================================================
void SRSPedestal::Init() {
  Clear();
  fIsFirstEvent = kTRUE;
  SRSMapping * mapping = SRSMapping::GetInstance();
  Int_t nbOfPlanes = mapping->GetNbOfDetectorPlane();

  for (Int_t apvKey = 0; apvKey < fNbOfAPVs; apvKey++) {
    fNoises.push_back(BookHistos(apvKey, "noise_", ""));
    fOffsets.push_back(BookHistos(apvKey, "offset_", ""));
    //    fMaskedChannels.push_back(BookHistos(apvKey, "maskedCh_", ""));
    fPed2DHistos.push_back(Book2DHistos(apvKey));
  }

  Int_t nbBin =  150;
  Int_t max = 15;
  fRMSDist.push_back(new TH1F("allstripsAPVsPedestalRMSDist", "allstripsAPVsPedestalRMSDist", nbBin, -0.5, max));
  fRMSDist.push_back(new TH1F("allXstripsAPVsPedestalRMSDist", "allXstripsAPVsPedestalRMSDist", nbBin, -0.5, max));
  fRMSDist.push_back(new TH1F("allYstripsAPVsPedestalRMSDist", "allYstripsAPVsPedestalRMSDist", nbBin, -0.5, max));

  fComModeFluctDist.push_back(new TH1F("allstripsAPVsComModeFluctDist", "allstripsAPVsComModeFluctDist", nbBin, -1*max, max));
  fComModeFluctDist.push_back(new TH1F("allXstripsAPVsComModeFluctDist", "allXstripsAPVsComModeFluctDist", nbBin, -1*max, max));
  fComModeFluctDist.push_back(new TH1F("allYstripsAPVsComModeFluctSDist", "allYstripsAPVsComModeFluctSDist", nbBin, -1*max, max));

  for (Int_t chNo = 0; chNo < NCH; chNo++) {
    for (Int_t apvKey = 0; apvKey < fNbOfAPVs; apvKey++) {
      stringstream out;
      out << chNo;
      TString chNoStr = out.str();
      fPedHistos.push_back(BookHistos(apvKey, "hped_", chNoStr));
    }
  }
  printf("  SRSPedestal::Init() ==> leaving Pedestal init\n");
}

//==================================================================================
TH1F * SRSPedestal::BookHistos(Int_t apvKey, TString dataType, TString dataNb) {
  stringstream out;
  out << apvKey;
  TString apvKeyStr = out.str();
  TString histoName = GetHistoName(apvKey, dataType, dataNb);

  Float_t min = -0.5;
  Float_t max = 127.5;
  Int_t nbin = 128;

  if (dataType.Contains("hped")) {
    min = -2048;
    max = 2048;
    nbin = 4097;
  }

  TH1F * h = new TH1F(histoName, histoName, nbin, min, max);
  //h->StatOverflows(true);
  return h;
}

//==================================================================================
TH2F * SRSPedestal::Book2DHistos(Int_t apvKey) {
  stringstream out;
  out << apvKey;
  TString apvKeyStr = out.str();
  TString pedName = "ped2D_apvNo" + apvKeyStr;
  TH2F * h = new TH2F(pedName, pedName, 128, -0.5, 127.5, 101, 0, 100);
	//h->StatOverflows(true);
  return h;
}

//==================================================================================
void SRSPedestal::FillPedestalHistos(SRSFECPedestalDecoder * pedestalDecoder, SRSRawPedestal * rawped) {
  fEventNb++;
  SRSMapping * mapping = SRSMapping::GetInstance();

  TList * listOfAPVEvents = pedestalDecoder->GetFECEvents();
  TIter nextAPVEvent(listOfAPVEvents);

  while (SRSAPVEvent * apvEvent = (SRSAPVEvent *) nextAPVEvent()) {
    Int_t apvID = apvEvent->GetAPVID();
    Int_t apvKey = mapping->GetAPVNoFromID(apvID);
    TString apvName = mapping->GetAPVFromID(apvID);

    apvEvent->SetRawPedestals(rawped->GetAPVNoises(apvID), rawped->GetAPVOffsets(apvID));
    apvEvent->ComputeMeanTimeBinPedestalData();

    Int_t chNo = 0, stripNo;
    fPedestalData = apvEvent->GetPedestalData();
    vector<Float_t>::const_iterator fPedestalData_itr;
    for (fPedestalData_itr = fPedestalData.begin();	fPedestalData_itr != fPedestalData.end(); ++fPedestalData_itr) {
      Float_t data = *fPedestalData_itr;
      fPedHistos[fNbOfChannels * apvKey + chNo]->Fill(data);
      stripNo = apvEvent->StripMapping(chNo);
      fPed2DHistos[apvKey]->Fill(stripNo, fEventNb, data);
      ++chNo;
    }

    vector<Float_t> comModeFluctData;
    comModeFluctData = apvEvent->GetComModeFluctData();
    //    printf("  SRSPedestal::FillPedestalHistos() ==> apvID =%d apvName = %s, commonMode size = %d \n",  apvID, apvName.Data(), comModeFluctData.size()) ;

    vector<Float_t>::const_iterator comModeFluctData_itr;
    for (comModeFluctData_itr = comModeFluctData.begin(); comModeFluctData_itr != comModeFluctData.end(); ++comModeFluctData_itr) {
      Float_t cMdata = *comModeFluctData_itr;

      Int_t maxCMValue = 250 ;
      Int_t nBin = 10 * maxCMValue ;
      fComModeFluctDist[0]->SetBins(nBin, -1*maxCMValue, maxCMValue) ;
      fComModeFluctDist[1]->SetBins(nBin, -1*maxCMValue, maxCMValue) ;
      fComModeFluctDist[2]->SetBins(nBin, -1*maxCMValue, maxCMValue) ;
      fComModeFluctDist[0]->Fill(cMdata);

      if (apvName.Contains("X")) {
	fComModeFluctDist[1]->SetName("allXstripsComModeFluctDist");
	fComModeFluctDist[1]->SetTitle("allXstripComModeFluctsDist");
        fComModeFluctDist[1]->Fill(cMdata);
      }
      else {
	fComModeFluctDist[2]->SetName("allYstripsComModeFluctDist");
	fComModeFluctDist[2]->SetTitle("allYstripsComModeFluctDist");
        fComModeFluctDist[2]->Fill(cMdata);
      }

    }

  }
  listOfAPVEvents->Delete();
}

//==================================================================================
void SRSPedestal::ComputePedestalData() {
  SRSMapping * mapping = SRSMapping::GetInstance();
  Int_t nbAPVs = mapping->GetNbOfAPVs();
  printf("  SRSPedestal::ComputePedestal() ==> Compute the pedestals for %d APVs \n", nbAPVs);
  Int_t maxValue = 0;

  for (Int_t apvKey = 0; apvKey < nbAPVs; apvKey++) {
    for (Int_t chno = 0; chno < NCH; chno++) {
      Float_t offset = fPedHistos[fNbOfChannels * apvKey + chno]->GetMean();
      Float_t noise  = fPedHistos[fNbOfChannels * apvKey + chno]->GetRMS();
      if (maxValue < ((Int_t) noise)) maxValue = ((Int_t) noise) + 1 ;
      fNoises[apvKey]->Fill(chno, noise);
      fOffsets[apvKey]->Fill(chno, offset);
      Int_t apvID = mapping->GetAPVIDFromAPVNo(apvKey);
      TString apvName = mapping->GetAPVFromID(apvID);

      Int_t nBin = 2 * maxValue ;
      fRMSDist[0]->SetBins(nBin, 0, maxValue) ;
      fRMSDist[1]->SetBins(nBin, 0, maxValue) ;
      fRMSDist[2]->SetBins(nBin, 0, maxValue) ;

      fRMSDist[0]->Fill(noise);
      if (apvName.Contains("X") || apvName.Contains("U")) {
	if (apvName.Contains("X")) {
	  fRMSDist[1]->SetName("allXstripsPedestalRMSDist");
	  fRMSDist[1]->SetTitle("allXstripsPedestalRMSDist");
	}
	if (apvName.Contains("U")) {
	  fRMSDist[1]->SetName("allUstripsPedestalRMSDist");
	  fRMSDist[1]->SetTitle("allUstripsPedestalRMSDist");
	}
	fRMSDist[1]->Fill(noise);
      }
      else {
	fRMSDist[1]->Fill(noise);
      }

      if (apvName.Contains("Y") || apvName.Contains("V")) {
	if (apvName.Contains("Y")) {
	  fRMSDist[2]->SetName("allYstripsPedestalRMSDist");
	  fRMSDist[2]->SetTitle("allYstripsPedestalRMSDist");
	}
	if (apvName.Contains("V")) {
	  fRMSDist[2]->SetName("allVstripsPedestalRMSDist");
	  fRMSDist[2]->SetTitle("allVstripsPedestalRMSDist");
	}
	fRMSDist[2]->Fill(noise);
      }
      else {
	fRMSDist[2]->Fill(noise);
      }
      //      printf("  SRSPedestal::ComputePedestal() ==> planeName=%s, chNo=%d, stripNoOnPlane=%d, noise=%f \n",  planeName.Data(), chNo, stripNoOnPlane, noise) ;
    }

    fNoises[apvKey]->Write();
    fOffsets[apvKey]->Write();
  }

  fRMSDist[0]->Write();
  fRMSDist[1]->Write();
  fRMSDist[2]->Write();

  fComModeFluctDist[0]->Write();
  fComModeFluctDist[1]->Write();
  fComModeFluctDist[2]->Write();

  fIsPedestalComputed = kTRUE;
}

//==================================================================================
Float_t SRSPedestal::GetOnlinePedestalMean(Int_t apvID, Int_t chNo) {
  SRSMapping * mapping = SRSMapping::GetInstance();
  Int_t apvKey = mapping->GetAPVNoFromID(apvID);
  return fPedHistos[fNbOfChannels * apvKey + chNo]->GetMean();
}

//==================================================================================
Float_t SRSPedestal::GetOnlinePedestalRMS(Int_t apvID, Int_t chNo) {
  SRSMapping * mapping = SRSMapping::GetInstance();
  Int_t apvKey = mapping->GetAPVNoFromID(apvID);
  return fPedHistos[fNbOfChannels * apvKey + chNo]->GetRMS();
}

//==================================================================================
void SRSPedestal::LoadPedestalData(const char * filename) {
  TFile f(filename);
  SRSMapping * mapping = SRSMapping::GetInstance();
  Int_t nbAPVs = mapping->GetNbOfAPVs();
  printf("  SRSPedestal::LoadPedestalData() ==> Compute the pedestals from %s for %d APVs \n", filename, nbAPVs);

  for (Int_t apvKey = 0; apvKey < nbAPVs; apvKey++) {
    stringstream out;
    out << apvKey;
    TString apvKeyStr = out.str();

    //The pedestals data
    TString noiseName = GetHistoName(apvKey, "noise_", "");
    TString offsetName = GetHistoName(apvKey, "offset_", "");
    //    TString maskedChName = GetHistoName(apvKey, "maskedCh_", "");

    TH1F * noiseHisto = (TH1F *) f.Get(noiseName);
    TH1F * offsetHisto = (TH1F *) f.Get(offsetName);
    //    TH1F * maskedChHisto = (TH1F *) f.Get(maskedChName);

    for (Int_t chNo = 0; chNo < NCH; chNo++) {
      Int_t binNumber = chNo + 1; // This is an issue with ROOT Histo bin numbering with:
      // ==========================================//
      // bin = 0 is underflow bin                  //
      // bin = 1 is the first bin of the histogram //
      // bin = nbin is the last bin                //
      // bin = nbin + 1 is the overflow bin        //
      // ==========================================//

      Float_t noise = noiseHisto->GetBinContent(binNumber);
      Float_t offset = offsetHisto->GetBinContent(binNumber);
      //      Float_t maskedCh = maskedChHisto->GetBinContent(binNumber);

      fNoises[apvKey]->Fill(chNo, noise);
      fOffsets[apvKey]->Fill(chNo, offset);
      //      fMaskedChannels[apvKey]->Fill(chNo, maskedCh);
    }
  }
  //  fIsMaskedChComputed = kTRUE;
  fIsPedestalComputed = kTRUE;
  f.Close();
}

//==================================================================================
Float_t SRSPedestal::GetNoise(Int_t apvID, Int_t chNo) {
  SRSMapping * mapping = SRSMapping::GetInstance();
  if (!fIsPedestalComputed) {
    Warning("SRSPedestal", "GetNoise:=> Pedestals & Noise not yet computed");
    return 0;
  }
  Int_t apvKey = mapping->GetAPVNoFromID(apvID);
  Int_t binNumber = chNo + 1; // This is an issue with ROOT Histo bin numbering with:
  // ==========================================//
  // bin = 0 is underflow bin                  //
  // bin = 1 is the first bin of the histogram //
  // bin = nbin is the last bin                //
  // bin = nbin + 1 is the overflow bin        //
  // ==========================================//
  return fNoises[apvKey]->GetBinContent(binNumber);
}

//==================================================================================
Float_t SRSPedestal::GetOffset(Int_t apvID, Int_t chNo) {
  SRSMapping * mapping = SRSMapping::GetInstance();
  if (!fIsPedestalComputed) {
    Warning("SRSPedestal","GetPedestal:=> Pedestals & Noises not yet computed");
    return 0;
  }

  Int_t apvKey = mapping->GetAPVNoFromID(apvID);
  Int_t binNumber = chNo + 1; // This is an issue with ROOT Histo bin numbering with:
  // ==========================================//
  // bin = 0 is underflow bin                  //
  // bin = 1 is the first bin of the histogram //
  // bin = nbin is the last bin                //
  // bin = nbin + 1 is the overflow bin        //
  // ==========================================//
  cout << "Offset Value: " << fOffsets[apvKey]->GetBinContent(binNumber) << endl;
  return fOffsets[apvKey]->GetBinContent(binNumber);
}

//==================================================================================
vector<Float_t> SRSPedestal::GetAPVNoises(Int_t apvID) {
  SRSMapping * mapping = SRSMapping::GetInstance();
  vector < Float_t > apvNoises;
  if (!fIsPedestalComputed) {
    Warning("SRSPedestal", "GetAPVNoise:=> Pedestals & Noise not yet computed");
    for (Int_t chNo = 1; chNo <= NCH; ++chNo)
      apvNoises.push_back(0);
  }
  Int_t apvKey = mapping->GetAPVNoFromID(apvID);
  for (Int_t chNo = 1; chNo <= NCH; ++chNo)
    apvNoises.push_back(fNoises[apvKey]->GetBinContent(chNo));
  //  printf("  SRSPedestal::GetAPVNoises() ==> Got the pedestals \n") ;
  return apvNoises;
}

//==================================================================================
vector<Float_t> SRSPedestal::GetAPVOffsets(Int_t apvID) {
  SRSMapping * mapping = SRSMapping::GetInstance();
  vector < Float_t > apvOffsets;
  if (!fIsPedestalComputed) {
    Warning("SRSPedestal", "GetAPVOffsets:=> Pedestals & Noises not yet computed");
    for (Int_t chNo = 1; chNo <= NCH; ++chNo)
      apvOffsets.push_back(0);
  }
  Int_t apvKey = mapping->GetAPVNoFromID(apvID);
  for (Int_t chNo = 1; chNo <= NCH; ++chNo) {
    apvOffsets.push_back(fOffsets[apvKey]->GetBinContent(chNo));
  }
  //  printf("  SRSPedestal::GetAPVOffsets() ==> Got the pedestals \n") ;
  return apvOffsets;
}

//==================================================================================
TH1F * SRSPedestal::GetPedHisto(Int_t apvID, Int_t chNo) {
  SRSMapping * mapping = SRSMapping::GetInstance();
  if (!fIsPedestalComputed) {
    Warning("GetPedestal", "Pedestals not yet computed");
    return 0;
  }
  Int_t apvKey = mapping->GetAPVNoFromID(apvID);
  return fPedHistos[fNbOfChannels * apvKey + chNo];
}

//==================================================================================
SRSPedestal * SRSPedestal::GetPedestalRootFile(const char * filename) {

  TFile * f = new TFile(filename, "read");
  SRSPedestal * pedestalToUse = (SRSPedestal *) f->Get("SRSPedestal");
  //  printf("SRSPedestal::GetPedestalRootFile() ==> load pedestal root file %s \n",filename) ;
  f->Close();
  return pedestalToUse;
}

//==================================================================================
void SRSPedestal::SavePedestalRunHistos() {

  SRSMapping * mapping = SRSMapping::GetInstance();
  Int_t nbAPVs = mapping->GetNbOfAPVs();
  printf("SRSPedestal::SavePedestalRunHistos() ==> Get Mapping \n");

  TCanvas c("c1", "c1", 80, 80, 1200, 600);
  c.cd();

  TString distname = fRMSDist[0]->GetName();
  TString distName = fOutputDir + "_" + distname + ".png";
  fRMSDist[0]->Draw("");
  fRMSDist[0]->SetYTitle("Frequency");
  fRMSDist[0]->SetXTitle("Pedestal RMS (ADC count)");
  gStyle->SetOptStat(1111);
  c.SaveAs(distName);

  distname = fRMSDist[1]->GetName();
  distName = fOutputDir + "_" + distname + ".png";
  fRMSDist[1]->Draw("");
  fRMSDist[1]->Draw("");
  fRMSDist[1]->SetYTitle("Frequency");
  fRMSDist[1]->SetXTitle("Y-Strips pedestal RMS (ADC count)");
  gStyle->SetOptStat(1111);
  c.SaveAs(distName);

  distname = fRMSDist[2]->GetName();
  distName = fOutputDir + "_" + distname + ".png";
  fRMSDist[2]->Draw("");
  fRMSDist[2]->SetYTitle("Frequency");
  fRMSDist[2]->SetXTitle("X-Strips pedestal RMS (ADC count)");
  gStyle->SetOptStat(1111);
  c.SaveAs(distName);
  printf("SRSPedestal::SavePedestalRunHistos() ==> save ped dist plots \n");
  distname = fComModeFluctDist[0]->GetName();
  distName = fOutputDir + "_" + distname + ".png";
  fComModeFluctDist[0]->Draw("");
  fComModeFluctDist[0]->SetYTitle("Frequency");
  fComModeFluctDist[0]->SetXTitle("Pedestal RMS (ADC count)");
  gStyle->SetOptStat(1111);
  c.SaveAs(distName);

  distname = fComModeFluctDist[1]->GetName();
  distName = fOutputDir + "_" + distname + ".png";
  fComModeFluctDist[1]->Draw("");
  fComModeFluctDist[1]->Draw("");
  fComModeFluctDist[1]->SetYTitle("Frequency");
  fComModeFluctDist[1]->SetXTitle("Y-Strips pedestal RMS (ADC count)");
  gStyle->SetOptStat(1111);
  c.SaveAs(distName);

  distname = fComModeFluctDist[2]->GetName();
  distName = fOutputDir + "_" + distname + ".png";
  fComModeFluctDist[2]->Draw("");
  fComModeFluctDist[2]->SetYTitle("Frequency");
  fComModeFluctDist[2]->SetXTitle("X-Strips pedestal RMS (ADC count)");
  gStyle->SetOptStat(1111);
  c.SaveAs(distName);
  printf("SRSPedestal::SavePedestalRunHistos() ==> save ped dist plots \n");

  GetStyle();
 
  for (Int_t apvKey = 0; apvKey < nbAPVs; apvKey++) {
	  
    TString histoname = fNoises[apvKey]->GetName();
    TString picturename = fOutputDir + "_ped_" + histoname + ".png";
    fNoises[apvKey]->Draw("");
    fNoises[apvKey]->UseCurrentStyle();
    fNoises[apvKey]->SetXTitle("APV Channel No");
    fNoises[apvKey]->SetYTitle("Pedestal Noise  (ADC counts)");
    c.SaveAs(picturename);

    /**		
    histoname = fPed2DHistos[apvKey]->GetName() ;
    picturename = fOutputDir + "_ped_" +  histoname + ".png" ;
    fPed2DHistos[apvKey]->Draw("LEGO2") ;
    fPed2DHistos[apvKey]->SetXTitle("APV Channel No") ;
    fPed2DHistos[apvKey]->SetYTitle("Event No") ;
    fPed2DHistos[apvKey]->SetZTitle("ADC charge (A.U.)") ;
    c.SaveAs(picturename) ;

    histoname = fOffsets[apvKey]->GetName() ;
    picturename = fOutputDir + "_ped_" +  histoname + ".png" ;
    fOffsets[apvKey]->Draw("") ;
    fOffsets[apvKey]->UseCurrentStyle() ;
    fOffsets[apvKey]->SetXTitle("APV Channel No") ;
    fOffsets[apvKey]->SetYTitle("Pedestal Offset  (ADC counts)") ;
    c.SaveAs(picturename) ;

    histoname = fMaskedChannels[apvKey]->GetName() ;
    picturename = fOutputDir + "_ped_" +  histoname + ".png" ;
    fMaskedChannels[apvKey]->Draw("") ;
    fMaskedChannels[apvKey]->UseCurrentStyle() ;
    fMaskedChannels[apvKey]->SetXTitle("APV Channel No") ;
    fMaskedChannels[apvKey]->SetYTitle("maskChannels") ;
    c.SaveAs(picturename) ;    
    */
  }
}

//==================================================================================
void SRSPedestal::GetStyle() {
  gStyle->SetOptStat(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasBorderMode(0);

  gStyle->SetLabelFont(62, "xyz");
  gStyle->SetLabelSize(0.03, "xyz");
  gStyle->SetLabelColor(1, "xyz");
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleSize(0.05, "xyz");
  gStyle->SetTitleOffset(1., "xy");
  gStyle->SetTitleOffset(1., "z");
  gStyle->SetPalette(1);

  const Int_t NRGBs = 5;
  const Int_t NCont = 32;
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs] = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs] = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
}

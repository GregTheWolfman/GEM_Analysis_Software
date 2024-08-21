#include <vector>
#include <TVector.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "TH1D.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TF1.h"
#include "TFile.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "TImage.h"
#include "TCanvas.h"
#include "TString.h"
#include "TAttFill.h"
#include "TStyle.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLatex.h"
#include <cmath>
#include <TPaveStats.h>
#include <TAttText.h>
#include <TAttLine.h>
#include <TAttMarker.h>
#include <TAxis.h>
#include <TLorentzVector.h>
#include <TParameter.h>
#include <TPad.h>
#include <TAxis.h>
#include <TSpectrum.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraph2D.h>
#include <TText.h>
#include <cstdlib>
#include <TGraphErrors.h>
#include <RtypesCore.h>
#include <THistPainter.h>
#include "doubleGausFit_withHistParameter.C"
#include <TLatex.h>
#include <unistd.h>
#include <TMath.h>
//Extra functions to perform alignment
#include "CalculateCosTheta1.C"
#include "Plot_Aligned_residuals.C"
#include "Optimized_XY_Rotation.C"
#include "Optimized_Rotation.C"
#include "Optimized_XY_shift.C"
#include "Residuals.C"
#include "ResCut.C"
#include "StripSep.C"
#include "ChiSquared.C"
#include "TFitResult.h"
#include "TMatrixD.h"
#include "TFitter.h"
#include "MultiClusterReducer.C"
#include <TProfile.h>
#include <TProfile2D.h>
#include "EffCounter.C"

//#include <gSystem.h>
//#include <gPad.h>
//#include "Optimized_XY_shift.C"

//#include <bits/stdc++.h>

//vector<float> FitParabola (TGraph);

//vector<float> CorrectCrosstalk (TFile*);


/*
vector<float> CorrectCrosstalk (TFile* myFile){

  vector<float> yranges;

  
  TTreeReader myReader("THit", myFile);
    
    //assign trees and leaves to new trees and leaves
    //info about the sector
    TTreeReaderValue<int>      evtID(myReader, "evtID");
    TTreeReaderValue<int>      nch(myReader, "nch");
    TTreeReaderArray<int>      strip(myReader, "strip"); 
    //TTreeReaderArray<string>      hitTimebin(myReader, "hitTimebin");//this variable is not working well :(   
    TTreeReaderArray<int>      detID(myReader, "detID");
    TTreeReaderArray<int>      planeID(myReader, "planeID");
    TTreeReaderArray<short>      etasec(myReader, "etaSector");
    //data for pulse height

    TTreeReaderArray<short>      adc0(myReader, "adc0");     
    TTreeReaderArray<short>      adc1(myReader, "adc1");    
    TTreeReaderArray<short>      adc2(myReader, "adc2");
    TTreeReaderArray<short>      adc3(myReader, "adc3");
    TTreeReaderArray<short>      adc4(myReader, "adc4");
    TTreeReaderArray<short>      adc5(myReader, "adc5");

    
    vector<float> APVstriphits;
           
      
    while (myReader.Next()){
      totalchannel++;
      
      if(totalchannel >= 128*11 && totalchannel <= 127 + 128*11){
		
	APVstriphits.push_back((adc0)[i]);
	APVstriphits.push_back((adc1)[i]);
	APVstriphits.push_back((adc2)[i]);
	APVstriphits.push_back((adc3)[i]);
	APVstriphits.push_back((adc4)[i]);
	APVstriphits.push_back((adc5)[i]);

	      
	maxcharge = *max_element(APVstriphits.begin(), APVstriphits.end());
	
	if(maxcharge >=  striptol[i]){

	  Tracker2ymaxcharges.push_back(maxcharge);
	  Tracker2ymaxstrips.push_back((strip)[i]);
		  
	}

      }
    }


}


*/

//vector<float> Fitparams;


//using namespace std;

void APVEvent(){

  //This line tells root not to write data stats in the histograms
  gStyle->SetOptStat();
  
  map<string, TString> inputfiles;
  //map<string, TString> pedestalfiles;
  
  map<int, float> striptol;
  
  map<int, TH1D*> adcsstrip;

  //Assign tolerance to each strip for further hit determination    
  
  //TFile* pedFile = TFile::Open("Data/PED_dataTree01.root");
  //TFile* pedFile = TFile::Open("APV3Data/APV3_Ped_dataTree01.root");

  //TFile* pedFile = TFile::Open("Data/FromscratchPedestal_dataTree01.root");//this one works!
  TFile* pedFile = TFile::Open("Data/Second_ZZ_MultiFilesOtherDataPlace_ped_dataTree01.root");//use this one generally
  //TFile* pedFile = TFile::Open("Data/NewMappingAPVs89test_ped_dataTree01.root");

  //TFile* pedFile = TFile::Open("Data/changedAPV3readoutToStandardpedestal_dataTree01.root");
  
  //  TFile* pedFile = TFile::Open("Data/GEMandTrksPed_pedestaloptionfirst_dataTree01.root");
  //TFile* pedFile = TFile::Open("Data/pedestal_TRKandFITZZ_Zsup0.root");
  //TFile* pedFile = TFile::Open("Data/APV3_HVscanPed__dataTree01.root");
  //TFile* pedFile = TFile::Open("Data/APV3_file0_apv3pedestal_dataTree01.root");
  
  
  //TFile* pedFile = TFile::Open("Data/GEM_and_TRKs_rootfile_dataTree01.root");//?????
  //TFile* pedFile = TFile::Open("Data/GEMandTrksPed_RawPedthenPedThenrootfileoutput.root");
  
  //cout << "Jere" << endl;
  TTreeReader pedReader("THit", pedFile);//Assign the root TTreereader to the correct branch 
  
  //assign trees and leaves to new trees and leaves
  //Each variable here aligns with the data in the root file
  TTreeReaderValue<int>      evtID(pedReader, "evtID");
  TTreeReaderValue<int>      nch(pedReader, "nch");
  TTreeReaderArray<int>      strip(pedReader, "strip");
  
  //data for pulse height

  TTreeReaderArray<short>      adc0(pedReader, "adc0");     
  TTreeReaderArray<short>      adc1(pedReader, "adc1");    
  TTreeReaderArray<short>      adc2(pedReader, "adc2");
  TTreeReaderArray<short>      adc3(pedReader, "adc3");
  TTreeReaderArray<short>      adc4(pedReader, "adc4");
  TTreeReaderArray<short>      adc5(pedReader, "adc5");
  //TTreeReaderArray<short>      adc6(pedReader, "adc6");//all adcs past 6 are 0 for these files

  //  auto stddev_strip1 = new TH1D("stds strip1", "StdDev for Strip 1", 200, 0, 300);

  //auto APVevent = new TGraph2D();
  //auto toliter = 0;
  //auto evts = 5001;
  auto pedevt = 0;
  //cout << "Jere" << endl;
  //Iterate through the events in the rootfile
  while (pedReader.Next()){
    //plot for looking at pedestal events
    //TH2D* Pedevt = new TH2D("Pedevt", "Event Pedestals", *nch, 0, *nch, 7, 0, 6);
    //if(*evtID > 1) break;
    //cout << *nch << endl;
    //iterate through the channels/strips
    int pediter=-1;
    for (uint i = 0; i < abs(*nch); i++){
      //if(*evtID > 1) break;
      
      //  toliter = (*evtID-1)*(*nch) + i;
      //we only need the pedestals from the first event to use for all
      if(*evtID == 1){
	//cout << "Jere, but inside" << endl;
	//now the process of generating a specific name for all of the graphs this generates
	

	string name;
	string title = "Pedestal ADCs for strip: ";

	name += to_string(i);
	title += to_string(i);

	int y = title.length();
	int n = name.length();
	  
	char charname[n];
	char chartit[y];
      
	strcpy(charname, name.c_str());
	strcpy(chartit, title.c_str());
	//initiate the plot
	adcsstrip[i] = new TH1D(charname, chartit, 600, -300, 300);

	int APVind = 10;
	/*
	if(i >= 128*APVind && i < 128*(APVind+1)){
	  cout << i-APVind << ", " <<  strip[i] << ", " <<  adc0[i] << ", "<<  adc1[i] << ", "<<  adc2[i] << ", "<<  adc3[i] << ", "<<  adc4[i] << ", "<<  adc5[i] << endl;
	  
	  APVevent->SetPoint(pediter,i-128*APVind, 0, adc0[i]);
	  pediter++;	  
	  APVevent->SetPoint(pediter,i-128*APVind, 1, adc1[i]);
	  pediter++;	  
	  APVevent->SetPoint(pediter,i-128*APVind, 2, adc2[i]);
	  pediter++;	  
	  APVevent->SetPoint(pediter,i-128*APVind, 3, adc3[i]);
	  pediter++;	  
	  APVevent->SetPoint(pediter,i-128*APVind, 4, adc4[i]);
	  pediter++;	  
	  APVevent->SetPoint(pediter,i-128*APVind, 5, adc5[i]);
	  pediter++;
	  
	}
	*/
      }
      //fill the histogram with the pedestal signal pulse on strip i
      if(adc0[i] != 0) adcsstrip[i]->Fill(adc0[i]);
      //cout << "ADC 0: " << adc0[i] << endl;
      if(adc1[i] != 0) adcsstrip[i]->Fill(adc1[i]);
      //cout << "ADC 1: " << adc1[i] << endl;
      if(adc2[i] != 0) adcsstrip[i]->Fill(adc2[i]);
      //cout << "ADC 2: " << adc2[i] << endl;
      if(adc3[i] != 0) adcsstrip[i]->Fill(adc3[i]);
      //cout << "ADC 3: " << adc3[i] << endl;
      if(adc4[i] != 0) adcsstrip[i]->Fill(adc4[i]);
      //cout << "ADC 4: " << adc4[i] << endl;
      if(adc5[i] != 0) adcsstrip[i]->Fill(adc5[i]);
      //cout << "ADC 5: " << adc5[i] << endl;
      
    }
    
  }
  /*
  APVevent->SetMarkerStyle(20);
  APVevent->Draw("pcol");
  APVevent->SetTitle("Tracker 2X pedestal");
  APVevent->GetXaxis()->SetTitle("Strip");
  APVevent->GetXaxis()->CenterTitle(true);
  APVevent->GetYaxis()->SetTitle("TimeBin");
  APVevent->GetYaxis()->CenterTitle(true);
  APVevent->GetZaxis()->SetTitle("ADC charge");
  APVevent->GetZaxis()->CenterTitle(true);
  */
  //cout << "Past?" << endl;
  //pedFile->Close();
  
  //adcsstrip[1151]->Draw();
  
  //return;
  
  //data file for each APV
  //APV 3
  //the mapping of inputfiles connects the HV setting string inside to the pathway to the data string

  //this data is for APV3 over a wide range of HV
  
  inputfiles["3729"] = "Data/HVscanOtherDirTest_MultiFiles_4150_dataTree01.root";
   
  inputfiles["3819"] = "Data/HVscanOtherDirTest_MultiFiles_4250_dataTree01.root";  
  
  inputfiles["3846"] = "Data/HVscanOtherDirTest_MultiFiles_4280_dataTree01.root";
  
  inputfiles["3873"] = "Data/HVscanOtherDirTest_MultiFiles_4310_dataTree01.root";//corrupted

  inputfiles["3900"] = "Data/HVscanOtherDirTest_MultiFiles_4340_dataTree01.root";

  inputfiles["3927"] = "Data/HVscanOtherDirTest_MultiFiles_4370_dataTree01.root";
  
  inputfiles["3953"] = "Data/HVscanOtherDirTest_MultiFiles_4400_dataTree01.root";//weird file
  
  inputfiles["3981"] = "Data/HVscanOtherDirTest_MultiFiles_4430_dataTree01.root";
  
  inputfiles["3998"] = "Data/HVscanOtherDirTest_MultiFiles_4450_dataTree01.root"; 
  
  //inputfiles["4460"] = "Data/HVscanOtherDirTest_MultiFiles_4460_dataTree01.root";//also a wierd file due to statistics
  
 

  //Straight strip large statistics
  
  // inputfiles["0"] = "Data/SS_MultiFilesOtherDataPlace_0_dataTree01.root";  
  
  // inputfiles["1"] = "Data/SS_MultiFilesOtherDataPlace_1_dataTree01.root";  
  
  // inputfiles["2"] = "Data/SS_MultiFilesOtherDataPlace_2_dataTree01.root";
  // inputfiles["3"] = "Data/SS_MultiFilesOtherDataPlace_3_dataTree01.root";
  
  // inputfiles["4"] = "Data/SS_MultiFilesOtherDataPlace_4_dataTree01.root";
  
  // inputfiles["5"] = "Data/SS_MultiFilesOtherDataPlace_5_dataTree01.root";
  // inputfiles["6"] = "Data/SS_MultiFilesOtherDataPlace_6_dataTree01.root";
  
  // inputfiles["7"] = "Data/SS_MultiFilesOtherDataPlace_7_dataTree01.root";
  
  // inputfiles["8"] = "Data/SS_MultiFilesOtherDataPlace_8_dataTree01.root";
  // inputfiles["9"] = "Data/SS_MultiFilesOtherDataPlace_9_dataTree01.root";
  
  // inputfiles["10"] = "Data/SS_MultiFilesOtherDataPlace_10_dataTree01.root";
  // inputfiles["11"] = "Data/SS_MultiFilesOtherDataPlace_11_dataTree01.root";
  // inputfiles["12"] = "Data/SS_MultiFilesOtherDataPlace_12_dataTree01.root";

  // inputfiles["13"] = "Data/SS_MultiFilesOtherDataPlace_13_dataTree01.root";
  // inputfiles["14"] = "Data/SS_MultiFilesOtherDataPlace_14_dataTree01.root";
  // inputfiles["15"] = "Data/SS_MultiFilesOtherDataPlace_15_dataTree01.root";
  // inputfiles["16"] = "Data/SS_MultiFilesOtherDataPlace_16_dataTree01.root";
  // inputfiles["17"] = "Data/SS_MultiFilesOtherDataPlace_17_dataTree01.root";
  // inputfiles["18"] = "Data/SS_MultiFilesOtherDataPlace_18_dataTree01.root";
  
  // inputfiles["19"] = "Data/SS_MultiFilesOtherDataPlace_19_dataTree01.root";
  // inputfiles["20"] = "Data/SS_MultiFilesOtherDataPlace_20_dataTree01.root";


  //3rd ZigZag Sector (APV5E4, 1500, 217)
  
  // inputfiles["0"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_0_dataTree01.root";  
   
  // inputfiles["1"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_1_dataTree01.root";  
  
  // inputfiles["2"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_2_dataTree01.root";
  
  // inputfiles["3"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_3_dataTree01.root";
  
  // inputfiles["4"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_4_dataTree01.root";
  
  // inputfiles["5"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_5_dataTree01.root";
  
  // inputfiles["6"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_6_dataTree01.root";
  
  // inputfiles["7"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_7_dataTree01.root";
  
  // inputfiles["8"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_8_dataTree01.root";
  // inputfiles["9"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_9_dataTree01.root";
  
  // inputfiles["10"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_10_dataTree01.root";
  // inputfiles["11"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_11_dataTree01.root";
  // inputfiles["12"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_12_dataTree01.root";

  // inputfiles["13"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_13_dataTree01.root";
  // inputfiles["14"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_14_dataTree01.root";
  // inputfiles["15"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_15_dataTree01.root";
  // inputfiles["16"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_16_dataTree01.root";
  // inputfiles["17"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_17_dataTree01.root";
  // inputfiles["18"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_18_dataTree01.root";
  // inputfiles["19"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_19_dataTree01.root";
  // inputfiles["20"] = "Data/SZZ_APV5_MultiFilesOtherDataPlace_20_dataTree01.root";
  
  //4th ZigZag Sector (APV4E4, 1717, 217)
  
  // inputfiles["0"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_0_dataTree01.root";  
   
  // inputfiles["1"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_1_dataTree01.root";  
  
  // inputfiles["2"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_2_dataTree01.root";
  
  // inputfiles["3"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_3_dataTree01.root";
  
  // inputfiles["4"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_4_dataTree01.root";
  
  // inputfiles["5"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_5_dataTree01.root";
  
  // inputfiles["6"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_6_dataTree01.root";
  
  // inputfiles["7"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_7_dataTree01.root";
  
  // inputfiles["8"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_8_dataTree01.root";
  // inputfiles["9"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_9_dataTree01.root";
  
  // inputfiles["10"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_10_dataTree01.root";
  // inputfiles["11"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_11_dataTree01.root";
  // inputfiles["12"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_12_dataTree01.root";

  // inputfiles["13"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_13_dataTree01.root";
  // inputfiles["14"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_14_dataTree01.root";
  // inputfiles["15"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_15_dataTree01.root";
  // inputfiles["16"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_16_dataTree01.root";
  // inputfiles["17"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_17_dataTree01.root";
  // inputfiles["18"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_18_dataTree01.root";
  // inputfiles["19"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_19_dataTree01.root";
  // inputfiles["20"] = "Data/SZZ_APV4_MultiFilesOtherDataPlace_20_dataTree01.root";
   
  
  //Second ZigZag Sector
  
  // inputfiles["0"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_0_dataTree01.root";  
  
  // inputfiles["1"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_1_dataTree01.root";  
  
  // inputfiles["2"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_2_dataTree01.root";
  
  // inputfiles["3"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_3_dataTree01.root";
  
  // inputfiles["4"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_4_dataTree01.root";
  
  // inputfiles["5"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_5_dataTree01.root";
  // inputfiles["6"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_6_dataTree01.root";
  
  // inputfiles["7"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_7_dataTree01.root";
  
  // inputfiles["8"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_8_dataTree01.root";
  // inputfiles["9"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_9_dataTree01.root";
  
  // inputfiles["10"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_10_dataTree01.root";
  // inputfiles["11"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_11_dataTree01.root";
  
  // inputfiles["12"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_12_dataTree01.root";
  
  // inputfiles["13"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_13_dataTree01.root";
  // inputfiles["14"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_14_dataTree01.root";
  // inputfiles["15"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_15_dataTree01.root";
  // inputfiles["16"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_16_dataTree01.root";
  // inputfiles["17"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_17_dataTree01.root";
  // inputfiles["18"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_18_dataTree01.root";
  // inputfiles["19"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_19_dataTree01.root";
  // inputfiles["20"] = "Data/Second_ZZ_MultiFilesOtherDataPlace_20_dataTree01.root";
  
  //First ZigZag Sector
  // inputfiles["1"] = "Data/First_ZZ_MultiFilesOtherDataPlace_1_dataTree01.root";  
  
  // inputfiles["2"] = "Data/First_ZZ_MultiFilesOtherDataPlace_2_dataTree01.root";  

  
  // inputfiles["3"] = "Data/First_ZZ_MultiFilesOtherDataPlace_3_dataTree01.root";  
  
  // inputfiles["4"] = "Data/First_ZZ_MultiFilesOtherDataPlace_4_dataTree01.root";  
  // inputfiles["5"] = "Data/First_ZZ_MultiFilesOtherDataPlace_5_dataTree01.root";  
  
  // inputfiles["6"] = "Data/First_ZZ_MultiFilesOtherDataPlace_6_dataTree01.root";  
  // inputfiles["7"] = "Data/First_ZZ_MultiFilesOtherDataPlace_7_dataTree01.root";  
  // inputfiles["8"] = "Data/First_ZZ_MultiFilesOtherDataPlace_8_dataTree01.root";  
  // inputfiles["9"] = "Data/First_ZZ_MultiFilesOtherDataPlace_9_dataTree01.root";  
  // inputfiles["10"] = "Data/First_ZZ_MultiFilesOtherDataPlace_10_dataTree01.root";  
  // inputfiles["11"] = "Data/First_ZZ_MultiFilesOtherDataPlace_11_dataTree01.root";  
  
  // inputfiles["12"] = "Data/First_ZZ_MultiFilesOtherDataPlace_12_dataTree01.root";  

  
  // inputfiles["13"] = "Data/First_ZZ_MultiFilesOtherDataPlace_13_dataTree01.root";  
  
  // inputfiles["14"] = "Data/First_ZZ_MultiFilesOtherDataPlace_14_dataTree01.root";  
  // inputfiles["15"] = "Data/First_ZZ_MultiFilesOtherDataPlace_15_dataTree01.root";  
  
  // inputfiles["16"] = "Data/First_ZZ_MultiFilesOtherDataPlace_16_dataTree01.root";  
  // inputfiles["17"] = "Data/First_ZZ_MultiFilesOtherDataPlace_17_dataTree01.root";  
  // inputfiles["18"] = "Data/First_ZZ_MultiFilesOtherDataPlace_18_dataTree01.root";  
  // inputfiles["19"] = "Data/First_ZZ_MultiFilesOtherDataPlace_19_dataTree01.root";  
  // inputfiles["20"] = "Data/First_ZZ_MultiFilesOtherDataPlace_20_dataTree01.root"; 
  
  
  // first zigzag sector, second location (1100, 217)
  
  // inputfiles["0"] = "Data/LZZ2_MultiFilesOtherDataPlace_0_dataTree01.root";  
  // inputfiles["1"] = "Data/LZZ2_MultiFilesOtherDataPlace_1_dataTree01.root";  
  // inputfiles["2"] = "Data/LZZ2_MultiFilesOtherDataPlace_2_dataTree01.root";  

  
  // inputfiles["3"] = "Data/LZZ2_MultiFilesOtherDataPlace_3_dataTree01.root";      
  // inputfiles["4"] = "Data/LZZ2_MultiFilesOtherDataPlace_4_dataTree01.root";  
  // inputfiles["5"] = "Data/LZZ2_MultiFilesOtherDataPlace_5_dataTree01.root";  
  
  // inputfiles["6"] = "Data/LZZ2_MultiFilesOtherDataPlace_6_dataTree01.root";  
  // inputfiles["7"] = "Data/LZZ2_MultiFilesOtherDataPlace_7_dataTree01.root";  
  // inputfiles["8"] = "Data/LZZ2_MultiFilesOtherDataPlace_8_dataTree01.root";  
  // inputfiles["9"] = "Data/LZZ2_MultiFilesOtherDataPlace_9_dataTree01.root";  
  // inputfiles["10"] = "Data/LZZ2_MultiFilesOtherDataPlace_10_dataTree01.root";  
  // inputfiles["11"] = "Data/LZZ2_MultiFilesOtherDataPlace_11_dataTree01.root";  
  
  // inputfiles["12"] = "Data/LZZ2_MultiFilesOtherDataPlace_12_dataTree01.root";  

  
  // inputfiles["13"] = "Data/LZZ2_MultiFilesOtherDataPlace_13_dataTree01.root";  
  // inputfiles["14"] = "Data/LZZ2_MultiFilesOtherDataPlace_14_dataTree01.root";  
  // inputfiles["15"] = "Data/LZZ2_MultiFilesOtherDataPlace_15_dataTree01.root";  
  
  // inputfiles["16"] = "Data/LZZ2_MultiFilesOtherDataPlace_16_dataTree01.root";  
  // inputfiles["17"] = "Data/LZZ2_MultiFilesOtherDataPlace_17_dataTree01.root";  
  // inputfiles["18"] = "Data/LZZ2_MultiFilesOtherDataPlace_18_dataTree01.root";  
  // inputfiles["19"] = "Data/LZZ2_MultiFilesOtherDataPlace_19_dataTree01.root";  
  // inputfiles["20"] = "Data/LZZ2_MultiFilesOtherDataPlace_20_dataTree01.root";  
  
  
  //data for each sector
  //inputfiles["APV3&7"] = "Data/APV3and7_file5_dataTree01.root";//"Data/APV3_FixedAPVID_dataTree01.root";
  //inputfiles["APV7"] = "APV7_AllDataforAPVEvents_totalADCs_dataTree01.root";
  //inputfiles["APV6&2"] = "Data/APV2and6_dataTree01.root";
  //inputfiles["4400"] = "Data/APV4_dataTree01.root";
  //inputfiles["APV5"] = "Data/APV5_dataTree01.root";


  string location = "4250";

  map<string, float> LAGDXoffsets;

  map<string, float> LAGDYoffsets;

  map<string, float> HVres;

  LAGDXoffsets["Data/SS_"] = 80 + 25 + 7.2913;
  LAGDYoffsets["Data/SS_"] = 8.30171;
  HVres["Data/SS_"] = 0.48166;//mRad

  
  LAGDXoffsets["Data/Fir"] = 80 + 120 + 58.68747;
  LAGDYoffsets["Data/Fir"] = 8.31925;
  HVres["Data/Fir"] = 0.27246;//mRad

  LAGDXoffsets["LZZ2"] = 80+120+100+9.85;
  LAGDYoffsets["LZZ2"] = 10.96;
  HVres["LZZ2"] = 0.74715;//not correct!
  
  LAGDXoffsets["3729"] = -29.6265 + 80 + 28 + 32.05;
  LAGDYoffsets["3729"] = 9.05-15.6052;
  HVres["3729"] = 0.00074715;
  
  LAGDXoffsets["3819"] = -29.6265+ 80 + 28 + 20 + 11.65;
  LAGDYoffsets["3819"] = -15.6052 + 5 + 3.93;
  HVres["3819"] = 0.00062367;
  
  LAGDXoffsets["3846"] = -29.6265+80 + 28 + 20 + 9.01025;
  LAGDYoffsets["3846"] = -15.6052 + 5 + 3.80914;
  HVres["3846"] = 0.00056946;
  
  LAGDXoffsets["3873"] = -29.6265 + 80 + 28 + 20+10.7101;
  LAGDYoffsets["3873"] = -15.6052 + 8.97935;
  HVres["3873"] = 0.00053823;
  
  LAGDXoffsets["3900"] = -29.6265 + 80+28 + 20+10.2803;
  LAGDYoffsets["3900"] = -15.6052 +5  + 3.89424;
  HVres["3900"] = 0.00052005;
  
  LAGDXoffsets["3927"] = -29.6265 +80+28 + 20  +9.34121;
  LAGDYoffsets["3927"] =  -15.6052 +5+  3.88302;
  HVres["3927"] = 0.00050436;
  
  LAGDXoffsets["3953"] = -29.6265 +80+28 + 20  +10.0003;
  LAGDYoffsets["3953"] =  -15.6052 +5+  4.037;
  HVres["3953"] = 0.00050743;
  
  LAGDXoffsets["3981"] = -29.6265 +80+28 + 20+9.05033;
  LAGDYoffsets["3981"] = -15.6052 +5 + 3.76143;
  HVres["3981"] = 0.00048578;
 
  LAGDXoffsets["3998"] = + -29.6265 +80+28 + 20 + 11.0005;
  LAGDYoffsets["3998"] = -15.6052 + 5+3.74;
  HVres["3998"] = 0.00048723;



  //(80 + 120 + 58.68747, 8.31925)
  

  
  vector<float> SMofTheseevents;
  vector<float> Clusterchargeoftheseevnts;
  vector<float> Evtids;
  
  map<int, TCanvas*> PHCanvas;

  
  map<int, TGraph2D*> PulseHeight;
 
  map<int, TGraph2D*> Tracker1XPulseHeight;
  map<int, TGraph2D*> Tracker1YPulseHeight;
  map<int, TGraph2D*> Tracker2XPulseHeight;
  map<int, TGraph2D*> Tracker2YPulseHeight;
  map<int, TGraph2D*> Tracker3XPulseHeight;
  map<int, TGraph2D*> Tracker3YPulseHeight;
  map<int, TGraph2D*> Tracker4XPulseHeight;
  map<int, TGraph2D*> Tracker4YPulseHeight;

  map<int, TGraph2D*> APV2PulseHeight;
  map<int, TGraph2D*> APV3E1PulseHeight;
  map<int, TGraph2D*> APV3E2PulseHeight;
  map<int, TGraph2D*> APV4E3PulseHeight;
  map<int, TGraph2D*> APV4E4PulseHeight;
  map<int, TGraph2D*> APV5E3PulseHeight;
  map<int, TGraph2D*> APV5E4PulseHeight;
  map<int, TGraph2D*> APV6E3PulseHeight;
  map<int, TGraph2D*> APV6E4PulseHeight;
  map<int, TGraph2D*> APV7E1PulseHeight;
  map<int, TGraph2D*> APV7E2PulseHeight;
  map<int, TGraph2D*> APV8and9PulseHeight;
  
  //map<string, TCanvas*> TrackerCanvas;

  TH2F* Tracker1Hitmap;
  TH2F* Tracker2Hitmap;
  TH2F* Tracker3Hitmap;
  TH2F* Tracker4Hitmap;

  
  map<int, int> Pit;

  map<int, int> Tracker1xPit;
  map<int, int> Tracker1yPit;
  map<int, int> Tracker2xPit;
  map<int, int> Tracker2yPit;
  map<int, int> Tracker3xPit;
  map<int, int> Tracker3yPit;
  map<int, int> Tracker4xPit;
  map<int, int> Tracker4yPit;
  
  map<int, int> APV2Pit;
  map<int, int> APV3E1Pit;
  map<int, int> APV3E2Pit;
  map<int, int> APV4E3Pit;
  map<int, int> APV4E4Pit;
  map<int, int> APV5E3Pit;
  map<int, int> APV5E4Pit;
  map<int, int> APV6E3Pit;
  map<int, int> APV6E4Pit;
  map<int, int> APV7E1Pit;
  map<int, int> APV7E2Pit;
  map<int, int> APV8and9Pit;
  
  map<int, int> totalchannel;
  map<int, bool> GoodEvent;

  map<int, int> stripmulti;
  map<int, int> Tracker1xprevstripmulti;
  map<int, int> Tracker1yprevstripmulti;
  map<int, int> Tracker2xprevstripmulti;
  map<int, int> Tracker2yprevstripmulti;
  map<int, int> Tracker3xprevstripmulti;
  map<int, int> Tracker3yprevstripmulti;
  map<int, int> Tracker4xprevstripmulti;
  map<int, int> Tracker4yprevstripmulti;
  
  map<int, TGraph2D*> APV2prevstripmulti;
  map<int, TGraph2D*> APV3E1prevstripmulti;
  map<int, TGraph2D*> APV3E2prevstripmulti;
  map<int, TGraph2D*> APV4E3prevstripmulti;
  map<int, TGraph2D*> APV4E4prevstripmulti;
  map<int, TGraph2D*> APV5E3prevstripmulti;
  map<int, TGraph2D*> APV5E4prevstripmulti;
  map<int, TGraph2D*> APV6E3prevstripmulti;
  map<int, TGraph2D*> APV6E4prevstripmulti;
  map<int, TGraph2D*> APV7E1prevstripmulti;
  map<int, TGraph2D*> APV7E2prevstripmulti;
  map<int, TGraph2D*> APV8prevstripmulti;
  map<int, TGraph2D*> APV9prevstripmulti;
  
  map<string, TH1D*> Clusterdist;

  vector<double> Maxcluster;
  vector<double> ClusterErrors;
  vector<float> ClusterchargeErrors;
  
  map<string, TH1F*> EventAvgCharge;
  vector<double> MaxChargeVec;
  vector<double> ChargeErrorsVec;

  vector<int> HVsetting;
  vector<int> HVErrors;

  map<string, vector<float>> Tracker1hitcoords;
  map<string, vector<float>> Tracker2hitcoords;
  map<string, vector<float>> Tracker3hitcoords;
  map<string, vector<float>> Tracker4hitcoords;
  
  vector<float> TotalTracker1hitcoords;
  vector<float> TotalTracker2hitcoords;
  vector<float> TotalTracker3hitcoords;
  vector<float> TotalTracker4hitcoords;
  
  TH2F* T1ChargeRatio;
  T1ChargeRatio = new TH2F("T1ChargeRatio", "Charge ratio of Tracker 1", 200,0,2500,200,0,2500);

  vector<float> EventIDs;
  vector<float> TrackerEventIDs;
  


  //TFile f("Plots.root", "recreate");


  //user input variables based on what analysis we want

  /*
  Functions in each:
   Display mode 0:
   used for looking at individual APVs and the data from the events
   Generates SM dist plot as well as Average SM and gain per HV setting
   plots events 


   Display mode 1:
   Look at events on all APVs at the same time


   Display mode 2:
   Generate Hitmap from trackers

   Display mode 3:
   look at events on LAGD APVs only

   Display mode 4: 
   look at pedestal values for all APVs

  */
 
  vector<string> BadFiles;
  vector<string> Badevents;

  vector<float> LAGDHitsVec;
  cout << "Event Display (SINGLE APV(0),  ALL(1), TRACKERS(2), LAGD APVs(3), Pedestal Data(4), Residuals(5)):";
  int display_mode=5;
  cin >> display_mode; 

  bool displaymode0event = false;
  bool Clusterdistevents = false;
  bool Gaininfo = false;
  bool ChargeRatio = false;
  bool ClusterChargeDist = false;
  bool Efficiency = false;
  TH1D* ClusterChargeAllFiles = new TH1D("", "Cluster Charge Distribution of (1100, 217);Cluster Charge [AU];Events", 100, 0, 10000);
  bool EventCutflow = false;
  int displaymode0dataoutput;
  int AlignmentMode=0;
  
  /*
  if(display_mode == 5){
    cout << "What Alignment do you want?" << endl;
    cout << "Trackers XYRot (0),  Trackers + GEM XYRot(1)" << endl;
    cin >> AlignmentMode;
  }
  */
  

  
  if(display_mode == 0){
    cout << "What data do you want to see?" << endl;
    cout << "Events? (0), Cluster data? (1), Gain data? (2), ChargeRatio (3), Cluster Charge Dist (4), Event Cut Info (5), Efficiency (6)?: " << endl;
    cin >> displaymode0dataoutput;

    if(displaymode0dataoutput == 0) displaymode0event = true;
    if(displaymode0dataoutput == 1) Clusterdistevents = true;
    if(displaymode0dataoutput == 2) Gaininfo = true;
    if(displaymode0dataoutput == 3) ChargeRatio = true;
    if(displaymode0dataoutput == 4) ClusterChargeDist = true;
    if(displaymode0dataoutput == 5) EventCutflow = true;
    if(displaymode0dataoutput == 6) Efficiency = true;
  }
  
  float apvnum=1;
  
  if(display_mode == 0 || display_mode == 5 || display_mode == 3){
    
    cout << "For reference:" << endl;
    cout << "APV2: 0" << endl;//not being used
    cout << "APV3E1: 1" << endl;//straight strips
    cout << "APV3E2: 1.5" << endl;//small zigzags
    cout << "APV4E3: 2" << endl;//large zigzags
    cout << "APV4E4: 2.5" << endl;//large zigzags
    cout << "APV5E3: 3" << endl;//large zigzags
    cout << "APV5E4: 3.5" << endl;//large zigzags
    cout << "APV6E3: 4" << endl;//large zigzags
    cout << "APV6E4: 4.5" << endl;//large zigzags
    cout << "APV7E1: 5" << endl;//straight strips
    cout << "APV7E2: 5.5" << endl;//small zigzags
    cout << "APV8: 6" << endl;//not being used
    cout << "APV9: 7" << endl;//not being used
    cout << "GEM1X: 8" << endl;//Straight X and Y strips for trackers
    cout << "GEM1Y: 9" << endl;
    cout << "GEM2X: 10" << endl;
    cout << "GEM2Y: 11" << endl;
    cout << "GEM3X: 12" << endl;
    cout << "GEM3Y: 13" << endl;
    cout << "TUGEMX: 14" << endl;
    cout << "TUGEMY: 15" << endl;
    
    cout << "Which APV? ";

    cin >> apvnum;
    
  }

  ofstream ResCorrFile("Aligned_Data/LAGD_Residual_Corrections_Efftest.txt");
  //ofstream ResCorrFileSM2("LAGD_Residual_Corrections_4250_SM2.txt");    
  //ofstream ResCorrFileSM3("LAGD_Residual_Corrections_4250_SM3.txt");    
  //ofstream ResCorrFileSM4("LAGD_Residual_Corrections_4250_SM4.txt");    
  //ofstream ResCorrFileSM5("LAGD_Residual_Corrections_4250_SM5.txt");    
  
  //return;

  //APVindex's

  //OG locations
  //int LAGDDetID = 0;
  
  int APV2index = 0;
  int APV3index = 1;
  int APV4index = 2;
  int APV5index = 3;
  int APV6index = 4;
  int APV7index = 5;
  int APV8index = 6;
  int APV9index = 7;
  
  int Tracker1DetID = 8;
  int Tracker2DetID = 9;
  int Tracker3DetID = 10;
  int Tracker4DetID = 11;
  
  int Tracker1xPlaneID = 1;
  int Tracker1yPlaneID = 0;
  int Tracker2xPlaneID = 1;
  int Tracker2yPlaneID = 0;
  int Tracker3xPlaneID = 1;
  int Tracker3yPlaneID = 0;
  int Tracker4xPlaneID = 1;
  int Tracker4yPlaneID = 0;

  
  if(display_mode == 2){
    //define Hitmaps for each tracker per file
    Tracker1Hitmap = new TH2F("PHT1", "Tracker 1 Hitmap; x Distance; y distance, Hits", 100, 0, 50, 100, 0, 50);
    Tracker3Hitmap = new TH2F("PHT3", "Tracker 3 Hitmap; x Distance; y distance, Hits", 100, 0, 50, 100, 0, 50);
    Tracker2Hitmap = new TH2F("PHT2", "Tracker 2 Hitmap; x Distance; y distance, Hits", 100, 0, 50, 100, 0, 50);
    Tracker4Hitmap = new TH2F("PHT4", "Tracker 4 Hitmap; x Distance; y distance, Hits", 100, 0, 50, 100, 0, 50);

  }

  
  map<float, string> APVname;
  //maps this number to the APV in that range of strips
  APVname[15] =  "TUGEMY";
  APVname[14] =  "TUGEMX";
  APVname[13] =  "GEM3Y";
  APVname[12] =  "GEM3X";
  APVname[11] =  "GEM2Y";
  APVname[10] =  "GEM2X";
  APVname[9] =  "GEM1Y";
  APVname[8] =  "GEM1X";
  APVname[7] =  "APV9";
  APVname[6] =  "APV8";
  APVname[5] =   "APV7E1";
  APVname[5.5] =   "APV7E2";
  APVname[4] =   "APV6E3";
  APVname[4.5] =   "APV6E4";
  APVname[3] =   "APV5E3";
  APVname[3.5] =   "APV5E4";
  APVname[2] =   "APV4E3";
  APVname[2.5] =   "APV4E4";
  APVname[1] =   "APV3E1";
  APVname[1.5] =   "APV3E2";
  APVname[0] =   "APV2";


  map<string, vector<float>> LAGDMaxStrips;
  map<string, vector<float>> LAGDMaxCharges;
  map<string, vector<float>> LAGDTimeBin;
  auto DeltaTTrackersLAGD = new TH1D("DeltaT", "Time Difference between Tracker and LAGD coincidence hit;Time difference [Clock Ticks]; Instances", 13, -6.5, 6.5);

  map<string, float> LAGDnum;
  map<string, float> LAGDtotalcharge;
  map<string, float> LAGDloc;



  //here we define the number of events we want to either plot or analyze
  int plotnum = 5001;
  if(display_mode == 4)
    plotnum = 1;
  if(display_mode == 2 || display_mode == 5)
    plotnum = 5000;
  if(displaymode0event || display_mode == 1 || display_mode == 3){ 
    cout << "How many events? ";
    cin >> plotnum;    
  }
  //strip correltion plots need to have different Y bins for the LAGD depending on the file!!
  auto Stripcorr = new TH2D("", "", 100, 0, 100, 150, 0, 150);
  auto TrackerStripcorr = new TH2D("", "", 120, 0, 120, 100, 0, 100);  
  int Stripcorrit=0;
  auto StripcorrProf = new TProfile("Stripcorr", "Profile plot of Strip correlation pre alignment;Tracker 1y strip [strip]; LAGD strip [strip]", 10, 0, 100, "s");
  auto TrackerStripcorrProf = new TProfile("TrackerStripcorr", "Profile plot of Strip correlation pre alignment;Tracker 1y strip [strip]; Tracker 3y strip [strip]", 10, 0, 100,"s");
  auto LAGDstrips = new TGraph();
  auto Trackerstrips = new TGraph();
  auto Diffbtwnstrips = new TGraph();
  
  vector<float> EfficiencyPerHV;
  vector<float> EfficiencyErrorPerHV;
  vector<float> NumNoLAGDEvent(9);
  vector<float> NumNoLAGDEventSeen(9);
  

    
  int fileit=0;
  
  double totalEventiterator=0;
  vector<int> EventIterator;
  vector<int> BadEvents_OGIndex;
  vector<int> GoodEvents_OGIndex;
  auto StripChargeHist = new TH1F("StripCharges", "Strip Charge Distribution of Poorly Recreated Events; Charge (AU); Counts/4",200 , 0, 800);
  auto ClusterChargeHist = new TH1F("ClusterCharges", "Cluster Charge Distribution of (1100, 217); Charge (AU); Counts/25", 80, 0, 2000);
  auto ClusterNumberHist = new TH1F("Clusterhits", "Cluster Number Distribution at (1200, 217); Number of Clusters per event; Events/4",11 , -0.5, 10.5);
  auto StripsHist = new TH1F("Strips", "Strip Distribution of Poorly Recreated Events; Strip (ID); Counts/4", 300, 0, 128);
  //auto ClusterChargeHist = new TH1F("ClusterCharges", "Cluster Charge Distribution of Correctly Recreated Events; Charge (AU); Counts/4",200 , 0, 1500);
  auto StripsHithist = new TH1F("Strips", "Strip hit Distribution in LAGD; Strip (ID); Counts", 128, 0, 128);
  int striphitit=0;
  int toleranceit = 0;
  TH1D* Tracker2Ystriphits = new TH1D("", "", 128, 0, 128);
  TH1D* ClusterdistAll = new TH1D("Clusterdist", "Strip Multiplicty Distribution of all data at 1200, 217", 10, 0.5, 10.5);
  TH1D* EventsLostToTrackers = new TH1D("LostEvents", "Events Lost distribution", 2, 0.5,2.5);
  //cout << inputfiles.size() << endl;
  //this for loop will iterate through the data files
  int numEventsWith0hitclusterintrackers = 0;
  int numEventsWith2ormorehitclusterintrackers = 0;
  vector<int> LAGDEventCutflowinfo(11); //num events w cluster num == 0, ""below 5, ..., ""equal to 1, Num events SM == 1, Num events with SM below 5, ""below 4, ..., "" == 2
  vector<int> TrackerEventCutflowinfo(10); //Tracker1x num clusters == 1,2,3; AND Tracker1y num clusters == 1,2,3; AND Tracker2x num clusters == 1,2,3; ....  
  vector<int> TrackerEventCutflowinfo2(10); //Tracker1x num clusters == 1,2; AND Tracker1y num clusters == 1,2; AND Tracker2x num clusters == 1,2; .... 
  vector<int> TrackerEventCutflowinfo3(10); //Tracker1x num clusters == 1; AND Tracker1y num clusters == 1; AND Tracker2x num clusters == 1; .... 

  auto xHits0 = new TH1D("0xhit", "Instances of N number of trackers with 0 x hit; N Trackers; Instances", 4, 0.5, 4.5);
  auto xHits1 = new TH1D("1xhit", "Instances of N number of trackers with 1 x hit; N Trackers; Instances", 4, 0.5, 4.5);
  auto xHits2 = new TH1D("2xhit", "Instances of N number of trackers with 2 x hits; N Trackers; Instances", 4, 0.5, 4.5);
  auto xHits3 = new TH1D("3xhit", "Instances of N number of trackers with 3 x hits; N Trackers; Instances", 4, 0.5, 4.5);
  auto xHits4 = new TH1D("4xhit", "Instances of N number of trackers with 4 x hits; N Trackers; Instances", 4, 0.5, 4.5);
  auto xHits5 = new TH1D("5xhit", "Instances of N number of trackers with 5 x hits; N Trackers; Instances", 4, 0.5, 4.5);
  auto yHits0 = new TH1D("0yhit", "Instances of N number of trackers with 0 y hit; N Trackers; Instances", 4, 0.5, 4.5);
  auto yHits1 = new TH1D("1yhit", "Instances of N number of trackers with 1 y hit; N Trackers; Instances", 4, 0.5, 4.5);
  auto yHits2 = new TH1D("2yhit", "Instances of N number of trackers with 2 y hits; N Trackers; Instances", 4, 0.5, 4.5);
  auto yHits3 = new TH1D("3yhit", "Instances of N number of trackers with 3 y hits; N Trackers; Instances", 4, 0.5, 4.5);
  auto yHits4 = new TH1D("4yhit", "Instances of N number of trackers with 4 y hits; N Trackers; Instances", 4, 0.5, 4.5);
  auto yHits5 = new TH1D("5yhit", "Instances of N number of trackers with 5 y hits; N Trackers; Instances", 4, 0.5, 4.5);
  
  auto SMloss = new TH1D("SMloss", "Instances of LAGD or tracker SM outside range; Detectors; Instances", 9, 0.5, 9.5);

  auto SM1x = new TH1D("1xSM", "SM Dist of Tracker 1x; N strips; Instances", 10, 0.5, 10.5);
  auto SM1y = new TH1D("1ySM", "SM Dist of Tracker 1y; N strips; Instances", 10, 0.5, 10.5);
  auto SM2x = new TH1D("2xSM", "SM Dist of Tracker 2x; N strips; Instances", 10, 0.5, 10.5);
  auto SM2y = new TH1D("2ySM", "SM Dist of Tracker 2y; N strips; Instances", 10, 0.5, 10.5);
  auto SM3x = new TH1D("3xSM", "SM Dist of Tracker 3x; N strips; Instances", 10, 0.5, 10.5);
  auto SM3y = new TH1D("3ySM", "SM Dist of Tracker 3y; N strips; Instances", 10, 0.5, 10.5);
  auto SM4x = new TH1D("4xSM", "SM Dist of Tracker 4x; N strips; Instances", 10, 0.5, 10.5);
  auto SM4y = new TH1D("4ySM", "SM Dist of Tracker 4y; N strips; Instances", 10, 0.5, 10.5);
  auto SMLAGD = new TH1D("SMLAGD", "SM Dist of LAGD; N strips; Instances", 10, 0.5, 10.5);


  int lostcounter=0;
  
  for(map<string,TString>::iterator i = inputfiles.begin(); i != inputfiles.end(); i++){
    TFile* myFile = TFile::Open(i->second);
    string evtype = i->first;



    if(stoi(evtype) < 30){
      string locstring = std::string(inputfiles[evtype].Data());
      location = locstring.substr(0,8);
      cout << location << endl;
    }
    else{
      location = evtype;
      cout << location << endl;
    }

    //    return;
    //vector<float> APV3E1Range = CorrectCrosstalk(myFile);
    
    //open up the branch
    TTreeReader myReader("THit", myFile);
    
    //assign trees and leaves to new trees and leaves
    //info about the sector
    TTreeReaderValue<int>      evtID(myReader, "evtID");
    TTreeReaderValue<int>      nch(myReader, "nch");
    TTreeReaderArray<int>      strip(myReader, "strip"); 
    //TTreeReaderArray<string>      hitTimebin(myReader, "hitTimebin");//this variable is not working well :(   
    TTreeReaderArray<int>      detID(myReader, "detID");
    TTreeReaderArray<int>      apvID(myReader, "apvID");
    
    TTreeReaderArray<int>      planeID(myReader, "planeID");
    TTreeReaderArray<short>      etasec(myReader, "etaSector");
    //data for pulse height

    TTreeReaderArray<short>      adc0(myReader, "adc0");     
    TTreeReaderArray<short>      adc1(myReader, "adc1");    
    TTreeReaderArray<short>      adc2(myReader, "adc2");
    TTreeReaderArray<short>      adc3(myReader, "adc3");
    TTreeReaderArray<short>      adc4(myReader, "adc4");
    TTreeReaderArray<short>      adc5(myReader, "adc5");

    
    //loop over events to add them all onto one file
    //cout << evtype << endl;
    //int it = 0;

    

    //define other file-based histograms for other display modes
    Clusterdist[evtype] = new TH1D("Clusterdist", ("Strip Multiplicty Distribution with HV "+evtype).c_str(), 6, 1, 7);
    
    
    //iterate through each event

  
    /*
    for(auto i = BadEventVector.begin(); i != BadEventVector.end(); i++){
      cout << *i << endl;
    }
    */
    
    EventAvgCharge[evtype] = new TH1F("", "", 300, 0, 2500);
    
    bool GoodFile = true;
    float LAGDhitsnum=0;
    float Trackerhits=0;
    
    while (myReader.Next() && GoodFile){// && *nch == 2048){
      cout << "total channels: " << *nch << endl;

      // if(*nch != 2048){
      // 	cout << "Channel number mismatch! check file " << inputfiles[evtype] << " :(" << endl;
      // 	BadFiles.push_back(evtype);
      // 	BadFiles.push_back(to_string(*evtID));
      // 	BadFiles.push_back(to_string(*nch));
      // 	GoodFile = false;
      // 	//break;
      // 	//continue;
	
      // 	//return;
      // }
      
      //////////////////////////////////////////////herererererer for nch thing
      //if(*nch != 2048){continue;}
      totalEventiterator++;
      cout << "Event ID: " << *evtID << endl;
      //if(find(AllEventsVector.begin(), AllEventsVector.end(), totalEventiterator) != AllEventsVector.end() || *evtID == 1){// == vector.end() means the event ID isnt in the vector
	//Top inequality is always != so only events considered in the tracking analysis. To plot only good events, set bottom to !=,  opposite for plotting bad events
      
	//if(*evtID != 1){
      //if(find(BadEventVector.begin(), BadEventVector.end(), totalEventiterator) != BadEventVector.end()){continue;}//so != vector.end() means the event IS in that vector 
      //	}
      cout << "Total Event ID: " << totalEventiterator << endl;
      
      //if the event number is larger than what we want to plot, stop
      if(*evtID > plotnum) break;
      //plotnum = 2;
      //if(totalEventiterator != 1316 && totalEventiterator != 1){continue;}//need to include event 1 or else tolerance wont be set to discriminate hits from noise
      // cout << "continues after?" << endl;
      
      //iterating variables
      Pit[*evtID] = 0;

      Tracker1xPit[*evtID] = 0;
      Tracker1yPit[*evtID] = 0;
      Tracker2xPit[*evtID] = 0;
      Tracker2yPit[*evtID] = 0;
      Tracker3xPit[*evtID] = 0;
      Tracker3yPit[*evtID] = 0;
      Tracker4xPit[*evtID] = 0;
      Tracker4yPit[*evtID] = 0;

      APV2Pit[*evtID] = 0;
      APV3E1Pit[*evtID] = 0;
      APV3E2Pit[*evtID] = 0;
      APV4E3Pit[*evtID] = 0;
      APV4E4Pit[*evtID] = 0;
      APV5E3Pit[*evtID] = 0;
      APV5E4Pit[*evtID] = 0;
      APV6E3Pit[*evtID] = 0;
      APV6E4Pit[*evtID] = 0;
      APV7E1Pit[*evtID] = 0;
      APV7E2Pit[*evtID] = 0;
      APV8and9Pit[*evtID] = 0;
      
      totalchannel[*evtID] = 0;
      
      //time to define more variables that are now event based

      
      vector<int> PedXvals;
      for(int y = 1; y <= 128; y++)
	PedXvals.push_back(y);
      
      vector<float> Tracker1xPedstds;
      vector<float> Tracker1yPedstds;      
      vector<float> Tracker2xPedstds;
      vector<float> Tracker2yPedstds;
      vector<float> Tracker3xPedstds;
      vector<float> Tracker3yPedstds;
      vector<float> Tracker4xPedstds;
      vector<float> Tracker4yPedstds;

      vector<float> APV2Pedstds;
      vector<float> APV3E1Pedstds;
      vector<float> APV3E2Pedstds;
      vector<float> APV4E3Pedstds;
      vector<float> APV4E4Pedstds;
      vector<float> APV5E3Pedstds;
      vector<float> APV5E4Pedstds;
      vector<float> APV6E3Pedstds;
      vector<float> APV6E4Pedstds;
      vector<float> APV7E1Pedstds;
      vector<float> APV7E2Pedstds;
      vector<float> APV8Pedstds;
      vector<float> APV9Pedstds;

      vector<float> stripcharges;
     
      vector<float> Hitcoords;
      cout << display_mode << endl;
      if(display_mode != 5){
	PulseHeight[*evtID] = new TGraph2D(2);
	//cout << "Better not be here rn" << endl;
	Tracker1XPulseHeight[*evtID] = new TGraph2D(2);
	Tracker1YPulseHeight[*evtID] = new TGraph2D(2);            
	Tracker2XPulseHeight[*evtID] = new TGraph2D(2);
	Tracker2YPulseHeight[*evtID] = new TGraph2D(2);
	Tracker3XPulseHeight[*evtID] = new TGraph2D(2);
	Tracker3YPulseHeight[*evtID] = new TGraph2D(2);
	Tracker4XPulseHeight[*evtID] = new TGraph2D(2);
	Tracker4YPulseHeight[*evtID] = new TGraph2D(2);
      
     
	//Uneccesary ATM since theyre more complicated
      
	APV2PulseHeight[*evtID] = new TGraph2D(2);
	APV8and9PulseHeight[*evtID] = new TGraph2D(2);
	APV3E1PulseHeight[*evtID] = new TGraph2D(2);
	APV3E2PulseHeight[*evtID] = new TGraph2D(2);
	APV4E3PulseHeight[*evtID] = new TGraph2D(2);
	APV4E4PulseHeight[*evtID] = new TGraph2D(2);
	APV5E3PulseHeight[*evtID] = new TGraph2D(2);
	APV5E4PulseHeight[*evtID] = new TGraph2D(2);
	APV6E3PulseHeight[*evtID] = new TGraph2D(2);
	APV6E4PulseHeight[*evtID] = new TGraph2D(2);
	APV7E1PulseHeight[*evtID] = new TGraph2D(2);
	APV7E2PulseHeight[*evtID] = new TGraph2D(2);          
      }

      /*
      vector<float> Tracker1hitcoords;//stored as (X, Y, Z)
      vector<float> Tracker2hitcoords;
      vector<float> Tracker3hitcoords;
      vector<float> Tracker4hitcoords;
      */
      /*
      vector<float> APV2hitcoords;
      vector<float> APV3E1hitcoords;
      vector<float> APV3E2hitcoords;
      vector<float> APV4E3hitcoords;
      vector<float> APV4E4hitcoords;
      vector<float> APV5E3hitcoords;
      vector<float> APV5E4hitcoords;
      vector<float> APV6E3hitcoords;
      vector<float> APV6E4hitcoords;
      vector<float> APV7E1hitcoords;
      vector<float> APV7E2hitcoords;
      vector<float> APV8hitcoords;
      vector<float> APV9hitcoords;
      */

      
      vector<float> maxstrips;
      vector<float> Tracker1xmaxstrips;
      vector<float> Tracker1ymaxstrips;      
      vector<float> Tracker2xmaxstrips;
      vector<float> Tracker2ymaxstrips;
      vector<float> Tracker3xmaxstrips;
      vector<float> Tracker3ymaxstrips;
      vector<float> Tracker4xmaxstrips;
      vector<float> Tracker4ymaxstrips;

      
      vector<float> maxcharges;
      vector<float> Tracker1xmaxcharges;
      vector<float> Tracker1ymaxcharges;
      vector<float> Tracker2xmaxcharges;
      vector<float> Tracker2ymaxcharges;
      vector<float> Tracker3xmaxcharges;
      vector<float> Tracker3ymaxcharges;
      vector<float> Tracker4xmaxcharges;
      vector<float> Tracker4ymaxcharges;

      vector<float> maxtimebin;
      vector<float> Tracker1xtimebin;
      vector<float> Tracker1ytimebin;
      vector<float> Tracker2xtimebin;
      vector<float> Tracker2ytimebin;
      vector<float> Tracker3xtimebin;
      vector<float> Tracker3ytimebin;
      vector<float> Tracker4xtimebin;
      vector<float> Tracker4ytimebin;

      
      LAGDMaxCharges["APV9"].clear();
      LAGDMaxCharges["APV8"].clear();
      LAGDMaxCharges["APV7E1"].clear();
      LAGDMaxCharges["APV7E2"].clear();
      LAGDMaxCharges["APV6E3"].clear();
      LAGDMaxCharges["APV6E4"].clear();
      LAGDMaxCharges["APV5E3"].clear();
      LAGDMaxCharges["APV5E4"].clear();
      LAGDMaxCharges["APV4E3"].clear();
      LAGDMaxCharges["APV4E4"].clear();
      LAGDMaxCharges["APV3E1"].clear();
      LAGDMaxCharges["APV3E2"].clear();
      LAGDMaxCharges["APV2"].clear();

      LAGDMaxStrips["APV9"].clear();
      LAGDMaxStrips["APV8"].clear();
      LAGDMaxStrips["APV7E1"].clear();
      LAGDMaxStrips["APV7E2"].clear();
      LAGDMaxStrips["APV6E3"].clear();
      LAGDMaxStrips["APV6E4"].clear();
      LAGDMaxStrips["APV5E3"].clear();
      LAGDMaxStrips["APV5E4"].clear();
      LAGDMaxStrips["APV4E3"].clear();
      LAGDMaxStrips["APV4E4"].clear();
      LAGDMaxStrips["APV3E1"].clear();
      LAGDMaxStrips["APV3E2"].clear();
      LAGDMaxStrips["APV2"].clear();

      LAGDTimeBin["APV9"].clear();
      LAGDTimeBin["APV8"].clear();
      LAGDTimeBin["APV7E1"].clear();
      LAGDTimeBin["APV7E2"].clear();
      LAGDTimeBin["APV6E3"].clear();
      LAGDTimeBin["APV6E4"].clear();
      LAGDTimeBin["APV5E3"].clear();
      LAGDTimeBin["APV5E4"].clear();
      LAGDTimeBin["APV4E3"].clear();
      LAGDTimeBin["APV4E4"].clear();
      LAGDTimeBin["APV3E1"].clear();
      LAGDTimeBin["APV3E2"].clear();
      LAGDTimeBin["APV2"].clear();
      

      float prevstripmax=0;
      
      float futstripmax=0;
      float maxcharge=0;
      

    
      int h=0;
      /*
      //for strip means and std devs plot
      
      vector<float> means;
      vector<float> stddevs;
      vector<float> xvals;
      vector<float> xerr;//for consistancy            
      */
      //cout << "Here?" << endl;
      for (uint i = 0; i < abs(*nch); i++){
	
	if(*evtID == 1){

	  if(i == 0){cout << "|  detID  |  stripID  |  PlaneID  | APVID |" << endl;}
	  h = 256;
	  if((strip)[i] < 128)
	    h = 128;
	  
	  
	  if((strip)[i]%128 == 0)
	    cout << "|     " << (detID)[i] << "     |    " << (strip)[i] << "    |    " << (planeID)[i] << "    |   " << (apvID)[i] << "   |    " << endl;   
	  /*
	  if((detID)[i] == 0)
	    cout << "From Event: " << *evtID << "; this is Strip: " << (strip)[i]  << " from APV: " << (apvID)[i] + 2 << " of plane: " << (planeID)[i] << endl;
	  if((detID)[i] > 0)
	    cout << "From Event: " << *evtID << "; this is Strip: " << (strip)[i]  << " from APV: " << (apvID)[i]  << " of plane: " << (planeID)[i] << endl;
	  */
	  //toliter = (*evtID)*(*nch) + i;
	  
	  float stdev=0;
	  float mean=0;
	
	  //cout << "yes" << endl;
	  stdev = adcsstrip[i]->GetStdDev();///sqrt(adcsstrip[i]->GetEntries());
	  mean = adcsstrip[i]->GetMean();
	  //cout << stdev << endl;

	  striptol[i] = mean + 5*stdev;
	  //striptol[i] = 40;
	  cout << "Strip: " << i << " has noise tolerance: " << striptol[i] <<  endl;
	  //return;
	}
	
	//vector<vector<short>> adcvals = {(adc0), (adc1), (adc2), (adc3), (adc4), (adc5)};
	//cout << "Here " << i <<  endl;

	//this chunk is for visualizing individual APV events
	
	if(display_mode == 0 && totalchannel[*evtID] >= 128*apvnum && totalchannel[*evtID] <= 64 + 128*apvnum){	 
	   
	  if(displaymode0event){	    
	    PulseHeight[*evtID]->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);	      
	    PulseHeight[*evtID]->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	    PulseHeight[*evtID]->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	    PulseHeight[*evtID]->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	    PulseHeight[*evtID]->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	    PulseHeight[*evtID]->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);	 
	  }
	  
	  stripcharges.push_back((adc0)[i]);	
	  stripcharges.push_back((adc1)[i]);
	  stripcharges.push_back((adc2)[i]);
	  stripcharges.push_back((adc3)[i]);
	  stripcharges.push_back((adc4)[i]);
	  stripcharges.push_back((adc5)[i]);	  
	  
	  
	  maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	  auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(),stripcharges.end()));
	  
	  if(maxcharge >= striptol[i]){
	    maxcharges.push_back(maxcharge-striptol[i]);
	    maxstrips.push_back((strip)[i]);
	    maxtimebin.push_back(timebin);
	    cout << "Hit timebin: " << timebin << endl;
	  
	    StripChargeHist->Fill(maxcharge-striptol[i]);
	    StripsHist->Fill((strip)[i]);

	  }	  
	  stripcharges.clear();	  
	}
	
	
	if(display_mode == 0 || display_mode == 1 || display_mode == 2 || display_mode == 4 || display_mode == 5){
	  	 
	  //Fill Tracker Histograms/look for hits
	  if((detID)[i] ==  Tracker1DetID && (planeID)[i] == Tracker1xPlaneID){
	  
	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      cout << "doesnt go here I hope" << endl;
	      Tracker1XPulseHeight[*evtID]->SetPoint(Tracker1xPit[*evtID]++, (strip)[i], 0, (adc0)[i]);	      
	      Tracker1XPulseHeight[*evtID]->SetPoint(Tracker1xPit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      Tracker1XPulseHeight[*evtID]->SetPoint(Tracker1xPit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      Tracker1XPulseHeight[*evtID]->SetPoint(Tracker1xPit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      Tracker1XPulseHeight[*evtID]->SetPoint(Tracker1xPit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      Tracker1XPulseHeight[*evtID]->SetPoint(Tracker1xPit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	      
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(),stripcharges.end()));
	  
	    if(maxcharge >= striptol[i]){
		
	      Tracker1xmaxcharges.push_back(maxcharge-striptol[i]);				
	      Tracker1xmaxstrips.push_back((strip)[i]%128);
	      Tracker1xtimebin.push_back(timebin);
		
	    }
	    
	    stripcharges.clear();
	    
	    if(*evtID == 1)
	      //Tracker1xPedstds.insert(0, stdev);
	      Tracker1xPedstds.push_back(striptol[i]);	      	      
	  }
	  //cout << "Pass tracker 1?" << endl;
	  if((detID)[i] == Tracker3DetID && (planeID)[i] == Tracker3xPlaneID){//og 12 cjanged to 12, now 1
	    
	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      Tracker3XPulseHeight[*evtID]->SetPoint(Tracker3xPit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      Tracker3XPulseHeight[*evtID]->SetPoint(Tracker3xPit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      Tracker3XPulseHeight[*evtID]->SetPoint(Tracker3xPit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      Tracker3XPulseHeight[*evtID]->SetPoint(Tracker3xPit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      Tracker3XPulseHeight[*evtID]->SetPoint(Tracker3xPit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      Tracker3XPulseHeight[*evtID]->SetPoint(Tracker3xPit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }

	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);
	     
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(),stripcharges.end()));
	  
	    if(maxcharge >=  striptol[i]){
		
	      Tracker3xmaxcharges.push_back(maxcharge-striptol[i]);
	      Tracker3xmaxstrips.push_back((strip)[i]%128);
	      Tracker3xtimebin.push_back(timebin);
	    }
	      
	    stripcharges.clear();	  	  
	    if(*evtID == 1)
	      Tracker3xPedstds.push_back(striptol[i]);	      
	      
	  }
	      
	  if((detID)[i] == Tracker1DetID && (planeID)[i] == Tracker1yPlaneID){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      Tracker1YPulseHeight[*evtID]->SetPoint(Tracker1yPit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      Tracker1YPulseHeight[*evtID]->SetPoint(Tracker1yPit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      Tracker1YPulseHeight[*evtID]->SetPoint(Tracker1yPit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      Tracker1YPulseHeight[*evtID]->SetPoint(Tracker1yPit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      Tracker1YPulseHeight[*evtID]->SetPoint(Tracker1yPit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);    
	      Tracker1YPulseHeight[*evtID]->SetPoint(Tracker1yPit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	      
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);	      

	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());	   	      
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(),stripcharges.end()));
	    
	    if(maxcharge >= striptol[i]){
		  
	      Tracker1ymaxcharges.push_back(maxcharge-striptol[i]);		  
	      Tracker1ymaxstrips.push_back((strip)[i]%128);
	      Tracker1ytimebin.push_back(timebin);
	    }
	      
	    stripcharges.clear();
	    if(*evtID == 1)
	      Tracker1yPedstds.push_back(striptol[i]);	      
	      
	  }
	    
	  if((detID)[i] == Tracker3DetID && (planeID)[i] == Tracker3yPlaneID){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      Tracker3YPulseHeight[*evtID]->SetPoint(Tracker3yPit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      Tracker3YPulseHeight[*evtID]->SetPoint(Tracker3yPit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      Tracker3YPulseHeight[*evtID]->SetPoint(Tracker3yPit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      Tracker3YPulseHeight[*evtID]->SetPoint(Tracker3yPit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      Tracker3YPulseHeight[*evtID]->SetPoint(Tracker3yPit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      Tracker3YPulseHeight[*evtID]->SetPoint(Tracker3yPit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }

	      
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);	      

	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(),stripcharges.end()));
	    if(maxcharge >=  striptol[i]){

	      Tracker3ymaxcharges.push_back(maxcharge-striptol[i]);
	      Tracker3ymaxstrips.push_back((strip)[i]%128);
	      Tracker3ytimebin.push_back(timebin);
	    }
	      
	    stripcharges.clear();
	    if(*evtID == 1)
	      Tracker3yPedstds.push_back(striptol[i]);	      
	      
	  }
	    
	  if((detID)[i] == Tracker2DetID && (planeID)[i] == Tracker2xPlaneID){//og 10, switched to 14, now 5

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      Tracker2XPulseHeight[*evtID]->SetPoint(Tracker2xPit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);  
	      Tracker2XPulseHeight[*evtID]->SetPoint(Tracker2xPit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      Tracker2XPulseHeight[*evtID]->SetPoint(Tracker2xPit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      Tracker2XPulseHeight[*evtID]->SetPoint(Tracker2xPit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      Tracker2XPulseHeight[*evtID]->SetPoint(Tracker2xPit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      Tracker2XPulseHeight[*evtID]->SetPoint(Tracker2xPit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);		
	    }
		
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);
		
	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(),stripcharges.end()));
	    
	    if(maxcharge >=  striptol[i]){

	      Tracker2xmaxcharges.push_back(maxcharge-striptol[i]);
	      Tracker2xmaxstrips.push_back((strip)[i]%128);
	      Tracker2xtimebin.push_back(timebin);
	    }
		
	    stripcharges.clear();
	    if(*evtID == 1)
	      Tracker2xPedstds.push_back(striptol[i]);	      
		
	      		
	  }
	  if((detID)[i] == Tracker4DetID && (planeID)[i] == Tracker4xPlaneID){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      Tracker4XPulseHeight[*evtID]->SetPoint(Tracker4xPit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      Tracker4XPulseHeight[*evtID]->SetPoint(Tracker4xPit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      Tracker4XPulseHeight[*evtID]->SetPoint(Tracker4xPit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      Tracker4XPulseHeight[*evtID]->SetPoint(Tracker4xPit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      Tracker4XPulseHeight[*evtID]->SetPoint(Tracker4xPit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      Tracker4XPulseHeight[*evtID]->SetPoint(Tracker4xPit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
		
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);


	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(),stripcharges.end()));

	    if(maxcharge >=  striptol[i]){
	      Tracker4xmaxcharges.push_back(maxcharge-striptol[i]);
	      Tracker4xmaxstrips.push_back((strip)[i]%128);
	      Tracker4xtimebin.push_back(timebin);
	    }
		
	    stripcharges.clear();
	    if(*evtID == 1)
	      Tracker4xPedstds.push_back(striptol[i]);	      	      
	      
	  }

	      
	  if((detID)[i] == Tracker2DetID && (planeID)[i] == Tracker2yPlaneID){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      Tracker2YPulseHeight[*evtID]->SetPoint(Tracker2yPit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      Tracker2YPulseHeight[*evtID]->SetPoint(Tracker2yPit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      Tracker2YPulseHeight[*evtID]->SetPoint(Tracker2yPit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      Tracker2YPulseHeight[*evtID]->SetPoint(Tracker2yPit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      Tracker2YPulseHeight[*evtID]->SetPoint(Tracker2yPit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      Tracker2YPulseHeight[*evtID]->SetPoint(Tracker2yPit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
		
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(),stripcharges.end()));
		    
	    if(maxcharge >=  striptol[i]){
	      //cout << "Here?" << endl;
	      Tracker2ymaxcharges.push_back(maxcharge-striptol[i]);
	      Tracker2ymaxstrips.push_back((strip)[i]%128);
	      Tracker2ytimebin.push_back(timebin);
		  
	    }
		
	    stripcharges.clear();
	    if(*evtID == 1)
	      Tracker2yPedstds.push_back(striptol[i]);	      	      
		
	  }
	  
	  if((detID)[i] == Tracker4DetID && (planeID)[i] == Tracker4yPlaneID){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      Tracker4YPulseHeight[*evtID]->SetPoint(Tracker4yPit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      Tracker4YPulseHeight[*evtID]->SetPoint(Tracker4yPit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      Tracker4YPulseHeight[*evtID]->SetPoint(Tracker4yPit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      Tracker4YPulseHeight[*evtID]->SetPoint(Tracker4yPit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      Tracker4YPulseHeight[*evtID]->SetPoint(Tracker4yPit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      Tracker4YPulseHeight[*evtID]->SetPoint(Tracker4yPit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
		
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(),stripcharges.end()));

	    if(maxcharge >=  striptol[i]){
	      Tracker4ymaxcharges.push_back(maxcharge-striptol[i]);
	      Tracker4ymaxstrips.push_back((strip)[i]%128);
	      Tracker4ytimebin.push_back(timebin);
	    }
		
	    stripcharges.clear();	  	  
	    if(*evtID == 1)
	      Tracker4yPedstds.push_back(striptol[i]);	      	      
	      
	  }
	  //cout << "Pass trackers?" << endl;
	}
	//end filling tracker plots
	
	if(display_mode == 1 || display_mode == 2 || display_mode == 3 || display_mode == 4 || display_mode == 5){
	  //start LAGD APVs
	  
	  //To save space, we do not need APVs 2, 8, and 9 for now
	  if((detID)[i] == APV8index && (apvID)[i] == APV8index){
	     
	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	           
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(),stripcharges.end()));
	    
	    if(maxcharge >=  striptol[i]){			
	      LAGDMaxCharges["APV8"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV8"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV8"].push_back(timebin);
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV8Pedstds.push_back(striptol[i]);	      	      	      	
	  }
	  
	
	  if((detID)[i] == APV9index && (apvID)[i] == APV9index){
	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV8and9PulseHeight[*evtID]->SetPoint(APV8and9Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	    	       
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	  
	    if(maxcharge >=  striptol[i]){			
	      LAGDMaxCharges["APV9"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV9"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV9"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV9Pedstds.push_back(striptol[i]);	      	      
	      
	  }	  
	  
	  if((detID)[i] == APV5index && (apvID)[i] == APV5index && (strip)[i]%128 < 64){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV5E3PulseHeight[*evtID]->SetPoint(APV5E3Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV5E3PulseHeight[*evtID]->SetPoint(APV5E3Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV5E3PulseHeight[*evtID]->SetPoint(APV5E3Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV5E3PulseHeight[*evtID]->SetPoint(APV5E3Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV5E3PulseHeight[*evtID]->SetPoint(APV5E3Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV5E3PulseHeight[*evtID]->SetPoint(APV5E3Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	       
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	  
	    if(maxcharge >=  striptol[i]){			
	      LAGDMaxCharges["APV5E3"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV5E3"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV5E3"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV5E3Pedstds.push_back(striptol[i]);	      	      
	      
	  }
	  
	  if((detID)[i] == APV5index && (apvID)[i] == APV5index && (strip)[i]%128 >= 64){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV5E4PulseHeight[*evtID]->SetPoint(APV5E4Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV5E4PulseHeight[*evtID]->SetPoint(APV5E4Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV5E4PulseHeight[*evtID]->SetPoint(APV5E4Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV5E4PulseHeight[*evtID]->SetPoint(APV5E4Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV5E4PulseHeight[*evtID]->SetPoint(APV5E4Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV5E4PulseHeight[*evtID]->SetPoint(APV5E4Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	           
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);	    

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	    
	    if(maxcharge >=  striptol[i]){			
	      LAGDMaxCharges["APV5E4"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV5E4"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV5E4"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV5E4Pedstds.push_back(striptol[i]);	      	      
	      
	  }
	  

	  if((detID)[i] == APV4index && (apvID)[i] == APV4index && (strip)[i]%128 < 64){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV4E3PulseHeight[*evtID]->SetPoint(APV4E3Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV4E3PulseHeight[*evtID]->SetPoint(APV4E3Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV4E3PulseHeight[*evtID]->SetPoint(APV4E3Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV4E3PulseHeight[*evtID]->SetPoint(APV4E3Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV4E3PulseHeight[*evtID]->SetPoint(APV4E3Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV4E3PulseHeight[*evtID]->SetPoint(APV4E3Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	           
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);
	    
	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	    
	    if(maxcharge >= striptol[i]){			
	      LAGDMaxCharges["APV4E3"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV4E3"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV4E3"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV4E3Pedstds.push_back(striptol[i]);	      
	  }
	  
	  if((detID)[i] == APV4index && (apvID)[i] == APV4index && (strip)[i]%128 >= 64){
	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV4E4PulseHeight[*evtID]->SetPoint(APV4E4Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV4E4PulseHeight[*evtID]->SetPoint(APV4E4Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV4E4PulseHeight[*evtID]->SetPoint(APV4E4Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV4E4PulseHeight[*evtID]->SetPoint(APV4E4Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV4E4PulseHeight[*evtID]->SetPoint(APV4E4Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV4E4PulseHeight[*evtID]->SetPoint(APV4E4Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
  	    }	    
	    
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);	    

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	    
	    if(maxcharge >= striptol[i]){			
	      LAGDMaxCharges["APV4E4"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV4E4"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV4E4"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV4E4Pedstds.push_back(striptol[i]);	      
	  }
	  
	  //APV 3 in eta 1
	  if((detID)[i] == APV3index && (apvID)[i] == APV3index && (strip)[i]%128 < 64){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV3E1PulseHeight[*evtID]->SetPoint(APV3E1Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV3E1PulseHeight[*evtID]->SetPoint(APV3E1Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV3E1PulseHeight[*evtID]->SetPoint(APV3E1Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV3E1PulseHeight[*evtID]->SetPoint(APV3E1Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV3E1PulseHeight[*evtID]->SetPoint(APV3E1Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV3E1PulseHeight[*evtID]->SetPoint(APV3E1Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	      
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	    
	    if(maxcharge >= striptol[i]){
	      LAGDMaxCharges["APV3E1"].push_back(maxcharge-striptol[i]);
	      LAGDMaxStrips["APV3E1"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV3E1"].push_back(timebin);
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV3E1Pedstds.push_back(striptol[i]);	      
	    
	  }
	  
	  //APV 3 in eta 2
	  if((detID)[i] == APV3index && (apvID)[i] == APV3index && (strip)[i]%128 >= 64){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV3E2PulseHeight[*evtID]->SetPoint(APV3E2Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV3E2PulseHeight[*evtID]->SetPoint(APV3E2Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV3E2PulseHeight[*evtID]->SetPoint(APV3E2Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV3E2PulseHeight[*evtID]->SetPoint(APV3E2Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV3E2PulseHeight[*evtID]->SetPoint(APV3E2Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV3E2PulseHeight[*evtID]->SetPoint(APV3E2Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	    
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);	    

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));

	    if(maxcharge >= striptol[i]){
	      LAGDMaxCharges["APV3E2"].push_back(maxcharge-striptol[i]);
	      LAGDMaxStrips["APV3E2"].push_back((strip)[i]%64);
	      LAGDTimeBin["APV3E2"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV3E2Pedstds.push_back(striptol[i]);	      
	  }	  

	  if((detID)[i] == APV6index && (apvID)[i] == APV6index && (strip)[i]%128 < 64){
	    //cout << "Atleast going hERE" << endl;
	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV6E3PulseHeight[*evtID]->SetPoint(APV6E3Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV6E3PulseHeight[*evtID]->SetPoint(APV6E3Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV6E3PulseHeight[*evtID]->SetPoint(APV6E3Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV6E3PulseHeight[*evtID]->SetPoint(APV6E3Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV6E3PulseHeight[*evtID]->SetPoint(APV6E3Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV6E3PulseHeight[*evtID]->SetPoint(APV6E3Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	    
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	    
	    if(maxcharge >= striptol[i]){
	      cout << "what about here? " << endl;
	      LAGDMaxCharges["APV6E3"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV6E3"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV6E3"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV6E3Pedstds.push_back(striptol[i]);	      
	  }
	  
	  if((detID)[i] == APV6index && (apvID)[i] == APV6index && (strip)[i]%128 >= 64){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV6E4PulseHeight[*evtID]->SetPoint(APV6E4Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV6E4PulseHeight[*evtID]->SetPoint(APV6E4Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV6E4PulseHeight[*evtID]->SetPoint(APV6E4Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV6E4PulseHeight[*evtID]->SetPoint(APV6E4Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV6E4PulseHeight[*evtID]->SetPoint(APV6E4Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV6E4PulseHeight[*evtID]->SetPoint(APV6E4Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	    
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	    
	    if(maxcharge >= striptol[i]){			
	      LAGDMaxCharges["APV6E4"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV6E4"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV6E4"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV6E4Pedstds.push_back(striptol[i]);	      
	  }

	  
	  //Again, dont need APV 2 rn
  
	  if((detID)[i] == APV2index && (apvID)[i] == APV2index){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV2PulseHeight[*evtID]->SetPoint(APV2Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV2PulseHeight[*evtID]->SetPoint(APV2Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV2PulseHeight[*evtID]->SetPoint(APV2Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV2PulseHeight[*evtID]->SetPoint(APV2Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV2PulseHeight[*evtID]->SetPoint(APV2Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV2PulseHeight[*evtID]->SetPoint(APV2Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	    	   
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);
	    
	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	    
	    if(maxcharge >= striptol[i]){			
	      LAGDMaxCharges["APV2"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV2"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV2"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV2Pedstds.push_back(striptol[i]);	      	  
	    
	  }
	  
	  
	  //APV 7 eta 1
	  if((detID)[i] == APV7index && (apvID)[i] == APV7index && (strip)[i]%128 < 64){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      APV7E1PulseHeight[*evtID]->SetPoint(APV7E1Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      APV7E1PulseHeight[*evtID]->SetPoint(APV7E1Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV7E1PulseHeight[*evtID]->SetPoint(APV7E1Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV7E1PulseHeight[*evtID]->SetPoint(APV7E1Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV7E1PulseHeight[*evtID]->SetPoint(APV7E1Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV7E1PulseHeight[*evtID]->SetPoint(APV7E1Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	    	   
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);	    

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	    
	    if(maxcharge >= striptol[i]){			
	      LAGDMaxCharges["APV7E1"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV7E1"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV7E1"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV7E1Pedstds.push_back(striptol[i]);	      
	  }
	  
	  //APV 7 eta 2
	  if((detID)[i] == APV7index && (apvID)[i] == APV7index && (strip)[i]%128 >= 64){

	    if(display_mode != 2 && display_mode != 5 && display_mode != 0){
	      //cout << "Pit variable: " << APV7E2Pit[*evtID] << endl;
	      APV7E2PulseHeight[*evtID]->SetPoint(APV7E2Pit[*evtID]++, (strip)[i]%128, 0, (adc0)[i]);
	      //cout << "Pit variable for first entry: " << APV7E2Pit[*evtID] << endl;
	      
	      APV7E2PulseHeight[*evtID]->SetPoint(APV7E2Pit[*evtID]++, (strip)[i]%128, 1, (adc1)[i]);
	      APV7E2PulseHeight[*evtID]->SetPoint(APV7E2Pit[*evtID]++, (strip)[i]%128, 2, (adc2)[i]);
	      APV7E2PulseHeight[*evtID]->SetPoint(APV7E2Pit[*evtID]++, (strip)[i]%128, 3, (adc3)[i]);
	      APV7E2PulseHeight[*evtID]->SetPoint(APV7E2Pit[*evtID]++, (strip)[i]%128, 4, (adc4)[i]);
	      APV7E2PulseHeight[*evtID]->SetPoint(APV7E2Pit[*evtID]++, (strip)[i]%128, 5, (adc5)[i]);
	    }
	    
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    auto timebin = distance(stripcharges.begin(), max_element(stripcharges.begin(), stripcharges.end()));
	    
	    if(maxcharge >= striptol[i]){			
	      LAGDMaxCharges["APV7E2"].push_back(maxcharge-striptol[i]);	      
	      LAGDMaxStrips["APV7E2"].push_back((strip)[i]%128);
	      LAGDTimeBin["APV7E2"].push_back(timebin);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV7E2Pedstds.push_back(striptol[i]);	      
	  }	  
	}
	
	//end LAGD APVs
	totalchannel[*evtID]++;	
	//cout << "channel number" << totalchannel[*evtID] -1 << ", " << (detID)[i] << endl;		
	
	if(display_mode != 3 && display_mode != 1 && display_mode != 0 && display_mode != 5){
	//cout << display_mode << " and " << *evtID << endl;
	 
	  
	  APV3E1PulseHeight[*evtID]->Clear();
	  APV3E2PulseHeight[*evtID]->Clear();
	  APV4E3PulseHeight[*evtID]->Clear();
	  APV4E4PulseHeight[*evtID]->Clear();
	  APV5E3PulseHeight[*evtID]->Clear();
	  APV5E4PulseHeight[*evtID]->Clear();
	  APV6E3PulseHeight[*evtID]->Clear();
	  APV6E4PulseHeight[*evtID]->Clear();
	  APV7E1PulseHeight[*evtID]->Clear();
	  APV7E2PulseHeight[*evtID]->Clear();
	  
	  //cout << "Going here?" << endl;
	  Tracker1XPulseHeight[*evtID]->Clear();
	  Tracker1YPulseHeight[*evtID]->Clear();
	  Tracker2XPulseHeight[*evtID]->Clear();
	  Tracker2YPulseHeight[*evtID]->Clear();
	  Tracker3XPulseHeight[*evtID]->Clear();
	  Tracker3YPulseHeight[*evtID]->Clear();
	  Tracker4XPulseHeight[*evtID]->Clear();
	  Tracker4YPulseHeight[*evtID]->Clear();
	  //cout << "Past here?" << endl;
	  
	  delete PulseHeight[*evtID];
	  PulseHeight[*evtID] = nullptr;	  
	  //cout << "Past here???" << endl;
	  
	  
	}
	
	//	toleranceit++;
      }/////////////////////end of for loop over the channels in one event
      /*
      auto avg=0.0;
      for(auto h = maxtimebin.begin(); h < maxtimebin.end(); h++){
	cout << *h << endl;
	avg+=*h;
      }
      avg = avg/maxtimebin.size();
      cout << "Average Hit Timebin for this event: " << avg << endl;
      */
      
      //now that hits have beem identified in the different APVs, we need to create a quality flag. This currently only allows events with one hit in them, and restricts the stip multiplicity of the hit to between 2 and 5
      //triggers
      
      cout << "Done Loop over all channels" << endl;
      GoodEvent[totalEventiterator] = true;
      //cout << "yes" << endl;
      //only allowing events with 1 hit as of right now, ie, only one sub event->
      bool trackerinvolvement=true;
      vector<float> NumSubEventsAPV;

      vector<float> NumSubEvents1X;
      vector<float> NumSubEvents1Y;
      vector<float> NumSubEvents2X;
      vector<float> NumSubEvents2Y;
      vector<float> NumSubEvents3X;
      vector<float> NumSubEvents3Y;
      vector<float> NumSubEvents4X;
      vector<float> NumSubEvents4Y;


      vector<float> ReducedTracker1xmaxcharges;
      vector<float> ReducedTracker1xmaxstrips;
      vector<float> ReducedTracker1ymaxcharges;
      vector<float> ReducedTracker1ymaxstrips;
      vector<float> ReducedTracker2xmaxcharges;
      vector<float> ReducedTracker2xmaxstrips;
      vector<float> ReducedTracker2ymaxcharges;
      vector<float> ReducedTracker2ymaxstrips;
      vector<float> ReducedTracker3xmaxcharges;
      vector<float> ReducedTracker3xmaxstrips;
      vector<float> ReducedTracker3ymaxcharges;
      vector<float> ReducedTracker3ymaxstrips;
      vector<float> ReducedTracker4xmaxcharges;
      vector<float> ReducedTracker4xmaxstrips;
      vector<float> ReducedTracker4ymaxcharges;
      vector<float> ReducedTracker4ymaxstrips;
      vector<float> ReducedLAGDmaxcharges;
      vector<float> ReducedLAGDmaxstrips;
      
      vector<float> NewNumSubEvents1X;
      vector<float> NewNumSubEvents1Y;
      vector<float> NewNumSubEvents2X;
      vector<float> NewNumSubEvents2Y;
      vector<float> NewNumSubEvents3X;
      vector<float> NewNumSubEvents3Y;
      vector<float> NewNumSubEvents4X;
      vector<float> NewNumSubEvents4Y;
      vector<float> NewNumSubEventsLAGD;
      
      if(display_mode == 0){
	if(trackerinvolvement){
	  if(maxstrips.size() == 0){
	    cout << "No event seen?" << endl;	 
	    ClusterNumberHist->Fill(0);
	    NumNoLAGDEvent.at(fileit)++;
	    //LAGDEventCutflowinfo[1]++;//<=5
	    //LAGDEventCutflowinfo[2]++;//<=4
	    //LAGDEventCutflowinfo[3]++;//<=3
	    //LAGDEventCutflowinfo[4]++;//<=2
	    //LAGDEventCutflowinfo[5]++;//==1	    
	  }
	  else{	    
	    NumSubEventsAPV = StripSep(maxstrips, maxcharges);
	    ClusterNumberHist->Fill(NumSubEventsAPV.size());
	    LAGDEventCutflowinfo[0]++;//!=0
	    if(NumSubEventsAPV.size() <= 5){	      
	      for(int y = 1; y <= 6-NumSubEventsAPV.size(); y++){
		LAGDEventCutflowinfo[y]++;
	      }
	    }
	  
	  
	    // if(NumSubEventsAPV[0] >= 1){
	    //   LAGDEventCutflowinfo[6]++;
	    // }
	    if(NumSubEventsAPV.size() == 1){
	      //[6] == 1 already done in previous if statement
	      //[7] <= 5
	      //[8] <= 4
	      //[9] <= 3
	      //[10] == 2
	      LAGDEventCutflowinfo[6]++;
	      if(NumSubEventsAPV[0] <= 5){
		for(int y = 7; y <= 12 - NumSubEventsAPV[0]; y++){
		  LAGDEventCutflowinfo[y]++;
		}
	      }
	    }
	  }
	  
	  if((Tracker1xmaxstrips.empty() || Tracker1ymaxstrips.empty() || Tracker2xmaxstrips.empty() || Tracker2ymaxstrips.empty() || Tracker3xmaxstrips.empty() || Tracker3ymaxstrips.empty() || Tracker4xmaxstrips.empty() || Tracker4ymaxstrips.empty()) && (NumSubEventsAPV.size() != 0 && NumSubEventsAPV.size() <= 5)){
	    EventsLostToTrackers->Fill(1);
	    numEventsWith0hitclusterintrackers++;
	  }


	  //TrackerEventCutflowinfo here

	  if(EventCutflow){


	    int exhits = 0;
	    if(Tracker1xmaxstrips.size() == 0)
	      exhits++;
	    if(Tracker2xmaxstrips.size() == 0)
	      exhits++;
	    if(Tracker3xmaxstrips.size() == 0)
	      exhits++;
	    if(Tracker4xmaxstrips.size() == 0)
	      exhits++;
	    
	    xHits0->Fill(exhits);

	    exhits = 0;
	    if(Tracker1ymaxstrips.size() == 0)
	      exhits++;
	    if(Tracker2ymaxstrips.size() == 0)
	      exhits++;
	    if(Tracker3ymaxstrips.size() == 0)
	      exhits++;
	    if(Tracker4ymaxstrips.size() == 0)
	      exhits++;
	    
	    yHits0->Fill(exhits);

	    
	    

	    
	    if(Tracker1xmaxstrips.size() != 0)
	      NumSubEvents1X = StripSep(Tracker1xmaxstrips, Tracker1xmaxcharges);
	    if(Tracker1ymaxstrips.size() != 0)
	      NumSubEvents1Y = StripSep(Tracker1ymaxstrips, Tracker1ymaxcharges);
	    if(Tracker2xmaxstrips.size() != 0)
	      NumSubEvents2X = StripSep(Tracker2xmaxstrips, Tracker2xmaxcharges);
	    if(Tracker2ymaxstrips.size() != 0)
	      NumSubEvents2Y = StripSep(Tracker2ymaxstrips, Tracker2ymaxcharges);
	    if(Tracker3xmaxstrips.size() != 0)
	      NumSubEvents3X = StripSep(Tracker3xmaxstrips, Tracker3xmaxcharges);
	    if(Tracker3ymaxstrips.size() != 0)
	      NumSubEvents3Y = StripSep(Tracker3ymaxstrips, Tracker3ymaxcharges);
	    if(Tracker4xmaxstrips.size() != 0)
	      NumSubEvents4X = StripSep(Tracker4xmaxstrips, Tracker4xmaxcharges);
	    if(Tracker4ymaxstrips.size() != 0)
	      NumSubEvents4Y = StripSep(Tracker4ymaxstrips, Tracker4ymaxcharges);
	    

	   

	    //int 1xhits = 0;

	    for(auto e = 1; e <=5; e++){
	      exhits = 0;
	      if(NumSubEvents1X.size() == e)
		exhits++;
	      if(NumSubEvents2X.size() == e)
		exhits++;
	      if(NumSubEvents3X.size() == e)
		exhits++;
	      if(NumSubEvents4X.size() == e)
		exhits++;

	      if(e == 1)
		xHits1->Fill(exhits);
	      if(e == 2)
		xHits2->Fill(exhits);
	      if(e == 3)
		xHits3->Fill(exhits);
	      if(e == 4)
		xHits4->Fill(exhits);
	      if(e == 5)
		xHits5->Fill(exhits);
	    }

	    for(auto e = 1; e <=5; e++){
	      exhits = 0;
	      if(NumSubEvents1Y.size() == e)
		exhits++;
	      if(NumSubEvents2Y.size() == e)
		exhits++;
	      if(NumSubEvents3Y.size() == e)
		exhits++;
	      if(NumSubEvents4Y.size() == e)
		exhits++;

	      if(e == 1)
		yHits1->Fill(exhits);
	      if(e == 2)
		yHits2->Fill(exhits);
	      if(e == 3)
		yHits3->Fill(exhits);
	      if(e == 4)
		yHits4->Fill(exhits);
	      if(e == 5)
		yHits5->Fill(exhits);
	      
	    }
	    
	    

	    
	    if(NumSubEvents1X.size() == 1 || NumSubEvents1X.size() == 2 || NumSubEvents1X.size() == 3){	     
	      TrackerEventCutflowinfo.at(0)++;
	      if(NumSubEvents1Y.size() == 1 || NumSubEvents1Y.size() == 2 || NumSubEvents1Y.size() == 3){
		TrackerEventCutflowinfo.at(1)++;
		if(NumSubEvents2X.size() == 1 || NumSubEvents2X.size() == 2 || NumSubEvents2X.size() == 3){
		  TrackerEventCutflowinfo.at(2)++;
		  if(NumSubEvents2Y.size() == 1 || NumSubEvents2Y.size() == 2 || NumSubEvents2Y.size() == 3){
		    TrackerEventCutflowinfo.at(3)++;
		    if(NumSubEvents3X.size() == 1 || NumSubEvents3X.size() == 2 || NumSubEvents3X.size() == 3){
		      TrackerEventCutflowinfo.at(4)++;
		      if(NumSubEvents3Y.size() == 1 || NumSubEvents3Y.size() == 2 || NumSubEvents3Y.size() == 3){
			TrackerEventCutflowinfo.at(5)++;
			if(NumSubEvents4X.size() == 1 || NumSubEvents4X.size() == 2 || NumSubEvents4X.size() == 3){
			  TrackerEventCutflowinfo.at(6)++;
			  if(NumSubEvents4Y.size() == 1 || NumSubEvents4Y.size() == 2 || NumSubEvents4Y.size() == 3){
			    TrackerEventCutflowinfo.at(7)++;
			    if(NumSubEventsAPV.size() == 1){
			      TrackerEventCutflowinfo.at(8)++;
			      bool y = false;
			      for(auto u = 0; u < NumSubEventsAPV.size(); u++){
				if(NumSubEventsAPV.at(u) == 2 || NumSubEventsAPV.at(u) == 3 || NumSubEventsAPV.at(u) == 4 || NumSubEventsAPV.at(u) == 5){
				  y = true;
				}
			      }
			      if(y){
				TrackerEventCutflowinfo.at(9)++;
			      }			      
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }


	    
	    if(NumSubEvents1X.size() == 1 || NumSubEvents1X.size() == 2){
	      TrackerEventCutflowinfo2.at(0)++;
	      if(NumSubEvents1Y.size() == 1 || NumSubEvents1Y.size() == 2){
		TrackerEventCutflowinfo2.at(1)++;
		if(NumSubEvents2X.size() == 1 || NumSubEvents2X.size() == 2){
		  TrackerEventCutflowinfo2.at(2)++;
		  if(NumSubEvents2Y.size() == 1 || NumSubEvents2Y.size() == 2){
		    TrackerEventCutflowinfo2.at(3)++;
		    if(NumSubEvents3X.size() == 1 || NumSubEvents3X.size() == 2){
		      TrackerEventCutflowinfo2.at(4)++;
		      if(NumSubEvents3Y.size() == 1 || NumSubEvents3Y.size() == 2){
			TrackerEventCutflowinfo2.at(5)++;
			if(NumSubEvents4X.size() == 1 || NumSubEvents4X.size() == 2){
			  TrackerEventCutflowinfo2.at(6)++;
			  if(NumSubEvents4Y.size() == 1 || NumSubEvents4Y.size() == 2){
			    TrackerEventCutflowinfo2.at(7)++;
			    if(NumSubEventsAPV.size() == 1){
			      TrackerEventCutflowinfo2.at(8)++;
			      bool y = false;
			      for(auto u = 0; u < NumSubEventsAPV.size(); u++){
				if(NumSubEventsAPV.at(u) == 2 || NumSubEventsAPV.at(u) == 3 || NumSubEventsAPV.at(u) == 4 || NumSubEventsAPV.at(u) == 5){
				  y = true;
				}
			      }
			      if(y){
				TrackerEventCutflowinfo2.at(9)++;
			      }		
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	    
	    if(NumSubEvents1X.size() == 1){
	      TrackerEventCutflowinfo3.at(0)++;
	      if(NumSubEvents1Y.size() == 1){
		TrackerEventCutflowinfo3.at(1)++;
		if(NumSubEvents2X.size() == 1){
		  TrackerEventCutflowinfo3.at(2)++;
		  if(NumSubEvents2Y.size() == 1){
		    TrackerEventCutflowinfo3.at(3)++;
		    if(NumSubEvents3X.size() == 1){
		      TrackerEventCutflowinfo3.at(4)++;
		      if(NumSubEvents3Y.size() == 1){
			TrackerEventCutflowinfo3.at(5)++;
			if(NumSubEvents4X.size() == 1){
			  TrackerEventCutflowinfo3.at(6)++;
			  if(NumSubEvents4Y.size() == 1){
			    TrackerEventCutflowinfo3.at(7)++;
			    if(NumSubEventsAPV.size() == 1){
			      TrackerEventCutflowinfo3.at(8)++;
			      bool y = false;
			      for(auto u = 0; u < NumSubEventsAPV.size(); u++){
				if(NumSubEventsAPV.at(u) == 2 || NumSubEventsAPV.at(u) == 3 || NumSubEventsAPV.at(u) == 4 || NumSubEventsAPV.at(u) == 5){
				  y = true;
				}
			      }
			      if(y){
				TrackerEventCutflowinfo3.at(9)++;
			      }		
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	  
	  // if(Efficiency){
	  //   if(Tracker1xmaxstrips.empty() || Tracker1ymaxstrips.empty() || Tracker2xmaxstrips.empty() || Tracker2ymaxstrips.empty() || Tracker3xmaxstrips.empty() || Tracker3ymaxstrips.empty() || Tracker4xmaxstrips.empty() || Tracker4ymaxstrips.empty()){}//if anything is empty, skip this
	  //   else{
	      
	  //     auto NumSubEvents1X = StripSep(Tracker1xmaxstrips, Tracker1xmaxcharges);
	  //     auto NumSubEvents1Y = StripSep(Tracker1ymaxstrips, Tracker1ymaxcharges);
	   
	  //     auto NumSubEvents2X = StripSep(Tracker2xmaxstrips, Tracker2xmaxcharges);
	  //     auto NumSubEvents2Y = StripSep(Tracker2ymaxstrips, Tracker2ymaxcharges);
	   	    
	  //     auto NumSubEvents3X = StripSep(Tracker3xmaxstrips, Tracker3xmaxcharges);
	  //     auto NumSubEvents3Y = StripSep(Tracker3ymaxstrips, Tracker3ymaxcharges);
	      
	  //     auto NumSubEvents4X = StripSep(Tracker4xmaxstrips, Tracker4xmaxcharges);
	  //     auto NumSubEvents4Y = StripSep(Tracker4ymaxstrips, Tracker4ymaxcharges);

	  //     Trackerhits++;
	  //     if(maxstrips.empty()){cout << "Yes" << endl;}
	  //     else{
	  // 	cout << "LAGD hit size: " << maxstrips.size() << endl;
	  // 	NumSubEventsAPV = StripSep(maxstrips, maxcharges);

	  // 	int LAGDhit = EffCounter(Tracker1xmaxstrips, Tracker1xmaxcharges, Tracker1ymaxstrips, Tracker1ymaxcharges,Tracker2xmaxstrips, Tracker2xmaxcharges,Tracker2ymaxstrips, Tracker2ymaxcharges, Tracker3xmaxstrips, Tracker3xmaxcharges, Tracker3ymaxstrips, Tracker3ymaxcharges,Tracker4xmaxstrips, Tracker4xmaxcharges,Tracker4ymaxstrips, Tracker4ymaxcharges, LAGDMaxStrips[APVname[apvnum]], LAGDMaxCharges[APVname[apvnum]], LAGDXoffsets[evtype], LAGDYoffsets[evtype], HVres[evtype]);
		
	  // 	return;

	  //     }
	  //   }
	      

	  // }
	  
	  
	 
	  cout << maxstrips.empty() << endl;
	  cout << LAGDMaxStrips[APVname[apvnum]].empty() << endl;
	  
	  if(Tracker1xmaxstrips.empty() || Tracker1ymaxstrips.empty() || Tracker2xmaxstrips.empty() || Tracker2ymaxstrips.empty() || Tracker3xmaxstrips.empty() || Tracker3ymaxstrips.empty() || Tracker4xmaxstrips.empty() || Tracker4ymaxstrips.empty() || maxstrips.empty()){	    
	  GoodEvent[totalEventiterator] = false;
	  cout << "Lost Event" << endl;	  
	  }
	  else{
	    auto NumSubEvents1X = StripSep(Tracker1xmaxstrips, Tracker1xmaxcharges);
	    auto NumSubEvents1Y = StripSep(Tracker1ymaxstrips, Tracker1ymaxcharges);
	   
	    auto NumSubEvents2X = StripSep(Tracker2xmaxstrips, Tracker2xmaxcharges);
	    auto NumSubEvents2Y = StripSep(Tracker2ymaxstrips, Tracker2ymaxcharges);
	   	    
	    auto NumSubEvents3X = StripSep(Tracker3xmaxstrips, Tracker3xmaxcharges);
	    auto NumSubEvents3Y = StripSep(Tracker3ymaxstrips, Tracker3ymaxcharges);
	      
	    auto NumSubEvents4X = StripSep(Tracker4xmaxstrips, Tracker4xmaxcharges);
	    auto NumSubEvents4Y = StripSep(Tracker4ymaxstrips, Tracker4ymaxcharges);
	   
	    auto NumSubEventsLAGD = StripSep(maxstrips, maxcharges);

	    

	    // if((NumSubEvents1X.size() > 1 || NumSubEvents1Y.size() > 1  || NumSubEvents2X.size() > 1 || NumSubEvents2Y.size() > 1  ||NumSubEvents3X.size() > 1 || NumSubEvents3Y.size() > 1  || NumSubEvents4X.size() > 1 || NumSubEvents4Y.size() > 1) && (NumSubEventsLAGD.size() == 1)){
	      
	      
	    // }
	      
	    int Trackerclustnum = 5;
	    if(NumSubEvents1X.size() > Trackerclustnum || NumSubEvents1Y.size() > Trackerclustnum  || NumSubEvents2X.size() > Trackerclustnum || NumSubEvents2Y.size() > Trackerclustnum  ||NumSubEvents3X.size() > Trackerclustnum || NumSubEvents3Y.size() > Trackerclustnum  || NumSubEvents4X.size() > Trackerclustnum || NumSubEvents4Y.size() > Trackerclustnum || NumSubEventsLAGD.size() > Trackerclustnum){
	      GoodEvent[totalEventiterator] = false;
	      EventsLostToTrackers->Fill(1);
	      numEventsWith2ormorehitclusterintrackers++;
	      //cout << "JHerererer?" << endl;	    
	    }

	    
	    if(GoodEvent[totalEventiterator]){
	      auto Newtrackerhits = MultiClusterReducer(Tracker1xmaxstrips, Tracker1xmaxcharges, Tracker1ymaxstrips, Tracker1ymaxcharges,Tracker2xmaxstrips, Tracker2xmaxcharges,Tracker2ymaxstrips, Tracker2ymaxcharges, Tracker3xmaxstrips, Tracker3xmaxcharges, Tracker3ymaxstrips, Tracker3ymaxcharges,Tracker4xmaxstrips, Tracker4xmaxcharges,Tracker4ymaxstrips, Tracker4ymaxcharges, maxstrips, maxcharges, LAGDXoffsets[location], LAGDYoffsets[location]);
	      cout << "past this?" << endl;
	      if(Newtrackerhits.empty()){// || *evtID == 514){
		cout << "For now, bad!" << endl;
		lostcounter++;
		continue;
		//return;
	      }
	      else{
		//break;
		ReducedTracker1xmaxstrips = Newtrackerhits[0];
		ReducedTracker1xmaxcharges = Newtrackerhits[1];
		ReducedTracker2xmaxstrips = Newtrackerhits[2];
		ReducedTracker2xmaxcharges = Newtrackerhits[3];
		ReducedTracker3xmaxstrips = Newtrackerhits[4];
		ReducedTracker3xmaxcharges = Newtrackerhits[5];
		ReducedTracker4xmaxstrips = Newtrackerhits[6];
		ReducedTracker4xmaxcharges = Newtrackerhits[7];
	    
		ReducedTracker1ymaxstrips = Newtrackerhits[8];
		ReducedTracker1ymaxcharges = Newtrackerhits[9];
		ReducedTracker2ymaxstrips = Newtrackerhits[10];
		ReducedTracker2ymaxcharges = Newtrackerhits[11];
		ReducedTracker3ymaxstrips = Newtrackerhits[12];
		ReducedTracker3ymaxcharges = Newtrackerhits[13];
		ReducedTracker4ymaxstrips = Newtrackerhits[14];
		ReducedTracker4ymaxcharges = Newtrackerhits[15];

		ReducedLAGDmaxstrips = Newtrackerhits[16];
		ReducedLAGDmaxcharges = Newtrackerhits[17];
	      }
	    
	      cout << "In main code: " << endl;
	      for(int y = 0; y < Newtrackerhits.size(); y++){
		for(int j = 0; j < Newtrackerhits[y].size(); j++){
		  cout << Newtrackerhits[y][j] << ", ";
		}
		cout << endl;
	      }
	      

	      //break;
	      NewNumSubEvents1X = StripSep(ReducedTracker1xmaxstrips, ReducedTracker1xmaxcharges);
	      NewNumSubEvents1Y = StripSep(ReducedTracker1ymaxstrips, ReducedTracker1ymaxcharges);
	      NewNumSubEvents2X = StripSep(ReducedTracker2xmaxstrips, ReducedTracker2xmaxcharges);
	      NewNumSubEvents2Y = StripSep(ReducedTracker2ymaxstrips, ReducedTracker2ymaxcharges);
	      NewNumSubEvents3X = StripSep(ReducedTracker3xmaxstrips, ReducedTracker3xmaxcharges);
	      NewNumSubEvents3Y = StripSep(ReducedTracker3ymaxstrips, ReducedTracker3ymaxcharges);
	      NewNumSubEvents4X = StripSep(ReducedTracker4xmaxstrips, ReducedTracker4xmaxcharges);
	      NewNumSubEvents4Y = StripSep(ReducedTracker4ymaxstrips, ReducedTracker4ymaxcharges);
	      NewNumSubEventsLAGD = StripSep(ReducedLAGDmaxstrips, ReducedLAGDmaxcharges);
	      
	      
	      

	      int Trackmax_StripHits = 10;
	      int Trackmin_StripHits = 1;//strip multiplicities will be between min and below max (not including the values themselves)
	    
	      int LAGDmax_StripHits = 5;
	      int LAGDmin_StripHits = 0;
	      bool eventlostduetoSM = false;
	      if(*max_element(NewNumSubEvents1X.begin(), NewNumSubEvents1X.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents1X.begin(), NewNumSubEvents1X.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;eventlostduetoSM = true;SMloss->Fill(1);}	  
	      if(*max_element(NewNumSubEvents1Y.begin(), NewNumSubEvents1Y.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents1Y.begin(), NewNumSubEvents1Y.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;eventlostduetoSM = true;SMloss->Fill(2);}
	      if(*max_element(NewNumSubEvents2X.begin(), NewNumSubEvents2X.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents2X.begin(), NewNumSubEvents2X.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;eventlostduetoSM = true;SMloss->Fill(3);}
	      if(*max_element(NewNumSubEvents2Y.begin(), NewNumSubEvents2Y.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents2Y.begin(), NewNumSubEvents2Y.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;eventlostduetoSM = true;SMloss->Fill(4);}
	      if(*max_element(NewNumSubEvents3X.begin(), NewNumSubEvents3X.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents3X.begin(), NewNumSubEvents3X.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;eventlostduetoSM = true;SMloss->Fill(5);}
	      if(*max_element(NewNumSubEvents3Y.begin(), NewNumSubEvents3Y.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents3Y.begin(), NewNumSubEvents3Y.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;eventlostduetoSM = true;SMloss->Fill(6);}
	      if(*max_element(NewNumSubEvents4X.begin(), NewNumSubEvents4X.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents4X.begin(), NewNumSubEvents4X.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;eventlostduetoSM = true;SMloss->Fill(7);}
	      if(*max_element(NewNumSubEvents4Y.begin(), NewNumSubEvents4Y.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents4Y.begin(), NewNumSubEvents4Y.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;eventlostduetoSM = true;SMloss->Fill(8);}

	      if(Efficiency && GoodEvent[totalEventiterator])
		Trackerhits++;
	      
	      
	      if(*max_element(NewNumSubEventsLAGD.begin(), NewNumSubEventsLAGD.end()) >= LAGDmax_StripHits || *min_element(NewNumSubEventsLAGD.begin(), NewNumSubEventsLAGD.end()) <= LAGDmin_StripHits){GoodEvent[totalEventiterator] = false;eventlostduetoSM = true;SMloss->Fill(9);}
	      if(eventlostduetoSM == true){
		EventsLostToTrackers->Fill(2);
	      
	      }

	      if(Efficiency && GoodEvent[totalEventiterator]){
		cout << ReducedLAGDmaxstrips.size() << endl;
		int LAGDhit = EffCounter(ReducedTracker1xmaxstrips, ReducedTracker1xmaxcharges, ReducedTracker1ymaxstrips, ReducedTracker1ymaxcharges,ReducedTracker2xmaxstrips, ReducedTracker2xmaxcharges,ReducedTracker2ymaxstrips, ReducedTracker2ymaxcharges, ReducedTracker3xmaxstrips, ReducedTracker3xmaxcharges, ReducedTracker3ymaxstrips, ReducedTracker3ymaxcharges,ReducedTracker4xmaxstrips, ReducedTracker4xmaxcharges,ReducedTracker4ymaxstrips, ReducedTracker4ymaxcharges, ReducedLAGDmaxstrips, ReducedLAGDmaxcharges, LAGDXoffsets[location], LAGDYoffsets[location], HVres[location]);

		cout << "Hit or no?: " << LAGDhit << endl;
		if(LAGDhit == 0)
		  NumNoLAGDEventSeen.at(fileit)++;

		
		LAGDhitsnum += LAGDhit;
		cout << LAGDhitsnum << endl;

		if(LAGDhitsnum >  Trackerhits){return;}
		// if(LAGDhit == 1)
		//   return;
		
	      }
	    }
	  }
	}
	
	if(trackerinvolvement == false){
	  if(maxstrips.empty())
	    GoodEvent[totalEventiterator] = false;
	  else{
	    auto NumSubEvents = StripSep(maxstrips, maxcharges);	
	    if(NumSubEvents.size() > 1)
	      GoodEvent[totalEventiterator] = false;
	    
	    int max_StripHits = 4;
	    int min_StripHits = 2;	    
	    if(*max_element(NumSubEvents.begin(), NumSubEvents.end()) > max_StripHits || *min_element(NumSubEvents.begin(), NumSubEvents.end()) < min_StripHits){GoodEvent[totalEventiterator] = false;}
	  }	
	}
      }
      //float LAGDhitsnum=0;
      //float Trackerhits=0;
      //Only allow events with one peak in the hit including APV3 for now
      //note that depending on where you expect data to be, you need to change the APV its analyzing
      
      if(display_mode == 1 || display_mode == 2 || display_mode == 3 || display_mode == 5){
	cout << "Enters here??" << endl;


	//cout << "Is event good?: " << GoodEvent[totalEventiterator] << endl;


	// if(ReducedTracker1xmaxstrips.empty() || ReducedTracker1ymaxstrips.empty() || ReducedTracker2xmaxstrips.empty() || ReducedTracker2ymaxstrips.empty() || ReducedTracker3xmaxstrips.empty() || ReducedTracker3ymaxstrips.empty() || ReducedTracker4xmaxstrips.empty() || ReducedTracker4ymaxstrips.empty() || LAGDMaxCharges[APVname[apvnum]].empty()){
	//   GoodEvent[totalEventiterator] = false;
	//   cout << "Lost Event" << endl;	  
	// }
	if(Tracker1xmaxstrips.empty() || Tracker1ymaxstrips.empty() || Tracker2xmaxstrips.empty() || Tracker2ymaxstrips.empty() || Tracker3xmaxstrips.empty() || Tracker3ymaxstrips.empty() || Tracker4xmaxstrips.empty() || Tracker4ymaxstrips.empty() || LAGDMaxCharges[APVname[apvnum]].empty()){
	  GoodEvent[totalEventiterator] = false;
	  cout << "Lost Event" << endl;	  
	}
	else{
	  //cout << "Is event good?: " << GoodEvent[totalEventiterator] << endl;
	  cout << "here?" << endl;
	  auto NumSubEvents1X = StripSep(Tracker1xmaxstrips, Tracker1xmaxcharges);
	  auto NumSubEvents1Y = StripSep(Tracker1ymaxstrips, Tracker1ymaxcharges);
	  auto NumSubEvents2X = StripSep(Tracker2xmaxstrips, Tracker2xmaxcharges);
	  auto NumSubEvents2Y = StripSep(Tracker2ymaxstrips, Tracker2ymaxcharges);
	  auto NumSubEvents3X = StripSep(Tracker3xmaxstrips, Tracker3xmaxcharges);
	  auto NumSubEvents3Y = StripSep(Tracker3ymaxstrips, Tracker3ymaxcharges);
	  auto NumSubEvents4X = StripSep(Tracker4xmaxstrips, Tracker4xmaxcharges);
	  auto NumSubEvents4Y = StripSep(Tracker4ymaxstrips, Tracker4ymaxcharges);
	  cout << APVname[apvnum] << endl;
	  auto NumSubEventsLAGD = StripSep(LAGDMaxStrips[APVname[apvnum]], LAGDMaxCharges[APVname[apvnum]]);

	 
	  //cout << "Is event good?: " << GoodEvent[totalEventiterator] << endl;

	  
	  int Trackerclustnum = 3;
	  if(NumSubEvents1X.size() > Trackerclustnum || NumSubEvents1Y.size() > Trackerclustnum  || NumSubEvents2X.size() > Trackerclustnum || NumSubEvents2Y.size() > Trackerclustnum  ||NumSubEvents3X.size() > Trackerclustnum || NumSubEvents3Y.size() > Trackerclustnum  || NumSubEvents4X.size() > Trackerclustnum || NumSubEvents4Y.size() > Trackerclustnum || NumSubEventsLAGD.size() > Trackerclustnum){
	    GoodEvent[totalEventiterator] = false;
	    //cout << "JHerererer?" << endl;	    
	  }
	  
	  //cout << "Is event good?: " << GoodEvent[totalEventiterator] << endl;


	  if(GoodEvent[totalEventiterator] == true){
	    //cout << "Evtype: " << evtype << endl;
	    auto Newtrackerhits = MultiClusterReducer(Tracker1xmaxstrips, Tracker1xmaxcharges, Tracker1ymaxstrips, Tracker1ymaxcharges,Tracker2xmaxstrips, Tracker2xmaxcharges,Tracker2ymaxstrips, Tracker2ymaxcharges, Tracker3xmaxstrips, Tracker3xmaxcharges, Tracker3ymaxstrips, Tracker3ymaxcharges,Tracker4xmaxstrips, Tracker4xmaxcharges,Tracker4ymaxstrips, Tracker4ymaxcharges, LAGDMaxStrips[APVname[apvnum]], LAGDMaxCharges[APVname[apvnum]], LAGDXoffsets[location], LAGDYoffsets[location]);
	    cout << "past this?" << endl;
	    if(Newtrackerhits.empty()){// || *evtID == 514){
	      cout << "For now, bad!" << endl;
	      lostcounter++;
	      continue;
	      //return;
	    }
	    else{
	      //break;
	      ReducedTracker1xmaxstrips = Newtrackerhits[0];
	      ReducedTracker1xmaxcharges = Newtrackerhits[1];
	      ReducedTracker2xmaxstrips = Newtrackerhits[2];
	      ReducedTracker2xmaxcharges = Newtrackerhits[3];
	      ReducedTracker3xmaxstrips = Newtrackerhits[4];
	      ReducedTracker3xmaxcharges = Newtrackerhits[5];
	      ReducedTracker4xmaxstrips = Newtrackerhits[6];
	      ReducedTracker4xmaxcharges = Newtrackerhits[7];
	    
	      ReducedTracker1ymaxstrips = Newtrackerhits[8];
	      ReducedTracker1ymaxcharges = Newtrackerhits[9];
	      ReducedTracker2ymaxstrips = Newtrackerhits[10];
	      ReducedTracker2ymaxcharges = Newtrackerhits[11];
	      ReducedTracker3ymaxstrips = Newtrackerhits[12];
	      ReducedTracker3ymaxcharges = Newtrackerhits[13];
	      ReducedTracker4ymaxstrips = Newtrackerhits[14];
	      ReducedTracker4ymaxcharges = Newtrackerhits[15];

	      ReducedLAGDmaxstrips = Newtrackerhits[16];
	      ReducedLAGDmaxcharges = Newtrackerhits[17];
	    }
	    
	    cout << "In main code: " << endl;
	    for(int y = 0; y < Newtrackerhits.size(); y++){
	      for(int j = 0; j < Newtrackerhits[y].size(); j++){
		cout << Newtrackerhits[y][j] << ", ";
	      }
	      cout << endl;
	    }
	    //break;
	    NewNumSubEvents1X = StripSep(ReducedTracker1xmaxstrips, ReducedTracker1xmaxcharges);
	    NewNumSubEvents1Y = StripSep(ReducedTracker1ymaxstrips, ReducedTracker1ymaxcharges);
	    NewNumSubEvents2X = StripSep(ReducedTracker2xmaxstrips, ReducedTracker2xmaxcharges);
	    NewNumSubEvents2Y = StripSep(ReducedTracker2ymaxstrips, ReducedTracker2ymaxcharges);
	    NewNumSubEvents3X = StripSep(ReducedTracker3xmaxstrips, ReducedTracker3xmaxcharges);
	    NewNumSubEvents3Y = StripSep(ReducedTracker3ymaxstrips, ReducedTracker3ymaxcharges);
	    NewNumSubEvents4X = StripSep(ReducedTracker4xmaxstrips, ReducedTracker4xmaxcharges);
	    NewNumSubEvents4Y = StripSep(ReducedTracker4ymaxstrips, ReducedTracker4ymaxcharges);
	    NewNumSubEventsLAGD = StripSep(ReducedLAGDmaxstrips, ReducedLAGDmaxcharges);
	    
	    //cout << GoodEvent[totalEventiterator] << endl;
	    if(GoodEvent[totalEventiterator]){
	      int Trackmax_StripHits = 10;
	      int Trackmin_StripHits = 1;//strip multiplicities will be between min and below max (not including the values themselves)
	  
	      int LAGDmax_StripHits = 5;
	      int LAGDmin_StripHits = 1;
	  
	      if(*max_element(NewNumSubEvents1X.begin(), NewNumSubEvents1X.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents1X.begin(), NewNumSubEvents1X.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;SMloss->Fill(1);
		cout << *max_element(NewNumSubEvents1X.begin(), NewNumSubEvents1X.end()) << endl;
		cout << *min_element(NewNumSubEvents1X.begin(), NewNumSubEvents1X.end()) << endl;
	    
	      }
	      ////cout << "Is event good?: " << GoodEvent[totalEventiterator] << endl;
	      for(auto y = 0; y < NewNumSubEvents1X.size(); y++)
		SM1x->Fill(NewNumSubEvents1X.at(y));
	      //cout << "after 1x: Is event good?: " << GoodEvent[totalEventiterator] << endl;


	  
	      if(*max_element(NewNumSubEvents1Y.begin(), NewNumSubEvents1Y.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents1Y.begin(), NewNumSubEvents1Y.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;SMloss->Fill(2);}
	      ////cout << "Is event good?: " << GoodEvent[totalEventiterator] << endl;
	      for(auto y = 0; y < NewNumSubEvents1Y.size(); y++)
		SM1y->Fill(NewNumSubEvents1Y.at(y));
	      //cout << "after 1y: Is event good?: " << GoodEvent[totalEventiterator] << endl;



	      if(*max_element(NewNumSubEvents2X.begin(), NewNumSubEvents2X.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents2X.begin(), NewNumSubEvents2X.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;SMloss->Fill(3);}
	      for(auto y = 0; y < NewNumSubEvents2X.size(); y++)
		SM2x->Fill(NewNumSubEvents2X.at(y));
	      //cout << "after 2x: Is event good?: " << GoodEvent[totalEventiterator] << endl;
	      if(*max_element(NewNumSubEvents2Y.begin(), NewNumSubEvents2Y.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents2Y.begin(), NewNumSubEvents2Y.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;SMloss->Fill(4);
		cout << *max_element(NewNumSubEvents2Y.begin(), NewNumSubEvents2Y.end()) << endl;
		cout << *min_element(NewNumSubEvents2Y.begin(), NewNumSubEvents2Y.end()) << endl;}
	      for(auto y = 0; y < NewNumSubEvents2Y.size(); y++)
		SM2y->Fill(NewNumSubEvents2Y.at(y));
	      //cout << "after 2y: Is event good?: " << GoodEvent[totalEventiterator] << endl;
	      if(*max_element(NewNumSubEvents3X.begin(), NewNumSubEvents3X.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents3X.begin(), NewNumSubEvents3X.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;SMloss->Fill(5);}
	      for(auto y = 0; y < NewNumSubEvents3X.size(); y++)
		SM3x->Fill(NewNumSubEvents3X.at(y));
	      //cout << "after 3x: Is event good?: " << GoodEvent[totalEventiterator] << endl;
	      //cout << "B4 3y: " << GoodEvent[totalEventiterator] << endl;
	      if(*max_element(NewNumSubEvents3Y.begin(), NewNumSubEvents3Y.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents3Y.begin(), NewNumSubEvents3Y.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;SMloss->Fill(6);}
	      for(auto y = 0; y < NewNumSubEvents3Y.size(); y++)
		SM3y->Fill(NewNumSubEvents3Y.at(y));
	      //cout << "after 3y: Is event good?: " << GoodEvent[totalEventiterator] << endl;
	      //cout << "4fter 3y: " <<  GoodEvent[totalEventiterator] << endl;
	      if(*max_element(NewNumSubEvents4X.begin(), NewNumSubEvents4X.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents4X.begin(), NewNumSubEvents4X.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;SMloss->Fill(7);}
	      for(auto y = 0; y < NewNumSubEvents4X.size(); y++)
		SM4x->Fill(NewNumSubEvents4X.at(y));
	      //cout << "after 4x: Is event good?: " << GoodEvent[totalEventiterator] << endl;
	      if(*max_element(NewNumSubEvents4Y.begin(), NewNumSubEvents4Y.end()) >= Trackmax_StripHits || *min_element(NewNumSubEvents4Y.begin(), NewNumSubEvents4Y.end()) <= Trackmin_StripHits){GoodEvent[totalEventiterator] = false;SMloss->Fill(8);}
	      for(auto y = 0; y < NewNumSubEvents4Y.size(); y++)
		SM4y->Fill(NewNumSubEvents4Y.at(y));
	      //cout << "after 4y: Is event good?: " << GoodEvent[totalEventiterator] << endl;
	      if(*max_element(NewNumSubEventsLAGD.begin(), NewNumSubEventsLAGD.end()) >= LAGDmax_StripHits || *min_element(NewNumSubEventsLAGD.begin(), NewNumSubEventsLAGD.end()) <= LAGDmin_StripHits){GoodEvent[totalEventiterator] = false;SMloss->Fill(9);}
	      for(auto y = 0; y < NewNumSubEventsLAGD.size(); y++)
		SMLAGD->Fill(NewNumSubEventsLAGD.at(y));
	      //cout << "after LAGD: Is event good?: " << GoodEvent[totalEventiterator] << endl;
	      //cout << GoodEvent[totalEventiterator] << endl;       
	      if(GoodEvent[totalEventiterator] == true){
		for(uint p = 0; p < NewNumSubEventsLAGD.size();p++){
		  SMofTheseevents.push_back(NewNumSubEventsLAGD.at(p));
	      
		}
	      }
	      cout << "After SM counter?" << endl;
	      //cout << "Is event good?: " << GoodEvent[totalEventiterator] << endl;
	    }
	  }
	}
      }
      
      /*
	cout << "Tracker1x:" << endl;
	for(auto i = Tracker1xmaxstrips.begin(); i != Tracker1xmaxstrips.end(); i++){
	cout << *i << endl;	      
	}

	
      */
      
      //cout << endl;
      
      float loc=0;
      float Tracker1xloc=0;
      float Tracker1yloc=0;
      float Tracker2xloc=0;
      float Tracker2yloc=0;
      float Tracker3xloc=0;
      float Tracker3yloc=0;
      float Tracker4xloc=0;
      float Tracker4yloc=0;
      /*
      float APV2loc=0;
      float APV3E1loc=0;
      float APV3E2loc=0;
      float APV4E3loc=0;
      float APV4E4loc=0;
      float APV5E3loc=0;
      float APV5E4loc=0;
      float APV6E3loc=0;
      float APV6E4loc=0;
      float APV7E1loc=0; 
      float APV7E2loc=0; 
      float APV8loc=0;
      float APV9loc=0;
      */
      LAGDloc[APVname[apvnum]] = 0;
      
      float num=0;
      float Tracker1xnum=0;      
      float Tracker1ynum=0;
      float Tracker2xnum=0;
      float Tracker2ynum=0;
      float Tracker3xnum=0;
      float Tracker3ynum=0;
      float Tracker4xnum=0;
      float Tracker4ynum=0;

      /*
      float APV2num=0;
      float APV3E1num=0;
      float APV3E2num=0;
      float APV4E3num=0;
      float APV4E4num=0;
      float APV5E3num=0;
      float APV5E4num=0;
      float APV6E3num=0;
      float APV6E4num=0;
      float APV7E1num=0;
      float APV7E2num=0;
      float APV8num=0;
      float APV9num=0;
      */
      LAGDnum[APVname[apvnum]] = 0;
      
      float totalcharge=0;      
      float Tracker1xtotalcharge=0;      
      float Tracker1ytotalcharge=0;
      float Tracker2xtotalcharge=0;
      float Tracker2ytotalcharge=0;
      float Tracker3xtotalcharge=0;
      float Tracker3ytotalcharge=0;
      float Tracker4xtotalcharge=0;
      float Tracker4ytotalcharge=0;
      /*
      float APV2totalcharge=0;
      float APV3E1totalcharge=0;
      float APV3E2totalcharge=0;      
      float APV4E3totalcharge=0;
      float APV4E4totalcharge=0;
      float APV5E3totalcharge=0;
      float APV5E4totalcharge=0;
      float APV6E3totalcharge=0;
      float APV6E4totalcharge=0;
      float APV7E1totalcharge=0;
      float APV7E2totalcharge=0;
      float APV8totalcharge=0;
      float APV9totalcharge=0;
      */

      LAGDtotalcharge[APVname[apvnum]] = 0;
      
      float Currentindex=0;
      float previndex;
      float tolerance = 2;
      
      //Find hit locations in trackers and LAGD by summing over hits found and using the signal pulses to find an exact location via COM equation. Tracker hits are then converted to mm, LAGD is left in terms of strip for now

      
      if(display_mode == 0){
	//if(maxstrips.size() == 0)
	//ClusterdistAll->Fill(0);
	
	if(GoodEvent[totalEventiterator]){
	  int k=0;	
	  cout << *evtID << endl;
	  //cout << maxstrips.empty() << endl;
	  for(auto i = maxstrips.begin(); i != maxstrips.end(); i++){    
	    cout << *i <<  ", " << maxcharges[k] << endl;
	    k++;
	  }

	  
	  auto NumSubEvents = StripSep(maxstrips, maxcharges);

	 
	  
	  cout << "Number of sub events in event " << *evtID << ": " << NumSubEvents.size() << endl;
	  //ClusterNumberHist->Fill(NumSubEvents.size());
	
	  cout << "Length of each Sub Event: " << endl;
	
	  for(auto i = NumSubEvents.begin(); i != NumSubEvents.end(); i++){
	    cout << "This eventplot had a strip multiplicity of " << *i << endl;
	    ClusterdistAll->Fill(*i);
	    if(*i > 1){//only allow hits with more than one strip
	      Clusterdist[evtype]->Fill(*i);	      
	    }
	  }
        	
	  previndex = maxstrips.at(0);	
	  for(auto i = 0; i < abs(int(maxcharges.size())); i++){

	    num += maxstrips.at(i)*maxcharges.at(i);
	    totalcharge += maxcharges.at(i);
	  }
	
	  cout << num << ", " << totalcharge << endl;
	  cout << num/totalcharge << endl;
	  ClusterChargeHist->Fill(totalcharge);
	  //if(Currentindex > tolerance + previndex){	    
	  EventAvgCharge[evtype]->Fill(totalcharge);
		
	  //}
	  //if(*evtID == 20) return;
	}
      }
      
      if(display_mode == 1 || display_mode == 2 || display_mode == 3 || display_mode == 5){

	      
	if(GoodEvent[totalEventiterator]){
	  cout << "What event is in here?: " << *evtID << endl;
	  
	  EventIterator.push_back(totalEventiterator);
	  //{
	  /*
	  if(LAGDMaxStrips[APVname[apvnum]].size() == 2){
	    for(auto i = LAGDMaxStrips[APVname[apvnum]].begin(); i < LAGDMaxStrips[APVname[apvnum]].end(); i++){
	      ResCorrFileSM2 << *i << " ";
	    }
	    ResCorrFileSM2 << endl;;
	    for(auto i = LAGDMaxCharges[APVname[apvnum]].begin(); i < LAGDMaxCharges[APVname[apvnum]].end(); i++){
	      ResCorrFileSM2 << *i << " ";
	    }    
	    ResCorrFileSM2 << endl;

	  }
	  if(LAGDMaxStrips[APVname[apvnum]].size() == 3){
	    for(auto i = LAGDMaxStrips[APVname[apvnum]].begin(); i < LAGDMaxStrips[APVname[apvnum]].end(); i++){
	      ResCorrFileSM3 << *i << " ";
	    }
	    ResCorrFileSM3 << endl;;
	    for(auto i = LAGDMaxCharges[APVname[apvnum]].begin(); i < LAGDMaxCharges[APVname[apvnum]].end(); i++){
	      ResCorrFileSM3 << *i << " ";
	    }    
	    ResCorrFileSM3 << endl;

	  }
	  if(LAGDMaxStrips[APVname[apvnum]].size() == 4){
	    for(auto i = LAGDMaxStrips[APVname[apvnum]].begin(); i < LAGDMaxStrips[APVname[apvnum]].end(); i++){
	      ResCorrFileSM4 << *i << " ";
	    }
	    ResCorrFileSM4 << endl;;
	    for(auto i = LAGDMaxCharges[APVname[apvnum]].begin(); i < LAGDMaxCharges[APVname[apvnum]].end(); i++){
	      ResCorrFileSM4 << *i << " ";
	    }    
	    ResCorrFileSM4 << endl;

	  }
	  if(LAGDMaxStrips[APVname[apvnum]].size() == 5){
	    for(auto i = LAGDMaxStrips[APVname[apvnum]].begin(); i < LAGDMaxStrips[APVname[apvnum]].end(); i++){
	      ResCorrFileSM5 << *i << " ";
	    }
	    ResCorrFileSM5 << endl;;
	    for(auto i = LAGDMaxCharges[APVname[apvnum]].begin(); i < LAGDMaxCharges[APVname[apvnum]].end(); i++){
	      ResCorrFileSM5 << *i << " ";
	    }    
	    ResCorrFileSM5 << endl;

	  }
	  */
	  for(auto i = LAGDMaxStrips[APVname[apvnum]].begin(); i < LAGDMaxStrips[APVname[apvnum]].end(); i++){
	    ResCorrFile << *i << " ";
	  }
	  ResCorrFile << endl;;
	  for(auto i = LAGDMaxCharges[APVname[apvnum]].begin(); i < LAGDMaxCharges[APVname[apvnum]].end(); i++){
	    ResCorrFile << *i << " ";
	  }    
	  ResCorrFile << endl;

	  
	  
	    //loop to find exact hit location
	    //cout << "Here?" << endl;
	  // for(auto i = ReducedTracker1ymaxstrips.begin(); i < ReducedTracker1ymaxstrips.end(); i++){
	  //   cout << *i << endl;
	  // }

	  // cout << "LAGD strips" << endl;
	  // for(auto i = LAGDMaxStrips[APVname[apvnum]].begin(); i < LAGDMaxStrips[APVname[apvnum]].end(); i++){
	  //   cout << *i << endl;
	  // }



	  
	  


	  


	  
	  cout << "End of OG vector" << endl;
	  ReducedTracker1xmaxstrips.resize(10,0);
	  ReducedTracker1ymaxstrips.resize(10,0);
	  ReducedTracker2xmaxstrips.resize(10,0);
	  ReducedTracker2ymaxstrips.resize(10,0);
	  ReducedTracker3xmaxstrips.resize(10,0);
	  ReducedTracker3ymaxstrips.resize(10,0);
	  ReducedTracker4xmaxstrips.resize(10,0);
	  ReducedTracker4ymaxstrips.resize(10,0);
	  
	  ReducedTracker1xmaxcharges.resize(10,0);
	  ReducedTracker1ymaxcharges.resize(10,0);
	  ReducedTracker2xmaxcharges.resize(10,0);
	  ReducedTracker2ymaxcharges.resize(10,0);
	  ReducedTracker3xmaxcharges.resize(10,0);
	  ReducedTracker3ymaxcharges.resize(10,0);
	  ReducedTracker4xmaxcharges.resize(10,0);
	  ReducedTracker4ymaxcharges.resize(10,0);
	  
	  LAGDMaxStrips[APVname[apvnum]].resize(10,0);	    
	  LAGDMaxCharges[APVname[apvnum]].resize(10,0);
	  
	  // for(auto i = ReducedTracker1ymaxstrips.begin(); i < ReducedTracker1ymaxstrips.end(); i++){
	  //   cout << *i << endl;
	  // }
	  // cout << "End of resized vector" << endl;

	  // cout << "LAGD strips" << endl;
	  // for(auto i = LAGDMaxStrips[APVname[apvnum]].begin(); i < LAGDMaxStrips[APVname[apvnum]].end(); i++){
	  //   cout << *i << endl;
	  // }
	  
	  for(int m = 0; m < 10; m++){	 
	    
	    if(display_mode == 0){
	      num += maxstrips.at(m)*maxcharges.at(m);
	      totalcharge += maxcharges.at(m);	      
	    }
	    
	  
	    if(display_mode == 1 || display_mode == 2 || display_mode == 5){
	      Tracker1xnum += ReducedTracker1xmaxstrips.at(m)*ReducedTracker1xmaxcharges.at(m);
	      Tracker1ynum += (ReducedTracker1ymaxstrips.at(m))*ReducedTracker1ymaxcharges.at(m);
	      Tracker2xnum += ReducedTracker2xmaxstrips.at(m)*ReducedTracker2xmaxcharges.at(m);
	      Tracker2ynum += (ReducedTracker2ymaxstrips.at(m))*ReducedTracker2ymaxcharges.at(m);	    
	      Tracker3xnum += ReducedTracker3xmaxstrips.at(m)*ReducedTracker3xmaxcharges.at(m);
	      Tracker3ynum += (ReducedTracker3ymaxstrips.at(m))*ReducedTracker3ymaxcharges.at(m);
	      Tracker4xnum += ReducedTracker4xmaxstrips.at(m)*ReducedTracker4xmaxcharges.at(m);
	      Tracker4ynum += (ReducedTracker4ymaxstrips.at(m))*ReducedTracker4ymaxcharges.at(m);
	      
	      
	      Tracker1xtotalcharge += ReducedTracker1xmaxcharges.at(m);
	      Tracker1ytotalcharge += ReducedTracker1ymaxcharges.at(m);
	      Tracker2xtotalcharge += ReducedTracker2xmaxcharges.at(m);
	      Tracker2ytotalcharge += ReducedTracker2ymaxcharges.at(m);	    
	      Tracker3xtotalcharge += ReducedTracker3xmaxcharges.at(m);
	      Tracker3ytotalcharge += ReducedTracker3ymaxcharges.at(m);
	      Tracker4xtotalcharge += ReducedTracker4xmaxcharges.at(m);
	      Tracker4ytotalcharge += ReducedTracker4ymaxcharges.at(m);
	    }
	  

	    if(display_mode == 3 || display_mode == 1  || AlignmentMode == 0){

	      LAGDnum[APVname[apvnum]] += LAGDMaxStrips[APVname[apvnum]].at(m)*LAGDMaxCharges[APVname[apvnum]].at(m);
	      LAGDtotalcharge[APVname[apvnum]] += LAGDMaxCharges[APVname[apvnum]].at(m);
	      	     
	    }

	  }
	  Clusterchargeoftheseevnts.push_back(LAGDtotalcharge[APVname[apvnum]]);

	  cout << "Tx: " << Tracker1xtotalcharge << ", " << "Ty: " << Tracker1ytotalcharge << endl;
	  T1ChargeRatio->Fill(Tracker1xtotalcharge, Tracker1ytotalcharge);
	      
	  //Other hits
	  //cout << "num: " << num << " and total charge " << totalcharge << endl;
	  //if(display_mode == 0)
	  //  loc = num/totalcharge;
	  //	cout << "Hit at location: " << loc << endl;
	  if(display_mode == 1 || display_mode == 2 || display_mode == 5){
	    Tracker1xloc = Tracker1xnum/Tracker1xtotalcharge;
	    Tracker1yloc = Tracker1ynum/Tracker1ytotalcharge;
	    Tracker3xloc = Tracker3xnum/Tracker3xtotalcharge;
	    Tracker3yloc = Tracker3ynum/Tracker3ytotalcharge;
	    Tracker2xloc = Tracker2xnum/Tracker2xtotalcharge;
	    Tracker2yloc = Tracker2ynum/Tracker2ytotalcharge;
	    Tracker4xloc = Tracker4xnum/Tracker4xtotalcharge;
	    Tracker4yloc = Tracker4ynum/Tracker4ytotalcharge;

	   
	  }
	 
	  if(display_mode == 1 || display_mode == 3|| AlignmentMode == 0){

	    LAGDloc[APVname[apvnum]] = LAGDnum[APVname[apvnum]]/LAGDtotalcharge[APVname[apvnum]];

	    
	    LAGDHitsVec.push_back(LAGDloc[APVname[apvnum]]);
	    EventIDs.push_back(totalEventiterator);
	    //efficiencynum++;
	  }       
	  
	  //Stripcorr->SetPoint(Stripcorrit, Tracker1yloc, LAGDloc[APVname[apvnum]]);
	  //TrackerStripcorr->SetPoint(Stripcorrit, Tracker1yloc, Tracker2yloc);
	  cout << "going here?" << endl;
	  cout << Tracker1yloc << ", " << LAGDloc[APVname[apvnum]] << endl;
	  Stripcorr->Fill(Tracker1yloc, LAGDloc[APVname[apvnum]]);
	  TrackerStripcorr->Fill(Tracker1yloc, Tracker3yloc);
	  
	  StripcorrProf->Fill(Tracker1yloc, LAGDloc[APVname[apvnum]]);
	  TrackerStripcorrProf->Fill(Tracker1yloc, Tracker3yloc);
	  
	  //LAGDstrips->SetPoint(Stripcorrit, Stripcorrit, LAGDloc[APVname[apvnum]]);
	  //Trackerstrips->SetPoint(Stripcorrit, Stripcorrit, Tracker1yloc);
	  //Diffbtwnstrips->SetPoint(Stripcorrit, Stripcorrit, Tracker1yloc - APV3E1loc);
	  Stripcorrit++;
	  
	  cout << "Hit at (" << Tracker1xloc << "," << Tracker1yloc << ") in Tracker 1" << endl;
	  cout << "Hit at (" << Tracker2xloc << "," << Tracker2yloc << ") in Tracker 2" << endl;
	  cout << "Hit at (" << Tracker3xloc << "," << Tracker3yloc << ") in Tracker 3" << endl;
	  cout << "Hit at (" << Tracker4xloc << "," << Tracker4yloc << ") in Tracker 4" << endl;
	  cout << "Hit at (" << LAGDloc[APVname[apvnum]] << ")" << " in " << APVname[apvnum] << endl;
      
	  //if(Tracker1yloc <= 15 && LAGDloc[APVname[apvnum]] < 26){break;}
	  //if(Tracker1yloc/LAGDloc[APVname[apvnum]] <= 2){Badevents.push_back(to_string(totalEventiterator));Badevents.push_back(evtype);Badevents.push_back(to_string(Tracker1yloc));Badevents.push_back(to_string(LAGDloc[APVname[apvnum]]));}

	  auto Tracker1xavg=0.0;
	  for(auto h = Tracker1xtimebin.begin(); h < Tracker1xtimebin.end(); h++){
	    //cout << *h << endl;
	    Tracker1xavg+=*h;
	  }
	  Tracker1xavg = Tracker1xavg/Tracker1xtimebin.size();

	  auto LAGDavg=0.0;
	  for(auto h = LAGDTimeBin[APVname[apvnum]].begin(); h < LAGDTimeBin[APVname[apvnum]].end(); h++){
	    //cout << *h << endl;
	    LAGDavg+=*h;
	  }
	  LAGDavg = LAGDavg/LAGDTimeBin[APVname[apvnum]].size();
	  
	  DeltaTTrackersLAGD->Fill(Tracker1xavg - LAGDavg);
	  /*
	    cout << "Hit at (" << APV3E2loc << ")" << " in APV3E2" << endl;
	    cout << "Hit at (" << APV4E3loc << ")" << " in APV4E3" << endl;
	    cout << "Hit at (" << APV4E4loc << ")" << " in APV4E4" << endl;
	    cout << "Hit at (" << APV5E3loc << ")" << " in APV5E3" << endl;
	    cout << "Hit at (" << APV5E4loc << ")" << " in APV5E4" << endl;
	    cout << "Hit at (" << APV6E3loc << ")" << " in APV6E3" << endl;
	    cout << "Hit at (" << APV6E4loc << ")" << " in APV6E4" << endl;
	    cout << "Hit at (" << APV7E1loc << ")" << " in APV7E1" << endl;
	    cout << "Hit at (" << APV7E2loc << ")" << " in APV7E2" << endl;
	  */

	  //return;

	  if(display_mode == 2 || display_mode == 5){
	    auto T1xcoord = Tracker1xloc*0.4; //0.4 = 51.2/128	    
	    auto T1ycoord = Tracker1yloc*0.4;//0.4 = 102.4/256
	    auto T2xcoord = Tracker2xloc*0.4;
	    auto T2ycoord = Tracker2yloc*0.4;

	    cout << LAGDloc[APVname[apvnum]] << endl;
	    //Tracker2Ystriphits->Fill(APV3E1loc);
	    
	    auto T3xcoord = Tracker3xloc*0.4;
	    auto T3ycoord = Tracker3yloc*0.4;
	    auto T4xcoord = Tracker4xloc*0.4;
	    auto T4ycoord = Tracker4yloc*0.4;

	    TrackerEventIDs.push_back(totalEventiterator);
	    //cout << "Pushing back hit coordinates" << endl;
	    Tracker1hitcoords[evtype].push_back(T1xcoord);	    
	    Tracker1hitcoords[evtype].push_back(T1ycoord);
	    Tracker1hitcoords[evtype].push_back(1596.4);//1596.4	  
	    
	    Tracker2hitcoords[evtype].push_back(T2xcoord);
	    Tracker2hitcoords[evtype].push_back(T2ycoord);
	    Tracker2hitcoords[evtype].push_back(1541.6);//1541.6
	    
	    Tracker3hitcoords[evtype].push_back(T3xcoord);
	    Tracker3hitcoords[evtype].push_back(T3ycoord);
	    Tracker3hitcoords[evtype].push_back(54.8);//54.8
	    
	    Tracker4hitcoords[evtype].push_back(T4xcoord);
	    Tracker4hitcoords[evtype].push_back(T4ycoord);
	    Tracker4hitcoords[evtype].push_back(0);//0
	    
	    if(display_mode == 2){
	      Tracker1Hitmap->Fill(T1xcoord, T1ycoord);
	      Tracker3Hitmap->Fill(T3xcoord, T3ycoord);
	      Tracker2Hitmap->Fill(T2xcoord, T2ycoord);
	      Tracker4Hitmap->Fill(T4xcoord, T4ycoord);
	    }
	    
	  }
	}
      }
      
      
      
      cout << "Made it to here?" << endl;
      
      string title = "Event: ";
      string rest = ", from ";
      string restAPV = ", from APV ";
      
      //int n = strtitle.length();
      
      string subtitle = "ph ";
      //char subtitle[n+1];
      
      
      if(display_mode == 0){      
	//cout << "here?" << endl;

	if(displaymode0event){
	  title += to_string(*evtID);
	  title += rest;
	  title += inputfiles[evtype];
	  title += restAPV;
	  title += APVname[apvnum];
	  int n=title.length();
	  
	  char chartitle[n];
	  strcpy(chartitle, title.c_str());
	  
	  //cout << "1 here?" << endl;
	  	  
	  subtitle += to_string(*evtID);
	  subtitle += inputfiles[evtype];
	  subtitle += apvnum;
	  
	  n=subtitle.length();
	  char charsubtitle[n];
	  	  
	  strcpy(charsubtitle, subtitle.c_str());

	  //cout << chartitle << endl;
	  //cout << subtitle << endl;
	  // cout << "Past the wierd part" << endl;
      
	
	  PHCanvas[*evtID] = new TCanvas(charsubtitle, chartitle, 500, 500);
      

	  PHCanvas[*evtID]->Draw();

	  //PulseHeight[*evtID]Hist[*evtID]->Draw("LEGO2");
	  //gPad->DrawFrame(0,64,0,300);
	  PulseHeight[*evtID]->SetMarkerStyle(20);
	  PulseHeight[*evtID]->Draw("LEGO");
	  PulseHeight[*evtID]->SetNpx(128);
	  PulseHeight[*evtID]->SetNpy(6);
	  PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	  PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	  PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	  PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	  PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	  PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	  	  
	}
      }
      
      
      //Pedestal display mode
      if(display_mode == 4){
	
	float PEDXvals[PedXvals.size()];
	std::copy(PedXvals.begin(), PedXvals.end(), PEDXvals);

      
	float Tracker1XPS[Tracker1xPedstds.size()];
	std::copy(Tracker1xPedstds.begin(), Tracker1xPedstds.end(), Tracker1XPS);
	float Tracker1YPS[Tracker1yPedstds.size()];
	std::copy(Tracker1yPedstds.begin(), Tracker1yPedstds.end(), Tracker1YPS);
	float Tracker2XPS[Tracker2xPedstds.size()];
	std::copy(Tracker2xPedstds.begin(), Tracker2xPedstds.end(), Tracker2XPS);
	float Tracker2YPS[Tracker2yPedstds.size()];
	std::copy(Tracker2yPedstds.begin(), Tracker2yPedstds.end(), Tracker2YPS);
	float Tracker3XPS[Tracker3xPedstds.size()];
	std::copy(Tracker3xPedstds.begin(), Tracker3xPedstds.end(), Tracker3XPS);
	float Tracker3YPS[Tracker3yPedstds.size()];
	std::copy(Tracker3yPedstds.begin(), Tracker3yPedstds.end(), Tracker3YPS);
	float Tracker4XPS[Tracker4xPedstds.size()];
	std::copy(Tracker4xPedstds.begin(), Tracker4xPedstds.end(), Tracker4XPS);
	float Tracker4YPS[Tracker4yPedstds.size()];
	std::copy(Tracker4yPedstds.begin(), Tracker4yPedstds.end(), Tracker4YPS);

	float APV2PS[APV2Pedstds.size()];
	std::copy(APV2Pedstds.begin(), APV2Pedstds.end(), APV2PS);
	float APV3E1PS[APV3E1Pedstds.size()];
	std::copy(APV3E1Pedstds.begin(), APV3E1Pedstds.end(), APV3E1PS);
	float APV3E2PS[APV3E2Pedstds.size()];
	std::copy(APV3E2Pedstds.begin(), APV3E2Pedstds.end(), APV3E2PS);
	float APV4E3PS[APV4E3Pedstds.size()];
	std::copy(APV4E3Pedstds.begin(), APV4E3Pedstds.end(), APV4E3PS);
	float APV4E4PS[APV4E4Pedstds.size()];
	std::copy(APV4E4Pedstds.begin(), APV4E4Pedstds.end(), APV4E4PS);
	float APV5E3PS[APV5E3Pedstds.size()];
	std::copy(APV5E3Pedstds.begin(), APV5E3Pedstds.end(), APV5E3PS);
	float APV5E4PS[APV5E4Pedstds.size()];
	std::copy(APV5E4Pedstds.begin(), APV5E4Pedstds.end(), APV5E4PS);
	float APV6E3PS[APV6E3Pedstds.size()];
	std::copy(APV6E3Pedstds.begin(), APV6E3Pedstds.end(), APV6E3PS);
	float APV6E4PS[APV6E4Pedstds.size()];
	std::copy(APV6E4Pedstds.begin(), APV6E4Pedstds.end(), APV6E4PS);
	float APV7E1PS[APV7E1Pedstds.size()];
	std::copy(APV7E1Pedstds.begin(), APV7E1Pedstds.end(), APV7E1PS);
	float APV7E2PS[APV7E2Pedstds.size()];
	std::copy(APV7E2Pedstds.begin(), APV7E2Pedstds.end(), APV7E2PS);
	float APV8PS[APV8Pedstds.size()];
	std::copy(APV8Pedstds.begin(), APV8Pedstds.end(), APV8PS);
	float APV9PS[APV9Pedstds.size()];
	std::copy(APV9Pedstds.begin(), APV9Pedstds.end(), APV9PS);

        
	//auto Tracker1XPedData = new TGraph(Tracker1xPedstds.size(), PEDXvals, Tracker1XPS);
	auto Tracker1XPedData = new TH1D("Name", "Title; Strips; Strip Noise", Tracker1xPedstds.size(), 0, Tracker1xPedstds.size());
	for(uint i=1;i <= Tracker1xPedstds.size();i++)
	  Tracker1XPedData->Fill(PEDXvals[Tracker1xPedstds.size() - i], Tracker1XPS[i]);

	cout << Tracker1xPedstds.size() << endl;
	
	//auto Tracker1YPedData = new TGraph(Tracker1yPedstds.size(), PEDXvals, Tracker1YPS);
	auto Tracker1YPedData = new TH1D("Tracker1Y", "Title; Strips, ADC charge", Tracker1yPedstds.size(), 0, Tracker1yPedstds.size());
	for(uint i=1;i <= Tracker1yPedstds.size();i++)
	  Tracker1YPedData->Fill(PEDXvals[Tracker1yPedstds.size() - i], Tracker1YPS[i]);

	
	//auto Tracker2XPedData = new TGraph(Tracker2xPedstds.size(), PEDXvals, Tracker2XPS);
	auto Tracker2XPedData = new TH1D("Tracker2X", "Title; Strips, ADC charge", Tracker2xPedstds.size(), 0, Tracker2xPedstds.size());
	for(uint i=1;i <= Tracker2xPedstds.size();i++)
	  Tracker2XPedData->Fill(PEDXvals[Tracker2xPedstds.size() - i], Tracker2XPS[i]);

	//auto Tracker2YPedData = new TGraph(Tracker2yPedstds.size(), PEDXvals, Tracker2YPS);
	auto Tracker2YPedData = new TH1D("Tracker2Y", "Title; Strips, ADC charge", Tracker2yPedstds.size(), 0, Tracker2yPedstds.size());
	for(uint i=1;i <= Tracker2yPedstds.size();i++)
	  Tracker2YPedData->Fill(PEDXvals[Tracker2yPedstds.size() - i], Tracker2YPS[i]);

	//auto Tracker3XPedData = new TGraph(Tracker3xPedstds.size(), PEDXvals, Tracker3XPS);
	auto Tracker3XPedData = new TH1D("Tracker3X", "Title; Strips, ADC charge", Tracker3xPedstds.size(), 0, Tracker3xPedstds.size());
	for(uint i=1;i <= Tracker3xPedstds.size();i++)
	  Tracker3XPedData->Fill(PEDXvals[Tracker3xPedstds.size() - i], Tracker3XPS[i]);
	
	//auto Tracker3YPedData = new TGraph(Tracker3yPedstds.size(), PEDXvals, Tracker3YPS);
	auto Tracker3YPedData = new TH1D("Tracker3Y", "Title; Strips, ADC charge", Tracker3yPedstds.size(), 0, Tracker3yPedstds.size());
	for(uint i=1;i <= Tracker3yPedstds.size();i++)
	  Tracker3YPedData->Fill(PEDXvals[Tracker3yPedstds.size() - i], Tracker3YPS[i]);
	
	//auto Tracker4XPedData = new TGraph(Tracker4xPedstds.size(), PEDXvals, Tracker4XPS);
	auto Tracker4XPedData = new TH1D("Tracker4X", "Title; Strips, ADC charge", Tracker4xPedstds.size(), 0, Tracker4xPedstds.size());
	for(uint i=1;i <= Tracker4xPedstds.size();i++)
	  Tracker4XPedData->Fill(PEDXvals[Tracker4xPedstds.size() - i], Tracker4XPS[i]);
	
	//auto Tracker4YPedData = new TGraph(Tracker4yPedstds.size(), PEDXvals, Tracker4YPS);
	auto Tracker4YPedData = new TH1D("Tracker4Y", "Title; Strips, ADC charge", Tracker4yPedstds.size(), 0, Tracker4yPedstds.size());
	for(uint i=1;i <= Tracker4yPedstds.size();i++)
	  Tracker4YPedData->Fill(PEDXvals[Tracker4yPedstds.size() - i], Tracker4YPS[i]);

	auto APV2PedData= new TGraph(APV2Pedstds.size(), PEDXvals, APV2PS);
	auto APV3E1PedData= new TGraph(APV3E1Pedstds.size(), PEDXvals, APV3E1PS);
	auto APV3E2PedData = new TGraph(APV3E2Pedstds.size(), PEDXvals, APV3E2PS);
	auto APV4E3PedData = new TGraph(APV4E3Pedstds.size(), PEDXvals, APV4E3PS);
	auto APV4E4PedData = new TGraph(APV4E4Pedstds.size(), PEDXvals, APV4E4PS);
	auto APV5E3PedData = new TGraph(APV5E3Pedstds.size(), PEDXvals, APV5E3PS);
	auto APV5E4PedData = new TGraph(APV5E4Pedstds.size(), PEDXvals, APV5E4PS);
	auto APV6E3PedData = new TGraph(APV6E3Pedstds.size(), PEDXvals, APV6E3PS);
	auto APV6E4PedData = new TGraph(APV6E4Pedstds.size(), PEDXvals, APV6E4PS);
	auto APV7E1PedData = new TGraph(APV7E1Pedstds.size(), PEDXvals, APV7E1PS);
	auto APV7E2PedData = new TGraph(APV7E2Pedstds.size(), PEDXvals, APV7E2PS);
	auto APV8PedData= new TGraph(APV8Pedstds.size(), PEDXvals, APV8PS);
	auto APV9PedData= new TGraph(APV9Pedstds.size(), PEDXvals, APV9PS);

      
	auto PedCanvas2 = new TCanvas("Pedcanvas2", "Pedestal Std Devs per channel on all 8 Tracking APVs", 2000, 1000);

	PedCanvas2->Divide(4,2);
	
	PedCanvas2->Draw();
	
	//first display (Tracker X and Y)
	PedCanvas2->cd(1);
	Tracker1XPedData->Draw();
	Tracker1XPedData->SetTitle("Tracker1X Pedestal derived tolerances");
	Tracker1XPedData->GetXaxis()->SetTitle("Strip Number");
	Tracker1XPedData->GetXaxis()->CenterTitle(true);
	//Tracker1XPedData->GetYaxis()->SetTitle("ADC Charge");
	//
	Tracker1XPedData->GetYaxis()->CenterTitle(true);
	
	
	PedCanvas2->cd(5);
	Tracker1YPedData->Draw();
	Tracker1YPedData->SetTitle("Tracker1Y Pedestal derived tolerances");
	Tracker1YPedData->GetXaxis()->SetTitle("Strip Number");
	Tracker1YPedData->GetXaxis()->CenterTitle(true);
	Tracker1YPedData->GetYaxis()->SetTitle("ADC Charge");
	Tracker1YPedData->GetYaxis()->CenterTitle(true);
	
	
	PedCanvas2->cd(2);
	Tracker2XPedData->Draw();
	Tracker2XPedData->SetTitle("Tracker2X Pedestal derived tolerances");
	Tracker2XPedData->GetXaxis()->SetTitle("Strip Number");
	Tracker2XPedData->GetXaxis()->CenterTitle(true);
	Tracker2XPedData->GetYaxis()->SetTitle("ADC Charge");
	Tracker2XPedData->GetYaxis()->CenterTitle(true);
	
	PedCanvas2->cd(6);
	Tracker2YPedData->Draw();
	Tracker2YPedData->SetTitle("Tracker2Y Pedestal derived tolerances");
	Tracker2YPedData->GetXaxis()->SetTitle("Strip Number");
	Tracker2YPedData->GetXaxis()->CenterTitle(true);
	Tracker2YPedData->GetYaxis()->SetTitle("ADC Charge");
	Tracker2YPedData->GetYaxis()->CenterTitle(true);

	PedCanvas2->cd(3);
	Tracker3XPedData->Draw();
	Tracker3XPedData->SetTitle("Tracker3X Pedestal derived tolerances");
	Tracker3XPedData->GetXaxis()->SetTitle("Strip Number");
	Tracker3XPedData->GetXaxis()->CenterTitle(true);
	Tracker3XPedData->GetYaxis()->SetTitle("ADC Charge");
	Tracker3XPedData->GetYaxis()->CenterTitle(true);
	

	PedCanvas2->cd(7);
	Tracker3YPedData->Draw();
	Tracker3YPedData->SetTitle("Tracker3Y Pedestal derived tolerances");
	Tracker3YPedData->GetXaxis()->SetTitle("Strip Number");
	Tracker3YPedData->GetXaxis()->CenterTitle(true);
	Tracker3YPedData->GetYaxis()->SetTitle("ADC Charge");
	Tracker3YPedData->GetYaxis()->CenterTitle(true);
	
	PedCanvas2->cd(4);
	Tracker4XPedData->Draw();
	Tracker4XPedData->SetTitle("Tracker4X Pedestal derived tolerances");
	Tracker4XPedData->GetXaxis()->SetTitle("Strip Number");
	Tracker4XPedData->GetXaxis()->CenterTitle(true);
	Tracker4XPedData->GetYaxis()->SetTitle("ADC Charge");
	Tracker4XPedData->GetYaxis()->CenterTitle(true);
	
	PedCanvas2->cd(8);
	Tracker4YPedData->Draw();
	Tracker4YPedData->SetTitle("Tracker4Y Pedestal derived tolerances");
	Tracker4YPedData->GetXaxis()->SetTitle("Strip Number");
	Tracker4YPedData->GetXaxis()->CenterTitle(true);
	Tracker4YPedData->GetYaxis()->SetTitle("ADC Charge");
	Tracker4YPedData->GetYaxis()->CenterTitle(true);
	
	//Now for LAGD APVs
	auto PedCanvas1 = new TCanvas("Pedcanvas1", "Pedestal Std Devs per channel on all 8 LAGD APVs", 2000, 1000);

	PedCanvas1->Divide(5,2);
	
	PedCanvas1->Draw();

	PedCanvas1->cd(1);
	APV3E1PedData->Draw();
	APV3E1PedData->SetTitle("APV3E1 Pedestal Std Devs");
	APV3E1PedData->GetXaxis()->SetTitle("Strip Number");
	APV3E1PedData->GetXaxis()->CenterTitle(true);
	APV3E1PedData->GetYaxis()->SetTitle("ADC Charge");
	APV3E1PedData->GetYaxis()->CenterTitle(true);
		
	PedCanvas1->cd(6);
	APV7E1PedData->Draw();
	APV7E1PedData->SetTitle("APV7E1 Pedestal Std Devs");
	APV7E1PedData->GetXaxis()->SetTitle("Strip Number");
	APV7E1PedData->GetXaxis()->CenterTitle(true);
	APV7E1PedData->GetYaxis()->SetTitle("ADC Charge");
	APV7E1PedData->GetYaxis()->CenterTitle(true);
	
	PedCanvas1->cd(2);
	APV3E2PedData->Draw();
	APV3E2PedData->SetTitle("APV3E2 Pedestal Std Devs");
	APV3E2PedData->GetXaxis()->SetTitle("Strip Number");
	APV3E2PedData->GetXaxis()->CenterTitle(true);
	APV3E2PedData->GetYaxis()->SetTitle("ADC Charge");
	APV3E2PedData->GetYaxis()->CenterTitle(true);
	
	PedCanvas1->cd(7);
	APV7E2PedData->Draw();
	APV7E2PedData->SetTitle("APV7E2 Pedestal Std Devs");
	APV7E2PedData->GetXaxis()->SetTitle("Strip Number");
	APV7E2PedData->GetXaxis()->CenterTitle(true);
	APV7E2PedData->GetYaxis()->SetTitle("ADC Charge");
	APV7E2PedData->GetYaxis()->CenterTitle(true);

	PedCanvas1->cd(3);
        APV6E3PedData->Draw();
	APV6E3PedData->SetTitle("APV6E3 Pedestal Std Devs");
	APV6E3PedData->GetXaxis()->SetTitle("Strip Number");
	APV6E3PedData->GetXaxis()->CenterTitle(true);
	APV6E3PedData->GetYaxis()->SetTitle("ADC Charge");
	APV6E3PedData->GetYaxis()->CenterTitle(true);
	
	PedCanvas1->cd(8);
	APV6E4PedData->Draw();
	APV6E4PedData->SetTitle("APV6E4 Pedestal Std Devs");
	APV6E4PedData->GetXaxis()->SetTitle("Strip Number");
	APV6E4PedData->GetXaxis()->CenterTitle(true);
	APV6E4PedData->GetYaxis()->SetTitle("ADC Charge");
	APV6E4PedData->GetYaxis()->CenterTitle(true);
	
	PedCanvas1->cd(4);
	APV5E3PedData->Draw();
	APV5E3PedData->SetTitle("APV5E3 Pedestal Std Devs");
	APV5E3PedData->GetXaxis()->SetTitle("Strip Number");
	APV5E3PedData->GetXaxis()->CenterTitle(true);
	APV5E3PedData->GetYaxis()->SetTitle("ADC Charge");
	APV5E3PedData->GetYaxis()->CenterTitle(true);
	
	PedCanvas1->cd(9);
	APV5E4PedData->Draw();
	APV5E4PedData->SetTitle("APV5E4 Pedestal Std Devs");
	APV5E4PedData->GetXaxis()->SetTitle("Strip Number");
	APV5E4PedData->GetXaxis()->CenterTitle(true);
	APV5E4PedData->GetYaxis()->SetTitle("ADC Charge");
	APV5E4PedData->GetYaxis()->CenterTitle(true);

	PedCanvas1->cd(5);
	APV4E3PedData->Draw();
	APV4E3PedData->SetTitle("APV4E3 Pedestal Std Devs");
	APV4E3PedData->GetXaxis()->SetTitle("Strip Number");
	APV4E3PedData->GetXaxis()->CenterTitle(true);
	APV4E3PedData->GetYaxis()->SetTitle("ADC Charge");
	APV4E3PedData->GetYaxis()->CenterTitle(true);
	
	PedCanvas1->cd(10);
	APV4E4PedData->Draw();
	APV4E4PedData->SetTitle("APV4E4 Pedestal Std Devs");
	APV4E4PedData->GetXaxis()->SetTitle("Strip Number");
	APV4E4PedData->GetXaxis()->CenterTitle(true);
	APV4E4PedData->GetYaxis()->SetTitle("ADC Charge");
	APV4E4PedData->GetYaxis()->CenterTitle(true);	
      

	//Now, the abnormal APVs: 2,8,9

	auto PedCanvas3 = new TCanvas("Pedcanvas3", "Pedestal Std Devs per channel on APVs 2, 8, and 9", 1000, 1000);

	PedCanvas3->Divide(2,2);

	PedCanvas3->Draw();

	//first display (Tracker X and Y)
	PedCanvas3->cd(1);
	APV2PedData->Draw();
	APV2PedData->SetTitle("APV2 Pedestal Std Devs");
	APV2PedData->GetXaxis()->SetTitle("Strip Number");
	APV2PedData->GetXaxis()->CenterTitle(true);
	APV2PedData->GetYaxis()->SetTitle("ADC Charge");
	APV2PedData->GetYaxis()->CenterTitle(true);


	PedCanvas3->cd(2);
	APV8PedData->Draw();
	APV8PedData->SetTitle("APV8 Pedestal Std Devs");
	APV8PedData->GetXaxis()->SetTitle("Strip Number");
	APV8PedData->GetXaxis()->CenterTitle(true);
	APV8PedData->GetYaxis()->SetTitle("ADC Charge");
	APV8PedData->GetYaxis()->CenterTitle(true);


	PedCanvas3->cd(3);
	APV9PedData->Draw();
	APV9PedData->SetTitle("APV9 Pedestal Std Devs");
	APV9PedData->GetXaxis()->SetTitle("Strip Number");
	APV9PedData->GetXaxis()->CenterTitle(true);
	APV9PedData->GetYaxis()->SetTitle("ADC Charge");
	APV9PedData->GetYaxis()->CenterTitle(true);      
      }
      
      //currently just APVs 3,4,5,6,7

      if(display_mode == 3){
	//cout << "Here?" << endl;
	title += to_string(*evtID);
	title += rest;
	title += inputfiles[evtype];

	int n=title.length();
	  
	char chartitle[n];
	strcpy(chartitle, title.c_str());
	  
	//cout << "1 here?" << endl;
	  	  
	subtitle += to_string(*evtID);
	subtitle += inputfiles[evtype];
	  
	n=subtitle.length();
	char charsubtitle[n];
	  	  
	strcpy(charsubtitle, subtitle.c_str());
	  
	PHCanvas[*evtID] = new TCanvas(charsubtitle, chartitle, 1000, 1800);
	//cout << "yes" << endl;
	  
	PHCanvas[*evtID]->Divide(6,2);
	PHCanvas[*evtID]->Draw();
	
	//first column of display (APV3 and APV7)
	PHCanvas[*evtID]->cd(1);
	PHCanvas[*evtID]->DrawFrame(0,0,63,30);
	  
	//APV3E1PulseHeight[*evtID]->SetAxisRange(0.0,63.0,"X");
	//APV3E1PulseHeight[*evtID]->SetAxisRange(0.0,400.0,"Z");
	  
	APV3E1PulseHeight[*evtID]->Draw("LEGO");

	APV3E1PulseHeight[*evtID]->SetTitle("APV3");
	APV3E1PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV3E1PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV3E1PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV3E1PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV3E1PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV3E1PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	  

	PHCanvas[*evtID]->cd(7);
	APV7E1PulseHeight[*evtID]->Draw("LEGO");
	APV7E1PulseHeight[*evtID]->SetTitle("APV7");
	APV7E1PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV7E1PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV7E1PulseHeight[*evtID]->GetXaxis()->SetRange(0,63);
	APV7E1PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV7E1PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV7E1PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV7E1PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV7E1PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
		  

	//second column
	PHCanvas[*evtID]->cd(2);
	PHCanvas[*evtID]->DrawFrame(64,0,128,30);
	APV3E2PulseHeight[*evtID]->GetXaxis()->SetRangeUser(64,127);
	APV3E2PulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
	  
	APV3E2PulseHeight[*evtID]->Draw("LEGO");
	  
	APV3E2PulseHeight[*evtID]->SetTitle("APV3");
	APV3E2PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV3E2PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV3E2PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV3E2PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV3E2PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV3E2PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	  
	
	PHCanvas[*evtID]->cd(8);
	APV7E2PulseHeight[*evtID]->Draw("LEGO");
	APV7E2PulseHeight[*evtID]->SetTitle("APV7");
	APV7E2PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV7E2PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV7E2PulseHeight[*evtID]->GetXaxis()->SetRangeUser(64,127);
	APV7E2PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV7E2PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV7E2PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV7E2PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV7E2PulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
	
	//third colmun
        
	PHCanvas[*evtID]->cd(3);
	APV6E3PulseHeight[*evtID]->Draw("LEGO");
	APV6E3PulseHeight[*evtID]->SetTitle("APV6");
	APV6E3PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV6E3PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV6E3PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV6E3PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV6E3PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV6E3PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV6E3PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(9);
	APV6E4PulseHeight[*evtID]->Draw("LEGO");
	APV6E4PulseHeight[*evtID]->SetTitle("APV6");
	APV6E4PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV6E4PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV6E4PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV6E4PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV6E4PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV6E4PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV6E4PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
	//fourth column

	PHCanvas[*evtID]->cd(4);
	APV5E3PulseHeight[*evtID]->Draw("LEGO");
	APV5E3PulseHeight[*evtID]->SetTitle("APV5");
	APV5E3PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV5E3PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV5E3PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV5E3PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV5E3PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV5E3PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV5E3PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
 
	PHCanvas[*evtID]->cd(10);
	APV5E4PulseHeight[*evtID]->Draw("LEGO");
	APV5E4PulseHeight[*evtID]->SetTitle("APV5");
	APV5E4PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV5E4PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV5E4PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV5E4PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV5E4PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV5E4PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV5E4PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);

	//last column

	PHCanvas[*evtID]->cd(5);
	APV4E3PulseHeight[*evtID]->Draw("LEGO");
	APV4E3PulseHeight[*evtID]->SetTitle("APV4");
	APV4E3PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV4E3PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV4E3PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV4E3PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV4E3PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV4E3PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV4E3PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(11);
	APV4E4PulseHeight[*evtID]->Draw("LEGO");
	APV4E4PulseHeight[*evtID]->SetTitle("APV4");
	APV4E4PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV4E4PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV4E4PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV4E4PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV4E4PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV4E4PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV4E4PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);

	
	PHCanvas[*evtID]->cd(6);
	APV8and9PulseHeight[*evtID]->Draw("LEGO");
	APV8and9PulseHeight[*evtID]->SetTitle("APVs 8 and 9");
	APV8and9PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV8and9PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV8and9PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV8and9PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV8and9PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV8and9PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);

      }
	
      //Trackers and LAGD APVs
	
      if(display_mode == 1){
	  	  
	title += to_string(*evtID);
	title += rest;
	title += inputfiles[evtype];

	int n=title.length();
	  
	char chartitle[n];
	strcpy(chartitle, title.c_str());
	  
	cout << "1 here?" << endl;
	  	  
	subtitle += to_string(*evtID);
	subtitle += inputfiles[evtype];
	  
	n=subtitle.length();
	char charsubtitle[n];
	  	  
	strcpy(charsubtitle, subtitle.c_str());
	  
	PHCanvas[*evtID] = new TCanvas(charsubtitle, chartitle, 1000, 1500);
	//cout << "no" << endl;
	PHCanvas[*evtID]->Divide(7,4);
	PHCanvas[*evtID]->Draw();
	PHCanvas[*evtID]->SetTheta(0);
	PHCanvas[*evtID]->SetPhi(0);
	

	
	//trackers first
	PHCanvas[*evtID]->cd(1);
	//PHCanvas[*evtID]->SetTheta(-45);
	//PHCanvas[*evtID]->SetPhi(0);
	
	Tracker1XPulseHeight[*evtID]->SetMarkerStyle(20);
	//Tracker1XPulseHeight[*evtID]->SetFillColor(1);
	Tracker1XPulseHeight[*evtID]->Draw("LEGO");
	Tracker1XPulseHeight[*evtID]->SetNpx(128);
	Tracker1XPulseHeight[*evtID]->SetNpy(6);
	
	
	Tracker1XPulseHeight[*evtID]->SetTitle("GEM1X");
	Tracker1XPulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	Tracker1XPulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//Tracker1XPulseHeight[*evtID]->GetXaxis()->SetRangeUser(0,127);
	Tracker1XPulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	Tracker1XPulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	Tracker1XPulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	Tracker1XPulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	Tracker1XPulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(2);
	Tracker1YPulseHeight[*evtID]->SetMarkerStyle(20);	
	Tracker1YPulseHeight[*evtID]->Draw("LEGO");
	Tracker1YPulseHeight[*evtID]->SetNpx(128);
	Tracker1YPulseHeight[*evtID]->SetNpy(6);	
	Tracker1YPulseHeight[*evtID]->SetTitle("GEM1Y");
	Tracker1YPulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	Tracker1YPulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//Tracker1YPulseHeight[*evtID]->GetXaxis()->SetRangeUser(128,255);
	Tracker1YPulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	Tracker1YPulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	Tracker1YPulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	Tracker1YPulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	Tracker1YPulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(8);
	Tracker2XPulseHeight[*evtID]->SetMarkerStyle(20);
	Tracker2XPulseHeight[*evtID]->Draw("LEGO");
	Tracker2XPulseHeight[*evtID]->SetNpx(128);
	Tracker2XPulseHeight[*evtID]->SetNpy(6);	
	Tracker2XPulseHeight[*evtID]->SetTitle("GEM2X");
	Tracker2XPulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	Tracker2XPulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//Tracker2XPulseHeight[*evtID]->GetXaxis()->SetRangeUser(0,127);
	Tracker2XPulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	Tracker2XPulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	Tracker2XPulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	Tracker2XPulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	Tracker2XPulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(9);
	Tracker2YPulseHeight[*evtID]->SetMarkerStyle(20);
	Tracker2YPulseHeight[*evtID]->Draw("LEGO");
	Tracker2YPulseHeight[*evtID]->SetNpx(128);
	Tracker2YPulseHeight[*evtID]->SetNpy(6);	
	Tracker2YPulseHeight[*evtID]->SetTitle("GEM2Y");
	Tracker2YPulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	Tracker2YPulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//Tracker2YPulseHeight[*evtID]->GetXaxis()->SetRangeUser(128,255);
	Tracker2YPulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	Tracker2YPulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	Tracker2YPulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	Tracker2YPulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	Tracker2YPulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
 
	PHCanvas[*evtID]->cd(15);
	Tracker3XPulseHeight[*evtID]->SetMarkerStyle(20);
	Tracker3XPulseHeight[*evtID]->Draw("LEGO");
	Tracker3XPulseHeight[*evtID]->SetNpx(128);
	Tracker3XPulseHeight[*evtID]->SetNpy(6);	
	Tracker3XPulseHeight[*evtID]->SetTitle("GEM3X");
	Tracker3XPulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	Tracker3XPulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//Tracker3XPulseHeight[*evtID]->GetXaxis()->SetRangeUser(0,127);
	Tracker3XPulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	Tracker3XPulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	Tracker3XPulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	Tracker3XPulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	Tracker3XPulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(16);
	Tracker3YPulseHeight[*evtID]->SetMarkerStyle(20);
	Tracker3YPulseHeight[*evtID]->Draw("LEGO");
	Tracker3YPulseHeight[*evtID]->SetNpx(128);
	Tracker3YPulseHeight[*evtID]->SetNpy(6);	
	Tracker3YPulseHeight[*evtID]->SetTitle("GEM3Y");
	Tracker3YPulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	Tracker3YPulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//Tracker3YPulseHeight[*evtID]->GetXaxis()->SetRangeUser(128,255);
	Tracker3YPulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	Tracker3YPulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	Tracker3YPulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	Tracker3YPulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	Tracker3YPulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
 
	PHCanvas[*evtID]->cd(22);
	Tracker4XPulseHeight[*evtID]->SetMarkerStyle(20);
	Tracker4XPulseHeight[*evtID]->Draw("LEGO");
	Tracker4XPulseHeight[*evtID]->SetNpx(128);
	Tracker4XPulseHeight[*evtID]->SetNpy(6);	
	Tracker4XPulseHeight[*evtID]->SetTitle("GEM4X");
	Tracker4XPulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	Tracker4XPulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//Tracker4XPulseHeight[*evtID]->GetXaxis()->SetRangeUser(0,127);
	Tracker4XPulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	Tracker4XPulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	Tracker4XPulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	Tracker4XPulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	Tracker4XPulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
 
	PHCanvas[*evtID]->cd(23);
	Tracker4YPulseHeight[*evtID]->SetMarkerStyle(20);
	Tracker4YPulseHeight[*evtID]->Draw("LEGO");
	Tracker4YPulseHeight[*evtID]->SetNpx(128);
	Tracker4YPulseHeight[*evtID]->SetNpy(6);	
	Tracker4YPulseHeight[*evtID]->SetTitle("GEM4Y");
	Tracker4YPulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	Tracker4YPulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//Tracker4YPulseHeight[*evtID]->GetXaxis()->SetRangeUser(128,255);
	Tracker4YPulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	Tracker4YPulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	Tracker4YPulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	Tracker4YPulseHeight[*evtID]->GetZaxis()->CenterTitle(true);	
	Tracker4YPulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
	  
	//Now we can add the LAGD APVs
	PHCanvas[*evtID]->cd(10);
	APV3E1PulseHeight[*evtID]->SetMarkerStyle(20);
	APV3E1PulseHeight[*evtID]->Draw("LEGO");
	APV3E1PulseHeight[*evtID]->SetNpx(128);
	APV3E1PulseHeight[*evtID]->SetNpy(6);	
	APV3E1PulseHeight[*evtID]->SetTitle("APV3 E1");
	APV3E1PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV3E1PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//APV3E1PulseHeight[*evtID]->GetXaxis()->SetRangeUser(0,63);
	APV3E1PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV3E1PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV3E1PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV3E1PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV3E1PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(11);
	APV3E2PulseHeight[*evtID]->SetMarkerStyle(20);
	APV3E2PulseHeight[*evtID]->Draw("LEGO");
	APV3E2PulseHeight[*evtID]->SetNpx(128);
	APV3E2PulseHeight[*evtID]->SetNpy(6);	
	APV3E2PulseHeight[*evtID]->SetTitle("APV3 E2");
	APV3E2PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV3E2PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV3E2PulseHeight[*evtID]->GetXaxis()->SetRangeUser(64,127);
	APV3E2PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV3E2PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV3E2PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV3E2PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV3E2PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(12);
	APV6E3PulseHeight[*evtID]->SetMarkerStyle(20);
	APV6E3PulseHeight[*evtID]->Draw("LEGO");
	APV6E3PulseHeight[*evtID]->SetNpx(128);
	APV6E3PulseHeight[*evtID]->SetNpy(6);	
	APV6E3PulseHeight[*evtID]->SetTitle("APV6 E3");
	APV6E3PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	//APV6E3PulseHeight[*evtID]->GetXaxis()->SetRangeUser(0,63);
	APV6E3PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV6E3PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV6E3PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV6E3PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV6E3PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV6E3PulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
 
	PHCanvas[*evtID]->cd(13);
	APV5E3PulseHeight[*evtID]->SetMarkerStyle(20);
	APV5E3PulseHeight[*evtID]->Draw("LEGO");
	APV5E3PulseHeight[*evtID]->SetNpx(128);
	APV5E3PulseHeight[*evtID]->SetNpy(6);	
	APV5E3PulseHeight[*evtID]->SetTitle("APV5 E3");
	APV5E3PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	//APV5E3PulseHeight[*evtID]->GetXaxis()->SetRangeUser(0,63);
	APV5E3PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV5E3PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV5E3PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV5E3PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV5E3PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV5E3PulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(14);
	APV4E3PulseHeight[*evtID]->SetMarkerStyle(20);
	APV4E3PulseHeight[*evtID]->Draw("LEGO");
	APV4E3PulseHeight[*evtID]->SetNpx(128);
	APV4E3PulseHeight[*evtID]->SetNpy(6);	
	APV4E3PulseHeight[*evtID]->SetTitle("APV4 E3");
	APV4E3PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	//APV4E3PulseHeight[*evtID]->GetXaxis()->SetRangeUser(0,63);
	APV4E3PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV4E3PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV4E3PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV4E3PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV4E3PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV4E3PulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(17);
	APV7E1PulseHeight[*evtID]->SetMarkerStyle(20);
	APV7E1PulseHeight[*evtID]->Draw("LEGO");
	APV7E1PulseHeight[*evtID]->SetNpx(128);
	APV7E1PulseHeight[*evtID]->SetNpy(6);	
	APV7E1PulseHeight[*evtID]->SetTitle("APV7 E1");
	APV7E1PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV7E1PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//APV7E1PulseHeight[*evtID]->GetXaxis()->SetRangeUser(0,63);
	APV7E1PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV7E1PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV7E1PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV7E1PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV7E1PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(18);
	APV7E2PulseHeight[*evtID]->SetMarkerStyle(20);
	APV7E2PulseHeight[*evtID]->Draw("LEGO");
	APV7E2PulseHeight[*evtID]->SetNpx(128);
	APV7E2PulseHeight[*evtID]->SetNpy(6);	
	APV7E2PulseHeight[*evtID]->SetTitle("APV7 E2");
	APV7E2PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV7E2PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	//APV7E2PulseHeight[*evtID]->GetXaxis()->SetRangeUser(64,127);
	APV7E2PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV7E2PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV7E2PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV7E2PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV7E2PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(19);
	APV6E4PulseHeight[*evtID]->SetMarkerStyle(20);
	APV6E4PulseHeight[*evtID]->Draw("LEGO");
	APV6E4PulseHeight[*evtID]->SetNpx(128);
	APV6E4PulseHeight[*evtID]->SetNpy(6);	
	APV6E4PulseHeight[*evtID]->SetTitle("APV6 E4");
	APV6E4PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	//APV6E4PulseHeight[*evtID]->GetXaxis()->SetRangeUser(64,127);
	APV6E4PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV6E4PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV6E4PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV6E4PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV6E4PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV6E4PulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
 
	PHCanvas[*evtID]->cd(20);
	APV5E4PulseHeight[*evtID]->SetMarkerStyle(20);
	APV5E4PulseHeight[*evtID]->Draw("LEGO");
	APV5E4PulseHeight[*evtID]->SetNpx(128);
	APV5E4PulseHeight[*evtID]->SetNpy(6);	
	APV5E4PulseHeight[*evtID]->SetTitle("APV5 E4");
	APV5E4PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	//APV5E4PulseHeight[*evtID]->GetXaxis()->SetRangeUser(64,127);
	APV5E4PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV5E4PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV5E4PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV5E4PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV5E4PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV5E4PulseHeight[*evtID]->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(21);
	APV4E4PulseHeight[*evtID]->SetMarkerStyle(20);
	APV4E4PulseHeight[*evtID]->Draw("LEGO");
	APV4E4PulseHeight[*evtID]->SetNpx(128);
	APV4E4PulseHeight[*evtID]->SetNpy(6);	
	APV4E4PulseHeight[*evtID]->SetTitle("APV4 E4");
	APV4E4PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	//APV4E4PulseHeight[*evtID]->GetXaxis()->SetRangeUser(64,127);
	APV4E4PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV4E4PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV4E4PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV4E4PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV4E4PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV4E4PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);

	PHCanvas[*evtID]->cd(24);
	APV8and9PulseHeight[*evtID]->SetMarkerStyle(20);
	APV8and9PulseHeight[*evtID]->Draw("LEGO");
	APV8and9PulseHeight[*evtID]->SetNpx(128);
	APV8and9PulseHeight[*evtID]->SetNpy(6);	
	APV8and9PulseHeight[*evtID]->SetTitle("APVs 8 and 9");
	APV8and9PulseHeight[*evtID]->GetXaxis()->SetTitle("Strip");
	APV8and9PulseHeight[*evtID]->GetXaxis()->CenterTitle(true);
	APV8and9PulseHeight[*evtID]->GetYaxis()->SetTitle("TimeBin");
	APV8and9PulseHeight[*evtID]->GetYaxis()->CenterTitle(true);
	APV8and9PulseHeight[*evtID]->GetZaxis()->SetTitle("ADC charge");
	APV8and9PulseHeight[*evtID]->GetZaxis()->CenterTitle(true);
	APV8and9PulseHeight[*evtID]->GetZaxis()->SetRange(0,400);
      }    
      //cout << "Maybe?" << endl;
      //cout << "Hit coordinates in Tracker 1" << endl;
      //for(auto i = Tracker1hitcoords[evtype].begin(); i != Tracker1hitcoords[evtype].end(); i++)
      //cout << *i << endl;	      

      //}
      //totalEventiterator++;
    }/////////////////////////////////////////////////////////////////////////////////End of While loop over all events
  
    //cout << "Number of Events?: " << EventIterator.size() << endl;
    /*
    auto *t = new TCanvas("stripscahrges", "", 500, 1500);
      t->Divide(1,3);
      t->cd(1);
      StripsHist->Draw();
      t->cd(2);
      ClusterChargeHist->Draw();
      t->cd(3);
      StripChargeHist->Draw();
      t->Draw();  
    */
    //return;

    //float LAGDhitsnum=0;
    //float Trackerhits=0;

    //return;
    cout <<  LAGDhitsnum << "/" << Trackerhits << " = " << 100*LAGDhitsnum/Trackerhits << "% +/- " << (100/Trackerhits)*sqrt(Trackerhits*(1-LAGDhitsnum/Trackerhits)) << "%" << endl;
    //EffperHV->SetPoint(fileit, stof(evtype), 100*(LAGDhitsnum/Trackerhits));
    EfficiencyPerHV.push_back(100*LAGDhitsnum/Trackerhits);
    EfficiencyErrorPerHV.push_back((100/Trackerhits)*sqrt(Trackerhits*(1-LAGDhitsnum/Trackerhits)));
    
    fileit++;

    
    
    if(display_mode == 5){      
      cout << Tracker1hitcoords[evtype].size() << " and " << evtype << endl;
      //return;
      for(auto i = Tracker1hitcoords[evtype].begin(); i != Tracker1hitcoords[evtype].end(); i++){
	//cout << *i << endl;
	TotalTracker1hitcoords.push_back(*i);
      }
      
      for(auto i = Tracker2hitcoords[evtype].begin(); i != Tracker2hitcoords[evtype].end(); i++){	
	TotalTracker2hitcoords.push_back(*i);
      }

      for(auto i = Tracker3hitcoords[evtype].begin(); i != Tracker3hitcoords[evtype].end(); i++){	
	TotalTracker3hitcoords.push_back(*i);
      }

      for(auto i = Tracker4hitcoords[evtype].begin(); i != Tracker4hitcoords[evtype].end(); i++){	
	TotalTracker4hitcoords.push_back(*i);
      }
    }
    
    if(Clusterdistevents || Gaininfo || Efficiency){
      if(inputfiles.size() == 1 && !Efficiency){
	Clusterdist[evtype]->Draw();
	Clusterdist[evtype]->SetTitle("Strip Multiplicity Distribution of Large ZigZag Strip Sector");
	Clusterdist[evtype]->GetXaxis()->SetTitle("Strip Multiplicity");
	Clusterdist[evtype]->GetXaxis()->CenterTitle(true);
	Clusterdist[evtype]->GetYaxis()->SetTitle("Events");
	Clusterdist[evtype]->GetYaxis()->CenterTitle(true);
	Clusterdist[evtype]->SaveAs("StripMultiplicity_LargeeZigZag.png");
	return;
      }

      if(Gaininfo){
	cout << "Event num: " << *evtID << endl;
	//EventAvgCharge[evtype]->Draw();
	if(EventAvgCharge[evtype]->Integral() == 0){continue;}
	TF1 *j = new TF1("landfit", "landau", 0, 2500);
      
	EventAvgCharge[evtype]->Fit("landfit");
	cout << j->GetParameter(1) << endl;
	float meanclust = j->GetParameter(1);
	cout << "Event Avg Charge: " << meanclust << endl;	
	//return;  
	MaxChargeVec.push_back(meanclust);
	ClusterchargeErrors.push_back(j->GetParError(1));
	Maxcluster.push_back(Clusterdist[evtype]->GetMean());
	cout << Clusterdist[evtype]->GetMean() << endl;
	//return;
      }
      if(Gaininfo || Efficiency){
      ClusterErrors.push_back((Clusterdist[evtype]->GetMeanError()));
      HVsetting.push_back(stoi(evtype));
      HVErrors.push_back(0);
      }
	  
    }
    //cout << "yes?" << endl;
   
  }//////////////////////////////////////////////////////////////////////////////end of loop over files
  ResCorrFile.close();

  if(Efficiency){

    for(auto y = 0; y < EfficiencyPerHV.size(); y++){
      cout << EfficiencyPerHV.at(y) << "% +\- " << EfficiencyErrorPerHV.at(y) << "%" << endl;
      cout << "Number of empty LAGD events: " << NumNoLAGDEvent.at(y) << endl;
      cout << "Number of No LAGD events in Eff: " << NumNoLAGDEventSeen.at(y) << endl;
      
    }
    float EfficiencyPerHVArr[EfficiencyPerHV.size()];
    std::copy(EfficiencyPerHV.begin(), EfficiencyPerHV.end(), EfficiencyPerHVArr);

    float EfficiencyErrorPerHVArr[EfficiencyErrorPerHV.size()];
    std::copy(EfficiencyErrorPerHV.begin(), EfficiencyErrorPerHV.end(), EfficiencyErrorPerHVArr);

    float HVsettingArr[HVsetting.size()];
    std::copy(HVsetting.begin(), HVsetting.end(), HVsettingArr);

    float HVErrorsArr[HVErrors.size()];
    std::copy(HVErrors.begin(), HVErrors.end(), HVErrorsArr);
    
    auto EffperHV = new TGraphErrors(EfficiencyPerHV.size(),HVsettingArr,EfficiencyPerHVArr,HVErrorsArr,EfficiencyErrorPerHVArr);

    auto canvas = new TCanvas("", "", 500, 500);
    EffperHV->Draw("A*");
    EffperHV->SetTitle("LAGD Efficiency during HV scan");
    EffperHV->GetYaxis()->SetRangeUser(40, 100);
    EffperHV->GetYaxis()->SetTitle("LAGD Efficiency (%)");
    EffperHV->GetXaxis()->SetTitle("Drift Voltage (V)");

    EffperHV->Draw("A*");
    auto EffCurrentaxis = new TGaxis(3700,100,4025,100,3700/5.46,4025/5.46,510,"-");	
    EffCurrentaxis->SetTitle("Divider Current [#muA]");
    EffCurrentaxis->SetLabelFont(42);
    EffCurrentaxis->SetLabelSize(0.035);
    EffCurrentaxis->SetTitleSize(0.035);
    EffCurrentaxis->SetTitleOffset(1.5);
  
    EffCurrentaxis->Draw();

    
    return;
  }

  if(display_mode == 2 && ChargeRatio != true){

    
    auto TrackerCanvas = new TCanvas("ph", "Trackers", 1000, 1000);
    TrackerCanvas->Divide(2,2);
    TrackerCanvas->Draw();
    
    TrackerCanvas->cd(1);	
    Tracker1Hitmap->Draw("colz");
    cout << "Events in Tracker 1: " << Tracker1Hitmap->GetEntries() << endl;  
    Tracker1Hitmap->SetTitle("Tracker GEM 1 Hitmap");
    
    TrackerCanvas->cd(3);
    Tracker3Hitmap->Draw("colz");
    cout << "Events in Tracker 3: " << Tracker3Hitmap->GetEntries() << endl;  
    Tracker3Hitmap->SetTitle("Tracker GEM 3 Hitmap");
    
    TrackerCanvas->cd(2);	
    Tracker2Hitmap->Draw("colz");
    cout << "Events in Tracker 2: " << Tracker2Hitmap->GetEntries() << endl;  
    Tracker2Hitmap->SetTitle("Tracker GEM 2 Hitmap");
    
    TrackerCanvas->cd(4);
    Tracker4Hitmap->Draw("colz");
    cout << "Events in Tracker 4: " << Tracker4Hitmap->GetEntries() << endl;  
    Tracker4Hitmap->SetTitle("Tracker GEM 4 Hitmap");
      
  } 


  // DeltaTTrackersLAGD->Draw();
  // return;

  if(display_mode == 0){
    if(Clusterdistevents){
      if(inputfiles.size() > 1){
	ClusterdistAll->Draw();
	ClusterdistAll->SetTitle("Strip Multiplicity Distribution of SZZ at 1200, 217");
	ClusterdistAll->GetXaxis()->SetTitle("Strip Multiplicity");
	ClusterdistAll->GetXaxis()->CenterTitle(true);
	ClusterdistAll->GetYaxis()->SetTitle("Events");
	ClusterdistAll->GetYaxis()->CenterTitle(true);
	ClusterdistAll->SaveAs("StripMultiplicity_smallZigZag_1200217.png");
	return;
      }
    }

    if(ClusterChargeDist){
      ClusterChargeHist->Draw();
      return;
    }

    if(EventCutflow){

      
      auto j = new TCanvas("", "", 2500, 1000);
      j->Divide(6,2);
      j->cd(1);
      xHits0->Draw();
      xHits0->GetYaxis()->SetRangeUser(0, 5000);      
      j->cd(2);
      xHits1->Draw();
      xHits1->GetYaxis()->SetRangeUser(0, 5000);
      j->cd(3);
      xHits2->Draw();
      xHits2->GetYaxis()->SetRangeUser(0, 5000);
      j->cd(4);
      xHits3->Draw();
      xHits3->GetYaxis()->SetRangeUser(0, 5000);
      j->cd(5);
      xHits4->Draw();
      xHits4->GetYaxis()->SetRangeUser(0, 5000);
      j->cd(6);
      xHits5->Draw();
      xHits5->GetYaxis()->SetRangeUser(0, 5000);
      j->cd(7);
      yHits0->Draw();
      yHits0->GetYaxis()->SetRangeUser(0, 5000);      
      j->cd(8);
      yHits1->Draw();
      yHits1->GetYaxis()->SetRangeUser(0, 5000);
      j->cd(9);
      yHits2->Draw();
      yHits2->GetYaxis()->SetRangeUser(0, 5000);
      j->cd(10);
      yHits3->Draw();
      yHits3->GetYaxis()->SetRangeUser(0, 5000);
      j->cd(11);
      yHits4->Draw();
      yHits4->GetYaxis()->SetRangeUser(0, 5000);
      j->cd(12);
      yHits5->Draw();
      yHits5->GetYaxis()->SetRangeUser(0, 5000);

      j->Draw();
      
      

      auto k = new TCanvas("", "", 500, 500);
      SMloss->Draw();

      SMloss->GetXaxis()->SetBinLabel(1,"Tracker1x");
      SMloss->GetXaxis()->SetBinLabel(2,"Tracker1y");
      SMloss->GetXaxis()->SetBinLabel(3,"Tracker2x");
      SMloss->GetXaxis()->SetBinLabel(4,"Tracker2y");
      SMloss->GetXaxis()->SetBinLabel(5,"Tracker3x");
      SMloss->GetXaxis()->SetBinLabel(6,"Tracker3y");
      SMloss->GetXaxis()->SetBinLabel(7,"Tracker4x");
      SMloss->GetXaxis()->SetBinLabel(8,"Tracker4y");
      SMloss->GetXaxis()->SetBinLabel(9,"LAGD APV");
      
      k->Draw();
      

      
      double NEvtsCluster1=ClusterNumberHist->GetBinContent(2);
      double NEvtsSM2345=ClusterdistAll->GetEntries();
      double NTrackerEvtsClusternot1=EventsLostToTrackers->GetEntries();
      vector<string> Trackernames= {"Tracker 1x   & ", "And Tracker 1y  & ", "And Tracker 2x  & ", "And Tracker 2y  & ", "And Tracker 3x  & ", "And Tracker 3y  & ", "And Tracker 4x  & ", "And Tracker 4y & ", " And LAGD Cluster (1) & ", "LAGD Events with SM = 2,3,4,5 & "};
      vector<string> RowHeaders = {"\\# Clusters != 0", "\\# Clusters $\\leq$ 5", "\\# Clusters $\\leq$ 4", "\\# Clusters $\\leq$ 3", "\\# Clusters $\\leq$ 2", "\\# Clusters == 1", "SM $\\geq$ 1", "SM $\\leq$ 5", "SM $\\leq$ 4", "SM $\\leq$ 3", "SM == 2"};
      //vector<int> TrackerEventCutflowinfo;//Tracker1x Ncluster1, Tracker1x Ncluster2345, Tracker1y .., .., Tracker2x .., .., ...... Tracker4y Nclusters2345
      
      ofstream LAGDCutflowFile("LAGD_CutFlow_Info.txt");
      //LAGDCutflowFile << "Event Cut Flow Table information\n\n\n";
      LAGDCutflowFile << "\\begin{tabular}{|c|c|c|c|}" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      LAGDCutflowFile << "\\multicolumn{4}{|c|}{LAGD} \\\\" << endl;      
      LAGDCutflowFile << "\\hline" << endl;
      LAGDCutflowFile << "      Cut Reason       & Events & Relative Cut Efficiency (\\%) & Total Efficiency (\\%) \\\\" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      
      bool first = true;
      //bool second



      // for(int y = 0; y < LAGDEventCutflowinfo.size(); y++){
      // 	cout << LAGDEventCutflowinfo.at(y) << endl;
      // }

      //Form("%.2f",LAGDResM2->GetMean())
      for(int h = 0; h < LAGDEventCutflowinfo.size()+1; h++){	
	if(h == 0){
	  LAGDCutflowFile <<" Total Events  & " << totalEventiterator << "    &   " << Form("%.2f",100.0*totalEventiterator/totalEventiterator) << "    &    " <<  Form("%.2f",100.0*totalEventiterator/totalEventiterator) << "   \\\\" << endl;
	  LAGDCutflowFile <<"\\hline"<<endl;
	}
	else if(h == 1){
	  LAGDCutflowFile << RowHeaders[h-1] << "  &     " << LAGDEventCutflowinfo[h-1] << "    &   " << Form("%.2f",100.0*(LAGDEventCutflowinfo[h-1])/totalEventiterator) << "    &    " <<  Form("%.2f",100.0*(LAGDEventCutflowinfo[h-1])/totalEventiterator) << "   \\\\" << endl;
	  LAGDCutflowFile <<"\\hline"<<endl;
	}
	// else if(h == 2){
	//   LAGDCutflowFile << RowHeaders[h-1] << "  &     " << LAGDEventCutflowinfo[h-1] << "    &   " << Form("%.2f",100.0*LAGDEventCutflowinfo[h-1]/(totalEventiterator - LAGDEventCutflowinfo[h-2])) << "    &    " <<  Form("%.2f",100.0*(totalEventiterator - LAGDEventCutflowinfo[h-1])/totalEventiterator) << "   \\\\" << endl;
	//   LAGDCutflowFile <<"\\hline"<<endl;
	// }
	
	// else if(h == 8){
	//   LAGDCutflowFile << RowHeaders[h-1] << "  &     " << LAGDEventCutflowinfo[h-1] << "    &   " << Form("%.2f",100.0*LAGDEventCutflowinfo[h-1]/LAGDEventCutflowinfo[h-3]) << "    &    " <<  Form("%.2f",100.0*LAGDEventCutflowinfo[h-1]/totalEventiterator) << "   \\\\" << endl;
	//   LAGDCutflowFile <<"\\hline"<<endl;
	// }
	else {
	  LAGDCutflowFile << RowHeaders[h-1] << "  &     " << LAGDEventCutflowinfo[h-1] << "    &   " << Form("%.2f",100.0*LAGDEventCutflowinfo[h-1]/LAGDEventCutflowinfo[h-2]) << "    &    " <<  Form("%.2f",100.0*LAGDEventCutflowinfo[h-1]/totalEventiterator) << "   \\\\" << endl;
	  LAGDCutflowFile <<"\\hline"<<endl;
	}
      }

      LAGDCutflowFile << "\\end{tabular}\n\n" << endl;
      
      for(int y = 0; y < TrackerEventCutflowinfo.size(); y++){
	cout << TrackerEventCutflowinfo.at(y) << endl;
      }
      
      int nameint=0;
      LAGDCutflowFile << "\\begin{tabular}{|c|c|c|c|}" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      LAGDCutflowFile << "\\multicolumn{4}{|c|}{ Clusters = 1,2,3 } \\\\" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      LAGDCutflowFile << "      Cut Reason       & Events & Relative Cut Efficiency (\\%) & Total Efficiency (\\%) \\\\" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      for(auto r = 0; r < Trackernames.size(); r++){
	if(r == 0){
	  LAGDCutflowFile << Trackernames.at(r) << TrackerEventCutflowinfo.at(r) << "    &   " << Form("%.2f",100.0*(TrackerEventCutflowinfo.at(r))/totalEventiterator) << "    &    " <<  Form("%.2f",100.0*TrackerEventCutflowinfo.at(r)/totalEventiterator) << "   \\\\" << endl;
	  LAGDCutflowFile << "\\hline" << endl;	 
	}
	else {
	  LAGDCutflowFile << Trackernames.at(r) << TrackerEventCutflowinfo.at(r) << "    &   " << Form("%.2f",100.0*(TrackerEventCutflowinfo.at(r))/TrackerEventCutflowinfo.at(r-1)) << "    &    " <<  Form("%.2f",100.0*TrackerEventCutflowinfo.at(r)/totalEventiterator) << "   \\\\" << endl;
	  LAGDCutflowFile << "\\hline" << endl;
	}
      }

      LAGDCutflowFile << "\\end{tabular}\n\n" << endl;
      LAGDCutflowFile << "\\vspace{1cm}\n\n" << endl;


      LAGDCutflowFile << "\\begin{tabular}{|c|c|c|c|}" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      LAGDCutflowFile << "\\multicolumn{4}{|c|}{ Clusters = 1,2 } \\\\" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      LAGDCutflowFile << "      Cut Reason       & Events & Relative Cut Efficiency (\\%) & Total Efficiency (\\%) \\\\" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      for(auto r = 0; r < Trackernames.size(); r++){
	if(r == 0){
	  LAGDCutflowFile << Trackernames.at(r) << TrackerEventCutflowinfo2.at(r) << "    &   " << Form("%.2f",100.0*(TrackerEventCutflowinfo2.at(r))/totalEventiterator) << "    &    " <<  Form("%.2f",100.0*TrackerEventCutflowinfo2.at(r)/totalEventiterator) << "   \\\\" << endl;
	  LAGDCutflowFile << "\\hline" << endl;	 
	}
	else {
	  LAGDCutflowFile << Trackernames.at(r) << TrackerEventCutflowinfo2.at(r) << "    &   " << Form("%.2f",100.0*(TrackerEventCutflowinfo2.at(r))/TrackerEventCutflowinfo2.at(r-1)) << "    &    " <<  Form("%.2f",100.0*TrackerEventCutflowinfo2.at(r)/totalEventiterator) << "   \\\\" << endl;
	  LAGDCutflowFile << "\\hline" << endl;
	}
      }

      LAGDCutflowFile << "\\end{tabular}\n\n" << endl;
      LAGDCutflowFile << "\\vspace{1cm}\n\n" << endl;

      LAGDCutflowFile << "\\begin{tabular}{|c|c|c|c|}" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      LAGDCutflowFile << "\\multicolumn{4}{|c|}{ Clusters = 1 } \\\\" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      LAGDCutflowFile << "      Cut Reason       & Events & Relative Cut Efficiency (\\%) & Total Efficiency (\\%) \\\\" << endl;
      LAGDCutflowFile << "\\hline" << endl;
      for(auto r = 0; r < Trackernames.size(); r++){
	if(r == 0){
	  LAGDCutflowFile << Trackernames.at(r) << TrackerEventCutflowinfo3.at(r) << "    &   " << Form("%.2f",100.0*(TrackerEventCutflowinfo3.at(r))/totalEventiterator) << "    &    " <<  Form("%.2f",100.0*TrackerEventCutflowinfo3.at(r)/totalEventiterator) << "   \\\\" << endl;
	  LAGDCutflowFile << "\\hline" << endl;	 
	}
	else {
	  LAGDCutflowFile << Trackernames.at(r) << TrackerEventCutflowinfo3.at(r) << "    &   " << Form("%.2f",100.0*(TrackerEventCutflowinfo3.at(r))/TrackerEventCutflowinfo3.at(r-1)) << "    &    " <<  Form("%.2f",100.0*TrackerEventCutflowinfo3.at(r)/totalEventiterator) << "   \\\\" << endl;
	  LAGDCutflowFile << "\\hline" << endl;
	}
      }

      LAGDCutflowFile << "\\end{tabular}\n\n" << endl;
      LAGDCutflowFile << "\\vspace{1cm}\n\n" << endl;
      
      
      
      LAGDCutflowFile.close();


      
      auto t = new TCanvas("", "LAGD Cut flow Plots", 1500, 500);
      t->Divide(3,1);
      t->cd(1);//N hits per 100,000 evts
      ClusterNumberHist->Draw();

      TLatex latexTA1;
      latexTA1.SetTextSize(0.035);
      latexTA1.SetTextAlign(13);  //align at top
      //latexTA1.SetNDC();
      for(int u = 1; u <=5; u++){
	latexTA1.DrawLatex(u-1.4,ClusterNumberHist->GetBinContent(u)+200,(to_string(int(ClusterNumberHist->GetBinContent(u)))).c_str());
      }
      /*
	latexTA1.DrawLatex(0.6,ClusterNumberHist->GetBinContent(2)+200,(to_string(int(ClusterNumberHist->GetBinContent(2)))).c_str());
      latexTA1.DrawLatex(1.8,ClusterNumberHist->GetBinContent(3)+200,(to_string(int(ClusterNumberHist->GetBinContent(3)))).c_str());
      latexTA1.DrawLatex(2.8,ClusterNumberHist->GetBinContent(4)+200,(to_string(int(ClusterNumberHist->GetBinContent(4)))).c_str());
      latexTA1.DrawLatex(3.8,ClusterNumberHist->GetBinContent(5)+200,(to_string(int(ClusterNumberHist->GetBinContent(5)))).c_str());
      */
      for(auto i = 1; i < 11; i++){
	cout << ClusterNumberHist->GetBinContent(i) << " events with " << i-1 << " clusters" << endl;
      }
      t->cd(2);
      EventsLostToTrackers->Draw();
      EventsLostToTrackers->GetXaxis()->SetBinLabel(1,"Tracker Clust Num loss");
      EventsLostToTrackers->GetXaxis()->SetBinLabel(2,"SM constraint");

      TLatex latexTA2;
      latexTA2.SetTextSize(0.035);
      latexTA2.SetTextAlign(13);  //align at top
      latexTA2.DrawLatex(0.6,1.1*(EventsLostToTrackers->GetBinContent(1)),("0 hits: "+to_string(numEventsWith0hitclusterintrackers)+"- >1 hits: "+to_string(numEventsWith2ormorehitclusterintrackers)).c_str());

    
      cout << EventsLostToTrackers->GetBinContent(1) << " lost events due to Trackers" << endl;
      cout << EventsLostToTrackers->GetBinContent(2) << " lost events due to SM constraints" << endl;

      
      
      t->cd(3);//SM Dist
      ClusterdistAll->Draw();
      for(auto i = 1; i < 11; i++){
	cout << ClusterdistAll->GetBinContent(i) << " events with SM: " << i-1 << " " << endl;
      }

      t->Draw();
      return;
      
    }
  }

  
  //return;
  
  //if(Stripcorr->GetN() == 0){cout << "No events :(" << endl;}
  //else{
  /*
  auto h = new TCanvas("", "", 1000, 500);
  h->Divide(2,1);
  h->cd(1);
  Stripcorr->Draw("A*");
  Stripcorr->SetTitle("Correlation between Y coordinate of LAGD and Tracker Hits");
  Stripcorr->GetXaxis()->SetTitle("LAGD hit location [strip]");
  Stripcorr->GetYaxis()->SetTitle("Tracker 1y hit location [strip]");
    
  h->cd(2);
  TrackerStripcorr->Draw("A*");  
  TrackerStripcorr->SetTitle("Correlation between Y coordinate of 2 Tracker's Hits");
  TrackerStripcorr->GetXaxis()->SetTitle("Tracker 1 Y hit location [strip]");
  TrackerStripcorr->GetYaxis()->SetTitle("Tracker 3 Y hit location [strip]");
    
  h->Draw();
  auto filename = inputfiles["0"];
  filename += ".png";
  h->SaveAs(filename);
  
  cout << "Graph 1 totalevents: " << Stripcorr->GetN() << endl;
  cout << "Graph 2 totalevents: " << TrackerStripcorr->GetN() << endl;
  return;
  */

  //this part checks to make sure the experimental sector is mapped accurately with tracker 1. note that this can be changed to other detectors above

  if(display_mode == 5){


    //error message for bad files:
    // cout << "Files that potentially are corrupted: " << endl;
    // for(auto y = BadFiles.begin(); y != BadFiles.end(); y++){
    //   cout << *y << ", ";
      
    // }    
    // cout << endl;
    // cout << "Corrupted events: " << endl;
    // for(auto y = Badevents.begin(); y != Badevents.end(); y++){
    //   cout << *y << ", ";
      
    //}
    
    // auto k = new TCanvas("", "", 500, 500);
    // SMloss->Draw();

    // SMloss->GetXaxis()->SetBinLabel(1,"Tracker1x");
    // SMloss->GetXaxis()->SetBinLabel(2,"Tracker1y");
    // SMloss->GetXaxis()->SetBinLabel(3,"Tracker2x");
    // SMloss->GetXaxis()->SetBinLabel(4,"Tracker2y");
    // SMloss->GetXaxis()->SetBinLabel(5,"Tracker3x");
    // SMloss->GetXaxis()->SetBinLabel(6,"Tracker3y");
    // SMloss->GetXaxis()->SetBinLabel(7,"Tracker4x");
    // SMloss->GetXaxis()->SetBinLabel(8,"Tracker4y");
    // SMloss->GetXaxis()->SetBinLabel(9,"LAGD APV");
      
    // k->Draw();


    // auto y = new TCanvas("", "", 2000, 1500);
    // y->Divide(4,3);
    // y->cd(1);
    // SM1x->Draw();
    // y->cd(2);
    // SM2x->Draw();
    // y->cd(3);
    // SM3x->Draw();
    // y->cd(4);
    // SM4x->Draw();
    // y->cd(5);
    // SM1y->Draw();
    // y->cd(6);
    // SM2y->Draw();
    // y->cd(7);
    // SM3y->Draw();
    // y->cd(8);
    // SM4y->Draw();
    // y->cd(9);
    // SMLAGD->Draw();

    // y->Draw();
    
    
    cout << "A" << endl;
    TF1* Stripfit = new TF1("stripfit","pol1");
    //Stripcorr->Fit("stripfit","Q");
    auto r = Stripcorr->Fit("stripfit", "S");
    TMatrixD cor = r->GetCorrelationMatrix();
    TMatrixD cov = r->GetCovarianceMatrix();
    cor.Print();
    cov.Print();
    /*
      TVirtualFitter *fitter = TVirtualFitter::GetFitter();
      //TMatrixDSym cov;
      cor.Use(fitter->GetNumberTotalParameters(),fitter->GetCovarianceMatrix());
      TVectorD var = TMatrixDDiag(cor);
      var.Print();
    */
    auto striptrackercor=cor(1,0);//Stripfit->GetCorrelationFactor(); 
    auto striptrackercorsign = striptrackercor/abs(striptrackercor);
    cout << "B" << endl;
    //cout << striptrackercor << ", " << striptrackercorsign << endl;
  
    TF1* Trackerfit = new TF1("trackfit","pol1");
    //Stripcorr->Fit("stripfit","Q");
    TFitResultPtr r2 = TrackerStripcorr->Fit("trackfit", "S");
    TMatrixD cor2 = r2->GetCorrelationMatrix();
    TMatrixD cov2 = r2->GetCovarianceMatrix();
    cor2.Print();
    cov2.Print();
    cout << "C" << endl;
    auto striptrackercor2 = cor2(1,0);
    auto striptrackercorsign2 = striptrackercor2/abs(striptrackercor2);

  
    //cout << striptrackercor2 << ", " << striptrackercorsign2 << endl;

    cout << "LAGD correlation with tracker 1y: " << abs(striptrackercor) << ", " << striptrackercorsign << endl;
    cout << "Tracker 1y correlation with Tracker 3y: " << abs(striptrackercor2) << ", " << striptrackercorsign2 << endl;
    
    bool showplot=false;

    if(abs(striptrackercor) < 0.85 || abs(striptrackercor2) < 0.85 || striptrackercorsign != striptrackercorsign2 || showplot){
      cout << "Mapping problem!!!! Consult the manual" << endl;

      auto h = new TCanvas("", "", 2000, 1000);
      h->Divide(2,2);
      h->cd(1);
      Stripcorr->Draw("colz");//AP
      //Stripcorr->SetNpx(200);
      //Stripcorr->SetNpx(200);      
      Stripcorr->SetTitle("Correlation between Y coordinate of LAGD and Tracker Hits");
      Stripcorr->GetYaxis()->SetTitle("LAGD hit location [strip]");
      Stripcorr->GetXaxis()->SetTitle("Tracker 1y hit location [strip]");
    
      h->cd(2);
      TrackerStripcorr->Draw("colz");//AP
      //TrackerStripcorr->SetNpx(200);
      //TrackerStripcorr->SetNpx(200);
      //gPad->SetTheta(90); // default is 30
      //gPad->SetPhi(0); // default is 30
      //gPad->Update();
      TrackerStripcorr->SetTitle("Correlation between Y coordinate of 2 Tracker's Hits");
      TrackerStripcorr->GetXaxis()->SetTitle("Tracker 1 Y hit location [strip]");
      TrackerStripcorr->GetYaxis()->SetTitle("Tracker 3 Y hit location [strip]");

      h->cd(3);
      StripcorrProf->Draw();

      h->cd(4);
      TrackerStripcorrProf->Draw();
      
      h->Draw();
      auto filename = inputfiles["0"];
      filename += ".png";
      h->SaveAs(filename);
      cout << lostcounter << " Lost events due to MHR" << endl;
      //cout << "Graph 1 totalevents: " << Stripcorr->GetN() << endl;
      //cout << "Graph 2 totalevents: " << TrackerStripcorr->GetN() << endl;


      for(int y = 0; y < 10; y++){
	cout << StripcorrProf->GetBinError(y) << endl;
      }
      for(int y = 0; y < 10; y++){
	cout << TrackerStripcorrProf->GetBinError(y) << endl;
      }
      return;
    }
  }	
  
  
    //}
  
  if(display_mode == 2 && ChargeRatio){
    T1ChargeRatio->Draw("colz");
    //T1ChargeRatio->SetTitle("APV4 E4");
    T1ChargeRatio->GetXaxis()->SetTitle("Tracker 1x ADC Charge");
    T1ChargeRatio->GetXaxis()->CenterTitle(true);
    T1ChargeRatio->GetYaxis()->SetTitle("Tracker 1y ADC Charge");
    T1ChargeRatio->GetYaxis()->CenterTitle(true);
    return;
  }
  //Tracker2Ystriphits->Draw();
  /*
  auto h = new TCanvas("", "", 1000, 500);
  h->Divide(2,1);
  h->cd(1);
  Tracker2Ystriphits->Draw();
  Tracker2Ystriphits->SetLineColor(2);
  Tracker2Ystriphits->SetTitle("Cluster Hit distribution in APV3E1");
  
  h->cd(2);
  StripsHithist->Draw();
  StripsHithist->SetTitle("Stip hit distribution in Tracker APV3E1");
  h->Draw();
  return;
  */
  if(display_mode == 5){


    //auto CorrectedLAGDHits = ResidualCorrection(LAGDHitsVec);
    
    //auto BadEvents = ResCut(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords);
    vector<float>  BadEvents;
    //return;
    vector<float> TrackerOptimizedAnglesAndXY;
    float Tracker2x1stval = TotalTracker2hitcoords.at(0);
    //auto TrackerOptimizedAnglesAndXY = Optimized_Rotation(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords, TrackerOptimizedOffsets, 1);

    //cout << "Next" << endl;
    //return;
    auto TrackerOptimizedOffsets = Optimized_XY_shift(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords, TrackerOptimizedAnglesAndXY, BadEvents, 0); //returns vector of XY Offsets for trackers 1-4 that minimize the residual)

    //return;

    // TrackerOptimizedAnglesAndXY = Optimized_Rotation(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords, TrackerOptimizedOffsets, BadEvents, 0);
    // cout << "Rotation and XY shift" << endl;
    // for(auto i = TrackerOptimizedAnglesAndXY.begin(); i < TrackerOptimizedAnglesAndXY.end(); i++){
    //   cout << *i << endl;
    // }
    
    
    //return;
    vector<float> FinalOptimizedOffsets;
    FinalOptimizedOffsets = Optimized_XY_Rotation(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords, TrackerOptimizedOffsets, BadEvents, 0);
    cout << "Optimized rotation" << endl;
    for(auto i = FinalOptimizedOffsets.begin(); i < FinalOptimizedOffsets.end(); i++){
      cout << *i << endl;
    }
    
    //return;
    Plot_Aligned_residuals(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords, FinalOptimizedOffsets, BadEvents, LAGDHitsVec, SMofTheseevents, Clusterchargeoftheseevnts, EventIDs, 1);//comehere
    cout << "Tracker2x first hitcoordinate pre shift: " << Tracker2x1stval << endl;
  }

    
  if(display_mode == 0){
   
  
    float HVsettings[HVsetting.size()];
    std::copy(HVsetting.begin(), HVsetting.end(), HVsettings);
    float fixedHVErrors[HVErrors.size()];
    std::copy(HVErrors.begin(), HVErrors.end(), fixedHVErrors);

    vector<float> MeansperHVsetting;
    float previousHVsetting = HVsetting[0];

    if(Clusterdistevents){
      int k=0;
      cout << "Max SM per HV " << endl;
      for(auto i = Maxcluster.begin(); i != Maxcluster.end(); i++){
	cout << *i <<  " +/- " << ClusterErrors[k] << ", " << HVsetting[k] << "(" << k << ")" << endl;
	k++;
	
	//if(HVsetting[k] != previousHVsetting)
	  
      }

      float MaxClusters[Maxcluster.size()];
      std::copy(Maxcluster.begin(), Maxcluster.end(), MaxClusters);
      float fixedClusterErrors[ClusterErrors.size()];
      std::copy(ClusterErrors.begin(), ClusterErrors.end(), fixedClusterErrors);
    

    
      auto CVSHVCanvas = new TCanvas("CVSHV", "", 800, 500);
      CVSHVCanvas->SetGrid();
      auto ClustervsHV = new TGraphErrors(HVsetting.size(), HVsettings, MaxClusters, fixedHVErrors, fixedClusterErrors);
      //ClustervsHV->Fit("expo");
      //ClustervsHV->Fit("pol1");
      ClustervsHV->Draw("A*");

      //char title="Strip Multiplicity versus HV Setting in ";
      //title = title+apvnum;
 
      ClustervsHV->SetTitle("Strip Multiplicity versus HV Setting in Straight Strip Sector");
      ClustervsHV->GetXaxis()->SetTitle("HV Setting (V)");
      ClustervsHV->GetXaxis()->CenterTitle(true);
      ClustervsHV->GetYaxis()->SetTitle("Strip Multiplicity");
      ClustervsHV->GetYaxis()->CenterTitle(true);
      ClustervsHV->GetYaxis()->SetRangeUser(2,3);
      ClustervsHV->SaveAs("StripMult_Vs_HV_APV3.png");
    }
    
    if(Gaininfo){
      
      int k=0;
      cout << "Average ADC charge per HV " << endl;  
      for(auto i = MaxChargeVec.begin(); i != MaxChargeVec.end(); i++){    
	cout << *i <<  ", " << HVsetting[k] << endl;
	k++;
      }

      
      float MaxChargeVector[MaxChargeVec.size()];
      std::copy(MaxChargeVec.begin(), MaxChargeVec.end(), MaxChargeVector);
      float fixedClusterchargeErrors[ClusterchargeErrors.size()];
      std::copy(ClusterchargeErrors.begin(), ClusterchargeErrors.end(), fixedClusterchargeErrors);
    
      auto GVSHVCanvas = new TCanvas("GVSHV", "", 800, 500);
      auto GainvsHV = new TGraphErrors(HVsetting.size(), HVsettings, MaxChargeVector,fixedHVErrors, fixedClusterchargeErrors);
      //GainvsHV->Fit("expo");
      GainvsHV->Draw("A*");
      GainvsHV->SetTitle("Average Hit Charge versus HV Setting in Straight Strip Sector");
      GainvsHV->GetXaxis()->SetTitle("HV Setting (V)");
      GainvsHV->GetXaxis()->CenterTitle(true);
      GainvsHV->GetYaxis()->SetTitle("Charge");
      GainvsHV->GetYaxis()->CenterTitle(true);
      GainvsHV->SaveAs("Gain_Vs_HV_APV3.png");
    }
  }
}

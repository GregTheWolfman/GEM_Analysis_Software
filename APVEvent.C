#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "TH1D.h"
#include "TH2D.h"
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
#include "FNAL-Beam-Test-Scripts/doubleGausFit_withHistParameter.C"
#include <TLatex.h>
#include <unistd.h>
#include <TMath.h>

//#include <gSystem.h>
//#include <gPad.h>
//#include "Optimized_XY_shift.C"

//#include <bits/stdc++.h>

//vector<float> FitParabola (TGraph);
void Plot_Aligned_residuals (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, bool);
vector<float> Optimized_XY_Rotation (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, bool);
vector<float> Optimized_Rotation (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, bool);
vector<float> Optimized_XY_shift (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, bool);
vector<float> Residuals (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>);
vector<float> Residuals (vector<float>, vector<float>, vector<float>, vector<float>);
vector<float> ResCut (vector<float>&, vector<float>&, vector<float>&, vector<float>&);
vector<float> ChiSquared (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>);
vector<float> ChiSquared (vector<float>, vector<float>, vector<float>, vector<float>);
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


//this function caluclates the angle between hits wrt a common origin
double CalculateCosTheta1(double x, double y, double x1,double y1){
  //double cosineTheta = ( x*x1 + y*y1 )/(x*x+y*y);
  double sineTheta = (x1*y - x*y1)/(x*x + y*y);
  //if(cosineTheta>1) cosineTheta=2-cosineTheta;
  //cout<<sineTheta<<endl;
  return asin(sineTheta);
}


//This function is for the final step of alignment, apply optimized offsets, save new hit coordinates to an output file and plot the residuals
void Plot_Aligned_residuals(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> OptimizedXYOffests, vector<float> BadEvents, vector<float> LAGDHits, vector<float> SMvec,  vector<float> CCvec,  vector<float> Eventidvec, bool plots){

  ofstream MyFile("Trackers_Hits_swappedtrackers.txt");    
  MyFile << "#PedFile: GEMandTrksPed_pedestaloptionfirst_dataTree01.root \n";
  MyFile << "#DataFiles: APV3_ADC_0_dataTree01.root \n";
  MyFile << "#Tracker1x, Tracker1y, Tracker2x, Tracker2y, Tracker3x, Tracker3y, Tracker4x, Tracker4y \n";


  ofstream LAGDhits("LAGD_hits_swappedtrackers.txt");
 
  
  LAGDhits << "#PedFile: GEMandTrksPed_pedestaloptionfirst_dataTree01.root \n";
  LAGDhits << "#DataFiles: APV3_ADC_0_dataTree01.root, APV3_ADC_1_dataTree01.root, APV3_ADC_2_dataTree01.root \n";
  LAGDhits << "#EventID, AVP3E1" << endl;

  
  vector<float> T1Hitcoords;
  vector<float> T2Hitcoords;
  vector<float> T3Hitcoords;
  vector<float> T4Hitcoords;
  vector<float> LAGDHitcoords;
  vector<float> FixedSMvec;
  vector<float> FixedCCvec;
  vector<float> FixedEvtidvec;
  
  cout << BadEvents.size() << endl;
  //now, actually remove the bad events from the final list of tracker/LAGD hits to go onto alignment
  cout << Tracker1HitlocVec.size() << endl;
  cout << LAGDHits.size() << endl;
  
  cout << "Total Events with bad events in **" << endl;

  auto SMdist = new TH1F("","Strip Multiplicity Distribution for removed Events; Number of strips involved; Frequency",5, 0.5,5.5);
  auto CCdist = new TH1F("","Cluster Charge Distribution for removed Events; Cluster charge [a.u.]; frequency",500, 0,2500);
  auto Angdist = new TH1F("","Hit Distribution for removed Events; Angle [#phi]; frequency",50, 0, 0.4);

  auto SMdistgd = new TH1F("","Strip Multiplicity Distribution for Accepted Events; Number of strips involved; Frequency",5, 0.5,5.5);
  auto CCdistgd = new TH1F("","Cluster Charge Distribution for Accepted Events; Cluster charge [a.u.]; frequency",500, 0,2500);
  auto Angdistgd = new TH1F("","Hit Distribution for Accepted Events; Angle [#phi]; frequency",50, 0, 0.4);

  auto Hitmap1 = new TH2F("", "Beam Spot on Tracker 1", 100, 0, 50, 100, 0, 50);
  auto Hitmap2 = new TH2F("", "Beam Spot on Tracker 2", 100, 0, 50, 100, 0, 50);
  
  for(auto i = 0; i < Tracker1HitlocVec.size();i++){
    //cout << i/3 << endl;
    if(find(BadEvents.begin(), BadEvents.end(), i/3) == BadEvents.end()){
      //cout << "ttackers" << endl;
      T1Hitcoords.push_back(Tracker1HitlocVec.at(i));T1Hitcoords.push_back(Tracker1HitlocVec.at(i+1));T1Hitcoords.push_back(Tracker1HitlocVec.at(i+2));
      T2Hitcoords.push_back(Tracker2HitlocVec.at(i));T2Hitcoords.push_back(Tracker2HitlocVec.at(i+1));T2Hitcoords.push_back(Tracker2HitlocVec.at(i+2));
      T3Hitcoords.push_back(Tracker3HitlocVec.at(i));T3Hitcoords.push_back(Tracker3HitlocVec.at(i+1));T3Hitcoords.push_back(Tracker3HitlocVec.at(i+2));
      T4Hitcoords.push_back(Tracker4HitlocVec.at(i));T4Hitcoords.push_back(Tracker4HitlocVec.at(i+1));T4Hitcoords.push_back(Tracker4HitlocVec.at(i+2));

      Hitmap1->Fill(Tracker1HitlocVec.at(i), Tracker1HitlocVec.at(i+1));
      Hitmap2->Fill(Tracker2HitlocVec.at(i), Tracker2HitlocVec.at(i+1));
      
      //cout << "LAGD" << endl;
      LAGDhits << LAGDHits.at(i/3) << endl;
      LAGDHitcoords.push_back(LAGDHits.at(i/3));
      //cout << "LAGD2" << endl;
     
      cout << SMvec.at(i/3) << endl;
      FixedSMvec.push_back(SMvec.at(i/3));
      SMdist->Fill(SMvec.at(i/3));
  
      FixedCCvec.push_back(CCvec.at(i/3));
      CCdist->Fill(CCvec.at(i/3));
      //cout << Tracker1HitlocVec.at(i) << endl;      
      //cout << Tracker1HitlocVec.at(i+1) << endl;
      //cout << Tracker1HitlocVec.at(i+2) << endl;
      cout << LAGDHits.at(i/3) << endl;
      
      Angdist->Fill(LAGDHits.at(i/3)*0.0041);
      i++;
      i++;
      //cout << i << "?" << endl;
    }
    else{     
      cout << SMvec.at(i/3) << endl;
      FixedSMvec.push_back(SMvec.at(i/3));
      SMdistgd->Fill(SMvec.at(i/3));
  
      FixedCCvec.push_back(CCvec.at(i/3));
      CCdistgd->Fill(CCvec.at(i/3));

      Angdistgd->Fill(LAGDHits.at(i/3)*0.0041);
      i++;
      i++;
    }
  }
  /*
  auto h = new TCanvas("","", 1500, 500);
  h->Divide(3,1);
  h->cd(1);
  SMdist->Draw();
  h->cd(2);
  CCdist->Draw();
  h->cd(3);
  Angdist->Draw();

  h->Draw();
  */
  
  auto h = new TCanvas("","", 1000, 500);
  h->Divide(2,1);
  h->cd(1);
  Hitmap1->Draw("colz");
  h->cd(2);
  Hitmap2->Draw("colz");
  

  h->Draw();
  return;
  /*
  auto hi = new TCanvas("","", 1500, 500);
  hi->Divide(3,1);
  hi->cd(1);
  SMdistgd->Draw();
  hi->cd(2);
  CCdistgd->Draw();
  hi->cd(3);
  Angdistgd->Draw();

  hi->Draw();
  */
  /*
  int k=0;
  cout << "passed first part" << endl;
  cout << LAGDHits.size() << endl;
  cout << Tracker1HitlocVec.size() << endl;
  int i =0;
  while(i<Tracker1HitlocVec.size()){
    //cout << k++ << endl;
    cout << "Evnt num: " << i << endl;
    if(find(BadEvents.begin(), BadEvents.end(), i/3) != BadEvents.end()){//if the current event (i/3) IS in the vector of bad events,....
      cout << "k: " << k++ << endl;
      Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i-1, Tracker1HitlocVec.begin()+i+1);
      Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i-1, Tracker2HitlocVec.begin()+i+1);
      Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i-1, Tracker3HitlocVec.begin()+i+1);
      Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i-1, Tracker4HitlocVec.begin()+i+1);
      LAGDHits.erase(LAGDHits.begin()+i-1);
      cout << "Goodbye event: " << i/3 << endl;
    }
    if(find(BadEvents.begin(), BadEvents.end(), i/3) == BadEvents.end()){
      cout << "k: " << k++ << endl;      
      i++;
      i++;
      i++;
    }
    else{
      i++;
      i++;
      i++;
    }
  }
    
  */

  //return;

  


  //cout << "passed weird part" << endl;
  
 
  float Shift_T1x = OptimizedXYOffests[0];
  float Shift_T1y = OptimizedXYOffests[1];
  float Shift_T2x = OptimizedXYOffests[2];
  float Shift_T2y = OptimizedXYOffests[3];
  float Shift_T3x = OptimizedXYOffests[4];
  float Shift_T3y = OptimizedXYOffests[5];
  float Shift_T4x = OptimizedXYOffests[6];
  float Shift_T4y = OptimizedXYOffests[7];

  float angleT2=0;
  float angleT3=0;
  float angleT4=0;
  
  if(OptimizedXYOffests.size() > 8){
    float angleT2 = OptimizedXYOffests[8];
    float angleT3 = OptimizedXYOffests[9];
    float angleT4 = OptimizedXYOffests[10];
  }

  
  float T1Hitcoordsx_tmp=0;
  float T1Hitcoordsy_tmp=0;
  float T2Hitcoordsx_tmp=0;
  float T2Hitcoordsy_tmp=0;
  float T3Hitcoordsx_tmp=0;
  float T3Hitcoordsy_tmp=0;
  float T4Hitcoordsx_tmp=0;
  float T4Hitcoordsy_tmp=0;
  

  char nameRes2X[40];sprintf(nameRes2X,"PostTrackerAlignment_residual_Tracker2x");char nameRes2Y[40];sprintf(nameRes2Y,"PostTrackerAlignment_residual_Tracker2y");
  char nameRes3X[40];sprintf(nameRes3X,"PostTrackerAlignment_residual_Tracker3x");char nameRes3Y[40];sprintf(nameRes3Y,"PostTrackerAlignment_residual_Tracker3y");
  char nameRes1X[40];sprintf(nameRes1X,"PostTrackerAlignment_residual_Tracker1x");char nameRes1Y[40];sprintf(nameRes1Y,"PostTrackerAlignment_residual_Tracker1y");
  char nameRes4X[40];sprintf(nameRes4X,"PostTrackerAlignment_residual_Tracker4x");char nameRes4Y[40];sprintf(nameRes4Y,"PostTrackerAlignment_residual_Tracker4y");

  
  TH1F* h_residual_Tracker2x = new TH1F(nameRes2X,"",300,-0.5,0.5); h_residual_Tracker2x->SetXTitle("Residual [mm]"); h_residual_Tracker2x->SetYTitle("Frequency");h_residual_Tracker2x->SetLabelSize(0.042,"XY");h_residual_Tracker2x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker2y = new TH1F(nameRes2Y,"",300,-0.5,0.5); h_residual_Tracker2y->SetXTitle("Residual [mm]"); h_residual_Tracker2y->SetYTitle("Frequency");h_residual_Tracker2y->SetLabelSize(0.042,"XY");h_residual_Tracker2y->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker3x = new TH1F(nameRes3X,"",300,-0.5,0.5); h_residual_Tracker3x->SetXTitle("Residual [mm]"); h_residual_Tracker3x->SetYTitle("Frequency");h_residual_Tracker3x->SetLabelSize(0.042,"XY");h_residual_Tracker3x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker3y = new TH1F(nameRes3Y,"",300,-0.5,0.5); h_residual_Tracker3y->SetXTitle("Residual [mm]"); h_residual_Tracker3y->SetYTitle("Frequency");h_residual_Tracker3y->SetLabelSize(0.042,"XY");h_residual_Tracker3y->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker1x = new TH1F(nameRes1X,"",300,-0.5,0.5); h_residual_Tracker1x->SetXTitle("Residual [mm]"); h_residual_Tracker1x->SetYTitle("Frequency");h_residual_Tracker1x->SetLabelSize(0.042,"XY");h_residual_Tracker1x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker1y = new TH1F(nameRes1Y,"",300,-0.5,0.5); h_residual_Tracker1y->SetXTitle("Residual [mm]"); h_residual_Tracker1y->SetYTitle("Frequency");h_residual_Tracker1y->SetLabelSize(0.042,"XY");h_residual_Tracker1y->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker4x = new TH1F(nameRes4X,"",300,-0.5,0.5); h_residual_Tracker4x->SetXTitle("Residual [mm]"); h_residual_Tracker4x->SetYTitle("Frequency");h_residual_Tracker4x->SetLabelSize(0.042,"XY");h_residual_Tracker4x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker4y = new TH1F(nameRes4Y,"",300,-0.5,0.5); h_residual_Tracker4y->SetXTitle("Residual [mm]"); h_residual_Tracker4y->SetYTitle("Frequency");h_residual_Tracker4y->SetLabelSize(0.042,"XY");h_residual_Tracker4y->SetTitleSize(0.042,"XY");


  for(unsigned int i=0; i< T1Hitcoords.size(); i++){//shift all of the XY hit coordinates
	  
    T1Hitcoords[i] = T1Hitcoords[i] - Shift_T1x;
    //T1Hitcoordsx_tmp = T1Hitcoords[i];
    T2Hitcoords[i] = T2Hitcoords[i] - Shift_T2x;
    //T2Hitcoordsx_tmp = T2Hitcoords[i]; 
    T3Hitcoords[i] = T3Hitcoords[i] - Shift_T3x;
    //T3Hitcoordsx_tmp = T3Hitcoords[i];
    T4Hitcoords[i] = T4Hitcoords[i] - Shift_T4x;
    //T4Hitcoordsx_tmp = T4Hitcoords[i];

    i++;//move to y components
	  
    T1Hitcoords[i] = T1Hitcoords[i] - Shift_T1y;
    //T1Hitcoordsy_tmp = T1Hitcoords[i];
    T2Hitcoords[i] = T2Hitcoords[i] - Shift_T2y;
    //T2Hitcoordsy_tmp = T2Hitcoords[i];
    T3Hitcoords[i] = T3Hitcoords[i] - Shift_T3y;
    //T3Hitcoordsy_tmp = T3Hitcoords[i];
    T4Hitcoords[i] = T4Hitcoords[i] - Shift_T4y;
    //T4Hitcoordsy_tmp = T4Hitcoords[i];
     

    i++;
  }

  for(unsigned int i=0; i< T1Hitcoords.size(); i++){
      
    T2Hitcoordsx_tmp = T2Hitcoords[i]; 	
    T2Hitcoords[i] = T2Hitcoords[i]*cos(angleT2) - T2Hitcoords[i+1]*sin(angleT2);	  
      
    T3Hitcoordsx_tmp = T3Hitcoords[i]; 	
    T3Hitcoords[i] = T3Hitcoords[i]*cos(angleT3) - T3Hitcoords[i+1]*sin(angleT3);	  
    
    T4Hitcoordsx_tmp = T4Hitcoords[i]; 	
    T4Hitcoords[i] = T4Hitcoords[i]*cos(angleT4) - T4Hitcoords[i+1]*sin(angleT4);	  
      
    
    i++;//move to y components
    
    T2Hitcoords[i] = T2Hitcoordsx_tmp*sin(angleT2) + T2Hitcoords[i]*cos(angleT2);	  
    
    T3Hitcoords[i] = T3Hitcoordsx_tmp*sin(angleT3) + T3Hitcoords[i]*cos(angleT3);	  
    T4Hitcoords[i] = T4Hitcoordsx_tmp*sin(angleT4) + T4Hitcoords[i]*cos(angleT4);	  
    
    
    MyFile << T1Hitcoords[i-1] << " " << T1Hitcoords[i] << " " <<T2Hitcoords[i-1] << " " << T2Hitcoords[i] << " " <<T3Hitcoords[i-1] << " " << T3Hitcoords[i] << " " <<T4Hitcoords[i-1] << " " << T4Hitcoords[i] << endl;

   
    
    i++;         	  
  }
  
  MyFile.close();
  //cout << T1Hitcoords.size() << endl;
  //ResCut(T1Hitcoords, T2Hitcoords, T3Hitcoords, T4Hitcoords);
  //cout << T1Hitcoords.size() << endl;
  
  auto NewRes = Residuals(T1Hitcoords, T2Hitcoords, T3Hitcoords, T4Hitcoords);
  auto Chisquares = ChiSquared(T1Hitcoords, T2Hitcoords, T3Hitcoords, T4Hitcoords);
  

  for(unsigned int i=0; i< NewRes.size(); i++){//Put all residuals into plots

    h_residual_Tracker1x->Fill(NewRes[i]);	 
    i++;
    h_residual_Tracker1y->Fill(NewRes[i]);
    i++;
    h_residual_Tracker2x->Fill(NewRes[i]);
    i++;
    h_residual_Tracker2y->Fill(NewRes[i]);
    i++;
    h_residual_Tracker3x->Fill(NewRes[i]);
    i++;
    h_residual_Tracker3y->Fill(NewRes[i]);
    i++;
    h_residual_Tracker4x->Fill(NewRes[i]);
    i++;
    h_residual_Tracker4y->Fill(NewRes[i]);
  }
 
    
  TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 2000, 1000);
  TrackerRes->Divide(4,2);

  TrackerRes->cd(1);
  h_residual_Tracker1x->SetTitle("Tracker1x residual");
  h_residual_Tracker1x->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker1x->Draw();
  h_residual_Tracker1x->SaveAs("ResT1.png");
  TrackerRes->cd(5);
  h_residual_Tracker1y->SetTitle("Tracker1y residual");
  h_residual_Tracker1y->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker1y->Draw();
  TrackerRes->cd(2);
  h_residual_Tracker2x->SetTitle("Tracker2x residual");
  h_residual_Tracker2x->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker2x->Draw();
  TrackerRes->cd(6);
  h_residual_Tracker2y->SetTitle("Tracker2y residual");
  h_residual_Tracker2y->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker2y->Draw();
  TrackerRes->cd(3);
  h_residual_Tracker3x->SetTitle("Tracker3x residual");
  h_residual_Tracker3x->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker3x->Draw();
  TrackerRes->cd(7);
  h_residual_Tracker3y->SetTitle("Tracker3y residual");
  h_residual_Tracker3y->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker3y->Draw();
  TrackerRes->cd(4);
  h_residual_Tracker4x->SetTitle("Tracker4x residual");
  h_residual_Tracker4x->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker4x->Draw();
  TrackerRes->cd(8);
  h_residual_Tracker4y->SetTitle("Tracker4y residual");
  h_residual_Tracker4y->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker4y->Draw();
	  
  gPad->Update();
	  
  TrackerRes->Draw();
  gPad->Update();
  TrackerRes->SaveAs("FinalResiduals_All20FilesAllEvents.png");

}

vector<float> Optimized_XY_Rotation(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> OptimizedXYOffests, vector<float> BadEvents, bool plots){

  
  float Shift_T1x = OptimizedXYOffests[0];
  float Shift_T1y = OptimizedXYOffests[1];
  float Shift_T2x = OptimizedXYOffests[2];
  float Shift_T2y = OptimizedXYOffests[3];
  float Shift_T3x = OptimizedXYOffests[4];
  float Shift_T3y = OptimizedXYOffests[5];
  float Shift_T4x = OptimizedXYOffests[6];
  float Shift_T4y = OptimizedXYOffests[7];
  float angleT2 = OptimizedXYOffests[8];
  float angleT3 = OptimizedXYOffests[9];
  float angleT4 = OptimizedXYOffests[10];
  


  double mean_Tracker1xcoordRes=0.0, mean_Tracker1ycoordRes=0.0;
  double mean_Tracker2xcoordRes=0.0, mean_Tracker2ycoordRes=0.0;
  double mean_Tracker3xcoordRes=0.0, mean_Tracker3ycoordRes=0.0;
  double mean_Tracker4xcoordRes=0.0, mean_Tracker4ycoordRes=0.0;  
  
  int Iterator=0;
  float MeanangleT2=0.0;
  float MeanangleT3=0.0;
  float MeanangleT4=0.0;
  float TotalangleT2=0.0;
  float TotalangleT3=0.0;
  float TotalangleT4=0.0;

  float Total1Xshift=0;
  float Total1Yshift=0;
  float Total2Xshift=0;
  float Total2Yshift=0;
  float Total3Xshift=0;
  float Total3Yshift=0;
  float Total4Xshift=0;
  float Total4Yshift=0;
  
  vector<float> Optimized_Angles_And_XY;
  
  int ResIt = 5;

  
  TGraph* Residual1XPerIteration = new TGraph();Residual1XPerIteration->GetXaxis()->SetTitle("Iterations");Residual1XPerIteration->GetYaxis()->SetTitle("Residual");Residual1XPerIteration->SetTitle("Residual Per Iteration in Tracker1 x coordinate");
  TGraph* Residual1YPerIteration = new TGraph();Residual1YPerIteration->GetXaxis()->SetTitle("Iterations");Residual1YPerIteration->GetYaxis()->SetTitle("Residual");Residual1YPerIteration->SetTitle("Residual Per Iteration in Tracker1 y coordinate");
  TGraph* Residual2XPerIteration = new TGraph();Residual2XPerIteration->GetXaxis()->SetTitle("Iterations");Residual2XPerIteration->GetYaxis()->SetTitle("Residual");Residual2XPerIteration->SetTitle("Residual Per Iteration in Tracker2 x coordinate");
  TGraph* Residual2YPerIteration = new TGraph();Residual2YPerIteration->GetXaxis()->SetTitle("Iterations");Residual2YPerIteration->GetYaxis()->SetTitle("Residual");Residual2YPerIteration->SetTitle("Residual Per Iteration in Tracker2 y coordinate");
  TGraph* Residual3XPerIteration = new TGraph();Residual3XPerIteration->GetXaxis()->SetTitle("Iterations");Residual3XPerIteration->GetYaxis()->SetTitle("Residual");Residual3XPerIteration->SetTitle("Residual Per Iteration in Tracker3 x coordinate");
  TGraph* Residual3YPerIteration = new TGraph();Residual3YPerIteration->GetXaxis()->SetTitle("Iterations");Residual3YPerIteration->GetYaxis()->SetTitle("Residual");Residual3YPerIteration->SetTitle("Residual Per Iteration in Tracker3 y coordinate");
  TGraph* Residual4XPerIteration = new TGraph();Residual4XPerIteration->GetXaxis()->SetTitle("Iterations");Residual4XPerIteration->GetYaxis()->SetTitle("Residual");Residual4XPerIteration->SetTitle("Residual Per Iteration in Tracker4 x coordinate");
  TGraph* Residual4YPerIteration = new TGraph();Residual4YPerIteration->GetXaxis()->SetTitle("Iterations");Residual4YPerIteration->GetYaxis()->SetTitle("Residual");Residual4YPerIteration->SetTitle("Residual Per Iteration in Tracker4 y coordinate");

  TGraph* AngleT2PerIteration = new TGraph();AngleT2PerIteration->GetXaxis()->SetTitle("Iterations");AngleT2PerIteration->GetYaxis()->SetTitle("Angle (Rad)");AngleT2PerIteration->SetTitle("Angle rotation Per Iteration in Tracker 2");
  TGraph* AngleT3PerIteration = new TGraph();AngleT3PerIteration->GetXaxis()->SetTitle("Iterations");AngleT3PerIteration->GetYaxis()->SetTitle("Angle (Rad)");AngleT3PerIteration->SetTitle("Angle rotation Per Iteration in Tracker 3");
  TGraph* AngleT4PerIteration = new TGraph();AngleT4PerIteration->GetXaxis()->SetTitle("Iterations");AngleT4PerIteration->GetYaxis()->SetTitle("Angle (Rad)");AngleT4PerIteration->SetTitle("Angle rotation Per Iteration in Tracker 4");

  TGraph* xChi2PerIteration = new TGraph();xChi2PerIteration->GetXaxis()->SetTitle("Iteration");xChi2PerIteration->GetYaxis()->SetTitle("#chi^2");xChi2PerIteration->SetTitle("#chi^2 of track in X-Z projection plane");//xChi2PerIteration
  TGraph* yChi2PerIteration = new TGraph();yChi2PerIteration->GetXaxis()->SetTitle("Iteration");yChi2PerIteration->GetYaxis()->SetTitle("#chi^2");yChi2PerIteration->SetTitle("#chi^2 of track in Y-Z projection plane");
  
  
 

  char nameRes2X[30];sprintf(nameRes2X,"residual_Tracker2x_%i",Iterator);char nameRes2Y[30];sprintf(nameRes2Y,"residual_Tracker2y_%i",Iterator);
  char nameRes3X[30];sprintf(nameRes3X,"residual_Tracker3x_%i",Iterator);char nameRes3Y[30];sprintf(nameRes3Y,"residual_Tracker3y_%i",Iterator);
  char nameRes1X[30];sprintf(nameRes1X,"residual_Tracker1x_%i",Iterator);char nameRes1Y[30];sprintf(nameRes1Y,"residual_Tracker1y_%i",Iterator);
  char nameRes4X[30];sprintf(nameRes4X,"residual_Tracker4x_%i",Iterator);char nameRes4Y[30];sprintf(nameRes4Y,"residual_Tracker4y_%i",Iterator);
	
  TH1F* h_residual_Tracker2x = new TH1F(nameRes2X,"",100,-20,20); h_residual_Tracker2x->SetXTitle("Residual [mm]"); h_residual_Tracker2x->SetYTitle("Frequency");h_residual_Tracker2x->SetLabelSize(0.045,"XY");h_residual_Tracker2x->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker2y = new TH1F(nameRes2Y,"",100,-20,20); h_residual_Tracker2y->SetXTitle("Residual [mm]"); h_residual_Tracker2y->SetYTitle("Frequency");h_residual_Tracker2y->SetLabelSize(0.045,"XY");h_residual_Tracker2y->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker3x = new TH1F(nameRes3X,"",100,-20,20); h_residual_Tracker3x->SetXTitle("Residual [mm]"); h_residual_Tracker3x->SetYTitle("Frequency");h_residual_Tracker3x->SetLabelSize(0.045,"XY");h_residual_Tracker3x->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker3y = new TH1F(nameRes3Y,"",100,-20,20); h_residual_Tracker3y->SetXTitle("Residual [mm]"); h_residual_Tracker3y->SetYTitle("Frequency");h_residual_Tracker3y->SetLabelSize(0.045,"XY");h_residual_Tracker3y->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker1x = new TH1F(nameRes1X,"",100,-20,20); h_residual_Tracker1x->SetXTitle("Residual [mm]"); h_residual_Tracker1x->SetYTitle("Frequency");h_residual_Tracker1x->SetLabelSize(0.045,"XY");h_residual_Tracker1x->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker1y = new TH1F(nameRes1Y,"",100,-20,20); h_residual_Tracker1y->SetXTitle("Residual [mm]"); h_residual_Tracker1y->SetYTitle("Frequency");h_residual_Tracker1y->SetLabelSize(0.045,"XY");h_residual_Tracker1y->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker4x = new TH1F(nameRes4X,"",100,-20,20); h_residual_Tracker4x->SetXTitle("Residual [mm]"); h_residual_Tracker4x->SetYTitle("Frequency");h_residual_Tracker4x->SetLabelSize(0.045,"XY");h_residual_Tracker4x->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker4y = new TH1F(nameRes4Y,"",100,-20,20); h_residual_Tracker4y->SetXTitle("Residual [mm]"); h_residual_Tracker4y->SetYTitle("Frequency");h_residual_Tracker4y->SetLabelSize(0.045,"XY");h_residual_Tracker4y->SetTitleSize(0.045,"XY");

  TH1F* angleREF2 = new TH1F("angleREF2","Rotation angle distribution btwn REF1 and REF2",100,-3,3); angleREF2->SetXTitle("Angle [radian]"); angleREF2->SetYTitle("Counts/1uRad");
  TH1F* angleREF3 = new TH1F("angleREF3","Rotation angle distribution btwn REF1 and REF3",100,-3,3); angleREF3->SetXTitle("Angle [radian]"); angleREF3->SetYTitle("Counts/1uRad");
  TH1F* angleREF4 = new TH1F("angleREF3","Rotation angle distribution btwn REF1 and REF4",100,-3,3); angleREF4->SetXTitle("Angle [radian]"); angleREF4->SetYTitle("Counts/1uRad");


  map<int, TH2F*> Tracker1HitmapPerRotation;
  map<int, TH2F*> Tracker2HitmapPerRotation;
  map<int, TH2F*> Tracker3HitmapPerRotation;
  map<int, TH2F*> Tracker4HitmapPerRotation;
    
    

  //cout << "B" << endl;
  //first, shift all hits coordinats to optimal XY offsets

  float Tracker1HitlocVecx_tmp=0;
  float Tracker1HitlocVecy_tmp=0;
  float Tracker2HitlocVecx_tmp=0;
  float Tracker2HitlocVecy_tmp=0;
  float Tracker3HitlocVecx_tmp=0;
  float Tracker3HitlocVecy_tmp=0;
  float Tracker4HitlocVecx_tmp=0;
  float Tracker4HitlocVecy_tmp=0;

   
  for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){//shift all of the XY hit coordinates
	  
    Tracker1HitlocVec[i] = Tracker1HitlocVec[i] - Shift_T1x;
    //Tracker1HitlocVecx_tmp = Tracker1HitlocVec[i];
    Tracker2HitlocVec[i] = Tracker2HitlocVec[i] - Shift_T2x;
    //Tracker2HitlocVecx_tmp = Tracker2HitlocVec[i]; 
    Tracker3HitlocVec[i] = Tracker3HitlocVec[i] - Shift_T3x;
    //Tracker3HitlocVecx_tmp = Tracker3HitlocVec[i];
    Tracker4HitlocVec[i] = Tracker4HitlocVec[i] - Shift_T4x;
    //Tracker4HitlocVecx_tmp = Tracker4HitlocVec[i];

    i++;//move to y components
	  
    Tracker1HitlocVec[i] = Tracker1HitlocVec[i] - Shift_T1y;
    //Tracker1HitlocVecy_tmp = Tracker1HitlocVec[i];
    Tracker2HitlocVec[i] = Tracker2HitlocVec[i] - Shift_T2y;
    //Tracker2HitlocVecy_tmp = Tracker2HitlocVec[i];
    Tracker3HitlocVec[i] = Tracker3HitlocVec[i] - Shift_T3y;
    //Tracker3HitlocVecy_tmp = Tracker3HitlocVec[i];
    Tracker4HitlocVec[i] = Tracker4HitlocVec[i] - Shift_T4y;
    //Tracker4HitlocVecy_tmp = Tracker4HitlocVec[i];
     

    i++;
  }

  vector<float> ShiftedTracker1HitlocVec;

  ShiftedTracker1HitlocVec.insert(ShiftedTracker1HitlocVec.begin(), Tracker1HitlocVec.begin(), Tracker1HitlocVec.end());

  vector<float> ShiftedTracker2HitlocVec;

  ShiftedTracker2HitlocVec.insert(ShiftedTracker2HitlocVec.begin(), Tracker2HitlocVec.begin(), Tracker2HitlocVec.end());

  vector<float> ShiftedTracker3HitlocVec;

  ShiftedTracker3HitlocVec.insert(ShiftedTracker3HitlocVec.begin(), Tracker3HitlocVec.begin(), Tracker3HitlocVec.end());

  vector<float> ShiftedTracker4HitlocVec;

  ShiftedTracker4HitlocVec.insert(ShiftedTracker4HitlocVec.begin(), Tracker4HitlocVec.begin(), Tracker4HitlocVec.end());

  //ResCut(ShiftedTracker1HitlocVec, ShiftedTracker2HitlocVec, ShiftedTracker3HitlocVec, ShiftedTracker4HitlocVec);

  TGraph* xChi2PerAngleT2 = new TGraph();xChi2PerAngleT2->GetXaxis()->SetTitle("Angle [Rad]");xChi2PerAngleT2->GetYaxis()->SetTitle("#chi^2");xChi2PerAngleT2->SetTitle("#chi^2 of Tracks in X-Z projection plane of Tracker 2");//xChi2PerAngleT2
  TGraph* yChi2PerAngleT2 = new TGraph();yChi2PerAngleT2->GetXaxis()->SetTitle("Angle [Rad]");yChi2PerAngleT2->GetYaxis()->SetTitle("#chi^2");yChi2PerAngleT2->SetTitle("#chi^2 of Tracks in Y-Z projection plane of Tracker 2");
  TGraph* xChi2PerAngleT3 = new TGraph();xChi2PerAngleT3->GetXaxis()->SetTitle("Angle [Rad]");xChi2PerAngleT3->GetYaxis()->SetTitle("#chi^2");xChi2PerAngleT3->SetTitle("#chi^2 of track in X-Z projection plane");//xChi2PerAngleT3
  TGraph* yChi2PerAngleT3 = new TGraph();yChi2PerAngleT3->GetXaxis()->SetTitle("AngleT3");yChi2PerAngleT3->GetYaxis()->SetTitle("#chi^2");yChi2PerAngleT3->SetTitle("#chi^2 of track in Y-Z projection plane");
  TGraph* xChi2PerAngleT4 = new TGraph();xChi2PerAngleT4->GetXaxis()->SetTitle("AngleT4");xChi2PerAngleT4->GetYaxis()->SetTitle("#chi^2");xChi2PerAngleT4->SetTitle("#chi^2 of track in X-Z projection plane");//xChi2PerAngleT4
  TGraph* yChi2PerAngleT4 = new TGraph();yChi2PerAngleT4->GetXaxis()->SetTitle("AngleT4");yChi2PerAngleT4->GetYaxis()->SetTitle("#chi^2");yChi2PerAngleT4->SetTitle("#chi^2 of track in Y-Z projection plane");

  
  //Now rotate each angle (3) around a small range to tune the angles (just roatating Tracker 2 for now)
  int bounds = 10;
  float anglescale_Factor = 0.0003;//300 urad
  float angleT2temp=0;
  float angleT3temp=0;
  float angleT4temp=0;

  float prealignedX =  Tracker1HitlocVec[0];
  float prealignedY =  Tracker1HitlocVec[1];   
  for(int AngleIt=-bounds;AngleIt <= bounds; AngleIt++){
      
    angleT2temp = angleT2 + anglescale_Factor*AngleIt;
    angleT3temp = angleT3 + anglescale_Factor*AngleIt;
    angleT4temp = angleT4 + anglescale_Factor*AngleIt;
      
    char nameRot1[30];sprintf(nameRot1,"Rotation_Tracker1_%i",AngleIt);
    char nameRot2[30];sprintf(nameRot2,"Rotation_Tracker2_%i",AngleIt);
    char nameRot3[30];sprintf(nameRot3,"Rotation_Tracker3_%i",AngleIt);
    char nameRot4[30];sprintf(nameRot4,"Rotation_Tracker4_%i",AngleIt);
    char nameRotCanvas[30];sprintf(nameRotCanvas,"Rotation_canvas_%f",angleT2temp);

    char titleRot1[30];sprintf(titleRot1,"Rotation_Tracker1_%i",AngleIt);
    char titleRot2[30];sprintf(titleRot2,"Rotation_Tracker2_%i",AngleIt);
    char titleRot3[30];sprintf(titleRot3,"Rotation_Tracker3_%i",AngleIt);
    char titleRot4[30];sprintf(titleRot4,"Rotation_Tracker4_%i",AngleIt);
    char titleRotCanvas[30];sprintf(titleRotCanvas,"Rotation_canvas_%f",angleT2temp);
    
    Tracker1HitmapPerRotation[AngleIt] = new TH2F(nameRot1, titleRot1, 100, -50, 50, 100, -50, 50);
    Tracker3HitmapPerRotation[AngleIt] = new TH2F(nameRot3, titleRot3, 100, -50, 50, 100, -50, 50);
    Tracker2HitmapPerRotation[AngleIt] = new TH2F(nameRot2, titleRot2, 100, -50, 50, 100, -50, 50);
    Tracker4HitmapPerRotation[AngleIt] = new TH2F(nameRot4, titleRot4, 100, -50, 50, 100, -50, 50);
    //      float Tracker2Rotatedxcoord=0; float Tracker2Rotatedycoord=0;

    
    //angleT2 += anglescale_Factor*AngleIt;
      
    //angleT3tmep =
    //AngleT2PerIteration->SetPoint(AngleIt+bounds, AngleIt, angleT2temp);
      
    cout << "Angle rotated to: " << angleT2temp << " at iterator: " << AngleIt << endl;
     
    for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){
      
      Tracker2HitlocVecx_tmp = ShiftedTracker2HitlocVec[i]; 	
      Tracker2HitlocVec[i] = ShiftedTracker2HitlocVec[i]*cos(angleT2temp) - ShiftedTracker2HitlocVec[i+1]*sin(angleT2temp);	  
      
      Tracker3HitlocVecx_tmp = ShiftedTracker3HitlocVec[i]; 	
      Tracker3HitlocVec[i] = ShiftedTracker3HitlocVec[i]*cos(angleT3temp) - ShiftedTracker3HitlocVec[i+1]*sin(angleT3temp);	  

      Tracker4HitlocVecx_tmp = ShiftedTracker4HitlocVec[i]; 	
      Tracker4HitlocVec[i] = ShiftedTracker4HitlocVec[i]*cos(angleT4temp) - ShiftedTracker4HitlocVec[i+1]*sin(angleT4temp);	  
      
      
      i++;//move to y components
	
      Tracker2HitlocVec[i] = Tracker2HitlocVecx_tmp*sin(angleT2temp) + ShiftedTracker2HitlocVec[i]*cos(angleT2temp);	  
      
      Tracker3HitlocVec[i] = Tracker3HitlocVecx_tmp*sin(angleT3temp) + ShiftedTracker3HitlocVec[i]*cos(angleT3temp);	  
      Tracker4HitlocVec[i] = Tracker4HitlocVecx_tmp*sin(angleT4temp) + ShiftedTracker4HitlocVec[i]*cos(angleT4temp);	  
 
      
      //Tracker2HitmapPerRotation[AngleIt]->Fill(Tracker2HitlocVec[i-1], Tracker2HitlocVec[i]);
      //Tracker1HitmapPerRotation[AngleIt]->Fill(Tracker1HitlocVec[i-1], Tracker1HitlocVec[i]);
	
      i++;
      
      //double cosineREF2;
      //angleREF2->Fill(cosineREF2 = CalculateCosTheta1(Tracker1HitlocVec[i-1],Tracker1HitlocVec[i],Tracker2HitlocVec[i-1],Tracker2HitlocVec[i]));    	  
    }

    // cout << "Average angle difference between Tracker 1 and 2: " << angleREF2->GetMean() << endl;
      	
    auto NewRes2 = Residuals(Tracker1HitlocVec, Tracker2HitlocVec, ShiftedTracker3HitlocVec, ShiftedTracker4HitlocVec, BadEvents);
    auto NewRes3 = Residuals(Tracker1HitlocVec, ShiftedTracker2HitlocVec, Tracker3HitlocVec, ShiftedTracker4HitlocVec, BadEvents);
    auto NewRes4 = Residuals(Tracker1HitlocVec, ShiftedTracker2HitlocVec, ShiftedTracker3HitlocVec, Tracker4HitlocVec, BadEvents);


    auto Chisquares2 = ChiSquared(Tracker1HitlocVec, Tracker2HitlocVec, ShiftedTracker3HitlocVec, ShiftedTracker4HitlocVec, BadEvents);
    auto Chisquares3 = ChiSquared(Tracker1HitlocVec, ShiftedTracker2HitlocVec, Tracker3HitlocVec, ShiftedTracker4HitlocVec, BadEvents);
    auto Chisquares4 = ChiSquared(Tracker1HitlocVec, ShiftedTracker2HitlocVec, ShiftedTracker3HitlocVec, Tracker4HitlocVec, BadEvents);
 
    xChi2PerAngleT2->SetPoint(AngleIt+bounds, angleT2temp, Chisquares2[0]);
    yChi2PerAngleT2->SetPoint(AngleIt+bounds, angleT2temp, Chisquares2[1]);

    xChi2PerAngleT3->SetPoint(AngleIt+bounds, angleT3temp, Chisquares3[0]);
    yChi2PerAngleT3->SetPoint(AngleIt+bounds, angleT3temp, Chisquares3[1]);

    xChi2PerAngleT4->SetPoint(AngleIt+bounds, angleT4temp, Chisquares4[0]);
    yChi2PerAngleT4->SetPoint(AngleIt+bounds, angleT4temp, Chisquares4[1]);
    
        
    if((MeanangleT2 <= -2 || MeanangleT2 > 2) || (MeanangleT3 <= -2 || MeanangleT3 > 2) || (MeanangleT4 <= -2 || MeanangleT4 > 2)){
      
      TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 500, 500);
      //TrackerRes->Divide(2,2);
      
      //TrackerRes->cd(1);
      angleREF2->Draw();
	
      return Optimized_Angles_And_XY;

    }
    
    
    
    /*
      if(plots){
      TCanvas* TrackerHitmaps = new TCanvas(nameRotCanvas, "", 1000, 500);
    
      TrackerHitmaps->Divide(2,1);
      TrackerHitmaps->cd(1);
      Tracker1HitmapPerRotation[AngleIt]->Draw("colz");
      gPad->Update();
	  
      TrackerHitmaps->cd(2);
      Tracker2HitmapPerRotation[AngleIt]->Draw("colz");
      gPad->Update();
	
      TrackerHitmaps->Draw();
      gPad->Update();
      //gSystem->ProcessEvents();
      //gSystem->ProcessEvents();
      TrackerHitmaps->SaveAs(nameRotCanvas);
	
      continue;
      }
    */
  }

  xChi2PerAngleT2->Fit("pol2");
  TF1 *fit2x = xChi2PerAngleT2->GetFunction("pol2");
  auto optxAngle2 = -(fit2x->GetParameter(1))/(2*fit2x->GetParameter(2));
  cout << "Angle of vertex is: " << optxAngle2 << endl;
  
  yChi2PerAngleT2->Fit("pol2");
  TF1 *fit2y = yChi2PerAngleT2->GetFunction("pol2");
  auto optyAngle2 = -(fit2y->GetParameter(1))/(2*fit2y->GetParameter(2));
  cout << "Angle of vertex is: " << optyAngle2 << endl;

  auto optAngleT2 = (optxAngle2 + optyAngle2)/2;
  
  xChi2PerAngleT3->Fit("pol2");
  TF1 *fit3x = xChi2PerAngleT3->GetFunction("pol2");
  auto optxAngle3 = -(fit3x->GetParameter(1))/(2*fit3x->GetParameter(2));
  cout << "Angle of vertex is: " << optxAngle3 << endl;
  
  yChi2PerAngleT3->Fit("pol2");
  TF1 *fit3y = xChi2PerAngleT3->GetFunction("pol2");
  auto optyAngle3 = -(fit3y->GetParameter(1))/(2*fit3y->GetParameter(2));
  cout << "Angle of vertex is: " << optyAngle3 << endl;

  auto optAngleT3 = (optxAngle3 + optyAngle3)/2;
    
  xChi2PerAngleT4->Fit("pol2");
  TF1 *fit4x = xChi2PerAngleT4->GetFunction("pol2");
  auto optxAngle4 = -(fit4x->GetParameter(1))/(2*fit4x->GetParameter(2));
  cout << "Angle of vertex is: " << optxAngle4 << endl;
  
  yChi2PerAngleT4->Fit("pol2");
  TF1 *fit4y = xChi2PerAngleT4->GetFunction("pol2");
  auto optyAngle4 = -(fit4y->GetParameter(1))/(2*fit4y->GetParameter(2));
  cout << "Angle of vertex is: " << optyAngle4 << endl;

  auto optAngleT4 = (optxAngle4 + optyAngle4)/2;

  
    
  Optimized_Angles_And_XY.push_back(Shift_T1x); Optimized_Angles_And_XY.push_back(Shift_T1y);
  Optimized_Angles_And_XY.push_back(Shift_T2x); Optimized_Angles_And_XY.push_back(Shift_T2y);
  Optimized_Angles_And_XY.push_back(Shift_T3x); Optimized_Angles_And_XY.push_back(Shift_T3y);
  Optimized_Angles_And_XY.push_back(Shift_T4x); Optimized_Angles_And_XY.push_back(Shift_T4y);
      
  Optimized_Angles_And_XY.push_back(optAngleT2);
  Optimized_Angles_And_XY.push_back(optAngleT3);
  Optimized_Angles_And_XY.push_back(optAngleT4);

  cout << "Pre Aligment first X value: " << prealignedX << endl;
  cout << "Pre Aligment first Y value: " << prealignedY << endl;
  
  cout << "PostAligment first X value: " << Tracker1HitlocVec[0] << endl;
  cout << "PostAligment first Y value: " << Tracker1HitlocVec[1] << endl;
	
  //Tracker plot for IEEE abstract
  if(plots){

    /*
      
      TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1000, 1000);
    
      TrackerRes->Divide(2,2);

      TrackerRes->cd(1);
      angleREF2->Draw();
      TrackerRes->cd(2);
      angleREF3->Draw();
      TrackerRes->cd(3);
      angleREF4->Draw();
      
      TrackerRes->Draw();
      
      TCanvas* AnglesCanvas = new TCanvas("", "", 1500, 500);
      AnglesCanvas->Divide(3,1);
      AnglesCanvas->cd(1);
      AngleT2PerIteration->Draw("A*");
      AnglesCanvas->cd(2);
      AngleT3PerIteration->Draw("A*");
      AnglesCanvas->cd(3);
      AngleT4PerIteration->Draw("A*");

      AnglesCanvas->Draw();
      
      
      TCanvas* c1 = new TCanvas("c1","", 700,500);
      c1->cd(1);
      Residual1XPerIteration->SetTitle("Mean XY Residuals of Trackers 1-4");
      Residual1XPerIteration->SetMarkerStyle(20);
      Residual1XPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual1XPerIteration->Draw("AP");
      Residual1YPerIteration->SetMarkerStyle(24);
      Residual1YPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual1YPerIteration->Draw("Psame");
      Residual2XPerIteration->SetMarkerStyle(21);
      Residual2XPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual2XPerIteration->Draw("Psame");
      Residual2YPerIteration->SetMarkerStyle(25);
      Residual2YPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual2YPerIteration->Draw("Psame");
      Residual3XPerIteration->SetMarkerStyle(22);
      Residual3XPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual3XPerIteration->Draw("Psame");
      Residual3YPerIteration->SetMarkerStyle(26);
      Residual3YPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual3YPerIteration->Draw("Psame");
      Residual4YPerIteration->SetMarkerStyle(23);
      Residual4YPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual4XPerIteration->Draw("Psame");
      Residual4YPerIteration->SetMarkerStyle(27);
      Residual4YPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual4YPerIteration->Draw("Psame");

      auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
      legend->AddEntry(Residual1XPerIteration, "Tracker 1X", "P");
      legend->AddEntry(Residual1YPerIteration, "Tracker 1Y", "P");
      legend->AddEntry(Residual2XPerIteration, "Tracker 2X", "P");
      legend->AddEntry(Residual2YPerIteration, "Tracker 2Y", "P");
      legend->AddEntry(Residual3XPerIteration, "Tracker 3X", "P");
      legend->AddEntry(Residual3YPerIteration, "Tracker 3Y", "P");
      legend->AddEntry(Residual4XPerIteration, "Tracker 4X", "P");
      legend->AddEntry(Residual4YPerIteration, "Tracker 4Y", "P");

      legend->Draw();

      c1->Draw();
    */
    TCanvas* ChiSquareData = new TCanvas("", "", 1000, 1500);
    ChiSquareData->Divide(2,3);

    ChiSquareData->cd(1);
    xChi2PerAngleT2->Draw("A*");	 
    ChiSquareData->cd(2);
    yChi2PerAngleT2->Draw("A*");
    ChiSquareData->cd(3);
    xChi2PerAngleT3->Draw("A*");	 
    ChiSquareData->cd(4);
    yChi2PerAngleT3->Draw("A*");
    ChiSquareData->cd(5);
    xChi2PerAngleT4->Draw("A*");	 
    ChiSquareData->cd(6);
    yChi2PerAngleT4->Draw("A*");

    ChiSquareData->SaveAs("Alignment3_chisquared.png");
      
    return Optimized_Angles_And_XY;
      
  }
    
  //double scale_Factor = -0.1;	
  //XY shift values stay constant

  /*
    Shift_T1x = scale_Factor*mean_Tracker1xcoordRes; Shift_T1y = scale_Factor*mean_Tracker1ycoordRes;
    Shift_T2x = scale_Factor*mean_Tracker2xcoordRes; Shift_T2y = scale_Factor*mean_Tracker2ycoordRes;
    Shift_T3x = scale_Factor*mean_Tracker3xcoordRes; Shift_T3y = scale_Factor*mean_Tracker3ycoordRes;
    Shift_T4x = scale_Factor*mean_Tracker4xcoordRes; Shift_T4y = scale_Factor*mean_Tracker4ycoordRes;
  */

  /*
    double anglescale_Factor = 0.1;	
    angleT2 = anglescale_Factor*MeanangleT2;
    angleT3 = anglescale_Factor*MeanangleT3;
    angleT4 = anglescale_Factor*MeanangleT4;
  */
    
  TotalangleT2 += angleT2;
  TotalangleT3 += angleT3;
  TotalangleT4 += angleT4;

  Total1Xshift += Shift_T1x;
  Total1Yshift += Shift_T1y;
  Total2Xshift += Shift_T2x;
  Total2Yshift += Shift_T2y;
  Total3Xshift += Shift_T3x;
  Total3Yshift += Shift_T3y;
  Total4Xshift += Shift_T4x;
  Total4Yshift += Shift_T4y;
  //break;
  
  
  return Optimized_Angles_And_XY;


}


vector<float> Optimized_Rotation(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> OptimizedXYOffests, vector<float> BadEvents, bool plots){

  float Shift_T1x;
  float Shift_T1y;
  float Shift_T2x;
  float Shift_T2y;
  float Shift_T3x;
  float Shift_T3y;
  float Shift_T4x;
  float Shift_T4y;
  
  
  if(OptimizedXYOffests.empty()){
    Shift_T1x = 0;
    Shift_T1y = 0;
    Shift_T2x = 0;
    Shift_T2y = 0;
    Shift_T3x = 0;
    Shift_T3y = 0;
    Shift_T4x = 0;
    Shift_T4y = 0;
  }
  else {
    Shift_T1x = OptimizedXYOffests[0];
    Shift_T1y = OptimizedXYOffests[1];
    Shift_T2x = OptimizedXYOffests[2];
    Shift_T2y = OptimizedXYOffests[3];
    Shift_T3x = OptimizedXYOffests[4];
    Shift_T3y = OptimizedXYOffests[5];
    Shift_T4x = OptimizedXYOffests[6];
    Shift_T4y = OptimizedXYOffests[7];
    
  }
  
  cout << Shift_T1x << endl;
  cout << Shift_T1y << endl;
  cout << Shift_T2x << endl;
  cout << Shift_T2y << endl;
  cout << Shift_T3x << endl;
  cout << Shift_T3y << endl;
  cout << Shift_T4x << endl;
  cout << Shift_T4y << endl;



  double mean_Tracker1xcoordRes=0.0, mean_Tracker1ycoordRes=0.0;
  double mean_Tracker2xcoordRes=0.0, mean_Tracker2ycoordRes=0.0;
  double mean_Tracker3xcoordRes=0.0, mean_Tracker3ycoordRes=0.0;
  double mean_Tracker4xcoordRes=0.0, mean_Tracker4ycoordRes=0.0;  
  
  int Iterator=0;
  float angleT2=0.0;
  float angleT3=0.0;
  float angleT4=0.0;
  float MeanangleT2=0.0;
  float MeanangleT3=0.0;
  float MeanangleT4=0.0;
  float TotalangleT2=0.0;
  float TotalangleT3=0.0;
  float TotalangleT4=0.0;

  float Total1Xshift=0;
  float Total1Yshift=0;
  float Total2Xshift=0;
  float Total2Yshift=0;
  float Total3Xshift=0;
  float Total3Yshift=0;
  float Total4Xshift=0;
  float Total4Yshift=0;
  
  vector<float> Optimized_Angles_And_XY;

  
  int ResIt = 5;

  //if(plots){
    TGraph* Residual1XPerIteration = new TGraph();Residual1XPerIteration->GetXaxis()->SetTitle("Iterations");Residual1XPerIteration->GetYaxis()->SetTitle("Residual");Residual1XPerIteration->SetTitle("Residual Per Iteration in Tracker1 x coordinate");
    TGraph* Residual1YPerIteration = new TGraph();Residual1YPerIteration->GetXaxis()->SetTitle("Iterations");Residual1YPerIteration->GetYaxis()->SetTitle("Residual");Residual1YPerIteration->SetTitle("Residual Per Iteration in Tracker1 y coordinate");
    TGraph* Residual2XPerIteration = new TGraph();Residual2XPerIteration->GetXaxis()->SetTitle("Iterations");Residual2XPerIteration->GetYaxis()->SetTitle("Residual");Residual2XPerIteration->SetTitle("Residual Per Iteration in Tracker2 x coordinate");
    TGraph* Residual2YPerIteration = new TGraph();Residual2YPerIteration->GetXaxis()->SetTitle("Iterations");Residual2YPerIteration->GetYaxis()->SetTitle("Residual");Residual2YPerIteration->SetTitle("Residual Per Iteration in Tracker2 y coordinate");
    TGraph* Residual3XPerIteration = new TGraph();Residual3XPerIteration->GetXaxis()->SetTitle("Iterations");Residual3XPerIteration->GetYaxis()->SetTitle("Residual");Residual3XPerIteration->SetTitle("Residual Per Iteration in Tracker3 x coordinate");
    TGraph* Residual3YPerIteration = new TGraph();Residual3YPerIteration->GetXaxis()->SetTitle("Iterations");Residual3YPerIteration->GetYaxis()->SetTitle("Residual");Residual3YPerIteration->SetTitle("Residual Per Iteration in Tracker3 y coordinate");
    TGraph* Residual4XPerIteration = new TGraph();Residual4XPerIteration->GetXaxis()->SetTitle("Iterations");Residual4XPerIteration->GetYaxis()->SetTitle("Residual");Residual4XPerIteration->SetTitle("Residual Per Iteration in Tracker4 x coordinate");
    TGraph* Residual4YPerIteration = new TGraph();Residual4YPerIteration->GetXaxis()->SetTitle("Iterations");Residual4YPerIteration->GetYaxis()->SetTitle("Residual");Residual4YPerIteration->SetTitle("Residual Per Iteration in Tracker4 y coordinate");

    TGraph* AngleT2PerIteration = new TGraph();AngleT2PerIteration->GetXaxis()->SetTitle("Iterations");AngleT2PerIteration->GetYaxis()->SetTitle("Angle (Rad)");AngleT2PerIteration->SetTitle("Angle rotation Per Iteration in Tracker 2");
    TGraph* AngleT3PerIteration = new TGraph();AngleT3PerIteration->GetXaxis()->SetTitle("Iterations");AngleT3PerIteration->GetYaxis()->SetTitle("Angle (Rad)");AngleT3PerIteration->SetTitle("Angle rotation Per Iteration in Tracker 3");
    TGraph* AngleT4PerIteration = new TGraph();AngleT4PerIteration->GetXaxis()->SetTitle("Iterations");AngleT4PerIteration->GetYaxis()->SetTitle("Angle (Rad)");AngleT4PerIteration->SetTitle("Angle rotation Per Iteration in Tracker 4");

    TGraph* xChi2PerIteration = new TGraph();xChi2PerIteration->GetXaxis()->SetTitle("Iteration");xChi2PerIteration->GetYaxis()->SetTitle("#chi^2");xChi2PerIteration->SetTitle("#chi^2 of track in X-Z projection plane");//xChi2PerIteration
    TGraph* yChi2PerIteration = new TGraph();yChi2PerIteration->GetXaxis()->SetTitle("Iteration");yChi2PerIteration->GetYaxis()->SetTitle("#chi^2");yChi2PerIteration->SetTitle("#chi^2 of track in Y-Z projection plane");
    //}

    float prealignedX =  Tracker1HitlocVec[0];
    float prealignedY =  Tracker1HitlocVec[1];   
    while(1){

      Iterator++;
   

    
      char nameRes2X[30];sprintf(nameRes2X,"residual_Tracker2x_%i",Iterator);char nameRes2Y[30];sprintf(nameRes2Y,"residual_Tracker2y_%i",Iterator);
      char nameRes3X[30];sprintf(nameRes3X,"residual_Tracker3x_%i",Iterator);char nameRes3Y[30];sprintf(nameRes3Y,"residual_Tracker3y_%i",Iterator);
      char nameRes1X[30];sprintf(nameRes1X,"residual_Tracker1x_%i",Iterator);char nameRes1Y[30];sprintf(nameRes1Y,"residual_Tracker1y_%i",Iterator);
      char nameRes4X[30];sprintf(nameRes4X,"residual_Tracker4x_%i",Iterator);char nameRes4Y[30];sprintf(nameRes4Y,"residual_Tracker4y_%i",Iterator);
	
      TH1F* h_residual_Tracker2x = new TH1F(nameRes2X,"",100,-2,2); h_residual_Tracker2x->SetXTitle("Residual [mm]"); h_residual_Tracker2x->SetYTitle("Frequency");h_residual_Tracker2x->SetLabelSize(0.042,"XY");h_residual_Tracker2x->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker2y = new TH1F(nameRes2Y,"",100,-2,2); h_residual_Tracker2y->SetXTitle("Residual [mm]"); h_residual_Tracker2y->SetYTitle("Frequency");h_residual_Tracker2y->SetLabelSize(0.042,"XY");h_residual_Tracker2y->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker3x = new TH1F(nameRes3X,"",100,-2,2); h_residual_Tracker3x->SetXTitle("Residual [mm]"); h_residual_Tracker3x->SetYTitle("Frequency");h_residual_Tracker3x->SetLabelSize(0.042,"XY");h_residual_Tracker3x->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker3y = new TH1F(nameRes3Y,"",100,-2,2); h_residual_Tracker3y->SetXTitle("Residual [mm]"); h_residual_Tracker3y->SetYTitle("Frequency");h_residual_Tracker3y->SetLabelSize(0.042,"XY");h_residual_Tracker3y->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker1x = new TH1F(nameRes1X,"",100,-2,2); h_residual_Tracker1x->SetXTitle("Residual [mm]"); h_residual_Tracker1x->SetYTitle("Frequency");h_residual_Tracker1x->SetLabelSize(0.042,"XY");h_residual_Tracker1x->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker1y = new TH1F(nameRes1Y,"",100,-2,2); h_residual_Tracker1y->SetXTitle("Residual [mm]"); h_residual_Tracker1y->SetYTitle("Frequency");h_residual_Tracker1y->SetLabelSize(0.042,"XY");h_residual_Tracker1y->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker4x = new TH1F(nameRes4X,"",100,-2,2); h_residual_Tracker4x->SetXTitle("Residual [mm]"); h_residual_Tracker4x->SetYTitle("Frequency");h_residual_Tracker4x->SetLabelSize(0.042,"XY");h_residual_Tracker4x->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker4y = new TH1F(nameRes4Y,"",100,-2,2); h_residual_Tracker4y->SetXTitle("Residual [mm]"); h_residual_Tracker4y->SetYTitle("Frequency");h_residual_Tracker4y->SetLabelSize(0.042,"XY");h_residual_Tracker4y->SetTitleSize(0.042,"XY");

      TH1F* angleREF2 = new TH1F("angleREF2","Rotation angle distribution btwn REF1 and REF2",100,-0.5,0.5); angleREF2->SetXTitle("Angle [radian]"); angleREF2->SetYTitle("Counts/1uRad");
      TH1F* angleREF3 = new TH1F("angleREF3","Rotation angle distribution btwn REF1 and REF3",100,-0.5,0.5); angleREF3->SetXTitle("Angle [radian]"); angleREF3->SetYTitle("Counts/1uRad");
      TH1F* angleREF4 = new TH1F("angleREF3","Rotation angle distribution btwn REF1 and REF4",100,-0.5,0.5); angleREF4->SetXTitle("Angle [radian]"); angleREF4->SetYTitle("Counts/1uRad");

    

      //cout << "B" << endl;
      //first, shift all hits coordinats to optimal XY offsets

      float Tracker1HitlocVecx_tmp=0;
      float Tracker1HitlocVecy_tmp=0;
      float Tracker2HitlocVecx_tmp=0;
      float Tracker2HitlocVecy_tmp=0;
      float Tracker3HitlocVecx_tmp=0;
      float Tracker3HitlocVecy_tmp=0;
      float Tracker4HitlocVecx_tmp=0;
      float Tracker4HitlocVecy_tmp=0;   
   
      for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){//shift all of the XY hit coordinates
	  
	Tracker1HitlocVec[i] = Tracker1HitlocVec[i] - Shift_T1x;
	//Tracker1HitlocVecx_tmp = Tracker1HitlocVec[i];
	Tracker2HitlocVec[i] = Tracker2HitlocVec[i] - Shift_T2x;
	//Tracker2HitlocVecx_tmp = Tracker2HitlocVec[i];
	Tracker3HitlocVec[i] = Tracker3HitlocVec[i] - Shift_T3x;
	//Tracker3HitlocVecx_tmp = Tracker3HitlocVec[i];
	Tracker4HitlocVec[i] = Tracker4HitlocVec[i] - Shift_T4x;
	//Tracker4HitlocVecx_tmp = Tracker4HitlocVec[i];

      
	i++;//move to y components
	  
	Tracker1HitlocVec[i] = Tracker1HitlocVec[i] - Shift_T1y;
	//Tracker1HitlocVecy_tmp = Tracker1HitlocVec[i];
	Tracker2HitlocVec[i] = Tracker2HitlocVec[i] - Shift_T2y;
	//Tracker2HitlocVecy_tmp = Tracker2HitlocVec[i];
	Tracker3HitlocVec[i] = Tracker3HitlocVec[i] - Shift_T3y;
	//Tracker3HitlocVecy_tmp = Tracker3HitlocVec[i];
	Tracker4HitlocVec[i] = Tracker4HitlocVec[i] - Shift_T4y;
	//Tracker4HitlocVecy_tmp = Tracker4HitlocVec[i];

      
	i++;
      }

      vector<float> ShiftedTracker1HitlocVec;

      ShiftedTracker1HitlocVec.insert(ShiftedTracker1HitlocVec.begin(), Tracker1HitlocVec.begin(), Tracker1HitlocVec.end());

      vector<float> ShiftedTracker2HitlocVec;

      ShiftedTracker2HitlocVec.insert(ShiftedTracker2HitlocVec.begin(), Tracker2HitlocVec.begin(), Tracker2HitlocVec.end());

      vector<float> ShiftedTracker3HitlocVec;

      ShiftedTracker3HitlocVec.insert(ShiftedTracker3HitlocVec.begin(), Tracker3HitlocVec.begin(), Tracker3HitlocVec.end());

      vector<float> ShiftedTracker4HitlocVec;

      ShiftedTracker4HitlocVec.insert(ShiftedTracker4HitlocVec.begin(), Tracker4HitlocVec.begin(), Tracker4HitlocVec.end());
  
    

    
      //Now rotate

      for(unsigned int i=0; i< ShiftedTracker1HitlocVec.size(); i++){	  
	  
	//h_Pos_Tracker1xCoord->Fill(Tracker1HitlocVec[i]);
	Tracker2HitlocVecx_tmp = ShiftedTracker2HitlocVec[i];  
	Tracker2HitlocVec[i] = ShiftedTracker2HitlocVec[i]*cos(angleT2) - ShiftedTracker2HitlocVec[i+1]*sin(angleT2);	  
	//h_Pos_Tracker2xCoord->Fill(Tracker2HitlocVec[i]);

	Tracker3HitlocVecx_tmp = ShiftedTracker3HitlocVec[i];
	Tracker3HitlocVec[i] = ShiftedTracker3HitlocVec[i]*cos(angleT3) - ShiftedTracker3HitlocVec[i+1]*sin(angleT3);	  
	//h_Pos_Tracker3xCoord->Fill(Tracker3HitlocVec[i]);
    
	Tracker4HitlocVecx_tmp = ShiftedTracker4HitlocVec[i];    
	Tracker4HitlocVec[i] = ShiftedTracker4HitlocVec[i]*cos(angleT4) - ShiftedTracker4HitlocVec[i+1]*sin(angleT4);	  
	//h_Pos_Tracker4xCoord->Fill(Tracker4HitlocVec[i]);

	i++;//move to y components
	  
	//h_Pos_Tracker1yCoord->Fill(Tracker1HitlocVec[i]);

	Tracker2HitlocVec[i] = Tracker2HitlocVecx_tmp*sin(angleT2) + ShiftedTracker2HitlocVec[i]*cos(angleT2);	  
	//h_Pos_Tracker2xCoord->Fill(Tracker2HitlocVec[i]);

	Tracker3HitlocVec[i] = Tracker3HitlocVecx_tmp*sin(angleT3) + ShiftedTracker3HitlocVec[i]*cos(angleT3);	  
	//h_Pos_Tracker3xCoord->Fill(Tracker3HitlocVec[i]);

	Tracker4HitlocVec[i] = Tracker4HitlocVecx_tmp*sin(angleT4) + ShiftedTracker4HitlocVec[i]*cos(angleT4);	  
	//h_Pos_Tracker4xCoord->Fill(Tracker4HitlocVec[i]);

  
	double cosineREF2;
	double cosineREF3;
	double cosineREF4;
	angleREF2->Fill(cosineREF2 = CalculateCosTheta1(Tracker1HitlocVec[i-1],Tracker1HitlocVec[i],Tracker2HitlocVec[i-1],Tracker2HitlocVec[i]));
	angleREF3->Fill(cosineREF3 = CalculateCosTheta1(Tracker1HitlocVec[i-1],Tracker1HitlocVec[i],Tracker3HitlocVec[i-1],Tracker3HitlocVec[i]));
	angleREF4->Fill(cosineREF4 = CalculateCosTheta1(Tracker1HitlocVec[i-1],Tracker1HitlocVec[i],Tracker4HitlocVec[i-1],Tracker4HitlocVec[i]));
      
	
	i++;
	  
      }

	
      auto NewRes = Residuals(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec, BadEvents);
      auto Chisquares = ChiSquared(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec, BadEvents);

	
	
      //cout << "Size of Residual vector: " << NewRes.size() << endl;
      for(unsigned int i=0; i< NewRes.size(); i++){//Put all residuals into plots

	h_residual_Tracker1x->Fill(NewRes[i]);	 
	i++;
	h_residual_Tracker1y->Fill(NewRes[i]);
	i++;
	h_residual_Tracker2x->Fill(NewRes[i]);
	i++;
	h_residual_Tracker2y->Fill(NewRes[i]);
	i++;
	h_residual_Tracker3x->Fill(NewRes[i]);
	i++;
	h_residual_Tracker3y->Fill(NewRes[i]);
	i++;
	h_residual_Tracker4x->Fill(NewRes[i]);
	i++;
	h_residual_Tracker4y->Fill(NewRes[i]);
      }
      /*
	TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1000, 500);
	TrackerRes->Divide(2,1);
    
	TrackerRes->cd(1);
	h_residual_Tracker2x->Draw();
	TrackerRes->cd(2);
	h_residual_Tracker2y->Draw();  

	TrackerRes->Draw();
	TrackerRes->SaveAs("Tracker2Residual_Alignment2.png");
      */
    
      I2GFvalues myValues;

	
      myValues = I2GFmainLoop(h_residual_Tracker1x,1,4,1);                           //converges!	  
      mean_Tracker1xcoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
      //mean_Tracker1xcoordRes = h_residual_Tracker1x->GetMean();
      //cout << "Mean Residual in x coordinate: " << mean_Tracker1xcoordRes << endl;
	
      myValues = I2GFmainLoop(h_residual_Tracker1y,1,4,1);	                            //breaks :(
      mean_Tracker1ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
      //mean_Tracker1ycoordRes = h_residual_Tracker1y->GetMean(); //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in y coordinate: " << mean_Tracker1ycoordRes << endl;
      //cout << "E" << endl;
	
      myValues = I2GFmainLoop(h_residual_Tracker2x,1,4,1);
      mean_Tracker2xcoordRes = myValues.mean;//converges!	  
      //mean_Tracker2xcoordRes = h_residual_Tracker2x->GetMean(); //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in x coordinate: " << mean_Tracker2xcoordRes << endl;

      myValues = I2GFmainLoop(h_residual_Tracker2y,1,4,1);	                            //breaks :(... im sensing a pattern...
      mean_Tracker2ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in y coordinate: " << mean_Tracker2ycoordRes << endl;
      myValues = I2GFmainLoop(h_residual_Tracker3x,1,4,1);
      mean_Tracker3xcoordRes = myValues.mean;
      //mean_Tracker3xcoordRes =h_residual_Tracker3x->GetMean(); //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in x coordinate: " << mean_Tracker3xcoordRes << endl;
      myValues = I2GFmainLoop(h_residual_Tracker3y,1,4,1);	  
      mean_Tracker3ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in y coordinate: " << mean_Tracker3ycoordRes << endl;
      myValues = I2GFmainLoop(h_residual_Tracker4x,1,4,1);	  
      mean_Tracker4xcoordRes = myValues.mean;
      //mean_Tracker4xcoordRes = h_residual_Tracker4x->GetMean(); //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in x coordinate: " << mean_Tracker4xcoordRes << endl;
      myValues = I2GFmainLoop(h_residual_Tracker4y,1,4,1);	  
      mean_Tracker4ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in y coordinate: " << mean_Tracker4ycoordRes << endl;

    
      myValues = I2GFmainLoop(angleREF2,1,5,1);	  
      MeanangleT2 = myValues.mean;    
      AngleT2PerIteration->SetPoint(Iterator-1, Iterator, MeanangleT2);
	  
      myValues = I2GFmainLoop(angleREF3,1,5,1);	  
      MeanangleT3 = myValues.mean;    
      AngleT3PerIteration->SetPoint(Iterator-1, Iterator, MeanangleT3);
	
      myValues = I2GFmainLoop(angleREF4,1,5,1);	  
      MeanangleT4 = myValues.mean;
      AngleT4PerIteration->SetPoint(Iterator-1, Iterator, MeanangleT4);	

    
    
      cout << "mean angle2: " << MeanangleT2 << endl;
      cout << "mean angle3: " << MeanangleT3 << endl;
      cout << "mean angle4: " << MeanangleT4 << endl;

      cout << "mean angle2 from ->GetMean(): " << angleREF2->GetMean() << endl;
      cout << "mean angle3 from ->GetMean(): " << angleREF3->GetMean() << endl;
      cout << "mean angle4 from ->GetMean(): " << angleREF4->GetMean() << endl;
      cout << "Iterator: " << Iterator << endl;

      xChi2PerIteration->SetPoint(Iterator-1, Iterator, Chisquares[0]);
      yChi2PerIteration->SetPoint(Iterator-1, Iterator, Chisquares[1]);
    
      /*
	cout << "Mean Residual in 1x coordinate: " << mean_Tracker1xcoordRes << endl;
	cout << "Was Shifted by: " << Shift_T1x << endl;
	cout << "Mean Residual in 1y coordinate: " << mean_Tracker1ycoordRes << endl;
	cout << "Was Shifted by: " << Shift_T1y << endl;
	cout << "Mean Residual in 2x coordinate: " << mean_Tracker2xcoordRes << endl;
	cout << "Was Shifted by: " << Shift_T2x << endl;
	cout << "Mean Residual in 2y coordinate: " << mean_Tracker2ycoordRes << endl;
	cout << "Was Shifted by: " << Shift_T2y << endl;
	cout << "Mean Residual in 3x coordinate: " << mean_Tracker3xcoordRes << endl;
	cout << "Was Shifted by: " << Shift_T3x << endl;
	cout << "Mean Residual in 3y coordinate: " << mean_Tracker3ycoordRes << endl;
	cout << "Was Shifted by: " << Shift_T3y << endl;
	cout << "Mean Residual in 4x coordinate: " << mean_Tracker4xcoordRes << endl;
	cout << "Was Shifted by: " << Shift_T4x << endl;
	cout << "Mean Residual in 4y coordinate: " << mean_Tracker4ycoordRes << endl;
	cout << "Was Shifted by: " << Shift_T4y << endl;
      */
    
      if((MeanangleT2 < -2 || MeanangleT2 > 2) || (MeanangleT3 > 2 || MeanangleT3 < -2) || (MeanangleT4 > 2 || MeanangleT4 < -2)){

	cout << "MeanangleT2: " << MeanangleT2 << ", MeanangleT3: " << MeanangleT3 << ", MeanangleT4: " << MeanangleT4 << endl;
	TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1000, 1000);
	TrackerRes->Divide(2,2);
      
	TrackerRes->cd(1);
	angleREF2->Draw();
	TrackerRes->cd(2);
	angleREF3->Draw();
	TrackerRes->cd(3);
	angleREF4->Draw();
      
	return Optimized_Angles_And_XY;

      }
	
   	
	
      //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
      Residual1XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker1xcoordRes);
      //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
      Residual1YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker1ycoordRes);
      //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
      Residual2XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker2xcoordRes);
      //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
      Residual2YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker2ycoordRes);
      //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
      Residual3XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker3xcoordRes);
      //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
      Residual3YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker3ycoordRes);
      //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
      Residual4XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker4xcoordRes);
      //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
      Residual4YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker4ycoordRes);
    
    
	
      if((mean_Tracker1xcoordRes >= -0.01 && mean_Tracker1xcoordRes <= 0.01) && (mean_Tracker1ycoordRes >= -0.01 && mean_Tracker1ycoordRes <= 0.01) && (mean_Tracker2xcoordRes >= -0.01 && mean_Tracker2xcoordRes <= 0.01) && (mean_Tracker2ycoordRes >= -0.01 && mean_Tracker2ycoordRes <= 0.01) && (mean_Tracker3xcoordRes >= -0.01 && mean_Tracker3xcoordRes <= 0.01) && (mean_Tracker3ycoordRes >= -0.01 && mean_Tracker3ycoordRes <= 0.01) && (mean_Tracker4xcoordRes >= -0.01 && mean_Tracker4xcoordRes <= 0.01) && (mean_Tracker4ycoordRes >= -0.01 && mean_Tracker4ycoordRes <= 0.01) && (MeanangleT2>= -0.01 && MeanangleT2 <= 0.01) && (MeanangleT3>= -0.01 && MeanangleT3 <= 0.01) && (MeanangleT4>= -0.01 && MeanangleT4 <= 0.01)){


	Optimized_Angles_And_XY.push_back(Total1Xshift + OptimizedXYOffests[0]); Optimized_Angles_And_XY.push_back(Total1Yshift + OptimizedXYOffests[1]);
	Optimized_Angles_And_XY.push_back(Total2Xshift + OptimizedXYOffests[2]); Optimized_Angles_And_XY.push_back(Total2Yshift + OptimizedXYOffests[3]);
	Optimized_Angles_And_XY.push_back(Total3Xshift + OptimizedXYOffests[4]); Optimized_Angles_And_XY.push_back(Total3Yshift + OptimizedXYOffests[5]);
	Optimized_Angles_And_XY.push_back(Total4Xshift + OptimizedXYOffests[6]); Optimized_Angles_And_XY.push_back(Total4Yshift + OptimizedXYOffests[7]);
      
	Optimized_Angles_And_XY.push_back(TotalangleT2);
	Optimized_Angles_And_XY.push_back(TotalangleT3);
	Optimized_Angles_And_XY.push_back(TotalangleT4);


      
	cout<<"find it...iterating "<< Iterator <<" times."<<endl;
      
	if(plots){
	
	  TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1000, 1000);
	  TrackerRes->Divide(2,2);

	  TrackerRes->cd(1);
	  angleREF2->Draw();
	  TrackerRes->cd(2);
	  angleREF3->Draw();
	  TrackerRes->cd(3);
	  angleREF4->Draw();
      
	  TrackerRes->Draw();
      
	  TCanvas* c1 = new TCanvas("c1","", 700,500);
	  c1->cd(1);
	  Residual1XPerIteration->SetTitle("Mean XY Residuals of Trackers 1-4");
	  Residual1XPerIteration->SetMarkerStyle(20);
	  Residual1XPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual1XPerIteration->Draw("AP");
	  Residual1YPerIteration->SetMarkerStyle(24);
	  Residual1YPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual1YPerIteration->Draw("Psame");
	  Residual2XPerIteration->SetMarkerStyle(21);
	  Residual2XPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual2XPerIteration->Draw("Psame");
	  Residual2YPerIteration->SetMarkerStyle(25);
	  Residual2YPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual2YPerIteration->Draw("Psame");
	  Residual3XPerIteration->SetMarkerStyle(22);
	  Residual3XPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual3XPerIteration->Draw("Psame");
	  Residual3YPerIteration->SetMarkerStyle(26);
	  Residual3YPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual3YPerIteration->Draw("Psame");
	  Residual4YPerIteration->SetMarkerStyle(23);
	  Residual4YPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual4XPerIteration->Draw("Psame");
	  Residual4YPerIteration->SetMarkerStyle(27);
	  Residual4YPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual4YPerIteration->Draw("Psame");

	  auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
	  legend->AddEntry(Residual1XPerIteration, "Tracker 1X", "P");
	  legend->AddEntry(Residual1YPerIteration, "Tracker 1Y", "P");
	  legend->AddEntry(Residual2XPerIteration, "Tracker 2X", "P");
	  legend->AddEntry(Residual2YPerIteration, "Tracker 2Y", "P");
	  legend->AddEntry(Residual3XPerIteration, "Tracker 3X", "P");
	  legend->AddEntry(Residual3YPerIteration, "Tracker 3Y", "P");
	  legend->AddEntry(Residual4XPerIteration, "Tracker 4X", "P");
	  legend->AddEntry(Residual4YPerIteration, "Tracker 4Y", "P");

	  legend->Draw();



	  c1->Draw();
	
	}
      
	break;
      }

    
      if(Iterator >= 20){	  
      
	Optimized_Angles_And_XY.push_back(Total1Xshift + OptimizedXYOffests[0]); Optimized_Angles_And_XY.push_back(Total1Yshift + OptimizedXYOffests[1]);
	Optimized_Angles_And_XY.push_back(Total2Xshift + OptimizedXYOffests[2]); Optimized_Angles_And_XY.push_back(Total2Yshift + OptimizedXYOffests[3]);
	Optimized_Angles_And_XY.push_back(Total3Xshift + OptimizedXYOffests[4]); Optimized_Angles_And_XY.push_back(Total3Yshift + OptimizedXYOffests[5]);
	Optimized_Angles_And_XY.push_back(Total4Xshift + OptimizedXYOffests[6]); Optimized_Angles_And_XY.push_back(Total4Yshift + OptimizedXYOffests[7]);
      
	Optimized_Angles_And_XY.push_back(TotalangleT2);
	Optimized_Angles_And_XY.push_back(TotalangleT3);
	Optimized_Angles_And_XY.push_back(TotalangleT4);


	if(plots){
	  //Tracker plot for IEEE abstract

	  /*
	    TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1000, 1000);
	    TrackerRes->Divide(2,2);

	    TrackerRes->cd(1);
	    angleREF2->Draw();
	    TrackerRes->cd(2);
	    angleREF3->Draw();
	    TrackerRes->cd(3);
	    angleREF4->Draw();
      
	    TrackerRes->Draw();
	  */

	  TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 2000, 1000);
	  TrackerRes->Divide(4,2);

	  TrackerRes->cd(1);
	  h_residual_Tracker1x->Draw();
	  TrackerRes->cd(5);
	  h_residual_Tracker1y->Draw();
	  TrackerRes->cd(2);
	  h_residual_Tracker2x->Draw();
	  TrackerRes->cd(6);
	  h_residual_Tracker2y->Draw();
	  TrackerRes->cd(3);
	  h_residual_Tracker3x->Draw();
	  TrackerRes->cd(7);
	  h_residual_Tracker3y->Draw();
	  TrackerRes->cd(4);
	  h_residual_Tracker4x->Draw();
	  TrackerRes->cd(8);
	  h_residual_Tracker4y->Draw();
	  

	  TrackerRes->Draw();

	
	  TCanvas* AnglesCanvas = new TCanvas("", "", 1500, 500);
	  AnglesCanvas->Divide(3,1);
	  AnglesCanvas->cd(1);
	  AngleT2PerIteration->Draw("A*");
	  AnglesCanvas->cd(2);
	  AngleT3PerIteration->Draw("A*");
	  AnglesCanvas->cd(3);
	  AngleT4PerIteration->Draw("A*");

	  AnglesCanvas->Draw();
      

	  TCanvas* c1 = new TCanvas("c1","", 700,500);
	  c1->cd(1);
	  Residual1XPerIteration->SetTitle("Mean XY Residuals of Trackers 1-4");
	  Residual1XPerIteration->SetMarkerStyle(20);
	  Residual1XPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual1XPerIteration->Draw("AP");
	  Residual1YPerIteration->SetMarkerStyle(24);
	  Residual1YPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual1YPerIteration->Draw("Psame");
	  Residual2XPerIteration->SetMarkerStyle(21);
	  Residual2XPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual2XPerIteration->Draw("Psame");
	  Residual2YPerIteration->SetMarkerStyle(25);
	  Residual2YPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual2YPerIteration->Draw("Psame");
	  Residual3XPerIteration->SetMarkerStyle(22);
	  Residual3XPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual3XPerIteration->Draw("Psame");
	  Residual3YPerIteration->SetMarkerStyle(26);
	  Residual3YPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual3YPerIteration->Draw("Psame");
	  Residual4YPerIteration->SetMarkerStyle(23);
	  Residual4YPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual4XPerIteration->Draw("Psame");
	  Residual4YPerIteration->SetMarkerStyle(27);
	  Residual4YPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual4YPerIteration->Draw("Psame");

	  auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
	  legend->AddEntry(Residual1XPerIteration, "Tracker 1X", "P");
	  legend->AddEntry(Residual1YPerIteration, "Tracker 1Y", "P");
	  legend->AddEntry(Residual2XPerIteration, "Tracker 2X", "P");
	  legend->AddEntry(Residual2YPerIteration, "Tracker 2Y", "P");
	  legend->AddEntry(Residual3XPerIteration, "Tracker 3X", "P");
	  legend->AddEntry(Residual3YPerIteration, "Tracker 3Y", "P");
	  legend->AddEntry(Residual4XPerIteration, "Tracker 4X", "P");
	  legend->AddEntry(Residual4YPerIteration, "Tracker 4Y", "P");

	  legend->Draw();

	  c1->Draw();
	
	  TCanvas* ChiSquareData = new TCanvas("", "", 1000, 500);
	  ChiSquareData->Divide(2,1);

	  ChiSquareData->cd(1);
	  xChi2PerIteration->Draw("A*");	 
	  ChiSquareData->cd(2);
	  yChi2PerIteration->Draw("A*");

	  ChiSquareData->SaveAs("Alignment2_Chisquared.png");
       
	  return Optimized_Angles_And_XY;
	}
      
	break;
      }

      double scale_Factor = -0.01;	
      //XY shift values stay constant

      Shift_T1x = scale_Factor*mean_Tracker1xcoordRes; Shift_T1y = scale_Factor*mean_Tracker1ycoordRes;
      Shift_T2x = scale_Factor*mean_Tracker2xcoordRes; Shift_T2y = scale_Factor*mean_Tracker2ycoordRes;
      Shift_T3x = scale_Factor*mean_Tracker3xcoordRes; Shift_T3y = scale_Factor*mean_Tracker3ycoordRes;
      Shift_T4x = scale_Factor*mean_Tracker4xcoordRes; Shift_T4y = scale_Factor*mean_Tracker4ycoordRes;
	
      double anglescale_Factor = -0.001;	
      angleT2 = anglescale_Factor*MeanangleT2;
      angleT3 = anglescale_Factor*MeanangleT3;
      angleT4 = anglescale_Factor*MeanangleT4;

      TotalangleT2 += angleT2;
      TotalangleT3 += angleT3;
      TotalangleT4 += angleT4;

      Total1Xshift += Shift_T1x;
      Total1Yshift += Shift_T1y;
      Total2Xshift += Shift_T2x;
      Total2Yshift += Shift_T2y;
      Total3Xshift += Shift_T3x;
      Total3Yshift += Shift_T3y;
      Total4Xshift += Shift_T4x;
      Total4Yshift += Shift_T4y;
	
    }
  
    return Optimized_Angles_And_XY;


}


vector<float> Optimized_XY_shift(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> OptimizedOffests, vector<float> BadEvents, bool plots){//find optimized XY offsets 

  
  vector<float> OptimizedXYOffsets;
  
  if(Tracker1HitlocVec.empty() || Tracker2HitlocVec.empty() || Tracker3HitlocVec.empty() || Tracker4HitlocVec.empty()){
    cout << "No hits in vectors" << endl;
    return OptimizedXYOffsets;
  }
  cout << "past here?" << endl;


  float Shift_T1x;
  float Shift_T1y;
  float Shift_T2x;
  float Shift_T2y;
  float Shift_T3x;
  float Shift_T3y;
  float Shift_T4x;
  float Shift_T4y;
  float REf2ang;
  float REf3ang;
  float REf4ang;
  
  
  if(OptimizedOffests.empty()){
    Shift_T1x = 0.0;
    Shift_T1y = 0.0;
    Shift_T2x = 0.0;
    Shift_T2y = 0.0;
    Shift_T3x = 0.0;
    Shift_T3y = 0.0;
    Shift_T4x = 0.0;
    Shift_T4y = 0.0;
    REf2ang=0.0;
    REf3ang=0.0;
    REf4ang=0.0;
  
  }
  else {
    Shift_T1x = OptimizedOffests[0];
    Shift_T1y = OptimizedOffests[1];
    Shift_T2x = OptimizedOffests[2];
    Shift_T2y = OptimizedOffests[3];
    Shift_T3x = OptimizedOffests[4];
    Shift_T3y = OptimizedOffests[5];
    Shift_T4x = OptimizedOffests[6];
    Shift_T4y = OptimizedOffests[7];
    REf2ang = OptimizedOffests[8];
    REf3ang = OptimizedOffests[9];
    REf4ang = OptimizedOffests[10];
    
    
  }

  double mean_Tracker1xcoordRes=0.0, mean_Tracker1ycoordRes=0.0;
  double mean_Tracker2xcoordRes=0.0, mean_Tracker2ycoordRes=0.0;
  double mean_Tracker3xcoordRes=0.0, mean_Tracker3ycoordRes=0.0;
  double mean_Tracker4xcoordRes=0.0, mean_Tracker4ycoordRes=0.0;  

  double width_Tracker1xcoordRes=0.0, width_Tracker1ycoordRes=0.0;
  double width_Tracker2xcoordRes=0.0, width_Tracker2ycoordRes=0.0;
  double width_Tracker3xcoordRes=0.0, width_Tracker3ycoordRes=0.0;
  double width_Tracker4xcoordRes=0.0, width_Tracker4ycoordRes=0.0;  

  
  int Iterator=0;
  float Total1Xshift=0.0;
  float Total1Yshift=0.0;
  float Total2Xshift=0.0;
  float Total2Yshift=0.0;
  float Total3Xshift=0.0;
  float Total3Yshift=0.0;
  float Total4Xshift=0.0;
  float Total4Yshift=0.0;

 
  TGraph* Residual1XPerIteration = new TGraph();Residual1XPerIteration->GetXaxis()->SetTitle("Iterations");Residual1XPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual1XPerIteration->SetTitle("Residual Per Iteration in Tracker1 x coordinate");
  TGraph* Residual1YPerIteration = new TGraph();Residual1YPerIteration->GetXaxis()->SetTitle("Iterations");Residual1YPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual1YPerIteration->SetTitle("Residual Per Iteration in Tracker1 y coordinate");
  TGraph* Residual2XPerIteration = new TGraph();Residual2XPerIteration->GetXaxis()->SetTitle("Iterations");Residual2XPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual2XPerIteration->SetTitle("Residual Per Iteration in Tracker2 x coordinate");
  TGraph* Residual2YPerIteration = new TGraph();Residual2YPerIteration->GetXaxis()->SetTitle("Iterations");Residual2YPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual2YPerIteration->SetTitle("Residual Per Iteration in Tracker2 y coordinate");
  TGraph* Residual3XPerIteration = new TGraph();Residual3XPerIteration->GetXaxis()->SetTitle("Iterations");Residual3XPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual3XPerIteration->SetTitle("Residual Per Iteration in Tracker3 x coordinate");
  TGraph* Residual3YPerIteration = new TGraph();Residual3YPerIteration->GetXaxis()->SetTitle("Iterations");Residual3YPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual3YPerIteration->SetTitle("Residual Per Iteration in Tracker3 y coordinate");
  TGraph* Residual4XPerIteration = new TGraph();Residual4XPerIteration->GetXaxis()->SetTitle("Iterations");Residual4XPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual4XPerIteration->SetTitle("Residual Per Iteration in Tracker4 x coordinate");
  TGraph* Residual4YPerIteration = new TGraph();Residual4YPerIteration->GetXaxis()->SetTitle("Iterations");Residual4YPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual4YPerIteration->SetTitle("Residual Per Iteration in Tracker4 y coordinate");
  
  TGraph* TrackerResidualsPerIteration = new TGraph();TrackerResidualsPerIteration->GetXaxis()->SetTitle("Iterations");TrackerResidualsPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");TrackerResidualsPerIteration->SetTitle("Iteration");

  TGraph* xChi2PerIteration = new TGraph();xChi2PerIteration->GetXaxis()->SetTitle("Iterator");xChi2PerIteration->GetYaxis()->SetTitle("#chi^2");xChi2PerIteration->SetTitle("#chi^2 of track in X-Z projection plane");
  TGraph* yChi2PerIteration = new TGraph();yChi2PerIteration->GetXaxis()->SetTitle("Iterator");yChi2PerIteration->GetYaxis()->SetTitle("#chi^2");yChi2PerIteration->SetTitle("#chi^2 of track in Y-Z projection plane");

  TGraph* Shift1XPerIteration = new TGraph();Shift1XPerIteration->GetXaxis()->SetTitle("Iterations");Shift1XPerIteration->GetYaxis()->SetTitle("Shift value");Shift1XPerIteration->SetTitle("Shift Per Iteration in Tracker1 x coordinate");
  TGraph* Shift2XPerIteration = new TGraph();Shift2XPerIteration->GetXaxis()->SetTitle("Iterations");Shift2XPerIteration->GetYaxis()->SetTitle("Shift value");Shift2XPerIteration->SetTitle("Shift Per Iteration in Tracker2 x coordinate");
  
  map<int, TH2F*> Tracker1HitmapPerRotation;
  map<int, TH2F*> Tracker2HitmapPerRotation;
  map<int, TH2F*> Tracker3HitmapPerRotation;
  map<int, TH2F*> Tracker4HitmapPerRotation;

  float prealignedX =  Tracker1HitlocVec[0];
  float prealignedY =  Tracker1HitlocVec[1];   
  while(1){    

  
    Iterator++;

    
    char name2X[15];sprintf(name2X,"Pos_g2xcl_%i",Iterator); char name2Y[15];sprintf(name2Y,"Pos_g2ycl_%i",Iterator);
    char name3X[15];sprintf(name3X,"Pos_g3xcl_%i",Iterator); char name3Y[15];sprintf(name3Y,"Pos_g3ycl_%i",Iterator);
    char name1X[15];sprintf(name1X,"Pos_g1xcl_%i",Iterator); char name1Y[15];sprintf(name1Y,"Pos_g1xcl_%i",Iterator);
    char name4X[15];sprintf(name4X,"Pos_g4xcl_%i",Iterator); char name4Y[15];sprintf(name4Y,"Pos_g4xcl_%i",Iterator);

    
    TH1F* h_Pos_g2xcl = new TH1F(name2X,"",300,-110,110); h_Pos_g2xcl->SetXTitle("Cluster position [mm]"); h_Pos_g2xcl->SetYTitle("Frequency");h_Pos_g2xcl->SetLabelSize(0.045,"XY");h_Pos_g2xcl->SetTitleSize(0.045,"XY");
    TH1F* h_Pos_g2ycl = new TH1F(name2Y,"",300,-110,110); h_Pos_g2ycl->SetXTitle(" Cluster position [mm]"); h_Pos_g2ycl->SetYTitle("Frequency");h_Pos_g2ycl->SetLabelSize(0.045,"XY");h_Pos_g2ycl->SetTitleSize(0.045,"XY");  
    TH1F* h_Pos_g1xcl = new TH1F(name1X,"",300,-110,110); h_Pos_g1xcl->SetXTitle("Cluster position [mm]"); h_Pos_g1xcl->SetYTitle("Frequency");h_Pos_g1xcl->SetLabelSize(0.045,"XY");h_Pos_g1xcl->SetTitleSize(0.045,"XY");
    TH1F* h_Pos_g1ycl = new TH1F(name1Y,"",300,-110,110); h_Pos_g1ycl->SetXTitle("Cluster position [mm]"); h_Pos_g1ycl->SetYTitle("Frequency");h_Pos_g1ycl->SetLabelSize(0.045,"XY");h_Pos_g1ycl->SetTitleSize(0.045,"XY");  
    TH1F* h_Pos_g3xcl = new TH1F(name3X,"",300,-110,110); h_Pos_g3xcl->SetXTitle("Cluster position [mm]"); h_Pos_g3xcl->SetYTitle("Frequency");h_Pos_g3xcl->SetLabelSize(0.045,"XY");h_Pos_g3xcl->SetTitleSize(0.045,"XY");
    TH1F* h_Pos_g3ycl = new TH1F(name3Y,"",300,-110,110); h_Pos_g3ycl->SetXTitle(" Cluster position [mm]"); h_Pos_g3ycl->SetYTitle("Frequency");h_Pos_g3ycl->SetLabelSize(0.045,"XY");h_Pos_g3ycl->SetTitleSize(0.045,"XY");
    TH1F* h_Pos_g4xcl = new TH1F(name4X,"",300,-110,110); h_Pos_g4xcl->SetXTitle("Cluster position [mm]"); h_Pos_g4xcl->SetYTitle("Frequency");h_Pos_g4xcl->SetLabelSize(0.045,"XY");h_Pos_g4xcl->SetTitleSize(0.045,"XY");
    TH1F* h_Pos_g4ycl = new TH1F(name4Y,"",300,-110,110); h_Pos_g4ycl->SetXTitle("Cluster position [mm]"); h_Pos_g4ycl->SetYTitle("Frequency");h_Pos_g4ycl->SetLabelSize(0.045,"XY");h_Pos_g4ycl->SetTitleSize(0.045,"XY");
          
   
    char nameRes2X[30];sprintf(nameRes2X,"residual_Tracker2x_%i",Iterator);char nameRes2Y[30];sprintf(nameRes2Y,"residual_Tracker2y_%i",Iterator);
    char nameRes3X[30];sprintf(nameRes3X,"residual_Tracker3x_%i",Iterator);char nameRes3Y[30];sprintf(nameRes3Y,"residual_Tracker3y_%i",Iterator);
    char nameRes1X[30];sprintf(nameRes1X,"residual_Tracker1x_%i",Iterator);char nameRes1Y[30];sprintf(nameRes1Y,"residual_Tracker1y_%i",Iterator);
    char nameRes4X[30];sprintf(nameRes4X,"residual_Tracker4x_%i",Iterator);char nameRes4Y[30];sprintf(nameRes4Y,"residual_Tracker4y_%i",Iterator);


    char nameRot1[30];sprintf(nameRot1,"Rotation_Tracker1_%i",Iterator);
    char nameRot2[30];sprintf(nameRot2,"Rotation_Tracker2_%i",Iterator);
    char nameRot3[30];sprintf(nameRot3,"Rotation_Tracker3_%i",Iterator);
    char nameRot4[30];sprintf(nameRot4,"Rotation_Tracker4_%i",Iterator);
    char nameRotCanvas[30];sprintf(nameRotCanvas,"Rotation_canvas_%i",Iterator);

    char titleRot1[30];sprintf(titleRot1,"Rotation_Tracker1_%i",Iterator);
    char titleRot2[30];sprintf(titleRot2,"Rotation_Tracker2_%i",Iterator);
    char titleRot3[30];sprintf(titleRot3,"Rotation_Tracker3_%i",Iterator);
    char titleRot4[30];sprintf(titleRot4,"Rotation_Tracker4_%i",Iterator);
    char titleRotCanvas[30];sprintf(titleRotCanvas,"Rotation_canvas_%i",Iterator);
    
    int resbounds = 100;
    int bincountMultiplier = 80;
    TH1F* h_residual_Tracker2x = new TH1F(nameRes2X,"Tracker 2 x-Residual after 281 Iterations", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker2x->SetXTitle("Residual [mm]"); h_residual_Tracker2x->SetYTitle("Counts/0.025mm");h_residual_Tracker2x->SetLabelSize(0.045,"XY");h_residual_Tracker2x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker2y = new TH1F(nameRes2Y,"", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker2y->SetXTitle("Residual [mm]"); h_residual_Tracker2y->SetYTitle("Counts/0.025mm");h_residual_Tracker2y->SetLabelSize(0.045,"XY");h_residual_Tracker2y->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker3x = new TH1F(nameRes3X,"Tracker 3 x-Residual after 281 Iterations", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker3x->SetXTitle("Residual [mm]"); h_residual_Tracker3x->SetYTitle("Counts/0.01mm");h_residual_Tracker3x->SetLabelSize(0.045,"XY");h_residual_Tracker3x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker3y = new TH1F(nameRes3Y,"", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker3y->SetXTitle("Residual [mm]"); h_residual_Tracker3y->SetYTitle("Counts/0.025mm");h_residual_Tracker3y->SetLabelSize(0.045,"XY");h_residual_Tracker3y->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker1x = new TH1F(nameRes1X,"Tracker 1 x-Residual after 281 Iterations", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker1x->SetXTitle("Residual [mm]"); h_residual_Tracker1x->SetYTitle("Counts/0.01mm");h_residual_Tracker1x->SetLabelSize(0.045,"XY");h_residual_Tracker1x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker1y = new TH1F(nameRes1Y,"", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker1y->SetXTitle("Residual [mm]"); h_residual_Tracker1y->SetYTitle("Counts/0.025mm");h_residual_Tracker1y->SetLabelSize(0.045,"XY");h_residual_Tracker1y->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker4x = new TH1F(nameRes4X,"Tracker 4 x-Residual after 281 Iterations", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker4x->SetXTitle("Residual [mm]"); h_residual_Tracker4x->SetYTitle("Counts/0.01mm");h_residual_Tracker4x->SetLabelSize(0.045,"XY");h_residual_Tracker4x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker4y = new TH1F(nameRes4Y,"", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker4y->SetXTitle("Residual [mm]"); h_residual_Tracker4y->SetYTitle("Counts/0.025mm");h_residual_Tracker4y->SetLabelSize(0.045,"XY");h_residual_Tracker4y->SetTitleSize(0.045,"XY");

    /*
    Tracker1HitmapPerRotation[Iterator] = new TH2F(nameRes1X, "", 100, -50, 50, 100, -50, 50);
    Tracker3HitmapPerRotation[Iterator] = new TH2F(nameRot3, "", 100, -50, 50, 100, -50, 50);
    Tracker2HitmapPerRotation[Iterator] = new TH2F(nameRot2, "", 100, -50, 50, 100, -50, 50);
    Tracker4HitmapPerRotation[Iterator] = new TH2F(nameRot4, "", 100, -50, 50, 100, -50, 50);
    */
    //TH1F* DiffBtwnT12Y = new TH1F(nameRes2X,"Difference between Trackers 3 and 4 Y coordinate", 100,-5,5); DiffBtwnT12Y->SetXTitle("Difference between Trackers 3 and 4 Y coordinate [mm]"); DiffBtwnT12Y->SetYTitle("Counts/0.01mm");DiffBtwnT12Y->SetLabelSize(0.045,"XY");DiffBtwnT12Y->SetTitleSize(0.045,"XY");
    /*
    cout << "Tracker 1x shifted by: " << Shift_T1x << endl;
    Shift1XPerIteration->SetPoint(Iterator-1, Iterator, Shift_T1x);
    cout << "Tracker 2x shifted by: " << Shift_T2x << endl;
    Shift2XPerIteration->SetPoint(Iterator-1, Iterator, Shift_T2x);
    */
    
    for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){//shift all of the XY hit coordinates

      Tracker1HitlocVec[i] = Tracker1HitlocVec[i] - Shift_T1x;
      //Tracker1xHitlocVec_tmp = Tracker1HitlocVec[i];
	  
      Tracker2HitlocVec[i] = Tracker2HitlocVec[i] - Shift_T2x;
      // Tracker2xHitlocVec_tmp = Tracker2HitlocVec[i];
      //cout << "Shifted 2X coordinate: " << Tracker2xHitlocVec_tmp << endl;
	  
      Tracker3HitlocVec[i] = Tracker3HitlocVec[i] - Shift_T3x;
      //Tracker3xHitlocVec_tmp = Tracker3HitlocVec[i];

      Tracker4HitlocVec[i] = Tracker4HitlocVec[i] - Shift_T4x;
      //Tracker4xHitlocVec_tmp = Tracker4HitlocVec[i];

      h_Pos_g1xcl->Fill(Tracker1HitlocVec[i]);////////////////////////////////////put this in the function below this one :)
      h_Pos_g2xcl->Fill(Tracker2HitlocVec[i]);
      h_Pos_g3xcl->Fill(Tracker3HitlocVec[i]);
      h_Pos_g4xcl->Fill(Tracker4HitlocVec[i]);
            
      i++;//move to y components

      Tracker1HitlocVec[i] = Tracker1HitlocVec[i] - Shift_T1y;
      //Tracker1yHitlocVec_tmp = Tracker1HitlocVec[i];
	  
      Tracker2HitlocVec[i] = Tracker2HitlocVec[i] - Shift_T2y;
     
      
     
      
      //Tracker2yHitlocVec_tmp = Tracker2HitlocVec[i];//need these non-rotated values for rotating the xy components next
      //cout << "Shifted 2Y coordinate: " << Tracker2yHitlocVec_tmp << endl;
	  
      Tracker3HitlocVec[i] = Tracker3HitlocVec[i] - Shift_T3y;
      //Tracker3yHitlocVec_tmp = Tracker3HitlocVec[i];
	  
      Tracker4HitlocVec[i] = Tracker4HitlocVec[i] - Shift_T4y;
      //Tracker4yHitlocVec_tmp = Tracker4HitlocVec[i];
      //DiffBtwnT12Y->Fill(Tracker4HitlocVec[i] - Tracker3HitlocVec[i]);
      
      h_Pos_g1ycl->Fill(Tracker1HitlocVec[i]);
      h_Pos_g2ycl->Fill(Tracker2HitlocVec[i]);
      h_Pos_g3ycl->Fill(Tracker3HitlocVec[i]);
      h_Pos_g4ycl->Fill(Tracker4HitlocVec[i]);

      //Tracker2HitmapPerRotation[Iterator]->Fill(Tracker2HitlocVec[i-1], Tracker2HitlocVec[i]);
      //Tracker1HitmapPerRotation[Iterator]->Fill(Tracker1HitlocVec[i-1], Tracker1HitlocVec[i]);
      
      i++;
    }

    /*
    for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){//rotate too

      float Tracker2HitlocVecx_tmp = Tracker2HitlocVec[i]; 	
      Tracker2HitlocVec[i] = Tracker2HitlocVec[i]*cos(REf2ang) - Tracker2HitlocVec[i+1]*sin(REf2ang);	  
      
      float Tracker3HitlocVecx_tmp = Tracker3HitlocVec[i]; 	
      Tracker3HitlocVec[i] = Tracker3HitlocVec[i]*cos(REf3ang) - Tracker3HitlocVec[i+1]*sin(REf3ang);	  

      float Tracker4HitlocVecx_tmp = Tracker4HitlocVec[i]; 	
      Tracker4HitlocVec[i] = Tracker4HitlocVec[i]*cos(REf4ang) - Tracker4HitlocVec[i+1]*sin(REf4ang);	  
      
      
      i++;//move to y components
	
      Tracker2HitlocVec[i] = Tracker2HitlocVecx_tmp*sin(REf2ang) + Tracker2HitlocVec[i]*cos(REf2ang);	  
      
      Tracker3HitlocVec[i] = Tracker3HitlocVecx_tmp*sin(REf3ang) + Tracker3HitlocVec[i]*cos(REf3ang);	  
      Tracker4HitlocVec[i] = Tracker4HitlocVecx_tmp*sin(REf4ang) + Tracker4HitlocVec[i]*cos(REf4ang);	  
 
      
      i++;
      
    }
    */
    /*
    TCanvas* yea = new TCanvas("","", 500, 500);
    DiffBtwnT12Y->Draw();
    yea->Draw();
    break;
    */
    /*
    if(plots){
      TCanvas* TrackerHitmaps = new TCanvas(nameRotCanvas, "", 1000, 500);
    
      TrackerHitmaps->Divide(2,1);
      TrackerHitmaps->cd(1);
      Tracker1HitmapPerRotation[Iterator]->Draw("colz");
      gPad->Update();
	  
      TrackerHitmaps->cd(2);
      Tracker2HitmapPerRotation[Iterator]->Draw("colz");
      gPad->Update();
	
      TrackerHitmaps->Draw();
      gPad->Update();
      //gSystem->ProcessEvents();
      //gSystem->ProcessEvents();
      TrackerHitmaps->SaveAs(nameRotCanvas);
	
    }
    */

    //vector<float> BadEvents;
    //cout << Tracker1HitlocVec.size()/3 << endl;
    //if(Iterator == 1)
    //  BadEvents = ResCut(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec);
    //cout << Tracker1HitlocVec.size()/3 << endl;

    
    auto NewRes = Residuals(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec, BadEvents);

    //break;

    //return NewRes;

    auto Chisquares = ChiSquared(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec, BadEvents);

        
    xChi2PerIteration->SetPoint(Iterator-1, Iterator, Chisquares[0]);
    yChi2PerIteration->SetPoint(Iterator-1, Iterator, Chisquares[1]);
  
    //cout << "Size of Residual vector: " << NewRes.size() << endl;
    
    for(unsigned int i=0; i< NewRes.size(); i++){//Put all residuals into plots

      h_residual_Tracker1x->Fill(NewRes[i]);
      i++;
      h_residual_Tracker1y->Fill(NewRes[i]);
      i++;
      h_residual_Tracker2x->Fill(NewRes[i]);
      i++;
      h_residual_Tracker2y->Fill(NewRes[i]);
      i++;
      h_residual_Tracker3x->Fill(NewRes[i]);
      i++;
      h_residual_Tracker3y->Fill(NewRes[i]);
      i++;
      h_residual_Tracker4x->Fill(NewRes[i]);
      i++;
      h_residual_Tracker4y->Fill(NewRes[i]);
    }
    	
    //return OptimizedXYOffsets; 

    I2GFvalues myValues;
	
    //might need to clone to see the OG histogram if need be!

    //Looking for which residuals converge to 0!
	
    myValues = I2GFmainLoop(h_residual_Tracker1x,1,4,1);                           //converges!	  
    mean_Tracker1xcoordRes = myValues.mean; width_Tracker1xcoordRes=myValues.sigma;
    cout << "Mean Residual in 1x coordinate: " << mean_Tracker1xcoordRes << endl;
    cout << "Residual width in 1x coordinate: " << width_Tracker1xcoordRes << endl;
    
    myValues = I2GFmainLoop(h_residual_Tracker1y,1,4,1);	                        
    mean_Tracker1ycoordRes = myValues.mean; width_Tracker1ycoordRes=myValues.sigma;
    //mean_Tracker1ycoordRes = h_residual_Tracker1y->GetMean(); //sigmaEta5=myValues.sigma;
    cout << "Mean Residual in 1y coordinate: " << mean_Tracker1ycoordRes << endl;
    cout << "Residual width in 1y coordinate: " << width_Tracker1ycoordRes << endl;
    
    myValues = I2GFmainLoop(h_residual_Tracker2x,1,4,1);                               //converges!	  
    mean_Tracker2xcoordRes = myValues.mean;
    cout << "Mean Residual in 2x coordinate: " << mean_Tracker2xcoordRes << endl;

    myValues = I2GFmainLoop(h_residual_Tracker2y,1,4,1);	                            //breaks :(... im sensing a pattern...
    //mean_Tracker2ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
    mean_Tracker2ycoordRes = h_residual_Tracker2y->GetMean(); //sigmaEta5=myValues.    
    cout << "Mean Residual in 2y coordinate: " << mean_Tracker2ycoordRes << endl;
    
    myValues = I2GFmainLoop(h_residual_Tracker3x,1,4,1);
    mean_Tracker3xcoordRes = myValues.mean;	
    cout << "Mean Residual in 3x coordinate: " << mean_Tracker3xcoordRes << endl;

    myValues = I2GFmainLoop(h_residual_Tracker3y,1,4,1);	  
    mean_Tracker3ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
    cout << "Mean Residual in 3y coordinate: " << mean_Tracker3ycoordRes << endl;

    myValues = I2GFmainLoop(h_residual_Tracker4x,1,4,1);
    mean_Tracker4xcoordRes = myValues.mean;
    cout << "Mean Residual in 4x coordinate: " << mean_Tracker4xcoordRes << endl;

    myValues = I2GFmainLoop(h_residual_Tracker4y,1,10,1);	  
    mean_Tracker4ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
    cout << "Mean Residual in 4y coordinate: " << mean_Tracker4ycoordRes << endl;
    
	
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual1XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker1xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual1YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker1ycoordRes);
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual2XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker2xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual2YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker2ycoordRes);
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual3XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker3xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual3YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker3ycoordRes);
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual4XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker4xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual4YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker4ycoordRes);

	
    if((mean_Tracker1xcoordRes >= -0.005 && mean_Tracker1xcoordRes <= 0.005) && (mean_Tracker1ycoordRes >= -0.005 && mean_Tracker1ycoordRes <= 0.005) && (mean_Tracker2xcoordRes >= -0.005 && mean_Tracker2xcoordRes <= 0.005) && (mean_Tracker2ycoordRes >= -0.005 && mean_Tracker2ycoordRes <= 0.005) && (mean_Tracker3xcoordRes >= -0.005 && mean_Tracker3xcoordRes <= 0.005) && (mean_Tracker3ycoordRes >= -0.005 && mean_Tracker3ycoordRes <= 0.005) && (mean_Tracker4xcoordRes >= -0.005 && mean_Tracker4xcoordRes <= 0.005) && (mean_Tracker4ycoordRes >= -0.005 && mean_Tracker4ycoordRes <= 0.005)){ 
      
      OptimizedXYOffsets.push_back(Total1Xshift); OptimizedXYOffsets.push_back(Total1Yshift);
      OptimizedXYOffsets.push_back(Total2Xshift); OptimizedXYOffsets.push_back(Total2Yshift);
      OptimizedXYOffsets.push_back(Total3Xshift); OptimizedXYOffsets.push_back(Total3Yshift);
      OptimizedXYOffsets.push_back(Total4Xshift); OptimizedXYOffsets.push_back(Total4Yshift);

      cout << "Pre Aligment first X value: " << prealignedX << endl;
      cout << "Pre Aligment first Y value: " << prealignedY << endl;
           
      cout << "PostAligment first X value: " << Tracker1HitlocVec[0] << endl;
      cout << "PostAligment first Y value: " << Tracker1HitlocVec[1] << endl;
    
      cout<<"find it...iterating "<< Iterator <<" times."<<endl;
      if(plots){
	/*
	TCanvas* TrackerHits = new TCanvas(name2X, "", 2000, 1000);
	TrackerHits->Divide(4,2);

	//int rangevals = 1;
	TrackerHits->cd(1);
	//h_residual_Tracker1x->SetTitle("Tracker1x residual");
	//h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g1xcl->Draw();
	TrackerHits->cd(5);
	//h_residual_Tracker1y->SetTitle("Tracker1y residual");
	//h_residual_Tracker1y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g1ycl->Draw();
	TrackerHits->cd(2);
	//h_residual_Tracker2x->SetTitle("Tracker2x residual");
	//h_residual_Tracker2x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g2xcl->Draw();
	TrackerHits->cd(6);
	//h_residual_Tracker2y->SetTitle("Tracker2y residual");
	//h_residual_Tracker2y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g2ycl->Draw();
	TrackerHits->cd(3);
	//h_residual_Tracker3x->SetTitle("Tracker3x residual");
	//h_residual_Tracker3x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g3xcl->Draw();
	TrackerHits->cd(7);
	//h_residual_Tracker3y->SetTitle("Tracker3y residual");
	//h_residual_Tracker3y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g3ycl->Draw();
	TrackerHits->cd(4);
	//h_residual_Tracker4x->SetTitle("Tracker4x residual");
	//h_residual_Tracker4x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g4xcl->Draw();
	TrackerHits->cd(8);
	//h_residual_Tracker4y->SetTitle("Tracker4y residual");
	//h_residual_Tracker4y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g4ycl->Draw();
	  
	gPad->Update();
	  
	TrackerHits->Draw();
	gPad->Update();
	*/
	TCanvas* c1 = new TCanvas("c1","", 700,500);
	c1->cd(1);
	Residual1XPerIteration->SetTitle("Mean XY Residuals of Trackers 1-4");
	Residual1XPerIteration->SetMarkerStyle(20);
	Residual1XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual1XPerIteration->Draw("AP");
	Residual1YPerIteration->SetMarkerStyle(24);
	Residual1YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual1YPerIteration->Draw("Psame");
	Residual2XPerIteration->SetMarkerStyle(21);
	Residual2XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual2XPerIteration->Draw("Psame");
	Residual2YPerIteration->SetMarkerStyle(25);
	Residual2YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual2YPerIteration->Draw("Psame");
	Residual3XPerIteration->SetMarkerStyle(22);	  
	Residual3XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual3XPerIteration->Draw("Psame");
	Residual3YPerIteration->SetMarkerStyle(26);
	Residual3YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual3YPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(23);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual4XPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(27);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual4YPerIteration->Draw("Psame");

	auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
	legend->AddEntry(Residual1XPerIteration, "Tracker 1X", "P");
	legend->AddEntry(Residual1YPerIteration, "Tracker 1Y", "P");
	legend->AddEntry(Residual2XPerIteration, "Tracker 2X", "P");
	legend->AddEntry(Residual2YPerIteration, "Tracker 2Y", "P");
	legend->AddEntry(Residual3XPerIteration, "Tracker 3X", "P");
	legend->AddEntry(Residual3YPerIteration, "Tracker 3Y", "P");
	legend->AddEntry(Residual4XPerIteration, "Tracker 4X", "P");
	legend->AddEntry(Residual4YPerIteration, "Tracker 4Y", "P");

	legend->Draw();



	c1->Draw();
	
	TCanvas* ChiSquareData = new TCanvas("", "", 1000, 500);
	ChiSquareData->Divide(2,1);
	  
	ChiSquareData->cd(1);
	xChi2PerIteration->GetYaxis()->SetRangeUser(0, 0.6);
	xChi2PerIteration->Draw("A*");	 
	ChiSquareData->cd(2);
	yChi2PerIteration->GetYaxis()->SetRangeUser(0, 0.6);
	yChi2PerIteration->Draw("A*");

	ChiSquareData->SaveAs("Aligment1_Chisquared.png");

		//IEEE Conference figure
	TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1100, 500);
	TrackerRes->Divide(2,1);

	int rangevals = 1;
	TrackerRes->cd(1);
	h_residual_Tracker1x->SetTitle("Tracker1x residual");
	h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	//h_residual_Tracker1x->GetPad()->SetBorderSize(2);
	
	h_residual_Tracker1x->Draw();
	TrackerRes->cd(2);
	h_residual_Tracker1y->SetTitle("Tracker1y residual");
	h_residual_Tracker1y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker1y->Draw();
	/*
	TrackerRes->cd(2);
       
	h_residual_Tracker2x->SetTitle("Tracker2x residual");
	h_residual_Tracker2x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker2x->Draw();
	TrackerRes->cd(6);
	h_residual_Tracker2y->SetTitle("Tracker2y residual");
	h_residual_Tracker2y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker2y->Draw();
	TrackerRes->cd(3);
	h_residual_Tracker3x->SetTitle("Tracker3x residual");
	h_residual_Tracker3x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker3x->Draw();
	TrackerRes->cd(7);
	h_residual_Tracker3y->SetTitle("Tracker3y residual");
	h_residual_Tracker3y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker3y->Draw();
	TrackerRes->cd(4);
	h_residual_Tracker4x->SetTitle("Tracker4x residual");
	h_residual_Tracker4x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker4x->Draw();
	TrackerRes->cd(8);
	h_residual_Tracker4y->SetTitle("Tracker4y residual");
	h_residual_Tracker4y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker4y->Draw();
	*/
	gPad->Update();
	  
	TrackerRes->Draw();
	gPad->Update();
	

      }
      break;
    }
    
    if(Iterator >= 100){	  

      OptimizedXYOffsets.push_back(Total1Xshift); OptimizedXYOffsets.push_back(Total1Yshift);
      OptimizedXYOffsets.push_back(Total2Xshift); OptimizedXYOffsets.push_back(Total2Yshift);
      OptimizedXYOffsets.push_back(Total3Xshift); OptimizedXYOffsets.push_back(Total3Yshift);
      OptimizedXYOffsets.push_back(Total4Xshift); OptimizedXYOffsets.push_back(Total4Yshift);

      if(plots){
	/*
	TCanvas* TrackerHits = new TCanvas(name2X, "", 2000, 1000);
	TrackerHits->Divide(4,2);

	//int rangevals = 1;
	TrackerHits->cd(1);
	//h_residual_Tracker1x->SetTitle("Tracker1x residual");
	//h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g1xcl->Draw();
	TrackerHits->cd(5);
	//h_residual_Tracker1y->SetTitle("Tracker1y residual");
	//h_residual_Tracker1y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g1ycl->Draw();
	TrackerHits->cd(2);
	//h_residual_Tracker2x->SetTitle("Tracker2x residual");
	//h_residual_Tracker2x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g2xcl->Draw();
	TrackerHits->cd(6);
	//h_residual_Tracker2y->SetTitle("Tracker2y residual");
	//h_residual_Tracker2y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g2ycl->Draw();
	TrackerHits->cd(3);
	//h_residual_Tracker3x->SetTitle("Tracker3x residual");
	//h_residual_Tracker3x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g3xcl->Draw();
	TrackerHits->cd(7);
	//h_residual_Tracker3y->SetTitle("Tracker3y residual");
	//h_residual_Tracker3y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g3ycl->Draw();
	TrackerHits->cd(4);
	//h_residual_Tracker4x->SetTitle("Tracker4x residual");
	//h_residual_Tracker4x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g4xcl->Draw();
	TrackerHits->cd(8);
	//h_residual_Tracker4y->SetTitle("Tracker4y residual");
	//h_residual_Tracker4y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g4ycl->Draw();
	  
	gPad->Update();
	  
	TrackerHits->Draw();
	gPad->Update();
	*/
	
	TCanvas* c1 = new TCanvas("c1","", 700,500);
	c1->cd(1);
	Residual1XPerIteration->SetTitle("Mean XY Residuals of Trackers 1-4");
	Residual1XPerIteration->SetMarkerStyle(20);
	Residual1XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual1XPerIteration->Draw("AP");
	Residual1YPerIteration->SetMarkerStyle(24);
	Residual1YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual1YPerIteration->Draw("Psame");
	Residual2XPerIteration->SetMarkerStyle(21);
	Residual2XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual2XPerIteration->Draw("Psame");
	Residual2YPerIteration->SetMarkerStyle(25);
	Residual2YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual2YPerIteration->Draw("Psame");
	Residual3XPerIteration->SetMarkerStyle(22);	  
	Residual3XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual3XPerIteration->Draw("Psame");
	Residual3YPerIteration->SetMarkerStyle(26);
	Residual3YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual3YPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(23);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual4XPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(27);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual4YPerIteration->Draw("Psame");

	auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
	legend->AddEntry(Residual1XPerIteration, "Tracker 1X", "P");
	legend->AddEntry(Residual1YPerIteration, "Tracker 1Y", "P");
	legend->AddEntry(Residual2XPerIteration, "Tracker 2X", "P");
	legend->AddEntry(Residual2YPerIteration, "Tracker 2Y", "P");
	legend->AddEntry(Residual3XPerIteration, "Tracker 3X", "P");
	legend->AddEntry(Residual3YPerIteration, "Tracker 3Y", "P");
	legend->AddEntry(Residual4XPerIteration, "Tracker 4X", "P");
	legend->AddEntry(Residual4YPerIteration, "Tracker 4Y", "P");

	legend->Draw();



	c1->Draw();
	
	TCanvas* ChiSquareData = new TCanvas("", "", 1000, 500);
	ChiSquareData->Divide(2,1);
	  
	ChiSquareData->cd(1);
	xChi2PerIteration->GetYaxis()->SetRangeUser(0, 0.6);
	xChi2PerIteration->Draw("A*");	 
	ChiSquareData->cd(2);
	yChi2PerIteration->GetYaxis()->SetRangeUser(0, 0.6);
	yChi2PerIteration->Draw("A*");

	ChiSquareData->SaveAs("Aligment1_Chisquared.png");
	
	
	//IEEE Conference figure
	TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 2000, 1000);
	TrackerRes->Divide(4,2);

	int rangevals = 1;
	TrackerRes->cd(1);
	h_residual_Tracker1x->SetTitle("Tracker1x residual");
	h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker1x->Draw();
	TrackerRes->cd(5);
	h_residual_Tracker1y->SetTitle("Tracker1y residual");
	h_residual_Tracker1y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker1y->Draw();
	TrackerRes->cd(2);
	h_residual_Tracker2x->SetTitle("Tracker2x residual");
	h_residual_Tracker2x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker2x->Draw();
	TrackerRes->cd(6);
	h_residual_Tracker2y->SetTitle("Tracker2y residual");
	h_residual_Tracker2y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker2y->Draw();
	TrackerRes->cd(3);
	h_residual_Tracker3x->SetTitle("Tracker3x residual");
	h_residual_Tracker3x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker3x->Draw();
	TrackerRes->cd(7);
	h_residual_Tracker3y->SetTitle("Tracker3y residual");
	h_residual_Tracker3y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker3y->Draw();
	TrackerRes->cd(4);
	h_residual_Tracker4x->SetTitle("Tracker4x residual");
	h_residual_Tracker4x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker4x->Draw();
	TrackerRes->cd(8);
	h_residual_Tracker4y->SetTitle("Tracker4y residual");
	h_residual_Tracker4y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker4y->Draw();
	
	gPad->Update();
	  
	TrackerRes->Draw();
	gPad->Update();
      }            
      break;
    }

	    
    float Shift_scale_Factor = -0.1;
	
    Shift_T1x = Shift_scale_Factor*mean_Tracker1xcoordRes; Shift_T1y = Shift_scale_Factor*mean_Tracker1ycoordRes;
    Shift_T2x = Shift_scale_Factor*mean_Tracker2xcoordRes; Shift_T2y = Shift_scale_Factor*mean_Tracker2ycoordRes;
    Shift_T3x = Shift_scale_Factor*mean_Tracker3xcoordRes; Shift_T3y = Shift_scale_Factor*mean_Tracker3ycoordRes;
    Shift_T4x = Shift_scale_Factor*mean_Tracker4xcoordRes; Shift_T4y = Shift_scale_Factor*mean_Tracker4ycoordRes;

    Total1Xshift += Shift_T1x;
    Total1Yshift += Shift_T1y;
    Total2Xshift += Shift_T2x;
    Total2Yshift += Shift_T2y;
    Total3Xshift += Shift_T3x;
    Total3Yshift += Shift_T3y;
    Total4Xshift += Shift_T4x;
    Total4Yshift += Shift_T4y;
    

    /*
    Total1Xshift += mean_Tracker1xcoordRes;
    Total1Yshift += mean_Tracker1ycoordRes;
    Total2Xshift += mean_Tracker2xcoordRes;
    Total2Yshift += mean_Tracker2ycoordRes;
    Total3Xshift += mean_Tracker3xcoordRes;
    Total3Yshift += mean_Tracker3ycoordRes;
    Total4Xshift += mean_Tracker4xcoordRes;
    Total4Yshift += mean_Tracker4ycoordRes;
    */
    
    delete h_residual_Tracker1x;
    delete h_residual_Tracker1y;
    delete h_residual_Tracker2x;
    delete h_residual_Tracker2y;
    delete h_residual_Tracker3x;
    delete h_residual_Tracker3y;
    delete h_residual_Tracker4x;
    delete h_residual_Tracker4y;
  
  }
 
  return OptimizedXYOffsets;

}

//vector<float> Fitparams;

vector<float> Residuals(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> BadEvents){
  
  //vector format: (x1, y1, z1, x2, y2, z2,...)
  //coordinate, eventnum (x1)

  /*
  bool firsttime=true;
  if(Fitparams.empty() == 0){firsttime=false;}
  cout << firsttime << endl;
  float MeanxSlope=0;
  float MeanySlope=0;
  float Meanxyint=0;
  float Meanyyint=0;
  */
  vector<float> Res;

  
  if(Tracker1HitlocVec.empty() || Tracker2HitlocVec.empty() || Tracker3HitlocVec.empty() || Tracker4HitlocVec.empty())
    return Res;

  /*
  //if(firsttime){
    TH1F* xslopeplot = new TH1F("xz slopes","xz slopes from fit",400,-0.01,0.01); xslopeplot->SetXTitle("Slope"); xslopeplot->SetYTitle("Frequency");xslopeplot->SetLabelSize(0.045,"XY");xslopeplot->SetTitleSize(0.045,"XY");
    TH1F* yslopeplot = new TH1F("yz slopes","yz slopes from fit",400, -0.01,0.01); yslopeplot->SetXTitle("Slope"); yslopeplot->SetYTitle("Frequency");yslopeplot->SetLabelSize(0.045,"XY");yslopeplot->SetTitleSize(0.045,"XY");
    
    TH1F* xz_yintplot = new TH1F("xz Y intercepts","xz y intercepts from fit",100,-70,70); xz_yintplot->SetXTitle("y intercepts"); xz_yintplot->SetYTitle("Frequency"); xz_yintplot->SetLabelSize(0.045,"XY"); xz_yintplot->SetTitleSize(0.045,"XY");
    TH1F* yz_yintplot = new TH1F("yz Y intercepts","yz y intercepts from fit",100,-70, 70); yz_yintplot->SetXTitle("y intercepts"); yz_yintplot->SetYTitle("Frequency");yz_yintplot->SetLabelSize(0.045,"XY");yz_yintplot->SetTitleSize(0.045,"XY");
    //}

    */
  /*
  TH1F* Calculated2xcl = new TH1F("2X","Tracker2 X coordinate from fit",100,-50,50); Calculated2xcl->SetXTitle("Calculated 2x coordinate [mm]"); Calculated2xcl->SetYTitle("Frequency");Calculated2xcl->SetLabelSize(0.045,"XY");Calculated2xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated2ycl = new TH1F("2Y","Tracker2 Y coordinate from fit",100,-50,50); Calculated2ycl->SetXTitle("Calculated 2y coordinate [mm]"); Calculated2ycl->SetYTitle("Frequency");Calculated2ycl->SetLabelSize(0.045,"XY");Calculated2ycl->SetTitleSize(0.045,"XY");  
  TH1F* Calculated1xcl = new TH1F("1X","Tracker1 X coordinate from fit",100,-50,50); Calculated1xcl->SetXTitle("Calculated 1x coordinate [mm]"); Calculated1xcl->SetYTitle("Frequency");Calculated1xcl->SetLabelSize(0.045,"XY");Calculated1xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated1ycl = new TH1F("1Y","Tracker1 Y coordinate from fit",100,-50,50); Calculated1ycl->SetXTitle("Calculated 1y coordinate [mm]"); Calculated1ycl->SetYTitle("Frequency");Calculated1ycl->SetLabelSize(0.045,"XY");Calculated1ycl->SetTitleSize(0.045,"XY");  
  TH1F* Calculated3xcl = new TH1F("3X","Tracker3 X coordinate from fit",100,-50,50); Calculated3xcl->SetXTitle("Calculated 3x coordinate [mm]"); Calculated3xcl->SetYTitle("Frequency");Calculated3xcl->SetLabelSize(0.045,"XY");Calculated3xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated3ycl = new TH1F("3Y","Tracker3 Y coordinate from fit",100,-50,50); Calculated3ycl->SetXTitle("Calculated 3y coordinate [mm]"); Calculated3ycl->SetYTitle("Frequency");Calculated3ycl->SetLabelSize(0.045,"XY");Calculated3ycl->SetTitleSize(0.045,"XY");
  TH1F* Calculated4xcl = new TH1F("4X","Tracker4 X coordinate from fit",100,-50,50); Calculated4xcl->SetXTitle("Calculated 4x coordinate [mm]"); Calculated4xcl->SetYTitle("Frequency");Calculated4xcl->SetLabelSize(0.045,"XY");Calculated4xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated4ycl = new TH1F("4Y","Tracker4 Y coordinate from fit",100,-50,50); Calculated4ycl->SetXTitle("Calculated 4y coordinate [mm]"); Calculated4ycl->SetYTitle("Frequency");Calculated4ycl->SetLabelSize(0.045,"XY");Calculated4ycl->SetTitleSize(0.045,"XY");
  */
  TGraph* OGVals = new TGraph();
  TGraph* CalcVals = new TGraph();
  TGraph* ResVals = new TGraph();

  TGraph* XResvsY = new TGraph(2);
  TGraph* YResvsX = new TGraph(2);
  
  //vector<float> BadEvents;
  //auto *xTracks = new TMultiGraph();
  //auto *yTracks = new TMultiGraph();
  for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){
    //x1, y1, z1, x2   
    if(find(BadEvents.begin(), BadEvents.end(), i/3) == BadEvents.end()){
      
      //cout << i/3 << endl;
      TGraph* gx = new TGraph();
      TF1* fx = new TF1("line1","pol1");
      TGraph* gy = new TGraph();
      TF1* fy = new TF1("line2","pol1");   
    
  
      //cout << "A" << endl;
      gx->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i]);
      gx->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i]);
      gx->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i]);
      gx->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i]);
    

      gy->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i+1]);
      gy->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i+1]);
      gy->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i+1]);
      gy->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i+1]);
  
   
      //return;
    
      //if (verbose)
      //  cout<<"vPos_g1xcl["<<i<<"] = "<<vPos_g1xcl[i]<<"\tvPos_g2xcl["<<i<<"] = "<<vPos_g2xcl[i]<<"\tvPos_g3xcl["<<i<<"] = "<<vPos_g3xcl[i]<<endl;
	    
      gx->Fit("line1","Q");
    
      gy->Fit("line2","Q");
   
      //gy->Draw("A*");
      gy->SetTitle("Y Coordinates of Hit");
      gy->GetXaxis()->SetTitle("Z coordinate");
      gy->GetYaxis()->SetTitle("Y coordinate");
      //break;
      float interceptx = fx->GetParameter(0);
      //cout << "x Yintercepts: " << interceptx << endl;    
      float slopex     = fx->GetParameter(1);
    
      float intercepty = fy->GetParameter(0);
      //cout << "y Yintercepts: " << intercepty << endl;    
      float slopey = fy->GetParameter(1);
    
      //a bunch of stuff from a different way to do fitting 
      //if(firsttime){
      /*
	yslopeplot->Fill(slopey);
	xz_yintplot->Fill(interceptx);    
	xslopeplot->Fill(slopex);
	yz_yintplot->Fill(intercepty);
	//}
	*/
    
      /*
	if(i <= 2){      
	gx->GetYaxis()->SetRangeUser(5, 35);
	//gx->Draw("A*");
	//float xname = i/3;
	xTracks->Add(gx);
	gy->GetYaxis()->SetRangeUser(25, 55);
	//gy->Draw("A*");
	yTracks->Add(gy);     
	}    
      */
      /*
	if(firsttime){
	TCanvas* Quickplot = new TCanvas("Yea", "Hists of x and y fit parameters", 1000, 1000);
	Quickplot->Divide(2,2);
	Quickplot->cd(1);
	xslopeplot->Draw();
	Quickplot->cd(2);
	yslopeplot->Draw();
	Quickplot->cd(3);
	xz_yintplot->Draw();
	Quickplot->cd(4);
	yz_yintplot->Draw();
	
	Quickplot->SaveAs("Fit_Parameters.png");
	}
	
  
	MeanxSlope = xslopeplot->GetMean();
	cout << "  MeanxSlope: " <<   MeanxSlope << endl;
	Fitparams.push_back(MeanxSlope);
	MeanySlope = yslopeplot->GetMean();
	cout << "  MeanySlope: " <<   MeanySlope << endl;
	Fitparams.push_back(MeanySlope);
	Meanxyint = xz_yintplot->GetMean();
	cout << "  Meanxyint: " <<   Meanxyint << endl;
	Fitparams.push_back(Meanxyint);
	Meanyyint = yz_yintplot->GetMean();
	cout << "  Meanyyint: " <<   Meanyyint << endl;
	Fitparams.push_back(Meanyyint);
      */
    
    
      float Measured_Tracker1x = interceptx + slopex*Tracker1HitlocVec[i+2];
      //cout << "Measured 1x: " << Measured_Tracker1x << endl;
      //Calculated1xcl->Fill(Measured_Tracker1x);
      float Tracker1xRes = Measured_Tracker1x - Tracker1HitlocVec[i];    
      //cout << "1X Coord:: " << Measured_Tracker1x << " - " <<  Tracker1HitlocVec[i] << " = " << Measured_Tracker1x - Tracker1HitlocVec[i] << endl;
      Res.push_back(Tracker1xRes);
    
      float Measured_Tracker1y = intercepty + slopey*Tracker1HitlocVec[i+2];
      ////Calculated1ycl->Fill(Measured_Tracker1y);
      float Tracker1yRes = Measured_Tracker1y - Tracker1HitlocVec[i+1];
      Res.push_back(Tracker1yRes);

   
    
      float Measured_Tracker2x = interceptx + slopex*Tracker2HitlocVec[i+2];
      //Calculated2xcl->Fill(Measured_Tracker2x);
      float Tracker2xRes = Measured_Tracker2x - Tracker2HitlocVec[i];
       
    
      //cout << "2X Coord:: " << Measured_Tracker2x << " - " <<  Tracker2HitlocVec[i] << " = " << Measured_Tracker2x - Tracker2HitlocVec[i] << endl;
      Res.push_back(Tracker2xRes);
      float Measured_Tracker2y = intercepty + slopey*Tracker2HitlocVec[i+2];
      //Calculated2ycl->Fill(Measured_Tracker2y);
      float Tracker2yRes = Measured_Tracker2y - Tracker2HitlocVec[i+1];  
      //cout << "2Y Coord:: " << Measured_Tracker2y << " - " <<  Tracker2HitlocVec[i+1] << " = " << Measured_Tracker2y - Tracker2HitlocVec[i+1] << endl;
      Res.push_back(Tracker2yRes);

      /*    
	    if(Tracker2yRes > 0.275 || Tracker2yRes <= 0.125){
	    Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i);Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i+1);Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i+2);
	    Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i);Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i+1);Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i+2);
	    Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i);Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i+1);Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i+2);
	    Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i);Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i+1);Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i+2);
	    i++;
	    i++;
	    continue;
	    }
      */
      /*
      if(i < 30){
	OGVals->SetPoint(i+1, i/3, Tracker2HitlocVec[i+1]);
	CalcVals->SetPoint(i+1, i/3, Measured_Tracker2y);
	ResVals->SetPoint(i+1, i/3, Tracker2yRes);
      
	//cout << "2y Coord " << i << ":: " << Measured_Tracker2y << " - " <<  Tracker2HitlocVec[i+1] << " = " << Tracker2yRes << endl;
      }
      */
    
      float Measured_Tracker3x = interceptx + slopex*Tracker3HitlocVec[i+2];
      //Calculated3xcl->Fill(Measured_Tracker3x);
      float Tracker3xRes = Measured_Tracker3x - Tracker3HitlocVec[i];
      Res.push_back(Tracker3xRes);
    
      float Measured_Tracker3y = intercepty + slopey*Tracker3HitlocVec[i+2];
      //Calculated3ycl->Fill(Measured_Tracker3y);
      float Tracker3yRes = Measured_Tracker3y - Tracker3HitlocVec[i+1];
      Res.push_back(Tracker3yRes);
       
         
      float Measured_Tracker4x = interceptx + slopex*Tracker4HitlocVec[i+2];
      //Calculated4xcl->Fill(Measured_Tracker4x);
      float Tracker4xRes = Measured_Tracker4x - Tracker4HitlocVec[i];
      Res.push_back(Tracker4xRes);
      float Measured_Tracker4y = intercepty + slopey*Tracker4HitlocVec[i+2];
      //Calculated4ycl->Fill(Measured_Tracker4y);
      float Tracker4yRes = Measured_Tracker4y - Tracker4HitlocVec[i+1];
      Res.push_back(Tracker4yRes);
    
      //XResvsY->SetPoint(i+1, Tracker4HitlocVec[i+1], Tracker4xRes);
      //YResvsX->SetPoint(i+1, Tracker4HitlocVec[i], Tracker4yRes);
    

      delete gx;
      delete fx;
      delete gy;
      delete fy;
    }
    i++;
    i++;
  }


  
  /*
  for(auto i = BadEvents.begin(); i != BadEvents.end(); i++){
    cout << *i << endl;
  }
  */
  //return BadEvents;
  
  
  //char Canvasname[30];sprintf(Canvasname,"Canvas_%i",i);
  //char Filename[30];sprintf(Filename,"Hits_%i.png",i);
  /*
  TCanvas* Locs = new TCanvas("Canvasname", "X and Y residuals vs X and Y", 1000, 500);
  Locs->Divide(2,1);
  Locs->Draw();
  Locs->cd(1);
  cout << "Yes" << endl;
  XResvsY->SetTitle("X Residual vs Y coordinate");
  XResvsY->GetXaxis()->SetTitle("Y coordinate [mm]");
  XResvsY->GetYaxis()->SetTitle("X residual [mm]");
  XResvsY->Draw("A*");
  cout << "no" << endl;
  Locs->cd(2);
  YResvsX->SetTitle("Y Residual vs X coordinate");
  YResvsX->GetXaxis()->SetTitle("X coordinate [mm]");
  YResvsX->GetYaxis()->SetTitle("Y residual [mm]");
  YResvsX->Draw("A*");
  gPad->Update();
  cout << "Before Draw" << endl;
  
  //cout << "nMAybe" << endl;
  Locs->Draw();
  //Locs->SaveAs(".png");     
  */
  
  /*
  TCanvas* Locs = new TCanvas("Canvasname", "Tracks in 1 File", 1000, 500);
  Locs->Divide(2,1);
  Locs->Draw("A");
  Locs->cd(1);
  cout << "Yes" << endl;
  xTracks->Draw("A");
  cout << "no" << endl;
  Locs->cd(2);
  yTracks->Draw("A");
  gPad->Update();
  cout << "Before Draw" << endl;
  
  cout << "nMAybe" << endl;
  
  Locs->SaveAs("Tracks.png");     
  */
  
  //cout << "C" << endl;

  /*
  TCanvas* slops = new TCanvas("slops", "yes", 1000, 500);
  slops->Divide(2,1);
  slops->cd(1);
  xslopeplot->Draw("A*");
  slops->cd(2);
  yslopeplot->Draw("A*");

  slops->Draw();

  slops->SaveAs("SLopes.png");
  */

  /*
  TCanvas* slops = new TCanvas("slops", "", 2000, 1000);
  slops->Divide(4,2);
  slops->cd(1);
  Calculated1xcl->Draw();
  slops->cd(5);
  Calculated1ycl->Draw();
  slops->cd(2);
  Calculated2xcl->Draw();
  slops->cd(6);
  Calculated2ycl->Draw();
  slops->cd(3);
  Calculated3xcl->Draw();
  slops->cd(7);
  Calculated3ycl->Draw();
  slops->cd(4);
  Calculated4xcl->Draw();
  slops->cd(8);
  Calculated4ycl->Draw();

  slops->SaveAs("CalculatedXYxoordinates.png");
  
  slops->Draw();
  */
  
  /*
  TCanvas* slops = new TCanvas("slops", "", 500, 500);
  OGVals->GetYaxis()->SetRangeUser(-10, 90);
  //OGVals->GetXaxis()->SetRangeUser(0, 5000);  
  OGVals->SetMarkerStyle(20);
  OGVals->Draw("AP");
  CalcVals->GetYaxis()->SetRangeUser(-10, 90);
  //CalcVals->GetXaxis()->SetRangeUser(0, 5000);
  CalcVals->SetMarkerColor(3);
  CalcVals->SetMarkerStyle(21);
  CalcVals->Draw("PSame");
  ResVals->GetYaxis()->SetRangeUser(-10, 90);
  //ResVals->GetXaxis()->SetRangeUser(0, 5000);
  ResVals->SetMarkerColor(4);
  ResVals->SetMarkerStyle(22);
  ResVals->Draw("PSame");

  auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
  legend->AddEntry(OGVals, "Original Values", "P");
  legend->AddEntry(CalcVals, "Calculated Values", "P");
  legend->AddEntry(ResVals, "Residuals", "P");
  */
  //legend->Draw();
  //slops->SaveAs("All1Yvalues.png");
  
  //slops->Draw();
  
  
  return Res;//returns vector of the form (resx11, resy11, (other 3 trackers x and y) ,  resx21, resy21, ...) residual of tracker 1x event 1, ..
}


vector<float> Residuals(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec){
  
  //vector format: (x1, y1, z1, x2, y2, z2,...)
  //coordinate, eventnum (x1)

  /*
  bool firsttime=true;
  if(Fitparams.empty() == 0){firsttime=false;}
  cout << firsttime << endl;
  float MeanxSlope=0;
  float MeanySlope=0;
  float Meanxyint=0;
  float Meanyyint=0;
  */
  vector<float> Res;

  
  if(Tracker1HitlocVec.empty() || Tracker2HitlocVec.empty() || Tracker3HitlocVec.empty() || Tracker4HitlocVec.empty())
    return Res;

  /*
  //if(firsttime){
    TH1F* xslopeplot = new TH1F("xz slopes","xz slopes from fit",400,-0.01,0.01); xslopeplot->SetXTitle("Slope"); xslopeplot->SetYTitle("Frequency");xslopeplot->SetLabelSize(0.045,"XY");xslopeplot->SetTitleSize(0.045,"XY");
    TH1F* yslopeplot = new TH1F("yz slopes","yz slopes from fit",400, -0.01,0.01); yslopeplot->SetXTitle("Slope"); yslopeplot->SetYTitle("Frequency");yslopeplot->SetLabelSize(0.045,"XY");yslopeplot->SetTitleSize(0.045,"XY");
    
    TH1F* xz_yintplot = new TH1F("xz Y intercepts","xz y intercepts from fit",100,-70,70); xz_yintplot->SetXTitle("y intercepts"); xz_yintplot->SetYTitle("Frequency"); xz_yintplot->SetLabelSize(0.045,"XY"); xz_yintplot->SetTitleSize(0.045,"XY");
    TH1F* yz_yintplot = new TH1F("yz Y intercepts","yz y intercepts from fit",100,-70, 70); yz_yintplot->SetXTitle("y intercepts"); yz_yintplot->SetYTitle("Frequency");yz_yintplot->SetLabelSize(0.045,"XY");yz_yintplot->SetTitleSize(0.045,"XY");
    //}

    */
  /*
  TH1F* Calculated2xcl = new TH1F("2X","Tracker2 X coordinate from fit",100,-50,50); Calculated2xcl->SetXTitle("Calculated 2x coordinate [mm]"); Calculated2xcl->SetYTitle("Frequency");Calculated2xcl->SetLabelSize(0.045,"XY");Calculated2xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated2ycl = new TH1F("2Y","Tracker2 Y coordinate from fit",100,-50,50); Calculated2ycl->SetXTitle("Calculated 2y coordinate [mm]"); Calculated2ycl->SetYTitle("Frequency");Calculated2ycl->SetLabelSize(0.045,"XY");Calculated2ycl->SetTitleSize(0.045,"XY");  
  TH1F* Calculated1xcl = new TH1F("1X","Tracker1 X coordinate from fit",100,-50,50); Calculated1xcl->SetXTitle("Calculated 1x coordinate [mm]"); Calculated1xcl->SetYTitle("Frequency");Calculated1xcl->SetLabelSize(0.045,"XY");Calculated1xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated1ycl = new TH1F("1Y","Tracker1 Y coordinate from fit",100,-50,50); Calculated1ycl->SetXTitle("Calculated 1y coordinate [mm]"); Calculated1ycl->SetYTitle("Frequency");Calculated1ycl->SetLabelSize(0.045,"XY");Calculated1ycl->SetTitleSize(0.045,"XY");  
  TH1F* Calculated3xcl = new TH1F("3X","Tracker3 X coordinate from fit",100,-50,50); Calculated3xcl->SetXTitle("Calculated 3x coordinate [mm]"); Calculated3xcl->SetYTitle("Frequency");Calculated3xcl->SetLabelSize(0.045,"XY");Calculated3xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated3ycl = new TH1F("3Y","Tracker3 Y coordinate from fit",100,-50,50); Calculated3ycl->SetXTitle("Calculated 3y coordinate [mm]"); Calculated3ycl->SetYTitle("Frequency");Calculated3ycl->SetLabelSize(0.045,"XY");Calculated3ycl->SetTitleSize(0.045,"XY");
  TH1F* Calculated4xcl = new TH1F("4X","Tracker4 X coordinate from fit",100,-50,50); Calculated4xcl->SetXTitle("Calculated 4x coordinate [mm]"); Calculated4xcl->SetYTitle("Frequency");Calculated4xcl->SetLabelSize(0.045,"XY");Calculated4xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated4ycl = new TH1F("4Y","Tracker4 Y coordinate from fit",100,-50,50); Calculated4ycl->SetXTitle("Calculated 4y coordinate [mm]"); Calculated4ycl->SetYTitle("Frequency");Calculated4ycl->SetLabelSize(0.045,"XY");Calculated4ycl->SetTitleSize(0.045,"XY");
  */
  TGraph* OGVals = new TGraph();
  TGraph* CalcVals = new TGraph();
  TGraph* ResVals = new TGraph();

  TGraph* XResvsY = new TGraph(2);
  TGraph* YResvsX = new TGraph(2);
  
  //vector<float> BadEvents;
  //auto *xTracks = new TMultiGraph();
  //auto *yTracks = new TMultiGraph();
  for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){
    //x1, y1, z1, x2   
   
      
    //cout << i/3 << endl;
    TGraph* gx = new TGraph();
    TF1* fx = new TF1("line1","pol1");
    TGraph* gy = new TGraph();
    TF1* fy = new TF1("line2","pol1");   
    
  
    //cout << "A" << endl;
    gx->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i]);
    gx->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i]);
    gx->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i]);
    gx->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i]);
    

    gy->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i+1]);
    gy->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i+1]);
    gy->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i+1]);
    gy->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i+1]);
  
   
    //return;
    
    //if (verbose)
    //  cout<<"vPos_g1xcl["<<i<<"] = "<<vPos_g1xcl[i]<<"\tvPos_g2xcl["<<i<<"] = "<<vPos_g2xcl[i]<<"\tvPos_g3xcl["<<i<<"] = "<<vPos_g3xcl[i]<<endl;
	    
    gx->Fit("line1","Q");
    
    gy->Fit("line2","Q");
   
    //gy->Draw("A*");
    gy->SetTitle("Y Coordinates of Hit");
    gy->GetXaxis()->SetTitle("Z coordinate");
    gy->GetYaxis()->SetTitle("Y coordinate");
    
    float interceptx = fx->GetParameter(0);
    //cout << "x Yintercepts: " << interceptx << endl;    
    float slopex     = fx->GetParameter(1);
    
    float intercepty = fy->GetParameter(0);
    //cout << "y Yintercepts: " << intercepty << endl;    
    float slopey = fy->GetParameter(1);
    
    //a bunch of stuff from a different way to do fitting 
    //if(firsttime){
    /*
      yslopeplot->Fill(slopey);
      xz_yintplot->Fill(interceptx);    
      xslopeplot->Fill(slopex);
      yz_yintplot->Fill(intercepty);
      //}
      */
    
    /*
      if(i <= 2){      
      gx->GetYaxis()->SetRangeUser(5, 35);
      //gx->Draw("A*");
      //float xname = i/3;
      xTracks->Add(gx);
      gy->GetYaxis()->SetRangeUser(25, 55);
      //gy->Draw("A*");
      yTracks->Add(gy);     
      }    
    */
    /*
      if(firsttime){
      TCanvas* Quickplot = new TCanvas("Yea", "Hists of x and y fit parameters", 1000, 1000);
      Quickplot->Divide(2,2);
      Quickplot->cd(1);
      xslopeplot->Draw();
      Quickplot->cd(2);
      yslopeplot->Draw();
      Quickplot->cd(3);
      xz_yintplot->Draw();
      Quickplot->cd(4);
      yz_yintplot->Draw();
	
      Quickplot->SaveAs("Fit_Parameters.png");
      }
	
  
      MeanxSlope = xslopeplot->GetMean();
      cout << "  MeanxSlope: " <<   MeanxSlope << endl;
      Fitparams.push_back(MeanxSlope);
      MeanySlope = yslopeplot->GetMean();
      cout << "  MeanySlope: " <<   MeanySlope << endl;
      Fitparams.push_back(MeanySlope);
      Meanxyint = xz_yintplot->GetMean();
      cout << "  Meanxyint: " <<   Meanxyint << endl;
      Fitparams.push_back(Meanxyint);
      Meanyyint = yz_yintplot->GetMean();
      cout << "  Meanyyint: " <<   Meanyyint << endl;
      Fitparams.push_back(Meanyyint);
    */
    
    
    float Measured_Tracker1x = interceptx + slopex*Tracker1HitlocVec[i+2];
    //cout << "Measured 1x: " << Measured_Tracker1x << endl;
    //Calculated1xcl->Fill(Measured_Tracker1x);
    float Tracker1xRes = Measured_Tracker1x - Tracker1HitlocVec[i];    
    //cout << "1X Coord:: " << Measured_Tracker1x << " - " <<  Tracker1HitlocVec[i] << " = " << Measured_Tracker1x - Tracker1HitlocVec[i] << endl;
    Res.push_back(Tracker1xRes);
    
    float Measured_Tracker1y = intercepty + slopey*Tracker1HitlocVec[i+2];
    ////Calculated1ycl->Fill(Measured_Tracker1y);
    float Tracker1yRes = Measured_Tracker1y - Tracker1HitlocVec[i+1];
    Res.push_back(Tracker1yRes);

   
    
    float Measured_Tracker2x = interceptx + slopex*Tracker2HitlocVec[i+2];
    //Calculated2xcl->Fill(Measured_Tracker2x);
    float Tracker2xRes = Measured_Tracker2x - Tracker2HitlocVec[i];
       
    
    //cout << "2X Coord:: " << Measured_Tracker2x << " - " <<  Tracker2HitlocVec[i] << " = " << Measured_Tracker2x - Tracker2HitlocVec[i] << endl;
    Res.push_back(Tracker2xRes);
    float Measured_Tracker2y = intercepty + slopey*Tracker2HitlocVec[i+2];
    //Calculated2ycl->Fill(Measured_Tracker2y);
    float Tracker2yRes = Measured_Tracker2y - Tracker2HitlocVec[i+1];  
    //cout << "2Y Coord:: " << Measured_Tracker2y << " - " <<  Tracker2HitlocVec[i+1] << " = " << Measured_Tracker2y - Tracker2HitlocVec[i+1] << endl;
    Res.push_back(Tracker2yRes);

    /*    
	  if(Tracker2yRes > 0.275 || Tracker2yRes <= 0.125){
	  Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i);Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i+1);Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i+2);
	  Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i);Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i+1);Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i+2);
	  Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i);Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i+1);Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i+2);
	  Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i);Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i+1);Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i+2);
	  i++;
	  i++;
	  continue;
	  }
    */
    /*
      if(i < 30){
      OGVals->SetPoint(i+1, i/3, Tracker2HitlocVec[i+1]);
      CalcVals->SetPoint(i+1, i/3, Measured_Tracker2y);
      ResVals->SetPoint(i+1, i/3, Tracker2yRes);
      
      //cout << "2y Coord " << i << ":: " << Measured_Tracker2y << " - " <<  Tracker2HitlocVec[i+1] << " = " << Tracker2yRes << endl;
      }
    */
    
    float Measured_Tracker3x = interceptx + slopex*Tracker3HitlocVec[i+2];
    //Calculated3xcl->Fill(Measured_Tracker3x);
    float Tracker3xRes = Measured_Tracker3x - Tracker3HitlocVec[i];
    Res.push_back(Tracker3xRes);
    
    float Measured_Tracker3y = intercepty + slopey*Tracker3HitlocVec[i+2];
    //Calculated3ycl->Fill(Measured_Tracker3y);
    float Tracker3yRes = Measured_Tracker3y - Tracker3HitlocVec[i+1];
    Res.push_back(Tracker3yRes);
       
         
    float Measured_Tracker4x = interceptx + slopex*Tracker4HitlocVec[i+2];
    //Calculated4xcl->Fill(Measured_Tracker4x);
    float Tracker4xRes = Measured_Tracker4x - Tracker4HitlocVec[i];
    Res.push_back(Tracker4xRes);
    float Measured_Tracker4y = intercepty + slopey*Tracker4HitlocVec[i+2];
    //Calculated4ycl->Fill(Measured_Tracker4y);
    float Tracker4yRes = Measured_Tracker4y - Tracker4HitlocVec[i+1];
    Res.push_back(Tracker4yRes);
    
    //XResvsY->SetPoint(i+1, Tracker4HitlocVec[i+1], Tracker4xRes);
    //YResvsX->SetPoint(i+1, Tracker4HitlocVec[i], Tracker4yRes);
    

    delete gx;
    delete fx;
    delete gy;
    delete fy;
    
    i++;
    i++;
  }


  
  /*
  for(auto i = BadEvents.begin(); i != BadEvents.end(); i++){
    cout << *i << endl;
  }
  */
  //return BadEvents;
  
  
  //char Canvasname[30];sprintf(Canvasname,"Canvas_%i",i);
  //char Filename[30];sprintf(Filename,"Hits_%i.png",i);
  /*
  TCanvas* Locs = new TCanvas("Canvasname", "X and Y residuals vs X and Y", 1000, 500);
  Locs->Divide(2,1);
  Locs->Draw();
  Locs->cd(1);
  cout << "Yes" << endl;
  XResvsY->SetTitle("X Residual vs Y coordinate");
  XResvsY->GetXaxis()->SetTitle("Y coordinate [mm]");
  XResvsY->GetYaxis()->SetTitle("X residual [mm]");
  XResvsY->Draw("A*");
  cout << "no" << endl;
  Locs->cd(2);
  YResvsX->SetTitle("Y Residual vs X coordinate");
  YResvsX->GetXaxis()->SetTitle("X coordinate [mm]");
  YResvsX->GetYaxis()->SetTitle("Y residual [mm]");
  YResvsX->Draw("A*");
  gPad->Update();
  cout << "Before Draw" << endl;
  
  //cout << "nMAybe" << endl;
  Locs->Draw();
  //Locs->SaveAs(".png");     
  */
  
  /*
  TCanvas* Locs = new TCanvas("Canvasname", "Tracks in 1 File", 1000, 500);
  Locs->Divide(2,1);
  Locs->Draw("A");
  Locs->cd(1);
  cout << "Yes" << endl;
  xTracks->Draw("A");
  cout << "no" << endl;
  Locs->cd(2);
  yTracks->Draw("A");
  gPad->Update();
  cout << "Before Draw" << endl;
  
  cout << "nMAybe" << endl;
  
  Locs->SaveAs("Tracks.png");     
  */
  
  //cout << "C" << endl;

  /*
  TCanvas* slops = new TCanvas("slops", "yes", 1000, 500);
  slops->Divide(2,1);
  slops->cd(1);
  xslopeplot->Draw("A*");
  slops->cd(2);
  yslopeplot->Draw("A*");

  slops->Draw();

  slops->SaveAs("SLopes.png");
  */

  /*
  TCanvas* slops = new TCanvas("slops", "", 2000, 1000);
  slops->Divide(4,2);
  slops->cd(1);
  Calculated1xcl->Draw();
  slops->cd(5);
  Calculated1ycl->Draw();
  slops->cd(2);
  Calculated2xcl->Draw();
  slops->cd(6);
  Calculated2ycl->Draw();
  slops->cd(3);
  Calculated3xcl->Draw();
  slops->cd(7);
  Calculated3ycl->Draw();
  slops->cd(4);
  Calculated4xcl->Draw();
  slops->cd(8);
  Calculated4ycl->Draw();

  slops->SaveAs("CalculatedXYxoordinates.png");
  
  slops->Draw();
  */
  
  /*
  TCanvas* slops = new TCanvas("slops", "", 500, 500);
  OGVals->GetYaxis()->SetRangeUser(-10, 90);
  //OGVals->GetXaxis()->SetRangeUser(0, 5000);  
  OGVals->SetMarkerStyle(20);
  OGVals->Draw("AP");
  CalcVals->GetYaxis()->SetRangeUser(-10, 90);
  //CalcVals->GetXaxis()->SetRangeUser(0, 5000);
  CalcVals->SetMarkerColor(3);
  CalcVals->SetMarkerStyle(21);
  CalcVals->Draw("PSame");
  ResVals->GetYaxis()->SetRangeUser(-10, 90);
  //ResVals->GetXaxis()->SetRangeUser(0, 5000);
  ResVals->SetMarkerColor(4);
  ResVals->SetMarkerStyle(22);
  ResVals->Draw("PSame");

  auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
  legend->AddEntry(OGVals, "Original Values", "P");
  legend->AddEntry(CalcVals, "Calculated Values", "P");
  legend->AddEntry(ResVals, "Residuals", "P");
  */
  //legend->Draw();
  //slops->SaveAs("All1Yvalues.png");
  
  //slops->Draw();
  
  
  return Res;//returns vector of the form (resx11, resy11, (other 3 trackers x and y) ,  resx21, resy21, ...) residual of tracker 1x event 1, ..
}


vector<float> ResCut(vector<float>& Tracker1HitlocVec, vector<float>& Tracker2HitlocVec, vector<float>& Tracker3HitlocVec, vector<float>& Tracker4HitlocVec){


  //this vector: x1, y1, z1, x2, y2, z2, . . 
  //TGraph* OGVals = new TGraph();
  //TGraph* CalcVals = new TGraph();
  //TGraph* ResVals = new TGraph();

  vector<float> T1X;
  vector<float> T1Y;
  vector<float> T2X;
  vector<float> T2Y;
  vector<float> T3X;
  vector<float> T3Y;
  vector<float> T4X;
  vector<float> T4Y;
  float T1Z;
  float T2Z;
  float T3Z;
  float T4Z;

  vector<float> BadEventIDs;
  TH1F* resplot = new TH1F("","Residual", 50,-1,1); resplot->SetXTitle("Residual [mm]"); resplot->SetYTitle("Counts");resplot->SetLabelSize(0.045,"XY");resplot->SetTitleSize(0.045,"XY");
  for(int i=0;i<Tracker1HitlocVec.size();i++){
    
    T1X.push_back(Tracker1HitlocVec.at(i));
    T2X.push_back(Tracker2HitlocVec.at(i));
    T3X.push_back(Tracker3HitlocVec.at(i));
    T4X.push_back(Tracker4HitlocVec.at(i));
    i++;
    T1Y.push_back(Tracker1HitlocVec.at(i));
    T2Y.push_back(Tracker2HitlocVec.at(i));
    T3Y.push_back(Tracker3HitlocVec.at(i));
    T4Y.push_back(Tracker4HitlocVec.at(i));
    i++;
    //cout << i << endl;
    if(i <= 5 ){
      T1Z = Tracker1HitlocVec.at(i);
      T2Z = Tracker2HitlocVec.at(i);
      T3Z = Tracker3HitlocVec.at(i);
      T4Z = Tracker4HitlocVec.at(i);
    }
  }

  
  float inter=0;
  //cout << T1Z << endl;
  //cout << T2Z << endl;
  //cout << T3Z << endl;
  //cout << T4Z << endl;
  //return;
  
  for(int i=0; i< T1X.size(); i++){
    //cout << i << endl;
    TGraph* gx = new TGraph();
    TF1* fx = new TF1("line1","pol1", 0, 1600);
    TGraph* gy = new TGraph();
    TF1* fy = new TF1("line2","pol1", 0, 1600);   
    
  
    //cout << "A" << endl;
    
    gx->SetPoint(0, T1Z, T1X.at(i));
    gx->SetPoint(1, T2Z, T2X.at(i));
    gx->SetPoint(2, T3Z, T3X.at(i));
    gx->SetPoint(3, T4Z, T4X.at(i));
    
    //cout << T2X.at(i) << ", " <<  T2Y.at(i) << endl;
    
    gy->SetPoint(0, T1Z, T1Y.at(i));
    gy->SetPoint(1, T2Z, T2Y.at(i));
    gy->SetPoint(2, T3Z, T3Y.at(i));
    gy->SetPoint(3, T4Z, T4Y.at(i));
  
   
  	    
    gx->Fit("line1","Q");
    
    gy->Fit("line2","Q");
   
    //gy->Draw("A*");
    gy->SetTitle("Y Coordinates of Hit");
    gy->GetXaxis()->SetTitle("Z coordinate");
    gy->GetYaxis()->SetTitle("Y coordinate");
    
    float interceptx = fx->GetParameter(0);
    //cout << "x Yintercepts: " << interceptx << endl;    
    float slopex     = fx->GetParameter(1);
    
    float intercepty = fy->GetParameter(0);
    //cout << "y Yintercepts: " << intercepty << endl;    
    float slopey = fy->GetParameter(1);
    
    
    
    float Measured_Tracker1x = interceptx + slopex*T1Z;
    //cout << "Measured 1x: " << Measured_Tracker1x << endl;
    //Calculated1xcl->Fill(Measured_Tracker1x);
    float Tracker1xRes = Measured_Tracker1x - T1X.at(i);    
    //cout << "1X Coord:: " << Measured_Tracker1x << " - " <<  Tracker1HitlocVec[i] << " = " << Measured_Tracker1x - Tracker1HitlocVec[i] << endl;
    
    float Measured_Tracker1y = intercepty + slopey*T1Z;
    //Calculated1ycl->Fill(Measured_Tracker1y);
    float Tracker1yRes = Measured_Tracker1y - T1Y.at(i);
    resplot->Fill(Tracker1yRes);
    
    if(Tracker1yRes > 0.531 || Tracker1yRes < 0.235){//use double gaussian fit to find the range of the main peak
      cout << T1X.at(i) << ", " << T1Y.at(i) << endl;
      cout << "Bad Event ID:  " << i << endl;
      
      inter++;

      BadEventIDs.push_back(i);
      //T1X.erase(T1X.begin()+i);T1Y.erase(T1Y.begin()+i);
      //T2X.erase(T2X.begin()+i);T2Y.erase(T2Y.begin()+i);
      //T3X.erase(T3X.begin()+i);T3Y.erase(T3Y.begin()+i);
      //T4X.erase(T4X.begin()+i);T4Y.erase(T4Y.begin()+i);
      
    }
    
   
    
    float Measured_Tracker2x = interceptx + slopex*T2Z;
    ////Calculated2xcl->Fill(Measured_Tracker2x);
    float Tracker2xRes = Measured_Tracker2x - T2X.at(i);    
    
    //cout << "2X Coord:: " << Measured_Tracker2x << " - " <<  Tracker2HitlocVec[i] << " = " << Measured_Tracker2x - Tracker2HitlocVec[i] << endl;
    float Measured_Tracker2y = intercepty + slopey*T2Z;
    //Calculated2ycl->Fill(Measured_Tracker2y);
    float Tracker2yRes = Measured_Tracker2y - T2Y.at(i);  
    //cout << "2Y Coord:: " << Measured_Tracker2y << " - " <<  T2Y.at(i) << " = " << Measured_Tracker2y - T2Y.at(i) << endl;
   
    
    float Measured_Tracker3x = interceptx + slopex*T3Z;
    //Calculated3xcl->Fill(Measured_Tracker3x);
    float Tracker3xRes = Measured_Tracker3x - T3X.at(i);
    
    float Measured_Tracker3y = intercepty + slopey*T3Z;
    //Calculated3ycl->Fill(Measured_Tracker3y);
    float Tracker3yRes = Measured_Tracker3y - T3Y.at(i);
         
    float Measured_Tracker4x = interceptx + slopex*T4Z;
    //Calculated4xcl->Fill(Measured_Tracker4x);
    float Tracker4xRes = Measured_Tracker4x - T4X.at(i);

    float Measured_Tracker4y = intercepty + slopey*T4Z;
    //Calculated4ycl->Fill(Measured_Tracker4y);
    float Tracker4yRes = Measured_Tracker4y - T4Y.at(i);


    
    /*   
    if(i < 2000){
      OGVals->SetPoint(i+1, i/3, T2Y.at(i));
      CalcVals->SetPoint(i+1, i/3, Measured_Tracker2y);
      ResVals->SetPoint(i+1, i/3, Tracker2yRes);
    }
    */
   
    
    
    delete gx;
    delete fx;
    delete gy;
    delete fy;
    
    //i++;
    //i++;
  }
  //cout << inter << endl;

  
  I2GFvalues myValues;
	
  myValues = I2GFmainLoop(resplot,3,2,1);                           //converges!	  
  float ResMean = myValues.mean; float ResWidth=myValues.sigma;
  cout << ResMean << ", " << ResWidth << endl;
  cout << "bounds: " << ResMean-5*ResWidth << ", " << ResMean+5*ResWidth << endl;
  //TF1* h = new TF1("h", "gaus + gaus");
  //resplot->Fit("h", "Q");
  //resplot->Draw();
  
  
  //  for(auto j = T2X.begin(); j != T2X.end(); j++){
  //  cout << *j << endl;
  //}

  Tracker1HitlocVec.clear();
  Tracker2HitlocVec.clear();
  Tracker3HitlocVec.clear();
  Tracker4HitlocVec.clear();

  for(auto i = 0; i < T1X.size(); i++){
    Tracker1HitlocVec.push_back(T1X.at(i));
    Tracker2HitlocVec.push_back(T2X.at(i));
    Tracker3HitlocVec.push_back(T3X.at(i));
    Tracker4HitlocVec.push_back(T4X.at(i));

    Tracker1HitlocVec.push_back(T1Y.at(i));
    Tracker2HitlocVec.push_back(T2Y.at(i));
    Tracker3HitlocVec.push_back(T3Y.at(i));
    Tracker4HitlocVec.push_back(T4Y.at(i));

    Tracker1HitlocVec.push_back(T1Z);
    Tracker2HitlocVec.push_back(T2Z);
    Tracker3HitlocVec.push_back(T3Z);
    Tracker4HitlocVec.push_back(T4Z);
  }
    
  //for(auto j = Tracker2HitlocVec.begin(); j != Tracker2HitlocVec.end(); j++){
  //  cout << *j << endl;
  //}

  /*
  TCanvas* slops = new TCanvas("slops", "", 500, 500);
  OGVals->GetYaxis()->SetRangeUser(-10, 50);
  //OGVals->GetXaxis()->SetRangeUser(0, 5000);  
  OGVals->SetMarkerStyle(20);
  OGVals->Draw("AP");
  CalcVals->GetYaxis()->SetRangeUser(-10, 50);
  //CalcVals->GetXaxis()->SetRangeUser(0, 5000);
  CalcVals->SetMarkerColor(3);
  CalcVals->SetMarkerStyle(21);
  CalcVals->Draw("PSame");
  ResVals->GetYaxis()->SetRangeUser(-10, 50);
  //ResVals->GetXaxis()->SetRangeUser(0, 5000);
  ResVals->SetMarkerColor(4);
  ResVals->SetMarkerStyle(22);
  ResVals->Draw("PSame");

  auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
  legend->AddEntry(OGVals, "Original Values", "P");
  legend->AddEntry(CalcVals, "Calculated Values", "P");
  legend->AddEntry(ResVals, "Residuals", "P");
  
  legend->Draw();
  slops->SaveAs("All1Yvalues.png");
  
  slops->Draw();
  */
  return BadEventIDs;
}



vector<float> StripSep (vector<float>, vector<float>);
//Make function that separtes a vector of lists consecutive numbers and returns the sizes of each list.
vector<float> StripSep(vector<float>  Stripvec, vector<float>  Chargevec){

  
  vector<float> SubEventSizes;
  float previndx=Stripvec.at(0);
  
  float Currentindx=0; 
  int Vecsize=0;
  int maxVecsize=1;
  float tolerance = 1;
  int NumSubEvents=1;
  
  for(uint i = 0; i < Stripvec.size(); i++){
    Currentindx = Stripvec.at(i);
    //cout << "Strip: " << Currentindx << "with charge " << Chargevec.at(i) << endl;
    //if current strip number is sufficiently close to the previous strip in the hit, increase the vector size and check if it's the largest
    if(Currentindx <= previndx + tolerance){
      Vecsize++;
      //this determines the longest list in the vector. idk if it's necessary tbh
      if(Vecsize > maxVecsize)
	maxVecsize = Vecsize;
      //need this condition incase the last event is more than one strip
      if(i == (Stripvec.size()-1))
	SubEventSizes.push_back(Vecsize);
      
    }
    //if current strip number is NOT consecutive with the previous strip
    if(Currentindx > tolerance + previndx){
      //append the size of the previous list
      SubEventSizes.push_back(Vecsize);
      NumSubEvents++;
      Vecsize = 1;
      //if the last event is one strip long, this will catch it
      if(i == (Stripvec.size()-1))
	SubEventSizes.push_back(Vecsize);
    }

    previndx = Currentindx;
  } 
  cout << "Sub Events: " <<  NumSubEvents << ", Max SM: " << maxVecsize << endl;

  return SubEventSizes;//resultstripsandcharges;
}


vector<float> ChiSquared(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> BadEvents){
  
  vector<float> ChiSquaredMeans;

  TH1F* xTrackChi2 = new TH1F("XTrackChi2","Chi square of tracks in X projection",1000,0,100); xTrackChi2->SetXTitle("#chi^{2} of track in X"); xTrackChi2->SetYTitle("Frequency"); xTrackChi2->SetTitleSize(0.04,"XY"); xTrackChi2->SetLabelSize(0.04,"XY");
  TH1F* yTrackChi2 = new TH1F("YTrackChi2","Chi square of tracks in Y projection",1000,0,100); yTrackChi2->SetXTitle("#chi^{2} of track in Y"); yTrackChi2->SetYTitle("Frequency"); yTrackChi2->SetTitleSize(0.04,"XY"); yTrackChi2->SetLabelSize(0.04,"XY");
  
  if(Tracker1HitlocVec.empty() || Tracker2HitlocVec.empty() || Tracker3HitlocVec.empty() || Tracker4HitlocVec.empty())
    return ChiSquaredMeans;

    
  for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){
    //cout << i << endl;
    if(find(BadEvents.begin(), BadEvents.end(), i/3) == BadEvents.end()){
      TGraph* gx = new TGraph();
      TF1* fx = new TF1("line1","[0]+[1]*x",0,1600);
      TGraph* gy = new TGraph();
      TF1* fy = new TF1("line2","[0]+[1]*x",0,1600);

	    
  
      gx->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i]);
      gx->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i]);
      gx->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i]);
      gx->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i]);
    

      gy->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i+1]);
      gy->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i+1]);
      gy->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i+1]);
      gy->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i+1]);
 
      gx->Fit("line1","Q");
    
      gy->Fit("line2","Q");
    
      double interceptx = fx->GetParameter(0);
      double slopex     = fx->GetParameter(1);

      double intercepty = fy->GetParameter(0);
      double slopey     = fy->GetParameter(1);

      //cout << "X Chi squared: " << fx->GetChisquare() << endl;
      //cout << "Y Chi squared: " << fy->GetChisquare() << endl;
      xTrackChi2->Fill(fx->GetChisquare());
      yTrackChi2->Fill(fy->GetChisquare());
    
      delete gx;
      delete fx;
      delete gy;
      delete fy;
    }
    i++;
    i++;
  }
  
  //cout << "Mean X Chi Squared: " << xTrackChi2->GetMean() << endl;
  //cout << "Mean Y Chi Squared: " << yTrackChi2->GetMean() << endl;
  ChiSquaredMeans.push_back(xTrackChi2->GetMean());
  ChiSquaredMeans.push_back(yTrackChi2->GetMean());

  delete xTrackChi2;
  delete yTrackChi2;
      
  return ChiSquaredMeans;
}

vector<float> ChiSquared(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec){
  
  vector<float> ChiSquaredMeans;

  TH1F* xTrackChi2 = new TH1F("XTrackChi2","Chi square of tracks in X projection",1000,0,100); xTrackChi2->SetXTitle("#chi^{2} of track in X"); xTrackChi2->SetYTitle("Frequency"); xTrackChi2->SetTitleSize(0.04,"XY"); xTrackChi2->SetLabelSize(0.04,"XY");
  TH1F* yTrackChi2 = new TH1F("YTrackChi2","Chi square of tracks in Y projection",1000,0,100); yTrackChi2->SetXTitle("#chi^{2} of track in Y"); yTrackChi2->SetYTitle("Frequency"); yTrackChi2->SetTitleSize(0.04,"XY"); yTrackChi2->SetLabelSize(0.04,"XY");
  
  if(Tracker1HitlocVec.empty() || Tracker2HitlocVec.empty() || Tracker3HitlocVec.empty() || Tracker4HitlocVec.empty())
    return ChiSquaredMeans;

    
  for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){
    //cout << i << endl;
    TGraph* gx = new TGraph();
    TF1* fx = new TF1("line1","[0]+[1]*x",0,1600);
    TGraph* gy = new TGraph();
    TF1* fy = new TF1("line2","[0]+[1]*x",0,1600);

	    
  
    gx->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i]);
    gx->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i]);
    gx->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i]);
    gx->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i]);
    

    gy->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i+1]);
    gy->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i+1]);
    gy->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i+1]);
    gy->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i+1]);
 
    gx->Fit("line1","Q");
    
    gy->Fit("line2","Q");
    
    double interceptx = fx->GetParameter(0);
    double slopex     = fx->GetParameter(1);

    double intercepty = fy->GetParameter(0);
    double slopey     = fy->GetParameter(1);

    //cout << "X Chi squared: " << fx->GetChisquare() << endl;
    //cout << "Y Chi squared: " << fy->GetChisquare() << endl;
    xTrackChi2->Fill(fx->GetChisquare());
    yTrackChi2->Fill(fy->GetChisquare());
    
    delete gx;
    delete fx;
    delete gy;
    delete fy;    
    i++;
    i++;
  }
  
  //cout << "Mean X Chi Squared: " << xTrackChi2->GetMean() << endl;
  //cout << "Mean Y Chi Squared: " << yTrackChi2->GetMean() << endl;
  ChiSquaredMeans.push_back(xTrackChi2->GetMean());
  ChiSquaredMeans.push_back(yTrackChi2->GetMean());

  delete xTrackChi2;
  delete yTrackChi2;
      
  return ChiSquaredMeans;
}

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

  TFile* pedFile = TFile::Open("Data/FromscratchPedestal_dataTree01.root");//this one works!
  //TFile* pedFile = TFile::Open("Data/AddedApvIDInRootFile_Ped_dataTree01.root");

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
	//now the process of generating a smecific name for all of the graphs this generates
	

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
  
  //inputfiles["4150"] = "APV3Data/APV3_4150_dataTree01.root";
  
  //inputfiles["4250"] = "APV3Data/APV3_4250_dataTree01.root";  
  
  //inputfiles["4280"] = "APV3Data/APV3_4280_dataTree01.root";
  /*
  inputfiles["4310"] = "APV3Data/APV3_4310_dataTree01.root";

  inputfiles["4340"] = "APV3Data/APV3_4340_dataTree01.root";

  inputfiles["4370"] = "APV3Data/APV3_4370_dataTree01.root";
  
  //inputfiles["4400"] = "APV3Data/APV3_4400_dataTree01.root";//weird file
  */
  //inputfiles["4430"] = "APV3Data/APV3_4430_dataTree01.root";
  
  //inputfiles["4450"] = "APV3Data/APV3_4450_dataTree01.root"; 
  
  //inputfiles["4460"] = "APV3Data/APV3_4460_dataTree01.root";//also a wierd file due to statistics
  
  //inputfiles["4550"] = "APV3Data/APV3_4550_dataTree01.root";
  //inputfiles["4580"] = "APV3Data/APV3_4580_dataTree01.root";//4580_2400_2400  
  
  //inputfiles["4600"] = "APV3Data/APV3_4600_dataTree01.root";
  //inputfiles["4620"] = "APV3Data/APV3_4620_dataTree01.root";

  //From scratch data file to test
  //inputfiles["0"] = "Data/FromscratchData_dataTree01.root";
  //inputfiles["0"] = "Data/Data_check_dataTree01.root";//worksfine
  //inputfiles["0"] = "Data/Data_check_1Dstrips_dataTree01.root";
  //inputfiles["0"] = "Data/Data_check_removedmappingpart_dataTree01.root";
  //inputfiles["0"] = "Data/Data_check_changedorientationofAPV3bothtimes_dataTree01.root";
  //inputfiles["0"] = "Data/Check_DetectorType_CMSGEM_dataTree01.root";
  //inputfiles["0"] = "Data/FixingAPV3issues_dataTree01.root";
  //inputfiles["0"] = "Data/AddingfunctionsfromMikePhillipsAmore_dataTree01.root";
  //inputfiles["0"] = "Data/ChangedAPVOrientationsTo0forLAGD_dataTree01.root";
  //inputfiles["0"] = "Data/OnlyAPV3AsDet_Data_dataTree01.root";
  inputfiles["0"] = "Data/Last_ZZ_sector_0_ROOTfile_dataTree01.root";









  
  //Files for big statistics
  //inputfiles["0"] = "APV3Data/APV3_ADC_0_dataTree01.root";
  //inputfiles["0"] = "APV3Data/Initialtest_2_dataTree01.root";
  
  //inputfiles["1"] = "APV3Data/APV3_ADC_1_dataTree01.root";
  
  //inputfiles["2"] = "APV3Data/APV3_ADC_2_dataTree01.root";
  //inputfiles["3"] = "APV3Data/APV3_ADC_3_dataTree01.root";
  //inputfiles["4"] = "APV3Data/APV3_ADC_4_dataTree01.root";
  /*
  inputfiles["5"] = "APV3Data/APV3_ADC_5_dataTree01.root";
  inputfiles["6"] = "APV3Data/APV3_ADC_6_dataTree01.root";
  
  inputfiles["7"] = "APV3Data/APV3_ADC_7_dataTree01.root";
  inputfiles["8"] = "APV3Data/APV3_ADC_8_dataTree01.root";
  inputfiles["9"] = "APV3Data/APV3_ADC_9_dataTree01.root";
  inputfiles["10"] = "APV3Data/APV3_ADC_10_dataTree01.root";
  /*
  inputfiles["11"] = "APV3Data/APV3_ADC_11_dataTree01.root";
  inputfiles["12"] = "APV3Data/APV3_ADC_12_dataTree01.root";
  inputfiles["13"] = "APV3Data/APV3_ADC_13_dataTree01.root";
  inputfiles["14"] = "APV3Data/APV3_ADC_14_dataTree01.root";
  inputfiles["15"] = "APV3Data/APV3_ADC_15_dataTree01.root";
  inputfiles["16"] = "APV3Data/APV3_ADC_16_dataTree01.root";
  inputfiles["17"] = "APV3Data/APV3_ADC_17_dataTree01.root";
  
  inputfiles["18"] = "APV3Data/APV3_ADC_18_dataTree01.root";
  
  inputfiles["19"] = "APV3Data/APV3_ADC_19_dataTree01.root";
  
  inputfiles["20"] = "APV3Data/APV3_ADC_20_dataTree01.root";
  */
  //data for each sector
  //inputfiles["APV3&7"] = "Data/APV3and7_file5_dataTree01.root";//"Data/APV3_FixedAPVID_dataTree01.root";
  //inputfiles["APV7"] = "APV7_AllDataforAPVEvents_totalADCs_dataTree01.root";
  //inputfiles["APV6&2"] = "Data/APV2and6_dataTree01.root";
  //inputfiles["4400"] = "Data/APV4_dataTree01.root";
  //inputfiles["APV5"] = "Data/APV5_dataTree01.root";


  vector<float> SMofTheseevents;
  vector<float> Clusterchargeoftheseevnts;
  vector<float> Evtids;
  
  map<int, TCanvas*> PHCanvas;

  /*
  map<int, TGraph2D*> PulseHeight;
  map<int, TH2D*> PulseHeightHist;

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
  map<int, TGraph2D*> APV8PulseHeight;
  map<int, TGraph2D*> APV9PulseHeight;
  */
  map<string, TCanvas*> TrackerCanvas;

  map<string, TH2F*> Tracker1Hitmap;
  map<string, TH2F*> Tracker2Hitmap;
  map<string, TH2F*> Tracker3Hitmap;
  map<string, TH2F*> Tracker4Hitmap;
    
  map<int, int> Pit;
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
  
  vector<float> EfficiencyPerHV;
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
 

  vector<float> LAGDHitsVec;
  cout << "Event Display (SINGLE APV(0),  ALL(1), TRACKERS(2), LAGD APVs(3), Pedestal Data(4), Residuals(5)):";
  int display_mode=5;
  //  cin >> display_mode; 

  bool displaymode0event = false;
  bool Clusterdistevents = false;
  bool Gaininfo = false;
  bool ChargeRatio = false;
    
  int displaymode0dataoutput;
  int AlignmentMode=0;
  
  if(display_mode == 5){
    cout << "What Alignment do you want?" << endl;
    cout << "Trackers XYRot (0),  Trackers + GEM XYRot(1)" << endl;
    //cin >> AlignmentMode;
  }

  
  if(display_mode == 0){
    cout << "What data do you want to see?" << endl;
    cout << "Events? (0), Cluster data? (1), Gain data? (2), or ChargeRatio (3): " << endl;
    //cin >> displaymode0dataoutput;

    if(displaymode0dataoutput == 0) displaymode0event = true;
    if(displaymode0dataoutput == 1) Clusterdistevents = true;
    if(displaymode0dataoutput == 2) Gaininfo = true;
    if(displaymode0dataoutput == 3) ChargeRatio = true;
  }
  
  float apvnum=2;
  
  if(display_mode == 0){
    
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

    //cin >> apvnum;
    
  }


  //APVindex's

  //OG locations
  int APV2index = 0;
  int APV3index = 1;
  int APV4index = 2;
  int APV5index = 3;
  int APV6index = 4;
  int APV7index = 5;
  int APV8index = 6;
  int APV9index = 7;
  
  int Tracker1xindex = 8;
  int Tracker1yindex = 9;
  int Tracker2xindex = 12;
  int Tracker2yindex = 13;
  int Tracker3xindex = 10;
  int Tracker3yindex = 11;
  int Tracker4xindex = 14;
  int Tracker4yindex = 15;
  

  
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


  //here we define the number of events we want to either plot or analyze
  int plotnum = 5001;
  if(display_mode == 4)
    plotnum = 1;
  if(display_mode == 2 || display_mode == 5)
    plotnum = 5000;
  if(display_mode == 0 || display_mode == 1 || display_mode == 3){ 
    cout << "How many events? ";
    //cin >> plotnum;    
  }
  auto Stripcorr = new TGraph();
  auto TrackerStripcorr = new TGraph();  
  int Stripcorrit=0;
  auto LAGDstrips = new TGraph();
  auto Trackerstrips = new TGraph();
  auto Diffbtwnstrips = new TGraph();
  auto EffperHV = new TGraph();
  int fileit=0;
  
  int totalEventiterator=0;
  vector<int> EventIterator;
  vector<int> BadEvents_OGIndex;
  vector<int> GoodEvents_OGIndex;
  auto StripChargeHist = new TH1F("StripCharges", "Strip Charge Distribution of Poorly Recreated Events; Charge (AU); Counts/4",200 , 0, 800);
  auto ClusterChargeHist = new TH1F("ClusterCharges", "Cluster Charge Distribution of Poorly Recreated Events; Charge (AU); Counts/4",200 , 0, 1500);
  auto StripsHist = new TH1F("Strips", "Strip Distribution of Poorly Recreated Events; Strip (ID); Counts/4", 300, 0, 128);
  //auto ClusterChargeHist = new TH1F("ClusterCharges", "Cluster Charge Distribution of Correctly Recreated Events; Charge (AU); Counts/4",200 , 0, 1500);
  auto StripsHithist = new TH1F("Strips", "Strip hit Distribution in LAGD; Strip (ID); Counts", 128, 0, 128);
  int striphitit=0;
  int toleranceit = 0;
  TH1D* Tracker2Ystriphits = new TH1D("", "", 128, 0, 128);
  
  //cout << inputfiles.size() << endl;
  //this for loop will iterate through the data files
  for(map<string,TString>::iterator i = inputfiles.begin(); i != inputfiles.end(); i++){
    TFile* myFile = TFile::Open(i->second);
    string evtype = i->first;


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

    //define Hitmaps for each tracker per file
    Tracker1Hitmap[evtype] = new TH2F("PHT1", "Tracker 1 Hitmap; x Distance; y distance, Hits", 100, 0, 50, 100, 0, 50);
    Tracker3Hitmap[evtype] = new TH2F("PHT3", "Tracker 3 Hitmap; x Distance; y distance, Hits", 100, 0, 50, 100, 0, 50);
    Tracker2Hitmap[evtype] = new TH2F("PHT2", "Tracker 2 Hitmap; x Distance; y distance, Hits", 100, 0, 50, 100, 0, 50);
    Tracker4Hitmap[evtype] = new TH2F("PHT4", "Tracker 4 Hitmap; x Distance; y distance, Hits", 100, 0, 50, 100, 0, 50);


    //define other file-based histograms for other display modes
    Clusterdist[evtype] = new TH1D("Clusterdist", ("Strip Multiplicty Distribution with HV "+evtype).c_str(), 6, 1, 7);
    
    
    //iterate through each event

  
    /*
    for(auto i = BadEventVector.begin(); i != BadEventVector.end(); i++){
      cout << *i << endl;
    }
    */
    auto PulseHeight = new TGraph2D(2);
    auto PulseHeightHist = new TH2D("PHHistversion", "Title;Strip;Time;ADC Charge", 128, 0, 128, 6, 0, 6);
    auto Tracker1XPulseHeight = new TGraph2D(2);
    auto Tracker1YPulseHeight = new TGraph2D(2);            
    auto Tracker2XPulseHeight = new TGraph2D(2);
    auto Tracker2YPulseHeight = new TGraph2D(2);
    auto Tracker3XPulseHeight = new TGraph2D(2);
    auto Tracker3YPulseHeight = new TGraph2D(2);
    auto Tracker4XPulseHeight = new TGraph2D(2);
    auto Tracker4YPulseHeight = new TGraph2D(2);
      
     
    //Uneccesary ATM since theyre more complicated
      
    auto APV2PulseHeight = new TGraph2D(2);
    auto APV8PulseHeight = new TGraph2D(2);
    auto APV9PulseHeight = new TGraph2D(2);
      
      
    auto APV3E1PulseHeight = new TGraph2D(2);
    auto APV3E2PulseHeight = new TGraph2D(2);
    auto APV4E3PulseHeight = new TGraph2D(2);
    auto APV4E4PulseHeight = new TGraph2D(2);
    auto APV5E3PulseHeight = new TGraph2D(2);
    auto APV5E4PulseHeight = new TGraph2D(2);
    auto APV6E3PulseHeight = new TGraph2D(2);
    auto APV6E4PulseHeight = new TGraph2D(2);
    auto APV7E1PulseHeight = new TGraph2D(2);
    auto APV7E2PulseHeight = new TGraph2D(2);          

    EventAvgCharge[evtype] = new TH1F("", "", 300, 0, 2500);
    float LAGDhitsnum=0;
    float Trackerhits=0;
    while (myReader.Next()){
      cout << *nch << endl;
      //if(*nch != 2048){continue;}
      totalEventiterator++;
      cout << "Event ID: " << *evtID << endl;
      //if(find(AllEventsVector.begin(), AllEventsVector.end(), totalEventiterator) != AllEventsVector.end() || *evtID == 1){// == vector.end() means the event ID isnt in the vector
	//Top inequality is always != so only events considered in the tracking analysis. To plot only good events, set bottom to !=,  opposite for plotting bad events
      
	//if(*evtID != 1){
      //if(find(BadEventVector.begin(), BadEventVector.end(), totalEventiterator) != BadEventVector.end()){continue;}//so != vector.end() means the event IS in that vector 
      //	}
	cout << "Event ID: " << totalEventiterator << endl;
      
      //if the event number is larger than what we want to plot, stop
      if(*evtID > plotnum) break;
      plotnum = 100;

      
      //iterating variables
      Pit[*evtID] = 0;
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
      /*
      vector<float> Tracker1hitcoords;//stored as (X, Y, Z)
      vector<float> Tracker2hitcoords;
      vector<float> Tracker3hitcoords;
      vector<float> Tracker4hitcoords;
      */
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

      vector<float> maxstrips;
      vector<float> Tracker1xmaxstrips;
      vector<float> Tracker1ymaxstrips;      
      vector<float> Tracker2xmaxstrips;
      vector<float> Tracker2ymaxstrips;
      vector<float> Tracker3xmaxstrips;
      vector<float> Tracker3ymaxstrips;
      vector<float> Tracker4xmaxstrips;
      vector<float> Tracker4ymaxstrips;

      vector<float> APV2maxstrips;
      vector<float> APV3E1maxstrips;
      vector<float> APV3E2maxstrips;
      vector<float> APV4E3maxstrips;
      vector<float> APV4E4maxstrips;
      vector<float> APV5E3maxstrips;
      vector<float> APV5E4maxstrips;
      vector<float> APV6E3maxstrips;
      vector<float> APV6E4maxstrips;
      vector<float> APV7E1maxstrips;
      vector<float> APV7E2maxstrips;
      vector<float> APV8maxstrips;
      vector<float> APV9maxstrips;
      
      vector<float> maxcharges;
      vector<float> Tracker1xmaxcharges;
      vector<float> Tracker1ymaxcharges;
      vector<float> Tracker2xmaxcharges;
      vector<float> Tracker2ymaxcharges;
      vector<float> Tracker3xmaxcharges;
      vector<float> Tracker3ymaxcharges;
      vector<float> Tracker4xmaxcharges;
      vector<float> Tracker4ymaxcharges;

      vector<float> APV2maxcharges;
      vector<float> APV3E1maxcharges;
      vector<float> APV3E2maxcharges;
      vector<float> APV4E3maxcharges;
      vector<float> APV4E4maxcharges;
      vector<float> APV5E3maxcharges;
      vector<float> APV5E4maxcharges;
      vector<float> APV6E3maxcharges;
      vector<float> APV6E4maxcharges;
      vector<float> APV7E1maxcharges;
      vector<float> APV7E2maxcharges;
      vector<float> APV8maxcharges;
      vector<float> APV9maxcharges;

    
      
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
	    
	  h = 256;
	  if((strip)[i] < 128)
	    h = 128;
	  
	  
	  if((strip)[i]%128 == 0)
	    cout << "|     " << (detID)[i] << "     |    " << (strip)[i] << "    |    " << (planeID)[i] << "    |   " << (apvID)[i] << "   |    " << endl;   
	  if((detID)[i] == 0)
	    cout << "From Event: " << *evtID << "; this is Strip: " << (strip)[i]  << " from APV: " << (apvID)[i] + 2 << " of plane: " << (planeID)[i] << endl;
	  if((detID)[i] > 0)
	    cout << "From Event: " << *evtID << "; this is Strip: " << (strip)[i]  << " from APV: " << (apvID)[i]  << " of plane: " << (planeID)[i] << endl;
	  
	  //toliter = (*evtID)*(*nch) + i;
	  
	  float stdev=0;
	  float mean=0;
	
	  //cout << "yes" << endl;
	  stdev = adcsstrip[i]->GetStdDev();///sqrt(adcsstrip[i]->GetEntries());
	  mean = adcsstrip[i]->GetMean();
	  //cout << stdev << endl;

	  striptol[i] = mean + 5*stdev;
	  //striptol[i] = 40;
	  //cout << "Strip: " << i << " has noise tolerance: " << striptol[i] <<  endl;
	  //return;
	}
	
	//vector<vector<short>> adcvals = {(adc0), (adc1), (adc2), (adc3), (adc4), (adc5)};
	//cout << "Here " << i <<  endl;
	if(display_mode == 0 && totalchannel[*evtID] >= 128*apvnum && totalchannel[*evtID] <= 64 + 128*apvnum){	 
	    
	  if(display_mode != 2 || display_mode != 5){
	    /*
	    for(uint k=0; k<=5; k++){
	      PulseHeight[*evtID]->SetPoint(Pit[*evtID]++, (strip)[i], k, adcvals[k].at(i));
	    }
	    */
	    PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	    PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	    PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	    PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	    PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	    PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    
	    //to try and histogram the plot instead of using LEGO

	    //for(uint k=0; k<=5;k++){//iterate over the 6 timebins
	    //cout << "k: " << k << endl;
	    /*
	      for(uint u=0; u<abs((adc0)[i]);u++){
	        //cout << abs((adc0)[i]) << endl;
		PulseHeightHist[*evtID]->Fill((strip[i]), 0);
	      }
	      for(uint u=0; u<abs((adc1)[i]);u++){
		//cout << abs((adc1)[i]) << endl;
		PulseHeightHist[*evtID]->Fill((strip[i]), 1);		
	      }
	      for(uint u=0; u<abs((adc2)[i]);u++){
		PulseHeightHist[*evtID]->Fill((strip[i]), 2);
		//cout << abs((adc2)[i]) << endl;
	      }
	      for(uint u=0; u<abs((adc3)[i]);u++){
		PulseHeightHist[*evtID]->Fill((strip[i]), 3);
		//cout << abs((adc3)[i]) << endl;
	      }
	      for(uint u=0; u<abs((adc4)[i]);u++){
		PulseHeightHist[*evtID]->Fill((strip[i]), 4);
		//cout << abs((adc4)[i]) << endl;
	      }
	      for(uint u=0; u<abs((adc5)[i]);u++){
		PulseHeightHist[*evtID]->Fill((strip[i]), 5);
		//cout << abs((adc5)[i]) << endl;
	      }
	    */
	      //cout << "Strip " << (strip)[i] << " has charge " << PuleHeightHist->GetBinContent((strip)[i], 0) << 
		//}	    	    
	  }
	  
	  stripcharges.push_back((adc0)[i]);	
	  stripcharges.push_back((adc1)[i]);
	  stripcharges.push_back((adc2)[i]);
	  stripcharges.push_back((adc3)[i]);
	  stripcharges.push_back((adc4)[i]);
	  stripcharges.push_back((adc5)[i]);	  

	  /*
	  if(*evtID == 15 && (strip)[i] >= 125){
	    
	    cout << "Weird event adcs" << endl;

	    cout << (adc0)[i] << endl;
	    cout << (adc1)[i] << endl;
	    cout << (adc2)[i] << endl;
	    cout << (adc3)[i] << endl;
	    cout << (adc4)[i] << endl;
	    cout << (adc5)[i] << endl;
	    cout << "Tolerance for strip " << i << ": " << striptol[i] << endl;
	    //return;
	    
	  }
	  */
	     maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	  /*
	  auto SumOfSignal=0;
	  for (auto& n : stripcharges)
	    SumOfSignal += n;

	  */
	  
	  if(maxcharge >= striptol[i]){
	    //cout << "Strip: " << i << " has noise std: " << striptol[i] <<  endl;
	    //maxcharges.push_back(SumOfSignal);
	    maxcharges.push_back(maxcharge-striptol[i]);
	    maxstrips.push_back((strip)[i]);

	    StripChargeHist->Fill(maxcharge-striptol[i]);
	    StripsHist->Fill((strip)[i]);

	  }	  
	  stripcharges.clear();	  
	}
	
	
	if(display_mode == 1 || display_mode == 2 || display_mode == 4 || display_mode == 5){
	  
	  //if(totalchannel[*evtID] >= 128*8 && totalchannel[*evtID] < 127 + 128*15){
	    //Trackers
	    //cout << "in trackers?" << endl;
	    if(totalchannel[*evtID] >= 128*Tracker1xindex && totalchannel[*evtID] <= 127 + 128*Tracker1xindex){//og 8 changed to 10, now to 0

	      if(display_mode != 2 || display_mode != 5){
		Tracker1XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);	      
		Tracker1XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
		Tracker1XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
		Tracker1XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
		Tracker1XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
		Tracker1XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	      }
	      
	      stripcharges.push_back((adc0)[i]);
	      stripcharges.push_back((adc1)[i]);
	      stripcharges.push_back((adc2)[i]);
	      stripcharges.push_back((adc3)[i]);
	      stripcharges.push_back((adc4)[i]);
	      stripcharges.push_back((adc5)[i]);

	      
	      maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	      
	      if(maxcharge >= striptol[i]){
		
		Tracker1xmaxcharges.push_back(maxcharge-striptol[i]);				
		Tracker1xmaxstrips.push_back((strip)[i]);
		
	      }

	      stripcharges.clear();
	      
	      if(*evtID == 1)
		//Tracker1xPedstds.insert(0, stdev);
		Tracker1xPedstds.push_back(striptol[i]);	      	      
	    }
	    // cout << "Pass tracker 1?" << endl;
	    if(totalchannel[*evtID] >= 128*Tracker3xindex && totalchannel[*evtID] <= 127 + 128*Tracker3xindex){//og 12 cjanged to 12, now 1
	      
	      if(display_mode != 2 || display_mode != 5){
		Tracker3XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
		Tracker3XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
		Tracker3XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
		Tracker3XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
		Tracker3XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
		Tracker3XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	      }

	      stripcharges.push_back((adc0)[i]);
	      stripcharges.push_back((adc1)[i]);
	      stripcharges.push_back((adc2)[i]);
	      stripcharges.push_back((adc3)[i]);
	      stripcharges.push_back((adc4)[i]);
	      stripcharges.push_back((adc5)[i]);
	     
	      maxcharge = *max_element(stripcharges.begin(), stripcharges.end());

	      if(maxcharge >=  striptol[i]){
		
		Tracker3xmaxcharges.push_back(maxcharge-striptol[i]);
		Tracker3xmaxstrips.push_back((strip)[i]);
		
	      }
	      
	      stripcharges.clear();	  	  
	      if(*evtID == 1)
		Tracker3xPedstds.push_back(striptol[i]);	      
	      
	    }
	      
	    if(totalchannel[*evtID] >= 128*Tracker1yindex && totalchannel[*evtID] <= 127 + 128*Tracker1yindex){//og 9, changed to 11, now 2

		if(display_mode != 2 || display_mode != 5){
		  Tracker1YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
		  Tracker1YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
		  Tracker1YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
		  Tracker1YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
		  Tracker1YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);	      
		  Tracker1YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
		}
	      
		stripcharges.push_back((adc0)[i]);
		stripcharges.push_back((adc1)[i]);
		stripcharges.push_back((adc2)[i]);
		stripcharges.push_back((adc3)[i]);
		stripcharges.push_back((adc4)[i]);
		stripcharges.push_back((adc5)[i]);	      

		maxcharge = *max_element(stripcharges.begin(), stripcharges.end());	   	      
	
		if(maxcharge >= striptol[i]){
		  
		  Tracker1ymaxcharges.push_back(maxcharge-striptol[i]);		  
		  Tracker1ymaxstrips.push_back((strip)[i]);
        
		}
	      
		stripcharges.clear();
		if(*evtID == 1)
		  Tracker1yPedstds.push_back(striptol[i]);	      
	      
	      }
	    
	    if(totalchannel[*evtID] >= 128*Tracker3yindex && totalchannel[*evtID] <= 127 + 128*Tracker3yindex){//was 13, now 3

		if(display_mode != 2 || display_mode != 5){
		  Tracker3YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
		  Tracker3YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
		  Tracker3YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
		  Tracker3YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
		  Tracker3YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
		  Tracker3YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
		}

	      
		stripcharges.push_back((adc0)[i]);
		stripcharges.push_back((adc1)[i]);
		stripcharges.push_back((adc2)[i]);
		stripcharges.push_back((adc3)[i]);
		stripcharges.push_back((adc4)[i]);
		stripcharges.push_back((adc5)[i]);	      

		maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
		
		if(maxcharge >=  striptol[i]){

		  Tracker3ymaxcharges.push_back(maxcharge-striptol[i]);
		  Tracker3ymaxstrips.push_back((strip)[i]);
		  
		}
	      
		stripcharges.clear();
		if(*evtID == 1)
		  Tracker3yPedstds.push_back(striptol[i]);	      
	      
	      }
	    
	      if(totalchannel[*evtID] >= 128*Tracker2xindex && totalchannel[*evtID] <= 127 + 128*Tracker2xindex){//og 10, switched to 14, now 5

		if(display_mode != 2 || display_mode != 5){
		  Tracker2XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);	      
		  Tracker2XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
		  Tracker2XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
		  Tracker2XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
		  Tracker2XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
		  Tracker2XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);		
		}
		
		stripcharges.push_back((adc0)[i]);
		stripcharges.push_back((adc1)[i]);
		stripcharges.push_back((adc2)[i]);
		stripcharges.push_back((adc3)[i]);
		stripcharges.push_back((adc4)[i]);
		stripcharges.push_back((adc5)[i]);
		
	      
		maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
		
		if(maxcharge >=  striptol[i]){

		  Tracker2xmaxcharges.push_back(maxcharge-striptol[i]);
		  Tracker2xmaxstrips.push_back((strip)[i]);
		  
		}
		
		stripcharges.clear();
		if(*evtID == 1)
		  Tracker2xPedstds.push_back(striptol[i]);	      
		
	      		
	      }
	      if(totalchannel[*evtID] >= 128*Tracker4xindex && totalchannel[*evtID] <= 127 + 128*Tracker4xindex){//og 14, changed to 8, now 4

		if(display_mode != 2 || display_mode != 5){
		  Tracker4XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
		  Tracker4XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
		  Tracker4XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
		  Tracker4XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
		  Tracker4XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
		  Tracker4XPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
		}
		
		stripcharges.push_back((adc0)[i]);
		stripcharges.push_back((adc1)[i]);
		stripcharges.push_back((adc2)[i]);
		stripcharges.push_back((adc3)[i]);
		stripcharges.push_back((adc4)[i]);
		stripcharges.push_back((adc5)[i]);


	      
		maxcharge = *max_element(stripcharges.begin(), stripcharges.end());

		if(maxcharge >=  striptol[i]){
		  Tracker4xmaxcharges.push_back(maxcharge-striptol[i]);
		  Tracker4xmaxstrips.push_back((strip)[i]);
		  
		}
		
		stripcharges.clear();
		if(*evtID == 1)
		  Tracker4xPedstds.push_back(striptol[i]);	      	      
	      
	      }

	      
	      if(totalchannel[*evtID] >= 128*Tracker2yindex && totalchannel[*evtID] <= 127 + 128*Tracker2yindex){//og 11, was 15, now 7

		if(display_mode != 2 || display_mode != 5){
		  Tracker2YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
		  Tracker2YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
		  Tracker2YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
		  Tracker2YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
		  Tracker2YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
		  Tracker2YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
		}
		
		stripcharges.push_back((adc0)[i]);
		stripcharges.push_back((adc1)[i]);
		stripcharges.push_back((adc2)[i]);
		stripcharges.push_back((adc3)[i]);
		stripcharges.push_back((adc4)[i]);
		stripcharges.push_back((adc5)[i]);

	      
		maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	
		if(maxcharge >=  striptol[i]){
		  //cout << "Here?" << endl;
		  Tracker2ymaxcharges.push_back(maxcharge-striptol[i]);
		  Tracker2ymaxstrips.push_back((strip)[i]);
		 
		  
		}
		
		stripcharges.clear();
		if(*evtID == 1)
		  Tracker2yPedstds.push_back(striptol[i]);	      	      
		
	      }
	  
	      if(totalchannel[*evtID] >= 128*Tracker4yindex && totalchannel[*evtID] <= 127 + 128*Tracker4yindex){//og 15, was 9, now 6

		if(display_mode != 2 || display_mode != 5){
		  Tracker4YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
		  Tracker4YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
		  Tracker4YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
		  Tracker4YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
		  Tracker4YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
		  Tracker4YPulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
		}
		
		stripcharges.push_back((adc0)[i]);
		stripcharges.push_back((adc1)[i]);
		stripcharges.push_back((adc2)[i]);
		stripcharges.push_back((adc3)[i]);
		stripcharges.push_back((adc4)[i]);
		stripcharges.push_back((adc5)[i]);

	      
		maxcharge = *max_element(stripcharges.begin(), stripcharges.end());

		if(maxcharge >=  striptol[i]){
		  Tracker4ymaxcharges.push_back(maxcharge-striptol[i]);
		  Tracker4ymaxstrips.push_back((strip)[i]);
		  
		}
		
		stripcharges.clear();	  	  
		if(*evtID == 1)
		  Tracker4yPedstds.push_back(striptol[i]);	      	      
	      
	      }

	      	  //APV 3 in eta 1
	  if(totalchannel[*evtID] >= 128*APV3index && totalchannel[*evtID] <= 64 + 128*APV3index){

	    if(display_mode != 2 || display_mode != 5){
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	      
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    
	    if(maxcharge >= striptol[i]){
	      APV3E1maxcharges.push_back(maxcharge-striptol[i]);
	      APV3E1maxstrips.push_back((strip)[i]);
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV3E1Pedstds.push_back(striptol[i]);	      
	    
	  }
	  
	  //cout << "Left trackers?" << endl;
	}
	//end filling tracker plots
	
	if(display_mode == 1 || display_mode == 3 || display_mode == 4 || display_mode == 5){
	  //start LAGD APVs
	  
	  //To save space, we do not need APVs 2, 8, and 9 for now
	  if(totalchannel[*evtID] >= 128*APV8index && totalchannel[*evtID] <= 127 + 128*APV8index){
	     
	    if(display_mode != 2 || display_mode != 5){
	      APV8PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV8PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV8PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV8PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV8PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV8PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	           
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	  
	    if(maxcharge >=  striptol[i]){			
	      APV8maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV8maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV8Pedstds.push_back(striptol[i]);	      	      	      	
	  }
	  
	
	  if(totalchannel[*evtID] >= 128*APV9index && totalchannel[*evtID] <= 127 + 128*APV9index){
	    if(display_mode != 2 || display_mode != 5){
	      APV9PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV9PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV9PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV9PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV9PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV9PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	    	       
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	  
	    if(maxcharge >=  striptol[i]){			
	      APV9maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV9maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV9Pedstds.push_back(striptol[i]);	      	      
	      
	  }	  
	  
	  if(totalchannel[*evtID] >= 128*APV5index && totalchannel[*evtID] <= 63 + 128*APV5index){

	    if(display_mode != 2 || display_mode != 5){
	      APV5E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV5E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV5E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV5E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV5E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV5E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	       
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	  
	    if(maxcharge >=  striptol[i]){			
	      APV5E3maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV5E3maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV5E3Pedstds.push_back(striptol[i]);	      	      
	      
	  }
	  
	  if(totalchannel[*evtID] >= 64 + 128*APV5index && totalchannel[*evtID] <= 127 + 128*APV5index){

	    if(display_mode !=2){
	      APV5E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV5E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV5E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV5E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV5E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV5E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	           
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);	    

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    
	    if(maxcharge >=  striptol[i]){			
	      APV5E4maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV5E4maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV5E4Pedstds.push_back(striptol[i]);	      	      
	      
	  }
	  

	  if(totalchannel[*evtID] >= 128*APV4index && totalchannel[*evtID] <= 63 + 128*APV4index){

	    if(display_mode != 2 || display_mode != 5){
	      APV4E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV4E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV4E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV4E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV4E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV4E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	           
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);
	    
	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    
	    if(maxcharge >= striptol[i]){			
	      APV4E3maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV4E3maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV4E3Pedstds.push_back(striptol[i]);	      
	  }
	  
	  if(totalchannel[*evtID] >= 64 + 128*APV4index && totalchannel[*evtID] <= 127 + 128*APV4index){
	    if(display_mode != 2 || display_mode != 5){
	      APV4E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV4E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV4E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV4E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV4E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV4E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
  	    }	    
	    
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);	    

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    
	    if(maxcharge >= striptol[i]){			
	      APV4E4maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV4E4maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV4E4Pedstds.push_back(striptol[i]);	      
	  }
	  /*
	  //APV 3 in eta 1
	  if(totalchannel[*evtID] >= 128*APV3index && totalchannel[*evtID] <= 64 + 128*APV3index){

	    if(display_mode != 2 || display_mode != 5){
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV3E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	      
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    
	    if(maxcharge >= striptol[i]){
	      APV3E1maxcharges.push_back(maxcharge-striptol[i]);
	      APV3E1maxstrips.push_back((strip)[i]);
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV3E1Pedstds.push_back(striptol[i]);	      
	    
	  }
	  */
	  //APV 3 in eta 2
	  if(totalchannel[*evtID] >= 64+ 128*APV3index && totalchannel[*evtID] <= 128 + 128*APV3index){

	    if(display_mode != 2 || display_mode != 5){
	      APV3E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV3E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV3E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV3E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV3E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV3E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	    
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);	    

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());

	    if(maxcharge >= striptol[i]){
	      APV3E2maxcharges.push_back(maxcharge-striptol[i]);
	      APV3E2maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV3E2Pedstds.push_back(striptol[i]);	      
	  }	  

	  if(totalchannel[*evtID] >= 128*APV6index && totalchannel[*evtID] <= 63 + 128*APV6index){

	    if(display_mode != 2 || display_mode != 5){
	      APV6E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV6E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV6E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV6E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV6E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV6E3PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	    
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    
	    if(maxcharge >= striptol[i]){			
	      APV6E3maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV6E3maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV6E3Pedstds.push_back(striptol[i]);	      
	  }
	  
	  if(totalchannel[*evtID] >= 64 + 128*APV6index && totalchannel[*evtID] <= 127 + 128*APV6index){

	    if(display_mode != 2 || display_mode != 5){
	      APV6E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV6E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV6E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV6E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV6E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV6E4PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	    
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    
	    if(maxcharge >= striptol[i]){			
	      APV6E4maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV6E4maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV6E4Pedstds.push_back(striptol[i]);	      
	  }

	  
	  //Again, dont need APV 2 rn
  
	  if(totalchannel[*evtID] >= 128*APV2index && totalchannel[*evtID] <= 127 + 128*APV2index){

	    if(display_mode != 2 || display_mode != 5){
	      APV2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	    	   
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);
	    
	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    
	    if(maxcharge >= striptol[i]){			
	      APV2maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV2maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV2Pedstds.push_back(striptol[i]);	      	  
	    
	  }
	  
	  
	  //APV 7 eta 1
	  if(totalchannel[*evtID] >= 128*APV7index && totalchannel[*evtID] <= 63 + 128*APV7index){

	    if(display_mode != 2 || display_mode != 5){
	      APV7E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV7E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV7E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV7E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV7E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV7E1PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	    	   
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);	    

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    
	    if(maxcharge >= striptol[i]){			
	      APV7E1maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV7E1maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV7E1Pedstds.push_back(striptol[i]);	      
	  }
	  
	  //APV 7 eta 2
	  if(totalchannel[*evtID] >= 64 + 128*APV7index && totalchannel[*evtID] <= 127 + 128*APV7index){

	    if(display_mode != 2 || display_mode != 5){
	      APV7E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 0, (adc0)[i]);
	      APV7E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 1, (adc1)[i]);
	      APV7E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 2, (adc2)[i]);
	      APV7E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 3, (adc3)[i]);
	      APV7E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 4, (adc4)[i]);
	      APV7E2PulseHeight->SetPoint(Pit[*evtID]++, (strip)[i], 5, (adc5)[i]);
	    }
	    
	    stripcharges.push_back((adc0)[i]);
	    stripcharges.push_back((adc1)[i]);
	    stripcharges.push_back((adc2)[i]);
	    stripcharges.push_back((adc3)[i]);
	    stripcharges.push_back((adc4)[i]);
	    stripcharges.push_back((adc5)[i]);

	      
	    maxcharge = *max_element(stripcharges.begin(), stripcharges.end());
	    
	    if(maxcharge >= striptol[i]){			
	      APV7E2maxcharges.push_back(maxcharge-striptol[i]);	      
	      APV7E2maxstrips.push_back((strip)[i]);
	      
	    }
	    
	    stripcharges.clear();
	    if(*evtID == 1)
	      APV7E2Pedstds.push_back(striptol[i]);	      
	  }	  
	}
	
	//end LAGD APVs
	totalchannel[*evtID]++;	
	//cout << "channel number" << totalchannel[*evtID] -1 << ", " << (detID)[i] << endl;
	
	if(display_mode != 3 && display_mode != 1 && display_mode != 0){
	  //cout << display_mode << " and " << *evtID << endl;
	  
	  APV3E1PulseHeight->Clear();
	  APV3E2PulseHeight->Clear();
	  APV4E3PulseHeight->Clear();
	  APV4E4PulseHeight->Clear();
	  APV5E3PulseHeight->Clear();
	  APV5E4PulseHeight->Clear();
	  APV6E3PulseHeight->Clear();
	  APV6E4PulseHeight->Clear();
	  APV7E1PulseHeight->Clear();
	  APV7E2PulseHeight->Clear();
	  
	  Tracker1XPulseHeight->Clear();
	  Tracker1YPulseHeight->Clear();
	  Tracker2XPulseHeight->Clear();
	  Tracker2YPulseHeight->Clear();
	  Tracker3XPulseHeight->Clear();
	  Tracker3YPulseHeight->Clear();
	  Tracker4XPulseHeight->Clear();
	  Tracker4YPulseHeight->Clear();
	  /*
	  delete PulseHeight[*evtID];
	  PulseHeight[*evtID] = nullptr;	  
	  */
	  
	}
	
	//	toleranceit++;
      }/////////////////////end of for loop over the channels in one event


      
      cout << "Done Loop over all channels" << endl;
      GoodEvent[*evtID] = true;
      //cout << "yes" << endl;
      //only allowing events with 1 hit as of right now, ie, only one sub event->
      if(display_mode == 0){
	if(maxstrips.empty())
	  GoodEvent[*evtID] = false;
	else{
	  auto NumSubEvents = StripSep(maxstrips, maxcharges);	
	  if(NumSubEvents.size() > 1)
	    GoodEvent[*evtID] = false;
	  
	  int max_StripHits = 4;
	  int min_StripHits = 2;	    
	  if(*max_element(NumSubEvents.begin(), NumSubEvents.end()) > max_StripHits || *min_element(NumSubEvents.begin(), NumSubEvents.end()) < min_StripHits){GoodEvent[*evtID] = false;}
	}	
      }
      //float LAGDhitsnum=0;
      //float Trackerhits=0;
      //Only allow events with one peak in the hit including APV3 for now
      /*
      if(display_mode == 1 || display_mode == 2 || display_mode == 3 || display_mode == 5){

	if(Tracker1xmaxstrips.empty() == 0 && Tracker1ymaxstrips.empty() == 0 && Tracker2xmaxstrips.empty() == 0 && Tracker2ymaxstrips.empty() == 0 && Tracker3xmaxstrips.empty() == 0 && Tracker3ymaxstrips.empty() == 0 && Tracker4xmaxstrips.empty() == 0 && Tracker4ymaxstrips.empty() == 0){
	  Trackerhits++;
	}
	//cout << "here?2" << endl;

	if(Tracker1xmaxstrips.empty() == 0 && Tracker1ymaxstrips.empty() == 0 && Tracker2xmaxstrips.empty() == 0 && Tracker2ymaxstrips.empty() == 0 && Tracker3xmaxstrips.empty() == 0 && Tracker3ymaxstrips.empty() == 0 && Tracker4xmaxstrips.empty() == 0 && Tracker4ymaxstrips.empty() == 0 && APV3E1maxstrips.empty() == 0){
	  LAGDhitsnum++;
	}

	if(Tracker1xmaxstrips.empty() || Tracker1ymaxstrips.empty() || Tracker2xmaxstrips.empty() || Tracker2ymaxstrips.empty() || Tracker3xmaxstrips.empty() || Tracker3ymaxstrips.empty() || Tracker4xmaxstrips.empty() || Tracker4ymaxstrips.empty() || APV3E1maxstrips.empty()){
	  GoodEvent[*evtID] = false;
	  cout << "Lost Event" << endl;	  
	}
	//cout << "here?" << endl;
	
	
	else{	  
	  auto NumSubEvents1X = StripSep(Tracker1xmaxstrips, Tracker1xmaxcharges);
	  auto NumSubEvents1Y = StripSep(Tracker1ymaxstrips, Tracker1ymaxcharges);
	  auto NumSubEvents2X = StripSep(Tracker2xmaxstrips, Tracker2xmaxcharges);
	  auto NumSubEvents2Y = StripSep(Tracker2ymaxstrips, Tracker2ymaxcharges);
	  auto NumSubEvents3X = StripSep(Tracker3xmaxstrips, Tracker3xmaxcharges);
	  auto NumSubEvents3Y = StripSep(Tracker3ymaxstrips, Tracker3ymaxcharges);
	  auto NumSubEvents4X = StripSep(Tracker4xmaxstrips, Tracker4xmaxcharges);
	  auto NumSubEvents4Y = StripSep(Tracker4ymaxstrips, Tracker4ymaxcharges);
	  auto NumSubEventsAPV3E1 = StripSep(APV3E1maxstrips, APV3E1maxcharges);//this needs to be changed depending on what APV is being looked at!!

	 
  
	  
	  if(NumSubEvents1X.size() > 1 || NumSubEvents1Y.size() > 1  || NumSubEvents2X.size() > 1 || NumSubEvents2Y.size() > 1  ||NumSubEvents3X.size() > 1 || NumSubEvents3Y.size() > 1  || NumSubEvents4X.size() > 1 || NumSubEvents4Y.size() > 1 || NumSubEventsAPV3E1.size() > 1)
	    GoodEvent[*evtID] = false;	  
	  
	  
	  
	  int max_StripHits = 4;
	  int min_StripHits = 2;	    
	  if(*max_element(NumSubEvents1X.begin(), NumSubEvents1X.end()) > max_StripHits || *min_element(NumSubEvents1X.begin(), NumSubEvents1X.end()) < min_StripHits){GoodEvent[*evtID] = false;}	  
	  if(*max_element(NumSubEvents1Y.begin(), NumSubEvents1Y.end()) > max_StripHits || *min_element(NumSubEvents1Y.begin(), NumSubEvents1Y.end()) < min_StripHits){GoodEvent[*evtID] = false;}
	  if(*max_element(NumSubEvents2X.begin(), NumSubEvents2X.end()) > max_StripHits || *min_element(NumSubEvents2X.begin(), NumSubEvents2X.end()) < min_StripHits){GoodEvent[*evtID] = false;}
	  if(*max_element(NumSubEvents2Y.begin(), NumSubEvents2Y.end()) > max_StripHits || *min_element(NumSubEvents2Y.begin(), NumSubEvents2Y.end()) < min_StripHits){GoodEvent[*evtID] = false;}
	  if(*max_element(NumSubEvents3X.begin(), NumSubEvents3X.end()) > max_StripHits || *min_element(NumSubEvents3X.begin(), NumSubEvents3X.end()) < min_StripHits){GoodEvent[*evtID] = false;}
	  if(*max_element(NumSubEvents3Y.begin(), NumSubEvents3Y.end()) > max_StripHits || *min_element(NumSubEvents3Y.begin(), NumSubEvents3Y.end()) < min_StripHits){GoodEvent[*evtID] = false;}
	  if(*max_element(NumSubEvents4X.begin(), NumSubEvents4X.end()) > max_StripHits || *min_element(NumSubEvents4X.begin(), NumSubEvents4X.end()) < min_StripHits){GoodEvent[*evtID] = false;}
	  if(*max_element(NumSubEvents4Y.begin(), NumSubEvents4Y.end()) > max_StripHits || *min_element(NumSubEvents4Y.begin(), NumSubEvents4Y.end()) < min_StripHits){GoodEvent[*evtID] = false;}
	  if(*max_element(NumSubEventsAPV3E1.begin(), NumSubEventsAPV3E1.end()) > max_StripHits || *min_element(NumSubEventsAPV3E1.begin(), NumSubEventsAPV3E1.end()) < min_StripHits){GoodEvent[*evtID] = false;}
	  
	  if(GoodEvent[*evtID] == true){
	    for(uint p = 0; p < NumSubEvents1Y.size();p++){
	      SMofTheseevents.push_back(NumSubEventsAPV3E1.at(p));
	    }
	  }
	}
      }
      */
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
      
      float num=0;
      float Tracker1xnum=0;      
      float Tracker1ynum=0;
      float Tracker2xnum=0;
      float Tracker2ynum=0;
      float Tracker3xnum=0;
      float Tracker3ynum=0;
      float Tracker4xnum=0;
      float Tracker4ynum=0;
      
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
      
      float totalcharge=0;      
      float Tracker1xtotalcharge=0;      
      float Tracker1ytotalcharge=0;
      float Tracker2xtotalcharge=0;
      float Tracker2ytotalcharge=0;
      float Tracker3xtotalcharge=0;
      float Tracker3ytotalcharge=0;
      float Tracker4xtotalcharge=0;
      float Tracker4ytotalcharge=0;
	
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

      float Currentindex=0;
      float previndex;
      float tolerance = 2;
      
      //new method to find hit locations. Dont assume strip multiplicity to be 3, so we iterate each APV channel until the end of the hits

      if(display_mode == 0 && GoodEvent[*evtID]){
	int k=0;	
	cout << *evtID << endl;
	//cout << maxstrips.empty() << endl;
	for(auto i = maxstrips.begin(); i != maxstrips.end(); i++){    
	  cout << *i <<  ", " << maxcharges[k] << endl;
	  k++;
	}
	
	auto NumSubEvents = StripSep(maxstrips, maxcharges);
	
	cout << "Number of sub events in event " << *evtID << ": " << NumSubEvents.size() << endl;
	
	cout << "Length of each Sub Event: " << endl;

	for(auto i = NumSubEvents.begin(); i != NumSubEvents.end(); i++){
	  cout << "This eventplot had a strip multiplicity of " << *i << endl;     
	  if(*i > 1)//only allow hits with more than one strip
	    Clusterdist[evtype]->Fill(*i);	  
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
      
      if(display_mode == 1 || display_mode == 2 || display_mode == 3 || display_mode == 5){

	      
	//if(GoodEvent[*evtID]){
	  EventIterator.push_back(totalEventiterator);
	  //{
	  
	  //loop to find exact hit location
	  //cout << "Here?" << endl;
	  for(auto i = Tracker1ymaxstrips.begin(); i < Tracker1ymaxstrips.end(); i++){
	    cout << *i << endl;
	  }

	  cout << "LAGD strips" << endl;
	  for(auto i = APV4E4maxstrips.begin(); i < APV4E4maxstrips.end(); i++){
	    cout << *i << endl;
	  }
	  
	  cout << "End of OG vector" << endl;
	  Tracker1xmaxstrips.resize(10,0);
	  Tracker1ymaxstrips.resize(10,0);
	  Tracker2xmaxstrips.resize(10,0);
	  Tracker2ymaxstrips.resize(10,0);
	  Tracker3xmaxstrips.resize(10,0);
	  Tracker3ymaxstrips.resize(10,0);
	  Tracker4xmaxstrips.resize(10,0);
	  Tracker4ymaxstrips.resize(10,0);

	  Tracker1xmaxcharges.resize(10,0);
	  Tracker1ymaxcharges.resize(10,0);
	  Tracker2xmaxcharges.resize(10,0);
	  Tracker2ymaxcharges.resize(10,0);
	  Tracker3xmaxcharges.resize(10,0);
	  Tracker3ymaxcharges.resize(10,0);
	  Tracker4xmaxcharges.resize(10,0);
	  Tracker4ymaxcharges.resize(10,0);

	  APV2maxcharges.resize(10,0);	    
	  APV3E1maxcharges.resize(10,0);
	  APV3E2maxcharges.resize(10,0);	    
	  APV4E3maxcharges.resize(10,0);
	  APV4E4maxcharges.resize(10,0);
	  APV5E3maxcharges.resize(10,0);
	  APV5E4maxcharges.resize(10,0);
	  APV6E3maxcharges.resize(10,0);
	  APV6E4maxcharges.resize(10,0);
	  APV7E1maxcharges.resize(10,0);
	  APV7E2maxcharges.resize(10,0);
	  APV8maxcharges.resize(10,0);
	  APV9maxcharges.resize(10,0);

	  APV2maxstrips.resize(10,0);	    
	  APV3E1maxstrips.resize(10,0);
	  APV3E2maxstrips.resize(10,0);	    
	  APV4E3maxstrips.resize(10,0);
	  APV4E4maxstrips.resize(10,0);
	  APV5E3maxstrips.resize(10,0);
	  APV5E4maxstrips.resize(10,0);
	  APV6E3maxstrips.resize(10,0);
	  APV6E4maxstrips.resize(10,0);
	  APV7E1maxstrips.resize(10,0);
	  APV7E2maxstrips.resize(10,0);
	  APV8maxstrips.resize(10,0);
	  APV9maxstrips.resize(10,0);

	  for(auto i = Tracker1ymaxstrips.begin(); i < Tracker1ymaxstrips.end(); i++){
	    cout << *i << endl;
	  }
	  cout << "End of resized vector" << endl;
	  cout << "APV4E4 strips" << endl;
	  for(auto i = APV4E4maxstrips.begin(); i < APV4E4maxstrips.end(); i++){
	    cout << *i << endl;
	  }
	  
	  for(int m = 0; m < 10; m++){	 
	    
	    if(display_mode == 0){
	      num += maxstrips.at(m)*maxcharges.at(m);
	      totalcharge += maxcharges.at(m);	      
	    }
	    
	  
	    if(display_mode == 1 || display_mode == 2 || display_mode == 5){
	      Tracker1xnum += Tracker1xmaxstrips.at(m)*Tracker1xmaxcharges.at(m);
	      Tracker1ynum += (Tracker1ymaxstrips.at(m))*Tracker1ymaxcharges.at(m);
	      Tracker2xnum += Tracker2xmaxstrips.at(m)*Tracker2xmaxcharges.at(m);
	      Tracker2ynum += (Tracker2ymaxstrips.at(m))*Tracker2ymaxcharges.at(m);	    
	      Tracker3xnum += Tracker3xmaxstrips.at(m)*Tracker3xmaxcharges.at(m);
	      Tracker3ynum += (Tracker3ymaxstrips.at(m))*Tracker3ymaxcharges.at(m);
	      Tracker4xnum += Tracker4xmaxstrips.at(m)*Tracker4xmaxcharges.at(m);
	      Tracker4ynum += (Tracker4ymaxstrips.at(m))*Tracker4ymaxcharges.at(m);

	      
	      Tracker1xtotalcharge += Tracker1xmaxcharges.at(m);
	      Tracker1ytotalcharge += Tracker1ymaxcharges.at(m);
	      Tracker2xtotalcharge += Tracker2xmaxcharges.at(m);
	      Tracker2ytotalcharge += Tracker2ymaxcharges.at(m);	    
	      Tracker3xtotalcharge += Tracker3xmaxcharges.at(m);
	      Tracker3ytotalcharge += Tracker3ymaxcharges.at(m);
	      Tracker4xtotalcharge += Tracker4xmaxcharges.at(m);
	      Tracker4ytotalcharge += Tracker4ymaxcharges.at(m);
	    }
	  

	    if(display_mode == 3 || display_mode == 1  || AlignmentMode == 0){

	      APV2num += APV2maxstrips.at(m)*APV2maxcharges.at(m);	    
	      APV3E1num += APV3E1maxstrips.at(m)*APV3E1maxcharges.at(m);
	      APV3E2num += APV3E2maxstrips.at(m)*APV3E2maxcharges.at(m);	    
	      APV4E3num += APV4E3maxstrips.at(m)*APV4E3maxcharges.at(m);
	      APV4E4num += APV4E4maxstrips.at(m)*APV4E4maxcharges.at(m);	    
	      APV5E3num += APV5E3maxstrips.at(m)*APV5E3maxcharges.at(m);
	      APV5E4num += APV5E4maxstrips.at(m)*APV5E4maxcharges.at(m);	    
	      APV6E3num += APV6E3maxstrips.at(m)*APV6E3maxcharges.at(m);
	      APV6E4num += APV6E4maxstrips.at(m)*APV6E4maxcharges.at(m);
	      APV7E1num += APV7E1maxstrips.at(m)*APV7E1maxcharges.at(m);
	      APV7E2num += APV7E2maxstrips.at(m)*APV7E2maxcharges.at(m);	    
	      APV8num += APV8maxstrips.at(m)*APV8maxcharges.at(m);
	      APV9num += APV9maxstrips.at(m)*APV9maxcharges.at(m);	    
	      
	     
	      //striphitit++;
	      
	    	   
	      APV2totalcharge += APV2maxcharges.at(m);	    
	      APV3E1totalcharge += APV3E1maxcharges.at(m);
	      APV3E2totalcharge += APV3E2maxcharges.at(m);	    
	      APV4E3totalcharge += APV4E3maxcharges.at(m);
	      APV4E4totalcharge += APV4E4maxcharges.at(m);	    
	      APV5E3totalcharge += APV5E3maxcharges.at(m);
	      APV5E4totalcharge += APV5E4maxcharges.at(m);	    
	      APV6E3totalcharge += APV6E3maxcharges.at(m);
	      APV6E4totalcharge += APV6E4maxcharges.at(m);
	      APV7E1totalcharge += APV7E1maxcharges.at(m);
	      APV7E2totalcharge += APV7E2maxcharges.at(m);	    
	      APV8totalcharge += APV8maxcharges.at(m);
	      APV9totalcharge += APV9maxcharges.at(m);	  

	    }

	  }
	  Clusterchargeoftheseevnts.push_back(APV3E1totalcharge);

	  cout << "Tx: " << Tracker1xtotalcharge << ", " << "Ty: " << Tracker1ytotalcharge << endl;
	  T1ChargeRatio->Fill(Tracker1xtotalcharge, Tracker1ytotalcharge);
	      
	  //Other hits
	  //cout << "num: " << num << " and total charge " << totalcharge << endl;
	  if(display_mode == 0)
	    loc = num/totalcharge;
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
	    APV2loc = APV2num/APV2totalcharge;	    
	    APV3E1loc = APV3E1num/APV3E1totalcharge;
	    APV3E2loc = APV3E2num/APV3E2totalcharge;	    
	    APV4E3loc = APV4E3num/APV4E3totalcharge;
	    APV4E4loc = APV4E4num/APV4E4totalcharge;	    
	    APV5E3loc = APV5E3num/APV5E3totalcharge;
	    APV5E4loc = APV5E4num/APV5E4totalcharge;	    
	    APV6E3loc = APV6E3num/APV6E3totalcharge;
	    APV6E4loc = APV6E4num/APV6E4totalcharge;
	    APV7E1loc = APV7E1num/APV7E1totalcharge;
	    APV7E2loc = APV7E2num/APV7E2totalcharge;	    
	    APV8loc = APV8num/APV8totalcharge;
	    APV9loc = APV9num/APV9totalcharge;

	    
	    
	    LAGDHitsVec.push_back(APV3E1loc);
	    EventIDs.push_back(totalEventiterator);
	    //efficiencynum++;
	  }       
	  
	  Stripcorr->SetPoint(Stripcorrit, APV4E4loc, Tracker1yloc);
	  TrackerStripcorr->SetPoint(Stripcorrit, Tracker1yloc, Tracker2yloc);	  
	  LAGDstrips->SetPoint(Stripcorrit, Stripcorrit, APV4E4loc);
	  Trackerstrips->SetPoint(Stripcorrit, Stripcorrit, Tracker1yloc);
	  //Diffbtwnstrips->SetPoint(Stripcorrit, Stripcorrit, Tracker1yloc - APV3E1loc);
	  Stripcorrit++;
	  
	  cout << "Hit at (" << Tracker1xloc << "," << Tracker1yloc << ") in Tracker 1" << endl;
	  cout << "Hit at (" << Tracker2xloc << "," << Tracker2yloc << ") in Tracker 2" << endl;
	  cout << "Hit at (" << Tracker3xloc << "," << Tracker3yloc << ") in Tracker 3" << endl;
	  cout << "Hit at (" << Tracker4xloc << "," << Tracker4yloc << ") in Tracker 4" << endl;
	  cout << "Hit at (" << APV4E3loc << ")" << " in APV4E3" << endl;
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



	  if(display_mode == 2 || display_mode == 5){
	    auto T1xcoord = Tracker1xloc*0.4; //0.4 = 51.2/128	    
	    auto T1ycoord = Tracker1yloc*0.4;//0.4 = 102.4/256
	    auto T2xcoord = Tracker2xloc*0.4;
	    auto T2ycoord = Tracker2yloc*0.4;

	    cout << APV3E1loc << endl;
	    Tracker2Ystriphits->Fill(APV3E1loc);
	    
	    auto T3xcoord = Tracker3xloc*0.4;
	    auto T3ycoord = Tracker3yloc*0.4;
	    auto T4xcoord = Tracker4xloc*0.4;
	    auto T4ycoord = Tracker4yloc*0.4;

	    TrackerEventIDs.push_back(totalEventiterator);
	    //cout << "Pushing back hit coordinates" << endl;
	    Tracker1hitcoords[evtype].push_back(T1xcoord);	    
	    Tracker1hitcoords[evtype].push_back(T1ycoord);
	    Tracker1hitcoords[evtype].push_back(1541.6);//1596.4	  
	    
	    Tracker2hitcoords[evtype].push_back(T2xcoord);
	    Tracker2hitcoords[evtype].push_back(T2ycoord);
	    Tracker2hitcoords[evtype].push_back(0);//1541.6
	    
	    Tracker3hitcoords[evtype].push_back(T3xcoord);
	    Tracker3hitcoords[evtype].push_back(T3ycoord);
	    Tracker3hitcoords[evtype].push_back(54.8);//54.8
	    
	    Tracker4hitcoords[evtype].push_back(T4xcoord);
	    Tracker4hitcoords[evtype].push_back(T4ycoord);
	    Tracker4hitcoords[evtype].push_back(1596.4);//0
	    
	    if(display_mode == 2){
	      Tracker1Hitmap[evtype]->Fill(T1xcoord, T1ycoord);
	      Tracker3Hitmap[evtype]->Fill(T3xcoord, T3ycoord);
	      Tracker2Hitmap[evtype]->Fill(T2xcoord, T2ycoord);
	      Tracker4Hitmap[evtype]->Fill(T4xcoord, T4ycoord);
	    }
	    
	  }	  
      }
    
      
      
      //cout << "Made it to here" << endl;
                 
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

	  //PulseHeightHist[*evtID]->Draw("LEGO2");
	  
	  PulseHeight->SetMarkerStyle(20);
	  PulseHeight->Draw("pcol");
	  
	  PulseHeight->GetXaxis()->SetTitle("Strip");
	  PulseHeight->GetXaxis()->CenterTitle(true);
	  PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  PulseHeight->GetYaxis()->CenterTitle(true);
	  PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  PulseHeight->GetZaxis()->CenterTitle(true);
	  
	  
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
	  
	PHCanvas[*evtID] = new TCanvas(charsubtitle, chartitle, 1000, 1500);
	//cout << "yes" << endl;
	  
	PHCanvas[*evtID]->Divide(5,2);
	PHCanvas[*evtID]->Draw();
	
	//first column of display (APV3 and APV7)
	PHCanvas[*evtID]->cd(1);
	PHCanvas[*evtID]->DrawFrame(0,0,63,30);
	  
	//APV3E1PulseHeight->SetAxisRange(0.0,63.0,"X");
	//APV3E1PulseHeight->SetAxisRange(0.0,400.0,"Z");
	  
	APV3E1PulseHeight->Draw("LEGO");

	APV3E1PulseHeight->SetTitle("APV3");
	APV3E1PulseHeight->GetXaxis()->SetTitle("Strip");
	APV3E1PulseHeight->GetXaxis()->CenterTitle(true);
	APV3E1PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV3E1PulseHeight->GetYaxis()->CenterTitle(true);
	APV3E1PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV3E1PulseHeight->GetZaxis()->CenterTitle(true);
	  

	PHCanvas[*evtID]->cd(6);
	APV7E1PulseHeight->Draw("LEGO");
	APV7E1PulseHeight->SetTitle("APV7");
	APV7E1PulseHeight->GetXaxis()->SetTitle("Strip");
	APV7E1PulseHeight->GetXaxis()->CenterTitle(true);
	APV7E1PulseHeight->GetXaxis()->SetRange(0,63);
	APV7E1PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV7E1PulseHeight->GetYaxis()->CenterTitle(true);
	APV7E1PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV7E1PulseHeight->GetZaxis()->CenterTitle(true);
	APV7E1PulseHeight->GetZaxis()->SetRange(0,400);
		  

	//second column
	PHCanvas[*evtID]->cd(2);
	PHCanvas[*evtID]->DrawFrame(64,0,128,30);
	APV3E2PulseHeight->GetXaxis()->SetRangeUser(64,127);
	APV3E2PulseHeight->GetZaxis()->SetRangeUser(0,400);
	  
	APV3E2PulseHeight->Draw("LEGO");
	  
	APV3E2PulseHeight->SetTitle("APV3");
	APV3E2PulseHeight->GetXaxis()->SetTitle("Strip");
	APV3E2PulseHeight->GetXaxis()->CenterTitle(true);
	APV3E2PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV3E2PulseHeight->GetYaxis()->CenterTitle(true);
	APV3E2PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV3E2PulseHeight->GetZaxis()->CenterTitle(true);
	  
	
	PHCanvas[*evtID]->cd(7);
	APV7E2PulseHeight->Draw("LEGO");
	APV7E2PulseHeight->SetTitle("APV7");
	APV7E2PulseHeight->GetXaxis()->SetTitle("Strip");
	APV7E2PulseHeight->GetXaxis()->CenterTitle(true);
	APV7E2PulseHeight->GetXaxis()->SetRangeUser(64,127);
	APV7E2PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV7E2PulseHeight->GetYaxis()->CenterTitle(true);
	APV7E2PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV7E2PulseHeight->GetZaxis()->CenterTitle(true);
	APV7E2PulseHeight->GetZaxis()->SetRangeUser(0,400);
	
	//third colmun
	/*
	  PHCanvas[*evtID]->cd(3);
	  APV2PulseHeight->Draw("LEGO");
	  APV2PulseHeight->SetTitle("APV2");
	  APV2PulseHeight->GetXaxis()->SetTitle("Strip");
	  APV2PulseHeight->GetXaxis()->CenterTitle(true);
	  APV2PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  APV2PulseHeight->GetYaxis()->CenterTitle(true);
	  APV2PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  APV2PulseHeight->GetZaxis()->CenterTitle(true);

	  PHCanvas[*evtID]->cd(8);
	  APV2PulseHeight->Draw("LEGO");
	  APV2PulseHeight->SetTitle("APV2");
	  APV2PulseHeight->GetXaxis()->SetTitle("Strip");
	  APV2PulseHeight->GetXaxis()->CenterTitle(true);
	  APV2PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  APV2PulseHeight->GetYaxis()->CenterTitle(true);
	  APV2PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  APV2PulseHeight->GetZaxis()->CenterTitle(true);
	*/
	PHCanvas[*evtID]->cd(3);
	APV6E3PulseHeight->Draw("LEGO");
	APV6E3PulseHeight->SetTitle("APV6");
	APV6E3PulseHeight->GetXaxis()->SetTitle("Strip");
	APV6E3PulseHeight->GetXaxis()->CenterTitle(true);
	APV6E3PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV6E3PulseHeight->GetYaxis()->CenterTitle(true);
	APV6E3PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV6E3PulseHeight->GetZaxis()->CenterTitle(true);
	APV6E3PulseHeight->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(8);
	APV6E4PulseHeight->Draw("LEGO");
	APV6E4PulseHeight->SetTitle("APV6");
	APV6E4PulseHeight->GetXaxis()->SetTitle("Strip");
	APV6E4PulseHeight->GetXaxis()->CenterTitle(true);
	APV6E4PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV6E4PulseHeight->GetYaxis()->CenterTitle(true);
	APV6E4PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV6E4PulseHeight->GetZaxis()->CenterTitle(true);
	APV6E4PulseHeight->GetZaxis()->SetRange(0,400);
	/*
	  PHCanvas[*evtID]->cd(23);
	  APV8PulseHeight->Draw("LEGO");
	  APV8PulseHeight->SetTitle("APV8");
	  APV8PulseHeight->GetXaxis()->SetTitle("Strip");
	  APV8PulseHeight->GetXaxis()->CenterTitle(true);
	  APV8PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  APV8PulseHeight->GetYaxis()->CenterTitle(true);
	  APV8PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  APV8PulseHeight->GetZaxis()->CenterTitle(true);

	  PHCanvas[*evtID]->cd(28);
	  APV8PulseHeight->Draw("LEGO");
	  APV8PulseHeight->SetTitle("APV8");
	  APV8PulseHeight->GetXaxis()->SetTitle("Strip");
	  APV8PulseHeight->GetXaxis()->CenterTitle(true);
	  APV8PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  APV8PulseHeight->GetYaxis()->CenterTitle(true);
	  APV8PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  APV8PulseHeight->GetZaxis()->CenterTitle(true);
	*/
	//fourth column
	/*
	  PHCanvas[*evtID]->cd(4);
	  APV2PulseHeight->Draw("LEGO");
	  APV2PulseHeight->SetTitle("APV2/1");
	  APV2PulseHeight->GetXaxis()->SetTitle("Strip");
	  APV2PulseHeight->GetXaxis()->CenterTitle(true);
	  APV2PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  APV2PulseHeight->GetYaxis()->CenterTitle(true);
	  APV2PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  APV2PulseHeight->GetZaxis()->CenterTitle(true);

	  PHCanvas[*evtID]->cd(9);
	  APV2PulseHeight->Draw("LEGO");
	  APV2PulseHeight->SetTitle("APV2/1");
	  APV2PulseHeight->GetXaxis()->SetTitle("Strip");
	  APV2PulseHeight->GetXaxis()->CenterTitle(true);
	  APV2PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  APV2PulseHeight->GetYaxis()->CenterTitle(true);
	  APV2PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  APV2PulseHeight->GetZaxis()->CenterTitle(true);
	*/
	PHCanvas[*evtID]->cd(4);
	APV5E3PulseHeight->Draw("LEGO");
	APV5E3PulseHeight->SetTitle("APV5");
	APV5E3PulseHeight->GetXaxis()->SetTitle("Strip");
	APV5E3PulseHeight->GetXaxis()->CenterTitle(true);
	APV5E3PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV5E3PulseHeight->GetYaxis()->CenterTitle(true);
	APV5E3PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV5E3PulseHeight->GetZaxis()->CenterTitle(true);
	APV5E3PulseHeight->GetZaxis()->SetRange(0,400);
 
	PHCanvas[*evtID]->cd(9);
	APV5E4PulseHeight->Draw("LEGO");
	APV5E4PulseHeight->SetTitle("APV5");
	APV5E4PulseHeight->GetXaxis()->SetTitle("Strip");
	APV5E4PulseHeight->GetXaxis()->CenterTitle(true);
	APV5E4PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV5E4PulseHeight->GetYaxis()->CenterTitle(true);
	APV5E4PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV5E4PulseHeight->GetZaxis()->CenterTitle(true);
	APV5E4PulseHeight->GetZaxis()->SetRange(0,400);
	/*
	  PHCanvas[*evtID]->cd(24);
	  APV8PulseHeight->Draw("LEGO");
	  APV8PulseHeight->SetTitle("APV8");
	  APV8PulseHeight->GetXaxis()->SetTitle("Strip");
	  APV8PulseHeight->GetXaxis()->CenterTitle(true);
	  APV8PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  APV8PulseHeight->GetYaxis()->CenterTitle(true);
	  APV8PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  APV8PulseHeight->GetZaxis()->CenterTitle(true);

	  PHCanvas[*evtID]->cd(29);
	  APV8PulseHeight->Draw("LEGO");
	  APV8PulseHeight->SetTitle("APV8");
	  APV8PulseHeight->GetXaxis()->SetTitle("Strip");
	  APV8PulseHeight->GetXaxis()->CenterTitle(true);
	  APV8PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  APV8PulseHeight->GetYaxis()->CenterTitle(true);
	  APV8PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  APV8PulseHeight->GetZaxis()->CenterTitle(true);
	*/
	//last column

	PHCanvas[*evtID]->cd(5);
	APV4E3PulseHeight->Draw("LEGO");
	APV4E3PulseHeight->SetTitle("APV4");
	APV4E3PulseHeight->GetXaxis()->SetTitle("Strip");
	APV4E3PulseHeight->GetXaxis()->CenterTitle(true);
	APV4E3PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV4E3PulseHeight->GetYaxis()->CenterTitle(true);
	APV4E3PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV4E3PulseHeight->GetZaxis()->CenterTitle(true);
	APV4E3PulseHeight->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(10);
	APV4E4PulseHeight->Draw("LEGO");
	APV4E4PulseHeight->SetTitle("APV4");
	APV4E4PulseHeight->GetXaxis()->SetTitle("Strip");
	APV4E4PulseHeight->GetXaxis()->CenterTitle(true);
	APV4E4PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV4E4PulseHeight->GetYaxis()->CenterTitle(true);
	APV4E4PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV4E4PulseHeight->GetZaxis()->CenterTitle(true);
	APV4E4PulseHeight->GetZaxis()->SetRange(0,400);
	/*
	  PHCanvas[*evtID]->cd(25);
	  APV9PulseHeight->Draw("LEGO");
	  APV9PulseHeight->SetTitle("APV9");
	  APV9PulseHeight->GetXaxis()->SetTitle("Strip");
	  APV9PulseHeight->GetXaxis()->CenterTitle(true);
	  APV9PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  APV9PulseHeight->GetYaxis()->CenterTitle(true);
	  APV9PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  APV9PulseHeight->GetZaxis()->CenterTitle(true);

	  PHCanvas[*evtID]->cd(30);
	  APV9PulseHeight->Draw("LEGO");
	  APV9PulseHeight->SetTitle("APV9");
	  APV9PulseHeight->GetXaxis()->SetTitle("Strip");
	  APV9PulseHeight->GetXaxis()->CenterTitle(true);
	  APV9PulseHeight->GetYaxis()->SetTitle("TimeBin");
	  APV9PulseHeight->GetYaxis()->CenterTitle(true);
	  APV9PulseHeight->GetZaxis()->SetTitle("ADC charge");
	  APV9PulseHeight->GetZaxis()->CenterTitle(true);
	*/
      }
	
      //Trackers and LAGD APVs
	
      if(display_mode == 1){
	  	  
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
	  
	PHCanvas[*evtID] = new TCanvas(charsubtitle, chartitle, 1000, 1500);
	//cout << "no" << endl;
	PHCanvas[*evtID]->Divide(7,4);
	PHCanvas[*evtID]->Draw();
	  	
	//trackers first
	PHCanvas[*evtID]->cd(1);
	//PHCanvas[*evtID]->SetTheta(-45);
	//PHCanvas[*evtID]->SetPhi(0);
	
	Tracker1XPulseHeight->SetMarkerStyle(20);
	//Tracker1XPulseHeight->SetFillColor(1);
	Tracker1XPulseHeight->Draw("pcol");
	Tracker1XPulseHeight->SetTitle("GEM1X");
	Tracker1XPulseHeight->GetXaxis()->SetTitle("Strip");
	Tracker1XPulseHeight->GetXaxis()->CenterTitle(true);
	//Tracker1XPulseHeight->GetXaxis()->SetRangeUser(0,127);
	Tracker1XPulseHeight->GetYaxis()->SetTitle("TimeBin");
	Tracker1XPulseHeight->GetYaxis()->CenterTitle(true);
	Tracker1XPulseHeight->GetZaxis()->SetTitle("ADC charge");
	Tracker1XPulseHeight->GetZaxis()->CenterTitle(true);
	Tracker1XPulseHeight->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(2);
	Tracker1YPulseHeight->SetMarkerStyle(20);	
	Tracker1YPulseHeight->Draw("pcol");
	Tracker1YPulseHeight->SetTitle("GEM1Y");
	Tracker1YPulseHeight->GetXaxis()->SetTitle("Strip");
	Tracker1YPulseHeight->GetXaxis()->CenterTitle(true);
	//Tracker1YPulseHeight->GetXaxis()->SetRangeUser(128,255);
	Tracker1YPulseHeight->GetYaxis()->SetTitle("TimeBin");
	Tracker1YPulseHeight->GetYaxis()->CenterTitle(true);
	Tracker1YPulseHeight->GetZaxis()->SetTitle("ADC charge");
	Tracker1YPulseHeight->GetZaxis()->CenterTitle(true);
	Tracker1YPulseHeight->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(8);
	Tracker2XPulseHeight->SetMarkerStyle(20);
	Tracker2XPulseHeight->Draw("pcol");
	Tracker2XPulseHeight->SetTitle("GEM2X");
	Tracker2XPulseHeight->GetXaxis()->SetTitle("Strip");
	Tracker2XPulseHeight->GetXaxis()->CenterTitle(true);
	//Tracker2XPulseHeight->GetXaxis()->SetRangeUser(0,127);
	Tracker2XPulseHeight->GetYaxis()->SetTitle("TimeBin");
	Tracker2XPulseHeight->GetYaxis()->CenterTitle(true);
	Tracker2XPulseHeight->GetZaxis()->SetTitle("ADC charge");
	Tracker2XPulseHeight->GetZaxis()->CenterTitle(true);
	Tracker2XPulseHeight->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(9);
	Tracker2YPulseHeight->SetMarkerStyle(20);
	Tracker2YPulseHeight->Draw("pcol");
	Tracker2YPulseHeight->SetTitle("GEM2Y");
	Tracker2YPulseHeight->GetXaxis()->SetTitle("Strip");
	Tracker2YPulseHeight->GetXaxis()->CenterTitle(true);
	//Tracker2YPulseHeight->GetXaxis()->SetRangeUser(128,255);
	Tracker2YPulseHeight->GetYaxis()->SetTitle("TimeBin");
	Tracker2YPulseHeight->GetYaxis()->CenterTitle(true);
	Tracker2YPulseHeight->GetZaxis()->SetTitle("ADC charge");
	Tracker2YPulseHeight->GetZaxis()->CenterTitle(true);
	Tracker2YPulseHeight->GetZaxis()->SetRangeUser(0,400);
 
	PHCanvas[*evtID]->cd(15);
	Tracker3XPulseHeight->SetMarkerStyle(20);
	Tracker3XPulseHeight->Draw("pcol");
	Tracker3XPulseHeight->SetTitle("GEM3X");
	Tracker3XPulseHeight->GetXaxis()->SetTitle("Strip");
	Tracker3XPulseHeight->GetXaxis()->CenterTitle(true);
	//Tracker3XPulseHeight->GetXaxis()->SetRangeUser(0,127);
	Tracker3XPulseHeight->GetYaxis()->SetTitle("TimeBin");
	Tracker3XPulseHeight->GetYaxis()->CenterTitle(true);
	Tracker3XPulseHeight->GetZaxis()->SetTitle("ADC charge");
	Tracker3XPulseHeight->GetZaxis()->CenterTitle(true);
	Tracker3XPulseHeight->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(16);
	Tracker3YPulseHeight->SetMarkerStyle(20);
	Tracker3YPulseHeight->Draw("pcol");
	Tracker3YPulseHeight->SetTitle("GEM3Y");
	Tracker3YPulseHeight->GetXaxis()->SetTitle("Strip");
	Tracker3YPulseHeight->GetXaxis()->CenterTitle(true);
	//Tracker3YPulseHeight->GetXaxis()->SetRangeUser(128,255);
	Tracker3YPulseHeight->GetYaxis()->SetTitle("TimeBin");
	Tracker3YPulseHeight->GetYaxis()->CenterTitle(true);
	Tracker3YPulseHeight->GetZaxis()->SetTitle("ADC charge");
	Tracker3YPulseHeight->GetZaxis()->CenterTitle(true);
	Tracker3YPulseHeight->GetZaxis()->SetRangeUser(0,400);
 
	PHCanvas[*evtID]->cd(22);
	Tracker4XPulseHeight->SetMarkerStyle(20);
	Tracker4XPulseHeight->Draw("pcol");
	Tracker4XPulseHeight->SetTitle("GEM4X");
	Tracker4XPulseHeight->GetXaxis()->SetTitle("Strip");
	Tracker4XPulseHeight->GetXaxis()->CenterTitle(true);
	//Tracker4XPulseHeight->GetXaxis()->SetRangeUser(0,127);
	Tracker4XPulseHeight->GetYaxis()->SetTitle("TimeBin");
	Tracker4XPulseHeight->GetYaxis()->CenterTitle(true);
	Tracker4XPulseHeight->GetZaxis()->SetTitle("ADC charge");
	Tracker4XPulseHeight->GetZaxis()->CenterTitle(true);
	Tracker4XPulseHeight->GetZaxis()->SetRangeUser(0,400);
 
	PHCanvas[*evtID]->cd(23);
	Tracker4YPulseHeight->SetMarkerStyle(20);
	Tracker4YPulseHeight->Draw("pcol");
	Tracker4YPulseHeight->SetTitle("GEM4Y");
	Tracker4YPulseHeight->GetXaxis()->SetTitle("Strip");
	Tracker4YPulseHeight->GetXaxis()->CenterTitle(true);
	//Tracker4YPulseHeight->GetXaxis()->SetRangeUser(128,255);
	Tracker4YPulseHeight->GetYaxis()->SetTitle("TimeBin");
	Tracker4YPulseHeight->GetYaxis()->CenterTitle(true);
	Tracker4YPulseHeight->GetZaxis()->SetTitle("ADC charge");
	Tracker4YPulseHeight->GetZaxis()->CenterTitle(true);	
	Tracker4YPulseHeight->GetZaxis()->SetRange(0,400);
	  
	//Now we can add the LAGD APVs
	PHCanvas[*evtID]->cd(10);
	APV3E1PulseHeight->SetMarkerStyle(20);
	APV3E1PulseHeight->Draw("pcol");
	APV3E1PulseHeight->SetTitle("APV3 E1");
	APV3E1PulseHeight->GetXaxis()->SetTitle("Strip");
	APV3E1PulseHeight->GetXaxis()->CenterTitle(true);
	//APV3E1PulseHeight->GetXaxis()->SetRangeUser(0,63);
	APV3E1PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV3E1PulseHeight->GetYaxis()->CenterTitle(true);
	APV3E1PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV3E1PulseHeight->GetZaxis()->CenterTitle(true);
	APV3E1PulseHeight->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(11);
	APV3E2PulseHeight->SetMarkerStyle(20);
	APV3E2PulseHeight->Draw("pcol");
	APV3E2PulseHeight->SetTitle("APV3 E2");
	APV3E2PulseHeight->GetXaxis()->SetTitle("Strip");
	APV3E2PulseHeight->GetXaxis()->CenterTitle(true);
	//APV3E2PulseHeight->GetXaxis()->SetRangeUser(64,127);
	APV3E2PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV3E2PulseHeight->GetYaxis()->CenterTitle(true);
	APV3E2PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV3E2PulseHeight->GetZaxis()->CenterTitle(true);
	APV3E2PulseHeight->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(12);
	APV6E3PulseHeight->SetMarkerStyle(20);
	APV6E3PulseHeight->Draw("pcol");
	APV6E3PulseHeight->SetTitle("APV6 E3");
	APV6E3PulseHeight->GetXaxis()->SetTitle("Strip");
	//APV6E3PulseHeight->GetXaxis()->SetRangeUser(0,63);
	APV6E3PulseHeight->GetXaxis()->CenterTitle(true);
	APV6E3PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV6E3PulseHeight->GetYaxis()->CenterTitle(true);
	APV6E3PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV6E3PulseHeight->GetZaxis()->CenterTitle(true);
	APV6E3PulseHeight->GetZaxis()->SetRangeUser(0,400);
 
	PHCanvas[*evtID]->cd(13);
	APV5E3PulseHeight->SetMarkerStyle(20);
	APV5E3PulseHeight->Draw("pcol");
	APV5E3PulseHeight->SetTitle("APV5 E3");
	APV5E3PulseHeight->GetXaxis()->SetTitle("Strip");
	//APV5E3PulseHeight->GetXaxis()->SetRangeUser(0,63);
	APV5E3PulseHeight->GetXaxis()->CenterTitle(true);
	APV5E3PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV5E3PulseHeight->GetYaxis()->CenterTitle(true);
	APV5E3PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV5E3PulseHeight->GetZaxis()->CenterTitle(true);
	APV5E3PulseHeight->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(14);
	APV4E3PulseHeight->SetMarkerStyle(20);
	APV4E3PulseHeight->Draw("pcol");
	APV4E3PulseHeight->SetTitle("APV4 E3");
	APV4E3PulseHeight->GetXaxis()->SetTitle("Strip");
	//APV4E3PulseHeight->GetXaxis()->SetRangeUser(0,63);
	APV4E3PulseHeight->GetXaxis()->CenterTitle(true);
	APV4E3PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV4E3PulseHeight->GetYaxis()->CenterTitle(true);
	APV4E3PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV4E3PulseHeight->GetZaxis()->CenterTitle(true);
	APV4E3PulseHeight->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(17);
	APV7E1PulseHeight->SetMarkerStyle(20);
	APV7E1PulseHeight->Draw("pcol");
	APV7E1PulseHeight->SetTitle("APV7 E1");
	APV7E1PulseHeight->GetXaxis()->SetTitle("Strip");
	APV7E1PulseHeight->GetXaxis()->CenterTitle(true);
	//APV7E1PulseHeight->GetXaxis()->SetRangeUser(0,63);
	APV7E1PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV7E1PulseHeight->GetYaxis()->CenterTitle(true);
	APV7E1PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV7E1PulseHeight->GetZaxis()->CenterTitle(true);
	APV7E1PulseHeight->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(18);
	APV7E2PulseHeight->SetMarkerStyle(20);
	APV7E2PulseHeight->Draw("pcol");
	APV7E2PulseHeight->SetTitle("APV7 E2");
	APV7E2PulseHeight->GetXaxis()->SetTitle("Strip");
	APV7E2PulseHeight->GetXaxis()->CenterTitle(true);
	//APV7E2PulseHeight->GetXaxis()->SetRangeUser(64,127);
	APV7E2PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV7E2PulseHeight->GetYaxis()->CenterTitle(true);
	APV7E2PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV7E2PulseHeight->GetZaxis()->CenterTitle(true);
	APV7E2PulseHeight->GetZaxis()->SetRange(0,400);
	  
	PHCanvas[*evtID]->cd(19);
	APV6E4PulseHeight->SetMarkerStyle(20);
	APV6E4PulseHeight->Draw("pcol");
	APV6E4PulseHeight->SetTitle("APV6 E4");
	APV6E4PulseHeight->GetXaxis()->SetTitle("Strip");
	//APV6E4PulseHeight->GetXaxis()->SetRangeUser(64,127);
	APV6E4PulseHeight->GetXaxis()->CenterTitle(true);
	APV6E4PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV6E4PulseHeight->GetYaxis()->CenterTitle(true);
	APV6E4PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV6E4PulseHeight->GetZaxis()->CenterTitle(true);
	APV6E4PulseHeight->GetZaxis()->SetRangeUser(0,400);
 
	PHCanvas[*evtID]->cd(20);
	APV5E4PulseHeight->SetMarkerStyle(20);
	APV5E4PulseHeight->Draw("pcol");
	APV5E4PulseHeight->SetTitle("APV5 E4");
	APV5E4PulseHeight->GetXaxis()->SetTitle("Strip");
	//APV5E4PulseHeight->GetXaxis()->SetRangeUser(64,127);
	APV5E4PulseHeight->GetXaxis()->CenterTitle(true);
	APV5E4PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV5E4PulseHeight->GetYaxis()->CenterTitle(true);
	APV5E4PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV5E4PulseHeight->GetZaxis()->CenterTitle(true);
	APV5E4PulseHeight->GetZaxis()->SetRangeUser(0,400);
	  
	PHCanvas[*evtID]->cd(21);
	APV4E4PulseHeight->SetMarkerStyle(20);
	APV4E4PulseHeight->Draw("pcol");
	APV4E4PulseHeight->SetTitle("APV4 E4");
	APV4E4PulseHeight->GetXaxis()->SetTitle("Strip");
	//APV4E4PulseHeight->GetXaxis()->SetRangeUser(64,127);
	APV4E4PulseHeight->GetXaxis()->CenterTitle(true);
	APV4E4PulseHeight->GetYaxis()->SetTitle("TimeBin");
	APV4E4PulseHeight->GetYaxis()->CenterTitle(true);
	APV4E4PulseHeight->GetZaxis()->SetTitle("ADC charge");
	APV4E4PulseHeight->GetZaxis()->CenterTitle(true);
	APV4E4PulseHeight->GetZaxis()->SetRange(0,400);
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


    cout <<  LAGDhitsnum << "/" << Trackerhits << " = " << LAGDhitsnum/Trackerhits << endl;
    EffperHV->SetPoint(fileit, stoi(evtype), LAGDhitsnum/Trackerhits);
    fileit++;
    
    
    if(display_mode == 5){
      //return;
      //do something w the tracker hit vector to store them for big analyis (just need to be careful about different files combining data)((idc for rn tho sooooooooooooooooooooooooooooo))
      //or maybe add all the residual histograms together at the end somehow?
      //cout << efficiencynum << "/" << Tracker1hitcoords[evtype].size()/3 << " = " << efficiencynum/(Tracker1hitcoords[evtype].size()/3) << endl;
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
    
    if(Clusterdistevents || Gaininfo){
      if(inputfiles.size() == 1){
	Clusterdist[evtype]->Draw();
	Clusterdist[evtype]->SetTitle("Strip Multiplicity Distribution of Large ZigZag Strip Sector");
	Clusterdist[evtype]->GetXaxis()->SetTitle("Strip Multiplicity");
	Clusterdist[evtype]->GetXaxis()->CenterTitle(true);
	Clusterdist[evtype]->GetYaxis()->SetTitle("Events");
	Clusterdist[evtype]->GetYaxis()->CenterTitle(true);
	Clusterdist[evtype]->SaveAs("StripMultiplicity_LargeeZigZag.png");
	return;
      }
	  
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
      ClusterErrors.push_back((Clusterdist[evtype]->GetMeanError()));
      HVsetting.push_back(stoi(evtype));
      HVErrors.push_back(0);
	  
    }
    //cout << "yes?" << endl;
   
    
    if(display_mode == 2 && ChargeRatio != true){
      TrackerCanvas[evtype] = new TCanvas("ph", "Trackers", 1000, 1000);
      TrackerCanvas[evtype]->Divide(2,2);
      TrackerCanvas[evtype]->Draw();
      
      TrackerCanvas[evtype]->cd(1);	
      Tracker1Hitmap[evtype]->Draw("colz");
      cout << "Events in Tracker 1: " << Tracker1Hitmap[evtype]->Integral() << endl;  
      Tracker1Hitmap[evtype]->SetTitle("Tracker GEM 1 Hitmap");

      TrackerCanvas[evtype]->cd(3);
      Tracker3Hitmap[evtype]->Draw("colz");
      cout << "Events in Tracker 3: " << Tracker3Hitmap[evtype]->Integral() << endl;  
      Tracker3Hitmap[evtype]->SetTitle("Tracker GEM 3 Hitmap");

      TrackerCanvas[evtype]->cd(2);	
      Tracker2Hitmap[evtype]->Draw("colz");
      cout << "Events in Tracker 2: " << Tracker2Hitmap[evtype]->Integral() << endl;  
      Tracker2Hitmap[evtype]->SetTitle("Tracker GEM 2 Hitmap");

      TrackerCanvas[evtype]->cd(4);
      Tracker4Hitmap[evtype]->Draw("colz");
      cout << "Events in Tracker 4: " << Tracker4Hitmap[evtype]->Integral() << endl;  
      Tracker4Hitmap[evtype]->SetTitle("Tracker GEM 4 Hitmap");
      
    } 
  }//////////////////////////////////////////////////////////////////////////////end of loop over files

  //EffperHV->Draw("A*");
  //return;
  if(Stripcorr->GetN() == 0){cout << "No events :(" << endl;}
  else{
    auto h = new TCanvas("", "", 1000, 500);
    h->Divide(2,1);
    h->cd(1);
    Stripcorr->Draw("A*");
    Stripcorr->SetTitle("Correlation between Y coordinate of LAGD and Tracker Hits");
    Stripcorr->GetXaxis()->SetTitle("LAGD hit location [strip]");
    Stripcorr->GetYaxis()->SetTitle("Tracker hit location [strip]");
    
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
  }
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
    //cout << "yess?" << endl;
    /*
    for(auto i = TotalTracker1hitcoords.begin(); i != TotalTracker1hitcoords.end(); i++)
      cout << *i << endl;	      
    */
    /*
    cout << TrackerEventIDs.size() << endl;
    cout << EventIDs.size() << endl;

    for(auto t = 0; t <= EventIDs.size(); t++){
      cout << "Tracker EventID: " << TrackerEventIDs.at(t) << endl;
      cout << "LAGD EventID: " << EventIDs.at(t) << endl;
      if(TrackerEventIDs.at(t) != EventIDs.at(t)){break;}
    }
    */
    //return;

    auto BadEvents = ResCut(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords);
    //vector<float>  BadEvents;
    //return;
    vector<float> TrackerOptimizedAnglesAndXY;
    
    //auto TrackerOptimizedAnglesAndXY = Optimized_Rotation(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords, TrackerOptimizedOffsets, 1);

    //cout << "Next" << endl;
    //return;
    auto TrackerOptimizedOffsets = Optimized_XY_shift(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords, TrackerOptimizedAnglesAndXY, BadEvents, 0); //returns vector of XY Offsets for trackers 1-4 that minimize the residual)

    //return;
    

    TrackerOptimizedAnglesAndXY = Optimized_Rotation(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords, TrackerOptimizedOffsets, BadEvents, 0);

    //cout << "Optimized XY Offsets for Trackers after alignment 1:" << endl;
    
    
    //cout << "Optimized XY and Angular Offsets for Trackers after aligment 2:" << endl;
    //for(auto i = TrackerOptimizedAnglesAndXY.begin(); i != TrackerOptimizedAnglesAndXY.end(); i++)
    //  cout << *i << endl;	      

    //return;
    vector<float> FinalOptimizedOffsets;
    FinalOptimizedOffsets = Optimized_XY_Rotation(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords, TrackerOptimizedAnglesAndXY, BadEvents, 0);

    //return;
    
    ///cout << "Optimized XY Offsets for Trackers after alignment 1:" << endl;
    // for(auto i = TrackerOptimizedOffsets.begin(); i != TrackerOptimizedOffsets.end(); i++)
    //  cout << *i << endl;	      

    /*
    cout << "Optimized XY and Angular Offsets for Trackers after aligment 2:" << endl;
    for(auto i = TrackerOptimizedAnglesAndXY.begin(); i != TrackerOptimizedAnglesAndXY.end(); i++)
      cout << *i << endl;	      
    */
    
    // cout << "Optimized XY and Angular Offsets for Trackers after aligment 3:" << endl;
    // for(auto i = FinalOptimizedOffsets.begin(); i != FinalOptimizedOffsets.end(); i++)
    //  cout << *i << endl;	      
    
    //return;
    Plot_Aligned_residuals(TotalTracker1hitcoords, TotalTracker2hitcoords, TotalTracker3hitcoords, TotalTracker4hitcoords, FinalOptimizedOffsets, BadEvents, LAGDHitsVec, SMofTheseevents, Clusterchargeoftheseevnts, EventIDs, 1);//comehere
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


#include <vector>
#include <TVector.h>
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
//#include "FNAL-Beam-Test-Scripts/doubleGausFit_withHistParameter.C"
#include "doubleGausFit_withHistParameter.C"
#include <TLatex.h>
#include <unistd.h>
//#include <gSystem.h>
//#include <gPad.h>
//#include "Optimized_XY_shift.C"
#include "TGaxis.h"
#include <numeric>
#include <TLine.h>
//#include "ResidualCorrection.C"
#include <TProfile.h>



void LAGD_Rotation_Alignment(){

  string Eventhits;
  ifstream TrackingFile("Aligned_Data/Trackers_Hits_Merged89_All.txt");
  ifstream LAGDFile("Aligned_Data/LAGD_Hits_Merged89_All.txt");

  map<float, float> HVXoffsets;
  map<float, float> HVYoffsets;
  map<float, float> HVres;
  string LAGDHits;

  vector<float> Tracker1x;
  vector<float> Tracker1y;
  
  vector<float> Tracker2x;
  vector<float> Tracker2y;
  vector<float> Tracker3x;
  vector<float> Tracker3y;
  vector<float> Tracker4x;
  vector<float> Tracker4y;
  vector<float> APV3E1;

  vector<float> Tracker1Phi;
  vector<float> Tracker2Phi;
  vector<float> Tracker3Phi;
  vector<float> Tracker4Phi;
  

  //these variables are used for initial offsets for trackers
  
  float shiftx = -29.7665+80+120+120+220+155+10+20;
  float shifty = -15.0655+100-7;
  
  
  while(getline(TrackingFile, Eventhits)){
    
    if(Eventhits.at(0) == '#')
      continue;
    
    stringstream Eventstringstream(Eventhits);
    //cout << Eventhits << endl;
    vector<float> Eventvector(istream_iterator<float>(Eventstringstream),{});

    Tracker1x.push_back(Eventvector.at(0)+shiftx);
  
    Tracker1y.push_back(Eventvector.at(1)+shifty);
    
    Tracker2x.push_back(Eventvector.at(2)+shiftx);
    Tracker2y.push_back(Eventvector.at(3)+shifty);
    Tracker3x.push_back(Eventvector.at(4)+shiftx);
    Tracker3y.push_back(Eventvector.at(5)+shifty);
    Tracker4x.push_back(Eventvector.at(6)+shiftx);
    Tracker4y.push_back(Eventvector.at(7)+shifty);

  }

  for(auto i = 0; i < Tracker1x.size(); i++){

    auto angle1 = atan(Tracker1y.at(i)/Tracker1x.at(i));
    Tracker1Phi.push_back(angle1);

    auto angle2 = atan(Tracker2y.at(i)/Tracker2x.at(i));
    Tracker2Phi.push_back(angle2);

    auto angle3 = atan(Tracker3y.at(i)/Tracker3x.at(i));
    Tracker3Phi.push_back(angle3);

    auto angle4 = atan(Tracker4y.at(i)/Tracker4x.at(i));
    Tracker4Phi.push_back(angle4);
    
  }
  

  auto LAGDstripDist = new TH1F("", "LAGD Cluster Distribution; Strip [Strip]; Counts/mRad",128, 0, 128);
  string LAGDEvent;
  while(getline(LAGDFile, LAGDEvent)){
    //cout << LAGDEvent << endl;
    
    if(LAGDEvent.at(0) == '#')
      continue;
    
    else{
      stringstream Eventstringstream(LAGDEvent);
      //cout << Eventhits << endl;
      vector<float> Eventvector(istream_iterator<float>(Eventstringstream),{});

      APV3E1.push_back(Eventvector.at(0)+64);
      
      LAGDstripDist->Fill(Eventvector.at(0)+64);//-64 for sZZ sectors due to the sector location on the readoutboard
     
      //cout << Eventvector.at(0) << endl;
    }
  }
  // cout << "size of LAGD hitspot: " << LAGDstripDist->GetStdDev() << endl; 
  // LAGDstripDist->Draw();
  // return;

  
  double mean_LAGDRes=0.0;

  double width_LAGDRes=0.0;

  double LAGD_Angle=0.0;
  
  int Iterator=0;
  
  
  
  TGraph* ResidualLAGDPerIteration = new TGraph();ResidualLAGDPerIteration->GetXaxis()->SetTitle("Iterations");ResidualLAGDPerIteration->GetYaxis()->SetTitle("Residual [uRad]");ResidualLAGDPerIteration->SetTitle("Residual Per Iteration in LAGD Angle");
 
 
  TGraph* LAGDChi2PerIteration = new TGraph();LAGDChi2PerIteration->GetXaxis()->SetTitle("Iteration");LAGDChi2PerIteration->GetYaxis()->SetTitle("#chi^2");LAGDChi2PerIteration->SetTitle("#chi^2 of Events");//xChi2PerIteration
  

  char nameRes2X[30];sprintf(nameRes2X,"residual_Tracker2x_%i",Iterator);char nameRes2Y[30];sprintf(nameRes2Y,"residual_Tracker2y_%i",Iterator);
 	
  TH1F* h_residual_LAGD = new TH1F(nameRes2X,"",100,-20,20); h_residual_LAGD->SetXTitle("Residual [mm]"); h_residual_LAGD->SetYTitle("Frequency");h_residual_LAGD->SetLabelSize(0.045,"XY");h_residual_LAGD->SetTitleSize(0.045,"XY");
 
  TH1F* angleREF2 = new TH1F("angleREF2","Rotation angle distribution of LAGD",100,-3,3); angleREF2->SetXTitle("Angle [radian]"); angleREF2->SetYTitle("Counts/1uRad");
 

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

  
  //Now rotate each angle (3) around a small range to tune the angles
  int bounds = 5;
  float anglescale_Factor = 0.0005;//1 mrad
  float StripPitch = 0.00137;
  auto Chiperangle = new TGraph();
  int chiit = 0;
  for(int AngleIt=-bounds;AngleIt <= bounds; AngleIt++){
      
    float angleT2temp = anglescale_Factor*AngleIt;
      
    char nameRot1[30];sprintf(nameRot1,"Rotation_Tracker1_%i",AngleIt);
    char nameRotCanvas[30];sprintf(nameRotCanvas,"Rotation_canvas_%f",angleT2temp);
    char titleRot1[30];sprintf(titleRot1,"Rotation_Tracker1_%i",AngleIt);
    char titleRotCanvas[30];sprintf(titleRotCanvas,"Rotation_canvas_%f",angleT2temp);

    vector<float> ShiftedLAGDHits;
    
    auto xTrackChi2 = new TH1D("", "", 100, 0, 2); 
    cout << "Angle rotated to: " << angleT2temp << " at iterator: " << AngleIt << endl;
     
    for(unsigned int i=0; i< APV3E1.size(); i++){

      auto newangle = APV3E1.at(i)*StripPitch + angleT2temp;
      //cout << "New LAGD Angle : "<< newangle << endl;
      ShiftedLAGDHits.push_back(newangle);
      //cout << "here1" << endl;
    

      //plot new shifted events and store chi^2 per event
      //return average chi^2 squared

      TGraph* gx = new TGraph();
      //cout << "here2" << endl;
      TF1* fx = new TF1("line1","[0]+[1]*x",0,1600);
      //cout << "here3" << endl;
      gx->SetPoint(0, 1596.4, Tracker1Phi.at(i));
      //cout << "Tracker1 angle: " << Tracker1Phi.at(i) << endl;
      gx->SetPoint(1, 1541.6, Tracker2Phi.at(i));
      gx->SetPoint(2, 828.8, newangle);
      gx->SetPoint(3, 54.8, Tracker3Phi.at(i));
      gx->SetPoint(4, 0, Tracker4Phi.at(i));
    
      gx->Fit("line1","Q");
   
      double interceptx = fx->GetParameter(0);
      double slopex     = fx->GetParameter(1);
      cout << "here" << endl;
    
      xTrackChi2->Fill(fx->GetChisquare());
      
      delete gx;
      delete fx;
      //delete xTrackChi2;
    }

    //xTrackChi2->Draw();
    Chiperangle->SetPoint(chiit, angleT2temp, xTrackChi2->GetMean());
    chiit++;    
  }
  TF1* fq = new TF1("linef","pol2");
  Chiperangle->Fit("linef");
  Chiperangle->Draw("A*");

  float GEMrotation = -fq->GetParameter(1)/(2*fq->GetParameter(2));

  cout << "Vertex?: " << GEMrotation << endl;
 
  ofstream OutputFile("Aligned_Data/LAGD_Hits_Merged89_All_rotated.txt");
  
  for(unsigned int i=0; i< APV3E1.size(); i++){
    OutputFile << ((APV3E1.at(i)*StripPitch + GEMrotation)/StripPitch) << endl;
  }
  OutputFile.close();
}

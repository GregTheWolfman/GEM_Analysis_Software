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


void LAGD_Tracker_Alignment(){

  //files for Tracker hits and LAGD hits
  //also opens the output file for residual means and standard deviations
  string Eventhits;
  ifstream TrackingFile("Aligned_Data/Trackers_Hits_Merged_all.txt");
  ifstream LAGDFile("Aligned_Data/LAGD_Hits_Merged_all.txt");
  ifstream LAGDrescorrFile("Aligned_Data/LAGD_Residual_Corrections_Merged_all.txt");
  int HVsetting = 3729;
  map<float, float> HVXoffsets;
  map<float, float> HVYoffsets;
  map<float, float> HVres;
  string LAGDHits;

  vector<float> etavalues;
  vector<int> SMperEvent;
  int linenum=0;
  vector<float> LAGDStrips;
  vector<float> LAGDCharges;
    
  while(getline(LAGDrescorrFile, LAGDHits)){
    stringstream LAGDstringstream(LAGDHits);
    //cout << LAGDstringstream.str() << endl;
    vector<float> LAGDvector(istream_iterator<float>(LAGDstringstream),{});
    if(linenum%2 == 0){      
      for(auto u = LAGDvector.begin(); u < LAGDvector.end(); u++){
	//cout << *u << endl;
	LAGDStrips.push_back(*u);
      }
    }
    if(linenum%2 == 1){
      for(auto u = LAGDvector.begin(); u < LAGDvector.end(); u++){
	//cout << *u << endl;
	LAGDCharges.push_back(*u);      	
      }

      //cout << *max_element(LAGDCharges.begin(),LAGDCharges.end()) << endl;
      //cout << distance(LAGDCharges.begin(), max_element(LAGDCharges.begin(),LAGDCharges.end())) << endl;
      auto sm = LAGDStrips.at(distance(LAGDCharges.begin(), max_element(LAGDCharges.begin(),LAGDCharges.end())));
      //cout << sm << endl;
      SMperEvent.push_back(LAGDStrips.size());
      //return;

      float sCOM=0;
      float snum=0;
      float sden=0;
      
      for(uint y = 0; y < LAGDStrips.size(); y++){
	snum += LAGDStrips.at(y)*LAGDCharges.at(y);
	sden += LAGDCharges.at(y);
      }
      sCOM = snum/sden;
      //cout << sCOM << endl;

      //cout << sm - sCOM << endl;

      etavalues.push_back(sm - sCOM);
      //return;
      LAGDStrips.clear();
      LAGDCharges.clear();      
    }
    linenum++;
    
  }
  //cout << etavalues.size() << endl;
  //cout << SMperEvent.size() << endl;
  // for(uint y = 0; y < SMperEvent.size(); y++){
  //   cout << SMperEvent.at(y) << endl;
  // }
  //return;
  
  cout << "yes" << endl;
  vector<float> Tracker1x;
  vector<float> Tracker1y;
  
  vector<float> Tracker2x;
  vector<float> Tracker2y;
  vector<float> Tracker3x;
  vector<float> Tracker3y;
  vector<float> Tracker4x;
  vector<float> Tracker4y;
  vector<float> APV3E1;

  //these variables are used for initial offsets for trackers
  float sumy=0.0;
  float sumx=0.0;
  auto Tracker1Hitmap = new TH2F("PHT1", "Tracker 1 Beam Spot; X position [mm]; Y position [mm]", 100, 0, 50, 100, 0, 50);
  auto Tracker2Hitmap = new TH2F("PHT2", "Tracker 2 Beam Spot; X position [mm]; Y position [mm]", 50, 0, 50, 50, 0, 50);
  auto Tracker3Hitmap = new TH2F("PHT3", "Tracker 3 Beam Spot; X position [mm]; Y position [mm]", 50, 0, 50, 50, 0, 50);
  auto Tracker4Hitmap = new TH2F("PHT4", "Tracker 4 Beam Spot; X position [mm]; Y position [mm]", 50, 0, 50, 50, 0, 50);
  auto HitspotprojX = new TH1F("", "", 100, 0, 50);
  auto HitspotprojY = new TH1F("", "", 100, 0, 50);
  TGraph* LAGDandTrackerStripCheck = new TGraph();
  LAGDandTrackerStripCheck->SetTitle("Correlation between LAGD and Tracker hits;LAGD Hits [Strip]; Tracker Hits [mm]");
  TGraph* TrackerStripCheck = new TGraph();
  TrackerStripCheck->SetTitle("Correlation between Different Tracker hits;Tracker Hits [mm]; Tracker Hits [mm]");
  
  //int hitit=0;
  //separate the tracker file of coordinates into their own vectors
  //ofstream OtherBeamFile("Aligned_Data/TrackerIntLAGDBeamspots.txt");
  
  while(getline(TrackingFile, Eventhits)){
    //cout << Eventhits.at(0) << endl;
    
    if(Eventhits.at(0) == '#')
      continue;
    
    stringstream Eventstringstream(Eventhits);
    cout << Eventhits << endl;
    vector<float> Eventvector(istream_iterator<float>(Eventstringstream),{});

    Tracker1x.push_back(Eventvector.at(0));
    //cout << "Tracker1x coordinate?: " << Eventvector.at(0) << endl;
    sumx += Eventvector.at(0);    

    Tracker1y.push_back(Eventvector.at(1));
    sumy += Eventvector.at(1);

    Tracker2x.push_back(Eventvector.at(2));
    Tracker2y.push_back(Eventvector.at(3));
    Tracker3x.push_back(Eventvector.at(4));
    Tracker3y.push_back(Eventvector.at(5));
    Tracker4x.push_back(Eventvector.at(6));
    Tracker4y.push_back(Eventvector.at(7));

    Tracker1Hitmap->Fill(Eventvector.at(0), Eventvector.at(1));
    Tracker2Hitmap->Fill(Eventvector.at(2), Eventvector.at(3));
    Tracker3Hitmap->Fill(Eventvector.at(4), Eventvector.at(5));
    Tracker4Hitmap->Fill(Eventvector.at(6), Eventvector.at(7));
    
    HitspotprojX->Fill(Eventvector.at(0));
    HitspotprojX->Fill(Eventvector.at(2));
    HitspotprojY->Fill(Eventvector.at(1));
    HitspotprojY->Fill(Eventvector.at(3));

    // //beamspots at LAGD
    // TGraph* gx = new TGraph();
    // gx->SetPoint(0, 1596.4, Eventvector.at(0));
    // gx->SetPoint(1, 1541.6, Eventvector.at(2));
    // gx->SetPoint(2, 54.8, Eventvector.at(4));
    // gx->SetPoint(3, 0, Eventvector.at(6));

    // TF1* fx = new TF1("linex","pol1");
    // gx->Fit("linex", "Q");

    // float LAGDxcoord = fx->GetParameter(0) + fx->GetParameter(1)*828.2;

    // delete gx; delete fx;

    // TGraph* gy = new TGraph();
    // gy->SetPoint(0, 1596.4, Eventvector.at(1));
    // gy->SetPoint(1, 1541.6, Eventvector.at(3));
    // gy->SetPoint(2, 54.8, Eventvector.at(5));
    // gy->SetPoint(3, 0, Eventvector.at(7));

    // TF1* fy = new TF1("liney","pol1");
    // gy->Fit("liney", "Q");

    // float LAGDycoord = fy->GetParameter(0) + fy->GetParameter(1)*828.2;

    
    // delete gy; delete fy;

    // OtherBeamFile << LAGDxcoord << " " << LAGDycoord << endl;
    
    
    
    /*
    TGraph* g2 = new TGraph();

    g2->SetPoint(0,1596.4, sqrt(Eventvector.at(0)*Eventvector.at(0) + Eventvector.at(1)*Eventvector.at(1)));
    g2->SetPoint(1,54.8, sqrt(Eventvector.at(2)*Eventvector.at(2) + Eventvector.at(3)*Eventvector.at(3)));	//al;so swapped 2 and 3
    g2->SetPoint(2,1541.6, sqrt(Eventvector.at(4)*Eventvector.at(4) + Eventvector.at(5)*Eventvector.at(5)));	
    g2->SetPoint(3,0, sqrt(Eventvector.at(6)*Eventvector.at(6) + Eventvector.at(7)*Eventvector.at(7)));		
    
    // inclusive
    TF1* f2 = new TF1("line2","pol1");
    g2->Fit("line2", "Q");
    //  return;
    //if(Yoffset == 8 && Xoffset == 0){
    g2->Draw("A*");
    return;
    */
    
  }
  //return;
  
  // auto CoordinateTest = new TCanvas("", "Coordinate Translation to LAGD Sector", 500, 500);
  
  
  // Tracker1Hitmap->Draw("colz");
  // TLine *line1 = new TLine(80,0,200,0);
  // line1->SetLineWidth(8);
  // //line1->SetLineColor(2);
  // line1->Draw();

  // TLine *line2 = new TLine(80,0,80,30);
  // line2->SetLineWidth(8);
  // //line2->SetLineColor(2);
  // line2->Draw();

  // TLine *line3 = new TLine(80,30,200,60);
  // line3->SetLineWidth(8);
  // //line3->SetLineColor(2);
  // line3->Draw();

  // TLine *line4 = new TLine(200,60,200,0);
  // line4->SetLineWidth(8);
  // //line4->SetLineColor(2);
  // line4->Draw();
  
  // CoordinateTest->Draw();
  // return;
  
  
  // auto Hitspots = new TCanvas("", "Post Tracker Alignment BeamSpots", 1000, 1000);
  // Hitspots->Divide(2,2);
  // Hitspots->cd(1);
  // Tracker1Hitmap->Draw("colz");
  // Hitspots->cd(2);
  // Tracker2Hitmap->Draw("colz");
  // Hitspots->cd(3);
  // Tracker3Hitmap->Draw("colz");
  // Hitspots->cd(4);
  // Tracker4Hitmap->Draw("colz");

  // Hitspots->Draw();
  
  // return;

  float sizeX = HitspotprojX->GetStdDev();
  float sizeY = HitspotprojY->GetStdDev();
  float avesize = (sizeX + sizeY)/2.0;
  cout << "SizeX: " << sizeX << ", " << "SizeY: " << sizeY << ", average size: " << (sizeX + sizeY)/2.0 << endl; 

  cout << -sumx/Tracker1x.size() << endl;
  cout << -sumy/Tracker1y.size() << endl;
  
  //return;
  //now do the LAGD hits

  auto LAGDstripDist = new TH1F("", "LAGD Cluster Distribution; Angle [mrad]; Counts/4 mrad",65, -0.5*4.1, 63.5*4.1);
  string LAGDEvent;
  while(getline(LAGDFile, LAGDEvent)){
    //cout << LAGDEvent << endl;
    
    if(LAGDEvent.at(0) == '#')
      continue;
    
    else{
      stringstream Eventstringstream(LAGDEvent);
      //cout << Eventhits << endl;
      vector<float> Eventvector(istream_iterator<float>(Eventstringstream),{});
      
      APV3E1.push_back(Eventvector.at(0));
      
      LAGDstripDist->Fill(Eventvector.at(0) + 64);//-64 for sZZ sectors due to the sector location on the readoutboard, +64 for interweaved bois
     
      cout << Eventvector.at(0) << endl;
    }
  }
  // cout << "size of LAGD hitspot: " << LAGDstripDist->GetStdDev() << endl; 
  // LAGDstripDist->Draw();
  // return;
  //vectors have been created
  //Step one: find tracker offset (unique to each sector)
  //-- double iteration over Y and X offset
  //-- increment in Y, iterate through all x offsets, convert to r,phi for residual calculations!
  //-- plot res mean vs Yoffset and res sigma vs Xoffset 
  //strip correction checking plots
  /*
  for(auto i = 0; i < APV3E1.size();i++){
    LAGDandTrackerStripCheck->SetPoint(i, APV3E1.at(i), Tracker1y.at(i));
    TrackerStripCheck->SetPoint(i, Tracker2y.at(i), Tracker1y.at(i));
  }
  
  auto w = new TCanvas("", "Strip Correlations after Tracker Alignment", 1000, 500);
  w->Divide(2,1);
  w->cd(1);
  LAGDandTrackerStripCheck->Draw("A*");
  w->cd(2);
  TrackerStripCheck->Draw("A*");

  w->Draw();
  
  return;
  */
  
  vector<float> T1r;
  vector<float> T1phi;
  vector<float> T2r;
  vector<float> T2phi;
  vector<float> T3r;
  vector<float> T3phi;
  vector<float> T4r;
  vector<float> T4phi;
  vector<float> LAGDphi;

  TH1F* residualREF1X = 0;
  TH1F* residualREF1Y = 0;
  TH1F* residualREF2X = 0;
  TH1F* residualREF2Y = 0;
  TH1F* residualREF3X = 0;
  TH1F* residualREF3Y = 0;
  TH1F* residualREF4X = 0;
  TH1F* residualREF4Y = 0;
  TH1F* residualLAGD = 0;
  TH1F* residualLAGDr = 0;

  //TH1F* incresidualREF1Y = 0;
  //TH1F* incresidualREF2Y = 0;
  //TH1F* incresidualREF3Y = 0;
  //TH1F* incresidualREF4Y = 0;
  //TH1F* incresidualLAGD = 0;
  TH1F* TrackErrorsAtLAGD = 0;
  TH1F* TrackSlopeErrorsAtLAGD = 0;
  TH1F* TrackYintErrorsAtLAGD = 0;
  TH1F* LAGDResM2=0;
  //TH1F* LAGDResM3=0;
  TGraph* LAGDResPerX =0;
  TGraph* LAGDResPerY =0;
  TProfile* LAGDResPerXProf =0;
  TProfile* LAGDResPerYProf =0;
  TGraph* LAGDResPerEta=0;
  TH2D* LAGDResPerEtaHist=0;
  TH2D* LAGDResPerEtaEven=0;
  TH2D* LAGDResPerEtaOdd=0;
  TGraph* LAGDResPerEtaEvenG=0;
  TGraph* LAGDResPerEtaOddG=0;  
  TH2D* LAGDResPerX2D=0;
  TH2D* LAGDResPerY2D=0;
  TProfile* LAGDResPerEtaProf=0;
  TProfile* LAGDResPerEtaProfEven=0;
  TProfile* LAGDResPerEtaProfOdd=0;

  
  //TH1F* TAngDist = 0;
  //TH1F* T2AngDist = 0;
  //TH1F* T3AngDist = 0;
  
  //TH1F* T4AngDist = 0;
  TH1F* AvgAngDist = 0;  
  TH1F* LAngDist = 0;
  //TH1F* rphiDist = 0;
  //TH1F* yDist = 0;
  
  
  
  double meanREF2X=0.0, meanREF2Y=0.0, meanREF3X=0.0, meanREF3Y=0.0, meanREF1X=0.0, meanREF1Y=0.0, meanREF4X=0.0, meanREF4Y=0.0, meanLAGD=0.0, meanLAGDr=0.0, LAGDResM2mean=0.0, LAGDResM3mean=0.0;
  double sigmaREF2X=0.0,sigmaREF2Y=0.0,sigmaREF3X=0.0,sigmaREF3Y=0.0, sigmaREF1X=0.0,sigmaREF1Y=0.0,sigmaREF4X=0.0,sigmaREF4Y=0.0,sigmaLAGD=0.0,sigmaLAGDr=0.0, LAGDResM2sigma=0.0, LAGDResM3sigma=0.0;
  double meanXChi2=0.0,meanYChi2=0.0, totalChi2=0.0;

  double incmeanREF2Y=0.0, incmeanREF3Y=0.0, incmeanREF1Y=0.0, incmeanREF4Y=0.0, incmeanLAGD=0.0, Trackerrormean=0.0;
  double incsigmaREF2Y=0.0,incsigmaREF3Y=0.0,incsigmaREF1Y=0.0,incsigmaREF4Y=0.0,incsigmaLAGD=0.0, Trackerrorsigma=0.0;

  float EventsthatPass = 0.0;
  float totalEvents = 0.0;
 
  //TH1F* xTrackChi2=0;
  //TH1F* yTrackChi2=0;
  

  auto Stripcorr = new TH2D("", "", 120, 0, 120, 120, 0, 120);
  auto TrackerStripcorr = new TH2D("", "", 120, 0, 120, 120, 0, 120);
  auto StripcorrProf = new TProfile("Stripcorr", "Profile plot of Strip correlation pre alignment;Tracker 1y Angle [mRad]; LAGD Angle [mRad]", 10, 0, 100, "s");
  auto TrackerStripcorrProf = new TProfile("TrackerStripcorr", "Profile plot of Strip correlation pre alignment;Tracker 1y Angle [mRad]; Tracker 3y Angle [mRad]", 10, 0, 100,"s");
  
  TGraph* ResSigmaVsXoffset = new TGraph();
  TGraph* IdealXYoffsets = new TGraph();
  TGraph* RealIdealYoffsetVsXoffset = new TGraph();
  TGraph* ReswidthperYoffset = new TGraph();

  float Yoffset=0.0;
  float Xoffset=0.0;
  float LAGDAngle = 0.525;//30.1 degrees;
  float Strippitch = 1.37;// 4.1 mRad/strip for SS, LZZ, 1.37 for sZZ
    
  float sectorXwidth = 5.0;//120;//look at Amore for these values in mm
  float sectorYwidth = 5.0;//60;

  float LAGDanglehere = LAGDstripDist->GetMean()*Strippitch;

  HVXoffsets[3729] = 80 + 28 + 32.05;
  HVYoffsets[3729] = 9.05;
  HVres[3729] = 0.74715;

  //80 + 28 + 26 + 3.67933
  //+7+1.53323 for 1 strips hits too
  
  HVXoffsets[3819] = 80 + 28 + 20 + 11.65;
  HVYoffsets[3819] = 5 + 3.93;
  HVres[3819] = 0.62367;
  
  HVXoffsets[3981] = 80 + 28 + 20 + 9.01025;
  HVYoffsets[3981] = 5 + 3.80914;
  HVres[3981] = 0.56946;
  
  HVXoffsets[4310] = 80 + 28 + 20+10.7101;
  HVYoffsets[4310] = 8.97935;
  HVres[4310] = 0.53823;
  
  HVXoffsets[4340] = 80+28 + 20+10.2803;
  HVYoffsets[4340] = 5  + 3.89424;
  HVres[4340] = 0.52005;
  
  HVXoffsets[4370] = 80+28 + 20  +9.34121;
  HVYoffsets[4370] =  5+  3.88302;
  HVres[4370] = 0.50436;
  
  HVXoffsets[4400] = 80+28 + 20  +10.0003;
  HVYoffsets[4400] =  5+  4.037;
  HVres[4400] = 0.50743;
  
  HVXoffsets[4430] = 80+28 + 20+9.05033;
  HVYoffsets[4430] = 5 + 3.76143;
  HVres[4430] = 0.48578;

  HVXoffsets[3998] = + 80+28 + 20 + 11.0005;
  HVYoffsets[3998] =  5+3.74;
  HVres[3998] = 0.48723;


  float LAGDREs = HVres[HVsetting];//in mRad!
  
  
  /*
    First ZigZag (SM2345) offets: (80 + 120 + 58.8799, 7.93058) 
    First ZZ Larger data set:   (80 + 120 + 58.68747, 8.31925) shift = 0.06
    SS offsets: (80 + 25 + 7.2913, 8.30171)
    APV5E4 offsets: 80+120+120+220+160 +2 +1.75015, 10.27526
    APV4E4 offsets: 80+120+120+220+220+160+8.87121, 10.5006 shift == -0.025
    3729 offsets: 80 + 28 + 32.05, 9.05
    4250 offsets: 80 + +28 + 20 + 11.65, 5 + 3.93
    4280 offsets: 80+28 + 20 + 9.01025, + 5+ 3.80914
    4310 offsets: 80+28 + 20+10.7101, 8.97935
    4340 offsets: 80+28 + 20+10.2803    ,5  + 3.89424
    4370 offsets: 80+28 + 20  +9.34121  , 5+  3.88302
    4400 offsets: 80+28 + 20  +10.0003  , 5+  4.037
    4430 offsets: + 80+28 + 20+9.05033, 5+ 3.76143
    4450 offsets: + 80+28 + 20 + 11.0005, 5+3.74
    
    Second ZZ:   80+120+120+80 +15+2+0.820191, +9+1.24299
    FZZ 1100, 217:  + 80+120+100+5+4.3, 6 + 5.3 shift == -0.045
   */

  bool HVscantime = false;
  float InitialXoffset=0;
  float InitialYoffset=0;
  float shift = 0.0;
  
  double offests[2] = {25, -83};
  if(HVscantime){
    
    InitialXoffset = -sumx/Tracker1x.size() + HVXoffsets[HVsetting]; // add other initial offsets here, including optimized offsets at the end for residual plots
    InitialYoffset = -sumy/Tracker1y.size() + HVYoffsets[HVsetting];
  }
  else{
    InitialXoffset = -sumx/Tracker1x.size() + 80+120+120+220+155 +  offests[0] + shift; // add other initial offsets here, including optimized offsets at the end for residual plots
    InitialYoffset = -sumy/Tracker1y.size()  +  6 +100  + offests[1] + shift;
  }

  cout << InitialXoffset << endl;
  cout << InitialYoffset << endl;
  //return;
  int totalits = 0;
  float Xmulti = 0.5;
  float Ymulti = 1;
  int ressigmait=0;

  float Tracker1xtemp;
  float Tracker1ytemp;
  float Tracker2xtemp;
  float Tracker2ytemp;
  float Tracker3xtemp;
  float Tracker3ytemp;
  float Tracker4xtemp;
  float Tracker4ytemp;
  float APV3E1temp;

  float T1rval;
  float T2rval;
  float T3rval;
  float T4rval;
  float LAGDrval;

  float minx = 715;//FZZ: 235, SS: 105, APV5E4 690, APV4E4 902, APV3E2 290
  float maxx = 722;//FZZ: 285, SS: 110, APV5E4 710. APV4E4 922  APV3E2 310
  float miny = 23;
  float maxy = 30;
  int binnum = 6;
  float binwidthx = (maxx-minx)/binnum;
  float binwidthy = (maxy-miny)/binnum;
  
  map <int, TH1F*> LAGDResXHist;
  map <int, TH1F*> LAGDResYHist;
  
  vector<float> Modulusvals;
  bool finalRes = false;//True means you want resolution calc with offsets, choose false if youre generating data file for determining offsets
  auto rescor = false;//do you want to apply residual corrections for ZZ strips?
  
  float TrackerError = 0;
  
  string filename = "Aligned_Data/DataPerXandY_Merged_fixed_aligned.txt";
  bool write = !finalRes;
  
  fstream OutputFile(filename, std::ios::in | std::ios::out);

  if (!OutputFile.is_open()) {
    // If file does not exist, open it in write mode
    OutputFile.clear();  // Clear any error flags
    OutputFile.open(filename, std::ios::out);  // Create the file if it doesn't exist
  }

  if (write){
    // Now reopen the OutputFile in truncating mode to overwrite content
    OutputFile.close();  // Close the existing stream
    OutputFile.open(filename, std::ios::out | std::ios::trunc);  // Reopen for overwriting

    if (OutputFile.is_open()) {
      std::cout << "File was written!" << std::endl;
    } else {
      std::cerr << "Error: Could not open file for writing!" << std::endl;
    }
  } else {
    std::cout << "File was not written." << std::endl;
  }

  
  
  bool Plottingfile = false;
  //ofstream BeamsFile("Aligned_Data/LAGD_Beamspots_LZZ2.txt");
  //ofstream BeamsCartFile("Aligned_Data/LAGD_Beamspots_LZZ2_Trackers.txt");

  //float efficiencynumerator=0.0;
  //float tracking_error=0.0;
  //auto Avgtrackerror = new TH1F("", "", 100, -0.1, 0.1);
  auto LAGDRfromTrackers = new TH1F("", "", 4000, 0, 2000);
  //auto Hitspot = new TH2F("", "", 100, 0, 50, 100, 0, 50);
  auto Hitspotproj = new TH1F("", "", 100, 0, 50);
  auto ReswidthdiffperXoffset = new TGraph();
  auto AllHitsFitplot = new TGraph();
  int allhitsit=0;
  auto ResPerAngle = new TGraph();
  //auto CartesianSizeDiffperRadius = new TGraph();
  int cartit=0;
  float LAGDResolution=0;
  bool ft = true;
  auto resplotcheck = new TH1F("", "", 100, -10, 10);
  for(uint Xiter=0; Xiter <= sectorXwidth/Xmulti; Xiter++){//iterate over Xoffsets in increments of 1mm
    Xoffset = Xmulti*Xiter;
    
    TGraph* ResMeanVsYoffset = new TGraph();
    vector<float> ResidualMeanPerYoffset;
    vector<float> ResidualRMSPerYoffset;
    
    for(uint Yiter=0; Yiter <= sectorYwidth/Ymulti; Yiter++){//iterate over Yoffsets in increments of 0.2mm
      Yoffset = Ymulti*Yiter;
      cout << Xoffset << ", " << Yoffset << endl;
      //string Yoffset = Yoffset;
      
      //rename all of this to the vectors in this code!
      char nameRes1X[50];sprintf(nameRes1X,"residualREF1_r_X_%.1f_Y_%.1f",Xoffset, Yoffset);char nameRes1Y[50];sprintf(nameRes1Y,"residualREF1_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameRes2X[50];sprintf(nameRes2X,"residualREF2_r_X_%.1f_Y_%.1f",Xoffset, Yoffset);char nameRes2Y[50];sprintf(nameRes2Y,"residualREF2_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameRes3X[50];sprintf(nameRes3X,"residualREF3_r_X_%.1f_Y_%.1f",Xoffset, Yoffset);char nameRes3Y[50];sprintf(nameRes3Y,"residualREF3_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameRes4X[50];sprintf(nameRes4X,"residualREF4_r_X_%.1f_Y_%.1f",Xoffset, Yoffset);char nameRes4Y[50];sprintf(nameRes4Y,"residualREF4_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);

      char nameResLAGD[50];sprintf(nameResLAGD,"residualLAGD_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameTDist[50];sprintf(nameTDist,"Tracker1 Dist_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameT2Dist[50];sprintf(nameT2Dist,"Tracker2 Dist_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameT3Dist[50];sprintf(nameT3Dist,"Tracker3 Dist_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameT4Dist[50];sprintf(nameT4Dist,"Tracker4 Dist_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameAvgDist[50];sprintf(nameAvgDist,"Average Tracker Dist_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameLDist[50];sprintf(nameLDist,"LAGD Dist_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameCanv[50];sprintf(nameCanv,"canvas_phi_X_%.1f_Y_%.1f",Xoffset, Yoffset);

      float PlotAxisRange = 20000;
      float PlotBins = 100;
      residualREF1X = new TH1F(nameRes1X,"",PlotBins,-5,5); residualREF1X->SetXTitle("Residual in r [mm]"); residualREF1X->SetYTitle("Frequency");residualREF1X->SetLabelSize(0.05,"XY");residualREF1X->SetTitleSize(0.05,"XY");
      residualREF1Y = new TH1F(nameRes1Y,"",PlotBins,-PlotAxisRange,PlotAxisRange); residualREF1Y->SetXTitle("Residual in Angle [urad]"); residualREF1Y->SetYTitle("Frequency");residualREF1Y->SetLabelSize(0.05,"XY");residualREF1Y->SetTitleSize(0.05,"XY");
      //residualREF1y0 = new TH1F(nameRes1Y,"",400,-0.05,0.05); residualREFy0->SetXTitle("Residual in Y position [mm]"); residualREFy0->SetYTitle("Frequency");residualREFy0->SetLabelSize(0.05,"XY");residualREFy0->SetTitleSize(0.05,"XY");
      residualREF2X = new TH1F(nameRes2X,"",PlotBins,-5,5); residualREF2X->SetXTitle("Residual in r [mm]"); residualREF2X->SetYTitle("Frequency");residualREF2X->SetLabelSize(0.05,"XY");residualREF2X->SetTitleSize(0.05,"XY");
      residualREF2Y = new TH1F(nameRes2Y,"",PlotBins,-PlotAxisRange,PlotAxisRange); residualREF2Y->SetXTitle("Residual in Angle [urad]"); residualREF2Y->SetYTitle("Frequency");residualREF2Y->SetLabelSize(0.05,"XY");residualREF2Y->SetTitleSize(0.05,"XY");
      //residualREF2y0 = new TH1F(nameRes2Y,"",400,-0.05,0.05); residualREF2y0->SetXTitle("Residual in Y position [mm]"); residualREF2y0->SetYTitle("Frequency");residualREF2y0->SetLabelSize(0.05,"XY");residualREF2y0->SetTitleSize(0.05,"XY");
      residualREF3X = new TH1F(nameRes3X,"",PlotBins,-5,5); residualREF3X->SetXTitle("Residual in r [mm]"); residualREF3X->SetYTitle("Frequency");residualREF3X->SetLabelSize(0.05,"XY");residualREF3X->SetTitleSize(0.05,"XY");
      residualREF3Y = new TH1F(nameRes3Y,"",PlotBins,-PlotAxisRange,PlotAxisRange); residualREF3Y->SetXTitle("Residual in Angle [urad]"); residualREF3Y->SetYTitle("Frequency");residualREF3Y->SetLabelSize(0.05,"XY");residualREF3Y->SetTitleSize(0.05,"XY");
      residualREF4X = new TH1F(nameRes4X,"",PlotBins,-5,5); residualREF4X->SetXTitle("Residual in r [mm]"); residualREF4X->SetYTitle("Frequency");residualREF4X->SetLabelSize(0.05,"XY");residualREF4X->SetTitleSize(0.05,"XY");
      residualREF4Y = new TH1F(nameRes4Y,"",PlotBins,-PlotAxisRange,PlotAxisRange); residualREF4Y->SetXTitle("Residual in Angle [urad]"); residualREF4Y->SetYTitle("Frequency");residualREF4Y->SetLabelSize(0.05,"XY");residualREF4Y->SetTitleSize(0.05,"XY");
      if(finalRes == true){
	//LAGDResM3 = new TH1F(nameRes2X,"LMG Resolution Method 3",100,-5000,5000); LAGDResM3->SetXTitle("Residual in Angle [urad]"); LAGDResM3->SetYTitle("Frequency");LAGDResM3->SetLabelSize(0.05,"XY");LAGDResM3->SetTitleSize(0.05,"XY");
	
	LAGDResM2 = new TH1F(nameRes1X,"LMG Resolution Method 2",100,-1500,1500); LAGDResM2->SetXTitle("Residual in Angle [urad]"); LAGDResM2->SetYTitle("Frequency");LAGDResM2->SetLabelSize(0.05,"XY");LAGDResM2->SetTitleSize(0.05,"XY");
	LAGDResPerX = new TGraph();
	LAGDResPerY = new TGraph();

	LAGDResPerX2D = new TH2D("X2D", "Residual vs X coordinate; X coordinate [mm];Residual [urad]", 100, minx, maxx, 10000, -10000, 10000);
	LAGDResPerY2D = new TH2D("Y2D", "Residual vs Y coordinate; Y coordinate [mm];Residual [urad]", 100, miny, maxy, 10000, -10000, 10000);

	
	
	LAGDResPerXProf = new TProfile("Xprofile", "Profile Plot of Residual vs X coordinate;X Coordinate [mm];LMG Residual [urad]", binnum, minx, maxx, -10000, 10000, "s");
	LAGDResPerYProf = new TProfile("Yprofile", "Profile Plot of Residual vs Y coordinate;Y Coordinate [mm];LMG Residual [urad]", binnum, miny, maxy, -10000, 10000, "s");
	LAGDResPerEtaProf = new TProfile("Etaprofile", "Profile Plot of Residual vs #eta;#eta [strip];LMG Residual [urad]", 30, -0.5, 0.5, -1000, 1000, "s");
	LAGDResPerEtaProfEven = new TProfile("Etaprofileeven", "Profile Plot of Residual vs #eta of Even SM Events; #eta; LMG Residual [urad]", 16, -0.5, 0.5, -1000, 1000, "s");
	LAGDResPerEtaProfOdd = new TProfile("Etaprofileodd", "Profile Plot of Residual vs #eta of Odd SM Events; #eta; LMG Residual [urad]", 16, -0.5, 0.5, -400, 400, "s");

	//binnum = 10;
	//binwidth = (maxx-minx)/binnum;
	for(uint u = 0; u < binnum; u++){
	  string name;
	  name += to_string(u);
	  char names[name.size()+1];
	  strcpy(names, name.c_str());

	  
	  
	  LAGDResXHist[u] = new TH1F(names, "Residual Per X chunk", 300, -2000, 2000);
	  LAGDResYHist[u] = new TH1F(names, "Residual Per Y chunk", 300, -2000, 2000);
	}
	LAGDResPerEta = new TGraph();
	LAGDResPerEtaHist = new TH2D("AllSMreseta", "LMG residual vs #eta;#eta;LMG Residual [urad]", 150, -0.5, 0.5, 150, -5000, 5000);
	LAGDResPerEtaEven = new TH2D("EvenSMreseta", "LMG residual vs #eta for Even SM events;#eta;LMG Residual [urad]", 150, -0.5, 0.5, 150, -5000, 5000);
	LAGDResPerEtaEvenG = new TGraph();
	LAGDResPerEtaOdd = new TH2D("OddSMreseta", "LMG residual vs #eta for Odd SM events;#eta;LMG Residual [urad]", 150, -0.5, 0.5, 150, -5000, 5000);
	LAGDResPerEtaOddG = new TGraph();
	residualLAGD = new TH1F(nameResLAGD,"",PlotBins,-PlotAxisRange,PlotAxisRange); residualLAGD->SetXTitle("Residual in Angle [urad]"); residualLAGD->SetYTitle("Frequency");residualLAGD->SetLabelSize(0.05,"XY");residualLAGD->SetTitleSize(0.05,"XY");
	//incresidualREF1Y = new TH1F(nameRes1Y,"",100,-5000,5000); incresidualREF1Y->SetXTitle("Inclusive Residual in Angle [urad]"); incresidualREF1Y->SetYTitle("Frequency");incresidualREF1Y->SetLabelSize(0.05,"XY");incresidualREF1Y->SetTitleSize(0.05,"XY");
	//incresidualREF2Y = new TH1F(nameRes2Y,"",100,-5000,5000); incresidualREF2Y->SetXTitle("Inclusive Residual in Angle [urad]"); incresidualREF2Y->SetYTitle("Frequency");incresidualREF2Y->SetLabelSize(0.05,"XY");incresidualREF2Y->SetTitleSize(0.05,"XY");
	//incresidualREF3Y = new TH1F(nameRes3Y,"",100,-5000,5000); incresidualREF3Y->SetXTitle("Inclusive Residual in Angle [urad]"); incresidualREF3Y->SetYTitle("Frequency");incresidualREF3Y->SetLabelSize(0.05,"XY");incresidualREF3Y->SetTitleSize(0.05,"XY");
	//incresidualREF4Y = new TH1F(nameRes4Y,"",100,-5000,5000); incresidualREF4Y->SetXTitle("Inclusive Residual in Angle [urad]"); incresidualREF4Y->SetYTitle("Frequency");incresidualREF4Y->SetLabelSize(0.05,"XY");incresidualREF4Y->SetTitleSize(0.05,"XY");
	//incresidualLAGD = new TH1F(nameResLAGD,"",100,-5000,5000); incresidualLAGD->SetXTitle("Inclusive Residual in Angle [urad]"); incresidualLAGD->SetYTitle("Frequency");incresidualLAGD->SetLabelSize(0.05,"XY");incresidualLAGD->SetTitleSize(0.05,"XY");
	TrackErrorsAtLAGD = new TH1F(nameRes1X,"",100,-2,2); TrackErrorsAtLAGD->SetXTitle("Error in Fitted Track at LAGD Z location"); TrackErrorsAtLAGD->SetYTitle("Frequency");TrackErrorsAtLAGD->SetLabelSize(0.05,"XY");TrackErrorsAtLAGD->SetTitleSize(0.05,"XY");
	TrackSlopeErrorsAtLAGD = new TH1F(nameRes1X,"",80,0,0.001); TrackSlopeErrorsAtLAGD->SetXTitle("Error in Fitted Track Slope"); TrackSlopeErrorsAtLAGD->SetYTitle("Frequency");TrackSlopeErrorsAtLAGD->SetLabelSize(0.05,"XY");TrackSlopeErrorsAtLAGD->SetTitleSize(0.05,"XY");
	TrackYintErrorsAtLAGD = new TH1F(nameRes1X,"",80,-0.1,1); TrackYintErrorsAtLAGD->SetXTitle("Error in Fitted Track Y intercept"); TrackYintErrorsAtLAGD->SetYTitle("Frequency");TrackYintErrorsAtLAGD->SetLabelSize(0.05,"XY");TrackYintErrorsAtLAGD->SetTitleSize(0.05,"XY");
	

      }
      if(finalRes == false){residualLAGD = new TH1F(nameResLAGD,"",PlotBins,-PlotAxisRange,PlotAxisRange); residualLAGD->SetXTitle("Residual in Angle [urad]"); residualLAGD->SetYTitle("Frequency");residualLAGD->SetLabelSize(0.05,"XY");residualLAGD->SetTitleSize(0.05,"XY");}

      //residualLAGD = new TH1F(nameResLAGD,"",800,-500,500); residualLAGD->SetXTitle("Residual in Angle [mRad]"); residualLAGD->SetYTitle("Frequency");residualLAGD->SetLabelSize(0.05,"XY");residualLAGD->SetTitleSize(0.05,"XY");
      residualLAGDr = new TH1F(nameResLAGD,"",10000,-1000,1000); residualLAGDr->SetXTitle("Residual in r [mm]"); residualLAGDr->SetYTitle("Frequency");residualLAGDr->SetLabelSize(0.05,"XY");residualLAGDr->SetTitleSize(0.05,"XY");
      //TAngDist = new TH1F(nameTDist,"",200,-0.3,0.3); TAngDist->SetXTitle("Angle [Rad]"); TAngDist->SetYTitle("Frequency");TAngDist->SetLabelSize(0.05,"XY");TAngDist->SetTitleSize(0.05,"XY");
      //T2AngDist = new TH1F(nameT2Dist,"",200,-0.3,0.3); T2AngDist->SetXTitle("Angle [Rad]"); T2AngDist->SetYTitle("Frequency");T2AngDist->SetLabelSize(0.05,"XY");T2AngDist->SetTitleSize(0.05,"XY");
      //T3AngDist = new TH1F(nameT3Dist,"",200,-0.3,0.3); T3AngDist->SetXTitle("Angle [Rad]"); T3AngDist->SetYTitle("Frequency");T3AngDist->SetLabelSize(0.05,"XY");T3AngDist->SetTitleSize(0.05,"XY");
      //T4AngDist = new TH1F(nameT4Dist,"",200,-0.3,0.3); T4AngDist->SetXTitle("Angle [Rad]"); T4AngDist->SetYTitle("Frequency");T4AngDist->SetLabelSize(0.05,"XY");T4AngDist->SetTitleSize(0.05,"XY");
      AvgAngDist = new TH1F(nameAvgDist,"",200,-100,100); AvgAngDist->SetXTitle("Angle [mRad]"); AvgAngDist->SetYTitle("Frequency");AvgAngDist->SetLabelSize(0.05,"XY");AvgAngDist->SetTitleSize(0.05,"XY");
      LAngDist = new TH1F(nameLDist,"",200,-100, 100); LAngDist->SetXTitle("Angle [mRad]"); LAngDist->SetYTitle("Frequency");LAngDist->SetLabelSize(0.05,"XY");LAngDist->SetTitleSize(0.05,"XY");
      //rphiDist = new TH1F(nameLDist,"",200,0,50); rphiDist->SetXTitle("r*#phi [mm]"); rphiDist->SetYTitle("Frequency");rphiDist->SetLabelSize(0.05,"XY");rphiDist->SetTitleSize(0.05,"XY");
      //yDist = new TH1F(nameLDist,"",200,0,50); yDist->SetXTitle("Average Tracker Y at LAGD location [mm]"); yDist->SetYTitle("Frequency");yDist->SetLabelSize(0.05,"XY");yDist->SetTitleSize(0.05,"XY");
      /*
      char nameChi2X[50]; sprintf(nameChi2X,"XTrackChi2_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameChi2Y[50]; sprintf(nameChi2Y,"YTrackChi2_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      xTrackChi2 = new TH1F(nameChi2X,"Chi square of tracks in X projection",1000,0,0.2); xTrackChi2->SetXTitle("#chi^{2} of track in r"); xTrackChi2->SetYTitle("Frequency");
      xTrackChi2->SetTitleSize(0.04,"XY"); xTrackChi2->SetLabelSize(0.04,"XY");
      yTrackChi2 = new TH1F(nameChi2Y,"Chi square of tracks in Y projection",1000,0,0.00002); yTrackChi2->SetXTitle("#chi^{2} of track in #phi"); yTrackChi2->SetYTitle("Frequency");
      yTrackChi2->SetTitleSize(0.04,"XY"); yTrackChi2->SetLabelSize(0.04,"XY");
      */
      //cout << "Here" << endl;
      sumx=0.0;
      sumy=0.0;
      //float T12AvgAngle = 0.0;
      //float T34AvgAngle = 0.0;
      float AvgAngle = 0.0;
      for(unsigned int i=0; i< APV3E1.size(); i++){//iterate through all events in the LAGD hit vector
	//Apply offsets to tracker coordinates
	//cout << "Here" << endl;
	//cout << Tracker1x.at(i) << endl;
	Tracker1xtemp = Tracker1x.at(i) + Xoffset + InitialXoffset;// - XrFix;
	//cout << Tracker1xtemp << endl;
	
	sumx += Tracker1xtemp;
	Tracker1ytemp = Tracker1y.at(i) + Yoffset + InitialYoffset;// - YrFix;
	//cout << Tracker1ytemp << endl;
	Tracker2xtemp = Tracker2x.at(i) + Xoffset + InitialXoffset;// - XrFix;
	Tracker2ytemp = Tracker2y.at(i) + Yoffset + InitialYoffset;// - YrFix;
	//Hitspot->Fill(Tracker2x.at(i), Tracker2y.at(i));
	//Hitspotproj->Fill(Tracker2y.at(i));
	Tracker3xtemp = Tracker3x.at(i) + Xoffset + InitialXoffset;// - XrFix;
	//cout << "Tracker3x Coordinate: " << Tracker3xtemp << endl;
	Tracker3ytemp = Tracker3y.at(i) + Yoffset + InitialYoffset;// - YrFix;
	//cout << "Tracker3y Coordinate: " << Tracker3ytemp << endl;
	Tracker4xtemp = Tracker4x.at(i) + Xoffset + InitialXoffset;// - XrFix;
	Tracker4ytemp = Tracker4y.at(i) + Yoffset + InitialYoffset;// - YrFix;

	//now convert to angle and put into their own vectors

	float Tracker1R = sqrt((Tracker1xtemp)*(Tracker1xtemp) + Tracker1ytemp*Tracker1ytemp);
	T1r.push_back(Tracker1R);
	//cout << Tracker1ytemp << ", " << Tracker1xtemp << endl;	
	float Tracker1Phi = atan(Tracker1ytemp/Tracker1xtemp);
	//cout << Tracker1Phi*1000 << endl;
	T1phi.push_back(Tracker1Phi*1000);
	//cout << T1r.at(i) << ", " << T1phi.at(i) << endl;
	//TAngDist->Fill(Tracker1Phi);
	//T12AvgAngle = Tracker1Phi;
	//cout << Tracker2xtemp << ", " << Tracker2ytemp <<endl;	  
	float Tracker2R = sqrt((Tracker2xtemp)*(Tracker2xtemp) + Tracker2ytemp*Tracker2ytemp);
	T2r.push_back(Tracker2R);
	float Tracker2Phi = atan(Tracker2ytemp/Tracker2xtemp);
	T2phi.push_back(1000*Tracker2Phi);
	//T2AngDist->Fill(atan(Tracker2ytemp/Tracker2xtemp));
	//T12AvgAngle += atan(Tracker2ytemp/Tracker2xtemp);	
	//cout << T2r.at(i) << ", " << T2phi.at(i) << endl;

	//cout << Tracker3xtemp << ", " << Tracker3ytemp <<endl;	  
	float Tracker3R = sqrt((Tracker3xtemp)*(Tracker3xtemp) + Tracker3ytemp*Tracker3ytemp);
	T3r.push_back(Tracker3R);
	float Tracker3Phi = atan(Tracker3ytemp/Tracker3xtemp);
	T3phi.push_back(1000*Tracker3Phi);
	//T3AngDist->Fill(atan(Tracker3ytemp/Tracker3xtemp));
	//T34AvgAngle = atan(Tracker3ytemp/Tracker3xtemp);
	//cout << T3r.at(i) << ", " << T3phi.at(i) << endl;

	//cout << Tracker4xtemp << ", " << Tracker4ytemp <<endl;	  		  
	float Tracker4R = sqrt((Tracker4xtemp)*(Tracker4xtemp) + Tracker4ytemp*Tracker4ytemp);
	T4r.push_back(Tracker4R);
	float Tracker4Phi = atan(Tracker4ytemp/Tracker4xtemp);
	T4phi.push_back(1000*Tracker4Phi);
	//T4AngDist->Fill(atan(Tracker4ytemp/Tracker4xtemp));
	//T34AvgAngle += atan(Tracker4ytemp/Tracker4xtemp);
	//cout << T4r.at(i) << ", " << T4phi.at(i) << endl;

	//cout << "OG value: " << APV3E1.at(i) << endl;
	float LAGDangle = (APV3E1.at(i))*Strippitch;



	LAGDphi.push_back(LAGDangle);//angle is relative to the middle line (ie, add half of the angle to this afterwards to get the real angle)
	LAngDist->Fill(LAGDangle);       
	//cout << "Angular value: " << LAGDangle << endl;
	//AvgAngle = 1000*((T12AvgAngle/2) + (T34AvgAngle/2))*(828.2/1596);//avg angle at the LAGD location

	if(finalRes){
	  cout << 1000*Tracker1Phi << ", " << LAGDangle << endl;
	  Stripcorr->Fill(1000*Tracker1Phi, LAGDangle);
	  TrackerStripcorr->Fill(1000*Tracker1Phi, 1000*Tracker3Phi);
	  
	  StripcorrProf->Fill(1000*Tracker1Phi, LAGDangle);
	  TrackerStripcorrProf->Fill(1000*Tracker1Phi, 1000*Tracker3Phi);
	}
	
	//yDist->Fill((((Tracker1ytemp+Tracker2ytemp)/2.0) + ((Tracker3ytemp+Tracker4ytemp)/2.0))*(828.2/1596.4));
	//both vectors are now in polar coordinates, time to find residuals  
	
	  
	
	//fit track in r direction
	TGraph* g1 = new TGraph();
	g1->SetPoint(0,1596.4, T1r.at(i));
	g1->SetPoint(1,1541.6, T2r.at(i));//swapped 2 and 3
	g1->SetPoint(2,54.8, T3r.at(i));
	g1->SetPoint(3,0, T4r.at(i));
	//g1->GetXaxis()->SetRangeUser(0,1600);

	TF1* f1 = new TF1("line1","pol1",0,1600);
	g1->Fit("line1","Q");
	double intercept1 = f1->GetParameter(0);
	double slope1     = f1->GetParameter(1);
	//cout << intercept1 << ", " << slope1 << endl;
	double MeasuredREF1X = intercept1 + slope1*1596.4;
	double MeasuredREF2X = intercept1 + slope1*1541.6;
	double MeasuredLAGDr = intercept1 + slope1*828.2;	
	double MeasuredREF3X = intercept1 + slope1*54.8;
	double MeasuredREF4X = intercept1 + slope1*0;



	//cout << "Here" << endl;
	residualREF1X->Fill(MeasuredREF1X-T1r.at(i));
	residualREF2X->Fill(MeasuredREF2X-T2r.at(i));
	residualREF3X->Fill(MeasuredREF3X-T3r.at(i));
	residualREF4X->Fill(MeasuredREF4X-T4r.at(i));

	LAGDRfromTrackers->Fill(MeasuredLAGDr);
	
	//cout << "Ideal Z? " << (-4.1 + LAGDr-intercept1)/slope1 << endl;
	//xTrackChi2->Fill(f1->GetChisquare());
	//totalChi2 += f1->GetChisquare();
	delete f1; delete g1;
	
	//residualLAGDr->Fill(MeasuredLAGDr - LAGDr);
	//fit track in phi direction

	TGraph* g2 = new TGraph();

	
	g2->SetPoint(0,1596.4, T1phi.at(i));
	g2->SetPoint(1,1541.6, T2phi.at(i));	
	//g2->SetPoint(2,828.2, LAGDphi.at(i));	
	g2->SetPoint(2,54.8, T3phi.at(i));	
	g2->SetPoint(3,0, T4phi.at(i));		
	
	// inclusive
	TF1* f2 = new TF1("line2","pol1");
	g2->Fit("line2", "Q");
	//  return;
	//if(Yoffset == 8 && Xoffset == 0){
	//g2->Draw("A*");
	// return;
	  //}


	//cout << "Here" << endl;
	double intercept2 = f2->GetParameter(0);
	double slope2     = f2->GetParameter(1);

	double sloperr = f2->GetParError(1);
	//TrackSlopeErrorsAtLAGD->Fill(sloperr);
	double yinterr = f2->GetParError(0);
	//TrackYintErrorsAtLAGD->Fill(yinterr);
	  
	//cout << "Error in slope: " << sloperr << ", Error in y int: " << yinterr << endl;
	if(finalRes){  
	  TrackerError = sqrt(abs((828.2*sloperr)*(828.2*sloperr) + yinterr*yinterr));
	  //cout << TrackerError << endl;
	  
	  TrackErrorsAtLAGD->Fill(TrackerError);
	}
	double MeasuredREF1Y = intercept2 + slope2*1596.4;
	double MeasuredREF2Y = intercept2 + slope2*1541.6;
	double MeasuredLAGD  = intercept2 + slope2*828.2;//828.2;
	double MeasuredREF3Y = intercept2 + slope2*54.8;
	double MeasuredREF4Y = intercept2 + slope2*0;	  


	if(Plottingfile){
	  float LAGDXcoor = MeasuredLAGDr*cos(LAGDangle/1000);
	  float LAGDYcoor = MeasuredLAGDr*sin(LAGDangle/1000);
	  //BeamsFile << LAGDXcoor << " " << LAGDYcoor << endl;

	  float TrackerXcoor = MeasuredLAGDr*cos(MeasuredLAGD/1000);
	  float TrackerYcoor = MeasuredLAGDr*sin(MeasuredLAGD/1000);
	  //BeamsCartFile << TrackerXcoor << " " << TrackerYcoor << endl;
	}

	
	//efficiency testing
	//totalEvents++;
	// if(LAGDangle <= MeasuredLAGD + 5*LAGDREs && LAGDangle >= MeasuredLAGD - 5*LAGDREs){
	//   EventsthatPass++;        
	// }
	
	AvgAngDist->Fill(MeasuredLAGD);
	//cout << "LAGD angle from fit: " << MeasuredLAGD*1000.0 << endl;

	//cout << "Tracker reco LAGD Angle: " << MeasuredLAGD << "; LAGD angle: " << LAGDphi.at(i) << endl;
	float ExclusiveRes = MeasuredLAGD-LAGDphi.at(i);
	//cout << "Res Here:" << ExclusiveRes*1000 << endl;

	residualREF1Y->Fill((MeasuredREF1Y-T1phi.at(i))*1000);//0.4
	residualREF2Y->Fill((MeasuredREF2Y-T2phi.at(i))*1000);
	residualREF3Y->Fill((MeasuredREF3Y-T3phi.at(i))*1000);
	residualREF4Y->Fill((MeasuredREF4Y-T4phi.at(i))*1000);
	
	auto Rescorrection=0.0;
	auto FixedExRes=0.0;
	if(rescor == false)
	  residualLAGD->Fill((ExclusiveRes)*1000);//4.1
	
	if(rescor == true){
	  if(SMperEvent.at(i) != 1){
	    if(SMperEvent.at(i)%2 == 1){
	      double coefficients[2] = {0.266275, 96.2073};
	      Rescorrection = coefficients[1]*coefficients[0]*etavalues.at(i) / (etavalues.at(i)*etavalues.at(i) + coefficients[0]*coefficients[0]);
	      cout << ExclusiveRes*1000 << " - " << Rescorrection << endl;
	      FixedExRes = (ExclusiveRes*1000)-Rescorrection;
	      residualLAGD->Fill(FixedExRes);//4.1
	    }	  
	    if(SMperEvent.at(i)%2 == 0){
	      if(etavalues.at(i) <= 0){
		double coefficients[6] = {77.2602, 739.855, 3638.77, 13398.1, 25364.7, 16776.9};
		Rescorrection = coefficients[5]*pow(etavalues.at(i),5) + coefficients[4]*pow(etavalues.at(i),4) + coefficients[3]*pow(etavalues.at(i),3) + coefficients[2]*pow(etavalues.at(i),2) + coefficients[1]*etavalues.at(i) + coefficients[0];;
		cout << ExclusiveRes*1000 << " - " << Rescorrection << endl;
		FixedExRes = (ExclusiveRes*1000)-Rescorrection;
		residualLAGD->Fill(FixedExRes);//4.1
	      }
	      if(etavalues.at(i) > 0){
		double coefficients[5] = {-96.9384, 677.594, -1736.05, 3049.43, -3016.76};
		Rescorrection = coefficients[4]*pow(etavalues.at(i),4) + coefficients[3]*pow(etavalues.at(i),3) + coefficients[2]*pow(etavalues.at(i),2) + coefficients[1]*etavalues.at(i) + coefficients[0];
		cout << ExclusiveRes*1000 << " - " << Rescorrection << endl;
		FixedExRes = (ExclusiveRes*1000)-Rescorrection;
		residualLAGD->Fill(FixedExRes);//4.1
	      }
	    }
	  } else {	    
	    Rescorrection = 0;
	  }
	}
	//cout << "Residual here: " << (ExclusiveRes)*1000 << endl;
	//yTrackChi2->Fill(f2->GetChisquare());
	//totalChi2 += f2->GetChisquare();
	delete f2; delete g2;

	if(finalRes){
	  
	  cout << ExclusiveRes*1000 << endl;
	  
	  if(rescor == true)
	    FixedExRes = ExclusiveRes - Rescorrection/1000;
	  if(rescor == false)
	    FixedExRes = ExclusiveRes;

	  cout << "Ex Res: " << FixedExRes*1000 << ", TE: " << TrackerError*1000 << endl; 
	  auto LAGDresHere = sqrt(abs(FixedExRes*1000*FixedExRes*1000 - (TrackerError*1000)*(TrackerError*1000)));
	  //cout << LAGDresHere*1000 << endl; 
	  LAGDResM2->Fill(LAGDresHere);

	  ResPerAngle->SetPoint(cartit, LAGDphi.at(i)/1000, FixedExRes*1000);
	  cartit++;
	  
	  auto xcor = (MeasuredLAGDr/1000)*cos(LAGDphi.at(i)/1000);
	  //cout << (MeasuredLAGDr/1000) << ", " << LAGDphi.at(i) << endl;
	  auto ycor = (MeasuredLAGDr/1000)*sin(LAGDphi.at(i)/1000);
	  auto cartRes = (LAGDresHere/1000)*(MeasuredLAGDr);
	  //cout << i << ", " << xcor << ", " << 100*xcor << ", " << int(100*xcor) << ", " <<  int(100*xcor)%20 << ", " << double(int(100*xcor)%20)/100 << endl;
	  //return;
	  //LAGDResPerX->SetPoint(i,xcor*1000,cartRes);
	  //LAGDResPerY->SetPoint(i,ycor*1000,cartRes);
	  //cout << double(int(1000*xcor)%200)/1000 << endl;
	  //auto moduleiter = double(int(1000*xcor)%200)/1000;
	  /*
	  Modulusvals.push_back(moduleiter);
	  if(find(Modulusvals.begin(), Modulusvals.end(), moduleiter) == BadEventVector.end()){//so != vector.end() means the event IS in that vector
	    HitsPerxrange[moduleiter] = new TH1F(moduleiter, "", )
	  }
	  */
	  //rescor = true;
	  cout << xcor*1000 << ", " << ycor*1000 << endl;
	  
	  if(((xcor*1000 <= maxx && xcor*1000 >= minx) && (ycor*1000 <= maxy && ycor*1000 >= miny))){
	    cout << xcor*1000 << ", " << ycor*1000 << endl;
	    
	      auto xhistnum = int((xcor*1000 - minx)/binwidthx);
	      auto yhistnum = int((ycor*1000 - miny)/binwidthy);
	      if(find(Modulusvals.begin(), Modulusvals.end(), xhistnum) == Modulusvals.end())
	      Modulusvals.push_back(xhistnum);
	  
	      //cout << "hist num: " << histnum << endl;
	      LAGDResXHist[xhistnum]->Fill(FixedExRes*1000);

	      LAGDResYHist[yhistnum]->Fill(FixedExRes*1000);
		
	      LAGDResPerX->SetPoint(i,xcor*1000,FixedExRes*1000);
	      LAGDResPerY->SetPoint(i,ycor*1000,FixedExRes*1000);

	      LAGDResPerXProf->Fill(xcor*1000,FixedExRes*1000, 1);
	      LAGDResPerYProf->Fill(ycor*1000,FixedExRes*1000, 1);

	  
	      LAGDResPerX2D->Fill(xcor*1000,FixedExRes*1000);
	      LAGDResPerY2D->Fill(ycor*1000,FixedExRes*1000);
	      
	    /*
	      LAGDResPerX->SetPoint(i,xcor*1000,cartRes);
	      LAGDResPerY->SetPoint(i,ycor*1000,cartRes);

	      LAGDResPerXProf->Fill(xcor*1000,cartRes);
	      LAGDResPerYProf->Fill(ycor*1000,cartRes);
	    */
	  
	    cout << "HERE" << endl;
	    //if(FixedExRes*MeasuredLAGDr < 800){
	    //LAGDResPerEta->SetPoint(i, etavalues.at(i), (ExclusiveRes)*1000);
	    //LAGDResPerEtaProf->Fill(etavalues.at(i), (ExclusiveRes)*1000);
	    LAGDResPerEta->SetPoint(i, etavalues.at(i), (FixedExRes)*MeasuredLAGDr);
	    LAGDResPerEtaHist->Fill(etavalues.at(i), (FixedExRes)*MeasuredLAGDr);	      
	    LAGDResPerEtaProf->Fill(etavalues.at(i), (FixedExRes)*MeasuredLAGDr);
	    cout << "HERE" << endl;
	    if(SMperEvent.at(i)%2 == 0){
	      LAGDResPerEtaEvenG->SetPoint(i, etavalues.at(i), (FixedExRes)*MeasuredLAGDr);
	      LAGDResPerEtaEven->Fill(etavalues.at(i), (FixedExRes)*MeasuredLAGDr);
	      LAGDResPerEtaProfEven->Fill(etavalues.at(i), (FixedExRes)*MeasuredLAGDr);
	    }
	    cout << "HERE" << endl;
	    if(SMperEvent.at(i)%2 == 1){
	      LAGDResPerEtaOddG->SetPoint(i, etavalues.at(i), (FixedExRes)*MeasuredLAGDr);
	      LAGDResPerEtaOdd->Fill(etavalues.at(i), (FixedExRes)*MeasuredLAGDr);
	      LAGDResPerEtaProfOdd->Fill(etavalues.at(i), (FixedExRes)*MeasuredLAGDr);
	    }
	    cout << "HERE" << endl;
	      
	    //}
	  }
	  // cout << "after" << endl;
	  // //need inclusive residual too
	  // TGraph* g3 = new TGraph();
	  
	  
	  // g3->SetPoint(0,1596.4, T1phi.at(i));
	  // g3->SetPoint(1,1541.6, T2phi.at(i));	
	  // g3->SetPoint(2,828.2, LAGDphi.at(i));	
	  // g3->SetPoint(3,54.8, T3phi.at(i));	
	  // g3->SetPoint(4,0, T4phi.at(i));		
	  
	  // // inclusive
	  // TF1* f3 = new TF1("line2","pol1");
	  // g3->Fit("line2", "Q");
	  // //  return;
	  // //if(Yoffset == 8 && Xoffset == 0){
	  // //g2->Draw("A*");
	  // // return;
	  // //}
	  

	
	  // double intercept3 = f3->GetParameter(0);
	  // double slope3     = f3->GetParameter(1);
	  
	  // sloperr = f2->GetParError(1);
	  // //TrackSlopeErrorsAtLAGD->Fill(sloperr);
	  // yinterr = f2->GetParError(0);
	  // //TrackYintErrorsAtLAGD->Fill(yinterr);
	  
	  // //cout << "Error in slope: " << sloperr << ", Error in y int: " << yinterr << endl;
	  
	  // //TrackerError = sqrt((828.2*sloperr)*(828.2*sloperr) + yinterr*yinterr);
	  // //cout << TrackerError << endl;
	
	  // //TrackErrorsAtLAGD->Fill(TrackerError);
	  
	  // double InclusiveLAGDval  = intercept3 + slope3*828.2;//828.2;

	  // auto InclusiveRes = InclusiveLAGDval - LAGDphi.at(i);
	  // //cout << InclusiveRes << endl;
	  // //LAGDResM3->Fill(sqrt(InclusiveRes*ExclusiveRes)*1000);
	  // cout << InclusiveRes << endl;
	  
	}
	
	//cout << "Here" << endl;
	Tracker1xtemp=0;
	Tracker1ytemp=0;
	Tracker2xtemp=0;
	Tracker2ytemp=0;
	Tracker3xtemp=0;
	Tracker3ytemp=0;
	Tracker4xtemp=0;
	Tracker4ytemp=0;
	APV3E1temp=0;


	
      }

      if(finalRes){
	T1rval = accumulate(T1r.begin(), T1r.end(), 0.0)/T1r.size();	  
	T2rval = accumulate(T2r.begin(), T2r.end(), 0.0)/T2r.size();
	T3rval = accumulate(T3r.begin(), T3r.end(), 0.0)/T3r.size();
	T4rval = accumulate(T4r.begin(), T4r.end(), 0.0)/T4r.size();
	LAGDrval = LAGDRfromTrackers->GetMean();
      }
      
      //int EventsthatPass = 0;
      //int totalEvents = 0;
      //float LAGDREs = 0;//in mRad!
  
      
     
      //return;
      
      
      bool rescor = false;
      //int SM = 0;
      if(rescor){
	
      
        auto h = new TCanvas("", "", 1000, 500);
	h->Divide(2,1);
	gStyle->SetOptStat(0);
	int rangevals = 5000;

	 
	auto h1 = new TCanvas("", "", 1500, 500);
	h1->Divide(3,1);
	h1->cd(1);
	LAGDResPerEtaEven->Draw("COLZ");
	LAGDResPerEtaEven->GetYaxis()->SetRangeUser(-rangevals, rangevals);
	auto EvenCartaxis = new TGaxis(0.5,-rangevals,0.5,rangevals,-rangevals*(LAGDrval/1000),rangevals*(LAGDrval/1000),510,"+L");	
	EvenCartaxis->SetTitle("LAGD Residual [um]");
	EvenCartaxis->SetLabelFont(42);
	EvenCartaxis->SetLabelSize(0.035);
	EvenCartaxis->SetTitleSize(0.035);
	EvenCartaxis->SetTitleOffset(1.5);
	
	EvenCartaxis->Draw();

	h1->cd(2);
	LAGDResPerEtaOdd->Draw("COLZ");
	LAGDResPerEtaOdd->GetYaxis()->SetRangeUser(-rangevals, rangevals);
	auto OddCartaxis = new TGaxis(0.5,-rangevals,0.5,rangevals,-rangevals*(LAGDrval/1000),rangevals*(LAGDrval/1000),510,"+L");	
	OddCartaxis->SetTitle("LAGD Residual [um]");
	OddCartaxis->SetLabelFont(42);
	OddCartaxis->SetLabelSize(0.035);
	OddCartaxis->SetTitleSize(0.035);
	OddCartaxis->SetTitleOffset(1.5);
	
	OddCartaxis->Draw();

	h1->cd(3);
	LAGDResPerEtaHist->Draw("COLZ");
	LAGDResPerEtaHist->GetYaxis()->SetRangeUser(-rangevals, rangevals);
	auto BothCartaxis = new TGaxis(0.5,-rangevals,0.5,rangevals,-rangevals*(LAGDrval/1000),rangevals*(LAGDrval/1000),510,"+L");	
	BothCartaxis->SetTitle("LAGD Residual [um]");
	BothCartaxis->SetLabelFont(42);
	BothCartaxis->SetLabelSize(0.035);
	BothCartaxis->SetTitleSize(0.035);
	BothCartaxis->SetTitleOffset(1.5);
	
	BothCartaxis->Draw();

	h1->Draw();
	
	return;
	
	/*
	h->cd(1);	
	LAGDResPerEtaOddG->Draw("AP");
	TF1* f1 = new TF1("lineOdd", "[0]*[1]*x/(x*x + [0]*[0])", -0.5, 0.5);
	f1->SetParameter(0,415);
	f1->SetParameter(1,0.3097);
	
	LAGDResPerEtaOddG->Fit("lineOdd", "R");
	h->Draw();
	return;
	*/
	rangevals = 500;
	//if(SM == 3){
	//TF1* f1 = new TF1("lineOdd","[0]*[1]*x/(x*x + [0]*[0])", -0.5, 0.5);
	
	h->cd(1);
	LAGDResPerEtaOddG->Draw("AP");
	LAGDResPerEtaOddG->GetXaxis()->SetRangeUser(-0.5, 0.5);
	LAGDResPerEtaOddG->GetYaxis()->SetRangeUser(-rangevals, rangevals);
	
	TF1* f1 = new TF1("lineOdd", "[0]*[1]*x/(x*x + [0]*[0])", -0.5, 0.5);
	f1->SetParameter(0, 0.01);
	f1->SetParameter(1,50);

	LAGDResPerEtaOddG->Fit("lineOdd", "R");		
	//LAGDResPerEtaProfOdd->Fit("lineOdd", "R");		
	LAGDResPerEtaProfOdd->Draw("same");
	//LAGDResPerEtaOdd->Draw("same");
	//h->Draw();
	//return;
	//LAGDResPerEtaOdd->SetTitle("LAGD Residual vs #eta in FZZ SM:3;#eta;LAGD Residual [um]");

	auto CartXaxis = new TGaxis(0.5,-rangevals,0.5,rangevals,-rangevals*(LAGDrval/1000),rangevals*(LAGDrval/1000),510,"+L");	
	CartXaxis->SetTitle("LMG Residual [um]");
	CartXaxis->SetLabelFont(42);
	CartXaxis->SetLabelSize(0.035);
	CartXaxis->SetTitleSize(0.035);
	CartXaxis->SetTitleOffset(1.5);
	
	CartXaxis->Draw();

	cout << "Odd SM fit function coefficients: " << f1->GetParameter(0) << ", " << f1->GetParameter(1) << endl;
	
	h->cd(2);
	rangevals = 500;
	LAGDResPerEtaProfEven->Draw();
	LAGDResPerEtaProfEven->GetXaxis()->SetRangeUser(-0.5, 0.5);
	LAGDResPerEtaProfEven->GetYaxis()->SetRangeUser(-rangevals, rangevals);
	
	TF1* f2 = new TF1("lineEven1","pol4", 0, 0.5);
	TF1* f3 = new TF1("lineEven2","pol5", -0.5, 0);
	
	//f1->SetParameter(0,415);
	//f1->SetParameter(1,0.3097);
	
	LAGDResPerEtaProfEven->Fit("lineEven1", "R");
	LAGDResPerEtaProfEven->Fit("lineEven2", "R+");
	
	LAGDResPerEtaEvenG->Draw("Psame");
	//LAGDResPerEtaEven->Draw("same");
	auto CartYaxis = new TGaxis(0.5,-rangevals,0.5,rangevals,-rangevals*(LAGDrval/1000),rangevals*(LAGDrval/1000),510,"+L");	
	CartYaxis->SetTitle("LMG Residual [um]");
	CartYaxis->SetLabelFont(42);
	CartYaxis->SetLabelSize(0.035);
	CartYaxis->SetTitleSize(0.035);
	CartYaxis->SetTitleOffset(1.5);
	
	CartYaxis->Draw();	
	h->Draw();

	cout << "Even SM Positive side fit function coefficients: " << f2->GetParameter(0) << ", " << f2->GetParameter(1) << ", " << f2->GetParameter(2) << ", " << f2->GetParameter(3) << ", " << f2->GetParameter(4) << endl;
	cout << "Even SM Negative side fit function coefficients: " << f3->GetParameter(0) << ", " << f3->GetParameter(1) << ", " << f3->GetParameter(2) << ", " << f3->GetParameter(3) << ", " << f3->GetParameter(4) << ", " << f3->GetParameter(5) << endl;
	//return;
	
	//h->SaveAs("EvenOddResidualcorrections.png");
	
	//auto Poffset = f4->GetParameter(0);
	//auto PAmp = f4->GetParameter(1);
	//auto topcoef = Poffset*PAmp;
	
	//cout << "Function is: " << topcoef << "x/(x^2 + " << Poffset << "^2)" << endl;


	//}

	///if(SM == 2){
	//h->cd(1);
	//LAGDResPerEta->Draw("AP");

	//h->cd(2);
	 

	//f4 = new TF1("line24","[0]*exp([1] + [2]*x)", 0, 0.5);
	//f5 = new TF1("line25","[0]*exp([1] + [2]*x)", -0.5, 0);
	/*
	LAGDResPerEtaProf->Fit("line24", "R");
	LAGDResPerEtaProf->Fit("line25", "R+");

	cout << "Left Function is: " << f5->GetParameter(6) << "x^6 + "<< f5->GetParameter(5) << "x^5 + "<< f5->GetParameter(4) << "x^4 + "<< f5->GetParameter(3) << "x^3 + "<< f5->GetParameter(2) << "x^2 + "<< f5->GetParameter(1) << "x + "<< f5->GetParameter(0) << endl;
	cout << "Right Function is: " << f4->GetParameter(6) << "x^6 + "<< f4->GetParameter(5) << "x^5 + "<< f4->GetParameter(4) << "x^4 + "<< f4->GetParameter(3) << "x^3 + "<< f4->GetParameter(2) << "x^2 + "<< f4->GetParameter(1) << "x + "<< f4->GetParameter(0) << endl;
	
	//cout << "Right side fit eqn: " << f4->GetParameter(0) << "e^(" << f4->GetParameter(1) << "+" << f4->GetParameter(2) << "x)" << endl;
	//cout << "Left side fit eqn: " << f5->GetParameter(0) << "e^(" << f5->GetParameter(1) << "+" << f5->GetParameter(2) << "x)" << endl;	
	LAGDResPerEta->SetTitle("LAGD Residual vs #eta in FZZ SM:3;#eta;LAGD Residual [um]");
	LAGDResPerEtaProf->Draw();
	
	h->SaveAs("SM2_residualcorrection.png");
        */
	//}

  
	return;
      }
      //TrackErrorsAtLAGD->Draw();
      /*
      auto qc = new TCanvas("","", 1000, 500);
      qc->Divide(2,1);
      qc->cd(1);
      incresidualLAGD->Draw();
      qc->cd(2);
      residualLAGD->Draw();
      qc->Draw();
      return;
      */
      
      	
   
	

      bool thispart = false;
      if(thispart){ 
	vector<float>  uniquehists;
	for(auto k = Modulusvals.begin(); k < Modulusvals.end(); k++){
	  cout << *k << endl;
	  uniquehists.push_back(*k);
	}
	cout << binwidthx << endl;

      
	vector<float> Xvalues;
	vector<float> XvalueWidths;
	vector<float> LAGDResstddev;
	vector<float> LAGDResstddevErrors;

	vector<float> Yvalues;
	vector<float> YvalueWidths;
	vector<float> YLAGDResstddev;
	vector<float> YLAGDResstddevErrors;
      
	for(auto k = uniquehists.begin(); k < uniquehists.end(); k++){
	  // if(*k == 0){continue;}
	  // auto z = new TCanvas("", "", 1000, 500);       
	  // string canvtitle = "LAGD Residuals from ";
	  // canvtitle += to_string(minx + *k*binwidthx);
	  // canvtitle += " to ";
	  // canvtitle += to_string(minx + (*k+1)*binwidthx);
	  // char canvtitles[canvtitle.size()+1];
	  // strcpy(canvtitles, canvtitle.c_str());

	  // z->Divide(2,1);
	  // z->cd(1);
	  // LAGDResXHist[*k]->Draw();
	  // LAGDResXHist[*k]->SetTitle(canvtitles);
	  // LAGDResXHist[*k]->GetXaxis()->SetTitle("LAGD Residual");
	  // LAGDResXHist[*k]->GetYaxis()->SetTitle("Frequency");

	  // z->cd(2);
	  // LAGDResYHist[*k]->Draw();
	  // LAGDResYHist[*k]->SetTitle(canvtitles);
	  // LAGDResYHist[*k]->GetXaxis()->SetTitle("LAGD Residual");
	  // LAGDResYHist[*k]->GetYaxis()->SetTitle("Frequency");

	  // canvtitle += ".png";
	  //z->SaveAs((canvtitle).c_str());
	

	  //z->Draw();
	
	  cout << "Hist num: " << *k << endl;
	  cout << "Hist Std Dev: " << LAGDResXHist[*k]->GetStdDev() << endl;
	  cout << "Hist Mean: " << LAGDResXHist[*k]->GetMean() << endl;


	  float xhistmean=0;
	  float xhistsigma=0;
	  float xhistsigmaerror=0;
	  float yhistmean=0;
	  float yhistsigma=0;
	  float yhistsigmaerror=0;
	  cout << LAGDResXHist[*k]->GetEntries() << endl;
	
	  // if(LAGDResXHist[*k]->GetEntries() >= 3){
	  
	  //   I2GFvalues myValues;
	  
	  //   myValues = I2GFmainLoop(LAGDResXHist[*k],1,5,1);  auto xhistmean = myValues.mean; auto xhistsigma=myValues.sigma; auto xhistsigmaerror=myValues.sigma_err;
	  
	  //   myValues = I2GFmainLoop(LAGDResYHist[*k],1,5,1);  auto yhistmean = myValues.mean; auto yhistsigma=myValues.sigma; auto yhistsigmaerror=myValues.sigma_err;

	  // }
	  // else{
	  //   xhistmean=LAGDResXHist[*k]->GetMean();
	  //   xhistsigma=LAGDResXHist[*k]->GetStdDev();
	  //   xhistsigmaerror=LAGDResXHist[*k]->GetStdDevError();
	  //   yhistmean=LAGDResYHist[*k]->GetMean();
	  //   yhistsigma=LAGDResYHist[*k]->GetStdDev();
	  //   yhistsigmaerror=LAGDResYHist[*k]->GetStdDevError();
	  
	  // }

	  I2GFvalues myValues;
	
	  myValues = I2GFmainLoop(LAGDResXHist[*k],1,5,1);  xhistmean = myValues.mean; xhistsigma=myValues.sigma; xhistsigmaerror=myValues.sigma_err;
	
	  myValues = I2GFmainLoop(LAGDResYHist[*k],1,5,1);  yhistmean = myValues.mean; yhistsigma=myValues.sigma; yhistsigmaerror=myValues.sigma_err;

	  //LAGDResXHist[*k]->Draw();
	  cout << "XHist double gaus Std Dev: " << xhistsigma << "+/- " << xhistsigmaerror << endl;
	  cout << "YHist double gaus Std Dev: " << yhistsigma << "+/- " << yhistsigmaerror << endl;
	  //return;

	  // if(xhistsigma > 1000){xhistsigma = LAGDResXHist[*k]->GetStdDev();}
	  // if(xhistsigmaerror > 1000){xhistsigma = LAGDResXHist[*k]->GetStdDev(); xhistsigmaerror = LAGDResXHist[*k]->GetStdDevError();}
	  // if(yhistsigma > 1000){yhistsigma = LAGDResYHist[*k]->GetStdDev();}
	  // if(yhistsigmaerror > 1000){yhistsigma = LAGDResYHist[*k]->GetStdDev(); yhistsigmaerror = LAGDResYHist[*k]->GetStdDevError();}
	
	
	  Xvalues.push_back(minx + *k*binwidthx);
	  XvalueWidths.push_back(binwidthx/2);
	  LAGDResstddev.push_back(xhistsigma);
	  LAGDResstddevErrors.push_back(xhistsigmaerror/2);

	  Yvalues.push_back(miny + *k*binwidthy);
	  YvalueWidths.push_back(binwidthy/2);
	  YLAGDResstddev.push_back(yhistsigma);
	  YLAGDResstddevErrors.push_back(yhistsigmaerror/2);
	
	  //y->SetPoint(*k, minx + *k*binwidth, LAGDResXHist[*k]->GetStdDev());
	
	  //y->SetPoint(*k, minx + *k*binwidth, histsigma);
	
	}

	float Xvaluess[Xvalues.size()];
	std::copy(Xvalues.begin(), Xvalues.end(), Xvaluess);
	
	float XvalueWidthss[XvalueWidths.size()];
	std::copy(XvalueWidths.begin(), XvalueWidths.end(), XvalueWidthss);

	float LAGDResstddevs[LAGDResstddev.size()];
	std::copy(LAGDResstddev.begin(), LAGDResstddev.end(), LAGDResstddevs);

	float LAGDResstddevErrorss[LAGDResstddevErrors.size()];
	std::copy(LAGDResstddevErrors.begin(), LAGDResstddevErrors.end(), LAGDResstddevErrorss);

	float Yvaluess[Yvalues.size()];
	std::copy(Yvalues.begin(), Yvalues.end(), Yvaluess);
	
	float YvalueWidthss[YvalueWidths.size()];
	std::copy(YvalueWidths.begin(), YvalueWidths.end(), YvalueWidthss);

	float YLAGDResstddevs[YLAGDResstddev.size()];
	std::copy(YLAGDResstddev.begin(), YLAGDResstddev.end(), YLAGDResstddevs);

	float YLAGDResstddevErrorss[YLAGDResstddevErrors.size()];
	std::copy(YLAGDResstddevErrors.begin(), YLAGDResstddevErrors.end(), YLAGDResstddevErrorss);

	auto y = new TGraphErrors(Xvalues.size(), Xvaluess, LAGDResstddevs, XvalueWidthss, LAGDResstddevErrorss);
	auto yy = new TGraphErrors(Yvalues.size(), Yvaluess, YLAGDResstddevs, YvalueWidthss, YLAGDResstddevErrorss);


	int Yminhere = 0;
	int Ymaxhere = 5000;
      
	auto i = new TCanvas("", "", 1000, 500);
	i->Divide(2,1);
	i->cd(1);
	y->Draw("AP");
	y->GetXaxis()->SetRangeUser(minx-1, maxx+1);
	y->GetYaxis()->SetRangeUser(Yminhere, Ymaxhere);       
	y->SetTitle(("LAGD Y Residual width per "+to_string(double(binwidthx))+" mm chunk of Beamspot X coordinate;X coordinate [mm]; LAGD Residual width [urad]").c_str());

	auto CartXaxis = new TGaxis(maxx+1,Yminhere,maxx+1,Ymaxhere,Yminhere*(LAGDrval/1000),Ymaxhere*(LAGDrval/1000),510,"+L");	
	CartXaxis->SetTitle("LAGD Residual Width [um]");
	CartXaxis->SetLabelFont(42);
	CartXaxis->SetLabelSize(0.035);
	CartXaxis->SetTitleSize(0.035);
	CartXaxis->SetTitleOffset(1.5);
      
	CartXaxis->Draw();
	i->cd(2);
	yy->Draw("AP");
	yy->GetXaxis()->SetRangeUser(miny-1, maxy+1);
	yy->GetYaxis()->SetRangeUser(Yminhere, Ymaxhere);             
	yy->SetTitle(("LAGD Y Residual width per "+to_string(double(binwidthy))+" mm chunk of Beamspot Y coordinate;Y coordinate [mm]; LAGD Residual width [urad]").c_str());
	auto CartYaxis = new TGaxis(maxy+1,Yminhere,maxy+1,Ymaxhere,Yminhere*(LAGDrval/1000),Ymaxhere*(LAGDrval/1000),510,"+L");	
	CartYaxis->SetTitle("LAGD Residual Width [um]");
	CartYaxis->SetLabelFont(42);
	CartYaxis->SetLabelSize(0.035);
	CartYaxis->SetTitleSize(0.035);
	CartYaxis->SetTitleOffset(1.5);
      
	CartYaxis->Draw();
      
	i->SaveAs("LAGDXYResidualWidthPer5mmXchunk.png");
      
	//break;
	//histograms of residuals are now filled, time to find the means of these hisograms and do further analysis
	//lets look at some of these histograms shall we:

	//CartesianSizeDiffperRadius->SetPoint(cartit, LAGDRfromTrackers->GetMean(), (LAngDist->GetStdDev()/1000)*(LAGDRfromTrackers->GetMean()) - avesize);
	//cartit++;
	//residualLAGD->Draw();


	return;
      }

      //ResPerAngle->Draw("AP");
      I2GFvalues myValues;
	
      myValues = I2GFmainLoop(residualREF1X,1,5,1);  meanREF1X = myValues.mean; sigmaREF1X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF1Y,1,5,1);  meanREF1Y = myValues.mean; sigmaREF1Y=myValues.sigma;
      myValues = I2GFmainLoop(residualREF2X,1,5,1);  meanREF2X = myValues.mean; sigmaREF2X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF2Y,1,5,1);  meanREF2Y = myValues.mean; sigmaREF2Y=myValues.sigma;
      myValues = I2GFmainLoop(residualREF3X,1,5,1);  meanREF3X = myValues.mean; sigmaREF3X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF3Y,1,5,1);  meanREF3Y = myValues.mean; sigmaREF3Y=myValues.sigma;
      myValues = I2GFmainLoop(residualREF4X,1,5,1);  meanREF4X = myValues.mean; sigmaREF4X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF4Y,1,5,1);  meanREF4Y = myValues.mean; sigmaREF4Y=myValues.sigma;
      myValues = I2GFmainLoop(residualLAGD,1,5,1);  meanLAGD = myValues.mean; sigmaLAGD=myValues.sigma;
      auto sigmaLAGDsigma = myValues.sigma_err;
      //LAGDResM2->Draw();

      
      //sigmaLAGD = residualLAGD->GetStdDev();
      if(sigmaLAGD < 1){
	residualLAGD->Draw();
	return;
      }
      
      // auto u = new TCanvas("", "", 500, 500);	 
      // residualLAGD->Draw();
      // u->Draw();

      
      // if(abs(sigmaLAGD) >= 4000){
      // 	cout << sigmaLAGD << endl;
      // 	cout << residualLAGD->GetStdDev() << endl;
      // 	auto u = new TCanvas("", "", 500, 500);	 
      // 	residualLAGD->Draw();
      // 	u->Draw();
      // 	break;
      // }

      // if(sigmaLAGD > 4000){
      // 	cout << residualLAGD->GetStdDev() << endl;
      // 	sigmaLAGD = residualLAGD->GetStdDev();
      // }
      
      
      cout << "Exclusive LAGD mean Res and width: " << meanLAGD << ", " << sigmaLAGD << endl;
      cout << "Difference in size of hitspots (Theo - Exp): " << (AvgAngDist->GetStdDev() - LAngDist->GetStdDev()) << endl;
      cout << "Efficiency for this data: " << EventsthatPass/totalEvents << " +/- " << (1/totalEvents)*sqrt(EventsthatPass*(1-(EventsthatPass/totalEvents))) << endl;


      // string y = to_string(Xoffset);
      // char cart1[y.size()];
      // strcpy(cart1, y.c_str());
      // auto t = new TCanvas(cart1, "", 500, 500);
      // residualLAGD->Draw();
      // t->Draw();
      
      // canv->SaveAs((y+"_Resdist.png").c_str());//Draw();
      //return;
      
      if(finalRes){

	// auto h = new TCanvas("", "", 2000, 1000);
	// h->Divide(2,2);
	// h->cd(1);
	// Stripcorr->Draw("colz");//AP
	// //Stripcorr->SetNpx(200);
	// //Stripcorr->SetNpx(200);      
	// Stripcorr->SetTitle("Correlation between Y coordinate of LAGD and Tracker Hits");
	// Stripcorr->GetYaxis()->SetTitle("LAGD hit location [mRad]");
	// Stripcorr->GetXaxis()->SetTitle("Tracker 1y hit location [mRad]");
    
	// h->cd(2);
	// TrackerStripcorr->Draw("colz");//AP
	// //TrackerStripcorr->SetNpx(200);
	// //TrackerStripcorr->SetNpx(200);
	// //gPad->SetTheta(90); // default is 30
	// //gPad->SetPhi(0); // default is 30
	// //gPad->Update();
	// TrackerStripcorr->SetTitle("Correlation between Y coordinate of 2 Tracker's Hits");
	// TrackerStripcorr->GetXaxis()->SetTitle("Tracker 1 Y hit location [mRad]");
	// TrackerStripcorr->GetYaxis()->SetTitle("Tracker 3 Y hit location [mRad]");

	// h->cd(3);
	// StripcorrProf->Draw();

	// h->cd(4);
	// TrackerStripcorrProf->Draw();
      
	// h->Draw();
	//gStyle->SetOptStat(1111);

	int plotlim = 3000;
	auto ResPercor = new TCanvas("","", 1000, 1500);
	ResPercor->Divide(2,2);
	ResPercor->cd(1);
	LAGDResPerX2D->Draw();
	//cout << LAGDResPerX->GetN() << endl;;
	LAGDResPerX2D->SetTitle("LMG Y Residual per Hit X coordinate");
	LAGDResPerX2D->GetXaxis()->SetTitle("Hit X coordinate [mm]");
	LAGDResPerX2D->GetYaxis()->SetTitle("LMG Y Residual [urad]");
	LAGDResPerX2D->GetYaxis()->SetRangeUser(-plotlim, plotlim);	
	auto Xaxis2dplot = new TGaxis(maxx,-plotlim,maxx,plotlim,-plotlim*(LAGDrval/1000),plotlim*(LAGDrval/1000),510,"+L");	
	Xaxis2dplot->SetTitle("LMG Residual [um]");
	Xaxis2dplot->SetLabelFont(42);
	Xaxis2dplot->SetLabelSize(0.035);
	Xaxis2dplot->SetTitleSize(0.035);
	Xaxis2dplot->SetTitleOffset(1.5);
	
	Xaxis2dplot->Draw();

	
	ResPercor->cd(2);
	LAGDResPerY2D->Draw();
	//cout << LAGDResPerY->GetN() << endl;
	LAGDResPerY2D->SetTitle("LMG Y Residual per Hit Y coordinate");
	LAGDResPerY2D->GetXaxis()->SetTitle("Hit Y coordinate [mm]");
	LAGDResPerY2D->GetYaxis()->SetTitle("LMG Y Residual [urad]");
	LAGDResPerY2D->GetYaxis()->SetRangeUser(-plotlim, plotlim);
	auto Yaxis2dplot = new TGaxis(maxy,-plotlim,maxy,plotlim,-plotlim*(LAGDrval/1000),plotlim*(LAGDrval/1000),510,"+L");	
	Yaxis2dplot->SetTitle("LMG Residual [um]");
	Yaxis2dplot->SetLabelFont(42);
	Yaxis2dplot->SetLabelSize(0.035);
	Yaxis2dplot->SetTitleSize(0.035);
	Yaxis2dplot->SetTitleOffset(1.5);	
	Yaxis2dplot->Draw();

	// TF1* fzig = new TF1("linezig","[0]*acos(cos([3]*x-[1]))+[2]", 5,10);
	// fzig->SetParameter(0,);
	// fzig->SetParameter(1,0.3097);
	// LAGDResPerY2D->Fit("linezig", "R");
	
	ResPercor->cd(3);
	LAGDResPerXProf->Draw();
	LAGDResPerXProf->GetYaxis()->SetRangeUser(-plotlim, plotlim);	
	auto ProfXaxis = new TGaxis(maxx,-1600,maxx,1500,-1600*(LAGDrval/1000),1500*(LAGDrval/1000),510,"+L");	
	ProfXaxis->SetTitle("LMG Residual [um]");
	ProfXaxis->SetLabelFont(42);
	ProfXaxis->SetLabelSize(0.035);
	ProfXaxis->SetTitleSize(0.035);
	ProfXaxis->SetTitleOffset(1.5);
	
	ProfXaxis->Draw();
	
	ResPercor->cd(4);
	LAGDResPerYProf->Draw();
	LAGDResPerYProf->GetYaxis()->SetRangeUser(-plotlim, plotlim);	
	auto ProfYaxis = new TGaxis(maxy,-1600,maxy,1500,-1600*(LAGDrval/1000),1500*(LAGDrval/1000),510,"+L");	
	ProfYaxis->SetTitle("LMG Residual [um]");
	ProfYaxis->SetLabelFont(42);
	ProfYaxis->SetLabelSize(0.035);
	ProfYaxis->SetTitleSize(0.035);
	ProfYaxis->SetTitleOffset(1.5);
	
	ProfYaxis->Draw();
		

	ResPercor->Draw();
	//return;
	
	// vector<float>  Stds;
	// vector<float>  Xvals;
	// vector<float>  errors;
	// vector<float>  yerrors;
	
	// for(uint o = 0; o < binnum; o++){
	//   cout << LAGDResPerXProf->GetBinContent(o+1) << " +/- " << LAGDResPerXProf->GetBinError(o+1) << endl;	  
	//   Stds.push_back(LAGDResPerXProf->GetBinError(o+1));
	//   Xvals.push_back(minx + (o)*binwidthx);
	//   errors.push_back(binwidthx/2);
	//   yerrors.push_back(0);
	// }

	// float Stdss[Stds.size()];
	// std::copy(Stds.begin(), Stds.end(), Stdss);
	// float Xvalss[Xvals.size()];
	// std::copy(Xvals.begin(), Xvals.end(), Xvalss);
	// float errorss[errors.size()];
	// std::copy(errors.begin(), errors.end(), errorss);
	// float yerrorss[yerrors.size()];
	// std::copy(yerrors.begin(), yerrors.end(), yerrorss);

	// auto ResidualStdDevPerX = new TGraphErrors(binnum, Xvalss, Stdss, errorss, yerrorss);

	// Stds.clear();
	// Xvals.clear();

	// vector<float>  Ycoorerrors;
	// vector<float>  Ycooryerrors;
	
	// for(uint o = 0; o < binnum; o++){
	//   //cout << LAGDResPerXProf->GetBinError(o) << endl;
	//   Stds.push_back(LAGDResPerYProf->GetBinError(o+1));
	//   Xvals.push_back(miny + (o)*binwidthy);
	//   Ycoorerrors.push_back(binwidthy/2);
	//   Ycooryerrors.push_back(0);	  
	// }
	
	
	// float Stdsx[Stds.size()];
	// std::copy(Stds.begin(), Stds.end(), Stdsx);
	// float Xvalsx[Xvals.size()];
	// std::copy(Xvals.begin(), Xvals.end(), Xvalsx);
	// float Ycoorerrorss[Ycoorerrors.size()];
	// std::copy(Ycoorerrors.begin(), Ycoorerrors.end(), Ycoorerrorss);
	// float Ycooryerrorss[Ycooryerrors.size()];
	// std::copy(Ycooryerrors.begin(), Ycooryerrors.end(), Ycooryerrorss);

	// auto ResidualStdDevPerY = new TGraphErrors(binnum, Xvalsx, Stdsx, Ycoorerrorss, Ycooryerrorss);


	// auto ResPerCorStdDEv = new TCanvas("", "", 1000, 500);
	// ResPerCorStdDEv->Divide(2,1);
	// ResPerCorStdDEv->cd(1);
	// ResidualStdDevPerX->Draw("A*");
	// ResidualStdDevPerX->SetTitle(("Std Dev of LAGD Residual per "+to_string(binwidthx)+" mm in FZZ sector;Hit X Coordinate [mm]; LAGD Std Dev of Residual [urad]").c_str());
	// ResPerCorStdDEv->cd(2);
	// ResidualStdDevPerY->Draw("A*");
	// ResidualStdDevPerY->SetTitle(("Std Dev of LAGD Residual per "+to_string(binwidthy)+" mm in FZZ sector;Hit Y Coordinate [mm]; LAGD Std Dev of Residual [urad]").c_str());

	// ResPerCorStdDEv->Draw();
	
	//return;

	
	//return;
	//cout << "Inclusive LAGD mean Res and width: " << incmeanLAGD << ", " << incsigmaLAGD << endl;
	//cout << "Tracker error mean and width: " << Trackerrormean << ", " << Trackerrorsigma << endl;

	// auto qc = new TCanvas("","", 500, 500);
	// TrackErrorsAtLAGD->Draw();
	
	// qc->Draw();
	// return;
	auto h = new TCanvas("", "", 500, 500);	
	auto Trackererrorsigma = TrackErrorsAtLAGD->GetMeanError();
	TF1* f3 = new TF1("line1","landau",0, 2);
	TrackErrorsAtLAGD->Fit("line1", "R");
	 
	auto TrackerAvgerror = f3->GetParameter(1);//TrackErrorsAtLAGD->GetMean();

	//TF1* fn = new TF1("line2","landau",0, 1500);
	//LAGDResM2->Fit("line2", "R");
	//cout << fn->GetParameter(1) << endl;
	cout << sigmaLAGD << ", " << TrackerAvgerror*1000 << endl;
	LAGDResolution = sqrt(sigmaLAGD*sigmaLAGD - (TrackerAvgerror*1000)*(TrackerAvgerror*1000));
	cout << "Angle: " << LAGDanglehere << endl;
	auto LAGDResError = (1/LAGDResolution)*sqrt(sigmaLAGD*sigmaLAGD*sigmaLAGDsigma*sigmaLAGDsigma + (TrackerAvgerror*1000)*(TrackerAvgerror*1000)*(Trackererrorsigma*1000)*(Trackererrorsigma*1000));
	  
	cout << "Final LAGD Resoulution in this location: " << LAGDResolution << " +/- " << LAGDResError << " urad (" << (LAGDrval/1000)*LAGDResolution << " +/- " << (LAGDrval/1000)*LAGDResError << " um)" <<endl;
	
	//cout << "Final LAGD Resoulution in this location from M2 (Histogram Individual Resolutions): " << fn->GetParameter(1) << " urad +/- " << fn->GetParError(1) << "(" << (LAGDrval/1000)*fn->GetParameter(1) << " um +/- " << (LAGDrval/1000)*fn->GetParError(1) << "um )" << endl;

	//cout << "Final LAGD Resoulution in this location from M3 (Histogram TE and remove from LAGD res width at the end): " << LAGDResM2->GetMean() << " urad +/- " << LAGDResM2->GetMeanError() << "(" << (LAGDrval/1000)*LAGDResM2->GetMean() << " um +/- " << (LAGDrval/1000)*LAGDResM2->GetMeanError() << "um )" << endl;
	//auto qc = new TCanvas("","", 500, 500);
	//qc->Divide(2,1);
	//qc->cd(1);
	
	// LAGDResM2->Draw();
	// TF1* f3 = new TF1("line1","landau",0, 1500);
	// LAGDResM2->Fit("line1", "R");
	// cout << f3->GetParameter(1) << endl;
	
	// qc->Draw();
	// return;
	
	/*
	//cout << "Final LAGD Resoulution in this location from M3 (Geometric Mean of Inclusive and Exclusive Residuals): " << LAGDResM3->GetMean() << " mRad +/- " << LAGDResM3->GetMeanError() << endl;

	auto qc = new TCanvas("","", 1000, 500);
	qc->Divide(2,1);
	qc->cd(1);
	LAGDResM2->Draw();
	qc->cd(2);
	LAGDResM3->Draw();
	qc->Draw();
	//return;
	*/
      }
      //if(Yoffset == 12)
      //residualLAGD->Draw();
      
      
    
      
      //if(Xoffset == 0){ 
      //ResSigmaVsXoffset->SetPoint(ressigmait, Xoffset, sigmaLAGD);
      //ressigmait++;
     /*
      auto radialhists = new TCanvas("", "Radial Residual Histrogams", 1500, 1000);
      
      radialhists->Divide(3,2);
      radialhists->cd(1);
      residualREF1X->Draw();
      radialhists->cd(2);
      residualREF2X->Draw();
      radialhists->cd(3);
      residualREF3X->Draw();
      radialhists->cd(4);
      residualREF4X->Draw();
      radialhists->cd(5);
      residualLAGDr->Draw();
     */
      
      if(finalRes){
	string name;
	string title = "Angular residuals for (";

	name += to_string(Yoffset);
	title += to_string(Yoffset);

	name += ", ";
	title += ", ";

	name += to_string(Xoffset);
	title += to_string(Xoffset);

	name += ")";
	title += ")";

	int y = title.length();
	int n = name.length();
	  
	char charname[n];
	char chartit[y];
      
	strcpy(charname, name.c_str());
	strcpy(chartit, title.c_str());


	gStyle->SetOptFit();
	gStyle->SetStatX(0.45);
	gStyle->SetStatW(0.18);	
	gStyle->SetStatY(0.85);
	//gStyle->SetStatH(0.4);
	
	
	int rangevals = 5000;
	auto Angularhists = new TCanvas(charname, chartit, 1500, 1000);
	Angularhists->Divide(2,3);
	Angularhists->cd(1);
	residualREF1Y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	residualREF1Y->Draw();
    
	
	auto T1Maxis = new TGaxis(-rangevals,1.05*(residualREF1Y->GetMaximum()),rangevals,1.05*(residualREF1Y->GetMaximum()),-rangevals*(T1rval/1000),rangevals*(T1rval/1000),510,"-");	
	T1Maxis->SetTitle("[um]");
	T1Maxis->Draw();
	
        
	cout << "Tracker 1 Residual width: " << sigmaREF1Y << endl;
	
	Angularhists->cd(2);
	residualREF2Y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	residualREF2Y->Draw();

	auto T2Maxis = new TGaxis(-rangevals,1.05*(residualREF2Y->GetMaximum()),rangevals,1.05*(residualREF2Y->GetMaximum()),-rangevals*(T2rval/1000),rangevals*(T2rval/1000),510,"-");	
	T2Maxis->SetTitle("[um]");
	T2Maxis->Draw();
	

	cout << "Tracker 2 Residual width: " << sigmaREF2Y << endl;

	Angularhists->cd(3);
	residualREF3Y->GetXaxis()->SetRangeUser(-rangevals, rangevals);		
	residualREF3Y->Draw();

	
	auto T3Maxis = new TGaxis(-rangevals,1.05*(residualREF3Y->GetMaximum()),rangevals,1.05*(residualREF3Y->GetMaximum()),-rangevals*(T3rval/1000),rangevals*(T3rval/1000),510,"-");	
	T3Maxis->SetTitle("[um]");
	T3Maxis->Draw();

	cout << "Tracker 3 Residual width: " << sigmaREF3Y << endl;
	Angularhists->cd(4);
	residualREF4Y->GetXaxis()->SetRangeUser(-rangevals, rangevals);		
	residualREF4Y->Draw();

	
	auto T4Maxis = new TGaxis(-rangevals,1.05*(residualREF4Y->GetMaximum()),rangevals,1.05*(residualREF4Y->GetMaximum()),-rangevals*(T4rval/1000),rangevals*(T4rval/1000),510,"-");	
	T4Maxis->SetTitle("[um]");
	T4Maxis->Draw();

	cout << "Tracker 4 Residual width: " << sigmaREF4Y << endl;
	
	Angularhists->cd(5);
	residualLAGD->GetXaxis()->SetRangeUser(-rangevals, rangevals);		
	residualLAGD->Draw();

	auto LAGDMaxis = new TGaxis(-rangevals,1.05*(residualLAGD->GetMaximum()),rangevals,1.05*(residualLAGD->GetMaximum()),-rangevals*(LAGDrval/1000),rangevals*(LAGDrval/1000),510,"-");	
	LAGDMaxis->SetTitle("[um]");
	LAGDMaxis->Draw();

	string TA1legendentry3 = "Resolution: ";
	TA1legendentry3 += Form("%.2f",LAGDResolution);
	TA1legendentry3 += " urad (";
	TA1legendentry3 += Form("%.2f",(LAGDrval/1000)*LAGDResolution);
	TA1legendentry3 += " um)";
	char TA1charlegendentry3[TA1legendentry3.size()+1];
	strcpy(TA1charlegendentry3, TA1legendentry3.c_str());

	string TA1legendentry5 = "Radius: ";
	TA1legendentry5 += Form("%.2f",LAGDrval);
	TA1legendentry5 += " mm";
	char TA1charlegendentry5[TA1legendentry5.size()+1];
	strcpy(TA1charlegendentry5, TA1legendentry5.c_str());

	TLatex latexTA1;
	latexTA1.SetTextSize(0.035);
	latexTA1.SetTextAlign(13);  //align at top
	latexTA1.SetNDC();
	latexTA1.DrawLatex(0.6,.4,TA1charlegendentry3);
	latexTA1.DrawLatex(0.6,.35,TA1charlegendentry5);
	cout << "LAGD Residual  width: " << sigmaLAGD << endl;

	//Avgtrackerror->Draw();
	
	//cout << "Tracker error: " << Avgtrackerror->GetMean() << endl;

	//cout << "LAGD angular resolution: " << sqrt((residualLAGD->GetStdDev())*(residualLAGD->GetStdDev()) - (Avgtrackerror->GetMean())*(Avgtrackerror->GetMean())) << endl;
	//radialhists->Draw();
	  
	Angularhists->Draw();
	title += "_test.png";
	Angularhists->SaveAs(title.c_str());

	return;
      }
      //continue;
		
      //}
    
      //cout << AvgAngDist->GetStdDev() << ", " << LAngDist->GetStdDev()/1000 << endl;
      if(finalRes == false){
	//OutputFile << "#For APV3E1 " << endl;
	
	if(ft){
	  OutputFile << "#X offset,  Y offset,  LAGD Residual Mean,  LAGD Residual Width,  Difference between Avg Anglestd dev and LAGD angle std dev" << endl;
	  ft = false;
	}
	OutputFile << Xoffset << " " << Yoffset << " " << meanLAGD << " " << sigmaLAGD << " " <<  (AvgAngDist->GetStdDev() - LAngDist->GetStdDev()) << endl;
	
      }
     
      ResMeanVsYoffset->SetPoint(Yiter, Yoffset, meanLAGD);
      
      ResidualMeanPerYoffset.push_back(meanLAGD);
      ResidualRMSPerYoffset.push_back(sigmaLAGD);

      
      // auto test = new TCanvas("yes", "", 500, 500);
      // AvgAngDist->Draw();
      // LAngDist->SetLineColor(2);
      // LAngDist->Draw("same");
      
      // test->Draw();
      // test->SaveAs("BothBeamspots.png");
      
      T1r.clear(); T1phi.clear();
      T2r.clear(); T2phi.clear();
      T3r.clear(); T3phi.clear();
      T4r.clear(); T4phi.clear();
      LAGDphi.clear();

      residualREF1X=NULL; residualREF1Y=NULL;
      residualREF2X=NULL; residualREF2Y=NULL;
      residualREF3X=NULL; residualREF3Y=NULL;
      residualREF4X=NULL; residualREF4Y=NULL;
      residualLAGD=NULL;

      //LAngDist->Clear();
      //TAngDist->Clear();
      //T2AngDist->Clear();
      //T3AngDist->Clear();
      //T4AngDist->Clear();

    }
    
      
    /*
    if(Xoffset == 10){
      ReswidthperYoffset->Draw("A*");
      ReswidthperYoffset->GetXaxis()->SetTitle("Tracker Y offset [mm]");
      ReswidthperYoffset->GetYaxis()->SetTitle("LAGD Residual Width [Rad]");
      return;
    }
    
    ResMeanVsYoffset->Draw("A*");
    ResMeanVsYoffset->GetXaxis()->SetTitle("Tracker Y offset [mm]");
    ResMeanVsYoffset->GetYaxis()->SetTitle("LAGD Mean Residual [Rad]");
    */
    // inclusive
    TF1* f3 = new TF1("line3","pol1",0, Yoffset);
    ResMeanVsYoffset->Fit("line3","Q");
       
    double intercept3 = f3->GetParameter(0);
    double slope3     = f3->GetParameter(1);
    double ZeroResYoffset = -intercept3/slope3;
    cout << "Ideal Y offset: " << ZeroResYoffset << endl;
    //return;
    //cout << ResidualMeanPerYoffset.size() << ", " << ResidualRMSPerYoffset.size() << endl;
    for(uint j=0; j < ResidualMeanPerYoffset.size(); j++){
      ResidualMeanPerYoffset.at(j) = abs(ResidualMeanPerYoffset.at(j));
      ResidualRMSPerYoffset.at(j) = abs(ResidualRMSPerYoffset.at(j));      
    }
    //if(Xoffset >= 1)
    //  return;
    
    float MinimumResidual = *min_element(ResidualMeanPerYoffset.begin(), ResidualMeanPerYoffset.end());
    auto MinimumResidualit = min_element(ResidualMeanPerYoffset.begin(), ResidualMeanPerYoffset.end());
    float MinimumResidualYval = distance(ResidualMeanPerYoffset.begin(), MinimumResidualit);
    float MinimumResWidth = *min_element(ResidualRMSPerYoffset.begin(), ResidualRMSPerYoffset.end());
    float MinimumResMeansWidth = ResidualRMSPerYoffset.at(MinimumResidualYval);
    //cout << "Realistic Y value: " << MinimumResidualYval*Ymulti << endl;
    RealIdealYoffsetVsXoffset->SetPoint(Xiter, Xoffset, ZeroResYoffset);
    
    //cout << "The X offset value is: " << Xoffset << " and the Minimum residual width is: " << MinimumResWidth << endl;
    ResSigmaVsXoffset->SetPoint(Xiter, Xoffset, MinimumResWidth);

    ResidualMeanPerYoffset.clear();
    ResidualRMSPerYoffset.clear();
    /*
    IdealXYoffsets->SetPoint(totalits, Xoffset, ZeroResYoffset);
    delete f3;
    ResMeanVsYoffset=NULL;
    
    //return;
    
    //cout << AveResPerYoffset <<endl;
    //ResVsYoffset->SetPoint(totalits, Yoffset, AveResPerYoffset);
    */
    totalits++;        
  }
  /*
  CartesianSizeDiffperRadius->Draw("A*");
  TF1* fy = new TF1("line3","pol1",0, Yoffset);
  CartesianSizeDiffperRadius->Fit("line3","Q");
       
  double intercept3 = fy->GetParameter(0);
  double slope3     = fy->GetParameter(1);
  double ZeroResYoffset = -intercept3/slope3;
  cout << "Ideal X offset: " << ZeroResYoffset << endl;
  return;
  */
  //ReswidthdiffperXoffset->Draw("A*");
  //TF1* f3 = new TF1("line3","pol1",0, Xoffset);
  //ReswidthdiffperXoffset->Fit("line3","Q");
  
  //double intercept3 = f3->GetParameter(0);
  //double slope3     = f3->GetParameter(1);
  //double ZeroResYoffset = -intercept3/slope3;
  //cout << "Ideal X offset: " << ZeroResYoffset << endl;
  //return;
  auto t = new TCanvas("","", 1000, 500);
  t->Divide(2,1);
  t->cd(1);
  ResSigmaVsXoffset->Draw("A*");
  ResSigmaVsXoffset->GetXaxis()->SetTitle("X offset");
  ResSigmaVsXoffset->GetYaxis()->SetTitle("LAGD Minimum Residual Sigma");
  
  ResSigmaVsXoffset->Fit("pol2");
  TF1* XoffsetFit = new TF1("line3","pol2");
  
  t->cd(2);
  RealIdealYoffsetVsXoffset->Draw("A*");
  RealIdealYoffsetVsXoffset->GetXaxis()->SetTitle("Y offset");
  RealIdealYoffsetVsXoffset->GetYaxis()->SetTitle("LAGD Minimum Residual Mean");
  RealIdealYoffsetVsXoffset->Fit("pol1");
  
  t->SaveAs("AiwuplotsforAlignment.png");
  //IdealXYoffsets->Draw("A*");
  //ResVsYoffset->Draw("A*");
}

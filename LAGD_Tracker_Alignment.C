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
//#include "doubleGausFit_withHistParameter.C"
#include <TLatex.h>
#include <unistd.h>
//#include <gSystem.h>
//#include <gPad.h>
//#include "Optimized_XY_shift.C"



void LAGD_Tracker_Alignment(){

  string Eventhits;
  ifstream TrackingFile("Trackers_Hits_restest_donotuse.txt");
  ifstream LAGDFile("LAGD_hits_restest_donotuse.txt");

  ofstream OutputFile("DataPerXandYoffset_liltest.txt");

  OutputFile << "#For APV3E1" << endl;
  OutputFile << "#X offset,  Y offset,  LAGD Residual Mean,  LAGD Residual Width,  Difference between Avg Angle std dev and LAGD angle std dev" << endl;
  
  //cout << "yes" << endl;
  vector<float> Tracker1x;
  vector<float> Tracker1y;
  
  vector<float> Tracker2x;
  vector<float> Tracker2y;
  vector<float> Tracker3x;
  vector<float> Tracker3y;
  vector<float> Tracker4x;
  vector<float> Tracker4y;
  vector<float> APV3E1;
  
  float sumy=0.0;
  float sumx=0.0;
  auto Tracker1Hitmap = new TH2F("PHT1", "Tracker 2 Beam Spot; X position [Strip]; Y position [Strip]", 50, 0, 50, 50, 0, 50);
  auto HitspotprojX = new TH1F("", "", 100, 0, 50);
  auto HitspotprojY = new TH1F("", "", 100, 0, 50);
  
  //int hitit=0;
  //separate the tracker file of coordinates into their own vectors
  while(getline(TrackingFile, Eventhits)){
    //cout << Eventhits.at(0) << endl;
    
    if(Eventhits.at(0) == '#')
      continue;

    
    stringstream Eventstringstream(Eventhits);
    //cout << Eventhits << endl;
    vector<float> Eventvector(istream_iterator<float>(Eventstringstream),{});

    Tracker1x.push_back(Eventvector.at(0));
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
    Tracker1Hitmap->Fill(Eventvector.at(2), Eventvector.at(3));
    HitspotprojX->Fill(Eventvector.at(0));
    HitspotprojX->Fill(Eventvector.at(2));
    HitspotprojY->Fill(Eventvector.at(1));
    HitspotprojY->Fill(Eventvector.at(3));
    
    //Tracker1Hitmap->Fill(Eventvector.at(4), Eventvector.at(5));
    //Tracker1Hitmap->Fill(Eventvector.at(6), Eventvector.at(7));
    
  }
  //Tracker1Hitmap->Draw("colz");
  float sizeX = HitspotprojX->GetStdDev();
  float sizeY = HitspotprojY->GetStdDev();
  float avesize = (sizeX + sizeY)/2.0;
  cout << "SizeX: " << sizeX << ", " << "SizeY: " << sizeY << ", average size: " << (sizeX + sizeY)/2.0 << endl; 
    
  //return;
  //now do the LAGD hits

  auto LAGDstripDist = new TH1F("", "",64, 0, 64);
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
      LAGDstripDist->Fill(((Eventvector.at(0))));
     
      //cout << Eventvector.at(0) << endl;
    }
  }

  //LAGDstripDist->Draw();
  //return;
  //vectors have been created
  //Step one: find tracker offset (unique to each sector)
  //-- double iteration over Y and X offset
  //-- increment in Y, iterate through all x offsets, convert to r,phi for residual calculations!
  //-- plot res mean vs Yoffset and res sigma vs Xoffset 
  
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
  TH1F* TAngDist = 0;
  TH1F* T2AngDist = 0;
  TH1F* T3AngDist = 0;
  
  TH1F* T4AngDist = 0;
  TH1F* AvgAngDist = 0;  
  TH1F* LAngDist = 0;
  TH1F* rphiDist = 0;
  TH1F* yDist = 0;
  
  
  
  double meanREF2X=0.0, meanREF2Y=0.0, meanREF3X=0.0, meanREF3Y=0.0, meanREF1X=0.0, meanREF1Y=0.0, meanREF4X=0.0, meanREF4Y=0.0, meanLAGD=0.0, meanLAGDr=0.0;
  double sigmaREF2X=0.0,sigmaREF2Y=0.0,sigmaREF3X=0.0,sigmaREF3Y=0.0, sigmaREF1X=0.0,sigmaREF1Y=0.0,sigmaREF4X=0.0,sigmaREF4Y=0.0,sigmaLAGD=0.0,sigmaLAGDr=0.0;
  double meanXChi2=0.0,meanYChi2=0.0, totalChi2=0.0;
  
  //TH1F* xTrackChi2=0;
  //TH1F* yTrackChi2=0;
  
  
  TGraph* ResSigmaVsXoffset = new TGraph();
  TGraph* IdealXYoffsets = new TGraph();
  TGraph* RealIdealYoffsetVsXoffset = new TGraph();
  TGraph* ReswidthperYoffset = new TGraph();

   float Yoffset=0.0;
  float Xoffset=0.0;
  float LAGDAngle = 0.525;//30.1 degrees;
  float Strippitch = 4.1;// 4.1 mRad/strip
    
  float sectorXwidth = 80.0;//120;//look at Amore for these values in mm
  float sectorYwidth = 30.0;//60;

  //cout << -sumx/Tracker1x.size() << ", " << -sumy/Tracker1y.size() << endl;
  //return;
  float LAGDanglehere = LAGDstripDist->GetMean()*Strippitch;
  //cout << LAGDanglehere << endl;
  //return;
  float InitialXoffset = -sumx/Tracker1x.size()+80+45;// add other initial offsets here, including optimized offsets at the end for residual plots
  float InitialYoffset = -sumy/Tracker1y.size()+11;
  cout << InitialXoffset-80.0 << endl;
  cout << InitialYoffset << endl;
  
  int totalits = 0;
  float Xmulti = 5.0;
  float Ymulti = 2.0;
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


  float efficiencynumerator=0.0;
  float tracking_error=0.0;
  auto Avgtrackerror = new TH1F("", "", 100, -0.1, 0.1);
  auto FitR = new TH1F("", "", 4000, 0, 2000);
  auto Hitspot = new TH2F("", "", 100, 0, 50, 100, 0, 50);
  auto Hitspotproj = new TH1F("", "", 100, 0, 50);
  auto ReswidthdiffperXoffset = new TGraph();
  auto AllHitsFitplot = new TGraph();
  int allhitsit=0;
  auto CartesianSizeDiffperRadius = new TGraph();
  int cartit=0;
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

      
      residualREF1X = new TH1F(nameRes1X,"",1000,-0.2,0.2); residualREF1X->SetXTitle("Residual in r [mm]"); residualREF1X->SetYTitle("Frequency");residualREF1X->SetLabelSize(0.05,"XY");residualREF1X->SetTitleSize(0.05,"XY");
      residualREF1Y = new TH1F(nameRes1Y,"",100,-5,5); residualREF1Y->SetXTitle("Residual in Angle [mRad]"); residualREF1Y->SetYTitle("Frequency");residualREF1Y->SetLabelSize(0.05,"XY");residualREF1Y->SetTitleSize(0.05,"XY");
      //residualREF1y0 = new TH1F(nameRes1Y,"",400,-0.05,0.05); residualREFy0->SetXTitle("Residual in Y position [mm]"); residualREFy0->SetYTitle("Frequency");residualREFy0->SetLabelSize(0.05,"XY");residualREFy0->SetTitleSize(0.05,"XY");
      residualREF2X = new TH1F(nameRes2X,"",1000,-0.2,0.2); residualREF2X->SetXTitle("Residual in r [mm]"); residualREF2X->SetYTitle("Frequency");residualREF2X->SetLabelSize(0.05,"XY");residualREF2X->SetTitleSize(0.05,"XY");
      residualREF2Y = new TH1F(nameRes2Y,"",100,-5,5); residualREF2Y->SetXTitle("Residual in Angle [mRad]"); residualREF2Y->SetYTitle("Frequency");residualREF2Y->SetLabelSize(0.05,"XY");residualREF2Y->SetTitleSize(0.05,"XY");
      //residualREF2y0 = new TH1F(nameRes2Y,"",400,-0.05,0.05); residualREF2y0->SetXTitle("Residual in Y position [mm]"); residualREF2y0->SetYTitle("Frequency");residualREF2y0->SetLabelSize(0.05,"XY");residualREF2y0->SetTitleSize(0.05,"XY");
      residualREF3X = new TH1F(nameRes3X,"",1000,-0.2,0.2); residualREF3X->SetXTitle("Residual in r [mm]"); residualREF3X->SetYTitle("Frequency");residualREF3X->SetLabelSize(0.05,"XY");residualREF3X->SetTitleSize(0.05,"XY");
      residualREF3Y = new TH1F(nameRes3Y,"",100,-5,5); residualREF3Y->SetXTitle("Residual in Angle [mRad]"); residualREF3Y->SetYTitle("Frequency");residualREF3Y->SetLabelSize(0.05,"XY");residualREF3Y->SetTitleSize(0.05,"XY");
      residualREF4X = new TH1F(nameRes4X,"",1000,-0.2,0.2); residualREF4X->SetXTitle("Residual in r [mm]"); residualREF4X->SetYTitle("Frequency");residualREF4X->SetLabelSize(0.05,"XY");residualREF4X->SetTitleSize(0.05,"XY");
      residualREF4Y = new TH1F(nameRes4Y,"",100,-5,5); residualREF4Y->SetXTitle("Residual in Angle [mRad]"); residualREF4Y->SetYTitle("Frequency");residualREF4Y->SetLabelSize(0.05,"XY");residualREF4Y->SetTitleSize(0.05,"XY");
      residualLAGD = new TH1F(nameResLAGD,"",100,-500,500); residualLAGD->SetXTitle("Residual in Angle [mRad]"); residualLAGD->SetYTitle("Frequency");residualLAGD->SetLabelSize(0.05,"XY");residualLAGD->SetTitleSize(0.05,"XY");
      residualLAGDr = new TH1F(nameResLAGD,"",1000,-10,10); residualLAGDr->SetXTitle("Residual in r [mm]"); residualLAGDr->SetYTitle("Frequency");residualLAGDr->SetLabelSize(0.05,"XY");residualLAGDr->SetTitleSize(0.05,"XY");
      TAngDist = new TH1F(nameTDist,"",200,-0.3,0.3); TAngDist->SetXTitle("Angle [Rad]"); TAngDist->SetYTitle("Frequency");TAngDist->SetLabelSize(0.05,"XY");TAngDist->SetTitleSize(0.05,"XY");
      T2AngDist = new TH1F(nameT2Dist,"",200,-0.3,0.3); T2AngDist->SetXTitle("Angle [Rad]"); T2AngDist->SetYTitle("Frequency");T2AngDist->SetLabelSize(0.05,"XY");T2AngDist->SetTitleSize(0.05,"XY");
      T3AngDist = new TH1F(nameT3Dist,"",200,-0.3,0.3); T3AngDist->SetXTitle("Angle [Rad]"); T3AngDist->SetYTitle("Frequency");T3AngDist->SetLabelSize(0.05,"XY");T3AngDist->SetTitleSize(0.05,"XY");
      T4AngDist = new TH1F(nameT4Dist,"",200,-0.3,0.3); T4AngDist->SetXTitle("Angle [Rad]"); T4AngDist->SetYTitle("Frequency");T4AngDist->SetLabelSize(0.05,"XY");T4AngDist->SetTitleSize(0.05,"XY");
      AvgAngDist = new TH1F(nameAvgDist,"",200,-400,400); AvgAngDist->SetXTitle("Angle [mRad]"); AvgAngDist->SetYTitle("Frequency");AvgAngDist->SetLabelSize(0.05,"XY");AvgAngDist->SetTitleSize(0.05,"XY");
      LAngDist = new TH1F(nameLDist,"",200,-400,400); LAngDist->SetXTitle("Angle [mRad]"); LAngDist->SetYTitle("Frequency");LAngDist->SetLabelSize(0.05,"XY");LAngDist->SetTitleSize(0.05,"XY");
      rphiDist = new TH1F(nameLDist,"",200,0,50); rphiDist->SetXTitle("r*#phi [mm]"); rphiDist->SetYTitle("Frequency");rphiDist->SetLabelSize(0.05,"XY");rphiDist->SetTitleSize(0.05,"XY");
      yDist = new TH1F(nameLDist,"",200,0,50); yDist->SetXTitle("Average Tracker Y at LAGD location [mm]"); yDist->SetYTitle("Frequency");yDist->SetLabelSize(0.05,"XY");yDist->SetTitleSize(0.05,"XY");
      /*
      char nameChi2X[50]; sprintf(nameChi2X,"XTrackChi2_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      char nameChi2Y[50]; sprintf(nameChi2Y,"YTrackChi2_X_%.1f_Y_%.1f",Xoffset, Yoffset);
      xTrackChi2 = new TH1F(nameChi2X,"Chi square of tracks in X projection",1000,0,0.2); xTrackChi2->SetXTitle("#chi^{2} of track in r"); xTrackChi2->SetYTitle("Frequency");
      xTrackChi2->SetTitleSize(0.04,"XY"); xTrackChi2->SetLabelSize(0.04,"XY");
      yTrackChi2 = new TH1F(nameChi2Y,"Chi square of tracks in Y projection",1000,0,0.00002); yTrackChi2->SetXTitle("#chi^{2} of track in #phi"); yTrackChi2->SetYTitle("Frequency");
      yTrackChi2->SetTitleSize(0.04,"XY"); yTrackChi2->SetLabelSize(0.04,"XY");
      */
      sumx=0.0;
      sumy=0.0;
      float T12AvgAngle = 0.0;
      float T34AvgAngle = 0.0;
      float AvgAngle = 0.0;
      for(unsigned int i=0; i< APV3E1.size(); i++){//iterate through all events in the LAGD hit vector
	/*
	float XrFix = -4.1*cos(0.07056);
	cout << "Xproj: " << XrFix << endl;
	float YrFix = -4.1*sin(0.07056);
	cout << "Yproj: " << YrFix << endl;
	*/
	
	//cout << Tracker1y.at(i) << endl;
	Tracker1xtemp = Tracker1x.at(i) + Xoffset + InitialXoffset;// - XrFix;
	sumx += Tracker1xtemp;
	Tracker1ytemp = Tracker1y.at(i) + Yoffset + InitialYoffset;// - YrFix;
	//cout << Tracker1ytemp << endl;
	Tracker2xtemp = Tracker2x.at(i) + Xoffset + InitialXoffset;// - XrFix;
	Tracker2ytemp = Tracker2y.at(i) + Yoffset + InitialYoffset;// - YrFix;
	//Hitspot->Fill(Tracker2x.at(i), Tracker2y.at(i));
	//Hitspotproj->Fill(Tracker2y.at(i));
	Tracker3xtemp = Tracker3x.at(i) + Xoffset + InitialXoffset;// - XrFix;
	Tracker3ytemp = Tracker3y.at(i) + Yoffset + InitialYoffset;// - YrFix;
	Tracker4xtemp = Tracker4x.at(i) + Xoffset + InitialXoffset;// - XrFix;
	Tracker4ytemp = Tracker4y.at(i) + Yoffset + InitialYoffset;// - YrFix;

	/*
	TGraph* g0 = new TGraph();
	g0->SetPoint(0,1596.4, Tracker1y.at(i));
	g0->SetPoint(1,1541.6, Tracker2y.at(i));
	g0->SetPoint(2,54.8, Tracker3y.at(i));
	g0->SetPoint(3,0, Tracker4y.at(i));
	TF1* f0 = new TF1("line1","pol1",0,1600);
	g0->Fit("line1","Q");
	double intercept0 = f0->GetParameter(0);
	double slope0     = f0->GetParameter(1);
	//cout << intercept0 << ", " << slope0 << endl;
	double MeasuredREF1y0 = intercept0 + slope0*1596.4;
	double MeasuredREF2y0 = intercept0 + slope0*1541.6;
	  

	  
	residualREF1y->Fill(MeasuredREF1y0-Tracker1y.at(i));
	residualREF2y->Fill(MeasuredREF2y0-Tracker2y.at(i));
	*/
	//cout << "The current Tracker1 coords are: (" << Tracker1xtemp << ", " << Tracker1ytemp << ")" << endl;

	//cout << Tracker1xtemp << ", " << Tracker1ytemp <<endl;
	float Tracker1R = sqrt(Tracker1xtemp*Tracker1xtemp + Tracker1ytemp*Tracker1ytemp);
	T1r.push_back(Tracker1R);
	float Tracker1Phi = atan(Tracker1ytemp/Tracker1xtemp);
	T1phi.push_back(Tracker1Phi*1000);
	//cout << T1r.at(i) << ", " << T1phi.at(i) << endl;
	TAngDist->Fill(Tracker1Phi);
	T12AvgAngle = Tracker1Phi;
	//cout << Tracker2xtemp << ", " << Tracker2ytemp <<endl;	  
	float Tracker2R = sqrt(Tracker2xtemp*Tracker2xtemp + Tracker2ytemp*Tracker2ytemp);
	T2r.push_back(Tracker2R);
	T2phi.push_back(1000*atan(Tracker2ytemp/Tracker2xtemp));
	T2AngDist->Fill(atan(Tracker2ytemp/Tracker2xtemp));
	T12AvgAngle += atan(Tracker2ytemp/Tracker2xtemp);	
	//cout << T2r.at(i) << ", " << T2phi.at(i) << endl;

	//cout << Tracker3xtemp << ", " << Tracker3ytemp <<endl;	  
	float Tracker3R = sqrt(Tracker3xtemp*Tracker3xtemp + Tracker3ytemp*Tracker3ytemp);
	T3r.push_back(Tracker3R);
	T3phi.push_back(1000*atan(Tracker3ytemp/Tracker3xtemp));
	T3AngDist->Fill(atan(Tracker3ytemp/Tracker3xtemp));
	T34AvgAngle = atan(Tracker3ytemp/Tracker3xtemp);
	//cout << T3r.at(i) << ", " << T3phi.at(i) << endl;

	//cout << Tracker4xtemp << ", " << Tracker4ytemp <<endl;	  		  
	float Tracker4R = sqrt(Tracker4xtemp*Tracker4xtemp + Tracker4ytemp*Tracker4ytemp);
	T4r.push_back(Tracker4R);
	T4phi.push_back(1000*atan(Tracker4ytemp/Tracker4xtemp));
	T4AngDist->Fill(atan(Tracker4ytemp/Tracker4xtemp));
	T34AvgAngle += atan(Tracker4ytemp/Tracker4xtemp);
	//cout << T4r.at(i) << ", " << T4phi.at(i) << endl;

	//cout << "OG value: " << APV3E1.at(i) << endl;
	float LAGDangle = APV3E1.at(i)*Strippitch;
	
	LAGDphi.push_back(LAGDangle);//angle is relative to the middle line (ie, add half of the angle to this afterwards to get the real angle)
	LAngDist->Fill(LAGDangle);       
	//cout << "Angular value: " << LAGDangle << endl;
	//AvgAngle = 1000*((T12AvgAngle/2) + (T34AvgAngle/2))*(828.2/1596);//avg angle at the LAGD location
	
       
	//yDist->Fill((((Tracker1ytemp+Tracker2ytemp)/2.0) + ((Tracker3ytemp+Tracker4ytemp)/2.0))*(828.2/1596.4));
	//both vectors are now in polar coordinates, time to find residuals  

      
	//fill track in r direction
	TGraph* g1 = new TGraph();
	g1->SetPoint(0,1596.4, T1r.at(i));
	g1->SetPoint(1,1541.6, T2r.at(i));
	//g1->SetPoint(2,828.2, ((((T1r.at(i) + T2r.at(i))/2) + ((T3r.at(i) + T4r.at(i))/2))*(828.2/1596))*LAGDphi.at(i));
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
	  
	  
	residualREF1X->Fill(MeasuredREF1X-T1r.at(i));
	residualREF2X->Fill(MeasuredREF2X-T2r.at(i));
	residualREF3X->Fill(MeasuredREF3X-T3r.at(i));
	residualREF4X->Fill(MeasuredREF4X-T4r.at(i));
	//float LAGDr = (((T1r.at(i) + T2r.at(i))/2.0) + ((T3r.at(i) + T4r.at(i))/2.0))*(828.2/1596.4);
	//cout << MeasuredLAGDr << endl;
	FitR->Fill(MeasuredLAGDr);
	//cout << "Ideal Z? " << (-4.1 + LAGDr-intercept1)/slope1 << endl;
	//xTrackChi2->Fill(f1->GetChisquare());
	//totalChi2 += f1->GetChisquare();
	delete f1; delete g1;


	
	
	//residualLAGDr->Fill(MeasuredLAGDr - LAGDr);
	//fill track in phi direction
	TGraph* g2 = new TGraph();

	g2->SetPoint(0,1596.4, T1phi.at(i));
	//AllHitsFitplot->SetPoint(allhitsit,1596.4, 1000*T1phi.at(i));
	//allhitsit++;
 	g2->SetPoint(1,1541.6, T2phi.at(i));
	//AllHitsFitplot->SetPoint(allhitsit,1541.6, 1000*T2phi.at(i));
	//allhitsit++;

	//g2->SetPoint(2,828.2, LAGDphi.at(i));
	//AllHitsFitplot->SetPoint(allhitsit,828.2, LAGDphi.at(i));
	//allhitsit++;
	//g2->SetPoint(3,902, LAGDphi.at(i));
	//g2->SetPoint(4,902, (((T1phi.at(i) + T2phi.at(i))*0.5) + ((T3phi.at(i) + T4phi.at(i))*0.5))*(902.0/1596.0));
	
	g2->SetPoint(2,54.8, T3phi.at(i));
	//AllHitsFitplot->SetPoint(allhitsit,54.8, 1000*T3phi.at(i));
	//allhitsit++;
	
	g2->SetPoint(3,0, T4phi.at(i));
	//AllHitsFitplot->SetPoint(allhitsit,0, 1000*T4phi.at(i));
	//allhitsit++;
	
	//cout << (((T1phi.at(i) + T2phi.at(i))/2.0) + ((T3phi.at(i) + T4phi.at(i))/2.0))*(828.2/1596.4) << endl;
	//cout << T1phi.at(i) << ", " << T2phi.at(i) << ", " << T3phi.at(i) << ", " << T4phi.at(i) << ", " << LAGDphi.at(i) << endl;
	
	  	
	//cout << T1phi.at(i) << ", " << T2phi.at(i) << ", "<< T3phi.at(i) << ", "<< T4phi.at(i) << ", "<< LAGDphi.at(i) << endl;
	//g2->GetXaxis()->SetRangeUser(0,1600);
	

	// inclusive
	TF1* f2 = new TF1("line2","pol1");
	g2->Fit("line2", "Q");
	//  return;
	//if(Yoffset == 8 && Xoffset == 0){
	//g2->Draw("A*");
	// return;
	  //}


	
	double intercept2 = f2->GetParameter(0);
	double slope2     = f2->GetParameter(1);
	double MeasuredREF1Y = intercept2 + slope2*1596.4;
	double MeasuredREF2Y = intercept2 + slope2*1541.6;
	double MeasuredLAGD  = intercept2 + slope2*828.2;//828.2;
	double MeasuredREF3Y = intercept2 + slope2*54.8;
	double MeasuredREF4Y = intercept2 + slope2*0;	  

	AvgAngDist->Fill(MeasuredLAGD);
	//cout << "LAGD angle from fit: " << MeasuredLAGD*1000.0 << endl;
       
	//tracking_error = f2->GetChisquare();
	//cout << tracking_error << endl;
	//Avgtrackerror->Fill(tracking_error);
	/*
	float angres = 0.0455921;
	float upperbound = MeasuredLAGD + 5*angres;//tracking_error;
	float lowerbound = MeasuredLAGD - 5*angres;//tracking_error;

	if(LAGDphi.at(i) <= upperbound && LAGDphi.at(i) >= lowerbound){
	  //cout << lowerbound << " < " << LAGDphi.at(i) << " < " << upperbound << endl;
	  efficiencynumerator++;
	}
	*/
	//cout << LAGDphi.at(i) <<"*" << MeasuredLAGDr << endl;
        //rphiDist->Fill(0.0455*MeasuredLAGDr);
	//return;
	cout << MeasuredLAGD-LAGDphi.at(i) << endl;
	residualREF1Y->Fill((MeasuredREF1Y-T1phi.at(i)));//0.4
	residualREF2Y->Fill((MeasuredREF2Y-T2phi.at(i)));
	residualREF3Y->Fill((MeasuredREF3Y-T3phi.at(i)));
	residualREF4Y->Fill((MeasuredREF4Y-T4phi.at(i)));
	residualLAGD->Fill((MeasuredLAGD-LAGDphi.at(i)));//4.1
	//yTrackChi2->Fill(f2->GetChisquare());
	//totalChi2 += f2->GetChisquare();
	delete f2; delete g2;

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
      //Hitspotproj->Draw();
      //return;
      //AllHitsFitplot->Draw("A*");
      //AllHitsFitplot->Fit("pol1");
      //return;
      //yDist->Draw();
      //rphiDist->Draw();
      //FitR->Draw();
      //return;
      //cout << efficiencynumerator << " / " << Tracker1x.size() << " = " << efficiencynumerator/Tracker1x.size() << endl;
      //cout << "Tracker beamspot location: " << sumx/Tracker1x.size()-80 << ", " << sumy/Tracker1y.size()<< endl;
      //return;
      /*
      auto H = new TCanvas(nameCanv, nameCanv, 500, 500);
      AvgAngDist->Draw();
      AvgAngDist->SetLineColor(3);      
      
      LAngDist->Draw("same");
     
      cout << AvgAngDist->GetStdDev() << " - " << LAngDist->GetStdDev() << " = " << AvgAngDist->GetStdDev() - LAngDist->GetStdDev() << endl;
     
      gPad->Update();
      string titl = nameCanv;
      titl += ".png";
      //H->SaveAs(titl.c_str());
      return;
      */
      /*
      cout << "LAngDist size in mm: " << AvgZideal->GetMean() << "*" << LAngDist->GetStdDev()/1000 << " = " <<  AvgZideal->GetMean()*LAngDist->GetStdDev()/1000.0 << endl;//r*dphi
      cout << "TrackerAngDist size in mm: " << AvgZideal->GetMean() << "*" << AvgAngDist->GetStdDev() << " = " <<  AvgZideal->GetMean()*AvgAngDist->GetStdDev() << endl;//r*dphi
      */
      //if(Yoffset == 9)
      //ReswidthdiffperXoffset->SetPoint(Xiter, Xoffset, AvgAngDist->GetStdDev() - LAngDist->GetStdDev());
      
      /*
      if(Xoffset == 30){
	
	TAngDist->Draw();
	TAngDist->SetLineColor(3);
	T2AngDist->Draw("same");
	T2AngDist->SetLineColor(4);     
	T3AngDist->Draw("same");
	T3AngDist->SetLineColor(5);     
	T4AngDist->Draw("same");
	T4AngDist->SetLineColor(6);     
	
	AvgAngDist->Draw();
	AvgAngDist->SetLineColor(3);
	
	LAngDist->Draw("same");
	gPad->Update();
	H->Draw();
	//return;
	}
      */
      //break;
      //histograms of residuals are now filled, time to find the means of these hisograms and do further analysis
      //lets look at some of these histograms shall we:

      CartesianSizeDiffperRadius->SetPoint(cartit, FitR->GetMean(), (LAngDist->GetStdDev()/1000)*(FitR->GetMean()) - avesize);
      cartit++;
      
      I2GFvalues myValues;
	
      myValues = I2GFmainLoop(residualREF1X,1,10,1);  meanREF1X = myValues.mean; sigmaREF1X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF1Y,1,10,1);  meanREF1Y = myValues.mean; sigmaREF1Y=myValues.sigma;
      myValues = I2GFmainLoop(residualREF2X,1,10,1);  meanREF2X = myValues.mean; sigmaREF2X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF2Y,1,10,1);  meanREF2Y = myValues.mean; sigmaREF2Y=myValues.sigma;
      myValues = I2GFmainLoop(residualREF3X,1,10,1);  meanREF3X = myValues.mean; sigmaREF3X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF3Y,1,10,1);  meanREF3Y = myValues.mean; sigmaREF3Y=myValues.sigma;
      myValues = I2GFmainLoop(residualREF4X,1,10,1);  meanREF4X = myValues.mean; sigmaREF4X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF4Y,1,10,1);  meanREF4Y = myValues.mean; sigmaREF4Y=myValues.sigma;
      myValues = I2GFmainLoop(residualLAGD,1,10,1);  meanLAGD = myValues.mean; sigmaLAGD=myValues.sigma;
      //myValues = I2GFmainLoop(residualLAGDr,1,10,1);  meanLAGDr = myValues.mean; sigmaLAGDr=myValues.sigma;
      
      
      //cout << Xoffset << ", " << Yoffset << endl;

      /*
      meanREF1X = residualREF1X->GetMean(); sigmaREF1X=residualREF1X->GetRMS();
      meanREF1Y = residualREF1Y->GetMean(); sigmaREF1Y=residualREF1Y->GetRMS();
      meanREF2X = residualREF2X->GetMean(); sigmaREF2X=residualREF2X->GetRMS();
      meanREF2Y = residualREF2Y->GetMean(); sigmaREF2Y=residualREF2Y->GetRMS();
      meanREF3X = residualREF3X->GetMean(); sigmaREF3X=residualREF3X->GetRMS();
      meanREF3Y = residualREF3Y->GetMean(); sigmaREF3Y=residualREF3Y->GetRMS();
      meanREF4X = residualREF4X->GetMean(); sigmaREF4X=residualREF4X->GetRMS();
      meanREF4Y = residualREF4Y->GetMean(); sigmaREF4Y=residualREF4Y->GetRMS();
      
      meanLAGD = residualLAGD->GetMean(); sigmaLAGD = residualLAGD->GetRMS();
      */
      cout << "LAGD mean Res and width: " << meanLAGD << ", " << sigmaLAGD << endl;


      //if(Yoffset == 12)
      //residualLAGD->Draw();
      
      
    
      
      //if(Xoffset == 0){ 
      //ResSigmaVsXoffset->SetPoint(ressigmait, Xoffset, sigmaLAGD);
      //ressigmait++;
      
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
	
      
	string name;
	string title = "Angular residuals for (";

	name += to_string(Yoffset);
	title += to_string(Yoffset);

	name += ", ";
	title += ", ";

	name += to_string(Xoffset);
	title += to_string(Xoffset);


	int y = title.length();
	int n = name.length();
	  
	char charname[n];
	char chartit[y];
      
	strcpy(charname, name.c_str());
	strcpy(chartit, title.c_str());
	
	auto Angularhists = new TCanvas(charname, chartit, 1500, 1000);
	Angularhists->Divide(2,3);
	Angularhists->cd(1);
	residualREF1Y->Draw();
	cout << "Tracker 1 angular width: " << sigmaREF1Y << endl;
	Angularhists->cd(2);
	residualREF2Y->Draw();
	cout << "Tracker 2 angular width: " << sigmaREF2Y << endl;
	Angularhists->cd(3);
	residualREF3Y->Draw();
	cout << "Tracker 3 angular width: " << sigmaREF3Y << endl;
	Angularhists->cd(4);
	residualREF4Y->Draw();
	cout << "Tracker 4 angular width: " << sigmaREF4Y << endl;
	Angularhists->cd(5);
	residualLAGD->Draw();
	
	
	cout << "Res width: " << residualLAGD->GetStdDev() << endl;

	//Avgtrackerror->Draw();
	
	cout << "Tracker error: " << Avgtrackerror->GetMean() << endl;

	cout << "LAGD angular resolution: " << sqrt((residualLAGD->GetStdDev())*(residualLAGD->GetStdDev()) - (Avgtrackerror->GetMean())*(Avgtrackerror->GetMean())) << endl;
	//radialhists->Draw();
	  
	Angularhists->Draw();
	title += ".png";
	Angularhists->SaveAs(title.c_str());

	return;
      
	//continue;
		
	//}
      
      cout << AvgAngDist->GetStdDev() << ", " << LAngDist->GetStdDev()/1000 << endl;
      OutputFile << Xoffset << " " << Yoffset << " " << meanLAGD << " " << sigmaLAGD << " " <<  (AvgAngDist->GetStdDev() - LAngDist->GetStdDev()) << endl;

      
      //if(Xoffset == 10)
      //	ReswidthperYoffset->SetPoint(Yiter, Yoffset, sigmaLAGD);

      ResMeanVsYoffset->SetPoint(Yiter, Yoffset, meanLAGD);
      
      ResidualMeanPerYoffset.push_back(meanLAGD);
      ResidualRMSPerYoffset.push_back(sigmaLAGD);
      
      
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

      LAngDist->Clear();
      TAngDist->Clear();
      T2AngDist->Clear();
      T3AngDist->Clear();
      T4AngDist->Clear();

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
    
    cout << "The X offset value is: " << Xoffset << " and the Minimum residual width is: " << MinimumResWidth << endl;
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
  CartesianSizeDiffperRadius->Draw("A*");
  TF1* fy = new TF1("line3","pol1",0, Yoffset);
  CartesianSizeDiffperRadius->Fit("line3","Q");
       
  double intercept3 = fy->GetParameter(0);
  double slope3     = fy->GetParameter(1);
  double ZeroResYoffset = -intercept3/slope3;
  cout << "Ideal X offset: " << ZeroResYoffset << endl;
  return;
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
  ResSigmaVsXoffset->Fit("pol2");
  t->cd(2);
  RealIdealYoffsetVsXoffset->Draw("A*");
  RealIdealYoffsetVsXoffset->Fit("pol1");
  
  t->SaveAs("AiwuplotsforAlignment.png");
  //IdealXYoffsets->Draw("A*");
  //ResVsYoffset->Draw("A*");
}

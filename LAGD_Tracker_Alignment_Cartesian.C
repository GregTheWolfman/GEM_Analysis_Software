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



void LAGD_Tracker_Alignment_Cartesian(){

  string Eventhits;
  ifstream TrackingFile("Trackers_Hits_swappedtrackers.txt");
  ifstream LAGDFile("LAGD_hits_swappedtrackers.txt");

  //ofstream OutputFile("DataPerXandYoffset_quicktest.txt");

  //OutputFile << "#For APV3E1" << endl;
  //OutputFile << "#X offset,  Y offset,  LAGD Residual Mean,  LAGD Residual Width,  Difference between Avg Angle std dev and LAGD angle std dev" << endl;
  
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
  auto HitspotprojX = new TH1F("", "", 500, 0, 50);
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
    HitspotprojX->Fill(Eventvector.at(1));
    //HitspotprojX->Fill(Eventvector.at(2));
    //HitspotprojY->Fill(Eventvector.at(1));
    //HitspotprojY->Fill(Eventvector.at(3));
    
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

  auto LAGDstripDist = new TH1F("", "",400, 0, 40);
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
      LAGDstripDist->Fill((Eventvector.at(0))*0.4);//in mm
     
      //cout << Eventvector.at(0) << endl;
    }
  }
  auto h = new TCanvas("","", 1000, 500);
  h->Divide(2,1);
  h->cd(1);
  HitspotprojX->Draw();
  h->cd(2);
  LAGDstripDist->Draw();
  return;
  //vectors have been created
  //Step one: find tracker offset (unique to each sector)
  //-- double iteration over Y and X offset
  //-- increment in Y, iterate through all x offsets, convert to r,phi for residual calculations!
  //-- plot res mean vs Yoffset and res sigma vs Xoffset 
  
 
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
  TH1F* residualLAGDx = 0;
  TH1F* residualLAGDy = 0;
  
  
  
  
  
  double meanREF2X=0.0, meanREF2Y=0.0, meanREF3X=0.0, meanREF3Y=0.0, meanREF1X=0.0, meanREF1Y=0.0, meanREF4X=0.0, meanREF4Y=0.0, meanLAGDx=0.0, meanLAGDy=0.0;
  double sigmaREF2X=0.0,sigmaREF2Y=0.0,sigmaREF3X=0.0,sigmaREF3Y=0.0, sigmaREF1X=0.0,sigmaREF1Y=0.0,sigmaREF4X=0.0,sigmaREF4Y=0.0,sigmaLAGDx=0.0,sigmaLAGDy=0.0;
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
  float Strippitch = 0.0041;// 4.1 mRad/strip
    
  float sectorXwidth = 80.0;//120;//look at Amore for these values in mm
  float sectorYwidth = 20.0;//60;

  //cout << -sumx/Tracker1x.size() << ", " << -sumy/Tracker1y.size() << endl;
  //return;
  float LAGDanglehere = LAGDstripDist->GetMean()*Strippitch;
  //cout << LAGDanglehere << endl;
  //return;
  float InitialXoffset = -sumx/Tracker1x.size();// add other initial offsets here, including optimized offsets at the end for residual plots
  float InitialYoffset = -sumy/Tracker1y.size();// + LAGDstripDist->GetMean();
  cout << InitialXoffset << endl;
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
  float APV3E1xtemp;
  float APV3E1ytemp;

  float efficiencynumerator=0.0;
  float tracking_error=0.0;
  auto Avgtrackerror = new TH1F("", "", 100, -0.1, 0.1);
  auto FitR = new TH1F("", "", 4000, 0, 2000);
  auto Hitspot = new TH2F("", "", 100, 0, 50, 100, 0, 50);
  auto Hitspotproj = new TH1F("", "", 100, 0, 50);
  auto HitspotLAGDproj = new TH1F("", "", 100, 0, 50);
  auto ReswidthdiffperXoffset = new TGraph();
  auto AllHitsFitplot = new TGraph();
  int allhitsit=0;
  auto CartesianSizeDiffperRadius = new TGraph();
  int cartit=0;
  auto resplotcheck = new TH1F("", "", 100, -10, 10);
  auto YprojvsYexp = new TGraph();
  auto YprojvsYtrack = new TGraph();
  
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

      
      residualREF1X = new TH1F(nameRes1X,"",30,-1,1); residualREF1X->SetXTitle("Residual in r [mm]"); residualREF1X->SetYTitle("Frequency");residualREF1X->SetLabelSize(0.05,"XY");residualREF1X->SetTitleSize(0.05,"XY");
      residualREF1Y = new TH1F(nameRes1Y,"",30,-1,1); residualREF1Y->SetXTitle("Residual in position [mm]"); residualREF1Y->SetYTitle("Frequency");residualREF1Y->SetLabelSize(0.05,"XY");residualREF1Y->SetTitleSize(0.05,"XY");     
      residualREF2X = new TH1F(nameRes2X,"",30,-1,1); residualREF2X->SetXTitle("Residual in r [mm]"); residualREF2X->SetYTitle("Frequency");residualREF2X->SetLabelSize(0.05,"XY");residualREF2X->SetTitleSize(0.05,"XY");
      residualREF2Y = new TH1F(nameRes2Y,"",30,-1,1); residualREF2Y->SetXTitle("Residual in position [mm]"); residualREF2Y->SetYTitle("Frequency");residualREF2Y->SetLabelSize(0.05,"XY");residualREF2Y->SetTitleSize(0.05,"XY");      
      residualREF3X = new TH1F(nameRes3X,"",30,-1,1); residualREF3X->SetXTitle("Residual in r [mm]"); residualREF3X->SetYTitle("Frequency");residualREF3X->SetLabelSize(0.05,"XY");residualREF3X->SetTitleSize(0.05,"XY");
      residualREF3Y = new TH1F(nameRes3Y,"",30,-1,1); residualREF3Y->SetXTitle("Residual in position [mm]"); residualREF3Y->SetYTitle("Frequency");residualREF3Y->SetLabelSize(0.05,"XY");residualREF3Y->SetTitleSize(0.05,"XY");
      residualREF4X = new TH1F(nameRes4X,"",30,-1,1); residualREF4X->SetXTitle("Residual in r [mm]"); residualREF4X->SetYTitle("Frequency");residualREF4X->SetLabelSize(0.05,"XY");residualREF4X->SetTitleSize(0.05,"XY");
      residualREF4Y = new TH1F(nameRes4Y,"",30,-1,1); residualREF4Y->SetXTitle("Residual in position [mm]"); residualREF4Y->SetYTitle("Frequency");residualREF4Y->SetLabelSize(0.05,"XY");residualREF4Y->SetTitleSize(0.05,"XY");
      residualLAGD = new TH1F(nameResLAGD,"",50,-30,5); residualLAGD->SetXTitle("Residual in position [mm]"); residualLAGD->SetYTitle("Frequency");residualLAGD->SetLabelSize(0.05,"XY");residualLAGD->SetTitleSize(0.05,"XY");
      residualLAGDx = new TH1F(nameResLAGD,"",50,-30,5); residualLAGDx->SetXTitle("Residual in r [mm]");residualLAGDx->SetYTitle("Frequency");residualLAGDx->SetLabelSize(0.05,"XY");residualLAGDx->SetTitleSize(0.05,"XY");
      residualLAGDy = new TH1F(nameTDist,"",50,-30,5); residualLAGDy->SetXTitle("Residual in r [mm]");residualLAGDy->SetYTitle("Frequency");residualLAGDy->SetLabelSize(0.05,"XY");residualLAGDy->SetTitleSize(0.05,"XY");
  
      sumx=0.0;
      sumy=0.0;
      float T12AvgAngle = 0.0;
      float T34AvgAngle = 0.0;
      float AvgAngle = 0.0;
      for(unsigned int i=0; i< APV3E1.size(); i++){//iterate through all events in the LAGD hit vector

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

	APV3E1ytemp = APV3E1.at(i)*0.4;
	//APV3E1xtemp = 29.1322;//realistically doesnt matter
	
	  
	/*
	//x residuals
	TGraph* g1 = new TGraph();
	g1->SetPoint(0,1596.4, Tracker1xtemp);
	g1->SetPoint(1,1541.6, Tracker2xtemp);
	g1->SetPoint(2,828.2, APV3E1xtemp);	
	g1->SetPoint(3,54.8, Tracker3xtemp);
	g1->SetPoint(4,0, Tracker4xtemp);


	TF1* f1 = new TF1("line1","pol1",0,1600);
	g1->Fit("line1","Q");
	double intercept1 = f1->GetParameter(0);
	double slope1     = f1->GetParameter(1);
	//cout << intercept1 << ", " << slope1 << endl;
	double MeasuredREF1X = intercept1 + slope1*1596.4;
	double MeasuredREF2X = intercept1 + slope1*1541.6;
	double MeasuredLAGDx = intercept1 + slope1*828.2;	
	double MeasuredREF3X = intercept1 + slope1*54.8;
	double MeasuredREF4X = intercept1 + slope1*0;
	  
	  
	residualREF1X->Fill(MeasuredREF1X-Tracker1xtemp);
	residualREF2X->Fill(MeasuredREF2X-Tracker2xtemp);
	residualREF3X->Fill(MeasuredREF3X-Tracker3xtemp);
	residualREF4X->Fill(MeasuredREF4X-Tracker4xtemp);
	residualLAGDx->Fill(MeasuredLAGDx - APV3E1xtemp);

	delete f1; delete g1;
	*/
	//y residuals
	TGraph* g2 = new TGraph();
	g2->SetPoint(0,1596.4, Tracker1ytemp);
	g2->SetPoint(1,1541.6, Tracker2ytemp);
	//g2->SetPoint(2,828.2, APV3E1ytemp);	
	g2->SetPoint(2,54.8, Tracker3ytemp);
	g2->SetPoint(3,0, Tracker4ytemp);


	TF1* f2 = new TF1("line1","pol1",0,1600);
	g2->Fit("line1","Q");
	double intercept2 = f2->GetParameter(0);
	double slope2     = f2->GetParameter(1);
	//cout << intercept1 << ", " << slope1 << endl;
	double MeasuredREF1Y = intercept2 + slope2*1596.4;
	double MeasuredREF2Y = intercept2 + slope2*1541.6;
	double MeasuredLAGDY = intercept2 + slope2*828.2;	
	double MeasuredREF3Y = intercept2 + slope2*54.8;
	double MeasuredREF4Y = intercept2 + slope2*0;
	  
	  
	residualREF1Y->Fill(MeasuredREF1Y-Tracker1ytemp);
	residualREF2Y->Fill(MeasuredREF2Y-Tracker2ytemp);
	residualREF3Y->Fill(MeasuredREF3Y-Tracker3ytemp);
	residualREF4Y->Fill(MeasuredREF4Y-Tracker4ytemp);
	residualLAGDy->Fill(MeasuredLAGDY-APV3E1ytemp);

	YprojvsYexp->SetPoint(i, APV3E1ytemp,MeasuredLAGDY);
	YprojvsYtrack->SetPoint(i, Tracker1ytemp,MeasuredREF1Y);
	delete f2; delete g2;
       
	Hitspotproj->Fill(MeasuredLAGDY);
	HitspotLAGDproj->Fill(APV3E1ytemp);
	

	Tracker1xtemp=0;
	Tracker1ytemp=0;
	Tracker2xtemp=0;
	Tracker2ytemp=0;
	Tracker3xtemp=0;
	Tracker3ytemp=0;
	Tracker4xtemp=0;
	Tracker4ytemp=0;
	APV3E1xtemp=0;
	APV3E1ytemp=0;
	
     }

      auto o = new TCanvas("", "", 1000, 500);
      o->Divide(2,1);
      o->cd(1);
      YprojvsYexp->Draw("A*");
      YprojvsYexp->SetTitle("LAGD Y coordinate vs Fit value");
      YprojvsYexp->GetXaxis()->SetTitle("LAGD Y coordinate [mm]");
      YprojvsYexp->GetYaxis()->SetTitle("Fit Y coordinate [mm]");    
      o->cd(2);
      YprojvsYtrack->Draw("A*");
      YprojvsYtrack->SetTitle("Tracker Y coordinate vs Tracker fit value");
      YprojvsYtrack->GetXaxis()->SetTitle("Tracker 1 Y coordinate [mm]");
      YprojvsYtrack->GetYaxis()->SetTitle("Tracker 1 Y fit coordinate [mm]");  
      string titl = nameCanv;
      titl += ".png";
      o->SaveAs(titl.c_str());;
      o->Draw();

      return;
      /*
      auto o = new TCanvas("", "", 500, 500);
      Hitspotproj->Draw();
      Hitspotproj->SetLineColor(2);
      HitspotLAGDproj->Draw("same");
      gPad->Update();
      string titl = nameCanv;
      titl += ".png";
      o->SaveAs(titl.c_str());;
      o->Draw();

      
      Hitspotproj->Reset();
      HitspotLAGDproj->Reset();
      */
      //return;
      //cout << efficiencynumerator << " / " << Tracker1x.size() << " = " << efficiencynumerator/Tracker1x.size() << endl;
      //cout << "Tracker beamspot location: " << sumx/Tracker1x.size()-80 << ", " << sumy/Tracker1y.size()<< endl;
      //return;
    
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

      //CartesianSizeDiffperRadius->SetPoint(cartit, FitR->GetMean(), (LAngDist->GetStdDev()/1000)*(FitR->GetMean()) - avesize);
      //cartit++;
      
      I2GFvalues myValues;
	
      //myValues = I2GFmainLoop(residualREF1X,1,10,1);  meanREF1X = myValues.mean; sigmaREF1X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF1Y,1,10,1);  meanREF1Y = myValues.mean; sigmaREF1Y=myValues.sigma;
      //myValues = I2GFmainLoop(residualREF2X,1,10,1);  meanREF2X = myValues.mean; sigmaREF2X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF2Y,1,10,1);  meanREF2Y = myValues.mean; sigmaREF2Y=myValues.sigma;
      //myValues = I2GFmainLoop(residualREF3X,1,10,1);  meanREF3X = myValues.mean; sigmaREF3X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF3Y,1,10,1);  meanREF3Y = myValues.mean; sigmaREF3Y=myValues.sigma;
      //myValues = I2GFmainLoop(residualREF4X,1,10,1);  meanREF4X = myValues.mean; sigmaREF4X=myValues.sigma;
      myValues = I2GFmainLoop(residualREF4Y,1,10,1);  meanREF4Y = myValues.mean; sigmaREF4Y=myValues.sigma;
      //myValues = I2GFmainLoop(residualLAGDx,1,10,1);  meanLAGDx = myValues.mean; sigmaLAGDx=myValues.sigma;
      myValues = I2GFmainLoop(residualLAGDy,1,10,1);  meanLAGDy = myValues.mean; sigmaLAGDy=myValues.sigma;      
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
      //cout << "LAGD mean Res and width: " << meanLAGD << ", " << sigmaLAGD << endl;


      //if(Yoffset == 12)
      //residualLAGD->Draw();
      
      
    
      
      //if(Xoffset == 0){ 
      //ResSigmaVsXoffset->SetPoint(ressigmait, Xoffset, sigmaLAGD);
      //ressigmait++;
      
	auto Xhists = new TCanvas("", "X Residual Histrogams", 1500, 1000);
	/*
	Xhists->Divide(3,2);
	Xhists->cd(1);
	residualREF1X->Draw();
	Xhists->cd(2);
	residualREF2X->Draw();
	Xhists->cd(3);
	residualREF3X->Draw();
	Xhists->cd(4);
	residualREF4X->Draw();
	Xhists->cd(5);
	residualLAGDx->Draw();
      
	*/
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
	residualLAGDy->Draw();
	
	
	//cout << "Res width: " << residualLAGD->GetStdDev() << endl;

	//Avgtrackerror->Draw();
	
	  
	Angularhists->Draw();
	title += ".png";
	Angularhists->SaveAs(title.c_str());

	return;
      
	//continue;
		
	//}
      
      //OutputFile << Xoffset << " " << Yoffset << " " << meanLAGD << " " << sigmaLAGD << " " <<  (AvgAngDist->GetStdDev() - LAngDist->GetStdDev()/1000) << endl;

      
      //if(Xoffset == 10)
      //	ReswidthperYoffset->SetPoint(Yiter, Yoffset, sigmaLAGD);

      //ResMeanVsYoffset->SetPoint(Yiter, Yoffset, meanLAGD);
      
      //ResidualMeanPerYoffset.push_back(meanLAGD);
      //ResidualRMSPerYoffset.push_back(sigmaLAGD);
      
      
      
      residualREF1X=NULL; residualREF1Y=NULL;
      residualREF2X=NULL; residualREF2Y=NULL;
      residualREF3X=NULL; residualREF3Y=NULL;
      residualREF4X=NULL; residualREF4Y=NULL;
      residualLAGDx=NULL; residualLAGDy=NULL;

     
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
    /*
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
    */
    //ResidualMeanPerYoffset.clear();
    //ResidualRMSPerYoffset.clear();
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

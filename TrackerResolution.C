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



void TrackerResolution(){

  //files for Tracker hits

  string Eventhits;
  ifstream TrackingFile("Aligned_Data/Trackers_Hits_SS_Test.txt");


  vector<float> Tracker1x;
  vector<float> Tracker1y;
  
  vector<float> Tracker2x;
  vector<float> Tracker2y;
  vector<float> Tracker3x;
  vector<float> Tracker3y;
  vector<float> Tracker4x;
  vector<float> Tracker4y;

  map<double, vector<float>> TrackerRadii;
  map<double, vector<float>> TrackerAngle;
  map<double, float> TrackerZ;
  map<double, TH1F*> TrackRadialErrorsAtRef;
  map<double, TH1F*> TrackAngularErrorsAtRef;
  map<double, TH1F*> TrackerRadResolutionHere;
  map<double, TH1F*> TrackerAngResolutionHere;
  map<double, TH1F*> TrackerRadiiplot;
  
  //these variables are used for initial offsets for trackers
  float sumy=0.0;
  float sumx=0.0;
  auto Tracker1Hitmap = new TH2F("PHT1", "Tracker 1 Beam Spot; X position [mm]; Y position [mm]", 50, -20, 200, 50, -20, 100);
  auto Tracker2Hitmap = new TH2F("PHT2", "Tracker 2 Beam Spot; X position [mm]; Y position [mm]", 50, 0, 50, 50, 0, 50);
  auto Tracker3Hitmap = new TH2F("PHT3", "Tracker 3 Beam Spot; X position [mm]; Y position [mm]", 50, 0, 50, 50, 0, 50);
  auto Tracker4Hitmap = new TH2F("PHT4", "Tracker 4 Beam Spot; X position [mm]; Y position [mm]", 50, 0, 50, 50, 0, 50);
  auto HitspotprojX = new TH1F("", "", 100, 0, 50);
  auto HitspotprojY = new TH1F("", "", 100, 0, 50);
  TGraph* TrackerStripCheck = new TGraph();
  TrackerStripCheck->SetTitle("Correlation between Different Tracker hits;Tracker Hits [mm]; Tracker Hits [mm]");
  
  int i=0;
  //separate the tracker file of coordinates into their own vectors
  while(getline(TrackingFile, Eventhits)){
    //cout << Eventhits.at(0) << endl;
    
    if(Eventhits.at(0) == '#')
      continue;
    
    stringstream Eventstringstream(Eventhits);
    //cout << Eventhits << endl;
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


    /*	g1->SetPoint(0,1596.4, T1r.at(i));
	g1->SetPoint(1,1541.6, T2r.at(i));//swapped 2 and 3
	g1->SetPoint(2,54.8, T3r.at(i));
	g1->SetPoint(3,0, T4r.at(i));
    */
    
    float Tracker1R = sqrt((Tracker1x.at(i))*(Tracker1x.at(i)) + Tracker1y.at(i)*Tracker1y.at(i));
    TrackerRadii[0].push_back(Tracker1R);
    float Tracker1Phi = atan(Tracker1y.at(i)/Tracker1x.at(i));
    TrackerAngle[0].push_back(Tracker1Phi*1000);//in urad   
    cout << Tracker1R << endl;
    float Tracker2R = sqrt((Tracker2x.at(i))*(Tracker2x.at(i)) + Tracker2y.at(i)*Tracker2y.at(i));
    TrackerRadii[1].push_back(Tracker2R);
    float Tracker2Phi = atan(Tracker2y.at(i)/Tracker2x.at(i));
    TrackerAngle[1].push_back(Tracker2Phi*1000);//in urad

    float Tracker3R = sqrt((Tracker3x.at(i))*(Tracker3x.at(i)) + Tracker3y.at(i)*Tracker3y.at(i));
    TrackerRadii[2].push_back(Tracker3R);
    float Tracker3Phi = atan(Tracker3y.at(i)/Tracker3x.at(i));
    TrackerAngle[2].push_back(Tracker3Phi*1000);//in urad

    
    float Tracker4R = sqrt((Tracker4x.at(i))*(Tracker4x.at(i)) + Tracker4y.at(i)*Tracker4y.at(i));
    TrackerRadii[3].push_back(Tracker4R);
    float Tracker4Phi = atan(Tracker4y.at(i)/Tracker4x.at(i));
    TrackerAngle[3].push_back(Tracker4Phi*1000);//in urad

    i++;
  
  }
  TrackerZ[0] = 1596.4;//in mm
  TrackerZ[1] = 1541.6;//in mm
  TrackerZ[2] = 54.8;//in mm
  TrackerZ[3] = 0;//in mm
  /*
  auto Hitspots = new TCanvas("", "Post Tracker Alignment BeamSpots", 1000, 1000);
  Hitspots->Divide(2,2);
  Hitspots->cd(1);
  Tracker1Hitmap->Draw("colz");
  Hitspots->cd(2);
  Tracker2Hitmap->Draw("colz");
  Hitspots->cd(3);
  Tracker3Hitmap->Draw("colz");
  Hitspots->cd(4);
  Tracker4Hitmap->Draw("colz");

  Hitspots->Draw();
  
  return;
  */
  float sizeX = HitspotprojX->GetStdDev();
  float sizeY = HitspotprojY->GetStdDev();
  float avesize = (sizeX + sizeY)/2.0;
  cout << "SizeX: " << sizeX << ", " << "SizeY: " << sizeY << ", average size: " << (sizeX + sizeY)/2.0 << endl; 

 
  TH1F* residualREF1X = 0;
  TH1F* residualREF1Y = 0;
  TH1F* residualREF2X = 0;
  TH1F* residualREF2Y = 0;
  TH1F* residualREF3X = 0;
  TH1F* residualREF3Y = 0;
  TH1F* residualREF4X = 0;
  TH1F* residualREF4Y = 0;
  TH1F* residualLAGD = 0;
  
  
   
  
  for(unsigned int F=0; F<= 4; F++){//iterate through the trackers

    //rename all of this to the vectors in this code!
    char nameRes1X[50];sprintf(nameRes1X,"residualREFX%.1u",F%4);char nameRes1Y[50];sprintf(nameRes1Y,"residualREFY%.1u",F%4);
    char nameRes2X[50];sprintf(nameRes2X,"residualREFX%.1u",(F+1)%4);char nameRes2Y[50];sprintf(nameRes2Y,"residualREFY%.1u",F%4+1);
    char nameRes3X[50];sprintf(nameRes3X,"residualREFX%.1u",(F+2)%4);char nameRes3Y[50];sprintf(nameRes3Y,"residualREFY%.1u",F%4+2);
    char nameRes4X[50];sprintf(nameRes4X,"residualREFX%.1u",(F+3)%4);char nameRes4Y[50];sprintf(nameRes4Y,"residualREFY%.1u",F%4+3);
    char nameRadial[50];sprintf(nameRadial,"RadialResTracker_%.1u",(F+3)%4);
    char nameAngular[50];sprintf(nameAngular,"AngularResTracker_%.1u",(F+3)%4);
    char nameRadialError[50];sprintf(nameRadialError,"RadialErrorTracker_%.1u",(F+3)%4);
    char nameAngularError[50];sprintf(nameAngularError,"AngularErrorTracker_%.1u",(F+3)%4);
    char nameTrackRadRes[50];sprintf(nameTrackRadRes,"TrackRadRes_%.1u",(F+3)%4);
    char nameTrackAngRes[50];sprintf(nameTrackRadRes,"TrackRadRes_%.1u",(F+3)%4);
      

    residualREF1X = new TH1F(nameRes1X,"",100,-5,5); residualREF1X->SetXTitle("Residual in Radii [mm]"); residualREF1X->SetYTitle("Frequency");residualREF1X->SetLabelSize(0.05,"XY");residualREF1X->SetTitleSize(0.05,"XY");  
    residualREF1Y = new TH1F(nameRes1Y,"",100,-5,5); residualREF1Y->SetXTitle("Residual in Angle [urad]"); residualREF1Y->SetYTitle("Frequency");residualREF1Y->SetLabelSize(0.05,"XY");residualREF1Y->SetTitleSize(0.05,"XY");
    residualREF2X = new TH1F(nameRes2X,"",100,-5,5); residualREF2X->SetXTitle("Residual in Radii [mm]"); residualREF2X->SetYTitle("Frequency");residualREF2X->SetLabelSize(0.05,"XY");residualREF2X->SetTitleSize(0.05,"XY");  
    residualREF2Y = new TH1F(nameRes2Y,"",100,-5,5); residualREF2Y->SetXTitle("Residual in Angle [urad]"); residualREF2Y->SetYTitle("Frequency");residualREF2Y->SetLabelSize(0.05,"XY");residualREF2Y->SetTitleSize(0.05,"XY");
    residualREF3X = new TH1F(nameRes3X,"",100,-5,5); residualREF3X->SetXTitle("Residual in Radii [mm]"); residualREF3X->SetYTitle("Frequency");residualREF3X->SetLabelSize(0.05,"XY");residualREF3X->SetTitleSize(0.05,"XY");  
    residualREF3Y = new TH1F(nameRes3Y,"",100,-5,5); residualREF3Y->SetXTitle("Residual in Angle [urad]"); residualREF3Y->SetYTitle("Frequency");residualREF3Y->SetLabelSize(0.05,"XY");residualREF3Y->SetTitleSize(0.05,"XY");
    residualREF4X = new TH1F(nameRes4X,"",100,-5,5); residualREF4X->SetXTitle("Residual in Radii [mm]"); residualREF4X->SetYTitle("Frequency");residualREF4X->SetLabelSize(0.05,"XY");residualREF4X->SetTitleSize(0.05,"XY");  
    residualREF4Y = new TH1F(nameRes4Y,"",100,-5,5); residualREF4Y->SetXTitle("Residual in Angle [urad]"); residualREF4Y->SetYTitle("Frequency");residualREF4Y->SetLabelSize(0.05,"XY");residualREF4Y->SetTitleSize(0.05,"XY"); 
    TrackRadialErrorsAtRef[(F+3)%4] = new TH1F(nameRadialError,"",100,-5,5); TrackRadialErrorsAtRef[(F+3)%4]->SetXTitle("Error in Radius [mm]"); TrackRadialErrorsAtRef[(F+3)%4]->SetYTitle("Frequency");TrackRadialErrorsAtRef[(F+3)%4]->SetLabelSize(0.05,"XY");TrackRadialErrorsAtRef[(F+3)%4]->SetTitleSize(0.05,"XY");
    TrackAngularErrorsAtRef[(F+3)%4] = new TH1F(nameAngularError,"",100,-5,5); TrackAngularErrorsAtRef[(F+3)%4]->SetXTitle("Error in Angle [urad]"); TrackAngularErrorsAtRef[(F+3)%4]->SetYTitle("Frequency");TrackAngularErrorsAtRef[(F+3)%4]->SetLabelSize(0.05,"XY");TrackAngularErrorsAtRef[(F+3)%4]->SetTitleSize(0.05,"XY");
    TrackerRadResolutionHere[(F+3)%4] = new TH1F(nameTrackRadRes,"",80,0,300); TrackerRadResolutionHere[(F+3)%4]->SetXTitle("Radial Resolution [um]"); TrackerRadResolutionHere[(F+3)%4]->SetYTitle("Frequency");TrackerRadResolutionHere[(F+3)%4]->SetLabelSize(0.05,"XY");TrackerRadResolutionHere[(F+3)%4]->SetTitleSize(0.05,"XY");
    TrackerAngResolutionHere[(F+3)%4] = new TH1F(nameTrackAngRes,"",40, -20, 20); TrackerAngResolutionHere[(F+3)%4]->SetXTitle("Angular Resolution [urad]"); TrackerAngResolutionHere[(F+3)%4]->SetYTitle("Count/1 urad");TrackerAngResolutionHere[(F+3)%4]->SetLabelSize(0.05,"XY");TrackerAngResolutionHere[(F+3)%4]->SetTitleSize(0.05,"XY");
    TrackerRadiiplot[(F+3)%4] = new TH1F(nameTrackAngRes,"",100, -50, 50); TrackerRadiiplot[(F+3)%4]->SetXTitle("Angular Resolution [urad]"); TrackerRadiiplot[(F+3)%4]->SetYTitle("Frequency");TrackerRadiiplot[(F+3)%4]->SetLabelSize(0.05,"XY");TrackerRadiiplot[(F+3)%4]->SetTitleSize(0.05,"XY");
    for(unsigned int i=0; i< Tracker1x.size(); i++){//iterate through all events in the LAGD hit vector

      // cout << "First Tracker: " << F%4 << endl;
      // cout << "Second Tracker: " << (F+1)%4 << endl;
      // cout << "Third Tracker: " << (F+2)%4 << endl;
      // cout << "Tracker being measured: " << (F+3)%4 << endl;
	

      //first, find exclusive residuals for both Radial and Angular coordinates
      TGraph* g1 = new TGraph();
      g1->SetPoint(0,TrackerZ[F%4], TrackerRadii[F%4].at(i));
      g1->SetPoint(1,TrackerZ[(F+1)%4], TrackerRadii[(F+1)%4].at(i));
      g1->SetPoint(2,TrackerZ[(F+2)%4], TrackerRadii[(F+2)%4].at(i));	

      TF1* f1 = new TF1("line1","pol1",0,1600);
      g1->Fit("line1","QR");
      double intercept1 = f1->GetParameter(0);
      double slope1     = f1->GetParameter(1);

      double sloperr1 = f1->GetParError(1);
      double yinterr1 = f1->GetParError(0);

      //cout << "Error in slope: " << sloperr1 << ", Error in y int: " << yinterr1 << endl;
	  
      auto TrackerRadialError = sqrt((TrackerZ[(F+3)%4]*sloperr1)*(TrackerZ[(F+3)%4]*sloperr1) + yinterr1*yinterr1);
      cout << "Tracker Error: " << TrackerRadialError << endl;
      TrackRadialErrorsAtRef[(F+3)%4]->Fill(TrackerRadialError);
	  
      double MeasuredREF1X = intercept1 + slope1*TrackerZ[F%4];
      double MeasuredREF2X = intercept1 + slope1*TrackerZ[(F+1)%4];
      double MeasuredREF3X = intercept1 + slope1*TrackerZ[(F+2)%4];
      double MeasuredREF4X = intercept1 + slope1*TrackerZ[(F+3)%4];
	  

      TrackerRadiiplot[(F+3)%4]->Fill(MeasuredREF4X);
	
      residualREF1X->Fill(MeasuredREF1X-TrackerRadii[F%4].at(i));
      residualREF2X->Fill(MeasuredREF2X-TrackerRadii[(F+1)%4].at(i));
      residualREF3X->Fill(MeasuredREF3X-TrackerRadii[(F+2)%4].at(i));
      residualREF4X->Fill(MeasuredREF4X-TrackerRadii[(F+3)%4].at(i));


      
      auto RadialRes = sqrt(abs((MeasuredREF4X-TrackerRadii[(F+3)%4].at(i))*(MeasuredREF4X-TrackerRadii[(F+3)%4].at(i)) - TrackerRadialError*TrackerRadialError));
      cout << "Rad res: " << RadialRes << endl;
      TrackerRadResolutionHere[(F+3)%4]->Fill(RadialRes*1000);
	
      delete f1; delete g1;
	
      //residualLAGDr->Fill(MeasuredLAGDr - LAGDr);
      //fit track in phi direction

      TGraph* g2 = new TGraph();

      g2->SetPoint(0,TrackerZ[F%4], TrackerAngle[F%4].at(i));
      g2->SetPoint(1,TrackerZ[(F+1)%4], TrackerAngle[(F+1)%4].at(i));
      g2->SetPoint(2,TrackerZ[(F+2)%4], TrackerAngle[(F+2)%4].at(i));	
	
      TF1* f2 = new TF1("line2","pol1",0,1600);
      g2->Fit("line2","QR");
      double intercept2 = f2->GetParameter(0);
      double slope2     = f2->GetParameter(1);
	
      double sloperr2 = f2->GetParError(1);
      double yinterr2 = f2->GetParError(0);

      //cout << "Error in slope: " << sloperr2 << ", Error in y int: " << yinterr2 << endl;
	  
      auto TrackerAngularError = sqrt((TrackerZ[(F+3)%4]*sloperr2)*(TrackerZ[(F+3)%4]*sloperr2) + yinterr2*yinterr2);
      cout << "Tracker Error: " << TrackerAngularError << endl;
      TrackAngularErrorsAtRef[(F+3)%4]->Fill(TrackerAngularError);
	
      double MeasuredREF1Y = intercept2 + slope2*TrackerZ[F%4];
      double MeasuredREF2Y = intercept2 + slope2*TrackerZ[(F+1)%4];
      double MeasuredREF3Y = intercept2 + slope2*TrackerZ[(F+2)%4];
      double MeasuredREF4Y = intercept2 + slope2*TrackerZ[(F+3)%4];
	
	  
      residualREF1Y->Fill(MeasuredREF1Y-TrackerAngle[F%4].at(i));
      residualREF2Y->Fill(MeasuredREF2Y-TrackerAngle[(F+1)%4].at(i));
      residualREF3Y->Fill(MeasuredREF3Y-TrackerAngle[(F+2)%4].at(i));
      residualREF4Y->Fill(MeasuredREF4Y-TrackerAngle[(F+3)%4].at(i));

      //auto AngularRes = sqrt(abs((MeasuredREF4Y-TrackerAngle[(F+3)%4].at(i))*(MeasuredREF4Y-TrackerAngle[(F+3)%4].at(i)) - TrackerAngularError*TrackerAngularError));
      auto AngularRes = MeasuredREF4Y-TrackerAngle[(F+3)%4].at(i);
      cout << "Ang Res: " << AngularRes << endl;
      TrackerAngResolutionHere[(F+3)%4]->Fill(AngularRes);
       
      delete f2; delete g2;
    }
    gStyle->SetOptStat(220);
    TGaxis::SetMaxDigits();

    TF1* ferr = new TF1("lineerr","landau",0, 10);
    TrackAngularErrorsAtRef[(F+3)%4]->Fit("lineerr", "R");

    float MVPTrackerr = ferr->GetParameter(1);


    I2GFvalues myValues;
    double meanLAGD = 0.0, sigmaLAGD = 0.0;
    myValues = I2GFmainLoop(TrackerAngResolutionHere[(F+3)%4],1,5,1);  meanLAGD = myValues.mean; sigmaLAGD=myValues.sigma;
    
    auto resolution = sqrt(sigmaLAGD*sigmaLAGD - MVPTrackerr*MVPTrackerr);

    
    auto u = new TCanvas("", "", 1000, 500);
    u->Divide(2,1);
    u->cd(1);
    TrackerAngResolutionHere[(F+3)%4]->Draw();
     string TA1legendentry3 = "Resolution: ";
    TA1legendentry3 += Form("%.2f",resolution);
    TA1legendentry3 += " urad (";
    TA1legendentry3 += Form("%.2f",(TrackerRadiiplot[(F+3)%4]->GetMean()/1000)*resolution);
    TA1legendentry3 += " um)";
    char TA1charlegendentry3[TA1legendentry3.size()+1];
    strcpy(TA1charlegendentry3, TA1legendentry3.c_str());

    string TA1legendentry5 = "Radius: ";
    TA1legendentry5 += Form("%.2f",TrackerRadiiplot[(F+3)%4]->GetMean());
    TA1legendentry5 += " mm";
    char TA1charlegendentry5[TA1legendentry5.size()+1];
    strcpy(TA1charlegendentry5, TA1legendentry5.c_str());

    TLatex latexTA1;
    latexTA1.SetTextSize(0.035);
    latexTA1.SetTextAlign(13);  //align at top
    latexTA1.SetNDC();
    latexTA1.DrawLatex(0.6,.4,TA1charlegendentry3);
    latexTA1.DrawLatex(0.6,.35,TA1charlegendentry5);
    
    u->cd(2);    
    TrackerRadiiplot[(F+3)%4]->Draw();
    
    u->Draw();

   

    string title = "RadialAndAngularResolutionForTracker";

    title += to_string(((F+3)%4)+1);

    title += ".png";
    u->SaveAs(title.c_str());
    
    cout << "Tracker " << ((F+3)%4)+1 << " has Angular resolution: " << TrackerAngResolutionHere[(F+3)%4]->GetMean() << " mRad +/- " << TrackerAngResolutionHere[(F+3)%4]->GetMeanError() << " mRad" << endl;
    cout << "Tracker " << ((F+3)%4)+1 << " has Radial resolution: " << 	TrackerRadResolutionHere[(F+3)%4]->GetMean() << " um +/- " << TrackerRadResolutionHere[(F+3)%4]->GetMeanError() << " um" << endl;
    //return;
    
    
          
      
    residualREF1X=NULL; residualREF1Y=NULL;
    residualREF2X=NULL; residualREF2Y=NULL;
    residualREF3X=NULL; residualREF3Y=NULL;
    residualREF4X=NULL; residualREF4Y=NULL;

  }
  return;
  
}
  

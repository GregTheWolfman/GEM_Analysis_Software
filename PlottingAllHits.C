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
#include <TLatex.h>
#include <unistd.h>
#include <TMath.h>


void PlottingAllHits(){

  gStyle->SetOptStat(0);
  map<string, TString> inputfiles;
  map<string, float> Xoff;
  map<string, float> Yoff;

  inputfiles["SS"] = "Aligned_Data/LAGD_Beamspots_SS.txt";
  Xoff["SS"] = 20-80;
  Yoff["SS"] = 550/2;

  inputfiles["LZZ1"] = "Aligned_Data/LAGD_Beamspots_LZZ.txt";
  Xoff["LZZ1"] = 20-80;
  Yoff["LZZ1"] = 550/2;

  //inputfiles["LZZ2"] = "Aligned_Data/LAGD_Beamspots_APV3E2.txt";
  inputfiles["LZZ2"] = "Aligned_Data/LAGD_Beamspots_LZZ2.txt";
  Xoff["LZZ2"] = 20-80;
  Yoff["LZZ2"] = 550/2;
  
  inputfiles["APV5"] = "Aligned_Data/LAGD_Beamspots_APV5.txt";
  Xoff["APV5"] = 20-80+40;
  Yoff["APV5"] = 550/2;

  // // inputfiles["APV52"] = "Aligned_Data/LAGD_Beamspots_APV5_Trackers.txt";
  // // Xoff["APV52"] = 20-80;
  // // Yoff["APV52"] = 550/2;

  // // inputfiles["APV7E1"] = "Aligned_Data/LAGD_Beamspots_APV7E1.txt";
  // // Xoff["APV7E1"] = 20-80;
  // // Yoff["APV7E1"] = 550/2;

  inputfiles["APV4"] = "Aligned_Data/LAGD_Beamspots_APV4.txt";
  Xoff["APV4"] = 20-80;
  Yoff["APV4"] = 550/2;

  inputfiles["APV6"] = "Aligned_Data/LAGD_Beamspots_APV6.txt";
  Xoff["APV6"] = 20-80;
  Yoff["APV6"] = 550/2;


  inputfiles["APV89"] = "Aligned_Data/LAGD_Beamspots_APV89.txt";
  Xoff["APV89"] = 20-80;
  Yoff["APV89"] = 550/2;

  //Tracker Beamspots interpolated to LAGD
  // inputfiles["APV4"] = "Aligned_Data/Trackers_Hits_APV4NoLAGD_test.txt";
  // Xoff["APV4"] = 20 +120+120+220+220+150+7.785 -29.7665;
  // Yoff["APV4"] = 550/2 + 9.965 -15.0655;

  // inputfiles["SS"] = "Aligned_Data/Trackers_Hits_SSNoLAGD_test.txt";
  // Xoff["SS"] = 20 + 25 + 7.2913-29.7665;
  // Yoff["SS"] = 550/2 + 8.30171 -15.0655;

  // inputfiles["LZZ1"] = "Aligned_Data/Trackers_Hits_LZZNoLAGD_test.txt";
  // Xoff["LZZ1"] = 20+ 120 + 58.68747 - 29.7665;
  // Yoff["LZZ1"] = 550/2 + 8.31925 - 15.0655;

  // inputfiles["LZZ2"] = "Aligned_Data/Trackers_Hits_LZZ2NoLAGD_test.txt";
  // Xoff["LZZ2"] = 20+120+100+9.85-29.7665;
  // Yoff["LZZ2"] = 550/2 +  10.96-15.0655;
  
  // inputfiles["APV5"] = "Aligned_Data/Trackers_Hits_APV5NoLAGD_test.txt";
  // Xoff["APV5"] = 20+120+120+220+160 +4.75 - 29.7665;
  // Yoff["APV5"] = 550/2 + 10.26-15.0655;

  // inputfiles["APV6"] = "Aligned_Data/Trackers_Hits_APV6NoLAGD_test.txt";
  // Xoff["APV6"] = 20+120+120+80+10.41442 - 29.7665;
  // Yoff["APV6"] = 550/2 + 9.94989 - 15.0655;

  
  

  
  // ifstream LAGDFile("LAGD_Hits_SSAllFiles.txt");


  auto PlottingHist = new TH2D("", "Beam Spots on LAGD; X [mm]; Y [mm]", 950,0,950, 550, 0, 550);


  
  
  for(map<string,TString>::iterator i = inputfiles.begin(); i != inputfiles.end(); i++){
    ifstream myFile(i->second);
    string evtype = i->first;

    cout << evtype << endl;
    string LAGDEvent;
    while(getline(myFile, LAGDEvent)){
      //cout << LAGDEvent << endl;
      
      if(LAGDEvent.at(0) == '#')      
	continue;
      
      else{
	
	stringstream Eventstringstream(LAGDEvent);
	//cout << Eventhits << endl;
	vector<float> Eventvector(istream_iterator<float>(Eventstringstream),{});

	
	//auto R = Rad[evtype];
	//cout << Eventvector.at(0) << endl;
	//float Angle=0;
	// if(evtype == "APV5" || evtype == "APV6" || evtype == "APV4"){	  
	//   Angle = (Eventvector.at(0)-64)*StripPitch[evtype];
	// }
	//else{
	cout << Eventvector.at(0) << ", " << Eventvector.at(1) << endl;
	//	Angle = Eventvector.at(0)*StripPitch[evtype];
	  //}
	if(evtype == "APV89")	  
	  PlottingHist->Fill(Xoff[evtype] + Eventvector.at(0),Yoff[evtype] - Eventvector.at(1));
	else
	  PlottingHist->Fill(Xoff[evtype] + Eventvector.at(0),Yoff[evtype] + Eventvector.at(1));

	
	//LAGDstripDist->Fill(Eventvector.at(0));//-64 for sZZ sectors due to the sector location on the readoutboard
     
	//cout << Eventvector.at(0) << endl;
      }
    }
  }

  
  
  auto LAGDHitscanvas = new TCanvas("", "", 1000, 500);
  PlottingHist->Draw("colz");
  auto Narrowside = new TLine(0+20, (550/2)-(43/2), 0+20, (550/2)+(43/2));
  Narrowside->Draw();
  auto Wideside = new TLine(950-20, (550/2)-(530/2), 950-20, (550/2)+(530/2));
  Wideside->Draw();
  auto Topside = new TLine(0+20, (550/2)+(43/2), 950-20, (550/2)+(530/2));
  Topside->Draw();
  auto Bottomside = new TLine(0+20, (550/2)-(43/2), 950-20, (550/2)-(530/2));
  Bottomside->Draw();

  //sectors
  auto S0 = new TLine(0+20, (550/2), 0+20+120+120, (550/2));
  S0->Draw();
  auto S1 = new TLine(0+20+120, (550/2)-(43/2)-120*tan(0.525/2), 0+20+120, (550/2)+(43/2)+120*tan(0.525/2));
  S1->Draw();
  auto S2 = new TLine(0+20+120+120, (550/2)-(43/2)-(120+120)*tan(0.525/2), 0+20+120+120, (550/2)+(43/2)+(120+120)*tan(0.525/2));
  S2->Draw();
  auto S3 = new TLine(0+20+120+120+220, (550/2)-(43/2)-(120+120+220)*tan(0.525/2), 0+20+120+120+220, (550/2)+(43/2)+(120+120+220)*tan(0.525/2));
  S3->Draw();
  auto S4 = new TLine(0+20+120+120+220+220, (550/2)-(43/2)-(120+120+220+220)*tan(0.525/2), 0+20+120+120+220+220, (550/2)+(43/2)+(120+120+220+220)*tan(0.525/2));
  S4->Draw();

  //lsat 3 sectors dividers
  //cout << 0+20+120+120 << ", " << (550/2)-(120+120)*tan(0.525/6) << ", " << 0+20+120+120  << ", " << (550/2)+(120+120+220+220+220)*tan(0.525/6) << endl;
  auto D1 = new TLine(0+20+120+120, (550/2)+(120+120)*tan(0.525/6), 0+20+120+120+220+220+220+10, (550/2)+(120+120+220+220+220)*tan(0.525/6));
  D1->Draw();
  auto D2 = new TLine(0+20+120+120, (550/2)-(120+120)*tan(0.525/6), 0+20+120+120+220+220+220+10, (550/2)-(120+120+220+220+220)*tan(0.525/6));
  D2->Draw();

  

  LAGDHitscanvas->Draw();
  
















  return;  
}

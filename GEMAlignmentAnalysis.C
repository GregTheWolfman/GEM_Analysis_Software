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
//#include <TSpectrum.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraph2D.h>
#include <TText.h>
#include <cstdlib>
#include <TGraphErrors.h>
#include <RtypesCore.h>
#include <THistPainter.h>
//#include "FNAL-Beam-Test-Scripts/doubleGausFit_withHistParameter.C"
#include <TLatex.h>
#include <unistd.h>
//#include <gSystem.h>
//#include <gPad.h>
//#include "Optimized_XY_shift.C"



void GEMAlignmentAnalysis(){


  ifstream DataFile("DataPerXandYoffset_liltest.txt");

  vector<float> Xoffsets;
  vector<float> Yoffsets;
  vector<float> ResMeans;
  vector<float> ResWidths;
  vector<float> WidthDiffs;
  string Eventhits;

  while(getline(DataFile, Eventhits)){
    //cout << Eventhits.at(0) << endl;
    
    if(Eventhits.at(0) == '#')
      continue;

    
    stringstream Eventstringstream(Eventhits);
    //cout << Eventhits << endl;
    vector<float> Eventvector(istream_iterator<float>(Eventstringstream),{});
    //for(auto i = Eventvector.begin(); i != Eventvector.end(); i++){
    //  cout << *i << endl;
    //}
    //if(Eventvector.at(0) == 150){break;}
    Xoffsets.push_back(Eventvector.at(0));
    Yoffsets.push_back(Eventvector.at(1));
    ResMeans.push_back(Eventvector.at(2));
    ResWidths.push_back(Eventvector.at(3));
    WidthDiffs.push_back(Eventvector.at(4));
  }

  //for(auto i = Yoffsets.begin(); i != Yoffsets.end(); i++){
  //  cout << *i << endl;
  //}
  

  
  //float minreswidth = *min_element(ResWidths.begin(), ResWidths.end());
  //float minreswidthindex = std::distance(std::begin(ResWidths), std::min_element(std::begin(ResWidths), std::end(ResWidths)));

  //cout << Xoffsets.at(minreswidthindex) << ", " << Yoffsets.at(minreswidthindex) << ", " << ResMeans.at(minreswidthindex) << ", " << ResWidths.at(minreswidthindex) << endl;
  //return;
  map<float, TGraph*> ResMeanVsYoffsetPerXoffset;
  map<float, TGraph*> ReswidthVsXoffset;
  map<float, TGraph*> DistDiffvsXoffset;
  
  auto IdealYoffsetVsXoffset = new TGraph();
  auto IdealXoffsetVsYoffset = new TGraph();
  auto widthdiffvsXoffset = new TGraph();  
  
  auto IdealOffsets = new TGraph();
  int idealoffsetiter=0;
  auto MinimumResidualWidthPerXoffset = new TGraph();
  int Xit=-1;
  int Yit=-1;

  float PrevX=-1;
  float PrevY=-1;
  vector<float> PreviousXoffsetResMeans;
  vector<float> PreviousXoffsetResWidths;

  map<float, TGraph*> ResWidthsPerX;
  
  for(uint i = 0; i<Xoffsets.size();i++){
    float currentX=Xoffsets.at(i);
    float currentY=Yoffsets.at(i);

    cout << currentX << endl;
    cout << currentY << endl;

    
    
    if(currentX == 0){
      ResWidthsPerX[Yoffsets.at(i)] = new TGraph();
      DistDiffvsXoffset[Yoffsets.at(i)] = new TGraph(); 
      
    }
    //cout << "HERE" << endl;
    if(currentX != PrevX){
      ResMeanVsYoffsetPerXoffset[Xoffsets.at(i)] = new TGraph();
      Xit++;
      Yit=0;      
      //cout << "YES" << endl;
      if(currentX > 0){
	/*
	auto t = new TCanvas("", "", 500, 500);
	ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->Draw("A*");
	ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->SetTitle("LAGD Mean Residual per Y offset at X offset of 1");
	ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->GetXaxis()->SetTitle("Yoffset [mm]");
	ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->GetYaxis()->SetTitle("LAGD Mean Angular Residual [Rad]");
	*/
	TF1* f1 = new TF1("line1","pol1",0,50);
	ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->Fit("line1","Q");
	cout << "Ideal Y offsets for this Xoffset: " << -(f1->GetParameter(0))/(f1->GetParameter(1)) << endl;
	//t->Draw();
	//return;
	
	//cout << "Here?" << endl;
	IdealYoffsetVsXoffset->SetPoint(Xit-1, Xoffsets.at(i-1),  -(f1->GetParameter(0))/(f1->GetParameter(1)));
	IdealOffsets->SetPoint(idealoffsetiter, Xoffsets.at(i-1),  -(f1->GetParameter(0))/(f1->GetParameter(1)));
	idealoffsetiter++;
	
	delete f1;
	ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]=NULL;
	//return;

	MinimumResidualWidthPerXoffset->SetPoint(Xit-1, Xoffsets.at(i-1), *min_element(PreviousXoffsetResWidths.begin(), PreviousXoffsetResWidths.end()));

	PreviousXoffsetResWidths.clear();
	PreviousXoffsetResMeans.clear();
	  
      }

      PrevX = currentX;
    }//after first new Xoffset

    
    ResWidthsPerX[Yoffsets.at(i)]->SetPoint(Xit, Xoffsets.at(i), ResWidths.at(i));
    DistDiffvsXoffset[Yoffsets.at(i)]->SetPoint(Xit, Xoffsets.at(i), WidthDiffs.at(i));
    PreviousXoffsetResMeans.push_back(ResMeans.at(i));
    PreviousXoffsetResWidths.push_back(ResWidths.at(i));

    //cout << PrevX << endl;
    cout << Xit << ", " <<  Yit << ", " << Yoffsets.at(i) << ", " << ResMeans.at(i) << endl;

    ResMeanVsYoffsetPerXoffset[Xoffsets.at(i)]->SetPoint(Yit, Yoffsets.at(i), ResMeans.at(i));
    Yit++;
    //cout << Xit << ", " <<  Yit << ", " << Yoffsets.at(i) << ", " << ResMeans.at(i) << endl;

   
    /*
    if(currentX == 1){
      auto t = new TCanvas("", "", 500, 500);
      ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->Draw("A*");
      ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->SetTitle("LAGD Mean Residual per Y offset at X offset of 9");
      ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->GetXaxis()->SetTitle("Yoffset [mm]");
      ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->GetYaxis()->SetTitle("LAGD Mean Angular Residual [Rad]");
      TF1* f1 = new TF1("line1","pol1",0,50);
      ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->Fit("line1","Q");
      cout << "Ideal Y offsets for this Xoffset: " << -(f1->GetParameter(0))/(f1->GetParameter(1)) << endl;
      t->Draw();
      delete f1;
      return;
    }
    */
  
    

    //Xit++;
    // Yit++;

    if(Xoffsets.at(i) == Xoffsets.back()) {
      auto fn = new TF1("line1", "pol2", 0,Xoffsets.back());
      ResWidthsPerX[Yoffsets.at(i)]->Fit("line1", "Q");
      auto idealX = -(fn->GetParameter(1))/(2*fn->GetParameter(2));
      cout << "Ideal X offset for Yoffset " << Yoffsets.at(i) << " is " << idealX << endl;
      //ResWidthsPerX[Yoffsets.at(i)]->Draw("A*");
      
      IdealXoffsetVsYoffset->SetPoint(Yit-1, idealX, Yoffsets.at(i));
      IdealOffsets->SetPoint(idealoffsetiter, idealX, Yoffsets.at(i));
      idealoffsetiter++;

      
      auto fm = new TF1("line5", "pol2", 0,Xoffsets.back());
      DistDiffvsXoffset[Yoffsets.at(i)]->Fit("line5", "Q");
      
      cout << fm->GetParameter(0) << ", " << fm->GetParameter(1) << ", " << fm->GetParameter(2) << endl;
      auto c = fm->GetParameter(0);
      auto b = fm->GetParameter(1);
      auto a = fm->GetParameter(2);

      
      auto idealX2 = min((-b-sqrt(b*b - 4*a*c))/(2*a), (-b+sqrt(b*b - 4*a*c))/(2*a));
      cout << "Ideal X2 offset for Yoffset " << Yoffsets.at(i) << " is " << idealX2 << endl;
      widthdiffvsXoffset->SetPoint(Yit-1, idealX2, Yoffsets.at(i));
      IdealOffsets->SetPoint(idealoffsetiter, idealX2, Yoffsets.at(i));
      idealoffsetiter++;
      
      //DistDiffvsXoffset[Yoffsets.at(i)]->Draw("A*");
      
      //return;

      /*
      if(Yoffsets.at(i) == 10){
	cout << Xoffsets.at(i) << endl;
	ResWidthsPerX[Yoffsets.at(i)]->Draw("A*");
	ResWidthsPerX[Yoffsets.at(i)]->GetXaxis()->SetTitle("X offset [mm]");
	ResWidthsPerX[Yoffsets.at(i)]->GetYaxis()->SetTitle("LAGD inclusive residual width [Rad]");
	ResWidthsPerX[Yoffsets.at(i)]->SetTitle("Minimum residual width per X offset at Yoffset = 5mm");
  
	
	return;
      }
      */  
    }
  }


  
  
  // IdealYoffsetVsXoffset->Draw("A*");
  IdealYoffsetVsXoffset->SetTitle("Ideal Y offset for each X offset");
  IdealYoffsetVsXoffset->GetXaxis()->SetTitle("X offset [mm]");
  IdealYoffsetVsXoffset->GetYaxis()->SetTitle("Ideal Y offset [mm]");
  /*
  //MinimumResidualWidthPerXoffset->Draw("A*");
  TF1* f1 = new TF1("line1", "pol2",0,120);
  MinimumResidualWidthPerXoffset->Fit("line1","Q");
  cout << f1->GetParameter(0) << ", " << f1->GetParameter(1) << ", " << f1->GetParameter(2) << endl; 
  cout << "Ideal Xoffset: " << -(f1->GetParameter(1))/(2*f1->GetParameter(2)) << endl;
  */
    

  //TGraph *widthdiffvsXoffsetclone = (TGraph*) widthdiffvsXoffset->Clone();
  
  //widthdiffvsXoffset->Draw("A*");
  //widthdiffvsXoffset->GetXaxis()->SetTitle("Y offset [mm]");
  //widthdiffvsXoffset->GetYaxis()->SetTitle("Ideal X offset [mm]");
  //widthdiffvsXoffset->SetTitle("Ideal X offset Per Y offset");
  
  //TF1* f0 = new TF1("line0", "pol2");
  //widthdiffvsXoffset->Fit("line0", "Q");
  //cout << "Fit Line eqn: " << f0->GetParameter(2) << "x2 + " <<  f0->GetParameter(1) << "x + " <<  f0->GetParameter(0) << endl;

      
 
  //return;
  

  //TGraph *IdealYoffsetVsXoffsetclone = (TGraph*) IdealYoffsetVsXoffset->Clone();
  
  //IdealYoffsetVsXoffset->Draw("A*");
  IdealYoffsetVsXoffset->GetXaxis()->SetTitle("X offset [mm]");
  IdealYoffsetVsXoffset->GetYaxis()->SetTitle("Ideal Y offset [mm]");
  IdealYoffsetVsXoffset->SetTitle("Ideal Y offset Per X offset");
  
  TF1* f1 = new TF1("line1", "pol1",0,240);
  IdealYoffsetVsXoffset->Fit("line1", "Q");
  cout << "Fit Line eqn: " << f1->GetParameter(1) << "x + " <<  f1->GetParameter(0) << endl;


  //return;
  
  //IdealXoffsetVsYoffset->Draw("A*");
  IdealXoffsetVsYoffset->GetXaxis()->SetTitle("Y offset [mm]");
  IdealXoffsetVsYoffset->GetXaxis()->SetRangeUser(0,150);
  IdealXoffsetVsYoffset->GetYaxis()->SetTitle("Ideal X offset [mm]");
  IdealXoffsetVsYoffset->SetTitle("Ideal X offset Per Y offset");
  TF1* f2 = new TF1("line2", "pol1",0,240);
  IdealXoffsetVsYoffset->Fit("line2", "Q");
  cout << "Fit Line eqn: " << f2->GetParameter(1) << "x + " <<  f2->GetParameter(0) << endl;

  float a = f1->GetParameter(1);
  float b = f1->GetParameter(0);
  float c = f2->GetParameter(1);
  float d = f2->GetParameter(0);
  
  float bestX = (d-b)/(a-c);
  float bestY = a*bestX + b;
  cout << "Ideal X offset: " << bestX << ", " << bestY << endl;
    
  
  //return;
  /*
  TF1* f2 = new TF1("line2", "pol1");
  IdealXoffsetVsYoffset->Fit("line2","Q");
  cout << "Fit Line eqn: " << f2->GetParameter(1) << "x + " <<  f2->GetParameter(0) << endl;
  */
  //return;
  //IdealOffsets->Draw("A*");

  //ax^2 + bx + c = dx + e
  //ax^2 + (b - d)x + (c - e) = 0

  //auto c = f0->GetParameter(0) - f1->GetParameter(0);
  //auto b = f0->GetParameter(1) - f1->GetParameter(1);
  //auto a = f0->GetParameter(2);

   
  //auto BestX = max((-b-sqrt(b*b - 4*a*c))/(2*a), (-b+sqrt(b*b - 4*a*c))/(2*a));
  
  //auto BestY = f1->GetParameter(1)*BestX + f1->GetParameter(0);
  
  
  //cout << "The ideal offsets for this sector with Method 2 are (" <<  BestX << ", " << BestY << ")" << endl;
  

  auto lastcanv = new TCanvas("", "", 1000, 1000);
  lastcanv->Divide(2,2);
  lastcanv->cd(1);
  
  IdealYoffsetVsXoffset->Draw("A*");
  IdealYoffsetVsXoffset->GetXaxis()->SetTitle("X offset [mm]");
  IdealYoffsetVsXoffset->GetYaxis()->SetTitle("Ideal Y offset [mm]");
  IdealYoffsetVsXoffset->SetTitle("Ideal Y offset Per X offset");
  
  lastcanv->cd(2);
    
  IdealXoffsetVsYoffset->Draw("A*");
  IdealXoffsetVsYoffset->GetXaxis()->SetTitle("Y offset [mm]");
  //IdealXoffsetVsYoffset->GetXaxis()->SetRangeUser(0,150);
  IdealXoffsetVsYoffset->GetYaxis()->SetTitle("Ideal X offset [mm]");
  IdealXoffsetVsYoffset->SetTitle("Ideal X offset Per Y offset method 1");
  //return;
  /*
  DistDiffvsXoffset[10]->Draw("A*");
  DistDiffvsXoffset[10]->GetXaxis()->SetTitle("X offset [mm]");
  DistDiffvsXoffset[10]->GetYaxis()->SetTitle("#sigma_{Trackers} - #sigma_{LAGD} [Rad]");
  DistDiffvsXoffset[10]->SetTitle("Difference in Tracker and LAGD Angular Distributions at Y = 10 mm");
  */
  
  lastcanv->cd(3);
  widthdiffvsXoffset->Draw("A*");
  widthdiffvsXoffset->GetXaxis()->SetTitle("Ideal X offsets [mm]");
  widthdiffvsXoffset->GetYaxis()->SetTitle("Y offsets [mm]");
  widthdiffvsXoffset->SetTitle("Method 2 to find X offset");
  
  lastcanv->cd(4);
  IdealOffsets->Draw("A*");
  IdealOffsets->GetXaxis()->SetTitle("Ideal X offsets [mm]");
  IdealOffsets->GetYaxis()->SetTitle("Ideal Y offsets [mm]");
  IdealOffsets->SetTitle("Ideal X and Y offsets");
  
  
  
  
  lastcanv->Draw();
  return;
}

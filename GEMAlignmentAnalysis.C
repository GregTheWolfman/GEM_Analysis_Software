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


  ifstream DataFile("Aligned_Data/DataPerXandY_Merged_fixed_aligned.txt");

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

  
  for(auto i = Yoffsets.begin(); i != Yoffsets.end(); i++){
    //cout << *i << endl;
  }

  //cout << Xoffsets.size()<< endl;
  

  
  //float minreswidth = *min_element(ResWidths.begin(), ResWidths.end());
  //float minreswidthindex = std::distance(std::begin(ResWidths), std::min_element(std::begin(ResWidths), std::end(ResWidths)));

  //cout << Xoffsets.at(minreswidthindex) << ", " << Yoffsets.at(minreswidthindex) << ", " << ResMeans.at(minreswidthindex) << ", " << ResWidths.at(minreswidthindex) << endl;
  //return;
  map<float, TGraph*> ResMeanVsYoffsetPerXoffset;
  map<float, TGraph*> ReswidthVsXoffset;
  map<float, TGraph*> DistDiffvsXoffset;
  
  auto IdealYoffsetVsXoffset = new TGraph();
  auto widthdiffvsXoffset = new TGraph();  

  
  vector<float> Yoffsets_h;
  vector<float> IdealXes;
  vector<float> IdealXeserrs;
  vector<float> Yerrors;
  
  vector<float> idealX2vec;
  vector<float> idealXvec;
  
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
  map<float, vector<float>> ResWidthsvec;
  map<float, vector<float>> XoffsetsforeachY;
  
  // cout << "What is happening" << endl;
  for(uint i = 0; i<Xoffsets.size();i++){
    float currentX=Xoffsets.at(i);
    float currentY=Yoffsets.at(i);
    //return;
    //cout << "Whattt" << endl;
    cout << currentX << endl;
    cout << currentY << endl;
    //return;
    
    
    if(currentX == 0){
      ResWidthsPerX[Yoffsets.at(i)] = new TGraph();
      DistDiffvsXoffset[Yoffsets.at(i)] = new TGraph(); 
      //cout << "Atleast Goes here?" << endl;
    }
    //cout << "HERE" << endl;
    if(currentX != PrevX){
      ResMeanVsYoffsetPerXoffset[Xoffsets.at(i)] = new TGraph();
      Xit++;
      Yit=0;      
      //cout << "YES" << endl;
      if(currentX > 0){
	// string ht1 = to_string(Xoffsets.at(i));
	// char cart1[ht1.size()];
	// strcpy(cart1, ht1.c_str());
	// auto t = new TCanvas(cart1, "", 500, 500);
	// ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->Draw("A*");
	// ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->SetTitle("LAGD Mean Residual per Y offset at X offset of 2;Yoffset [mm];LAGD Mean Angular Residual [uRad]");
	
	TF1* f1 = new TF1("line1","pol1",0,50);
	ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->Fit("line1","Q");
	auto Yoffseterr = sqrt(((-(f1->GetParError(0))/(f1->GetParameter(1)))*(-(f1->GetParError(0))/(f1->GetParameter(1)))) + ((-(f1->GetParameter(0)*f1->GetParError(1))/(f1->GetParameter(1)*f1->GetParameter(1)))*(-(f1->GetParameter(0)*f1->GetParError(1))/(f1->GetParameter(1)*f1->GetParameter(1)))));
	cout << "Ideal Y offsets for this Xoffset: " << -(f1->GetParameter(0))/(f1->GetParameter(1)) << " +/- " << Yoffseterr << endl;
	
	// string T3xlegendentry = "Y Offset of 0 Mean Residual from Fit: ";
	// //      T3xlegendentry += Form("%.2f",fn->GetParameter(1)/fn->GetParameter(0));
	// T3xlegendentry += Form("%.2f",-(f1->GetParameter(0))/(f1->GetParameter(1)));
	// T3xlegendentry += " mm";
	// char T3xcharlegendentry[T3xlegendentry.size()];
	// strcpy(T3xcharlegendentry, T3xlegendentry.c_str());
      
	// TLatex latexT3x;
	// latexT3x.SetTextSize(0.035);
	// latexT3x.SetTextAlign(13);  //align at top
	// latexT3x.SetNDC();
	// latexT3x.DrawLatex(0.5,.5,T3xcharlegendentry);

	
	// t->Draw();
	
	// return;
	

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
    ResWidthsvec[Yoffsets.at(i)].push_back(ResWidths.at(i));
    XoffsetsforeachY[Yoffsets.at(i)].push_back(Xoffsets.at(i));
    DistDiffvsXoffset[Yoffsets.at(i)]->SetPoint(Xit, Xoffsets.at(i), WidthDiffs.at(i));
    PreviousXoffsetResMeans.push_back(ResMeans.at(i));
    PreviousXoffsetResWidths.push_back(ResWidths.at(i));

    //cout << PrevX << endl;
    cout << Xit << ", " <<  Yit << ", " << Yoffsets.at(i) << ", " << ResMeans.at(i) << endl;

    ResMeanVsYoffsetPerXoffset[Xoffsets.at(i)]->SetPoint(Yit, Yoffsets.at(i), ResMeans.at(i));
    Yit++;
    //cout << Xit << ", " <<  Yit << ", " << Yoffsets.at(i) << ", " << ResMeans.at(i) << endl;

   
    
    // if(currentX == 1){
    // string ht1 = to_string(Xoffsets.at(i));
    // char cart1[ht1.size()];
    // strcpy(cart1, ht1.c_str());
    // auto t = new TCanvas(cart1, "", 500, 500);
    // ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->Draw("A*");
    // ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->SetTitle("LAGD Mean Residual per Y offset at X offset of 9");
    // ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->GetXaxis()->SetTitle("Yoffset [mm]");
    // ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->GetYaxis()->SetTitle("LAGD Mean Angular Residual [Rad]");
    // TF1* f1 = new TF1("line1","pol1",0,50);
    // ResMeanVsYoffsetPerXoffset[Xoffsets.at(i-1)]->Fit("line1","Q");
    // auto Yoffseterr = sqrt(((-(f1->GetParError(0))/(f1->GetParameter(1)))*(-(f1->GetParError(0))/(f1->GetParameter(1)))) + ((-(f1->GetParameter(0)*f1->GetParError(1))/(f1->GetParameter(1)*f1->GetParameter(1)))*(-(f1->GetParameter(0)*f1->GetParError(1))/(f1->GetParameter(1)*f1->GetParameter(1)))));
    // cout << "Ideal Y offsets for this Xoffset: " << -(f1->GetParameter(0))/(f1->GetParameter(1)) << " +/- " << Yoffseterr << endl;
    // t->Draw();
    //   delete f1;
    //   return;
    // }
    
  
    

    //Xit++;
    // Yit++;

    if(Xoffsets.at(i) == Xoffsets.back()) {
      //auto fn = new TF1("line1", "abs([0]*x-[1]) + [2]", 0,Xoffsets.back());
      auto MinReswidth = min_element(ResWidthsvec[Yoffsets.at(i)].begin(), ResWidthsvec[Yoffsets.at(i)].end());
      float MinimumResidualXval = distance(ResWidthsvec[Yoffsets.at(i)].begin(), MinReswidth);      
      auto minimizedXval = XoffsetsforeachY[Yoffsets.at(i)].at(MinimumResidualXval);
      //cout << minimizedXval << endl;
      
      auto fn = new TF1("line1", "pol2", minimizedXval-10,minimizedXval+10);      
      ResWidthsPerX[Yoffsets.at(i)]->Fit("line1", "R");
      //cout << "Parameter 1: " << fn->GetParameter(0) << endl;
      //cout << "Parameter 2: " << fn->GetParameter(1) << endl;
      //cout << "Parameter 3: " << fn->GetParameter(2) << endl;
      //cout << "Absolute Value Predicted vertex: " << fn->GetParameter(1)/fn->GetParameter(0) << endl;
      float idealX = -(fn->GetParameter(1))/(2*fn->GetParameter(2));
      auto xerror = sqrt((0.5*fn->GetParameter(1)*fn->GetParError(2)/(fn->GetParameter(2)*fn->GetParameter(2)))*(0.5*fn->GetParameter(1)*fn->GetParError(2)/(fn->GetParameter(2)*fn->GetParameter(2))) + (fn->GetParError(1)/(2*fn->GetParameter(2)))*(fn->GetParError(1)/(2*fn->GetParameter(2))));
      
      //auto idealX = fn->GetParameter(1)/fn->GetParameter(0);
      cout << "Ideal X offset for Yoffset " << Yoffsets.at(i) << " is " << idealX << " +/- " << xerror << endl;
      idealXvec.push_back(idealX);
      if(Yoffsets.at(i) == -1){
	string ht = to_string(Yoffsets.at(i));
	char cart[ht.size()];
	strcpy(cart, ht.c_str());
	auto h = new TCanvas(cart, "", 500, 500);
	ResWidthsPerX[Yoffsets.at(i)]->Draw("A*");
	ResWidthsPerX[Yoffsets.at(i)]->SetTitle("LAGD Residual Width for each X offset at Y = 0; X offset [mm]; LAGD Residual Width");

	string T3xlegendentry = "Fit X: ";
	//      T3xlegendentry += Form("%.2f",fn->GetParameter(1)/fn->GetParameter(0));
	T3xlegendentry += Form("%.2f",idealX);
	T3xlegendentry += " mm";
	char T3xcharlegendentry[T3xlegendentry.size()];
	strcpy(T3xcharlegendentry, T3xlegendentry.c_str());
      
	TLatex latexT3x;
	latexT3x.SetTextSize(0.035);
	latexT3x.SetTextAlign(13);  //align at top
	latexT3x.SetNDC();
	latexT3x.DrawLatex(0.5,.5,T3xcharlegendentry);
	h->Draw();
	
	return;
      }

      Yoffsets_h.push_back(Yoffsets.at(i));
      IdealXes.push_back(idealX);
      IdealXeserrs.push_back(xerror);
      Yerrors.push_back(0);
      
      //IdealXoffsetVsYoffset->SetPoint(Yit-1,Yoffsets.at(i), idealX, );
      IdealOffsets->SetPoint(idealoffsetiter, idealX, Yoffsets.at(i));
      idealoffsetiter++;
     
      
      // auto fm = new TF1("line5", "pol2", 0,Xoffsets.back());
      // DistDiffvsXoffset[Yoffsets.at(i)]->Fit("line5", "Q");
      
      // cout << fm->GetParameter(0) << ", " << fm->GetParameter(1) << ", " << fm->GetParameter(2) << endl;
      // auto c = fm->GetParameter(0);
      // auto b = fm->GetParameter(1);
      // auto a = fm->GetParameter(2);

      

      // cout << b*b - 4*a*c << endl;

      // float idealX2=0;
      // if(b*b - 4*a*c < 0){
      // 	// DistDiffvsXoffset[Yoffsets.at(i)]->Draw("A*");
      // 	// return;
      // 	idealX2 = -b/(2*a);
      // }
      // else{
      // 	idealX2 = min((-b-sqrt(b*b - 4*a*c))/(2*a), (-b+sqrt(b*b - 4*a*c))/(2*a));
      // }

      // idealX2vec.push_back(idealX2);
      // cout << "Ideal X2 offset for Yoffset " << Yoffsets.at(i) << " is " << idealX2 << endl;
      // widthdiffvsXoffset->SetPoint(Yit-1, Yoffsets.at(i),idealX2);
      //IdealOffsets->SetPoint(idealoffsetiter, idealX2, Yoffsets.at(i));
      //idealoffsetiter++;
      /*
      DistDiffvsXoffset[Yoffsets.at(i)]->Draw("A*");
      DistDiffvsXoffset[Yoffsets.at(i)]->SetTitle("Differrence in Width Between LAGD and Tracker Angular Distributions each X offset at Y = 0; X offset [mm]; Fit Angular Distribution - LAGD Angular Distribution [mRad]");
      string WidthDifflegendentry = "Fit X int: ";
      //      T3xlegendentry += Form("%.2f",fn->GetParameter(1)/fn->GetParameter(0));
      WidthDifflegendentry += Form("%.2f",idealX2);
      WidthDifflegendentry += " mm";
      char WidthDiffcharlegendentry[WidthDifflegendentry.size()];
      strcpy(WidthDiffcharlegendentry, WidthDifflegendentry.c_str());
      
      TLatex latexT3x;
      latexT3x.SetTextSize(0.035);
      latexT3x.SetTextAlign(13);  //align at top
      latexT3x.SetNDC();
      latexT3x.DrawLatex(0.7,.5,WidthDiffcharlegendentry);

      return;
      */
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

  auto IdealXoffsetVsYoffset = new TGraphErrors(Yoffsets_h.size(), Yoffsets_h.data(), IdealXes.data(), Yerrors.data(), IdealXeserrs.data());
  
  
  // IdealYoffsetVsXoffset->Draw("A*");
  //IdealYoffsetVsXoffset->SetTitle("Ideal Y offset for each X offset");
  //IdealYoffsetVsXoffset->GetXaxis()->SetTitle("X offset [mm]");
  //IdealYoffsetVsXoffset->GetYaxis()->SetTitle("Ideal Y offset [mm]");
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
  IdealYoffsetVsXoffset->GetYaxis()->SetTitle("Optimal Y offset [mm]");
  IdealYoffsetVsXoffset->SetTitle("Optimal Y offset Per X offset");
  
  TF1* f1 = new TF1("line1", "pol1",0,100);
  IdealYoffsetVsXoffset->Fit("line1", "Rrobust");
  //IdealYoffsetVsXoffset->Draw();
  //return;
  cout << "Fit Line eqn?: " << f1->GetParameter(1) << "x + " <<  f1->GetParameter(0) << endl;


  //return;
  
  //IdealXoffsetVsYoffset->Draw("A*");
  IdealXoffsetVsYoffset->GetYaxis()->SetTitle("Y offset [mm]");
  IdealXoffsetVsYoffset->GetXaxis()->SetRangeUser(0,70);
  IdealXoffsetVsYoffset->GetXaxis()->SetTitle("Optimal X offset [mm]");
  IdealXoffsetVsYoffset->SetTitle("Optimal X offset Per Y offset");//min_element(ResWidthsvec[Yoffsets.at(i)].begin(), ResWidthsvec[Yoffsets.at(i)].end());
  //TF1* f2 = new TF1("line2", "pol1",*min_element(idealXvec.begin(), idealXvec.end()),*max_element(idealXvec.begin(), idealXvec.end()));
  TF1* f2 = new TF1("line2", "pol1",*min_element(Yoffsets.begin(), Yoffsets.end()),*max_element(Yoffsets.begin(), Yoffsets.end()));
  //f2->SetParameter(0,9.4);
  //f2->SetParameter(1,100);
  
  IdealXoffsetVsYoffset->Fit("line2", "Rrobust");
  cout << "Fit Line eqn: " << f2->GetParameter(1) << "x + " <<  f2->GetParameter(0) << endl;
  auto newf2param0 = -f2->GetParameter(0)/f2->GetParameter(1);//  -b/a
  auto newf2param1 = 1/f2->GetParameter(1);
  
  
  float a = f1->GetParameter(1);
  float b = f1->GetParameter(0);
  float c = newf2param1;
  float d = newf2param0;
  
  float bestX = (d-b)/(a-c);
  float bestY = a*bestX + b;
  cout << "Ideal offsets from method 1: " << bestX << ", " << bestY << endl;
    
  
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
  

  auto lastcanv = new TCanvas("", "", 1500, 1000);
  lastcanv->Divide(3,1);
  lastcanv->cd(1);
  
  IdealYoffsetVsXoffset->Draw("A*");
  IdealYoffsetVsXoffset->GetXaxis()->SetTitle("X offset [mm]");
  IdealYoffsetVsXoffset->GetYaxis()->SetTitle("Optimal Y offset [mm]");
  IdealYoffsetVsXoffset->SetTitle("Optimal Y offset Per X offset");

  lastcanv->cd(2);
    
  IdealXoffsetVsYoffset->Draw("A*");
  IdealXoffsetVsYoffset->GetXaxis()->SetTitle("Y offset [mm]");
  //IdealXoffsetVsYoffset->GetXaxis()->SetRangeUser(0,150);
  IdealXoffsetVsYoffset->GetYaxis()->SetTitle("Optimal X offset [mm]");
  IdealXoffsetVsYoffset->SetTitle("Optimal X offset Per Y offset method 1");
  //return;
  /*
  DistDiffvsXoffset[10]->Draw("A*");
  DistDiffvsXoffset[10]->GetXaxis()->SetTitle("X offset [mm]");
  DistDiffvsXoffset[10]->GetYaxis()->SetTitle("#sigma_{Trackers} - #sigma_{LAGD} [Rad]");
  DistDiffvsXoffset[10]->SetTitle("Difference in Tracker and LAGD Angular Distributions at Y = 10 mm");
  */
  
  lastcanv->cd(3);
 
  
  IdealOffsets->Draw("A*");
  IdealOffsets->GetXaxis()->SetTitle("Optimal X offsets [mm]");
  IdealOffsets->GetYaxis()->SetTitle("Optimal Y offsets [mm]");
  IdealOffsets->SetTitle("Optimal X and Y offsets");

  float xmin = 0;
  float ymin = (xmin*f1->GetParameter(1)) +  f1->GetParameter(0);
  float xmax = 2;
  float ymax = (xmax*f1->GetParameter(1)) +  f1->GetParameter(0);
  
  TLine *liney = new TLine(xmin, ymin, xmax, ymax);
  liney->SetLineColor(kRed);  // Set line color to red
  liney->SetLineWidth(2);     // Set line thickness
  //liney->SetLineStyle(2);     // Dashed line
  liney->Draw("same");  // Draw the line on the same

  xmin = 1.01;
  ymin = (xmin*newf2param1) +  newf2param0;
  xmax = 1.09;
  ymax = (xmax*newf2param1) +  newf2param0;
  
  TLine *linex = new TLine(xmin, ymin, xmax, ymax);
  linex->SetLineColor(kRed);  // Set line color to red
  linex->SetLineWidth(2);     // Set line thickness
  //liney->SetLineStyle(2);     // Dashed line
  linex->Draw("same");  // Draw the line on the same
  
  
  
  lastcanv->Draw();
  return;
}

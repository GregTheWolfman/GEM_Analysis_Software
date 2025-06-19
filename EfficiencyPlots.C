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
#include "TGaxis.h"


void EfficiencyPlots(){


  
  const int n = 9;
  double Voltages[n] = {3729.1,3819,3845.9,3872.8,3899.9,3926.7,3952.7,3980.6,3998.5};
  double Imin = 682.7;
  double Imax = 732.4;

  //Efficiency numbers
  // double Eff[n] =  {97.4,97.1,97.5,97.8,97.9,97.1,96.3,97.5,97.8};

  // double EffErr[n] =  {0.56,0.48,0.41,0.41,0.36,0.41,0.97,0.37,0.35};
  // double VErr[n] = {0,0,0,0,0,0,0,0,0};

  //resolution numbers
  double Eff[n] =  {115.5, 79.8, 72.2, 72.0, 66.6, 63.4, 60.6, 57.8, 51.4};
  
  double EffErr[n] =  {1.85, 1.82, 1.7, 1.7, 1.6, 2.3, 3.0, 1.4, 1.2};
  
  double VErr[n] = {0,0,0,0,0,0,0,0,0};

  auto Plot = new TGraphErrors(n, Voltages, Eff, VErr, EffErr);

  
  
  auto canvas = new TCanvas("", "", 500, 500);
  TF1* f1 = new TF1("line1","expo + [2]");
  Plot->Fit("line1");
  Plot->Draw("A*");
  // Plot->SetTitle("LAGD Efficiency during HV scan");
  // Plot->GetYaxis()->SetRangeUser(60, 110);
  // Plot->GetYaxis()->SetTitle("LAGD Efficiency (%)");
  // Plot->GetXaxis()->SetTitle("Drift Voltage (V)");

  // auto EffCurrentaxis = new TGaxis(3700,100,4025,100,3700/5.46,4025/5.46,510,"-");	
  // EffCurrentaxis->SetTitle("Divider Current [#muA]");
  // EffCurrentaxis->SetLabelFont(42);
  // EffCurrentaxis->SetLabelSize(0.035);
  // EffCurrentaxis->SetTitleSize(0.035);
  // EffCurrentaxis->SetTitleOffset(1.5);
  
  // EffCurrentaxis->Draw();

  // TF1* f1 = new TF1("line1","pol1");
  // Plot->Fit("line1","Q");
  auto T1Maxis = new TGaxis(4000,50,4000,120,50/0.134,120/0.134,510,"+L");	
  T1Maxis->SetTitle("LMG Resolution [urad]");
  T1Maxis->SetLabelFont(42);
  T1Maxis->SetLabelSize(0.035);
  T1Maxis->SetTitleSize(0.035);
  T1Maxis->SetTitleOffset(1.5);
  
  T1Maxis->Draw();

  auto EffCurrentaxis = new TGaxis(3700,100,4025,100,3700/5.46,4025/5.46,510,"-");	
  EffCurrentaxis->SetTitle("Divider Current [#muA]");
  EffCurrentaxis->SetLabelFont(42);
  EffCurrentaxis->SetLabelSize(0.035);
  EffCurrentaxis->SetTitleSize(0.035);
  EffCurrentaxis->SetTitleOffset(1.5);
  
  EffCurrentaxis->Draw();
  
  //T2Maxis->Draw();

  
  Plot->SetTitle("LMG Resolution during HV scan");
  Plot->GetYaxis()->SetRangeUser(50, 120);
  Plot->GetYaxis()->SetTitle("LMG Resolution [um]");
  Plot->GetXaxis()->SetTitle("Drift Voltage [V]");

  string TA1legendentry3 = "Fit: exp(";
  TA1legendentry3 += Form("%.2f",f1->GetParameter(0));
  TA1legendentry3 += " + ";
  TA1legendentry3 += Form("%.2f",f1->GetParameter(1));
  TA1legendentry3 += " x) + ";
  TA1legendentry3 += Form("%.2f",f1->GetParameter(2));
  char TA1charlegendentry3[TA1legendentry3.size()+1];
  strcpy(TA1charlegendentry3, TA1legendentry3.c_str());

  // string TA1legendentry5 = "Radius: ";
  // TA1legendentry5 += Form("%.2f",LAGDrval);
  // TA1legendentry5 += " mm";
  // char TA1charlegendentry5[TA1legendentry5.size()+1];
  // strcpy(TA1charlegendentry5, TA1legendentry5.c_str());

  TLatex latexTA1;
  latexTA1.SetTextSize(0.035);
  latexTA1.SetTextAlign(13);  //align at top
  latexTA1.SetNDC();
  latexTA1.DrawLatex(0.35,.6,TA1charlegendentry3);
  //latexTA1.DrawLatex(0.6,.35,TA1charlegendentry5);
  //cout << "LAGD Residual  width: " << sigmaLAGD << endl;
  
  canvas->Draw();
  

  
  
  
  
  return;
}

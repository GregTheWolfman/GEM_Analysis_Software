//////////////////////////////////////////////////
//*********Iterative Double Gaussian Fit********//
//////////////////////////////////////////////////
//basic C++ commands
#include <vector>
#include <iostream>
#include <string>
#include "TROOT.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TDatime.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMarker.h"
#include "TString.h"
#include "TFile.h"
#include "TMath.h"
#include "TLine.h"
#include "TBranch.h"
#include "TTree.h"
#include "TPad.h"
#include "TChain.h"
#include "TSystem.h"
#include "TKey.h"
#include <cmath>
#include "TCanvas.h"
#include "TMath.h"
#include "TH1.h"
#include "TF1.h"
#include "TRandom.h"
#include "TSpectrum.h"
#include "TVirtualFitter.h"

using namespace std;

struct I2GFvalues
{
  float rchi2;
  float mean;
  float mean_err;
  float sigma;
  float sigma_err;
  TF1 *fit_func;
};

I2GFvalues I2GFmainLoop(TH1F *htemp, int N_iter, float N_sigma_range, bool ShowFit)
//Arguments: (histo to be fit, N iterations to find peak using gaus fit, fit range param., do or do not plot fit on histo)
{
  //TH1F *htemp = (TH1F*) htemp1->Clone();
  I2GFvalues myI2GFvalues;

  bool verbose = 0;

  //Set initial values...(in case fit fails)
  myI2GFvalues.rchi2 = -100;
  myI2GFvalues.mean = -100;
  myI2GFvalues.mean_err = -100;
  myI2GFvalues.sigma = -100;
  myI2GFvalues.sigma_err = -100;


  Int_t NPeaks ;
  TSpectrum *s= new TSpectrum(); //TSpectrum(1,1)->Argument: (Number of peaks to find, Distance to neighboring peak: "1"-->3sigma)
 
  //int NPeaks;
  Double_t *Peak;                     //TSpectrum *s = new TSpectrum(); --> No warning message 

  Double_t *PeakAmp;                    
  float peak_pos = 0;
  float peak_pos_amp = 0;  //Initial value, assuming positive going peaks
  int peak_pos_bin = 0;
  
  int binMaxCnt = 0;
  float binMaxCnt_value = 0;
  int binMaxCnt_counts = 0;

  int Nbins = 0;
  Int_t zero_value_bin = 0;
  float low_limit = 0;
  float high_limit = 0;
  float peak_pos_count = 0;
  float zero_bin_value = 0;
  float max_bin_value = 0;
 

  TF1 *func;
  TF1 *func1;
  TF1 *func2;
  TF1 *func3;
  float Chi2;
  int NDF = 1;
   

  float f_RChi2 = 1.0; 
  float f_const = 1.0;
  float f_mean = 1.0;
  float f_mean_err = 1.0;
  float f_sigma = 1.0;
  float f_sigma_err = 1.0;
  
  float peak = 1.0;  

  float f_const2 = 1.0;
  float f_mean2 = 1.0;
  float f_mean_err2 = 1.0;
  float f_sigma2 = 1.0;
  float f_sigma_err2 = 1.0;

  //---------Basic Histo Peak Finding Parameters----------
  binMaxCnt = htemp->GetMaximumBin();  //Finds bin w/ max counts (can be dubious, so use TSpectrum)
  binMaxCnt_value = (Double_t) htemp->GetXaxis()->GetBinCenter(binMaxCnt); //if the bin number is known and the bin value (in x-axis units) is wanted
  binMaxCnt_counts = (Int_t) htemp->GetBinContent(binMaxCnt); //Finds counts within particular bin

  if (verbose)
  cout<<"binMaxCnt = "<<binMaxCnt<<"\tbinMaxCnt_value = "<<binMaxCnt_value<<"\t binMaxCnt_counts = "<<binMaxCnt_counts<<endl;

  //---------TSpectrum Peak Finding Parameters--------
  if(ShowFit) NPeaks = s->Search(htemp, 2,"",0.5); 
  if (verbose)
      cout<<"NPeaks = "<<NPeaks<<endl;

  /*opens a canvas (one time in a loop), even with:  s->Search(htemp, 2, "nodraw", 0.9);  else  NPeaks = s->Search(htemp, 2, "", 0.5);  //s->Search(htemp, 2, "nodraw", 0.9);

  Npeaks = s->GetNPeaks(); //If using this, need pointer in declaration above: Int_t *NPeaks
  N_peaks =  Npeaks[0];
*/
  Peak = s->GetPositionX();
  PeakAmp = s->GetPositionY();


  if (verbose)
      cout<<"Peak = "<<Peak[0]<<"\tPeakAmp = "<<PeakAmp[0]<<endl;

  for (int i=0; i<NPeaks; i++)
    {
      if (peak_pos_amp < PeakAmp[i])
	{
	    peak_pos_amp = PeakAmp[i]; //TSpectrum finds peak counts
	    peak_pos = Peak[i]; //TSpectrum finds pos. of peak in x-axis units
	}
    }
  peak_pos_bin = htemp->GetXaxis()->FindBin(peak_pos); //if the bin value (in x-axis units) is known and the bin number is wanted
  peak_pos_count = htemp->GetBinContent(peak_pos_bin);  //counts in particular bin


  if (verbose)
      cout<<"peak_pos_bin = "<<peak_pos_bin<<"\tpeak_pos_count = "<<peak_pos_count<<endl;



  //------------------------------------------------------------------------------------------------------------------  

  zero_value_bin = htemp->GetXaxis()->FindBin(0.0); //if the bin value (in x-axis units) is known and the bin number is wanted
  Nbins = htemp->GetSize() - 2; //total number of bins in histo
  zero_bin_value =  htemp->GetXaxis()->GetBinCenter(0); //if the bin number is known and the bin value (in x-axis units) is wanted.
  max_bin_value =  htemp->GetXaxis()->GetBinCenter(Nbins); //if the bin number is known and the bin value (in x-axis units) is wanted.
 
  int TS = 0;
  if (peak_pos >= zero_bin_value  &&  peak_pos <= max_bin_value)  //Make sure that TSpectrum peak is within histo range
    {                                                             //if not, use Par initial values from Basic Histo Peak Find
             
      TS=1; //for cout below                        
      low_limit = peak_pos - (0.1 * abs(max_bin_value-zero_bin_value)); //peakpos-10% of full range of histo
      high_limit = peak_pos + (0.1 * abs(max_bin_value-zero_bin_value)); //peakpos+10% of full range of histo
                                                       
      func = new TF1("func", "gaus");
      //func->FixParameter(1,0);
      //func->SetParLimits(0, 0, 1000000);
      func->SetParameter(0, peak_pos_count);
      func->SetParameter(1, peak_pos);
    }
  else
    {

      low_limit = binMaxCnt_value - (0.1 * abs(max_bin_value-zero_bin_value)); //peakpos-10% of full range of histo
      high_limit = binMaxCnt_value + (0.1 * abs(max_bin_value-zero_bin_value)); //peakpos+10% of full range of histo
      
      func = new TF1("func", "gaus");
      //func->SetParLimits(0, 0, 1000000);
      func->SetParameter(0, binMaxCnt_counts);
      func->SetParameter(1, binMaxCnt_value);
    }
  
  htemp->Fit("gaus", "Q0", "", low_limit, high_limit); //low_limit, high_limit); //  To Show fit: htemp->Fit("gaus"); //better fit?-> Fit("gaus", "MQ", "", "", ""); 
  func  = htemp->GetFunction("gaus");
  Chi2 = func->GetChisquare();
  NDF = func->GetNDF();
  if (NDF != 0) f_RChi2 = Chi2/NDF;
  f_const = func->GetParameter(0);
  f_mean = func->GetParameter(1);
  f_mean_err = func->GetParError(1);
  f_sigma = func->GetParameter(2);
  f_sigma_err = func->GetParError(2);
 


 /*
  if (N_sigma_range == 7)
    {
      cout<<""<<endl;
      cout<<"---Basic Histo Peak Find---"<<endl;
      cout<<"binMaxCnt: "<<binMaxCnt<<endl;
      cout<<"binMaxCnt_value: "<<binMaxCnt_value<<endl;
      cout<<"binMaxCnt_counts: "<<binMaxCnt_counts<<endl;
      cout<<""<<endl;
      if (TS == 1) cout<<"---TSpectrum Peak Find: Succeeded!---"<<endl;
      else cout<<"---TSpectrum Peak Find: Failed!---"<<endl;
      cout<<"NPeaks: "<<NPeaks<<endl;
      cout<<"peak_pos (find max peak): "<<peak_pos<<endl;
      cout<<"peak_pos_amp (eval. amp of max peak) : "<<peak_pos_amp<<endl;
      cout<<""<<endl;
      cout<<"----------------------------------------"<<endl;
      cout<<"peak_pos_bin: "<<peak_pos_bin<<endl;
      cout<<"peak_pos_count: "<<peak_pos_count<<endl;
      cout<<"zero_value_bin: "<<zero_value_bin<<", Nbins: "<<Nbins<<endl;
      cout<<"zero_bin_value: "<<zero_bin_value<<", max_bin_value: "<<max_bin_value<<endl;
      cout<<"low, high limit: "<<low_limit<<", "<<high_limit<<endl;
    }
  */




  for (int i=0; i< (N_iter ); i++)
  //for (int i=0; i< 2; i++)  //8 seems to work well, so let's keep it constant here.
    {
      //htemp->Fit("gaus", "", "",(f_mean - (N_sigma_range*f_sigma)), (f_mean + (N_sigma_range*f_sigma) ) ); //show fit
      htemp->Fit("gaus", "Q0", "",(f_mean - (N_sigma_range*f_sigma)), (f_mean + (N_sigma_range*f_sigma) ) ); //don't show fit
      func  = htemp->GetFunction("gaus");
      Chi2 = func->GetChisquare();
      NDF = func->GetNDF();
      if (NDF != 0) f_RChi2 = Chi2/NDF;
      f_const = func->GetParameter(0);
      f_mean = func->GetParameter(1);
      f_mean_err = func->GetParError(1);
      f_sigma = func->GetParameter(2);
cout << " sigma " << f_sigma << endl;
      f_sigma_err = func->GetParError(2);
    }

  peak =  func->GetParameter(0); //Amplitude

  if (verbose)
      cout<<"peak = "<<peak<<endl;
  float bgd_h = 0.25; //background height ~ bgd_h*gaus_amp

  func1 = new TF1("func1", "gaus",(f_mean - 3*f_sigma), (f_mean + 3*f_sigma));
  //-----------Fit Parameter constraints not needed here (see below):
  //func1->SetParLimits(0, 0, 1000000);
  //func1->SetParameters(f_const, f_mean, f_sigma);    //Here: (Par1 initial value, Par2 initial value, Par3 initial value, etc)
  //func1->SetParameter(0, 200);  //Here: Initial value of Par 0 = 200
  //func1->SetParLimits(0, f_const*(1-bgd_h-0.2),f_const*(1-bgd_h+0.2) );  //Here: (Par_lower limit, Par upper limit)
  //func1->SetParLimits(1, f_mean-(f_sigma/2),f_mean+(f_sigma/2) );
  //func1->SetParLimits(2, f_sigma-(f_sigma/2),f_sigma+(f_sigma/2) );
  
  func2 = new TF1("func2", "gaus",(f_mean - 3*f_sigma), (f_mean + 3*f_sigma));
  //-----------Fit Parameter constraints not needed here (see below):
  //func2->SetParameters(f_const/10, f_mean, 4*f_sigma); //(const, mean, sigma)     
  //func2->SetParLimits(0,-1,(0.1*peak_pos_amp) );        //(const)     
  //func2->SetParameters(1, f_mean, 4*f_sigma); //(mean)      
  //func2->SetParameters(2, 4*f_sigma);         //(sigma)     
  //func2->SetParLimits(0, 0, f_const*(0.1) );                                      
  //func2->SetParLimits(1, f_mean-(4*f_sigma),f_mean+(4*f_sigma) );
  //func2->SetParLimits(2, f_sigma-(10*f_sigma),f_sigma+(10*f_sigma) ); 
  //func2->FixParameter(0, 40); //Test     

  //cout<<"peak amp: "<<0.1*peak_pos_amp<<endl;

  //func2 = new TF1("func2", "pol2");
  
  if (verbose)
      cout<<"f_mean = "<<f_mean<<"\tf_sigma = "<<f_sigma<<endl;

  //func3 = new TF1("func3", "func1 + func2", (f_mean - 3*f_sigma), (f_mean + 3*f_sigma) );
  func3 = new TF1("func3","gaus(0) + gaus(3)",(f_mean - 3*f_sigma), (f_mean + 3*f_sigma));
  //func3 = new TF1("func3", "func1 ", (f_mean - 3*f_sigma), (f_mean + 3*f_sigma) );

  if (verbose)
      cout<<"ERROR 1"<<endl;
  //-----------Fit Parameter constraints:
  func3->SetParameters(f_const, f_mean, f_sigma, f_const/10, f_mean, 4*f_sigma); //Set Initial Valules
  //func3->SetParLimits(0, 0, 1000000);
  //func3->SetParLimits(1, 0, 1000000);
  //func3->SetParLimits(2, 0, 1000000);
  func3->SetParLimits(3, 0, (0.05*peak_pos_amp) ); //Max=5% of peak amp //************Set peak limit of background sigma*************
  //func3->SetParLimits(4, 0, 1000000);
  //func3->SetParLimits(5, 0, 1000000);

  //htemp->Fit("func3");//, "", "",(f_mean - (N_sigma_range*f_sigma)), (f_mean + (N_sigma_range*f_sigma) ) ); //Show Fit
  if (verbose)
      cout<<"ERROR 2"<<endl;
  htemp->Fit("func3", "Q0"); //Don't show fit
  if (verbose)
      cout<<"ERROR 3"<<endl;
  func  = htemp->GetFunction("func3");
  Chi2 = func3->GetChisquare();
  NDF = func->GetNDF();
  if (verbose)
      cout<<"Chi2 = "<<Chi2<<endl;
  if (NDF != 0) f_RChi2 = Chi2/NDF;
  f_const = func3->GetParameter(0);
  f_mean = func3->GetParameter(1);
  f_mean_err = func3->GetParError(1);
  f_sigma = func3->GetParameter(2);
  f_sigma_err = func3->GetParError(2);
  
  f_const2 = func3->GetParameter(3);
  f_mean2 = func3->GetParameter(4);
  f_mean_err2 = func3->GetParError(4);
  f_sigma2 = func3->GetParameter(5);
  f_sigma_err2 = func3->GetParError(5);
  func3->SetParNames("Primary Constant", "Primary Mean", "Primary Sigma", "Background Constant", "Background Mean", "Background Sigma");


  if (verbose)
      cout<<"f_mean = "<<f_mean<<"\tf_mean_err = "<<f_mean_err<<endl;
  //for (int j=0; j< (N_iter - 1); j++)
  for (int j=0; j<4; j++)
    {
      func3->SetParameters(f_const, f_mean, f_sigma, f_const2, f_mean2, f_sigma2);
      //htemp->Fit("func3", "Q", "", "",(f_mean - (N_sigma_range*f_sigma)), (f_mean + (N_sigma_range*f_sigma) ) ); 
      
      //----------------Show or don't show fit----------------- 
      if (ShowFit) htemp->Fit("func3");//*************Show Histo & Fit in quiet mode
      //if (ShowFit) htemp->Fit("func3", "Q");//*************Show Histo & Fit in quiet mode
      else htemp->Fit("func3", "Q0"); //*****************Don't show Histo & Fit in quiet mode
      //-------------------------------------------------------     

      func3  = htemp->GetFunction("func3");
      func3->SetLineColor(2);
      //htemp->SetLineColor(2);
      Chi2 = func3->GetChisquare();
      NDF = func3->GetNDF();
      if (NDF != 0) f_RChi2 = Chi2/NDF;
      f_const = func3->GetParameter(0);
      f_mean = func3->GetParameter(1);
      f_mean_err = func3->GetParError(1);
      f_sigma = func3->GetParameter(2);
      f_sigma_err = func3->GetParError(2);
      f_const2 = func3->GetParameter(3);
      f_mean2 = func3->GetParameter(4);
      f_mean_err2 = func3->GetParError(4);
      f_sigma2 = func3->GetParameter(5);
      f_sigma_err2 = func3->GetParError(5);
    }
  if (verbose)
      cout<<"ERROR 4"<<endl;

  

  //gStyle->SetOptFit(kTRUE);

  //if (f_const > f_const2)
  if (abs(f_const-peak) < abs(f_const2-peak))
    {
      myI2GFvalues.rchi2 = f_RChi2;
      myI2GFvalues.mean = f_mean;
      myI2GFvalues.mean_err = f_mean_err;
      myI2GFvalues.sigma = abs(f_sigma);
      myI2GFvalues.sigma_err = f_sigma_err;
      //myI2GFvalues.fit_func = new TF1("fit_func", "func3"); //not needed
    }
  else
    {
      myI2GFvalues.rchi2 = f_RChi2;
      myI2GFvalues.mean = f_mean2;
      myI2GFvalues.mean_err = f_mean_err2;
      myI2GFvalues.sigma = abs(f_sigma2);
      myI2GFvalues.sigma_err = f_sigma_err2;
      //myI2GFvalues.fit_func = new TF1("fit_func", "func3"); //not needed
    }
  if (verbose)
      cout<<"ERROR 5"<<endl;
  
  return myI2GFvalues;

  delete s;
  delete func;
  delete func1;
  delete func2;
  delete func3;
    
}

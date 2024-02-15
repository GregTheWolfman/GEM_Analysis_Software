#ifndef SRSTRACKFIT_H
#define SRSTRACKFIT_H 
/*******************************************************************************
*  AMORE FOR SRS - SRS                                                         *
*  SRSTrackFit                                                                 *
*  SRS Module Class                                                            *
*  Author: Kondo GNANVO 01/05/2011                                             *
*******************************************************************************/

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <map>
#include <vector>
#include <TMath.h>
#include <iostream>
#include <TObject.h>
#include "TCanvas.h"
#include "TH1F.h"
#include "TString.h"

#include "TROOT.h"
#include "TFitResult.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TDatime.h"

#include "TRandom.h"
#include "TStyle.h"
#include "TLatex.h"

#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1I.h"
#include "TH2I.h"
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
#include "TObject.h"
#include "TString.h"
#include "TKey.h"

#include "TColor.h"
#include "TNtuple.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TPRegexp.h"
#include "TSpectrum.h"
#include "TASImage.h"

#endif

#define PI 3.14159265359 

using namespace std;

class SRSTrackFit : public TObject {

public:

  SRSTrackFit(map<TString, vector<Float_t> > trackerData, map<TString, vector<Float_t> > rawData);
  ~SRSTrackFit();

  void LeastSquareLinearFitTrack() ;

  map<TString, Float_t> GetFitParameters() { return fFitParameters ; }

  map<TString, vector<Float_t> > GetTrackerData()    { return fTrackerData ;}
  map<TString, vector<Float_t> > GetTrackFittedData() { return fTrackFittedData ;}

  Float_t GetSmallNumber()  { return smallNumber ;}
  void setSmallNumber(Float_t small_number) { smallNumber = small_number ;}

  Float_t convertRadianToDegree(Float_t angle_rad)  { return (Float_t) ((180*angle_rad) / PI);}
  Float_t convertDegreeToRadian(Float_t angle_deg)  { return (Float_t) ((PI*angle_deg) / 180);}

  Float_t abs(Float_t x) ;
  Float_t normVec(const vector<Float_t> u) ;
  Float_t dotVec(const vector<Float_t> u ,const vector<Float_t> v) ;
  Float_t getAngleTo(const vector<Float_t> u, const vector<Float_t> v) ;
  Float_t projectedAngleXY(const vector<Float_t> u, TString xORy) ;
  Float_t projectedAngleXZ(const vector<Float_t> u, TString xORz) ;

  const vector<Float_t> prodVec(Float_t a , const vector<Float_t> u) ;
  const vector<Float_t> addVec(const vector<Float_t> u ,const vector<Float_t> v) ;
  const vector<Float_t> subVec(const vector<Float_t> u ,const vector<Float_t> v) ;
  const vector<Float_t> getDirection(const vector<Float_t> u) ;
  const vector<Float_t> directionVectorFrom2Points(const vector<Float_t> u, const vector<Float_t> v) ;
  const vector<Float_t> getXandYKnowingZ(const vector<Float_t> w, const vector<Float_t> v, Float_t z0) ;

  void PlaneRotationAlongZaxis(Float_t alpha, vector<Float_t> & u) ;
  Float_t GetAngleBetweenTwoTracks(map<TString, vector<Float_t> > firstTrack, map<TString, vector<Float_t> > secondTrack) ;

  void ClearTracks();

private:

  Float_t smallNumber ; 
  map<TString, vector<Float_t> > fTrackerData, fTrackFittedData, fRawData;
  map<TString, Float_t> fFitParameters; 
  ClassDef(SRSTrackFit,1)  
};

#endif

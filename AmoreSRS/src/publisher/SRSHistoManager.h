#ifndef __SRSHISTO__
#define __SRSHISTO__
/*******************************************************************************
*  AMORE FOR SRS - SRS                                                         *
*  SRSHistoManager                                                             *
*  SRS Module Class                                                            *
*  Author: Kondo GNANVO 18/08/2010                                             *
*******************************************************************************/

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include "event.h"
#include <cmath>

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
#include "TProfile.h"
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

#include "SRSHit.h"
#include "SRSTrack.h"
#include "SRSMapping.h"
#include "SRSAPVEvent.h"
#include "SRSPedestal.h"
#include "SRSTrackFit.h"
#include "SRSRawPedestal.h"
#include "SRSEventBuilder.h"
#include "SRSFECEventDecoder.h"
#include "SRSTrackBasedEfficiency.h"

#endif
#define NCH 128  
#define PI 3.14159265359 

using namespace std;

class SRSHistoManager : public TObject {

 public:
  SRSHistoManager(const char * histoCfgname, Int_t minClustSize, Int_t maxClustSize, Int_t maxClustMult, Int_t minPadClustSize, Int_t maxPadClustSize, Int_t maxPadClustMult);
  //  SRSHistoManager(const char * histoCfgname, Int_t minClustSize, Int_t maxClustSize, Int_t maxClustMult);
  SRSHistoManager(){;}
  ~SRSHistoManager();

  void SaveAllHistos();
  void BookHitTimingHistos() ;
  void Book1DHistos(TString name, TString title, TString type, TString sigName);
  void Book1DHistos(TString name, TString title, TString type, TString sigName1, TString sigName2);
  void Book2DHistos(TString name, TString title, TString type, TString sigName);
  void Book2DHistos(TString name, TString title, TString type, TString sigName1, TString sigName2);
  void ZeroSupAnalysis(SRSTrackBasedEfficiency * track, SRSEventBuilder * eventbuilder, const eventHeaderStruct * eventHeader);
  void PedSubAnalysis(SRSEventBuilder * eventbuilder, const eventHeaderStruct * eventHeader);

  void BookEvent3DDisplayNtuple(SRSTrack * track) ;
  void Event3DDisplay(SRSTrack * track, SRSEventBuilder * eventbuilder) ;
  void SaveNTuple() ;

  void BookCalibrationHistos(SRSTrack * track) ;
  void CalibrationRun(SRSTrack * track, SRSEventBuilder * eventbuilder, const eventHeaderStruct * eventHeader) ;
  void SaveCalibrationROOTFile(SRSTrack * track) ;

  void BookRawDataHistos() ;
  void RawDataRun(SRSEventBuilder * eventbuilder) ;

  void BookTrackingHistos(SRSTrack * track) ;
  void TrackingRun(SRSTrack * track, SRSEventBuilder * eventbuilder) ;
  void SaveTrackingROOTFile(SRSTrack * track) ;

  void BookTrackBasedEfficiencyHistos(SRSTrackBasedEfficiency * track) ;
  void TrackBasedEfficiencyRun(SRSTrackBasedEfficiency * track, SRSEventBuilder * eventbuilder) ;
  void SaveTrackBasedEfficiencyROOTFile(SRSTrackBasedEfficiency * track) ;


  void GaussFit(TH1F* h) ;
  void LandauFit(TH1F* h) ;
  void DoubleGaussFit(TH1F *h, Int_t N_iter, Float_t N_sigma_range, Bool_t ShowFit) ;

  void GaussFit(TH1S* h) ;
  void LandauFit(TH1S* h) ;
  void DoubleGaussFit(TH1S *h, Int_t N_iter, Float_t N_sigma_range, Bool_t ShowFit) ;

  vector < Float_t > CartesianToCylindricalCoordinates(Float_t offsetx, Float_t offsety,  vector <Float_t> cartesianCoordinates ) ;
  void TrackFit(SRSTrack * track) ;
  Float_t PlaneRotationAngle(vector <Float_t> referencePoint,  vector <Float_t> rotationPoint) ;

  void DumpList();
  void ResetHistos();
  void RefreshHistos();
  void DeleteHistos();
  void DeleteROOTObjs() ;

  void ClearMaps();
  void ClearMapOfHistos( map<TString,  TH1* > mapOfHistos) ;
  void ClearMapOfVectors( map<TString, vector <TString> > mapOfVectors) ;

  void DeleteListOfClusters(TList * listOfClusters) ;

  void GetStyle(Bool_t setStats) ;
  void ReadHistoCfgFile(const char * histoCfgname) ;

  void SetHistoTitle(TString histoName, Int_t goodEvents, Int_t trigger) ;
  void SetHistoTitle(TString histoName) ;
  void SetRunParameters(TString amoreAgentId, TString runtype, TString outputdir, TString outputfile);
  void SetEventNumber(Int_t eventNb) { fEventNumber = eventNb;}

  void SetBinning(TString type, TString detPlane1, TString detPlane2);

  void Reset1DHistBinning(TString type, Int_t nbin, Float_t min, Float_t max);
  void Reset2DHistBinning(TString type, Int_t nbin, Float_t min, Float_t max, Int_t nbin2, Float_t min2, Float_t max2) ;

  Int_t GetPedestalOffset(SRSAPVEvent * apvEv, SRSPedestal * ped) ;

  TString GetVarType(TString name) {return fVarType[name];}

  map<TString, TObject*> GetAllObjectsToBePublished()        { return fObjToDraw;}
  map<TString, TH1*>     GetResidualHistosToBePublished()    { return fResidualHistos;}
  map<TString, TH1*>     GetCalibrationHistosToBePublished() { return fCalibrationHistos ;}
  map<TString, TH1*>     GetEfficiencyHistosToBePublished() { return  fEfficiencyHistos;}

  TNtuple* GetNtupleToBePublished()  {return fNtuple;}

 private:

  SRSMapping * fMapping ;

  TString fRunType, fOutputDir, fOutputFile, fAmoreAgentID;
  Float_t fPreviousAdcCount ;

  vector<TH1*> fHist1dToReset;
  vector<TH2*> fHist2dToReset;

  TNtuple * fNtuple ;
  TFile * fOffsetFile ;

  TList *   fListOfClustersInPlane, * fListOfHitsInPlane;

  map<TString, TH1*> fEfficiencyHistos, fResidualHistos, fCalibrationHistos;
  map<TString, TObject*> fObjToDraw; 

  map<TString, Int_t> fNbOfEvents ;

  map<TString, TString> fObjType, fVarType, fObjTitle;
  map<TString, TString> fDetector, fDetectorPlane1;
  map<TString, TString> fDetectorPlane2, fFitFunction, fFitParam1Str, fFitParam2Str, fDividedHistos ;

  map<TString, vector <TString> > fFittedHistos ;
  map<TString, vector <TString> > fHistoTypeToDetectorMap ;

  map<TString, Int_t> fAPVKeyStr;
  map<TString, Int_t> fAPVIDStr;
  map<Int_t, TString> fNameAPVIDMap;

  Int_t   fNBin;        
  Float_t fRangeMin, fRangeMax; 

  Int_t   fNBin2;       
  Float_t fRangeMin2, fRangeMax2;

  Int_t   fNBinPlane;   
  Float_t fRangeMinPlane, fRangeMaxPlane;

  Int_t   fNBinTime;    
  Float_t fRangeMinTime, fRangeMaxTime;

  Int_t   fNBinSpectrum;  
  Float_t fRangeMinSpectrum, fRangeMaxSpectrum ;

  Int_t   fNBinMap;   
  Float_t fRangeMinMap, fRangeMaxMap ;

  Int_t   fNBinMap2;   
  Float_t fRangeMinMap2, fRangeMaxMap2 ;

  Int_t   f1DHistNBin ;
  Float_t f1DHistMinRange, f1DHistMaxRange;
 
  Float_t fNtupleSizeX, fNtupleSizeY, fNtupleSizeZ ;
  Int_t   fHistosRatioCut, fClustSizeRange, fMinClustSize, fMaxClustSize, fMaxClustMult;
  Int_t   fMinPadClustSize, fMaxPadClustSize, fMaxPadClustMult;
  Bool_t  fIsFirstEvent ;       

  unsigned long long fTriggerPattern;
  UInt_t  fEventType;

  Int_t fEventNumber ;
  Float_t fRChi2, fMean, fMeanError, fSigma, fSigmaError ; 

  vector<TString>  fDetPlaneNameList ; 
  ClassDef(SRSHistoManager, 1)
};


#endif

#ifndef __SRSCONFIGURATION__
#define __SRSCONFIGURATION__
/*******************************************************************************
*  AMORE FOR SRS - SRS                                                         *
*  SRSConfiguration                                                            *
*  SRS Module Class                                                            *
*  Author: Kondo GNANVO 18/08/2010                                             *
*******************************************************************************/

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <fstream>
#include <iostream>
#include "TObject.h"
#include "TH1F.h"
#include "TSystem.h"
#endif

// Class containing all configuration parameters for configuration from file(s)
using namespace std;

class SRSConfiguration : public TObject {

 public:
  SRSConfiguration();
  SRSConfiguration(const char * file);
  ~SRSConfiguration();

  void Init(const char * file = 0);
  bool FileExists(const char* name) const;

  const char * GetCycleWait() const {return gSystem->ExpandPathName(fCycleWait.Data());}
  void  SetCycleWait(const char * name) {fCycleWait = name;}
  //
  const char * GetRunNbFile() const {return gSystem->ExpandPathName(fRunNbFile.Data());}
  const char * GetRunType() const {return gSystem->ExpandPathName(fRunType.Data());}
  const char * GetROOTDataType() const {return gSystem->ExpandPathName(fROOTDataType.Data());}
  void  SetRunType(const char * name) {fRunType = name;}
  void  SetRunNbFile(const char * name) {fRunNbFile = name;}
  void  SetROOTDataType(const char * name) {fROOTDataType = name;}
  //
  const char * GetOutputDir() const {return gSystem->ExpandPathName(fOutputDir.Data());}
  const char * GetOutputFile() const {return gSystem->ExpandPathName(fOutputFile.Data());}
  void  SetOutputDir(const char * name) {fOutputDir = name;}
  void  SetOutputFile(const char * name) {fOutputFile = name;}
  //
  const char * GetZeroSupCut() const {return gSystem->ExpandPathName(fZeroSupCut.Data());}
  const char * GetPadZeroSupCut() const {return gSystem->ExpandPathName(fPadZeroSupCut.Data());}
  void  SetZeroSupCut(const char * name) {fZeroSupCut = name;}
  void  SetPadZeroSupCut(const char * name) {fPadZeroSupCut = name;}
  //
  const char * GetMaskedChannelCut() const {return gSystem->ExpandPathName(fMaskedChannelCut.Data());}
  const char * GetAPVGainCalibrationFile() const {return gSystem->ExpandPathName(fAPVGainCalibrationFile.Data());}
  void  SetMaskedChannelCut(const char * name) {fMaskedChannelCut = name;}
  void  SetAPVGainCalibrationFile(const char * name) {fAPVGainCalibrationFile = name;}
  //    
  const char * GetStartEventNumber() const {return gSystem->ExpandPathName(fStartEventNumber.Data());}
  const char * GetEventFrequencyNumber() const {return gSystem->ExpandPathName(fEventFrequencyNumber.Data());}
  void  SetStartEventNumber(const char * name) {fStartEventNumber = name;}
  void  SetEventFrequencyNumber(const char * name) {fEventFrequencyNumber = name;}
  // 
  const char * GetClusterMinSize() const {return gSystem->ExpandPathName(fClusterMinSize.Data());}
  const char * GetClusterMaxSize() const {return gSystem->ExpandPathName(fClusterMaxSize.Data());}
  const char * GetClusterMinADCs() const {return gSystem->ExpandPathName(fClusterMinADCs.Data());};
  const char * GetClusterMaxMultiplicity() const {return gSystem->ExpandPathName(fClusterMaxMultiplicity.Data());}
  void  SetClusterMinSize(const char * name) {fClusterMinSize = name;}
  void  SetClusterMaxSize(const char * name) {fClusterMaxSize = name;}
  void  SetClusterMinADCs(const char * name) { fClusterMinADCs = name;}
  void  SetClusterMaxMultiplicity(const char * name) {fClusterMaxMultiplicity = name;}
  //
  const char * GetPadClusterMinSize() const {return gSystem->ExpandPathName(fPadClusterMinSize.Data());}
  const char * GetPadClusterMaxSize() const {return gSystem->ExpandPathName(fPadClusterMaxSize.Data());}
  const char * GetPadClusterMinADCs() const {return gSystem->ExpandPathName(fPadClusterMinADCs.Data());};
  const char * GetPadClusterMaxMultiplicity() const {return gSystem->ExpandPathName(fPadClusterMaxMultiplicity.Data());}
  void  SetPadClusterMinSize(const char * name) {fPadClusterMinSize = name;}
  void  SetPadClusterMaxSize(const char * name) {fPadClusterMaxSize = name;}
  void  SetPadClusterMinADCs(const char * name) {fPadClusterMinADCs = name;}
  void  SetPadClusterMaxMultiplicity(const char * name) {fPadClusterMaxMultiplicity = name;}
  //
  const char * GetMappingFile() const {return gSystem->ExpandPathName(fMappingFile.Data());}
  const char * GetPadMappingFile() const {return gSystem->ExpandPathName(fPadMappingFile.Data());}
  const char * GetSavedMappingFile() const {return gSystem->ExpandPathName(fSavedMappingFile.Data());}
  void  SetMappingFile(const char * name) {fMappingFile = name;}
  void  SetPadMappingFile(const char * name) {fPadMappingFile = name;}
  void  SetSavedMappingFile(const char * name) {fSavedMappingFile = name;}
  //
  const char * GetHistoCfgName() const {return gSystem->ExpandPathName(fHistosFile.Data());}
  const char * GetDisplayCfgName() const {return gSystem->ExpandPathName(fDisplayFile.Data());}
  void  SetHistoCfgName(const char * name) {fHistosFile = name;}
  void  SetDisplayCfgName(const char * name) {fDisplayFile = name;}
  //
  const char * GetPedestalFile() const {return gSystem->ExpandPathName(fPedestalFile.Data());}
  const char * GetRawPedestalFile() const {return gSystem->ExpandPathName(fRawPedestalFile.Data());}
  void  SetPedestalFile(const char * name) {fPedestalFile = name;}
  void  SetRawPedestalFile(const char * name) {fRawPedestalFile = name;}
  //
  const char * GetTrackingOffsetDir() const {return gSystem->ExpandPathName(fTrackingOffsetDir.Data());}
  void  SetTrackingOffsetDir(const char * name) {fTrackingOffsetDir = name;}
  //
  const char * GetClusterPositionCorrectionFile() const {return gSystem->ExpandPathName(fPositionCorrectionFile.Data());}
  const char * GetClusterPositionCorrectionFlag() const {return gSystem->ExpandPathName(fPositionCorrectionFlag.Data());}
  void  setClusterPositionCorrectionFile(const char * name) {fPositionCorrectionFile = name;}
  void  setClusterPositionCorrectionFlag(const char * name) {fPositionCorrectionFlag = name;}
  //
  const char * GetClusterMaxOrTotalADCs() const {return gSystem->ExpandPathName(fIsClusterMaxOrTotalADCs.Data());};
  const char * GetHitMaxOrTotalADCs() const {return gSystem->ExpandPathName(fIsHitMaxOrTotalADCs.Data());};
  void  SetHitMaxOrTotalADCs(const char * name) {fIsHitMaxOrTotalADCs  = name;}
  void  SetClusterMaxOrTotalADCs(const char * name) {fIsClusterMaxOrTotalADCs  = name;}
  //
  const char * GetCrossTalkRemovalFlag() const {return gSystem->ExpandPathName(fCrossTalkRemovalFlag.Data());};
  const char * GetCrossTalkMaxClustSize() const {return gSystem->ExpandPathName(fCrossTalkMaxClustSize.Data());};
  void  SetCrossTalkRemovalFlag(const char * name) {fCrossTalkRemovalFlag = name;}
  void  SetCrossTalkMaxClustSize(const char * name) {fCrossTalkMaxClustSize = name;}
  //
  const char * GetSignalPeakMinTimeBin() const {return gSystem->ExpandPathName(fSignalPeakMinTimeBin.Data());};
  const char * GetSignalPeakMaxTimeBin() const {return gSystem->ExpandPathName(fSignalPeakMaxTimeBin.Data());};
  void  SetSignalPeakMinTimeBin(const char * name) { fSignalPeakMinTimeBin = name;}
  void  SetSignalPeakMaxTimeBin(const char * name) { fSignalPeakMaxTimeBin = name;}

  Bool_t Load(const char * filename);
  void Save(const char * filename) const; 
  void Dump() const;

  SRSConfiguration & operator=(const SRSConfiguration &rhs);

 private:
  void BookArrays();
  void SetDefaults();

  TString fRunType, fROOTDataType;
  TString fStartEventNumber, fEventFrequencyNumber, fCycleWait;
  TString fZeroSupCut, fPadZeroSupCut, fMaskedChannelCut;
  TString fOutputDir, fTrackingOffsetDir;
  TString fMappingFile, fPadMappingFile, fSavedMappingFile, fRunNbFile, fOutputFile, fDisplayFile, fHistosFile, fPedestalFile, fRawPedestalFile; 
  TString fPositionCorrectionFile, fPositionCorrectionFlag, fAPVGainCalibrationFile;
  TString fCrossTalkRemovalFlag, fCrossTalkMaxClustSize;
  TString fPadClusterMaxSize, fPadClusterMinSize, fPadClusterMinADCs, fPadClusterMaxMultiplicity;
  TString fClusterMaxSize, fClusterMinSize, fClusterMinADCs, fClusterMaxMultiplicity;
  TString fIsHitMaxOrTotalADCs, fIsClusterMaxOrTotalADCs, fSignalPeakMaxTimeBin, fSignalPeakMinTimeBin;

  ClassDef(SRSConfiguration, 1)
};

#endif

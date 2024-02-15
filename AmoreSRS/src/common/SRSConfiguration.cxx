#include "SRSConfiguration.h"
ClassImp(SRSConfiguration);

SRSConfiguration::SRSConfiguration() {
  Init();
}
SRSConfiguration::SRSConfiguration(const char * file) {
  Init(file);
}
SRSConfiguration::~SRSConfiguration(){
}

bool SRSConfiguration::FileExists(const char* name) const {
ifstream f(gSystem->ExpandPathName(name));
 if (f.good()) {
   f.close();
   return true;
 } 
 else {
   f.close();
   return false;
 }
}


//============================================================================================
SRSConfiguration & SRSConfiguration::operator=(const SRSConfiguration &rhs) {
  fCycleWait                  = rhs.GetCycleWait() ;
  fRunType                    = rhs.GetRunType() ;
  fOutputDir                  = rhs.GetOutputDir() ;
  fOutputFile                 = rhs.GetOutputFile() ;
  fROOTDataType               = rhs.GetROOTDataType();
  fRunNbFile                  = rhs.GetRunNbFile();
  fMappingFile                = rhs.GetMappingFile();
  fPadMappingFile             = rhs.GetPadMappingFile();
  fSavedMappingFile           = rhs.GetSavedMappingFile();
  fHistosFile                 = rhs.GetHistoCfgName();
  fDisplayFile                = rhs.GetDisplayCfgName();
  fPedestalFile               = rhs.GetPedestalFile() ;
  fTrackingOffsetDir          = rhs.GetTrackingOffsetDir();
  fPositionCorrectionFile     = rhs.GetClusterPositionCorrectionFile() ;
  fPositionCorrectionFlag     = rhs.GetClusterPositionCorrectionFlag() ;
  fMaskedChannelCut           = rhs.GetMaskedChannelCut() ;
  fStartEventNumber           = rhs.GetStartEventNumber() ;
  fEventFrequencyNumber       = rhs.GetEventFrequencyNumber() ;
  fRawPedestalFile            = rhs.GetRawPedestalFile() ;
  fZeroSupCut                 = rhs.GetZeroSupCut() ;
  fClusterMinSize             = rhs.GetClusterMinSize() ;
  fClusterMaxSize             = rhs.GetClusterMaxSize() ;
  fClusterMinADCs             = rhs.GetClusterMinADCs() ;
  fClusterMaxMultiplicity     = rhs.GetClusterMaxMultiplicity() ;
  fPadZeroSupCut              = rhs.GetPadZeroSupCut() ;
  fPadClusterMinSize          = rhs.GetPadClusterMinSize() ;
  fPadClusterMaxSize          = rhs.GetPadClusterMaxSize() ;
  fPadClusterMinADCs          = rhs.GetClusterMinADCs() ;
  fPadClusterMaxMultiplicity  = rhs.GetPadClusterMaxMultiplicity() ;
  fIsHitMaxOrTotalADCs        = rhs.GetHitMaxOrTotalADCs() ;
  fIsClusterMaxOrTotalADCs    = rhs.GetClusterMaxOrTotalADCs() ;
  fAPVGainCalibrationFile     = rhs.GetAPVGainCalibrationFile() ;
  fCrossTalkRemovalFlag       = rhs.GetCrossTalkRemovalFlag() ;
  fCrossTalkMaxClustSize      = rhs.GetCrossTalkMaxClustSize() ;
  fSignalPeakMaxTimeBin       = rhs.GetSignalPeakMaxTimeBin() ;
  fSignalPeakMinTimeBin       = rhs.GetSignalPeakMinTimeBin() ;
  return *this;
}

//============================================================================================
void SRSConfiguration::Init(const char * file) {
  if(!file) {
    Warning("Init", "conf file not specified. Setting defaults." );
    SetDefaults();
  }
  else {
    if(!Load(file)) {
      Warning("Init", "Cannot open conf file. Setting defaults." );
      SetDefaults();
    }
  }
}

//============================================================================================
void SRSConfiguration::SetDefaults() {
  fRunType                    = "RAWDATA" ;
  fOutputDir                  = "data" ;
  fOutputFile                 = "output_";
  fCycleWait                  = "1" ;
  fMaskedChannelCut           = "20" ;
  fZeroSupCut                 = "3" ;
  fClusterMaxSize             = "10" ;
  fClusterMinSize             = "1" ;
  fClusterMinADCs             = "200";
  fClusterMaxMultiplicity     = "1" ;
  fPadZeroSupCut              = "1" ;
  fPadClusterMaxSize          = "16" ;
  fPadClusterMinSize          = "3" ;
  fPadClusterMinADCs          = "200";
  fPadClusterMaxMultiplicity  = "1" ;
  fMappingFile                = "../../configFileDir/mapping.cfg";
  fPadMappingFile             = "../../configFileDir/padMapping.cfg";
  fSavedMappingFile           = "../../results/savedMapping.cfg";
  fRunNbFile                  = "../../configFileDir/runNb.cfg";
  fHistosFile                 = "../../configFileDir/histogram.cfg";
  fDisplayFile                = "../../configFileDir/display.cfg" ;
  fPedestalFile               = "../../pedestalDir/pedestal.root";
  fRawPedestalFile            = "../../pedestalDir/rawpedestal.root";
  fPositionCorrectionFile     = "../../pedestalDir/clusterPositionCorrection.root";
  fPositionCorrectionFlag     = "NO" ;
  fTrackingOffsetDir          = "../../offsetDir/";
  fROOTDataType               = "HITS_ONLY" ;
  fAPVGainCalibrationFile     = "" ;
  fIsHitMaxOrTotalADCs        = "signalPeak" ;
  fIsClusterMaxOrTotalADCs    = "TotalCharges" ;
  fStartEventNumber           = "0" ;
  fEventFrequencyNumber       = "1" ;
  fCrossTalkRemovalFlag       = "YES";
  fCrossTalkMaxClustSize      = "3";
  fSignalPeakMaxTimeBin       = "2" ;
  fSignalPeakMinTimeBin       = "7" ;
}

//============================================================================================
void SRSConfiguration::Save(const char * filename) const {
#ifdef DEBUG
  cout << "saving conf in " << gSystem->ExpandPathName(filename) << endl;
#endif
  ofstream file ( gSystem->ExpandPathName(filename));
  file << "CYCLEWAIT "            << fCycleWait                 << endl;
  file << "RUNTYPE "              << fRunType                   << endl;
  file << "RUNNBFILE "            << fRunNbFile                 << endl;
  file << "STARTEVENTNUMBER "     << fStartEventNumber          << endl;
  file << "EVENTFREQUENCYNUMBER " << fEventFrequencyNumber      << endl;
  //
  file << "CHMASKCUT "            << fMaskedChannelCut          << endl;
  //
  file << "ZEROSUPCUT "           << fZeroSupCut                << endl;
  file << "CLUSTER_MINSIZE "      << fClusterMinSize            << endl;
  file << "CLUSTER_MAXSIZE "      << fClusterMaxSize            << endl;
  file << "CLUSTER_MINADCS "      << fClusterMinADCs            << endl;
  file << "CLUSTER_MULT "         << fClusterMaxMultiplicity    << endl;
  //
  file << "PADZEROSUPCUT "        << fPadZeroSupCut             << endl;
  file << "PADCLUSTER_MINSIZE "   << fPadClusterMinSize         << endl;
  file << "PADCLUSTER_MAXSIZE "   << fPadClusterMaxSize         << endl;
  file << "PADCLUSTER_MINADCS "   << fPadClusterMinADCs         << endl;
  file << "PADCLUSTER_MULT "      << fPadClusterMaxMultiplicity << endl;
  //
  file << "HIT_ADCS "             << fIsHitMaxOrTotalADCs       << endl;
  file << "CLUSTER_ADCS "         << fIsClusterMaxOrTotalADCs   << endl;
  //
  file << "MAPFILE "              << fMappingFile               << endl;
  file << "PADMAPFILE "           << fPadMappingFile            << endl;
  file << "SAVEDMAPFILE "         << fSavedMappingFile          << endl;
  file << "PEDFILE "              << fPedestalFile              << endl;
  file << "RAWPEDFILE "           << fRawPedestalFile           << endl;
  //
  file << "HISTCFG "              << fHistosFile                << endl;
  file << "DISPCFG "              << fDisplayFile               << endl;
  //
  file << "OUTPUTDIR "            << fOutputDir                 << endl;
  file << "OUTPUTFILE "           << fOutputFile                << endl;
  file << "OFFSETDIR "            << fTrackingOffsetDir         << endl;
  file << "ROOTDATATYPE "         << fROOTDataType              << endl;
  //
  file << "APVGAINCALIB "         << fAPVGainCalibrationFile    << endl;
  file << "CLUSTERPOSCORRFILE "   << fPositionCorrectionFile    << endl;
  file << "CLUSTERPOSCORRFLAG "   << fPositionCorrectionFlag    << endl;
  //
  file << "CROSSTALKFLAG "        << fCrossTalkRemovalFlag      << endl;
  file << "CROSSTALKMAXCLUST "    << fCrossTalkMaxClustSize     << endl;
  //
  file << "SIGNALPEAKMINTIMEBIN " <<  fSignalPeakMinTimeBin     << endl;
  file << "SIGNALPEAKMAXTIMEBIN " <<  fSignalPeakMaxTimeBin     << endl;
  //
  file.close();
}

//============================================================================================
Bool_t SRSConfiguration::Load(const char * filename) {
  printf("  SRSConfiguration::Load() ==> Loading cfg from %s\n", gSystem->ExpandPathName(filename)) ;
  //  ifstream file (gSystem->ExpandPathName(filename), ifstream::in);
  //  if(!file.is_open()) return kFALSE;

  ifstream file;
  if (FileExists(gSystem->ExpandPathName(filename))) {
    try {
      file.open(gSystem->ExpandPathName(filename), ifstream::in);
    } catch (ifstream::failure e) {
      std::cerr << gSystem->ExpandPathName(filename)
		<< ": File does not exist or cannot be opened!\n";
    } catch (...) {
      std::cerr << "Non-processed exception!\n";
    }
    if (!file.is_open()) {
      return kFALSE;
    }
  } 
  else {
    return kFALSE;
  }

  TString line;
  while (line.ReadLine(file)) {

    // strip leading spaces and skip comments
    line.Remove(TString::kLeading, ' ');
    if(line.BeginsWith("#")) continue;        

    if(line.BeginsWith("RUNTYPE")) {
      char hfile[100];
      sscanf(line.Data(), "RUNTYPE %s", hfile);
      fRunType = hfile;
    }
    if(line.BeginsWith("OUTPUTDIR")) {
      char hfile[1000];
      sscanf(line.Data(), "OUTPUTDIR %s", hfile);
      fOutputDir = hfile;
    } 
    if(line.BeginsWith("OUTPUTFILE")) {
      char hfile[1000];
      sscanf(line.Data(), "OUTPUTFILE %s", hfile);
      fOutputFile = hfile;
    }
    //
    if(line.BeginsWith("ZEROSUPCUT")) {
      char hfile[10];
      sscanf(line.Data(), "ZEROSUPCUT %s", hfile);
      fZeroSupCut = hfile;
    }
    if(line.BeginsWith("CLUSTER_MAXSIZE")) {
      char hfile[10];
      sscanf(line.Data(), "CLUSTER_MAXSIZE %s", hfile);
      fClusterMaxSize = hfile;
    }
    if(line.BeginsWith("CLUSTER_MINSIZE")) {
      char hfile[10];
      sscanf(line.Data(), "CLUSTER_MINSIZE %s", hfile);
      fClusterMinSize = hfile;
    }
    if(line.BeginsWith("CLUSTER_MINADCS")) {
      char clusterMinADCs[10];
      sscanf(line.Data(), " CLUSTER_MINADCS%s", clusterMinADCs);
      fClusterMinADCs = clusterMinADCs ;
    }
    if(line.BeginsWith("CLUSTER_MULT")) {
      char hfile[100];
      sscanf(line.Data(), "CLUSTER_MULT %s", hfile);
      fClusterMaxMultiplicity = hfile;
    }
    if(line.BeginsWith("CLUSTER_ADCS")) {
      char isClusterMaxOrTotalADCs[100];
      sscanf(line.Data(), " CLUSTER_ADCS%s", isClusterMaxOrTotalADCs);
      fIsClusterMaxOrTotalADCs = isClusterMaxOrTotalADCs ;
    }
    //
    if(line.BeginsWith("PADZEROSUPCUT")) {
      char hfile[10];
      sscanf(line.Data(), "PADZEROSUPCUT %s", hfile);
      fPadZeroSupCut = hfile;
    }
    if(line.BeginsWith("PADCLUSTER_MINSIZE")) {
      char hfile[10];
      sscanf(line.Data(), "PADCLUSTER_MINSIZE %s", hfile);
      fPadClusterMinSize = hfile;
    }
    if(line.BeginsWith("PADCLUSTER_MAXSIZE")) {
      char hfile[10];
      sscanf(line.Data(), "PADCLUSTER_MAXSIZE %s", hfile);
      fPadClusterMaxSize = hfile;
    }
    if(line.BeginsWith("PADCLUSTER_MINADCS")) {
      char padClusterMinADCs[10];
      sscanf(line.Data(), " PADCLUSTER_MINADCS%s", padClusterMinADCs);
      fPadClusterMinADCs = padClusterMinADCs ;
    }
    if(line.BeginsWith("PADCLUSTER_MULT")) {
      char hfile[100];
      sscanf(line.Data(), "PADCLUSTER_MULT %s", hfile);
      fPadClusterMaxMultiplicity = hfile;
    }
    //
    if(line.BeginsWith("CYCLEWAIT")) {
      char hfile[100];
      sscanf(line.Data(), "CYCLEWAIT %s", hfile);
      fCycleWait = hfile;
    }
    if(line.BeginsWith("STARTEVENTNUMBER")) {
      char hfile[10];
      sscanf(line.Data(), "STARTEVENTNUMBER %s", hfile);
      fStartEventNumber = hfile;
    }
    if(line.BeginsWith("EVENTFREQUENCYNUMBER")) {
      char hfile[10];
      sscanf(line.Data(), "EVENTFREQUENCYNUMBER %s", hfile);
      fEventFrequencyNumber = hfile;
    }
    //
    if(line.BeginsWith("HISTCFG")) {
      char hfile[1000];
      sscanf(line.Data(), "HISTCFG %s", hfile);
      fHistosFile = hfile;
    }
    if(line.BeginsWith("DISPCFG")) {
      char displayfile[1000];
      sscanf(line.Data(), "DISPCFG %s", displayfile);
      fDisplayFile = displayfile;
    }
    // 
    if(line.BeginsWith("MAPFILE")) {
      char mapfile[1000];
      sscanf(line.Data(), "MAPFILE %s", mapfile);
      fMappingFile = mapfile;
    }
    if(line.BeginsWith("PADMAPFILE")) {
      char padmapfile[1000];
      sscanf(line.Data(), "PADMAPFILE %s", padmapfile);
      fPadMappingFile = padmapfile;
    }
    if(line.BeginsWith("SAVEDMAPFILE")) {
      char savedmapfile[1000];
      sscanf(line.Data(), "SAVEDMAPFILE %s", savedmapfile);
      fSavedMappingFile = savedmapfile;
    }
    if(line.BeginsWith("PEDFILE")) {  
      char pedfile[1000];  
      sscanf(line.Data(), "PEDFILE %s", pedfile);  
      fPedestalFile = pedfile; 
    }
    if(line.BeginsWith("RAWPEDFILE")) {  
      char rawpedfile[1000];  
      sscanf(line.Data(), "RAWPEDFILE %s", rawpedfile);  
      fRawPedestalFile = rawpedfile; 
    }
    //
    if(line.BeginsWith("OFFSETDIR")) {  
      char offsetfile[1000];  
      sscanf(line.Data(), "OFFSETDIR %s", offsetfile);  
      fTrackingOffsetDir = offsetfile; 
    }
    if(line.BeginsWith("RUNNBFILE")) {
      char runNbfile[1000];
      sscanf(line.Data(), "RUNNBFILE %s", runNbfile);
      fRunNbFile = runNbfile;
    }
    if(line.BeginsWith("ROOTDATATYPE")) {
      char rootdatatype[1000];
      sscanf(line.Data(), "ROOTDATATYPE %s", rootdatatype);
      fROOTDataType = rootdatatype;
    }
    //
    if(line.BeginsWith("HIT_ADCS")) {
      char isHitMaxOrTotalADCs[100];
      sscanf(line.Data(), " HIT_ADCS%s", isHitMaxOrTotalADCs);
      fIsHitMaxOrTotalADCs = isHitMaxOrTotalADCs ;
    }
    if(line.BeginsWith("CROSSTALKFLAG")) {
      char crossTalkRemovalFlag[100];
      sscanf(line.Data(), " CROSSTALKFLAG%s", crossTalkRemovalFlag);
      fCrossTalkRemovalFlag = crossTalkRemovalFlag ;
    }
    if(line.BeginsWith("CROSSTALKMAXCLUST")) {
      char crossTalkMaxClustSize[10];
      sscanf(line.Data(), " CROSSTALKMAXCLUST%s", crossTalkMaxClustSize);
      fCrossTalkMaxClustSize = crossTalkMaxClustSize ;
    }
    if(line.BeginsWith("SIGNALPEAKMINTIMEBIN")) {
      char signalPeakMinTimeBin[10];
      sscanf(line.Data(), " SIGNALPEAKMINTIMEBIN%s", signalPeakMinTimeBin);
      fSignalPeakMinTimeBin = signalPeakMinTimeBin;
    }
    if(line.BeginsWith("SIGNALPEAKMAXTIMEBIN")) {
      char signalPeakMaxTimeBin[10];
      sscanf(line.Data(), " SIGNALPEAKMAXTIMEBIN%s", signalPeakMaxTimeBin);
      fSignalPeakMaxTimeBin = signalPeakMaxTimeBin;
    }
    if(line.BeginsWith("CHMASKCUT")) {
      char maskchannels[1000];
      sscanf(line.Data(), "CHMASKCUT %s", maskchannels);
      fMaskedChannelCut = maskchannels;
    }
    if(line.BeginsWith("CLUSTERPOSCORRFILE")) {  
      char posCorrectfile[1000];  
      sscanf(line.Data(), "CLUSTERPOSCORRFILE %s", posCorrectfile);  
      fPositionCorrectionFile = posCorrectfile; 
    }
    if(line.BeginsWith("CLUSTERPOSCORRFLAG")) {  
      char posCorrectflag[10];  
      sscanf(line.Data(), "CLUSTERPOSCORRFLAG %s", posCorrectflag);  
      fPositionCorrectionFlag = posCorrectflag; 
    }
    if(line.BeginsWith("APVGAINCALIB")) {  
      char apvGainCalibrationFile[1000];  
      sscanf(line.Data(), "APVGAINCALIB %s", apvGainCalibrationFile);  
      fAPVGainCalibrationFile = apvGainCalibrationFile; 
    }
  }
  Dump();
  return kTRUE;
}

void SRSConfiguration::Dump() const {
  printf("  SRSConfiguration::Load() ==> RUNTYPE               %s\n", fRunType.Data()) ;
  printf("  SRSConfiguration::Load() ==> OUTPUTFILE            %s\n", fOutputFile.Data()) ;
  printf("  SRSConfiguration::Load() ==> OUTPUTDIR             %s\n", fOutputDir.Data()) ;
  printf("  SRSConfiguration::Load() ==> CYCLEWAIT             %s\n", fCycleWait.Data()) ;
  printf("  SRSConfiguration::Load() ==> HIT_ADCS              %s\n", fIsHitMaxOrTotalADCs.Data()) ;
  printf("  SRSConfiguration::Load() ==> CLUSTER_ADCS          %s\n", fIsClusterMaxOrTotalADCs.Data()) ;
  //
  printf("  SRSConfiguration::Load() ==> ZEROSUPCUT            %s\n", fZeroSupCut.Data()) ;
  printf("  SRSConfiguration::Load() ==> CLUSTER_MINSIZE       %s\n", fClusterMinSize.Data()) ;
  printf("  SRSConfiguration::Load() ==> CLUSTER_MAXSIZE       %s\n", fClusterMaxSize.Data()) ;
  printf("  SRSConfiguration::Load() ==> CLUSTER_MINADCS       %s\n", fClusterMinADCs.Data()) ;
  printf("  SRSConfiguration::Load() ==> CLUSTER_MULT          %s\n", fClusterMaxMultiplicity.Data()) ;
  //
  printf("  SRSConfiguration::Load() ==> PADZEROSUPCUT         %s\n", fPadZeroSupCut.Data()) ;
  printf("  SRSConfiguration::Load() ==> PADCLUSTER_MINSIZE    %s\n", fPadClusterMinSize.Data()) ;
  printf("  SRSConfiguration::Load() ==> PADCLUSTER_MAXSIZE    %s\n", fPadClusterMaxSize.Data()) ;
  printf("  SRSConfiguration::Load() ==> PADCLUSTER_MINADCS    %s\n", fPadClusterMinADCs.Data()) ;
  printf("  SRSConfiguration::Load() ==> PADCLUSTER_MULT       %s\n", fPadClusterMaxMultiplicity.Data()) ;
  //
  printf("  SRSConfiguration::Load() ==> HISTCFG               %s\n", fHistosFile.Data()) ;
  printf("  SRSConfiguration::Load() ==> DISPCFG               %s\n", fDisplayFile.Data()) ;
  printf("  SRSConfiguration::Load() ==> MAPFILE               %s\n", fMappingFile.Data()) ;
  printf("  SRSConfiguration::Load() ==> PADMAPFILE            %s\n", fPadMappingFile.Data()) ;
  printf("  SRSConfiguration::Load() ==> SAVEDMAPFILE          %s\n", fSavedMappingFile.Data()) ;
  printf("  SRSConfiguration::Load() ==> PEDFILE               %s\n", fPedestalFile.Data()) ;
  printf("  SRSConfiguration::Load() ==> RAWPEDFILE            %s\n", fRawPedestalFile.Data()) ;
  //
  printf("  SRSConfiguration::Load() ==> CLUSTERPOSCORRFILE    %s\n", fPositionCorrectionFile.Data()) ;
  printf("  SRSConfiguration::Load() ==> CLUSTERPOSCORRFLAG    %s\n", fPositionCorrectionFlag.Data()) ;
  printf("  SRSConfiguration::Load() ==> STARTEVENTNUMBER      %s\n", fStartEventNumber.Data()) ;
  printf("  SRSConfiguration::Load() ==> EVENTFREQUENCYNUMBER  %s\n", fEventFrequencyNumber.Data()) ;
  printf("  SRSConfiguration::Load() ==> APVGAINCALIB          %s\n", fAPVGainCalibrationFile.Data()) ;
  printf("  SRSConfiguration::Load() ==> ROOTDATATYPE          %s\n", fROOTDataType.Data()) ;
  printf("  SRSConfiguration::Load() ==> OFFSETDIR             %s\n", fTrackingOffsetDir.Data()) ;
  printf("  SRSConfiguration::Load() ==> CROSSTALKFLAG         %s\n", fCrossTalkRemovalFlag.Data()) ;
  printf("  SRSConfiguration::Load() ==> CROSSTALKMAXCLUST     %s\n", fCrossTalkMaxClustSize.Data()) ;
  printf("  SRSConfiguration::Load() ==> SIGNALPEAKMAXTIMEBIN  %s\n", fSignalPeakMaxTimeBin.Data()) ;
  printf("  SRSConfiguration::Load() ==> SIGNALPEAKMINTIMEBIN  %s\n", fSignalPeakMinTimeBin.Data()) ;
}

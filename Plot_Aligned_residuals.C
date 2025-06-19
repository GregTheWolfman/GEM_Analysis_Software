#include "TGaxis.h"
#include <numeric>


void Plot_Aligned_residuals (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, bool);
vector<float> Optimized_XY_Rotation (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, bool);
vector<float> Optimized_Rotation (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, bool);
vector<float> Optimized_XY_shift (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, vector<float>, bool);
vector<float> Residuals (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>);
vector<float> Residuals (vector<float>, vector<float>, vector<float>, vector<float>);
vector<float> ResCut (vector<float>&, vector<float>&, vector<float>&, vector<float>&);
vector<float> ChiSquared (vector<float>, vector<float>, vector<float>, vector<float>, vector<float>);
vector<float> ChiSquared (vector<float>, vector<float>, vector<float>, vector<float>);
//This function is for the final step of alignment, apply optimized offsets, save new hit coordinates to an output file and plot the residuals
void Plot_Aligned_residuals(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> OptimizedXYOffests, vector<float> BadEvents, vector<float> LAGDHits, vector<float> SMvec,  vector<float> CCvec,  vector<float> Eventidvec, bool plots){

  ofstream MyFile("Aligned_Data/Trackers_Hits_Merged_all.txt");    
  
  //MyFile << "#PedFile: GEMandTrksPed_pedestaloptionfirst_dataTree01.root \n";
  //MyFile << "#DataFiles: APV3_ADC_0_dataTree01.root \n";
  MyFile << "#Tracker1x, Tracker1y, Tracker2x, Tracker2y, Tracker3x, Tracker3y, Tracker4x, Tracker4y \n";
  

  ofstream LAGDhits("Aligned_Data/LAGD_Hits_Merged_all.txt");
  
  //for(uint p = 0; p < SMvec.size();p++){
  //  cout << SMvec.at(p) << endl;
  //}
  //cout << SMvec.size() << endl;
  //return;
  
  //LAGDhits << "#PedFile: GEMandTrksPed_pedestaloptionfirst_dataTree01.root \n";
  //LAGDhits << "#DataFiles: APV3_ADC_0_dataTree01.root, APV3_ADC_1_dataTree01.root, APV3_ADC_2_dataTree01.root \n";
  LAGDhits << "#EventID, AVP3E1" << endl;
  
  /*
  vector<float> T1Hitcoords;
  vector<float> T2Hitcoords;
  vector<float> T3Hitcoords;
  vector<float> T4Hitcoords;
  vector<float> LAGDHitcoords;
  vector<float> FixedSMvec;
  vector<float> FixedCCvec;
  vector<float> FixedEvtidvec;
  
  //cout << BadEvents.size() << endl;
  //now, actually remove the bad events from the final list of tracker/LAGD hits to go onto alignment
  cout << Tracker1HitlocVec.size()/3 << endl;
  cout << LAGDHits.size() << endl;
  */
  //cout << "Total Events with bad events in" << endl;

  //auto SMdist = new TH1F("","Strip Multiplicity Distribution for removed Events; Number of strips involved; Frequency",5, 0.5,5.5);
  //auto CCdist = new TH1F("","Cluster Charge Distribution for removed Events; Cluster charge [a.u.]; frequency",500, 0,2500);
  //auto Angdist = new TH1F("","Hit Distribution for removed Events; Angle [#phi]; frequency",50, 0, 0.4);

  //auto SMdistgd = new TH1F("","Strip Multiplicity Distribution for Accepted Events; Number of strips involved; Frequency",5, 0.5,5.5);
  //auto CCdistgd = new TH1F("","Cluster Charge Distribution for Accepted Events; Cluster charge [a.u.]; frequency",500, 0,2500);
  //auto Angdistgd = new TH1F("","Hit Distribution for Accepted Events; Angle [#phi]; frequency",50, 0, 0.4);

  //auto Hitmap1 = new TH2F("", "Beam Spot on Tracker 1", 100, 0, 50, 100, 0, 50);
  //auto Hitmap2 = new TH2F("", "Beam Spot on Tracker 2", 100, 0, 50, 100, 0, 50);
  /*
  for(auto i = 0; i < Tracker1HitlocVec.size();i++){
    //cout << i/3 << endl;
    //if(find(BadEvents.begin(), BadEvents.end(), i/3) == BadEvents.end()){
      //cout << "ttackers" << endl;
      T1Hitcoords.push_back(Tracker1HitlocVec.at(i));T1Hitcoords.push_back(Tracker1HitlocVec.at(i+1));T1Hitcoords.push_back(Tracker1HitlocVec.at(i+2));
      T2Hitcoords.push_back(Tracker2HitlocVec.at(i));T2Hitcoords.push_back(Tracker2HitlocVec.at(i+1));T2Hitcoords.push_back(Tracker2HitlocVec.at(i+2));
      T3Hitcoords.push_back(Tracker3HitlocVec.at(i));T3Hitcoords.push_back(Tracker3HitlocVec.at(i+1));T3Hitcoords.push_back(Tracker3HitlocVec.at(i+2));
      T4Hitcoords.push_back(Tracker4HitlocVec.at(i));T4Hitcoords.push_back(Tracker4HitlocVec.at(i+1));T4Hitcoords.push_back(Tracker4HitlocVec.at(i+2));

      //Hitmap1->Fill(Tracker1HitlocVec.at(i), Tracker1HitlocVec.at(i+1));
      //Hitmap2->Fill(Tracker2HitlocVec.at(i), Tracker2HitlocVec.at(i+1));
      
      //cout << "LAGD" << endl;
      LAGDhits << LAGDHits.at(i/3) << endl;
      LAGDHitcoords.push_back(LAGDHits.at(i/3));
      //cout << "LAGD2" << endl;
     
      //cout << SMvec.at(i/3) << endl;
      //FixedSMvec.push_back(SMvec.at(i/3));
      //SMdist->Fill(SMvec.at(i/3));
  
      //FixedCCvec.push_back(CCvec.at(i/3));
      //CCdist->Fill(CCvec.at(i/3));
      //cout << Tracker1HitlocVec.at(i) << endl;      
      //cout << Tracker1HitlocVec.at(i+1) << endl;
      //cout << Tracker1HitlocVec.at(i+2) << endl;
      //cout << LAGDHits.at(i/3) << endl;
      
      //Angdist->Fill(LAGDHits.at(i/3)*0.0041);
      i++;
      i++;
      //cout << i << "?" << endl;
      //}
    /*
    
    else{     
      cout << SMvec.at(i/3) << endl;
      FixedSMvec.push_back(SMvec.at(i/3));
      SMdistgd->Fill(SMvec.at(i/3));
  
      FixedCCvec.push_back(CCvec.at(i/3));
      CCdistgd->Fill(CCvec.at(i/3));

      Angdistgd->Fill(LAGDHits.at(i/3)*0.0041);
      i++;
      i++;
    }
    
  }
  */
  /*
  auto h = new TCanvas("","", 1500, 500);
  h->Divide(3,1);
  h->cd(1);
  SMdist->Draw();
  h->cd(2);
  CCdist->Draw();
  h->cd(3);
  Angdist->Draw();

  h->Draw();
  */
  /*
  auto h = new TCanvas("","", 1000, 500);
  h->Divide(2,1);
  h->cd(1);
  Hitmap1->Draw("colz");
  h->cd(2);
  Hitmap2->Draw("colz");
  

  h->Draw();
  return;
  */
  /*
  auto hi = new TCanvas("","", 1500, 500);
  hi->Divide(3,1);
  hi->cd(1);
  SMdistgd->Draw();
  hi->cd(2);
  CCdistgd->Draw();
  hi->cd(3);
  Angdistgd->Draw();

  hi->Draw();
  */
  /*
  int k=0;
  cout << "passed first part" << endl;
  cout << LAGDHits.size() << endl;
  cout << Tracker1HitlocVec.size() << endl;
  int i =0;
  while(i<Tracker1HitlocVec.size()){
    //cout << k++ << endl;
    cout << "Evnt num: " << i << endl;
    if(find(BadEvents.begin(), BadEvents.end(), i/3) != BadEvents.end()){//if the current event (i/3) IS in the vector of bad events,....
      cout << "k: " << k++ << endl;
      Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i-1, Tracker1HitlocVec.begin()+i+1);
      Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i-1, Tracker2HitlocVec.begin()+i+1);
      Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i-1, Tracker3HitlocVec.begin()+i+1);
      Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i-1, Tracker4HitlocVec.begin()+i+1);
      LAGDHits.erase(LAGDHits.begin()+i-1);
      cout << "Goodbye event: " << i/3 << endl;
    }
    if(find(BadEvents.begin(), BadEvents.end(), i/3) == BadEvents.end()){
      cout << "k: " << k++ << endl;      
      i++;
      i++;
      i++;
    }
    else{
      i++;
      i++;
      i++;
    }
  }
    
  */

  //return;

  


  // cout << "passed weird part" << endl;
  
  
  // float Shift_T1x = OptimizedXYOffests[0];
  // float Shift_T1y = OptimizedXYOffests[1];
  // float Shift_T2x = OptimizedXYOffests[2];
  // float Shift_T2y = OptimizedXYOffests[3];
  // float Shift_T3x = OptimizedXYOffests[4];
  // float Shift_T3y = OptimizedXYOffests[5];
  // float Shift_T4x = OptimizedXYOffests[6];
  // float Shift_T4y = OptimizedXYOffests[7];
  

  float Shift_T1x = 0.345;//mm
  float Shift_T1y = -0.698;
  float Shift_T2x = -0.379;
  float Shift_T2y = 0.745;
  float Shift_T3x = 0.613;
  float Shift_T3y = -0.621;
  float Shift_T4x = -0.579;
  float Shift_T4y = 0.573;
  
  float angleT2=0.0;
  float angleT3=0.0;
  float angleT4=0.0;
  
  // cout << OptimizedXYOffests.size() << endl;
  // if(OptimizedXYOffests.size() > 8){
  //   angleT2 = OptimizedXYOffests[8];
  //   cout << angleT2 << endl;
  //   angleT3 = OptimizedXYOffests[9];
  //   angleT4 = OptimizedXYOffests[10];
  // }

  cout << "Final Offsets for Trackers: " << endl;
  cout << "T1x: " <<  Shift_T1x << endl;
  cout << "T1y: " <<  Shift_T1y << endl;
  cout << "T2x: " <<  Shift_T2x << endl;
  cout << "T2y: " <<  Shift_T2y << endl;
  cout << "T3x: " <<  Shift_T3x << endl;
  cout << "T3y: " <<  Shift_T3y << endl;
  cout << "T4x: " <<  Shift_T4x << endl;
  cout << "T4y: " <<  Shift_T4y << endl;
  cout << "Angle btwn T2 and T1: " << angleT2 << endl;
  cout << "Angle btwn T3 and T1: " << angleT3 << endl;  
  cout << "Angle btwn T4 and T1: " << angleT4 << endl;
  
  float T1Hitcoordsx_tmp=0;
  float T1Hitcoordsy_tmp=0;
  float T2Hitcoordsx_tmp=0;
  float T2Hitcoordsy_tmp=0;
  float T3Hitcoordsx_tmp=0;
  float T3Hitcoordsy_tmp=0;
  float T4Hitcoordsx_tmp=0;
  float T4Hitcoordsy_tmp=0;

  double mean_Tracker1xcoordRes=0.0, mean_Tracker1ycoordRes=0.0;
  double mean_Tracker2xcoordRes=0.0, mean_Tracker2ycoordRes=0.0;
  double mean_Tracker3xcoordRes=0.0, mean_Tracker3ycoordRes=0.0;
  double mean_Tracker4xcoordRes=0.0, mean_Tracker4ycoordRes=0.0;  

  double width_Tracker1xcoordRes=0.0, width_Tracker1ycoordRes=0.0;
  double width_Tracker2xcoordRes=0.0, width_Tracker2ycoordRes=0.0;
  double width_Tracker3xcoordRes=0.0, width_Tracker3ycoordRes=0.0;
  double width_Tracker4xcoordRes=0.0, width_Tracker4ycoordRes=0.0;  

  char nameRes2X[40];sprintf(nameRes2X,"PostTrackerAlignment_residual_Tracker2x");char nameRes2Y[40];sprintf(nameRes2Y,"PostTrackerAlignment_residual_Tracker2y");
  char nameRes3X[40];sprintf(nameRes3X,"PostTrackerAlignment_residual_Tracker3x");char nameRes3Y[40];sprintf(nameRes3Y,"PostTrackerAlignment_residual_Tracker3y");
  char nameRes1X[40];sprintf(nameRes1X,"PostTrackerAlignment_residual_Tracker1x");char nameRes1Y[40];sprintf(nameRes1Y,"PostTrackerAlignment_residual_Tracker1y");
  char nameRes4X[40];sprintf(nameRes4X,"PostTrackerAlignment_residual_Tracker4x");char nameRes4Y[40];sprintf(nameRes4Y,"PostTrackerAlignment_residual_Tracker4y");

  
  TH1F* h_residual_Tracker2x = new TH1F(nameRes2X,"",60,-300,300); h_residual_Tracker2x->SetXTitle("Residual [um]"); h_residual_Tracker2x->SetYTitle("Counts/13 um");h_residual_Tracker2x->SetLabelSize(0.042,"XY");h_residual_Tracker2x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker2y = new TH1F(nameRes2Y,"",60,-300,300); h_residual_Tracker2y->SetXTitle("Residual [um]"); h_residual_Tracker2y->SetYTitle("Counts/13 um");h_residual_Tracker2y->SetLabelSize(0.042,"XY");h_residual_Tracker2y->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker3x = new TH1F(nameRes3X,"",60,-300,300); h_residual_Tracker3x->SetXTitle("Residual [um]"); h_residual_Tracker3x->SetYTitle("Counts/13 um");h_residual_Tracker3x->SetLabelSize(0.042,"XY");h_residual_Tracker3x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker3y = new TH1F(nameRes3Y,"",60,-300,300); h_residual_Tracker3y->SetXTitle("Residual [um]"); h_residual_Tracker3y->SetYTitle("Counts/13 um");h_residual_Tracker3y->SetLabelSize(0.042,"XY");h_residual_Tracker3y->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker1x = new TH1F(nameRes1X,"",60,-300,300); h_residual_Tracker1x->SetXTitle("Residual [um]"); h_residual_Tracker1x->SetYTitle("Counts/13 um");h_residual_Tracker1x->SetLabelSize(0.042,"XY");h_residual_Tracker1x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker1y = new TH1F(nameRes1Y,"",60,-300,300); h_residual_Tracker1y->SetXTitle("Residual [um]"); h_residual_Tracker1y->SetYTitle("Counts/13 um");h_residual_Tracker1y->SetLabelSize(0.042,"XY");h_residual_Tracker1y->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker4x = new TH1F(nameRes4X,"",60,-300,300); h_residual_Tracker4x->SetXTitle("Residual [um]"); h_residual_Tracker4x->SetYTitle("Counts/13 um");h_residual_Tracker4x->SetLabelSize(0.042,"XY");h_residual_Tracker4x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker4y = new TH1F(nameRes4Y,"",60,-300,300); h_residual_Tracker4y->SetXTitle("Residual [um]"); h_residual_Tracker4y->SetYTitle("Counts/13 um");h_residual_Tracker4y->SetLabelSize(0.042,"XY");h_residual_Tracker4y->SetTitleSize(0.042,"XY");

  TH1F* h_Pos_g2xcl = new TH1F(nameRes2X,"",300,-110,110); h_Pos_g2xcl->SetXTitle("Cluster position [mm]"); h_Pos_g2xcl->SetYTitle("Counts/13 um");h_Pos_g2xcl->SetLabelSize(0.045,"XY");h_Pos_g2xcl->SetTitleSize(0.045,"XY");
  TH1F* h_Pos_g2ycl = new TH1F(nameRes2Y,"",300,-110,110); h_Pos_g2ycl->SetXTitle(" Cluster position [mm]"); h_Pos_g2ycl->SetYTitle("Counts/13 um");h_Pos_g2ycl->SetLabelSize(0.045,"XY");h_Pos_g2ycl->SetTitleSize(0.045,"XY");  
  TH1F* h_Pos_g1xcl = new TH1F(nameRes1X,"",300,-110,110); h_Pos_g1xcl->SetXTitle("Cluster position [mm]"); h_Pos_g1xcl->SetYTitle("Counts/13 um");h_Pos_g1xcl->SetLabelSize(0.045,"XY");h_Pos_g1xcl->SetTitleSize(0.045,"XY");
  TH1F* h_Pos_g1ycl = new TH1F(nameRes1Y,"",300,-110,110); h_Pos_g1ycl->SetXTitle("Cluster position [mm]"); h_Pos_g1ycl->SetYTitle("Counts/13 um");h_Pos_g1ycl->SetLabelSize(0.045,"XY");h_Pos_g1ycl->SetTitleSize(0.045,"XY");  
  TH1F* h_Pos_g3xcl = new TH1F(nameRes3X,"",300,-110,110); h_Pos_g3xcl->SetXTitle("Cluster position [mm]"); h_Pos_g3xcl->SetYTitle("Counts/13 um");h_Pos_g3xcl->SetLabelSize(0.045,"XY");h_Pos_g3xcl->SetTitleSize(0.045,"XY");
  TH1F* h_Pos_g3ycl = new TH1F(nameRes3Y,"",300,-110,110); h_Pos_g3ycl->SetXTitle(" Cluster position [mm]"); h_Pos_g3ycl->SetYTitle("Counts/13 um");h_Pos_g3ycl->SetLabelSize(0.045,"XY");h_Pos_g3ycl->SetTitleSize(0.045,"XY");
  TH1F* h_Pos_g4xcl = new TH1F(nameRes4X,"",300,-110,110); h_Pos_g4xcl->SetXTitle("Cluster position [mm]"); h_Pos_g4xcl->SetYTitle("Counts/13 um");h_Pos_g4xcl->SetLabelSize(0.045,"XY");h_Pos_g4xcl->SetTitleSize(0.045,"XY");
  TH1F* h_Pos_g4ycl = new TH1F(nameRes4Y,"",300,-110,110); h_Pos_g4ycl->SetXTitle("Cluster position [mm]"); h_Pos_g4ycl->SetYTitle("Counts/13 um");h_Pos_g4ycl->SetLabelSize(0.045,"XY");h_Pos_g4ycl->SetTitleSize(0.045,"XY");

  vector<float> T1rvals;
  vector<float> T2rvals;
  vector<float> T3rvals;
  vector<float> T4rvals;

  auto Tracker1Hitmap = new TH2F("PHT1", "Tracker 1 Beam Spot; X position [mm]; Y position [mm]", 50, 0, 50, 50, 0, 50);
  auto Tracker2Hitmap = new TH2F("PHT2", "Tracker 2 Beam Spot; X position [mm]; Y position [mm]", 50, 0, 50, 50, 0, 50);
  auto Tracker3Hitmap = new TH2F("PHT3", "Tracker 3 Beam Spot; X position [mm]; Y position [mm]", 50, 0, 50, 50, 0, 50);
  auto Tracker4Hitmap = new TH2F("PHT4", "Tracker 4 Beam Spot; X position [mm]; Y position [mm]", 50, 0, 50, 50, 0, 50);
  //cout << "A" << endl;
  //cout << T1Hitcoords.size() << endl;

  
  for(unsigned int i=0; i < Tracker1HitlocVec.size(); i++){//shift all of the XY hit coordinates
	  
    Tracker1HitlocVec.at(i) = Tracker1HitlocVec.at(i) - Shift_T1x;
    //T1Hitcoordsx_tmp = T1Hitcoords.at(i);
    Tracker2HitlocVec.at(i) = Tracker2HitlocVec.at(i) - Shift_T2x;
    //T2Hitcoordsx_tmp = T2Hitcoords.at(i); 
    Tracker3HitlocVec.at(i) = Tracker3HitlocVec.at(i) - Shift_T3x;
    //T3Hitcoordsx_tmp = T3Hitcoords.at(i);
    Tracker4HitlocVec.at(i) = Tracker4HitlocVec.at(i) - Shift_T4x;
    //T4Hitcoordsx_tmp = T4Hitcoords.at(i);

    i++;//move to y components
	  
    Tracker1HitlocVec.at(i) = Tracker1HitlocVec.at(i) - Shift_T1y;
    //T1Hitcoordsx_tmp = T1Hitcoords.at(i);
    Tracker2HitlocVec.at(i) = Tracker2HitlocVec.at(i) - Shift_T2y;
    //T2Hitcoordsx_tmp = T2Hitcoords.at(i); 
    Tracker3HitlocVec.at(i) = Tracker3HitlocVec.at(i) - Shift_T3y;
    //T3Hitcoordsx_tmp = T3Hitcoords.at(i);
    Tracker4HitlocVec.at(i) = Tracker4HitlocVec.at(i) - Shift_T4y;
    //T4Hitcoordsx_tmp = T4Hitcoords.at(i);
 

    i++;
  }

  for(unsigned int i=0; i < Tracker1HitlocVec.size(); i++){
      
    T2Hitcoordsx_tmp = Tracker2HitlocVec.at(i); 	
    Tracker2HitlocVec.at(i) = Tracker2HitlocVec.at(i)*cos(angleT2) - Tracker2HitlocVec.at(i+1)*sin(angleT2);	  
    
    T3Hitcoordsx_tmp = Tracker3HitlocVec.at(i); 	
    Tracker3HitlocVec.at(i) = Tracker3HitlocVec.at(i)*cos(angleT3) - Tracker3HitlocVec.at(i+1)*sin(angleT3);	  
    //cout << "Tracker3x Coordinate: " << Tracker3HitlocVec.at(i) << endl;
    T4Hitcoordsx_tmp = Tracker4HitlocVec.at(i); 	
    Tracker4HitlocVec.at(i) = Tracker4HitlocVec.at(i)*cos(angleT4) - Tracker4HitlocVec.at(i+1)*sin(angleT4);	  
      
    h_Pos_g1xcl->Fill(Tracker1HitlocVec.at(i));
    h_Pos_g2xcl->Fill(Tracker2HitlocVec.at(i));
    h_Pos_g3xcl->Fill(Tracker3HitlocVec.at(i));
    h_Pos_g4xcl->Fill(Tracker4HitlocVec.at(i));

    if(i == 0)
      cout << "Post-offset Tracker 2x first value: " << Tracker2HitlocVec.at(i) << endl;
    
    i++;//move to y components
    
    Tracker2HitlocVec.at(i) = T2Hitcoordsx_tmp*sin(angleT2) + Tracker2HitlocVec.at(i)*cos(angleT2);	  
    
    Tracker3HitlocVec.at(i) = T3Hitcoordsx_tmp*sin(angleT3) + Tracker3HitlocVec.at(i)*cos(angleT3);	  
    //cout << "Tracker3y Coordinate: " << Tracker3HitlocVec.at(i) << endl;
    Tracker4HitlocVec.at(i) = T4Hitcoordsx_tmp*sin(angleT4) + Tracker4HitlocVec.at(i)*cos(angleT4);	  
    
    h_Pos_g1ycl->Fill(Tracker1HitlocVec.at(i));
    h_Pos_g2ycl->Fill(Tracker2HitlocVec.at(i));
    h_Pos_g3ycl->Fill(Tracker3HitlocVec.at(i));
    h_Pos_g4ycl->Fill(Tracker4HitlocVec.at(i));


    T1rvals.push_back(sqrt((Tracker1HitlocVec.at(i-1))*(Tracker1HitlocVec.at(i-1)) + Tracker1HitlocVec.at(i)*Tracker1HitlocVec.at(i)));
    T2rvals.push_back(sqrt((Tracker2HitlocVec.at(i-1))*(Tracker2HitlocVec.at(i-1)) + Tracker2HitlocVec.at(i)*Tracker2HitlocVec.at(i)));
    T3rvals.push_back(sqrt((Tracker3HitlocVec.at(i-1))*(Tracker3HitlocVec.at(i-1)) + Tracker3HitlocVec.at(i)*Tracker3HitlocVec.at(i)));
    T4rvals.push_back(sqrt((Tracker4HitlocVec.at(i-1))*(Tracker4HitlocVec.at(i-1)) + Tracker4HitlocVec.at(i)*Tracker4HitlocVec.at(i)));
    
    Tracker1Hitmap->Fill(Tracker1HitlocVec.at(i-1), Tracker1HitlocVec.at(i));
    Tracker2Hitmap->Fill(Tracker2HitlocVec.at(i-1), Tracker2HitlocVec.at(i));
    Tracker3Hitmap->Fill(Tracker3HitlocVec.at(i-1), Tracker3HitlocVec.at(i));
    Tracker4Hitmap->Fill(Tracker4HitlocVec.at(i-1), Tracker4HitlocVec.at(i));

    
    MyFile << Tracker1HitlocVec.at(i-1) << " " << Tracker1HitlocVec.at(i) << " " <<Tracker2HitlocVec.at(i-1) << " " << Tracker2HitlocVec.at(i) << " " <<Tracker3HitlocVec.at(i-1) << " " << Tracker3HitlocVec.at(i) << " " <<Tracker4HitlocVec.at(i-1) << " " << Tracker4HitlocVec.at(i) << endl;

    LAGDhits << LAGDHits.at(i/3) << endl;
    
    i++;         	  
  }
  MyFile.close();
  LAGDhits.close();
  
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

  Hitspots->SaveAs("PostAlignmentHitSpots.png");

  //return;
  
 

  auto T1R = accumulate(T1rvals.begin(), T1rvals.end(), 0.0)/T1rvals.size();
  auto T2R = accumulate(T2rvals.begin(), T2rvals.end(), 0.0)/T2rvals.size();
  auto T3R = accumulate(T3rvals.begin(), T3rvals.end(), 0.0)/T3rvals.size();
  auto T4R = accumulate(T4rvals.begin(), T4rvals.end(), 0.0)/T4rvals.size();
  
      
  
  
  auto NewRes = Residuals(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec);
  //cout << "HERE?" << endl;
  auto Chisquares = ChiSquared(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec);
  
  cout << "B" << endl;
  for(unsigned int i=0; i< NewRes.size(); i++){//Put all residuals into plots

    h_residual_Tracker1x->Fill(NewRes.at(i)*1000);	 
    i++;
    h_residual_Tracker1y->Fill(NewRes.at(i)*1000);
    i++;
    h_residual_Tracker2x->Fill(NewRes.at(i)*1000);
    i++;
    h_residual_Tracker2y->Fill(NewRes.at(i)*1000);
    i++;
    h_residual_Tracker3x->Fill(NewRes.at(i)*1000);
    i++;
    h_residual_Tracker3y->Fill(NewRes.at(i)*1000);
    i++;
    h_residual_Tracker4x->Fill(NewRes.at(i)*1000);
    i++;
    h_residual_Tracker4y->Fill(NewRes.at(i)*1000);
  }

  I2GFvalues myValues;

  myValues = I2GFmainLoop(h_residual_Tracker1x,1,4,1);                           //converges!	  
  mean_Tracker1xcoordRes = myValues.mean; width_Tracker1xcoordRes= myValues.sigma;
      	
  myValues = I2GFmainLoop(h_residual_Tracker1y,1,4,1);	                            //breaks :(
  mean_Tracker1ycoordRes = myValues.mean; width_Tracker1ycoordRes = myValues.sigma;
      	
  myValues = I2GFmainLoop(h_residual_Tracker2x,1,4,1);
  mean_Tracker2xcoordRes = myValues.mean; width_Tracker2xcoordRes = myValues.sigma;   //converges!	  
      
  myValues = I2GFmainLoop(h_residual_Tracker2y,1,4,1);	                            //breaks :(... im sensing a pattern...
  mean_Tracker2ycoordRes = myValues.mean; width_Tracker2ycoordRes = myValues.sigma;

  myValues = I2GFmainLoop(h_residual_Tracker3x,1,4,1);
  mean_Tracker3xcoordRes = myValues.mean; width_Tracker3xcoordRes = myValues.sigma;

  myValues = I2GFmainLoop(h_residual_Tracker3y,1,4,1);	  
  mean_Tracker3ycoordRes = myValues.mean; width_Tracker3ycoordRes = myValues.sigma;

  myValues = I2GFmainLoop(h_residual_Tracker4x,1,4,1);	  
  mean_Tracker4xcoordRes = myValues.mean; width_Tracker4xcoordRes = myValues.sigma;

  myValues = I2GFmainLoop(h_residual_Tracker4y,1,4,1);	  
  mean_Tracker4ycoordRes = myValues.mean; width_Tracker4ycoordRes = myValues.sigma;
    
    
  TCanvas* TrackerHits = new TCanvas(nameRes2X, "Final Alignment Residuals", 2000, 1000);
  TrackerHits->Divide(4,2);
  //TrackerHits->SetRightMargin(TrackerHits->PixeltoX(0.01));

  gStyle->SetOptFit();
  gStyle->SetStatX(0.45);
  gStyle->SetStatW(0.18);	
  gStyle->SetStatY(0.85);
  
  int rangevals = 300;
  TrackerHits->cd(1);
  h_residual_Tracker1x->SetTitle("Tracker1x residual");
  //h_residual_Tracker1x->SetTitleOffset(0.5);	
  h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
  h_residual_Tracker1x->Draw();
  //gStyle->SetOptStat(0);
  /*
  auto T1xRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker1x->GetMaximum()),rangevals,1.05*(h_residual_Tracker1x->GetMaximum()),-rangevals/(T1R),rangevals/(T1R),510,"-");	
  T1xRaxis->SetTitle("[mRad]");
  T1xRaxis->Draw();
	
	
  string T1xlegendentry = "Fit Width: ";
  string T1xlegendentry2 = "Fit Mean: ";
  T1xlegendentry += Form("%.2f",width_Tracker1xcoordRes);
  T1xlegendentry += " um";
  char T1xcharlegendentry[T1xlegendentry.size()];
  strcpy(T1xcharlegendentry, T1xlegendentry.c_str());

  T1xlegendentry2 += Form("%.2f",mean_Tracker1xcoordRes);
  T1xlegendentry2 += " um";
  char T1xcharlegendentry2[T1xlegendentry2.size()];
  strcpy(T1xcharlegendentry2, T1xlegendentry2.c_str());

  string T1xlegendentry3 = "Fit Width: ";
  string T1xlegendentry4 = "Fit Mean: ";
	
  T1xlegendentry3 += Form("%.2f",width_Tracker1xcoordRes/(T1R));
  T1xlegendentry3 += " mRad";
  char T1xcharlegendentry3[T1xlegendentry3.size()];
  strcpy(T1xcharlegendentry3, T1xlegendentry3.c_str());

  T1xlegendentry4 += Form("%.2f",mean_Tracker1xcoordRes/(T1R));
  T1xlegendentry4 += " mRad";
  char T1xcharlegendentry4[T1xlegendentry4.size()];
  strcpy(T1xcharlegendentry4, T1xlegendentry4.c_str());

  cout << T1xcharlegendentry4 << endl;
	
  string T1xlegendentry5 = "Radius: ";	
  T1xlegendentry5 += Form("%.2f",T1R);
  T1xlegendentry5 += " mm";
  char T1xcharlegendentry5[T1xlegendentry5.size()+1];
  strcpy(T1xcharlegendentry5, T1xlegendentry5.c_str());
	
  TLatex latexT1x;
  latexT1x.SetTextSize(0.035);
  latexT1x.SetTextAlign(13);  //align at top
  latexT1x.SetNDC();
  latexT1x.DrawLatex(0.55,.85,T1xcharlegendentry);
  latexT1x.DrawLatex(0.55,.8,T1xcharlegendentry2);
  latexT1x.DrawLatex(0.55,.75,T1xcharlegendentry3);
  latexT1x.DrawLatex(0.55,.7,T1xcharlegendentry4);
  latexT1x.DrawLatex(0.55,.65,T1xcharlegendentry5);
  */	
	
  TrackerHits->cd(5);
  h_residual_Tracker1y->SetTitle("Tracker1y residual");
  h_residual_Tracker1y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
  h_residual_Tracker1y->Draw();
  /*
  auto T1yRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker1y->GetMaximum()),rangevals,1.05*(h_residual_Tracker1y->GetMaximum()),-rangevals/(T1R),rangevals/(T1R),510,"-");	
  T1yRaxis->SetTitle("[mRad]");
  T1yRaxis->Draw();
	
  string T1ylegendentry = "Fit Width: ";
  string T1ylegendentry2 = "Fit Mean: ";
  T1ylegendentry += Form("%.2f",width_Tracker1ycoordRes);
  T1ylegendentry += " um";
  char T1ycharlegendentry[T1ylegendentry.size()];
  strcpy(T1ycharlegendentry, T1ylegendentry.c_str());

  T1ylegendentry2 += Form("%.2f",mean_Tracker1ycoordRes);
  T1ylegendentry2 += " um";
  char T1ycharlegendentry2[T1ylegendentry2.size()];
  strcpy(T1ycharlegendentry2, T1ylegendentry2.c_str());

  string T1ylegendentry3 = "Fit Width: ";
  string T1ylegendentry4 = "Fit Mean: ";
  T1ylegendentry3 += Form("%.2f",width_Tracker1ycoordRes/(T1R));
  T1ylegendentry3 += " mRad";
  char T1ycharlegendentry3[T1ylegendentry3.size()];
  strcpy(T1ycharlegendentry3, T1ylegendentry3.c_str());

  T1ylegendentry4 += Form("%.2f",mean_Tracker1ycoordRes/(T1R));
  T1ylegendentry4 += " mRad";
  char T1ycharlegendentry4[T1ylegendentry4.size()];
  strcpy(T1ycharlegendentry4, T1ylegendentry4.c_str());

  string T1ylegendentry5 = "Radius: ";	
  T1ylegendentry5 += Form("%.2f",T1R);
  T1ylegendentry5 += " mm";
  char T1ycharlegendentry5[T1ylegendentry5.size()+1];
  strcpy(T1ycharlegendentry5, T1ylegendentry5.c_str());

  TLatex latexT1y;
  latexT1y.SetTextSize(0.035);
  latexT1y.SetTextAlign(13);  //align at top
  latexT1y.SetNDC();
  latexT1y.DrawLatex(0.55,.85,T1ycharlegendentry);
  latexT1y.DrawLatex(0.55,.8,T1ycharlegendentry2);
  latexT1y.DrawLatex(0.55,.75,T1ycharlegendentry3);
  latexT1y.DrawLatex(0.55,.7,T1ycharlegendentry4);
  latexT1y.DrawLatex(0.55,.65,T1ycharlegendentry5);
  */	
	
  TrackerHits->cd(2);
  h_residual_Tracker2x->SetTitle("Tracker2x residual");
  h_residual_Tracker2x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
  h_residual_Tracker2x->Draw();
  /*	
  auto T2xRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker2x->GetMaximum()),rangevals,1.05*(h_residual_Tracker2x->GetMaximum()),-rangevals/(T2R),rangevals/(T2R),510,"-");	
  T2xRaxis->SetTitle("[mRad]");
  T2xRaxis->Draw();
	
  string T2xlegendentry = "Fit Width: ";
  string T2xlegendentry2 = "Fit Mean: ";
  T2xlegendentry += Form("%.2f",width_Tracker2xcoordRes);
  T2xlegendentry += " um";
  char T2xcharlegendentry[T2xlegendentry.size()];
  strcpy(T2xcharlegendentry, T2xlegendentry.c_str());

  T2xlegendentry2 += Form("%.2f",mean_Tracker2xcoordRes);
  T2xlegendentry2 += " um";
  char T2xcharlegendentry2[T2xlegendentry2.size()];
  strcpy(T2xcharlegendentry2, T2xlegendentry2.c_str());

  string T2xlegendentry3 = "Fit Width: ";
  string T2xlegendentry4 = "Fit Mean: ";
  T2xlegendentry3 += Form("%.2f",width_Tracker2xcoordRes/(T2R));
  T2xlegendentry3 += " mRad";
  char T2xcharlegendentry3[T2xlegendentry3.size()];
  strcpy(T2xcharlegendentry3, T2xlegendentry3.c_str());

  T2xlegendentry4 += Form("%.2f",mean_Tracker2xcoordRes/(T2R));
  T2xlegendentry4 += " mRad";
  char T2xcharlegendentry4[T2xlegendentry4.size()];
  strcpy(T2xcharlegendentry4, T2xlegendentry4.c_str());
	
  string T2xlegendentry5 = "Radius: ";	
  T2xlegendentry5 += Form("%.2f",T2R);
  T2xlegendentry5 += " mm";
  char T2xcharlegendentry5[T2xlegendentry5.size()+1];
  strcpy(T2xcharlegendentry5, T2xlegendentry5.c_str());

  TLatex latexT2x;
  latexT2x.SetTextSize(0.035);
  latexT2x.SetTextAlign(13);  //align at top
  latexT2x.SetNDC();
  latexT2x.DrawLatex(0.55,.85,T2xcharlegendentry);
  latexT2x.DrawLatex(0.55,.8,T2xcharlegendentry2);
  latexT2x.DrawLatex(0.55,.75,T2xcharlegendentry3);
  latexT2x.DrawLatex(0.55,.7,T2xcharlegendentry4);
  latexT2x.DrawLatex(0.55,.65,T2xcharlegendentry5);
  */	
	
  TrackerHits->cd(6);
  h_residual_Tracker2y->SetTitle("Tracker2y residual");
  h_residual_Tracker2y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
  h_residual_Tracker2y->Draw();
	
  /*
  auto T2yRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker2y->GetMaximum()),rangevals,1.05*(h_residual_Tracker2y->GetMaximum()),-rangevals/(T2R),rangevals/(T2R),510,"-");	
  T2yRaxis->SetTitle("[mRad]");
  T2yRaxis->Draw();
	
  string T2ylegendentry = "Fit Width: ";
  string T2ylegendentry2 = "Fit Mean: ";
  T2ylegendentry += Form("%.2f",width_Tracker2ycoordRes);
  T2ylegendentry += " um";
  char T2ycharlegendentry[T2ylegendentry.size()];
  strcpy(T2ycharlegendentry, T2ylegendentry.c_str());

  T2ylegendentry2 += Form("%.2f",mean_Tracker2ycoordRes);
  T2ylegendentry2 += " um";
  char T2ycharlegendentry2[T2ylegendentry2.size()];
  strcpy(T2ycharlegendentry2, T2ylegendentry2.c_str());

  string T2ylegendentry3 = "Fit Width: ";
  string T2ylegendentry4 = "Fit Mean: ";
  T2ylegendentry3 += Form("%.2f",width_Tracker2ycoordRes/(T2R));
  T2ylegendentry3 += " mRad";
  char T2ycharlegendentry3[T2ylegendentry3.size()];
  strcpy(T2ycharlegendentry3, T2ylegendentry3.c_str());

  T2ylegendentry4 += Form("%.2f",mean_Tracker2ycoordRes/(T2R));
  T2ylegendentry4 += " mRad";
  char T2ycharlegendentry4[T2ylegendentry4.size()];
  strcpy(T2ycharlegendentry4, T2ylegendentry4.c_str());

  string T2ylegendentry5 = "Radius: ";	
  T2ylegendentry5 += Form("%.2f",T2R);
  T2ylegendentry5 += " mm";
  char T2ycharlegendentry5[T2ylegendentry5.size()+1];
  strcpy(T2ycharlegendentry5, T2ylegendentry5.c_str());

  TLatex latexT2y;
  latexT2y.SetTextSize(0.035);
  latexT2y.SetTextAlign(13);  //align at top
  latexT2y.SetNDC();
  latexT2y.DrawLatex(0.55,.85,T2ycharlegendentry);
  latexT2y.DrawLatex(0.55,.8,T2ycharlegendentry2);
  latexT2y.DrawLatex(0.55,.75,T2ycharlegendentry3);
  latexT2y.DrawLatex(0.55,.7,T2ycharlegendentry4);
  latexT2y.DrawLatex(0.55,.65,T2ycharlegendentry5);
  */	
		
  TrackerHits->cd(3);
  h_residual_Tracker3x->SetTitle("Tracker3x residual");
  h_residual_Tracker3x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
  h_residual_Tracker3x->Draw();
  /*	
	
  auto T3xRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker3x->GetMaximum()),rangevals,1.05*(h_residual_Tracker3x->GetMaximum()),-rangevals/(T3R),rangevals/(T3R),510,"-");	
  T3xRaxis->SetTitle("[mRad]");
  T3xRaxis->Draw();
	
  string T3xlegendentry = "Fit Width: ";
  string T3xlegendentry2 = "Fit Mean: ";
  T3xlegendentry += Form("%.2f",width_Tracker3xcoordRes);
  T3xlegendentry += " um";
  char T3xcharlegendentry[T3xlegendentry.size()];
  strcpy(T3xcharlegendentry, T3xlegendentry.c_str());

  T3xlegendentry2 += Form("%.2f",mean_Tracker3xcoordRes);
  T3xlegendentry2 += " um";
  char T3xcharlegendentry2[T3xlegendentry2.size()];
  strcpy(T3xcharlegendentry2, T3xlegendentry2.c_str());

  string T3xlegendentry3 = "Fit Width: ";
  string T3xlegendentry4 = "Fit Mean: ";
  T3xlegendentry3 += Form("%.2f",width_Tracker3xcoordRes/(T3R));
  T3xlegendentry3 += " mRad";
  char T3xcharlegendentry3[T3xlegendentry3.size()];
  strcpy(T3xcharlegendentry3, T3xlegendentry3.c_str());

  T3xlegendentry4 += Form("%.2f",mean_Tracker3xcoordRes/(T3R));
  T3xlegendentry4 += " mRad";
  char T3xcharlegendentry4[T3xlegendentry4.size()];
  strcpy(T3xcharlegendentry4, T3xlegendentry4.c_str());

  string T3xlegendentry5 = "Radius: ";	
  T3xlegendentry5 += Form("%.2f",T3R);
  T3xlegendentry5 += " mm";
  char T3xcharlegendentry5[T3xlegendentry5.size()+1];
  strcpy(T3xcharlegendentry5, T3xlegendentry5.c_str());

  TLatex latexT3x;
  latexT3x.SetTextSize(0.035);
  latexT3x.SetTextAlign(13);  //align at top
  latexT3x.SetNDC();
  latexT3x.DrawLatex(0.55,.85,T3xcharlegendentry);
  latexT3x.DrawLatex(0.55,.8,T3xcharlegendentry2);
  latexT3x.DrawLatex(0.55,.75,T3xcharlegendentry3);
  latexT3x.DrawLatex(0.55,.7,T3xcharlegendentry4);
  latexT3x.DrawLatex(0.55,.65,T3xcharlegendentry5);
  */	

	
	
  TrackerHits->cd(7);
  h_residual_Tracker3y->SetTitle("Tracker3y residual");
  h_residual_Tracker3y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
  h_residual_Tracker3y->Draw();
	
  /*	
  auto T3yRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker3y->GetMaximum()),rangevals,1.05*(h_residual_Tracker3y->GetMaximum()),-rangevals/(T3R),rangevals/(T3R),510,"-");	
  T3yRaxis->SetTitle("[mRad]");
  T3yRaxis->Draw();
	
  string T3ylegendentry = "Fit Width: ";
  string T3ylegendentry2 = "Fit Mean: ";
  T3ylegendentry += Form("%.2f",width_Tracker3ycoordRes);
  T3ylegendentry += " um";
  char T3ycharlegendentry[T3ylegendentry.size()];
  strcpy(T3ycharlegendentry, T3ylegendentry.c_str());

  T3ylegendentry2 += Form("%.2f",mean_Tracker3ycoordRes);
  T3ylegendentry2 += " um";
  char T3ycharlegendentry2[T3ylegendentry2.size()];
  strcpy(T3ycharlegendentry2, T3ylegendentry2.c_str());

  string T3ylegendentry3 = "Fit Width: ";
  string T3ylegendentry4 = "Fit Mean: ";
  T3ylegendentry3 += Form("%.2f",width_Tracker3ycoordRes/(T3R));
  T3ylegendentry3 += " mRad";
  char T3ycharlegendentry3[T3ylegendentry3.size()];
  strcpy(T3ycharlegendentry3, T3ylegendentry3.c_str());

  T3ylegendentry4 += Form("%.2f",mean_Tracker3ycoordRes/(T3R));
  T3ylegendentry4 += " mRad";
  char T3ycharlegendentry4[T3ylegendentry4.size()];
  strcpy(T3ycharlegendentry4, T3ylegendentry4.c_str());

  string T3ylegendentry5 = "Radius: ";	
  T3ylegendentry5 += Form("%.2f",T3R);
  T3ylegendentry5 += " mm";
  char T3ycharlegendentry5[T3ylegendentry5.size()+1];
  strcpy(T3ycharlegendentry5, T3ylegendentry5.c_str());

  TLatex latexT3y;
  latexT3y.SetTextSize(0.035);
  latexT3y.SetTextAlign(13);  //align at top
  latexT3y.SetNDC();
  latexT3y.DrawLatex(0.55,.85,T3ycharlegendentry);
  latexT3y.DrawLatex(0.55,.8,T3ycharlegendentry2);
  latexT3y.DrawLatex(0.55,.75,T3ycharlegendentry3);
  latexT3y.DrawLatex(0.55,.7,T3ycharlegendentry4);
  latexT3y.DrawLatex(0.55,.65,T3ycharlegendentry5);
	
  */
	
	
  TrackerHits->cd(4);
  h_residual_Tracker4x->SetTitle("Tracker4x residual");
  h_residual_Tracker4x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
  h_residual_Tracker4x->Draw();
  /*	

  auto T4xRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker4x->GetMaximum()),rangevals,1.05*(h_residual_Tracker4x->GetMaximum()),-rangevals/(T4R),rangevals/(T4R),510,"-");	
  T4xRaxis->SetTitle("[mRad]");
  T4xRaxis->Draw();
	
  string T4xlegendentry = "Fit Width: ";
  string T4xlegendentry2 = "Fit Mean: ";
  T4xlegendentry += Form("%.2f",width_Tracker4xcoordRes);
  T4xlegendentry += " um";
  char T4xcharlegendentry[T4xlegendentry.size()];
  strcpy(T4xcharlegendentry, T4xlegendentry.c_str());

  T4xlegendentry2 += Form("%.2f",mean_Tracker4xcoordRes);
  T4xlegendentry2 += " um";
  char T4xcharlegendentry2[T4xlegendentry2.size()];
  strcpy(T4xcharlegendentry2, T4xlegendentry2.c_str());

  string T4xlegendentry3 = "Fit Width: ";
  string T4xlegendentry4 = "Fit Mean: ";
  T4xlegendentry3 += Form("%.2f",width_Tracker4xcoordRes/(T4R));
  T4xlegendentry3 += " mRad";
  char T4xcharlegendentry3[T4xlegendentry3.size()];
  strcpy(T4xcharlegendentry3, T4xlegendentry3.c_str());

  T4xlegendentry4 += Form("%.2f",mean_Tracker4xcoordRes/(T4R));
  T4xlegendentry4 += " mRad";
  char T4xcharlegendentry4[T4xlegendentry4.size()];
  strcpy(T4xcharlegendentry4, T4xlegendentry4.c_str());

  string T4xlegendentry5 = "Radius: ";	
  T4xlegendentry5 += Form("%.2f",T4R);
  T4xlegendentry5 += " mm";
  char T4xcharlegendentry5[T4xlegendentry5.size()+1];
  strcpy(T4xcharlegendentry5, T4xlegendentry5.c_str());

  TLatex latexT4x;
  latexT4x.SetTextSize(0.035);
  latexT4x.SetTextAlign(13);  //align at top
  latexT4x.SetNDC();
  latexT4x.DrawLatex(0.55,.85,T4xcharlegendentry);
  latexT4x.DrawLatex(0.55,.8,T4xcharlegendentry2);
  latexT4x.DrawLatex(0.55,.75,T4xcharlegendentry3);
  latexT4x.DrawLatex(0.55,.7,T4xcharlegendentry4);
  latexT4x.DrawLatex(0.55,.65,T4xcharlegendentry5);
  */	
  TrackerHits->cd(8);
  h_residual_Tracker4y->SetTitle("Tracker4y residual");
  h_residual_Tracker4y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
  h_residual_Tracker4y->Draw();
	
  /*	
  auto T4yRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker4y->GetMaximum()),rangevals,1.05*(h_residual_Tracker4y->GetMaximum()),-rangevals/(T4R),rangevals/(T4R),510,"-");	
  T4yRaxis->SetTitle("[mRad]");
  T4yRaxis->Draw();
	
  string T4ylegendentry = "Fit Width: ";
  string T4ylegendentry2 = "Fit Mean: ";
  T4ylegendentry += Form("%.2f",width_Tracker4ycoordRes);
  T4ylegendentry += " um";
  char T4ycharlegendentry[T4ylegendentry.size()];
  strcpy(T4ycharlegendentry, T4ylegendentry.c_str());

  T4ylegendentry2 += Form("%.2f",mean_Tracker4ycoordRes);
  T4ylegendentry2 += " um";
  char T4ycharlegendentry2[T4ylegendentry2.size()];
  strcpy(T4ycharlegendentry2, T4ylegendentry2.c_str());

  string T4ylegendentry3 = "Fit Width: ";
  string T4ylegendentry4 = "Fit Mean: ";
  T4ylegendentry3 += Form("%.2f",width_Tracker4ycoordRes/(T4R));
  T4ylegendentry3 += " mRad";
  char T4ycharlegendentry3[T4ylegendentry3.size()];
  strcpy(T4ycharlegendentry3, T4ylegendentry3.c_str());

  T4ylegendentry4 += Form("%.2f",mean_Tracker4ycoordRes/(T4R));
  T4ylegendentry4 += " mRad";
  char T4ycharlegendentry4[T4ylegendentry4.size()];
  strcpy(T4ycharlegendentry4, T4ylegendentry4.c_str());

  string T4ylegendentry5 = "Radius: ";	
  T4ylegendentry5 += Form("%.2f",T4R);
  T4ylegendentry5 += " mm";
  char T4ycharlegendentry5[T4ylegendentry5.size()+1];
  strcpy(T4ycharlegendentry5, T4ylegendentry5.c_str());

  TLatex latexT4y;
  latexT4y.SetTextSize(0.035);
  latexT4y.SetTextAlign(13);  //align at top
  latexT4y.SetNDC();
  latexT4y.DrawLatex(0.55,.85,T4ycharlegendentry);
  latexT4y.DrawLatex(0.55,.8,T4ycharlegendentry2);
  latexT4y.DrawLatex(0.55,.75,T4ycharlegendentry3);
  latexT4y.DrawLatex(0.55,.7,T4ycharlegendentry4);
  latexT4y.DrawLatex(0.55,.65,T4ycharlegendentry5);
  */	
  gPad->Update();
	  
  TrackerHits->Draw();
  gPad->Update();
  //TrackerHits->SaveAs("CartesianAlignmentResiduals.png");
		

  TrackerHits->SaveAs("FinalResiduals_FixedAlignment_6Files.png");

}

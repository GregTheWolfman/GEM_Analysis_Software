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

  ofstream MyFile("Trackers_Hits_swappedtrackers.txt");    
  MyFile << "#PedFile: GEMandTrksPed_pedestaloptionfirst_dataTree01.root \n";
  MyFile << "#DataFiles: APV3_ADC_0_dataTree01.root \n";
  MyFile << "#Tracker1x, Tracker1y, Tracker2x, Tracker2y, Tracker3x, Tracker3y, Tracker4x, Tracker4y \n";


  ofstream LAGDhits("LAGD_hits_swappedtrackers.txt");
 
  
  LAGDhits << "#PedFile: GEMandTrksPed_pedestaloptionfirst_dataTree01.root \n";
  LAGDhits << "#DataFiles: APV3_ADC_0_dataTree01.root, APV3_ADC_1_dataTree01.root, APV3_ADC_2_dataTree01.root \n";
  LAGDhits << "#EventID, AVP3E1" << endl;

  
  vector<float> T1Hitcoords;
  vector<float> T2Hitcoords;
  vector<float> T3Hitcoords;
  vector<float> T4Hitcoords;
  vector<float> LAGDHitcoords;
  vector<float> FixedSMvec;
  vector<float> FixedCCvec;
  vector<float> FixedEvtidvec;
  
  cout << BadEvents.size() << endl;
  //now, actually remove the bad events from the final list of tracker/LAGD hits to go onto alignment
  cout << Tracker1HitlocVec.size() << endl;
  cout << LAGDHits.size() << endl;
  
  cout << "Total Events with bad events in **" << endl;

  auto SMdist = new TH1F("","Strip Multiplicity Distribution for removed Events; Number of strips involved; Frequency",5, 0.5,5.5);
  auto CCdist = new TH1F("","Cluster Charge Distribution for removed Events; Cluster charge [a.u.]; frequency",500, 0,2500);
  auto Angdist = new TH1F("","Hit Distribution for removed Events; Angle [#phi]; frequency",50, 0, 0.4);

  auto SMdistgd = new TH1F("","Strip Multiplicity Distribution for Accepted Events; Number of strips involved; Frequency",5, 0.5,5.5);
  auto CCdistgd = new TH1F("","Cluster Charge Distribution for Accepted Events; Cluster charge [a.u.]; frequency",500, 0,2500);
  auto Angdistgd = new TH1F("","Hit Distribution for Accepted Events; Angle [#phi]; frequency",50, 0, 0.4);

  auto Hitmap1 = new TH2F("", "Beam Spot on Tracker 1", 100, 0, 50, 100, 0, 50);
  auto Hitmap2 = new TH2F("", "Beam Spot on Tracker 2", 100, 0, 50, 100, 0, 50);
  
  for(auto i = 0; i < Tracker1HitlocVec.size();i++){
    //cout << i/3 << endl;
    if(find(BadEvents.begin(), BadEvents.end(), i/3) == BadEvents.end()){
      //cout << "ttackers" << endl;
      T1Hitcoords.push_back(Tracker1HitlocVec.at(i));T1Hitcoords.push_back(Tracker1HitlocVec.at(i+1));T1Hitcoords.push_back(Tracker1HitlocVec.at(i+2));
      T2Hitcoords.push_back(Tracker2HitlocVec.at(i));T2Hitcoords.push_back(Tracker2HitlocVec.at(i+1));T2Hitcoords.push_back(Tracker2HitlocVec.at(i+2));
      T3Hitcoords.push_back(Tracker3HitlocVec.at(i));T3Hitcoords.push_back(Tracker3HitlocVec.at(i+1));T3Hitcoords.push_back(Tracker3HitlocVec.at(i+2));
      T4Hitcoords.push_back(Tracker4HitlocVec.at(i));T4Hitcoords.push_back(Tracker4HitlocVec.at(i+1));T4Hitcoords.push_back(Tracker4HitlocVec.at(i+2));

      Hitmap1->Fill(Tracker1HitlocVec.at(i), Tracker1HitlocVec.at(i+1));
      Hitmap2->Fill(Tracker2HitlocVec.at(i), Tracker2HitlocVec.at(i+1));
      
      //cout << "LAGD" << endl;
      LAGDhits << LAGDHits.at(i/3) << endl;
      LAGDHitcoords.push_back(LAGDHits.at(i/3));
      //cout << "LAGD2" << endl;
     
      cout << SMvec.at(i/3) << endl;
      FixedSMvec.push_back(SMvec.at(i/3));
      SMdist->Fill(SMvec.at(i/3));
  
      FixedCCvec.push_back(CCvec.at(i/3));
      CCdist->Fill(CCvec.at(i/3));
      //cout << Tracker1HitlocVec.at(i) << endl;      
      //cout << Tracker1HitlocVec.at(i+1) << endl;
      //cout << Tracker1HitlocVec.at(i+2) << endl;
      cout << LAGDHits.at(i/3) << endl;
      
      Angdist->Fill(LAGDHits.at(i/3)*0.0041);
      i++;
      i++;
      //cout << i << "?" << endl;
    }
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
  
  auto h = new TCanvas("","", 1000, 500);
  h->Divide(2,1);
  h->cd(1);
  Hitmap1->Draw("colz");
  h->cd(2);
  Hitmap2->Draw("colz");
  

  h->Draw();
  return;
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

  


  //cout << "passed weird part" << endl;
  
 
  float Shift_T1x = OptimizedXYOffests[0];
  float Shift_T1y = OptimizedXYOffests[1];
  float Shift_T2x = OptimizedXYOffests[2];
  float Shift_T2y = OptimizedXYOffests[3];
  float Shift_T3x = OptimizedXYOffests[4];
  float Shift_T3y = OptimizedXYOffests[5];
  float Shift_T4x = OptimizedXYOffests[6];
  float Shift_T4y = OptimizedXYOffests[7];

  float angleT2=0;
  float angleT3=0;
  float angleT4=0;
  
  if(OptimizedXYOffests.size() > 8){
    float angleT2 = OptimizedXYOffests[8];
    float angleT3 = OptimizedXYOffests[9];
    float angleT4 = OptimizedXYOffests[10];
  }

  
  float T1Hitcoordsx_tmp=0;
  float T1Hitcoordsy_tmp=0;
  float T2Hitcoordsx_tmp=0;
  float T2Hitcoordsy_tmp=0;
  float T3Hitcoordsx_tmp=0;
  float T3Hitcoordsy_tmp=0;
  float T4Hitcoordsx_tmp=0;
  float T4Hitcoordsy_tmp=0;
  

  char nameRes2X[40];sprintf(nameRes2X,"PostTrackerAlignment_residual_Tracker2x");char nameRes2Y[40];sprintf(nameRes2Y,"PostTrackerAlignment_residual_Tracker2y");
  char nameRes3X[40];sprintf(nameRes3X,"PostTrackerAlignment_residual_Tracker3x");char nameRes3Y[40];sprintf(nameRes3Y,"PostTrackerAlignment_residual_Tracker3y");
  char nameRes1X[40];sprintf(nameRes1X,"PostTrackerAlignment_residual_Tracker1x");char nameRes1Y[40];sprintf(nameRes1Y,"PostTrackerAlignment_residual_Tracker1y");
  char nameRes4X[40];sprintf(nameRes4X,"PostTrackerAlignment_residual_Tracker4x");char nameRes4Y[40];sprintf(nameRes4Y,"PostTrackerAlignment_residual_Tracker4y");

  
  TH1F* h_residual_Tracker2x = new TH1F(nameRes2X,"",300,-0.5,0.5); h_residual_Tracker2x->SetXTitle("Residual [mm]"); h_residual_Tracker2x->SetYTitle("Frequency");h_residual_Tracker2x->SetLabelSize(0.042,"XY");h_residual_Tracker2x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker2y = new TH1F(nameRes2Y,"",300,-0.5,0.5); h_residual_Tracker2y->SetXTitle("Residual [mm]"); h_residual_Tracker2y->SetYTitle("Frequency");h_residual_Tracker2y->SetLabelSize(0.042,"XY");h_residual_Tracker2y->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker3x = new TH1F(nameRes3X,"",300,-0.5,0.5); h_residual_Tracker3x->SetXTitle("Residual [mm]"); h_residual_Tracker3x->SetYTitle("Frequency");h_residual_Tracker3x->SetLabelSize(0.042,"XY");h_residual_Tracker3x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker3y = new TH1F(nameRes3Y,"",300,-0.5,0.5); h_residual_Tracker3y->SetXTitle("Residual [mm]"); h_residual_Tracker3y->SetYTitle("Frequency");h_residual_Tracker3y->SetLabelSize(0.042,"XY");h_residual_Tracker3y->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker1x = new TH1F(nameRes1X,"",300,-0.5,0.5); h_residual_Tracker1x->SetXTitle("Residual [mm]"); h_residual_Tracker1x->SetYTitle("Frequency");h_residual_Tracker1x->SetLabelSize(0.042,"XY");h_residual_Tracker1x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker1y = new TH1F(nameRes1Y,"",300,-0.5,0.5); h_residual_Tracker1y->SetXTitle("Residual [mm]"); h_residual_Tracker1y->SetYTitle("Frequency");h_residual_Tracker1y->SetLabelSize(0.042,"XY");h_residual_Tracker1y->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker4x = new TH1F(nameRes4X,"",300,-0.5,0.5); h_residual_Tracker4x->SetXTitle("Residual [mm]"); h_residual_Tracker4x->SetYTitle("Frequency");h_residual_Tracker4x->SetLabelSize(0.042,"XY");h_residual_Tracker4x->SetTitleSize(0.042,"XY");
  TH1F* h_residual_Tracker4y = new TH1F(nameRes4Y,"",300,-0.5,0.5); h_residual_Tracker4y->SetXTitle("Residual [mm]"); h_residual_Tracker4y->SetYTitle("Frequency");h_residual_Tracker4y->SetLabelSize(0.042,"XY");h_residual_Tracker4y->SetTitleSize(0.042,"XY");


  for(unsigned int i=0; i< T1Hitcoords.size(); i++){//shift all of the XY hit coordinates
	  
    T1Hitcoords[i] = T1Hitcoords[i] - Shift_T1x;
    //T1Hitcoordsx_tmp = T1Hitcoords[i];
    T2Hitcoords[i] = T2Hitcoords[i] - Shift_T2x;
    //T2Hitcoordsx_tmp = T2Hitcoords[i]; 
    T3Hitcoords[i] = T3Hitcoords[i] - Shift_T3x;
    //T3Hitcoordsx_tmp = T3Hitcoords[i];
    T4Hitcoords[i] = T4Hitcoords[i] - Shift_T4x;
    //T4Hitcoordsx_tmp = T4Hitcoords[i];

    i++;//move to y components
	  
    T1Hitcoords[i] = T1Hitcoords[i] - Shift_T1y;
    //T1Hitcoordsy_tmp = T1Hitcoords[i];
    T2Hitcoords[i] = T2Hitcoords[i] - Shift_T2y;
    //T2Hitcoordsy_tmp = T2Hitcoords[i];
    T3Hitcoords[i] = T3Hitcoords[i] - Shift_T3y;
    //T3Hitcoordsy_tmp = T3Hitcoords[i];
    T4Hitcoords[i] = T4Hitcoords[i] - Shift_T4y;
    //T4Hitcoordsy_tmp = T4Hitcoords[i];
     

    i++;
  }

  for(unsigned int i=0; i< T1Hitcoords.size(); i++){
      
    T2Hitcoordsx_tmp = T2Hitcoords[i]; 	
    T2Hitcoords[i] = T2Hitcoords[i]*cos(angleT2) - T2Hitcoords[i+1]*sin(angleT2);	  
      
    T3Hitcoordsx_tmp = T3Hitcoords[i]; 	
    T3Hitcoords[i] = T3Hitcoords[i]*cos(angleT3) - T3Hitcoords[i+1]*sin(angleT3);	  
    
    T4Hitcoordsx_tmp = T4Hitcoords[i]; 	
    T4Hitcoords[i] = T4Hitcoords[i]*cos(angleT4) - T4Hitcoords[i+1]*sin(angleT4);	  
      
    
    i++;//move to y components
    
    T2Hitcoords[i] = T2Hitcoordsx_tmp*sin(angleT2) + T2Hitcoords[i]*cos(angleT2);	  
    
    T3Hitcoords[i] = T3Hitcoordsx_tmp*sin(angleT3) + T3Hitcoords[i]*cos(angleT3);	  
    T4Hitcoords[i] = T4Hitcoordsx_tmp*sin(angleT4) + T4Hitcoords[i]*cos(angleT4);	  
    
    
    MyFile << T1Hitcoords[i-1] << " " << T1Hitcoords[i] << " " <<T2Hitcoords[i-1] << " " << T2Hitcoords[i] << " " <<T3Hitcoords[i-1] << " " << T3Hitcoords[i] << " " <<T4Hitcoords[i-1] << " " << T4Hitcoords[i] << endl;

   
    
    i++;         	  
  }
  
  MyFile.close();
  //cout << T1Hitcoords.size() << endl;
  //ResCut(T1Hitcoords, T2Hitcoords, T3Hitcoords, T4Hitcoords);
  //cout << T1Hitcoords.size() << endl;
  
  auto NewRes = Residuals(T1Hitcoords, T2Hitcoords, T3Hitcoords, T4Hitcoords);
  auto Chisquares = ChiSquared(T1Hitcoords, T2Hitcoords, T3Hitcoords, T4Hitcoords);
  

  for(unsigned int i=0; i< NewRes.size(); i++){//Put all residuals into plots

    h_residual_Tracker1x->Fill(NewRes[i]);	 
    i++;
    h_residual_Tracker1y->Fill(NewRes[i]);
    i++;
    h_residual_Tracker2x->Fill(NewRes[i]);
    i++;
    h_residual_Tracker2y->Fill(NewRes[i]);
    i++;
    h_residual_Tracker3x->Fill(NewRes[i]);
    i++;
    h_residual_Tracker3y->Fill(NewRes[i]);
    i++;
    h_residual_Tracker4x->Fill(NewRes[i]);
    i++;
    h_residual_Tracker4y->Fill(NewRes[i]);
  }
 
    
  TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 2000, 1000);
  TrackerRes->Divide(4,2);

  TrackerRes->cd(1);
  h_residual_Tracker1x->SetTitle("Tracker1x residual");
  h_residual_Tracker1x->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker1x->Draw();
  h_residual_Tracker1x->SaveAs("ResT1.png");
  TrackerRes->cd(5);
  h_residual_Tracker1y->SetTitle("Tracker1y residual");
  h_residual_Tracker1y->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker1y->Draw();
  TrackerRes->cd(2);
  h_residual_Tracker2x->SetTitle("Tracker2x residual");
  h_residual_Tracker2x->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker2x->Draw();
  TrackerRes->cd(6);
  h_residual_Tracker2y->SetTitle("Tracker2y residual");
  h_residual_Tracker2y->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker2y->Draw();
  TrackerRes->cd(3);
  h_residual_Tracker3x->SetTitle("Tracker3x residual");
  h_residual_Tracker3x->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker3x->Draw();
  TrackerRes->cd(7);
  h_residual_Tracker3y->SetTitle("Tracker3y residual");
  h_residual_Tracker3y->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker3y->Draw();
  TrackerRes->cd(4);
  h_residual_Tracker4x->SetTitle("Tracker4x residual");
  h_residual_Tracker4x->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker4x->Draw();
  TrackerRes->cd(8);
  h_residual_Tracker4y->SetTitle("Tracker4y residual");
  h_residual_Tracker4y->GetXaxis()->SetRangeUser(-0.5, 0.5);
  h_residual_Tracker4y->Draw();
	  
  gPad->Update();
	  
  TrackerRes->Draw();
  gPad->Update();
  TrackerRes->SaveAs("FinalResiduals_All20FilesAllEvents.png");

}

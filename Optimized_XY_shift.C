vector<float> Optimized_XY_shift(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> OptimizedOffests, vector<float> BadEvents, bool plots){//find optimized XY offsets 

  
  vector<float> OptimizedXYOffsets;
  
  if(Tracker1HitlocVec.empty() || Tracker2HitlocVec.empty() || Tracker3HitlocVec.empty() || Tracker4HitlocVec.empty()){
    cout << "No hits in vectors" << endl;
    return OptimizedXYOffsets;
  }
  cout << "past here?" << endl;


  float Shift_T1x;
  float Shift_T1y;
  float Shift_T2x;
  float Shift_T2y;
  float Shift_T3x;
  float Shift_T3y;
  float Shift_T4x;
  float Shift_T4y;
  float REf2ang;
  float REf3ang;
  float REf4ang;
  
  
  if(OptimizedOffests.empty()){
    Shift_T1x = 0.0;
    Shift_T1y = 0.0;
    Shift_T2x = 0.0;
    Shift_T2y = 0.0;
    Shift_T3x = 0.0;
    Shift_T3y = 0.0;
    Shift_T4x = 0.0;
    Shift_T4y = 0.0;
    REf2ang=0.0;
    REf3ang=0.0;
    REf4ang=0.0;
  
  }
  else {
    Shift_T1x = OptimizedOffests[0];
    Shift_T1y = OptimizedOffests[1];
    Shift_T2x = OptimizedOffests[2];
    Shift_T2y = OptimizedOffests[3];
    Shift_T3x = OptimizedOffests[4];
    Shift_T3y = OptimizedOffests[5];
    Shift_T4x = OptimizedOffests[6];
    Shift_T4y = OptimizedOffests[7];
    REf2ang = OptimizedOffests[8];
    REf3ang = OptimizedOffests[9];
    REf4ang = OptimizedOffests[10];
    
    
  }

  double mean_Tracker1xcoordRes=0.0, mean_Tracker1ycoordRes=0.0;
  double mean_Tracker2xcoordRes=0.0, mean_Tracker2ycoordRes=0.0;
  double mean_Tracker3xcoordRes=0.0, mean_Tracker3ycoordRes=0.0;
  double mean_Tracker4xcoordRes=0.0, mean_Tracker4ycoordRes=0.0;  

  double width_Tracker1xcoordRes=0.0, width_Tracker1ycoordRes=0.0;
  double width_Tracker2xcoordRes=0.0, width_Tracker2ycoordRes=0.0;
  double width_Tracker3xcoordRes=0.0, width_Tracker3ycoordRes=0.0;
  double width_Tracker4xcoordRes=0.0, width_Tracker4ycoordRes=0.0;  

  
  int Iterator=0;
  float Total1Xshift=0.0;
  float Total1Yshift=0.0;
  float Total2Xshift=0.0;
  float Total2Yshift=0.0;
  float Total3Xshift=0.0;
  float Total3Yshift=0.0;
  float Total4Xshift=0.0;
  float Total4Yshift=0.0;

 
  TGraph* Residual1XPerIteration = new TGraph();Residual1XPerIteration->GetXaxis()->SetTitle("Iterations");Residual1XPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual1XPerIteration->SetTitle("Residual Per Iteration in Tracker1 x coordinate");
  TGraph* Residual1YPerIteration = new TGraph();Residual1YPerIteration->GetXaxis()->SetTitle("Iterations");Residual1YPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual1YPerIteration->SetTitle("Residual Per Iteration in Tracker1 y coordinate");
  TGraph* Residual2XPerIteration = new TGraph();Residual2XPerIteration->GetXaxis()->SetTitle("Iterations");Residual2XPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual2XPerIteration->SetTitle("Residual Per Iteration in Tracker2 x coordinate");
  TGraph* Residual2YPerIteration = new TGraph();Residual2YPerIteration->GetXaxis()->SetTitle("Iterations");Residual2YPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual2YPerIteration->SetTitle("Residual Per Iteration in Tracker2 y coordinate");
  TGraph* Residual3XPerIteration = new TGraph();Residual3XPerIteration->GetXaxis()->SetTitle("Iterations");Residual3XPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual3XPerIteration->SetTitle("Residual Per Iteration in Tracker3 x coordinate");
  TGraph* Residual3YPerIteration = new TGraph();Residual3YPerIteration->GetXaxis()->SetTitle("Iterations");Residual3YPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual3YPerIteration->SetTitle("Residual Per Iteration in Tracker3 y coordinate");
  TGraph* Residual4XPerIteration = new TGraph();Residual4XPerIteration->GetXaxis()->SetTitle("Iterations");Residual4XPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual4XPerIteration->SetTitle("Residual Per Iteration in Tracker4 x coordinate");
  TGraph* Residual4YPerIteration = new TGraph();Residual4YPerIteration->GetXaxis()->SetTitle("Iterations");Residual4YPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");Residual4YPerIteration->SetTitle("Residual Per Iteration in Tracker4 y coordinate");
  
  TGraph* TrackerResidualsPerIteration = new TGraph();TrackerResidualsPerIteration->GetXaxis()->SetTitle("Iterations");TrackerResidualsPerIteration->GetYaxis()->SetTitle("Mean Residual (#mu m)");TrackerResidualsPerIteration->SetTitle("Iteration");

  TGraph* xChi2PerIteration = new TGraph();xChi2PerIteration->GetXaxis()->SetTitle("Iterator");xChi2PerIteration->GetYaxis()->SetTitle("#chi^2");xChi2PerIteration->SetTitle("#chi^2 of track in X-Z projection plane");
  TGraph* yChi2PerIteration = new TGraph();yChi2PerIteration->GetXaxis()->SetTitle("Iterator");yChi2PerIteration->GetYaxis()->SetTitle("#chi^2");yChi2PerIteration->SetTitle("#chi^2 of track in Y-Z projection plane");

  TGraph* Shift1XPerIteration = new TGraph();Shift1XPerIteration->GetXaxis()->SetTitle("Iterations");Shift1XPerIteration->GetYaxis()->SetTitle("Shift value");Shift1XPerIteration->SetTitle("Shift Per Iteration in Tracker1 x coordinate");
  TGraph* Shift2XPerIteration = new TGraph();Shift2XPerIteration->GetXaxis()->SetTitle("Iterations");Shift2XPerIteration->GetYaxis()->SetTitle("Shift value");Shift2XPerIteration->SetTitle("Shift Per Iteration in Tracker2 x coordinate");
  
  map<int, TH2F*> Tracker1HitmapPerRotation;
  map<int, TH2F*> Tracker2HitmapPerRotation;
  map<int, TH2F*> Tracker3HitmapPerRotation;
  map<int, TH2F*> Tracker4HitmapPerRotation;

  float prealignedX =  Tracker1HitlocVec[0];
  float prealignedY =  Tracker1HitlocVec[1];   
  while(1){    

  
    Iterator++;

    
    char name2X[15];sprintf(name2X,"Pos_g2xcl_%i",Iterator); char name2Y[15];sprintf(name2Y,"Pos_g2ycl_%i",Iterator);
    char name3X[15];sprintf(name3X,"Pos_g3xcl_%i",Iterator); char name3Y[15];sprintf(name3Y,"Pos_g3ycl_%i",Iterator);
    char name1X[15];sprintf(name1X,"Pos_g1xcl_%i",Iterator); char name1Y[15];sprintf(name1Y,"Pos_g1xcl_%i",Iterator);
    char name4X[15];sprintf(name4X,"Pos_g4xcl_%i",Iterator); char name4Y[15];sprintf(name4Y,"Pos_g4xcl_%i",Iterator);

    
    TH1F* h_Pos_g2xcl = new TH1F(name2X,"",300,-110,110); h_Pos_g2xcl->SetXTitle("Cluster position [mm]"); h_Pos_g2xcl->SetYTitle("Frequency");h_Pos_g2xcl->SetLabelSize(0.045,"XY");h_Pos_g2xcl->SetTitleSize(0.045,"XY");
    TH1F* h_Pos_g2ycl = new TH1F(name2Y,"",300,-110,110); h_Pos_g2ycl->SetXTitle(" Cluster position [mm]"); h_Pos_g2ycl->SetYTitle("Frequency");h_Pos_g2ycl->SetLabelSize(0.045,"XY");h_Pos_g2ycl->SetTitleSize(0.045,"XY");  
    TH1F* h_Pos_g1xcl = new TH1F(name1X,"",300,-110,110); h_Pos_g1xcl->SetXTitle("Cluster position [mm]"); h_Pos_g1xcl->SetYTitle("Frequency");h_Pos_g1xcl->SetLabelSize(0.045,"XY");h_Pos_g1xcl->SetTitleSize(0.045,"XY");
    TH1F* h_Pos_g1ycl = new TH1F(name1Y,"",300,-110,110); h_Pos_g1ycl->SetXTitle("Cluster position [mm]"); h_Pos_g1ycl->SetYTitle("Frequency");h_Pos_g1ycl->SetLabelSize(0.045,"XY");h_Pos_g1ycl->SetTitleSize(0.045,"XY");  
    TH1F* h_Pos_g3xcl = new TH1F(name3X,"",300,-110,110); h_Pos_g3xcl->SetXTitle("Cluster position [mm]"); h_Pos_g3xcl->SetYTitle("Frequency");h_Pos_g3xcl->SetLabelSize(0.045,"XY");h_Pos_g3xcl->SetTitleSize(0.045,"XY");
    TH1F* h_Pos_g3ycl = new TH1F(name3Y,"",300,-110,110); h_Pos_g3ycl->SetXTitle(" Cluster position [mm]"); h_Pos_g3ycl->SetYTitle("Frequency");h_Pos_g3ycl->SetLabelSize(0.045,"XY");h_Pos_g3ycl->SetTitleSize(0.045,"XY");
    TH1F* h_Pos_g4xcl = new TH1F(name4X,"",300,-110,110); h_Pos_g4xcl->SetXTitle("Cluster position [mm]"); h_Pos_g4xcl->SetYTitle("Frequency");h_Pos_g4xcl->SetLabelSize(0.045,"XY");h_Pos_g4xcl->SetTitleSize(0.045,"XY");
    TH1F* h_Pos_g4ycl = new TH1F(name4Y,"",300,-110,110); h_Pos_g4ycl->SetXTitle("Cluster position [mm]"); h_Pos_g4ycl->SetYTitle("Frequency");h_Pos_g4ycl->SetLabelSize(0.045,"XY");h_Pos_g4ycl->SetTitleSize(0.045,"XY");
          
   
    char nameRes2X[30];sprintf(nameRes2X,"residual_Tracker2x_%i",Iterator);char nameRes2Y[30];sprintf(nameRes2Y,"residual_Tracker2y_%i",Iterator);
    char nameRes3X[30];sprintf(nameRes3X,"residual_Tracker3x_%i",Iterator);char nameRes3Y[30];sprintf(nameRes3Y,"residual_Tracker3y_%i",Iterator);
    char nameRes1X[30];sprintf(nameRes1X,"residual_Tracker1x_%i",Iterator);char nameRes1Y[30];sprintf(nameRes1Y,"residual_Tracker1y_%i",Iterator);
    char nameRes4X[30];sprintf(nameRes4X,"residual_Tracker4x_%i",Iterator);char nameRes4Y[30];sprintf(nameRes4Y,"residual_Tracker4y_%i",Iterator);


    char nameRot1[30];sprintf(nameRot1,"Rotation_Tracker1_%i",Iterator);
    char nameRot2[30];sprintf(nameRot2,"Rotation_Tracker2_%i",Iterator);
    char nameRot3[30];sprintf(nameRot3,"Rotation_Tracker3_%i",Iterator);
    char nameRot4[30];sprintf(nameRot4,"Rotation_Tracker4_%i",Iterator);
    char nameRotCanvas[30];sprintf(nameRotCanvas,"Rotation_canvas_%i",Iterator);

    char titleRot1[30];sprintf(titleRot1,"Rotation_Tracker1_%i",Iterator);
    char titleRot2[30];sprintf(titleRot2,"Rotation_Tracker2_%i",Iterator);
    char titleRot3[30];sprintf(titleRot3,"Rotation_Tracker3_%i",Iterator);
    char titleRot4[30];sprintf(titleRot4,"Rotation_Tracker4_%i",Iterator);
    char titleRotCanvas[30];sprintf(titleRotCanvas,"Rotation_canvas_%i",Iterator);
    
    int resbounds = 100;
    int bincountMultiplier = 80;
    TH1F* h_residual_Tracker2x = new TH1F(nameRes2X,"Tracker 2 x-Residual after 281 Iterations", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker2x->SetXTitle("Residual [mm]"); h_residual_Tracker2x->SetYTitle("Counts/0.025mm");h_residual_Tracker2x->SetLabelSize(0.045,"XY");h_residual_Tracker2x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker2y = new TH1F(nameRes2Y,"", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker2y->SetXTitle("Residual [mm]"); h_residual_Tracker2y->SetYTitle("Counts/0.025mm");h_residual_Tracker2y->SetLabelSize(0.045,"XY");h_residual_Tracker2y->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker3x = new TH1F(nameRes3X,"Tracker 3 x-Residual after 281 Iterations", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker3x->SetXTitle("Residual [mm]"); h_residual_Tracker3x->SetYTitle("Counts/0.01mm");h_residual_Tracker3x->SetLabelSize(0.045,"XY");h_residual_Tracker3x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker3y = new TH1F(nameRes3Y,"", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker3y->SetXTitle("Residual [mm]"); h_residual_Tracker3y->SetYTitle("Counts/0.025mm");h_residual_Tracker3y->SetLabelSize(0.045,"XY");h_residual_Tracker3y->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker1x = new TH1F(nameRes1X,"Tracker 1 x-Residual after 281 Iterations", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker1x->SetXTitle("Residual [mm]"); h_residual_Tracker1x->SetYTitle("Counts/0.01mm");h_residual_Tracker1x->SetLabelSize(0.045,"XY");h_residual_Tracker1x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker1y = new TH1F(nameRes1Y,"", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker1y->SetXTitle("Residual [mm]"); h_residual_Tracker1y->SetYTitle("Counts/0.025mm");h_residual_Tracker1y->SetLabelSize(0.045,"XY");h_residual_Tracker1y->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker4x = new TH1F(nameRes4X,"Tracker 4 x-Residual after 281 Iterations", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker4x->SetXTitle("Residual [mm]"); h_residual_Tracker4x->SetYTitle("Counts/0.01mm");h_residual_Tracker4x->SetLabelSize(0.045,"XY");h_residual_Tracker4x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker4y = new TH1F(nameRes4Y,"", bincountMultiplier*resbounds,-resbounds,resbounds); h_residual_Tracker4y->SetXTitle("Residual [mm]"); h_residual_Tracker4y->SetYTitle("Counts/0.025mm");h_residual_Tracker4y->SetLabelSize(0.045,"XY");h_residual_Tracker4y->SetTitleSize(0.045,"XY");

    /*
    Tracker1HitmapPerRotation[Iterator] = new TH2F(nameRes1X, "", 100, -50, 50, 100, -50, 50);
    Tracker3HitmapPerRotation[Iterator] = new TH2F(nameRot3, "", 100, -50, 50, 100, -50, 50);
    Tracker2HitmapPerRotation[Iterator] = new TH2F(nameRot2, "", 100, -50, 50, 100, -50, 50);
    Tracker4HitmapPerRotation[Iterator] = new TH2F(nameRot4, "", 100, -50, 50, 100, -50, 50);
    */
    //TH1F* DiffBtwnT12Y = new TH1F(nameRes2X,"Difference between Trackers 3 and 4 Y coordinate", 100,-5,5); DiffBtwnT12Y->SetXTitle("Difference between Trackers 3 and 4 Y coordinate [mm]"); DiffBtwnT12Y->SetYTitle("Counts/0.01mm");DiffBtwnT12Y->SetLabelSize(0.045,"XY");DiffBtwnT12Y->SetTitleSize(0.045,"XY");
    /*
    cout << "Tracker 1x shifted by: " << Shift_T1x << endl;
    Shift1XPerIteration->SetPoint(Iterator-1, Iterator, Shift_T1x);
    cout << "Tracker 2x shifted by: " << Shift_T2x << endl;
    Shift2XPerIteration->SetPoint(Iterator-1, Iterator, Shift_T2x);
    */
    
    for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){//shift all of the XY hit coordinates

      Tracker1HitlocVec[i] = Tracker1HitlocVec[i] - Shift_T1x;
      //Tracker1xHitlocVec_tmp = Tracker1HitlocVec[i];
	  
      Tracker2HitlocVec[i] = Tracker2HitlocVec[i] - Shift_T2x;
      // Tracker2xHitlocVec_tmp = Tracker2HitlocVec[i];
      //cout << "Shifted 2X coordinate: " << Tracker2xHitlocVec_tmp << endl;
	  
      Tracker3HitlocVec[i] = Tracker3HitlocVec[i] - Shift_T3x;
      //Tracker3xHitlocVec_tmp = Tracker3HitlocVec[i];

      Tracker4HitlocVec[i] = Tracker4HitlocVec[i] - Shift_T4x;
      //Tracker4xHitlocVec_tmp = Tracker4HitlocVec[i];

      h_Pos_g1xcl->Fill(Tracker1HitlocVec[i]);////////////////////////////////////put this in the function below this one :)
      h_Pos_g2xcl->Fill(Tracker2HitlocVec[i]);
      h_Pos_g3xcl->Fill(Tracker3HitlocVec[i]);
      h_Pos_g4xcl->Fill(Tracker4HitlocVec[i]);
            
      i++;//move to y components

      Tracker1HitlocVec[i] = Tracker1HitlocVec[i] - Shift_T1y;
      //Tracker1yHitlocVec_tmp = Tracker1HitlocVec[i];
	  
      Tracker2HitlocVec[i] = Tracker2HitlocVec[i] - Shift_T2y;
     
      
     
      
      //Tracker2yHitlocVec_tmp = Tracker2HitlocVec[i];//need these non-rotated values for rotating the xy components next
      //cout << "Shifted 2Y coordinate: " << Tracker2yHitlocVec_tmp << endl;
	  
      Tracker3HitlocVec[i] = Tracker3HitlocVec[i] - Shift_T3y;
      //Tracker3yHitlocVec_tmp = Tracker3HitlocVec[i];
	  
      Tracker4HitlocVec[i] = Tracker4HitlocVec[i] - Shift_T4y;
      //Tracker4yHitlocVec_tmp = Tracker4HitlocVec[i];
      //DiffBtwnT12Y->Fill(Tracker4HitlocVec[i] - Tracker3HitlocVec[i]);
      
      h_Pos_g1ycl->Fill(Tracker1HitlocVec[i]);
      h_Pos_g2ycl->Fill(Tracker2HitlocVec[i]);
      h_Pos_g3ycl->Fill(Tracker3HitlocVec[i]);
      h_Pos_g4ycl->Fill(Tracker4HitlocVec[i]);

      //Tracker2HitmapPerRotation[Iterator]->Fill(Tracker2HitlocVec[i-1], Tracker2HitlocVec[i]);
      //Tracker1HitmapPerRotation[Iterator]->Fill(Tracker1HitlocVec[i-1], Tracker1HitlocVec[i]);
      
      i++;
    }

    /*
    for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){//rotate too

      float Tracker2HitlocVecx_tmp = Tracker2HitlocVec[i]; 	
      Tracker2HitlocVec[i] = Tracker2HitlocVec[i]*cos(REf2ang) - Tracker2HitlocVec[i+1]*sin(REf2ang);	  
      
      float Tracker3HitlocVecx_tmp = Tracker3HitlocVec[i]; 	
      Tracker3HitlocVec[i] = Tracker3HitlocVec[i]*cos(REf3ang) - Tracker3HitlocVec[i+1]*sin(REf3ang);	  

      float Tracker4HitlocVecx_tmp = Tracker4HitlocVec[i]; 	
      Tracker4HitlocVec[i] = Tracker4HitlocVec[i]*cos(REf4ang) - Tracker4HitlocVec[i+1]*sin(REf4ang);	  
      
      
      i++;//move to y components
	
      Tracker2HitlocVec[i] = Tracker2HitlocVecx_tmp*sin(REf2ang) + Tracker2HitlocVec[i]*cos(REf2ang);	  
      
      Tracker3HitlocVec[i] = Tracker3HitlocVecx_tmp*sin(REf3ang) + Tracker3HitlocVec[i]*cos(REf3ang);	  
      Tracker4HitlocVec[i] = Tracker4HitlocVecx_tmp*sin(REf4ang) + Tracker4HitlocVec[i]*cos(REf4ang);	  
 
      
      i++;
      
    }
    */
    /*
    TCanvas* yea = new TCanvas("","", 500, 500);
    DiffBtwnT12Y->Draw();
    yea->Draw();
    break;
    */
    /*
    if(plots){
      TCanvas* TrackerHitmaps = new TCanvas(nameRotCanvas, "", 1000, 500);
    
      TrackerHitmaps->Divide(2,1);
      TrackerHitmaps->cd(1);
      Tracker1HitmapPerRotation[Iterator]->Draw("colz");
      gPad->Update();
	  
      TrackerHitmaps->cd(2);
      Tracker2HitmapPerRotation[Iterator]->Draw("colz");
      gPad->Update();
	
      TrackerHitmaps->Draw();
      gPad->Update();
      //gSystem->ProcessEvents();
      //gSystem->ProcessEvents();
      TrackerHitmaps->SaveAs(nameRotCanvas);
	
    }
    */

    //vector<float> BadEvents;
    //cout << Tracker1HitlocVec.size()/3 << endl;
    //if(Iterator == 1)
    //  BadEvents = ResCut(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec);
    //cout << Tracker1HitlocVec.size()/3 << endl;

    
    auto NewRes = Residuals(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec, BadEvents);

    //break;

    //return NewRes;

    auto Chisquares = ChiSquared(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec, BadEvents);

        
    xChi2PerIteration->SetPoint(Iterator-1, Iterator, Chisquares[0]);
    yChi2PerIteration->SetPoint(Iterator-1, Iterator, Chisquares[1]);
  
    //cout << "Size of Residual vector: " << NewRes.size() << endl;
    
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
    	
    //return OptimizedXYOffsets; 

    I2GFvalues myValues;
	
    //might need to clone to see the OG histogram if need be!

    //Looking for which residuals converge to 0!
	
    myValues = I2GFmainLoop(h_residual_Tracker1x,1,4,1);                           //converges!	  
    mean_Tracker1xcoordRes = myValues.mean; width_Tracker1xcoordRes=myValues.sigma;
    cout << "Mean Residual in 1x coordinate: " << mean_Tracker1xcoordRes << endl;
    cout << "Residual width in 1x coordinate: " << width_Tracker1xcoordRes << endl;
    
    myValues = I2GFmainLoop(h_residual_Tracker1y,1,4,1);	                        
    mean_Tracker1ycoordRes = myValues.mean; width_Tracker1ycoordRes=myValues.sigma;
    //mean_Tracker1ycoordRes = h_residual_Tracker1y->GetMean(); //sigmaEta5=myValues.sigma;
    cout << "Mean Residual in 1y coordinate: " << mean_Tracker1ycoordRes << endl;
    cout << "Residual width in 1y coordinate: " << width_Tracker1ycoordRes << endl;
    
    myValues = I2GFmainLoop(h_residual_Tracker2x,1,4,1);                               //converges!	  
    mean_Tracker2xcoordRes = myValues.mean;
    cout << "Mean Residual in 2x coordinate: " << mean_Tracker2xcoordRes << endl;

    myValues = I2GFmainLoop(h_residual_Tracker2y,1,4,1);	                            //breaks :(... im sensing a pattern...
    //mean_Tracker2ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
    mean_Tracker2ycoordRes = h_residual_Tracker2y->GetMean(); //sigmaEta5=myValues.    
    cout << "Mean Residual in 2y coordinate: " << mean_Tracker2ycoordRes << endl;
    
    myValues = I2GFmainLoop(h_residual_Tracker3x,1,4,1);
    mean_Tracker3xcoordRes = myValues.mean;	
    cout << "Mean Residual in 3x coordinate: " << mean_Tracker3xcoordRes << endl;

    myValues = I2GFmainLoop(h_residual_Tracker3y,1,4,1);	  
    mean_Tracker3ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
    cout << "Mean Residual in 3y coordinate: " << mean_Tracker3ycoordRes << endl;

    myValues = I2GFmainLoop(h_residual_Tracker4x,1,4,1);
    mean_Tracker4xcoordRes = myValues.mean;
    cout << "Mean Residual in 4x coordinate: " << mean_Tracker4xcoordRes << endl;

    myValues = I2GFmainLoop(h_residual_Tracker4y,1,10,1);	  
    mean_Tracker4ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
    cout << "Mean Residual in 4y coordinate: " << mean_Tracker4ycoordRes << endl;
    
	
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual1XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker1xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual1YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker1ycoordRes);
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual2XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker2xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual2YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker2ycoordRes);
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual3XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker3xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual3YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker3ycoordRes);
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual4XPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker4xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual4YPerIteration->SetPoint(Iterator-1, Iterator, mean_Tracker4ycoordRes);

	
    if((mean_Tracker1xcoordRes >= -0.005 && mean_Tracker1xcoordRes <= 0.005) && (mean_Tracker1ycoordRes >= -0.005 && mean_Tracker1ycoordRes <= 0.005) && (mean_Tracker2xcoordRes >= -0.005 && mean_Tracker2xcoordRes <= 0.005) && (mean_Tracker2ycoordRes >= -0.005 && mean_Tracker2ycoordRes <= 0.005) && (mean_Tracker3xcoordRes >= -0.005 && mean_Tracker3xcoordRes <= 0.005) && (mean_Tracker3ycoordRes >= -0.005 && mean_Tracker3ycoordRes <= 0.005) && (mean_Tracker4xcoordRes >= -0.005 && mean_Tracker4xcoordRes <= 0.005) && (mean_Tracker4ycoordRes >= -0.005 && mean_Tracker4ycoordRes <= 0.005)){ 
      
      OptimizedXYOffsets.push_back(Total1Xshift); OptimizedXYOffsets.push_back(Total1Yshift);
      OptimizedXYOffsets.push_back(Total2Xshift); OptimizedXYOffsets.push_back(Total2Yshift);
      OptimizedXYOffsets.push_back(Total3Xshift); OptimizedXYOffsets.push_back(Total3Yshift);
      OptimizedXYOffsets.push_back(Total4Xshift); OptimizedXYOffsets.push_back(Total4Yshift);

      cout << "Pre Aligment first X value: " << prealignedX << endl;
      cout << "Pre Aligment first Y value: " << prealignedY << endl;
           
      cout << "PostAligment first X value: " << Tracker1HitlocVec[0] << endl;
      cout << "PostAligment first Y value: " << Tracker1HitlocVec[1] << endl;
    
      cout<<"find it...iterating "<< Iterator <<" times."<<endl;
      if(plots){
	/*
	TCanvas* TrackerHits = new TCanvas(name2X, "", 2000, 1000);
	TrackerHits->Divide(4,2);

	//int rangevals = 1;
	TrackerHits->cd(1);
	//h_residual_Tracker1x->SetTitle("Tracker1x residual");
	//h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g1xcl->Draw();
	TrackerHits->cd(5);
	//h_residual_Tracker1y->SetTitle("Tracker1y residual");
	//h_residual_Tracker1y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g1ycl->Draw();
	TrackerHits->cd(2);
	//h_residual_Tracker2x->SetTitle("Tracker2x residual");
	//h_residual_Tracker2x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g2xcl->Draw();
	TrackerHits->cd(6);
	//h_residual_Tracker2y->SetTitle("Tracker2y residual");
	//h_residual_Tracker2y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g2ycl->Draw();
	TrackerHits->cd(3);
	//h_residual_Tracker3x->SetTitle("Tracker3x residual");
	//h_residual_Tracker3x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g3xcl->Draw();
	TrackerHits->cd(7);
	//h_residual_Tracker3y->SetTitle("Tracker3y residual");
	//h_residual_Tracker3y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g3ycl->Draw();
	TrackerHits->cd(4);
	//h_residual_Tracker4x->SetTitle("Tracker4x residual");
	//h_residual_Tracker4x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g4xcl->Draw();
	TrackerHits->cd(8);
	//h_residual_Tracker4y->SetTitle("Tracker4y residual");
	//h_residual_Tracker4y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g4ycl->Draw();
	  
	gPad->Update();
	  
	TrackerHits->Draw();
	gPad->Update();
	*/
	TCanvas* c1 = new TCanvas("c1","", 700,500);
	c1->cd(1);
	Residual1XPerIteration->SetTitle("Mean XY Residuals of Trackers 1-4");
	Residual1XPerIteration->SetMarkerStyle(20);
	Residual1XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual1XPerIteration->Draw("AP");
	Residual1YPerIteration->SetMarkerStyle(24);
	Residual1YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual1YPerIteration->Draw("Psame");
	Residual2XPerIteration->SetMarkerStyle(21);
	Residual2XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual2XPerIteration->Draw("Psame");
	Residual2YPerIteration->SetMarkerStyle(25);
	Residual2YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual2YPerIteration->Draw("Psame");
	Residual3XPerIteration->SetMarkerStyle(22);	  
	Residual3XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual3XPerIteration->Draw("Psame");
	Residual3YPerIteration->SetMarkerStyle(26);
	Residual3YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual3YPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(23);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual4XPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(27);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual4YPerIteration->Draw("Psame");

	auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
	legend->AddEntry(Residual1XPerIteration, "Tracker 1X", "P");
	legend->AddEntry(Residual1YPerIteration, "Tracker 1Y", "P");
	legend->AddEntry(Residual2XPerIteration, "Tracker 2X", "P");
	legend->AddEntry(Residual2YPerIteration, "Tracker 2Y", "P");
	legend->AddEntry(Residual3XPerIteration, "Tracker 3X", "P");
	legend->AddEntry(Residual3YPerIteration, "Tracker 3Y", "P");
	legend->AddEntry(Residual4XPerIteration, "Tracker 4X", "P");
	legend->AddEntry(Residual4YPerIteration, "Tracker 4Y", "P");

	legend->Draw();



	c1->Draw();
	
	TCanvas* ChiSquareData = new TCanvas("", "", 1000, 500);
	ChiSquareData->Divide(2,1);
	  
	ChiSquareData->cd(1);
	xChi2PerIteration->GetYaxis()->SetRangeUser(0, 0.6);
	xChi2PerIteration->Draw("A*");	 
	ChiSquareData->cd(2);
	yChi2PerIteration->GetYaxis()->SetRangeUser(0, 0.6);
	yChi2PerIteration->Draw("A*");

	ChiSquareData->SaveAs("Aligment1_Chisquared.png");

		//IEEE Conference figure
	TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1100, 500);
	TrackerRes->Divide(2,1);

	int rangevals = 1;
	TrackerRes->cd(1);
	h_residual_Tracker1x->SetTitle("Tracker1x residual");
	h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	//h_residual_Tracker1x->GetPad()->SetBorderSize(2);
	
	h_residual_Tracker1x->Draw();
	TrackerRes->cd(2);
	h_residual_Tracker1y->SetTitle("Tracker1y residual");
	h_residual_Tracker1y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker1y->Draw();
	/*
	TrackerRes->cd(2);
       
	h_residual_Tracker2x->SetTitle("Tracker2x residual");
	h_residual_Tracker2x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker2x->Draw();
	TrackerRes->cd(6);
	h_residual_Tracker2y->SetTitle("Tracker2y residual");
	h_residual_Tracker2y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker2y->Draw();
	TrackerRes->cd(3);
	h_residual_Tracker3x->SetTitle("Tracker3x residual");
	h_residual_Tracker3x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker3x->Draw();
	TrackerRes->cd(7);
	h_residual_Tracker3y->SetTitle("Tracker3y residual");
	h_residual_Tracker3y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker3y->Draw();
	TrackerRes->cd(4);
	h_residual_Tracker4x->SetTitle("Tracker4x residual");
	h_residual_Tracker4x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker4x->Draw();
	TrackerRes->cd(8);
	h_residual_Tracker4y->SetTitle("Tracker4y residual");
	h_residual_Tracker4y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker4y->Draw();
	*/
	gPad->Update();
	  
	TrackerRes->Draw();
	gPad->Update();
	

      }
      break;
    }
    
    if(Iterator >= 100){	  

      OptimizedXYOffsets.push_back(Total1Xshift); OptimizedXYOffsets.push_back(Total1Yshift);
      OptimizedXYOffsets.push_back(Total2Xshift); OptimizedXYOffsets.push_back(Total2Yshift);
      OptimizedXYOffsets.push_back(Total3Xshift); OptimizedXYOffsets.push_back(Total3Yshift);
      OptimizedXYOffsets.push_back(Total4Xshift); OptimizedXYOffsets.push_back(Total4Yshift);

      if(plots){
	/*
	TCanvas* TrackerHits = new TCanvas(name2X, "", 2000, 1000);
	TrackerHits->Divide(4,2);

	//int rangevals = 1;
	TrackerHits->cd(1);
	//h_residual_Tracker1x->SetTitle("Tracker1x residual");
	//h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g1xcl->Draw();
	TrackerHits->cd(5);
	//h_residual_Tracker1y->SetTitle("Tracker1y residual");
	//h_residual_Tracker1y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g1ycl->Draw();
	TrackerHits->cd(2);
	//h_residual_Tracker2x->SetTitle("Tracker2x residual");
	//h_residual_Tracker2x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g2xcl->Draw();
	TrackerHits->cd(6);
	//h_residual_Tracker2y->SetTitle("Tracker2y residual");
	//h_residual_Tracker2y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g2ycl->Draw();
	TrackerHits->cd(3);
	//h_residual_Tracker3x->SetTitle("Tracker3x residual");
	//h_residual_Tracker3x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g3xcl->Draw();
	TrackerHits->cd(7);
	//h_residual_Tracker3y->SetTitle("Tracker3y residual");
	//h_residual_Tracker3y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g3ycl->Draw();
	TrackerHits->cd(4);
	//h_residual_Tracker4x->SetTitle("Tracker4x residual");
	//h_residual_Tracker4x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g4xcl->Draw();
	TrackerHits->cd(8);
	//h_residual_Tracker4y->SetTitle("Tracker4y residual");
	//h_residual_Tracker4y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_Pos_g4ycl->Draw();
	  
	gPad->Update();
	  
	TrackerHits->Draw();
	gPad->Update();
	*/
	
	TCanvas* c1 = new TCanvas("c1","", 700,500);
	c1->cd(1);
	Residual1XPerIteration->SetTitle("Mean XY Residuals of Trackers 1-4");
	Residual1XPerIteration->SetMarkerStyle(20);
	Residual1XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual1XPerIteration->Draw("AP");
	Residual1YPerIteration->SetMarkerStyle(24);
	Residual1YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual1YPerIteration->Draw("Psame");
	Residual2XPerIteration->SetMarkerStyle(21);
	Residual2XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual2XPerIteration->Draw("Psame");
	Residual2YPerIteration->SetMarkerStyle(25);
	Residual2YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual2YPerIteration->Draw("Psame");
	Residual3XPerIteration->SetMarkerStyle(22);	  
	Residual3XPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual3XPerIteration->Draw("Psame");
	Residual3YPerIteration->SetMarkerStyle(26);
	Residual3YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual3YPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(23);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual4XPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(27);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-1, 1);
	Residual4YPerIteration->Draw("Psame");

	auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
	legend->AddEntry(Residual1XPerIteration, "Tracker 1X", "P");
	legend->AddEntry(Residual1YPerIteration, "Tracker 1Y", "P");
	legend->AddEntry(Residual2XPerIteration, "Tracker 2X", "P");
	legend->AddEntry(Residual2YPerIteration, "Tracker 2Y", "P");
	legend->AddEntry(Residual3XPerIteration, "Tracker 3X", "P");
	legend->AddEntry(Residual3YPerIteration, "Tracker 3Y", "P");
	legend->AddEntry(Residual4XPerIteration, "Tracker 4X", "P");
	legend->AddEntry(Residual4YPerIteration, "Tracker 4Y", "P");

	legend->Draw();



	c1->Draw();
	
	TCanvas* ChiSquareData = new TCanvas("", "", 1000, 500);
	ChiSquareData->Divide(2,1);
	  
	ChiSquareData->cd(1);
	xChi2PerIteration->GetYaxis()->SetRangeUser(0, 0.6);
	xChi2PerIteration->Draw("A*");	 
	ChiSquareData->cd(2);
	yChi2PerIteration->GetYaxis()->SetRangeUser(0, 0.6);
	yChi2PerIteration->Draw("A*");

	ChiSquareData->SaveAs("Aligment1_Chisquared.png");
	
	
	//IEEE Conference figure
	TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 2000, 1000);
	TrackerRes->Divide(4,2);

	int rangevals = 1;
	TrackerRes->cd(1);
	h_residual_Tracker1x->SetTitle("Tracker1x residual");
	h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker1x->Draw();
	TrackerRes->cd(5);
	h_residual_Tracker1y->SetTitle("Tracker1y residual");
	h_residual_Tracker1y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker1y->Draw();
	TrackerRes->cd(2);
	h_residual_Tracker2x->SetTitle("Tracker2x residual");
	h_residual_Tracker2x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker2x->Draw();
	TrackerRes->cd(6);
	h_residual_Tracker2y->SetTitle("Tracker2y residual");
	h_residual_Tracker2y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker2y->Draw();
	TrackerRes->cd(3);
	h_residual_Tracker3x->SetTitle("Tracker3x residual");
	h_residual_Tracker3x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker3x->Draw();
	TrackerRes->cd(7);
	h_residual_Tracker3y->SetTitle("Tracker3y residual");
	h_residual_Tracker3y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker3y->Draw();
	TrackerRes->cd(4);
	h_residual_Tracker4x->SetTitle("Tracker4x residual");
	h_residual_Tracker4x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker4x->Draw();
	TrackerRes->cd(8);
	h_residual_Tracker4y->SetTitle("Tracker4y residual");
	h_residual_Tracker4y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker4y->Draw();
	
	gPad->Update();
	  
	TrackerRes->Draw();
	gPad->Update();
      }            
      break;
    }

	    
    float Shift_scale_Factor = -0.1;
	
    Shift_T1x = Shift_scale_Factor*mean_Tracker1xcoordRes; Shift_T1y = Shift_scale_Factor*mean_Tracker1ycoordRes;
    Shift_T2x = Shift_scale_Factor*mean_Tracker2xcoordRes; Shift_T2y = Shift_scale_Factor*mean_Tracker2ycoordRes;
    Shift_T3x = Shift_scale_Factor*mean_Tracker3xcoordRes; Shift_T3y = Shift_scale_Factor*mean_Tracker3ycoordRes;
    Shift_T4x = Shift_scale_Factor*mean_Tracker4xcoordRes; Shift_T4y = Shift_scale_Factor*mean_Tracker4ycoordRes;

    Total1Xshift += Shift_T1x;
    Total1Yshift += Shift_T1y;
    Total2Xshift += Shift_T2x;
    Total2Yshift += Shift_T2y;
    Total3Xshift += Shift_T3x;
    Total3Yshift += Shift_T3y;
    Total4Xshift += Shift_T4x;
    Total4Yshift += Shift_T4y;
    

    /*
    Total1Xshift += mean_Tracker1xcoordRes;
    Total1Yshift += mean_Tracker1ycoordRes;
    Total2Xshift += mean_Tracker2xcoordRes;
    Total2Yshift += mean_Tracker2ycoordRes;
    Total3Xshift += mean_Tracker3xcoordRes;
    Total3Yshift += mean_Tracker3ycoordRes;
    Total4Xshift += mean_Tracker4xcoordRes;
    Total4Yshift += mean_Tracker4ycoordRes;
    */
    
    delete h_residual_Tracker1x;
    delete h_residual_Tracker1y;
    delete h_residual_Tracker2x;
    delete h_residual_Tracker2y;
    delete h_residual_Tracker3x;
    delete h_residual_Tracker3y;
    delete h_residual_Tracker4x;
    delete h_residual_Tracker4y;
  
  }
 
  return OptimizedXYOffsets;

}

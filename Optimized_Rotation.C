vector<float> Optimized_Rotation(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> OptimizedXYOffests, vector<float> BadEvents, bool plots){

  float Shift_T1x;
  float Shift_T1y;
  float Shift_T2x;
  float Shift_T2y;
  float Shift_T3x;
  float Shift_T3y;
  float Shift_T4x;
  float Shift_T4y;
  
  
  if(OptimizedXYOffests.empty()){
    Shift_T1x = 0;
    Shift_T1y = 0;
    Shift_T2x = 0;
    Shift_T2y = 0;
    Shift_T3x = 0;
    Shift_T3y = 0;
    Shift_T4x = 0;
    Shift_T4y = 0;
  }
  else {
    Shift_T1x = OptimizedXYOffests[0];
    Shift_T1y = OptimizedXYOffests[1];
    Shift_T2x = OptimizedXYOffests[2];
    Shift_T2y = OptimizedXYOffests[3];
    Shift_T3x = OptimizedXYOffests[4];
    Shift_T3y = OptimizedXYOffests[5];
    Shift_T4x = OptimizedXYOffests[6];
    Shift_T4y = OptimizedXYOffests[7];
    
  }
  
  cout << Shift_T1x << endl;
  cout << Shift_T1y << endl;
  cout << Shift_T2x << endl;
  cout << Shift_T2y << endl;
  cout << Shift_T3x << endl;
  cout << Shift_T3y << endl;
  cout << Shift_T4x << endl;
  cout << Shift_T4y << endl;



  double mean_Tracker1xcoordRes=0.0, mean_Tracker1ycoordRes=0.0;
  double mean_Tracker2xcoordRes=0.0, mean_Tracker2ycoordRes=0.0;
  double mean_Tracker3xcoordRes=0.0, mean_Tracker3ycoordRes=0.0;
  double mean_Tracker4xcoordRes=0.0, mean_Tracker4ycoordRes=0.0;  
  
  int Iterator=0;
  float angleT2=0.0;
  float angleT3=0.0;
  float angleT4=0.0;
  float MeanangleT2=0.0;
  float MeanangleT3=0.0;
  float MeanangleT4=0.0;
  float TotalangleT2=0.0;
  float TotalangleT3=0.0;
  float TotalangleT4=0.0;

  float Total1Xshift=0;
  float Total1Yshift=0;
  float Total2Xshift=0;
  float Total2Yshift=0;
  float Total3Xshift=0;
  float Total3Yshift=0;
  float Total4Xshift=0;
  float Total4Yshift=0;
  
  vector<float> Optimized_Angles_And_XY;

  
  int ResIt = 5;

  //if(plots){
    TGraph* Residual1XPerIteration = new TGraph();Residual1XPerIteration->GetXaxis()->SetTitle("Iterations");Residual1XPerIteration->GetYaxis()->SetTitle("Residual");Residual1XPerIteration->SetTitle("Residual Per Iteration in Tracker1 x coordinate");
    TGraph* Residual1YPerIteration = new TGraph();Residual1YPerIteration->GetXaxis()->SetTitle("Iterations");Residual1YPerIteration->GetYaxis()->SetTitle("Residual");Residual1YPerIteration->SetTitle("Residual Per Iteration in Tracker1 y coordinate");
    TGraph* Residual2XPerIteration = new TGraph();Residual2XPerIteration->GetXaxis()->SetTitle("Iterations");Residual2XPerIteration->GetYaxis()->SetTitle("Residual");Residual2XPerIteration->SetTitle("Residual Per Iteration in Tracker2 x coordinate");
    TGraph* Residual2YPerIteration = new TGraph();Residual2YPerIteration->GetXaxis()->SetTitle("Iterations");Residual2YPerIteration->GetYaxis()->SetTitle("Residual");Residual2YPerIteration->SetTitle("Residual Per Iteration in Tracker2 y coordinate");
    TGraph* Residual3XPerIteration = new TGraph();Residual3XPerIteration->GetXaxis()->SetTitle("Iterations");Residual3XPerIteration->GetYaxis()->SetTitle("Residual");Residual3XPerIteration->SetTitle("Residual Per Iteration in Tracker3 x coordinate");
    TGraph* Residual3YPerIteration = new TGraph();Residual3YPerIteration->GetXaxis()->SetTitle("Iterations");Residual3YPerIteration->GetYaxis()->SetTitle("Residual");Residual3YPerIteration->SetTitle("Residual Per Iteration in Tracker3 y coordinate");
    TGraph* Residual4XPerIteration = new TGraph();Residual4XPerIteration->GetXaxis()->SetTitle("Iterations");Residual4XPerIteration->GetYaxis()->SetTitle("Residual");Residual4XPerIteration->SetTitle("Residual Per Iteration in Tracker4 x coordinate");
    TGraph* Residual4YPerIteration = new TGraph();Residual4YPerIteration->GetXaxis()->SetTitle("Iterations");Residual4YPerIteration->GetYaxis()->SetTitle("Residual");Residual4YPerIteration->SetTitle("Residual Per Iteration in Tracker4 y coordinate");

    TGraph* AngleT2PerIteration = new TGraph();AngleT2PerIteration->GetXaxis()->SetTitle("Iterations");AngleT2PerIteration->GetYaxis()->SetTitle("Angle (Rad)");AngleT2PerIteration->SetTitle("Angle rotation Per Iteration in Tracker 2");
    TGraph* AngleT3PerIteration = new TGraph();AngleT3PerIteration->GetXaxis()->SetTitle("Iterations");AngleT3PerIteration->GetYaxis()->SetTitle("Angle (Rad)");AngleT3PerIteration->SetTitle("Angle rotation Per Iteration in Tracker 3");
    TGraph* AngleT4PerIteration = new TGraph();AngleT4PerIteration->GetXaxis()->SetTitle("Iterations");AngleT4PerIteration->GetYaxis()->SetTitle("Angle (Rad)");AngleT4PerIteration->SetTitle("Angle rotation Per Iteration in Tracker 4");

    TGraph* xChi2PerIteration = new TGraph();xChi2PerIteration->GetXaxis()->SetTitle("Iteration");xChi2PerIteration->GetYaxis()->SetTitle("#chi^2");xChi2PerIteration->SetTitle("#chi^2 of track in X-Z projection plane");//xChi2PerIteration
    TGraph* yChi2PerIteration = new TGraph();yChi2PerIteration->GetXaxis()->SetTitle("Iteration");yChi2PerIteration->GetYaxis()->SetTitle("#chi^2");yChi2PerIteration->SetTitle("#chi^2 of track in Y-Z projection plane");
    //}

    float prealignedX =  Tracker1HitlocVec[0];
    float prealignedY =  Tracker1HitlocVec[1];   
    while(1){

      Iterator++;
   

    
      char nameRes2X[30];sprintf(nameRes2X,"residual_Tracker2x_%i",Iterator);char nameRes2Y[30];sprintf(nameRes2Y,"residual_Tracker2y_%i",Iterator);
      char nameRes3X[30];sprintf(nameRes3X,"residual_Tracker3x_%i",Iterator);char nameRes3Y[30];sprintf(nameRes3Y,"residual_Tracker3y_%i",Iterator);
      char nameRes1X[30];sprintf(nameRes1X,"residual_Tracker1x_%i",Iterator);char nameRes1Y[30];sprintf(nameRes1Y,"residual_Tracker1y_%i",Iterator);
      char nameRes4X[30];sprintf(nameRes4X,"residual_Tracker4x_%i",Iterator);char nameRes4Y[30];sprintf(nameRes4Y,"residual_Tracker4y_%i",Iterator);
	
      TH1F* h_residual_Tracker2x = new TH1F(nameRes2X,"",100,-2,2); h_residual_Tracker2x->SetXTitle("Residual [mm]"); h_residual_Tracker2x->SetYTitle("Frequency");h_residual_Tracker2x->SetLabelSize(0.042,"XY");h_residual_Tracker2x->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker2y = new TH1F(nameRes2Y,"",100,-2,2); h_residual_Tracker2y->SetXTitle("Residual [mm]"); h_residual_Tracker2y->SetYTitle("Frequency");h_residual_Tracker2y->SetLabelSize(0.042,"XY");h_residual_Tracker2y->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker3x = new TH1F(nameRes3X,"",100,-2,2); h_residual_Tracker3x->SetXTitle("Residual [mm]"); h_residual_Tracker3x->SetYTitle("Frequency");h_residual_Tracker3x->SetLabelSize(0.042,"XY");h_residual_Tracker3x->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker3y = new TH1F(nameRes3Y,"",100,-2,2); h_residual_Tracker3y->SetXTitle("Residual [mm]"); h_residual_Tracker3y->SetYTitle("Frequency");h_residual_Tracker3y->SetLabelSize(0.042,"XY");h_residual_Tracker3y->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker1x = new TH1F(nameRes1X,"",100,-2,2); h_residual_Tracker1x->SetXTitle("Residual [mm]"); h_residual_Tracker1x->SetYTitle("Frequency");h_residual_Tracker1x->SetLabelSize(0.042,"XY");h_residual_Tracker1x->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker1y = new TH1F(nameRes1Y,"",100,-2,2); h_residual_Tracker1y->SetXTitle("Residual [mm]"); h_residual_Tracker1y->SetYTitle("Frequency");h_residual_Tracker1y->SetLabelSize(0.042,"XY");h_residual_Tracker1y->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker4x = new TH1F(nameRes4X,"",100,-2,2); h_residual_Tracker4x->SetXTitle("Residual [mm]"); h_residual_Tracker4x->SetYTitle("Frequency");h_residual_Tracker4x->SetLabelSize(0.042,"XY");h_residual_Tracker4x->SetTitleSize(0.042,"XY");
      TH1F* h_residual_Tracker4y = new TH1F(nameRes4Y,"",100,-2,2); h_residual_Tracker4y->SetXTitle("Residual [mm]"); h_residual_Tracker4y->SetYTitle("Frequency");h_residual_Tracker4y->SetLabelSize(0.042,"XY");h_residual_Tracker4y->SetTitleSize(0.042,"XY");

      TH1F* angleREF2 = new TH1F("angleREF2","Rotation angle distribution btwn REF1 and REF2",100,-0.5,0.5); angleREF2->SetXTitle("Angle [radian]"); angleREF2->SetYTitle("Counts/1uRad");
      TH1F* angleREF3 = new TH1F("angleREF3","Rotation angle distribution btwn REF1 and REF3",100,-0.5,0.5); angleREF3->SetXTitle("Angle [radian]"); angleREF3->SetYTitle("Counts/1uRad");
      TH1F* angleREF4 = new TH1F("angleREF3","Rotation angle distribution btwn REF1 and REF4",100,-0.5,0.5); angleREF4->SetXTitle("Angle [radian]"); angleREF4->SetYTitle("Counts/1uRad");

    

      //cout << "B" << endl;
      //first, shift all hits coordinats to optimal XY offsets

      float Tracker1HitlocVecx_tmp=0;
      float Tracker1HitlocVecy_tmp=0;
      float Tracker2HitlocVecx_tmp=0;
      float Tracker2HitlocVecy_tmp=0;
      float Tracker3HitlocVecx_tmp=0;
      float Tracker3HitlocVecy_tmp=0;
      float Tracker4HitlocVecx_tmp=0;
      float Tracker4HitlocVecy_tmp=0;   
   
      for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){//shift all of the XY hit coordinates
	  
	Tracker1HitlocVec[i] = Tracker1HitlocVec[i] - Shift_T1x;
	//Tracker1HitlocVecx_tmp = Tracker1HitlocVec[i];
	Tracker2HitlocVec[i] = Tracker2HitlocVec[i] - Shift_T2x;
	//Tracker2HitlocVecx_tmp = Tracker2HitlocVec[i];
	Tracker3HitlocVec[i] = Tracker3HitlocVec[i] - Shift_T3x;
	//Tracker3HitlocVecx_tmp = Tracker3HitlocVec[i];
	Tracker4HitlocVec[i] = Tracker4HitlocVec[i] - Shift_T4x;
	//Tracker4HitlocVecx_tmp = Tracker4HitlocVec[i];

      
	i++;//move to y components
	  
	Tracker1HitlocVec[i] = Tracker1HitlocVec[i] - Shift_T1y;
	//Tracker1HitlocVecy_tmp = Tracker1HitlocVec[i];
	Tracker2HitlocVec[i] = Tracker2HitlocVec[i] - Shift_T2y;
	//Tracker2HitlocVecy_tmp = Tracker2HitlocVec[i];
	Tracker3HitlocVec[i] = Tracker3HitlocVec[i] - Shift_T3y;
	//Tracker3HitlocVecy_tmp = Tracker3HitlocVec[i];
	Tracker4HitlocVec[i] = Tracker4HitlocVec[i] - Shift_T4y;
	//Tracker4HitlocVecy_tmp = Tracker4HitlocVec[i];

      
	i++;
      }

      vector<float> ShiftedTracker1HitlocVec;

      ShiftedTracker1HitlocVec.insert(ShiftedTracker1HitlocVec.begin(), Tracker1HitlocVec.begin(), Tracker1HitlocVec.end());

      vector<float> ShiftedTracker2HitlocVec;

      ShiftedTracker2HitlocVec.insert(ShiftedTracker2HitlocVec.begin(), Tracker2HitlocVec.begin(), Tracker2HitlocVec.end());

      vector<float> ShiftedTracker3HitlocVec;

      ShiftedTracker3HitlocVec.insert(ShiftedTracker3HitlocVec.begin(), Tracker3HitlocVec.begin(), Tracker3HitlocVec.end());

      vector<float> ShiftedTracker4HitlocVec;

      ShiftedTracker4HitlocVec.insert(ShiftedTracker4HitlocVec.begin(), Tracker4HitlocVec.begin(), Tracker4HitlocVec.end());
  
    

    
      //Now rotate

      for(unsigned int i=0; i< ShiftedTracker1HitlocVec.size(); i++){	  
	  
	//h_Pos_Tracker1xCoord->Fill(Tracker1HitlocVec[i]);
	Tracker2HitlocVecx_tmp = ShiftedTracker2HitlocVec[i];  
	Tracker2HitlocVec[i] = ShiftedTracker2HitlocVec[i]*cos(angleT2) - ShiftedTracker2HitlocVec[i+1]*sin(angleT2);	  
	//h_Pos_Tracker2xCoord->Fill(Tracker2HitlocVec[i]);

	Tracker3HitlocVecx_tmp = ShiftedTracker3HitlocVec[i];
	Tracker3HitlocVec[i] = ShiftedTracker3HitlocVec[i]*cos(angleT3) - ShiftedTracker3HitlocVec[i+1]*sin(angleT3);	  
	//h_Pos_Tracker3xCoord->Fill(Tracker3HitlocVec[i]);
    
	Tracker4HitlocVecx_tmp = ShiftedTracker4HitlocVec[i];    
	Tracker4HitlocVec[i] = ShiftedTracker4HitlocVec[i]*cos(angleT4) - ShiftedTracker4HitlocVec[i+1]*sin(angleT4);	  
	//h_Pos_Tracker4xCoord->Fill(Tracker4HitlocVec[i]);

	i++;//move to y components
	  
	//h_Pos_Tracker1yCoord->Fill(Tracker1HitlocVec[i]);

	Tracker2HitlocVec[i] = Tracker2HitlocVecx_tmp*sin(angleT2) + ShiftedTracker2HitlocVec[i]*cos(angleT2);	  
	//h_Pos_Tracker2xCoord->Fill(Tracker2HitlocVec[i]);

	Tracker3HitlocVec[i] = Tracker3HitlocVecx_tmp*sin(angleT3) + ShiftedTracker3HitlocVec[i]*cos(angleT3);	  
	//h_Pos_Tracker3xCoord->Fill(Tracker3HitlocVec[i]);

	Tracker4HitlocVec[i] = Tracker4HitlocVecx_tmp*sin(angleT4) + ShiftedTracker4HitlocVec[i]*cos(angleT4);	  
	//h_Pos_Tracker4xCoord->Fill(Tracker4HitlocVec[i]);

  
	double cosineREF2;
	double cosineREF3;
	double cosineREF4;
	angleREF2->Fill(cosineREF2 = CalculateCosTheta1(Tracker1HitlocVec[i-1],Tracker1HitlocVec[i],Tracker2HitlocVec[i-1],Tracker2HitlocVec[i]));
	angleREF3->Fill(cosineREF3 = CalculateCosTheta1(Tracker1HitlocVec[i-1],Tracker1HitlocVec[i],Tracker3HitlocVec[i-1],Tracker3HitlocVec[i]));
	angleREF4->Fill(cosineREF4 = CalculateCosTheta1(Tracker1HitlocVec[i-1],Tracker1HitlocVec[i],Tracker4HitlocVec[i-1],Tracker4HitlocVec[i]));
      
	
	i++;
	  
      }

	
      auto NewRes = Residuals(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec, BadEvents);
      auto Chisquares = ChiSquared(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec, BadEvents);

	
	
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
      /*
	TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1000, 500);
	TrackerRes->Divide(2,1);
    
	TrackerRes->cd(1);
	h_residual_Tracker2x->Draw();
	TrackerRes->cd(2);
	h_residual_Tracker2y->Draw();  

	TrackerRes->Draw();
	TrackerRes->SaveAs("Tracker2Residual_Alignment2.png");
      */
    
      I2GFvalues myValues;

	
      myValues = I2GFmainLoop(h_residual_Tracker1x,1,4,1);                           //converges!	  
      mean_Tracker1xcoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
      //mean_Tracker1xcoordRes = h_residual_Tracker1x->GetMean();
      //cout << "Mean Residual in x coordinate: " << mean_Tracker1xcoordRes << endl;
	
      myValues = I2GFmainLoop(h_residual_Tracker1y,1,4,1);	                            //breaks :(
      mean_Tracker1ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
      //mean_Tracker1ycoordRes = h_residual_Tracker1y->GetMean(); //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in y coordinate: " << mean_Tracker1ycoordRes << endl;
      //cout << "E" << endl;
	
      myValues = I2GFmainLoop(h_residual_Tracker2x,1,4,1);
      mean_Tracker2xcoordRes = myValues.mean;//converges!	  
      //mean_Tracker2xcoordRes = h_residual_Tracker2x->GetMean(); //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in x coordinate: " << mean_Tracker2xcoordRes << endl;

      myValues = I2GFmainLoop(h_residual_Tracker2y,1,4,1);	                            //breaks :(... im sensing a pattern...
      mean_Tracker2ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in y coordinate: " << mean_Tracker2ycoordRes << endl;
      myValues = I2GFmainLoop(h_residual_Tracker3x,1,4,1);
      mean_Tracker3xcoordRes = myValues.mean;
      //mean_Tracker3xcoordRes =h_residual_Tracker3x->GetMean(); //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in x coordinate: " << mean_Tracker3xcoordRes << endl;
      myValues = I2GFmainLoop(h_residual_Tracker3y,1,4,1);	  
      mean_Tracker3ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in y coordinate: " << mean_Tracker3ycoordRes << endl;
      myValues = I2GFmainLoop(h_residual_Tracker4x,1,4,1);	  
      mean_Tracker4xcoordRes = myValues.mean;
      //mean_Tracker4xcoordRes = h_residual_Tracker4x->GetMean(); //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in x coordinate: " << mean_Tracker4xcoordRes << endl;
      myValues = I2GFmainLoop(h_residual_Tracker4y,1,4,1);	  
      mean_Tracker4ycoordRes = myValues.mean; //sigmaEta5=myValues.sigma;
      //cout << "Mean Residual in y coordinate: " << mean_Tracker4ycoordRes << endl;

    
      myValues = I2GFmainLoop(angleREF2,1,5,1);	  
      MeanangleT2 = myValues.mean;    
      AngleT2PerIteration->SetPoint(Iterator-1, Iterator, MeanangleT2);
	  
      myValues = I2GFmainLoop(angleREF3,1,5,1);	  
      MeanangleT3 = myValues.mean;    
      AngleT3PerIteration->SetPoint(Iterator-1, Iterator, MeanangleT3);
	
      myValues = I2GFmainLoop(angleREF4,1,5,1);	  
      MeanangleT4 = myValues.mean;
      AngleT4PerIteration->SetPoint(Iterator-1, Iterator, MeanangleT4);	

    
    
      cout << "mean angle2: " << MeanangleT2 << endl;
      cout << "mean angle3: " << MeanangleT3 << endl;
      cout << "mean angle4: " << MeanangleT4 << endl;

      cout << "mean angle2 from ->GetMean(): " << angleREF2->GetMean() << endl;
      cout << "mean angle3 from ->GetMean(): " << angleREF3->GetMean() << endl;
      cout << "mean angle4 from ->GetMean(): " << angleREF4->GetMean() << endl;
      cout << "Iterator: " << Iterator << endl;

      xChi2PerIteration->SetPoint(Iterator-1, Iterator, Chisquares[0]);
      yChi2PerIteration->SetPoint(Iterator-1, Iterator, Chisquares[1]);
    
      /*
	cout << "Mean Residual in 1x coordinate: " << mean_Tracker1xcoordRes << endl;
	cout << "Was Shifted by: " << Shift_T1x << endl;
	cout << "Mean Residual in 1y coordinate: " << mean_Tracker1ycoordRes << endl;
	cout << "Was Shifted by: " << Shift_T1y << endl;
	cout << "Mean Residual in 2x coordinate: " << mean_Tracker2xcoordRes << endl;
	cout << "Was Shifted by: " << Shift_T2x << endl;
	cout << "Mean Residual in 2y coordinate: " << mean_Tracker2ycoordRes << endl;
	cout << "Was Shifted by: " << Shift_T2y << endl;
	cout << "Mean Residual in 3x coordinate: " << mean_Tracker3xcoordRes << endl;
	cout << "Was Shifted by: " << Shift_T3x << endl;
	cout << "Mean Residual in 3y coordinate: " << mean_Tracker3ycoordRes << endl;
	cout << "Was Shifted by: " << Shift_T3y << endl;
	cout << "Mean Residual in 4x coordinate: " << mean_Tracker4xcoordRes << endl;
	cout << "Was Shifted by: " << Shift_T4x << endl;
	cout << "Mean Residual in 4y coordinate: " << mean_Tracker4ycoordRes << endl;
	cout << "Was Shifted by: " << Shift_T4y << endl;
      */
    
      if((MeanangleT2 < -2 || MeanangleT2 > 2) || (MeanangleT3 > 2 || MeanangleT3 < -2) || (MeanangleT4 > 2 || MeanangleT4 < -2)){

	cout << "MeanangleT2: " << MeanangleT2 << ", MeanangleT3: " << MeanangleT3 << ", MeanangleT4: " << MeanangleT4 << endl;
	TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1000, 1000);
	TrackerRes->Divide(2,2);
      
	TrackerRes->cd(1);
	angleREF2->Draw();
	TrackerRes->cd(2);
	angleREF3->Draw();
	TrackerRes->cd(3);
	angleREF4->Draw();
      
	return Optimized_Angles_And_XY;

      }
	
   	
	
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
    
    
	
      if((mean_Tracker1xcoordRes >= -0.01 && mean_Tracker1xcoordRes <= 0.01) && (mean_Tracker1ycoordRes >= -0.01 && mean_Tracker1ycoordRes <= 0.01) && (mean_Tracker2xcoordRes >= -0.01 && mean_Tracker2xcoordRes <= 0.01) && (mean_Tracker2ycoordRes >= -0.01 && mean_Tracker2ycoordRes <= 0.01) && (mean_Tracker3xcoordRes >= -0.01 && mean_Tracker3xcoordRes <= 0.01) && (mean_Tracker3ycoordRes >= -0.01 && mean_Tracker3ycoordRes <= 0.01) && (mean_Tracker4xcoordRes >= -0.01 && mean_Tracker4xcoordRes <= 0.01) && (mean_Tracker4ycoordRes >= -0.01 && mean_Tracker4ycoordRes <= 0.01) && (MeanangleT2>= -0.01 && MeanangleT2 <= 0.01) && (MeanangleT3>= -0.01 && MeanangleT3 <= 0.01) && (MeanangleT4>= -0.01 && MeanangleT4 <= 0.01)){


	Optimized_Angles_And_XY.push_back(Total1Xshift + OptimizedXYOffests[0]); Optimized_Angles_And_XY.push_back(Total1Yshift + OptimizedXYOffests[1]);
	Optimized_Angles_And_XY.push_back(Total2Xshift + OptimizedXYOffests[2]); Optimized_Angles_And_XY.push_back(Total2Yshift + OptimizedXYOffests[3]);
	Optimized_Angles_And_XY.push_back(Total3Xshift + OptimizedXYOffests[4]); Optimized_Angles_And_XY.push_back(Total3Yshift + OptimizedXYOffests[5]);
	Optimized_Angles_And_XY.push_back(Total4Xshift + OptimizedXYOffests[6]); Optimized_Angles_And_XY.push_back(Total4Yshift + OptimizedXYOffests[7]);
      
	Optimized_Angles_And_XY.push_back(TotalangleT2);
	Optimized_Angles_And_XY.push_back(TotalangleT3);
	Optimized_Angles_And_XY.push_back(TotalangleT4);


      
	cout<<"find it...iterating "<< Iterator <<" times."<<endl;
      
	if(plots){
	
	  TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1000, 1000);
	  TrackerRes->Divide(2,2);

	  TrackerRes->cd(1);
	  angleREF2->Draw();
	  TrackerRes->cd(2);
	  angleREF3->Draw();
	  TrackerRes->cd(3);
	  angleREF4->Draw();
      
	  TrackerRes->Draw();
      
	  TCanvas* c1 = new TCanvas("c1","", 700,500);
	  c1->cd(1);
	  Residual1XPerIteration->SetTitle("Mean XY Residuals of Trackers 1-4");
	  Residual1XPerIteration->SetMarkerStyle(20);
	  Residual1XPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual1XPerIteration->Draw("AP");
	  Residual1YPerIteration->SetMarkerStyle(24);
	  Residual1YPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual1YPerIteration->Draw("Psame");
	  Residual2XPerIteration->SetMarkerStyle(21);
	  Residual2XPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual2XPerIteration->Draw("Psame");
	  Residual2YPerIteration->SetMarkerStyle(25);
	  Residual2YPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual2YPerIteration->Draw("Psame");
	  Residual3XPerIteration->SetMarkerStyle(22);
	  Residual3XPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual3XPerIteration->Draw("Psame");
	  Residual3YPerIteration->SetMarkerStyle(26);
	  Residual3YPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual3YPerIteration->Draw("Psame");
	  Residual4YPerIteration->SetMarkerStyle(23);
	  Residual4YPerIteration->GetYaxis()->SetRangeUser(-4, 4);
	  Residual4XPerIteration->Draw("Psame");
	  Residual4YPerIteration->SetMarkerStyle(27);
	  Residual4YPerIteration->GetYaxis()->SetRangeUser(-4, 4);
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
	
	}
      
	break;
      }

    
      if(Iterator >= 10){	  
      
	Optimized_Angles_And_XY.push_back(Total1Xshift + OptimizedXYOffests[0]); Optimized_Angles_And_XY.push_back(Total1Yshift + OptimizedXYOffests[1]);
	Optimized_Angles_And_XY.push_back(Total2Xshift + OptimizedXYOffests[2]); Optimized_Angles_And_XY.push_back(Total2Yshift + OptimizedXYOffests[3]);
	Optimized_Angles_And_XY.push_back(Total3Xshift + OptimizedXYOffests[4]); Optimized_Angles_And_XY.push_back(Total3Yshift + OptimizedXYOffests[5]);
	Optimized_Angles_And_XY.push_back(Total4Xshift + OptimizedXYOffests[6]); Optimized_Angles_And_XY.push_back(Total4Yshift + OptimizedXYOffests[7]);
      
	Optimized_Angles_And_XY.push_back(TotalangleT2);
	Optimized_Angles_And_XY.push_back(TotalangleT3);
	Optimized_Angles_And_XY.push_back(TotalangleT4);


	if(plots){
	  //Tracker plot for IEEE abstract

	  /*
	    TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 1000, 1000);
	    TrackerRes->Divide(2,2);

	    TrackerRes->cd(1);
	    angleREF2->Draw();
	    TrackerRes->cd(2);
	    angleREF3->Draw();
	    TrackerRes->cd(3);
	    angleREF4->Draw();
      
	    TrackerRes->Draw();
	  */

	  TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 2000, 1000);
	  TrackerRes->Divide(4,2);

	  TrackerRes->cd(1);
	  h_residual_Tracker1x->Draw();
	  TrackerRes->cd(5);
	  h_residual_Tracker1y->Draw();
	  TrackerRes->cd(2);
	  h_residual_Tracker2x->Draw();
	  TrackerRes->cd(6);
	  h_residual_Tracker2y->Draw();
	  TrackerRes->cd(3);
	  h_residual_Tracker3x->Draw();
	  TrackerRes->cd(7);
	  h_residual_Tracker3y->Draw();
	  TrackerRes->cd(4);
	  h_residual_Tracker4x->Draw();
	  TrackerRes->cd(8);
	  h_residual_Tracker4y->Draw();
	  

	  TrackerRes->Draw();

	
	  TCanvas* AnglesCanvas = new TCanvas("", "", 1500, 500);
	  AnglesCanvas->Divide(3,1);
	  AnglesCanvas->cd(1);
	  AngleT2PerIteration->Draw("A*");
	  AnglesCanvas->cd(2);
	  AngleT3PerIteration->Draw("A*");
	  AnglesCanvas->cd(3);
	  AngleT4PerIteration->Draw("A*");

	  AnglesCanvas->Draw();
      

	  TCanvas* c1 = new TCanvas("c1","", 700,500);
	  c1->cd(1);
	  Residual1XPerIteration->SetTitle("Mean XY Residuals of Trackers 1-4");
	  Residual1XPerIteration->SetMarkerStyle(20);
	  Residual1XPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual1XPerIteration->Draw("AP");
	  Residual1YPerIteration->SetMarkerStyle(24);
	  Residual1YPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual1YPerIteration->Draw("Psame");
	  Residual2XPerIteration->SetMarkerStyle(21);
	  Residual2XPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual2XPerIteration->Draw("Psame");
	  Residual2YPerIteration->SetMarkerStyle(25);
	  Residual2YPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual2YPerIteration->Draw("Psame");
	  Residual3XPerIteration->SetMarkerStyle(22);
	  Residual3XPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual3XPerIteration->Draw("Psame");
	  Residual3YPerIteration->SetMarkerStyle(26);
	  Residual3YPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual3YPerIteration->Draw("Psame");
	  Residual4YPerIteration->SetMarkerStyle(23);
	  Residual4YPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
	  Residual4XPerIteration->Draw("Psame");
	  Residual4YPerIteration->SetMarkerStyle(27);
	  Residual4YPerIteration->GetYaxis()->SetRangeUser(-0.03, 0.03);
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
	  xChi2PerIteration->Draw("A*");	 
	  ChiSquareData->cd(2);
	  yChi2PerIteration->Draw("A*");

	  ChiSquareData->SaveAs("Alignment2_Chisquared.png");
       
	  return Optimized_Angles_And_XY;
	}
      
	break;
      }

      double scale_Factor = -0.01;	
      //XY shift values stay constant

      Shift_T1x = scale_Factor*mean_Tracker1xcoordRes; Shift_T1y = scale_Factor*mean_Tracker1ycoordRes;
      Shift_T2x = scale_Factor*mean_Tracker2xcoordRes; Shift_T2y = scale_Factor*mean_Tracker2ycoordRes;
      Shift_T3x = scale_Factor*mean_Tracker3xcoordRes; Shift_T3y = scale_Factor*mean_Tracker3ycoordRes;
      Shift_T4x = scale_Factor*mean_Tracker4xcoordRes; Shift_T4y = scale_Factor*mean_Tracker4ycoordRes;
	
      double anglescale_Factor = -0.001;	
      angleT2 = anglescale_Factor*MeanangleT2;
      angleT3 = anglescale_Factor*MeanangleT3;
      angleT4 = anglescale_Factor*MeanangleT4;

      TotalangleT2 += angleT2;
      TotalangleT3 += angleT3;
      TotalangleT4 += angleT4;

      Total1Xshift += Shift_T1x;
      Total1Yshift += Shift_T1y;
      Total2Xshift += Shift_T2x;
      Total2Yshift += Shift_T2y;
      Total3Xshift += Shift_T3x;
      Total3Yshift += Shift_T3y;
      Total4Xshift += Shift_T4x;
      Total4Yshift += Shift_T4y;
	
    }
  
    return Optimized_Angles_And_XY;

}

vector<float> Optimized_XY_Rotation(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> OptimizedXYOffests, vector<float> BadEvents, bool plots){

  
  float Shift_T1x = OptimizedXYOffests[0];
  float Shift_T1y = OptimizedXYOffests[1];
  float Shift_T2x = OptimizedXYOffests[2];
  float Shift_T2y = OptimizedXYOffests[3];
  float Shift_T3x = OptimizedXYOffests[4];
  float Shift_T3y = OptimizedXYOffests[5];
  float Shift_T4x = OptimizedXYOffests[6];
  float Shift_T4y = OptimizedXYOffests[7];
  float angleT2 = OptimizedXYOffests[8];
  float angleT3 = OptimizedXYOffests[9];
  float angleT4 = OptimizedXYOffests[10];
  


  double mean_Tracker1xcoordRes=0.0, mean_Tracker1ycoordRes=0.0;
  double mean_Tracker2xcoordRes=0.0, mean_Tracker2ycoordRes=0.0;
  double mean_Tracker3xcoordRes=0.0, mean_Tracker3ycoordRes=0.0;
  double mean_Tracker4xcoordRes=0.0, mean_Tracker4ycoordRes=0.0;  
  
  int Iterator=0;
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
  
  
 

  char nameRes2X[30];sprintf(nameRes2X,"residual_Tracker2x_%i",Iterator);char nameRes2Y[30];sprintf(nameRes2Y,"residual_Tracker2y_%i",Iterator);
  char nameRes3X[30];sprintf(nameRes3X,"residual_Tracker3x_%i",Iterator);char nameRes3Y[30];sprintf(nameRes3Y,"residual_Tracker3y_%i",Iterator);
  char nameRes1X[30];sprintf(nameRes1X,"residual_Tracker1x_%i",Iterator);char nameRes1Y[30];sprintf(nameRes1Y,"residual_Tracker1y_%i",Iterator);
  char nameRes4X[30];sprintf(nameRes4X,"residual_Tracker4x_%i",Iterator);char nameRes4Y[30];sprintf(nameRes4Y,"residual_Tracker4y_%i",Iterator);
	
  TH1F* h_residual_Tracker2x = new TH1F(nameRes2X,"",100,-20,20); h_residual_Tracker2x->SetXTitle("Residual [mm]"); h_residual_Tracker2x->SetYTitle("Frequency");h_residual_Tracker2x->SetLabelSize(0.045,"XY");h_residual_Tracker2x->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker2y = new TH1F(nameRes2Y,"",100,-20,20); h_residual_Tracker2y->SetXTitle("Residual [mm]"); h_residual_Tracker2y->SetYTitle("Frequency");h_residual_Tracker2y->SetLabelSize(0.045,"XY");h_residual_Tracker2y->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker3x = new TH1F(nameRes3X,"",100,-20,20); h_residual_Tracker3x->SetXTitle("Residual [mm]"); h_residual_Tracker3x->SetYTitle("Frequency");h_residual_Tracker3x->SetLabelSize(0.045,"XY");h_residual_Tracker3x->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker3y = new TH1F(nameRes3Y,"",100,-20,20); h_residual_Tracker3y->SetXTitle("Residual [mm]"); h_residual_Tracker3y->SetYTitle("Frequency");h_residual_Tracker3y->SetLabelSize(0.045,"XY");h_residual_Tracker3y->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker1x = new TH1F(nameRes1X,"",100,-20,20); h_residual_Tracker1x->SetXTitle("Residual [mm]"); h_residual_Tracker1x->SetYTitle("Frequency");h_residual_Tracker1x->SetLabelSize(0.045,"XY");h_residual_Tracker1x->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker1y = new TH1F(nameRes1Y,"",100,-20,20); h_residual_Tracker1y->SetXTitle("Residual [mm]"); h_residual_Tracker1y->SetYTitle("Frequency");h_residual_Tracker1y->SetLabelSize(0.045,"XY");h_residual_Tracker1y->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker4x = new TH1F(nameRes4X,"",100,-20,20); h_residual_Tracker4x->SetXTitle("Residual [mm]"); h_residual_Tracker4x->SetYTitle("Frequency");h_residual_Tracker4x->SetLabelSize(0.045,"XY");h_residual_Tracker4x->SetTitleSize(0.045,"XY");
  TH1F* h_residual_Tracker4y = new TH1F(nameRes4Y,"",100,-20,20); h_residual_Tracker4y->SetXTitle("Residual [mm]"); h_residual_Tracker4y->SetYTitle("Frequency");h_residual_Tracker4y->SetLabelSize(0.045,"XY");h_residual_Tracker4y->SetTitleSize(0.045,"XY");

  TH1F* angleREF2 = new TH1F("angleREF2","Rotation angle distribution btwn REF1 and REF2",100,-3,3); angleREF2->SetXTitle("Angle [radian]"); angleREF2->SetYTitle("Counts/1uRad");
  TH1F* angleREF3 = new TH1F("angleREF3","Rotation angle distribution btwn REF1 and REF3",100,-3,3); angleREF3->SetXTitle("Angle [radian]"); angleREF3->SetYTitle("Counts/1uRad");
  TH1F* angleREF4 = new TH1F("angleREF3","Rotation angle distribution btwn REF1 and REF4",100,-3,3); angleREF4->SetXTitle("Angle [radian]"); angleREF4->SetYTitle("Counts/1uRad");


  map<int, TH2F*> Tracker1HitmapPerRotation;
  map<int, TH2F*> Tracker2HitmapPerRotation;
  map<int, TH2F*> Tracker3HitmapPerRotation;
  map<int, TH2F*> Tracker4HitmapPerRotation;
    
    

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

  //ResCut(ShiftedTracker1HitlocVec, ShiftedTracker2HitlocVec, ShiftedTracker3HitlocVec, ShiftedTracker4HitlocVec);

  TGraph* xChi2PerAngleT2 = new TGraph();xChi2PerAngleT2->GetXaxis()->SetTitle("Angle [Rad]");xChi2PerAngleT2->GetYaxis()->SetTitle("#chi^2");xChi2PerAngleT2->SetTitle("#chi^2 of Tracks in X-Z projection plane of Tracker 2");//xChi2PerAngleT2
  TGraph* yChi2PerAngleT2 = new TGraph();yChi2PerAngleT2->GetXaxis()->SetTitle("Angle [Rad]");yChi2PerAngleT2->GetYaxis()->SetTitle("#chi^2");yChi2PerAngleT2->SetTitle("#chi^2 of Tracks in Y-Z projection plane of Tracker 2");
  TGraph* xChi2PerAngleT3 = new TGraph();xChi2PerAngleT3->GetXaxis()->SetTitle("Angle [Rad]");xChi2PerAngleT3->GetYaxis()->SetTitle("#chi^2");xChi2PerAngleT3->SetTitle("#chi^2 of track in X-Z projection plane");//xChi2PerAngleT3
  TGraph* yChi2PerAngleT3 = new TGraph();yChi2PerAngleT3->GetXaxis()->SetTitle("AngleT3");yChi2PerAngleT3->GetYaxis()->SetTitle("#chi^2");yChi2PerAngleT3->SetTitle("#chi^2 of track in Y-Z projection plane");
  TGraph* xChi2PerAngleT4 = new TGraph();xChi2PerAngleT4->GetXaxis()->SetTitle("AngleT4");xChi2PerAngleT4->GetYaxis()->SetTitle("#chi^2");xChi2PerAngleT4->SetTitle("#chi^2 of track in X-Z projection plane");//xChi2PerAngleT4
  TGraph* yChi2PerAngleT4 = new TGraph();yChi2PerAngleT4->GetXaxis()->SetTitle("AngleT4");yChi2PerAngleT4->GetYaxis()->SetTitle("#chi^2");yChi2PerAngleT4->SetTitle("#chi^2 of track in Y-Z projection plane");

  
  //Now rotate each angle (3) around a small range to tune the angles (just roatating Tracker 2 for now)
  int bounds = 10;
  float anglescale_Factor = 0.0003;//300 urad
  float angleT2temp=0;
  float angleT3temp=0;
  float angleT4temp=0;

  float prealignedX =  Tracker1HitlocVec[0];
  float prealignedY =  Tracker1HitlocVec[1];   
  for(int AngleIt=-bounds;AngleIt <= bounds; AngleIt++){
      
    angleT2temp = angleT2 + anglescale_Factor*AngleIt;
    angleT3temp = angleT3 + anglescale_Factor*AngleIt;
    angleT4temp = angleT4 + anglescale_Factor*AngleIt;
      
    char nameRot1[30];sprintf(nameRot1,"Rotation_Tracker1_%i",AngleIt);
    char nameRot2[30];sprintf(nameRot2,"Rotation_Tracker2_%i",AngleIt);
    char nameRot3[30];sprintf(nameRot3,"Rotation_Tracker3_%i",AngleIt);
    char nameRot4[30];sprintf(nameRot4,"Rotation_Tracker4_%i",AngleIt);
    char nameRotCanvas[30];sprintf(nameRotCanvas,"Rotation_canvas_%f",angleT2temp);

    char titleRot1[30];sprintf(titleRot1,"Rotation_Tracker1_%i",AngleIt);
    char titleRot2[30];sprintf(titleRot2,"Rotation_Tracker2_%i",AngleIt);
    char titleRot3[30];sprintf(titleRot3,"Rotation_Tracker3_%i",AngleIt);
    char titleRot4[30];sprintf(titleRot4,"Rotation_Tracker4_%i",AngleIt);
    char titleRotCanvas[30];sprintf(titleRotCanvas,"Rotation_canvas_%f",angleT2temp);
    
    Tracker1HitmapPerRotation[AngleIt] = new TH2F(nameRot1, titleRot1, 100, -50, 50, 100, -50, 50);
    Tracker3HitmapPerRotation[AngleIt] = new TH2F(nameRot3, titleRot3, 100, -50, 50, 100, -50, 50);
    Tracker2HitmapPerRotation[AngleIt] = new TH2F(nameRot2, titleRot2, 100, -50, 50, 100, -50, 50);
    Tracker4HitmapPerRotation[AngleIt] = new TH2F(nameRot4, titleRot4, 100, -50, 50, 100, -50, 50);
    //      float Tracker2Rotatedxcoord=0; float Tracker2Rotatedycoord=0;

    
    //angleT2 += anglescale_Factor*AngleIt;
      
    //angleT3tmep =
    //AngleT2PerIteration->SetPoint(AngleIt+bounds, AngleIt, angleT2temp);
      
    cout << "Angle rotated to: " << angleT2temp << " at iterator: " << AngleIt << endl;
     
    for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){
      
      Tracker2HitlocVecx_tmp = ShiftedTracker2HitlocVec[i]; 	
      Tracker2HitlocVec[i] = ShiftedTracker2HitlocVec[i]*cos(angleT2temp) - ShiftedTracker2HitlocVec[i+1]*sin(angleT2temp);	  
      
      Tracker3HitlocVecx_tmp = ShiftedTracker3HitlocVec[i]; 	
      Tracker3HitlocVec[i] = ShiftedTracker3HitlocVec[i]*cos(angleT3temp) - ShiftedTracker3HitlocVec[i+1]*sin(angleT3temp);	  

      Tracker4HitlocVecx_tmp = ShiftedTracker4HitlocVec[i]; 	
      Tracker4HitlocVec[i] = ShiftedTracker4HitlocVec[i]*cos(angleT4temp) - ShiftedTracker4HitlocVec[i+1]*sin(angleT4temp);	  
      
      
      i++;//move to y components
	
      Tracker2HitlocVec[i] = Tracker2HitlocVecx_tmp*sin(angleT2temp) + ShiftedTracker2HitlocVec[i]*cos(angleT2temp);	  
      
      Tracker3HitlocVec[i] = Tracker3HitlocVecx_tmp*sin(angleT3temp) + ShiftedTracker3HitlocVec[i]*cos(angleT3temp);	  
      Tracker4HitlocVec[i] = Tracker4HitlocVecx_tmp*sin(angleT4temp) + ShiftedTracker4HitlocVec[i]*cos(angleT4temp);	  
 
      
      //Tracker2HitmapPerRotation[AngleIt]->Fill(Tracker2HitlocVec[i-1], Tracker2HitlocVec[i]);
      //Tracker1HitmapPerRotation[AngleIt]->Fill(Tracker1HitlocVec[i-1], Tracker1HitlocVec[i]);
	
      i++;
      
      //double cosineREF2;
      //angleREF2->Fill(cosineREF2 = CalculateCosTheta1(Tracker1HitlocVec[i-1],Tracker1HitlocVec[i],Tracker2HitlocVec[i-1],Tracker2HitlocVec[i]));    	  
    }

    // cout << "Average angle difference between Tracker 1 and 2: " << angleREF2->GetMean() << endl;
      	
    auto NewRes2 = Residuals(Tracker1HitlocVec, Tracker2HitlocVec, ShiftedTracker3HitlocVec, ShiftedTracker4HitlocVec, BadEvents);
    auto NewRes3 = Residuals(Tracker1HitlocVec, ShiftedTracker2HitlocVec, Tracker3HitlocVec, ShiftedTracker4HitlocVec, BadEvents);
    auto NewRes4 = Residuals(Tracker1HitlocVec, ShiftedTracker2HitlocVec, ShiftedTracker3HitlocVec, Tracker4HitlocVec, BadEvents);


    auto Chisquares2 = ChiSquared(Tracker1HitlocVec, Tracker2HitlocVec, ShiftedTracker3HitlocVec, ShiftedTracker4HitlocVec, BadEvents);
    auto Chisquares3 = ChiSquared(Tracker1HitlocVec, ShiftedTracker2HitlocVec, Tracker3HitlocVec, ShiftedTracker4HitlocVec, BadEvents);
    auto Chisquares4 = ChiSquared(Tracker1HitlocVec, ShiftedTracker2HitlocVec, ShiftedTracker3HitlocVec, Tracker4HitlocVec, BadEvents);
 
    xChi2PerAngleT2->SetPoint(AngleIt+bounds, angleT2temp, Chisquares2[0]);
    yChi2PerAngleT2->SetPoint(AngleIt+bounds, angleT2temp, Chisquares2[1]);

    xChi2PerAngleT3->SetPoint(AngleIt+bounds, angleT3temp, Chisquares3[0]);
    yChi2PerAngleT3->SetPoint(AngleIt+bounds, angleT3temp, Chisquares3[1]);

    xChi2PerAngleT4->SetPoint(AngleIt+bounds, angleT4temp, Chisquares4[0]);
    yChi2PerAngleT4->SetPoint(AngleIt+bounds, angleT4temp, Chisquares4[1]);
    
        
    if((MeanangleT2 <= -2 || MeanangleT2 > 2) || (MeanangleT3 <= -2 || MeanangleT3 > 2) || (MeanangleT4 <= -2 || MeanangleT4 > 2)){
      
      TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 500, 500);
      //TrackerRes->Divide(2,2);
      
      //TrackerRes->cd(1);
      angleREF2->Draw();
	
      return Optimized_Angles_And_XY;

    }
    
    
    
    /*
      if(plots){
      TCanvas* TrackerHitmaps = new TCanvas(nameRotCanvas, "", 1000, 500);
    
      TrackerHitmaps->Divide(2,1);
      TrackerHitmaps->cd(1);
      Tracker1HitmapPerRotation[AngleIt]->Draw("colz");
      gPad->Update();
	  
      TrackerHitmaps->cd(2);
      Tracker2HitmapPerRotation[AngleIt]->Draw("colz");
      gPad->Update();
	
      TrackerHitmaps->Draw();
      gPad->Update();
      //gSystem->ProcessEvents();
      //gSystem->ProcessEvents();
      TrackerHitmaps->SaveAs(nameRotCanvas);
	
      continue;
      }
    */
  }

  xChi2PerAngleT2->Fit("pol2");
  TF1 *fit2x = xChi2PerAngleT2->GetFunction("pol2");
  auto optxAngle2 = -(fit2x->GetParameter(1))/(2*fit2x->GetParameter(2));
  cout << "Angle of vertex is: " << optxAngle2 << endl;
  
  yChi2PerAngleT2->Fit("pol2");
  TF1 *fit2y = yChi2PerAngleT2->GetFunction("pol2");
  auto optyAngle2 = -(fit2y->GetParameter(1))/(2*fit2y->GetParameter(2));
  cout << "Angle of vertex is: " << optyAngle2 << endl;

  auto optAngleT2 = (optxAngle2 + optyAngle2)/2;
  
  xChi2PerAngleT3->Fit("pol2");
  TF1 *fit3x = xChi2PerAngleT3->GetFunction("pol2");
  auto optxAngle3 = -(fit3x->GetParameter(1))/(2*fit3x->GetParameter(2));
  cout << "Angle of vertex is: " << optxAngle3 << endl;
  
  yChi2PerAngleT3->Fit("pol2");
  TF1 *fit3y = xChi2PerAngleT3->GetFunction("pol2");
  auto optyAngle3 = -(fit3y->GetParameter(1))/(2*fit3y->GetParameter(2));
  cout << "Angle of vertex is: " << optyAngle3 << endl;

  auto optAngleT3 = (optxAngle3 + optyAngle3)/2;
    
  xChi2PerAngleT4->Fit("pol2");
  TF1 *fit4x = xChi2PerAngleT4->GetFunction("pol2");
  auto optxAngle4 = -(fit4x->GetParameter(1))/(2*fit4x->GetParameter(2));
  cout << "Angle of vertex is: " << optxAngle4 << endl;
  
  yChi2PerAngleT4->Fit("pol2");
  TF1 *fit4y = xChi2PerAngleT4->GetFunction("pol2");
  auto optyAngle4 = -(fit4y->GetParameter(1))/(2*fit4y->GetParameter(2));
  cout << "Angle of vertex is: " << optyAngle4 << endl;

  auto optAngleT4 = (optxAngle4 + optyAngle4)/2;

  
    
  Optimized_Angles_And_XY.push_back(Shift_T1x); Optimized_Angles_And_XY.push_back(Shift_T1y);
  Optimized_Angles_And_XY.push_back(Shift_T2x); Optimized_Angles_And_XY.push_back(Shift_T2y);
  Optimized_Angles_And_XY.push_back(Shift_T3x); Optimized_Angles_And_XY.push_back(Shift_T3y);
  Optimized_Angles_And_XY.push_back(Shift_T4x); Optimized_Angles_And_XY.push_back(Shift_T4y);
      
  Optimized_Angles_And_XY.push_back(optAngleT2);
  Optimized_Angles_And_XY.push_back(optAngleT3);
  Optimized_Angles_And_XY.push_back(optAngleT4);

  cout << "Pre Aligment first X value: " << prealignedX << endl;
  cout << "Pre Aligment first Y value: " << prealignedY << endl;
  
  cout << "PostAligment first X value: " << Tracker1HitlocVec[0] << endl;
  cout << "PostAligment first Y value: " << Tracker1HitlocVec[1] << endl;
	
  //Tracker plot for IEEE abstract
  if(plots){

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
      Residual1XPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual1XPerIteration->Draw("AP");
      Residual1YPerIteration->SetMarkerStyle(24);
      Residual1YPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual1YPerIteration->Draw("Psame");
      Residual2XPerIteration->SetMarkerStyle(21);
      Residual2XPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual2XPerIteration->Draw("Psame");
      Residual2YPerIteration->SetMarkerStyle(25);
      Residual2YPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual2YPerIteration->Draw("Psame");
      Residual3XPerIteration->SetMarkerStyle(22);
      Residual3XPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual3XPerIteration->Draw("Psame");
      Residual3YPerIteration->SetMarkerStyle(26);
      Residual3YPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual3YPerIteration->Draw("Psame");
      Residual4YPerIteration->SetMarkerStyle(23);
      Residual4YPerIteration->GetYaxis()->SetRangeUser(-2, 2);
      Residual4XPerIteration->Draw("Psame");
      Residual4YPerIteration->SetMarkerStyle(27);
      Residual4YPerIteration->GetYaxis()->SetRangeUser(-2, 2);
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
    */
    TCanvas* ChiSquareData = new TCanvas("", "", 1000, 1500);
    ChiSquareData->Divide(2,3);

    ChiSquareData->cd(1);
    xChi2PerAngleT2->Draw("A*");	 
    ChiSquareData->cd(2);
    yChi2PerAngleT2->Draw("A*");
    ChiSquareData->cd(3);
    xChi2PerAngleT3->Draw("A*");	 
    ChiSquareData->cd(4);
    yChi2PerAngleT3->Draw("A*");
    ChiSquareData->cd(5);
    xChi2PerAngleT4->Draw("A*");	 
    ChiSquareData->cd(6);
    yChi2PerAngleT4->Draw("A*");

    ChiSquareData->SaveAs("Alignment3_chisquared.png");
      
    return Optimized_Angles_And_XY;
      
  }
    
  //double scale_Factor = -0.1;	
  //XY shift values stay constant

  /*
    Shift_T1x = scale_Factor*mean_Tracker1xcoordRes; Shift_T1y = scale_Factor*mean_Tracker1ycoordRes;
    Shift_T2x = scale_Factor*mean_Tracker2xcoordRes; Shift_T2y = scale_Factor*mean_Tracker2ycoordRes;
    Shift_T3x = scale_Factor*mean_Tracker3xcoordRes; Shift_T3y = scale_Factor*mean_Tracker3ycoordRes;
    Shift_T4x = scale_Factor*mean_Tracker4xcoordRes; Shift_T4y = scale_Factor*mean_Tracker4ycoordRes;
  */

  /*
    double anglescale_Factor = 0.1;	
    angleT2 = anglescale_Factor*MeanangleT2;
    angleT3 = anglescale_Factor*MeanangleT3;
    angleT4 = anglescale_Factor*MeanangleT4;
  */
    
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
  //break;
  
  
  return Optimized_Angles_And_XY;


}

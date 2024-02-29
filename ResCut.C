vector<float> ResCut(vector<float>& Tracker1HitlocVec, vector<float>& Tracker2HitlocVec, vector<float>& Tracker3HitlocVec, vector<float>& Tracker4HitlocVec){

  //this vector: x1, y1, z1, x2, y2, z2, . . 
  //TGraph* OGVals = new TGraph();
  //TGraph* CalcVals = new TGraph();
  //TGraph* ResVals = new TGraph();

  vector<float> T1X;
  vector<float> T1Y;
  vector<float> T2X;
  vector<float> T2Y;
  vector<float> T3X;
  vector<float> T3Y;
  vector<float> T4X;
  vector<float> T4Y;
  float T1Z;
  float T2Z;
  float T3Z;
  float T4Z;

  vector<float> BadEventIDs;
  TH1F* resplot = new TH1F("","Residual", 50,-1,1); resplot->SetXTitle("Residual [mm]"); resplot->SetYTitle("Counts");resplot->SetLabelSize(0.045,"XY");resplot->SetTitleSize(0.045,"XY");
  for(int i=0;i<Tracker1HitlocVec.size();i++){
    
    T1X.push_back(Tracker1HitlocVec.at(i));
    T2X.push_back(Tracker2HitlocVec.at(i));
    T3X.push_back(Tracker3HitlocVec.at(i));
    T4X.push_back(Tracker4HitlocVec.at(i));
    i++;
    T1Y.push_back(Tracker1HitlocVec.at(i));
    T2Y.push_back(Tracker2HitlocVec.at(i));
    T3Y.push_back(Tracker3HitlocVec.at(i));
    T4Y.push_back(Tracker4HitlocVec.at(i));
    i++;
    //cout << i << endl;
    if(i <= 5 ){
      T1Z = Tracker1HitlocVec.at(i);
      T2Z = Tracker2HitlocVec.at(i);
      T3Z = Tracker3HitlocVec.at(i);
      T4Z = Tracker4HitlocVec.at(i);
    }
  }

  
  float inter=0;
  //cout << T1Z << endl;
  //cout << T2Z << endl;
  //cout << T3Z << endl;
  //cout << T4Z << endl;
  //return;
  
  for(int i=0; i< T1X.size(); i++){
    //cout << i << endl;
    TGraph* gx = new TGraph();
    TF1* fx = new TF1("line1","pol1", 0, 1600);
    TGraph* gy = new TGraph();
    TF1* fy = new TF1("line2","pol1", 0, 1600);   
    
  
    //cout << "A" << endl;
    
    gx->SetPoint(0, T1Z, T1X.at(i));
    gx->SetPoint(1, T2Z, T2X.at(i));
    gx->SetPoint(2, T3Z, T3X.at(i));
    gx->SetPoint(3, T4Z, T4X.at(i));
    
    //cout << T2X.at(i) << ", " <<  T2Y.at(i) << endl;
    
    gy->SetPoint(0, T1Z, T1Y.at(i));
    gy->SetPoint(1, T2Z, T2Y.at(i));
    gy->SetPoint(2, T3Z, T3Y.at(i));
    gy->SetPoint(3, T4Z, T4Y.at(i));
  
   
  	    
    gx->Fit("line1","Q");
    
    gy->Fit("line2","Q");
   
    //gy->Draw("A*");
    gy->SetTitle("Y Coordinates of Hit");
    gy->GetXaxis()->SetTitle("Z coordinate");
    gy->GetYaxis()->SetTitle("Y coordinate");
    
    float interceptx = fx->GetParameter(0);
    //cout << "x Yintercepts: " << interceptx << endl;    
    float slopex     = fx->GetParameter(1);
    
    float intercepty = fy->GetParameter(0);
    //cout << "y Yintercepts: " << intercepty << endl;    
    float slopey = fy->GetParameter(1);
    
    
    
    float Measured_Tracker1x = interceptx + slopex*T1Z;
    //cout << "Measured 1x: " << Measured_Tracker1x << endl;
    //Calculated1xcl->Fill(Measured_Tracker1x);
    float Tracker1xRes = Measured_Tracker1x - T1X.at(i);    
    //cout << "1X Coord:: " << Measured_Tracker1x << " - " <<  Tracker1HitlocVec[i] << " = " << Measured_Tracker1x - Tracker1HitlocVec[i] << endl;
    
    float Measured_Tracker1y = intercepty + slopey*T1Z;
    //Calculated1ycl->Fill(Measured_Tracker1y);
    float Tracker1yRes = Measured_Tracker1y - T1Y.at(i);
    resplot->Fill(Tracker1yRes);
    
    if(Tracker1yRes > 0.531 || Tracker1yRes < 0.235){//use double gaussian fit to find the range of the main peak
      cout << T1X.at(i) << ", " << T1Y.at(i) << endl;
      cout << "Bad Event ID:  " << i << endl;
      
      inter++;

      BadEventIDs.push_back(i);
      //T1X.erase(T1X.begin()+i);T1Y.erase(T1Y.begin()+i);
      //T2X.erase(T2X.begin()+i);T2Y.erase(T2Y.begin()+i);
      //T3X.erase(T3X.begin()+i);T3Y.erase(T3Y.begin()+i);
      //T4X.erase(T4X.begin()+i);T4Y.erase(T4Y.begin()+i);
      
    }
    
   
    
    float Measured_Tracker2x = interceptx + slopex*T2Z;
    ////Calculated2xcl->Fill(Measured_Tracker2x);
    float Tracker2xRes = Measured_Tracker2x - T2X.at(i);    
    
    //cout << "2X Coord:: " << Measured_Tracker2x << " - " <<  Tracker2HitlocVec[i] << " = " << Measured_Tracker2x - Tracker2HitlocVec[i] << endl;
    float Measured_Tracker2y = intercepty + slopey*T2Z;
    //Calculated2ycl->Fill(Measured_Tracker2y);
    float Tracker2yRes = Measured_Tracker2y - T2Y.at(i);  
    //cout << "2Y Coord:: " << Measured_Tracker2y << " - " <<  T2Y.at(i) << " = " << Measured_Tracker2y - T2Y.at(i) << endl;
   
    
    float Measured_Tracker3x = interceptx + slopex*T3Z;
    //Calculated3xcl->Fill(Measured_Tracker3x);
    float Tracker3xRes = Measured_Tracker3x - T3X.at(i);
    
    float Measured_Tracker3y = intercepty + slopey*T3Z;
    //Calculated3ycl->Fill(Measured_Tracker3y);
    float Tracker3yRes = Measured_Tracker3y - T3Y.at(i);
         
    float Measured_Tracker4x = interceptx + slopex*T4Z;
    //Calculated4xcl->Fill(Measured_Tracker4x);
    float Tracker4xRes = Measured_Tracker4x - T4X.at(i);

    float Measured_Tracker4y = intercepty + slopey*T4Z;
    //Calculated4ycl->Fill(Measured_Tracker4y);
    float Tracker4yRes = Measured_Tracker4y - T4Y.at(i);


    
    /*   
    if(i < 2000){
      OGVals->SetPoint(i+1, i/3, T2Y.at(i));
      CalcVals->SetPoint(i+1, i/3, Measured_Tracker2y);
      ResVals->SetPoint(i+1, i/3, Tracker2yRes);
    }
    */
   
    
    
    delete gx;
    delete fx;
    delete gy;
    delete fy;
    
    //i++;
    //i++;
  }
  //cout << inter << endl;

  
  I2GFvalues myValues;
	
  myValues = I2GFmainLoop(resplot,3,2,1);                           //converges!	  
  float ResMean = myValues.mean; float ResWidth=myValues.sigma;
  cout << ResMean << ", " << ResWidth << endl;
  cout << "bounds: " << ResMean-5*ResWidth << ", " << ResMean+5*ResWidth << endl;
  //TF1* h = new TF1("h", "gaus + gaus");
  //resplot->Fit("h", "Q");
  //resplot->Draw();
  
  
  //  for(auto j = T2X.begin(); j != T2X.end(); j++){
  //  cout << *j << endl;
  //}

  Tracker1HitlocVec.clear();
  Tracker2HitlocVec.clear();
  Tracker3HitlocVec.clear();
  Tracker4HitlocVec.clear();

  for(auto i = 0; i < T1X.size(); i++){
    Tracker1HitlocVec.push_back(T1X.at(i));
    Tracker2HitlocVec.push_back(T2X.at(i));
    Tracker3HitlocVec.push_back(T3X.at(i));
    Tracker4HitlocVec.push_back(T4X.at(i));

    Tracker1HitlocVec.push_back(T1Y.at(i));
    Tracker2HitlocVec.push_back(T2Y.at(i));
    Tracker3HitlocVec.push_back(T3Y.at(i));
    Tracker4HitlocVec.push_back(T4Y.at(i));

    Tracker1HitlocVec.push_back(T1Z);
    Tracker2HitlocVec.push_back(T2Z);
    Tracker3HitlocVec.push_back(T3Z);
    Tracker4HitlocVec.push_back(T4Z);
  }
    
  //for(auto j = Tracker2HitlocVec.begin(); j != Tracker2HitlocVec.end(); j++){
  //  cout << *j << endl;
  //}

  /*
  TCanvas* slops = new TCanvas("slops", "", 500, 500);
  OGVals->GetYaxis()->SetRangeUser(-10, 50);
  //OGVals->GetXaxis()->SetRangeUser(0, 5000);  
  OGVals->SetMarkerStyle(20);
  OGVals->Draw("AP");
  CalcVals->GetYaxis()->SetRangeUser(-10, 50);
  //CalcVals->GetXaxis()->SetRangeUser(0, 5000);
  CalcVals->SetMarkerColor(3);
  CalcVals->SetMarkerStyle(21);
  CalcVals->Draw("PSame");
  ResVals->GetYaxis()->SetRangeUser(-10, 50);
  //ResVals->GetXaxis()->SetRangeUser(0, 5000);
  ResVals->SetMarkerColor(4);
  ResVals->SetMarkerStyle(22);
  ResVals->Draw("PSame");

  auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
  legend->AddEntry(OGVals, "Original Values", "P");
  legend->AddEntry(CalcVals, "Calculated Values", "P");
  legend->AddEntry(ResVals, "Residuals", "P");
  
  legend->Draw();
  slops->SaveAs("All1Yvalues.png");
  
  slops->Draw();
  */
  return BadEventIDs;
}

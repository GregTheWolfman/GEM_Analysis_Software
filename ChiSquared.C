vector<float> ChiSquared(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> BadEvents){
  
  vector<float> ChiSquaredMeans;

  TH1F* xTrackChi2 = new TH1F("XTrackChi2","Chi square of tracks in X projection",1000,0,100); xTrackChi2->SetXTitle("#chi^{2} of track in X"); xTrackChi2->SetYTitle("Frequency"); xTrackChi2->SetTitleSize(0.04,"XY"); xTrackChi2->SetLabelSize(0.04,"XY");
  TH1F* yTrackChi2 = new TH1F("YTrackChi2","Chi square of tracks in Y projection",1000,0,100); yTrackChi2->SetXTitle("#chi^{2} of track in Y"); yTrackChi2->SetYTitle("Frequency"); yTrackChi2->SetTitleSize(0.04,"XY"); yTrackChi2->SetLabelSize(0.04,"XY");
  
  if(Tracker1HitlocVec.empty() || Tracker2HitlocVec.empty() || Tracker3HitlocVec.empty() || Tracker4HitlocVec.empty())
    return ChiSquaredMeans;

    
  for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){
    //cout << i << endl;
    if(find(BadEvents.begin(), BadEvents.end(), i/3) == BadEvents.end()){
      TGraph* gx = new TGraph();
      TF1* fx = new TF1("line1","[0]+[1]*x",0,1600);
      TGraph* gy = new TGraph();
      TF1* fy = new TF1("line2","[0]+[1]*x",0,1600);

	    
  
      gx->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i]);
      gx->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i]);
      gx->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i]);
      gx->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i]);
    

      gy->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i+1]);
      gy->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i+1]);
      gy->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i+1]);
      gy->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i+1]);
 
      gx->Fit("line1","Q");
    
      gy->Fit("line2","Q");
    
      double interceptx = fx->GetParameter(0);
      double slopex     = fx->GetParameter(1);

      double intercepty = fy->GetParameter(0);
      double slopey     = fy->GetParameter(1);

      //cout << "X Chi squared: " << fx->GetChisquare() << endl;
      //cout << "Y Chi squared: " << fy->GetChisquare() << endl;
      xTrackChi2->Fill(fx->GetChisquare());
      yTrackChi2->Fill(fy->GetChisquare());
    
      delete gx;
      delete fx;
      delete gy;
      delete fy;
    }
    i++;
    i++;
  }
  
  //cout << "Mean X Chi Squared: " << xTrackChi2->GetMean() << endl;
  //cout << "Mean Y Chi Squared: " << yTrackChi2->GetMean() << endl;
  ChiSquaredMeans.push_back(xTrackChi2->GetMean());
  ChiSquaredMeans.push_back(yTrackChi2->GetMean());

  delete xTrackChi2;
  delete yTrackChi2;
      
  return ChiSquaredMeans;
}

vector<float> ChiSquared(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec){
  
  vector<float> ChiSquaredMeans;

  TH1F* xTrackChi2 = new TH1F("XTrackChi2","Chi square of tracks in X projection",1000,0,100); xTrackChi2->SetXTitle("#chi^{2} of track in X"); xTrackChi2->SetYTitle("Frequency"); xTrackChi2->SetTitleSize(0.04,"XY"); xTrackChi2->SetLabelSize(0.04,"XY");
  TH1F* yTrackChi2 = new TH1F("YTrackChi2","Chi square of tracks in Y projection",1000,0,100); yTrackChi2->SetXTitle("#chi^{2} of track in Y"); yTrackChi2->SetYTitle("Frequency"); yTrackChi2->SetTitleSize(0.04,"XY"); yTrackChi2->SetLabelSize(0.04,"XY");
  
  if(Tracker1HitlocVec.empty() || Tracker2HitlocVec.empty() || Tracker3HitlocVec.empty() || Tracker4HitlocVec.empty())
    return ChiSquaredMeans;

    
  for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){
    //cout << i << endl;
    TGraph* gx = new TGraph();
    TF1* fx = new TF1("line1","[0]+[1]*x",0,1600);
    TGraph* gy = new TGraph();
    TF1* fy = new TF1("line2","[0]+[1]*x",0,1600);

	    
  
    gx->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i]);
    gx->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i]);
    gx->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i]);
    gx->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i]);
    

    gy->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i+1]);
    gy->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i+1]);
    gy->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i+1]);
    gy->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i+1]);
 
    gx->Fit("line1","Q");
    
    gy->Fit("line2","Q");
    
    double interceptx = fx->GetParameter(0);
    double slopex     = fx->GetParameter(1);

    double intercepty = fy->GetParameter(0);
    double slopey     = fy->GetParameter(1);

    //cout << "X Chi squared: " << fx->GetChisquare() << endl;
    //cout << "Y Chi squared: " << fy->GetChisquare() << endl;
    xTrackChi2->Fill(fx->GetChisquare());
    yTrackChi2->Fill(fy->GetChisquare());
    
    delete gx;
    delete fx;
    delete gy;
    delete fy;    
    i++;
    i++;
  }
  
  //cout << "Mean X Chi Squared: " << xTrackChi2->GetMean() << endl;
  //cout << "Mean Y Chi Squared: " << yTrackChi2->GetMean() << endl;
  ChiSquaredMeans.push_back(xTrackChi2->GetMean());
  ChiSquaredMeans.push_back(yTrackChi2->GetMean());

  delete xTrackChi2;
  delete yTrackChi2;
      
  return ChiSquaredMeans;
}



vector<vector<float>> MultiClusterReducer(vector<float>, vector<float>,vector<float>, vector<float>,vector<float>, vector<float>,vector<float>, vector<float>,vector<float>, vector<float>,vector<float>, vector<float>,vector<float>, vector<float>,vector<float>, vector<float>,vector<float>, vector<float>, float, float);
//Make function that takes multi cluster events and chooses which cluster is relevant

vector<vector<float>> MultiClusterReducer(vector<float>  Tracker1xmaxstrips, vector<float>  Tracker1xmaxcharges,vector<float>  Tracker1ymaxstrips, vector<float>  Tracker1ymaxcharges,vector<float>  Tracker2xmaxstrips, vector<float>  Tracker2xmaxcharges,vector<float>  Tracker2ymaxstrips, vector<float>  Tracker2ymaxcharges,vector<float>  Tracker3xmaxstrips, vector<float>  Tracker3xmaxcharges,vector<float>  Tracker3ymaxstrips, vector<float>  Tracker3ymaxcharges,vector<float>  Tracker4xmaxstrips, vector<float>  Tracker4xmaxcharges,vector<float>  Tracker4ymaxstrips, vector<float>  Tracker4ymaxcharges, vector<float> LAGDmaxstrips, vector<float>  LAGDmaxcharges, float LAGDOffsetX, float LAGDOffsetY){


  cout << LAGDOffsetX << ", " << LAGDOffsetY << endl; 
  vector<vector<float>> ReturnHitInfo;

  vector<vector<float>> XHits(4);
  vector<float> ReducedXHits;
  vector<vector<float>> YHits(4);
  vector<float> ReducedYHits;
  
  vector<vector<vector<float>>> NewTrackerXStripHits;
  vector<vector<vector<float>>> NewTrackerYStripHits;


  //offsets for trackers


  //divide by 0.4 to convert to strips?
  float shift1x = 0.345;//mm
  float shift1y = -0.698;
  float shift2x = -0.379;
  float shift2y = 0.745;
  float shift3x = 0.613;
  float shift3y = -0.621;
  float shift4x = -0.579;
  float shift4y = 0.573;

  //float LAGDOffsetX = -29.7665 + 80 + 31.269;//obtain offsets from golden set of data to add more LAGD data.
  //float LAGDOffsetY = -15.0655 + 7.728;
  float LAGDZ = 828.2;//mm
  //float LAGDRad = 0.10945;//m for easier conversion from mRad to mm
  
  // float shift1x = 0;
  // float shift1y = 0;
  // float shift2x = 0;
  // float shift2y = 0;
  // float shift3x = 0;
  // float shift3y = 0;
  // float shift4x = 0;
  // float shift4y = 0;

  // cout << "tracker1y issues" << endl;
  // for(int y = 0; y < Tracker1ymaxstrips.size(); y++){
  //   cout << Tracker1ymaxstrips.at(y) << endl;
  // }
  //if one hit and only one strip, go home
  if(Tracker1xmaxstrips.size() == 1)
    return ReturnHitInfo;
  if(Tracker1ymaxstrips.size() == 1){
    //cout << "Here?" << endl;
    return ReturnHitInfo;
  }
  if(Tracker2xmaxstrips.size() == 1)
    return ReturnHitInfo;
  if(Tracker2ymaxstrips.size() == 1)
    return ReturnHitInfo;
  if(Tracker3xmaxstrips.size() == 1)
    return ReturnHitInfo;
  if(Tracker3ymaxstrips.size() == 1)
    return ReturnHitInfo;
  if(Tracker4xmaxstrips.size() == 1)
    return ReturnHitInfo;
  if(Tracker4ymaxstrips.size() == 1)
    return ReturnHitInfo;
  

     
  auto NumSubEvents1X = StripSep(Tracker1xmaxstrips, Tracker1xmaxcharges);
  int q = 0;
  vector<vector<float>> Tracker1xhits;
  for(int u = 0; u < NumSubEvents1X.size(); u++){
    auto num=0.0;
    auto den=0.0;
    vector<float> trackerstrips;
    vector<float> trackercharges;
    
    for(int t = 0; t < NumSubEvents1X.at(u); t++){
      //if(NumSubEvents1X.at(u) == 1){cout << "Removed noise event" << endl;q++;continue;}
      num += Tracker1xmaxstrips.at(q)*Tracker1xmaxcharges.at(q);
      den += Tracker1xmaxcharges.at(q);

      trackerstrips.push_back(Tracker1xmaxstrips.at(q));
      trackercharges.push_back(Tracker1xmaxcharges.at(q));
      q++;
    }
    //if(num == 0){continue;}
    auto loc = num/den;
    //    loc += shift1x;
    loc = loc*0.4 - shift1x;
    cout << "1XHit loc: " << loc << endl;
    XHits[0].push_back(loc);
    XHits[0].push_back(1596.4);
    Tracker1xhits.push_back(trackerstrips);
    trackerstrips.clear();
    Tracker1xhits.push_back(trackercharges);
    trackercharges.clear();
  }
  NewTrackerXStripHits.push_back(Tracker1xhits);

  vector<vector<float>> Tracker1yhits;
  q=0;
  auto NumSubEvents1Y = StripSep(Tracker1ymaxstrips, Tracker1ymaxcharges);
  for(int u = 0; u < NumSubEvents1Y.size(); u++){
    auto num=0.0;
    auto den=0.0;
    vector<float> trackerstrips;
    vector<float> trackercharges;
    for(int t = 0; t < NumSubEvents1Y.at(u); t++){
      //if(NumSubEvents1Y.at(u) == 1){cout << "Removed noise event" << endl;q++;continue;}
      num += Tracker1ymaxstrips.at(q)*Tracker1ymaxcharges.at(q);
      den += Tracker1ymaxcharges.at(q);
      trackerstrips.push_back(Tracker1ymaxstrips.at(q));
      trackercharges.push_back(Tracker1ymaxcharges.at(q));
      q++;
    }
    //if(num == 0){continue;}
    auto loc = num/den;
    //loc += shift1y;
    loc = loc*0.4 - shift1y;
    //cout << "1YHit loc: " << loc << endl;
    YHits[0].push_back(loc);
    YHits[0].push_back(1596.4);
    Tracker1yhits.push_back(trackerstrips);
    trackerstrips.clear();
    Tracker1yhits.push_back(trackercharges);
    trackercharges.clear();
  }
  NewTrackerYStripHits.push_back(Tracker1yhits);

  auto NumSubEvents2X = StripSep(Tracker2xmaxstrips, Tracker2xmaxcharges);
  vector<vector<float>> Tracker2xhits;
  q=0;
  for(int u = 0; u < NumSubEvents2X.size(); u++){
    auto num=0.0;
    auto den=0.0;
    vector<float> trackerstrips;
    vector<float> trackercharges;
    
    for(int t = 0; t < NumSubEvents2X.at(u); t++){
      //if(NumSubEvents2X.at(u) == 1){cout << "Removed noise event" << endl;q++;continue;}
      num += Tracker2xmaxstrips.at(q)*Tracker2xmaxcharges.at(q);
      den += Tracker2xmaxcharges.at(q);

      trackerstrips.push_back(Tracker2xmaxstrips.at(q));
      trackercharges.push_back(Tracker2xmaxcharges.at(q));
      q++;
      
    }
    //if(num == 0){continue;}
    auto loc = num/den;
    //loc += shift2x;
    loc = loc*0.4 - shift2x;
    //cout << "2XHit loc: " << loc << endl;
    XHits[1].push_back(loc);
    XHits[1].push_back(1541.6);
    Tracker2xhits.push_back(trackerstrips);
    trackerstrips.clear();
    Tracker2xhits.push_back(trackercharges);
    trackercharges.clear();
  }
  NewTrackerXStripHits.push_back(Tracker2xhits);

  vector<vector<float>> Tracker2yhits;
  q=0;
  auto NumSubEvents2Y = StripSep(Tracker2ymaxstrips, Tracker2ymaxcharges);
  for(int u = 0; u < NumSubEvents2Y.size(); u++){
    auto num=0.0;
    auto den=0.0;
    vector<float> trackerstrips;
    vector<float> trackercharges;
    for(int t = 0; t < NumSubEvents2Y.at(u); t++){
      //if(NumSubEvents2Y.at(u) == 1){cout << "Removed noise event" << endl;q++;continue;}
      num += Tracker2ymaxstrips.at(q)*Tracker2ymaxcharges.at(q);
      den += Tracker2ymaxcharges.at(q);
      trackerstrips.push_back(Tracker2ymaxstrips.at(q));
      trackercharges.push_back(Tracker2ymaxcharges.at(q));
      q++;
    }
    //if(num == 0){continue;}
    auto loc = num/den;
    //loc += shift2y;
    loc = loc*0.4 - shift2y;
    //cout << "2YHit loc: " << loc << endl;
    YHits[1].push_back(loc);
    YHits[1].push_back(1541.6);
    Tracker2yhits.push_back(trackerstrips);
    trackerstrips.clear();
    Tracker2yhits.push_back(trackercharges);
    trackercharges.clear();
  }
  NewTrackerYStripHits.push_back(Tracker2yhits);

 
  auto NumSubEvents3X = StripSep(Tracker3xmaxstrips, Tracker3xmaxcharges);
  vector<vector<float>> Tracker3xhits;  
  q = 0;
  for(int u = 0; u < NumSubEvents3X.size(); u++){
    auto num=0.0;
    auto den=0.0;
    vector<float> trackerstrips;
    vector<float> trackercharges;
    for(int t = 0; t < NumSubEvents3X.at(u); t++){
      //if(NumSubEvents3X.at(u) == 1){cout << "Removed noise event" << endl;q++;continue;}
      num += Tracker3xmaxstrips.at(q)*Tracker3xmaxcharges.at(q);
      den += Tracker3xmaxcharges.at(q);
      //cout << Tracker3xmaxstrips.at(q) << endl;
      
      trackerstrips.push_back(Tracker3xmaxstrips.at(q));
      trackercharges.push_back(Tracker3xmaxcharges.at(q));
      
      q++;
    }
    //if(num == 0){continue;}
    auto loc = num/den;
    //loc += shift3x;
    loc = loc*0.4 - shift3x;
    //cout << "3XHit loc: " << loc << endl;
    XHits[2].push_back(loc);
    XHits[2].push_back(54.8);
    Tracker3xhits.push_back(trackerstrips);
    trackerstrips.clear();
    Tracker3xhits.push_back(trackercharges);
    trackercharges.clear();
  }
  NewTrackerXStripHits.push_back(Tracker3xhits);

  vector<vector<float>> Tracker3yhits;
  q=0;
  auto NumSubEvents3Y = StripSep(Tracker3ymaxstrips, Tracker3ymaxcharges);
  for(int u = 0; u < NumSubEvents3Y.size(); u++){
    auto num=0.0;
    auto den=0.0;
    vector<float> trackerstrips;
    vector<float> trackercharges;
    for(int t = 0; t < NumSubEvents3Y.at(u); t++){
      //if(NumSubEvents3Y.at(u) == 1){cout << "Removed noise event" << endl;q++;continue;}
      num += Tracker3ymaxstrips.at(q)*Tracker3ymaxcharges.at(q);
      den += Tracker3ymaxcharges.at(q);
      trackerstrips.push_back(Tracker3ymaxstrips.at(q));
      trackercharges.push_back(Tracker3ymaxcharges.at(q));
      q++;
    }
    //if(num == 0){continue;}
    auto loc = num/den;
    //loc += shift3y;
    loc = loc*0.4 - shift3y;
    //cout << "3YHit loc: " << loc << endl;
    YHits[2].push_back(loc);
    YHits[2].push_back(54.8);
    Tracker3yhits.push_back(trackerstrips);
    trackerstrips.clear();
    Tracker3yhits.push_back(trackercharges);
    trackercharges.clear();
  }
  NewTrackerYStripHits.push_back(Tracker3yhits);

  auto NumSubEvents4X = StripSep(Tracker4xmaxstrips, Tracker4xmaxcharges);
  vector<vector<float>> Tracker4xhits;
  q=0;
  for(int u = 0; u < NumSubEvents4X.size(); u++){
    auto num=0.0;
    auto den=0.0;
    vector<float> trackerstrips;
    vector<float> trackercharges;
    
    for(int t = 0; t < NumSubEvents4X.at(u); t++){
      //if(NumSubEvents4X.at(u) == 1){cout << "Removed noise event" << endl;q++;continue;}
      num += Tracker4xmaxstrips.at(q)*Tracker4xmaxcharges.at(q);
      den += Tracker4xmaxcharges.at(q);
      //cout << Tracker4xmaxstrips.at(q) << endl;

      trackerstrips.push_back(Tracker4xmaxstrips.at(q));
      trackercharges.push_back(Tracker4xmaxcharges.at(q));
      q++;
      
    }
    //if(num == 0){continue;}
    auto loc = num/den;
    //loc += shift4x;
    loc = loc*0.4 - shift4x;
    //cout << "4XHit loc: " << loc << endl;
    XHits[3].push_back(loc);
    XHits[3].push_back(0.0);
    Tracker4xhits.push_back(trackerstrips);
    trackerstrips.clear();
    Tracker4xhits.push_back(trackercharges);
    trackercharges.clear();
  }
  NewTrackerXStripHits.push_back(Tracker4xhits);

  vector<vector<float>> Tracker4yhits;
  q=0;
  auto NumSubEvents4Y = StripSep(Tracker4ymaxstrips, Tracker4ymaxcharges);
  for(int u = 0; u < NumSubEvents4Y.size(); u++){
    auto num=0.0;
    auto den=0.0;
    vector<float> trackerstrips;
    vector<float> trackercharges;
    for(int t = 0; t < NumSubEvents4Y.at(u); t++){
      //if(NumSubEvents4Y.at(u) == 1){cout << "Removed noise event" << endl;q++;continue;}
      num += Tracker4ymaxstrips.at(q)*Tracker4ymaxcharges.at(q);
      den += Tracker4ymaxcharges.at(q);
      trackerstrips.push_back(Tracker4ymaxstrips.at(q));
      trackercharges.push_back(Tracker4ymaxcharges.at(q));
      q++;
    }
    //if(num == 0){continue;}
    auto loc = num/den;
    //loc += shift4y;

    loc = loc*0.4 - shift4y;
    //cout << "4YHit loc: " << loc << endl;
    YHits[3].push_back(loc);
    YHits[3].push_back(0.0);
    Tracker4yhits.push_back(trackerstrips);
    trackerstrips.clear();
    Tracker4yhits.push_back(trackercharges);
    trackercharges.clear();
  }

  
  
  NewTrackerYStripHits.push_back(Tracker4yhits);

  // auto XallHits = new TGraph();
  // auto YallHits = new TGraph();
  // int k=0;
  // for(int y = 0; y < YHits.size(); y++){
  //   for(int j = 0; j < YHits[y].size(); j++){ 
  //     //cout << NewTrackerXStripHits[y][j][u] << ", ";
  //     YallHits->SetPoint(k,YHits[y][j+1],YHits[y][j]);
  //     k++;
  //     j++;
  //   }    
  // }
  // auto l = new TCanvas("", "",  500, 500);
  
  // YallHits->Draw("A*");
  // TF1* fthis = new TF1("lineyall","pol1",0,140);
  // YallHits->Fit("lineyall","R");
  // l->Draw();
  
  // //cout << "X hits detected" << endl;
  // for(int y = 0; y < NewTrackerXStripHits.size(); y++){
  //   for(int j = 0; j < NewTrackerXStripHits[y].size(); j++){ 
  //     for(int u = 0; u < NewTrackerXStripHits[y][j].size(); u++){  
  // 	cout << NewTrackerXStripHits[y][j][u] << ", ";
  //     }
  //     cout << endl;
  //   }
  //   cout << endl;
  //   cout << endl;
  // }


  // cout << "Y hits detected" << endl;
  // for(int y = 0; y < NewTrackerYStripHits.size(); y++){
  //   for(int j = 0; j < NewTrackerYStripHits[y].size(); j++){ 
  //     for(int u = 0; u < NewTrackerYStripHits[y][j].size(); u++){  
  //     cout << NewTrackerYStripHits[y][j][u] << ", ";
  //     }
  //     cout << endl;
  //   }
  //   cout << endl;
  //   cout << endl;
  // }

  //return ReturnHitInfo;
  
  
  //need to do X and Y separately

  //look at each X vector, separate by num sub events == 1 and == 2 for now
  //Average hit location in nCluster == 1 hits
  //in x vectors with ncluster == 2, chose the cluster closest to average hit location

  //for rn, assume only 1 or 2 hits are 2 hit clusters, that probably normal.... right?

  
  TGraph* g1 = new TGraph();
  int plotit=0;
  for(int x = 0; x < XHits.size(); x++){
    if(XHits[x].size() == 2){//cluster with 1 hit (x and z coordinate)
      g1->SetPoint(plotit, XHits[x][1], XHits[x][0]);
      plotit++;      
    }    
  }
  // auto test = new TCanvas("", "", 500, 500);
  // g1->Draw("A*");
  // test->Draw();
    
  //cout << "Number of 1 hit X events " << plotit << endl;
  //with good clusters plotted, we can fit a line to reconstruct a track
  //then we can individually find the exclusive residual of either cluster and choose the smallest value (in magnitude)

 
  
  TGraph* g2 = new TGraph();
  int yplotit=0;
  for(int x = 0; x < YHits.size(); x++){
    if(YHits[x].size() == 2){//cluster with 1 hit (y and z coordinate)
      g2->SetPoint(yplotit, YHits[x][1], YHits[x][0]);
      yplotit++;      
    }    
  }

  if(plotit < 2 || yplotit < 2){
    cout << plotit << " double clusters in X, " << yplotit << " in Y" << endl;
    cout << "Too many double clusters, removing event :(" << endl;
    return ReturnHitInfo;
  }

  //cout << "Number of 1 hit Y events " << yplotit << endl;
  TF1* f2 = new TF1("line2","pol1",0,1600);
  g2->Fit("line2","Q");
  double intercept2 = f2->GetParameter(0);
  double slope2     = f2->GetParameter(1);

  TF1* f1 = new TF1("line1","pol1",0,1600);
  g1->Fit("line1","Q");
  double intercept1 = f1->GetParameter(0);
  double slope1     = f1->GetParameter(1);

 
 
  //now, we can loop again, but this time over the double clusters. Find the exclusive res of the clusters, and choose the smallest value.
  
  for(int x = 0; x < XHits.size(); x++){
    
    if(XHits[x].size() != 2 && XHits[x].size()%2 == 0){//cluster with 2 or 3 hit (x and z coordinate)
      vector<float> avgres;      
      for(int h = 0; h < XHits[x].size()/2; h++){

	double Xtheo = intercept1 + slope1*XHits[x][1+(2*h)];
	double res = abs(XHits[x][0+(2*h)] - Xtheo);
	//cout << "residuals: " << res << endl;
	avgres.push_back(res);
      }
      int bestcluster = distance(avgres.begin(), min_element(avgres.begin(),avgres.end()));
      //cout << "Chosen cluster: " << bestcluster << endl;
      //cout << XHits[x][bestcluster*2] << ", " << XHits[x][(bestcluster*2)+1] << endl;
      //XHits[x].erase(XHits[x].begin()+(2*bestcluster));
      //XHits[x].erase(XHits[x].begin()+(2*bestcluster)+1);

      vector<float> substripvector;
      vector<float> subchargevector;

      
      for(int u = 0; u < NewTrackerXStripHits[x][bestcluster*2].size(); u++){  
	//cout << NewTrackerXStripHits[x][bestcluster*2][u] << ", ";
	substripvector.push_back(NewTrackerXStripHits[x][bestcluster*2][u]);
      }
      //cout << endl;

      for(int u = 0; u < NewTrackerXStripHits[x][(bestcluster*2)+1].size(); u++){  
	//cout << NewTrackerXStripHits[x][(bestcluster*2)+1][u] << ", ";
	subchargevector.push_back(NewTrackerXStripHits[x][(bestcluster*2)+1][u]);
      }
      //cout << endl;

      
      ReturnHitInfo.push_back(substripvector);
      ReturnHitInfo.push_back(subchargevector);
      ReducedXHits.push_back(XHits[x][bestcluster*2]);
      ReducedXHits.push_back(XHits[x][(bestcluster*2)+1]);
      
    }

    else if(XHits[x].size() == 2){
      vector<float> substripvector;
      vector<float> subchargevector;
      //cout << "only one hit seen" << endl;
      
      for(int u = 0; u < NewTrackerXStripHits[x][0].size(); u++){  
	//cout << NewTrackerXStripHits[x][0][u] << ", ";
	substripvector.push_back(NewTrackerXStripHits[x][0][u]);
      }
      //cout << endl;

      for(int u = 0; u < NewTrackerXStripHits[x][1].size(); u++){  
	//cout << NewTrackerXStripHits[x][1][u] << ", ";
	subchargevector.push_back(NewTrackerXStripHits[x][1][u]);
      }
      //cout << endl;

      ReturnHitInfo.push_back(substripvector);
      ReturnHitInfo.push_back(subchargevector);
      ReducedXHits.push_back(XHits[x][0]);
      ReducedXHits.push_back(XHits[x][1]);
      
    }
  }

 
  delete g1;
  delete f1;
  
  //cout << "goung to Y?" << endl;

  for(int x = 0; x < YHits.size(); x++){
    //    //cout << YHits[x].size() << endl;
    if(YHits[x].size() != 2 && YHits[x].size()%2 == 0){//cluster with 1 hit (y and z coordinate)
      vector<float> avgres;      
      for(int h = 0; h < YHits[x].size()/2; h++){

	double Ytheo = intercept2 + slope2*YHits[x][1+(2*h)];
	double res = abs(YHits[x][0+(2*h)] - Ytheo);
	//cout << "residuals: " << res << endl;
	avgres.push_back(res);
	
      }
      int bestcluster = distance(avgres.begin(), min_element(avgres.begin(),avgres.end()));
      //YHits[x].erase(YHits[x].begin()+(2*bestcluster));
      //YHits[x].erase(YHits[x].begin()+(2*bestcluster)+1);

      vector<float> substripvector;
      vector<float> subchargevector;

      
      for(int u = 0; u < NewTrackerYStripHits[x][bestcluster*2].size(); u++){  
	//cout << NewTrackerYStripHits[x][bestcluster*2][u] << ", ";
	substripvector.push_back(NewTrackerYStripHits[x][bestcluster*2][u]);
      }
      //cout << endl;

      for(int u = 0; u < NewTrackerYStripHits[x][(bestcluster*2)+1].size(); u++){  
	//cout << NewTrackerYStripHits[x][(bestcluster*2)+1][u] << ", ";
	subchargevector.push_back(NewTrackerYStripHits[x][(bestcluster*2)+1][u]);
      }
      //cout << endl;

      
      ReturnHitInfo.push_back(substripvector);
      ReturnHitInfo.push_back(subchargevector);
      ReducedYHits.push_back(YHits[x][bestcluster*2]);
      ReducedYHits.push_back(YHits[x][(bestcluster*2)+1]);
      
    }
    
    else if(YHits[x].size() == 2){
      vector<float> substripvector;
      vector<float> subchargevector;

      
      for(int u = 0; u < NewTrackerYStripHits[x][0].size(); u++){  
	//cout << NewTrackerYStripHits[x][0][u] << ", ";
	substripvector.push_back(NewTrackerYStripHits[x][0][u]);
      }
      //cout << endl;

      for(int u = 0; u < NewTrackerYStripHits[x][1].size(); u++){  
	//cout << NewTrackerYStripHits[x][1][u] << ", ";
	subchargevector.push_back(NewTrackerYStripHits[x][1][u]);
      }
      //cout << endl;

      
      ReturnHitInfo.push_back(substripvector);
      ReturnHitInfo.push_back(subchargevector);
      ReducedYHits.push_back(YHits[x][0]);
      ReducedYHits.push_back(YHits[x][1]);
      
    }
  }

  delete g2;
  delete f2;

  //cout << "In function: " << endl;
  for(int y = 0; y < ReturnHitInfo.size(); y++){
    for(int j = 0; j < ReturnHitInfo[y].size(); j++){
      cout << ReturnHitInfo[y][j] << ", ";
    }
    cout << endl;
  }

  cout << "==============================================" << endl;


  vector<float> AngularHits;
  for(int y = 0; y < ReducedXHits.size(); y++){    
    cout << ReducedXHits[y]+LAGDOffsetX << ", " << ReducedXHits[y+1];
    y++;
    cout << endl;
  }

  for(int y = 0; y < ReducedYHits.size(); y++){    
    cout << ReducedYHits[y]+LAGDOffsetY << ", " << ReducedYHits[y+1];
    y++;
    cout << endl;
  }
  for(int y = 0; y < ReducedYHits.size(); y++){    
    cout << atan((ReducedYHits[y]+LAGDOffsetY)/(ReducedXHits[y]+LAGDOffsetX)) << ", " << sqrt((ReducedYHits[y]+LAGDOffsetY)*(ReducedYHits[y]+LAGDOffsetY) + (ReducedXHits[y]+LAGDOffsetX)*(ReducedXHits[y]+LAGDOffsetX));

    cout << endl;
    AngularHits.push_back(atan((ReducedYHits[y]+LAGDOffsetY)/(ReducedXHits[y]+LAGDOffsetX)));
    y++;
    AngularHits.push_back(ReducedXHits[y]);//for Z comp
  }
  
  vector<float> LAGDhits;
  vector<vector<float>> LAGDhitStripCharge;
  cout << LAGDmaxstrips.size() << endl;
  q=0;
  auto NumSubEventsLAGD = StripSep(LAGDmaxstrips, LAGDmaxcharges);
  for(int u = 0; u < NumSubEventsLAGD.size(); u++){
    auto num=0.0;
    auto den=0.0;
    vector<float> LAGDstrips;
    vector<float> LAGDcharges;
    for(int t = 0; t < NumSubEventsLAGD.at(u); t++){
      cout << LAGDmaxstrips.at(q) << ", " << endl;
      num += LAGDmaxstrips.at(q)*LAGDmaxcharges.at(q);
      den += LAGDmaxcharges.at(q);
      LAGDstrips.push_back(LAGDmaxstrips.at(q));
      LAGDcharges.push_back(LAGDmaxcharges.at(q));
      q++;
    }
    //if(num == 0){continue;}
    auto loc = num/den;
    //loc += shift4y;

    loc = loc*0.0041;
    cout << "LAGD loc: " << loc << endl;
    //AngularHits.push_back(loc);
    LAGDhits.push_back(loc);
    LAGDhits.push_back(828.2);
    LAGDhitStripCharge.push_back(LAGDstrips);
    LAGDstrips.clear();
    LAGDhitStripCharge.push_back(LAGDcharges);
    LAGDcharges.clear();    
  }

  
  for(int x = 0; x < AngularHits.size(); x++){
    cout << AngularHits[x] << " , " << AngularHits[x+1] << endl;
    x++;
  }
  for(int x = 0; x < LAGDhits.size(); x++){
    cout << LAGDhits[x] << " , " << LAGDhits[x+1] << endl;
    x++;
  }

  
  // TGraph* g3 = new TGraph();
  // int LAGDplotit=0;
  // for(int x = 0; x < AngularHits.size(); x++){
  //   g3->SetPoint(LAGDplotit, AngularHits[x+1], AngularHits[x]);
  //   LAGDplotit++;
  //   x++;
  // }    
  
  // auto testcanvas = new TCanvas("", "", 500, 500);
  // g3->Draw("A*");
  // testcanvas->Draw();
  

  TGraph* g3 = new TGraph();
  int LAGDplotit=0;
  for(int x = 0; x < AngularHits.size(); x++){//only trackers
    g3->SetPoint(LAGDplotit, AngularHits[x+1], AngularHits[x]);
    LAGDplotit++;
    x++;
  }    
  TF1* f3 = new TF1("line3","pol1",0,1600);
  g3->Fit("line3","R");
  double intercept3 = f3->GetParameter(0);
  double slope3     = f3->GetParameter(1);

  vector<float> avgres;
  for(int h = 0; h < LAGDhits.size(); h++){
    
    double Xtheo = intercept3 + slope3*LAGDhits[1+(2*h)];
    double res = abs(LAGDhits[0+(2*h)] - Xtheo);
    cout << "residuals: " << res << endl;
    avgres.push_back(res);
    h++;
  }
  int bestcluster = distance(avgres.begin(), min_element(avgres.begin(),avgres.end()));

  cout << bestcluster << endl;

  vector<float> substripvector;
  vector<float> subchargevector;

  for(int u = 0; u < LAGDhitStripCharge[(bestcluster*2)].size(); u++){  
    //cout << NewTrackerYStripHits[x][(bestcluster*2)+1][u] << ", ";
    substripvector.push_back(LAGDhitStripCharge[(bestcluster*2)][u]);
  }
  for(int u = 0; u < LAGDhitStripCharge[(bestcluster*2)+1].size(); u++){  
    //cout << NewTrackerYStripHits[x][(bestcluster*2)+1][u] << ", ";
    subchargevector.push_back(LAGDhitStripCharge[(bestcluster*2)+1][u]);
  }
  //cout << endl;

      
  ReturnHitInfo.push_back(substripvector);
  ReturnHitInfo.push_back(subchargevector);

  for(int y = 0; y < ReturnHitInfo.size(); y++){
    for(int j = 0; j < ReturnHitInfo[y].size(); j++){
      cout << ReturnHitInfo[y][j] << ", ";
    }
    cout << endl;
  }

  
  return ReturnHitInfo;
}


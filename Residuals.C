vector<float> Residuals(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> BadEvents){
  
  //vector format: (x1, y1, z1, x2, y2, z2,...)
  //coordinate, eventnum (x1)

  /*
  bool firsttime=true;
  if(Fitparams.empty() == 0){firsttime=false;}
  cout << firsttime << endl;
  float MeanxSlope=0;
  float MeanySlope=0;
  float Meanxyint=0;
  float Meanyyint=0;
  */
  vector<float> Res;

  
  if(Tracker1HitlocVec.empty() || Tracker2HitlocVec.empty() || Tracker3HitlocVec.empty() || Tracker4HitlocVec.empty())
    return Res;

  /*
  //if(firsttime){
    TH1F* xslopeplot = new TH1F("xz slopes","xz slopes from fit",400,-0.01,0.01); xslopeplot->SetXTitle("Slope"); xslopeplot->SetYTitle("Frequency");xslopeplot->SetLabelSize(0.045,"XY");xslopeplot->SetTitleSize(0.045,"XY");
    TH1F* yslopeplot = new TH1F("yz slopes","yz slopes from fit",400, -0.01,0.01); yslopeplot->SetXTitle("Slope"); yslopeplot->SetYTitle("Frequency");yslopeplot->SetLabelSize(0.045,"XY");yslopeplot->SetTitleSize(0.045,"XY");
    
    TH1F* xz_yintplot = new TH1F("xz Y intercepts","xz y intercepts from fit",100,-70,70); xz_yintplot->SetXTitle("y intercepts"); xz_yintplot->SetYTitle("Frequency"); xz_yintplot->SetLabelSize(0.045,"XY"); xz_yintplot->SetTitleSize(0.045,"XY");
    TH1F* yz_yintplot = new TH1F("yz Y intercepts","yz y intercepts from fit",100,-70, 70); yz_yintplot->SetXTitle("y intercepts"); yz_yintplot->SetYTitle("Frequency");yz_yintplot->SetLabelSize(0.045,"XY");yz_yintplot->SetTitleSize(0.045,"XY");
    //}

    */
  /*
  TH1F* Calculated2xcl = new TH1F("2X","Tracker2 X coordinate from fit",100,-50,50); Calculated2xcl->SetXTitle("Calculated 2x coordinate [mm]"); Calculated2xcl->SetYTitle("Frequency");Calculated2xcl->SetLabelSize(0.045,"XY");Calculated2xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated2ycl = new TH1F("2Y","Tracker2 Y coordinate from fit",100,-50,50); Calculated2ycl->SetXTitle("Calculated 2y coordinate [mm]"); Calculated2ycl->SetYTitle("Frequency");Calculated2ycl->SetLabelSize(0.045,"XY");Calculated2ycl->SetTitleSize(0.045,"XY");  
  TH1F* Calculated1xcl = new TH1F("1X","Tracker1 X coordinate from fit",100,-50,50); Calculated1xcl->SetXTitle("Calculated 1x coordinate [mm]"); Calculated1xcl->SetYTitle("Frequency");Calculated1xcl->SetLabelSize(0.045,"XY");Calculated1xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated1ycl = new TH1F("1Y","Tracker1 Y coordinate from fit",100,-50,50); Calculated1ycl->SetXTitle("Calculated 1y coordinate [mm]"); Calculated1ycl->SetYTitle("Frequency");Calculated1ycl->SetLabelSize(0.045,"XY");Calculated1ycl->SetTitleSize(0.045,"XY");  
  TH1F* Calculated3xcl = new TH1F("3X","Tracker3 X coordinate from fit",100,-50,50); Calculated3xcl->SetXTitle("Calculated 3x coordinate [mm]"); Calculated3xcl->SetYTitle("Frequency");Calculated3xcl->SetLabelSize(0.045,"XY");Calculated3xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated3ycl = new TH1F("3Y","Tracker3 Y coordinate from fit",100,-50,50); Calculated3ycl->SetXTitle("Calculated 3y coordinate [mm]"); Calculated3ycl->SetYTitle("Frequency");Calculated3ycl->SetLabelSize(0.045,"XY");Calculated3ycl->SetTitleSize(0.045,"XY");
  TH1F* Calculated4xcl = new TH1F("4X","Tracker4 X coordinate from fit",100,-50,50); Calculated4xcl->SetXTitle("Calculated 4x coordinate [mm]"); Calculated4xcl->SetYTitle("Frequency");Calculated4xcl->SetLabelSize(0.045,"XY");Calculated4xcl->SetTitleSize(0.045,"XY");
  TH1F* Calculated4ycl = new TH1F("4Y","Tracker4 Y coordinate from fit",100,-50,50); Calculated4ycl->SetXTitle("Calculated 4y coordinate [mm]"); Calculated4ycl->SetYTitle("Frequency");Calculated4ycl->SetLabelSize(0.045,"XY");Calculated4ycl->SetTitleSize(0.045,"XY");
  */
  TGraph* OGVals = new TGraph();
  TGraph* CalcVals = new TGraph();
  TGraph* ResVals = new TGraph();

  TGraph* XResvsY = new TGraph(2);
  TGraph* YResvsX = new TGraph(2);
  
  //vector<float> BadEvents;
  //auto *xTracks = new TMultiGraph();
  //auto *yTracks = new TMultiGraph();
  for(unsigned int i=0; i< Tracker1HitlocVec.size(); i++){
    //x1, y1, z1, x2   
    if(find(BadEvents.begin(), BadEvents.end(), i/3) == BadEvents.end()){
      
      //cout << i/3 << endl;
      TGraph* gx = new TGraph();
      TF1* fx = new TF1("line1","pol1");
      TGraph* gy = new TGraph();
      TF1* fy = new TF1("line2","pol1");   
    
  
      //cout << "A" << endl;
      gx->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i]);
      gx->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i]);
      gx->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i]);
      gx->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i]);
    

      gy->SetPoint(0, Tracker1HitlocVec[i+2], Tracker1HitlocVec[i+1]);
      gy->SetPoint(1, Tracker2HitlocVec[i+2], Tracker2HitlocVec[i+1]);
      gy->SetPoint(2, Tracker3HitlocVec[i+2], Tracker3HitlocVec[i+1]);
      gy->SetPoint(3, Tracker4HitlocVec[i+2], Tracker4HitlocVec[i+1]);
  
   
      //return;
    
      //if (verbose)
      //  cout<<"vPos_g1xcl["<<i<<"] = "<<vPos_g1xcl[i]<<"\tvPos_g2xcl["<<i<<"] = "<<vPos_g2xcl[i]<<"\tvPos_g3xcl["<<i<<"] = "<<vPos_g3xcl[i]<<endl;
	    
      gx->Fit("line1","Q");
    
      gy->Fit("line2","Q");
   
      //gy->Draw("A*");
      gy->SetTitle("Y Coordinates of Hit");
      gy->GetXaxis()->SetTitle("Z coordinate");
      gy->GetYaxis()->SetTitle("Y coordinate");
      //break;
      float interceptx = fx->GetParameter(0);
      //cout << "x Yintercepts: " << interceptx << endl;    
      float slopex     = fx->GetParameter(1);
    
      float intercepty = fy->GetParameter(0);
      //cout << "y Yintercepts: " << intercepty << endl;    
      float slopey = fy->GetParameter(1);
    
      //a bunch of stuff from a different way to do fitting 
      //if(firsttime){
      /*
	yslopeplot->Fill(slopey);
	xz_yintplot->Fill(interceptx);    
	xslopeplot->Fill(slopex);
	yz_yintplot->Fill(intercepty);
	//}
	*/
    
      /*
	if(i <= 2){      
	gx->GetYaxis()->SetRangeUser(5, 35);
	//gx->Draw("A*");
	//float xname = i/3;
	xTracks->Add(gx);
	gy->GetYaxis()->SetRangeUser(25, 55);
	//gy->Draw("A*");
	yTracks->Add(gy);     
	}    
      */
      /*
	if(firsttime){
	TCanvas* Quickplot = new TCanvas("Yea", "Hists of x and y fit parameters", 1000, 1000);
	Quickplot->Divide(2,2);
	Quickplot->cd(1);
	xslopeplot->Draw();
	Quickplot->cd(2);
	yslopeplot->Draw();
	Quickplot->cd(3);
	xz_yintplot->Draw();
	Quickplot->cd(4);
	yz_yintplot->Draw();
	
	Quickplot->SaveAs("Fit_Parameters.png");
	}
	
  
	MeanxSlope = xslopeplot->GetMean();
	cout << "  MeanxSlope: " <<   MeanxSlope << endl;
	Fitparams.push_back(MeanxSlope);
	MeanySlope = yslopeplot->GetMean();
	cout << "  MeanySlope: " <<   MeanySlope << endl;
	Fitparams.push_back(MeanySlope);
	Meanxyint = xz_yintplot->GetMean();
	cout << "  Meanxyint: " <<   Meanxyint << endl;
	Fitparams.push_back(Meanxyint);
	Meanyyint = yz_yintplot->GetMean();
	cout << "  Meanyyint: " <<   Meanyyint << endl;
	Fitparams.push_back(Meanyyint);
      */
    
    
      float Measured_Tracker1x = interceptx + slopex*Tracker1HitlocVec[i+2];
      //cout << "Measured 1x: " << Measured_Tracker1x << endl;
      //Calculated1xcl->Fill(Measured_Tracker1x);
      float Tracker1xRes = Measured_Tracker1x - Tracker1HitlocVec[i];    
      //cout << "1X Coord:: " << Measured_Tracker1x << " - " <<  Tracker1HitlocVec[i] << " = " << Measured_Tracker1x - Tracker1HitlocVec[i] << endl;
      Res.push_back(Tracker1xRes);
    
      float Measured_Tracker1y = intercepty + slopey*Tracker1HitlocVec[i+2];
      ////Calculated1ycl->Fill(Measured_Tracker1y);
      float Tracker1yRes = Measured_Tracker1y - Tracker1HitlocVec[i+1];
      Res.push_back(Tracker1yRes);

   
    
      float Measured_Tracker2x = interceptx + slopex*Tracker2HitlocVec[i+2];
      //Calculated2xcl->Fill(Measured_Tracker2x);
      float Tracker2xRes = Measured_Tracker2x - Tracker2HitlocVec[i];
       
    
      //cout << "2X Coord:: " << Measured_Tracker2x << " - " <<  Tracker2HitlocVec[i] << " = " << Measured_Tracker2x - Tracker2HitlocVec[i] << endl;
      Res.push_back(Tracker2xRes);
      float Measured_Tracker2y = intercepty + slopey*Tracker2HitlocVec[i+2];
      //Calculated2ycl->Fill(Measured_Tracker2y);
      float Tracker2yRes = Measured_Tracker2y - Tracker2HitlocVec[i+1];  
      //cout << "2Y Coord:: " << Measured_Tracker2y << " - " <<  Tracker2HitlocVec[i+1] << " = " << Measured_Tracker2y - Tracker2HitlocVec[i+1] << endl;
      Res.push_back(Tracker2yRes);

      /*    
	    if(Tracker2yRes > 0.275 || Tracker2yRes <= 0.125){
	    Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i);Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i+1);Tracker1HitlocVec.erase(Tracker1HitlocVec.begin()+i+2);
	    Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i);Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i+1);Tracker2HitlocVec.erase(Tracker2HitlocVec.begin()+i+2);
	    Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i);Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i+1);Tracker3HitlocVec.erase(Tracker3HitlocVec.begin()+i+2);
	    Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i);Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i+1);Tracker4HitlocVec.erase(Tracker4HitlocVec.begin()+i+2);
	    i++;
	    i++;
	    continue;
	    }
      */
      /*
      if(i < 30){
	OGVals->SetPoint(i+1, i/3, Tracker2HitlocVec[i+1]);
	CalcVals->SetPoint(i+1, i/3, Measured_Tracker2y);
	ResVals->SetPoint(i+1, i/3, Tracker2yRes);
      
	//cout << "2y Coord " << i << ":: " << Measured_Tracker2y << " - " <<  Tracker2HitlocVec[i+1] << " = " << Tracker2yRes << endl;
      }
      */
    
      float Measured_Tracker3x = interceptx + slopex*Tracker3HitlocVec[i+2];
      //Calculated3xcl->Fill(Measured_Tracker3x);
      float Tracker3xRes = Measured_Tracker3x - Tracker3HitlocVec[i];
      Res.push_back(Tracker3xRes);
    
      float Measured_Tracker3y = intercepty + slopey*Tracker3HitlocVec[i+2];
      //Calculated3ycl->Fill(Measured_Tracker3y);
      float Tracker3yRes = Measured_Tracker3y - Tracker3HitlocVec[i+1];
      Res.push_back(Tracker3yRes);
       
         
      float Measured_Tracker4x = interceptx + slopex*Tracker4HitlocVec[i+2];
      //Calculated4xcl->Fill(Measured_Tracker4x);
      float Tracker4xRes = Measured_Tracker4x - Tracker4HitlocVec[i];
      Res.push_back(Tracker4xRes);
      float Measured_Tracker4y = intercepty + slopey*Tracker4HitlocVec[i+2];
      //Calculated4ycl->Fill(Measured_Tracker4y);
      float Tracker4yRes = Measured_Tracker4y - Tracker4HitlocVec[i+1];
      Res.push_back(Tracker4yRes);
    
      //XResvsY->SetPoint(i+1, Tracker4HitlocVec[i+1], Tracker4xRes);
      //YResvsX->SetPoint(i+1, Tracker4HitlocVec[i], Tracker4yRes);
    

      delete gx;
      delete fx;
      delete gy;
      delete fy;
    }
    i++;
    i++;
  }


  
  /*
  for(auto i = BadEvents.begin(); i != BadEvents.end(); i++){
    cout << *i << endl;
  }
  */
  //return BadEvents;
  
  
  //char Canvasname[30];sprintf(Canvasname,"Canvas_%i",i);
  //char Filename[30];sprintf(Filename,"Hits_%i.png",i);
  /*
  TCanvas* Locs = new TCanvas("Canvasname", "X and Y residuals vs X and Y", 1000, 500);
  Locs->Divide(2,1);
  Locs->Draw();
  Locs->cd(1);
  cout << "Yes" << endl;
  XResvsY->SetTitle("X Residual vs Y coordinate");
  XResvsY->GetXaxis()->SetTitle("Y coordinate [mm]");
  XResvsY->GetYaxis()->SetTitle("X residual [mm]");
  XResvsY->Draw("A*");
  cout << "no" << endl;
  Locs->cd(2);
  YResvsX->SetTitle("Y Residual vs X coordinate");
  YResvsX->GetXaxis()->SetTitle("X coordinate [mm]");
  YResvsX->GetYaxis()->SetTitle("Y residual [mm]");
  YResvsX->Draw("A*");
  gPad->Update();
  cout << "Before Draw" << endl;
  
  //cout << "nMAybe" << endl;
  Locs->Draw();
  //Locs->SaveAs(".png");     
  */
  
  /*
  TCanvas* Locs = new TCanvas("Canvasname", "Tracks in 1 File", 1000, 500);
  Locs->Divide(2,1);
  Locs->Draw("A");
  Locs->cd(1);
  cout << "Yes" << endl;
  xTracks->Draw("A");
  cout << "no" << endl;
  Locs->cd(2);
  yTracks->Draw("A");
  gPad->Update();
  cout << "Before Draw" << endl;
  
  cout << "nMAybe" << endl;
  
  Locs->SaveAs("Tracks.png");     
  */
  
  //cout << "C" << endl;

  /*
  TCanvas* slops = new TCanvas("slops", "yes", 1000, 500);
  slops->Divide(2,1);
  slops->cd(1);
  xslopeplot->Draw("A*");
  slops->cd(2);
  yslopeplot->Draw("A*");

  slops->Draw();

  slops->SaveAs("SLopes.png");
  */

  /*
  TCanvas* slops = new TCanvas("slops", "", 2000, 1000);
  slops->Divide(4,2);
  slops->cd(1);
  Calculated1xcl->Draw();
  slops->cd(5);
  Calculated1ycl->Draw();
  slops->cd(2);
  Calculated2xcl->Draw();
  slops->cd(6);
  Calculated2ycl->Draw();
  slops->cd(3);
  Calculated3xcl->Draw();
  slops->cd(7);
  Calculated3ycl->Draw();
  slops->cd(4);
  Calculated4xcl->Draw();
  slops->cd(8);
  Calculated4ycl->Draw();

  slops->SaveAs("CalculatedXYxoordinates.png");
  
  slops->Draw();
  */
  
  /*
  TCanvas* slops = new TCanvas("slops", "", 500, 500);
  OGVals->GetYaxis()->SetRangeUser(-10, 90);
  //OGVals->GetXaxis()->SetRangeUser(0, 5000);  
  OGVals->SetMarkerStyle(20);
  OGVals->Draw("AP");
  CalcVals->GetYaxis()->SetRangeUser(-10, 90);
  //CalcVals->GetXaxis()->SetRangeUser(0, 5000);
  CalcVals->SetMarkerColor(3);
  CalcVals->SetMarkerStyle(21);
  CalcVals->Draw("PSame");
  ResVals->GetYaxis()->SetRangeUser(-10, 90);
  //ResVals->GetXaxis()->SetRangeUser(0, 5000);
  ResVals->SetMarkerColor(4);
  ResVals->SetMarkerStyle(22);
  ResVals->Draw("PSame");

  auto legend = new TLegend(0.7, 0.6, 0.9, 0.9);
  legend->AddEntry(OGVals, "Original Values", "P");
  legend->AddEntry(CalcVals, "Calculated Values", "P");
  legend->AddEntry(ResVals, "Residuals", "P");
  */
  //legend->Draw();
  //slops->SaveAs("All1Yvalues.png");
  
  //slops->Draw();
  
  
  return Res;//returns vector of the form (resx11, resy11, (other 3 trackers x and y) ,  resx21, resy21, ...) residual of tracker 1x event 1, ..
}

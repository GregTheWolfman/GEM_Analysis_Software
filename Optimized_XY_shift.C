#include "TGaxis.h"

vector<float> Optimized_XY_shift(vector<float> Tracker1HitlocVec, vector<float> Tracker2HitlocVec, vector<float> Tracker3HitlocVec, vector<float> Tracker4HitlocVec, vector<float> OptimizedOffests, vector<float> BadEvents, bool plots){//find optimized XY offsets 
  //std::fixed << std::setprecision(2);
  
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
    
    int resbounds = 2;
    float bincount = 80;
    TH1F* h_residual_Tracker2x = new TH1F(nameRes2X,"Tracker 2 x-Residual", bincount,-resbounds,resbounds); h_residual_Tracker2x->SetXTitle("Residual [mm]"); h_residual_Tracker2x->SetYTitle("Counts/0.025mm");h_residual_Tracker2x->SetLabelSize(0.045,"XY");h_residual_Tracker2x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker2y = new TH1F(nameRes2Y,"", bincount,-resbounds,resbounds); h_residual_Tracker2y->SetXTitle("Residual [mm]"); h_residual_Tracker2y->SetYTitle("Counts/0.025mm");h_residual_Tracker2y->SetLabelSize(0.045,"XY");h_residual_Tracker2y->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker3x = new TH1F(nameRes3X,"Tracker 3 x-Residual", bincount,-resbounds,resbounds); h_residual_Tracker3x->SetXTitle("Residual [mm]"); h_residual_Tracker3x->SetYTitle("Counts/0.01mm");h_residual_Tracker3x->SetLabelSize(0.045,"XY");h_residual_Tracker3x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker3y = new TH1F(nameRes3Y,"", bincount,-resbounds,resbounds); h_residual_Tracker3y->SetXTitle("Residual [mm]"); h_residual_Tracker3y->SetYTitle("Counts/0.025mm");h_residual_Tracker3y->SetLabelSize(0.045,"XY");h_residual_Tracker3y->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker1x = new TH1F(nameRes1X,"Tracker 1 x-Residual", bincount,-resbounds,resbounds); h_residual_Tracker1x->SetXTitle("Residual [mm]"); h_residual_Tracker1x->SetYTitle("Counts/0.01mm");h_residual_Tracker1x->SetLabelSize(0.045,"XY");h_residual_Tracker1x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker1y = new TH1F(nameRes1Y,"", bincount,-resbounds,resbounds); h_residual_Tracker1y->SetXTitle("Residual [mm]"); h_residual_Tracker1y->SetYTitle("Counts/0.025mm");h_residual_Tracker1y->SetLabelSize(0.045,"XY");h_residual_Tracker1y->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker4x = new TH1F(nameRes4X,"Tracker 4 x-Residual", bincount,-resbounds,resbounds); h_residual_Tracker4x->SetXTitle("Residual [mm]"); h_residual_Tracker4x->SetYTitle("Counts/0.01mm");h_residual_Tracker4x->SetLabelSize(0.045,"XY");h_residual_Tracker4x->SetTitleSize(0.045,"XY");
    TH1F* h_residual_Tracker4y = new TH1F(nameRes4Y,"", bincount,-resbounds,resbounds); h_residual_Tracker4y->SetXTitle("Residual [mm]"); h_residual_Tracker4y->SetYTitle("Counts/0.025mm");h_residual_Tracker4y->SetLabelSize(0.045,"XY");h_residual_Tracker4y->SetTitleSize(0.045,"XY");

    
    Tracker1HitmapPerRotation[Iterator] = new TH2F(nameRes1X, "",400, -200, 200, 400, -200, 200);
    
    Tracker3HitmapPerRotation[Iterator] = new TH2F(nameRot3, "", 100, -50, 50, 100, -50, 50);
    Tracker2HitmapPerRotation[Iterator] = new TH2F(nameRot2, "", 400, -200, 200, 400, -200, 200);
    Tracker4HitmapPerRotation[Iterator] = new TH2F(nameRot4, "", 100, -50, 50, 100, -50, 50);
    
    //TH1F* DiffBtwnT12Y = new TH1F(nameRes2X,"Difference between Trackers 3 and 4 Y coordinate", 100,-5,5); DiffBtwnT12Y->SetXTitle("Difference between Trackers 3 and 4 Y coordinate [mm]"); DiffBtwnT12Y->SetYTitle("Counts/0.01mm");DiffBtwnT12Y->SetLabelSize(0.045,"XY");DiffBtwnT12Y->SetTitleSize(0.045,"XY");
    /*
    cout << "Tracker 1x shifted by: " << Shift_T1x << endl;
    Shift1XPerIteration->SetPoint(Iterator-1, Iterator, Shift_T1x);
    cout << "Tracker 2x shifted by: " << Shift_T2x << endl;
    Shift2XPerIteration->SetPoint(Iterator-1, Iterator, Shift_T2x);
    */
    cout << "Shift 1x: " << Shift_T1x << endl;
    cout << "Shift 1y: " << Shift_T1y << endl;
    cout << "Shift 2x: " << Shift_T2x << endl;
    cout << "Shift 2y: " << Shift_T2y << endl;
    cout << "Shift 3x: " << Shift_T3x << endl;
    cout << "Shift 3y: " << Shift_T3y << endl;
    cout << "Shift 4x: " << Shift_T4x << endl;
    cout << "Shift 4y: " << Shift_T4y << endl;
    
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

      Tracker2HitmapPerRotation[Iterator]->Fill(Tracker2HitlocVec[i-1]+80, Tracker2HitlocVec[i]);
      Tracker1HitmapPerRotation[Iterator]->Fill(Tracker1HitlocVec[i-1]+80, Tracker1HitlocVec[i]);
      
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
    /*
    auto T1xDist = h_Pos_g1xcl->GetMean()+80;
    cout << "X coordinate: " << T1xDist << endl;
    auto T2xDist = h_Pos_g2xcl->GetMean()+80;
    auto T3xDist = h_Pos_g3xcl->GetMean()+80;
    auto T4xDist = h_Pos_g4xcl->GetMean()+80;

    auto T1yDist = h_Pos_g1ycl->GetMean();
    cout << "Y coordinate: " << T1yDist << endl;
    auto T2yDist = h_Pos_g2ycl->GetMean();
    auto T3yDist = h_Pos_g3ycl->GetMean();
    auto T4yDist = h_Pos_g4ycl->GetMean();

    auto T1R = sqrt(T1xDist*T1xDist + T1yDist*T1yDist);
    auto T2R = sqrt(T2xDist*T2xDist + T2yDist*T2yDist);
    auto T3R = sqrt(T3xDist*T3xDist + T3yDist*T3yDist);
    auto T4R = sqrt(T4xDist*T4xDist + T4yDist*T4yDist);
    
    cout << T1R << " mm" << endl;
    */
    
    
    
    auto NewRes = Residuals(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec, BadEvents);

    if(NewRes.size() < Tracker1HitlocVec.size()){
      
      break;
    }
    
    //return NewRes;
    
    auto Chisquares = ChiSquared(Tracker1HitlocVec, Tracker2HitlocVec, Tracker3HitlocVec, Tracker4HitlocVec, BadEvents);

        
    xChi2PerIteration->SetPoint(Iterator-1, Iterator, Chisquares[0]);
    yChi2PerIteration->SetPoint(Iterator-1, Iterator, Chisquares[1]);
  
    //cout << "Size of Residual vector: " << NewRes.size() << endl;
    
    for(unsigned int i=0; i< NewRes.size(); i++){//Put all residuals into plots

      h_residual_Tracker1x->Fill(NewRes[i]);
      //cout << NewRes[i] << endl;
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

    cout << h_residual_Tracker1x->GetMean() << endl;
    if(Iterator == 1){
      h_residual_Tracker1x->Draw();
    }
    
    if(h_residual_Tracker1x->GetEntries() == 0){cout << "h_residual_Tracker1x big empty dawg; increase or decrease the bounds :D" << endl;h_residual_Tracker1x->Draw();} 
    myValues = I2GFmainLoop(h_residual_Tracker1x,1,5,1);                           //converges!	  
    mean_Tracker1xcoordRes = myValues.mean; width_Tracker1xcoordRes=myValues.sigma;
    auto mean_err_Tracker1xcoordRes = myValues.mean_err; auto width_err_Tracker1xcoordRes=myValues.sigma_err;
    auto mean_Tracker1xcoordRes2 = myValues.mean2; auto width_Tracker1xcoordRes2=myValues.sigma2;
    cout << "1st mean: " << mean_Tracker1xcoordRes << " +\- " << width_Tracker1xcoordRes << ", Second mean: " << mean_Tracker1xcoordRes2 << " +\- " << width_Tracker1xcoordRes2 << endl;
    
    cout << "Tracker1x mean via dubgaus: " << mean_Tracker1xcoordRes << endl;
    cout << "Tracker1x mean via GetMean(): " << h_residual_Tracker1x->GetMean() << endl;

    if(abs(mean_Tracker1xcoordRes) > 50){
       auto t = new TCanvas("", "", 500, 500);
      h_residual_Tracker1x->Draw();
      return OptimizedXYOffsets;
    }
    
    if(h_residual_Tracker1y->GetEntries() == 0){cout << "h_residual_Tracker1y big empty dawg; increase or decrease the bounds :D" << endl;h_residual_Tracker1y->Draw();}
    myValues = I2GFmainLoop(h_residual_Tracker1y,1,5,1);	                        
    mean_Tracker1ycoordRes = myValues.mean; width_Tracker1ycoordRes=myValues.sigma;

    if(h_residual_Tracker2x->GetEntries() == 0){cout << "h_residual_Tracker2x big empty dawg; increase or decrease the bounds :D" << endl;h_residual_Tracker2x->Draw();}
    myValues = I2GFmainLoop(h_residual_Tracker2x,1,5,1);                               //converges!	  
    mean_Tracker2xcoordRes = myValues.mean; width_Tracker2xcoordRes=myValues.sigma;

    if(h_residual_Tracker2y->GetEntries() == 0){cout << "h_residual_Tracker2y big empty dawg; increase or decrease the bounds :D" << endl;h_residual_Tracker2y->Draw();}
    myValues = I2GFmainLoop(h_residual_Tracker2y,1,5,1);	                            //breaks :(... im sensing a pattern...
    mean_Tracker2ycoordRes = myValues.mean; width_Tracker2ycoordRes=myValues.sigma;

    if(h_residual_Tracker3x->GetEntries() == 0){cout << "h_residual_Tracker3x big empty dawg; increase or decrease the bounds :D" << endl;h_residual_Tracker3x->Draw();}
    myValues = I2GFmainLoop(h_residual_Tracker3x,1,5,1);
    mean_Tracker3xcoordRes = myValues.mean;  width_Tracker3xcoordRes=myValues.sigma;	

    if(h_residual_Tracker3y->GetEntries() == 0){cout << "h_residual_Tracker3y big empty dawg; increase or decrease the bounds :D" << endl;h_residual_Tracker3y->Draw();}
    myValues = I2GFmainLoop(h_residual_Tracker3y,1,5,1);	  
    mean_Tracker3ycoordRes = myValues.mean; width_Tracker3ycoordRes=myValues.sigma;

    if(h_residual_Tracker4x->GetEntries() == 0){cout << "h_residual_Tracker4x big empty dawg; increase or decrease the bounds :D" << endl;h_residual_Tracker4x->Draw();}
    myValues = I2GFmainLoop(h_residual_Tracker4x,1,5,1);
    mean_Tracker4xcoordRes = myValues.mean;  width_Tracker4xcoordRes=myValues.sigma;

    if(h_residual_Tracker4y->GetEntries() == 0){cout << "h_residual_Tracker4y big empty dawg; increase or decrease the bounds :D" << endl;h_residual_Tracker4y->Draw();}
    myValues = I2GFmainLoop(h_residual_Tracker4y,1,5,1);	  
    mean_Tracker4ycoordRes = myValues.mean; width_Tracker4ycoordRes=myValues.sigma;
    
	
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual1XPerIteration->SetPoint(Iterator-1, Iterator, 1000*mean_Tracker1xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual1YPerIteration->SetPoint(Iterator-1, Iterator, 1000*mean_Tracker1ycoordRes);
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual2XPerIteration->SetPoint(Iterator-1, Iterator, 1000*mean_Tracker2xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual2YPerIteration->SetPoint(Iterator-1, Iterator, 1000*mean_Tracker2ycoordRes);
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual3XPerIteration->SetPoint(Iterator-1, Iterator, 1000*mean_Tracker3xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual3YPerIteration->SetPoint(Iterator-1, Iterator, 1000*mean_Tracker3ycoordRes);
    //cout << "mean_Tracker1xcoordRes at iterator " << Iterator << ": " << mean_Tracker1xcoordRes << endl;
    Residual4XPerIteration->SetPoint(Iterator-1, Iterator, 1000*mean_Tracker4xcoordRes);
    //cout << "mean_Tracker1ycoordRes at iterator " << Iterator << ": " << mean_Tracker1ycoordRes << endl;
    Residual4YPerIteration->SetPoint(Iterator-1, Iterator, 1000*mean_Tracker4ycoordRes);

	
    if((mean_Tracker1xcoordRes >= -0.0005 && mean_Tracker1xcoordRes <= 0.0005) && (mean_Tracker1ycoordRes >= -0.0005 && mean_Tracker1ycoordRes <= 0.0005) && (mean_Tracker2xcoordRes >= -0.0005 && mean_Tracker2xcoordRes <= 0.0005) && (mean_Tracker2ycoordRes >= -0.0005 && mean_Tracker2ycoordRes <= 0.0005) && (mean_Tracker3xcoordRes >= -0.0005 && mean_Tracker3xcoordRes <= 0.0005) && (mean_Tracker3ycoordRes >= -0.0005 && mean_Tracker3ycoordRes <= 0.0005) && (mean_Tracker4xcoordRes >= -0.0005 && mean_Tracker4xcoordRes <= 0.0005) && (mean_Tracker4ycoordRes >= -0.0005 && mean_Tracker4ycoordRes <= 0.0005)){ 
      
      OptimizedXYOffsets.push_back(Total1Xshift); OptimizedXYOffsets.push_back(Total1Yshift);
      OptimizedXYOffsets.push_back(Total2Xshift); OptimizedXYOffsets.push_back(Total2Yshift);
      OptimizedXYOffsets.push_back(Total3Xshift); OptimizedXYOffsets.push_back(Total3Yshift);
      OptimizedXYOffsets.push_back(Total4Xshift); OptimizedXYOffsets.push_back(Total4Yshift);

      cout << "Pre Aligment first X value: " << prealignedX << endl;
      cout << "Pre Aligment first Y value: " << prealignedY << endl;
           
      cout << "PostAligment first X value: " << Tracker1HitlocVec[0] << endl;
      cout << "PostAligment first Y value: " << Tracker1HitlocVec[1] << endl;
    
      cout<<"find it...iterating "<< Iterator <<" times."<<endl;

      cout << "Tracker 1x Residual Width: " << double(int(100000*width_Tracker1xcoordRes))/100 << " um (" << double(int(100000*h_residual_Tracker1x->GetStdDev()))/100 << ")" << endl;
      cout << "Tracker 1x Residual Width error: " << double(int(100000*width_err_Tracker1xcoordRes))/100 << " um" << endl;

      cout << "Tracker 1x Residual mean: " << double(int(100000*mean_Tracker1xcoordRes))/100 << " um (" << double(int(100000*h_residual_Tracker1x->GetMean()))/100 << ")" << endl;
      cout << "Tracker 1x Residual mean error: " << double(int(100000*mean_err_Tracker1xcoordRes))/100 << " um" << endl;
      
      cout << "Tracker 1y Residual Width: " << 1000*width_Tracker1ycoordRes << " um" << endl;
      cout << "Tracker 2x Residual Width: " << 1000*width_Tracker2xcoordRes << " um" << endl;
      cout << "Tracker 2y Residual Width: " << 1000*width_Tracker2ycoordRes << " um" << endl;
      cout << "Tracker 3x Residual Width: " << 1000*width_Tracker3xcoordRes << " um" << endl;
      cout << "Tracker 3y Residual Width: " << 1000*width_Tracker3ycoordRes << " um" << endl;
      cout << "Tracker 4x Residual Width: " << 1000*width_Tracker4xcoordRes << " um" << endl;
      cout << "Tracker 4y Residual Width: " << 1000*width_Tracker4ycoordRes << " um" << endl;

      
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
	Residual1XPerIteration->SetTitle("Average Tracker X and Y coordinate Residual per Iteration");
	Residual1XPerIteration->SetMarkerStyle(20);
	Residual1XPerIteration->GetYaxis()->SetRangeUser(-150, 150);
	Residual1XPerIteration->Draw("AP");
	Residual1YPerIteration->SetMarkerStyle(24);
	Residual1YPerIteration->GetYaxis()->SetRangeUser(-150, 150);
	Residual1YPerIteration->Draw("Psame");
	Residual2XPerIteration->SetMarkerStyle(21);
	Residual2XPerIteration->GetYaxis()->SetRangeUser(-150, 150);
	Residual2XPerIteration->Draw("Psame");
	Residual2YPerIteration->SetMarkerStyle(25);
	Residual2YPerIteration->GetYaxis()->SetRangeUser(-150, 150);
	Residual2YPerIteration->Draw("Psame");
	Residual3XPerIteration->SetMarkerStyle(22);	  
	Residual3XPerIteration->GetYaxis()->SetRangeUser(-150, 150);
	Residual3XPerIteration->Draw("Psame");
	Residual3YPerIteration->SetMarkerStyle(26);
	Residual3YPerIteration->GetYaxis()->SetRangeUser(-150, 150);
	Residual3YPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(23);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-150, 150);
	Residual4XPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(27);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-150, 150);
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

	
	TCanvas* TrackerHits = new TCanvas(name2X, "Cartesian Alignment Residuals", 2000, 1000);
	TrackerHits->Divide(4,2);
	TrackerHits->SetRightMargin(TrackerHits->PixeltoX(0.01));

	gStyle->SetOptFit();
	gStyle->SetStatX(0.45);
	gStyle->SetStatW(0.18);	
	gStyle->SetStatY(0.85);
	
	float rangevals = 0.5;
	TrackerHits->cd(1);
	h_residual_Tracker1x->SetTitle("Tracker1x residual");
	//h_residual_Tracker1x->SetTitleOffset(0.5);	
	h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker1x->Draw();
	//gStyle->SetOptStat(0);

	//auto T1xRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker1x->GetMaximum()),rangevals,1.05*(h_residual_Tracker1x->GetMaximum()),-rangevals/(T1R/1000),rangevals/(T1R/1000),510,"-");	
	//T1xRaxis->SetTitle("[mRad]");
	//T1xRaxis->Draw();
	
	/*
	string T1xlegendentry = "Fit Width: ";
	string T1xlegendentry2 = "Fit Mean: ";
	T1xlegendentry += Form("%.2f",1000*width_Tracker1xcoordRes);
	T1xlegendentry += " um";
	char T1xcharlegendentry[T1xlegendentry.size()];
	strcpy(T1xcharlegendentry, T1xlegendentry.c_str());

	T1xlegendentry2 += Form("%.2f",1000*mean_Tracker1xcoordRes);
	T1xlegendentry2 += " um";
	char T1xcharlegendentry2[T1xlegendentry2.size()];
	strcpy(T1xcharlegendentry2, T1xlegendentry2.c_str());

	string T1xlegendentry3 = "Fit Width: ";
	string T1xlegendentry4 = "Fit Mean: ";
	
	T1xlegendentry3 += Form("%.2f",width_Tracker1xcoordRes/(T1R/1000));
	T1xlegendentry3 += " mRad";
	char T1xcharlegendentry3[T1xlegendentry3.size()];
	strcpy(T1xcharlegendentry3, T1xlegendentry3.c_str());

	T1xlegendentry4 += Form("%.2f",mean_Tracker1xcoordRes/(T1R/1000));
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

	//auto T1yRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker1y->GetMaximum()),rangevals,1.05*(h_residual_Tracker1y->GetMaximum()),-rangevals/(T1R/1000),rangevals/(T1R/1000),510,"-");	
	//T1yRaxis->SetTitle("[mRad]");
	//T1yRaxis->Draw();
	/*
	string T1ylegendentry = "Fit Width: ";
	string T1ylegendentry2 = "Fit Mean: ";
	T1ylegendentry += Form("%.2f",1000*width_Tracker1ycoordRes);
	T1ylegendentry += " um";
	char T1ycharlegendentry[T1ylegendentry.size()];
	strcpy(T1ycharlegendentry, T1ylegendentry.c_str());

	T1ylegendentry2 += Form("%.2f",1000*mean_Tracker1ycoordRes);
	T1ylegendentry2 += " um";
	char T1ycharlegendentry2[T1ylegendentry2.size()];
	strcpy(T1ycharlegendentry2, T1ylegendentry2.c_str());

	string T1ylegendentry3 = "Fit Width: ";
	string T1ylegendentry4 = "Fit Mean: ";
	T1ylegendentry3 += Form("%.2f",width_Tracker1ycoordRes/(T1R/1000));
	T1ylegendentry3 += " mRad";
	char T1ycharlegendentry3[T1ylegendentry3.size()];
	strcpy(T1ycharlegendentry3, T1ylegendentry3.c_str());

	T1ylegendentry4 += Form("%.2f",mean_Tracker1ycoordRes/(T1R/1000));
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
	
	//auto T2xRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker2x->GetMaximum()),rangevals,1.05*(h_residual_Tracker2x->GetMaximum()),-rangevals/(T2R/1000),rangevals/(T2R/1000),510,"-");	
	//T2xRaxis->SetTitle("[mRad]");
	//T2xRaxis->Draw();
	/*
	string T2xlegendentry = "Fit Width: ";
	string T2xlegendentry2 = "Fit Mean: ";
	T2xlegendentry += Form("%.2f",1000*width_Tracker2xcoordRes);
	T2xlegendentry += " um";
	char T2xcharlegendentry[T2xlegendentry.size()];
	strcpy(T2xcharlegendentry, T2xlegendentry.c_str());

	T2xlegendentry2 += Form("%.2f",1000*mean_Tracker2xcoordRes);
	T2xlegendentry2 += " um";
	char T2xcharlegendentry2[T2xlegendentry2.size()];
	strcpy(T2xcharlegendentry2, T2xlegendentry2.c_str());

	string T2xlegendentry3 = "Fit Width: ";
	string T2xlegendentry4 = "Fit Mean: ";
	T2xlegendentry3 += Form("%.2f",width_Tracker2xcoordRes/(T2R/1000));
	T2xlegendentry3 += " mRad";
	char T2xcharlegendentry3[T2xlegendentry3.size()];
	strcpy(T2xcharlegendentry3, T2xlegendentry3.c_str());

	T2xlegendentry4 += Form("%.2f",mean_Tracker2xcoordRes/(T2R/1000));
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
	

	//auto T2yRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker2y->GetMaximum()),rangevals,1.05*(h_residual_Tracker2y->GetMaximum()),-rangevals/(T2R/1000),rangevals/(T2R/1000),510,"-");	
	//T2yRaxis->SetTitle("[mRad]");
	//T2yRaxis->Draw();
	/*
	string T2ylegendentry = "Fit Width: ";
	string T2ylegendentry2 = "Fit Mean: ";
	T2ylegendentry += Form("%.2f",1000*width_Tracker2ycoordRes);
	T2ylegendentry += " um";
	char T2ycharlegendentry[T2ylegendentry.size()];
	strcpy(T2ycharlegendentry, T2ylegendentry.c_str());

	T2ylegendentry2 += Form("%.2f",1000*mean_Tracker2ycoordRes);
	T2ylegendentry2 += " um";
	char T2ycharlegendentry2[T2ylegendentry2.size()];
	strcpy(T2ycharlegendentry2, T2ylegendentry2.c_str());

	string T2ylegendentry3 = "Fit Width: ";
	string T2ylegendentry4 = "Fit Mean: ";
	T2ylegendentry3 += Form("%.2f",width_Tracker2ycoordRes/(T2R/1000));
	T2ylegendentry3 += " mRad";
	char T2ycharlegendentry3[T2ylegendentry3.size()];
	strcpy(T2ycharlegendentry3, T2ylegendentry3.c_str());

	T2ylegendentry4 += Form("%.2f",mean_Tracker2ycoordRes/(T2R/1000));
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
	
	
	//auto T3xRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker3x->GetMaximum()),rangevals,1.05*(h_residual_Tracker3x->GetMaximum()),-rangevals/(T3R/1000),rangevals/(T3R/1000),510,"-");	
	//T3xRaxis->SetTitle("[mRad]");
	//T3xRaxis->Draw();
	/*
	string T3xlegendentry = "Fit Width: ";
	string T3xlegendentry2 = "Fit Mean: ";
	T3xlegendentry += Form("%.2f",1000*width_Tracker3xcoordRes);
	T3xlegendentry += " um";
	char T3xcharlegendentry[T3xlegendentry.size()];
	strcpy(T3xcharlegendentry, T3xlegendentry.c_str());

	T3xlegendentry2 += Form("%.2f",1000*mean_Tracker3xcoordRes);
	T3xlegendentry2 += " um";
	char T3xcharlegendentry2[T3xlegendentry2.size()];
	strcpy(T3xcharlegendentry2, T3xlegendentry2.c_str());

	string T3xlegendentry3 = "Fit Width: ";
	string T3xlegendentry4 = "Fit Mean: ";
	T3xlegendentry3 += Form("%.2f",width_Tracker3xcoordRes/(T3R/1000));
	T3xlegendentry3 += " mRad";
	char T3xcharlegendentry3[T3xlegendentry3.size()];
	strcpy(T3xcharlegendentry3, T3xlegendentry3.c_str());

	T3xlegendentry4 += Form("%.2f",mean_Tracker3xcoordRes/(T3R/1000));
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
	
	
	//auto T3yRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker3y->GetMaximum()),rangevals,1.05*(h_residual_Tracker3y->GetMaximum()),-rangevals/(T3R/1000),rangevals/(T3R/1000),510,"-");	
	//T3yRaxis->SetTitle("[mRad]");
	//T3yRaxis->Draw();
	/*
	string T3ylegendentry = "Fit Width: ";
	string T3ylegendentry2 = "Fit Mean: ";
	T3ylegendentry += Form("%.2f",1000*width_Tracker3ycoordRes);
	T3ylegendentry += " um";
	char T3ycharlegendentry[T3ylegendentry.size()];
	strcpy(T3ycharlegendentry, T3ylegendentry.c_str());

	T3ylegendentry2 += Form("%.2f",1000*mean_Tracker3ycoordRes);
	T3ylegendentry2 += " um";
	char T3ycharlegendentry2[T3ylegendentry2.size()];
	strcpy(T3ycharlegendentry2, T3ylegendentry2.c_str());

	string T3ylegendentry3 = "Fit Width: ";
	string T3ylegendentry4 = "Fit Mean: ";
	T3ylegendentry3 += Form("%.2f",width_Tracker3ycoordRes/(T3R/1000));
	T3ylegendentry3 += " mRad";
	char T3ycharlegendentry3[T3ylegendentry3.size()];
	strcpy(T3ycharlegendentry3, T3ylegendentry3.c_str());

	T3ylegendentry4 += Form("%.2f",mean_Tracker3ycoordRes/(T3R/1000));
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
	

	//auto T4xRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker4x->GetMaximum()),rangevals,1.05*(h_residual_Tracker4x->GetMaximum()),-rangevals/(T4R/1000),rangevals/(T4R/1000),510,"-");	
	//T4xRaxis->SetTitle("[mRad]");
	//T4xRaxis->Draw();
	/*
	string T4xlegendentry = "Fit Width: ";
	string T4xlegendentry2 = "Fit Mean: ";
	T4xlegendentry += Form("%.2f",1000*width_Tracker4xcoordRes);
	T4xlegendentry += " um";
	char T4xcharlegendentry[T4xlegendentry.size()];
	strcpy(T4xcharlegendentry, T4xlegendentry.c_str());

	T4xlegendentry2 += Form("%.2f",1000*mean_Tracker4xcoordRes);
	T4xlegendentry2 += " um";
	char T4xcharlegendentry2[T4xlegendentry2.size()];
	strcpy(T4xcharlegendentry2, T4xlegendentry2.c_str());

	string T4xlegendentry3 = "Fit Width: ";
	string T4xlegendentry4 = "Fit Mean: ";
	T4xlegendentry3 += Form("%.2f",width_Tracker4xcoordRes/(T4R/1000));
	T4xlegendentry3 += " mRad";
	char T4xcharlegendentry3[T4xlegendentry3.size()];
	strcpy(T4xcharlegendentry3, T4xlegendentry3.c_str());

	T4xlegendentry4 += Form("%.2f",mean_Tracker4xcoordRes/(T4R/1000));
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
	
	
	//auto T4yRaxis = new TGaxis(-rangevals,1.05*(h_residual_Tracker4y->GetMaximum()),rangevals,1.05*(h_residual_Tracker4y->GetMaximum()),-rangevals/(T4R/1000),rangevals/(T4R/1000),510,"-");	
	//T4yRaxis->SetTitle("[mRad]");
	//T4yRaxis->Draw();
	/*
	string T4ylegendentry = "Fit Width: ";
	string T4ylegendentry2 = "Fit Mean: ";
	T4ylegendentry += Form("%.2f",1000*width_Tracker4ycoordRes);
	T4ylegendentry += " um";
	char T4ycharlegendentry[T4ylegendentry.size()];
	strcpy(T4ycharlegendentry, T4ylegendentry.c_str());

	T4ylegendentry2 += Form("%.2f",1000*mean_Tracker4ycoordRes);
	T4ylegendentry2 += " um";
	char T4ycharlegendentry2[T4ylegendentry2.size()];
	strcpy(T4ycharlegendentry2, T4ylegendentry2.c_str());

	string T4ylegendentry3 = "Fit Width: ";
	string T4ylegendentry4 = "Fit Mean: ";
	T4ylegendentry3 += Form("%.2f",width_Tracker4ycoordRes/(T4R/1000));
	T4ylegendentry3 += " mRad";
	char T4ycharlegendentry3[T4ylegendentry3.size()];
	strcpy(T4ycharlegendentry3, T4ylegendentry3.c_str());

	T4ylegendentry4 += Form("%.2f",mean_Tracker4ycoordRes/(T4R/1000));
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
	TrackerHits->SaveAs("CartesianAlignmentResiduals.png");
		

	cout << "Tracker 1x Mean: " <<  h_residual_Tracker1x->GetMean() << ", StdDev: " << h_residual_Tracker1x->GetStdDev() << endl;
	cout << "Tracker 1y Mean: " <<  h_residual_Tracker1y->GetMean() << ", StdDev: " << h_residual_Tracker1y->GetStdDev() << endl;
	cout << "Tracker 2x Mean: " <<  h_residual_Tracker2x->GetMean() << ", StdDev: " << h_residual_Tracker2x->GetStdDev() << endl;
	cout << "Tracker 2y Mean: " <<  h_residual_Tracker2y->GetMean() << ", StdDev: " << h_residual_Tracker2y->GetStdDev() << endl;
	cout << "Tracker 3x Mean: " <<  h_residual_Tracker3x->GetMean() << ", StdDev: " << h_residual_Tracker3x->GetStdDev() << endl;
	cout << "Tracker 3y Mean: " <<  h_residual_Tracker3y->GetMean() << ", StdDev: " << h_residual_Tracker3y->GetStdDev() << endl;
	cout << "Tracker 4x Mean: " <<  h_residual_Tracker4x->GetMean() << ", StdDev: " << h_residual_Tracker4x->GetStdDev() << endl;
	cout << "Tracker 4y Mean: " <<  h_residual_Tracker4y->GetMean() << ", StdDev: " << h_residual_Tracker4y->GetStdDev() << endl;

      }
      break;
    }
    
    if(Iterator >= 80){	  

      OptimizedXYOffsets.push_back(Total1Xshift); OptimizedXYOffsets.push_back(Total1Yshift);
      OptimizedXYOffsets.push_back(Total2Xshift); OptimizedXYOffsets.push_back(Total2Yshift);
      OptimizedXYOffsets.push_back(Total3Xshift); OptimizedXYOffsets.push_back(Total3Yshift);
      OptimizedXYOffsets.push_back(Total4Xshift); OptimizedXYOffsets.push_back(Total4Yshift);

      cout << "Tracker 1x Residual Width: " << float(int(100000*width_Tracker1xcoordRes))/100 << " um" << endl;
      cout << "Tracker 1y Residual Width: " << 1000*width_Tracker1ycoordRes << " um" << endl;
      cout << "Tracker 2x Residual Width: " << 1000*width_Tracker2xcoordRes << " um" << endl;
      cout << "Tracker 2y Residual Width: " << 1000*width_Tracker2ycoordRes << " um" << endl;
      cout << "Tracker 3x Residual Width: " << 1000*width_Tracker3xcoordRes << " um" << endl;
      cout << "Tracker 3y Residual Width: " << 1000*width_Tracker3ycoordRes << " um" << endl;
      cout << "Tracker 4x Residual Width: " << 1000*width_Tracker4xcoordRes << " um" << endl;
      cout << "Tracker 4y Residual Width: " << 1000*width_Tracker4ycoordRes << " um" << endl;
      
      if(plots){
	
	gStyle->SetOptFit();
	gStyle->SetStatX(0.45);
	gStyle->SetStatW(0.18);	
	gStyle->SetStatY(0.85);
	
	TCanvas* TrackerHits = new TCanvas(name2X, "Cartesian Alignment Residuals", 2000, 1000);
	TrackerHits->Divide(4,2);
	//TrackerHits->SetRightMargin(TrackerHits->PixeltoX(0.01));

	float rangevals = 0.5;
	TrackerHits->cd(1);
	h_residual_Tracker1x->SetTitle("Tracker1x residual");
	//h_residual_Tracker1x->SetTitleOffset(0.5);	
	h_residual_Tracker1x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker1x->Draw();
	//gStyle->SetOptStat(0);

	
	TrackerHits->cd(5);
	h_residual_Tracker1y->SetTitle("Tracker1y residual");
	h_residual_Tracker1y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker1y->Draw();

	
	TrackerHits->cd(2);
	h_residual_Tracker2x->SetTitle("Tracker2x residual");
	h_residual_Tracker2x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker2x->Draw();

	TrackerHits->cd(6);
	h_residual_Tracker2y->SetTitle("Tracker2y residual");
	h_residual_Tracker2y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker2y->Draw();
	

		
	TrackerHits->cd(3);
	h_residual_Tracker3x->SetTitle("Tracker3x residual");
	h_residual_Tracker3x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker3x->Draw();
	


	
	
	TrackerHits->cd(7);
	h_residual_Tracker3y->SetTitle("Tracker3y residual");
	h_residual_Tracker3y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker3y->Draw();
	


	
	
	TrackerHits->cd(4);
	h_residual_Tracker4x->SetTitle("Tracker4x residual");
	h_residual_Tracker4x->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker4x->Draw();
	

	TrackerHits->cd(8);
	h_residual_Tracker4y->SetTitle("Tracker4y residual");
	h_residual_Tracker4y->GetXaxis()->SetRangeUser(-rangevals, rangevals);
	h_residual_Tracker4y->Draw();
	

	gPad->Update();
	  
	TrackerHits->Draw();
	gPad->Update();
	TrackerHits->SaveAs("CartesianAlignmentResiduals.png");
		

	TCanvas* c1 = new TCanvas("c1","", 700,500);
	c1->cd(1);
	Residual1XPerIteration->SetTitle("Average Tracker X and Y coordinate Residual per Iteration");
	Residual1XPerIteration->SetMarkerStyle(20);
	Residual1XPerIteration->GetYaxis()->SetRangeUser(-1000, 1000);
	Residual1XPerIteration->Draw("AP");
	Residual1YPerIteration->SetMarkerStyle(24);
	Residual1YPerIteration->GetYaxis()->SetRangeUser(-1000, 1000);
	Residual1YPerIteration->Draw("Psame");
	Residual2XPerIteration->SetMarkerStyle(21);
	Residual2XPerIteration->GetYaxis()->SetRangeUser(-1000, 1000);
	Residual2XPerIteration->Draw("Psame");
	Residual2YPerIteration->SetMarkerStyle(25);
	Residual2YPerIteration->GetYaxis()->SetRangeUser(-1000, 1000);
	Residual2YPerIteration->Draw("Psame");
	Residual3XPerIteration->SetMarkerStyle(22);	  
	Residual3XPerIteration->GetYaxis()->SetRangeUser(-1000, 1000);
	Residual3XPerIteration->Draw("Psame");
	Residual3YPerIteration->SetMarkerStyle(26);
	Residual3YPerIteration->GetYaxis()->SetRangeUser(-1000, 1000);
	Residual3YPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(23);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-1000, 1000);
	Residual4XPerIteration->Draw("Psame");
	Residual4YPerIteration->SetMarkerStyle(27);
	Residual4YPerIteration->GetYaxis()->SetRangeUser(-1000, 1000);
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
	
	/*
	//IEEE Conference figure
	TCanvas* TrackerRes = new TCanvas(nameRes2X, "", 2000, 1000);
	TrackerRes->Divide(4,2);

	rangevals = 5;
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
	*/
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

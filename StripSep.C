vector<float> StripSep (vector<float>, vector<float>);
//Make function that separtes a vector of lists consecutive numbers and returns the sizes of each list.
vector<float> StripSep(vector<float>  Stripvec, vector<float>  Chargevec){

  
  vector<float> SubEventSizes;
  float previndx=Stripvec.at(0);
  
  float Currentindx=0; 
  int Vecsize=0;
  int maxVecsize=1;
  float tolerance = 1;
  int NumSubEvents=1;
  
  for(uint i = 0; i < Stripvec.size(); i++){
    Currentindx = Stripvec.at(i);
    //cout << "Strip: " << Currentindx << "with charge " << Chargevec.at(i) << endl;
    //if current strip number is sufficiently close to the previous strip in the hit, increase the vector size and check if it's the largest
    if(Currentindx <= previndx + tolerance){
      Vecsize++;
      //this determines the longest list in the vector. idk if it's necessary tbh
      if(Vecsize > maxVecsize)
	maxVecsize = Vecsize;
      //need this condition incase the last event is more than one strip
      if(i == (Stripvec.size()-1))
	SubEventSizes.push_back(Vecsize);
      
    }
    //if current strip number is NOT consecutive with the previous strip
    if(Currentindx > tolerance + previndx){
      //append the size of the previous list
      SubEventSizes.push_back(Vecsize);
      NumSubEvents++;
      Vecsize = 1;
      //if the last event is one strip long, this will catch it
      if(i == (Stripvec.size()-1))
	SubEventSizes.push_back(Vecsize);
    }

    previndx = Currentindx;
  } 
  cout << "Sub Events: " <<  NumSubEvents << ", Max SM: " << maxVecsize << endl;

  return SubEventSizes;//resultstripsandcharges;
}

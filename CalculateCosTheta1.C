//this function caluclates the angle between hits wrt a common origin
double CalculateCosTheta1(double x, double y, double x1,double y1){
  //double cosineTheta = ( x*x1 + y*y1 )/(x*x+y*y);
  double sineTheta = (x1*y - x*y1)/(x*x + y*y);
  //if(cosineTheta>1) cosineTheta=2-cosineTheta;
  //cout<<sineTheta<<endl;
  return asin(sineTheta);
}

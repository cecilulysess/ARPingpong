#include <iostream>
#include <string>

#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"

using namespace std;



int main(){
  char* image_addr = "C:\\Users\\Public\\Pictures\\Sample Pictures\\Chrysanthemum.jpg";
  cv::Mat img = cv::imread(image_addr, CV_LOAD_IMAGE_GRAYSCALE);
  cv::imshow("Image to show", img);
  cv::waitKey();
  cout<<"Test"<<endl;
}
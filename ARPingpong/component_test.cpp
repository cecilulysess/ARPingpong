#include <iostream>
#include <string>

#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"

#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "ARRegistrationModule\registration_public_if.h"
#include "OpenCVHelperLibrary\cv_helper_lib.h"
#include "ARTagDetectionModule\tag_detection_ops.h"
using namespace std;

//VARIABLES for CODEBOOK METHOD:
CvBGCodeBookModel* model = 0;
const int NCHANNELS = 3;
bool ch[NCHANNELS]={true,true,true}; // This sets what channels should be adjusted for background bounds


int main(){
  //char* image_addr = "D:\\Dropbox\\Project PlanetAR\\UDResearch\\ExperimentData\\rgb.jpg";
  //cv::Mat img = cv::imread(image_addr, CV_LOAD_IMAGE_COLOR);
  //cv::imshow("Image to show", img);
  //cv::waitKey();
  //cout<<"Test:"<<registration_module::RegistrationOps::Add(1, 3.5)<<endl;




  cv::Mat frame;
  // for rgb color channel test
  cv::VideoCapture cap(1);
  //cv::VideoCapture cap("D:\\Downloads\\v4.avi");

  if (!cap.isOpened()) {
          std::cout << "Failed to open video capture device 0\n";
          return 1;
  }
  std::vector<cv::Mat> extframe;
  cv::Mat hsvframe;
  cv::namedWindow("picture window", CV_WINDOW_AUTOSIZE);          
  cv::namedWindow("R channel", CV_WINDOW_AUTOSIZE);     
  cv::namedWindow("B channel", CV_WINDOW_AUTOSIZE);     
  cv::namedWindow("G channel", CV_WINDOW_AUTOSIZE);     
    cv::namedWindow("H channel", CV_WINDOW_AUTOSIZE);     
  cv::namedWindow("S channel", CV_WINDOW_AUTOSIZE);     
  cv::namedWindow("V channel", CV_WINDOW_AUTOSIZE);     
  while (cv::waitKey(30) == -1) {
  
          cap >> frame;     
          //frame.copyTo(extframe);
          /*extframe = cv_helper::CvHelper::getBGRByChannel(frame);
          
          cv::imshow("B channel", extframe.at(0) );
          cv::imshow("G channel", extframe.at(1) );
          cv::imshow("R channel", extframe.at(2) );*/
          
          cv::imshow("picture window", frame);



          //uchar px = frame.ptr<uchar>(1) [3*50 + 0];
          //cout<<(unsigned short)px<<endl;
          //hsvframe = tag_detection_module::TagDetectionOp::DetectTag(frame);
          /*extframe = cv_helper::CvHelper::getBGRByChannel(hsvframe);
          cv::imshow("H channel", extframe.at(0) );
          cv::imshow("S channel", extframe.at(1) );
          cv::imshow("V channel", extframe.at(2) );*/
  }
  //frame=cv::imread("D:\\Dropbox\\Project PlanetAR\\UDResearch\\ExperimentData\\rgb.jpg");
  
  return 0;
}
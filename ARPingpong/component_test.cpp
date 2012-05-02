#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <Windows.h>
#include <tchar.h>

#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include <opencv2\opencv.hpp>

#include <opencv2/video/background_segm.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "ARRegistrationModule\registration_public_if.h"
#include "OpenCVHelperLibrary\cv_helper_lib.h"
#include "ARTagDetectionModule\tag_detection_ops.h"
#include "PositionEstimationModule\posit_estimation_ops.h"

using namespace std;


void AddAllFilesInDir(vector<string>& filenames, string dir_base);
void capture_calibration_images();

int main(){
  
  //char* image_addr = "D:\\Dropbox\\Project PlanetAR\\UDResearch\\ExperimentData\\rgb.jpg";
  //cv::Mat img = cv::imread(image_addr, CV_LOAD_IMAGE_COLOR);
  //cv::imshow("Image to show", img);
  //cv::waitKey();
  //cout<<"Test:"<<registration_module::RegistrationOps::Add(1, 3.5)<<endl;

  //string chess_board_path = "C:\\Users\\Julian\\Pictures\\camera_calib";
  ////position_estimation_module::CameraCalibrator::capture_calibration_images(chess_board_path, 1);

  //position_estimation_module::CameraCalibrator cam_cal;
  //cout<<"successful added chess boards image: "<<
  //    cam_cal.addChessboardPoints(chess_board_path, cv::Size(11,9) )
  //    <<endl;
  //cv::Size img_size(640,480);
  //cout<<"reproject error: "<<cam_cal.calibrate(img_size)<<endl;
  //cv::namedWindow("origin_img", CV_WINDOW_AUTOSIZE );
  //cv::namedWindow("undist_img", CV_WINDOW_AUTOSIZE );
  //cout<<"Try to undistort image "<<cam_cal.image_filename_list_().at(2)<<endl;
  //cv::imshow("origin_img", cv::imread(cam_cal.image_filename_list_().at(2)) );
  //cv::imshow("undist_img", cam_cal.remap( cv::imread(cam_cal.image_filename_list_().at(2)) ));
  //cout<<"Camera Matrix:"<<endl;
  //cv_helper::CvHelper::show_matrix_fc64(cam_cal.camera_matrix_());
  //
  //cv::waitKey(0);

  //cv::Mat frame = cv::imread("C:\\Users\\Julian\\Pictures\\camera_calib\\28.jpg", CV_LOAD_IMAGE_GRAYSCALE);
  
  //// for rgb color channel test
  //cv::VideoCapture cap(1);
  ////cv::VideoCapture cap("D:\\Downloads\\v4.avi");

  //if (!cap.isOpened()) {
  //        std::cout << "Failed to open video capture device 0\n";
  //        return 1;
  //}

  //std::vector<cv::Mat> extframe;
  //cv::Mat hsvframe;
  //cv::namedWindow("picture_window", CV_WINDOW_AUTOSIZE);          
/*  cv::namedWindow("R channel", CV_WINDOW_AUTOSIZE);     
  cv::namedWindow("B channel", CV_WINDOW_AUTOSIZE);     
  cv::namedWindow("G channel", CV_WINDOW_AUTOSIZE);     
    cv::namedWindow("H channel", CV_WINDOW_AUTOSIZE);     
  cv::namedWindow("S channel", CV_WINDOW_AUTOSIZE);     
  cv::namedWindow("V channel", CV_WINDOW_AUTOSIZE);   */  
  
  /*std::vector<cv::Point2f> imageCorners;
  cv::Size boardSize(11, 9);
  bool found = cv::findChessboardCorners(frame, boardSize, imageCorners);
  cv::drawChessboardCorners(frame, boardSize, imageCorners, found);
  cv::imshow("picture_window", frame);
  
  for( std::vector<cv::Point2f>::iterator itr = imageCorners.begin();
      itr != imageCorners.end(); itr++ ) {
    cout<<*itr<<endl;
  }
  cv::waitKey(0);*/
  //while (cv::waitKey(30) == -1) {
  //
  //        cap >> frame;     
  //        //frame.copyTo(extframe);
  //        /*extframe = cv_helper::CvHelper::getBGRByChannel(frame);
  //        
  //        cv::imshow("B channel", extframe.at(0) );
  //        cv::imshow("G channel", extframe.at(1) );
  //        cv::imshow("R channel", extframe.at(2) );*/
  //        
  //        cv::imshow("picture window", frame);



  //        //uchar px = frame.ptr<uchar>(1) [3*50 + 0];
  //        //cout<<(unsigned short)px<<endl;
  //        //hsvframe = tag_detection_module::TagDetectionOp::DetectTag(frame);
  //        /*extframe = cv_helper::CvHelper::getBGRByChannel(hsvframe);
  //        cv::imshow("H channel", extframe.at(0) );
  //        cv::imshow("S channel", extframe.at(1) );
  //        cv::imshow("V channel", extframe.at(2) );*/
  //}
  //frame=cv::imread("D:\\Dropbox\\Project PlanetAR\\UDResearch\\ExperimentData\\rgb.jpg");
	tag_detection_module::tag_detection_Test();
	return  0;
}




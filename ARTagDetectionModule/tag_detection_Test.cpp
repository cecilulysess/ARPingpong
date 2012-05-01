#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include "tag_detection_ops.h"
#include "OpenCVHelperLibrary\cv_helper_lib.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace tag_detection_module {

int  tag_detection_module::tag_detection_Test(){
   // Open the camera
   cv::VideoCapture capture(0);
   // check if video successfully opened
   if (!capture.isOpened())
   return 1;
   bool stop(false);   //tag to stope the video
   cv::Mat frame;      // current video frame
   cv::namedWindow("sourceFrame",1);
   cv::namedWindow("rRegion",1);
   cv::namedWindow("gRegion",1);
   cv::namedWindow("bRegion",1);

   //Define variable to hold the coped result
   std::vector<cv::Mat>  rgbChannels;      //hold the segmented result for rgb
   cv::Mat  rRegion;                       //hold the region of red
   cv::Mat  gRegion;                       //hold the region of green
   cv::Mat  bRegion;                       //hold the region of blue
   std::vector<tag_detection_module::cvCenterPoint> rCenter;     //hold the center point of red region
   std::vector<tag_detection_module::cvCenterPoint> gCenter;     //hold the center point of green region
   std::vector<tag_detection_module::cvCenterPoint> bCenter;     //hold the center point of blue region
   //Define the location of frame
   int  nframeNum=0;
   // for all frames in video
   while (!stop) {
   nframeNum++;
   //read next frame if any
   if (!capture.read(frame))
             break;
   //frame=cv::imread("testImg.bmp");
   if(nframeNum==1){
	rRegion=frame;
	gRegion=frame;
	bRegion=frame;
   }else{
	//get signal channel on rgb region
    rgbChannels=tag_detection_module::TagDetectionOp::cvColorDetect(frame); 
	rRegion=rgbChannels.at(0);
	gRegion=rgbChannels.at(1);
	bRegion=rgbChannels.at(2);
	//handle the signal channel bianry iamge
	rRegion=tag_detection_module::TagDetectionOp::cvHandleBinaryImg(rRegion);
	gRegion=tag_detection_module::TagDetectionOp::cvHandleBinaryImg(gRegion);
	bRegion=tag_detection_module::TagDetectionOp::cvHandleBinaryImg(bRegion);
	//get the signale channel rectangle cener point
    rCenter=tag_detection_module::TagDetectionOp::cvGetCenterPoint(rRegion,tag_detection_module::CV_TAG_RED);
	gCenter=tag_detection_module::TagDetectionOp::cvGetCenterPoint(gRegion,tag_detection_module::CV_TAG_GREEN);
	bCenter=tag_detection_module::TagDetectionOp::cvGetCenterPoint(bRegion,tag_detection_module::CV_TAG_BLUE);
	//draw the rectangle and center point  on the souce image
	std::vector<std::vector<cv::Point> > squares;
    std::vector<std::vector<cv::Point> > contours;
    contours=tag_detection_module::TagDetectionOp::cvGetRectangle(rRegion,squares);
	tag_detection_module::TagDetectionOp::cvDrawSourceImg(frame,squares,rCenter);
	std::vector<std::vector<cv::Point> >().swap(squares);
	std::vector<std::vector<cv::Point> >().swap(contours);
	//show each channel and the source image
	cv::imshow("sourceFrame",frame);
	cv::imshow("rRegion",rRegion);
	cv::imshow("gRegion",gRegion);
	cv::imshow("bRegion",bRegion);
    //free the memory of vector capacity
	std::vector<cv::Mat>().swap(rgbChannels);
	std::vector<tag_detection_module::cvCenterPoint>().swap(rCenter);
	std::vector<tag_detection_module::cvCenterPoint>().swap(gCenter);
	std::vector<tag_detection_module::cvCenterPoint>().swap(bCenter);
   }
  // introduce a delay
  // or press key to stop
    if (cv::waitKey(2)>=0)
     stop= true;
  }
   // Close the video file.
   // Not required since called by destructor
    capture.release();
    return 2;
	
 }

}
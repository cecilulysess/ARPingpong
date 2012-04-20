#include "tag_detection_ops.h"
#include <iostream>

#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include "OpenCVHelperLibrary\cv_helper_lib.h"

using namespace std;

namespace tag_detection_module {
  cv::Mat TagDetectionOp::DetectTag( cv::Mat& image ) {
    cv::Mat* img_hsv = new cv::Mat(image.size(), image.type());
    cv::cvtColor(image, *img_hsv, CV_BGR2HSV);
    std::vector<cv::Mat> chanelInfo=cv_helper::CvHelper::getBGRByChannel(*img_hsv);

    //define the threshold for each chanel on hsv ,which is reflect to rgb colorspace  .The value of pure rgb on hue is 0  1/3  2/3 
    //and the value of pure rgb on s,v is 1  
    double threshold_RH_low=0.01;
    double threshold_RH_high=0.99;
    double threshold_GH_low=0.32;
    double threshold_GH_high=0.34;
    double threshold_BH_low=0.65;
    double threshold_BH_high=0.67;
    double threshold_S =0.5;
    double threshold_V =0.5;
    //Iterator the mat of hue and change the hsv image to binaray image
     cv::Mat chanel_H=chanelInfo.at(0);
     cv::Mat chanel_S=chanelInfo.at(1);
     cv::Mat chanel_V=chanelInfo.at(2);
     int width=chanel_H.size().width;
     int height=chanel_H.size().height;

     cv::Mat* chanel_S_result =new cv::Mat(chanel_S.size(),chanel_S.type());
     //cv::cvThreshold(chanel_S,*chanel_S_result,threshold_RH_high,0);


       ///
       //cv::Mat s_thresholed_img = ....
       //cv::Mat v_thresholed_img = ...
       //cv::s_v_mask = s_thresholed_img && v_thresholed_img
       ////
    return *img_hsv;
    

  }
}
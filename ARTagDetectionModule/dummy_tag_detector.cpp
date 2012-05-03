#include "tag_detection_ops.h"
#include "OpenCVHelperLibrary\cv_helper_lib.h"
#include <time.h>
#include <stdio.h>

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

namespace tag_detection_module{
  std::vector<cv_helper::threshold_range> DummyTagDetector::green_tag_thresholds;
  std::vector<cv_helper::threshold_range> DummyTagDetector::blue_tag_thresholds;
   std::vector<cv_helper::threshold_range> DummyTagDetector::red_tag_thresholds;
  bool DummyTagDetector::is_threshold_ranges_inited = false;

  std::vector<cv_helper::threshold_range>& DummyTagDetector::InitThresholdRanges(){
    if ( is_threshold_ranges_inited ) {
      return green_tag_thresholds;
    }
    cv_helper::threshold_range green_L = {100, 175 },
                    green_a = {50,  105 }, //85
                    green_b = {140, 180 },
                    red_L = {30, 70},
                    red_a = {150, 175},
                    red_b = {135, 165},
                    blue_L = {0, 15},
                    blue_a = {140, 160},
                    blue_b = {80, 100};
                    
    green_tag_thresholds.push_back(green_L);
    green_tag_thresholds.push_back(green_a);
    green_tag_thresholds.push_back(green_b);
    red_tag_thresholds.push_back(red_L);
    red_tag_thresholds.push_back(red_a);
    red_tag_thresholds.push_back(red_b);
    blue_tag_thresholds.push_back(blue_L);
    blue_tag_thresholds.push_back(blue_a);
    blue_tag_thresholds.push_back(blue_b);
    DummyTagDetector::is_threshold_ranges_inited = true;
    return green_tag_thresholds;
  }

  DummyTagDetector::DummyTagDetector() {
    if ( !this->is_threshold_ranges_inited ) {
      this->InitThresholdRanges();
    }
#ifdef _DEBUG
    cv::namedWindow("L Channel");
    cv::namedWindow("a Channel");
    cv::namedWindow("b Channel");
    cv::namedWindow("TG Thresholded");
    cv::namedWindow("TB Thresholded");
    cv::namedWindow("TR Thresholded");
    cv::namedWindow("Combined Result");
    CvMouseCallback onMouse = &cv_helper::CvHelper::PrintPointValueWhenLeftClick;
    cv::setMouseCallback("Combined Result", onMouse, (void*) &Lab_image);
#endif
    this->extractor = (new cv_helper::ColorChannelExtractor());
  }

  DummyTagDetector::~DummyTagDetector(){
    this->extractor->~ColorChannelExtractor();
    this->tag_centers.~vector();
  }

  const std::vector<cv::Point2d>& DummyTagDetector::tag_centers_() {
    return this->tag_centers;
  }

  cv::Mat& DummyTagDetector::ExtractContourImage(cv::Mat& src){
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(src, contours, 
          CV_RETR_EXTERNAL, // retrieve external contours, 
                            //that is, connected components
          CV_CHAIN_APPROX_NONE); // all pixels of each contours
      this->contour_image = *(new cv::Mat(src.size(), CV_8U, cv::Scalar(255)));
      cv::drawContours(contour_image, contours, 
          -1, //draw all contours
          cv::Scalar(0), // in black
          2); // with a thickness of 2
      return this->contour_image;
  }


  const std::vector<cv::Point2d>& DummyTagDetector::DetectTags(
        const cv::Mat& frame2detect) {
    
    this->extractor->ExtractNewImage(frame2detect);
    /*cv::Mat chn_L = this->extractor->get_channels("L");
    cv::Mat chn_a = this->extractor->get_channels("a");
    cv::Mat chn_b = this->extractor->get_channels("b");
    */
    this->Lab_image = this->extractor->get_image_in_colorspace("Lab");
#ifdef _DEBUG
    clock_t str,end;
    cv::Mat thre_G(Lab_image.size(), CV_8U);
    cv::Mat thre_R(Lab_image.size(), CV_8U);
    cv::Mat thre_B(Lab_image.size(), CV_8U);
    str = clock();
    cv_helper::CvHelper::LabThresholdingByRange(
        this->green_tag_thresholds,
        Lab_image, thre_G);
    cv_helper::CvHelper::LabThresholdingByRange(
        this->red_tag_thresholds,
        Lab_image, thre_R);
    cv_helper::CvHelper::LabThresholdingByRange(
        this->blue_tag_thresholds,
        Lab_image, thre_B);
    end = clock();
    printf("Thresholding Time:%.3f sec\n", ((double)end-str)/CLOCKS_PER_SEC );
    //cv::Mat g_tag = this->ExtractContourImage(thre_G);
    //cv::imwrite("testl.bmp", thre_G);
    cv::imshow("Combined Result", Lab_image);
    cv::imshow("TG Thresholded", thre_G);
    cv::imshow("TR Thresholded", thre_R);
    cv::imshow("TB Thresholded", thre_B);
#endif
    return this->tag_centers_();
  }
}// ns tag_detection_module
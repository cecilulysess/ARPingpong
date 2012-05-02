#include "tag_detection_ops.h"
#include "OpenCVHelperLibrary\cv_helper_lib.h"

#include "opencv2\highgui\highgui.hpp"

namespace tag_detection_module{
  
  DummyTagDetector::DummyTagDetector() {
#ifdef _DEBUG
    cv::namedWindow("L Channel");
    cv::namedWindow("a Channel");
    cv::namedWindow("b Channel");
    cv::namedWindow("L Thresholded");
    cv::namedWindow("a Thresholded");
    cv::namedWindow("b Thresholded");
    cv::namedWindow("Combined Result");
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

  const std::vector<cv::Point2d>& DummyTagDetector::DetectTags(
        const cv::Mat& frame2detect) {
    
    this->extractor->ExtractNewImage(frame2detect);
    cv::Mat chn_L = this->extractor->get_channels("L");
    cv::Mat chn_a = this->extractor->get_channels("a");
    cv::Mat chn_b = this->extractor->get_channels("b");
#ifdef _DEBUG
    cv::imshow("L Channel", chn_L);
    cv::imshow("a Channel", chn_a);
    cv::imshow("b Channel", chn_b);
#endif
    
    return this->tag_centers_();
  }
}// ns tag_detection_module
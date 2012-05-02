#include "tag_detection_ops.h"
#include "OpenCVHelperLibrary\cv_helper_lib.h"

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

namespace tag_detection_module{
  std::vector<threshold_range> DummyTagDetector::green_tag_thresholds;
  bool DummyTagDetector::is_threshold_ranges_inited = false;

  std::vector<threshold_range>& DummyTagDetector::InitThresholdRanges(){
    if ( is_threshold_ranges_inited ) {
      return green_tag_thresholds;
    }
    threshold_range green_L = {150, 175 },
                    green_a = {50,  70  },
                    green_b = {160, 190 };

    green_tag_thresholds.push_back(green_L);
    green_tag_thresholds.push_back(green_a);
    green_tag_thresholds.push_back(green_b);
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
    cv::namedWindow("L Thresholded");
    cv::namedWindow("a Thresholded");
    cv::namedWindow("b Thresholded");
    cv::namedWindow("Combined Result");
    //CvMouseCallback onMouse = &cv_helper::CvHelper::PrintPointValueWhenLeftClick;
    //::setMouseCallback("Combined Result", onMouse, (void*) &Lab_image);
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
    cv::Mat thre_L(chn_L.size(), CV_8U);
    threshold_range rang_L = this->green_tag_thresholds.at(0);
    cv::threshold(chn_L, thre_L, rang_L.right, 255, CV_THRESH_TOZERO_INV);
    cv::threshold(thre_L, thre_L, rang_L.left, 255, CV_THRESH_BINARY);
    /*this->Lab_image = this->extractor->get_image_in_colorspace("Lab");
    cv::imshow("Combined Result", Lab_image);*/
    cv::imshow("L Thresholded", thre_L);
#endif
    return this->tag_centers_();
  }
}// ns tag_detection_module
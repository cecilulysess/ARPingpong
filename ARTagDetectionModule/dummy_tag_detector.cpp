#include "tag_detection_ops.h"
#include "OpenCVHelperLibrary\cv_helper_lib.h"
#include <time.h>
#include <stdio.h>

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

namespace tag_detection_module{
  // if new_pt changed less than change_threshold to origin, then return the old
  // one, else return true, and set new_pt to value of origin, otherwise, do nothing
  inline bool StablizePoint(const cv::Point2f& origin, 
      cv::Point2f& new_pt, 
      double change_threshold) {
    double dist = cv_helper::CvHelper::GetSqrEuclideanDist( 
        origin, new_pt);
    if ( dist < change_threshold * change_threshold) {
      new_pt.x = origin.x;
      new_pt.y = origin.y;
      return true;
    }
    return false;
  }
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
                    blue_L = {0, 20},
                    blue_a = {140, 160},
                    blue_b = {80, 95};
                    
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
  }

  const std::vector<cv::Point2d>& DummyTagDetector::tag_centers_() {
    return this->tag_centers;
  }

  std::vector<std::vector<cv::Point>>& DummyTagDetector::ExtractContours(
        cv::Mat& src, cv::Mat* contour_image){
    std::vector<std::vector<cv::Point>>* contours = new 
      std::vector<std::vector<cv::Point>>();
    cv::findContours(src, *contours, 
          CV_RETR_EXTERNAL, // retrieve external contours, 
                            //that is, connected components
          CV_CHAIN_APPROX_NONE); // all pixels of each contours
    if ( contour_image != NULL ) {
      contour_image->create(src.size(), CV_8U);
    }
      
      std::vector<std::vector<cv::Point>>::const_iterator 
          itc = contours->begin();
      while ( itc != contours->end() ) {
        if ( itc->size() < 10 || itc->size() > 150 ) {
          itc = contours->erase(itc);
        } else {
          ++itc;
        }
      }
      if ( contour_image != NULL ) {
        cv::drawContours(*contour_image, *contours, 
            -1, //draw all contours
            cv::Scalar(0), // in black
            2); // with a thickness of 2
      }
      return *contours;
  }


  const std::vector<cv::Point2d>& DummyTagDetector::DetectTags(
        const cv::Mat& frame2detect) {
    
    this->extractor->ExtractNewImage(frame2detect);
    this->Lab_image = this->extractor->get_image_in_colorspace("Lab");

    // thresholding 3 color tags
    cv::Mat thre_G(Lab_image.size(), CV_8U);
    cv::Mat thre_R(Lab_image.size(), CV_8U);
    cv::Mat thre_B(Lab_image.size(), CV_8U);
    cv_helper::CvHelper::Thresholding3ChannelsByRange(
        this->green_tag_thresholds,
        Lab_image, thre_G);
    cv_helper::CvHelper::Thresholding3ChannelsByRange(
        this->red_tag_thresholds,
        Lab_image, thre_R);
    cv_helper::CvHelper::Thresholding3ChannelsByRange(
        this->blue_tag_thresholds,
        Lab_image, thre_B);
    // extract coutour
    cv::Mat g_tag, r_tag, b_tag;
    std::vector<std::vector<cv::Point>>& g_centers = this->ExtractContours(thre_G, &g_tag);
    std::vector<std::vector<cv::Point>>& r_centers = this->ExtractContours(thre_R, &r_tag);
    std::vector<std::vector<cv::Point>>& b_centers = this->ExtractContours(thre_B, &b_tag);
    std::vector<cv::Point2f>& gt_centers = cv_helper::CvHelper::GetContoursCenter(g_centers, &g_tag);
    std::vector<cv::Point2f>& rt_centers = cv_helper::CvHelper::GetContoursCenter(r_centers, &r_tag);
    std::vector<cv::Point2f>& bt_centers = cv_helper::CvHelper::GetContoursCenter(b_centers, &b_tag);
    
    this->IdentifyTags(gt_centers, rt_centers, bt_centers);

#ifdef _DEBUG
    cv::Mat& combined_res = frame2detect.clone();

    for ( int i = 0; i < this->tag_centers.size(); ++i ) {
      cv::line(combined_res, this->tag_centers.at(i), 
        this->tag_centers.at((i+1)%4),
        cv::Scalar(20), 2);
    }
    //printf("Thresholding Time:%.3f sec\n", ((double)end-str)/CLOCKS_PER_SEC );
    
    //cv::imwrite("testl.bmp", thre_G);
    cv::imshow("Combined Result", combined_res);
    cv::imshow("TG Thresholded", g_tag);
    cv::imshow("TR Thresholded", r_tag);
    cv::imshow("TB Thresholded", b_tag);
#endif

    g_centers.~vector();
    r_centers.~vector();
    b_centers.~vector();
    gt_centers.~vector();
    rt_centers.~vector();
    bt_centers.~vector();
    return this->tag_centers_();
  }

  std::vector<cv::Point2d>& DummyTagDetector::IdentifyTags(
      std::vector<cv::Point2f> green_tags,
      std::vector<cv::Point2f> red_tags,
      std::vector<cv::Point2f> blue_tags) {
    // if tag is occluded, just ignore this frame
    if ( green_tags.size() < 2 || red_tags.size() < 1 || blue_tags.size() < 1)
      return this->tag_centers;
    
    cv::Point2f g1 = green_tags.at(0),
                g2 = green_tags.at(1),
                r1 = red_tags.at(0),
                b1 = blue_tags.at(0);
    double angA = cv_helper::CvHelper::GetAngleCosP1P0P2(g1, b1, r1),
           angB = cv_helper::CvHelper::GetAngleCosP1P0P2(g2, b1, r1);
    
    cv::Point2f true_g1, true_g2;
    if ( angA > angB ) {
      // the angA specify the actual G1 B R
      true_g1 = g2;
      true_g2 = g1;
    } else {
      true_g1 = g1;
      true_g2 = g2;
    }
    if ( this->tag_centers.size() > 1 ) {
      tag_detection_module::StablizePoint(
            this->tag_centers.at(0), 
            true_g1, 4);
      tag_detection_module::StablizePoint(
            this->tag_centers.at(1), 
            true_g2, 4);
      tag_detection_module::StablizePoint(
            this->tag_centers.at(2), 
            r1, 4);
      tag_detection_module::StablizePoint(
            this->tag_centers.at(3), 
            b1, 4) ;
    }
    this->tag_centers.clear();
    this->tag_centers.push_back(true_g1);
    this->tag_centers.push_back(true_g2);
    this->tag_centers.push_back(r1);
    this->tag_centers.push_back(b1);
    return this->tag_centers;
  }
}// ns tag_detection_module
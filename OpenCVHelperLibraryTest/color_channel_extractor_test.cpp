#include "test_lists.h"

#include <fstream>
#include <iostream>

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"

#include "cv_helper_lib.h"

namespace OCHL_test{
  namespace color_channel_extractor_test{
    bool ShouldThrowExceptionWhenNotInvokeExtractNewImage(){
      cv_helper::ColorChannelExtractor extractor;
      try {
        extractor.get_channels("R");
      } catch (...) {
        return true;
      }
      return false;
    }

    bool ShouldContainsLabChannels() {
      //system("%CD%");
      std::fstream f;
      f.open("TestFiles\\colorsc.png", std::ios::in);
      if ( !f ) {
        std::cout<<"Cannot Find test Image"<<std::endl;
        return false;
      }
      cv::Mat testimg = cv::imread("TestFiles\\colorsc.png");
      cv::namedWindow("ShouldContainsLabChannels");
      
      cv_helper::ColorChannelExtractor extractor;
      extractor.ExtractNewImage(testimg);
      try {
#ifdef _DEBUG
        cv::imshow("ShouldContainsLabChannels", testimg);
        cv::waitKey(300);
        cv::imshow("ShouldContainsLabChannels", extractor.get_channels("L"));
        cv::waitKey(300);
        cv::imshow("ShouldContainsLabChannels", extractor.get_channels("a"));
        cv::waitKey(300);
        cv::imshow("ShouldContainsLabChannels", extractor.get_channels("b"));
        cv::waitKey(300);
        cv::imshow("ShouldContainsLabChannels", extractor.get_image_in_colorspace("Lab"));
        cv::waitKey(300);
#endif
        assert(extractor.get_channels("L").channels() == 1);
        assert(extractor.get_channels("a").channels() == 1);
        assert(extractor.get_channels("b").channels() == 1);
        assert(extractor.get_image_in_colorspace("Lab").channels() == 3);
      } catch (...) {
        return false;
      }
      return true;
    }

    bool ShouldContainsHSVChannels() {
      std::fstream f;
      f.open("TestFiles\\colorsc.png", std::ios::in);
      if ( !f ) {
        std::cout<<"Cannot Find test Image"<<std::endl;
        return false;
      }
      cv::Mat testimg = cv::imread("TestFiles\\colorsc.png");
      cv::namedWindow("ShouldContainsHSVChannels");
      
      cv_helper::ColorChannelExtractor extractor;
      extractor.ExtractNewImage(testimg);
      try {
#ifdef _DEBUG
        cv::imshow("ShouldContainsHSVChannels", testimg);
        cv::waitKey(300);
        cv::imshow("ShouldContainsHSVChannels", extractor.get_channels("H"));
        cv::waitKey(300);
        cv::imshow("ShouldContainsHSVChannels", extractor.get_channels("S"));
        cv::waitKey(300);
        cv::imshow("ShouldContainsHSVChannels", extractor.get_channels("V"));
        cv::waitKey(300);
        cv::imshow("ShouldContainsHSVChannels", extractor.get_image_in_colorspace("HSV"));
        cv::waitKey(300);
#endif
        assert(extractor.get_channels("H").channels() == 1);
        assert(extractor.get_channels("S").channels() == 1);
        assert(extractor.get_channels("V").channels() == 1);
        assert(extractor.get_image_in_colorspace("HSV").channels() == 3);
      } catch (...) {
        return false;
      }
      return true;
    }
  } // ns ccet
}
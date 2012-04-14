#include <vector>
#include "cv_helper_lib.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cv_helper{
  std::vector<cv::Mat> CvHelper::getByChannel(const cv::Mat& image){
    std::vector<cv::Mat>* res_imgs = new std::vector<cv::Mat>();
    for( int i = 0; i < image.channels(); ++i ) {
      res_imgs->push_back(*(new cv::Mat( image.size(), image.type() )) );
    }

    cv::split(image, *res_imgs);

    //for( int y = 0; y < image.rows; ++y ) {
    //  for( int x = 0; x < image.cols; ++x ) {
    //    /*pixel = image.at<cv::Vec3b>(x, y);
    //    new_pixel = new cv::Vec3b();*/
    //    for( int i = 0 ; i < 3; ++i ) {
    //      if( i == channel ) {
    //        new_pixel[i] = pixel[i];
    //      } else {
    //        new_pixel[i] = 0;
    //      }
    //    }
    //    res_img->at<cv::Vec3b>(x, y) = pixel;
    //  }
    //}
    
    return *res_imgs;
  }

  std::vector<cv::Mat> CvHelper::getRGBImagesByChannel(const cv::Mat& image){
    std::vector<cv::Mat>* res_imgs = new std::vector<cv::Mat>();
    for( int i = 0; i < 3; ++i ) {
      cv::Mat cpimg = image.clone();
      
      cv::Mat_<cv::Vec3b>::iterator it = cpimg.begin<cv::Vec3b>(), it_end = cpimg.end<cv::Vec3b>();
      for( ; it!= it_end; ++it ){
        for( int j = 0 ; j < 3; ++j ) {
          if( i != j ) (*it)[j] &= 0;
        }
      }

      res_imgs->push_back(cpimg);
    }
    return *res_imgs;
  }

  void CvHelper::imShowRGB(std::vector<cv::Mat> images){
  }

}//ns cv_helper
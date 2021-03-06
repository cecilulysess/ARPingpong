#include <vector>
#include "cv_helper_lib.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <stdlib.h>

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace cv_helper{
  std::vector<cv::Mat> CvHelper::getBGRByChannel(const cv::Mat& image){

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

  std::vector<cv::Mat> CvHelper::getBGRImagesByChannel(const cv::Mat& image){
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


  void CvHelper::show_matrix_fc64(const cv::Mat& matrix){
    for (int i = 0 ;i < matrix.rows; ++i ) {
      printf("\t");
      for (int j = 0 ; j < matrix.cols; ++j ) {
        printf("%8.3f ", matrix.at<double>(i,j));
      }
      printf("\n");
    }
  }

  void CvHelper::PrintPointValueWhenLeftClick(int mouse_event, int x, int y, int flags, void* param){
    if ( mouse_event == CV_EVENT_LBUTTONUP && param != NULL ) {
      //cout<<"Trying :"<<x <<", " << y <<endl;
      cv::Mat* image = (cv::Mat*) param;
      cv::Vec3b value = (*image).at<cv::Vec3b>(y,x);
      cout<<"Value at ("<<x <<", "<<y<<") is: "<< (int) value[0] <<" "<<(int) value[1] <<" "<<(int) value[2]<<endl;
    }
  }

  void CvHelper::Thresholding3ChannelsByRange(
        const std::vector<threshold_range>& lab_range, 
        const cv::Mat src, 
        cv::Mat dst) {
    if ( src.channels() != 3 ) {
      throw std::invalid_argument("Source image is not a 3 channel image");
    }
    double  l_left  = lab_range.at(0).left,
            l_right = lab_range.at(0).right,
            a_left  = lab_range.at(1).left,
            a_right = lab_range.at(1).right,
            b_left  = lab_range.at(2).left,
            b_right = lab_range.at(2).right;
    uchar tabL[256], taba[256], tabb[256];
    for ( int i = 0; i< 256; ++i ) {
      tabL[i] = ( i > l_left && i < l_right ) ? 255 : 0; 
      taba[i] = ( i > a_left && i < a_right ) ? 255 : 0; 
      tabb[i] = ( i > b_left && i < b_right ) ? 255 : 0; 
      //cout<<"taba["<<i<<"]:"<<(short) taba[i]<<endl;
    }

    // scanning by 3 channels loop, this is the 
    // fastest way in theory
    int lines = src.rows, col = src.cols;
    if ( src.isContinuous() ) {
      col *= lines;
      lines = 1; // it's now a 1D array
    }
    // for each pixels
    int src_pix_idx = 0;
    for ( int j = 0; j < lines; ++j ) {
      // pointer to first column of line j
      const uchar* srcdata = src.ptr<uchar>(j);
      uchar* dstdata = dst.ptr<uchar>(j);
      for ( int i = 0; i < col; ++i ) {
        dstdata[i] = tabL[(srcdata[src_pix_idx])] & taba[srcdata[src_pix_idx+1]] & tabb[(srcdata[src_pix_idx+2])];
        src_pix_idx+=3;
      }
    }

  }

  std::vector<cv::Point2f>& CvHelper::GetContoursCenter(
        const std::vector<std::vector<cv::Point>>& contours, cv::Mat* draw_image ) {
    float radius;
    std::vector<cv::Point2f>* centers = new std::vector<cv::Point2f>();
    for ( int i = 0 ; i < contours.size(); ++i ) {
      cv::Point2f center;
      cv::minEnclosingCircle(cv::Mat(contours.at(i)), center, radius);
      centers->push_back(center);
      if ( draw_image != NULL ) {
        cv::circle(*draw_image,cv::Point(center),
            static_cast<int>(radius),cv::Scalar(0),2);
      }      
    }
    return *centers;
  }

  double CvHelper::GetAngleCosP1P0P2( 
      cv::Point pt1, 
      cv::Point pt0,
      cv::Point pt2) {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
  }

  double CvHelper::GetSqrEuclideanDist(cv::Point pt1, cv::Point pt2) {
    double yd = abs(pt2.y - pt1.y);
    double xd = abs(pt2.x - pt1.x);
    return yd*yd + xd*xd;
  }
}//ns cv_helper
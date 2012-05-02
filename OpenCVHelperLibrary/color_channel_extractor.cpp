#include "cv_helper_lib.h"
#include <exception>

#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace std;

namespace cv_helper{
  // init properties
  ColorChannelExtractor::ColorChannelExtractor() {
    this->is_HSV_ready = false;
    this->is_Lab_ready = false;
    this->is_BGR_ready = false;
    this->proc_image = NULL;
  }
  
  ColorChannelExtractor::~ColorChannelExtractor() {
    this->CleanChannels();
  }

  void ColorChannelExtractor::CleanChannels(){
    for ( std::map<const string, cv::Mat>::iterator itr =
        this->channels.begin(); itr != this->channels.end();
        itr++ ) {
      itr->second = NULL;
    }
    this->channels.clear();
  }

  const cv::Mat& ColorChannelExtractor::get_channels(
      const std::string channel_name) {
    if ( channel_name.length() > 2 ) {
      throw std::invalid_argument("Specified channel name is invalid");
    }
    if ( this->proc_image == NULL ) {
      throw std::invalid_argument("Must invoke ExtractNewImage first");
    }
    if ( (channel_name.compare("R") == 0 ||
          channel_name.compare("G") == 0  ||
          channel_name.compare("B") == 0  ) &&
          !this->is_BGR_ready ) {
      this->ExtractBGRChannels();
    }
    if ( (channel_name.compare("H") == 0  ||
          channel_name.compare("S") == 0  ||
          channel_name.compare("V") == 0 ) &&
          !this->is_HSV_ready ) {
      this->ExtractHSVChannels();
    }
    if ( (channel_name.compare("L") == 0  ||
          channel_name.compare("a") == 0  ||
          channel_name.compare("b") == 0 ) &&
          !this->is_Lab_ready ) {
      this->ExtractLabChannels();
    }
      
    //this->channels
    std::map<const std::string, cv::Mat>::iterator itr = 
        this->channels.find(channel_name);
    if ( itr == this->channels.end() ) {
      throw std::invalid_argument("Specified channel_name is invalid");
    } else {
      return itr->second;
    }
  }

  const cv::Mat& ColorChannelExtractor::get_image_in_colorspace(const std::string colorspace){
    if ( colorspace.length() < 3 ) {
      throw std::invalid_argument("Specified channel name is invalid");
    }
    if ( this->proc_image == NULL ) {
      throw std::invalid_argument("Must invoke ExtractNewImage first");
    }
    if ( colorspace.compare("BGR") == 0  &&
          !this->is_BGR_ready ) {
      this->ExtractBGRChannels();
    }
    if ( colorspace.compare("HSV") == 0  &&
          !this->is_HSV_ready ) {
      this->ExtractHSVChannels();
    }
    if ( colorspace.compare("Lab") == 0  &&
          !this->is_Lab_ready ) {
      this->ExtractLabChannels();
    }
    //this->channels
    std::map<const std::string, cv::Mat>::iterator itr = 
        this->channels.find(colorspace);
    if ( itr == this->channels.end() ) {
      throw std::invalid_argument("Specified colorspace is invalid");
    } else {
      return itr->second;
    }
  }

  bool ColorChannelExtractor::ExtractNewImage(const cv::Mat& image2extract) {
    if ( image2extract.channels() < 3 ) {
      throw std::invalid_argument("Must provide an RGB image");
    }
    this->proc_image = &image2extract;
    this->is_HSV_ready = false;
    this->is_Lab_ready = false;
    this->is_BGR_ready = false;
    this->CleanChannels();
  }

  void ColorChannelExtractor::ExtractLabChannels() {
    if ( this->is_Lab_ready ) {
      return;
    } else {
      cv::Mat lab_image(this->proc_image->size(), this->proc_image->type());
      cv::Mat channel_L(this->proc_image->size(), CV_8U);
      cv::Mat channel_a(this->proc_image->size(), CV_8U);
      cv::Mat channel_b(this->proc_image->size(), CV_8U);
      cv::cvtColor(*(this->proc_image), lab_image, CV_BGR2Lab);
      cv::Mat_<uchar>::iterator itl = channel_L.begin<uchar>(),
                                ita = channel_a.begin<uchar>(),
                                itb = channel_b.begin<uchar>();
      this->channels["Lab"] = lab_image;
      for ( cv::MatIterator_<cv::Vec3b> itr = lab_image.begin<cv::Vec3b>();
          itr != lab_image.end<cv::Vec3b>(); itr++, itl++, ita++, itb++  ) {
        (*itl) = (*itr)[0];
        (*ita) = (*itr)[1];
        (*itb) = (*itr)[2];
      }
      this->channels["L"] = channel_L;
      this->channels["a"] = channel_a;
      this->channels["b"] = channel_b;
      this->is_Lab_ready = true;
    }
  }
  void ColorChannelExtractor::ExtractBGRChannels() {
    throw std::invalid_argument("Not Implemented yet");
  }
  void ColorChannelExtractor::ExtractHSVChannels() {
    if ( this->is_HSV_ready ) {
      return;
    } else {
      cv::Mat hsv_image(this->proc_image->size(), this->proc_image->type());
      cv::Mat channel_H(this->proc_image->size(), CV_8U);
      cv::Mat channel_S(this->proc_image->size(), CV_8U);
      cv::Mat channel_V(this->proc_image->size(), CV_8U);
      cv::cvtColor(*(this->proc_image), hsv_image, CV_BGR2HSV);
      cv::Mat_<uchar>::iterator itl = channel_H.begin<uchar>(),
                                ita = channel_S.begin<uchar>(),
                                itb = channel_V.begin<uchar>();
      this->channels["HSV"] = hsv_image;
      for ( cv::MatIterator_<cv::Vec3b> itr = hsv_image.begin<cv::Vec3b>();
          itr != hsv_image.end<cv::Vec3b>(); itr++, itl++, ita++, itb++  ) {
        (*itl) = (*itr)[0];
        (*ita) = (*itr)[1];
        (*itb) = (*itr)[2];
      }
      this->channels["H"] = channel_H;
      this->channels["S"] = channel_S;
      this->channels["V"] = channel_V;
      this->is_HSV_ready = true;
    }
  }
} // ns cv_helper
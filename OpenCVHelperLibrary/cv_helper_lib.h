#ifndef CV_HELPER_LIB_H_
#define CV_HELPER_LIB_H_

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"



#ifdef _EXPORTINGOCVHL
  #define OCVHL_CLASS_DECLSPEC    __declspec(dllexport)
#else
  #define OCVHL_CLASS_DECLSPEC    __declspec(dllimport)
#endif

//forward declaration
namespace cv{
  class Mat;
};

namespace cv_helper{

  class OCVHL_CLASS_DECLSPEC CvHelper{
    public:
    // get a vector of matrix that contains each channels of image as 1 channel matrix
    static std::vector<cv::Mat> getBGRByChannel(const cv::Mat& image);

    // get a vector of matrix that contains each channels of image as 3 channel matrix 
    //  (R,G,B only image respectively)
    static std::vector<cv::Mat> getBGRImagesByChannel(const cv::Mat& image);

    // receive a rgb vector and show it
    static void imShowRGB(std::vector<cv::Mat> images);

    // show matrix as matrix format, at double or CV_64FC1
    static void show_matrix_fc64(const cv::Mat& matrix);
    
    // return the [l, a, b] channels as each is one channel image, if is_debug
    // then display each channel
    static std::vector<cv::Mat> get_lab_color(const cv::Mat& image, bool is_debug);
  };

  // extractor to extract color channels by its channels that as CV_8U Mat
  // note, currently supoort only Lab
  //  Lab: channel_L, channel_a, channel_b, 
  //  HSV: channel_H, channel_S, channel_V, 
  //  RGB: channel_R, channel_G, channel_B
  class OCVHL_CLASS_DECLSPEC ColorChannelExtractor {
  public:
    cv::Mat channel_L_() { return this->channel_L; }
    cv::Mat channel_a_() { return this->channel_a; }
    cv::Mat channel_b_() { return this->channel_b; }
    cv::Mat channel_H_() { return this->channel_H; }
    cv::Mat channel_S_() { return this->channel_S; }
    cv::Mat channel_V_() { return this->channel_V; }
    cv::Mat channel_R_() { return this->channel_H; }
    cv::Mat channel_G_() { return this->channel_S; }
    cv::Mat channel_B_() { return this->channel_V; }

  private:
    cv::Mat channel_L, channel_a, channel_b, 
            channel_H, channel_S, channel_V, 
            channel_R, channel_G, channel_B;
  };
}//ns cv_helper


#endif //CV_HELPER_LIB_H_
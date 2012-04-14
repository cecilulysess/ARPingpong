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
  };
}//ns cv_helper


#endif //CV_HELPER_LIB_H_
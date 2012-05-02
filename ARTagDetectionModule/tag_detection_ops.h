#ifndef TAG_DETECTION_OPS_H_
#define TAG_DETECTION_OPS_H_

#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"


#ifdef _EXPORTINGARTDM
  #define ARTDM_CLASS_DECLSPEC    __declspec(dllexport)
#else
  #define ARTDM_CLASS_DECLSPEC    __declspec(dllimport)
#endif


//forward declaration
namespace cv {
  class Mat;
}

namespace tag_detection_module{

/*the tag to identify defferent color*/
  enum  cvTagType{
        CV_TAG_RED=1,     //red tag 
	    CV_TAG_GREEN=2,   //green tag
	    CV_TAG_BLUE=3     //blue tag
  };
/*the struct to hold the information of center point of rectangle region*/
   struct cvCenterPoint{
	      cv::Point    center_Point;  //center point info 
          cvTagType    tag_Type;      //tag type info 
   };
/*the class of tag detect */
  class ARTDM_CLASS_DECLSPEC TagDetectionOp {
//public static operation to cope each frame from the camera 
  public:
	/*get the rgb channel binary image and hold them by Mat vector*/
    static std::vector<cv::Mat>  cvColorDetect(const cv::Mat&  src);
	/*cope the signal channel binary image to reduce the noise compact and prominent the rectangle feature */
    static cv::Mat cvHandleBinaryImg(const cv::Mat& src);
	/*cope the arguemented binary image and get the rectangel region*/
    static std::vector<std::vector<cv::Point> > cvGetRectangle(const cv::Mat& src, std::vector<std::vector<cv::Point> >& squares);
	/*calculate the cosine of the angle that struct by the three points we input*/
	static double angle( cv::Point pt1, cv::Point pt2, cv::Point pt0 );
	/*draw the rectangle on the image that you put in*/
    static void cvDrawSquares( cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares );
	/*draw the center point  on the image that you put in*/
    static void cvDrawCenters(cv::Mat& image, const std::vector<cvCenterPoint>& centers );
	/*draw the rectangel region and center point  on the image that you put in*/
    static void cvDrawSourceImg( cv::Mat& src, std::vector<std::vector<cv::Point> > & squares ,std::vector<cvCenterPoint>& centers);
	/*get the rectangle center point information and hold them by  vector of center point struct we define */
    static std::vector<cvCenterPoint> cvGetCenterPoint(cv::Mat& src,const cvTagType& type);
	/*get  the threshold value according to the real time conditon*/
	static int getThresholdS();
	static int getThresholdV();
	static int getThresholdHR_low();
	static int getThresholdHR_high();
	static int getThresholdHG_low();
	static int getThresholdHG_high();
	static int getThresholdHB_low();
	static int getThresholdHB_high();
//private variable with pretermit value to protect the threshold 
  private:                 
    static const int thresh_S = 150;            //the lowest  S values in HSV color space 
    static const int thresh_V = 150;            //the lowest  V values in HSV color space 
    static const int thresh_HR_low  = 5 ;       //the value of H of red color is smaller than thresh_HR_low and larger than thresh_HR_hight
	static const int thresh_HR_high = 125;
	static const int thresh_HG_low  = 55;       //the value of H of green color is between thresh_HR_low and thresh_HR_hight
	static const int thresh_HG_high = 65;
	static const int thresh_HB_low  = 115;      //the value of H of blue color is between thresh_HR_low and thresh_HR_hight
	static const int thresh_HB_high = 125;
  };
//the test function of tag detect module 
int ARTDM_CLASS_DECLSPEC tag_detection_Test(); 
}//ns tag_detection_module

#endif //TAG_DETECTION_OPS_H_
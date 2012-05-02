#ifndef CV_HELPER_LIB_H_
#define CV_HELPER_LIB_H_

#include <vector>
#include <map>

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
    
  };

  // extractor to extract color channels by its channels that as CV_8U Mat
  // note, currently supoort only Lab
  //  Lab: L, a, b
  //  HSV: H, S, V
  //  BGR: B, G, R
  // you can also use  HSV, BGR, Lab
  // to get the converted image
  class OCVHL_CLASS_DECLSPEC ColorChannelExtractor {
  public:
    ColorChannelExtractor();
    ~ColorChannelExtractor();

    // get channel by its name specified in the class comment
    // throw invalid exception if invalid channel_name
    const cv::Mat& get_channels(const std::string channel_name);

    // get image in specified colorspace
    // support HSV, BGR, Lab
    const cv::Mat& get_image_in_colorspace(const std::string colorspace);

    // extract channels from a new image, you can get the 
    // channel result by get_channels method after invoking this
    // method
    bool ExtractNewImage(const cv::Mat& image2extract);


  protected:
    void ExtractLabChannels();
    void ExtractBGRChannels();
    void ExtractHSVChannels();

  private:
    // only clean channels 
    void CleanChannels();

    const cv::Mat* proc_image;
    std::map<const std::string, cv::Mat> channels;
    // tag for lazy init
    bool is_BGR_ready, is_Lab_ready, is_HSV_ready;
  };
}//ns cv_helper


#endif //CV_HELPER_LIB_H_
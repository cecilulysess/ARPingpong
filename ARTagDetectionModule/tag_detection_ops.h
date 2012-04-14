#ifndef TAG_DETECTION_OPS_H_
#define TAG_DETECTION_OPS_H_

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
  class ARTDM_CLASS_DECLSPEC TagDetectionOp {
    public:
    static void DetectTag( cv::Mat& image );
  };
}//ns tag_detection_module

#endif //TAG_DETECTION_OPS_H_
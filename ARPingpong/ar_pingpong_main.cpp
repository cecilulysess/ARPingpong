#include <iostream>
#include <opencv2\highgui\highgui.hpp>

#include "OpenCVHelperLibrary\cv_helper_lib.h"
#include "ARTagDetectionModule\tag_detection_ops.h"
int main() {
  cv::VideoCapture webcam(1);
  //webcam.set(CV_CAP_PROP_FRAME_WIDTH, 320);
  //webcam.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
  
  cv::Mat frame;
  if (!webcam.isOpened()) {
    std::cout << "failed to open video capture device 1\n";
    return 1;
  }
  cv::namedWindow("Input");
  CvMouseCallback onMouse = &cv_helper::CvHelper::PrintPointValueWhenLeftClick;
  //cv::setMouseCallback("Input", onMouse, (void*) &frame);
  tag_detection_module::DummyTagDetector tag_dtc;
  
  while (cv::waitKey(30) == -1) {
    webcam >> frame;    
    cv::imshow("Input", frame);
    tag_dtc.DetectTags(frame);
  }

  
}
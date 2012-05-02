#include <iostream>
#include <opencv2\highgui\highgui.hpp>

#include "OpenCVHelperLibrary\cv_helper_lib.h"
#include "ARTagDetectionModule\tag_detection_ops.h"
int main() {
  cv::VideoCapture webcam(1);

  cv::Mat frame;
  if (!webcam.isOpened()) {
    std::cout << "failed to open video capture device 1\n";
    return 1;
  }
  cv::namedWindow("Input");
  tag_detection_module::DummyTagDetector tag_dtc;
  while (cv::waitKey(30) == -1) {
    webcam >> frame;    
    cv::imshow("Input", frame);
    tag_dtc.DetectTags(frame);
  }

  
}
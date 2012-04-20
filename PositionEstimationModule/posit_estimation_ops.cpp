#include "posit_estimation_ops.h"
#include "library_helper_functions.h"

#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include <iostream>

#include <Windows.h>
#include <tchar.h>

using namespace std;


namespace position_estimation_module{

  // Open chessboard images and extract corner points
  int CameraCalibrator::addChessboardPoints(
        // like c:\\chessboards directory
        const std::string chessboard_image_base_dir,  
        cv::Size & boardSize) {
   
    vector<string> filelist;
    AddAllFilesInDir(filelist, chessboard_image_base_dir + "\\*.jpg");
    // load files at chessboard_image_base_dir
#ifdef _DEBUG
    cout<<"Debug info: Loading chessboards image from "<<chessboard_image_base_dir + "\\*.jpg"<<endl;
    for (int i = 0 ; i < filelist.size(); ++i ){
      cout<<"\tImage: "<<filelist.at(i)<<endl;
    }
#endif

    // the points on the chessboard
    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;
    // 3D Scene Points:
    // Initialize the chessboard corners
    // in the chessboard reference frame
    // The corners are at 3D location (X,Y,Z)= (i,j,0)
    for (int i=0; i<boardSize.height; i++) {
      for (int j=0; j<boardSize.width; j++) {
        objectCorners.push_back(cv::Point3f(i, j, 0.0f));
      }
    }
    // 2D Image points:
    cv::Mat image; // to contain chessboard image
    int successes = 0;
    // for all viewpoints
    for (int i = 0; i < filelist.size(); i++) {
      std::cout<<"process file:" <<filelist.at(i) <<std::endl;
      // Open the image
      image = cv::imread(filelist[i],0);
      // Get the chessboard corners
      bool found = cv::findChessboardCorners(
      image, boardSize, imageCorners);
      cv::drawChessboardCorners(image, boardSize, imageCorners, found);
      cv::waitKey(1000);
      try{
      // Get subpixel accuracy on the corners
        cv::cornerSubPix( image, imageCorners,
                          cv::Size(5,5),
                          cv::Size(-1,-1),
                          cv::TermCriteria(cv::TermCriteria::MAX_ITER +
                          cv::TermCriteria::EPS,
                          30, // max number of iterations
                          0.1)); // min accuracy
      } catch (...) {
        std::cout<<"Error processing"<< filelist.at(i)<<std::endl;
      }
      //If we have a good board, add it to our data
      if (imageCorners.size() == boardSize.area()) {
        // Add image and scene points from one view
        addPoints(imageCorners, objectCorners);
        successes++;
      }
    }
    return successes;
  }

  // Add scene points and corresponding image points
  void CameraCalibrator::addPoints(const std::vector<cv::Point2f>&
      imageCorners, const std::vector<cv::Point3f>& objectCorners) {
    // 2D image points from one view
    imagePoints.push_back(imageCorners);
    // corresponding 3D scene points
    objectPoints.push_back(objectCorners);
  }

  // Calibrate the camera
  // returns the re-projection error
  double CameraCalibrator::calibrate(cv::Size &imageSize) {
    // undistorter must be reinitialized
    this->must_init_undistort= true;
    //Output rotations and translations
    std::vector<cv::Mat> rvecs, tvecs;
    // start calibration
    return calibrateCamera( objectPoints, // the 3D points
                            imagePoints, // the image points
                            imageSize, // image size
                            camera_matrix, // output camera matrix
                            dist_coefficients, // output distortion matrix
                            rvecs, tvecs, // Rs, Ts
                            flag); // set options
  }

  // remove distortion in an image (after calibration)
  cv::Mat CameraCalibrator::remap(const cv::Mat &image) {
    cv::Mat undistorted;
    if (must_init_undistort) { // called once per calibration
      cv::initUndistortRectifyMap(
      camera_matrix, // computed camera matrix
      dist_coefficients, // computed distortion matrix
      cv::Mat(), // optional rectification (none)
      cv::Mat(), // camera matrix to generate undistorted
      image.size(), // size of undistorted
      CV_32FC1, // type of output map
      map1, map2); // the x and y mapping functions
      must_init_undistort= false;
    }
    // Apply mapping functions
    cv::remap(image, undistorted, map1, map2,
    cv::INTER_LINEAR); // interpolation type
    return undistorted;
  }

  void CameraCalibrator::capture_calibration_images(string store_path, int devise_id) {
    cv::namedWindow("Capture Chessboard Images", CV_WINDOW_AUTOSIZE);      
    string base_dir = store_path + "\\";
  
    cv::Mat frame;
    cv::VideoCapture cap(devise_id);

    if (!cap.isOpened()) {
      std::cout << "Failed to open video capture device 0\n";
      return;
    }
    int image_count = 0, capture_count = 0;
    string filename;
    ostringstream convert;
    double fps = 1000.0;
    while (image_count < 30) {
    
      cap >> frame;
      convert << image_count << ".jpg";
      cv::imshow("\tCapture Chessboard Images", frame);
      filename = base_dir +  convert.str() ;
      cout<<"\tWrite file:" << filename <<endl;
      cv::imwrite(filename , frame);
      ++image_count;
      convert.str(std::string());
      if (cv::waitKey(2000) >= 0) {
        break;
      }
    
    }
  }

  std::string WCHAR2string(const std::wstring& s){
    int len;
    int slength = (int)s.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
    char* buf = new char[len];
    WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
    std::string r(buf);
    delete[] buf;
    return r;
  }

  wchar_t *string2wchar_t(const string &str){
      wchar_t wchar[260];
      int index = 0;
      while(index < str.size())
      {
          wchar[index] = (wchar_t)str[index];
          ++index;
      }
      wchar[index] = 0;
      return wchar;
  }

  void AddAllFilesInDir(vector<string>& filenames, string dir_base) {
    WIN32_FIND_DATA FindFileData;
      wchar_t * FileName = string2wchar_t(dir_base);
      HANDLE hFind = FindFirstFile(FileName, &FindFileData);
      string base_dir = dir_base.substr(0, dir_base.rfind('\\') + 1);
    
      filenames.push_back(base_dir + WCHAR2string(FindFileData.cFileName));

      while (FindNextFile(hFind, &FindFileData))
          filenames.push_back(base_dir + WCHAR2string(FindFileData.cFileName));

  }

  

}//ns position_estimation_module

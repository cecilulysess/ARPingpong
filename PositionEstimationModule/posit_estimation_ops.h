#ifndef POSIT_ESTIMATION_OPS_H_
#define POSIT_ESTIMATION_OPS_H_

#include "opencv2\core\core.hpp"

#ifdef _EXPORTINGPEM
  #define PEM_CLASS_DECLSPEC    __declspec(dllexport)
#else
  #define PEM_CLASS_DECLSPEC    __declspec(dllimport)
#endif

//forward declaration
namespace cv{
  class Mat;

};

namespace position_estimation_module{

  class PEM_CLASS_DECLSPEC CameraCalibrator{
  public:
    CameraCalibrator() : flag(0), must_init_undistort(true) { };

    // capture some chessboard image to store it in store_path like
    // c:\\chessboards
    static void capture_calibration_images(std::string store_path, int devise_id);

    // Open chessboard images and extract corner points
    int addChessboardPoints(
        // like c:\\chessboards directory
        const std::string chessboard_image_base_dir,  
        cv::Size & boardSize) ;

    // Calibrate the camera
    // returns the re-projection error
    double calibrate(cv::Size &imageSize);

    // remove distortion in an image (after calibration)
    cv::Mat remap(const cv::Mat &image) ;
    
    // get camera matrix
    cv::Mat camera_matrix_() {return this->camera_matrix; } 

    // get image file name list
    std::vector<std::string> image_filename_list_() {return this->image_filename_list; }

  private:
    // Add scene points and corresponding image points
    void CameraCalibrator::addPoints(const std::vector<cv::Point2f>&
        imageCorners, const std::vector<cv::Point3f>& objectCorners);

    // input points, the points in world coordinates
    std::vector<std::vector<cv::Point3f> > objectPoints;
    std::vector<std::vector<cv::Point2f> > imagePoints;

    // output points
    cv::Mat camera_matrix;
    cv::Mat dist_coefficients;
    // flag to specify how calibration is done
    int flag;
    // used in iamge undistortion
    cv::Mat map1, map2;
    bool must_init_undistort;
    
    std::vector<std::string> image_filename_list;
  };
}//ns position_estimation_module


#endif //POSIT_ESTIMATION_OPS_H_
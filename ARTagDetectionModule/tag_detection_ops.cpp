
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#include "tag_detection_ops.h"
#include "OpenCVHelperLibrary\cv_helper_lib.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

namespace tag_detection_module {
/*
//   This method is to detect the rgb region from the source image ,the detail way  is to change color space  from rgb to hsv in order to reduce 
//   the influence from the  illumination .Then we need   to create a mask from  s,v channel adapt a special threshold and use it  to  cope  H channel.
//   Next we need to segment the  H channel result up to the range of values on different color  and transfor it to binary image   
*/
std::vector<cv::Mat>  TagDetectionOp::cvColorDetect(const cv::Mat&  src){
	std::vector<cv::Mat> Result;
	std::vector<cv::Mat>* channels=new std::vector<cv::Mat>();
	cv::Mat rgb=src.clone();    //clone the source frame
	cv::Mat hsv(rgb);    //hold the hsv channels 
	/*smooth the source image */
	
	/*change the color space from rgb to hsv and get each channel*/
	cv::cvtColor(rgb,hsv,CV_BGR2HSV);
	cv::split(hsv,*channels);
	cv::Mat H=channels->at(0);
	cv::Mat S=channels->at(1);
	cv::Mat V=channels->at(2);
	/*cope the hsv channel and segment the rgb region ,then return back*/
	//define the variable to hold the coped result dure this progress
	cv::Mat sResult(S);
	cv::Mat vResult(V);
	cv::Mat copedH(H);
	cv::Mat SVMask=H.clone();
	cv::Mat HrResult=H.clone();
	cv::Mat HgResult=H.clone();
	cv::Mat HbResult=H.clone();
//the transfor formulate :
//                      V <- max(R,G,B)
//                      S <- (V-min(R,G,B))/V   if V¡Ù0, 0 otherwise
//
//                           (G - B)*60/S,  if V=R
//                      H <- 180+(B - R)*60/S,  if V=G
//                           240+(R - G)*60/S,  if V=B
//
//if H<0 then H<-H+360
//On output 0¡ÜV¡Ü1, 0¡ÜS¡Ü1, 0¡ÜH¡Ü360.
//The values are then converted to the destination data type:
//    8-bit images:
//        V <- V*255, S <- S*255, H <- H/2 (to fit to 0..255)
//    16-bit images (currently not supported):
//        V <- V*65535, S <- S*65535, H <- H
//    32-bit images:
//        H, S, V are left as is
//That means pure r to hsv is: 255,255,0
//           pure g to hsv is: 255,255,60
//		   pure b to hsv is: 255,255,120
	/*segment the source image by s,v channel*/
	cv::threshold(S,sResult,thresh_S,255,CV_THRESH_BINARY);
	cv::threshold(V,vResult,thresh_V,255,CV_THRESH_BINARY);
	SVMask=sResult&vResult;   //create a mask by threshold result in the channel of s and v
	H.copyTo(copedH,SVMask);  //use created mask to cope the h channel
	/*segment the source image to get blue region on the base of coped H channel by H values range */
	cv::threshold(copedH,HbResult,thresh_HB_high,255,CV_THRESH_TOZERO_INV);       // larger than thrshold is 0 and smaller is  not changed
    cv::threshold(HbResult,HbResult,thresh_HB_low,255,CV_THRESH_BINARY);           // larger than threshold is 255 ang smaller is 0
	/*segment the source image to get green region on the base of coped H channel by H values range */
    cv::threshold(copedH,HgResult,thresh_HG_high,255,CV_THRESH_TOZERO_INV);       // larger than thrshold is 0 and smaller is  not changed
    cv::threshold(HgResult,HgResult,thresh_HG_low,255,CV_THRESH_BINARY);           // larger than threshold is 255 ang smaller is 0
	/*segment the source image to get red  region on the base of coped H channel by H values range */
    cv::threshold(copedH,HrResult,thresh_HR_high,255,CV_THRESH_TOZERO_INV);       // larger than thrshold is 0 and smaller is  not changed
    cv::threshold(HrResult,HrResult,thresh_HR_low,255,CV_THRESH_BINARY);           // larger than threshold is 255 ang smaller is 0
    cv::threshold(HrResult,HrResult,254,255,CV_THRESH_BINARY_INV);                 //the source img is binary image and need reserve to get red channel
	/*exclude the pure wite and blace effect*/
	HrResult=HrResult&SVMask;                                                      
	HgResult=HgResult&SVMask; 
	HbResult=HbResult&SVMask; 
	/*return the coped result that are different region on the base of different valuse on hsv color space*/
	Result.push_back(HrResult);  
	Result.push_back(HgResult); 
	Result.push_back(HbResult); 
	delete channels;
	return Result;
}

/*
//  The input para is the binary image as result of color segment result ,and the output is the result we cope by morphology method 
//  to arguement the effect of selected region .In this method ,we need to delete the small dot and the noise interfere 
*/
cv::Mat TagDetectionOp::cvHandleBinaryImg(const cv::Mat& src){
	cv::Mat Result(src);
	cv::Mat element5(5,5,CV_8U,cv::Scalar(1));
    cv::morphologyEx(src,Result,cv::MORPH_OPEN,element5);
	cv::morphologyEx(Result,Result,cv::MORPH_CLOSE,element5);
	return Result;
}

/*
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
*/
double TagDetectionOp::angle( cv::Point pt1, cv::Point pt2, cv::Point pt0 ){
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/*
// The input para is the result binary image after handled ,and the output  is the detected info that have detect rectangle in the binary image
// In this method we need to use the corner detect that means the corner whose cos  of a angle is 0 we define it as a   right-angle
*/
//
std::vector<std::vector<cv::Point> > TagDetectionOp::cvGetRectangle(const cv::Mat& src, std::vector<std::vector<cv::Point> >& squares){
     squares.clear();
     cv::Mat gray=src.clone();
     std::vector<std::vector<cv::Point> > contours;           //the temp variable to hold all contours in binary image
     std::vector<std::vector<cv::Point> > resultContours;     //the detect result from binary image and hold final rectangle contour
     // find contours and store them all as a list
     cv::findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
     vector<cv::Point> approx;
     // test each contour
     for( int i = 0; i < contours.size(); i++ ){
     // approximate contour with accuracy proportional to the contour perimeter
     approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
     // square contours should have 4 vertices after approximation relatively large area (to filter out noisy contours) and be convex.
     // Note: absolute value of an area is used because area may be positive or negative - in accordance with the contour orientation
     if( approx.size() == 4 /*&& fabs(cv::contourArea(cv::Mat(approx))) > 1000*/ &&cv::isContourConvex(cv::Mat(approx))){
        double maxCosine = 0;
        for( int j = 2; j < 5; j++ ){
             // find the maximum cosine of the angle between joint edges
             double cosine = fabs(TagDetectionOp::angle(approx[j%4], approx[j-2], approx[j-1]));
             maxCosine = MAX(maxCosine, cosine);
             }
        // if cosines of all angles are small (all angles are ~90 degree) then write quandrange vertices to resultant sequence
        if( maxCosine <=0.1 ){
          squares.push_back(approx);
          resultContours.push_back(contours[i]);
       }
    }
  }
  return resultContours;
}

/*
//Get center point of detected rectangle region and draw it on source image
*/
std::vector<cvCenterPoint> TagDetectionOp::cvGetCenterPoint(cv::Mat& src,const cvTagType& type){
     std::vector<cvCenterPoint> centerPoints;
     std::vector<std::vector<cv::Point> > squares;
     std::vector<std::vector<cv::Point> > contours;
     contours=TagDetectionOp::cvGetRectangle(src,squares);            //get the rectangle contour
     TagDetectionOp::cvDrawSquares(src,squares);

     // iterate over all contours
     std::vector<std::vector<cv::Point> >::iterator  itc;
     itc= contours.begin();
     while (itc!=contours.end()) {
            cv::Moments mom= cv::moments(cv::Mat(*itc++)); // compute all moments
            cvCenterPoint centerPoint;                     // get mass center
            centerPoint.center_Point=cv::Point(mom.m10/mom.m00,mom.m01/mom.m00);
            centerPoint.tag_Type=type;
            centerPoints.push_back(centerPoint);
     }

     return centerPoints;
}

/*
// the function draws all the squares in the image
*/
void TagDetectionOp::cvDrawSquares(cv::Mat& image, const std::vector<std::vector<cv::Point> >& squares ){
    for( int i = 0; i < squares.size(); i++ ){
        const cv::Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true,cv::Scalar(0,255,0), 1, CV_AA);
    }
}

/*
// the function draws all the center point in the image
*/
void TagDetectionOp::cvDrawCenters(cv::Mat& image, const std::vector<cvCenterPoint>& centers ){
    for( int i = 0; i < centers.size(); i++ ){
		const cv::Point p = centers.at(i).center_Point;
        cv::circle(image,p,0.5,cv::Scalar(0),0.5);
    }
}

/*
//draw the rectangle and the center point on the source image
*/
void TagDetectionOp::cvDrawSourceImg( cv::Mat& src, std::vector<std::vector<cv::Point> > & squares ,std::vector<cvCenterPoint>& centers){
	     
	TagDetectionOp::cvDrawSquares(src,squares); // draw the rectangle region
    TagDetectionOp::cvDrawCenters(src,centers); // draw the center point  
}

/*get and set the threshold values of hsv channel according to the real time  need*/
int TagDetectionOp::getThresholdS(){
	return TagDetectionOp::thresh_S;
}
int TagDetectionOp::getThresholdV(){
	return TagDetectionOp::thresh_V;
}
int TagDetectionOp::getThresholdHR_low(){
	return TagDetectionOp::thresh_HR_low;
}
int TagDetectionOp::getThresholdHR_high(){
	return TagDetectionOp::thresh_HR_high;
}
int TagDetectionOp::getThresholdHG_low(){
	return TagDetectionOp::thresh_HG_low;
}
int TagDetectionOp::getThresholdHG_high(){
	return TagDetectionOp::thresh_HG_high;
}
int TagDetectionOp::getThresholdHB_low(){
	return TagDetectionOp::thresh_HB_low;
}
int TagDetectionOp::getThresholdHB_high(){
	return TagDetectionOp::thresh_HB_high;
}

}
/////////////////////////////////////////////////////////////////////
// Name: atag.cpp
// Author: Shaojie Wang
// Date: 10/30/2017
// Description: 
/////////////////////////////////////////////////////////////////////

//using namespace AprilTags;
using namespace std;

#include <vector>
#include "apriltags/TagDetector.h"
#include "apriltags/Tag36h11.h"
#include "apriltags/atag_itf.h"
#include "opencv2/opencv.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>

AprilTags::TagDetector* ptagDetector = NULL;
bool atag_detect_(cv::Mat& image_gray, 
    int& id, double& orientation, double& cx, double& cy);
void dump_result(cv::Mat& image_gray, AprilTags::TagDetection& detection);

bool DEBUG = false;
//bool DEBUG = true;

// Initialize tag detector
void atag_init_()
{
    AprilTags::TagCodes tag_codes(AprilTags::tagCodes36h11);
    if (ptagDetector) {
        delete ptagDetector;
    }
    ptagDetector = new AprilTags::TagDetector(tag_codes);
}

void atag_cleanup_()
{
    if (ptagDetector) {
        delete ptagDetector;
        ptagDetector = NULL;
    }
}

int atag_matrix_detect_(int width, int height, unsigned char *data, int* id,
    double* orientation, double* cx, double* cy)
{
    cv::Mat image_gray(height, width, CV_8U, data);
    bool ok = atag_detect_(image_gray, *id, *orientation, *cx, *cy);
    if (ok) {
        return 1;
    }
    return 0;
}

// return true if succeeds
bool atag_detect_(cv::Mat& image_gray, int& id, double& orientation, double& cx, double& cy) 
{
    if (!ptagDetector) {
        return false;
    }
    vector<AprilTags::TagDetection> detections = ptagDetector->extractTags(image_gray);
    if (detections.size() != 1) {
        return false;
    }

    AprilTags::TagDetection& detection = detections[0];
    if (!detection.good) {
        // This is just a sanity check. All detection is the vector detections
        // should be good. Something is wrong if code reaches here.
        return false;
    }
    id = detection.id;
    cx = detection.cxy.first;
    cy = detection.cxy.second;
    orientation = detection.getCodeOrientation();
    if (DEBUG) {
        std::cout << "Best rotation: " << detection.rotation
            << ", hamming dis = " << detection.hammingDistance
            << endl;

        dump_result(image_gray, detection);    
    }
    return true;
}

void dump_result(cv::Mat& image_gray, AprilTags::TagDetection& detection)
{
    cv::Mat cimg;
    cv::cvtColor(image_gray, cimg, CV_GRAY2BGR);

    std::pair<float, float> p0 = detection.interpolate(-1,-1);
    std::pair<float,float> p1 = detection.interpolate(1,-1);
    std::pair<float,float> p2 = detection.interpolate(1,1);
    std::pair<float,float> p3 = detection.interpolate(-1,1);

    cv::line(cimg, cv::Point2f(p0.first, p0.second), cv::Point2f(p1.first, p1.second), cv::Scalar(0,0,255,0) );
    cv::line(cimg, cv::Point2f(p1.first, p1.second), cv::Point2f(p2.first, p2.second), cv::Scalar(0,0,255,0) );
    cv::line(cimg, cv::Point2f(p2.first, p2.second), cv::Point2f(p3.first, p3.second), cv::Scalar(0,0,255,0) );
    cv::line(cimg, cv::Point2f(p3.first, p3.second), cv::Point2f(p0.first, p0.second), cv::Scalar(0,0,255,0) );

    cv::circle(cimg, cv::Point2f(p0.first, p0.second), 3, cv::Scalar(255,0, 0,0), 3);
    cv::circle(cimg, cv::Point2f(p1.first, p1.second), 3, cv::Scalar(255,0, 0,0), 1);
    cv::circle(cimg, cv::Point2f(p2.first, p2.second), 3, cv::Scalar(0, 255, 0,0), 3);
    cv::circle(cimg, cv::Point2f(p3.first, p3.second), 3, cv::Scalar(0, 255, 0,0), 1);

    float dy = -p2.second+p0.second;
    float dx = p2.first-p0.first;
    std::cout << "p0.x = " << p0.first << ", p0.y = " << p0.second << std::endl;
    std::cout << "p2.x = " << p2.first << ", p2.y = " << p2.second << std::endl;
    std::cout << "dx = " << dx << ", dy = " << dy << std::endl;
    std::cout << "orientation = " << atan2(dy, dx) << ", degree = " << atan2(dy, dx)*180/3.1415926 << std::endl;

    cv::imshow("result", cimg);
    cv::waitKey(0);
}

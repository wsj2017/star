#include <algorithm>
#include <cmath>
#include <climits>
#include <map>
#include <vector>
#include <iostream>

#include <Eigen/Dense>

#include "apriltags/TagDetector.h"
#include "apriltags/TagDetectionUtils.h"

using namespace std;

//#define ATAG_DEBUG

namespace AprilTags {
	
std::vector<TagDetection> TagDetector::extractTags(const cv::Mat& image) const {

	// convert to internal AprilTags image (todo: slow, change internally to OpenCV)
	int width = image.cols;
	int height = image.rows;
	
	std::pair<int,int> opticalCenter(width/2, height/2);
	cv::Mat fimOrigCV;
	image.convertTo( fimOrigCV, CV_32FC1, 1.0/255.0 );
	
	//================================================================
	// Step one: preprocess image (convert to grayscale) and low pass if necessary
	
	//================================================================
	// Step two: Compute the local gradient. We store the direction and magnitude.
	// This step is quite sensitve to noise, since a few bad theta estimates will
	// break up segments, causing us to miss Quads. It is useful to do a Gaussian
	// low pass on this step even if we don't want it for encoding.
	cv::Mat fimCV, fimSegCV, fimThetaCV, fimMagCV;
	preprocess_image( fimOrigCV, fimCV, fimSegCV, fimThetaCV, fimMagCV );
	
	//================================================================
	// Step three. Extract edges by grouping pixels with similar
	// thetas together. This is a greedy algorithm: we start with
	// the most similar pixels.  We use 4-connectivity.
	UnionFindSimple uf( fimSegCV.cols*fimSegCV.rows );
	extract_edges( fimSegCV, fimMagCV, fimThetaCV, uf );
	
	// Steps 4-5
	std::vector<Segment> segments;
	fit_segments( fimSegCV, fimMagCV, fimThetaCV, uf, segments );
#ifdef ATAG_DEBUG
    cout << "Segments size: " << segments.size() << endl;
    cv::Mat debug_img = image.clone();
    cv::cvtColor(debug_img, debug_img, cv::COLOR_GRAY2BGR);
    int idx = 0;
    for(auto iter = segments.begin(); iter != segments.end(); iter++) {
        Segment seg =  *iter;
        cout << "Seg [" << idx++ << "]: P0(" << seg.getX0() << ", "
            << seg.getY0() << ") P1(" << seg.getX1() << ", "
            << seg.getY1() << ")" 
            << " Length = " << seg.getLength() 
            << " Theta = " << seg.getTheta() << endl;

        if ((seg.getLength() >44) && (seg.getLength()<66)) {
            cv::line(debug_img, cv::Point(seg.getX0(),  seg.getY0()), 
                cv::Point(seg.getX1(), seg.getY1()), cv::Scalar(0,0,255));
        } else {
            cv::line(debug_img, cv::Point(seg.getX0(),  seg.getY0()), 
                cv::Point(seg.getX1(), seg.getY1()), cv::Scalar(255,0,0), 3);
        }
        //cv::imshow("segment", debug_img);
        //cv::waitKey(0);
    }
    cv::imshow("segment", debug_img);
    cv::waitKey(0);
#endif

	// Steps 6-7
	std::vector<Quad> quads;
	find_quads( segments, fimOrigCV.size(), opticalCenter, quads );

#ifdef ATAG_DEBUG
    cout << "Quads detected: " << quads.size() << endl;
#endif

	//================================================================
	// Step eight. Decode the quads. For each quad, we first estimate a
	// threshold color to decide between 0 and 1. Then, we read off the
	// bits and see if they make sense.
	//================================================================
	//Step nine: Some quads may be detected more than once, due to
	//partial occlusion and our aggressive attempts to recover from
	//broken lines. When two quads (with the same id) overlap, we will
	//keep the one with the lowest error, and if the error is the same,
	//the one with the greatest observed perimeter.
	return decode_quads( fimCV, quads, thisTagFamily );
}

} // namespace

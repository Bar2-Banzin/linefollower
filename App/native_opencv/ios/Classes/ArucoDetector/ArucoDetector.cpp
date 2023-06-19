#include <opencv2/imgproc.hpp>
#include "ArucoDetector.h"

using namespace std;
using namespace cv;

void ArucoDetector::set_image_lines(Mat image) {
    image_lines=image;
}

void ArucoDetector::set_start_end_points(vector<Vec4i> lines) {
	start_end_points=lines;
}

Mat ArucoDetector::get_image_lines()
{
	return image_lines;
}

vector<Vec4i> ArucoDetector::get_start_end_points()
{
	return start_end_points;
}
int ArucoDetector::inc()
{

	return ++indx;
}



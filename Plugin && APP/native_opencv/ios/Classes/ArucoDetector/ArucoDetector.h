#pragma once
#include <opencv2/core.hpp>

using namespace std;
using namespace cv;

class ArucoDetector {
public:
	void set_image_lines(Mat image);
	void set_start_end_points(vector<Vec4i> lines);
	Mat get_image_lines();
	vector<Vec4i>  get_start_end_points();
    int inc();
private:
	Mat image_lines;
	vector<Vec4i>start_end_points;
	int indx=0;

};

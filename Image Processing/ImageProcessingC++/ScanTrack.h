#ifndef SCANTRACK_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define SCANTRACK_H

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file

using namespace cv;
using namespace std;

bool scan_track(Mat& image_lines,Mat& transformation_matrix, vector<Vec4i>& start_end_points, Mat& track_image);
bool extract_paper(Mat& warped_image, Mat& transformation_matrix, Mat& img_bgr, bool draw = false);
void extract_lines(Mat& image_lines, vector<Vec4i>& start_end_points,Mat& image, double rho = 1, double  theta = 1 * CV_PI / 180, int threshold = 100, double minLineLength = 100, double  maxLineGap = 50, int thickness = 20);
#endif


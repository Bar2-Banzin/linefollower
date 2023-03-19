#ifndef PLAYER_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define PLAYER_H


#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file

//#include "utils.cpp";
using namespace cv;
using namespace std;


void scan_track(bool& wrapped, Mat& image_lines, vector<Vec4i>& start_end_points,Mat image, int thickness = 20);
void extract_lines(Mat& image_lines, vector<Vec4i>& start_end_points,Mat image, double rho = 1, double  theta = 1 * CV_PI / 180, int threshold = 100, double minLineLength = 100, double  maxLineGap = 50, int thickness = 20);


#endif


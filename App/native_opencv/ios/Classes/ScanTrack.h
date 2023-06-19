#ifndef SCANTRACK_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define SCANTRACK_H

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file

using namespace cv;
using namespace std;

bool scan_track(Mat& image_lines, Mat& track_image,Size s=Size(3,3));
void extract_lines(Mat& image_lines, Mat& image, Size s=Size(3,3),int sliding = 50, double rho = 1, double  theta = 1 * CV_PI / 180, int threshold = 170, double minLineLength = 80, double  maxLineGap = 70, int thickness = 20);
#endif


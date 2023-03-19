#ifndef UTILS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UTILS_H


#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file

using namespace cv;
using namespace std;

Mat thin_image(Mat image);
void color_center(bool& found, int& x, int& y, Mat image, Vec3b color);
Vec2i direction(int x1, int y1, int x2, int y2);
float calculateDistance(int x1, int y1, int x2, int y2);

#endif


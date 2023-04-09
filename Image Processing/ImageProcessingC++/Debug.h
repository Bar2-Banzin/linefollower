#pragma once
#ifndef UTILS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UTILS_H


#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file
#include <algorithm>

using namespace cv;
using namespace std;

Mat thin_image(Mat image);

#endif


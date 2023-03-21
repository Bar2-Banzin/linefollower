#ifndef UTILS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UTILS_H


#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file
#include <algorithm>

using namespace cv;
using namespace std;

Mat thin_image(Mat image);

bool color_center(int& x, int& y, Mat image, Scalar color);
bool color_mask(Mat& mask, Mat& masked_image, Mat image, Scalar color);
bool color_range(Scalar& lower_range, Scalar& upper_range, Scalar color);

//Distance and Direction
void  min_rectangle(Mat& image, int& x, int& y, vector<Point> contour, Scalar color = Scalar(255, 0, 0), int thickness = 10, bool draw = false);
Vec2i direction(int x1, int y1, int x2, int y2);
float calculateDistance(int x1, int y1, int x2, int y2);

//Contour
bool compareContourAreas(vector<Point> contour1, vector<Point> contour2);
void get_biggest_rectangular_contour(vector<Point>& biggest_contour, double& max_area, vector<vector<Point>> contours);
vector<Point2f> reorderPoints(vector<Point>points);
#endif


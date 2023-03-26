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
double calculateDistance(double x1, double y1, double x2, double y2);
Vec2i direction(double x1, double y1, double x2, double y2);
int sign(double x);

//Contour
bool compareContourAreas(vector<Point> contour1, vector<Point> contour2);
void get_biggest_rectangular_contour(vector<Point>& biggest_contour, double& max_area, vector<vector<Point>> contours);
vector<Point2f> reorderPoints(vector<Point>points);
#endif


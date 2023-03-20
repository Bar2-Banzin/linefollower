#ifndef SCANCAR_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define SCANCAR_H

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file

using namespace cv;
using namespace std;

bool find_car(int& x_center, int& y_center, int& x_f, int& y_f, int& x_b, int& y_b, Mat image, Scalar front_color, Scalar back_color);
void car_on_line(bool& on_line, int& line_index ,int x_car, int y_car, Mat lines_matrix, int threshold=1);
void increase_decrease_speed(bool& inc_speed, int x_car_front, int y_car_front, int x_car_back, int y_car_back, Vec4i line, int dist_threshold);

#endif
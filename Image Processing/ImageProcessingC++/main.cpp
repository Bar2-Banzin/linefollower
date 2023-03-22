#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file

#include "ScanTrack.h";
#include "ScanCar.h";
#include "utils.h";

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	//Step(1) Scan Track 
	//1.Read Track Image
	std::string path = "./assets/track/1.png";
	Mat image = imread(path, 1); //Reading from a path

	cout << "Size" << typeid(image.size()).name() << endl;
	/*imshow("Original Track main.cpp", image);*/

	//2.Scanning Track Initially
	Mat image_lines;
	vector<Vec4i>start_end_points;
	bool wrapped=scan_track(image_lines, start_end_points, image);

	if (!wrapped) {
		cout << "Failed To Scan Track" << endl;
		return -1;
	}

	///************************************************************************************************/
	////Step(2) Find Car on Track
	////1.Read an Car on Track Image
	//std::string path2 = "./assets/ontrack/0.jpeg";
	//Mat car_image = imread(path2, 1); //Reading from a path

	////cout << "size" << typeid(image.size()).name() << endl;
	////imshow("Original Car main.cpp", image);
	// 
	//bool car_found;
	//int x_center, y_center, x_f, y_f, x_b, y_b;
	//Scalar front_color(255,0,0);//Red
	//Scalar back_color(0, 255, 0);//Green
	//car_found=find_car(x_center, y_center, x_f, y_f, x_b, y_b, car_image,front_color,back_color);

	//if (!car_found) {
	//	cout << "Failed to Find Car 😟" << endl;
	//	return -1;
	//}

	///************************************************************************************************/
	////Step(3) Is Car on a straight line
	////int x_center = 20;
	////int y_center = 700;
	//bool on_line;
	//int line_index;
	//car_on_line(on_line, line_index, x_center, y_center, image_lines);
	//if (!on_line) {
	//	cout << "Car isn't on a straight line" << endl;
	//	return 0;
	//}
	//cout << "Car is on a straight line" << endl;

	/************************************************************************************************/
	//Step(4) Inc or Dec Speed
	/*bool inc_speed=false;
	int dist_threshold = 10;
	Vec4i line = start_end_points[line_index];
	//
	*/

	////Test Only Till Madbouly Finishes
	//bool inc_speed = false;
	//int dist_threshold = 400;
	//int x_f = 50, y_f = 200, x_b = 50, y_b = 250;
	//Vec4i lineto = start_end_points[1];
	//line(image, Point(x_f, y_f), Point(x_b, y_b), Scalar(0, 255, 255), 10);

	//imshow("Original Track main.cpp", image);
	//
	//inc_speed = increase_decrease_speed(x_f, y_f, x_b, y_b, lineto, dist_threshold);

	//if (!inc_speed) {
	//	cout << "Don't Inc Speed of the Car" << endl;
	//	return 0;
	//}
	//cout << "Inc Speed of the Car" << endl;
	///**********************************************************************************************/
	////--------- MATRIX DEFINITION
	//cv::Mat mat0 = Mat::zeros(5, 2, CV_8UC1); //ones, eye
	//cout << "mat0 = " << endl << " " << mat0 << endl << endl; //Display in shell

	////Creating different types of images: Mat name(rows, cols, type, Scalar);
	//Mat mat1(50, 20, CV_8UC1, Scalar(0));  //Create a 50 x 20 single-channel array with 8 bit unsigned numbers
	////(pixels value = 0)
	//Mat mat2(Size(80, 40), CV_8UC1, Scalar(127)); //Note: Size(width/cols,height/rows)
	//Mat mat3(Size(50, 100), CV_8UC3, Scalar(0, 0, 255)); //Scalar = color (BGR). Scalar represents a 4-element vector

	//Scalar my_color(255, 0, 0);
	//Mat mat4(Size(50, 100), CV_8UC3, my_color);

	//imshow("Display mat1", mat1);
	//imshow("Display mat2", mat2);
	//imshow("Display mat3", mat3);
	//imshow("Display mat4", mat4);
	waitKey(0); //Wait for any key press = pause

	destroyAllWindows(); //Close all windows

	return 0;
}

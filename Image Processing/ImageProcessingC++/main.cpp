#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file

#include "ScanTrack.h";
#include "ScanCar.h";
#include "utils.h";
#include "common.h";

using namespace cv;
using namespace std;

int testcase = 9;

int main(int argc, char** argv)
{

	bool found;
	int x, y;
	std::string path33 = "./assets/TestCases/TestCase" + std::to_string(testcase) + "/car.jpeg";
	Mat image33 = imread(path33, 1); //Reading from a path


	//cout << "Type" << image33.type() << endl;
	//cout << "channels" << image33.channels() << endl;

	//cout << image33 << endl;
	//152,  68,  50
	//auto scaler1 = (int)image33.at<uchar>(0,150, 150);
	//auto scaler2 = (int)image33.at<uchar>(1,150, 150);
	//auto scaler3= (int)image33.at<uchar>(2,150, 150);
	//cout << scaler1 << " " << scaler2 << " " << scaler3 << endl;


	//convert to rgb scale
	Mat image_rgb;
	cvtColor(image33, image_rgb, COLOR_BGR2RGB);
		
	found = color_center(x, y, image_rgb, Scalar(0,0,255), "blue");
	if (!found) {
		cout << "find_car():Couldn't find front of the car" << endl;
		return false;
	}

	return 0;
	 

	//Step(1) Scan Track 
	//1.Read Track Image
	std::string path = "./assets/TestCases/TestCase"+ std::to_string(testcase) +"/track.jpeg";
	Mat image = imread(path, 1); //Reading from a path

	cout << "Size" << typeid(image.size()).name() << endl;
	
	//2.Scanning Track Initially
	Mat image_lines;
	bool wrapped = scan_track(image_lines, image);

	if (!wrapped) {
		cout << "Failed To Scan Track" << endl;
		return -1;
	}

	//namedWindow("image_lines main()", WINDOW_NORMAL);
	//imshow("image_lines main()", image_lines);
	imwrite("./assets/TestCases/TestCase"+ std::to_string(testcase) +"/results/image_lines.jpeg", image_lines);
	//waitKey(0);

	//return 0;


	/************************************************************************************************/
	//step(2) find car on track
	//1.read an car on track image
	std::string path2 = "./assets/TestCases/TestCase" + std::to_string(testcase) + "/car.jpeg";
	Mat car_image = imread(path2, 1); //reading from a path

	bool car_found;
	int x_center, y_center, x_f, y_f, x_b, y_b;
	//Scalar front_color(247, 85, 109);//red
	//Scalar back_color(50, 68, 152);//blue
	//
	Scalar front_color(255, 0, 0);//red
	Scalar back_color(0, 255, 0);//blue
	car_found=find_car(x_center, y_center, x_f, y_f, x_b, y_b, car_image,front_color,back_color);

	if (!car_found) {
		cout << "failed to find car 😟" << endl;
		return -1;
	}

	//To see Image Car scan_car()
	// 
	//return 0;
	///************************************************************************************************/
	//Step(3) Is Car on a straight line
	bool on_line;
	
	//Debug [Comment]
	Mat draw_car_online = image_lines.clone();
	cv::line(draw_car_online, Point(0, 0), Point(x_f, y_f), Scalar(255, 0, 0), 5);
	cv::line(draw_car_online, Point(0, 0), Point(x_b, y_b), Scalar(0, 255, 255), 5);

	//namedWindow("car_on_line main()", WINDOW_NORMAL);
	//imshow("car_on_line main()", image_lines);
	imwrite("./assets/TestCases/TestCase"+ std::to_string(testcase) +"/results/car_on_line.jpeg", draw_car_online);
	//waitKey(0);

	car_on_line(on_line, x_f, y_f, x_b, y_b, image_lines,100);
	if (!on_line) {
		cout << "Car isn't on a straight line" << endl;
		return 0;
	}
	cout << "Car is on a straight line" << endl;
	return 0;

	/*/************************************************************************************************/
	//Step(4) Inc or Dec Speed
	/*bool inc_speed=false;
	int dist_threshold = 10;
	Vec4i line = start_end_points[line_index];

	
	bool inc_speed = false;
	int dist_threshold = calculateDistance(x_f, y_f, x_b, y_b)*2;
	//int x_f = 50, y_f = 200, x_b = 50, y_b = 250;
	Vec4i lineto = start_end_points[line_index];
	//line(image, Point(x_f, y_f), Point(x_b, y_b), Scalar(0, 255, 255), 10);
	//cv::line(image, Point(lineto[0], lineto[1]), Point(lineto[2], lineto[3]), Scalar(255, 0, 255), 5);

	//imshow("Original Track main.cpp", image);
	
	//inc_speed = increase_decrease_speed(x_f, y_f, x_b, y_b, lineto, dist_threshold);
	//imshow("Step 4 main.cpp", image);
	//waitKey(0);


	//imshow("Original Track main.cpp", image);
	Mat draw_temp = car_image.clone();
	inc_speed = increase_decrease_speed(draw_temp, x_f, y_f, x_b, y_b, lineto, dist_threshold);

	if (!inc_speed) {
		cout << "Don't Inc Speed of the Car" << endl;
		return 0;
	}
	cout << "Inc Speed of the Car" << endl;
	*/
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
	//waitKey(0); //Wait for any key press = pause

	destroyAllWindows(); //Close all windows

	return 0;
}

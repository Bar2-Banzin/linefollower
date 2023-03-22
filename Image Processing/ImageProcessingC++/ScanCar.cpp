# include"utils.h"
# include "ScanTrack.h";
#include <opencv2/core/mat.hpp>
#include <set>

//Basma :Not sure of Data Type of front_color- back_color check
bool find_car(int& x_center, int& y_center, int& x_f, int& y_f, int& x_b, int& y_b, Mat image_input, Scalar front_color, Scalar back_color) {
	/**
	* This function is used to find car in the picture
	*
	* @param boolean to detect wether car is found or not
	* @param x_center,y_center
	* @param x_f,y_f
	* @param x_b,y_b
	*
	* @param image: BGR img with car
	* @param front_color: front color of the car RGB color i.e[0, 255, 0]
	* @param back_color: back color of the car RGB color i.e[0, 255, 0]
	*/

	//1.Extract Track Paper From the Image
	Mat image;
	bool wrapped = extract_paper(image_input, image, false);

	if (!wrapped) {
		return false;
	}

	imshow("Wrapped Paper  find_car()", image);

	//Temp Till Tomorrow
	image = image_input;


	//Convert BGR to RGB
	Mat image_rgb;
	cvtColor(image, image_rgb, COLOR_BGR2RGB);

	//Detect front of the car
	//image isn't modified here 😊
	bool found;
	found = color_center(x_f, y_f, image_rgb, front_color);
	if (!found) {
		cout << "find_car():Couldn't find front of the car" << endl;
		return false;
	}

	//Detect back of the car
	//image isn't modified here 😊
	found = color_center(x_b, y_b, image_rgb, back_color);
	if (!found) {
		cout << "find_car():Couldn't find back of the car" << endl;
		return false;
	}

	line(image, Point(x_f, y_f), Point(x_b, y_b), Scalar(0, 255, 255), 10);
	imshow("find_car", image);

	//Car Center
	x_center = (x_f + x_b) / 2;
	x_center = (y_f + y_b) / 2;
	return true;
}

void car_on_line(bool& on_line, int& line_index, int x_car, int y_car, Mat lines_matrix, int threshold) {
	
	
	/*set<int>s;
	for (int i = 0; i < lines_matrix.rows; i++) {
		for (int j = 0; j < lines_matrix.cols; j++) {
			s.insert((int)lines_matrix.at<uchar>(i, j));
		}
	}
	for (auto m : s)cout << m << " ";*/

	int size_i = lines_matrix.rows;
	int size_j = lines_matrix.cols;
	int windo_size = threshold;
	int count = 0;
	for (int i = y_car - windo_size; i <= y_car + windo_size; i++) {
		if (i < 0 || i >= size_i)continue;
		for (int j = x_car - windo_size; j <= x_car + windo_size; j++) {
			if (j < 0 || j >= size_j)continue;
			count += ((int)lines_matrix.at<uchar>(i, j) != 0);
		}
	}
	on_line = (count >= threshold);
	line_index = (on_line)?(int)lines_matrix.at<uchar>(y_car, x_car):-1;
	
	/**
	* This function detrmines whether car is on a straight line or not
	*
	* @param on_line boolean to detect wether car is on a st line or not
	* @param line_index line index on which car is on

	* @param x_car, y_car : Center of the Car
	* @lines_matrix : Binary Matrix with 1's = lines
	* @threshold : min sum to consider Car on line[With the uncommented part to make Region for the Car]
	*/
}

bool increase_decrease_speed(double x_car_front, double  y_car_front, double  x_car_back, double y_car_back, Vec4i line, double dist_threshold) {
	/**
	* Control wether Inc or Dec Car Spped Depending on distance form car and end of the St line
	*
	* @param x_car_front, y_car_front : front Center of the Car
	* @param x_car_back, y_car_back : back Center of the Car
	* @param line : [x1, y1, x2, y2] : start and end point of the line
	* @param dist_threshold:thershold to depend on to take action of speed :D
	*
	* @return Boolean True = > increase False Decrease[FIXME:To Be Modified Later to Send Speed Depending on Distance not only a flag 😉]
	*
	*/

	//1. Get Car Direction[always from back to front]
	Vec2i car = direction(x_car_back, y_car_back, x_car_front, y_car_front);
	cout << "car Vector" << car << endl;

	double line_point[] = { 0, 0 };

	//2. Direction P1P2
	Vec2i P1P2 = direction(line[0], line[1], line[2], line[3]);
	cout << "P1P2" << P1P2 << endl;
	if (sign(P1P2[0]) == sign(car[0]) && sign(P1P2[1]) == sign(car[1])) {
		cout << "Car is Moving towards P2" << endl;
		line_point[0] = line[2];
		line_point[1] = line[3];
	}
	//3. Direction P2P1
	Vec2i P2P1 = direction(line[0], line[1], line[2], line[3]);
	cout << "P2P1" << P2P1 << endl;

	if (sign(P2P1[0]) == sign(car[0]) and sign(P2P1[1]) == sign(car[1])) {
		cout << "Car is Moving towards P1" << endl;
		line_point[0] = line[0];
		line_point[1] = line[1];
	}
	//  # 4. Take Action Depending on Distance between Car and the endpoint
	if (calculateDistance(x_car_front, y_car_front, line_point[0], line_point[1]) > dist_threshold){
		//Increase Speed
		cout << "Increase Speed" << endl;
		return true;
	}
	else{
		//Don't Increase Speed you are toward the line En
		cout << "Decrease Speed" << endl;
		return false;
	}
}
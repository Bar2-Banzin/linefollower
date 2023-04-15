# include"utils.h"
# include "ScanTrack.h"
#include <opencv2/core/mat.hpp>
#include <set>


bool find_car(int& x_center, int& y_center, int& x_f, int& y_f, int& x_b, int& y_b, Mat& image, Scalar front_color, Scalar back_color) {
	/**
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
	Mat paper_img;
	/*bool wrapped = extract_paper(paper_img, image, "car");

	if (!wrapped) {
		//cout << "Couldn't extract paper" << endl;
		return false;
	}*/

	//Uncomment to Disable extract_paper
	paper_img = image;

	//2.Find car centerimage
	//Convert BGR to RGB
	Mat image_rgb;
	cvtColor(paper_img, image_rgb, COLOR_BGR2RGB);

	//detect front of the car
	//image isn't modified here 😊
	bool found;
	found = color_center(x_f, y_f, image_rgb, front_color, "front");
	if (!found) {
		// Couldn't find front of the car
		return false;
	}

	//Detect back of the car
	//image isn't modified here 😊
	found = color_center(x_b, y_b, image_rgb, back_color, "back");
	if (!found) {
		// find_car():Couldn't find back of the car
		return false;
	}

	//car center
	x_center = (x_f + x_b) / 2;
	y_center = (y_f + y_b) / 2;

	return true;
}
void car_on_line(bool& on_line, double x_car_front, double  y_car_front, double  x_car_back, double y_car_back, Mat& lines_matrix, int threshold) {
	/**
	* This function detrmines whether car is on a straight line or not
	*
	* @param on_line boolean to detect wether car is on a st line or not
	* @param line_index line index on which car is on

	* @param x_car, y_car : Center of the Car
	* @lines_matrix : Binary Matrix with 1's = lines
	* @threshold : min sum to consider Car on line[With the uncommented part to make Region for the Car]
	*/
	int size_i = lines_matrix.rows;
	int size_j = lines_matrix.cols;
	int x_car = (x_car_front + x_car_back) / 2;
	int y_car = (y_car_front + y_car_back) / 2;
	int distance_between_2_centers= calculateDistance(x_car_front, y_car_front, x_car_back, y_car_back) ;
    int windo_size_x = distance_between_2_centers * 1.5;
    int windo_size_y = distance_between_2_centers * 2;


    int count = 0;
    on_line = false;

    int x_start = x_car_front;
    if (x_start < 0) {
        x_start = 0;
    }

    int y_start = y_car_front - windo_size_y / 2;
    if (y_start < 0) {
        y_start = 0;
    }

    int x_end = x_start + windo_size_x;
    if (x_end >= lines_matrix.cols) {
        x_end = lines_matrix.cols-1;
    }


    int y_end = y_start + windo_size_y;
    if (y_end >= lines_matrix.rows) {
        y_end = lines_matrix.rows-1;
    }
	Rect rectangle_var = Rect(x_start,y_start , x_end-x_start,y_end- y_start);
	Mat window = lines_matrix(rectangle_var);


	int count2 = 0;
	for (int i = 0;i < window.rows;i++) {
		for (int j = 0;j < window.cols;j++) {
			count2++;
			auto scaler = (int)window.at<uchar>(i, j);
			on_line = on_line || ((int)scaler != 0);
		}
	}
}


bool increase_decrease_speed(Mat draw, double x_car_front, double  y_car_front, double  x_car_back, double y_car_back, Vec4i line, double dist_threshold) {
	/**
	* Control wether Inc or Dec Car Speed Depending on distance form car and end of the St line
	*
	* @param x_car_front, y_car_front : front Center of the Car
	* @param x_car_back, y_car_back : back Center of the Car
	* @param line : [x1, y1, x2, y2] : start and end point of the line
	* @param dist_threshold:threshold to depend on to take action of speed :D
	*
	* @return Boolean True = > increase False Decrease[FIXME:To Be Modified Later to Send Speed Depending on Distance not only a flag 😉]
	*
	*/

	//1. Get Car Direction[always from back to front]
	Vec2i car = direction(x_car_back, y_car_back, x_car_front, y_car_front);
	// car Vector

	double line_point[] = { 0, 0 };

	//2. Direction P1P2
	Vec2i P1P2 = direction(line[0], line[1], line[2], line[3]);
	if ((sign(P1P2[0]) == sign(car[0]) || abs(car[0] - P1P2[0]) < 10) && (sign(P1P2[1]) == sign(car[1]) || abs(car[1] - P1P2[1]) < 10)) {
		line_point[0] = line[2];
		line_point[1] = line[3];
	}
	//3. Direction P2P1
	Vec2i P2P1 = direction(line[2], line[3], line[0], line[1]);

	if ((sign(P2P1[0]) == sign(car[0]) || abs(car[0] - P2P1[0]) < 10) && (sign(P2P1[1]) == sign(car[1]) || abs(car[1] - P2P1[1]) < 10)) {
		line_point[0] = line[0];
		line_point[1] = line[1];
	}
	//  # 4. Take Action Depending on Distance between Car and the endpoint
	int distance = calculateDistance((x_car_front+x_car_back)/2,( y_car_front+y_car_back)/2, line_point[0], line_point[1]);
	if (distance > dist_threshold) {
		//Increase Speed
		return true;
	}
	else {
		//Don't Increase Speed you are toward the line End
		return false;
	}
}
# include"utils.h"
# include "ScanTrack.h";

//Basma :Not sure of Data Type of front_color- back_color check
bool find_car(int &x_center,int &y_center,int &x_f,int &y_f,int &x_b,int &y_b,Mat image, Scalar front_color, Scalar back_color){
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
	x_center = (x_f+x_b) / 2;
	x_center = (y_f+y_b) / 2;
	return true;
}

void car_on_line(bool& on_line, int& line_index ,int x_car, int y_car, Mat lines_matrix, int threshold) {
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

void increase_decrease_speed(bool& inc_speed, int x_car_front, int y_car_front, int x_car_back, int y_car_back, Vec4i line, int dist_threshold) {
	/**
	* This function detrmines whether ti inc or dee speed according to the dsitance between center and end of the line
	*
	* @param inc_speed boolean to detect wether car inc speed or not

	* @param x_car_front, y_car_front : Front of the Car
	* @param x_car_back, y_car_back : Back of the Car
	* @param line Vector 4 x1,y1,x2,y2 start-end of the line
	* @param dist_threshold: min distance so that we can inc speed
	*/
}

# include"utils.h"
# include "ScanTrack.h";

//Basma :Not sure of Data Type of front_color- back_color check
void find_car(bool & car_found,int &x_center,int &y_center,int &x_f,int &y_f,int &x_b,int &y_b,Mat image, Vec3b front_color, Vec3b back_color){
	/**
	* This function is used to find car in the picture
	* 
	* @param boolean to detect wether car is found or not
	* @param x_center,y_center
	* @param x_f,y_f
	* @param x_b,y_b
	*
	* @param image: RGB img with car
	* @param front_color: front color of the car RGB color i.e[0, 255, 0]
	* @param back_color: back color of the car RGB color i.e[0, 255, 0]
	*/
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

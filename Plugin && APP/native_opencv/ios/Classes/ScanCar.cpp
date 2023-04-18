# include"utils.h"
# include "ScanTrack.h"
#include <opencv2/core/mat.hpp>
#include <set>


bool find_car(int& x_center, int& y_center, int& x_f, int& y_f, int& x_b, int& y_b, Mat& image, Scalar front_color, Scalar back_color,int indx) {
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
	//cvtColor(paper_img, image_rgb, COLOR_BGR2RGB);
	cvtColor(paper_img, image_rgb, COLOR_BGRA2RGB);

	//image_rgb=paper_img;

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
void car_on_line(bool& on_line, double x_car_front, double  y_car_front, double  x_car_back, double y_car_back, Mat& lines_matrix, int threshold,int indx) {
    /**
    * This function detrmines whether car is on a straight line or not
    *
    * @param on_line boolean to detect wether car is on a st line or not
    * @param line_index line index on which car is on

    * @param x_car, y_car : Center of the Car
    * @lines_matrix : Binary Matrix with 1's = lines
    * @Mat car_image_debug  :for drawing the car just for Debug not on Eslam and Zainab //Basma
    * @threshold : min sum to consider Car on line[With the uncommented part to make Region for the Car]
    */
    //(1)Car Center
    int size_i = lines_matrix.rows;
    int size_j = lines_matrix.cols;
    int x_car = (x_car_front + x_car_back) / 2;
    int y_car = (y_car_front + y_car_back) / 2;

    int distance_between_2_centers = calculateDistance(x_car_front, y_car_front, x_car_back, y_car_back);
    int windo_size_x = distance_between_2_centers * 1.5;
    int windo_size_y = distance_between_2_centers * 1;

    //(2)Unit vector from back to front
    double change_x = x_car_front - x_car_back, change_y = y_car_front - y_car_back;//Basma
    double length = sqrt(pow(change_x, 2) + pow(change_y, 2));//Basma
    change_x /= length; change_y /= length;//Basma
    double unit_vector_x = change_x, unit_vector_y = change_y;


    //(3)Perpendicular unit vector
    double per_unit_vector_x = 1;
    double per_unit_vector_y = 1;
    if (unit_vector_x == 0) {
        per_unit_vector_y = 0;
    }
    else if (unit_vector_y == 0) {
        per_unit_vector_x = 0;
    }
    else {
        per_unit_vector_y = -1 * per_unit_vector_x * unit_vector_x / unit_vector_y;
        double temp_vector_length = sqrt(pow(per_unit_vector_y, 2) + pow(per_unit_vector_x, 2));//mad;
        per_unit_vector_y /= temp_vector_length;
        per_unit_vector_x /= temp_vector_length;
    }

    //(4)Getting Seach window Rect
    change_x = 0.5 * unit_vector_x * (length);
    change_y = 0.5 * unit_vector_y * (length);

    //(4.1)Getting Search Window Center
    int x_window_center = x_car_front ;
    int y_window_center = y_car_front ;


    int count = 0;
    on_line = false;
    int count2 = 0;
    Mat temp_matrix = lines_matrix.clone();

    //Debug Only
    Mat image_test = lines_matrix.clone();

    for (int i = 0;i < windo_size_y;i++) {
        double base_x = x_window_center + i * unit_vector_x;
        double base_y = y_window_center + i * unit_vector_y;
        int point_x, point_y;
        for (int j = 0;j < windo_size_x / 2;j++) {
            point_x = round(base_x + j * per_unit_vector_x);
            point_y = round(base_y + j * per_unit_vector_y);
            if (point_x < 0 || point_y < 0 || point_x >= lines_matrix.cols || point_y >= lines_matrix.rows)
                continue;
            auto scaler = (int)temp_matrix.at<uchar>(point_y, point_x);
            count2 += ((int)scaler != 0) ? 1 : 0;

            //Debug only
            image_test.at<uchar>(point_y, point_x)=(int)100;

        }
        for (int j = 1;j < windo_size_x / 2;j++) {
            point_x = round(base_x - j * per_unit_vector_x);
            point_y = round(base_y - j * per_unit_vector_y);
            if (point_x < 0 || point_y < 0 || point_x >= lines_matrix.cols || point_y >= lines_matrix.rows)
                continue;
            auto scaler = (int)temp_matrix.at<uchar>(point_y,point_x);
            count2 += ((int)scaler != 0) ? 1 : 0;
            //Debug only
            image_test.at<uchar>(point_y, point_x)=(int)100;

        }
    }
    on_line = count2 > 100;

    //Car Center
    cv::line(image_test, Point(x_car_front, y_car_front), Point(0, 0), Scalar(100, 100, 100), 10);


   imwrite("./data/data/com.example.opencv_app/cache/"+to_string(indx)+"_car.jpg",image_test );

    return;

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
#include <opencv2/core/mat.hpp>
#include <set>

#include"utils.h";
#include"common.h";

#include "ScanTrack.h";
//Basma :Not sure of Data Type of front_color- back_color check
//bool find_car(int& x_center, int& y_center, int& x_f, int& y_f, int& x_b, int& y_b, Mat& image, Mat& transofmation_matrix, Scalar front_color, Scalar back_color) {
bool find_car(int& x_center, int& y_center, int& x_f, int& y_f, int& x_b, int& y_b, Mat & image, Scalar front_color, Scalar back_color) {
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
	bool wrapped = extract_paper(paper_img, image,"car");

	if (!wrapped) {
		return false;
	}
	//namedWindow("Wrapped Paper  find_car()", WINDOW_NORMAL);
	//imshow("Wrapped Paper  find_car()", paper_img);
	imwrite("./assets/TestCases/TestCase" + std::to_string(testcase) + "/results/car_paper.jpeg", paper_img);
	//waitKey(0);

	//Uncomment to Disable extract_paper
	//paper_img = image_input;

	/*****************************************************************************************************/
	//Mat warped_image;
	//int imgWidth = image.cols;
	//int imgHeight = image.rows;
	//warpPerspective(image, warped_image, transofmation_matrix, Size(imgWidth, imgHeight));
	//imshow("warped_car_image", warped_image);
	//waitKey(0);
	/*****************************************************************************************************/

	//Convert BGR to RGB
	Mat image_rgb;
	cvtColor(paper_img, image_rgb, COLOR_BGR2RGB);

	//detect front of the car
	//image isn't modified here 😊
	bool found;
	found = color_center(x_f, y_f, image_rgb, front_color,"front");
	if (!found) {
		cout << "find_car():Couldn't find front of the car" << endl;
		return false;
	}

	//Detect back of the car
	//image isn't modified here 😊
	found = color_center(x_b, y_b, image_rgb, back_color,"back");
	if (!found) {
		cout << "find_car():Couldn't find back of the car" << endl;
		return false;
	}

	//car center
	x_center = (x_f + x_b) / 2;
	y_center = (y_f + y_b) / 2;

	//Draw Car centers [Debug]
	//paper_img not used again 😉
	line(paper_img, Point(x_f, y_f), Point(x_b, y_b), Scalar(0, 255, 255), 10);
	//namedWindow("Wrapped Paper  scan_track()", WINDOW_NORMAL);
	//imshow("find_car()", image);
	imwrite("./assets/TestCases/TestCase" + std::to_string(testcase) + "/results/car.jpeg", paper_img);
	//waitKey(0);

	return true;
}

void car_on_line(bool& on_line, int& line_index, double x_car_front, double  y_car_front, double  x_car_back, double y_car_back, Mat& lines_matrix, int threshold) {
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
	int windo_size = calculateDistance(x_car_front, y_car_front, x_car_back, y_car_back) * 2;
	int count = 0;
	on_line = false;

	/*cv::line(lines_matrix, Point(x_car, y_car), Point(0, 0), Scalar(255, 255, 255), 10);
	imshow("lines_matrix", lines_matrix);
	waitKey(0);*/

	set<int>s;
	map<int, int>m, m2;
	/*for (int i = 0; i < lines_matrix.rows; i++) {
		for (int j = 0; j < lines_matrix.cols; j++) {
			s.insert((int)lines_matrix.at<uchar>(i, j));
			m2[(int)lines_matrix.at<char>(i, j)]++;
		}
	}*/
	//for (int i = y_car-50; i < y_car+50; i++) {
	//	for (int j = x_car-50; j < x_car+50; j++) {
	//		s.insert((int)lines_matrix.at<uchar>(i, j));
	//		m2[(int)lines_matrix.at<uchar>(i, j)]++;
	//		//cv::line(lines_matrix, Point(i, j), Point(0, 0), Scalar(255, 255, 255), 10);
	//	}
	//}
	/*imshow("lines_matrix", lines_matrix);
	waitKey(0);*/
	/*for (int i = x_car - 50; i  < x_car + 50; i++) {
		for (int j = y_car - 50; j < y_car + 50; j++) {
			s.insert((int)lines_matrix.at<uchar>(i, j));
			m2[(int)lines_matrix.at<uchar>(i, j)]++;
			cv::line(lines_matrix, Point(i, j), Point(0, 0), Scalar(255, 255, 255), 2);

		}
	}
	imshow("lines_matrix", lines_matrix);*/
	//waitKey(0);
	//for (auto m : s)cout << m << " ";
	
	for (int i = y_car - windo_size / 2; i <= y_car + windo_size / 2; i++) {
		if (i < 0 || i >= size_i)continue;
		for (int j = x_car - windo_size / 2; j <= x_car + windo_size / 2; j++) {
			if (j < 0 || j >= size_j)continue;
			auto scaler = (int)lines_matrix.at<uchar>(i, j);
			count += ((int)scaler != 0);
			//m[scaler]++;
			on_line=on_line|| ((int)scaler != 0);
			//cout << scaler << " ";
		}
		cout << endl;
	}
	int maxi = -1, maxi_indx = -1;
	/*for (auto x : m) {
		if (x.first && x.second > maxi) {
			maxi = x.second;
			maxi_indx = 255 - x.first;
		}
	}*/
	////threshold
	/*on_line = (count >= threshold);
	line_index = (on_line) ? maxi_indx : -1;*/
}

bool increase_decrease_speed(Mat& draw,double x_car_front, double  y_car_front, double  x_car_back, double y_car_back, Vec4i line, double dist_threshold) {
	/**
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

	//Drawing For Debug
	cv::line(draw, Point(line[0], line[1]), Point(line[2], line[3]), Scalar(255, 0, 0), 5);
	cv::line(draw, Point(x_car_front, y_car_front), Point(0, 0), Scalar(0, 255, 255), 5);
	cv::line(draw, Point(0, 0), Point(x_car_back, y_car_back), Scalar(0, 255,0 ), 5);

	imshow("increase_decrease_speed()", draw);
	//waitKey(0);


	//1. Get Car Direction[always from back to front]
	Vec2i car = direction(x_car_back, y_car_back, x_car_front, y_car_front);
	cout << "car Vector" << car << endl;

	double line_point[] = { 0, 0 };

	//2. Direction P1P2
	Vec2i P1P2 = direction(line[0], line[1], line[2], line[3]);
	cout << "P1P2" << P1P2 << endl;
	//if (sign(P1P2[0]) == sign(car[0]) && sign(P1P2[1]) == sign(car[1])) {
	if ((sign(P1P2[0]) == sign(car[0]) || abs(car[0] - P1P2[0]) < 10) && (sign(P1P2[1]) == sign(car[1]) || abs(car[1] - P1P2[1]) < 10)) {
		cout << "Car is Moving towards P2" << endl;
		line_point[0] = line[2];
		line_point[1] = line[3];
	}
	//3. Direction P2P1
	Vec2i P2P1 = direction(line[2], line[3], line[0], line[1]);
	cout << "P2P1" << P2P1 << endl;

	//if (sign(P2P1[0]) == sign(car[0]) and sign(P2P1[1]) == sign(car[1])) {
	if ((sign(P2P1[0]) == sign(car[0]) || abs(car[0] - P2P1[0]) < 10) && (sign(P2P1[1]) == sign(car[1]) || abs(car[1] - P2P1[1]) < 10)) {
		cout << "Car is Moving towards P1" << endl;
		line_point[0] = line[0];
		line_point[1] = line[1];
	}
	//  # 4. Take Action Depending on Distance between Car and the endpoint
	int distanse = calculateDistance((x_car_front+x_car_back)/2,( y_car_front+y_car_back)/2, line_point[0], line_point[1]);
	if (distanse > dist_threshold){
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
#include <opencv2/core/mat.hpp>
#include <set>

#include"utils.h";
#include"common.h";

#include "ScanTrack.h";
//Basma :Not sure of Data Type of front_color- back_color check
//bool find_car(int& x_center, int& y_center, int& x_f, int& y_f, int& x_b, int& y_b, Mat& image, Mat& transofmation_matrix, Scalar front_color, Scalar back_color) {
bool find_car(int& x_center, int& y_center, int& x_f, int& y_f, int& x_b, int& y_b, Mat& image, Scalar front_color, Scalar back_color, Mat& car_image_debug) {
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
	*
	* @param Mat&front_mask:  for drawing the car just for Debug not on Eslam and Zainab //Basma
	* @param Mat&back_mask : for drawing the car just for Debug not on Eslam and Zainab //Basma
	*/

	//1.Extract Track Paper From the Image
	Mat paper_img;
	//bool wrapped = extract_paper(paper_img, image, "car");

	//if (!wrapped) {
		//cout << "Couldn't extract paper" << endl;
		//return false;
	//}
	//namedWindow("Wrapped Paper  find_car()", WINDOW_NORMAL);
	//imshow("Wrapped Paper  find_car()", paper_img);
	//imwrite("./assets/TestCases/TestCase" + std::to_string(testcase) + "/results/car_paper.jpeg", paper_img);
	//waitKey(0);

	//Uncomment to Disable extract_paper
	paper_img = image;

	//2.Find car center
	//Convert BGR to RGB
	Mat image_rgb;
	cvtColor(paper_img, image_rgb, COLOR_BGR2RGB);

	//detect front of the car
	//image isn't modified here 😊
	bool found;
	//Debug only //Basma
	Mat front_mask, back_mask;
	found = color_center(x_f, y_f, image_rgb, front_color, front_mask, "front");
	if (!found) {
		cout << "find_car():Couldn't find front of the car" << endl;
		return false;
	}

	//Detect back of the car
	//image isn't modified here 😊
	found = color_center(x_b, y_b, image_rgb, back_color, back_mask, "back");
	if (!found) {
		cout << "find_car():Couldn't find back of the car" << endl;
		return false;
	}

	//car center
	x_center = (x_f + x_b) / 2;
	y_center = (y_f + y_b) / 2;



	//Debug only //Basma
	bitwise_or(front_mask, back_mask, car_image_debug);

	//Draw Car centers [Debug]
	//paper_img not used again 😉
	line(paper_img, Point(x_f, y_f), Point(x_b, y_b), Scalar(0, 255, 255), 10);
	//namedWindow("Wrapped Paper  scan_track()", WINDOW_NORMAL);
	//imshow("find_car()", image);
	imwrite("./assets/TestCases/TestCase" + std::to_string(testcase) + "/results/car.jpeg", paper_img);
	//waitKey(0);

	return true;
}

void car_on_line(bool& on_line, double x_car_front, double  y_car_front, double  x_car_back, double y_car_back, Mat& lines_matrix, Mat car_image_debug, int threshold) {
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
	int windo_size_y = distance_between_2_centers * 2;
	//int windo_size_y = distance_between_2_centers * 50;

	//(2)Unit vector from back to front
	double change_x = x_car_front - x_car_back, change_y = y_car_front - y_car_back;//Basma
	double length = sqrt(pow(change_x, 2) + pow(change_y, 2));//Basma
	change_x /= length; change_y /= length;//Basma
	double unit_vector_x = change_x, unit_vector_y = change_y;


	////(3)Perpendicular unit vector
	//double per_unit_vector_x = 1;
	//double per_unit_vector_y = 1;
	//if (unit_vector_x == 0) {
	//	per_unit_vector_y = 0;
	//}
	//else if (unit_vector_y == 0) {
	//	per_unit_vector_x = 0;
	//}
	//else {
	//	per_unit_vector_y = -1 * per_unit_vector_x * unit_vector_x / unit_vector_y;
	//	double temp_vector_length = sqrt(pow(per_unit_vector_y, 2) + pow(per_unit_vector_x, 2));//mad;
	//	per_unit_vector_y /= temp_vector_length;
	//	per_unit_vector_x /= temp_vector_length;
	//}

	//(4)Getting Seach window Rect
	change_x = 0.5 *  unit_vector_x*(length);///////////Basma
	change_y = 0.5 * unit_vector_y * (length);///////////Basma

	//(4.1)Getting Search Window Center
	int x_window_center = x_car_front + change_x;//Basma
	int y_window_center = y_car_front + change_y;//Basma


	//(4.1)Getting Search Window Angle
	float angle = 0;
	if (change_x == 0) {
		angle = 90;
	}
	else {
		angle = atan(change_y / change_x) * 180 / 3.1415;
	}
	//Rotate Image by angle
	//Mat lines_matrix_rotated=rotate_Image(lines_matrix, angle);

	//Point(x_car_front,y_car_front)
	
	//imwrite("./assets/TestCases/TestCase" + std::to_string(testcase) + "/results/Rotated lime_matrix on_line() .jpeg", lines_matrix_rotated);
	//return ;

	//if (x_window_center+ windo_size_x/2) {
	//	//out of boundary
	//}
	//if () {
	//	// out boundary
	//}
	RotatedRect rectangle_var = cv::RotatedRect::RotatedRect(Point2f(x_window_center, y_window_center), Size2f(windo_size_x, windo_size_y), angle);

	Mat rect_img2 = lines_matrix.clone();

	//Search window
	Point2f vertices[4];
	rectangle_var.points(vertices);
	for (int i = 0; i < 4; i++)
		line(rect_img2, vertices[i], vertices[(i + 1) % 4], Scalar(100, 100, 100), 2);
	//Center of car
	cv::line(rect_img2, Point(x_car, y_car), Point(0, 0), Scalar(100, 100, 100), 10);
	
	//Car Centers
	cv::line(rect_img2, Point(x_car_front, y_car_front), Point(x_car_back, y_car_back), Scalar(100, 100, 100), 10);
	//namedWindow("Search Window car_on_line()", WINDOW_NORMAL);
	//imshow("Search Window car_on_line()", rect_img);
	imwrite("./assets/TestCases/TestCase" + std::to_string(testcase) + "/results/Search Window car_on_line() from front of car.jpeg", rect_img2);
	//waitKey(0);



	return;

	Rect_<int> rcBound = rectangle_var.boundingRect();
	Mat window = lines_matrix(cv::Rect2f(cv::Point2f(x_window_center - rcBound.width / 2, y_window_center - rcBound.height / 2), cv::Size(rcBound.width, rcBound.height)));



	int count = 0;
	on_line = false;

	////int x_start = x_car_front - windo_size_x /2;//
	//int x_start = x_car_front;//Basma
	//if (x_start < 0) {
	//	x_start = 0;
	//}

	//int y_start = y_car_front - windo_size_y / 2;
	//if (y_start < 0) {
	//	y_start = 0;
	//}

	//int x_end = x_start + windo_size_x;
	//if (x_end >= lines_matrix.cols) {
	//	x_end = lines_matrix.cols-1;
	//}


	//int y_end = y_start + windo_size_y;
	//if (y_end >= lines_matrix.rows) {
	//	y_end = lines_matrix.rows-1;
	//}

	//int x_window_center = x_car_front + windo_size_x / 2;
	//if (x_window_center < 0) {
	//	x_window_center = 0;
	//}

	//int x_window_center = x_car_front - windo_size_x / 2;
	//if (x_window_center < 0) {
	//	x_window_center = 0;
	//}

	//cout << "x_start" << x_start << endl;
	//cout << "y_start" << y_start << endl;
	//cout << " x_end-x_start" << x_end - x_start << endl;
	//cout << "y_end- y_start" << y_end - y_start << endl;

	//Rect rectangle_var = Rect(x_start,y_start , x_end-x_start,y_end- y_start);
	//Rect rectangle_var = Rect(x_car, y_car, x_end - x_start, y_end - y_start);


	//Mat window = lines_matrix(rectangle_var);
	//cout << "window.rows" << window.rows << endl;
	//cout << "window.vols" << window.cols << endl;


	//Debug  Comment
	//namedWindow("window on_line()", WINDOW_NORMAL);
	imwrite("./assets/TestCases/TestCase" + std::to_string(testcase) + "/results/window on_line() Cropped.jpeg", window);
	//imshow("window on_line()", window);
	//waitKey(0);

	//cout << "Type" << window.type() << endl;
	//cout << "channels" << window.channels() << endl;

	int count2 = 0;
	for (int i = 0;i < window.rows;i++) {
		for (int j = 0;j < window.cols;j++) {

			auto scaler = (int)window.at<uchar>(i, j);
			//on_line = on_line || ((int)scaler != 0);
			count2 += ((int)scaler != 0) ? 1 : 0;
			//cout << scaler << endl;
		}
		//cout << endl << endl;
	}
	on_line = count2 > 20;

	//Debug Rectangle to We search in +Car 
	Mat rect_img = lines_matrix.clone();
	bitwise_or(rect_img, car_image_debug, rect_img);

	//Search window

	vector<Point2f> boxPts(4);
	rectangle_var.points(boxPts.data());
	//boxPts[3] = rectangle_var.br();
	//The order of the box points : bottom left, top left, top right, bottom right
	cv::line(rect_img, boxPts[0], boxPts[1], Scalar(100, 100, 100), 1);
	cv::line(rect_img, boxPts[1], boxPts[2], Scalar(100, 100, 100), 2);
	cv::line(rect_img, boxPts[2], boxPts[3], Scalar(100, 100, 100), 2);
	cv::line(rect_img, boxPts[3], boxPts[0], Scalar(100, 100, 100), 2);
	//rectangle(rect_img,va, (100, 100, 100), 2);
	//Center of car
	cv::line(rect_img, Point(x_car, y_car), Point(0, 0), Scalar(100, 100, 100), 20);
	//namedWindow("Search Window car_on_line()", WINDOW_NORMAL);
	//imshow("Search Window car_on_line()", rect_img);
	imwrite("./assets/TestCases/TestCase" + std::to_string(testcase) + "/results/Search Window car_on_line() from front of car.jpeg", rect_img);
	//waitKey(0);



	/*cv::line(lines_matrix, Point(x_car, y_car), Point(0, 0), Scalar(100, 100, 100), 20);
	imshow("lines_matrix", lines_matrix);
	waitKey(0);*/

	//set<int>s;
	//map<int, int>m, m2;
	/*for (int i = 0; i < lines_matrix.rows; i++) {
		for (int j = 0; j < lines_matrix.cols; j++) {
			s.insert((int)lines_matrix.at<uchar>(i, j));
			m2[(int)lines_matrix.at<char>(i, j)]++;
		}
	}*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//for (int i = y_car - 50; i < y_car + 50; i++) {
	//	for (int j = x_car - 50; j < x_car + 50; j++) {
	//		int value = (int)lines_matrix.at<uchar>(i, j);
	//		if (value == 100) {
	//			int x = 21;
	//		}
	//		s.insert(value);
	//		//s.insert((int)lines_matrix.at<uchar>(j, i));
	//		m2[(int)lines_matrix.at<uchar>(i, j)]++;
	//		cv::line(lines_matrix, Point(j, i), Point(0, 0), Scalar(150, 150, 150), 1);
	//	}
	//}
	/*imshow("lines_matrix", lines_matrix);
	waitKey(0);*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*for (int i = x_car - 50; i  < x_car + 50; i++) {
		for (int j = y_car - 50; j < y_car + 50; j++) {
			s.insert((int)lines_matrix.at<uchar>(i, j));
			m2[(int)lines_matrix.at<uchar>(i, j)]++;
			cv::line(lines_matrix, Point(i, j), Point(0, 0), Scalar(255, 255, 255), 1);

		}
	}
	imshow("lines_matrix", lines_matrix);
	waitKey(0);*/
	//for (auto m : s)cout << m << " ";
	/*Mat matrix = lines_matrix.clone();
	for (int i = 50;i < size_i;i++) {
		for (int j = 50;j < size_j;j++) {
			int pixel_val = (int)matrix.at<uchar>(i, j);
			s.insert(pixel_val);
			if (pixel_val == 100) {
				int x = 12;
			}
		}
	}*/
	//Mat rect_img = lines_matrix.clone();
	//rectangle(rect_img, Point(x_car- windo_size / 2, y_car- windo_size / 2), Point(x_car + windo_size / 2, y_car + windo_size / 2), (0, 255, 0), 2);
	////namedWindow("Bounding Rect color_center() "+string", WINDOW_NORMAL);
	////imshow("Bounding Rect color_center() "+string, mask);
	//imwrite("./assets/TestCases/TestCase" + std::to_string(6) + "/results/Bounding Rectangle color_center()" + "window car" + ".jpeg", rect_img);
	//waitKey(0);
	//cout << "y_car" << y_car << endl;
	//cout << "x_car" << x_car << endl;
	//cout<<"start in x: "<< x_car - windo_size / 2<<"\t the end of the x is : "<< x_car + windo_size / 2;
	//cout << "start in y: " << y_car - windo_size / 2 << "\t the end of the y is : " << y_car + windo_size / 2;

	//for (int i = y_car - windo_size / 2; i <= y_car + windo_size / 2; i++) {
	//	if (i < 0 || i >= lines_matrix.rows)continue;
	//	for (int j = x_car - windo_size / 2; j <= x_car + windo_size / 2; j++) {
	//		if (j < 0 || j >= lines_matrix.cols)continue;
	//		//auto scaler = (int)lines_matrix.at<uchar>(i, j);
	//		//count ++;
	//		////m[scaler]++;
	//		////s.insert(scaler);
	//		//on_line = on_line || ((int)scaler != 0);
	//		//cout << scaler ;

	//		//lines_matrix.at<uchar>(i, j) = 150;
	//		cv::line(lines_matrix, Point(j, i), Point(0, 0), Scalar(255, 255, 255), 1);
	//	}
	//	//cout << endl<<endl;
	//}


	//namedWindow("Bounding Rect color_center() "+string", WINDOW_NORMAL);
	//imshow("Test",lines_matrix);
	////imwrite("./assets/TestCases/TestCase" + std::to_string(6) + "/results/Bounding Rectangle color_center()" + "window car" + ".jpeg", rect_img);
	//waitKey(0);

	//////count =5929
	//Rect rectangle =  Rect(x_car - windo_size / 2, y_car- windo_size/2, windo_size, windo_size);
	//Mat temp=lines_matrix(rectangle);
	///*imshow("temp", temp);
	//waitKey(0);*/
	//int count2 = 0;
	//for (int i = 0;i < temp.rows;i++) {
	//	for (int j = 0;j < temp.cols;j++) {
	//		count2 ++;
	//		auto scaler = (int)temp.at<uchar>(i, j);
	//		on_line = on_line || ((int)scaler != 0);
	//		//cout << scaler;
	//	}
	//	//cout << endl << endl;
	//}
	////count =5776
	//int x = 12;
	//int maxi = -1, maxi_indx = -1;
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

bool increase_decrease_speed(Mat& draw, double x_car_front, double  y_car_front, double  x_car_back, double y_car_back, Vec4i line, double dist_threshold) {
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
	cv::line(draw, Point(0, 0), Point(x_car_back, y_car_back), Scalar(0, 255, 0), 5);

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
	int distanse = calculateDistance((x_car_front + x_car_back) / 2, (y_car_front + y_car_back) / 2, line_point[0], line_point[1]);
	if (distanse > dist_threshold) {
		//Increase Speed
		cout << "Increase Speed" << endl;
		return true;
	}
	else {
		//Don't Increase Speed you are toward the line En
		cout << "Decrease Speed" << endl;
		return false;
	}
}
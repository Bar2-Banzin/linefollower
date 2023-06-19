# include"utils.h"
# include "ScanTrack.h"

bool scan_track(Mat & image_lines, Mat & track_image,Size s) {
	/**
	* Scan Track Without A Car to Detect Straight Lines locations

	* @param image_lines: Matrix of size = Image size with St lines Drawn in white
	* @param start_end_points: Vector of 4 for evey line x1,y1  x2,y2 *****************

	* @param track_image: BGR Track Image
	* @param thickness : Thickness of the wite line Drawn : )
	* @return boolean if True = Scanning is Done Sucessfully
	*/

	//1.Extract Track Paper From the Image
	Mat paper_img;
	//bool wrapped = extract_paper(paper_img, track_image,"track");
	/*if (! wrapped) {
		return false;
	 }*/

	//Uncomment to Disable extract_paper
	paper_img = track_image;

	// 2.Detect Straight Lines in the Track
	extract_lines(image_lines, paper_img,s);
	return true;
}


struct str {
	bool operator() (Point a, Point b) {
		if (a.y != b.y)
			return a.y < b.y;
		return a.x <= b.x;
	}
} comp;

void extract_lines(Mat & image_lines, Mat& image,Size s,int sliding, double rho, double  theta, int threshold, double minLineLength, double  maxLineGap, int thickness) {
	/**
	* Extract Line out of the RGB image

	* @param image_lines: Matrix of size = Image size with St lines Drawn with color Depedning on its order
	* @param start_end_points: Vector of 4 for evey line x1,y1  x2,y2 ************************************

	* @param image: RGB Track image
	* @param sliding: length to be cut from the lines
	* @param rho: rho Resolution
	* @param theta: theta Resolution
	* @param threshold : no of min votes so that line is detected as line by Hough
	* @param minLineLength : min length so the line is detected
	* @param maxLineGap : max allowed gap between lines to be treated as a 1 line(this breaks long un connected lines to 2 lines :D)
	* @param thickness : thickness of white line drawn for straight line (image_lines)
	*/

	// Convert the image to gray - scale
	Mat  image_gray;
	cvtColor(image, image_gray, COLOR_RGB2GRAY);

	// Thinning Image
	Mat thinned = thin_image(image_gray);

	// Dilate
	Mat kernel, Dilate;
	kernel = getStructuringElement(MORPH_CROSS,s);
	dilate(thinned, Dilate, kernel);

	// Find the edges in the image using canny detector
		Mat edges = Dilate;
    	/************************************************************************************************************************************************/
    	vector<Point>biggest_contour;
    	double max_area=-1;
    	int counter_index = 0;
    	vector<vector<Point>> contours;
    	vector<Vec4i> hierarchy;

    	findContours(Dilate, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
    	for (int i = 0; i < contours.size(); i++) {
    		double area = contourArea(contours[i]);

    		//If Greater than Max Aeaand it is a rectangle
    		if (area > max_area) {
    			max_area = area;
    			biggest_contour = contours[i];
    			counter_index = i;
    		}
    	}
    	Mat mask= cv::Mat::zeros(cv::Size(Dilate.cols, Dilate.rows), CV_8UC1);
    	drawContours(mask, contours, counter_index, 255);
    	kernel = getStructuringElement(MORPH_CROSS, s);
    	dilate(mask, edges, kernel);

	/*************************************************************Hough Lines********************************************************/
	// Hough Lines
	// Syntax:HoughLinesP(Edged image, Rho Resolution, Angle Resolution, threshold, min length of line, max distance between lines)
	//	minLineLength : Lines shorter than that are rejected
	//	maxLineGap : max allowed gap between lines to be treated as a 1 line(this breaks long un connected lines to 2 lines :D)
	//	0 <= rho < Rmax
	vector<Vec4i>lines;


	HoughLinesP(edges, lines, 1, theta, threshold, minLineLength, maxLineGap);

	// # Draw lines on the image
	image_lines = Mat::zeros(image.size(), CV_8UC1);//Inverted image
       int old_size = lines.size();
       	vector<bool>valid(old_size, true);
       	for (int i = 0; i < old_size; i++) {
       		if (!valid[i])
       			continue;
       		double x1 = lines[i][0];
       		double y1 = lines[i][1];
       		double x2 = lines[i][2];
       		double y2 = lines[i][3];
       		for (int j = i + 1; j < old_size; j++) {
       			double x_1 = lines[j][0];
       			double y_1 = lines[j][1];
       			double x_2 = lines[j][2];
       			double y_2 = lines[j][3];
       			bool first = calculateDistance(x1, y1, x_1, y_1) <= 5 and calculateDistance(x2, y2, x_2, y_2) <= 5;
       			bool second = calculateDistance(x1, y1, x_2, y_2) <= 5 and calculateDistance(x2, y2, x_1, y_1) <= 5;
       			if (first or second)
       				valid[j] = 0;
       		}
       	}
       	vector<Vec4i>filteration = lines;
       	lines.clear();
       	for (int i = 0; i < old_size; i++) {
       		if (!valid[i])
       			continue;
       		lines.push_back(filteration[i]);
       	}
       	//cout << "Hough Lines Detected after filteration 1 " << lines.size() << endl;
       	/////////////////////////////////// intermidiate calculations //////////////////////////
       	old_size = lines.size();
       	vector<pair<Vec4i, Vec2d>>lines_vectors(old_size);
       	for (int i = 0; i < old_size; i++) {
       		double x1 = lines[i][0];
       		double y1 = lines[i][1];
       		double x2 = lines[i][2];
       		double y2 = lines[i][3];
       		double change_x = x1 - x2, change_y = y1 - y2;
       		double length = sqrt(pow(change_x, 2) + pow(change_y, 2));
       		change_x /= length; change_y /= length;
       		double unit_vector_x = change_x, unit_vector_y = change_y;
       		if (sign(unit_vector_x) == sign(unit_vector_y)) {
       			unit_vector_x = abs(unit_vector_x);
       			unit_vector_y = abs(unit_vector_y);
       		}
       		else {
       			unit_vector_x = abs(unit_vector_x);
       			unit_vector_y = -1 * abs(unit_vector_y);
       		}
       		lines_vectors[i].second[0] = unit_vector_x;
       		lines_vectors[i].second[1] = unit_vector_y;
       		double dist1 = x1 * unit_vector_x + y1 * unit_vector_y;
       		double dist2 = x2 * unit_vector_x + y2 * unit_vector_y;
       		if (dist1 > dist2) {
       			lines_vectors[i].first[0] = x2;
       			lines_vectors[i].first[1] = y2;
       			lines_vectors[i].first[2] = x1;
       			lines_vectors[i].first[3] = y1;
       		}
       		else {
       			lines_vectors[i].first[0] = x1;
       			lines_vectors[i].first[1] = y1;
       			lines_vectors[i].first[2] = x2;
       			lines_vectors[i].first[3] = y2;
       		}

       	}
       	/////////////////////////////////// end intermidiate calculations //////////////////////////

       	/////////////////////////////////// second filteration //////////////////
       	double diffrence = .05;
       	old_size = lines.size();
       	valid = vector<bool>(old_size, true);
       	for (int i = 0; i < old_size; i++) {
       		if (!valid[i])
       			continue;
       		double  unit_vectorx = lines_vectors[i].second[0];
       		double  unit_vectory = lines_vectors[i].second[1];
       		//(3)Perpendicular unit vector
       		double per_unit_vectorx = 1;
       		double per_unit_vectory = 1;
       		if (unit_vectorx == 0) {
       			per_unit_vectory = 0;
       		}
       		else if (unit_vectory == 0) {
       			per_unit_vectorx = 0;
       		}
       		else {
       			per_unit_vectory = -1 * per_unit_vectorx * unit_vectorx / unit_vectory;
       			double temp_vector_length = sqrt(pow(per_unit_vectory, 2) + pow(per_unit_vectorx, 2));//mad;
       			per_unit_vectory /= temp_vector_length;
       			per_unit_vectorx /= temp_vector_length;
       		}
       		double y1 = lines[i][1];
       		double x2 = lines[i][2];
       		double y2 = lines[i][3];
       		for (int j = i + 1; j < old_size; j++) {
       			double  unit_vector_x = lines_vectors[j].second[0];
       			double  unit_vector_y = lines_vectors[j].second[1];
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
       				double temp_vector_length_ = sqrt(pow(per_unit_vector_y, 2) + pow(per_unit_vector_x, 2));//mad;
       				per_unit_vector_y /= temp_vector_length_;
       				per_unit_vector_x /= temp_vector_length_;
       			}
       			double x_2 = lines[j][2];
       			double y_2 = lines[j][3];
       			bool first = abs(unit_vectorx - unit_vector_x) <= diffrence and abs(unit_vectory - unit_vector_y) <= diffrence;
       			bool second = false;
       			if (first) {
       				auto x_far_i = lines_vectors[i].first[2];
       				auto y_far_i = lines_vectors[i].first[3];
       				auto x_far_j = lines_vectors[j].first[2];
       				auto y_far_j = lines_vectors[j].first[3];

       				auto x_near_i = lines_vectors[i].first[0];
       				auto y_near_i = lines_vectors[i].first[1];
       				auto x_near_j = lines_vectors[j].first[0];
       				auto y_near_j = lines_vectors[j].first[1];
       				auto dist_far_i = x_far_i * unit_vectorx + y_far_i * unit_vectory;
       				auto dist_far_j = x_far_j * unit_vector_x + y_far_j * unit_vector_y;
       				auto dist_near_i = x_near_i * unit_vectorx + y_near_i * unit_vectory;
       				auto dist_near_j = x_near_j * unit_vector_x + y_near_j * unit_vector_y;
       				auto dist_per_i= x_far_i * per_unit_vectorx + y_far_i * per_unit_vectory;
       				auto dist_per_j = x_far_j * per_unit_vector_x + y_far_j * per_unit_vector_y;
       				if (abs(dist_per_i - dist_per_j) > 10)
       					continue;
       				if (dist_far_i >= dist_far_j and dist_far_j >= dist_near_i) {
       					second = true;
       					if (dist_near_i <= dist_near_j) {
       						lines_vectors[i].first[0] = x_near_i;
       						lines_vectors[i].first[1] = y_near_i;
       					}
       					else {
       						lines_vectors[i].first[0] = x_near_j;
       						lines_vectors[i].first[1] = y_near_j;
       					}
       					lines_vectors[i].first[2] = x_far_i;
       					lines_vectors[i].first[3] = y_far_i;
       				}
       				else if (dist_far_j >= dist_far_i and dist_far_i >= dist_near_j) {
       					second = true;
       					if (dist_near_i <= dist_near_j) {
       						lines_vectors[i].first[0] = x_near_i;
       						lines_vectors[i].first[1] = y_near_i;
       					}
       					else {
       						lines_vectors[i].first[0] = x_near_j;
       						lines_vectors[i].first[1] = y_near_j;
       					}
       					lines_vectors[i].first[2] = x_far_j;
       					lines_vectors[i].first[3] = y_far_j;

       				}
       			}
       			//bool second = calculateDistance(x1, y1, x_2, y_2) <= 5 and calculateDistance(x2, y2, x_1, y_1) <= 5;
       			if (first and second)
       				valid[j] = 0;
       		}
       	}
       	filteration = lines;
       	vector<pair<Vec4i, Vec2d>>filteration_vector = lines_vectors;
       	lines.clear();
       	lines_vectors.clear();
       	for (int i = 0; i < old_size; i++) {
       		if (!valid[i])
       			continue;
       		lines.push_back({ filteration_vector[i].first[0],filteration_vector[i].first[1],filteration_vector[i].first[2],filteration_vector[i].first[3] });
       		lines_vectors.push_back(filteration_vector[i]);
       	}

	for (auto line_inst : lines) {
		double x1 = line_inst[0];
		double y1 = line_inst[1];

		double x2 = line_inst[2];
		double y2 = line_inst[3];

		double change_x = x2 - x1, change_y = y2 - y1;
		double length = sqrt(pow(change_x, 2) + pow(change_y, 2));
		change_x /= length; change_y /= length;
		change_x *= 0.1*length;//0.3
		change_y *=  0.1*length;//0.3
		x1 += change_x;
		y1 += change_y;

		x2 -= change_x;
		y2 -= change_y;

		line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(250, 250, 250), thickness);
	}
	return;
}
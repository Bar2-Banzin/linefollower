#include "utils.h"

//bool extract_paper(Mat& warped_image, Mat& transformation_matrix, Mat& img_bgr, bool draw) {
bool extract_paper(Mat& warped_image, Mat& img_bgr, string name) {
	/**
	* extract paper out of the image
	* @param warped_image bgr warpPerspective OUTPUT image contain paper only
	*
	* @param img_bgr: bgr image INPUT IMAGE
	* @param name: String for the flag of saved image [For Debug Optional]
	*
	* @return boolean to determine wether paper is extracted sucessfully
	*/

	//get image dimensions
	int imgHeight = img_bgr.rows;
	int imgWidth = img_bgr.cols;

	//convert to rgb scale
	Mat image_rgb;
	cvtColor(img_bgr, image_rgb, COLOR_BGR2RGB);

	//convert to gray scale
	Mat image_gray;
	cvtColor(img_bgr, image_gray, COLOR_BGR2GRAY);


	//gaussian filter on the image to remove noise
	//syntax: gray scale image, kernel size(positive and odd), sigma
	Mat blurred_image_gaussian;
	GaussianBlur(image_gray, blurred_image_gaussian, Size(5, 5), 1);


	//converting grayscale image stored in converted matrix into binary image//
	Mat binary_image;
	threshold(image_gray, binary_image, 160, 255, THRESH_BINARY);


	//===========================================================erosion & dilation=======================================================
	Mat erosion_image;
	erode(binary_image, erosion_image, Mat(), Point(-1, -1), 7, 1, 1);

	Mat dilated_image;
	dilate(erosion_image, dilated_image, Mat(), Point(-1, -1), 7, 1, 1);

	//===========================================================edge detection=========================================================
	//canny edge detection (optimal edge detector)
	Mat edged_image;
	Canny(dilated_image, edged_image, 150, 350);

	//============================================================getting contours=========================================================

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(edged_image, contours, hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
	if (contours.size() <= 0) {
		// couldn't extract Contours out of image
		return false;
	}

	//getting biggest rectangular contour
	vector<Point>biggest_contour;
	double max_area;
	get_biggest_rectangular_contour(biggest_contour, max_area, contours);


	//Solving problem of False Contours The max area must be Greater than 10 % of the Image area
	if (max_area < 0.10 * imgHeight * imgWidth)
	{
		//cout << "extractpaper():Largest Contour is Very small :(" << endl;
		return false;
	}

	//Sort 4 Corner for Prespective
	vector<Point2f>biggest_contour_ordered = reorderPoints(biggest_contour);

	//Prespective
	vector<Point2f>image_corner{ Point(0,0),Point(imgWidth - 1, 0),Point(0,imgHeight - 1),Point(imgWidth - 1, imgHeight - 1) };
	Mat M = getPerspectiveTransform(biggest_contour_ordered, image_corner);
	//transformation_matrix = M;
	warpPerspective(img_bgr, warped_image, M, Size(imgWidth, imgHeight));

	return true;
}

Mat thin_image(Mat image) {
	/**
	* Thinning Gray Scale Image

	* @param image: Gray Image

	* @return thinned Binary Image
	*/

	//Convert to Binary
	//Syntax:threshold(src_gray, dst, threshold_value, max_binary_value, threshold_type);
	Mat image_binary;
	threshold(image, image_binary, 127, 255, 0);
	//Make lines white
	bitwise_not(image_binary, image_binary);

	//Structuring Element
	Mat kernel;
	kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
	//Create an empty output image to hold values
	Mat thin;
	thin = Mat::zeros(image_binary.size(), CV_8UC1);

	//Loop until erosion leads to an empty set
	while (countNonZero(image_binary) != 0) {
		// Erosion
		Mat erode_image;
		erode(image_binary, erode_image, kernel);
		// Opening on eroded image
		Mat opening;
		morphologyEx(erode_image, opening, MORPH_OPEN, kernel);
		//Subtract these two
		Mat subset;
		subset = erode_image - opening;

		//Union of all previous sets
		bitwise_or(thin, subset, thin);

		// Set the eroded image for next iteration
		image_binary = erode_image.clone();
	}
	return thin;
}

bool color_center(int& x, int& y, Mat image, Scalar color, string name) {
	/**
	* Get Center and Draw Rectangle Around Largest Contour of a given Color
	*
	* @param found boolean to determine wether this color is found or not
	* @param x,y Center of the Color
	*
	* @param image: RGB image with car on the track
	* @param color : RGB color i.e[0, 255, 0]
	* @param name: String for the flag of saved image [For Debug Optional]
	*
	*/

	//Get Color Mask
	Mat mask, masked_image;
	bool accepted = color_mask(mask, masked_image, image, color);

	//Apply Dialtion then Errosion on Mask to solve unconnected parts
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	morphologyEx(mask, mask, MORPH_CLOSE, kernel);

	if (!accepted)
		return false;

	//Find Mask Contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(mask, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

	if (contours.size() <= 0) {
		// Couldn't find contours of color_range passed
		x = -1;
		y = -1;
		return false;
	}


	//Grab contours [Biggest]
	vector<Point>biggestContour;
	double max_area;
	get_biggest_rectangular_contour(biggestContour, max_area, contours);

	//get image dimensions
	int imgHeight = image.rows;
	int imgWidth = image.cols;

	if (max_area < 0.001 * imgHeight * imgWidth)
	{
		// Largest Contour is Very small :(
		return false;
	}


	//Approcimate to Rect
	Rect rect = boundingRect(biggestContour);

	x = int(rect.x + rect.width / 2);
	y = int(rect.y + rect.height / 2);

	return true;
}



bool color_mask(Mat& mask, Mat& masked_image, Mat image, Scalar color) {
	/**
		* Get Center and Draw Rectangle Around Largest Contour of a given Color
		*
		* @param mask Binary Image with 1's are is are with color passed
		* @param masked_img RGB Image with mask applied on it (image passed)
		*
		* @param image: RGB image
		* @param color : RGB color i.e[0, 255, 0] to be masked
		*
		* @return bool if Error in Geeting Range of Color
		*/



		//Convert RGB image to HSV
	Mat image_hsv;
	cv::cvtColor(image, image_hsv, COLOR_RGB2HSV);



	if (color == Scalar(255, 0, 0)) {

		Mat1b mask1, mask2;
		cv::inRange(image_hsv, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);
		mask = mask2;
	}
	else if (color == Scalar(0, 0, 255)) {
		cv::inRange(image_hsv, Scalar(100, 147, 0), Scalar(144, 255, 255), mask);
	}

	//Check If This is Required
	image.copyTo(masked_image, mask);

	return true;
}


bool color_range(Scalar& lower_range, Scalar& upper_range, Scalar color) {
	/**
	* Gets Upper & Lower HSV Range of the RGB color
	*
	* @param lower_range Hue lower range for color
	* @param upper_range Hue upper range for color
	*
	* @param color : RGB color i.e[0, 255, 0] to be masked
	*
	* @return bool if Error in getting Hue Value
	*/

	Mat hsv(1, 1, CV_8UC3, color);
	cvtColor(hsv, hsv, COLOR_RGB2HSV);

	int Hue = hsv.at<Vec3b>(0, 0)[0];

	lower_range = Scalar(Hue - 10, 100, 100);
	upper_range = Scalar(Hue + 10, 255, 255);

	return true;
}


double calculateDistance(double x1, double y1, double x2, double y2) {
	/**
	* Calculate the Euclidean distance between the two vectors x1, y1and x2, y2.
	*
	* @param x1, y1:point 1
	* @param x2, y2:point 2
	*
	* @return : Euclidean distance
	*/
	double distance = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	return distance;
}

Vec2i direction(double x1, double y1, double x2, double y2) {
	/**
	* Gets Direction between 2 Points
	*
	* @param x1, y1:point 1
	* @param x2, y2:point 2
	*
	* @return : vector x1y1 - x2y2
	*/
	Vec2i arr(x2 - x1, y2 - y1);
	return arr;
}

int sign(double x)
{
	if (x > 0)
		return 1;
	if (x < 0)
		return -1;

	return 0;
}


//Comparison function object
bool compareContourAreas(vector<Point> contour1, vector<Point> contour2) {
	double i = fabs(contourArea(Mat(contour1)));
	double j = fabs(contourArea(Mat(contour2)));
	return (i < j);
}

void get_biggest_rectangular_contour(vector<Point>& biggest_contour, double& max_area, vector<vector<Point>> contours) {
	/**
	* Get Biggest Rectangular Contour from the given contours
	*
	* @param biggest_contour: Biggest Rectangular Contour
	* @param max_area: area of biggest_contour
	*
	* @param contour: contours to be sorted
	*/

	//Get Biggest Contour
	for (int i = 0;i < contours.size();i++) {
		double area = contourArea(contours[i]);

		//Approximate The Contour to the nearest Poly
		double perimeter = arcLength(contours[i], true);
		const double ratio = 0.02;
		vector<Point>approx;
		approxPolyDP(contours[i], approx, ratio * perimeter, true);

		//If Greater than Max Aeaand it is a rectangle
		if (area > max_area) {
			max_area = area;
			biggest_contour = approx;
		}
	}
}

vector<Point2f> reorderPoints(vector<Point>points) {
	/**
	* Reorder 4 Points in clockwise order, starting from top left
	*
	* @param points: vector of 4 points
	*
	* @return newPoints: ordered 4 points
	*/

	vector<int>sum(4);
	vector<int>diff(4);
	vector<Point2f>newPoints(4);

	sum[0] = points[0].x + points[0].y;
	sum[1] = points[1].x + points[1].y;
	sum[2] = points[2].x + points[2].y;
	sum[3] = points[3].x + points[3].y;

	double min = *std::min_element(sum.begin(), sum.end());
	double max = *std::max_element(sum.begin(), sum.end());

	int argMin = std::distance(sum.begin(), std::min_element(sum.begin(), sum.end()));
	int argMax = std::distance(sum.begin(), std::max_element(sum.begin(), sum.end()));

	newPoints[0] = points[argMin];
	newPoints[3] = points[argMax];

	diff[0] = points[0].x - points[0].y;
	diff[1] = points[1].x - points[1].y;
	diff[2] = points[2].x - points[2].y;
	diff[3] = points[3].x - points[3].y;

	min = *std::min_element(diff.begin(), diff.end());
	max = *std::max_element(diff.begin(), diff.end());
	argMin = std::distance(diff.begin(), std::min_element(diff.begin(), diff.end()));
	argMax = std::distance(diff.begin(), std::max_element(diff.begin(), diff.end()));

	newPoints[2] = points[argMin];
	newPoints[1] = points[argMax];

	return newPoints;
}

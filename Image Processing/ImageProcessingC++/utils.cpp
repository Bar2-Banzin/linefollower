# include "utils.h";

Mat thin_image(Mat image) {
	/**
	* Thinning Gray Scal Image

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

	// show_images([image, image_binary, thin], ["Original", "Binary", "Thin"])
	return thin;
}


//Basma: not sure about type if color 
bool color_center(int& x,int &y,Mat image, Scalar color) {
	/**
	* Get Center and Draw Rectangle Around Largest Contour of a given Color
	* 
	* @param found boolean to determine wether this color is found or not
	* @param x,y Center of the Color
	* 
	* @param image: RGB image with car on the track
	* @param color : RGB color i.e[0, 255, 0]
	*/

	//Get Color Mask
	Mat mask,masked_image;
	bool accepted = color_mask(mask,masked_image,image,color);
	if (!accepted)
		return false;

	//Find Mask Contours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(mask, contours, hierarchy, RETR_LIST,CHAIN_APPROX_SIMPLE);

	if (contours.size() <= 0) {
		cout << "color_center(): Couldn't find contours of color_range passed" << endl;
		x = -1;
		y = -1;
		return false;
	}

	//Sort contours
	std::sort(contours.begin(), contours.end(), compareContourAreas);

	//Grab contours [Biggest and Smallest]
	vector<Point> biggestContour = contours[contours.size() - 1];
	//std::vector<cv::Point> smallestContour = contours[0];


	//Caution:This Modifies on image  It Draws on it :D
	min_rectangle(image,x,y, biggestContour,color,10,true);

	//Show image is Here (image)
	return true;
}


bool color_mask(Mat&mask,Mat&masked_image, Mat image, Scalar color) {
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

	//Get Color Mask
	Scalar lower_range, upper_range;
	bool accepted =color_range(lower_range, upper_range, color);
	if (!accepted) {
		return false;
	}

	//Convert RGB image to HSV
	Mat image_hsv;
	cvtColor(image, image_hsv, COLOR_RGB2HSV);

	inRange(image_hsv, lower_range, upper_range, mask);

	//Check If This is Required
	image.copyTo(masked_image, mask);

	return true;
}


bool color_range(Scalar& lower_range, Scalar& upper_range,Scalar color) {
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

	if (Hue < 10 || Hue >255 - 10) {
		cout << "Error in Getting Range of Color" << endl;
		return false;
	}
	lower_range = Scalar(Hue - 10, 100, 100);
	upper_range = Scalar(Hue + 10, 255, 255);

	return true;
}

void  min_rectangle (Mat &image,int&x,int&y, vector<Point> contour, Scalar color, int thickness,bool draw) {
	/**
	* Get center of min rectangle around given contour
	*
	* @param center of rectangle
	* @param Can Return Dimensions of Rect and angle of Rotation [Need TO be Passed only :D]
	*
	* @param image : RGB image to Draw on it Rectangle
	* @param contour :  4 point contour
	* @param color color of Rectangle to be Drawn
	* @param thickness thickness of Rectangle to be Drawn
	* @param draw bool if true draw rectangle on image else no :( [Performance wise]
	*/

	//Min area of Rectangle 
	RotatedRect rect = minAreaRect(contour);

	//Get Points forming this Rectangle
	vector<Point2f> boxPts;
	boxPoints(rect, boxPts);


	//draw the Recatngle
	if (draw) {
		//Convert from float to int
		vector<Point2i> points;
		Mat(boxPts).convertTo(points, Mat(points).type());
		drawContours(image, points, 0, color, thickness);
	}

	//Center and angle of rotation of Rectangle
	Point2f center = rect.center;
	x = center.x;
	y = center.y;

	//Dimensions of the Rectangle 
	//int width = (int)(rect.size.width);
	//int height = (int)(rect.size.height);

	//Angle of Rotation
	//float angle_of_rotation = rect.angle;
	return;
}




Vec2i direction(int x1, int y1, int x2, int y2) {
	/**
	* Get vector x1y1 - x2y2
	*
	* @param x1, y1 : point 1
	* @param x2, y2 : point 2
	* 
	* @return vector x1y1 - x2y2
	*/
	return ((x2 - x1, y2 - y1));
}

float calculateDistance(int x1, int y1, int x2, int y2) {
	/**
	* Calculate the Euclidean distance between the two vectors x1, y1and x2, y2.
	*
	* @param x1, y1 : point 1
	* @param x2, y2 : point 2
	* 
	* @return Euclidean distance
	*
	* @return vector x1y1 - x2y2
	*/
	return ((x2 - x1, y2 - y1));
}


//Comparison function object
bool compareContourAreas(vector<Point> contour1, vector<Point> contour2) {
	double i = fabs(contourArea(Mat(contour1)));
	double j = fabs(contourArea(Mat(contour2)));
	return (i < j);
}
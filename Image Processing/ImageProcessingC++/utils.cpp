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
void color_center(bool&found ,int& x,int &y,Mat image, Vec3b color) {
	/**
	* Get Center and Draw Rectangle Around Largest Contour of a given Color
	* 
	* @param found boolean to determine wether this color is found or not
	* @param x,y Center of the Color
	* 
	* @param image: RGB image with car on the track
	* @param color : RGB color i.e[0, 255, 0]
	*/

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
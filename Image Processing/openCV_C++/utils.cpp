# include "utils.h";

Mat thin_image(Mat image) {
	/*
		 image : Gray Image
		Returns : thinned Binary Image
		*/
		//Convert to Binary
	//threshold(src_gray, dst, threshold_value, max_binary_value, threshold_type);
	Mat image_binary;
	threshold(image, image_binary, 127, 255, 0);

	//line is white
	bitwise_not(image_binary, image_binary);

	// Structuring Element
	Mat kernel;
	kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
	cout << kernel << endl;
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
		Mat result = thin;
		bitwise_or(thin, subset, result);
		thin = result;
		// Set the eroded image for next iteration
		image_binary = erode_image.clone();
	}
	// show_images([image, image_binary, thin], ["Original", "Binary", "Thin"])
	return thin;
}

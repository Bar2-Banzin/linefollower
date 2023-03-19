# include"utils.h"
# include "ScanTrack.h";

void extract_lines(Mat& image_lines, vector<Vec4i>& start_end_points, Mat image, double rho, double  theta, int threshold, double minLineLength, double  maxLineGap, int thickness) {
	/*
	Extract Line out of the RGB image
	Arguments :
image: RGB Track image
threshold : no of min votes so that line is detected as line by Hough
minLineLength : min length so the line is detected
maxLineGap : max allowed gap between lines to be treated as a 1 line(this breaks long un connected lines to 2 lines :D)
thickness : thickness of white line drawn for straight line(returned image)
Returns : Binary image of size = image size where 1's detect line
*/


// Convert the image to gray - scale
	Mat  image_gray;
	Mat kernel, Dilate;
	cvtColor(image, image_gray, COLOR_RGB2GRAY);


	// Thinning Image
	Mat thinned = thin_image(image_gray);
	// Dilate
	kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
	dilate(thinned, Dilate, kernel);

	// Find the edges in the image using canny detector
	Mat edges = Dilate;


	//# Hough Lines
	//	# Syntax:HoughLinesP(Edged image, Rho Resolution, Angle Resolution, threshold, min length of line, max distance between lines)
	//	# minLineLength : Lines shorter than that are rejected
	//	# maxLineGap : max allowed gap between lines to be treated as a 1 line(this breaks long un connected lines to 2 lines :D)
	//	# 0 <= rho < Rmax
	vector<Vec4i>lines;

	HoughLinesP(edges, lines, rho, theta, threshold, minLineLength, maxLineGap);
	cout << "Hough Lines Detected " << lines.size();

	// # Draw lines on the image
	image_lines = Mat::zeros(image.size(), CV_8UC1);//Inverted image


	/*	 Store start and end points of each line
		 start_end_points = np.empty((1, np.shape(lines))[0])# 1d with size = no of lines detected
		 FIXME: no of line, 1 * **, 4[Take care to tale[0] before taking point 😉](Solved this Problem by reshape)
	*/
	//Mat start_end_points = lines.reshape((np.shape(lines)[0], 4);
	start_end_points = lines;


	//Draw Lines on Image
	int index = 0;
	for (auto line_inst : lines) {
		int x1 = line_inst[0];
		int y1 = line_inst[1];

		int x2 = line_inst[2];
		int y2 = line_inst[3];
		line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(255 - index, 255, 255), thickness);
	}

		//# To See Each Line Detected Uncomment This
	for (int i; i < lines.size(); i++) {
		int x1 = lines[i][0];
		int y1 = lines[i][1];

		int x2 = lines[i][2];
		int y2 = lines[i][3];
		line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(255 - index, 255, 255), thickness);
		imshow("Track Lines", image_lines);
		waitKey(0);

	}

		//# Take one channel(Binary)
		//# image_lines = (image_lines[:, : , 0]//255).astype(int)
		//	image_lines = (image_lines[:, : , 0]).astype(int)


	//		if (globals.debug) :
	//			show_images([image, image_gray], ['Original', "Gray"], windowTitle = "extract_lines", BGR = False)
	//			show_images([thinned, edges, image_lines], ['Thinned', 'Edges', 'Lines Matrix'], windowTitle = "extract_lines", BGR = False)

	//			# Return a Binary Image(1 Channel) for the lines
	//			return image_lines, start_end_points

	return;
}


void scan_track(bool& wrapped, Mat& image_lines, vector<Vec4i>& start_end_points, Mat image, int thickness) {

	//track_image : BGR Track Image
	//thickness : Thickness of the wite line Drawn : )
	//return: Matrix of size = Image size and lines Detected are 1's

	// 1.Extract Track Paper From the Image
	//# paper_img, wrapped = ExtractPaper(track_image)

    // if(not wrapped):
	//     return False, None, None

	// 2.Detect Straight Lines in the Track
	Mat paper_img = image; // Temp till we have paper_img returned Correctly
	extract_lines(image_lines, start_end_points, paper_img, thickness = thickness);

	wrapped = true;

	return;
}
# include"utils.h"
# include "ScanTrack.h"

bool scan_track(Mat & image_lines, Mat & track_image) {
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
	extract_lines(image_lines, paper_img);
	return true;
}


struct str {
	bool operator() (Point a, Point b) {
		if (a.y != b.y)
			return a.y < b.y;
		return a.x <= b.x;
	}
} comp;

void extract_lines(Mat & image_lines, Mat& image,int sliding, double rho, double  theta, int threshold, double minLineLength, double  maxLineGap, int thickness) {
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
	kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
	dilate(thinned, Dilate, kernel);

	// Find the edges in the image using canny detector
	Mat edges = Dilate;

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

	for (auto line_inst : lines) {
		double x1 = line_inst[0];
		double y1 = line_inst[1];

		double x2 = line_inst[2];
		double y2 = line_inst[3];

		double change_x = x2 - x1, change_y = y2 - y1;
		double length = sqrt(pow(change_x, 2) + pow(change_y, 2));
		change_x /= length; change_y /= length;
		change_x *= 0.2*length;
		change_y *=  0.2*length;
		x1 += change_x;
		y1 += change_y;

		x2 -= change_x;
		y2 -= change_y;

		line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(250, 250, 250), thickness);
	}
	return;
}
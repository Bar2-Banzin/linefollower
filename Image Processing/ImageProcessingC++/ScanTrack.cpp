#include <math.h>

# include"utils.h"
#include"common.h"
# include "ScanTrack.h";

//bool scan_track(Mat& image_lines,Mat& transformation_matrix, vector<Vec4i>& start_end_points, Mat& track_image) {
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
	//namedWindow("Wrapped Paper  scan_track()", WINDOW_NORMAL);
	//imshow("Wrapped Paper  scan_track()", paper_img);
	//imwrite("./assets/TestCases/TestCase" + std::to_string(testcase) + "/results/track_paper.jpeg", paper_img);
	//waitKey(0);

	//if (! wrapped) {
		//return false;
	 //}

	//Uncomment to Disable extract_paper
	paper_img = track_image;

	// 2.Detect Straight Lines in the Track
	//extract_lines(image_lines, start_end_points, paper_img);
	extract_lines(image_lines, paper_img);
	//namedWindow("image_lines scan_track()", WINDOW_NORMAL);
	//imshow("image_lines scan_track()", image_lines);
	//waitKey(0);

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
	//imshow("thinned_image extract_lines()", thinned);
	//waitKey(0);

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
	//HoughLinesP(edges, lines, rho, theta, 100, minLineLength, maxLineGap);
	//imshow("	Diatled image", edges);
	//waitKey(0);

	HoughLinesP(edges, lines, 1, theta, threshold, minLineLength, maxLineGap);
	cout << "Hough Lines Detected " << lines.size();

	// # Draw lines on the image
	image_lines = Mat::zeros(image.size(), CV_8UC1);//Inverted image

	/*
	Store start and end points of each line
	start_end_points = np.empty((1, np.shape(lines))[0])# 1d with size = no of lines detected
	FIXME: no of line, 1 * **, 4[Take care to tale[0] before taking point 😉](Solved this Problem by reshape)
	*/
	//Mat start_end_points = lines.reshape((np.shape(lines)[0], 4);
	//start_end_points = lines;

	//cout << "Lines" << endl;
	//for (auto it : lines) {
	//	cout<< it << endl;
	//}

	//Draw Lines on Image
	//int index = 0;
	//int sliding = 50;
	for (auto line_inst : lines) {
		double x1 = line_inst[0];
		double y1 = line_inst[1];

		double x2 = line_inst[2];
		double y2 = line_inst[3];

		double change_x = x2 - x1, change_y = y2 - y1;
		double length = sqrt(pow(change_x, 2) + pow(change_y, 2));//length
		change_x /= length; change_y /= length;
		change_x *= 0.20*length;///////////Basma
		change_y *= 0.20* length;///////////Basma
		x1 += change_x;
		y1 += change_y;

		x2 -= change_x;
		y2 -= change_y;
		//int m = (x1 - x2) / (y1 - y2);
		//int c = x2 - m*y2;
		//double a = y1 - y2, b = x2 - x1, c = x1 * y2 - x2 * y1;
		//int threshold_cut = 50;
		/*if (x1 > x2) {
			x1 = x1 - threshold_cut;
			y1 = (a * x1 + c) / -b;

			x2 = x2 + threshold_cut;
			y2 = (a * x2 + c) / -b;

		}
		else {
			x1 = x1 + threshold_cut;
			y1 = (a * x1 + c) / -b;

			x2 = x2 - threshold_cut;
			y2 = (a * x2 + c) / -b;;

		}*/
		//line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(255-index, 255, 255), thickness);
		line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(250, 250, 250), thickness);
		//cout << "1:" << x1 << "," << y1 << "=>" << "2:" << x2 << "," << y2 << endl;
		//index++;
	}

	////// to see each line detected uncomment this
	//for (int i=0; i < lines.size(); i++) {
	//	Mat image_line = Mat::zeros(image.size(), CV_8UC1);//Inverted image
	//	int x1 = lines[i][0];
	//	int y1 = lines[i][1];

	//	int x2 = lines[i][2];
	//	int y2 = lines[i][3];
	//	Mat temo= image_lines.clone();
	//	if(i==0)
	//		line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(255  ,0,0), 3);
	//	if(i==1)
	//		line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(0, 255, 0), 3);
	//	if (i == 2)
	//		line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(0, 0,255 ), 3);
	//	imshow("track_lines_loop", image_lines);
	//	waitKey(0);
	//}



	//imshow("image_lines extract_lines()", image_lines);
	//waitKey(0);
	return;
}
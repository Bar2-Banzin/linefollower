# include"utils.h"
# include "ScanTrack.h";


bool scan_track(Mat& image_lines, vector<Vec4i>& start_end_points, Mat track_image) {
	/**
	* Scan Track Without A Car to Detect Straight Lines locations

	* @param wrapped: Flag if True = Scanning is Done Sucessfully
	* @param image_lines: Matrix of size = Image size with St lines Drawn with color Depedning on its order
	* @param start_end_points: Vector of 4 for evey line x1,y1  x2,y2

	* @param track_image: BGR Track Image
	* @param thickness : Thickness of the wite line Drawn : )
	* @return  Matrix of size = Image size and lines Detected are 1's
	*/

	//1.Extract Track Paper From the Image
	Mat paper_img;
	//bool wrapped=extract_paper( paper_img, track_image,false);

	//if (! wrapped) {
	//	return false;
	// }

	//imshow("Wrapped Paper  scan_track()", paper_img);

	//Temp Till Tomorrow
	paper_img = track_image;

	// 2.Detect Straight Lines in the Track
	extract_lines(image_lines, start_end_points, paper_img);

	imshow("image_lines scan_track()", image_lines);

	return true;
}

struct str {
	bool operator() (Point a, Point b) {
		if (a.y != b.y)
			return a.y < b.y;
		return a.x <= b.x;
	}
} comp;

bool extract_paper(Mat& warped_image, Mat img_bgr, bool draw) {
	/**
	* extract paper out of the image
	* @param warped_image bgr warpPerspective
	*
	* @param img_bgr: bgr image
	* @param draw bool if true draw rectangle on image else no :( [Performance wise]
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

	/*median filter to remove salt and pepper
	syntax: image-kernel size
	blurred_image_median = cv2.medianblur(img_gray, 5)*/

	//===========================================================edge detection=========================================================
	//canny edge detection (optimal edge detector)
	Mat edged_image;
	Canny(blurred_image_gaussian, edged_image, 180, 255);

	//===========================================================erosion & dilation=======================================================
	//mat dilated_img = edged_image;
	//mat errored_img = edged_image;


	//============================================================getting contours=========================================================
	//findcontour() works best on binary images
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(edged_image, contours, hierarchy, RETR_LIST, CHAIN_APPROX_SIMPLE);

	if (contours.size() <= 0) {
		cout << "extractpaper(): couldn't extract Contours out of image" << endl;
		return false;
	}

	/*drawContours(image_rgb, contours, -1, Scalar(255, 0, 0), 5);
	imshow("Largest Contour paperextractor.cpp", image_rgb);
	std::string path = "./test.jpeg";
	imwrite(path, image_rgb);
	waitKey(0);*/


	//getting biggest rectangular contour
	vector<Point>biggest_contour;
	double max_area;
	get_biggest_rectangular_contour(biggest_contour, max_area, contours);


	//Solving problem of False Contours The max area must be Greater than 10 % of the Image area
	if (max_area < 0.10 * imgHeight * imgWidth)
	{
		cout << "extractpaper():Largest Contour is Very small :(" << endl;
		return false;
	}


	if (draw) {
		//draw paper contour
		drawContours(image_rgb, vector<vector<Point> >(1, biggest_contour), -1, Scalar(255, 0, 0), 10);
		//show it
		imshow("Largest Contour paperextractor.cpp", image_rgb);
		//waitKey(0);
	}

	//Sort 4 Corner for Prespective
	vector<Point2f>biggest_contour_ordered = reorderPoints(biggest_contour);

	//Prespective
	vector<Point2f>image_corner {Point(0,0),Point(imgWidth -1, 0),Point(0,imgHeight-1),Point(imgWidth-1, imgHeight-1) };
	Mat M=getPerspectiveTransform(biggest_contour_ordered, image_corner);

	warpPerspective(img_bgr, warped_image, M, Size(imgWidth, imgHeight));

	return true;
}

void extract_lines(Mat& image_lines, vector<Vec4i>& start_end_points, Mat image, double rho, double  theta, int threshold, double minLineLength, double  maxLineGap, int thickness) {
	/**
	* Extract Line out of the RGB image
	
	* @param image_lines: Matrix of size = Image size with St lines Drawn with color Depedning on its order
	* @param start_end_points: Vector of 4 for evey line x1,y1  x2,y2

	* @param image: RGB Track image
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
	kernel = getStructuringElement(MORPH_CROSS, Size(3,3));
	dilate(thinned, Dilate, kernel);

	// Find the edges in the image using canny detector
	Mat edges = Dilate;

	// Hough Lines
	// Syntax:HoughLinesP(Edged image, Rho Resolution, Angle Resolution, threshold, min length of line, max distance between lines)
	//	minLineLength : Lines shorter than that are rejected
	//	maxLineGap : max allowed gap between lines to be treated as a 1 line(this breaks long un connected lines to 2 lines :D)
	//	0 <= rho < Rmax
	vector<Vec4i>lines;
	//HoughLinesP(edges, lines, rho, theta, 100, minLineLength, maxLineGap);
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
	start_end_points = lines;
	
	//cout << "Lines" << endl;
	//for (auto it : lines) {
	//	cout<< it << endl;
	//}

	//Draw Lines on Image
	int index = 0;
	for (auto line_inst : lines) {
		int x1 = line_inst[0];
		int y1 = line_inst[1];

		int x2 = line_inst[2];
		int y2 = line_inst[3];
		line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(255-index, 255- index, 255- index), thickness);
		index++;
	}

	//// to see each line detected uncomment this
	//for (int i=0; i < lines.size(); i++) {
	//	Mat image_line = Mat::zeros(image.size(), CV_8UC1);//Inverted image
	//	int x1 = lines[i][0];
	//	int y1 = lines[i][1];

	//	int x2 = lines[i][2];
	//	int y2 = lines[i][3];
	//	line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(255 - i, 255, 255), 1);
	//	imshow("track_lines_loop", image_lines);
	//	waitKey(0);
	//}


	//if (globals.debug) :
	//	show_images([image, image_gray], ['original', "gray"], windowtitle = "extract_lines", bgr = false)
	//	show_images([thinned, edges, image_lines], ['thinned', 'edges', 'lines matrix'], windowtitle = "extract_lines", bgr = false)
	return;
}
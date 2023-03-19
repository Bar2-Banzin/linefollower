# include"utils.h"
# include "ScanTrack.h";


void scan_track(bool& wrapped, Mat& image_lines, vector<Vec4i>& start_end_points, Mat track_image) {
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
	extract_paper(wrapped, paper_img, track_image);

	//if (! wrapped) {
	//	wrapped = false;	
	//	return;
	// }

	// 2.Detect Straight Lines in the Track
	extract_lines(image_lines, start_end_points, paper_img);

	wrapped = true;
	return;
}


void extract_paper(bool &wrapped, Mat &WarpedColoredImage, Mat img_BGR) {
	/**
	* Extract Paper out of the image

	* @param Wrapped: Flag if True = Extraction is Done Sucessfully
	* @param WarpedColoredImage: RGB wrapped Page

	* @param img_BGR: BGR image
	*/

	//Convert to RGB Scale
	 cvtColor(img_BGR, WarpedColoredImage, COLOR_BGR2RGB);
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
		line(image_lines, Point(x1, y1), Point(x2, y2), Scalar(255-index, 255, 255), thickness);
	}

	//// to see each line detected uncomment this
	//for (int i=0; i < lines.size(); i++) {
	//	Mat image_line = Mat::zeros(image.size(), CV_8UC1);//Inverted image
	//	int x1 = lines[i][0];
	//	int y1 = lines[i][1];

	//	int x2 = lines[i][2];
	//	int y2 = lines[i][3];
	//	line(image_line, Point(x1, y1), Point(x2, y2), Scalar(255 - i, 255, 255), 1);
	//	imshow("track_lines_loop", image_lines);
	//	imwrite("./img.png", image_line);
	//	waitKey(0);
	//}


	//if (globals.debug) :
	//	show_images([image, image_gray], ['original', "gray"], windowtitle = "extract_lines", bgr = false)
	//	show_images([thinned, edges, image_lines], ['thinned', 'edges', 'lines matrix'], windowtitle = "extract_lines", bgr = false)

	imshow("image_lines", image_lines);
	waitKey(0);
	return;
}
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "ArucoDetector.h"
#include "ScanTrack.h"
#include "ScanCar.h"
#include "utils.h"
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp> //Include OpenCV header file


using namespace std;
using namespace cv;

static ArucoDetector* detector = nullptr;

extern "C" {
	// Attributes to prevent 'unused' function from being removed and to make it visible
	__attribute__((visibility("default"))) __attribute__((used))
	const char* version() {
		return CV_VERSION;
	}

	__attribute__((visibility("default"))) __attribute__((used))
	int* detect(uint8_t* markerPngBytes, int inBytesCount) {
		/*if (detector != nullptr) {
			delete detector;
			detector = nullptr;
		}*/

		vector<uint8_t> buffer(markerPngBytes, markerPngBytes + inBytesCount);
		Mat marker = imdecode(buffer, IMREAD_COLOR);
        int* inc_speed=new int;
        *inc_speed = 0;


        // imwrite("detect.jpg", marker);
		// detector = new ArucoDetector(marker, bits);
		//Step(1) Scan Track
        	//1.Read Track Image
        	//std::string path = "./assets/track/00.jpeg";
        	/*Mat image = marker.clone();//imread(path, 1); //Reading from a path



        	//2.Scanning Track Initially
        	Mat image_lines;
        	vector<Vec4i>start_end_points;
        	bool wrapped=scan_track(image_lines, start_end_points, image);
            if (!wrapped) {
        		//cout << "Failed To Scan Track" << endl;
        		return exit;
        	}
        	*/
        	/************************************************************************************************/
        	//step(2) find car on track
        	//1.read an car on track image
        	//std::string path2 = "./assets/ontrack/02.jpeg";
        	Mat image_lines =detector->get_image_lines();
            vector<Vec4i>start_end_points=detector->get_start_end_points();

        	Mat car_image =  marker.clone();//imread(path2, 1); //reading from a path
            Mat image=marker.clone();

            bool car_found;
            int x_center, y_center, x_f, y_f, x_b, y_b;
            Scalar front_color(255, 0, 0);//red
            Scalar back_color(0, 255, 0);//green
           car_found = find_car(x_center, y_center, x_f, y_f, x_b, y_b, car_image, front_color, back_color);

        	if (!car_found) {
        		//cout << "failed to find car 😟" << endl;
        		*inc_speed=55;
        		return inc_speed;
        	}

        	///************************************************************************************************/
        	//Step(3) Is Car on a straight line
        	//int x_center = 20;
        	//int y_center = 700;
        	bool on_line;
            int line_index;
            car_on_line(on_line, line_index, x_center, y_center, image_lines);
            if (!on_line) {
                //cout << "Car isn't on a straight line" << endl;
             //continue;

                *inc_speed=44;
                return inc_speed;
            }

        	//cout << "Car is on a straight line" << endl;

        	/************************************************************************************************/
        	//Step(4) Inc or Dec Speed
        	/*bool inc_speed=false;
        	int dist_threshold = 10;
        	Vec4i line = start_end_points[line_index];

        	*/

        	//Test Only Till Madbouly Finishes
        	//bool* inc_speed ;


            int dist_threshold = 150;
            //int x_f = 50, y_f = 200, x_b = 50, y_b = 250;
            Vec4i lineto = start_end_points[line_index];
            line(image, Point(x_f, y_f), Point(x_b, y_b), Scalar(0, 255, 255), 10);


        	//imshow("Original Track main.cpp", image);
            Mat draw_temp = car_image.clone();
            *inc_speed = increase_decrease_speed(draw_temp, x_f, y_f, x_b, y_b, lineto, dist_threshold);


        	/*if (!*exit) {
        		//cout << "Don't Inc Speed of the Car" << endl;
        		return exit;
        	}*/
        	//cout << "Inc Speed of the Car" << endl;
        	///**********************************************************************************************/
        	////--------- MATRIX DEFINITION
        	//cv::Mat mat0 = Mat::zeros(5, 2, CV_8UC1); //ones, eye
        	//cout << "mat0 = " << endl << " " << mat0 << endl << endl; //Display in shell

        	////Creating different types of images: Mat name(rows, cols, type, Scalar);
        	//Mat mat1(50, 20, CV_8UC1, Scalar(0));  //Create a 50 x 20 single-channel array with 8 bit unsigned numbers
        	////(pixels value = 0)
        	//Mat mat2(Size(80, 40), CV_8UC1, Scalar(127)); //Note: Size(width/cols,height/rows)
        	//Mat mat3(Size(50, 100), CV_8UC3, Scalar(0, 0, 255)); //Scalar = color (BGR). Scalar represents a 4-element vector

        	//Scalar my_color(255, 0, 0);
        	//Mat mat4(Size(50, 100), CV_8UC3, my_color);
        return inc_speed;
	}

	__attribute__((visibility("default"))) __attribute__((used))
	void destroyDetector() {
		if (detector != nullptr) {
			delete detector;
			detector = nullptr;
		}
	}

	__attribute__((visibility("default"))) __attribute__((used))
	void initDetector(uint8_t* markerPngBytes, int inBytesCount) {

            //OutputDebugString(L"This debug is worth an upvote;)");
			vector<uint8_t> buffer(markerPngBytes, markerPngBytes + inBytesCount);
            Mat marker = imdecode(buffer, IMREAD_COLOR);
            //Step(1) Scan Track
            //1.Read Track Image
            //std::string path = "./assets/track/00.jpeg";
            Mat image = marker.clone();//imread(path, 1); //Reading from a path
         /*    Mat koko;
            koko= imread("D:/subjects/3-2/Embedded System/projects/flutter app/flutter-opencv-stream-processing-master/zeinb/native_opencv/ios/Classes/1.jpeg",1);
*/


            //2.Scanning Track Initially

            Mat image_lines;
            vector<Vec4i>start_end_points;
            bool wrapped=scan_track(image_lines, start_end_points, image);
            //imshow("image_lines.jpg", image);
            detector = new ArucoDetector();
            detector->set_image_lines(image_lines);
            detector->set_start_end_points(start_end_points);

	}
}
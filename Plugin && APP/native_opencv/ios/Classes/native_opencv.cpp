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
	int* detect(int width, int height, uint8_t* bytes, bool isYUV ) {
		 int* inc_speed=new int;
         *inc_speed = 0;
		if (detector == nullptr) {
			return inc_speed;
		}


        Mat frame;
        if (isYUV) {
           Mat myyuv(height + height / 2, width, CV_8UC1, bytes);
           cvtColor(myyuv, frame, COLOR_YUV2BGRA_NV21);
        } else {
           frame = Mat(height, width, CV_8UC4, bytes);
        }






        	/************************************************************************************************/
        	//step(2) find car on track
        	//1.read an car on track image


            vector<Vec4i>start_end_points=detector->get_start_end_points();
            Mat image_lines =detector->get_image_lines();
        	Mat car_image =  frame.clone();//imread(path2, 1); //reading from a path
            Mat image=frame.clone();

            bool car_found;
            int x_center, y_center, x_f, y_f, x_b, y_b;
            Scalar front_color(255, 0, 0);//red
            Scalar back_color(0, 255, 0);//green
           car_found = find_car(x_center, y_center, x_f, y_f, x_b, y_b, car_image, front_color, back_color);

        	if (!car_found) {
        		//cout << "failed to find car 😟" << endl;
        		*inc_speed=5;
        		return inc_speed;
        	}

        	///************************************************************************************************/
        	//Step(3) Is Car on a straight line

        	bool on_line;
            int line_index;
            car_on_line(on_line, line_index, x_center, y_center, image_lines);
            if (!on_line) {
                //cout << "Car isn't on a straight line" << endl;
                //continue;

                *inc_speed=4;
                return inc_speed;
            }


        	/************************************************************************************************/
        	//Step(4) Inc or Dec Speed
            int dist_threshold = 150;

            Vec4i lineto = start_end_points[line_index];
            //line(image, Point(x_f, y_f), Point(x_b, y_b), Scalar(0, 255, 255), 10);
            //cv::line(image, Point(lineto[0], lineto[1]), Point(lineto[2], lineto[3]), Scalar(255, 0, 255), 5);

            Mat draw_temp = car_image.clone();
            *inc_speed = increase_decrease_speed(draw_temp, x_f, y_f, x_b, y_b, lineto, dist_threshold);



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
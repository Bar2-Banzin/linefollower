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
#include <ctime>
#include <string>
using namespace std;
using namespace cv;

static ArucoDetector* detector = nullptr;


void rotateMat(Mat &matImage, int rotation)
{
	if (rotation == 90) {
		transpose(matImage, matImage);
		flip(matImage, matImage, 1); //transpose+flip(1)=CW
	} else if (rotation == 270) {
		transpose(matImage, matImage);
		flip(matImage, matImage, 0); //transpose+flip(0)=CCW
	} else if (rotation == 180) {
		flip(matImage, matImage, -1);    //flip(-1)=180
	}
}

extern "C" {
	// Attributes to prevent 'unused' function from being removed and to make it visible
	__attribute__((visibility("default"))) __attribute__((used))
	const char* version() {
		return CV_VERSION;
	}

	__attribute__((visibility("default"))) __attribute__((used))
	int* detect(int width, int height ,uint8_t* bytes, bool isYUV,int rotation ) {
		 int* inc_speed=new int;
         *inc_speed = 0;
		if (detector == nullptr) {
			return inc_speed;
		}
        time_t now = time(0);

        // convert now to string form

        Mat frame;
        if (isYUV) {
           Mat myyuv(height + height / 2, width, CV_8UC1, bytes);
           cvtColor(myyuv, frame, COLOR_YUV2BGRA_NV21);
        } else {
           frame = Mat(height, width, CV_8UC4, bytes);
        }
        rotateMat(frame, rotation);

        /************************************************************************************************/
        //step(2) find car on track
        //1.read an car on track image

        Mat image_lines =detector->get_image_lines();
        Mat car_image =  frame.clone();//imread(path2, 1); //reading from a path
        Mat image=frame.clone();
        char* date_time = ctime(&now);
        string name = date_time;
        imwrite("./data/data/com.example.opencv_app/cache/saher.jpg",image );

        bool car_found;
        int x_center, y_center, x_f, y_f, x_b, y_b;
        Scalar front_color(255, 0, 0);//red
        Scalar back_color(0, 0, 255);//blue

        car_found=find_car(x_center, y_center, x_f, y_f, x_b, y_b, car_image,front_color,back_color);

        if (!car_found) {
            // failed to find car 😟
            *inc_speed=5;
            return inc_speed;
        }

        ///************************************************************************************************/
        //Step(3) Is Car on a straight line

        bool on_line=0;

        car_on_line(on_line, x_f, y_f, x_b, y_b, image_lines,100);
        *inc_speed=on_line;
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
	int* initDetector(uint8_t* markerPngBytes, int inBytesCount) {
        //Mat input = imread("Internal storage\\Pictures\\Screenshots", IMREAD_GRAYSCALE);
        //imwrite(outputImagePath, input);
        // get image from bits
        vector<uint8_t> buffer(markerPngBytes, markerPngBytes + inBytesCount);
        Mat marker = imdecode(buffer, IMREAD_COLOR);
        //Step(1) Scan Track
        //1.Read Track Image

        Mat image = marker.clone();

        imwrite("./data/data/com.example.opencv_app/cache/eslam.jpg",image );

        //2.Scanning Track Initially

        Mat image_lines;
        int* extract=new int;
        *extract = 0;
        bool wrapped=scan_track(image_lines, image);
        if (!wrapped) {
            *extract = 5;
            return extract;
        }

        detector = new ArucoDetector();
        detector->set_image_lines(image_lines);

        return extract;



	}
}
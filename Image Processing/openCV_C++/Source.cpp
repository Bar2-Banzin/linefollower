/* @functions:  imshow, waitKey, destroyAllWindows, imread, flip, namedWindow, destroyWindow
 * @brief:      Basic functions of OpenCV are shown
 * @author:     C. Mauricio Arteaga-Escamilla
 */
#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file
#include "ScanTrack.h";
using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
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

    //imshow("Display mat1", mat1);
    //imshow("Display mat2", mat2);
    //imshow("Display mat3", mat3);
    //imshow("Display mat4", mat4);
    //waitKey(0); //Wait for any key press = pause

    //destroyAllWindows(); //Close all windows


    ////--------- READING IMAGES
	std::string path = "1.png";

    Mat image = imread(path, 1); //Reading from a path


    bool wrapped;
    Mat image_lines;
    vector<Vec4i>start_end_points;
    scan_track(wrapped, image_lines, start_end_points, image);
    imshow("Track Lines", image_lines);
    waitKey(0);

    return 0;
}

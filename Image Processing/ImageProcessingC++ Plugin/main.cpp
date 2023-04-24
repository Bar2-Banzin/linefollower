#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp> //Include OpenCV header file

#include "ScanTrack.h";
#include "ScanCar.h";
#include "utils.h";
#include "common.h";

#include <chrono>
using namespace std::chrono;
using namespace cv;
using namespace std;

int testcase = 3;

int main(int argc, char** argv)
{
    std::string path = "./assets/TestCases/TestCase" + std::to_string(testcase) + "/track.jpg";
    Mat image = imread(path, 1); //Reading from a path

    Mat image_lines;
    bool wrapped = scan_track(image_lines, image);
    cout << "wrapped: " << wrapped << endl;
    if (!wrapped) {
        cout <<"warp" << wrapped << endl;
        return 0;

    }


    std::string path2 = "./assets/TestCases/TestCase" + std::to_string(testcase) + "/car.jpg";
    Mat car_image = imread(path2, 1); //reading from a path


     bool car_found;
    int x_center, y_center, x_f, y_f, x_b, y_b;
    Scalar front_color(255, 0, 0);//red
    Scalar back_color(0, 0, 255);//blue


    int indx=0;
    auto start = high_resolution_clock::now();
    car_found = find_car(x_center, y_center, x_f, y_f, x_b, y_b, car_image, front_color, back_color, indx);
    cout << "car_found: " << car_found << endl;
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by find car: "
        << duration.count() << " microseconds" << endl;
    if (!car_found) {
        // failed to find car 😟
       
        cout << 5 << endl;
        return 0;
       
    }

    ///************************************************************************************************/
    //Step(3) Is Car on a straight line

    bool on_line = 0;
    start = high_resolution_clock::now();
    car_on_line(on_line, x_f, y_f, x_b, y_b, image_lines, 100,indx);
    cout << "on_line: " << on_line << endl;
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by on line function: "
        << duration.count() << " microseconds" << endl;
    //cout << on_line << endl;

  

}

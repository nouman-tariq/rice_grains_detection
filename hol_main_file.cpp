#include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	// loading the image file
	Mat image;
	image = imread("backside1.jpg", IMREAD_COLOR);
	imshow("backside of the grain", image);
	waitKey(0);
	return 0;
}

// g++ test.cpp -o testoutput -std=c++11 `pkg-config --cflags --libs opencv`
// ./testoutput
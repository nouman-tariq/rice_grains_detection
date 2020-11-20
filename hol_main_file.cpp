// #include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;
// using namespace cv;

void image2binary()
{
	// converts the image to a bit stream!
	// return type should be specified!
}

void label_window()
{
}

void serialize_object()
{
}

void calculate_ellipse()
{
}

void deserialize_object()
{
}

void check_connected()
{
}

void separate_connected()
{
}

void process_object()
{
}

void get_scan(){

}

int RGB2HSV(){

}

int calcMask(){

}

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
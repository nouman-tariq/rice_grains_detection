#include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;
using namespace cv;


// GLOBALS
uint16_t WINDOWMAP[1][1000][4];
uint32_t OBJHDRSIZE;
uint16_t ROWHDRSIZE;
uint16_t SECTIONHDRSIZE;
uint16_t STREAMVALID;
float HSV_RANGES[][2] = {0.45, 0.66, 0.7, 1, 0.5, 1};


struct Ellipse
{
    float x = 0;
    float y = 0;
    float theta = 0;
    float w = 0;
    float l = 0;
    float x1 = 0;
    float y1 = 0;
    float x2 = 0;
    float y2 = 0;
};


/* functions definitions
for the time being, i will avoid calling these
functions through the head
*/

int main(int argc, char **argv)
{
	/*
	// loading the image file
	Mat image;
	image = imread("backside1.jpg", IMREAD_COLOR);
	imshow("backside of the grain", image);
	waitKey(0);
	return 0;
	*/
	OBJHDRSIZE = 8;
	ROWHDRSIZE = 2;
	SECTIONHDRSIZE = 4;

	uint16_t Connected_grains = 0;
//	uint16_t NUM_ROWS = 1257;
//	uint16_t NUM_COLS = 901;

	Mat image;
	image = imread("backside1.jpg", IMREAD_COLOR);

	int rows, scan_width, channels;
	rows = image.rows;	
	scan_width = image.cols;
	channels = image.channels();

	if (!rows)
	{
		STREAMVALID = 0;
	}
	
	uint16_t rowcount = 1;
	uint16_t windwidx = 1;
	uint16_t label_index = 0;

	for (size_t k = 0; k < channels; k++)
	{
		for (size_t j = 0; j < scan_width; j++)
		{
			WINDOWMAP[0][j][k] = 0;
		}
		
	}
	
	







}

// g++ test.cpp -o testoutput -std=c++11 `pkg-config --cflags --libs opencv`
// ./testoutput
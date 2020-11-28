// #include <opencv2/highgui.hpp>
#include <iostream>
using namespace std;
// using namespace cv;

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

bool calcMask(float *HSV, float HSV_RANGES[][2]);

int main(int argc, char **argv)
{
	/*
	// loading the image file
	Mat image;
	image = imread("backside1.jpg", IMREAD_COLOR);
	imshow("backside of the grain", image);
	waitKey(0);
	return 0;
	float *HSV = new float[3];
	HSV[0] = 0.5924;
	HSV[1] = 0.9697;
	HSV[2] = 0.5176;

	
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
	uint16_t wndwidx = 1;
	uint16_t label_index = 0;

	for (size_t k = 0; k < channels; k++)
	{
		for (size_t j = 0; j < scan_width; j++)
		{
			WINDOWMAP[0][j][k] = 0;
		}
		
	}
	
	// get_scan()

	while(true)
	{
		if (WINDOWMAP)
		{
			// how to check that the above array is empty? 
			// get_scan();

			if (STREAMVALID == 0)
			{
				cout<<"Stream ended .."<<endl;
			}
			else
			{
				wndwidx = 1;
			}

		}

		for (size_t col = 0; col < scan_width; col++)
		{
			if (WINDOWMAP[wndwidx][col][4] == 1)
			{
				int east = col;
				int burstpos = col;
				label_index += 1;

				while ((east<scan_width) && (WINDOWMAP[wndwidx][east+1][4] == 1))
				{
					east += 1;
				}

				// label_window()
				// serialize_object()
				// calculate_ellipse()


				// deserialize_object();
				// check_connected;

				// separate_connected_grains();				
				
			}
			
			// process_object();
		}
		
	for (size_t k = 0; k < channels; k++)
	{
		for (size_t j = 0; j < scan_width; j++)
		{
			WINDOWMAP[wndwidx][j][k] = 0;
		}
		
	}
	
	int wdw_height = sizeof(WINDOWMAP);
	if ((STREAMVALID == 0) && (wdw_height <= 1))
	{
		break;
	}

	}

	*/

	// float *HSV[3] = {0.5924, 0.9697, 0.5176};
}

bool calcMask(float *HSV, float ranges[][2])
{

	float H, S, V, hmin, hmax, smin, smax, vmin, vmax;
	bool Hbinary, Sbinary, Vbinary, BW;

	H = HSV[0];
	S = HSV[1];
	V = HSV[2];

	// Get thresholds for Hue.
	hmin = ranges[0][0];
	hmax = ranges[0][1];

	// Get thresholds for Saturation
	smin = ranges[1][0];
	smax = ranges[1][1];

	// Get thresholds for Value
	vmin = ranges[2][0];
	vmax = ranges[2][1];

	// Create mask based on thresholds
	Hbinary = (H >= hmin) & (H <= hmax);
	Sbinary = (S >= smin) & (S <= smax);
	Vbinary = (V >= vmin) & (V <= vmax);
	BW = Hbinary & Sbinary & Vbinary;

	return BW;
}

// g++ test.cpp -o testoutput -std=c++11 `pkg-config --cflags --libs opencv`
// ./testoutput
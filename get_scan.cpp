#include <iostream>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat img = imread("backside1.jpg");
    Mat rgb_planes[3];
    split(img, rgb_planes);

    int *ptr_imgidx, imgidx, HSV, bg;
	//    imgidx = 0;
	ptr_imgidx = &imgidx;
	if (imgidx == 0)
	{
		*ptr_imgidx = 1;
	}

	int rows = img.rows;
	int cols = img.cols;
	int *RGBApixels[1][cols][4];
	int streamvalid;


	if (rows < imgidx)
	{
		//        imgidx = rows;
		*RGBApixels[1][cols][4] = {};
		streamvalid = 0;
		imgidx = 0;
	}

	else
	{
		RGBApixels[1][cols][4] = {};
		for (int col = 1; col <= cols; col++)
		{
//			HSV = RGB2HSV();
//			bg = calcMask();
			if (bg == 0)
			{
				for (int i = 1; i <= 3; i++)
				{
					*RGBApixels[1][col][i] = 1; // image indexing left!
					*RGBApixels[1][col][4] = 1;
				}
			}
		}
		streamvalid = 1;
		imgidx += 1;
	}
}
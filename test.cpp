#include <iostream>
// #include <opencv2/highgui.hpp>
// using namespace cv;
using namespace std;

// void array1(int a, int b, int c, int *abc);

int main()
{

    // Mat img;
    // img = imread("backside1.jpg", IMREAD_COLOR);
    // Mat planes[3];
    // split(img, planes);
    // char f1 = img.data[0];
    // unsigned short R = *(img.ptr(0,0));
    // cout<<R<<endl;
    // uint8_t f2 = img.at<uint8_t>(0,0);
    // cout<<f2<<endl;
    // Vec3b intensity = img.at<Vec3b>(0,0); //(y,x)
    // double blue = intensity.val[2];
    // cout<<blue<<endl;
    // double R = img.data[img.cols + 0];
    // cout<<R<<endl;
    // double value = planes[2].data[5890];
    // cout << value << endl;
    // double R = *(planes[0].ptr(0, 0));
    // double G = *(planes[1].ptr(0, 0));
    // double B = *(planes[2].ptr(0, 0));

    double HSV[3];
	double var_max, var_min, del_max, del_R, del_G, del_B;
	double H, S, V;
    double R, G, B;
    R = 23; G = 116; B = 235;
	double var_R = R / 255.0;
	double var_G = G / 255.0;
	double var_B = B / 255.0;

	// finding the maximum
	if ((var_R >= var_G) && (var_R >= var_B))
	{
		var_max = var_R;
	}
	else if ((var_G >= var_R) && (var_G >= var_B))
	{
		var_max = var_G;
	}
	else if ((var_B >= var_R) && (var_G >= var_G))
	{
		var_max = var_B;
	}

	//finding the minimum
	if ((var_R <= var_G) && (var_R <= var_B))
	{
		var_min = var_R;
	}
	else if ((var_G <= var_R) && (var_G <= var_B))
	{
		var_min = var_G;
	}
	else if ((var_B <= var_R) && (var_G <= var_G))
	{
		var_min = var_B;
	}

	del_max = var_max - var_min;

	V = var_max;
	if (del_max == 0)
	{
		H = 0;
		S = 0;
	}
	else
	{
		S = del_max / var_max;
		del_R = (((var_max - var_R) / 6) + (del_max / 2)) / del_max;
		del_G = (((var_max - var_G) / 6) + (del_max / 2)) / del_max;
		del_B = (((var_max - var_B) / 6) + (del_max / 2)) / del_max;

		if (var_R == var_max)
		{
			H = del_B - del_G;
		}
		else if (var_G == var_max)
		{
			H = 0.3334 + del_R - del_B;
		}
		else if (var_B == var_max)
		{
			H = 0.6667 + del_G - del_R;
		}

		if (H < 0)
		{
			H = H + 1;
		}
		if (H > 1)
		{
			H = H - 1;
		}
	}
	
	HSV[0] = H; HSV[1] = S; HSV[2] = V;





}
/*
int a = 10;
int b = 20;
int c = 30;

int *abc = new int[3];
array1(a, b, c, abc);

cout<<abc[0]<<endl;
}

void array1(int a, int b, int c, int *abc)
{
    abc[0] = a+100;
    abc[1] = b;
    abc[2] = c;

}
*/

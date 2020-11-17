


#include <iostream>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat img = imread("backside1.jpg");
    Mat rgb_planes[3];
    split(img, rgb_planes);
    cout<<rgb_planes[1].row(2).col(2)<<endl;
    

}
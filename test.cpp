#include <iostream>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;
/*
// global variables
int value = 10;
int function();
*/



int main()
{
/*
    cout<<"Inside main value = "<<value<<endl;
    int x = function();
    cout<<"Value of x = "<<x<<endl;
*/

    Mat img;
    img = imread("backside1.jpg", IMREAD_COLOR);
    Mat planes[3];
    split(img, planes);
    cout<<planes[0].row(0).col(0)<<endl;

    

}



/*
int function()
{
    return value;
    cout<<"from function definition value = "<<value<<endl;
}
*/
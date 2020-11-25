#include <iostream>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

int main()
{
    FILE * pf;
    pf = fopen("beans_text1.txt", "wb");
    
    Mat img = imread("backside1.jpg");

    if (pf!=NULL)
    {
        fwrite(img.data, sizeof(uint8_t), sizeof(img),pf);
    }
    fclose(pf);
}
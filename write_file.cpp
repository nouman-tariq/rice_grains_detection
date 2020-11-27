#include <iostream>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

int main()
{
    FILE * pf;
    pf = fopen("beans_text1.txt", "wb");
    
    Mat img = imread("backside1.jpg");
    size_t imgSize = img.rows*img.cols*3;
    cout<<imgSize<<endl;
    if (pf!=NULL)
    {
        fwrite(img.data, sizeof(uint8_t), imgSize, pf);
    }
    fclose(pf);
}
#include <iostream>
using namespace std;

//#include <opencv2/highgui.hpp>
//using namespace cv;

int main()
{
    FILE *pf = fopen("beans_text1.txt", "rb");
    long lsize;
    fseek(pf, 0, SEEK_END);
    lsize = ftell(pf);
    rewind(pf);

    uint8_t *buffer;
    buffer = (uint8_t *)malloc(sizeof(uint8_t) * lsize);

    size_t result;
    result = fread(buffer, 1, lsize, pf);
    fclose(pf);

    int num_rows = 309;
    int num_cols = 116;

    

//    Mat img[96];
//    imshow("image", buffer);
//    waitKey(0);
}
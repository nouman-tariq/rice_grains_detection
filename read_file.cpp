#include <iostream>
using namespace std;

int main()
{
    FILE *pf = fopen("beans_text1.txt", "rb");
    long lsize;
    fseek(pf, 0, SEEK_END);
    lsize = ftell(pf);
    rewind(pf);

    uint16_t *buffer;
    buffer = (uint16_t *)malloc(sizeof(uint16_t) * lsize);

    size_t result;
    result = fread(buffer, 1, lsize, pf);
    fclose(pf);

    int num_rows = 309;
    int num_cols = 116;

//    Mat img;
//    imshow("image", buffer);
//    waitKey(0);
}
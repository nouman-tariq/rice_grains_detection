#include <iostream>

using namespace std;

#define OBJHDRSIZE
#define ROWHDRSIZE
#define SECTIONHDRSIZE

int array2uint(uint8_t array[])
{
    // need to make this function
}

int main()
{
    int label, num_rows, object_width, data_index;
    int out_image[num_rows][object_width][3] = {}; // verify initialized to 0 or not!
    uint8_t label_arr[4], num_rows_arr[2], object_width_arr[2], indata[3080][3080];

    for (size_t i = 0; i < 4; i++)
    {
        label_arr[i] = indata[i][0];
    }
    label = array2uint(label_arr);

    for (size_t i = 0; i < 2; i++)
    {
        num_rows_arr[i] = indata[i+4][0];
    }
    num_rows = array2uint(label_arr);

    for (size_t i = 0; i < 2; i++)
    {
        object_width_arr[i] = indata[i+6][0];
    }
    object_width = array2uint(label_arr);

    data_index = OBJHDRSIZE + 1;

    
}
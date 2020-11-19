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
    int label, num_rows, object_width, data_index, num_sections, col_start, section_size;
    uint8_t out_image[num_rows][object_width][3] = {}; // verify initialized to 0 or not!
    uint8_t label_arr[4], num_rows_arr[2], object_width_arr[2],num_sections_arr[2],col_start_arr[2], section_size_arr[2] ,indata[3080][3080];

    for (size_t i = 0; i < 4; i++)
    {
        label_arr[i] = indata[i][0];
    }
    label = array2uint(label_arr);

    for (size_t i = 0; i < 2; i++)
    {
        num_rows_arr[i] = indata[i+4][0];
    }
    num_rows = array2uint(num_rows_arr);

    for (size_t i = 0; i < 2; i++)
    {
        object_width_arr[i] = indata[i+6][0];
    }
    object_width = array2uint(object_width_arr);

    data_index = OBJHDRSIZE + 1;

    // out_image initialization to zero.

    for (size_t row = 0; row < num_rows; row++)
    {
    
        for (size_t i = 0; i < 2; i++)
        {
            num_sections_arr[i] = indata[data_index+i][0];
        }
        num_sections = array2uint(num_sections_arr);

        data_index = ROWHDRSIZE + data_index;
        for (size_t i = 0; i < num_sections; i++)
        {
            
            for (size_t i = 0; i < 2; i++)
            {
                col_start_arr[i] = indata[data_index+i][0];
            }
            col_start = array2uint(col_start_arr);

            
            for (size_t i = 0; i < 2; i++)
            {
                section_size_arr[i] = indata[data_index+2+i][0];
            }
            section_size = array2uint(section_size_arr);

            data_index = SECTIONHDRSIZE + data_index;
            for (size_t i = 0; i < section_size-1; i++)
            {
                out_image[row][col_start+i][0] = indata[data_index][0];
                out_image[row][col_start+i][1] = indata[data_index+1][0];
                out_image[row][col_start+i][2] = indata[data_index+2][0];
                data_index = data_index + 3;
            }
            
        }
        

    }
    
    
}
#include <iostream>

using namespace std;

#define OBJHDRSIZE
#define ROWHDRSIZE
#define SECTIONHDRSIZE

int uint2array(int id, int size, int array[])
{
    array[size] = {};
    // need to make this function
}

int main()
{

    double inimage[][901][4] = {};
    double row_ends[][2] = {};
    double left_position, width;

    uint32_t ID = 0;
    double data_ptr = OBJHDRSIZE + ROWHDRSIZE + SECTIONHDRSIZE + 1;

    double num_rows = sizeof(row_ends) / sizeof(row_ends[0]);
    int out_size = (num_rows*width) + (num_rows*20);
    int outdata[][out_size] = {};

    // need of initializing outdata to zero?
    
    int hdr_id[4];
    uint2array(ID, 4, hdr_id);
    for (size_t i = 0; i < 4; i++)
    {
        outdata[i][0] = hdr_id[i];
    }
    
    int hdr_num_rows[2];
    uint2array(num_rows, 2, hdr_num_rows);
    for (size_t i = 0; i < 2; i++)
    {
        outdata[i+4][0] = hdr_num_rows[i];
    }

    int hdr_width[2];
    uint2array(width, 2, hdr_width);
    for (size_t i = 0; i < 2; i++)
    {
        outdata[i+6][0] = hdr_width[i];
    }

    int num_sections = 1;
    int section_size = 0;

    
    
    
}
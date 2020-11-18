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

    int rowhdrptr = OBJHDRSIZE+1;
    int sectionhdrptr = OBJHDRSIZE + ROWHDRSIZE + 1;

    for (size_t row = 0; row < num_rows; row++)
    {
        int instartcol = row_ends[row][0];
        int shift = row_ends[row][0] - left_position;
        int num_cols = 1 + (row_ends[row][1] - row_ends[row][0]);

        int idx = instartcol;
        while (idx<(instartcol + num_cols))
        {
            uint16_t sectionpos = (idx - instartcol + shift + 1);
            while ((idx<(instartcol+num_cols)) && (inimage[row][idx][4] != 0))
            {
                for (size_t i = data_ptr; i < data_ptr+2; i++)
                {
                    num_sections += 1;
                    section_size = 0;
                    sectionhdrptr = data_ptr;
                    data_ptr = SECTIONHDRSIZE + sectionhdrptr;
                    while (inimage[row][idx][4] == 0)
                    {
                        idx += 1;
                    }
                    
                }
                
            }
            
        }
        int row_hdr_array[2];
        uint2array(ID, 2, row_hdr_array);
        for (size_t i = 0; i < 2; i++)
        {
            outdata[rowhdrptr+i][0] = row_hdr_array[i];
        }

        rowhdrptr = data_ptr;
        sectionhdrptr = ROWHDRSIZE + rowhdrptr;       
        data_ptr = SECTIONHDRSIZE + sectionhdrptr;

        num_sections = 1;
        section_size = 0;
    }
       
}
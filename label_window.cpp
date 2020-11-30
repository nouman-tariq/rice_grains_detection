#include <iostream>
#include <vector>
using namespace std;

int main()
{
    static int moments, most_left, recursion_cnt;
    int section_start, instream, hsv_ranges, most_right;
    static int object_edges[][2] = {};
    int row, col;
    
    // GLOBALS
    int STREAMVALID;
    // WINDOWMAP

    if (!recursion_cnt)
    {
        recursion_cnt = 1;
        object_edges[1][1] = {section_start};
        object_edges[1][2] = {col};
        most_left = section_start;
        most_right = col;
        // moments = struct('m00', 0, 'm10', 0, 'm01', 0, 'm11', 0, 'm02', 0, 'm20', 0);
    }
    else
    {
        recursion_cnt += 1;
    }

    double height; //size of object_edges.
    height = sizeof(object_edges);

    if (height < row)
    {
        object_edges[row][1] = {col};
        object_edges[row][2] = {col};
    }

    if (col > object_edges[row][2])
    {
        object_edges[row][2] = col;
        most_right = col;
    }

    //while loop containing WINDOWMAP
    double WINDOWMAP[][col][4] = {};
    while ((col > 1) && (WINDOWMAP[row][col - 1][4] == 1))
    {
        col = col - 1;
    }

    int m = col;

    if (col < object_edges[row][1])
    {
        object_edges[row][1] = {col};
    }

    // size of WINDOWMAP
    int scan_width = sizeof(WINDOWMAP) / sizeof(WINDOWMAP[row][col - 1]);

    if ((m <= scan_width) && (WINDOWMAP[row][m][4]) == 1)
    {
        if (m < most_left)
        {
            most_left = m;
        }

        int data_valid = 1;
        while (data_valid == 1)
        {
            // moments = update_moments(moments, row, m);
            WINDOWMAP[row][m][4] = 2;

            if (( (m + 1) > scan_width) || (WINDOWMAP[row][m + 1][4] != 1))
            {
                data_valid = 0;
                if (object_edges[row][2] < m)
                {
                    object_edges[row][2] = m;
                }
            }
            m = m + 1;
        }
    }

    while ((col<m) && (col>0) && (col<=scan_width))
    {
        if ((row>1) && (WINDOWMAP[row-1][col][4] == 1))
        {
           // label_window(section_start, row-1, col, instream, hsv_ranges);
        }

        int wrows = sizeof(WINDOWMAP) / sizeof(WINDOWMAP[1]);
        if ((wrows == row) && (STREAMVALID == 1))
        {
            // = get_scan(instream, hsv_ranges);
        }
        
        if (WINDOWMAP[row+1][col][4] == 1)
        {
            // label_window(section_start, row-1, col, instream, hsv_ranges);
        }
                
        col = col + 1;        
    }


    recursion_cnt -= 1;
    if (recursion_cnt == 0)
    {
        recursion_cnt = {};
        int obj_edges = object_edges[0][2];
        int left_position = most_left;
        int out_moments = moments;
        int width = (most_right - most_left) + 1;
    }
    
    
}

/*
inputs: 
Ellipse (structure)
base_size

outputs:
is_connected
ave_value

*/


#include <iostream>

using namespace std;

int main()
{
    static int ave_cnt, CMA_ave;
    int MIN_AVE_COUNT = 5;
    int is_connected, ave_value, ellipse_size, base_size;

    if (!CMA_ave)
    {
        ave_cnt = 0;
        CMA_ave = 0;
    }

    // ellipse_size = Ellipse(end).l

    if (true) // nargin < 2
    {
        if (ave_cnt < MIN_AVE_COUNT)
        {
            is_connected = 0;
            CMA_ave = CMA_ave + (ellipse_size - CMA_ave) / (ave_cnt + 1);
            ave_cnt++;
        }
        else
        {
            if (ellipse_size >= CMA_ave*1.6)
            {
                is_connected = 1;
            }
            else
            {
                CMA_ave = CMA_ave + (ellipse_size - CMA_ave) / (ave_cnt + 1);
                is_connected = 0;
                ave_cnt++;
            }            
        }

        ave_value = CMA_ave;
        
        
    }
    else
    {
        if (ellipse_size >= base_size*1.6)
        {
            is_connected = 1;
        }
        else
        {
            is_connected = 0;
        }
        ave_value = base_size;
        
        
    }
    
    
    
}
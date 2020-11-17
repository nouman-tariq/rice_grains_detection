/*
GOALS:
1. convert incomming R,G,B into double
2. format of RGB in opencv is BGR!
*/

#include <iostream>

using namespace std;

int main()
{
    // input values between 0 and 255
    double R, G, B, var_max, var_min, del_max, del_R, del_G, del_B;

    // output H, S, V (float) values between 0 and 1
    double H, S, V;

    double var_R = R / 255.0;
    double var_G = R / 255.0;
    double var_B = R / 255.0;

    // finding the maximum
    if ((var_R >= var_G) && (var_R >= var_B))
    {
        var_max = var_R;
    }
    else if ((var_G >= var_R) && (var_G >= var_B))
    {
        var_max = var_G;
    }
    else if ((var_B >= var_R) && (var_G >= var_G))
    {
        var_max = var_B;
    }

    //finding the minimum
    if ((var_R <= var_G) && (var_R <= var_B))
    {
        var_min = var_R;
    }
    else if ((var_G <= var_R) && (var_G <= var_B))
    {
        var_min = var_G;
    }
    else if ((var_B <= var_R) && (var_G <= var_G))
    {
        var_min = var_B;
    }

    del_max = var_max - var_min;

    V = var_max;
    if (del_max == 0)
    {
        H = 0;
        S = 0;
    }
    else
    {
        S = del_max / var_max;
        del_R = (((var_max - var_R) / 6) + (del_max / 2)) / del_max;
        del_G = (((var_max - var_G) / 6) + (del_max / 2)) / del_max;
        del_B = (((var_max - var_B) / 6) + (del_max / 2)) / del_max;
 
        if (var_R == var_max)
        {
            H = del_B - del_G;
        }
        else if (var_G == var_max)
        {
            H = (1 / 3) + del_R - del_B;
        }
        else if (var_B == var_max)
        {
            H = (2 / 3) + del_G - del_R;
        }

        if (H < 0){
            H = H + 1;
        }
        if (H > 1){
            H = H - 1;
        }
    }
}
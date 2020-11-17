#include <iostream>

using namespace std;

int main()
{
    /*
    output = BW
    input = HSV, ranges
    HSV = array of 3 double values
    ranges = 3x2 matrix [.45,.66; .7,1; .5,1];
    */

    // random initialization
    double ranges[3][2] = {{0.45, 0.66}, {0.7, 1}, {0.5, 1}};
    double HSV[3] = {1, 1, 3};

    double H, S, V, hmin, hmax, smin, smax, vmin, vmax;
    bool Hbinary, Sbinary, Vbinary, BW;

    H = HSV[0];
    S = HSV[1];
    V = HSV[2];

    // Get thresholds for Hue.
    hmin = ranges[0][0];
    hmax = ranges[0][1];

    // Get thresholds for Saturation
    smin = ranges[1][0];
    smax = ranges[1][1];

    // Get thresholds for Value
    vmin = ranges[2][0];
    vmax = ranges[2][1];

    // Create mask based on thresholds
    Hbinary = (H >= hmin) & (H <= hmax);
    Sbinary = (S >= smin) & (S <= smax);
    Vbinary = (V >= vmin) & (V <= vmax);
    BW = Hbinary & Sbinary & Vbinary;
}
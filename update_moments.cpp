#include <iostream>

using namespace std;
// need to initialize the array to zero, somewhere else in the main program.
int main()
{
    double *M[6] = {};
    int row, col;
    int m00, m10, m01, m11, m02, m20;
    m00 = 0; m10 = 1; m01 = 2; m11 = 3; m02 = 4; m20 = 5;

    M[m00] = M[m00] + ((col^0) * (row^0));
    M[m10] = M[m10] + ((col^1) * (row^0));
    M[m01] = M[m01] + ((col^0) * (row^1));
    M[m11] = M[m11] + ((col^1) * (row^1));
    M[m02] = M[m02] + ((col^0) * (row^2));
    M[m20] = M[m20] + ((col^2) * (row^0));
}
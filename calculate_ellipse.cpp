#include <iostream>
#include <cmath>
using namespace std;

struct Ellipse
{
    float x = 0;
    float y = 0;
    float theta = 0;
    float w = 0;
    float l = 0;
    float x1 = 0;
    float y1 = 0;
    float x2 = 0;
    float y2 = 0;
};

struct moments
{
    float m00 = 0;
    float m10 = 0;
    float m01 = 0;
    float m11 = 0;
    float m02 = 0;
    float m20 = 0;
};

int main()
{  
    moments M; //input parameter!
    Ellipse E;
    float a, b, c, d;

    E.x = M.m10/M.m00;
    E.y = M.m01/M.m00;

    a = M.m20/M.m00 - (pow(E.x, 2));
    b = 2*(M.m11/M.m00 - ((E.x)*(E.y)));
    c = M.m02/M.m00 - (pow(E.y, 2));

    E.theta = 1/2*atan(b/(a-c)) + (a<c)*3.14/2.0;
    E.w = sqrt(8*(a+c-sqrt((pow(b, 2))+pow((a-c), 2))))/2.0;
    E.l = sqrt(8*(a+c+sqrt((pow(b, 2))+pow((a-c), 2))))/2.0;

    d = sqrt(pow(E.l, 2) -pow(E.w, 2));
    E.x1 = E.x + d*cos(E.theta);
    E.y1 = E.y + d*sin(E.theta);
    E.x2 = E.x - d*cos(E.theta);
    E.y2 = E.y - d*sin(E.theta);

}
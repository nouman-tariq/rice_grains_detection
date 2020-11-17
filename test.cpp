#include <iostream>

using namespace std;

int main()
{
    int array[3][2] = {0, 0, 0, 0, 1, 1};
    cout<<sizeof(array)/sizeof(array[0])<<endl;
}
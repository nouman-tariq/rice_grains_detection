#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<vector<double>> test;
    test = {
        {1, 2, 3},
        {4, 5, 6}
    };


    int array[1][4][3] = {
        {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}}
    };
    
    cout<<sizeof(array)/sizeof(array[1])<<endl;
}
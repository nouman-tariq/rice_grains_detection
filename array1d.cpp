#include <iostream>

using namespace std;

int update_array(int *array)
{
    array[0] += 1;
    array[1] += 1;
    array[2] += 1;
}

int main()
{

    int *array = new int[3];

    array[0] = 10;
    array[1] = 20;
    array[2] = 30;

    update_array(array);
}
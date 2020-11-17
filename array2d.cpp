#include <iostream>
using namespace std;

void update_array(int array[][2])
{
    array[0][0] += 1;
    array[0][1] += 1;
}

int main()
{
    int array[1][2] = {10, 20};
    update_array(array);
}
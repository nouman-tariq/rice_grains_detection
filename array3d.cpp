#include <iostream>
using namespace std;

// added the project to the git
// synchronized with the ubuntu as well
// fonts have been changed as well, Alhamdu lilah almost done with preferences!

void update_array(int array[][2][3])
{
    array[0][0][0] += 100;
    array[0][1][0] += 100;
}

int main()
{
    int array[1][2][3];
    array[0][0][0] = 10;
    array[0][0][1] = 11;
    array[0][0][2] = 12;
    array[0][1][0] = 20;
    array[0][1][1] = 21;
    array[0][1][2] = 22;
    
    update_array(array);
}
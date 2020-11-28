#include <iostream>
using namespace std;

// global variables
int value = 10;

int function();

int main()
{
    cout<<"Inside main value = "<<value<<endl;
    int x = function();
    cout<<"Value of x = "<<x<<endl;

}

int function()
{
    return value;
    cout<<"from function definition value = "<<value<<endl;
}
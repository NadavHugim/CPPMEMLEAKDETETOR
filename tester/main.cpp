
#include "memleakdetection.hpp"

using namespace std;

int main()
{
    int * pint = new int[15];
    
    PrintMemory();

    return 0;
}

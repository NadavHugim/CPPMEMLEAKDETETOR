
#include "memleakdetection.hpp"

using namespace std;

int main()
{
    int * pint = new int[15];
    char * pchar = new char[20];
    delete pint;
    PrintMemory();

    return 0;
}

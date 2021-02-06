# CPPMEMLEAKDETETOR
C++ (hpp) single file memory leak detector

This is a single HPP file that allow any C++ developer to detect any heap memory leak in his project EZ step:

1) Add the file “memleakdetection.hpp” to your project (no dependency)
2) Build your project in Debug or Release
3) Run!

----------------------------------------------------------------------------------------------------
The code in the tester is something like this:

#include "memleakdetection.hpp"

int main()
{
    int * pint = new int[15];

    PrintMemory();

    return 0;
}

----------------------------------------------------------------------------------------------------

The output to the screen (and file) is:

memory address = 0xff20d8

memory size = 60

ProcessName = /home/pi/Desktop/MemoryLeakDetector/MemLeakDetector/bin/Debug/MemLeakDetector

Process Time = Sat Feb  6 19:49:18 2021

ProcessId = 8415

ThreadId = 3069472784

code line = 6

code function = int main()

code filename = /home/pi/Desktop/MemoryLeakDetector/MemLeakDetector/main.cpp

Is Array = true

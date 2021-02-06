#ifndef NEWDELETE
#define NEWDELETE

#include <iostream>
#include <fstream>
#include <map>
#include <threads.h>
#include <string>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

using namespace std;

typedef struct
{
	bool bISArray;
	time_t DateTime;
	__pid_t ProcessId;
	thrd_t ThreadId;
	void * pMemory ;
	size_t MemorySize;
	size_t Linenumber;
	string Filename;
	string Function;
	string ProcessName;
	string Error;
} INFO, *PINFO ;

map<void*, PINFO> MemoryMap;

ostream & operator << (ostream & os, const INFO & info)
{
    os << hex << "memory address = " << info.pMemory << endl ;
    os << dec << "memory size = " << info.MemorySize << endl ;
    os << "ProcessName = " << info.ProcessName << endl ;
    os << "Process Time = " << ctime(&info.DateTime) ;
    os << "ProcessId = " << info.ProcessId << endl ;
    os << "ThreadId = " << info.ThreadId << endl ;
    os << "code line = " << info.Linenumber << endl ;
    os << "code function = " << info.Function << endl ;
    os << "code filename = " << info.Filename << endl ;
    os << "Is Array = " << std::boolalpha <<info.bISArray << endl ;
    os << endl<< endl<< endl;
    return os;
}


void * AddMemory(size_t memsize,  char const* filename,  char const * function, size_t linenumber, bool bISArray)
{
    char buffer[PATH_MAX]{0};

	PINFO pmem =  (PINFO) malloc(sizeof(INFO));

	if(NULL != pmem)
	{
		pmem->bISArray = bISArray;
		pmem->DateTime = time(0);
		pmem->pMemory = malloc(memsize);
		pmem->Filename = filename;
		pmem->Function = function;
		pmem->Linenumber = linenumber;
		pmem->MemorySize = memsize;
		pmem->ProcessId = getpid();
		pmem->ThreadId =  thrd_current();
		readlink( "/proc/self/exe", buffer, PATH_MAX-1 );
		pmem->ProcessName = buffer;

		MemoryMap[pmem->pMemory] = pmem;

		if(NULL != pmem->pMemory)
			return pmem->pMemory;
	}

	return NULL ;
}
void RemoveMemory(void* mem,bool bISArray)
{
	PINFO pmem = (PINFO) MemoryMap[mem];

	if(NULL != pmem)
	{
        if(bISArray != pmem->bISArray)
        {
            if(pmem->bISArray)
                pmem->Error = "ERORR: to delete array you must use delete[] and not delete";
            else
                pmem->Error = "ERORR: to delete single var you must use delete and not delete[]";

           cout << pmem->Error << endl;

        }
        else
        {
            if(0 < MemoryMap.count(mem))
                MemoryMap.erase(mem);

            free(pmem->pMemory);
            free(pmem);
		}
	}
}
void PrintMemory()
{
	int memoryLeakCounter = 0;
	ofstream  traceleak ( "/tmp/MemoryLeakFond.txt");

	for (auto MemoryMapIterator = MemoryMap.begin() ; MemoryMapIterator != MemoryMap.end() ; ++MemoryMapIterator)
	{
		PINFO pmem = (PINFO) MemoryMapIterator->second;

		if(NULL != pmem)
		{
			memoryLeakCounter++;
			cout << *pmem;

			if(traceleak.is_open())
			{
                if(pmem->Error.size() > 0)
                    traceleak << pmem->Error << endl;

                traceleak << *pmem;
            }
		}
	}

	if(traceleak.is_open())
	{
        traceleak.close();
	}

	if(0 == memoryLeakCounter)
	{
		printf("\n\n********** Well Done! NO memory link detected **********\n\n");
	}

	sleep(10000);
}

void * operator new(size_t size, const char* filename, int line, const char* function)
{
	return AddMemory(size, filename, function,line, false);
}
void * operator new[](size_t size, const char* filename, int line, const char* function)
{
	return AddMemory(size, filename, function,line, true);
}
void operator delete(void* mem)
{
	RemoveMemory(mem,false);
}
void operator delete[](void* mem)
{
    RemoveMemory(mem, true);
}

#endif // NEWDELETE

#define new new(__FILE__, __LINE__, __PRETTY_FUNCTION__)



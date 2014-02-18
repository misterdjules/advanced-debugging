/* 
* This project is a playground to experiment with calling conventions.
* 
*/
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstdarg>

int NextRandomValue()
{
	static bool generatorSeeded = false;
	if (!generatorSeeded)
	{
		srand(static_cast<int>(time(NULL)));
		generatorSeeded = true;
	}

	return rand();
}

int __cdecl CdeclSum(int fooParam1, int fooParam2)
{
	return fooParam1 + fooParam2;
}

int __stdcall StdcallSum(int fooParam1, int fooParam2)
{
	return fooParam1 + fooParam2;
}

int VarArgsSum(unsigned int nbParams, ...)
{
	int res = 0;

	va_list paramsList;
	va_start(paramsList, nbParams);
	for (unsigned int i = 0; i < nbParams; ++i)
	{
		res += va_arg(paramsList, int);
	}

	return res;
}

class MyClass
{
public:
	MyClass(int seed)
		: m_seed(seed)
	{}

	int Sum(int param1, int param2)
	{
		return m_seed + param1 + param2;
	}

private:
	int m_seed;	
};

int main()
{
	MyClass myInstance(42);
	
	printf("MyClass::Sum: %d\n", myInstance.Sum(NextRandomValue(), NextRandomValue()));
	printf("Cdecl Sum: %d\n", CdeclSum(NextRandomValue(), NextRandomValue()));
	printf("Stdcall Sum: %d\n", StdcallSum(NextRandomValue(), NextRandomValue()));
	printf("VarArgs Sum: %d\n", VarArgsSum(3, NextRandomValue(), NextRandomValue(), NextRandomValue()));

	return 0;
}
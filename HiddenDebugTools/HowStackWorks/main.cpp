/*
 * This project illustrates the basics of how the stack works. For more details
 * regarding calling conventions, open the project named "CallingConventions" in the same
 * solution.
 *
 * !!! WARNING !!! This project should be run by using the "Debug" configuration, otherwise
 * compiler's optimizations will generate assembly code that does not represent how the call stack
 * usually works.
*/

#include <cstdlib>
#include <ctime>
#include <cstdio>

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

int Foo(int fooParam1, int fooParam2)
{
	int fooLocal1 = NextRandomValue();
	int fooLocal2 = NextRandomValue();
	
	return fooLocal1 + fooLocal2 + fooParam1 + fooParam2;
}

int main(int argc, char* argv[])
{
	int mainLocal1 = NextRandomValue();
	int mainLocal2 = NextRandomValue();
	
	int res = Foo(mainLocal1, mainLocal2);
	printf("Result: %d\n", res);

	return 0;
}

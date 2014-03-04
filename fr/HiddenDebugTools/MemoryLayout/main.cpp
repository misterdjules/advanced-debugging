#include <cstdlib>
#include <ctime>
#include <cstdio>

void Sum(int x, int y, int* res)
{
	if (res)
		*res = x + y;
}

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

int main()
{
	int* heapInteger = new int(42);
	int randomValueOnStack = NextRandomValue();
	int otherRandomValueOnStack = NextRandomValue();
	
	Sum(randomValueOnStack, otherRandomValueOnStack, heapInteger);
	
	printf("Result: %d\n", *heapInteger);

	return *heapInteger;
}


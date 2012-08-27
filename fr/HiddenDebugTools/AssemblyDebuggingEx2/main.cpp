#include <cstdlib>
#include <ctime>
#include <cstdio>

void Sum(int x, int y, int& res)
{
	res = x + y;
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
	int value = 0;
	
	Sum(NextRandomValue(), 42, value);
	printf("Result: %d\n", value);

	return value;
}


#include <cstdlib>
#include <ctime>

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
	int x = NextRandomValue();
	int y = NextRandomValue();
	int value = 0;

	Sum(x, y, value);
	
	return value;
}


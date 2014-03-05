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

typedef void (*fun_ptr)(int a, int b, char c, char d, int& out);

void foo(int a, int b, char c, char d, int& out)
{
	out = a + b + c + d;
}

void bar(int a, int b, char c, char d, int& out)
{
	out = a - b - c - d;
}

int main()
{
	int x = NextRandomValue();
	int y = NextRandomValue();
	int value = 0;

	Sum(x, y, value);
	
	fun_ptr funToCall = NULL;
	if (value < 50)
	{
		funToCall = foo;
	}
	else
	{
		funToCall = bar;
	}

	int a = NextRandomValue();
	int b = NextRandomValue();
	char c = NextRandomValue();
	char d = NextRandomValue();
	funToCall(a, b, c, d, value);

	return value;
}


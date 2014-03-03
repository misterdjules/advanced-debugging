/*
 * This project shows how Edit & Continue can be used, and also how it works.
 *
 * !!! WARNING !!! Note that Edit & Continue is *not* compatible with NatVis.
 *
 *
*/
#include "Dummy.h"

void ChangeData(int& data)
{
	data = 42;
}

int main()
{
	int foo = 42;

	for (int loopFoo = 0; loopFoo < 10; ++loopFoo)
	{
		foo = foo * loopFoo;
	}

	ChangeData(foo);

	char bar = 'A';
	float baz = 0.f;

	foo = 0;

	ChangeData(foo);

	return 0;
}


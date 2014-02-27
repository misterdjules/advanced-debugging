/*
* This project illustrates the difference between the two main Intel desktop platforms:
* x86 and x64.
*
*/

void ChangeData(int& data)
{
	data = 42;
}

int main()
{
	int foo = 0;

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


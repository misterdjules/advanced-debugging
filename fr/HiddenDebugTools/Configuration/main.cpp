/*
 * This project illustrates the importance of some project configuration options regarding
 * debugging.
 *
 * - Symbols generation:
 *
 * - Edit & Continue:
 *
 * - Run time checks:
 *
 * - Platforms:
*/

void ChangeData(int& data)
{
	data = 42;
}

int main()
{
	// Even when no symbol is generated, it is still possible to break into
	// the debugger either programmatically using __debugbreak, DebugBreak or __asm int 3
	// __debugbreak();

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

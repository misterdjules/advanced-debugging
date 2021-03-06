/*
* This project is a playground to experiment with all kinds of breakpoints.
* 
* There's a loop that can be used to experiment with breakpoints conditions, traces and actions.
*
* There's also a function that changes some data. It is here to experiment with
* "data" breakpoints. 
*
* Finally, we show three examples of how to set breakpoints programmatically:
* - using inline assembly (note however that __asm int 3 does not work on the x64 platform).
* - using one of the compilers "intrinsincs": __debugbreak.
* - using the DebugBreak function.
*
* A note about ASLR:
* ------------------
* Remember that when using data breakpoints, deactivating ASLR 
* (Address Space Layout Randomization, see http://en.wikipedia.org/wiki/Address_space_layout_randomization)
* helps because then you don't have to reset your data breakpoints with each run of the program.
* To deactivate ASLR, simply go to Project -> Breakpoint Properties -> Configuration Properties -> Linker -> Advanced -> Randomized Base Address and
* set it to "NO". Of course, you will want to have this activated in the configuration used to ship your software.
*
* Last but not least, it is possible to use breakpoints in the "Release" configuration.
* However, you should run this program using the "Debug" configuration. It makes the experience a lot easier.
*/

#include <Windows.h>
#include <cassert>
#include <iostream>

class SomeClass
{
public:
	SomeClass() : m_someValue(0) { }
	void increaseSomeValue()
	{ 
		++m_someValue; 
	}

private:
	int m_someValue;
};

void ChangeData(int& data)
{
	data = 42;
}

int main()
{
	int foo = 0;
	char* strings[] = { "foo", "bar", "baz" };

	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	ChangeData(foo);

	// A loop to illustrate how conditional breakpoints
	// can be useful
	for (int loopFoo = 0; loopFoo < 10; ++loopFoo)
	{
		foo = foo * loopFoo;
		char* currentString = strings[loopFoo % 3];
		std::cout << "Current string: " << currentString << std::endl;
	}

	foo = 42;

	// Change data in "buffer" to test
	// conditional breakpoints using the "Has Changed" option
	for (unsigned int i = 0; i < 10; ++i)
	{
		buffer[512 + i] = '0' + i;
	}

	buffer[512] = 'o';
	
	SomeClass someInstance;
	someInstance.increaseSomeValue();
	someInstance.increaseSomeValue();

	if (IsDebuggerPresent())
	{
		// Inline assembly like __asm int 3 below does not work when compiling
		// for the x64 platform. However, you can use the compiler's intrinsics like __debugbreak:
		// http://msdn.microsoft.com/en-us/library/vstudio/26td21ds%28v=vs.100%29.aspx	
		// __asm int 3;

		__debugbreak();

		DebugBreak();
	}

	ChangeData(foo);

	char bar = 'A';
	float baz = 0.f;
	
	foo = 0;

	ChangeData(foo);
	
	free(strings[0]);
	free(strings[1]);
	free(strings[2]);

	return 0;	
}


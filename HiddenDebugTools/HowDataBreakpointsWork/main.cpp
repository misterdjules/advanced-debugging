/*
 * This project contains code that illustrates how data breakpoints are implemented.
 * It reads the four debug registers that hold data breakpoints' addresses
 * (DR0, DR1, DR2 and DR3) and writes the values that they contain on the standard output.
 *
 * Set a breakpoint on the "while (true)" loop statement so that it's trigger before
 * entering the loop every time.  When the breakpoint is triggered, add a data breakpoint
 * by clicking on DEBUG -> New Breakpoint -> New Data Breakpoint...
 *
 * Enter an address that is easy to remember for the newly created data breakpoint,
 * the size is not important (as long as it's less or equal than 4 bytes on x86,
 * and less or equal than 8 bytes on x64).
 *
 * Now press F5 to continue the debuggee's execution, you should hit the breakpoint
 * again. Press Alt + Tab to display the program's output window:
 * you should see the address of your data breakpoint in the output.
 *
 * Continue adding/disabling/removing data breakpoints and see how it affects the output.
*/

#include <iostream>
#include <Windows.h>

int main()
{
	CONTEXT threadCtx;
	threadCtx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	HANDLE currentThread = GetCurrentThread();
	std::cout << std::hex;
	while (true)
	{		
		GetThreadContext(currentThread, &threadCtx);
		std::cout	<< "1st Data breakpoint address: 0x" << threadCtx.Dr0 << std::endl
					<< "2nd Data breakpoint address: 0x" << threadCtx.Dr1 << std::endl
					<< "3rd Data breakpoint address: 0x" << threadCtx.Dr2 << std::endl
					<< "4th Data breakpoint address: 0x" << threadCtx.Dr3 << std::endl
					<< std::endl;
	}

	return 0;
}
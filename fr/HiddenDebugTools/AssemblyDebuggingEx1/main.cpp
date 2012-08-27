/*
 * First, compile this program in debug. Set breakpoints on all lines of code, and run the program.
 * After the first breakpoint is hit, continue through program's execution by pressing F5, and do so 
 * each time you hit a breakpoint. Observe that all breakpoints are hit, and that all variables declared 
 * within the code can be watched in the watch window.
 *
 * Second and finally, compile this program using the Release build configuration. Keep the breakpoints 
 * you set before running the Debug version, and run the program by pressing F5. Observe what happens:
 * most of the breakpoints are gone! 
 *
 * In order to understand the differences between the Debug and the Release configuration, use 
 * the disassembly window (right click anywhere in the code editor while the program is running and left 
 * click on "go to disassembly".
 */

void ChangeData(int& data)
{
	data = 42;
}

int main()
{
	int foo = 0;
	
	ChangeData(foo);

	char bar = 'A';
	float baz = 0.f;
	
	foo = 0;

	ChangeData(foo);

	return 0;	
}


/*
* This project can be used as a playground to try manipulating the next instruction(s) to
* be executed by the CPU. The "Set Next Statement" tool available in the right-click menu during
* a debugging session is the primary tool to do this. Another way to alter next instruction(s) is to write
* into the process' memory where the code is loaded, and replace next instruction(s) with other instructions that do nothing (e.g the x86 NOP instruction).
*
* First, let's use the "Set Next Statement" tool to skip a program crash.
*
* Run the program in the Debug configuration. It will crash at line 56.
* Place the cursor at the next instruction to execute, in this case line 58 "return 0;". Right click in the editor 
* and click on "Set Next Statement". The yellow arrow showing the next statement to execute will move to highlight this line of code.
* Continue the program execution by pressing "F5", and see how the program exits with no error. We just skipped a crash!
* This can be very handy in development when you're investigating a hard to reproduce bug and you wan to continue the 
* program execution to gather more information about what's going on.
*
* But more than skipping a statement, the "Set Next Statement" tool can also be use to travel backward in time.
* You can set the next statement to be any statement, including statement that have already been executed.
* Let's use this feature to "fix" our crash. First, let's run this program again. It will crash in the same way.
* Set the next statement to line 55: "int* fooAddress = reinterpret_cast<int*>(42);".
* Step over this statement (F10), The program is now about to dereference address 42, which will obviously crash.
* Display the watch window (Debug -> Windows -> Watch -> Watch 1) and add "fooAddress" to the values watched.
* Its value should display "42", change it to "&foo" and type enter. Now step over (F10), the program didn't crash and
* the variable "bar" now holds the same value as the variable foo.
* Of course, you can not always disrupt the execution flow as you wish, because previous statements usually rely on a given state that
* that has probably changed since then. But if you're careful not to travel too far from the current execution context, 
* you can often use Set Next Statement to your advantage without breaking your program.

* Now, let's use a more advanced feature of Visual C++'s debugger: modifiying code at runtime!
*
* Set a breakpoint at line 56 and restart the program. When the breakpoint is hit, go into the disassembly view (Alt + 8)
* and locate the instruction that makes the program crash.
* In this example, if you're running the "Debug" configuration, the assembly window will display:
*
* 012C13C7 8B 45 E0             mov         eax,dword ptr [fooAddress]  // load the value of fooAdress into register eax
* 012C13CA 8B 08                mov         ecx,dword ptr [eax] // load the value at address contained in eax (the address contained in fooAddress) into register ecx
* 012C13CC 89 4D EC             mov         dword ptr [bar],ecx  // write the value in ecx into "bar"
*
* In the disassembly window, if you "Step Over" (using F10), you'll see that it's the second instruction that makes the program crash.
* Indeed, it tries to load a value at address 42, which obviously can't work. We're going to replace this instruction with an instruction
* that does nothing in order to avoid the crash.
*
* First, display the "Memory" debug window (Debug -> Windows -> Memory -> Memory 1 or Alt + 6), and set the address to
* "012C13CA", the address of the instruction that crashes. Locate the two bytes that represent the MOV instruction that crashes (8B 08) and replace
* them with two NOP instruction by writing 90 90. Step Over (F10) and you'll see that this instruction has been replaced by two NOPs, and the program
* doesn't crash anymore!
*/
int main()
{
	int foo = 0;
	int bar = 0;

	foo = foo + 42;
	bar = bar + ((1 << 31) - 1);
	
	int* fooAddress = reinterpret_cast<int*>(42);
	bar = *fooAddress;
	
	return 0;
}
/* 
 * This project can be used to demonstrate the use of the /d2Zi+ compiler option.
 * /d2Zi+ is an undocumented feature that has been present in Visual C++ since 
 * Visual Studio 2012. It provides a better debugging experience when debugging optimized builds.
 * For instance, it shows the "Variable is optimized away and not available" message in the Watch window
 * if a given variable has been optimized at compilation time. Sometimes, it even manages to show 
 * reliable information about data that is otherwise not available when running an optimized build *without* 
 * /d2Zi+ enabled.
 *
 * However, it can not (yet) replace assembly debugging in general, since often the debugger won't be
 * able to track where your data is located.
 *
 * To see what it's capable of, first build the "Debug" configuration. Set a breakpoint on line 39:
 * "AbstractLogger* consoleLogger = new ConsoleLogger();". When the breakpoint is hit, add the "consoleLogger"
 * variable to the watch window. You'll see that the variable has been initialized with the value 0xcccccccc,
 * as usual for debug builds. When we step over the initialization of consoleLogger, the watch window is able
 * to display the consoleLogger instance properly.
 *
 * Now stop the program, select the "Release" build configuration and restart the program. When the breakpoint is hit,
 * this time the watch window displays "identifier consoleLogger is undefined". If you step over the initialization of 
 * consoleLogger, you will see the same message: the watch window cannot display information about consoleLogger.
 *
 * Once again, stop the debugging session. Now, we'll add the "/d2Zi+" compilation flag to the Release
 * configuration's compiler's command line. Go to "Project", "d2Zi+ properties". Then select 
 * "Configuration Properties" -> "C/C++" -> "Command line". In the "Additional Options" text input,
 * add "/d2Zi+" (without the double quotes). Click "OK" and then restart the debugging session.
 * This time, when the breakpoint is hit, the watch window will display "Variable is optimized away and not available".
 * When you step over the initialization instruction, the watch window will even be able to display the 
 * consoleLogger instance information properly.
*/

#include <iostream>

class AbstractLogger
{
public:
	AbstractLogger() : m_activated(true) {}
	virtual void logSomething() = 0;

private:
	bool m_activated;
};

class ConsoleLogger : public AbstractLogger
{
public:
	ConsoleLogger() : AbstractLogger() {}
	virtual void logSomething()
	{
		std::cout << "Something!" << std::endl;
	}
};

int main()
{
	AbstractLogger* consoleLogger = new ConsoleLogger();
	consoleLogger->logSomething();

	return 0;
}
/*
 * This program allows to see how adding breakpoints when debugging a program changes 
 * the code in memory.
 *
 * Simply set a breakpoint anywhere before the while (true) {...} loop, and start debugging the program.
 * When the breakpoint is hit, go to the disassembly window by pressing alt + F8 or by clicking on 
 * DEBUG -> Windows -> Disassembly.
 *
 * Copy and paste the address of the instruction that triggered the breakpoint and paste it in a notepad
 * somewhere (you'll need it soon after). To get the address, make sure that you show instructions' addresses in
 * the disassembly window by right clicking in the disassembly window and then click on "Show address".
 *
 * Hit F5 to continue the debuggee's execution, a console window should show up. Enter the address you 
 * just pasted and prefix it with "0x" (without the quotes).
 * Press enter: the program will show you the byte at the address you just pasted. It should display "0xcc", 
 * which is the opcode for the INT 3 instruction. INT 3 instruction trigger
 * exceptions that the debugger can catch to pause the debuggee. In other words, we just showed
 * that the debugger "patches" the debuggee's executable code to set breakpoints.
 * 
 * Now disable the breakpoint you previously set and enter the same address as you entered the first time.
 * The output of the program should be different. Instead of showing "0xcc", the opcode for the INT 3 instruction,
 * it should show the actual opcode of the original instruction that you had set the breakpoint on.
 *
 * Something worth noting is that if you follow the same steps and you use the Visual C++'s debugger Memory window to
 * examine the content of the memory where the code is loaded, you won't see that the code is patched by the debugger.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

const char* HEX_DIGITS				= "0123456789abcdefABCDEF";
const char* HEX_PREFIX				= "0x";
const unsigned char BRKPOINT_OPCODE = 0xCC;

int main()
{
	std::string			inputString;
	
	std::ostringstream hexConverterStream;
	hexConverterStream << std::hex;
	hexConverterStream << std::showbase;

	while (true)
	{
		std::cout	<< "Please enter the address of the byte you want to show." << std::endl
					<< "Use the hexadecimal format with a leading \"0x\" to do so," << std::endl
					<< "like following : 0xdeadbeef ." << std::endl;
		
		inputString.clear();
		std::cin >> inputString;
		if (inputString.substr(0, 2) != HEX_PREFIX &&
			inputString.find_first_not_of(HEX_DIGITS, 2) != std::string::npos)
		{
			std::cerr << "Input is not a valid hexadecimal address: " << inputString << std::endl;
			continue;
		}

		std::stringstream	hexReaderStream;
		hexReaderStream << std::hex;

		hexReaderStream << inputString;
		if (hexReaderStream.fail())
		{
			std::cerr << "Couldn't read hexadecimal number: " << inputString << std::endl;
			continue;
		}

		int address = -1;
		hexReaderStream >> address;
		
		if (address > 0)
		{
			unsigned char dataAtAddress = *(reinterpret_cast<unsigned char*>(address));
			
			hexConverterStream << static_cast<unsigned>(dataAtAddress);
			std::string byteAtAddress = hexConverterStream.str();

			hexConverterStream.str("");
			hexConverterStream.clear();
			hexConverterStream << address;
			std::string addressInHex = hexConverterStream.str();

			std::cout << "Data at address " << addressInHex
				<< ": " << byteAtAddress << std::endl;

			std::cout << "Data at address " << addressInHex;
			if (dataAtAddress == BRKPOINT_OPCODE)
			{
				std::cout << " IS a breakpoint opcode!";
			}
			else
			{
				std::cout << " is NOT a breakpoint opcode!";
			}
			std::cout << std::endl;
		}
		else
		{
			std::cerr << "Address should be greater than 0: " << inputString << std::endl;
			continue;
		}		
	}

	return 0;
}
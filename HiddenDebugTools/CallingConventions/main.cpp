/* 
* This project is a playground to experiment with calling conventions. This
* project is meant to be built using the "Debug" configuration, as optimizations
* used for the Release build generate assembly code that does not follow the 
* conventions presented below.
*
* Calling conventions defines how parameters are passed to a function, and 
* how the stack is cleaned up after the function call completes.
*
* Understanding calling conventions is key to being able to decipher assembly
* code when debugging using the "Disassemly" window. It will allow you to 
* find the true value of parameters, local variables and - in case of a method - 
* the this pointer.
*
* More information about calling conventions is available here:
* http://msdn.microsoft.com/en-us/library/984x0h58.aspx .
*
* This project illustrates 4 different calling conventions:
*
* __cdecl:
* --------
* The caller removes arguments from the stack. Here is the generated assembly code for
* the call to CdeclSum in the current project:
* 
* 000911A2 E8 68 FE FF FF       call        NextRandomValue (9100Fh)  
* 000911A7 50                   push        eax  
* 000911A8 E8 62 FE FF FF       call        NextRandomValue (9100Fh)  
* 000911AD 50                   push        eax  
* 000911AE E8 52 FE FF FF       call        CdeclSum (91005h)  
* 000911B3 83 C4 08             add         esp,8  
*
* As we can see, the caller shrinks the stack by two bytes (add esp,8) when the call to CdeclSum is done.
* Here's what the CdeclSum's generated assembly code looks like:
*
* 000910D0 55                   push        ebp  
* 000910D1 8B EC                mov         ebp,esp  
*	return fooParam1 + fooParam2;
* 000910D3 8B 45 08             mov         eax,dword ptr [fooParam1]  
* 000910D6 03 45 0C             add         eax,dword ptr [fooParam2]  
* }
* 000910D9 5D                   pop         ebp  
* 000910DA C3                   ret  
*
* Nothing special here, except that no operand is passed to the "ret" instruction, which means that the caller
* doesn't shrink the stack.
*
* __stdcall:
* --------
* The __stdcall calling convention, on the other hand, put the stack cleanup responsability on the callee.
* Here's the generated assembly for the call to StdcallSum:
*
* 000911CE E8 3C FE FF FF       call        NextRandomValue (9100Fh)  
* 000911D3 50                   push        eax  
* 000911D4 E8 36 FE FF FF       call        NextRandomValue (9100Fh)  
* 000911D9 50                   push        eax  
* 000911DA E8 2B FE FF FF       call        StdcallSum (9100Ah)  
*
* Contrary to the CdeclSum call, there's no instruction that shrinks the stack.
* Indeed, we find this instruction in the assembly generated for the StdcallSum function:
*
* int __stdcall StdcallSum(int fooParam1, int fooParam2)
* {
* 000910E0 55                   push        ebp  
* 000910E1 8B EC                mov         ebp,esp  
*	return fooParam1 + fooParam2;
* 000910E3 8B 45 08             mov         eax,dword ptr [fooParam1]  
* 000910E6 03 45 0C             add         eax,dword ptr [fooParam2]  
* }
* 000910E9 5D                   pop         ebp  
* 000910EA C2 08 00             ret         8  
*
* The "ret 8" instruction cleans up the stack.
*
* __thiscall:
* -----------
* The __thiscall calling convention is used by default for all C++ methods.
* It is similar to the __stdcall convention, except that the "this" pointer is
* passed in the ECX register. Let's have a look at the assembly code generated
* for the method call:
*
* 00EC1176 E8 94 FE FF FF       call        NextRandomValue (0EC100Fh)  
* 00EC117B 50                   push        eax  
* 00EC117C E8 8E FE FF FF       call        NextRandomValue (0EC100Fh)  
* 00EC1181 50                   push        eax  
* 00EC1182 8D 4D F8             lea         ecx,[myInstance]  
* 00EC1185 E8 8F FE FF FF       call        MyClass::Sum (0EC1019h)  
* 
* We see that the pointer to this is indeed loaded in the ECX register:
* lea         ecx,[myInstance]
* 
* If we look at the callee's assembly code, we also confirm that it's the callee's
* responsability to cleanup the stack ("ret 8" at the end of the generated assembly):
*
* 00A61330 55                   push        ebp  
* 00A61331 8B EC                mov         ebp,esp  
* 00A61333 51                   push        ecx  
* 00A61334 C7 45 FC CC CC CC CC mov         dword ptr [ebp-4],0CCCCCCCCh  
* 00A6133B 89 4D FC             mov         dword ptr [ebp-4],ecx  
* 		: m_seed(seed)
* 00A6133E 8B 45 FC             mov         eax,dword ptr [this]  
* 00A61341 8B 00                mov         eax,dword ptr [eax]  
* 00A61343 03 45 08             add         eax,dword ptr [param1]  
* 00A61346 03 45 0C             add         eax,dword ptr [param2]  
* 	{}
* 00A61349 8B E5                mov         esp,ebp  
* 00A6134B 5D                   pop         ebp  
* 00A6134C C2 08 00             ret         8  
* 
* Note also how in the code above, the compiler generates assembly code to allow the debug to
* retrieve the this pointer on the stack, regardless of how the ECX register is used later on
* in the method implementation:
* 
* 00A61333 51                   push        ecx  
* 00A61334 C7 45 FC CC CC CC CC mov         dword ptr [ebp-4],0CCCCCCCCh  
* 00A6133B 89 4D FC             mov         dword ptr [ebp-4],ecx
*
* This is what allows you to type "this" in the watch window and always get the right
* pointer to the current instance. In optimized builds, this code wouldn't be generated, and
* typing "this" in the watch window would most likely not give you the right pointer to the
* current instance. Instead, you would need to retrieve it yourself by investigating where 
* the compiler saved it.
* 
* Virtual methods:
* ----------------
*
* Virtual methods use the same __thiscall calling conventions, and do not represent a different
* calling conventions by themselves. However, they show another technical way to call methods which
* has a direct impact on performance and debugging.
*/

#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstdarg>

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

int __cdecl CdeclSum(int fooParam1, int fooParam2)
{
	int local1 = NextRandomValue();

	return local1 + fooParam1 + fooParam2;
}

int __stdcall StdcallSum(int fooParam1, int fooParam2)
{
	int local1 = NextRandomValue();
	
	return local1 + fooParam1 + fooParam2;
}

int VarArgsSum(unsigned int nbParams, ...)
{
	int res = 0;

	va_list paramsList;
	va_start(paramsList, nbParams);
	for (unsigned int i = 0; i < nbParams; ++i)
	{
		res += va_arg(paramsList, int);
	}

	return res;
}

class MyClass
{
public:
	MyClass(int seed)
		: m_seed(seed)
	{}

	int Sum(int param1, int param2)
	{
		int local1 = NextRandomValue();

		return local1 + m_seed + param1 + param2;
	}

	virtual int VirtualSum(int param1, int param2)
	{
		int local1 = NextRandomValue();

		return local1 + m_seed + param1 + param2;
	}

	virtual int AnotherVirtualSum(int param1, int param2)
	{
		int local1 = NextRandomValue();

		return local1 + m_seed + param1 + param2;
	}

private:
	int m_seed;	
};

int main()
{
	MyClass* myInstance = new MyClass(42);
	
	printf("MyClass::Sum: %d\n", myInstance->Sum(NextRandomValue(), NextRandomValue()));
	printf("MyClass::VirtualSum: %d\n", myInstance->VirtualSum(NextRandomValue(), NextRandomValue()));
	printf("MyClass::VirtualSum: %d\n", myInstance->AnotherVirtualSum(NextRandomValue(), NextRandomValue()));
	printf("Cdecl Sum: %d\n", CdeclSum(NextRandomValue(), NextRandomValue()));
	printf("Stdcall Sum: %d\n", StdcallSum(NextRandomValue(), NextRandomValue()));
	printf("VarArgs Sum: %d\n", VarArgsSum(3, NextRandomValue(), NextRandomValue(), NextRandomValue()));

	return 0;
}
/*
* This project is a playground to illustrate how virtual methods work.
*
* Virtual methods use the same __thiscall calling conventions (see the project called 
* "CallingConventions" in this solution for more info), and do not represent a different 
* calling convention by themselves.
* However, they show another technical way to call methods which has a direct impact on 
* performance and debugging.
*
* The difference between non-virtual methods and virtual methods lies in the assembly code
* generated at the caller site. Where a compiler would generate the following code for 
* a non-virtual method call:
*
* printf("MyClass::Sum: %d\n", myInstance->Sum(NextRandomValue(), NextRandomValue()));
* 002A14B1 E8 C2 FB FF FF       call        002A1078  
* 002A14B6 50                   push        eax  
* 002A14B7 E8 BC FB FF FF       call        002A1078  
* 002A14BC 50                   push        eax  
* 002A14BD 8B 4D F0             mov         ecx,dword ptr [ebp-10h]  
* 002A14C0 E8 03 FC FF FF       call        002A10C8  
*
* Note how the "call" instruction's argument is the absolute address of the method 
* implementation.
*
* Now, let's have a look at the code generated by the compiler for the call to the virtual method named
* "VirtualSum":
* 
* 002A14D4 E8 9F FB FF FF       call        002A1078  
* 002A14D9 50                   push        eax  
* 002A14DA E8 99 FB FF FF       call        002A1078  
* 002A14DF 50                   push        eax  
* 002A14E0 8B 45 F0             mov         eax,dword ptr [ebp-10h]  
* 002A14E3 8B 10                mov         edx,dword ptr [eax]  
* 002A14E5 8B 4D F0             mov         ecx,dword ptr [ebp-10h]  
* 002A14E8 8B 02                mov         eax,dword ptr [edx]  
* 002A14EA FF D0                call        eax  
*
* Here, the call to the method changed from a single call instruction to several instructions:
* 
* 002A14E0 8B 45 F0             mov         eax,dword ptr [ebp-10h]  
* 002A14E3 8B 10                mov         edx,dword ptr [eax]  
* 002A14E5 8B 4D F0             mov         ecx,dword ptr [ebp-10h]  
* 002A14E8 8B 02                mov         eax,dword ptr [edx]  
* 002A14EA FF D0                call        eax
* 
* This is how the cpu retrieves the virtual method's address from the virtual functions pointers table,
* index it to find the address of the method to call, store this address in a register and pass this
* register as a parameter to the call instruction. As you can see, calling a virtual method call generates 
* three more memory accesses than calling a non-virtual method.
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
	
	return 0;
}
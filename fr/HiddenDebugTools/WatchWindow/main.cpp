/*
 * This project can be used as a playground to experiment with the Watch Window's useful, less known features:
 * - Writing data at runtime.
 * - "Address of operator" &, Type casting and pointer arithmetic.
 * - Displaying lists of objects.
 * - Data format conversions.
 * - Custom visualization of complex types.
 * - Pseudo registers like the CPU clock or environment variables.
 * - Using it as a calculator.
 * - Calling functions.
 * 
 * Writing data at runtime:
 * ------------------------
 *
 * Pointers manipulation:
 * ----------------------
 *
 * The Watch Window can be used to manipulate pointers in a similar way than what can be done in C or C++.
 * First, you can use the "&" operator to get the address of any data. Then, pointer arithmetic works as you would
 * expected with C and C++. Finally, you can cast any data to any type.
 *
 * Displaying lists of objects:
 * ----------------------------
 * 
 * Data format conversions:
 * ------------------------
 *
 * Useful format conversions:
 * - d, i: numbers conversions. 
 * - s: string.
 * - wm: Windows Message numbers.
 * 
 * Custom visualization of complex types:
 * --------------------------------------
 *
 * Natvis is the new tool for custom data visualizations in the debugger. It's a replacement for
 * the old autoexp.dat solution that has been present for many releases.
 *
 * !!! WARNING !!!: NatVis doesn't work with Edit & Continue enabled! In my Visual C++ 2013 Express installation, 
 * I had to create a DWORD entry named "ENCEnable" in the registry in the 
 * "HKEY_CURRENT_USER\Software\Microsoft\WDExpress\12.0_Config\Debugger" key and set it to "0" to be able to disable
 * Edit & Continue in the "Debug" -> "Options And Settings" -> "Edit and Continue" dialog.
 *
 * More info about NatVis can be found at http://msdn.microsoft.com/en-us/library/jj620914.aspx .
 * 
 * Calling functions:
 * ------------------
 *
 * It is possible to call functions and methods from the Watch Window. Start a debugging session and continue execution until
 * after the LifeComponent is added:
 *
 * myGameEntity.AddComponent(new LifeComponent());
 *
 * Then add the "myGameEntity" variable to the Watch Window and expand its "m_components" attribute to show
 * the first and only "LifeComponent" instance and its properties.
 * Then add the following expression in the Watch Window:
 *
 * ((LifeComponent*)(((myGameEntity).m_components)._Myfirst)[0])->Die()
 *
 * Note how a message was outputed in the "Output" Window and how myGameEntity's LifeComponent instance
 * was changed (m_file dropped from 100 to 0).
 */

#include <Windows.h>

#include <vector>
#include <sstream>
#include <iostream>

class Component
{
public:
	virtual void Update() = 0;
};

class LifeComponent : public Component
{
public:
	LifeComponent()
		: m_life(100)
	{}

	virtual void Update()
	{
		if (m_life <= 0)
			Die();
	}

private:
	void Die()
	{
		OutputDebugString(L"Arrrrrrrgh!\n");
		m_life = 0;
	}

	unsigned int m_life;
};

struct Vec3f
{
	Vec3f() 
		: x(0.f)
		, y(0.f)
		, z(0.f)
	{}

	float x, y, z;
};

class GameEntity
{
public:
	GameEntity(const char* name) 
		: m_name(name)
		, m_isActive(false)
	{}

	void SetPos(const Vec3f& pos) { m_pos = pos; }

	void AddComponent(Component* component) { m_components.push_back(component); }

private:
	std::vector<Component*> m_components;
	bool					m_isActive;
	const char*				m_name;
	Vec3f					m_pos;
};

int main()
{
	int changeMe = 42;	
	std::wostringstream logMessage;
	logMessage << "ChangeMe: " << changeMe << std::endl;
	OutputDebugString(logMessage.str().c_str());
	
	static const int numbers[] = { 1, 2, 3, 4, 5 };
	size_t numbersSize = sizeof(numbers) / sizeof(int);
	static int* numbersList = new int[numbersSize];	
	memcpy(numbersList, numbers, sizeof(numbers));

	static const char letters[] = { 'A', 'B', 'C', 'D', 'E' };
	
	GameEntity myGameEntity("Foo");
	myGameEntity.AddComponent(new LifeComponent());

	return 0;
}
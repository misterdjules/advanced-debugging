#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <cstdlib>
#include <cstdio>
#include <cstring>

class Component
{
public:
	virtual void Update() = 0;
	virtual void BeforeDestruction() = 0;
};

class PhysicComponent : public Component
{
public:
	virtual void Update()
	{
		printf("Performing physic component update...\n");
	}

	virtual void BeforeDestruction()
	{		
	}
};


class GameObject
{
public:
	GameObject() {}

	virtual void Destroy()
	{
		for (unsigned int componentIndex = 0; componentIndex < m_lastComponentIndex; ++componentIndex)
		{			
			m_components[componentIndex]->BeforeDestruction();
		}
	}

	GameObject(const char* name)
		: m_name(name)
		, m_lastComponentIndex(0)
	{
		memset(m_components, 0, 10 * sizeof(Component*));
	}

	void AddComponent(Component* componentToAdd)
	{
		m_components[m_lastComponentIndex] = componentToAdd;
		++m_lastComponentIndex;
	}

	virtual ~GameObject()
	{
		for (unsigned int componentIndex = 0; componentIndex < m_lastComponentIndex; ++componentIndex)
		{
			m_components[componentIndex]->BeforeDestruction();

			delete m_components[componentIndex];
			m_components[componentIndex] = 0;			
		}		
	}

	GameObject(const GameObject& rhs)
		: m_name(rhs.m_name)
		, m_lastComponentIndex(rhs.m_lastComponentIndex)
	{
		for (unsigned int componentIndex = 0; componentIndex < 10; ++componentIndex)
		{
			m_components[componentIndex] = rhs.m_components[componentIndex];
		}
	}	
	
	void Update()
	{
		for (unsigned int componentIndex = 0; componentIndex < m_lastComponentIndex; ++componentIndex)
		{			
			m_components[componentIndex]->Update();
		}
	}

private:
	const char*		m_name;
	Component*		m_components[10];
	unsigned int	m_lastComponentIndex;
};

#endif // GAME_OBJECT_H_
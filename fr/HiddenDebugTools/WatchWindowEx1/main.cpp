#include <Windows.h>

#include <vector>

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
		OutputDebugString(L"Arrrrrrrgh!");
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
	GameEntity myGameEntity("Foo");
	myGameEntity.AddComponent(new LifeComponent());

	return 0;
}
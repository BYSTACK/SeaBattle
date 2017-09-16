#pragma once
#include <SFML\Graphics.hpp>

class Cell
{
public:
	Cell();

	bool isShip;
	bool isDead;
	bool isDone;

	void Shoot();
	void MakeShip(bool);

	bool GetShip();
	bool GetDead();
	~Cell();
};


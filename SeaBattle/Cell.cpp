#include "Cell.h"



Cell::Cell()
{
	isShip = false;
	isDead = false;
	isDone = false;
}

void Cell::Shoot()
{
	isDead = true;
}

bool Cell::GetShip()
{
	return isShip;
}


bool Cell::GetDead()
{
	return isDead;
}


void Cell::MakeShip(bool ship)
{
	isShip = ship;
}

Cell::~Cell()
{
}

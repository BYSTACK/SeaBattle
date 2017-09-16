#include "Field.h"
#include <cstdlib>
#include <iostream>

Field::Field(sf::Vector2f pos)
{
	this->pos = pos;
	enemyTransp = 0;

	lose = false;

	hit.setRadius(3.f);
	hit.setFillColor(sf::Color::Black);
	hit.setOrigin(-12.f, -12.f);

	shipHit.setRadius(6.f);
	shipHit.setFillColor(sf::Color::Red);
	shipHit.setOrigin(-9.f, -9.f);

	ship.setFillColor(sf::Color(128, 128, 128));
	ship.setSize(sf::Vector2f(30.f, 30.f));

	grid.setOutlineColor(sf::Color::Black);
	grid.setOutlineThickness(1.f);
	grid.setSize(sf::Vector2f(30.f, 30.f));
}

sf::Vector2i Field::KillShipUnsure(sf::Vector2i& pos)
{
	int rnd = rand() % 4;
	int strnd = rnd;

	while (true)
	{
		switch (rnd)
		{
		case 0:
			if (pos.x + 1 < 10)
			{
				if (!cells[pos.x + 1][pos.y].GetDead())
				{
					return  sf::Vector2i(pos.x + 1, pos.y);
				}
			}
			break;
		case 1:
			if (pos.y - 1 >= 0)
			{
				if (!cells[pos.x][pos.y - 1].GetDead())
				{
					return  sf::Vector2i(pos.x, pos.y - 1);
				}
			}
			break;
		case 2:
			if (pos.x - 1 >= 0)
			{
				if (!cells[pos.x - 1][pos.y].GetDead())
				{
					return  sf::Vector2i(pos.x - 1, pos.y);
				}
			}
			break;
		case 3:
			if (pos.y + 1 < 10)
			{
				if (!cells[pos.x][pos.y + 1].GetDead())
				{
					return  sf::Vector2i(pos.x, pos.y + 1);
				}
			}
			break;
		}
		rnd++;
		if (rnd >= 4)
		{
			rnd = 0;
		}
		if (rnd == strnd)
		{
			break;
		}
	}


	return sf::Vector2i(-1, -1);



}

sf::Vector2i Field::KillShip(sf::Vector2i& pos)
{
	for (int i = -1; i <= 1; i += 2)
	{
		if (pos.y + i < 10 && pos.y + i >= 0)
		{
			if (cells[pos.x][pos.y + i].GetDead() && cells[pos.x][pos.y + i].GetShip())
			{
				if (pos.y - i >= 0 && pos.y - i <10)
				{
					if (!cells[pos.x][pos.y - i].GetDead())
					{
						return sf::Vector2i(pos.x, pos.y - i);
					}
				}
			}
		}
		if (pos.x + i < 10 && pos.x + i >= 0)
		{
			if (cells[pos.x + i][pos.y].GetDead() && cells[pos.x + i][pos.y].GetShip())
			{
				if (pos.x - i >= 0 && pos.x - i <10)
				{
					if (!cells[pos.x - i][pos.y].GetDead())
					{
						return sf::Vector2i(pos.x - i, pos.y);
					}
				}
			}
		}
	}

	return sf::Vector2i(-1, -1);
}

bool Field::MakeAITurn()
{

	std::vector<sf::Vector2i> possibilities;

	possibilities.clear();

	bool foundShip = false;
	bool foundUnsureShip = false;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (cells[i][j].GetDead() && cells[i][j].GetShip() && !cells[i][j].isDone)
			{
				
				sf::Vector2i pz = KillShip(sf::Vector2i(i, j));
				if (pz.x != -1)
				{
					possibilities.clear();
					possibilities.push_back(pz);
					foundShip = true;
				}
				else if (!foundShip)
				{
					pz = KillShipUnsure(sf::Vector2i(i, j));

					if (pz.x != -1)
					{
						possibilities.clear();
						possibilities.push_back(pz);
						foundUnsureShip = true;
					}
				}
			}

			if (!cells[i][j].GetDead() && !foundShip && !foundUnsureShip)
			{
				possibilities.push_back(sf::Vector2i(i, j));
			}
		}
	}

	if (possibilities.size() != 0)
	{
		int rnd = rand() % possibilities.size();

		int x = possibilities[rnd].x;
		int y = possibilities[rnd].y;

		cells[x][y].Shoot();

		if (cells[x][y].GetShip())
		{
			if (isEntireShipDead(sf::Vector2i(x, y)))
			{
				RevealShip(sf::Vector2i(x, y));
			}
			CheckWin();
			return false;
		}
		CheckWin();
		return true;
	}
	CheckWin();
	return true;
}

bool Field::Click(sf::Vector2i clickpos)
{
	sf::Vector2i rClickPos = clickpos - sf::Vector2i((int)pos.x,(int)pos.y);
	rClickPos.x = (int)(floor((float)rClickPos.x / 30.f));
	rClickPos.y = (int)(floor((float)rClickPos.y / 30.f));
	
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (i == (int)rClickPos.x && j == (int)rClickPos.y)
			{
				if (!cells[i][j].GetDead())
				{
					cells[i][j].Shoot();

					if (cells[i][j].GetShip())
					{
						if (isEntireShipDead(sf::Vector2i(i, j)))
						{
							RevealShip(sf::Vector2i(i, j));
						}
						CheckWin();
						return false;
					}
					CheckWin();
					return true;
				}
				else
				{
					CheckWin();
					return false;
				}
			}
		}
	}
	CheckWin();
	return false;
}

void Field::RevealShip(sf::Vector2i& pos)
{
	cells[pos.x][pos.y].isDone = true;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (pos.x + i < 10 && pos.x + i >= 0 && pos.y + j < 10 && pos.y + j >= 0)
			{
				cells[pos.x + i][pos.y + j].Shoot();
				if (!cells[pos.x + i][pos.y + j].isDone && cells[pos.x + i][pos.y + j].GetShip())
				{
					RevealShip(sf::Vector2i(pos.x + i, pos.y + j));
				}
			}
		}
	}

}

bool Field::isEntireShipDead(sf::Vector2i& pos)
{
	checkedPositions.clear();
	return !doAnyCellHaveAliveNeighbors(pos);
}

bool Field::doAnyCellHaveAliveNeighbors(sf::Vector2i& pos)
{
	checkedPositions.push_back(pos);

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (pos.x + i < 10 && pos.x + i >= 0 && pos.y + j < 10 && pos.y + j >= 0)
			{
				if (!cells[pos.x + i][pos.y + j].GetDead() && cells[pos.x + i][pos.y + j].GetShip())
				{
					return true;
				}
			}
		}
	}

	bool done = false;
	bool result;

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (pos.x + i < 10 && pos.x + i >= 0 && pos.y + j < 10 && pos.y + j >= 0)
			{
				if (cells[pos.x + i][pos.y + j].GetDead() && cells[pos.x + i][pos.y + j].GetShip())
				{
					bool doCellChecked = false;
					for (int k = 0; k < checkedPositions.size(); k++)
					{
						if (pos.x + i == checkedPositions[k].x && pos.y + j == checkedPositions[k].y)
						{
							doCellChecked = true;
						}
					}
					if (!doCellChecked)
					{
						result = doAnyCellHaveAliveNeighbors(sf::Vector2i(pos.x + i, pos.y + j));
						done = true;
					}
				}
			}
		}
	}

	if (done)
	{
		return result;
	}

	return false;
}

void Field::Draw(sf::RenderWindow& window)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			grid.setPosition(sf::Vector2f((float)i*30.f, (float)j*30.f) + pos);
			window.draw(grid);

			if (cells[i][j].GetShip())
			{
				ship.setPosition(sf::Vector2f((float)i*30.f, (float)j*30.f)+pos);
				window.draw(ship);
				if (cells[i][j].GetDead())
				{
					shipHit.setPosition(sf::Vector2f((float)i*30.f, (float)j*30.f) + pos);
					window.draw(shipHit);
				}
			}
			else if (cells[i][j].GetDead())
			{
				hit.setPosition(sf::Vector2f((float)i*30.f, (float)j*30.f)+pos);
				window.draw(hit);
			}
		}
	}
}

void Field::EnemyDraw(sf::RenderWindow& window)
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			grid.setPosition(sf::Vector2f((float)i*30.f, (float)j*30.f) + pos);
			window.draw(grid);

			if (cells[i][j].GetShip())
			{
				//debug
				ship.setPosition(sf::Vector2f((float)i*30.f, (float)j*30.f) + pos);
				ship.setFillColor(sf::Color(128, 128, 128,enemyTransp));
				window.draw(ship);
				ship.setFillColor(sf::Color(128, 128, 128));
				//cloae debug
				if (cells[i][j].GetDead())
				{
					shipHit.setPosition(sf::Vector2f((float)i*30.f, (float)j*30.f) + pos);
					window.draw(shipHit);
				}
			}
			else if (cells[i][j].GetDead())
			{
				hit.setPosition(sf::Vector2f((float)i*30.f, (float)j*30.f) + pos);
				window.draw(hit);
			}
		}
	}
}

void Field::RandomizeShips()
{
	int size = 4;
	int sizestep = 1;

	for (int i = 0; i < 10; i++)
	{
		sizestep--;
		std::vector<Ship> possibilities;

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					if (k == 0)
					{
						if (CheckShip(sf::Vector2i(i, j), size, false))
						{
							possibilities.push_back(Ship(size, sf::Vector2i(i, j), false));
						}
					}
					else
					{
						if (CheckShip(sf::Vector2i(i, j), size, true))
						{
							possibilities.push_back(Ship(size, sf::Vector2i(i, j), true));
						}
					}

				}
			}
		}
		if (possibilities.size() != 0)
		{
			int ran = rand() % possibilities.size();
			AddShip(possibilities[ran]);
		}
		if (sizestep == 0)
		{
			size--;
			sizestep = 5 - size;
		}
	}
}

void Field::CheckWin()
{
	bool lose = true;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (cells[i][j].GetShip() && !cells[i][j].GetDead())
			{
				lose = false;
			}
		}
	}
	if (lose)
	{
		this->lose = true;
		grid.setFillColor(sf::Color::Red);
	}
}

void Field::AddShip(Ship& ship)
{
	for (int i = 0; i < ship.size; i++)
	{
		if (!ship.downFaced)
		{
			cells[ship.pos.x + i][ship.pos.y].MakeShip(true);
		}
		else
		{
			cells[ship.pos.x][ship.pos.y + i].MakeShip(true);
		}
	}
}

bool Field::CheckShip(sf::Vector2i& pos, int size, bool isDown)
{
	for (int i = 0; i < size; i++)
	{
		if (!isDown)
		{
			if (i + pos.x >= 10)
			{
				return false;
			}
			else
			{
				for (int j = -1; j <= 1; j++)
				{
					for (int k = -1; k <= 1; k++)
					{
						if (i + pos.x + j < 10 && pos.y + k < 10 && i + pos.x + j >=0 && pos.y + k >=0)
						{
							if (cells[i + pos.x + j][pos.y + k].GetShip())
							{
								return false;
							}
						}
					}
				}
			}
		}
		else
		{
			if (i + pos.y >= 10)
			{
				return false;
			}
			else
			{
				for (int j = -1; j <= 1; j++)
				{
					for (int k = -1; k <= 1; k++)
					{
						if (pos.x + k < 10 && pos.y + i + j < 10 && pos.x + k >=0 && pos.y + i + j >=0)
						{
							if (cells[pos.x + k][pos.y + i + j].GetShip())
							{
								return false;
							}
						}
					}
				}
			}
		}
	}
	return true;
}

sf::Vector2f Field::GetPos()
{
	return pos;
}

void Field::SetEnemyTransp(int t)
{
	enemyTransp = t;
}

bool Field::CheckLose()
{
	return lose;
}

Field::~Field()
{
}

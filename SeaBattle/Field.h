#pragma once
#include "Cell.h"
#include <vector>
#include <SFML\Graphics.hpp>
#include "Ship.h"

class Field
{
private:
	Cell cells[10][10];

	sf::CircleShape hit;
	sf::CircleShape shipHit;
	sf::RectangleShape ship;
	sf::RectangleShape grid;

	sf::Vector2f pos;

	int enemyTransp;
	bool lose;

	std::vector<sf::Vector2i> checkedPositions;

public:
	Field(sf::Vector2f pos);

	void RandomizeShips();
	bool CheckShip(sf::Vector2i& pos, int size, bool isDown);
	void AddShip(Ship& ship);
	void Draw(sf::RenderWindow& window);
	void EnemyDraw(sf::RenderWindow& window);
	bool Click(sf::Vector2i clickpos);

	bool isEntireShipDead(sf::Vector2i& pos);
	bool doAnyCellHaveAliveNeighbors(sf::Vector2i& pos);
	void RevealShip(sf::Vector2i& pos);
	bool MakeAITurn();
	sf::Vector2i KillShip(sf::Vector2i& pos);
	sf::Vector2i KillShipUnsure(sf::Vector2i& pos);

	sf::Vector2f GetPos();
	void CheckWin();
	void SetEnemyTransp(int);
	bool CheckLose();

	~Field();
};


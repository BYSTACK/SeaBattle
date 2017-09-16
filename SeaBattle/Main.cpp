#include <SFML/Graphics.hpp>
#include "Field.h"
#include <cstdlib>
#include "Ship.h"

sf::RectangleShape shipPart;



void drawShip(int size,sf::Vector2f pos, bool downFaced, sf::RenderWindow& window)
{
	for (int i = 0; i < size; i++)
	{
		if (downFaced)
		{
			shipPart.setPosition(pos.x, pos.y + i * 30);
		}
		else 
		{
			shipPart.setPosition(pos.x + i * 30, pos.y);
		}
		window.draw(shipPart);
	}
}

bool placeShip(sf::Vector2i pos, int size, bool downFaced, Field& f)
{
	if (f.CheckShip(pos, size, downFaced))
	{
		f.AddShip(Ship(size, pos, downFaced));
		return true;
	}
	return false;
}

int main()
{
	bool shipsReady = false;


	sf::RenderWindow window(sf::VideoMode(690, 360), "Sea Battle", sf::Style::Titlebar | sf::Style::Close);
	srand(static_cast<unsigned int>(time(0)));

	shipPart.setFillColor(sf::Color(128, 128, 128, 128));
	shipPart.setSize(sf::Vector2f(30.f, 30.f));

	Field f(sf::Vector2f(30, 30));
	Field f1(sf::Vector2f(360, 30));

	//f.RandomizeShips();
	f1.RandomizeShips();


	int size = 4;
	int sizestep = 1;
	bool downFaced = false;
	


	while (window.isOpen())
	{
		int x = sf::Mouse::getPosition(window).x - (sf::Mouse::getPosition(window).x % 30);
		int y = sf::Mouse::getPosition(window).y - (sf::Mouse::getPosition(window).y % 30);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (shipsReady)
				{
					if (f1.Click(sf::Mouse::getPosition(window)))
					{
						while (true)
						{
							if (f.MakeAITurn())
							{
								break;
							}
						}
					}
				}
				else
				{
					
					if (placeShip(sf::Vector2i((x - f.GetPos().x)/30, (y - f.GetPos().y)/30), size, downFaced, f))
					{
						sizestep--;

						if (sizestep == 0)
						{
							size--;
							sizestep = 5 - size;
						}
						if (size == 0)
						{
							 shipsReady = true;
						}
					}
				}
			}
			if (event.type == sf::Event::MouseWheelMoved)
			{
				downFaced = !downFaced;
			}
		}

		if (f.CheckLose())
		{
			f1.SetEnemyTransp(255);
		}

		window.clear(sf::Color(200,200,200));
		
		f.Draw(window);
		f1.EnemyDraw(window);

		drawShip(size, sf::Vector2f(x,y), downFaced, window);

		window.display();
	}

	return 0;
}

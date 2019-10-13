#include <SFML/Graphics.hpp>
#include <iostream>

struct CirclePositions
{
	float x = 0, y = 0, Xvel = 0, Yvel = 0, radius = 0;
} cs[10];

int main()
{
	srand(time(0));
	for (int i = 1; i < 10; i++)
	{
	cs[i].radius = rand() % 25;
GoBack:
		int WCollide = 800 - cs[i].radius * 2, HCollide = 600 - cs[i].radius * 2;
		cs[i].x = rand() % WCollide; cs[i].y = rand() % HCollide;
		for (int j = 0; j < 10; j++)
			if (abs(cs[i].x - cs[j].x) < cs[i].radius + cs[j].radius &&
				abs(cs[i].y - cs[j].y) < cs[i].radius + cs[j].radius &&
				i != j)
				goto GoBack;
	}

	sf::RenderWindow window(sf::VideoMode(800, 600), "circle sandbox");
	window.setKeyRepeatEnabled(false);

	sf::CircleShape circle(12, 12);
	cs[0].x = 387.5f;
	cs[0].y = 287.5f;
	cs[0].radius = 12.5;

	sf::Clock clock;
	float Ttime = 0.0f;

	while (window.isOpen())
	{
		Ttime += clock.restart().asSeconds();

		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(cs[0].x + 12.5f, cs[0].y + 12.5f)),
			sf::Vertex(sf::Vector2f(float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y)))
		};

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == event.MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
				cs[0].Xvel += (line[1].position.x - cs[0].x) / 100, cs[0].Yvel += (line[1].position.y - cs[0].y) / 100;
		}

		window.clear();
		// Circle Maintenance
		for (int i = 0; i < 10; i++) {
			// Motion
			if (Ttime > 0.01f)
			{
				cs[i].x += cs[i].Xvel;
				cs[i].y += cs[i].Yvel;
				cs[i].Xvel -= cs[i].Xvel / 100;
				cs[i].Yvel -= cs[i].Yvel / 100;
				if (i == 9)
					Ttime = 0;
			}

			// Collision
				// Window Collision
			if (cs[i].x + cs[i].radius * 2 >= 800.0f ||
				cs[i].x <= 0.0f)
			{	cs[i].x -= cs[i].Xvel; cs[i].Xvel = -cs[i].Xvel / 2;	}
			if (cs[i].y + cs[i].radius * 2 >= 600.0f ||
				cs[i].y <= 0.0f)
			{	cs[i].y -= cs[i].Yvel; cs[i].Yvel = -cs[i].Yvel / 2;	}

				// Circle Collision
			for (int j = 0; j < 10; j++) {
				if (abs((cs[i].x + cs[i].radius) - (cs[j].x + cs[j].radius)) < cs[i].radius + cs[j].radius &&
					abs((cs[i].y + cs[i].radius) - (cs[j].y + cs[j].radius)) < cs[i].radius + cs[j].radius &&
					i != j)
				{
					// Bounce
					cs[j].Xvel = cs[i].Xvel; cs[j].Yvel = cs[i].Yvel;
					cs[i].Xvel = -cs[i].Xvel; cs[i].Yvel = -cs[i].Yvel;
				}
			}

			circle.setPosition(cs[i].x, cs[i].y);
			circle.setRadius(cs[i].radius);

			window.draw(circle);
		}
		window.draw(line, 2, sf::Lines);
		window.display();
	}
}
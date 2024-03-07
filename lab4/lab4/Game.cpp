/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>

using namespace std;
using namespace sf;

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800U, 600U, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	init(); // load texture
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::W == t_event.key.code)
	{
		if (gamemode == GameMode::LevelEditing)
		{
			winTilePlacement = !winTilePlacement;
		}
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	switch (gamemode)
	{
	case GameMode::LevelEditing:
		levelEditingUpdate();
		moveScreen();
		levelInit();
		break;
	case GameMode::Playing:
		gameUpdate();
		break;
	case GameMode::MainMenu:
		menuUpdate();
		break;
	default:
		break;
	}

}

void Game::gameUpdate()
{

	if (clock.getElapsedTime().asMilliseconds() <= 1000)
	{
		counter++;
		cout << counter << endl;
	}
	else
	{
		counter = 0;
		clock.restart();
	}
	
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				/*std::cout << level[0][0].getPosition().x << std::endl;*/
				

				level[col][row].move(-3.5, 0);
				//spritesheet.setPosition(playerShape.getPosition());
			}

		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && velocityY == 0)
		{
			velocityY = -11.8;
		}

		velocityY = velocityY + gravity;
		playerShape.move(0, velocityY);


		gravity = 0.6;

		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				if (velocityY >= 0)
				{

					if (levelData[col][row] == 1)
					{
						if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
						{
							if (playerShape.getPosition().y < level[col][row].getPosition().y)
							{
								gravity = 0;
								velocityY = 0;
								playerShape.setPosition(playerShape.getPosition().x, level[col][row].getPosition().y);
								playerShape.move(0, -playerShape.getGlobalBounds().height);
								break;
							}
							else {
								init();
							}
						}
					}

				}
				if (velocityY < 0)
				{

					if (levelData[col][row] == 1)
					{
						if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
						{
							init();
						}
					}
				}

				if (levelData[col][row] == 100)
				{
					if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
					{
						std::cout << "Win";			
					}
				}
				


			}

			if (playerShape.getPosition().y > 600)
			{
				init();
			}

		}
}

void Game::menuUpdate()
{
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			level[col][row].setOutlineThickness(0.0f);
		}
	}
}

void Game::moveScreen()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{

				level[col][row].move(6.5, 0);
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{

				level[col][row].move(-6.5, 0);
			}
		}
	}
}

void Game::levelEditingUpdate()
{
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{

			if (levelData[col][row] == 1)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(level[col][row].getPosition());
				level[col][row].setFillColor(sf::Color::Red);

			}
			if (levelData[col][row] == 0)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(level[col][row].getPosition());
				level[col][row].setFillColor(sf::Color::Transparent);
			}
			if (levelData[col][row] == 100)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(level[col][row].getPosition());
				level[col][row].setFillColor(sf::Color::Cyan);
			}
		}
	}


	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				if (levelData[col][row] == 0)
				{
					if (level[col][row].getGlobalBounds().contains(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y))
					{
						if (!winTilePlacement)
						{
							levelData[col][row] = 1;
						}
						else
						{
							levelData[col][row] = 100;
						}

					}
				}
			}
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{
				if (levelData[col][row] == 1 || levelData[col][row] == 100)
				{
					if (level[col][row].getGlobalBounds().contains(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y))
					{
						levelData[col][row] = 0;
					}
				}
			}
		}
	}
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);
	switch (gamemode)
	{
	case GameMode::LevelEditing:
		brickDraw();
		break;
	case GameMode::Playing:
		brickDraw();
		m_window.draw(playerShape);
		break;
	case GameMode::MainMenu:
		break;
	default:
		break;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
	{
		gamemode = GameMode::Playing;
		init();
	}
	m_window.display();
}

void Game::brickDraw()
{
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			m_window.draw(level[col][row]);

		}
	}
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::levelInit()
{
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			if (levelData[col][row] == 0)
			{
				level[col][row].setOutlineThickness(0.5f);
			}
		}
		std::cout << std::endl;
	}
}

void Game::init()
{
	playerShape.setFillColor(sf::Color::Blue);
	playerShape.setSize(sf::Vector2f(20, 20));
	playerShape.setPosition(160, 500);

	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{

			if (levelData[col][row] == 1)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setFillColor(sf::Color::Red);
				level[col][row].setOutlineColor(sf::Color::White);
				level[col][row].setOutlineThickness(0.0f);

			}
			if (levelData[col][row] == 0)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setFillColor(sf::Color::Transparent);
				level[col][row].setOutlineColor(sf::Color::White);
				level[col][row].setOutlineThickness(0.0f);
			}
			if (levelData[col][row] == 100)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setFillColor(sf::Color::Cyan);
				level[col][row].setOutlineColor(sf::Color::White);
				level[col][row].setOutlineThickness(0.0f);

			}
		}
	}
}
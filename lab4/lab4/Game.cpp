/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



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
	setupSprite(); // load texture
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
		break;
	case GameMode::Playing:
		break;
	case GameMode::MainMenu:
		break;
	default:
		break;
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

				level[col][row].move(3.5, 0);
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		for (int row = 0; row < numRows; row++)
		{
			for (int col = 0; col < numCols; col++)
			{

				level[col][row].move(-3.5, 0);
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
						levelData[col][row] = 1;
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
				if (levelData[col][row] == 1)
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
	
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{
			m_window.draw(level[col][row]);

		}
	}

	m_window.display();
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
void Game::setupSprite()
{
	for (int row = 0; row < numRows; row++)
	{
		for (int col = 0; col < numCols; col++)
		{

			if (levelData[col][row] == 1)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setFillColor(sf::Color::Red);

			}
			if (levelData[col][row] == 0)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setFillColor(sf::Color::Transparent);
				level[col][row].setOutlineThickness(0.5f);
				level[col][row].setOutlineColor(sf::Color::White);
			}
		}
		std::cout << std::endl;
	}
}
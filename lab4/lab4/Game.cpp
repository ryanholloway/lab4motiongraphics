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
	if (sf::Keyboard::W == t_event.key.code)
	{
		if (gamemode == GameMode::LevelEditing)
		{
			winTilePlacement = !winTilePlacement;
		}
	}
	if (sf::Keyboard::S == t_event.key.code)
	{
		if (gamemode == GameMode::LevelEditing)
		{
			selectingTile = !selectingTile;
		}
	}
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		gamemode = GameMode::MainMenu;
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
	case GameMode::WinScreen:
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

	frameTime += clock.restart().asSeconds();
	if (frameTime >= timePerFrame) {
		currentFrame++;
		if (currentFrame >= 8)
			currentFrame = 0;

		frameTime = 0.0f;
	}
	playerShape.setTextureRect(frames[currentFrame]);
	
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
				if (levelData[col][row] == 2|| levelData[col][row]==5)
				{
					if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
					{
						init();
					}
				}
				if (levelData[col][row] == 3)
				{
					if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
					{

						velocityY = -20;
					}
				}
				if (levelData[col][row] == 100)
				{
					if (playerShape.getGlobalBounds().intersects(level[col][row].getGlobalBounds()))
					{
						std::cout << "Win";
						gamemode = GameMode::WinScreen;
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

	for (int i = 0; i < 3; i++)
	{
		MenuText[0].setPosition(MenuButtons[0].getPosition().x + MenuText[0].getGlobalBounds().width * 2, MenuButtons[0].getPosition().y + 20);
		MenuText[1].setPosition(MenuButtons[1].getPosition().x + MenuText[1].getGlobalBounds().width / 2.0f + 10, MenuButtons[1].getPosition().y + 20);
		MenuText[2].setPosition(MenuButtons[2].getPosition().x + MenuText[2].getGlobalBounds().width * 2, MenuButtons[2].getPosition().y + 20);

		if (MenuButtons[i].getGlobalBounds().contains(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y))
		{
			MenuButtons[i].setFillColor(sf::Color::Cyan);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				switch (i)
				{
				case 0:
					if (levelsBeenEdited)
					{
						gamemode = GameMode::Playing;
						init();
					}
					break;
				case 1:
					gamemode = GameMode::LevelEditing;
					break;
				case 2:
					exit(1);
					break;
				default:
					break;
				}
			}
		}
		else
		{
			MenuButtons[i].setFillColor(sf::Color::Blue);
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
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setTextureRect(mapItemsI[3]);

			}
			if (levelData[col][row] == 0)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(level[col][row].getPosition());
				level[col][row].setFillColor(sf::Color::Transparent);
			}
			if (levelData[col][row] == 100)
			{
				levelsBeenEdited = true;
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(level[col][row].getPosition());
				level[col][row].setFillColor(sf::Color::Cyan);
				level[col][row].setTexture(NULL);

			}
			if (levelData[col][row] == 2)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(level[col][row].getPosition());
				level[col][row].setFillColor(sf::Color(220, 220, 220));
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setTextureRect(mapItemsI[2]);
			}
			if (levelData[col][row] == 3)
			{
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(level[col][row].getPosition());
				level[col][row].setFillColor(sf::Color::Cyan);
				level[col][row].setTextureRect(mapItemsI[0]);
			}
			if (levelData[col][row] == 5)
			{
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(level[col][row].getPosition());
				level[col][row].setFillColor(sf::Color(220, 220, 220));
				level[col][row].setTextureRect(mapItemsI[1]);
			}
		}
	}

	if (!selectingTile)
	{
		selecting.setString("Press S to change Tile");
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
							levelData[col][row] = currentTile;
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
					if (levelData[col][row]!=0)
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
	else
	{
		switch (currentTile)
		{
		case 0:
			break;
		case 1:
			selecting.setString("Selecting Brick Tile\nPress S to Edit Map");
			break;
		case 2:
			selecting.setString("Selecting Spike Tile\nPress S to Edit Map");
			break;
		case 3:
			selecting.setString("Selecting Bounce Tile\nPress S to Edit Map");
			break;
		case 4:
			break;
		case 5: 
			selecting.setString("Selecting MultiSpike Tile\nPress S to Edit Map");
			break;
		case 100:
			selecting.setString("Selecting Win Tile\nPress S to Edit Map");
			break;
		default:
			break;
		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			for (int i = 0; i < 5; i++)
			{
				if (selectorButton[i].getGlobalBounds().contains(sf::Mouse::getPosition(m_window).x, sf::Mouse::getPosition(m_window).y))
				{
					switch (i)
					{
					case 0:
						currentTile = 3;
						break;
					case 1:
						currentTile = 5;
						break;
					case 2:
						currentTile = 2;
						break;
					case 3:
						currentTile = 1;
						break;
					case 4:
						currentTile = 100;
						break;
					default:
						currentTile = 1;
						break;
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
		if (selectingTile)
		{
			for (auto& button : selectorButton)
			{
				m_window.draw(button);
			}
		}
		m_window.draw(selecting);
		
		break;
	case GameMode::Playing:
		brickDraw();
		m_window.draw(playerShape);
		break;
	case GameMode::MainMenu:
		for (auto& buttons : MenuButtons)
		{
			m_window.draw(buttons);
		}
		for (auto& buttons : MenuText)
		{
			m_window.draw(buttons);
		}
		break;
	case GameMode::WinScreen:
		m_window.draw(winText);
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
	if (!spritesheetTexture.loadFromFile("ASSETS\\IMAGES\\spritesheet.png"))
	{
		std::cout << "problem loading spritesheet" << std::endl;
	}
	if (!mapItemsTexture.loadFromFile("ASSETS\\IMAGES\\mapItems.png"))
	{
		std::cout << "problem loading mapItems" << std::endl;
	}
	selecting.setCharacterSize(25U);
	selecting.setFillColor(sf::Color::Cyan);
	selecting.setOutlineColor(sf::Color::Red);
	selecting.setOutlineThickness(2);
	selecting.setFont(m_ArialBlackfont);
	selecting.setPosition(275, 40);
	selecting.setString("Selecting Tile...");
	
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
	playerShape.setSize(sf::Vector2f(20, 20));
	playerShape.setPosition(160, 500);
	playerShape.setTexture(&spritesheetTexture);
	playerShape.setTextureRect(frames[0]);
	MenuText[0].setString("Play");
	MenuText[1].setString("Edit Level");
	MenuText[2].setString("Exit");

	winText.setFillColor(sf::Color::White);
	winText.setCharacterSize(50U);
	winText.setPosition(250, 250);
	winText.setString("You Win!");
	winText.setFont(m_ArialBlackfont);

	for (int i = 0; i < 3; i++)
	{
		MenuButtons[i].setFillColor(sf::Color::Blue);
		MenuButtons[i].setPosition(250, (i * 100) + 100);
		MenuButtons[i].setSize(sf::Vector2f(300, 75));
		MenuText[i].setPosition(MenuButtons[i].getPosition().x+MenuButtons[i].getGlobalBounds().width/4.0f-MenuText[i].getGlobalBounds().width*2.0f, MenuButtons[i].getPosition().y + 20);
		MenuText[i].setCharacterSize(25U);
		MenuText[i].setFillColor(sf::Color::White);
		MenuText[i].setFont(m_ArialBlackfont);
		
	}
	
	for (int i = 0; i < 5; i++)
	{
		selectorButton[i].setSize(sf::Vector2f(100, 50));
		selectorButton[i].setPosition(15, (i * 60) + 10);
		selectorButton[i].setOutlineColor(sf::Color::White);
		selectorButton[i].setOutlineThickness(1.0f);
		if (i != 4)
		{
			selectorButton[i].setTexture(&mapItemsTexture);
			selectorButton[i].setTextureRect(mapItemsI[i]);
		}
		else
		{
			selectorButton[i].setFillColor(sf::Color::Cyan);
		}
		
	}
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
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setTextureRect(mapItemsI[3]);

			}
			if (levelData[col][row] == 0)
			{
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setFillColor(sf::Color::Transparent);
				level[col][row].setOutlineColor(sf::Color::White);
				level[col][row].setOutlineThickness(0.0f);
			}
			if (levelData[col][row] == 2)
			{
				
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setOutlineColor(sf::Color::White);
				level[col][row].setOutlineThickness(0.0f);
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setTextureRect(mapItemsI[2]);
			}
			if (levelData[col][row] == 5)
			{

				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setOutlineColor(sf::Color::White);
				level[col][row].setOutlineThickness(0.0f);
				level[col][row].setTextureRect(mapItemsI[1]);
			}
			if (levelData[col][row] == 3)
			{
				level[col][row].setTexture(&mapItemsTexture);
				level[col][row].setSize(sf::Vector2f(70, 30));
				level[col][row].setPosition(row * 70, col * 30);
				level[col][row].setOutlineColor(sf::Color::White);
				level[col][row].setOutlineThickness(0.0f);
				level[col][row].setTextureRect(mapItemsI[0]);
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
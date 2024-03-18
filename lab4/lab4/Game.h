/// <summary>
/// author Ryan Holloway
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
struct TileInfo {
	bool movingUp;      // Flag to indicate the direction of movement
	bool timerRunning;  // Flag to indicate whether the timer is running
	sf::Clock timer;    // Timer to track the movement duration
};

enum class GameMode{LevelEditing, Playing, MainMenu, WinScreen};

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void gameUpdate();
	void menuUpdate();
	void moveScreen();
	void levelEditingUpdate();
	void render();
	void init();
	void brickDraw();
	
	void setupFontAndText();
	void levelInit();

	bool winTilePlacement{ false };

	GameMode gamemode{ GameMode::MainMenu };
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message

	bool m_exitGame; // control exiting game
	sf::RectangleShape playerShape;
	sf::RectangleShape background;

	float velocityX = 0, velocityY = 0, gravity = 0.3;
	int counter = 0;
	static const int numRows = 45;
	static const int numCols = 20;
	TileInfo tilesInfo[20][45];
	sf::RectangleShape level[numCols][numRows];

	sf::Clock clock;

	int levelData[numCols][numRows] =
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};

	
	void saveLevelData(int levelData[numCols][numRows], const char* filename);
	void loadLevelData(int levelData[numCols][numRows], const char* filename);

	const char* filename = "ASSETS\\LEVELDATA\\levelData.txt";
	bool selectingTile{ true };
	int currentTile{ 1 };
	sf::RectangleShape selectorButton[5];
	
	sf::RectangleShape MenuButtons[3];
	sf::Text MenuText[3];

	bool levelsBeenEdited{ false };
	sf::Text winText;
	sf::Text selecting;
	sf::Texture spritesheetTexture;
	sf::Texture mapItemsTexture;
	sf::IntRect mapItemsI[4] =
	{
		{0,505,287,117}, //bounce
		{0,0,235,215}, //multispike
		{0,216,337,136}, //top spike
		{0,353,301,151} //brick
	};

	sf::IntRect frames[8] = {
		{564,0,563,757},
		{1692,0,563,822},
		{0,0,563,766},
		{564,767,563,595},
		{1128,576,563,642},
		{1128,0,563,575},
		{0,767,563,672},
		{1692,823,563,586}
	};
	int currentFrame = 0;
	float frameTime = 0.0f;
	float timePerFrame = 0.08f;

};

#endif // !GAME_HPP


#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <random>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

/*
	Class that acts as game engine
	Wrapper class
*/

class Game
{
private:
	// Variables
	// Window
	sf::RenderWindow* window;
	sf::VideoMode videomode;
	sf::Event ev;

	// Mouse positions
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// Resources
	sf::Font font;

	// Text
	sf::Text uiText;

	// Game logic
	bool endGame = false;
	unsigned points;
	int health {10};
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	float fallSpeed;
	int maxEnemies;
	bool mouseHeld;

	// Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	// Private functions
	void initVariables();
	void initWindow();
	void initFonts();
	void initText();
	void initEnemies();

public:
	// Constructors and Destructors
	Game();
	~Game();

	// Accessors
	// Getters
	const bool running() const;
	const bool getEndGame() const;

	// Setters
	void setDifficulty(std::string dif);

	// Functions
	void spawnEnemy();

	void pollEvents();
	void updateMousePositions();
	void updateText();
	void updateEnemies();
	void update();

	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void render();
	
};


#include "Game.h"

// CONSTRUCTOR
Game::Game() {
	this->initVariables();
	this->initWindow();
    this->initFonts();
    this->initText();
    this->initEnemies();
}
// DESTRUCTOR
Game::~Game() {
	delete this->window;
}

// ACCESSORS
const bool Game::running() const
{
	return this->window->isOpen();
}
const bool Game::getEndGame() const
{
    return this->endGame;
}
void Game::setDifficulty(std::string dif)
{
    if (dif == "easy") {
        this->fallSpeed = 1.f;
        this->enemySpawnTimerMax = 50.f;
    }
    else if (dif == "medium") {
        this->fallSpeed = 2.f;
        this->enemySpawnTimerMax = 40.f;
    }
    else if (dif == "hard") {
        this->fallSpeed = 3.f;
        this->enemySpawnTimerMax = 30.f;
    }
}

// INIT
// VARIABLES
void Game::initVariables()
{
	this->window = nullptr;

    // Game logic
    this->points = 0;
    this->enemySpawnTimerMax = 10.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->fallSpeed = 1.f;
    this->maxEnemies = 50;
    this->mouseHeld = false;
}

// WINDOW
void Game::initWindow()
{
	this->videomode.width = 1000;
	this->videomode.height = 600;
	this->window = new sf::RenderWindow(this->videomode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(60);
}

// FONTS
void Game::initFonts()
{
    if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
    {
        std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" << std::endl;
    }
}

// TEXT
void Game::initText()
{
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(25);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("NONE");
}

// ENEMIES (green squares)
void Game::initEnemies()
{
    this->enemy.setPosition(0.f, 0.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setFillColor(sf::Color::White);
}

// Functions
void Game::pollEvents()
{
    // Game polling
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
            // When 'X' button was tapped
        case sf::Event::Closed:
            this->window->close();
            break;

            // When 'Esc' button was tapped
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape) {
                this->window->close();
            }
            break;
        }
    }
}

void Game::spawnEnemy()
{
    /*
        @return void

        Spawns enemies and sets their types, colors, positions randomly
        - Sets a random type
        - Sets a random positions
        - Sets a random color
        - Adds enemy to the vector
    */
    this->enemy.setPosition(
        static_cast<float>(std::rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f
    );


    // Randomize enemy type
    int type = std::rand() % 5; // randint from 0 to 5 (not including 5)

    switch (type)
    {
    case 0:
        this->enemy.setFillColor(sf::Color::Red);
        this->enemy.setSize(sf::Vector2f(20.f, 20.f));
        break;
    case 1:
        this->enemy.setFillColor(sf::Color::Magenta);
        this->enemy.setSize(sf::Vector2f(30.f, 30.f));
        break;
    case 2:
        this->enemy.setFillColor(sf::Color::Cyan);
        this->enemy.setSize(sf::Vector2f(50.f, 50.f));
        break;
    case 3:
        this->enemy.setFillColor(sf::Color::Yellow);
        this->enemy.setSize(sf::Vector2f(60.f, 60.f));
        break;
    case 4:
        this->enemy.setFillColor(sf::Color::Green);
        this->enemy.setSize(sf::Vector2f(70.f, 70.f));
        break;

    }


    // Spawn the enemy
    this->enemies.push_back(this->enemy);
}

void Game::updateEnemies()
{
    /*
        @return void

        Updates the enemy spawn and spawns enemies
        when the total amount of enemies is smaller than the maximum.
        Moves the enemies downwards
        Removes the enemies at the edge of the screen // TODO
    */

    // Updating timer for enemy spawning

    if (this->enemies.size() < this->maxEnemies)
    {

        if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
            // Spawn the enemy and reset the timer

            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else {
            this->enemySpawnTimer += 1.f;
        }
    }

    // Moving and updating enemies
    for (int i{}; i < this->enemies.size(); i++)
    {
        // Move the enemies
        this->enemies[i].move(0.f, this->fallSpeed);

        // If the enemy is past the bottom of the window
        if (this->enemies[i].getPosition().y > this->window->getSize().y) {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
            std::cout << "You lost 1 hp :( " << "You still have " << this->health << " hp." << std::endl;
        }
    }

    // Check if clicked upon
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        if (this->mouseHeld == false)
        {
            this->mouseHeld = true;
            bool deleted = false;

            for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                    // Gain points
                    if (this->enemies[i].getFillColor() == sf::Color::Red)
                    {
                        this->points += 10;
                    }
                    else if (this->enemies[i].getFillColor() == sf::Color::Magenta)
                    {
                        this->points += 5;
                    }
                    else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
                    {
                        this->points += 3;
                    }
                    else if (this->enemies[i].getFillColor() == sf::Color::Yellow)
                    {
                        this->points += 2;
                    }
                    else if (this->enemies[i].getFillColor() == sf::Color::Green)
                    {
                        this->points += 1;
                    }

                    std::cout << "Current points: " << this->points << std::endl;
                    
                    

                    // Delete the enemy
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);

                }
            }
        }
    }
    else {
        this->mouseHeld = false;
    }
}

void Game::updateMousePositions()
{
    /*
        @return void

        updates mouse positions:
        - mouse positions relative to window (Vector2i)
    */

    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateText()
{
    std::stringstream ss;
    ss << "Points: " << this->points << "\n"
        << "Health: " << this->health;
    this->uiText.setString(ss.str());
}

void Game::update()
{
    /*
        @return void

        Updates everything that is going on in this game:
        - Checks poll events
        - Checks mouse positions
        - Updates enemies
    */

    this->pollEvents();

    if (!this->endGame) 
    {
        this->updateMousePositions();

        this->updateText();

        this->updateEnemies();
    }

    // Endgame condition
    if (this->health <= 0) {
        this->endGame = true;
    }
}



void Game::render()
{
    /*
        @return void 

        - clear old frame
        - render objects
        - display frame in window
        Renders the game objects
    */

    this->window->clear();

    // Draw game
    // Draw enemies
    this->renderEnemies(*this->window);

    // Draw text
    this->renderText(*this->window);

    this->window->display();
}

void Game::renderText(sf::RenderTarget& target)
{
    target.draw(this->uiText);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
    // Rendering all the enemies
    for (auto& e : this->enemies)
    {
        target.draw(e);
    }
}

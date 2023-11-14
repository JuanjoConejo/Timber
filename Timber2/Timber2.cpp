// Include important C++ libraries here
#include <sstream>
#include <SFML/Graphics.hpp>
// Make code easier to type with "using namespace"
using namespace sf;
using namespace std;

// Function declaration


// Where is the player/branch?
// Left or Right
const int MaxWidth = 1920;
const int MaxHeight = 1080;
const int PosTimeBar = 980;
const int NUM_BRANCHES = 6;

// Where is the player / branch ?

// Left or Right
enum class side { LEFT, RIGHT, NONE };

// Cloud data
struct Cloud {
	bool active;
	sf::Sprite sprite;
	int speed;
};

int main()
{


	int currentScore = 0;

	Sprite branches[NUM_BRANCHES];
	side branchPositions[NUM_BRANCHES];
	srand((int)time(0) * 10);

	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "JOC DE JUANJO", Style::Fullscreen);

	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	// Create a sprite
	Sprite spriteBackground;

	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	// Make a tree sprite
	Sprite spriteTree1, spriteTree2, spriteTree3;
	Texture textureTree1, textureTree2, textureTree3;

	textureTree1.loadFromFile("graphics/tree.png");
	spriteTree1.setTexture(textureTree1);
	spriteTree1.setPosition(1400, 0);
	spriteTree1.setScale(0.6, 1);

	textureTree2.loadFromFile("graphics/tree.png");
	spriteTree2.setTexture(textureTree2);
	spriteTree2.setPosition(810, 0);

	textureTree3.loadFromFile("graphics/tree.png");
	spriteTree3.setTexture(textureTree3);
	spriteTree3.setPosition(200, 0);
	spriteTree3.setScale(0.5, 1);


	// Prepare the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	// Is the bee currently moving?
	bool beeActive = false;
	// How fast can the bee fly
	float beeSpeed = 0.0f;
	// Define a structure for representing bees
	struct Bee {
		bool active;
		Sprite sprite;
		float speed;
	};

	const int numBees = 12;
	Bee spriteBees[numBees];
	// Initialize bee data
	for (int i = 0; i < numBees; ++i) {
		spriteBees[i].active = false;
		spriteBees[i].sprite.setTexture(textureBee);
		spriteBees[i].sprite.setPosition(0, 800);
		spriteBees[i].speed = 0.0f;
	}

	// make 3 cloud sprites from 1 texture
	Texture textureCloud;

	// Load 1 new texture
	textureCloud.loadFromFile("graphics/cloud.png");

	// 3 New sprites withe the same texture

	// Cloud data array
	const int numClouds = 12;
	const int distance = 150;
	Cloud spriteCloud[numClouds];
	for (int i = 0; i < numClouds; ++i) {
		spriteCloud[i].active = false;
		spriteCloud[i].sprite.setTexture(textureCloud);
		spriteCloud[i].sprite.setPosition(0, 0 * distance);
		spriteCloud[i].speed = 0.0f;
	}
	//MILLORA HUB
	RectangleShape rectangleBackground;
	rectangleBackground.setSize(Vector2f(620, 150));
	rectangleBackground.setPosition(10, 10);
	rectangleBackground.setFillColor(Color(0, 0, 0, 150));

	//MILLORA HUB FPC
	RectangleShape rectangle1Background;
	rectangle1Background.setSize(Vector2f(110, 30));
	rectangle1Background.setPosition(1815, 10);
	rectangle1Background.setFillColor(Color(0, 0, 0, 150));

	// Variables to control time itself
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((MaxWidth / 2) - timeBarStartWidth / 2, PosTimeBar);
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track whether the game is running
	bool paused = true;

	// Draw some text
	int score = 0;
	Text messageText;
	Text scoreText;
	// We need to choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);

	// Assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	// Make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	// Choose a color
	messageText.setFillColor(Color::White);
	Text fpsText;
	fpsText.setFont(font);
	fpsText.setCharacterSize(24);
	fpsText.setFillColor(Color::White);
	fpsText.setPosition(MaxWidth - 100, 10);

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top +
		textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);
	scoreText.setFillColor(Color::White);


	// Prepare 5 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		// Set the sprite's origin to dead centre
		// We can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

	// Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(600, 700);

	// The player starts on the left
	side playerSide = side::LEFT;

	// Prepare the gravestone
	Texture textureGravestone;
	textureGravestone.loadFromFile("graphics/rip.png");
	Sprite spriteGravestone;
	spriteGravestone.setTexture(textureGravestone);
	spriteGravestone.setPosition(600, 860);

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(AXE_POSITION_LEFT, 860);


	// Prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	// Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control the player input
	bool acceptInput = false;

	// Prepare the sound

	// Out of time

	// Track whether the game is running

	// Move all the branches down one place
	for (int seed = 1; seed <= 5; seed++)
	{
		for (int j = NUM_BRANCHES - 1; j > 0; j--) {
			branchPositions[j] = branchPositions[j - 1];
		}// Spawn a new branch at position 0
		// LEFT, RIGHT or NONE
		srand((int)time(0) + seed);
		int r = (rand() % 5);
		switch (r) {
		case 0:
			branchPositions[0] = side::LEFT;
			break;
		case 1:
			branchPositions[0] = side::RIGHT;
			break;
		default:
			branchPositions[0] = side::NONE;
			break;
		}
	}
	//game loop
	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::KeyReleased && !paused)
			{
				acceptInput = true;
				// hide the axe
				spriteAxe.setPosition(2000,
					spriteAxe.getPosition().y);
			}
		}
		// Start the game

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			// Reset the time and the score
			score = 0;

			timeRemaining = 6;
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}
			// Make sure the gravestone is hidden
			spriteGravestone.setPosition(675, 2000);
			// Move the player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		// Wrap the player controls to

		// Make sure we are accepting input
		if (acceptInput)
		{
			// More code here next...

			// First handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// Make sure the player is on the right
				playerSide = side::RIGHT;
				score++;
				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);

				// update the branches
				for (int j = NUM_BRANCHES - 1; j > 0; j--) {
					branchPositions[j] = branchPositions[j - 1];
				}
				// Spawn a new branch at position 0
				// LEFT, RIGHT or NONE
				int r = (rand() % 5);
				switch (r) {
				case 0:
					branchPositions[0] = side::LEFT;
					break;
				case 1:
					branchPositions[0] = side::RIGHT;
					break;
				default:
					branchPositions[0] = side::NONE;
					break;
				}
				// set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;

			}

			// Play a chop sound


			// Handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// Make sure the player is on the right
				playerSide = side::LEFT;
				score++;
				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);

				// update the branches
				for (int j = NUM_BRANCHES - 1; j > 0; j--) {
					branchPositions[j] = branchPositions[j - 1];
				}
				// Spawn a new branch at position 0
				// LEFT, RIGHT or NONE
				int r = (rand() % 5);
				switch (r) {
				case 0:
					branchPositions[0] = side::LEFT;
					break;
				case 1:
					branchPositions[0] = side::RIGHT;
					break;
				default:
					branchPositions[0] = side::NONE;
					break;
				}
				// set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;
			}
		}
		// Make sure the player is on the left

		// Add to the amount of time remaining

		// update the branches

		// set the log flying

		/*
		****************************************
		Update the scene
		****************************************
		*/
		if (!paused)
		{
			// Measure time


			Time dt = clock.restart();

			Time elapsedTime = dt;
			float fps = 1.0f / elapsedTime.asSeconds();
			std::stringstream ss;
			ss << "FPS: " << fps;
			fpsText.setString(ss.str());

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			// size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond *
				timeRemaining, timeBarHeight));
			if (timeRemaining <= 0.0f) {

				// Pause the game
				paused = true;

				// Change the message shown to the player
				messageText.setString("Out of time!!");

				//Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(MaxWidth / 2.0f, MaxHeight / 2.0f);
			}

			// Play the out of time sound


			// Setup the bee
			if (!beeActive)
			{
				beeSpeed = (rand() % 200) + 200;

				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
				//move the bee
			{
				spriteBee.setPosition(
					spriteBee.getPosition().x -
					(beeSpeed * dt.asSeconds()),
					spriteBee.getPosition().y);


				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}
			// Manage the bee
			for (int i = 0; i < numBees; ++i) {
				if (!spriteBees[i].active) {
					spriteBees[i].speed = static_cast<float>(rand() % 200 + 200);
					float height = static_cast<float>(rand() % 500 + 500);
					spriteBees[i].sprite.setPosition(2000, height);
					spriteBees[i].active = true;
				}
				else {
					spriteBees[i].sprite.setPosition(
						spriteBees[i].sprite.getPosition().x - (spriteBees[i].speed * dt.asSeconds()),
						spriteBees[i].sprite.getPosition().y);

					if (spriteBees[i].sprite.getPosition().x < -100) {
						spriteBees[i].active = false;
					}
				}
			}
			// Manage the clouds
			for (int i = 0; i < numClouds; ++i) {
				if (!spriteCloud[i].active) {
					spriteCloud[i].speed = rand() % 200;
					float height = rand() % (distance * (i + 1));
					spriteCloud[i].sprite.setPosition(-200, height);
					spriteCloud[i].active = true;

				}
				else {
					spriteCloud[i].sprite.setPosition(
						spriteCloud[i].sprite.getPosition().x + (spriteCloud[i].speed * dt.asSeconds()),
						spriteCloud[i].sprite.getPosition().y);
					if (spriteCloud[i].sprite.getPosition().x > MaxWidth) {
						spriteCloud[i].active = false;
					}
				}
			}




			// Update the score text
			if (score != currentScore) {
				std::stringstream ss;
				ss << "score = " << score;
				scoreText.setString(ss.str());
				currentScore = score;
			}

			// update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition(610, height);
					// Flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition(1330, height);
					// Set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					// Hide the branch
					branches[i].setPosition(3000, height);
				}

			}
			// Handle a flying log
			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x - (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y);
			}
			// has the player been squished by a branch?
			if (branchPositions[5] == playerSide)
			{
				//death
				paused = true;
				acceptInput = false;
				//draw the gravestone
				spriteGravestone.setPosition(525, 760);
				//hide the player
				spritePlayer.setPosition(2000, 600);
				//change the text of the message
				messageText.setString("SQUISHED !!");
				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				// play the death sound
			}
		}// End if(!paused)

		/*
		****************************************
		Draw the scene
		****************************************
		*/

		// Clear everything from the last frame
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);

		// Draw the clouds
		for (int i = 0; i < numClouds; i++)
		{
			window.draw(spriteCloud[i].sprite);
		}

		// Draw the tree
		window.draw(spriteTree1);
		window.draw(spriteTree2);
		window.draw(spriteTree3);

		// Draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}
		// Draw the player
		window.draw(spritePlayer);

		// Draw the axe
		window.draw(spriteAxe);
		// Draraw the flying log
		window.draw(spriteLog);
		// Draw the gravestone
		window.draw(spriteGravestone);
		// Draw the bee
		for (int i = 0; i < numBees; i++)
		{
			window.draw(spriteBees[i].sprite);
		}
		//Draw HUB
		window.draw(rectangleBackground);
		window.draw(rectangle1Background);

		// Draw the score
		window.draw(scoreText);

		if (paused)
		{
			// Draw our message
			window.draw(messageText);
		}
		window.draw(fpsText);

		// Draw the timebar
		window.draw(timeBar);
		// Show everything we just drew
		window.display();

		//FPS
	}

	return 0;
}
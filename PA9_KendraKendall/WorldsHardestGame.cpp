#include "WorldsHardestGame.h"

// PUBLIC MEMBER FUNCTIONS

/*Default Constructor*/
worldsHardestGame::worldsHardestGame()
{
	/*Set attributes*/
	mNumDeaths = 0;

	/*Load each level into map array*/
	for (int i = 0; i < NUM_LEVELS; ++i)
	{
		/*Define the level with an array of tile indices*/
		tileArrays[i] = new int[NUM_TILES];	//declare on the heap, need for collisions (pointers)
		createMapLayout(tileArrays[i], i);

		/*load returns false, file won't open*/
		if (!mLevelMaps[i].load("32squaretileset.png", sf::Vector2u(TILE_LENGTH, TILE_LENGTH),
			tileArrays[i], NUM_COLS, NUM_ROWS))
		{
			cout << "Error opening texture image file." << endl;
		}
	}
}

/*Destructor: deallocate the integer arrays on the heap*/
worldsHardestGame::~worldsHardestGame()
{
	for (int i = 0; i < NUM_LEVELS; ++i)
	{
		delete[] tileArrays[i]; //can delete array
	}
}

/*
Name: showMenu
Description: Displays menu to the screen.
*/
void worldsHardestGame::showMenu()
{
	cout << "1. Play game." << endl;
	cout << "2. Review game instructions." << endl;
	cout << "3. Exit." << endl;
}

/*
Name: printInstructions()
Description: Displays the game instructions to the screen.
*/
void worldsHardestGame::printInstructions()
{
	cout << "You are the red square." << endl;
	cout << "Move using WASD, for up, left, down, and right, respectively.";
	cout << "Avoid the pink circles and do not touch the walls, or your progress will be reset." << endl;
	cout << "Reach the yellow region to advance to the next level." << endl;
}

void worldsHardestGame::displayTotalDeaths()
{
	cout << "Total Deaths: " << mNumDeaths << endl;
}

/*
Name: getChoice
Description: Gets player menu choice.
*/
int worldsHardestGame::getChoice()
{
	int choice = 0;
	do
	{
		cout << "Please enter a choice <1-3>: ";
		cin >> choice;
	} while (choice > 3 || choice < 1);

	return choice;
}

/*
Name: sphereIntersect
Description: Checks if player hits sphere
*/
bool worldsHardestGame::sphereIntersect(Sphere * sphereArray)
{
	bool collision = false;

	/*Check only spheres used, number of spheres should be a part of level (currently MAX_SPHERE)*/
	for (int i = 0; i < MAX_SPHERES && collision == false; i++)
	{
		if (mPlayerSquare.getGlobalBounds().intersects(sphereArray[i].getGlobalBounds()))
		{
			collision = true;
		}
	}
	return collision;
}

/*
Name: runGame()
Description: Performs execution of the game loop
*/
void worldsHardestGame::runGame()
{
	sf::RenderWindow window; //declare window

							 /*Keys*/
	bool upKeyIsPressed = false, leftKeyIsPressed = false, downKeyIsPressed = false,
		rightKeyIsPressed = false, alive = false;

	int levelNum = 0; //start at level 0
	int direction = 1, direction2 = 1, direction3 = 1, direction4 = 1; //four directions of ball movement
	int speed = 1; //speed of spheres will increase when going up level

				   /*Public death sphere array for setting position*/
	Sphere gameSphereArray[MAX_SPHERES];

	/*Sounds*/
	sf::SoundBuffer slapSoundSource, winSoundSource;
	sf::Sound slapSound, winSound;
	sf::Music themeSong;

	/*Open window*/
	window.create(sf::VideoMode(512, 384), "World's Hardest Game"); //smaller screen size for better movement

																	/*Load sounds*/
	if (!this->loadSound("slap.wav", slapSoundSource, slapSound))
		cout << "Error loading 'slap' sound!" << endl;
	if (!this->loadSound("win.wav", winSoundSource, winSound))
		cout << "Error loading 'winning' sound!" << endl;

	/*Load game music*/
	if (!this->openMusicFile("themeSong.ogg", themeSong))
		cout << "Error loading theme song!" << endl;

	/*Play music*/
	themeSong.play(); //plays during gameplay

					  /*Load level information*/
	mLevelMaps[levelNum].loadSpheres(levelNum);

	/*Set position of objects*/
	mPlayerSquare.setPosition(sf::Vector2f(51, 275)); //initial level 1 location

													  /*Set position of spheres*/
	for (int i = 0; i < mLevelMaps[levelNum].getNumSpheres(); i++)
	{
		/*Set Position/X and Y values*/
		gameSphereArray[i].setPosition(mLevelMaps[levelNum].getDeathSphere(i).getXPosition(), mLevelMaps[levelNum].getDeathSphere(i).getYPosition());
		gameSphereArray[i].setXPosition(mLevelMaps[levelNum].getDeathSphere(i).getXPosition());
		gameSphereArray[i].setYPosition(mLevelMaps[levelNum].getDeathSphere(i).getYPosition());

		/*Set Direction and Range*/
		gameSphereArray[i].setDirection(mLevelMaps[levelNum].getDeathSphere(i).getDirection());
		gameSphereArray[i].setRange(mLevelMaps[levelNum].getDeathSphere(i).getRange());
	}

	/*Play the game until user wins or user exits*/
	while (window.isOpen()) //need to add option for user to exit/exit by winning
	{
		sf::Event event;

		/*Pops event onto event queue-- if none, returns false*/
		while (window.pollEvent(event))
		{
			// EVENT: window was closed
			if (event.type == sf::Event::Closed) //ensures we don't have an infinite loop
			{
				window.close();
			}

			// EVENT: keys pressed to move the square
			if (event.type == sf::Event::KeyPressed)
			{
				if (!upKeyIsPressed && event.key.code == sf::Keyboard::W)
					upKeyIsPressed = true;
				if (!leftKeyIsPressed && event.key.code == sf::Keyboard::A)
					leftKeyIsPressed = true;
				if (!downKeyIsPressed && event.key.code == sf::Keyboard::S)
					downKeyIsPressed = true;
				if (!rightKeyIsPressed && event.key.code == sf::Keyboard::D)
					rightKeyIsPressed = true;
			}

			// EVENT: keys releaased
			if (event.type == sf::Event::KeyReleased)
			{
				if (upKeyIsPressed && event.key.code == sf::Keyboard::W)
					upKeyIsPressed = false;
				if (leftKeyIsPressed && event.key.code == sf::Keyboard::A)
					leftKeyIsPressed = false;
				if (downKeyIsPressed && event.key.code == sf::Keyboard::S)
					downKeyIsPressed = false;
				if (rightKeyIsPressed && event.key.code == sf::Keyboard::D)
					rightKeyIsPressed = false;
			}

			/*Moves the square if the key press/release event occurred*/
			if (upKeyIsPressed)
				mPlayerSquare.move(0, -2);	// negative y = up
			if (leftKeyIsPressed)
				mPlayerSquare.move(-2, 0);	// negative x = left
			if (downKeyIsPressed)
				mPlayerSquare.move(0, 2);	// positive y = down
			if (rightKeyIsPressed)
				mPlayerSquare.move(2, 0);	// positive x = right

											// EVENT: Collision with obstacle or wall occurred
			if (squaredCollidedWithWall(levelNum) || sphereIntersect(gameSphereArray))
			{
				// 1. Send square back to starting position.
				alive = false;	// when !alive, loop below will send square back to start

				// 2. Slapping sound effect
				slapSound.play();

				// 3. Increment the number of deaths
				incrementNumDeaths();
			}

			// EVENT: player wins the level by collision with pale yellow tile
			if (playerBeatsLevel(levelNum))
			{

				// 1. Cool sound effect
				winSound.play();

				// if player wins (i.e. beats all 3 levels), print message to screen
				if (levelNum >= LEVEL_3)
				{
					cout << "You won! Congrats! More levels will be added soon :-)" << endl;
					window.close();
				}
				else {
					// 2. Increment num_level
					++levelNum;
				}
				// 3. Set player to !alive again so that square goes to initial position in next level.
				alive = false;

				// 4. Load in new level info
				/*Load level information*/
				mLevelMaps[levelNum].loadSpheres(levelNum);

				// 5. Fill new death sphere array
				for (int i = 0; i < mLevelMaps[levelNum].getNumSpheres(); i++)
				{
					/*Set Position/X and Y values*/
					gameSphereArray[i].setPosition(mLevelMaps[levelNum].getDeathSphere(i).getXPosition(), mLevelMaps[levelNum].getDeathSphere(i).getYPosition());
					gameSphereArray[i].setXPosition(mLevelMaps[levelNum].getDeathSphere(i).getXPosition());
					gameSphereArray[i].setYPosition(mLevelMaps[levelNum].getDeathSphere(i).getYPosition());

					/*Set Direction and Range*/
					gameSphereArray[i].setDirection(mLevelMaps[levelNum].getDeathSphere(i).getDirection());
					gameSphereArray[i].setRange(mLevelMaps[levelNum].getDeathSphere(i).getRange());
				}
					
			}

			/*If player is intially starting a level or died, sent to start*/
			if (!alive)
				sendToStart(levelNum);
			alive = true; // after, player should be alive regardless


		} // end of event loop

		//window.clear();	// clear window each time thru

		/*Draw items to window*/
		window.draw(mLevelMaps[levelNum]);	// draw map

											// draw moving ball obstacles on top of map
		for (int i = 0; i < mLevelMaps[levelNum].getNumSpheres(); i++)
		{
			window.draw(gameSphereArray[i]);
		}

		// draw num_deaths text (top right corner) on top of map

		window.draw(mPlayerSquare); //draw player

									/*Move spheres*/
		this->setBallSpeed(levelNum, speed);
		for (int i = 0; i < mLevelMaps[levelNum].getNumSpheres(); i++)
		{
			/*Set ball movement*/
			if (gameSphereArray[i].getDirection() == "up")
			{
				if (gameSphereArray[i].getPosition().y >= gameSphereArray[i].getYPosition())
					direction = -1; //move up
				else if (gameSphereArray[i].getPosition().y <= (gameSphereArray[i].getYPosition() - gameSphereArray[i].getRange()))
					direction = 1; //move down

				gameSphereArray[i].move(0, direction * speed);
			}

			else if (gameSphereArray[i].getDirection() == "down")
			{
				if (gameSphereArray[i].getPosition().y <= gameSphereArray[i].getYPosition())
					direction2 = 1; //move down
				else if (gameSphereArray[i].getPosition().y >= (gameSphereArray[i].getYPosition() + gameSphereArray[i].getRange()))
					direction2 = -1; //move up

				gameSphereArray[i].move(0, direction2 * speed);
			}

			else if (gameSphereArray[i].getDirection() == "right")
			{
				if (gameSphereArray[i].getPosition().x <= gameSphereArray[i].getXPosition())
					direction3 = 1; //move left
				else if (gameSphereArray[i].getPosition().x >= (gameSphereArray[i].getXPosition() + gameSphereArray[i].getRange()))
					direction3 = -1; //move right

				gameSphereArray[i].move(direction3 * speed, 0);
			}

			else if (gameSphereArray[i].getDirection() == "left")
			{
				if (gameSphereArray[i].getPosition().x >= gameSphereArray[i].getXPosition())
					direction4 = -1; //move right
				else if (gameSphereArray[i].getPosition().x <= (gameSphereArray[i].getXPosition() - gameSphereArray[i].getRange()))
					direction4 = 1; //move left

				gameSphereArray[i].move(direction4 * speed, 0);
			}
		}

		window.display(); //display drawn items to screen
	}
}



// PRIVATE MEMBER FUNCTIONS

/*
Name: sendToStart
Description: Sets the initial position of the square on the screen, dependant on current level.
If player dies, returns to start position.
*/
void worldsHardestGame::sendToStart(int level)
{
	switch (level)
	{
	case LEVEL_1:
		mPlayerSquare.setPosition(sf::Vector2f(51, 275));
		break;
	case LEVEL_2:
		mPlayerSquare.setPosition(sf::Vector2f(243, 307));
		break;
	case LEVEL_3:
		mPlayerSquare.setPosition(sf::Vector2f(51, 51));
		break;
	}
}

/*
Name: incrementNumDeaths()
Description: player died, so increment the number of deaths
*/
void worldsHardestGame::incrementNumDeaths()
{
	++mNumDeaths;
}

// Description: This function modifies map[] so that it contains each entry in the array
//				contains a number corresponding to a tile (0 == grey, 1 == white,
//				2 == green, 3 == yellow). Note: map[] must be of size 192 (it won't crash
//				if it is larger than 192, but still problematic).
//				Since the level number is passed in, this means that the level map
//				will differ for each level (obviously).
void worldsHardestGame::createMapLayout(int map[], int level)
{
	switch (level)
	{
	case LEVEL_1:
	{
		int levelOneMap[NUM_TILES] =
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2,
			2, 0, 1, 2, 1, 0, 1, 0, 2, 0, 1, 0, 1, 0, 1, 2,
			2, 1, 0, 2, 0, 1, 0, 1, 2, 2, 2, 2, 2, 2, 0, 2,
			2, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 2,
			2, 1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 0, 2,
			2, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 1, 2,
			2, 1, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 2, 2,
			2, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
		};
		for (int i = 0; i < NUM_TILES; i++)
			map[i] = levelOneMap[i];
		break;
	}
	case LEVEL_2:
	{
		int levelTwoMap[NUM_TILES] =
		{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 3, 3, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 3, 3, 2, 2, 2, 2, 2, 2, 2,
			2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2,
			2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2,
			2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2,
			2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2,
			2, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 2,
			2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2,
			2, 2, 2, 2, 2, 2, 2, 0, 1, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 1, 0, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
		};
		for (int i = 0; i < NUM_TILES; i++)
			map[i] = levelTwoMap[i];
		break;
	}
	case LEVEL_3:
	{
		int levelThreeMap[NUM_TILES] =
		{ 2, 2, 2, 2, 2, 2, 2, 1, 0, 1, 0, 1, 0, 1, 0, 2,
			2, 0, 1, 2, 2, 2, 2, 0, 1, 0, 1, 0, 1, 0, 1, 2,
			2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2,
			2, 2, 2, 2, 2, 2, 2, 0, 1, 0, 1, 0, 1, 0, 1, 2,
			2, 2, 2, 2, 2, 2, 2, 1, 0, 2, 2, 1, 0, 2, 0, 2,
			2, 2, 2, 2, 2, 2, 2, 0, 1, 2, 2, 0, 1, 2, 1, 2,
			2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 2,
			2, 0, 1, 2, 2, 2, 2, 0, 1, 0, 1, 0, 1, 0, 1, 2,
			2, 1, 0, 2, 2, 2, 2, 1, 0, 1, 0, 1, 0, 1, 0, 2,
			2, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
			2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
		};
		for (int i = 0; i < NUM_TILES; i++)
			map[i] = levelThreeMap[i];
		break;
	}
	}
}


// Description: if the player square collides with a green wall tile in the particular
//				level's map, then true is returned. otherwise, false is returned.
bool worldsHardestGame::squaredCollidedWithWall(int level)
{
	bool result = false;

	// Find tiles at top left corner & bottom right corner of mPlayerSquare
	int topLeftTileNum = (((int)mPlayerSquare.getPosition().x) / TILE_LENGTH) + (((int)mPlayerSquare.getPosition().y / TILE_LENGTH) * NUM_COLS),
		bottomRightTileNum = (((int)mPlayerSquare.getPosition().x + SQUARE_LEN) / TILE_LENGTH) + ((((int)mPlayerSquare.getPosition().y + SQUARE_LEN) / TILE_LENGTH) * NUM_COLS),
		topRightTileNum = (((int)mPlayerSquare.getPosition().x + SQUARE_LEN) / TILE_LENGTH) + (((int)mPlayerSquare.getPosition().y / TILE_LENGTH) * NUM_COLS),
		bottomLeftTileNum = (((int)mPlayerSquare.getPosition().x) / TILE_LENGTH) + ((((int)mPlayerSquare.getPosition().y + SQUARE_LEN) / TILE_LENGTH) * NUM_COLS);

	// If the tiles at bottom right or top left are the "death tile", then collision had occurred
	if ((tileArrays[level])[topLeftTileNum] == DEATH_TILE || (tileArrays[level])[bottomRightTileNum] == DEATH_TILE ||
		(tileArrays[level])[topRightTileNum] == DEATH_TILE || (tileArrays[level])[bottomLeftTileNum] == DEATH_TILE)
		// tilesArray[level] contains an address (i.e. the first location in memory of heap allocated integer array)
		result = true;

	return result;
}

// Description: essentially the same code as squareCollidedWithWall; I could have combined
//				the functions... but I think it's more clear this way
//				If the player's square collides with WIN_TILE, then the level is won.
bool worldsHardestGame::playerBeatsLevel(int level)
{
	bool result = false;

	// Find tiles at top left corner & bottom right corner of mPlayerSquare
	int topLeftTileNum = (((int)mPlayerSquare.getPosition().x) / TILE_LENGTH) + (((int)mPlayerSquare.getPosition().y / TILE_LENGTH) * NUM_COLS),
		bottomRightTileNum = (((int)mPlayerSquare.getPosition().x + SQUARE_LEN) / TILE_LENGTH) + ((((int)mPlayerSquare.getPosition().y + SQUARE_LEN) / TILE_LENGTH) * NUM_COLS);

	// If the tiles at bottom right or top left are the "death tile", then collision had occurred
	if ((tileArrays[level])[topLeftTileNum] == WIN_TILE || (tileArrays[level])[bottomRightTileNum] == WIN_TILE)
		// tilesArray[level] contains an address (i.e. the first location in memory of heap allocated integer array)
		result = true;

	return result;
}

// Description: sets the speed of the moving balls for the level!
void worldsHardestGame::setBallSpeed(int level, int &speed)
{
	switch (level)
	{
	case LEVEL_1:
		speed = 1;		// initial speed
		break;
	case LEVEL_2:
		speed = 2;
		break;
	case LEVEL_3:
		speed = 3;
		break;
	}
}

// Description: loads the sound file into "sound buffer" and then set's the sound's
//				buffer to this buffer; must pass by reference because the soundbuffer
//				must remain in scope (in the runGame() function) in order to work
bool worldsHardestGame::loadSound(const std::string &fileName, sf::SoundBuffer &source, sf::Sound &sound)
{
	if (!source.loadFromFile(fileName))
		return false;
	sound.setBuffer(source);
	return true;		// return true if we bypassed 'if' statement
}

/*
Name: openMusicFile
Description: Opens music file and sets attributes (pitch, loop)
*/
bool worldsHardestGame::openMusicFile(const std::string &fileName, sf::Music &music)
{
	if (!music.openFromFile(fileName))
		return false;
	music.setPitch(0.95);
	music.setLoop(true);

	return true;
}
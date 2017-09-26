#pragma once

/*Libraries*/
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp> //for sound effects
#include <SFML/Audio/Music.hpp>
#include <iostream>
#include <string>

/*Header Files*/
#include "Square.h"
#include "LevelMap.h"

/*Constants*/
#define NUM_LEVELS 3
#define NUM_TILES 192
#define TILE_LENGTH 32
#define NUM_ROWS 12
#define NUM_COLS 16
#define DEATH_TILE 2
#define WIN_TILE 3
#define LEVEL_1 0
#define LEVEL_2 1
#define LEVEL_3 2

using std::cin;
using std::cout;
using std::endl;
using std::string;

class worldsHardestGame
{
public:
	/*Constructor*/
	worldsHardestGame();

	/*Destructor*/
	~worldsHardestGame();

	/*Menu options*/
	void showMenu();
	void printInstructions();
	void displayTotalDeaths();

	/*Getters*/
	int getChoice();

	/*Runs the game, call in main*/
	void runGame();

private:
	/*Attributes*/
	int * tileArrays[NUM_LEVELS];		// shows which tiles are which colors
	LevelMap mLevelMaps[NUM_LEVELS];	// array of available levels

										/*Player Attributes*/
	Square mPlayerSquare;
	unsigned int mNumDeaths; // will need to set to 0 in constructor

							 /*Private Functions*/
	void sendToStart(int level);
	void incrementNumDeaths();
	void createMapLayout(int map[], int level);
	bool playerBeatsLevel(int level);
	bool squaredCollidedWithWall(int level);
	bool sphereIntersect(Sphere * sphereArray);
	void setBallSpeed(int level, int &speed);

	/*Sound effects*/
	bool loadSound(const std::string &fileName, sf::SoundBuffer &source, sf::Sound &sound);
	bool openMusicFile(const std::string &fileName, sf::Music &music);
};
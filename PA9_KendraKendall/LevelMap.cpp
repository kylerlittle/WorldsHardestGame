#include "LevelMap.h"

/*Public functions*/

/*
	Name: load()
	Description: Loads a tilemap based on the tile texture image and an integer array of
				 each tile "type" (ex 2 = green, etc.)
*/
bool LevelMap::load(const std::string& tileset, sf::Vector2u tileSize, int* tiles,
	unsigned int width, unsigned int height)
{
	/*Load tilesest from texture file*/
	if (!m_tileset.loadFromFile(tileset))
	{
		return false; // if file can't be loaded, return and break
	}
	
	/*Resize the vertex array to fit the level size*/
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(width * height * 4); //each tile has 4 coordinates, must have them available for each tile

	/*Populate the vertex array, with one quad per tile*/
	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j)
		{
			//Get the current tile number
			int tileNumber = tiles[i + j * width];

			//Find its position in the tileset texture
			int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
			int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

			//Get a pointer to the current tile's quad
			sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

			//Define its 4 corners, can use sides to detect collision
			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			//Define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
		}

	return true; //Return true if successful
}

/*Getters*/

/*
	Name: getDeathSphere
	Description: Returns one death sphere based on index arguement
*/
Sphere LevelMap::getDeathSphere(int index)
{
	return mDeathSphereArray[index];
}

/*
	Name: getNumSpheres
	Description: Returns the total number of spheres used in level
*/
int LevelMap::getNumSpheres()
{
	return mTotalNumSpheres;
}

/*
	Name: loadSpheres
	Description: Loads the sphere and level information from a file
*/
void LevelMap::loadSpheres(int level)
{
	if (level == 0)
	{
		mLevelFile.open("level1.txt", std::ios::in);
	}
	else if (level == 1)
	{
		mLevelFile.open("level2.txt", std::ios::in);
	}
	else if (level == 2)
	{
		mLevelFile.open("level3.txt", std::ios::in);
	}

	//temp string var
	std::string temp;

	/*Load level data from file*/
	while (!mLevelFile.eof())
	{
		/*Load in number of spheres in level*/
		std::getline(mLevelFile, temp, '\n');
		mTotalNumSpheres = atoi(temp.c_str());

		/*Load in Sphere information*/
		for (int i = 0; i < mTotalNumSpheres; i++)
		{
			std::getline(mLevelFile, temp, ','); //x position
			mDeathSphereArray[i].setXPosition(atoi(temp.c_str()));

			std::getline(mLevelFile, temp, '\n'); //y position
			mDeathSphereArray[i].setYPosition(atoi(temp.c_str()));

			std::getline(mLevelFile, temp, '\n'); //direction
			mDeathSphereArray[i].setDirection(temp);

			std::getline(mLevelFile, temp, '\n'); //range
			mDeathSphereArray[i].setRange(atoi(temp.c_str()));
		}
	}
}

/*Private Functions*/

/*
	Name: draw()
	Description: Overloads draw function to work with tilemap created in load. Call when drawing other
				 SFML objects to screen.
*/
void LevelMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = &m_tileset;

	// draw the vertex array
	target.draw(m_vertices, states);
}
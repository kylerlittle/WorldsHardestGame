#pragma once

/*Libraries*/
#include <iostream>
#include <SFML\Graphics.hpp>
#include <fstream>

#include "Sphere.h"

/*Constants*/
#define MAX_SPHERES 8

// Adapted from:
// https://www.sfml-dev.org/tutorials/2.4/graphics-vertex-array.php
// "TileMap Example" for loading in a tile texture and creating a map

class LevelMap : public sf::Drawable, public sf::Transformable
{
public:

	bool load(const std::string& tileset, sf::Vector2u tileSize, int* tiles,
			 unsigned int width, unsigned int height);

	/*Getters*/
	Sphere getDeathSphere(int index);
	int getNumSpheres();

	void loadSpheres(int level);

private:
	// Allows for the more natural window.draw(levelMap) function call rather than levelMap.draw()
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;

	/*Sphere Attributes*/
	Sphere mDeathSphereArray[MAX_SPHERES];
	int mTotalNumSpheres;

	/*Level File*/
	std::fstream mLevelFile;
};
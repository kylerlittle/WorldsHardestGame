#pragma once

/*Libaries*/
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>

/*Constants*/
#define SQUARE_LEN 25

using std::cin;
using std::cout;
using std::endl;
using std::string;

/*The user's movable "character". Inherits from drawable RectangleShape.*/
class Square : public sf::RectangleShape
{
public:
	Square();
private:
};
/*Libraries*/
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class Sphere : public sf::CircleShape
{
public:
	/*Constructors*/
	Sphere();
	Sphere(sf::Color newFill, int newThickness, sf::Color newOutline);

	/*Getters*/
	int getXPosition() const;
	int getYPosition() const;
	std::string getDirection() const;
	int getRange() const;

	/*Setters*/
	void setXPosition(int newX);
	void setYPosition(int newY);
	void setDirection(std::string newDirection);
	void setRange(int newRange);

private:
	/*Data members, tracks the spheres position and direction of movement*/
	int xPosition;
	int yPosition;

	std::string direction; //which direction ball is moving
	int range; //range of motion, how far can go before heading back to original position
};
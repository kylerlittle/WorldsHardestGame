#include "Sphere.h"

/*Public Functions*/

/*
	Name: Sphere()
	Date Created: 4/22/17
	Description: Default constructor
*/
Sphere::Sphere() : CircleShape(10.0, 300)
{
	setFillColor(sf::Color::Magenta);
	setOutlineThickness(1);
	setOutlineColor(sf::Color::Red);
}


/*
	Name: Sphere()
	Date Created: 4/22/17
	Description: Constructs a circleshape object and sets attributes with arguments
*/
Sphere::Sphere(sf::Color newFill, int newThickness, sf::Color newOutline) : CircleShape(10.0, 300)
{
	setFillColor(newFill);
	setOutlineThickness(newThickness);
	setOutlineColor(newOutline);
}

/*Getter Functions*/
int Sphere::getXPosition() const
{
	return xPosition;
}

int Sphere::getYPosition() const
{
	return yPosition;
}

std::string Sphere::getDirection() const
{
	return direction;
}

int Sphere::getRange() const
{
	return range;
}

/*Setter Functions*/
void Sphere::setXPosition(int newX)
{
	xPosition = newX;
}

void Sphere::setYPosition(int newY)
{
	yPosition = newY;
}

void Sphere::setDirection(std::string newDirection)
{
	direction = newDirection;
}

void Sphere::setRange(int newRange)
{
	range = newRange;
}
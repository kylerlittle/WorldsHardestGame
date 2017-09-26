#include "Square.h"

/*Default constructor*/
Square::Square() : RectangleShape(sf::Vector2f(SQUARE_LEN, SQUARE_LEN))
{
	setFillColor(sf::Color::Red);
	setOutlineThickness(1);
	setOutlineColor(sf::Color::Black);
}
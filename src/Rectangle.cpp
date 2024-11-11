
#include "Rectangle.h"
#include <SFML/Graphics.hpp>

Rectangle::Rectangle(sf::Vector2f size, sf::Vector2f position) : m_size(size)
{
    sf::Transformable::setPosition(position);
}

bool Rectangle::collidesWith(Rectangle* pOther)
{
    // Get the bounding boxes of both rectangles
    sf::FloatRect thisBounds(getPosition(), getSize());
    sf::FloatRect otherBounds(pOther->getPosition(), pOther->getSize());

    // Check for intersection using SFML's intersects method
    return thisBounds.intersects(otherBounds);
}

void Rectangle::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite);
}

sf::Vector2f Rectangle::getCenter()
{
    return getPosition() + (m_size * 0.5f);
}

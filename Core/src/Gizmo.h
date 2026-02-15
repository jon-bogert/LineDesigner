#pragma once

#include <SFML/Graphics.hpp>

class Gizmo
{
public:
	void Initialize();

	void DrawTo(sf::RenderTarget& target, const sf::Vector2f& position);

private:
	sf::Texture m_arrowTex;
	sf::Sprite m_xArrow;
	sf::Sprite m_yArrow;
	sf::CircleShape m_center;
};
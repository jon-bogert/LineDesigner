#pragma once

#include <SFML/Graphics.hpp>

class Gizmo
{
public:
	void Initialize();

	void DrawTo(sf::RenderTarget& target, const sf::Vector2f& position);

	bool IsDragging() const { return m_isDragging; }
	bool IsHovering() const { return m_moveMode != NoAxis; }
	void CheckHover(const float scale, const sf::Vector2f& mousePoint);

	void StartDragging() { m_isDragging = true; }
	void EndDragging() { m_isDragging = false; }
	sf::Vector2f Move(const sf::Vector2f& delta);

private:

	enum MoveMode
	{
		NoAxis,
		AllAxis,
		XAxis,
		YAxis,
	};

	sf::Texture m_arrowTex;
	sf::Sprite m_xArrow;
	sf::Sprite m_yArrow;
	sf::CircleShape m_center;

	sf::Color m_xColor = sf::Color::Red;
	sf::Color m_yColor = sf::Color::Green;
	sf::Color m_zColor = sf::Color::Blue;
	sf::Color m_selectColor = sf::Color::Yellow;

	MoveMode m_moveMode = NoAxis;
	bool m_isDragging = false;
};
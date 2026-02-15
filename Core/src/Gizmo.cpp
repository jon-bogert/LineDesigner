#include "Gizmo.h"

#define GIZ_VISUAL_SCALE 0.5f
#define GIZ_CENTER_RAD 10.f

void Gizmo::Initialize()
{
	m_arrowTex.loadFromFile("assets/arrow.png");
	m_xArrow.setTexture(m_arrowTex);
	m_yArrow.setTexture(m_arrowTex);

	m_xArrow.setOrigin((sf::Vector2f)m_arrowTex.getSize() * 0.5f);
	m_yArrow.setOrigin((sf::Vector2f)m_arrowTex.getSize() * 0.5f);

	m_xArrow.setColor(sf::Color::Red);
	m_yArrow.setColor(sf::Color::Green);

	m_xArrow.setRotation(-90.f);

	m_center.setRadius(GIZ_CENTER_RAD);
	m_center.setFillColor(sf::Color::Blue);
	m_center.setOrigin({ GIZ_CENTER_RAD , GIZ_CENTER_RAD });
}

void Gizmo::DrawTo(sf::RenderTarget& target, const sf::Vector2f& position)
{
	sf::View currView = target.getView();
	sf::View defaultView = target.getDefaultView();
	
	float scale = GIZ_VISUAL_SCALE * (currView.getSize().y / defaultView.getSize().y);
	m_xArrow.setScale({ scale, scale });
	m_yArrow.setScale({ scale, scale });
	m_center.setScale({ scale, scale });
	m_xArrow.setPosition(position);
	m_yArrow.setPosition(position);
	m_center.setPosition(position);

	target.draw(m_yArrow);
	target.draw(m_xArrow);
	target.draw(m_center);
}
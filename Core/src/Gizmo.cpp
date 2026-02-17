#include "Gizmo.h"
#include "arrow_png.h"

#define GIZ_VISUAL_SCALE 0.5f
#define GIZ_CENTER_RAD 10.f
#define GIZ_SELECT_SIZE 50.f
#define GIZ_SELECT_LENGTH 200.f

void Gizmo::Initialize()
{
	res::arrow_png(m_arrowData, m_arrowDataCount);
	m_arrowTex.loadFromMemory(m_arrowData.get(), m_arrowDataCount);
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
	switch (m_moveMode)
	{
	case AllAxis:
		m_center.setFillColor(m_selectColor);
		m_xArrow.setColor(m_selectColor);
		m_yArrow.setColor(m_selectColor);
		break;
	case XAxis:
		m_center.setFillColor(m_zColor);
		m_xArrow.setColor(m_selectColor);
		m_yArrow.setColor(m_yColor);
		break;
	case YAxis:
		m_center.setFillColor(m_zColor);
		m_xArrow.setColor(m_xColor);
		m_yArrow.setColor(m_selectColor);
		break;
	default:
		m_center.setFillColor(m_zColor);
		m_xArrow.setColor(m_xColor);
		m_yArrow.setColor(m_yColor);
		break;
	}

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

void Gizmo::CheckHover(const float scale, const sf::Vector2f& mousePoint)
{
	sf::FloatRect bounds;
	bounds.left = m_center.getPosition().x - (GIZ_SELECT_SIZE * GIZ_VISUAL_SCALE * scale * 0.5f);
	bounds.width = GIZ_SELECT_SIZE * GIZ_VISUAL_SCALE * scale;
	bounds.top = m_center.getPosition().y - (GIZ_SELECT_SIZE * GIZ_VISUAL_SCALE * scale * 0.5f);
	bounds.height = GIZ_SELECT_SIZE * GIZ_VISUAL_SCALE * scale;

	m_moveMode = NoAxis;

	if (bounds.contains(mousePoint))
	{
		m_moveMode = AllAxis;
	}

	bounds.left = m_center.getPosition().x + (GIZ_SELECT_SIZE * GIZ_VISUAL_SCALE * scale * 0.5f);
	bounds.width = (GIZ_SELECT_LENGTH * GIZ_VISUAL_SCALE * scale) - (GIZ_SELECT_SIZE * GIZ_VISUAL_SCALE * scale * 0.5f);

	if (bounds.contains(mousePoint))
	{
		m_moveMode = XAxis;
	}

	bounds.left = m_center.getPosition().x - (GIZ_SELECT_SIZE * GIZ_VISUAL_SCALE * scale * 0.5f);
	bounds.width = GIZ_SELECT_SIZE * GIZ_VISUAL_SCALE * scale;
	bounds.top = m_center.getPosition().y + (GIZ_SELECT_SIZE * GIZ_VISUAL_SCALE * scale * 0.5f);
	bounds.height = (GIZ_SELECT_LENGTH * GIZ_VISUAL_SCALE * scale) - (GIZ_SELECT_SIZE * GIZ_VISUAL_SCALE * scale * 0.5f);
	
	if (bounds.contains(mousePoint))
	{
		m_moveMode = YAxis;
	}

}

sf::Vector2f Gizmo::Move(const sf::Vector2f& delta)
{
	sf::Vector2f result = delta;
	switch (m_moveMode)
	{
	case XAxis:
		result.y = 0.f;
		break;
	case YAxis:
		result.x = 0.f;
		break;
	case NoAxis:
		result = sf::Vector2f();
		break;
	}

	return result;
}

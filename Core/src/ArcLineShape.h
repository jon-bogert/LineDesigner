#pragma once

#include <SFML/Graphics.hpp>

class ArcLineShape : public sf::Drawable
{
public:
	void SetStart(const sf::Vector2f& start);
	void SetEnd(const sf::Vector2f& end);
	void SetPoints(const sf::Vector2f& start, const sf::Vector2f& end);
	void SetWidth(const float width);
	void SetRadius(const float radius);
	void SetSegmentCount(const uint32_t count);
	void SetCapVertexCount(const uint32_t count);
	void SetInvertArc(const bool invertArc);

	void SetParameters(const sf::Vector2f& start, const sf::Vector2f& end, const float width, const float radius, const bool inevertArc, const uint32_t segmentCount, uint32_t capVertexCount, const sf::Color& color);

	sf::Vector2f GetStart() const { return m_start; }
	sf::Vector2f GetEnd() const { return m_end; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	//[[nodiscard]] std::size_t getPointCount() const override { return m_points.size(); }
	//[[nodiscard]] sf::Vector2f getPoint(std::size_t index) const override { return m_points[index]; }

private:
	void Calculate();

	sf::Vector2f m_start = { 0.f, 0.f };
	sf::Vector2f m_end = { 1.f, 1.f };
	float m_width = 1.f;
	float m_radius = 0.f;
	bool m_invertArc = false;
	uint32_t m_segmentCount = 32;
	uint32_t m_capVertCount = 1;
	sf::Color m_color = sf::Color::White;

	//std::vector<sf::Vector2f> m_points;
	sf::VertexArray m_vertices;
};
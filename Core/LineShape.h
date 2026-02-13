#pragma once

#include <SFML/Graphics.hpp>

class LineShape : public sf::Shape
{
public:
	void SetStart(const sf::Vector2f& start);
	void SetEnd(const sf::Vector2f& end);
	void SetPoints(const sf::Vector2f& start, const sf::Vector2f& end);
	void SetWidth(const float width);
	void SetCapVertexCount(const uint32_t count);
	void SetParameters(const sf::Vector2f& start, const sf::Vector2f& end, const float width, const uint32_t capVertCount);

	[[nodiscard]] std::size_t getPointCount() const override { return m_points.size(); }
	[[nodiscard]] sf::Vector2f getPoint(std::size_t index) const override { return m_points[index]; }

private:
	void Calculate();

	sf::Vector2f m_start = {0.f, 0.f};
	sf::Vector2f m_end = { 1.f, 1.f };
	float m_width = 1.f;
	uint32_t m_capVertCount = 1;

	std::vector<sf::Vector2f> m_points;
};
#include "LineShape.h"

#include "Mathematics.h"

void LineShape::SetStart(const sf::Vector2f& start)
{
	m_start = start;
	Calculate();
}

void LineShape::SetEnd(const sf::Vector2f& end)
{
	m_end = end;
	Calculate();
}

void LineShape::SetPoints(const sf::Vector2f& start, const sf::Vector2f& end)
{
	m_start = start;
	m_end = end;
	Calculate();
}

void LineShape::SetWidth(const float width)
{
	m_width = width;
}

void LineShape::SetCapVertexCount(const uint32_t count)
{
	m_capVertCount = count;
	Calculate();
}

void LineShape::SetParameters(const sf::Vector2f& start, const sf::Vector2f& end, const float width, const uint32_t capVertCount)
{
	m_start = start;
	m_end = end;
	m_width = width;
	m_capVertCount = capVertCount;
	Calculate();
}

void LineShape::Calculate()
{
	sf::Vector2f forward = xe::Math::Normalize(m_end - m_start);
	sf::Vector2f normal = { -forward.y, forward.x };
	float halfWidth = m_width * 0.5f;

	m_points.resize(m_capVertCount * 2 + 4);

	m_points[0] = m_start - (normal * halfWidth);
	m_points[1] = m_end - (normal * halfWidth);

	float radRotation = xe::Math::kPi / (m_capVertCount + 1);

	for (size_t i = 0; i < m_capVertCount; ++i)
	{
		sf::Vector2f rotatedVect = xe::Math::Rotate(-normal, (i + 1) * radRotation);
		m_points[i + 2] = m_end + rotatedVect * halfWidth;
	}

	m_points[2 + m_capVertCount] = m_end + (normal * halfWidth);
	m_points[2 + m_capVertCount + 1] = m_start + (normal * halfWidth);

	for (size_t i = 0; i < m_capVertCount; ++i)
	{
		sf::Vector2f rotatedVect = xe::Math::Rotate(normal, (i + 1) * radRotation);
		m_points[2 + i + m_capVertCount + 2] = m_start + rotatedVect * halfWidth;
	}

	update();
}

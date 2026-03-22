#include "ArcLineShape.h"

#include "Mathematics.h"

#define MAX_RADIUS_MULT 10.f

void ArcLineShape::SetStart(const sf::Vector2f& start)
{
	m_start = start;
	Calculate();
}

void ArcLineShape::SetEnd(const sf::Vector2f& end)
{
	m_end = end;
	Calculate();
}

void ArcLineShape::SetPoints(const sf::Vector2f& start, const sf::Vector2f& end)
{
	m_start = start;
	m_end = end;
	Calculate();
}

void ArcLineShape::SetWidth(const float width)
{
	m_width = width;
	Calculate();
}

void ArcLineShape::SetRadius(const float radius)
{
	m_radius = radius;
	Calculate();
}

void ArcLineShape::SetSegmentCount(const uint32_t count)
{
	m_segmentCount = count;
}

void ArcLineShape::SetCapVertexCount(const uint32_t count)
{
	m_capVertCount = count;
	Calculate();
}

void ArcLineShape::SetInvertArc(const bool invertArc)
{
	m_invertArc = invertArc;
}

void ArcLineShape::SetParameters(const sf::Vector2f& start, const sf::Vector2f& end, const float width, const float radius, const bool invertArc, const uint32_t segmentCount, uint32_t capVertexCount, const sf::Color& color)
{
	m_start = start;
	m_end = end;
	m_width = width;
	m_radius = radius;
	m_invertArc = invertArc;
	m_segmentCount = segmentCount;
	m_capVertCount = capVertexCount;
	m_color = color;

	Calculate();
}

void ArcLineShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_vertices);
}

void ArcLineShape::Calculate()
{
	m_vertices.clear();
	m_vertices.setPrimitiveType(sf::TriangleStrip);

	// 1. Find the center and radius
	sf::Vector2f center = (m_start + m_end) * 0.5f;
	sf::Vector2f dir = m_end - m_start;
	float halfDist = std::sqrt(dir.x * dir.x + dir.y * dir.y) / 2.0f;
	float maxRadius = halfDist * MAX_RADIUS_MULT;
	float radius = xe::Math::Lerp(maxRadius, halfDist, xe::Math::Abs(m_radius));
	
	sf::Vector2f normal = xe::Math::Normalize((m_radius < 0.f) ? sf::Vector2f(dir.y, -dir.x) : sf::Vector2(-dir.y, dir.x));
	//sf::Vector2f normal = xe::Math::Normalize(sf::Vector2f(dir.y, -dir.x));
	sf::Vector2f pivot = center + (normal * xe::Math::Sqrt(xe::Math::Sqr(radius) - xe::Math::Sqr(halfDist)));

	// 2. Determine angles
	float startAngle = std::atan2(m_start.y - pivot.y, m_start.x - pivot.x);
	float endAngle = std::atan2(m_end.y - pivot.y, m_end.x - pivot.x);

	if (xe::Math::XOR(m_radius < 0.f, m_invertArc))
	{
		float tmp = startAngle;
		startAngle = endAngle;
		endAngle = tmp;
	
	}

	// Handle wrap-around to ensure we rotate the correct way
	if (endAngle < startAngle) endAngle += 2.0f * 3.14159f;

	// 3. Generate Vertices
	for (int i = 0; i <= m_segmentCount; ++i) {
		float t = (float)i / (float)m_segmentCount;
		float currentAngle = startAngle + t * (endAngle - startAngle);

		// Unit vector for the current direction
		sf::Vector2f unitDir(std::cos(currentAngle), std::sin(currentAngle));

		// Outer vertex
		sf::Vector2f outerPos = pivot + unitDir * (radius + m_width / 2.0f);
		m_vertices.append(sf::Vertex(outerPos, m_color));

		// Inner vertex
		sf::Vector2f innerPos = pivot + unitDir * (radius - m_width / 2.0f);
		m_vertices.append(sf::Vertex(innerPos, m_color));
	}
}

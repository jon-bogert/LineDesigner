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
    m_vertices.setPrimitiveType(sf::Triangles);

    // 1. Core Logic (Pivot and Radius)
    sf::Vector2f dir = m_end - m_start;
    float halfDist = std::sqrt(dir.x * dir.x + dir.y * dir.y) / 2.0f;
    float radius = xe::Math::Lerp(halfDist * MAX_RADIUS_MULT, halfDist, xe::Math::Abs(m_radius));

    sf::Vector2f normal = (m_radius < 0.f) ? sf::Vector2f(dir.y, -dir.x) : sf::Vector2f(-dir.y, dir.x);
    normal = xe::Math::Normalize(normal);

    float pivotDist = std::sqrt(std::max(0.f, radius * radius - halfDist * halfDist));
    sf::Vector2f pivot = ((m_start + m_end) * 0.5f) + (normal * pivotDist);

    float startAngle = std::atan2(m_start.y - pivot.y, m_start.x - pivot.x);
    float endAngle = std::atan2(m_end.y - pivot.y, m_end.x - pivot.x);

    if (xe::Math::XOR(m_radius < 0.f, m_invertArc))
    {
        std::swap(startAngle, endAngle);
    }

    while (endAngle < startAngle)
    {
        endAngle += xe::Math::kTwoPi;
    }

    auto addTriangle = [&](sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
    {
        m_vertices.append(sf::Vertex(p1, m_color));
        m_vertices.append(sf::Vertex(p2, m_color));
        m_vertices.append(sf::Vertex(p3, m_color));
    };

    // 2. Generate Main Arc Segments
    for (int i = 0; i < m_segmentCount; ++i)
    {
        float t1 = (float)i / m_segmentCount;
        float t2 = (float)(i + 1) / m_segmentCount;
        float ang1 = startAngle + t1 * (endAngle - startAngle);
        float ang2 = startAngle + t2 * (endAngle - startAngle);
    
        sf::Vector2f dir1(std::cos(ang1), std::sin(ang1));
        sf::Vector2f dir2(std::cos(ang2), std::sin(ang2));
    
        sf::Vector2f outer1 = pivot + dir1 * (radius + m_width * 0.5f);
        sf::Vector2f inner1 = pivot + dir1 * (radius - m_width * 0.5f);
        sf::Vector2f outer2 = pivot + dir2 * (radius + m_width * 0.5f);
        sf::Vector2f inner2 = pivot + dir2 * (radius - m_width * 0.5f);
    
        // Triangle 1
        addTriangle(inner1, outer1, outer2);
        // Triangle 2
        addTriangle(inner1, outer2, inner2);
    }

    // 3. Generate End Caps
    auto addCap = [&](sf::Vector2f center, float baseAngle, bool isStart)
    {
        float angleStep = 3.14159265f / m_capVertCount;
        // Flip the cap direction for the start vs end
        float dirMod = isStart ? -1.0f : 1.0f;

        for (int i = 0; i < m_capVertCount; ++i)
        {
            float a1 = baseAngle + (i * angleStep * dirMod);
            float a2 = baseAngle + ((i + 1) * angleStep * dirMod);

            addTriangle(
                center,
                center + sf::Vector2f(std::cos(a1), std::sin(a1)) * (m_width * 0.5f),
                center + sf::Vector2f(std::cos(a2), std::sin(a2)) * (m_width * 0.5f)
            );
        }
    };
    addCap(m_start, startAngle, true);
    addCap(m_end, endAngle, false);
}

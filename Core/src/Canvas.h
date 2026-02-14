#pragma once

#include "Structs.h"
#include "LineShape.h"

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <unordered_map>
#include <vector>

class Canvas
{
	struct Point
	{
		sf::Vector2f coord;
		sf::CircleShape visual;
	};

	struct Line
	{
		Connection connection;
		LineShape visual;

		void PositionVisual(const sf::Vector2f& a, const sf::Vector2f& b)
		{
			visual.SetParameters(a, b, 10, 10);
			visual.setFillColor(sf::Color::White);
		}
	};

public:

	enum class ClickModifier
	{
		Primary,
		Secondary,
		Add,
	};

	void Update();
	void OnGUI();
	void DrawTo(sf::RenderTarget& m_target);

	void Load(const std::filesystem::path& path);

	uint32_t AddNewPoint(const sf::Vector2f& coord, uint32_t id = UINT32_MAX);
	uint32_t AddNewConnection(const Connection& connection, uint32_t id = UINT32_MAX);

	void TrySelect(const sf::Vector2f pos, const ClickModifier mod = ClickModifier::Primary);

private:
	std::unordered_map<uint32_t, Point> m_points;
	std::unordered_map<uint32_t, Line> m_connections;

	std::vector<uint32_t> m_pointSelection;

	bool m_showPoints = true;
};
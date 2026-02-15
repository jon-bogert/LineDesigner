#pragma once

#include "ConnectionGraph.h"
#include "LineShape.h"
#include "Gizmo.h"

#include <SFML/Graphics.hpp>

#include <XephTools/CommandStack.h>

#include <filesystem>
#include <memory>
#include <unordered_map>
#include <vector>


class Canvas
{
	struct Point
	{
		sf::Vector2f coord;
		sf::CircleShape visual;
	};

	struct LineDrawContext
	{
		LineDrawContext() = default;
		LineDrawContext(Canvas* self, sf::RenderTarget* target) : self(self), target(target), index(0) {}

		Canvas* self = nullptr;
		sf::RenderTarget* target = nullptr;
		size_t index = 0;
	};

public:

	enum class ClickModifier
	{
		Primary,
		Secondary,
		Add,
	};

	void Initialize();
	void Update();
	void OnGUI();
	void DrawTo(sf::RenderTarget& target);

	void Load(const std::filesystem::path& path);

	uint32_t AddPoint(const sf::Vector2f& coord, uint32_t id = UINT32_MAX);
	void RemovePoint(uint32_t id);
	void AddConnection(const uint32_t idA, const uint32_t idB);
	void AddMultipleConnections(const uint32_t idA, const std::unordered_set<uint32_t>& destIDs);
	void RemoveConnection(const uint32_t idA, const uint32_t idB);

	void NewPointCommand(const sf::Vector2f coord);
	void RemovePointCommand(uint32_t id);
	void RemoveSelectedPointsCommand();

	void TrySelect(const sf::Vector2f pos, const ClickModifier mod = ClickModifier::Primary);
	void TryDelete();

	void TempExport();

private:
	void GUIPointPosition(uint32_t id);
	void GUIConnectionBool(uint32_t idA, uint32_t idB);

	static void DrawLineCallback(uint32_t idA, uint32_t idB, void* data);

	std::unordered_map<uint32_t, Point> m_points;
	ConnectionGraph m_connections;
	std::vector<LineShape> m_lineBuffer;

	std::vector<uint32_t> m_pointSelection;
	std::unique_ptr<xe::Command> m_inspectorCommand = nullptr;

	Gizmo m_gizmo;

	float m_lineWidth = 10.f;
	bool m_showPoints = true;
	bool m_useRelationSelect = false;
	sf::Color m_pointColorDefault = { 127, 127, 127 };
	sf::Color m_pointColorPrimary = sf::Color::Red;
	sf::Color m_pointColorSecondary = sf::Color::Yellow;
};
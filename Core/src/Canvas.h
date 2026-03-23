#pragma once

#include "ConnectionGraph.h"
#include "LineShape.h"
#include "ArcLineShape.h"
#include "Gizmo.h"

#include <SFML/Graphics.hpp>

#include <XephTools/CommandStack.h>

#include <filesystem>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#ifndef MIRROR_DEFS
#define MIRROR_DEFS 0xFF

#define MIRROR_VERT 0x01
#define MIRROR_HORIZ 0x02
#define MIRROR_POS_30 0x04
#define MIRROR_POS_45 0x08
#define MIRROR_POS_60 0x10
#define MIRROR_NEG_30 0x20
#define MIRROR_NEG_45 0x40
#define MIRROR_NEG_60 0x80

#endif // MIRROR_DEFS

struct Exporter;
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
		LineDrawContext(Canvas* self, sf::RenderTarget* target) : self(self), target(target), straightIndex(0) {}

		Canvas* self = nullptr;
		sf::RenderTarget* target = nullptr;
		size_t straightIndex = 0;
		size_t arcIndex = 0;
	};

public:

	enum class ClickModifier
	{
		Primary,
		Secondary,
		Add,
	};

	virtual ~Canvas();

	void Initialize();
	void Update();
	void OnInspectorGUI();
	void OnExportGUI();
	void DrawTo(sf::RenderTarget& target, bool linesOnly = false);

	bool Load(const std::filesystem::path& path);
	bool Save(const std::filesystem::path& path);

	uint32_t AddPoint(const sf::Vector2f& coord, uint32_t id = UINT32_MAX);
	void RemovePoint(uint32_t id);
	void AddConnection(const uint32_t idA, const uint32_t idB);
	void AddMultipleConnections(const uint32_t idA, const std::unordered_set<uint32_t>& destIDs);
	void RemoveConnection(const uint32_t idA, const uint32_t idB);

	void NewPointCommand(const sf::Vector2f coord);
	void RemoveSelectedPointsCommand();

	void TrySelect(const sf::Vector2f pos, const ClickModifier mod = ClickModifier::Primary);
	void TryDelete();

	void StartMoveSelection();
	void MoveSelection(const sf::Vector2f& moveAmount);
	void EndMoveSelection();

	Gizmo& GetGizmo() { return m_gizmo; }
	const Gizmo& GetGizmo() const { return m_gizmo; }
	const bool IsGizmoActive() const { return !m_pointSelection.empty(); }

	std::filesystem::path GetPath() const { return m_filepath; }
	void SetPath(const std::filesystem::path& path) { m_filepath = path; }

private:
	void GUIPointPosition(uint32_t id);
	void GUIConnectionBool(uint32_t idA, uint32_t idB);
	void GUILineThickness();
	void GUILineColor();
	void GUISetMirrors();
	void GUIConnectionType(uint32_t idA, uint32_t idB);
	void GUIArcRadius(uint32_t idA, uint32_t idB);
	void GUIArcSegments(uint32_t idA, uint32_t idB);

	void DrawGrid(sf::RenderTarget& target);
	void DrawOrigin(sf::RenderTarget& target);
	bool PrepMirrorLine(sf::RectangleShape& line, const sf::FloatRect& bounds, float scale, float angle);
	void DrawMirrorLines(sf::RenderTarget& target);

	static void DrawLineCallback(uint32_t idA, uint32_t idB, ConnectionInfo& info, void* data);
	static void DrawStraightLine(uint32_t idA, uint32_t idB, ConnectionInfo& info, LineDrawContext& ctx);
	static void DrawArcLine(uint32_t idA, uint32_t idB, ConnectionInfo& info, LineDrawContext& ctx);
	static void MirrorLine(std::vector<size_t>& lineBuffer, std::vector<size_t>& tempBuffer, LineDrawContext& ctx, const std::function<void(sf::Vector2f&)>& transformPoint);
	static void ReflectAcrossAngle(sf::Vector2f& v, float degrees);

	std::filesystem::path m_filepath;

	std::unordered_map<uint32_t, Point> m_points;
	ConnectionGraph m_connections;
	std::vector<LineShape> m_straightLineBuffer;
	std::vector<ArcLineShape> m_arcLineBuffer;

	std::vector<uint32_t> m_pointSelection;
	std::unique_ptr<xe::Command> m_inspectorCommand = nullptr;

	Exporter* m_exporter = nullptr;

	Gizmo m_gizmo;
	sf::RectangleShape m_xAxisLine;
	sf::RectangleShape m_yAxisLine;
	std::vector<sf::RectangleShape> m_gridLines;
	std::vector<sf::RectangleShape> m_mirrorLines;

	float m_lineWidth = 10.f;
	float m_unitSize = 100.f;
	bool m_showPoints = true;
	bool m_showOrigin = true;
	bool m_showGrid = true;
	bool m_showMirrorLines = false; // TODO -- to `true` when visual is implemented properly
	bool m_useRelationSelect = false;
	uint8_t m_mirrorMask = 0;
	sf::Color m_lineColor = sf::Color::White;
	sf::Color m_pointColorDefault = { 127, 127, 127 };
	sf::Color m_pointColorPrimary = sf::Color::Red;
	sf::Color m_pointColorSecondary = sf::Color::Yellow;
};
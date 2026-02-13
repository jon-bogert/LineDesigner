#include "Canvas.h"

#include "Algorithms.h"
#include "Message.h"

#include <yaml-cpp/yaml.h>
#include <imgui.h>

#include <fstream>

#define nullid UINT32_MAX

void Canvas::Update()
{
}

void Canvas::OnGUI()
{
	ImGui::Checkbox("Show Points", &m_showPoints);
	
	ImGui::Separator();

	if (!m_pointSelection.empty())
	{
		for (size_t i = 0; i < m_pointSelection.size(); ++i)
		{
			ImGui::DragFloat2("Position", &m_points[m_pointSelection[i]].coord.x);
		}
	}
}

void Canvas::DrawTo(sf::RenderTarget& m_target)
{
	for (auto& connectPair : m_connections)
	{
		Line line = connectPair.second;

		m_target.draw(line.visual);
	}

	if (m_showPoints)
	{
		for (auto& pointPair : m_points)
		{
			Point& point = pointPair.second;

			m_target.draw(point.visual);
		}
	}
}

void Canvas::Load(const std::filesystem::path& path)
{
	std::ifstream file(path);
	if (!file.is_open())
	{
		std::stringstream msg;
		msg << "Could not open file at path: " << path;
		Message::InfoNotice(msg);
		return;
	}

	YAML::Node root{};

	try
	{
		root = YAML::Load(file);
	}
	catch (std::exception)
	{
		std::stringstream msg;
		msg << path << " was not formatted properly and could not be read.";
		Message::ErrorNotice(msg);
		return;
	}

	if (root["points"].IsDefined())
	{
		for (const YAML::Node& point : root["points"])
		{
			uint32_t id = Algorithm::HexToUInt32(point["id"].as<std::string>());
			sf::Vector2f pointData;
			pointData.x = point["data"][0].as<float>();
			pointData.y = point["data"][1].as<float>();
			AddNewPoint(pointData, id);
		}
	}
	if (root["connections"].IsDefined())
	{
		for (const YAML::Node& connection : root["connections"])
		{
			uint32_t id = Algorithm::HexToUInt32(connection["id"].as<std::string>());
			Connection connectionData;
			connectionData.a = connection["data"][0].as<int>();
			connectionData.b = connection["data"][1].as<int>();
			AddNewConnection(connectionData, id);
		}
	}
}

uint32_t Canvas::AddNewPoint(const sf::Vector2f& coord, uint32_t id)
{
    id = (id == UINT32_MAX) ? Algorithm::RandUInt32() : id;

	Point& point = m_points[id];
	point.coord = coord;

	point.visual.setRadius(5.f);
	point.visual.setFillColor(sf::Color::Red);
	point.visual.setOrigin({ 5.f, 5.f });
	point.visual.setPosition(coord);

	return id;
}

uint32_t Canvas::AddNewConnection(const Connection& connection, uint32_t id)
{
	id = (id == nullid) ? Algorithm::RandUInt32() : id;

	Line& line = m_connections[id];
	line.connection = connection;
	line.PositionVisual(m_points[connection.a].coord, m_points[connection.b].coord);
	line.visual.setFillColor(sf::Color::White);

	return id;
}

void Canvas::TrySelect(const sf::Vector2f pos, const ClickModifier mod)
{
	uint32_t toSelect = nullid;
	for (auto& pointPair : m_points)
	{
		if (pointPair.second.visual.getGlobalBounds().contains(pos))
		{
			toSelect = pointPair.first;
		}
	}

	if (toSelect == nullid)
	{
		if (mod == ClickModifier::Primary)
		{
			m_pointSelection.clear();
		}
		return;
	}
		
	if (m_pointSelection.empty())
	{
		m_pointSelection.push_back(toSelect);
		return;
	}

	if (mod == ClickModifier::Primary)
	{
		m_pointSelection.clear();
		m_pointSelection.push_back(toSelect);
		return;
	}

	if (mod == ClickModifier::Secondary)
	{
		m_pointSelection.resize(2);
		m_pointSelection[1] = toSelect;
		return;
	}

	// Add
	auto iter = std::find(m_pointSelection.begin(), m_pointSelection.end(), toSelect);
	if (iter == m_pointSelection.end())
	{
		m_pointSelection.push_back(toSelect);
		return;
	}

	m_pointSelection.erase(iter);
}

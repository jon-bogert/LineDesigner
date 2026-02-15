#include "Canvas.h"

#include "App.h"
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
			GUIPointPosition(m_pointSelection[i]);
		}
	}
}

void Canvas::DrawTo(sf::RenderTarget& m_target)
{
	LineDrawContext ctx(this, &m_target);
	m_connections.ForEach(DrawLineCallback, (void*)&ctx);

	if (m_showPoints)
	{
		for (auto& pointPair : m_points)
		{
			Point& point = pointPair.second;
			point.visual.setPosition(point.coord);
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
			uint32_t idA = connection["data"][0].as<int>();
			uint32_t idB = connection["data"][1].as<int>();
			AddConnection(idA, idB);
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

void Canvas::AddConnection(const uint32_t idA, const uint32_t idB)
{
	m_connections.CreateConnection(idA, idB);
}

void Canvas::NewPointCommand(const sf::Vector2f coord)
{
	uint32_t id = Algorithm::RandUInt32();
	xe::Command cmd;
	Message::DebugLog("TODO - New Point");
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

void Canvas::GUIPointPosition(uint32_t id)
{
	std::string label = "Position##" + std::to_string(id);
	Point& point = m_points[id];
	sf::Vector2f val = point.coord;
		ImGui::DragFloat2(label.c_str(), &point.coord.x);
	if (ImGui::IsItemClicked())
	{
		m_inspectorCommand = std::make_unique<xe::Command>();
		m_inspectorCommand->revert = [this, id, val]() {m_points[id].coord = val; };
	}
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		if (m_inspectorCommand == nullptr)
		{
			Message::ErrorNotice("Inspector command buffer instance not valid.");
			return;
		}
		val = point.coord;
		m_inspectorCommand->execute = [this, id, val]() { m_points[id].coord = val; };
		App::Do(*m_inspectorCommand);
		m_inspectorCommand = nullptr;
	}
	else if (ImGui::IsItemDeactivated())
	{
		m_inspectorCommand = nullptr;
	}
}

void Canvas::DrawLineCallback(uint32_t idA, uint32_t idB, void* data)
{
	LineDrawContext& ctx = *(LineDrawContext*)data;
	Canvas& self = *ctx.self;
	LineShape* currLine = (ctx.index == self.m_lineBuffer.size()) ? currLine = &self.m_lineBuffer.emplace_back() : &self.m_lineBuffer[ctx.index];

	currLine->SetParameters(self.m_points[idA].coord, self.m_points[idB].coord, self.m_lineWidth, 10);
	currLine->setFillColor(sf::Color::White);

	ctx.target->draw(*currLine);
	ctx.index += 1;
}

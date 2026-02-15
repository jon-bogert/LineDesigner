#include "Canvas.h"

#include "App.h"
#include "Algorithms.h"
#include "Message.h"

#include <yaml-cpp/yaml.h>
#include <imgui.h>
#include <stb_image_write.h>

#include <fstream>

#define nullid UINT32_MAX

void Canvas::Initialize()
{
	m_gizmo.Initialize();
}

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
		if (m_pointSelection.size() == 2)
		{
			GUIConnectionBool(m_pointSelection[0], m_pointSelection[1]);
		}
	}
}

void Canvas::DrawTo(sf::RenderTarget& target)
{
	LineDrawContext ctx(this, &target);
	m_connections.ForEach(DrawLineCallback, (void*)&ctx);

	if (m_showPoints)
	{
		for (auto& pointPair : m_points)
		{
			Point& point = pointPair.second;
			point.visual.setFillColor(m_pointColorDefault);
			point.visual.setPosition(point.coord);
		}

		for (uint32_t id : m_pointSelection)
		{
			m_points[id].visual.setFillColor((id == m_pointSelection[0]) ? m_pointColorPrimary : m_pointColorSecondary);
		}

		for (auto& pointPair : m_points)
		{
			target.draw(pointPair.second.visual);
		}
	}

	if (m_pointSelection.size() == 1 || (m_pointSelection.size() == 2 && m_useRelationSelect))
	{
		m_gizmo.DrawTo(target, m_points[m_pointSelection.back()].coord);
	}
	else
	{
		sf::Vector2f average{};
		for (size_t i = 0; i < m_pointSelection.size(); ++i)
		{
			average += m_points[m_pointSelection[i]].coord;
		}

		average /= (float)m_pointSelection.size();
		m_gizmo.DrawTo(target, average);
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
			AddPoint(pointData, id);
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

uint32_t Canvas::AddPoint(const sf::Vector2f& coord, uint32_t id)
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

void Canvas::RemovePoint(uint32_t id)
{
	if (m_connections.HasID(id))
	{
		ConnectionGraph::Node& node = m_connections[id];
		for (const uint32_t dest : node.connections)
		{
			m_connections.RemoveConnection(id, dest);
			if (!m_connections.HasID(id))
				break;
		}
	}

	m_points.erase(id);
}

void Canvas::AddConnection(const uint32_t idA, const uint32_t idB)
{
	m_connections.CreateConnection(idA, idB);
}

void Canvas::AddMultipleConnections(const uint32_t idA, const std::unordered_set<uint32_t>& destIDs)
{
	for (const uint32_t dest : destIDs)
	{
		m_connections.CreateConnection(idA, dest);
	}
}

void Canvas::RemoveConnection(const uint32_t idA, const uint32_t idB)
{
	m_connections.RemoveConnection(idA, idB);
}

void Canvas::NewPointCommand(const sf::Vector2f coord)
{
	uint32_t id = Algorithm::RandUInt32();
	xe::Command cmd;

	cmd.revert = [&, id]() { RemovePoint(id); };
	cmd.execute = [&, coord, id]() { AddPoint(coord, id); };

	App::Exec(cmd);

	m_pointSelection.resize(1);
	m_pointSelection[0] = id;
}

void Canvas::RemovePointCommand(uint32_t id)
{
	xe::Command cmd;
	sf::Vector2f coord = m_points[id].coord;
	if (m_connections.HasID(id))
	{
		std::unordered_set<uint32_t>& connections = m_connections[id].connections;
		cmd.revert = [this, connections, id, coord]
			{
				AddPoint(coord, id);
				AddMultipleConnections(id, connections);
			};
	}
	else
	{
		cmd.revert = [this, id, coord] { AddPoint(coord, id); };
	}
	cmd.execute = [&, id]() { RemovePoint(id); };

	App::Exec(cmd);
}

void Canvas::RemoveSelectedPointsCommand()
{
	xe::Command cmd;
	ConnectionGraph& connections = m_connections;
	std::vector<uint32_t>& selection = m_pointSelection;
	std::vector<sf::Vector2f> coords(selection.size());
	for (size_t i = 0; i < selection.size(); ++i)
	{
		coords[i] = m_points[selection[i]].coord;
	}

	cmd.revert = [this, connections, selection, coords]()
		{
			for (size_t i = 0; i < selection.size(); ++i)
			{
				AddPoint(coords[i], selection[i]);
			}
			m_connections = connections;
		};

	cmd.execute = [this, selection]()
		{
			for (const uint32_t id : selection)
			{
				RemovePoint(id);
			}
		};

	App::Exec(cmd);
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
		m_useRelationSelect = true;
		return;
	}

	// Add
	m_useRelationSelect = false;
	auto iter = std::find(m_pointSelection.begin(), m_pointSelection.end(), toSelect);
	if (iter == m_pointSelection.end())
	{
		m_pointSelection.push_back(toSelect);
		return;
	}

	m_pointSelection.erase(iter);
}

void Canvas::TryDelete()
{
	if (m_pointSelection.empty())
		return;

	if (m_pointSelection.size() == 1)
	{
		RemovePointCommand(m_pointSelection[0]);
		return;
	}

	RemoveSelectedPointsCommand();
}

void Canvas::TempExport()
{
	sf::RenderTexture tex;
	sf::ContextSettings winCtx;
	winCtx.antialiasingLevel = 8;
	tex.create(512, 512, winCtx);
	sf::View view = tex.getView();
	view.setCenter({ 0, 0 });
	tex.setView(view);

	tex.clear({ 0,0,0,0 });
	LineDrawContext ctx(this, &tex);
	m_connections.ForEach(DrawLineCallback, (void*)&ctx);
	tex.display();

	sf::Image img = tex.getTexture().copyToImage();
	stbi_write_png("arrow.png", 512, 512, 4, img.getPixelsPtr(), 512 * 4);
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
		App::Exec(*m_inspectorCommand);
		m_inspectorCommand = nullptr;
	}
	else if (ImGui::IsItemDeactivated())
	{
		m_inspectorCommand = nullptr;
	}
}

void Canvas::GUIConnectionBool(uint32_t idA, uint32_t idB)
{
	bool hasConnection = m_connections.HasConnection(idA, idB);
	if (!ImGui::Checkbox("Points Connected", &hasConnection))
		return;

	xe::Command cmd;
	if (hasConnection)
	{
		cmd.revert = [this, idA, idB]() { RemoveConnection(idA, idB); };
		cmd.execute = [this, idA, idB]() { AddConnection(idA, idB); };
	}
	else
	{
		cmd.revert = [this, idA, idB]() { AddConnection(idA, idB); };
		cmd.execute = [this, idA, idB]() { RemoveConnection(idA, idB); };
	}

	App::Exec(cmd);
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

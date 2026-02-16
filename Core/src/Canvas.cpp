#include "Canvas.h"

#include "App.h"
#include "Algorithms.h"
#include "Message.h"
#include "Mathematics.h"

#include <yaml-cpp/yaml.h>
#include <imgui.h>
#include <stb_image_write.h>

#include <fstream>

#define nullid UINT32_MAX

void Canvas::Initialize()
{
	m_gizmo.Initialize();
	m_mirrorLines.resize(8);
}

void Canvas::Update()
{

}

void Canvas::OnGUI()
{
	if (ImGui::CollapsingHeader("View", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Checkbox("Show Points", &m_showPoints);
		ImGui::Checkbox("Show Origin", &m_showOrigin);
		ImGui::Checkbox("Show Grid", &m_showGrid);
		ImGui::DragFloat("Grid Size", &m_unitSize);
		//ImGui::Checkbox("Show Mirror Lines", &m_showMirrorLines);
	}
	
	ImGui::NewLine();
	if (ImGui::CollapsingHeader("Global", ImGuiTreeNodeFlags_DefaultOpen))
	{
		GUILineThickness();
		GUILineColor();
	}

	if (!m_pointSelection.empty())
	{
		ImGui::NewLine();
		if (ImGui::CollapsingHeader("Selected", ImGuiTreeNodeFlags_DefaultOpen))
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
}

void Canvas::DrawTo(sf::RenderTarget& target)
{
	if (m_showGrid)
	{
		DrawGrid(target);
	}
	if (m_showOrigin)
	{
		DrawOrigin(target);
	}
	if (m_showMirrorLines)
	{
		DrawMirrorLines(target);
	}

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
			if (!m_connections.HasID(id)) // `node.connections` may be destroyed, prevent read
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

void Canvas::GUILineThickness()
{
	float val = m_lineWidth;
	ImGui::DragFloat("Line Thickness", &m_lineWidth, 1.f, 0.f);
	if (ImGui::IsItemClicked())
	{
		m_inspectorCommand = std::make_unique<xe::Command>();
		m_inspectorCommand->revert = [this, val]() { m_lineWidth = val; };
	}
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		if (m_inspectorCommand == nullptr)
		{
			Message::ErrorNotice("Inspector command buffer instance not valid.");
			return;
		}
		val = m_lineWidth;
		m_inspectorCommand->execute = [this, val]() { m_lineWidth = val; };
		App::Exec(*m_inspectorCommand);
		m_inspectorCommand = nullptr;
	}
	else if (ImGui::IsItemDeactivated())
	{
		m_inspectorCommand = nullptr;
	}
}

void Canvas::GUILineColor()
{
	sf::Color c8 = m_lineColor;
	float cf[] = { c8.r / 255.f, c8.g / 255.f , c8.b / 255.f , c8.a / 255.f };
	if (ImGui::ColorEdit4("Line Color", cf))
	{
		m_lineColor = sf::Color(cf[0] * 255, cf[1] * 255, cf[2] * 255 , cf[3] * 255);
	}
	if (ImGui::IsItemActivated())
	{
		m_inspectorCommand = std::make_unique<xe::Command>();
		m_inspectorCommand->revert = [this, c8]() { m_lineColor = c8; };
	}
	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		if (m_inspectorCommand == nullptr)
		{
			Message::ErrorNotice("Inspector command buffer instance not valid.");
			return;
		}
		c8 = m_lineColor;
		m_inspectorCommand->execute = [this, c8]() { m_lineColor = c8; };
		App::Exec(*m_inspectorCommand);
		m_inspectorCommand = nullptr;
	}
	else if (ImGui::IsItemDeactivated())
	{
		m_inspectorCommand = nullptr;
	}
}

void Canvas::DrawGrid(sf::RenderTarget& target)
{
	sf::View currView = target.getView();
	sf::View defaultView = target.getDefaultView();
	float scale = currView.getSize().y / defaultView.getSize().y;
	size_t counter = 0;

	sf::FloatRect bounds;
	bounds.width = currView.getSize().x;
	bounds.height = currView.getSize().y;
	bounds.left = currView.getCenter().x - bounds.width * 0.5f;
	bounds.top = currView.getCenter().y - bounds.height * 0.5f;

	// X Lines
	float cursor = ((int)(bounds.left / m_unitSize)) * m_unitSize;

	while (cursor <= bounds.left + bounds.width)
	{
		sf::RectangleShape* line = (counter == m_gridLines.size()) ? &m_gridLines.emplace_back() : &m_gridLines[counter];
		
		line->setFillColor({ 25, 25, 25 });
		line->setSize({ 2.f * scale, currView.getSize().y });
		line->setOrigin(line->getSize() * 0.5f);
		line->setPosition({ cursor, currView.getCenter().y });

		target.draw(*line);
		
		++counter;
		cursor += m_unitSize;
	}

	// Y Lines
	cursor = ((int)(bounds.top / m_unitSize)) * m_unitSize;

	while (cursor <= bounds.top + bounds.height)
	{
		sf::RectangleShape* line = (counter == m_gridLines.size()) ? &m_gridLines.emplace_back() : &m_gridLines[counter];

		line->setFillColor({ 25, 25, 25 });
		line->setSize({ currView.getSize().x, 2.f * scale });
		line->setOrigin(line->getSize() * 0.5f);
		line->setPosition({ currView.getCenter().x, cursor });

		target.draw(*line);

		++counter;
		cursor += m_unitSize;
	}
}

void Canvas::DrawOrigin(sf::RenderTarget& target)
{
	sf::View currView = target.getView();
	sf::View defaultView = target.getDefaultView();
	float scale = currView.getSize().y / defaultView.getSize().y;

	
	m_xAxisLine.setFillColor({ 50, 50, 50 });
	m_xAxisLine.setSize({ currView.getSize().x, 2.f * scale });
	m_xAxisLine.setOrigin(m_xAxisLine.getSize() * 0.5f);
	m_xAxisLine.setPosition({ currView.getCenter().x, 0.f });

	m_yAxisLine.setFillColor({ 50, 50, 50 });
	m_yAxisLine.setSize({ 2.f * scale, currView.getSize().y });
	m_yAxisLine.setOrigin(m_yAxisLine.getSize() * 0.5f);
	m_yAxisLine.setPosition({ 0.f, currView.getCenter().y });

	target.draw(m_xAxisLine);
	target.draw(m_yAxisLine);
}

bool Canvas::PrepMirrorLine(sf::RectangleShape& line, const sf::FloatRect& bounds, float scale, float angle)
{
	float radians = angle * (3.14159265359f / 180.f);
	sf::Vector2f start = {};// calculate top of line
	sf::Vector2f end = {};// calculate bottom of line
	bool success = false;

	sf::Vector2f dir(std::cos(radians), std::sin(radians));
	float slope = dir.y / dir.x;
	if (angle >= 0)
	{
		float y = slope * bounds.left;
		if (y >= bounds.top && y < bounds.top + bounds.height)
		{
			start = { bounds.left, y };
		}
		else
		{
			float x = bounds.left / slope;
			if (x >= bounds.left && x < bounds.left + bounds.width)
			{
				start = { x, bounds.top };
			}
			else
			{
				return false;
			}
		}

		y = slope * (bounds.left + bounds.width);
		if (y >= bounds.top && y < bounds.top + bounds.height)
		{
			end = { bounds.left + bounds.width, y };
		}
		else
		{
			float x = (bounds.top + bounds.height) / slope;
			if (x >= bounds.left && x < bounds.left + bounds.width)
			{
				end = { x, bounds.top + bounds.height };
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}


	sf::Vector2f position = ((end - start) * 0.5f) + start;
	float length = xe::Math::Distance(start, end);

	line.setFillColor({ 0, 255, 255 });
	line.setSize({ 2.f * scale, length });
	line.setOrigin(line.getSize() * 0.5f);
	line.setPosition(position);
	line.setRotation(angle);

	return true;
}

void Canvas::DrawMirrorLines(sf::RenderTarget& target)
{
	sf::View currView = target.getView();
	sf::View defaultView = target.getDefaultView();
	float scale = currView.getSize().y / defaultView.getSize().y;

	sf::FloatRect bounds;
	bounds.width = currView.getSize().x;
	bounds.height = currView.getSize().y;
	bounds.left = currView.getCenter().x - bounds.width * 0.5f;
	bounds.top = currView.getCenter().y - bounds.height * 0.5f;

	if (m_mirrorMask & MIRROR_VERT)
	{
		if (PrepMirrorLine(m_mirrorLines[0], bounds, scale, 0.f))
			target.draw(m_mirrorLines[0]);
	}
	if (m_mirrorMask & MIRROR_HORIZ)
	{
		if (PrepMirrorLine(m_mirrorLines[1], bounds, scale, 90.f))
			target.draw(m_mirrorLines[1]);
	}
	if (m_mirrorMask & MIRROR_POS_30)
	{
		if (PrepMirrorLine(m_mirrorLines[2], bounds, scale, 30.f))
			target.draw(m_mirrorLines[2]);
	}
	if (m_mirrorMask & MIRROR_POS_45)
	{
		if (PrepMirrorLine(m_mirrorLines[3], bounds, scale, 45.f))
			target.draw(m_mirrorLines[3]);
	}
	if (m_mirrorMask & MIRROR_POS_60)
	{
		if (PrepMirrorLine(m_mirrorLines[4], bounds, scale, 60.f))
			target.draw(m_mirrorLines[4]);
	}
	if (m_mirrorMask & MIRROR_NEG_30)
	{
		if (PrepMirrorLine(m_mirrorLines[5], bounds, scale, -30.f))
			target.draw(m_mirrorLines[5]);
	}
	if (m_mirrorMask & MIRROR_NEG_45)
	{
		if (PrepMirrorLine(m_mirrorLines[6], bounds, scale, -45.f))
			target.draw(m_mirrorLines[6]);
	}
	if (m_mirrorMask & MIRROR_NEG_60)
	{
		if (PrepMirrorLine(m_mirrorLines[7], bounds, scale, -60.f))
			target.draw(m_mirrorLines[7]);
	}

}

void Canvas::DrawLineCallback(uint32_t idA, uint32_t idB, void* data)
{
	LineDrawContext& ctx = *(LineDrawContext*)data;
	Canvas& self = *ctx.self;

	std::vector<size_t> lineBuffer;
	LineShape* currLine = (ctx.index == self.m_lineBuffer.size()) ? &self.m_lineBuffer.emplace_back() : &self.m_lineBuffer[ctx.index];
	lineBuffer.push_back(ctx.index);
	ctx.index += 1;

	currLine->SetParameters(self.m_points[idA].coord, self.m_points[idB].coord, self.m_lineWidth, 10);

	std::vector<size_t> tempBuffer;

	if (self.m_mirrorMask & MIRROR_VERT)
	{
		MirrorLine(lineBuffer, tempBuffer, ctx, [](sf::Vector2f& point)
			{
				point.x *= -1.f;
			});
	}
	if (self.m_mirrorMask & MIRROR_HORIZ)
	{
		MirrorLine(lineBuffer, tempBuffer, ctx, [](sf::Vector2f& point)
			{
				point.y *= -1.f;
			});
	}
	if (self.m_mirrorMask & MIRROR_POS_30)
	{
		MirrorLine(lineBuffer, tempBuffer, ctx, [](sf::Vector2f& point) { ReflectAcrossAngle(point, 30.f); });
	}
	if (self.m_mirrorMask & MIRROR_POS_45)
	{
		MirrorLine(lineBuffer, tempBuffer, ctx, [](sf::Vector2f& point) { ReflectAcrossAngle(point, 45.f); });
	}
	if (self.m_mirrorMask & MIRROR_POS_60)
	{
		MirrorLine(lineBuffer, tempBuffer, ctx, [](sf::Vector2f& point) { ReflectAcrossAngle(point, 60.f); });
	}
	if (self.m_mirrorMask & MIRROR_NEG_30)
	{
		MirrorLine(lineBuffer, tempBuffer, ctx, [](sf::Vector2f& point) { ReflectAcrossAngle(point, -30.f); });
	}
	if (self.m_mirrorMask & MIRROR_NEG_45)
	{
		MirrorLine(lineBuffer, tempBuffer, ctx, [](sf::Vector2f& point) { ReflectAcrossAngle(point, -45.f); });
	}
	if (self.m_mirrorMask & MIRROR_NEG_60)
	{
		MirrorLine(lineBuffer, tempBuffer, ctx, [](sf::Vector2f& point) { ReflectAcrossAngle(point, -60.f); });
	}

	for (size_t i : lineBuffer)
	{
        self.m_lineBuffer[i].setFillColor(self.m_lineColor);
	    ctx.target->draw(self.m_lineBuffer[i]);
	}
}

void Canvas::MirrorLine(std::vector<size_t>& lineBuffer, std::vector<size_t>& tempBuffer, LineDrawContext& ctx, const std::function<void(sf::Vector2f&)>& transformPoint)
{
	for (size_t i : lineBuffer)
	{
		LineShape* currLine = (ctx.index == ctx.self->m_lineBuffer.size()) ? &ctx.self->m_lineBuffer.emplace_back() : &ctx.self->m_lineBuffer[ctx.index];
		sf::Vector2f pointA = ctx.self->m_lineBuffer[i].GetStart();
		sf::Vector2f pointB = ctx.self->m_lineBuffer[i].GetEnd();

		transformPoint(pointA);
		transformPoint(pointB);

		currLine->SetParameters(pointA, pointB, ctx.self->m_lineWidth, 10);
		tempBuffer.push_back(ctx.index);
		ctx.index += 1;
	}
	lineBuffer.insert(lineBuffer.end(), std::move_iterator(tempBuffer.begin()), std::move_iterator(tempBuffer.end()));
	tempBuffer.clear();
}

void Canvas::ReflectAcrossAngle(sf::Vector2f& v, float degrees)
{
	float radians = degrees * (3.14159265359f / 180.f);
	sf::Vector2f u(std::cos(radians), std::sin(radians)); // unit direction

	float dot = v.x * u.x + v.y * u.y;

	v = sf::Vector2f( 2.f * dot * u.x - v.x, 2.f * dot * u.y - v.y );
}

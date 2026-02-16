#pragma once

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <fstream>
#include <vector>

#ifndef MAX_RECENT
#define MAX_RECENT 25;
#endif // MAX_RECENT

struct AppPrefs
{
	std::filesystem::path lastSave;
	std::string fontName = "segoeui.ttf";
	int fontSize = 20;
	uint32_t windowWidth = 1280;
	uint32_t windowHeight = 720;

	void Load()
	{
		std::ifstream file("prefs.yaml");
		if (!file.is_open())
			return;

		YAML::Node root = YAML::Load(file);

		if (root["last-save"].IsDefined())
		{
			lastSave = root["last-save"].as<std::string>();
		}
		if (root["font-name"].IsDefined())
		{
			std::filesystem::path path = "C:\\Windows\\Fonts";
			if (std::filesystem::exists(path / root["font-name"].as<std::string>()))
			{
				fontName = root["font-name"].as<std::string>();
			}
		}
		if (root["font-size"].IsDefined())
		{
			fontSize = root["font-size"].as<int>();
		}
		if (root["window-size"].IsDefined())
		{
			windowWidth = root["window-size"][0].as<uint32_t>();
			windowHeight = root["window-size"][1].as<uint32_t>();
		}
	}
	void Save()
	{
		YAML::Node root;
		root["last-save"] = lastSave.string();
		root["font-name"] = fontName;
		root["font-size"] = fontSize;
		root["window-size"].push_back(windowWidth);
		root["window-size"].push_back(windowHeight);
		root["window-size"].SetStyle(YAML::EmitterStyle::Flow);

		std::ofstream file("prefs.yaml");
		if (!file.is_open())
		{
			return;
		}

		file << root;
	}
};
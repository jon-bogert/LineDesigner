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
};
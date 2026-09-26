#pragma once

#include <string>
#include <map>
#include <filesystem>
#include <vector>

namespace core::yaml
{
	std::map<std::string, std::vector<std::string>> ReadFile(const std::filesystem::path& path);
}
#include "SessionHelpers.h"

#include "core/helpers/YamlHelpers.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace
{
	constexpr const char* c_ConfigPath = "config.yaml";

	std::optional<int> ToInt(const std::string& str)
	{
		int value;
		const auto [ptr, ec] = std::from_chars(
			str.data(),
			str.data() + str.size(),
			value
		);

		if (ec != std::errc{} || ptr != str.data() + str.size())
			return std::nullopt;

		return value;
	}

	std::optional<float> ToFloat(const std::string& str)
	{
		float value;
		const auto [ptr, ec] = std::from_chars(
			str.data(),
			str.data() + str.size(),
			value
		);

		if (ec != std::errc{} || ptr != str.data() + str.size())
			return std::nullopt;

		return value;
	}

	const std::vector<std::string>* TryGet(
		const std::map<std::string, std::vector<std::string>>& map,
		const std::string& key)
	{
		auto it = map.find(key);
		return it == map.end() || it->second.empty()
			? nullptr
			: &it->second;
	}
}

ttfe::session::DataComponent ttfe::session::GetSessionDataFromConfig()
{
	ttfe::session::DataComponent sessionData;
	std::filesystem::path path{ c_ConfigPath };

	if (!std::filesystem::exists(path))
		return sessionData;

	try
	{
		auto configMap = core::yaml::ReadFile(path);
		if (const auto* value = TryGet(configMap, "BoardRows"))
		{
			if(auto i = ToInt(value->at(0)))
				sessionData.m_BoardRows = *i;
		}
		if (const auto* value = TryGet(configMap, "BoardColumns"))
		{
			if (auto i = ToInt(value->at(0)))
				sessionData.m_BoardColumns = *i;
		}
		if (const auto* value = TryGet(configMap, "BoardTilesSize"))
		{
			if (auto f = ToFloat(value->at(0)))
				sessionData.m_BoardTilesSize = *f;
		}
		if (const auto* value = TryGet(configMap, "BoardTilesPadding"))
		{
			if (auto f = ToFloat(value->at(0)))
				sessionData.m_BoardTilesPadding = *f;
		}
		if (const auto* value = TryGet(configMap, "MoveSpeed"))
		{
			if (auto f = ToFloat(value->at(0)))
				sessionData.m_MoveSpeed = *f;
		}
		if (const auto* values = TryGet(configMap, "TileNumberBag"))
		{
			bool hasConfig = false;
			for (const auto& value : *values)
			{
				if (auto i = ToInt(value); i && *i >= 2)
				{
					if (!hasConfig)
					{
						hasConfig = true;
						sessionData.m_TileNumberBag.clear();
					}
					sessionData.m_TileNumberBag.push_back(*i);
				}
			}
		}
		if (const auto* values = TryGet(configMap, "TileColor"))
		{
			if (values->size() >= 3)
				if (auto r = ToInt(values->at(0)); r && *r >= 0 && *r <= 255)
					if (auto g = ToInt(values->at(1)); g && *g >= 0 && *g <= 255)
						if (auto b = ToInt(values->at(2)); b && *b >= 0 && *b <= 255)
							sessionData.m_TileColor = {
								static_cast<unsigned char>(*r),
								static_cast<unsigned char>(*g),
								static_cast<unsigned char>(*b),
								255 };
		}
		if (const auto* values = TryGet(configMap, "BackgroundTileColor"))
		{
			if (values->size() >= 3)
				if (auto r = ToInt(values->at(0)); r && *r >= 0 && *r <= 255)
					if (auto g = ToInt(values->at(1)); g && *g >= 0 && *g <= 255)
						if (auto b = ToInt(values->at(2)); b && *b >= 0 && *b <= 255)
							sessionData.m_BackgroundTileColor = {
								static_cast<unsigned char>(*r),
								static_cast<unsigned char>(*g),
								static_cast<unsigned char>(*b),
								255 };
		}
	}
	catch(std::runtime_error error)
	{
		std::cerr << "Error while reading " << path << " : " << error.what() << "\n";
	}

	return sessionData;	
}

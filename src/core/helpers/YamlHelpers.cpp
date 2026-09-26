#include "YamlHelpers.h"

#include <fstream>
#include <stdexcept>

namespace
{
    void Trim(std::string& str)
    {
        const auto begin = str.find_first_not_of(" \t");
        const auto end = str.find_last_not_of(" \t");
        if (begin == std::string::npos)
        {
            str.clear();
            return;
        }
        str = str.substr(begin, end - begin + 1);
    }
}

std::map<std::string, std::vector<std::string>> core::yaml::ReadFile(const std::filesystem::path& path)
{
    std::ifstream file(path);

    if (!file)
        throw std::runtime_error("Could not open file: " + path.string());

    std::map<std::string, std::vector<std::string>> map;

    std::string line;
    std::string currentKey;


    while (std::getline(file, line))
    {
        Trim(line);

        if (line.empty() || line.starts_with('#'))
            continue;

        if (line.starts_with("-"))
        {
            if (currentKey.empty())
                throw std::runtime_error("List item without a key: " + line);

            std::string value = line.substr(1);
            Trim(value);

            map.at(currentKey).push_back(value);
            continue;
        }

        const auto separator = line.find(':');

        if (separator == std::string::npos)
            throw std::runtime_error(
                "Invalid yaml line: " + line);

        std::string key = line.substr(0, separator);
        std::string value = line.substr(separator + 1);

        Trim(key);
        Trim(value);

        if (key.empty())
            throw std::runtime_error("Empty yaml key");

        currentKey = key;

        if (!value.empty())
        {
            map[key] = { value };
        }
        else
        {
            map[key] = {};
        }
    }

    return map;
}

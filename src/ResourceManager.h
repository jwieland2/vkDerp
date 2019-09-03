#pragma once
#include <vector>
class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    static std::vector<char> readFile(const std::string& filename);
};


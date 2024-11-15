#pragma once
#include <filesystem>
#include <rapidjson/document.h>

using jsonDocument = rapidjson::Document;
using jsonValue = rapidjson::Value;

using namespace rapidjson;

int GetFileCount(const std::string& directoryPath);
std::vector<std::filesystem::path> GetRandomJsonFiles(const std::string& directoryPath, int maxFiles);
std::vector<std::filesystem::path> GetJsonFiles(const std::string& directoryPath, int maxFiles);
jsonDocument ReadJsonFromFile(const std::string path);
bool WriteJsonToFile(const std::string path, jsonDocument data);
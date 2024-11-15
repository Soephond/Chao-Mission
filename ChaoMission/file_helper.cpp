#include "pch.h"
#include "file_helper.h"

#include <fstream>
#include <iostream>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <random>

int GetFileCount(const std::string& directoryPath) {
    namespace fs = std::filesystem;
    int fileCount = 0;

    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (fs::is_regular_file(entry.status())) {
            ++fileCount;
        }
    }

    return fileCount;
}

std::vector<std::filesystem::path> GetRandomJsonFiles(const std::string& directoryPath, int maxFiles) {
    auto jsonFiles = GetJsonFiles(directoryPath, maxFiles);
    
    // Shuffle the list of files
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(jsonFiles.begin(), jsonFiles.end(), g);

    return jsonFiles;
}

std::vector<std::filesystem::path> GetJsonFiles(const std::string& directoryPath, int maxFiles) {
    namespace fs = std::filesystem;
    std::vector<fs::path> jsonFiles;

    // Collect all .json files in the directory
    for (const auto& entry : fs::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".json") {
            jsonFiles.push_back(entry.path());
        }
    }

    // Select a random amount of files
    if ((int)jsonFiles.size() > maxFiles) {
        jsonFiles.resize(maxFiles);
    }

    return jsonFiles;
}

jsonDocument ReadJsonFromFile(const std::string path)
{
    std::ifstream file(path);

    jsonDocument jsonData;

    if (file.is_open())
    {
        IStreamWrapper wrapper(file);
        jsonData.ParseStream(wrapper);
        file.close();
    }
    else
    {
        std::cout << "Unable to open file " << path << std::endl;
        return NULL;
    }

    return jsonData;
}

bool WriteJsonToFile(const std::string path, jsonDocument data)
{
	std::ofstream file(path);

	if (file.is_open()) {
		OStreamWrapper wrapper(file);
		Writer writer(wrapper);
		data.Accept(writer);
		file.close();
	}
	else {
		std::cout << "Unable to open file " << path << std::endl;
		return false;
	
	}

	return true;
}
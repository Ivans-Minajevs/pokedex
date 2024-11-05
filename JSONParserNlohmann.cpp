#include "JSONParserNlohmann.h"
#include <fstream>
#include <memory>
#include <sstream>

//Custom deleter (PROG 3)
void close_file(std::FILE* file) {
    if (file) {
        std::fclose(file);
    }
}

bool JSONParserNlohmann::LoadFromFile(const std::string& filePath) {
    using unique_file_t = std::unique_ptr<std::FILE, decltype(&close_file)>;
    unique_file_t file(std::fopen(filePath.c_str(), "r"), close_file);

    if (!file) {
        m_LastError = "Error: Could not open file at " + filePath;
        return false;
    }

    std::stringstream buffer;
    buffer << std::ifstream(filePath).rdbuf(); 

    try {
        m_JSONData = nlohmann::json::parse(buffer.str()); 
        m_LastError.clear(); 
    } catch (const std::exception& e) {
        m_LastError = "Error: Failed to parse JSON - " + std::string(e.what());
        return false;
    }
    return true;
}

std::vector<std::unordered_map<std::string, JSONValue>> JSONParserNlohmann::GetArrayOfObjects(const std::string& key) const {
    std::vector<std::unordered_map<std::string, JSONValue>> result;

    if (m_JSONData.contains(key) && m_JSONData[key].is_array()) {
        for (const auto& item : m_JSONData[key]) {
            std::unordered_map<std::string, JSONValue> itemMap;

            for (auto it = item.begin(); it != item.end(); ++it) {
                if (it.value().is_string()) {
                    itemMap[it.key()] = it.value().get<std::string>();
                } else if (it.value().is_number_integer()) {
                    itemMap[it.key()] = it.value().get<int>();
                } else if (it.value().is_number_float()) {
                    itemMap[it.key()] = it.value().get<double>();
                } else if (it.value().is_array()) {
                    std::vector<std::string> arrayValues;
                    for (const auto& val : it.value()) {
                        if (val.is_string()) {
                            arrayValues.push_back(val.get<std::string>());
                        }
                    }
                    itemMap[it.key()] = arrayValues;
                }
            }
            result.push_back(itemMap);
        }
    }
    return result;
}

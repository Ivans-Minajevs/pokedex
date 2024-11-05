#include "JSONParserRapid.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <memory>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

//Custom deleter (PROG 3)
void close_file(std::FILE* file) {
    if (file) {
        std::fclose(file);
    }
}

bool JSONParserRapid::LoadFromFile(const std::string& filePath) {
    using unique_file_t = std::unique_ptr<std::FILE, decltype(&close_file)>;
    unique_file_t file(std::fopen(filePath.c_str(), "r"), close_file);

    if (!file) {
        m_LastError = "Error: Could not open file at " + filePath;
        return false;
    }

    std::stringstream buffer;
    buffer << std::ifstream(filePath).rdbuf(); 

    m_JSONData.Parse(buffer.str().c_str());

    if (m_JSONData.HasParseError()) {
        m_LastError = "Error: Failed to parse JSON - " +
                    std::string(rapidjson::GetParseError_En(m_JSONData.GetParseError()));
        return false;
    }

    m_LastError.clear();
    return true;
}

std::vector<std::unordered_map<std::string, JSONValue>> JSONParserRapid::GetArrayOfObjects(const std::string& key) const {
    std::vector<std::unordered_map<std::string, JSONValue>> result;

    if (m_JSONData.HasMember(key.c_str()) && m_JSONData[key.c_str()].IsArray()) {
        const auto& jsonArray = m_JSONData[key.c_str()].GetArray();

        for (const auto& item : jsonArray) {
            std::unordered_map<std::string, JSONValue> itemMap;

            if (item.IsObject()) {
                for (auto it = item.MemberBegin(); it != item.MemberEnd(); ++it) {
                    std::string itemKey = it->name.GetString();

                    if (it->value.IsString()) {
                        itemMap[itemKey] = it->value.GetString();
                    } else if (it->value.IsInt()) {
                        itemMap[itemKey] = it->value.GetInt();
                    } else if (it->value.IsDouble()) {
                        itemMap[itemKey] = it->value.GetDouble();
                    } else if (it->value.IsArray()) {
                        std::vector<std::string> arrayValues;
                        for (const auto& val : it->value.GetArray()) {
                            if (val.IsString()) {
                                arrayValues.push_back(val.GetString());
                            }
                        }
                        itemMap[itemKey] = arrayValues;
                    }
                }
                result.push_back(itemMap);
            }
        }
    }
    return result;
}

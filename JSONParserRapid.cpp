#include "JSONParserRapid.h"
#include <fstream>
#include <sstream>
#include <cstdio> // For std::fclose
#include <memory> // For std::unique_ptr
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

// Custom deleter function to close the file
void close_file(std::FILE* file) {
    if (file) {
        std::fclose(file);
    }
}

std::wstring StringToWString(const std::string& str) {
    if (str.empty()) {
        return L"";
    }

    // Calculate the required size of the wide character buffer
    size_t requiredSize = mbstowcs(nullptr, str.c_str(), 0);
    if (requiredSize == (size_t)-1) {
        throw std::runtime_error("Error converting string to wide string.");
    }

    // Allocate the buffer
    std::vector<wchar_t> buffer(requiredSize + 1); // +1 for null terminator

    // Perform the conversion
    size_t convertedChars;
    if (mbstowcs_s(&convertedChars, buffer.data(), buffer.size(), str.c_str(), str.size()) != 0) {
        throw std::runtime_error("Error converting string to wide string.");
    }

    return std::wstring(buffer.data());
}

bool JSONParserRapid::LoadFromFile(const std::string& filePath) {

    using unique_file_t = std::unique_ptr<std::FILE, decltype(&close_file)>;
    unique_file_t file(std::fopen(filePath.c_str(), "r"), close_file);

    if (!file) {
        m_LastError = "Error: Could not open file at " + filePath;
        return false;
    }

    // Read the content of the file into a string
    std::stringstream buffer;
    buffer << std::ifstream(filePath).rdbuf(); // Read the file content into a buffer

    // Use rapidjson's Parse method
    m_JSONData.Parse(buffer.str().c_str());

    // Check for parsing errors
    if  (m_JSONData.HasParseError()) {
        m_LastError = "Error: Failed to parse JSON - " +
                    std::string(rapidjson::GetParseError_En (m_JSONData.GetParseError()));
        return false;
    }

    m_LastError.clear(); // Clear any previous errors on success
    return true;
}

std::vector<std::unordered_map<std::string, std::any>> JSONParserRapid::GetArrayOfObjects(const std::string& key) const {
    std::vector<std::unordered_map<std::string, std::any>> result;

    // Check if the key exists and is an array
    if (m_JSONData.HasMember(key.c_str()) && m_JSONData[key.c_str()].IsArray()) {
        const auto& jsonArray = m_JSONData[key.c_str()].GetArray();

        // Iterate over each item in the array
        for (const auto& item : jsonArray) {
            std::unordered_map<std::string, std::any> itemMap;

            // Ensure the item is an object
            if (item.IsObject()) {
                for (auto it = item.MemberBegin(); it != item.MemberEnd(); ++it) {
                    std::string itemKey = it->name.GetString();

                    // Check the type of each JSON value and store it in std::any
                    if (it->value.IsString()) {
                        std::wstring wideValue = StringToWString(std::string(it->value.GetString()));
                        itemMap[itemKey] = wideValue;
                    } else if (it->value.IsInt()) {
                        itemMap[itemKey] = it->value.GetInt();
                    } else if (it->value.IsDouble()) {
                        itemMap[itemKey] = it->value.GetDouble();
                    } else if (it->value.IsArray()) {
                        std::vector<std::wstring> arrayValues;
                        for (const auto& val : it->value.GetArray()) {
                            if (val.IsString()) {
                                arrayValues.push_back(StringToWString(val.GetString()));
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

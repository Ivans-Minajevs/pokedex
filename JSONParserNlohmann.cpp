#include "JSONParserNlohmann.h"
#include <fstream>
#include <memory> 
#include <cstdio>
#include <sstream>


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

bool JSONParserNlohmann::LoadFromFile(const std::string& filePath) {
    // Use a unique pointer with a custom deleter for the file
    using unique_file_t = std::unique_ptr<std::FILE, decltype(&close_file)>;
    unique_file_t file(std::fopen(filePath.c_str(), "r"), close_file);

    if (!file) {
        m_LastError = "Error: Could not open file at " + filePath;
        return false;
    }

    // Read the content of the file into a string
    std::stringstream buffer;
    buffer << std::ifstream(filePath).rdbuf(); // Read the file content

    try {
        m_JSONData = nlohmann::json::parse(buffer.str()); // Parse JSON data from the string
        m_LastError.clear();  // Clear any previous errors on success
    } catch (const std::exception& e) {
        m_LastError = "Error: Failed to parse JSON - " + std::string(e.what());
        return false;
    }
    return true;
}

std::vector<std::unordered_map<std::string, std::any>> JSONParserNlohmann::GetArrayOfObjects(const std::string& key) const  {
    std::vector<std::unordered_map<std::string, std::any>> result;

    // Check if the parent key exists and is an array
    if (m_JSONData.contains(key) && m_JSONData[key].is_array()) {
        // Loop through each item in the array associated with the parent key
        for (const auto& item : m_JSONData[key]) {
            std::unordered_map<std::string, std::any> itemMap;

            // For each key-value pair in the JSON object (each Pokémon), insert into the map
            for (auto it = item.begin(); it != item.end(); ++it) {
                // Check the type of the JSON value to cast appropriately
                if (it.value().is_string()) {
                    
                    std::wstring wideValue = StringToWString(it.value().get<std::string>());
                    itemMap[it.key()] = wideValue;
                    
                    //itemMap[it.key()] = it.value().get<std::string>();
                } else if (it.value().is_number_integer()) {
                    itemMap[it.key()] = it.value().get<int>();
                } else if (it.value().is_number_float()) {
                    itemMap[it.key()] = it.value().get<double>();
                } else if (it.value().is_array()) {
                    std::vector<std::wstring> arrayValues;
                    for (const auto& val : it.value()) {
                        if (val.is_string()) {
                            arrayValues.push_back(StringToWString(val.get<std::string>()));
                        }
                    }
                    itemMap[it.key()] = arrayValues;
                }
                // Additional type checks can be added here if needed
            }

            // Add the populated map to the result vector
            result.push_back(itemMap);
        }
    }

    return result;
}

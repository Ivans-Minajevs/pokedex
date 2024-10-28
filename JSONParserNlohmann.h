#ifndef JSONPARSERNLOHMANN_H
#define JSONPARSERNLOHMANN_H

#include "JSONParserCustomLib.h"
#include <nlohmann/json.hpp>

class JSONParserNlohmann : public JSONParserCustomLib
{

public:
    bool LoadFromFile(const std::string& filePath) override;
    std::string GetLastError() const override { return m_LastError; }
    std::vector<std::unordered_map<std::string, std::any>> GetArrayOfObjects(const std::string& key) const override;
private:
    nlohmann::json m_JSONData;
    std::string m_LastError;
};


#endif JSONPARSERNLOHMANN_H
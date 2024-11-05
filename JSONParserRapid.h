#ifndef JSONPARSERRAPID_H
#define JSONPARSERRAPID_H

#include "JSONParserCustomLib.h"
#include <rapidjson/document.h>

class JSONParserRapid : public JSONParserCustomLib
{
public:
    bool LoadFromFile(const std::string& filePath) override;
    std::string GetLastError() const override { return m_LastError; }
    std::vector<std::unordered_map<std::string, JSONValue>> GetArrayOfObjects(const std::string& key) const override;

private:
    rapidjson::Document m_JSONData;
    std::string m_LastError;
};

#endif JSONPARSERRAPID_H

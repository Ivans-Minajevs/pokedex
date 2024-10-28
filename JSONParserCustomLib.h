#ifndef JSONPARSERCUSTOMLIB_H
#define JSONPARSERCUSTOMLIB_H

#include <string>
#include <vector>
#include <unordered_map>
#include <any>

class JSONParserCustomLib 
{
    public:
    virtual ~JSONParserCustomLib() = default;

    virtual bool LoadFromFile(const std::string& filePath) = 0;

    virtual std::string GetLastError() const = 0;
    
    virtual std::vector<std::unordered_map<std::string, std::any>> GetArrayOfObjects(const std::string& key) const = 0;
};

#endif JSONPARSERCUSTOMLIB_H
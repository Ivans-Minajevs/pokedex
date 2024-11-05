#ifndef JSONPARSERCUSTOMLIB_H
#define JSONPARSERCUSTOMLIB_H

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

//JSONValue type using std::variant
using JSONValue = std::variant<
    std::nullptr_t,
    bool,
    int,
    double,
    std::string,
    std::vector<std::string>
>;


class JSONParserCustomLib 
{
    public:
    virtual ~JSONParserCustomLib() = default;
    virtual bool LoadFromFile(const std::string& filePath) = 0;
    virtual std::string GetLastError() const = 0;
    virtual std::vector<std::unordered_map<std::string, JSONValue>> GetArrayOfObjects(const std::string& key) const = 0;
};

#endif JSONPARSERCUSTOMLIB_H
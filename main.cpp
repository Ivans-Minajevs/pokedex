#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <windows.h>
#include <variant>

#include "JSONParserCustomLib.h"

#if defined(USE_NLOHMANN)
    #include "JSONParserNlohmann.h"
#elif defined(USE_RAPID)
    #include "JSONParserRapid.h"
#else
    #error "No JSON parser library defined."
#endif

std::unique_ptr<JSONParserCustomLib> create_parser() {
#if defined(USE_NLOHMANN)
    return std::make_unique<JSONParserNlohmann>();
#elif defined(USE_RAPID)
    return std::make_unique<JSONParserRapid>();
#endif
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    std::unique_ptr<JSONParserCustomLib> jsonParser = create_parser();

    if (!jsonParser->LoadFromFile("Content/pokedex.json")) {
        std::cerr << jsonParser->GetLastError() << "\n";
        return 1;
    }

    std::vector<std::unordered_map<std::string, JSONValue>> pokemonData = jsonParser->GetArrayOfObjects("pokemon");

    for (const auto& pokemon : pokemonData) {
        int id = std::get<int>(pokemon.at("id"));
        std::string name = std::get<std::string>(pokemon.at("name"));
        std::vector<std::string> types = std::get<std::vector<std::string>>(pokemon.at("type"));

        std::string typesFormatted;
        for (const auto& type : types) {
            typesFormatted += type + ", ";
        }
        if (!typesFormatted.empty()) {
            typesFormatted = typesFormatted.substr(0, typesFormatted.length() - 2);
        }

        std::cout << id << ". " << name << "[" << typesFormatted << "]" << std::endl;
    }
 
    std::cin.get();
    return 0;
}

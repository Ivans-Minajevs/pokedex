#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <any>
#include <windows.h>

#include "JSONParserCustomLib.h"

#if defined(USE_NLOHMANN)
    #include "JSONParserNlohmann.h"
#elif defined(USE_RAPID)
    #include "JSONParserRapid.h"
#else
    #error "No JSON parser library defined."
#endif

// Function to create the appropriate parser
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
        return 1;  // Exit with an error code
    }
    std::vector<std::unordered_map<std::string, std::any>> pokemonData = jsonParser->GetArrayOfObjects("pokemon");

    for (const auto& pokemon : pokemonData) {
        int id = std::any_cast<int>(pokemon.at("id")); // Use at() for safe access
        std::wstring name = std::any_cast<std::wstring>(pokemon.at("name"));
        std::vector<std::wstring> types = std::any_cast<std::vector<std::wstring>>(pokemon.at("type"));
 
        // Format types into a string
        std::wstring typesFormatted;
        for (const auto& type : types) {
            typesFormatted += type + L", ";
        }

        // Remove the trailing comma and space if types are present
        if (!typesFormatted.empty()) {
            typesFormatted = typesFormatted.substr(0, typesFormatted.length() - 2);
        }

        // Print output in the specified format
        std::wcout << id << ". " << name  << " [" << typesFormatted << "]" << std::endl;
    }
 
    std::wcin.get();
    return 0;
}

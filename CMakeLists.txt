cmake_minimum_required(VERSION 3.15)

project(Pokedex)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options("/utf-8")  # MSVC-specific flag for UTF-8
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options("-finput-charset=UTF-8" "-fexec-charset=UTF-8")  # GCC and Clang-specific flags
endif()

add_executable(Pokedex main.cpp)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
                ${CMAKE_SOURCE_DIR}/Content
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/Content)


target_include_directories(Pokedex PUBLIC ./)

# Set the option to choose the JSON parser
option(USE_NLOHMANN "Use Nlohmann JSON library" OFF)
option(USE_RAPID "Use RapidJSON library" ON)

include(FetchContent)
if(USE_NLOHMANN)
    target_sources(Pokedex PRIVATE JSONParserNlohmann.cpp)
    FetchContent_Declare(nlohmann_json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)

    FetchContent_MakeAvailable(nlohmann_json)

    target_link_libraries(Pokedex PRIVATE nlohmann_json::nlohmann_json)
    add_definitions(-DUSE_NLOHMANN)
elseif(USE_RAPID)
    target_sources(Pokedex PRIVATE JSONParserRapid.cpp)
    FetchContent_Declare(
        rapid_json
        GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
        GIT_TAG master
        
    )
    set(RAPIDJSON_BUILD_TESTS OFF CACHE INTERNAL "")
    set(RAPIDJSON_BUILD_DOC OFF CACHE INTERNAL "")
    set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE INTERNAL "")
    set(TRAVIS_DOC OFF CACHE INTERNAL "") 
    set(RAPIDJSON_BUILD_CXX11 OFF)
    set(RAPIDJSON_BUILD_CXX20 ON)
    
    FetchContent_MakeAvailable(rapid_json)

    target_include_directories(Pokedex PRIVATE ${rapid_json_SOURCE_DIR}/include)
    add_definitions(-DUSE_RAPID)
else()
    message(FATAL_ERROR "No JSON parser library selected. Please enable either USE_NLOHMANN or USE_RAPIDJSON.")
endif()
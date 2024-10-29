
<h1>Pokedex (JSON)</h1>

A C++ console application that parses the given pokedex.json file and outputs to the console as requested
There are two parsing libraries user can switch between at build time:
1) Nlohmann
2) Rapid


<h2>How to use?</h2>

1. Open the <strong>CMD | Powershell</strong> in workspace folder.

2. Use the following commands:

```bash
cmake -S . -B build -DUSE_NLOHMANN=ON -DUSE_RAPID=OFF # specifies build directory
#to switch between modes -DUSE_NLOHMANN=OFF -DUSE_RAPID=ON
#NOTE: once set desired mode to ON, make sure the other is OFF
cmake --build build # builds the project
.\build\Debug\Pokedex.exe  #runs the executable
 #NOTE: Use .\ instead of ./ for Windows
```
3. Press <strong>Enter</strong> to exit application.

4. Clean the build folder:

CMD:
```bash
    rmdir /S /Q build #removes the directory and all subdirectories/files in CMD
```
Powershell:
```ps1
    rmdir .\build -Recurse -Force #removes the directory and all subdirectories/files in Powershell
```

<h2>Additional info</h2>
Application uses <strong>C++20 version</strong> and <storng>UTF-8 character encoding</strong>.

Application converts objects JSON to <strong>std::vector<std::unordered_map<std::string, std::any>></strong>
Check [cppreference std::unordered_map](https://en.cppreference.com/w/cpp/container/unordered_map) and [cppreference std::any (since C++17)](https://en.cppreference.com/w/cpp/utility/any) for more info.









    


# Cultural venue management (C++17)

## Prerequisites

- CMake 3.16 or newer
- A C++17 compiler (for example GCC)

## Build

From the project root, configure and then build the project:

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++
cmake --build build
```

With Visual Studio, you can specify the configuration:

```powershell
cmake --build build --config Release
```

## Run

On Windows (default configuration):

```powershell
Copy-Item data\test_data.txt lieu_data.txt
.\build\Projet-academiq.exe lieu_data.txt
```

After a Release build with Visual Studio:

```powershell
.\build\Release\Projet-academiq.exe lieu_data.txt
```

On Linux or macOS:

```sh
cp data/test_data.txt lieu_data.txt
./build/Projet-academiq lieu_data.txt
```

The argument is the path to the data file. If it is not provided, the application
uses `lieu_data.txt`. Changes are saved in this file. Copying the sample avoids
modifying files tracked by Git.

To check handling of an inconsistent file, run the application with
`data/test_data_corrompu.txt` as the argument. It displays the error and exits
with status code 2.

## Configuration and workshop

The default admin code is `Karm`; it is defined in `src/Config.hpp`.

The files in `ajout_atelier/` are not part of the current target.
To compile them with the application, add `ajout_atelier/Workshop.cpp`
to the `add_executable` source list in `CMakeLists.txt` and integrate the new type
into the application code.

A UTF-8 configured terminal is recommended to display accents correctly.
Run this command to configure UTF-8:
`chcp 65001`


# Growtopia Item Decoder

This program is used to decode the `items.dat` file used in the **Growtopia** game.

## Features
- Reads the `items.dat` file in binary format.
- Converts item data into JSON format.
- Saves the decoded result to a `data.json` file.

## System Requirements

Before you begin, make sure your system has the following required tools:

- **C++ Compiler**: For example, GCC or Clang.
- **CMake**: For configuring and building the C++ project.
- **Make**: For building the program (if using Makefile).

## Running the Program
```bash
/decoder-items.dat-growtopia
    ├── decoder.cpp
    └── items.dat // required

g++ -o decoder.exe decoder.cpp // using GCC
clang++ -o decoder.exe decoder.cpp // using Clang

run decoder.exe
```
### Clone Repository

Clone this repository to your local machine using Git:

```bash
git clone https://github.com/CodeHaichel/decoder-items.dat-growtopia.git
cd decoder-items.dat-growtopia

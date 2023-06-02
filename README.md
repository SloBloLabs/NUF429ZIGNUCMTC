How to convert a CubeMX / CubeIDE project to GNU ARM Toolchain with CMake, openocd and VS Code

Prerequisites

Available on PATH
 - ARM toolchain
 - make
 - cmake
 - openocd

VS Code extensions
 - MS C/C++
 - MS C/C++ Extension Pack
 - Cortex Debug

---

Workflow

Copy to project directory:
 - README.md
 - Makefile
 - CMakeLists.txt
 - cmake folder with arm toolchain cmake directives
 - .gitignore file
 - swvtrace.sh
 - svd file (optional)
 - folder .vscode/launch.json and adapt settings
 - folder "App"

then
 - Adapt settings in CMakeLists.txt header
 - rename myAppMain [.hpp/.cpp]
 - include <"AppName">Main.hpp in Core/Src/main.c
 - add projectMain() to main.c (right before while loop)
 - right click CMakeLists.txt -> configure all projects
 - start coding

Tested on
 - Mac (CMake 3.22.2, ARM Toolchain 10.3-2021.10, openocd 0.11.0 DEV g5ab74bde0, st-link 1.7.0)
 - Linux (CMake 3.18.4, ARM Toolchain 9.2.1 20191025, openocd 0.11.0-rc2, st-link 1.6.1)

---

Upload to github
 - git init
 - git add .
 - git config user.name "<<username>>"
 - git config user.email "<<email>>"
 - git commit -m "First commit"
 - git branch -M main
 - git remote add origin https://github.com/SloBloLabs/<"ProjectRepo">.git
 - git push -u origin main

---

Merge 2 branches without committing:
git merge --no-commit --no-ff <other branch>

---

Composite USB interface:

3 configurations
4 interfaces
6 endpoints

configuration 1:
 - CDC
 - 2 interfaces
   - 0: communications control
     - 1 endpoint 0x82
   - 1: communications data
     - 2 endpoints 0x01 + 0x81 (BULK)
 - 4 endpoints overall
   - EP0
   - EP1
   - EP2 (in only: 0x82)

configuration 2:
 - DFU
 - 1 interface
   - 0: application specific/DFU
 - 1 endpoint overall
   - EP0 only

configuration 3:
 - MIDI
 - 1 interface
   - 0: Audio/Streaming
     - 2 endpoints 0x02 + 0x83 (BULK)
 - 3 endpoints overall
   - EP0
   - EP2 (out only: 0x02)
   - EP3 (in only: 0x83)


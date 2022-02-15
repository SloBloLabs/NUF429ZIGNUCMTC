How to convert a CubeMX project to GNU ARM Toolchain with CMake and openocd

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

Adapt settings in CMakeLists.txt header
rename myAppMain [.hpp/.cpp]
include myAppMain.hpp in Core/Src/main.c
add projectMain() to main.c (right before while loop)
right click CMakeLists.txt -> configure all projects
start coding
-------------------------------------------------------------

Upload to github
git init
git add .
git config user.name "<<username>>"
git config user.email "<<email>>"
git commit -m "First commit"
git remote add origin https://github.com/SloBloLabs/<<ProjectRepo>>.git
git push -u origin master

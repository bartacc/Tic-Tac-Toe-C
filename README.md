# Tic-Tac-Toe-C

This is a modified Tic Tac Toe game written in C and GTK+3.

### Installing the game
#### On Linux:
1. Make sure GCC, GTK and CMake are installed. On Ubuntu: \
`sudo apt-get install gcc` \
`sudo apt-get install libgtk-3-dev` with all dependencies \
`sudo apt-get install cmake`
2. Compile resources: \
`cd resources` \
`./compile_resources.sh` \
`cd ..`
3. Build the project (inside project's root): \
`cmake -S . -B cmake-build-debug/` \
`cmake --build cmake-build-debug/ --target Tic_Tac_Toe_C`
4. Run the game (You should run it in two instances): \
`cd cmake-build-debug` \
`./Tic_Tac_Toe_C`

### On Windows:
1. Install GCC, GTK and CMake using MSys2 (https://www.gtk.org/docs/installations/windows/) \
   Especially run: \
   `pacman -S mingw-w64-x86_64-gtk3` \
   `pacman -S base-devel mingw-w64-x86_64-toolchain` \
   `pacman -S mingw-w64-x86_64-cmake`
2. Add `C:\msys64\mingw64\bin` to `PATH`
3. In `CMakeLists.txt` change `ADD_EXECUTABLE(... src/connection/lin_fifo.c ...)` to `ADD_EXECUTABLE(... src/connection/win_fifo.c ...)`
4. Compile resources: \
   `cd resources` \
   `"C:\msys64\mingw64\bin\glib-compile-resources.exe" gresource.xml --target=resources.c --generate-source`
5. Build the project (inside project's root): \
   `C:\msys64\mingw64\bin\cmake.exe -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=C:/msys64/mingw64/bin/make.exe -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe -G "CodeBlocks - MinGW Makefiles" .` \
   `C:\msys64\mingw64\bin\cmake.exe --build . --target Tic_Tac_Toe_C -- -j 4`
6. Run the game (You should run it in two instances): \
   `cd cmake-build-debug` \
   `./Tic_Tac_Toe_C`

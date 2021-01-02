CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
PROJECT(Tic_Tac_Toe_C C)

SET(CMAKE_C_STANDARD 11)

# Use the package PkgConfig to detect GTK+ headers/library files
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})

# Add other flags to the compiler
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

# Add an executable compiled from main.c
ADD_EXECUTABLE(Tic_Tac_Toe_C main.c resources/resources.c src/app/app.c src/app/app.h src/app/appwin.c src/app/appwin.h src/menu/menu.c src/menu/menu.h src/how_to_play/how_to_play.c src/how_to_play/how_to_play.h)

# Link the target to the GTK+ libraries
TARGET_LINK_LIBRARIES(Tic_Tac_Toe_C ${GTK3_LIBRARIES})

#TARGET_COMPILE_OPTIONS(Tic_Tac_Toe_C PRIVATE -Werror -Wall -Wextra)
CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
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
ADD_EXECUTABLE(Tic_Tac_Toe_C main.c src/connection/fifo.h src/connection/lin_fifo.c resources/resources.c src/menu/menu.c src/menu/menu.h src/how_to_play/how_to_play.c src/how_to_play/how_to_play.h src/connection/connection.c src/connection/connection.h src/constants/constants.h src/lobby/lobby.c src/lobby/lobby.h src/app/app.c src/app/app.h src/game/game.c src/game/game.h src/constants/constants.c src/game/utils/game_utils.c src/game/utils/game_utils.h src/game/utils/game_gtk_utils.c src/game/utils/game_gtk_utils.h src/modals/modals.c src/modals/modals.h)

# Link the target to the GTK+ libraries
TARGET_LINK_LIBRARIES(Tic_Tac_Toe_C ${GTK3_LIBRARIES})

#TARGET_COMPILE_OPTIONS(Tic_Tac_Toe_C PRIVATE -Werror -Wall -Wextra)
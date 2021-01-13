#ifndef TIC_TAC_TOE_C_CONSTANTS_H
#define TIC_TAC_TOE_C_CONSTANTS_H

#define MAX_CONSTANT_LENGTH 200

//Connection
#define REQUEST_CONNECTION "request_connection"
#define DROP_CONNECTION "drop_connection"
#define ACCEPT_CONNECTION "accept_connection"

#define COLUMNS_4 "columns_4"
#define COLUMNS_5 "columns_5"

#define MOVE_PUSH_PREFIX "move_push_"
#define MOVE_REPLACE_PREFIX "move_replace_"

//Pages
#define GAMEPLAY_PAGE "gameplay_page"
#define MENU_PAGE "menu_page"

//Gameplay
#define BOARD_SIZE 10
#define BOARD_SIZE_MAX_DIGITS 2

//CSS classes
#define CSS_GAMEPLAY_FRAME "gameplay-frame"
#define CSS_GAMEPLAY_FRAME_FOR_PLACEHOLDER "gameplay-frame-for-placeholder"
#define CSS_ADD_PLACEHOLDER "add-placeholder"
#define CSS_REMOVE_PLACEHOLDER "remove-placeholder"

typedef enum playerType {
    PLAYER_ONE, PLAYER_TWO
} PlayerType;

typedef enum element {
    X, O, EMPTY
} Element;

typedef enum placeholderType {
    PLACEHOLDER_ADD, PLACEHOLDER_REMOVE, NOT_PLACEHOLDER
} PlaceholderType;

Element get_element(PlayerType pType);

#endif //TIC_TAC_TOE_C_CONSTANTS_H

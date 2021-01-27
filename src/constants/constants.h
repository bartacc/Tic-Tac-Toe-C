#ifndef TIC_TAC_TOE_C_CONSTANTS_H
#define TIC_TAC_TOE_C_CONSTANTS_H

#define MAX_STRING_LENGTH 200
#define MAX_WINNING_SEQUENCE 5 //Maximum possible elements in a row/column/diagonal needed to win

//Connection
#define REQUEST_CONNECTION "request_connection"
#define DROP_CONNECTION "drop_connection"
#define ACCEPT_CONNECTION "accept_connection"

#define SIZE_4 "size_4"
#define SIZE_5 "size_5"

#define MOVE_PUSH_PREFIX "move_push_"
#define MOVE_REPLACE_PREFIX "move_replace_"

#define PLAY_AGAIN "play_again"

//Pages
#define GAMEPLAY_PAGE "gameplay_page"
#define MENU_PAGE "menu_page"

//Gameplay
#define BOARD_SIZE 10
#define BOARD_SIZE_MAX_DIGITS 2

//CSS classes
#define CSS_GAMEPLAY_FRAME "gameplay-frame"
#define CSS_GAMEPLAY_FRAME_IN_WINNING_SEQUENCE "gameplay-frame-in-winning-sequence"
#define CSS_GAMEPLAY_FRAME_FOR_PLACEHOLDER "gameplay-frame-for-placeholder"
#define CSS_ADD_PLACEHOLDER "add-placeholder"
#define CSS_REMOVE_PLACEHOLDER "remove-placeholder"

typedef enum playerType {
    PLAYER_ONE, PLAYER_TWO, PLAYER_NONE
} PlayerType;

typedef enum element {
    X, O, EMPTY
} Element;

typedef enum placeholderType {
    PLACEHOLDER_ADD, PLACEHOLDER_REMOVE, NOT_PLACEHOLDER
} PlaceholderType;

Element get_element(PlayerType pType);
PlayerType get_player(Element element);

#endif //TIC_TAC_TOE_C_CONSTANTS_H

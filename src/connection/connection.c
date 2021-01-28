#include <gtk/gtk.h>

#include "connection.h"
#include "fifo.h"
#include "../lobby/lobby.h"
#include "../app/app.h"
#include "../game/game.h"
#include "../modals/modals.h"

typedef struct state {
    bool requestReceived;
    bool requestSent;

    bool dropReceived;
    bool dropSent;

    bool connectionAccepted;

    bool connectionEstablished;
} State;

static State state;
static PipesPtr currentPipe = NULL;
static gint timeoutID = -1;

static void init_state();
static void connection_request();
static void act_on_state_change();
static gboolean get_text();

static void init_state() {
    state.requestReceived = false;
    state.requestSent = false;
    state.dropReceived = false;
    state.dropSent = false;
    state.connectionAccepted = false;
    state.connectionEstablished = false;
}

void connection_init(PlayerType playerType) {
    g_debug("%s", "Connection init");
    init_state();
    if (currentPipe != NULL) {
        close_pipes(currentPipe);
    }
    currentPipe = init_pipes(playerType);
    if (timeoutID != -1) {
        g_source_remove(timeoutID);
    }
    timeoutID = g_timeout_add(100, get_text, NULL);
    connection_request();
}

static void connection_request() {
    send_string_to_pipe(currentPipe, REQUEST_CONNECTION);
    state.requestSent = true;
    g_debug("%s", "Sent connection request");
    act_on_state_change();
}

void connection_drop() {
    if (state.dropSent || state.dropReceived) return;

    if (currentPipe != NULL && state.connectionEstablished) {
        send_string_to_pipe(currentPipe, DROP_CONNECTION);
        g_debug("%s", "Sent drop request");
    }
    state.dropSent = true;
    act_on_state_change();
}

static void connection_accept() {
    send_string_to_pipe(currentPipe, ACCEPT_CONNECTION);
    g_debug("%s", "Sent accept connection");
}

void connection_send_size(int size) {
    if (size == 4) {
        send_string_to_pipe(currentPipe, SIZE_4);
    } else {
        send_string_to_pipe(currentPipe, SIZE_5);
    }
    g_debug("%s%d", "Sent size_", size);
}

void connection_send_move(char *prefix, int column) {
    char operation[MAX_STRING_LENGTH];
    strcpy(operation, prefix);

    char columnString[BOARD_SIZE_MAX_DIGITS + 1];
    columnString[BOARD_SIZE_MAX_DIGITS] = '\0';
    for (int i = BOARD_SIZE_MAX_DIGITS - 1; i >= 0; i--) {
        int digit = column % 10;
        column = column / 10;
        char digitChar = digit + '0';
        columnString[i] = digitChar;
    }

    strcat(operation, columnString);

    send_string_to_pipe(currentPipe, operation);
    g_debug("Sent %s%s", prefix, columnString);
}

void connection_send_play_again() {
    send_string_to_pipe(currentPipe, PLAY_AGAIN);
    g_debug("Sent %s", PLAY_AGAIN);
}

static int read_move_column(char *numberPtr) {
    char column[BOARD_SIZE_MAX_DIGITS + 1];
    for (int i = 0; i < BOARD_SIZE_MAX_DIGITS; i++) {
        column[i] = *numberPtr;
        numberPtr++;
    }
    column[BOARD_SIZE_MAX_DIGITS] = '\0';
    return atoi(column);
}

static void act_on_state_change() {
    if (state.dropReceived) {
        app_quit();
    }

    if ((state.requestSent && state.requestReceived) || state.connectionAccepted) {
        init_state();
        state.connectionEstablished = true;
        g_debug("%s", "Connection has been established");
        lobby_connection_established();
    }
}

void connection_show_error(char *error) {
    app_show_error(error);
}

static gboolean get_text() {
    if (currentPipe == NULL) {
        return TRUE;
    }

    gchar input[MAX_STRING_LENGTH];
    if (get_string_from_pipe(currentPipe, input, MAX_STRING_LENGTH)) {
        g_debug("%s: %s", "Incoming operation", input);
        if (strstr(input, REQUEST_CONNECTION) != NULL) {
            state.requestReceived = true;
            g_debug("%s", "Received connection request");
            if (state.requestSent) {
                connection_accept();
            }
            act_on_state_change();
        }
        if (strstr(input, ACCEPT_CONNECTION) != NULL) {
            if (!state.connectionEstablished) {
                state.connectionAccepted = true;
                g_debug("%s", "Received accept connection");
                act_on_state_change();
            } else {
                g_debug("%s", "Received accept connection (Connection already established)");
            }
        }
        if (strstr(input, DROP_CONNECTION) != NULL) {
            state.dropReceived = true;
            g_debug("%s", "Received drop request");
            act_on_state_change();
        }
        if (strstr(input, SIZE_4) != NULL) {
            g_debug("Received %s", SIZE_4);
            lobby_size_received(4);
        }
        if (strstr(input, SIZE_5) != NULL) {
            g_debug("Received %s", SIZE_5);
            lobby_size_received(5);
        }
        if (strstr(input, MOVE_PUSH_PREFIX) != NULL) {
            char *stringPtr = strstr(input, MOVE_PUSH_PREFIX);
            stringPtr += strlen(MOVE_PUSH_PREFIX);
            int column = read_move_column(stringPtr);
            game_move_push(column, false);

            g_debug("Received %s%d", MOVE_PUSH_PREFIX, column);
        }
        if (strstr(input, MOVE_REPLACE_PREFIX) != NULL) {
            char *stringPtr = strstr(input, MOVE_REPLACE_PREFIX);
            stringPtr += strlen(MOVE_REPLACE_PREFIX);
            int column = read_move_column(stringPtr);
            game_move_replace(column);

            g_debug("Received %s%d", MOVE_REPLACE_PREFIX, column);
        }
        if (strstr(input, PLAY_AGAIN)) {
            g_debug("Received %s", PLAY_AGAIN);
            game_opponent_concede();
        }
    }
    return TRUE;
}

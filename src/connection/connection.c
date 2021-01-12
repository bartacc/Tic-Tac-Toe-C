#include <gtk/gtk.h>

#include "connection.h"
#include "fifo.h"
#include "../lobby/lobby.h"
#include "../app/app.h"

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
    printf("%s\n", "Connection init");
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
    printf("%s\n", "Sent connection request");
    act_on_state_change();
}

void connection_drop() {
    if (currentPipe != NULL && state.connectionEstablished) {
        send_string_to_pipe(currentPipe, DROP_CONNECTION);
        printf("%s\n", "Sent drop request");
    }
    state.dropSent = true;
    act_on_state_change();
}

static void connection_accept() {
    send_string_to_pipe(currentPipe, ACCEPT_CONNECTION);
    printf("%s\n", "Sent accept connection");
}

void connection_send_columns(int columns) {
    if (columns == 4) {
        send_string_to_pipe(currentPipe, COLUMNS_4);
    } else {
        send_string_to_pipe(currentPipe, COLUMNS_5);
    }
    printf("%s%d\n", "Sent columns_", columns);
}

static void act_on_state_change() {
    if ((state.requestSent && state.requestReceived) || state.connectionAccepted) {
        init_state();
        state.connectionEstablished = true;
        printf("%s\n", "Connection has been established");
        lobby_connection_established();
    } else if (state.dropSent || state.dropReceived) {
        if (state.connectionEstablished) {
            printf("%s\n", "Connection dropped");
        }
        init_state();
        currentPipe = NULL;
    }
}

void connection_show_error(char *error) {
    app_show_error(error);
}

static gboolean get_text() {
    if (currentPipe == NULL) {
        return TRUE;
    }

    gchar input[MAX_CONSTANT_LENGTH];
    if (get_string_from_pipe(currentPipe, input, MAX_CONSTANT_LENGTH)) {
        printf("%s: %s\n", "Incoming operation", input);
        if (strstr(input, REQUEST_CONNECTION) != NULL) {
            state.requestReceived = true;
            printf("%s\n", "Received connection request");
            if (state.requestSent) {
                connection_accept();
            }
            act_on_state_change();
        }
        if (strstr(input, ACCEPT_CONNECTION) != NULL) {
            if (!state.connectionEstablished) {
                state.connectionAccepted = true;
                printf("%s\n", "Received accept connection");
                act_on_state_change();
            } else {
                printf("%s\n", "Received accept connection (Connection already established)");
            }
        }
        if (strstr(input, DROP_CONNECTION) != NULL) {
            state.dropReceived = true;
            printf("%s\n", "Received drop request");
            act_on_state_change();
        }
        if (strstr(input, COLUMNS_4) != NULL) {
            printf("%s\n", "Received columns_4");
            lobby_columns_received(4);
        }
        if (strstr(input, COLUMNS_5) != NULL) {
            printf("%s\n", "Received columns_5");
            lobby_columns_received(5);
        }
    }
    return TRUE;
}

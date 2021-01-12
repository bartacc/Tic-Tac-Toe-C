#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "fifo.h"
#include "connection.h"


struct pipes {
    FILE *fifo_in, *fifo_out;
    int isPlayerOne;
};
int fileno(FILE *file);
static FILE *open_out_pipe(char *name);
static FILE *open_in_pipe(char *name);

void close_pipes(PipesPtr channel) {
    fclose(channel->fifo_in);
    fclose(channel->fifo_out);
    free(channel);
}

PipesPtr init_pipes(PlayerType playerType) {
    PipesPtr pipes = (PipesPtr) malloc(sizeof(struct pipes));
    if (pipes == NULL) {
        fprintf(stderr, "Memory allocation error\n");
    } else {
        pipes->isPlayerOne = (playerType == PLAYER_ONE);
        pipes->fifo_out = open_out_pipe(pipes->isPlayerOne ? "AtoB" : "BtoA");
        pipes->fifo_in = open_in_pipe(pipes->isPlayerOne ? "BtoA" : "AtoB");
    }
    return pipes;
}

static FILE *open_out_pipe(char *name) {
    mkfifo(name, 0664);
    FILE *pipe = fopen(name, "w+");
    if (pipe == NULL) {
        connection_show_error("Error in creating output pipe");
        exit(-1);
    }
    return pipe;
}

static FILE *open_in_pipe(char *name) {
    FILE *pipe = fopen(name, "r+");
    if (pipe == NULL) {
        mkfifo(name, 0664);
        pipe = fopen(name, "r+");
    }
    if (pipe == NULL) {
        connection_show_error("Error in creating input pipe");
        exit(-1);
    }
    int flags, fd;
    fd = fileno(pipe);
    flags = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    return pipe;
}

void send_string_to_pipe(PipesPtr channel, const char *data) {
    int result = fprintf(channel->fifo_out, "%s", data);
    fflush(channel->fifo_out);
    if (result == 0)
        connection_show_error("Failed to send data");
}

bool get_string_from_pipe(PipesPtr channel, char *buffer, size_t size) {
    char *result = fgets(buffer, size, channel->fifo_in);
    fflush(channel->fifo_in);
    //if (result == NULL) connection_show_error("Failed to read data");
    return result != NULL;
}
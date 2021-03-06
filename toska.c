#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

#include "chess.h"
#include "misc.h"

typedef struct {
    FILE *w;
    FILE *r;
    char *buffer;
    size_t bufsize;
} engineIO;

WINDOW *boardWin;
WINDOW *logWin;
WINDOW *inputWin;

void startEngine(char *cmd, int pipes[2][2]) {
    if (pipe(pipes[0]) < 0) {   // reading from engine
        perror("creating pipe");
        exit(-1);
    }

    if (pipe(pipes[1]) < 0) {   // writing to engine
        perror("creating pipe");
        exit(-1);
    }

    if (fork() == 0) {
        dup2(pipes[0][1], STDOUT_FILENO); // attach output pipe to stdout
        dup2(pipes[0][1], STDERR_FILENO); // attach output pipe to stderr
        dup2(pipes[1][0], STDIN_FILENO);  // attach input pipe to stdin 
        close(pipes[0][0]);       // close read end of output pipe
        close(pipes[0][1]);       // close read end of output pipe
        close(pipes[1][0]);       // close write end of input pipe
        close(pipes[1][1]);       // close write end of input pipe
        execl(cmd, cmd, (char *)NULL);          // run engine
    }
    
    close(pipes[0][1]);
    close(pipes[1][0]);
}

int sendCmd(engineIO e, char *cmd, char *waitFor) {
    wprintw(logWin, " << %s\n", cmd);
    wrefresh(logWin);
    if (fprintf(e.w, "%s\n", cmd) < 0) {
        return -1;
    }
    if (waitFor == NULL) {
        return 0;
    }
    size_t nread;
    size_t waitL = strlen(waitFor);
    for (;;) {
        nread = getline(&e.buffer, &e.bufsize, e.r);
        if (nread < 0) {
            perror("failed to read from engine");
            return -1;
        }
        wprintw(logWin, " >> %s", e.buffer);
        wrefresh(logWin);
        if (nread > waitL && memcmp(e.buffer, waitFor, waitL) == 0) {
            return 0;
        }
    }
}

char *parseEngineMove(char *l) {
    size_t n = strlen("bestmove ");
    for (size_t i = n; i < n+7; i++) {
        if (l[i] == ' ') {
            l[i] = '\0';
            return l+n;
        }
    }
    return NULL;
}


void engineMove(engineIO e, board *b) {
    snprintf(e.buffer, e.bufsize, "position fen %s", b->fen);
    sendCmd(e, e.buffer, NULL);
    sendCmd(e, "go movetime 500", "bestmove");
    makeMove(parseEngineMove(e.buffer), b);
}

int main(int argc, char *argv[]) {
    char *uciCmd = argv[1];

    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);


    boardWin = newwin(20, COLS/2-1, 1, 1);
    refresh();

    board b;
    strncpy(b.fen, START_POS, 100);
    parseFEN(&b);
    b.flipped = false;

    displayBoard(boardWin, b);

    logWin = newwin(LINES/2, COLS/2-2, 1, COLS/2+1);
    scrollok(logWin, true);
    wattron(logWin, COLOR_PAIR(2));
    wrefresh(logWin);
    
    inputWin = newwin(2, COLS/2-1, 22, 1);
    //box(inputWin, 0, 0);
    wrefresh(inputWin);

    int enginePipes[2][2];
    startEngine(uciCmd, enginePipes);
    engineIO engine = {
        .r = fdopen(enginePipes[0][0], "r"),
        .w = fdopen(enginePipes[1][1], "w")
    };
    setvbuf(engine.w, NULL, _IOLBF, 0);   // enable line bufferring for engine write FD
    if (engine.w == NULL || engine.r == NULL) {
        perror("fdopen() failed");
        exit(1);
    }
    engine.buffer = malloc(1024);
    engine.bufsize = 1024;
    char input[5];

    sendCmd(engine, "uci", "uciok");
    sendCmd(engine, "ucinewgame", NULL);
    sendCmd(engine, "isready", "readyok");

    for (;;) {
        werase(inputWin);
        mvwprintw(inputWin, 0, 0, "Your move: ");
        //debugBoard(logWin, b);
        printd(logWin, "FEN: %s\n", b.fen);
        wgetnstr(inputWin, input, 6);
        if (makeMove(input, &b) < 0) {
            werase(inputWin);
            mvwprintw(inputWin, 0, 0, "Invalid move!");
            wrefresh(inputWin);
            sleep(1);
        }
        displayBoard(boardWin, b);
        engineMove(engine, &b);
        displayBoard(boardWin, b);
    }

    endwin();
    return 0;
}


#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

#include "fen.h"
#include "misc.h"
#include "pieces.h"


int main(int argc, char *argv[]) {
    char *uciCmd = argv[1];

    /*
    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);


    WINDOW *boardWin = newwin(20, COLS/2-1, 1, 1);
    refresh();

    board b;
    strncpy(b.fen, START_POS, 100);
    parseFEN(&b);

    displayBoard(boardWin, b);

    WINDOW *logWin = newwin(LINES/2, COLS/2-2, 1, COLS/2+1);
    scrollok(logWin, true);
    wattron(logWin, COLOR_PAIR(1));
    wrefresh(logWin);
    
    WINDOW *inputWin = newwin(2, COLS/2-1, 22, 1);
    box(inputWin, 0, 0);
    wrefresh(inputWin);
    */

    int enginePipes[2][2];
    if (pipe(enginePipes[0]) < 0) {   // reading from engine
        perror("creating pipe");
        return -1;
    }

    if (pipe(enginePipes[1]) < 0) {   // writing to engine
        perror("creating pipe");
        return -1;
    }

    if (fork() == 0) {
        printf("starting %s\n", uciCmd);
        dup2(enginePipes[0][1], STDOUT_FILENO); // attach output pipe to stdout
        dup2(enginePipes[0][1], STDERR_FILENO); // attach output pipe to stderr
        dup2(enginePipes[1][0], STDIN_FILENO);  // attach input pipe to stdin 
        close(enginePipes[0][0]);       // close read end of output pipe
        close(enginePipes[0][1]);       // close read end of output pipe
        close(enginePipes[1][0]);       // close write end of input pipe
        close(enginePipes[1][1]);       // close write end of input pipe
        execl(uciCmd, uciCmd, (char *)NULL);          // run engine
    }
    
    close(enginePipes[0][1]);
    close(enginePipes[1][0]);
    FILE *engineOutput = fdopen(enginePipes[0][0], "r");
    int engineCmd = enginePipes[1][1];
    //FILE *engineCmd = fdopen(enginePipes[1][1], "w");
    size_t bufsize = 1024;
    char *buffer = malloc(bufsize);
    int nread;

    for (;;) {
        write(engineCmd, "uci\n", 4);

        for (;;) {
            nread = getline(&buffer, &bufsize, engineOutput);
            if (nread < 0) {
                perror("reading from engine ");
                return 0;
            } else {
                //wprintw(logWin, buffer);
                printf("read %d bytes: %s\n", nread, buffer);
            }
        }
    }


        
    endwin();

    return 0;
}


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
    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);


    WINDOW *win = newwin(20, 36, 1, 1);
    refresh();

    board b;
    strncpy(b.fen, START_POS, 100);
    parseFEN(&b);

    box(win, 0, 0);
    displayBoard(win, b);
    wrefresh(win);

    WINDOW *win2 = newwin(10, 40, 5, 80);
    scrollok(win2, true);
    box(win2, 0, 0);
    wattron(win, COLOR_PAIR(1));
    wmove(win2, 1,1);
    for (int i = 0; i < 101; i++) {
        wprintw(win2, PAWN);
        wrefresh(win2);
        getch();
    }

    getch();
    endwin();

    return 0;
}


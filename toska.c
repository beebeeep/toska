#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "fen.h"
#include "misc.h"

int main(int argc, char *argv[]) {
    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    WINDOW *win = newwin(20, 36, 1, 1);
    refresh();

    board b;
    char *pos = malloc(100);
    strncpy(pos, START_POS, 100);
    fenToBoard(pos, &b);

    box(win, 0, 0);
    displayBoard(win, b);
    wrefresh(win);

    WINDOW *win2 = newwin(10, 40, 5, 80);
    scrollok(win2, true);
    box(win2, 0, 0);
    wattron(win, COLOR_PAIR(1));
    wmove(win2, 1,1);
    for (int i = 0; i < 101; i++) {
        wprintw(win2, "AAAA %d\n", random()%100);
        wrefresh(win2);
        getch();
    }

    getch();
    endwin();

    return 0;
}


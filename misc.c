#include <stdbool.h>
#include <ncurses.h>
#include <stdarg.h>

#include "chess.h"
#include "misc.h"

bool isBlack(char p) {
    return isPiece(p) && p > 'a';
}

bool isPiece(char p) {
    const char *pieces = "RNBQKPrnbqkp";
    for (int i = 0; i < 12; i++) {
        if (p == pieces[i]) {
            return true;
        }
    }
    return false;
}

void debugBoard(WINDOW *win, board b) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            wprintw(win, "%c", b.board[rank][file]);
        }
        wprintw(win, "\n");
    }
    wprintw(win, "\n--\n");
    wrefresh(win);
}

void printd(WINDOW *win, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vwprintw(win, fmt, args);
    va_end(args);
    wrefresh(win);
}

void drawBoard(WINDOW *win, bool flipped) {
    /* draws board without pieces:
    ┌───┬───┬───┬───┬───┬───┬───┬───┐               ┌───┬───┬───┬───┬───┬───┬───┬───┐
    │ 70│   │   │   │   │   │   │ 77│ 8             │ 07│   │   │   │   │   │   │ 00│ 1
    ├───┼───┼───┼───┼───┼───┼───┼───┤               ├───┼───┼───┼───┼───┼───┼───┼───┤
    │   │   │   │   │   │   │   │   │ 7             │   │   │   │   │   │   │   │   │ 2
    ├───┼───┼───┼───┼───┼───┼───┼───┤               ├───┼───┼───┼───┼───┼───┼───┼───┤
    │   │   │   │   │   │   │   │   │ 6             │   │   │   │   │   │   │   │   │ 3
    ├───┼───┼───┼───┼───┼───┼───┼───┤               ├───┼───┼───┼───┼───┼───┼───┼───┤
    │   │   │   │   │   │   │   │   │ 5             │   │   │   │   │   │   │   │   │ 4
    ├───┼───┼───┼───┼───┼───┼───┼───┤    or flipped ├───┼───┼───┼───┼───┼───┼───┼───┤
    │   │   │   │   │   │   │   │   │ 4             │   │   │   │   │   │   │   │   │ 5
    ├───┼───┼───┼───┼───┼───┼───┼───┤               ├───┼───┼───┼───┼───┼───┼───┼───┤
    │   │   │   │   │   │   │   │   │ 3             │   │   │   │   │   │   │   │   │ 6
    ├───┼───┼───┼───┼───┼───┼───┼───┤               ├───┼───┼───┼───┼───┼───┼───┼───┤
    │   │   │   │   │   │   │   │   │ 2             │   │   │   │   │   │   │   │   │ 7
    ├───┼───┼───┼───┼───┼───┼───┼───┤               ├───┼───┼───┼───┼───┼───┼───┼───┤
    │ 00│   │   │   │   │   │   │ 07│ 1             │ 77│   │   │   │   │   │   │ 70│ 8
    └───┴───┴───┴───┴───┴───┴───┴───┘               └───┴───┴───┴───┴───┴───┴───┴───┘
      a   b   c   d   e   f   g   h                   h   g   f   e   d   c   b   a
    */

    wattron(win, COLOR_PAIR(2));
    wattroff(win, A_BOLD);
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int x = file*4, y = rank*2;
            chtype ul = ACS_PLUS;
            chtype ur = ACS_PLUS;
            chtype ll = ACS_PLUS;
            chtype lr = ACS_PLUS;
            if (y == 0) {
                ul = ACS_TTEE;
                ur = ACS_TTEE;
            } else if (y == 7*2) {
                ll = ACS_BTEE;
                lr = ACS_BTEE;
                mvwaddch(win, y+3, x+2, flipped?('h' - file):('a' + file));
            }
            if (x == 0) {
                ul = ACS_LTEE;
                ll = ACS_LTEE;
            } else if (x == 4*7) {
                ur = ACS_RTEE;
                lr = ACS_RTEE;
                mvwaddch(win, y+1, x+6, flipped?('1' + rank):('8' - rank));
            }

            if (y == 0 && x == 0) {
                ul = ACS_ULCORNER;
            }
            if (y == 0 && x == 4*7) {
                ur = ACS_URCORNER;
            }
            if (y == 7*2 && x == 0) {
                ll = ACS_LLCORNER;
            }
            if (y == 7*2 && x == 4*7) {
                lr = ACS_LRCORNER;
            }

            /*	 01234
             * 0 +---+
             * 1 |   |
             * 2 +---+
             */

            mvwaddch(win, y, x, ul);
            mvwaddch(win, y+2, x, ll);
            mvwaddch(win, y, x+4, ur);
            mvwaddch(win, y+2, x+4, lr);

            mvwaddch(win, y, x+1, ACS_HLINE);
            mvwaddch(win, y, x+2, ACS_HLINE);
            mvwaddch(win, y, x+3, ACS_HLINE);
            mvwaddch(win, y+2, x+1, ACS_HLINE);
            mvwaddch(win, y+2, x+2, ACS_HLINE);
            mvwaddch(win, y+2, x+3, ACS_HLINE);
            mvwaddch(win, y+1, x, ACS_VLINE);
            mvwaddch(win, y+1, x+4, ACS_VLINE);
        }
    }
}

void displayBoard(WINDOW *win, board b) {
    wattron(win, COLOR_PAIR(2));
    drawBoard(win, b.flipped);
    wattron(win, A_BOLD);
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            char c = b.board[rank][file];
            if (c == 0) {
                c = ' ';
            }
            if ((rank+file)%2 == 0) {
                if (isBlack(c)) {
                    wattron(win, A_DIM | COLOR_PAIR(2));
                } else {
                    wattron(win, COLOR_PAIR(2));
                }
            } else {
                if (isBlack(c)) {
                    wattron(win, A_DIM | COLOR_PAIR(1));
                } else {
                    wattron(win, COLOR_PAIR(1));
                }
            }

            int x, y;
            if (b.flipped) {
                x = (7-file)*4+1;
                y = rank*2+1;
            } else {
                x = file*4+1;
                y = (7-rank)*2+1;
            }
            mvwprintw(win, y, x, " %c ", c);
            wattroff(win, A_DIM);
        }
    }

    wrefresh(win);
}


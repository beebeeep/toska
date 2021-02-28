#include <stdbool.h>
#include <ncurses.h>

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


void drawBoard(WINDOW *win) {
    /* draws board without pieces:
            ┌───┬───┬───┬───┬───┬───┬───┬───┐  
            │   │   │   │   │   │   │   │   │ 8
            ├───┼───┼───┼───┼───┼───┼───┼───┤  
            │   │   │   │   │   │   │   │   │ 7
            ├───┼───┼───┼───┼───┼───┼───┼───┤  
            │   │   │   │   │   │   │   │   │ 6
            ├───┼───┼───┼───┼───┼───┼───┼───┤  
            │   │   │   │   │   │   │   │   │ 5
            ├───┼───┼───┼───┼───┼───┼───┼───┤  
            │   │   │   │   │   │   │   │   │ 4
            ├───┼───┼───┼───┼───┼───┼───┼───┤  
            │   │   │   │   │   │   │   │   │ 3
            ├───┼───┼───┼───┼───┼───┼───┼───┤  
            │   │   │   │   │   │   │   │   │ 2
            ├───┼───┼───┼───┼───┼───┼───┼───┤  
            │   │   │   │   │   │   │   │   │ 1
            └───┴───┴───┴───┴───┴───┴───┴───┘  
              a   b   c   d   e   f   g   h
    * TODO flip board so player's pieces are always at the bottom    
    */

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
                mvwaddch(win, y+3, x+2, 'a' + file);
            }
            if (x == 0) {
                ul = ACS_LTEE;
                ll = ACS_LTEE;
            } else if (x == 4*7) {
                ur = ACS_RTEE;
                lr = ACS_RTEE;
                mvwaddch(win, y+1, x+6, '1' + rank);
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
    drawBoard(win);
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

            mvwprintw(win, 1+rank*2, file*4+1, " %c ", c);
            wattroff(win, A_DIM);
        }
    }
    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, 8*2+3, 0, "%s", b.fen);

    wrefresh(win);
}


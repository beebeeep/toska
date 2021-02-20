#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>


void fenToBoard(char *fen, char board[8][8]) {
    int file = 0, rank = 0;
    for (int pos = 0; fen[pos] != ' '; pos++) {
        char c = fen[pos];
        if (c == '/') {
            rank++;
            file = 0;
            continue;
        }
        if (c >= '1' && c <= '9') {
            for (int i = 0 ; i < c - '0'; i++) {
                board[rank][file++] = ' ' ;
            }
            continue;
        }
        board[rank][file] = c;
        file++;
    }
}

bool isBlack(char p) {
    return p == 'r' || p == 'n' || p == 'b' || p == 'q' || p == 'k' || p == 'p';
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

void displayBoard(WINDOW *win, char board[8][8]) {
    wattron(win, COLOR_PAIR(2));
    drawBoard(win);
    wattron(win, A_BOLD);
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            char c = board[rank][file];
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
}
int main(int argc, char *argv[]) {
    initscr();
    cbreak();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    WINDOW *win = newwin(20, 36, 1, 1);
    refresh();

    char board[8][8];
    fenToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", board);

    box(win, 0, 0);
    displayBoard(win, board);
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


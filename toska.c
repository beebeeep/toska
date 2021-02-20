#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <time.h>

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
    */
    /*
     
            #####     #####     #####     #####     
            #####     #####     #####     #####     
            #####     #####     #####     #####     
       #####     #####     #####     #####     
       #####     #####     #####     #####     
       #####     #####     #####     #####     
            #####     #####     #####     #####     
            #####     #####     #####     #####     
            #####     #####     #####     #####     
       #####     #####     #####     #####     
       #####     #####     #####     #####     
       #####     #####     #####     #####     
            #####     #####     #####     #####     
            #####     #####     #####     #####     
            #####     #####     #####     #####     
       #####     #####     #####     #####     
       #####     #####     #####     #####     
       #####     #####     #####     #####     
            #####     #####     #####     #####     
            #####     #####     #####     #####     
            #####     #####     #####     #####     
       #####     #####     #####     #####     
       #####     #####     #####     #####     
       #####     #####     #####     #####     
    */

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            int x = file*4, y = (7-rank)*2;    // TODO flip board so player's pieces are always at the bottom
            chtype ul = ACS_PLUS;
            chtype ur = ACS_PLUS;
            chtype ll = ACS_PLUS;
            chtype lr = ACS_PLUS;
            if (y == 0) {
                ul = ACS_TTEE;
                ur = ACS_TTEE;
                mvwaddch(win, y+3, x+2, 'a' + file);
            } else if (y == 7*2) {
                ll = ACS_BTEE;
                lr = ACS_BTEE;
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

            mvwprintw(win, y+1, x+1, "%c%c", 'a'+file, '1'+rank);
        }
    }
}

void displayBoard(char board[8][8]) {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    WINDOW *win = stdscr;

    attron(COLOR_PAIR(3));
    // ┐┌┘└├┤┴┬│─┼
    /*
    mvwprintw(win, 0, 0, 
            );

    */
    drawBoard(win);
    return;
    attron(A_BOLD);
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            char c = board[rank][file];
            if (c == 0) {
                c = ' ';
            }
            if ((rank+file)%2 == 0) {
                if (c > 'a' && c < 'z') {
                    attron(A_DIM | COLOR_PAIR(4));
                } else {
                    attron(COLOR_PAIR(3));
                }
            } else {
                if (c > 'a' && c < 'z') {
                    attron(A_DIM | COLOR_PAIR(2));
                } else {
                    attron(COLOR_PAIR(1));
                }
            }
            mvwprintw(win, 1+rank*2, file*4+1, " %c ", c);
            attroff(A_DIM);

        }
    }
    refresh();
}
int main(int argc, char *argv[]) {
    char board[8][8];


    fenToBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", board);
    initscr();
    displayBoard(board);
    getch();
    endwin();


    return 0;
}


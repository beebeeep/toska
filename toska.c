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
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            chtype ul = ACS_PLUS;
            chtype ur = ACS_PLUS;
            chtype ll = ACS_PLUS;
            chtype lr = ACS_PLUS;
            if (rank == 0) {
                if (file == 0) {
                    ul = ACS_ULCORNER;
                }
                if (file == 7) {
                    ur = ACS_UR_CORNER;
                }
            }
            if (rank == 7) {
                if (file == 0) {
                    ul = ACS_LLCORNER;
                }
                if (file == 7) {
                    ur = ACS_LR_CORNER;
                }
            }
            mvwaddch(win, 




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
            "┌───┬───┬───┬───┬───┬───┬───┬───┐  \n"
            "│   │   │   │   │   │   │   │   │ 8\n"
            "├───┼───┼───┼───┼───┼───┼───┼───┤  \n"
            "│   │   │   │   │   │   │   │   │ 7\n"
            "├───┼───┼───┼───┼───┼───┼───┼───┤  \n"
            "│   │   │   │   │   │   │   │   │ 6\n"
            "├───┼───┼───┼───┼───┼───┼───┼───┤  \n"
            "│   │   │   │   │   │   │   │   │ 5\n"
            "├───┼───┼───┼───┼───┼───┼───┼───┤  \n"
            "│   │   │   │   │   │   │   │   │ 4\n"
            "├───┼───┼───┼───┼───┼───┼───┼───┤  \n"
            "│   │   │   │   │   │   │   │   │ 3\n"
            "├───┼───┼───┼───┼───┼───┼───┼───┤  \n"
            "│   │   │   │   │   │   │   │   │ 2\n"
            "├───┼───┼───┼───┼───┼───┼───┼───┤  \n"
            "│   │   │   │   │   │   │   │   │ 1\n"
            "└───┴───┴───┴───┴───┴───┴───┴───┘  \n"
            "  a   b   c   d   e   f   g   h    \n"
            );

    */
    draw_board(win);
    waddch(win, ACS_URCORNER);
    waddch(win, ACS_ULCORNER);
    waddch(win, ACS_LRCORNER);
    waddch(win, ACS_LLCORNER);
    waddch(win, ACS_LTEE);
    waddch(win, ACS_RTEE);
    waddch(win, ACS_BTEE);
    waddch(win, ACS_TTEE);
    waddch(win, ACS_VLINE);
    waddch(win, ACS_HLINE);
    waddch(win, ACS_PLUS);
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


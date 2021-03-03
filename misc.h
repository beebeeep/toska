#ifndef MISC_H
#define MISC_H

#include <ncurses.h>

bool isBlack(char p);
bool isPiece(char p);
void drawBoard(WINDOW *win, bool flipped);
void displayBoard(WINDOW *win, board b);
void debugBoard(WINDOW *win, board b);
void printd(WINDOW *win, char *fmt, ...);

#endif

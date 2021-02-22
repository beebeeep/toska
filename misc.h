#ifndef MISC_H
#define MISC_H

#include <ncurses.h>

bool isBlack(char p);
bool isPiece(char p);
void drawBoard(WINDOW *win);
void drawBoard(WINDOW *win);
void displayBoard(WINDOW *win, board b);

#endif

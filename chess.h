#ifndef CHESS_H
#define CHESS_H

#include <stdbool.h>

#define CAST_WHITE_KSIDE (1 << 0)
#define CAST_WHITE_QSIDE (1 << 1)
#define CAST_BLACK_KSIDE (1 << 2)
#define CAST_BLACK_QSIDE (1 << 3)
#define START_POS "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

typedef struct {
    char fen[100];
    char board[8][8];
    bool blackMove;
    unsigned char casting;
    unsigned int enPassant;
    unsigned int halfMoveClock;
    unsigned int move;
} board;

void parseFEN(board *b);
void updateFEN(board *b);

void getPlacement(char *t, board *b);
void getActiveColor(char *t, board *b);
void getCasting(char *t, board *b);
void getEnPassant(char *t, board *b);
void getHalfMoveClock(char *t, board *b);
void getMoveNumber(char *t, board *b);
int makeMove(char move[6], board *b);



#endif

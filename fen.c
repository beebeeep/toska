#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "fen.h"

void fenToBoard(char *fen, board *b) {
    int fieldNum = 0;
    char *field;
    char *last;
    for ((field = strtok_r(fen, " ", &last)); field; (field = strtok_r(NULL, " ", &last))) {
        switch(++fieldNum) {
            case 1:
                getPlacement(field, b);
                break;
            case 2:
                getActiveColor(field, b);
                break;
            case 3:
                getCasting(field, b);
                break;
            case 4:
                getEnPassant(field, b);
                break;
            case 5:
                getHalfMoveClock(field, b);
                break;
            case 6:
                getMoveNumber(field, b);
                break;
        }
    }
}

void boardToFen(board b, char *fen) {
    int pos = 0;
    bzero(fen, strlen(fen));
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            pos++;
        }
    }
}

void getPlacement(char *t, board *b) {
    int file = 0, rank = 0;
    for (int pos = 0; pos < strlen(t); pos++) {
        char c = t[pos];
        if (c == '/') {
            rank++;
            file = 0;
            continue;
        }
        if (c >= '1' && c <= '9') {
            for (int i = 0 ; i < c - '0'; i++) {
                b->board[rank][file++] = ' ' ;
            }
            continue;
        }
        b->board[rank][file] = c;
        file++;
    }
}

void getActiveColor(char *t, board *b) {
    if (t[0] == 'w') {
        b->blackMove = false;
    } else if (t[0] == 'b') {
        b->blackMove = true;
    }
}

void getCasting(char *t, board *b) {
    for (int i = 0; i < strlen(t); i++) {
        switch (t[i]) {
            case 'K':
                b->casting |= CAST_WHITE_KSIDE;
                break;
            case 'Q':
                b->casting |= CAST_WHITE_QSIDE;
                break;
            case 'k':
                b->casting |= CAST_BLACK_KSIDE;
                break;
            case 'q':
                b->casting |= CAST_BLACK_QSIDE;
                break;
        }
    }
}

void getEnPassant(char *t, board *b) {
    b->enPassant = malloc(strlen(t));
    strcpy(b->enPassant, t);
}

void getHalfMoveClock(char *t, board *b) {
    b->halfMoveClock = strtonum(t, 0, 51, NULL);
}

void getMoveNumber(char *t, board *b) {
    b->move = strtonum(t, 0, INT_MAX, NULL);
}

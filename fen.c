#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "fen.h"
#include "misc.h"

void parseFEN(board *b) {
    int fieldNum = 0;
    char *field;
    char *last;
    for ((field = strtok_r(b->fen, " ", &last)); field; (field = strtok_r(NULL, " ", &last))) {
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

void updateFEN(board *b) {
    int pos = 0;
    memset(b->fen, 0, 100);
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            char c = b->board[rank][file];
            if (isPiece(c)) {
                b->fen[pos++] = c;
                continue;
            }
            if (c == ' ') {
                if (c >= '1' && c < '9') {
                    b->fen[pos]++;
                } else {
                    b->fen[pos] = '1';
                }
            }
        }
        b->fen[pos++] = '/';
    }
    b->fen[pos++] = ' ';
    if (b->blackMove) {
        b->fen[pos++] = 'b';
    } else {
        b->fen[pos++] = 'w';
    }
    b->fen[pos++] = ' ';
    if (b->enPassant == 0) {
        b->fen[pos++] = '-';
    } else {
        b->fen[pos++] = 'a' + b->enPassant/8;
        b->fen[pos++] = '1'  + b->enPassant%8;
    }
    b->fen[pos++] = ' ';
    sprintf(&b->fen[pos], "%d %d", b->halfMoveClock, b->move);
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
}

void getHalfMoveClock(char *t, board *b) {
    b->halfMoveClock = atoi(t);
}

void getMoveNumber(char *t, board *b) {
    b->move = atoi(t);
}

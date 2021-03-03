#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "chess.h"
#include "misc.h"

void parseFEN(board *b) {
    int fieldNum = 0;
    char *field;
    char *last;
    char *fen = malloc(strlen(b->fen));
    strncpy(fen, b->fen, strlen(b->fen));
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
    free(fen);
}

void updateFEN(board *b) {
    int pos = 0;
    memset(b->fen, 0, 100);
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            char c = b->board[rank][file];
            if (isPiece(c)) {
                b->fen[pos++] = c;
                continue;
            }
            if (c == ' ') {
                b->fen[pos] = '0';
                while (b->board[rank][file] == ' ' && file < 8 ) {
                    b->fen[pos]++;
                    file++;
                }
                file--;
                pos++;
            }
        }
        if (rank != 0) {
            b->fen[pos++] = '/';
        }
    }
    b->fen[pos++] = ' ';
    // active color
    if (b->blackMove) {
        b->fen[pos++] = 'b';
    } else {
        b->fen[pos++] = 'w';
    }
    b->fen[pos++] = ' ';
    // casting
    if (b->casting | CAST_WHITE_KSIDE) {
        b->fen[pos++] = 'K';
    }
    if (b->casting | CAST_WHITE_QSIDE) {
        b->fen[pos++] = 'Q';
    }
    if (b->casting | CAST_BLACK_KSIDE) {
        b->fen[pos++] = 'k';
    }
    if (b->casting | CAST_BLACK_QSIDE) {
        b->fen[pos++] = 'q';
    }
    b->fen[pos++] = ' ';
    // en passant
    if (b->enPassant == 0) {
        b->fen[pos++] = '-';
    } else {
        b->fen[pos++] = 'a' + b->enPassant/8;
        b->fen[pos++] = '1' + b->enPassant%8;
    }

    //clock and move
    b->fen[pos++] = ' ';
    sprintf(&b->fen[pos], "%d %d", b->halfMoveClock, b->move);
}

void getPlacement(char *t, board *b) {
    int file = 0, rank = 7;
    for (int pos = 0; pos < strlen(t); pos++) {
        char c = t[pos];
        if (c == '/') {
            rank--;
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
    if (strlen(t) == 2) {
        b->enPassant = 8*(t[1] - '1') + (t[0]-'a');
    } else {
        b->enPassant = 0;
    }
}

void getHalfMoveClock(char *t, board *b) {
    b->halfMoveClock = atoi(t);
}

void getMoveNumber(char *t, board *b) {
    b->move = atoi(t);
}

bool isValidMove(char move[6], board *b) {
    return true;
}

int makeMove(char move[6], board *b) {
    int rank1, file1, rank2, file2;
    if (!isValidMove(move, b)) {
        return -1;
    }
    if (strlen(move) == 4) {
        file1 = move[0] - 'a';
        rank1 = move[1] - '1';
        file2 = move[2] - 'a';
        rank2 = move[3] - '1';
        b->board[rank2][file2] = b->board[rank1][file1];
        b->board[rank1][file1] = ' ';
        b->blackMove = !b->blackMove;
        if (!b->blackMove) {
            b->move++;
        }
        updateFEN(b);
    } else {
        // todo check casting
        return -1;
    }
    return 0;
}


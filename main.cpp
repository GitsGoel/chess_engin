    // gitanshgoelcc
    #include <bits/stdc++.h>
    using namespace std;
    const uint64_t A_FILE = 0x0101010101010101;
    const uint64_t B_FILE = 0x0202020202020202;
    const uint64_t C_FILE = 0x0404040404040404;
    const uint64_t D_FILE = 0x0808080808080808;
    const uint64_t E_FILE = 0x1010101010101010;
    const uint64_t F_FILE = 0x2020202020202020;
    const uint64_t G_FILE = 0x4040404040404040;
    const uint64_t H_FILE = 0x8080808080808080;

    const uint64_t RANK_8 = 0x00000000000000FF;
    const uint64_t RANK_7 = 0x000000000000FF00;
    const uint64_t RANK_6 = 0x0000000000FF0000;
    const uint64_t RANK_5 = 0x00000000FF000000;
    const uint64_t RANK_4 = 0x000000FF00000000;
    const uint64_t RANK_3 = 0x0000FF0000000000;
    const uint64_t RANK_2 = 0x00FF000000000000;
    const uint64_t RANK_1 = 0xFF00000000000000;

    const int dir1[4] = {8, -8, 1, -1}; 
    const int diag[4] = {9, -9, 7, -7}; 
    const int DIRECTIONS_KING[8] = {8, 9, 1, -7, -8, -9, -1, 7}; // N, NE, E, SE, S, SW, W, NW


    const int PIECE_PAWN = 0;
    const int PIECE_ROOK = 1;
    const int PIECE_BISHOP = 2; 
    const int PIECE_QUEEN = 3;
    const int PIECE_KNIGHT = 4;
    const int PIECE_KING = 5;
    const int PIECE_NONE = 6;

    const uint8_t COLOR_WHITE = 0x8;
    const uint8_t COLOR_BLACK = 0;
    const uint8_t PIECE_MASK = 0x7;
    const uint8_t COLOR_MASK = 0x8;

    struct Move {
        bool isWhite;          // True if white, false if black
        int pieceType;         // 0: Pawn, 1: Rook, 2: Bishop, 3: Queen, 4: Knight, 5: King
        int startSquare;       // 0-63
        int endSquare;         // 0-63
        int pieceCaptured;     // -1: None, otherwise piece type
        char promotion;        // 'Q', 'R', 'B', 'N' for pawn promotions, '-' otherwise

        Move() : isWhite(true), pieceType(-1), startSquare(-1), endSquare(-1), pieceCaptured(-1), promotion('-') {}
        Move(bool iw, int pt, int ss, int es, int pc, char promo = '-') 
            : isWhite(iw), pieceType(pt), startSquare(ss), endSquare(es), pieceCaptured(pc), promotion(promo) {}
    };

    class Board { 
    private:
        // white-centric
        uint64_t wp = 0;
        uint64_t wB = 0;
        uint64_t wK = 0;
        uint64_t wR = 0;
        uint64_t wKg = 0;
        uint64_t wQ = 0;
        uint64_t wpc = 0;

        uint64_t bp = 0;
        uint64_t bB = 0;
        uint64_t bK = 0;
        uint64_t bR = 0;
        uint64_t bKg = 0;
        uint64_t bQ = 0;
        uint64_t bpc = 0;
        
        int knight_pst[64] = {
            -167, -89, -34, -49,  61, -97, -15, -107,
            -73, -41,  72,  36,  23,  62,   7,  -17,
            -47,  60,  37,  65,  84, 129,  73,   44,
            -9,  17,  19,  53,  37,  69,  18,   22,
            -13,   4,  16,  13,  28,  19,  21,   -8,
            -23,  -9,  12,  10,  19,  17,  25,  -16,
            -29, -53, -12,  -3,  -1,  18, -14,  -19,
            -105, -21, -58, -33, -17, -28, -19,  -23,
        };

        int pawn_pst[64] = {
            0,   0,   0,   0,   0,   0,  0,   0,
            98, 134,  61,  95,  68, 126, 34, -11,
            -6,   7,  26,  31,  65,  56, 25, -20,
            -14,  13,   6,  21,  23,  12, 17, -23,
            -27,  -2,  -5,  12,  17,   6, 10, -25,
            -26,  -4,  -4, -10,   3,   3, 33, -12,
            -35,  -1, -20, -23, -15,  24, 38, -22,
            0,   0,   0,   0,   0,   0,  0,   0,
        };

        int bishop_pst[64] = {
            -29,   4, -82, -37, -25, -42,   7,  -8,
            -26,  16, -18, -13,  30,  59,  18, -47,
            -16,  37,  43,  40,  35,  50,  37,  -2,
            -4,   5,  19,  50,  37,  37,   7,  -2,
            -6,  13,  13,  26,  34,  12,  10,   4,
            0,  15,  15,  15,  14,  27,  18,  10,
            4,  15,  16,   0,   7,  21,  33,   1,
            -33,  -3, -14, -21, -13, -12, -39, -21,
        };

        int rook_pst[64] = {
            32,  42,  32,  51, 63,  9,  31,  43,
            27,  32,  58,  62, 80, 67,  26,  44,
            -5,  19,  26,  36, 17, 45,  61,  16,
            -24, -11,   7,  26, 24, 35,  -8, -20,
            -36, -26, -12,  -1,  9, -7,   6, -23,
            -45, -25, -16, -17,  3,  0,  -5, -33,
            -44, -16, -20,  -9, -1, 11,  -6, -71,
            -19, -13,   1,  17, 16,  7, -37, -26,
        };
        
        int queen_pst[64] = {
            -28,   0,  29,  12,  59,  44,  43,  45,
            -24, -39,  -5,   1, -16,  57,  28,  54,
            -13, -17,   7,   8,  29,  56,  47,  57,
            -27, -27, -16, -16,  -1,  17,  -2,   1,
            -9, -26,  -9, -10,  -2,  -4,   3,  -3,
            -14,   2, -11,  -2,  -5,   2,  14,   5,
            -35,  -8,  11,   2,   8,  15,  -3,   1,
            -1, -18,  -9,  10, -15, -25, -31, -50,
        };
        
        int king_pst[64] = {
            -65,  23,  16, -15, -56, -34,   2,  13,
            29,  -1, -20,  -7,  -8,  -4, -38, -29,
            -9,  24,   2, -16, -20,   6,  22, -22,
            -17, -20, -12, -27, -30, -25, -14, -36,
            -49,  -1, -27, -39, -46, -44, -33, -51,
            -14, -14, -22, -46, -44, -30, -15, -27,
            1,   7,  -8, -64, -43, -16,   9,   8,
            -15,  36,  12, -54,   8, -28,  24,  14,
        };

        int* psts[6] = {
            pawn_pst,
            rook_pst,
            bishop_pst,
            queen_pst,
            knight_pst,
            king_pst
        };

        bool whiteCanCastleKingSide;
        bool whiteCanCastleQueenSide;
        bool blackCanCastleKingSide;
        bool blackCanCastleQueenSide;
        int enPassantSquare; 
        int halfmoveClock;
        int fullmoveNumber;

        uint8_t allPieces[64];

        uint64_t knight_attacks[64] = {
            0x20400ULL, 0x50800ULL, 0xa1100ULL, 0x142200ULL, 0x284400ULL, 0x508800ULL, 0xa01000ULL, 0x402000ULL,
            0x2040004ULL, 0x5080008ULL, 0xa110011ULL, 0x14220022ULL, 0x28440044ULL, 0x50880088ULL, 0xa0100010ULL, 0x40200020ULL,
            0x204000402ULL, 0x508000805ULL, 0xa1100110aULL, 0x1422002214ULL, 0x2844004428ULL, 0x5088008850ULL, 0xa0100010a0ULL, 0x4020002040ULL,
            0x20400040200ULL, 0x50800080500ULL, 0xa1100110a00ULL, 0x142200221400ULL, 0x284400442800ULL, 0x508800885000ULL, 0xa0100010a000ULL, 0x402000204000ULL,
            0x2040004020000ULL, 0x5080008050000ULL, 0xa1100110a0000ULL, 0x14220022140000ULL, 0x28440044280000ULL, 0x50880088500000ULL, 0xa0100010a00000ULL, 0x40200020400000ULL,
            0x204000402000000ULL, 0x508000805000000ULL, 0xa1100110a000000ULL, 0x1422002214000000ULL, 0x2844004428000000ULL, 0x5088008850000000ULL, 0xa0100010a0000000ULL, 
            0x4020002040000000ULL, 0x400040200000000ULL, 0x800080500000000ULL, 0x1100110a00000000ULL, 0x2200221400000000ULL, 0x4400442800000000ULL, 0x8800885000000000ULL,  0x100010a000000000ULL,
            0x2000204000000000ULL, 0x4020000000000ULL, 0x8050000000000ULL, 0x110a0000000000ULL, 0x22140000000000ULL, 0x44280000000000ULL, 0x88500000000000ULL, 0x10a00000000000ULL, 0x20400000000000ULL
        };

        string indexToAlgebraic(int index) {
            if(index < 0 || index > 63) return "??";
            char file = 'a' + (index % 8);
            char rank = '1' + 7-(index / 8);
            return string(1, file) + string(1, rank);
        }


    public:
        Move bestMove;

        int moveSearched = 0;

        Board() {}

        void printGame();

        void setupGameFromFEN(string moveSet);

        void outputFen();

        void generatePieceMoves(bool isWhite, Move moveArray[], int &moveCount);

        void printMoves(Move moveArray[], int moveCount);

        long perft(int depth, bool isWhite, int initialDepth);
        
        void make_move(Move& mv);

        void unmake_move(Move & mv);

        void order_moves(Move *moves, int moveCount);

        int evaluateBoard();

        int alphaBeta(bool isWhite, int depth, int alpha, int beta);

        long perft2(int depth, bool isWhite);

        bool isSquareAttacked(int square, bool byWhite);

        bool isInCheck(bool isWhite);

        bool isMoveLegal(Move &mv);

        bool play(string moveStr);


    };

    int initialDepth = 7;

    void Board::setupGameFromFEN(string moveSet) {
        wp = wR = wB = wK = wQ = wKg = 0;
        bp = bR = bB = bK = bQ = bKg = 0;
        
        for(int i = 0; i < 64; i++) allPieces[i] = PIECE_NONE;

        int i = 0, j = 0, index = 0;
        for(char c: moveSet) {
            index++;
            int set = i*8 + j;
            if(c == ' ') break;
            if(c == '/') {
                i++;
                j = 0;
                continue;
            }
            else if(c == 'r') {
                bR |= (1LL << set);
                allPieces[set] = COLOR_BLACK | PIECE_ROOK;
            }
            else if(c == 'n') {
                bK |= (1LL << set);
                allPieces[set] = COLOR_BLACK | PIECE_KNIGHT;
            }
            else if(c == 'b') {
                bB |= (1LL << set);
                allPieces[set] = COLOR_BLACK | PIECE_BISHOP;
            }
            else if(c == 'q') {
                bQ |= (1LL << set);
                allPieces[set] = COLOR_BLACK | PIECE_QUEEN;
            }
            else if(c == 'k') {
                bKg |= (1LL << set);
                allPieces[set] = COLOR_BLACK | PIECE_KING;
            }
            else if(c == 'p') {
                bp |= (1LL << set);
                allPieces[set] = COLOR_BLACK | PIECE_PAWN;
            }
            else if(c == 'R') {
                wR |= (1LL << set);
                allPieces[set] = COLOR_WHITE | PIECE_ROOK;
            }
            else if(c == 'N') {
                wK |= (1LL << set);
                allPieces[set] = COLOR_WHITE | PIECE_KNIGHT;
            }
            else if(c == 'B') {
                wB |= (1LL << set);
                allPieces[set] = COLOR_WHITE | PIECE_BISHOP;
            }
            else if(c == 'Q') {
                wQ |= (1LL << set);
                allPieces[set] = COLOR_WHITE | PIECE_QUEEN;
            }
            else if(c == 'K') {
                wKg |= (1LL << set);
                allPieces[set] = COLOR_WHITE | PIECE_KING;
            }
            else if(c == 'P') {
                wp |= (1LL << set);
                allPieces[set] = COLOR_WHITE | PIECE_PAWN;
            }
            else if(isdigit(c)) {
                j += (c - '0') - 1;
            }
            j++;
        }
        wpc = wB | wKg | wp | wK | wQ | wR;
        bpc = bB | bKg | bp | bK | bQ | bR;
        
        index += 2; 
        
        whiteCanCastleKingSide = whiteCanCastleQueenSide = false;
        blackCanCastleKingSide = blackCanCastleQueenSide = false;
        
        while(moveSet[index] != ' ') {
            switch(moveSet[index]) {
                case 'K': whiteCanCastleKingSide = true; break;
                case 'Q': whiteCanCastleQueenSide = true; break;
                case 'k': blackCanCastleKingSide = true; break;
                case 'q': blackCanCastleQueenSide = true; break;
                case '-': break;
            }
            index++;
        }
        index++; 

        if(moveSet[index] == '-') {
            enPassantSquare = -1;
            index++;
        } else {
            int file = moveSet[index] - 'a';
            int rank = '8' - moveSet[index + 1];
            enPassantSquare = rank * 8 + file;
            index += 2;
        }
        index++; 
        
        halfmoveClock = 0;
        while(moveSet[index] != ' ') {
            halfmoveClock = halfmoveClock * 10 + (moveSet[index] - '0');
            index++;
        }
        index++; 
        fullmoveNumber = 0;
        while(index < moveSet.length() && isdigit(moveSet[index])) {
            fullmoveNumber = fullmoveNumber * 10 + (moveSet[index] - '0');
            index++;
        }

    }

    void Board::printGame(){
        for(int i = 0; i < 8; i++){
            cout << 8-i << "  ";
            for(int j = 0; j < 8; j++){
                int mask = i*8 + j;
                string piece = ".";
                if((wQ >> mask) & 1) piece = "WQ";
                else if((wp >> mask) & 1) piece = "wp";
                else if((wB >> mask) & 1) piece = "WB";
                else if((wKg >> mask) & 1) piece = "WKg";
                else if((wR >> mask) & 1) piece = "WR";
                else if((wK >> mask) & 1) piece = "Wk";

                else if((bQ >> mask) & 1) piece = "BK";
                else if((bp >> mask) & 1) piece = "bp";
                else if((bB >> mask) & 1) piece = "BB";
                else if((bKg >> mask) & 1) piece = "BKg";
                else if((bR >> mask) & 1) piece = "BR";
                else if((bK >> mask) & 1) piece = "Bk";

                cout << piece << " ";
            }
            cout << "\n";
        }
        cout << "\n   A B C D E F G H\n";
    }

    void Board::outputFen(){
        string fen = "";

        // 1. Piece Placement
        for(int rank = 0; rank < 8; rank++) { // From rank 8 to 1
            int emptyCount = 0;
            for(int file = 0; file < 8; file++) { // From file a to h
                int square = (rank) * 8 + file;
                char piece = '-';

                if(wp & (1ULL << square)) piece = 'P';
                else if(wR & (1ULL << square)) piece = 'R';
                else if(wB & (1ULL << square)) piece = 'B';
                else if(wK & (1ULL << square)) piece = 'N';
                else if(wQ & (1ULL << square)) piece = 'Q';
                else if(wKg & (1ULL << square)) piece = 'K';

                else if(bp & (1ULL << square)) piece = 'p';
                else if(bR & (1ULL << square)) piece = 'r';
                else if(bB & (1ULL << square)) piece = 'b';
                else if(bK & (1ULL << square)) piece = 'n';
                else if(bQ & (1ULL << square)) piece = 'q';
                else if(bKg & (1ULL << square)) piece = 'k';

                if(piece == '-') {
                    emptyCount++;
                }
                else {
                    if(emptyCount > 0) {
                        fen += to_string(emptyCount);
                        emptyCount = 0;
                    }
                    fen += piece;
                }
            }
            if(emptyCount > 0) {
                fen += to_string(emptyCount);
            }
            if(rank != 7) {
                fen += '/';
            }
        }
        cout << fen << "\n";
    }

    void Board::generatePieceMoves(bool isWhite, Move moveArray[], int &moveCount) {

        moveCount = 0;

        uint64_t ownPieces = isWhite ? wpc : bpc;
        uint64_t enemyPieces = isWhite ? bpc : wpc;

        uint64_t knights = isWhite ? wK : bK;
        while(knights) {
            int single_knight = __builtin_ctzll(knights);
            knights &= knights - 1;
            uint64_t knight_attack_bb = knight_attacks[single_knight];
            while(knight_attack_bb) {
                int target = __builtin_ctzll(knight_attack_bb);
                knight_attack_bb &= knight_attack_bb - 1;
                uint64_t targetMask = (1ULL << target);
                if(ownPieces & targetMask) continue;
                if(allPieces[target] != PIECE_NONE && ((allPieces[target] & COLOR_MASK) != (isWhite ? COLOR_WHITE : COLOR_BLACK))) {
                    moveArray[moveCount++] = Move(isWhite, PIECE_KNIGHT, single_knight, target, (allPieces[target] & PIECE_MASK), '-');
                } else {
                    moveArray[moveCount++] = Move(isWhite, PIECE_KNIGHT, single_knight, target, PIECE_NONE, '-');
                }
            }
        }

        uint64_t rooks = isWhite ? wR : bR;
        while(rooks && moveCount < 1024) {
            int single_rook = __builtin_ctzll(rooks);
            rooks &= rooks-1;
            for(int dir = 0; dir < 4; dir++) {
                int target = single_rook;
                while(true) {
                    target += dir1[dir];
                    if(target < 0 || target > 63) break;
                    if(dir1[dir] == 1 && (target % 8 == 0)) break; // right out-of-board
                    if(dir1[dir] == -1 && ((target+1) % 8 == 0)) break; // left out-of-board
                    uint64_t targetMask = 1ULL << target;
                    if(ownPieces & targetMask) {
                        break;
                    } 
                    if(allPieces[target] != PIECE_NONE && ((allPieces[target] & COLOR_MASK) != (isWhite ? COLOR_WHITE : COLOR_BLACK))) {
                        moveArray[moveCount++] = Move(isWhite, PIECE_ROOK, single_rook, target, (allPieces[target] & PIECE_MASK), '-');
                        break;
                    } else {
                        moveArray[moveCount++] = Move(isWhite, PIECE_ROOK, single_rook, target, PIECE_NONE, '-');
                    }
                    if(moveCount >= 1024) break;
                }
            }
        }

        uint64_t bishops = isWhite ? wB : bB;
        while(bishops && moveCount < 1024) {
            int single_bishop = __builtin_ctzll(bishops);
            bishops &= bishops - 1; 
            for(int dir = 0; dir < 4; dir++) {
                int target = single_bishop;
                while(true) {
                    target += diag[dir];
                    if(target < 0 || target > 63) break;
                    if((diag[dir] == 9 || diag[dir] == -7) && (target % 8 == 0)) break; // top-right wrapping
                    if((diag[dir] == -9 || diag[dir] == 7) && ((target + 1) % 8 == 0)) break; // top-left wrapping
                    uint64_t targetMask = 1ULL << target;
                    if(ownPieces & targetMask) {
                        break;
                    } 
                    else if(allPieces[target] != PIECE_NONE && ((allPieces[target] & COLOR_MASK) != (isWhite ? COLOR_WHITE: COLOR_BLACK))) {
                        moveArray[moveCount++] = Move(isWhite, PIECE_BISHOP, single_bishop, target, (allPieces[target] & PIECE_MASK), '-');
                        break;
                    } else {
                        moveArray[moveCount++] = Move(isWhite, PIECE_BISHOP, single_bishop, target, PIECE_NONE, '-');
                    }
                    if(moveCount >= 1024) break;
                }
            }
        }

        const int DIRECTIONS_QUEEN[8] = {8, -8, 1, -1, 9, -7, -9, 7}; 

        uint64_t queens = isWhite ? wQ : bQ;
        while(queens && moveCount < 1024) {
            int queenSquare = __builtin_ctzll(queens);
            queens &= queens - 1;
            for(int dir = 0; dir < 8; dir++) {
                int target = queenSquare;
                while(true) {
                    target += DIRECTIONS_QUEEN[dir];
                    if(target < 0 || target > 63) break;
                    if((DIRECTIONS_QUEEN[dir] == 1 && (target % 8 == 0)) || // right wrapping
                    (DIRECTIONS_QUEEN[dir] == -1 && ((target + 1) % 8 == 0)) || // left wrapping
                    (DIRECTIONS_QUEEN[dir] == 9 && (target % 8 == 0)) ||  // NE wrapping
                    (DIRECTIONS_QUEEN[dir] == -7 && (target % 8 == 0)) || // NE wrapping
                    (DIRECTIONS_QUEEN[dir] == -9 && ((target + 1) % 8 == 0)) || // NW wrapping
                    (DIRECTIONS_QUEEN[dir] == 7 && ((target + 1) % 8 == 0))) { // SW wrapping
                        break;
                    }
                    uint64_t targetMask = 1ULL << target;
                    if(ownPieces & targetMask) {
                        break;
                    } 
                    else if(allPieces[target] != PIECE_NONE && ((allPieces[target] & COLOR_MASK) != (isWhite ? COLOR_WHITE : COLOR_BLACK))) {
                        moveArray[moveCount++] = Move(isWhite, PIECE_QUEEN, queenSquare, target, (allPieces[target] & PIECE_MASK), '-');
                        break;
                    } else {
                        moveArray[moveCount++] = Move(isWhite, PIECE_QUEEN, queenSquare, target, PIECE_NONE, '-');
                    }
                    if(moveCount >= 1024) break;
                }
            }
        }
        // king 
        uint64_t kings = isWhite ? wKg : bKg;
        while(kings && moveCount < 1024) {
            int kingSquare = __builtin_ctzll(kings);
            kings &= kings-1;
            for(int dir = 0; dir < 8; dir++) {
                int target = kingSquare + DIRECTIONS_KING[dir];
                if(target < 0 || target > 63) continue;
                if((DIRECTIONS_KING[dir] == 1 || DIRECTIONS_KING[dir] == -7 || DIRECTIONS_KING[dir] == 9)&& (kingSquare % 8 == 7)) continue;   // East wrapping
                if((DIRECTIONS_KING[dir] == -1 || DIRECTIONS_KING[dir] == -9 || DIRECTIONS_KING[dir] == +7) && (kingSquare % 8 == 0)) continue; // West wrapping
                if((DIRECTIONS_KING[dir] == -9 || DIRECTIONS_KING[dir] == -7 || DIRECTIONS_KING[dir] == -8) && (kingSquare < 8)) continue; // North wrapping
                if((DIRECTIONS_KING[dir] == 9 || DIRECTIONS_KING[dir] == 8 || DIRECTIONS_KING[dir] == 7) && (kingSquare > 55)) continue; // South wrapping

                uint64_t targetMask = 1ULL << target;
                if(ownPieces & targetMask) {
                    continue;
                } 
                else {
                    // Create a temporary move
                    Move temp(isWhite, PIECE_KING, kingSquare, target, 
                            (allPieces[target] & PIECE_MASK), '-');
                    
                    make_move(temp);
                    bool underCheck = isInCheck(isWhite);
                    unmake_move(temp);
                    
                    if(!underCheck) {
                        moveArray[moveCount++] = temp;
                    }
                }

                if(moveCount >= 1024) break;
            }
        }

        // Castling Test

        if(isWhite) {
            // White King on e1 => square 60
            if((wKg & (1ULL << 60)) && !isInCheck(true)) {
                if(whiteCanCastleKingSide &&
                ((allPieces[61] & PIECE_MASK) == PIECE_NONE) && ((allPieces[62] & PIECE_MASK) == PIECE_NONE) && 
                !isSquareAttacked(61, false) && !isSquareAttacked(62, false)) {
                    moveArray[moveCount++] = Move(true, PIECE_KING, 60, 62, PIECE_NONE, 'C');
                }
                if(whiteCanCastleQueenSide &&
                ((allPieces[59] & PIECE_MASK)==PIECE_NONE) && 
                ((allPieces[58] & PIECE_MASK) == PIECE_NONE) &&
                ((allPieces[57] & PIECE_MASK) == PIECE_NONE) && 
                !isSquareAttacked(59, false) && !isSquareAttacked(58, false)) {
                    // Debug
                    // cout << "Adding white queen-side castle\n";
                    moveArray[moveCount++] = Move(true, PIECE_KING, 60, 58, PIECE_NONE, 'C');
                }
            }
        } else {
            // Black King on e8 => square 4
            if((bKg & (1ULL << 4)) && !isInCheck(false)) {
                if(blackCanCastleKingSide &&
                ((allPieces[5] & PIECE_MASK) == PIECE_NONE) && ((allPieces[6] & PIECE_MASK) == PIECE_NONE) &&
                !isSquareAttacked(5, true) && !isSquareAttacked(6, true)) {
                    // cout << "Adding black king-side castle\n";
                    moveArray[moveCount++] = Move(false, PIECE_KING, 4, 6, PIECE_NONE, 'C');
                }
                if(blackCanCastleQueenSide &&
                ((allPieces[3] & PIECE_MASK) == PIECE_NONE) && 
                ((allPieces[2] & PIECE_MASK) == PIECE_NONE) &&
                ((allPieces[1] & PIECE_MASK) == PIECE_NONE) &&
                !isSquareAttacked(3, true) && !isSquareAttacked(2, true)) {
                    // cout << "Adding black queen-side castle\n";
                    moveArray[moveCount++] = Move(false, PIECE_KING, 4, 2, PIECE_NONE, 'C');
                }
            }
        }


        // pawn move gen

        uint64_t pawns = isWhite ? wp : bp;
        while(pawns && moveCount < 1024) {
            int single_pawn = __builtin_ctzll(pawns);
            pawns &= pawns -1;
            if(isWhite) {
                // Single push
                int target = single_pawn - 8;
                if(target >= 0 && allPieces[target] == PIECE_NONE) {
                    if(target < 8) { // Promotion
                        moveArray[moveCount++] = Move(isWhite, PIECE_PAWN, single_pawn, target, PIECE_NONE, 'Q');
                        // Add other promotion pieces
                    } else {
                        moveArray[moveCount++] = Move(isWhite, PIECE_PAWN, single_pawn, target, PIECE_NONE, '-');
                    }
                }
                
                if(((1ULL << single_pawn) & RANK_2) && target >= 24) {
                    target = single_pawn - 16;
                    if(allPieces[target] == PIECE_NONE && allPieces[single_pawn - 8] == PIECE_NONE) {
                        moveArray[moveCount++] = Move(isWhite, PIECE_PAWN, single_pawn, target, PIECE_NONE, '-');
                    }
                }
                // capture left
                if((1ULL << single_pawn) & ~A_FILE) {
                    target = single_pawn - 9;
                    if((allPieces[target] & COLOR_MASK) != (isWhite ? COLOR_WHITE : COLOR_BLACK) && (allPieces[target] & PIECE_MASK) != PIECE_NONE){
                        // piece captured
                        moveArray[moveCount++] = Move(isWhite, PIECE_PAWN, single_pawn, target, (allPieces[target] & PIECE_MASK), '-');
                    }
                }
                // capture right
                if((1ULL << single_pawn) & ~H_FILE) {
                    target = single_pawn - 7;
                    if((allPieces[target] & COLOR_MASK) != (isWhite ? COLOR_WHITE : COLOR_BLACK) && (allPieces[target] & PIECE_MASK) != PIECE_NONE){
                        // piece captured
                        moveArray[moveCount++] = Move(isWhite, PIECE_PAWN, single_pawn, target, (allPieces[target] & PIECE_MASK), '-');
                    }
                }

                // En passant
                if(enPassantSquare != -1) {
                    if((single_pawn - 7 == enPassantSquare && (1ULL << single_pawn) & ~H_FILE) ||
                    (single_pawn - 9 == enPassantSquare && (1ULL << single_pawn) & ~A_FILE)) {
                        moveArray[moveCount++] = Move(isWhite, PIECE_PAWN, single_pawn, enPassantSquare, PIECE_PAWN, 'e');
                    }
                }
            } else {
                // single push
                int target = single_pawn + 8;
                if(target >= 0 && allPieces[target] == PIECE_NONE) {
                    moveArray[moveCount++] = Move(isWhite, PIECE_PAWN, single_pawn, target, PIECE_NONE, '-');
                }
                // double push
                if(((1ULL << single_pawn) & RANK_7) && target <= 39) {
                    target = single_pawn + 16;
                    if(allPieces[target] == PIECE_NONE && allPieces[single_pawn + 8] == PIECE_NONE) {
                        moveArray[moveCount++] = Move(isWhite, PIECE_PAWN, single_pawn, target, PIECE_NONE, '-');
                    }
                }
    

                // capture left
                if((1ULL << single_pawn) & ~A_FILE) {
                    target = single_pawn + 7;
                    if((allPieces[target] & COLOR_MASK) != (isWhite ? COLOR_WHITE : COLOR_BLACK) && (allPieces[target] & PIECE_MASK) != PIECE_NONE){
                        // piece captured
                        moveArray[moveCount++] = Move(isWhite, PIECE_PAWN, single_pawn, target, (allPieces[target] & PIECE_MASK), '-');
                    }
                }
                // capture right
                if((1ULL << single_pawn) & ~H_FILE) {
                    target = single_pawn + 9;
                    if((allPieces[target] & COLOR_MASK) != (isWhite ? COLOR_WHITE : COLOR_BLACK) && (allPieces[target] & PIECE_MASK) != PIECE_NONE){
                        // piece captured
                        moveArray[moveCount++] = Move(isWhite, PIECE_PAWN, single_pawn, target, (allPieces[target] & PIECE_MASK), '-');
                    }
                }
            }
        }
        int legalMoveCount = 0;
        for (int i = 0; i < moveCount; i++)
        {
            if(isMoveLegal(moveArray[i])) {
                if(legalMoveCount != i) {
                    moveArray[legalMoveCount] = moveArray[i];
                }
                legalMoveCount++;
            }
        }
        moveCount = legalMoveCount;
        
    }

    long Board::perft2(int depth, bool isWhite) {
        if (depth == 0)
            return 1;

        long nodes = 0;
        Move moves[1024];
        int moveCount = 0;

        generatePieceMoves(isWhite, moves, moveCount);

        for (int i = 0; i < moveCount; i++) {
            make_move(moves[i]);
            nodes += perft2(depth - 1, !isWhite);
            unmake_move(moves[i]);
        }

        return nodes;
    }

    void Board::printMoves(Move moveArray[], int moveCount) {
        for(int i = 0; i < moveCount; i++) {
            cout << (moveArray[i].isWhite ? "White " : "Black ") 
                    << "Move: " << indexToAlgebraic(moveArray[i].startSquare) << indexToAlgebraic(moveArray[i].endSquare) 
                    << "\n";
        }
    }

    void Board::make_move(Move &mv) {
        bool side = mv.isWhite;

        // Remove captured piece if any
        if(mv.pieceCaptured != PIECE_NONE) {
            uint64_t* capBoard = nullptr;
            switch(mv.pieceCaptured) {
                case PIECE_PAWN:   capBoard = side ? &bp   : &wp;   break;
                case PIECE_ROOK:   capBoard = side ? &bR   : &wR;   break;
                case PIECE_BISHOP: capBoard = side ? &bB : &wB; break;
                case PIECE_KNIGHT: capBoard = side ? &bK : &wK; break;
                case PIECE_QUEEN:  capBoard = side ? &bQ  : &wQ;  break;
                case PIECE_KING:   capBoard = side ? &bKg   : &wKg;   break;
            }
            if(capBoard) {
                *capBoard &= ~(1ULL << mv.endSquare);
                // Update combined pieces
                if(side) {
                    bpc &= ~(1ULL << mv.endSquare);
                } else {
                    wpc &= ~(1ULL << mv.endSquare);
                }
            }
        }

        // castling

        if(mv.promotion == 'C' && mv.pieceType == PIECE_KING) {
            // White
            if(mv.isWhite) {
                // King side castling e1->g1
                if(mv.startSquare == 60 && mv.endSquare == 62) {
                    wR &= ~(1ULL << 63); // remove rook from h1
                    wR |=  (1ULL << 61); // place rook on f1
                    wpc &= ~(1ULL << 63);
                    wpc |=  (1ULL << 61);
                    whiteCanCastleKingSide = false;
                    whiteCanCastleQueenSide = false;
                }
                // Queen side castling e1->c1
                else if(mv.startSquare == 60 && mv.endSquare == 58) {
                    wR &= ~(1ULL << 56); // remove rook from a1
                    wR |=  (1ULL << 59); // place rook on d1
                    wpc &= ~(1ULL << 56);
                    wpc |=  (1ULL << 59);
                    whiteCanCastleKingSide = false;
                    whiteCanCastleQueenSide = false;
                }
            } 
            // Black
            else {
                // King side castling e8->g8
                if(mv.startSquare == 4 && mv.endSquare == 6) {
                    bR &= ~(1ULL << 7); // remove rook from h8
                    bR |=  (1ULL << 5); // place rook on f8
                    bpc &= ~(1ULL << 7);
                    bpc |=  (1ULL << 5);
                    blackCanCastleKingSide = false;
                    blackCanCastleQueenSide = false;
                }
                // Queen side castling e8->c8
                else if(mv.startSquare == 4 && mv.endSquare == 2) {
                    bR &= ~(1ULL << 0); // remove rook from a8
                    bR |=  (1ULL << 3); // place rook on d8
                    bpc &= ~(1ULL << 0);
                    bpc |=  (1ULL << 3);
                    blackCanCastleKingSide = false;
                    blackCanCastleQueenSide = false;
                }
            }
        }

        // Move the piece
        uint64_t* movBoard = nullptr;
        switch(mv.pieceType) {
            case PIECE_PAWN:   movBoard = side ? &wp   : &bp;   break;
            case PIECE_ROOK:   movBoard = side ? &wR   : &bR;   break;
            case PIECE_BISHOP: movBoard = side ? &wB : &bB; break;
            case PIECE_KNIGHT: movBoard = side ? &wK : &bK; break;
            case PIECE_QUEEN:  movBoard = side ? &wQ  : &bQ;  break;
            case PIECE_KING:   movBoard = side ? &wKg   : &bKg;   break;
        }
        if(movBoard) {
            *movBoard &= ~(1ULL << mv.startSquare);
            *movBoard |= (1ULL << mv.endSquare);
            // Update combined pieces
            if(side) {
                wpc &= ~(1ULL << mv.startSquare);
                wpc |= (1ULL << mv.endSquare);
            } else {
                bpc &= ~(1ULL << mv.startSquare);
                bpc |= (1ULL << mv.endSquare);
            }
        }
        allPieces[mv.startSquare] = PIECE_NONE;
        allPieces[mv.endSquare] = side ? (COLOR_WHITE | mv.pieceType) : (COLOR_BLACK | mv.pieceType);
    }

    void Board::unmake_move(Move &mv) {
        bool side = mv.isWhite;

        // Move piece back
        uint64_t* movBoard = nullptr;
        switch(mv.pieceType) {
            case PIECE_PAWN:   movBoard = side ? &wp   : &bp;   break;
            case PIECE_ROOK:   movBoard = side ? &wR   : &bR;   break;
            case PIECE_BISHOP: movBoard = side ? &wB : &bB; break;
            case PIECE_KNIGHT: movBoard = side ? &wK : &bK; break;
            case PIECE_QUEEN:  movBoard = side ? &wQ  : &bQ;  break;
            case PIECE_KING:   movBoard = side ? &wKg   : &bKg;   break;
        }
        if(movBoard) {
            *movBoard &= ~(1ULL << mv.endSquare);
            *movBoard |= (1ULL << mv.startSquare);
            // Update combined pieces
            if(side) {
                wpc &= ~(1ULL << mv.endSquare);
                wpc |= (1ULL << mv.startSquare);
            } else {
                bpc &= ~(1ULL << mv.endSquare);
                bpc |= (1ULL << mv.startSquare);
            }
        }

        // castling

        if(mv.promotion == 'C' && mv.pieceType == PIECE_KING) {
            // White
            if(mv.isWhite) {
                // King side g1->e1
                if(mv.startSquare == 60 && mv.endSquare == 62) {
                    wR &= ~(1ULL << 61);
                    wR |=  (1ULL << 63);
                    wpc &= ~(1ULL << 61);
                    wpc |=  (1ULL << 63);
                }
                // Queen side c1->e1
                else if(mv.startSquare == 60 && mv.endSquare == 58) {
                    wR &= ~(1ULL << 59);
                    wR |=  (1ULL << 56);
                    wpc &= ~(1ULL << 59);
                    wpc |=  (1ULL << 56);
                }
            } 
            // Black
            else {
                // King side g8->e8
                if(mv.startSquare == 4 && mv.endSquare == 6) {
                    bR &= ~(1ULL << 5);
                    bR |=  (1ULL << 7);
                    bpc &= ~(1ULL << 5);
                    bpc |=  (1ULL << 7);
                }
                // Queen side c8->e8
                else if(mv.startSquare == 4 && mv.endSquare == 2) {
                    bR &= ~(1ULL << 3);
                    bR |=  (1ULL << 0);
                    bpc &= ~(1ULL << 3);
                    bpc |=  (1ULL << 0);
                }
            }
        }

        // Restore captured piece
        if(mv.pieceCaptured != PIECE_NONE) {
            uint64_t* capBoard = nullptr;
            switch(mv.pieceCaptured) {
                case PIECE_PAWN:   capBoard = side ? &bp   : &wp;   break;
                case PIECE_ROOK:   capBoard = side ? &bR   : &wR;   break;
                case PIECE_BISHOP: capBoard = side ? &bB : &wB; break;
                case PIECE_KNIGHT: capBoard = side ? &bK : &wK; break;
                case PIECE_QUEEN:  capBoard = side ? &bQ  : &wQ;  break;
                case PIECE_KING:   capBoard = side ? &bKg   : &wKg;   break;
            }
            if(capBoard) {
                *capBoard |= (1ULL << mv.endSquare);
                // Update combined pieces
                if(side) {
                    bpc |= (1ULL << mv.endSquare);
                } else {
                    wpc |= (1ULL << mv.endSquare);
                }
            }
        }

        allPieces[mv.startSquare] = side ? (COLOR_WHITE | mv.pieceType) : (COLOR_BLACK | mv.pieceType);
        allPieces[mv.endSquare] = mv.pieceCaptured != PIECE_NONE ? 
            (side ? (COLOR_BLACK | mv.pieceCaptured) : (COLOR_WHITE | mv.pieceCaptured)) : PIECE_NONE;
    }

    long Board::perft(int depth, bool isWhite, int initialDepth = -1) {
        if (initialDepth == -1) {
            initialDepth = depth;
        }

        if (depth == 0) {
            return 1;
        }

        Move moveArray[1024];
        int moveCount = 0;
        generatePieceMoves(isWhite, moveArray, moveCount);

        long nodes = 0;

        // Only print moves at root level
        if (depth == initialDepth) {
            for (int i = 0; i < moveCount; i++) {
                make_move(moveArray[i]);
                long childNodes = perft(depth - 1, !isWhite, initialDepth);
                string moveStr = indexToAlgebraic(moveArray[i].startSquare) + 
                            indexToAlgebraic(moveArray[i].endSquare);
                cout << moveStr << ": " << childNodes << "\n";
                nodes += childNodes;
                unmake_move(moveArray[i]);
            }
        } else {
            // Faster path for non-root nodes
            for (int i = 0; i < moveCount; i++) {
                make_move(moveArray[i]);
                nodes += perft(depth - 1, !isWhite, initialDepth);
                unmake_move(moveArray[i]);
            }
        }

        return nodes;
    }

    int Board::evaluateBoard() {

        const int PAWN_VALUE = 100;
        const int KNIGHT_VALUE = 320;
        const int BISHOP_VALUE = 330;
        const int ROOK_VALUE = 500;
        const int QUEEN_VALUE = 900;

        int score = 0;
        score += __builtin_popcountll(wp) * PAWN_VALUE;
        score += __builtin_popcountll(wB) * BISHOP_VALUE;
        score += __builtin_popcountll(wK) * KNIGHT_VALUE;
        score += __builtin_popcountll(wR) * ROOK_VALUE;
        score += __builtin_popcountll(wQ) * QUEEN_VALUE;
        score -= __builtin_popcountll(bp) * PAWN_VALUE;
        score -= __builtin_popcountll(bB) * BISHOP_VALUE;
        score -= __builtin_popcountll(bK) * KNIGHT_VALUE;
        score -= __builtin_popcountll(bR) * ROOK_VALUE;
        score -= __builtin_popcountll(bQ) * QUEEN_VALUE;

        // // new stuff added
        // for (int i = 0; i < 64; i++)
        // {
        //     if((allPieces[i] & PIECE_MASK) != PIECE_NONE) {

        //     }
        // }
        
        for (int i = 0; i < 64; i++)
        {
            uint8_t curr_piece = allPieces[i];
            int isWhite = (curr_piece & COLOR_MASK) == 8;
            int piece = (curr_piece & PIECE_MASK);
            if(piece == PIECE_NONE) continue;
            switch (piece) {
                case PIECE_PAWN:
                    score += (isWhite ? pawn_pst[56^i] : -pawn_pst[i]);
                    break;
                case PIECE_ROOK:
                    score += (isWhite ? rook_pst[56^i] : -rook_pst[i]);
                    break;
                case PIECE_BISHOP:
                    score += (isWhite ? bishop_pst[56^i] : -bishop_pst[i]);
                    break;
                case PIECE_KNIGHT:
                    score += (isWhite ? knight_pst[56^i] : -knight_pst[i]);
                    break;
                case PIECE_QUEEN:
                    score += (isWhite ? queen_pst[56^i] : -queen_pst[i]);
                    break;
                case PIECE_KING:
                    score += (isWhite ? king_pst[56^i] : -king_pst[i]);
                    break;
            }
        }

        if(isInCheck(true)) score -= 50;
        if(isInCheck(false)) score += 50;
        // // checkmates
        // Move moves[1024];
        // int moveCount = 0;
        // // white checkmated
        // generatePieceMoves(true, moves, moveCount);
        // if(moveCount == 0) score -= 99999;
        // moveCount = 0;
        // // black checkmated
        // generatePieceMoves(false, moves, moveCount);
        // if(moveCount == 0) score += 99999;

        return score;
    }

    int value(int piece) {
        switch (piece) {
            case PIECE_PAWN:
                return 100;
            case PIECE_KNIGHT:
                return 320;
            case PIECE_BISHOP:
                return 330;
            case PIECE_ROOK:
                return 500;
            case PIECE_QUEEN:
                return 900;
            default:
                return 0;
        }
    }

    int calculateScore(const Move &move) {
        return value(move.pieceCaptured) - value(move.pieceType);
    }

    bool compareMoves(const Move &a, const Move &b) {
        return calculateScore(a) > calculateScore(b);
    }

    void Board::order_moves(Move *moves, int moveCount) {
        sort(moves, moves+moveCount, compareMoves);
    }

    void print(Move *moves, int moveCount){
        for (int i = 0; i < moveCount; i++)
        {
            cout << moves[i].pieceCaptured << " " << moves[i].endSquare << " ";
        }
        cout << "\n";
    }

    int Board::alphaBeta(bool isWhite, int depth, int alpha, int beta) {

        if (depth == 0) {
            moveSearched++;
            return evaluateBoard();
        }
        
        int localBest = isWhite ? -100000 : 100000;
        Move bestLocalMove;
        
        Move moves[1024];
        int moveCount = 0;
        
        generatePieceMoves(isWhite, moves, moveCount);
        order_moves(moves, moveCount);
        for (int i = 0; i < moveCount; i++) {
            make_move(moves[i]);
            int score = alphaBeta(!isWhite, depth - 1, alpha, beta);
            unmake_move(moves[i]);
            
            if (isWhite) {
                if (score > localBest) {
                    localBest = score;
                    bestLocalMove = moves[i];
                }
                alpha = max(alpha, localBest);
            } else {
                if (score < localBest) {
                    localBest = score;
                    bestLocalMove = moves[i];
                }
                beta = min(beta, localBest);
            }
            
            if (beta <= alpha) {
                break; // Alpha-beta pruning
            }
        }
        
        // Store best move only at root
        if (depth == initialDepth) {
            bestMove = bestLocalMove;
        }
        
        return localBest;
    }

    // new stuff added, remove if the engine breaks
    bool Board::isSquareAttacked(int square, bool byWhite) {
        // Pawn attacks
        uint64_t pawns = byWhite ? wp : bp;
        if (byWhite) {
            int attack1 = square + 7;
            int attack2 = square + 9;

            if (attack1 >= 0 && (attack1 % 8 != 7)) { 
                if (pawns & (1ULL << attack1)) {
                    return true;
                }
            }
            if (attack2 >= 0 && (attack2 % 8 != 0)) { 
                if (pawns & (1ULL << attack2)) {
                    return true;
                }
            }
        } else {
            int attack1 = square - 7;
            int attack2 = square - 9;

            if (attack1 < 64 && (attack1 % 8 != 7)) {
                if (pawns & (1ULL << attack1)) {
                    return true;
                }
            }
            if (attack2 < 64 && (attack2 % 8 != 0)) {
                if (pawns & (1ULL << attack2)) {
                    return true;
                }
            }
        }

        // Knight attacks
        uint64_t knights = byWhite ? wK : bK;
        uint64_t knightAttacks = knight_attacks[square];
        if (knights & knightAttacks) {
            return true;
        }

        // Bishop/Queen diagonal attacks
        uint64_t bishopsQueens = byWhite ? (wB | wQ) : (bB | bQ);
        for (int dir = 0; dir < 4; dir++) {
            int target = square;
            while (true) {
                target += diag[dir];
                if (target < 0 || target > 63) break;

                if ((diag[dir] == 9 || diag[dir] == -7) && (target % 8 == 0)) break;
                if ((diag[dir] == -9 || diag[dir] == 7) && ((target + 1) % 8 == 0)) break;

                uint64_t targetBit = 1ULL << target;
                if (bishopsQueens & targetBit) {
                    return true;
                }
                if (allPieces[target] != PIECE_NONE) break;
            }
        }

        uint64_t rooksQueens = byWhite ? (wR | wQ) : (bR | bQ);
        for (int dir = 0; dir < 4; dir++) {
            int target = square;
            while (true) {
                target += dir1[dir];
                if (target < 0 || target > 63) break;

                if (dir1[dir] == 1 && (target % 8 == 0)) break;
                if (dir1[dir] == -1 && ((target + 1) % 8 == 0)) break;

                uint64_t targetBit = 1ULL << target;
                if (rooksQueens & targetBit) {
                    return true;
                }
                if (allPieces[target] != PIECE_NONE) break;
            }
        }

        uint64_t opponentKing = byWhite ? wKg : bKg;
        for (int dir = 0; dir < 8; dir++) {
            int adjacentSquare = square + DIRECTIONS_KING[dir];
            if (adjacentSquare < 0 || adjacentSquare > 63) continue;

            if ((DIRECTIONS_KING[dir] == 1 || DIRECTIONS_KING[dir] == -7 || DIRECTIONS_KING[dir] == 9) && (square % 8 == 7)) continue;  
            if ((DIRECTIONS_KING[dir] == -1 || DIRECTIONS_KING[dir] == -9 || DIRECTIONS_KING[dir] == 7) && (square % 8 == 0)) continue;
            if ((DIRECTIONS_KING[dir] == -9 || DIRECTIONS_KING[dir] == -7 || DIRECTIONS_KING[dir] == -8) && (square < 8)) continue;
            if ((DIRECTIONS_KING[dir] == 9 || DIRECTIONS_KING[dir] == 8 || DIRECTIONS_KING[dir] == 7) && (square > 55)) continue;
            if (opponentKing & (1ULL << adjacentSquare)) {
                return true;
            }
        }

        return false;
    }

    bool Board::isInCheck(bool isWhite) {
        uint64_t king = isWhite ? wKg : bKg;
        int kingSquare = __builtin_ctzll(king);
        return isSquareAttacked(kingSquare, !isWhite);
    }

    bool Board::isMoveLegal(Move &mv) {
        make_move(mv);
        bool legal = !isInCheck(mv.isWhite);
        unmake_move(mv);
        return legal;
    }

    int algebraicToIndex(string square) {
        int file = square[0] - 'a';
        int rank = '8' - square[1];
        return rank * 8 + file;
    }

    bool Board::play(string moveStr) {
        // Parse human move
        if(moveStr.length() < 4) return false;
        
        // Detect castling moves
        if(moveStr == "e1g1" || moveStr == "e1c1" || moveStr == "e8g8" || moveStr == "e8c8") {
            // Determine if White or Black is castling
            bool isWhiteCastling = (moveStr[1] == '1');
            bool isKingside = (moveStr[2] == 'g' || moveStr[2] == 'g'); // e1g1 or e8g8
            
            // Create castling move
            Move castlingMove;
            castlingMove.isWhite = isWhiteCastling;
            castlingMove.pieceType = PIECE_KING;
            castlingMove.startSquare = algebraicToIndex(moveStr.substr(0,2));
            castlingMove.endSquare = algebraicToIndex(moveStr.substr(2,2));
            castlingMove.promotion = 'C'; // Indicate castling
            
            // Validate and make the castling move
            if(isMoveLegal(castlingMove)) {
                // Make human castling move
                make_move(castlingMove);
                
                // Engine response
                alphaBeta(false, initialDepth, -100000, 100000);
                make_move(bestMove);
                printGame();
                
                return true;
            } else {
                return false;
            }
        }

        int startSquare = algebraicToIndex(moveStr.substr(0,2));
        int endSquare = algebraicToIndex(moveStr.substr(2,2));
        
        // Find and validate the move
        Move moves[1024];
        int moveCount = 0;
        generatePieceMoves(true, moves, moveCount);
        
        Move* selectedMove = nullptr;
        for(int i = 0; i < moveCount; i++) {
            if(moves[i].startSquare == startSquare && moves[i].endSquare == endSquare) {
                if(isMoveLegal(moves[i])) {
                    selectedMove = &moves[i];
                    break;
                }
            }
        }
        
        if(!selectedMove) return false;
        
        // Make human move
        make_move(*selectedMove);
        
        // Engine response
        alphaBeta(false, initialDepth, -100000, 100000);
        make_move(bestMove);
        printGame();
        
        return true;
    }

    // gitanshgoelcc///////////////////////////////////////
    int main() {
        Board b;
        b.setupGameFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        // b.setupGameFromFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
        // b.setupGameFromFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
        
        // Move moves[256];
        // int count = 0;
        // b.printGame();
        // b.generatePieceMoves(true, moves, count);
        // for (int i = 0; i < count; i++)
        // {
        //     cout << moves[i].startSquare << " " << moves[i].endSquare << "\n";
        // }
        string move;
        while(true) {
            b.printGame();
            cout << "Enter move: ";
            cin >> move;
            if(!b.play(move)) {
                cout << "Invalid Move\n";
            }
        }
        
        // int maxDepth = 5;
        // for(int depth = 1; depth <= maxDepth; depth++){
        //     long totalNodes = b.perft2(depth, true);
        //     cout << totalNodes << "\n";
        // }

        return 0;
    }
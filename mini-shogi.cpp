#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

enum Player { NONE = 0, PLAYER1, PLAYER2 };

class Piece {
public:
    char type;
    Player owner;

    Piece(char t = '.', Player p = NONE) : type(t), owner(p) {}

    char display() const {
        if (owner == NONE) return '.';
        return (owner == PLAYER2) ? tolower(type) : type;
    }
};

class Board {
private:
    static const int SIZE = 5;
    vector<vector<Piece>> grid;

public:
    Board() {
        grid = vector<vector<Piece>>(SIZE, vector<Piece>(SIZE, Piece()));

        grid[0][1] = Piece('G', PLAYER2);
        grid[0][2] = Piece('K', PLAYER2);
        grid[0][3] = Piece('G', PLAYER2);
        grid[1][2] = Piece('P', PLAYER2);

        grid[4][1] = Piece('G', PLAYER1);
        grid[4][2] = Piece('K', PLAYER1);
        grid[4][3] = Piece('G', PLAYER1);
        grid[3][2] = Piece('P', PLAYER1);
    }

    void print() const {
        cout << "  0 1 2 3 4" << endl;
        for (int y = 0; y < SIZE; ++y) {
            cout << y << " ";
            for (int x = 0; x < SIZE; ++x) {
                cout << grid[y][x].display() << " ";
            }
            cout << endl;
        }
    }

    bool isInside(int x, int y) const {
        return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
    }

    bool isLegalMove(Piece piece, int sx, int sy, int dx, int dy) const {
        int dxRel = dx - sx;
        int dyRel = dy - sy;

        switch (piece.type) {
            case 'P':
                return dxRel == 0 &&
                    ((piece.owner == PLAYER1 && dyRel == -1) ||
                     (piece.owner == PLAYER2 && dyRel == 1));

            case 'G':
                if (piece.owner == PLAYER1) {
                    return (dxRel == 0 && dyRel == -1) ||
                           (dxRel == 0 && dyRel == 1)  ||
                           (abs(dxRel) == 1 && dyRel == 0) ||
                           (abs(dxRel) == 1 && dyRel == -1);
                } else {
                    return (dxRel == 0 && dyRel == 1)  ||
                           (dxRel == 0 && dyRel == -1) ||
                           (abs(dxRel) == 1 && dyRel == 0) ||
                           (abs(dxRel) == 1 && dyRel == 1);
                }

            case 'K':
                return abs(dxRel) <= 1 && abs(dyRel) <= 1;
        }
        return false;
    }

    Piece& at(int y, int x) {
        return grid[y][x];
    }

    void movePiece(int sy, int sx, int dy, int dx) {
        grid[dy][dx] = grid[sy][sx];
        grid[sy][sx] = Piece();
    }

    bool kingExists(Player p) const {
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                if (cell.type == 'K' && cell.owner == p) return true;
            }
        }
        return false;
    }
};

int main() {
    Board board;
    Player turn = PLAYER1;

    while (true) {
        board.print();
        cout << "Player " << (turn == PLAYER1 ? "1" : "2") << "'s turn." << endl;

        int sy, sx, dy, dx;
        cout << "Enter move (sy sx dy dx): ";
        cin >> sy >> sx >> dy >> dx;

        if (!board.isInside(sx, sy) || !board.isInside(dx, dy)) {
            cout << "Invalid coordinates. Try again.\n";
            continue;
        }

        Piece from = board.at(sy, sx);
        Piece to = board.at(dy, dx);

        if (from.owner != turn) {
            cout << "Not your piece. Try again.\n";
            continue;
        }

        if (!board.isLegalMove(from, sx, sy, dx, dy)) {
            cout << "Illegal move. Try again.\n";
            continue;
        }

        if (to.owner == turn) {
            cout << "You can't capture your own piece.\n";
            continue;
        }

        board.movePiece(sy, sx, dy, dx);

        if (!board.kingExists((turn == PLAYER1) ? PLAYER2 : PLAYER1)) {
            cout << "Player " << (turn == PLAYER1 ? "1" : "2") << " wins!\n";
            break;
        }

        turn = (turn == PLAYER1) ? PLAYER2 : PLAYER1;
    }

    return 0;
}


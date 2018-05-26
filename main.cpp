// Minesweeper

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

struct cell {
    bool bomb = false;
    bool visible = false;
    int neighbors;
    int rn;
    int row;
    int col;
};

class Minesweeper {
public:
    Minesweeper () {
        srand(time(NULL));
        for (int r = 0; r < rows; ++r) {
            vector<cell> aRow;
            for (int c = 0; c < cols; ++c) {
                int randNum = rand()% 100;
                cell cell;
                cell.row = r;
                cell.col = c;
                cell.neighbors = 0;
                cell.rn = randNum;
                aRow.push_back(cell);
            }
            board.push_back(aRow);
        }
        for (int r = 1; r < rows -1; ++r) {
            vector<cell> aRow;
            for (int c = 1; c < cols -1; ++c) {
                if (board[r][c].rn < 10) {
                    board[r][c].neighbors = 0;
                    board[r][c].bomb = true;
                    numBombs += 1;
                    board[r][c + 1].neighbors += 1;
                    board[r][c - 1].neighbors +=1;
                    board[r + 1][c].neighbors +=1;
                    board[r + 1][c - 1].neighbors +=1;
                    board[r + 1][c + 1].neighbors +=1;
                    board[r - 1][c].neighbors +=1;
                    board[r - 1][c + 1].neighbors +=1;
                    board[r - 1][c -1].neighbors +=1;
                }
                else {
                    board[r][c].bomb = false;
                }
            }
            
        }
    }
    //counts the number of visible cells in the board
    int visible(){
        for (int r = 1; r < rows -1; ++r){
            for (int c = 1; c < cols -1; ++c) {
                if (board[r][c].visible == true) {
                    visibles += 1;
                }
            }
        }
        return visibles;
    }
    void display(bool flag) {
        // prints the board with all information after loss
        if (flag == true) {
            for (int r = 1; r < rows -1 ; ++r) {
                for (int c = 1; c < cols -1; ++c) {
                    if (board[r][c].bomb == true) {
                        cout << " B ";
                    }
                    else {
                        cout << ' ' << board[r][c].neighbors << ' ';
                    }
                }
                cout << endl;
            }
        }
        // prints the board to continue the game
        else {
            for (int r = 1; r < rows -1; ++r) {
                for (int c = 1; c < cols -1; ++c) {
                    if (board[r][c].visible == false) {
                        cout << " - ";
                    }
                    else {
                        cout << ' ' << board[r][c].neighbors << ' ';
                        
                    }
                }
                cout << endl;
            }
            
        }
    }
    
    bool done() {
        visibles = visible();
        for (int r = 0; r < board.size(); ++r) {
            for (int c = 0; c < cols; ++c) {
                //if bomb is visible then you lose
                if (board[r][c].bomb == true && board[r][c].visible == true){
                    return true;
                }
                // if bombs + visible = board then person wins
                int board_size = (rows-2) * (cols -2);
                if (numBombs + visibles == (board_size)){
                    return true;
                }
                // subtract a visible
                else if (board[r][c].visible == true){
                    visibles -= 1;
                }
            }
        }
        
        return false;
    }
    
    bool validRow(int r) {
        if (r > 0 && r < rows -1) {
            return true;
        }
        return false;
    }
    
    bool validCol(int c) {
        if (c > 0 && c < cols -1) {
            return true;
        }
        return false;
    }
    
    bool isVisible(int r, int c) {
        if (board[r][c].visible == true) {
            return true;
        }
        return false;
    }
    
    bool play(int r, int c){
        if (board[r][c].bomb == true) {
            return false;
        }
        else {
            //breadth first search
            if (board[r][c].neighbors == 0) {
                vector<cell> neighborhood;
                neighborhood.push_back(board[r][c]);
                while (neighborhood.size() != 0) {
                    int bombs = neighborhood.back().neighbors;
                    int r = neighborhood.back().row;
                    int c = neighborhood.back().col;
                    neighborhood.pop_back();
                    if(bombs == 0) {
                        if (board[r][c].visible !=  true){
                            board[r][c].visible = true;
                            if  (!(r == 0 || c == 0 || r == rows-1 || c == cols-1)){
                                for (int ro = -1; ro < 2; ++ro) {
                                    for (int co = -1; co < 2; ++co) {
                                        neighborhood.push_back(board[r+ro][c+co]);
                                    }
                                }
                            }
                        }
                    }
                    else {
                        board[r][c].visible = true;
                    }
                }
            }
            else {
                board[r][c].visible = true;
            }
        }
        
        
        
        return true;
    }
    
private:
    int rows = 12;
    int cols = 12;
    vector<vector<cell>> board;
    int visibles = 0;
    int numBombs = 0;
};




int main() {
    Minesweeper sweeper;
    // Continue until the only invisible cells are bombs
    while (!sweeper.done()) {
        sweeper.display(false); // display the board without bombs
        
        int row_sel = -1, col_sel = -1;
        while (row_sel == -1) {
            // Get a valid move
            int r, c;
            cout << "row? ";
            cin >> r;
            if (!sweeper.validRow(r)) {
                cout << "Row out of bounds\n";
                continue;
            }
            cout << "col? ";
            cin >> c;
            if (!sweeper.validCol(c)) {
                cout << "Column out of bounds\n";
                continue;
            }
            if (sweeper.isVisible(r,c)) {
                cout << "Square already visible\n";
                continue;
            }
            row_sel = r;
            col_sel = c;
        }
        // Set selected square to be visible. May effect other cells.
        if (!sweeper.play(row_sel,col_sel)) {
            cout << "BOOM!!!\n";
            sweeper.display(true);
            // We're done! Should consider ending more "cleanly",
            // eg. Ask user to play another game.
            exit(0);
        }
    }
    // Ah! All invisible cells are bombs, so you won!
    cout << "You won!!!!\n";
    sweeper.display(true); // Final board with bombs shown
}


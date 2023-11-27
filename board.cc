#include "board.h"
using namespace std;

Board::Board() : size{8}, td{make_unique<TextDisplay>()} {}

// Handle board orientation of commands in the main function
// Therefore a positive y value means moving upwards on the board display
// And a positive x value means moving to the right on the board display

bool Board::isInvalidMove(Link &link, int xCord, int yCord, Player &player) {
    int linkxcoord = link.getX();
    int linkycoord = link.getY();
    
    if (player.getplayerID() == 2) {
        // JAMES' previous logic, where x + y where 'amount to move by'
        // if (x + linkxcoord > 7 || x + linkxcoord < 0 || y + linkycoord > 7) {
        //     return true;
        // }
        // else if (player.hasLinkAt(x + linkxcoord, y + linkycoord)) {
        //     return true;
        // }
        // else if ((x + linkxcoord == 3 && y + linkycoord == 7) || (x + linkxcoord == 4 && y + linkycoord == 7)) {
        //     return true;
        // }

        // check out of bounds
        if (xCord > 7 || xCord < 0 || yCord > 7) {
            return true;
        }
        // player is trying to move onto their own link
        else if (player.hasLinkAt(xCord, yCord)) {
            return true;
        }
        // player trying to move onto server port
        else if (yCord == 7 && (xCord == 3 || xCord == 4)) {
            return true;
        }
        // valid move
        else {
            return false;
        }
    }

    else {
        // JAMES' prev. log
        // if (x + linkxcoord > 7 || x + linkxcoord < 0 || y + linkycoord < 0) {
        //     return true;
        // }

        // else if (player.hasLinkAt(x + linkxcoord, y + linkycoord)) {
        //     return true;
        // }

        // else if ((x + linkxcoord == 3 && y + linkycoord == 0) || (x + linkxcoord == 4 && y + linkycoord == 0)) {
        //     return true;
        // }


        // Player 1 : top of the board
        // check out of bounds
        if (xCord > 7 || xCord < 0 || yCord < 0) {
            return true;
        }
        // check if trying to move onto owned link
         else if (player.hasLinkAt(xCord, yCord)) {
            return true;
        }
        // check if trying to move onto owned server port
         else if (yCord == 0 && (xCord == 3 || xCord == 4)) {
            return true;
        }
        else {
            return false;
        }
}
}

// CHANGED: xCord, yCord are actual coordinates it moves into now
bool Board::isOccupiedByOpponent(Player *NonActivePlayer, int xCord, int yCord) {
    if (NonActivePlayer->hasLinkAt(xCord, yCord)) {
        return true;
    }
    else {
        return false;
    }
}


void Board::battle(Player &ActivePlayer, Player &NonActivePlayer, Link &ActivePlayerLink, Link &NonActivePlayerLink) {
    if (ActivePlayerLink.getStrength() >= NonActivePlayerLink.getStrength()) {
        if (NonActivePlayerLink.getType() == "D") {
            ActivePlayer.changeDataCount();
        }
        else {
            ActivePlayer.changeVirusCount();
        }
        NonActivePlayerLink.setX(-1);
        NonActivePlayerLink.setY(-1);
        NonActivePlayerLink.revealLink();
        ActivePlayerLink.revealLink();
        grid[NonActivePlayerLink.getY()][NonActivePlayerLink.getX()].setLinkNull();
        // td->notify(grid[NonActivePlayerLink.getY()][NonActivePlayerLink.getX()]);
        grid[NonActivePlayerLink.getY()][NonActivePlayerLink.getX()].notifyObservers();
    }

    else {
        if (ActivePlayerLink.getType() == "D") {
            NonActivePlayer.changeDataCount();
        }
        else {
            NonActivePlayer.changeVirusCount();
        }
        ActivePlayerLink.setX(-1);
        ActivePlayerLink.setY(-1);
        NonActivePlayerLink.revealLink();
        ActivePlayerLink.revealLink();
        grid[ActivePlayerLink.getY()][ActivePlayerLink.getX()].setLinkNull();
        // td->notify(grid[ActivePlayerLink.getY()][ActivePlayerLink.getX()]);
        grid[ActivePlayerLink.getY()][ActivePlayerLink.getX()].notifyObservers();
    }
}


void Board::move(Player* ActivePlayer, Player* NonActivePlayer, Link &link, int x, int y) {
    int linkxcoord = link.getX();
    int linkycoord = link.getY();

        if (grid[linkycoord + y][linkxcoord + x].getIsServerPort()) {
            if (link.getType() == "D") {
                NonActivePlayer->changeDataCount();
            }
            else {
                NonActivePlayer->changeVirusCount();
            }
            link.setX(-1);
            link.setY(-1);
            link.revealLink();
            grid[linkycoord + y][linkxcoord + x].setLinkNull();
            grid[linkycoord + y][linkxcoord + x].notifyObservers();
        }

        else {
            grid[linkycoord + y][linkxcoord + x].setLink(&link);
            grid[linkycoord][linkxcoord].setLinkNull();
            grid[linkycoord + y][linkxcoord + x].notifyObservers();
            grid[linkycoord][linkxcoord].notifyObservers();
        }
}

void Board::printTextDisplay() {
    cout << *td;
}

    // ===================== Board Setup =======================

    // sets the observers and coords for every cell.
    void setCellObservers() {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                Cell &current = grid[i][j];
                current.setCoords(i, j);
                current.attach(td.get()); // get and pass raw ptr
            }
        }   
    }

    void Board::setup() {
        // create cells
        for (int i = 0; i < size; ++i) {
            grid.emplace_back();
            for (int j = 0; j < size; ++j) {
                grid[i].emplace_back();
            }
        }

        // set cell observers
        setCellObservers();
    }

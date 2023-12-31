#include "linkboost.h"

LinkBoost::LinkBoost(Player *activePlayer, vector<vector<Cell>> *grid) : Ability(), activePlayer{activePlayer}, grid{grid} {}

void LinkBoost::apply(int x, int y) {
    if (checkValid(x, y)) {
        Link *link = (*grid)[y][x].getLink();
        link->incrementMoveFactor(1);
        isUsed = true;
    } else {
        throw(logic_error("Cannot use Linkboost on that.\n"));
    }
}

bool LinkBoost::checkValid(int x, int y) {
    return playerMatch(activePlayer, &((*grid)[y][x].getLink()->getPlayer()));
}

char LinkBoost::getType() {
    return 'L';
}

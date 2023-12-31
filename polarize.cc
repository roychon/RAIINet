#include "polarize.h"

Polarize::Polarize(Player *activePlayer, vector<vector<Cell>> *grid): Ability(), grid{grid}, activePlayer{activePlayer} {}

void Polarize::apply(int x, int y) {
    Cell targetCell = (*grid)[y][x];
    Link *link = targetCell.getLink();
    if (link->getType() == 'D') {
        link->setType('V');
        // in the event we are on a firewall not belonging to link's owner
        if (targetCell.getFirewallOwner() != nullptr && targetCell.getFirewallOwner() != &(link->getPlayer())) {
            link->getPlayer().incrementDownloads('V');
            targetCell.setLinkNull();
            link->setX(-1);
            link->setY(-1);
            link->setIsDead();
        }
        targetCell.notifyObservers();
    } else {
    link->setType('D');
    targetCell.notifyObservers();
    }
    isUsed = true;
}

bool Polarize::checkValid(int x, int y) { return false; }

char Polarize::getType() {
    return 'P';
}

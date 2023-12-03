#include "firewall.h"

Firewall::Firewall(Player *firewallOwner, vector<vector<Cell>> *grid): Ability(), grid{grid}, firewallOwner(firewallOwner) {}

void Firewall::apply(int x, int y) {
    if (checkValid(x, y)) {
        Cell &cell = (*grid)[y][x];
        cell.setFirewall();
        cell.setFirewallOwner(firewallOwner);
        cell.notifyObservers();
        isUsed = true;
    } else {
        cout << "abiltiy failed" << endl;
    }
}

bool Firewall::checkValid(int x, int y) {
    Cell &cell = (*grid)[y][x];
    if (cell.getIsFirewall() || cell.getIsServerPort() || cell.getLink() != nullptr) return false;
    return true;
}

char Firewall::getType() {
    return 'F';
}

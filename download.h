#ifndef __DOWNLOAD_H__
#define __DOWNLOAD_H__
#include "ability.h"
#include "cell.h"
#include <vector>
#include "link.h"
class Download : public Ability {
    Player *activePlayer; // player that owns the ability
    vector<vector<Cell>> *grid; // pass in grid of the board
public:
    Download(Player *activePlayer, vector<vector<Cell>> *grid);
    void apply(int x, int y) override; // passes in enemy link
    bool checkValid(int x, int y) override;
    char getType() override;
};

#endif

#ifndef __LINK_H__
#define __LINK_H__
#include <string>
#include "player.h"
using namespace std;

class Player;

// TODO: think about making Link abstract for lower coupling
// -> link will have to have a download function then
class Link {
    char id; // board display id, from a to z or A to Z depending on player
    int strength; // strength of link
    char type; // D: data, V : virus, S : server port
    bool isRevealed;
    int x;
    int y;
    Player *player; // ptr to player who owns this link
    int moveFactor; // controls distance of movement for link on game board
    bool isDead;

public:
    Link();
    void move(int x, int y);
    int getMoveFactor();
    char getId();
    char getType();
    int getStrength();
    Player &getPlayer();
    int getX();
    int getY();
    bool getIsRevealed();
    bool getIsDead();
    void setIsDead();
    void setX(int x);
    void setY(int y);
    void setStrength(int strength);
    void setType(char type);
    void setPlayer(Player *player); // Sets the owner of the link
    void revealLink();
    void setId(char id); // Sets the display id of a link
    void incrementMoveFactor(int factor); // Increases move factor
    void augmentpower(); // Increase strength
    void setRevealedFalse();
};

#endif

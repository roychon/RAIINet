#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <fstream>
#include <stdexcept>
#include "game.h"
#include "clparse.h"
using namespace std;

int main(int argc, char *argv[]) {
    string player1links = "Default";
    string player2links = "Default";
    string player1abilities = "LFDSP";
    string player2abilities = "LFDSP";
    bool graphicsOn = false;
    // Command line arguments
    for (int i = 1; i < argc; ++i) {
        string cmd = argv[i];
        CLParse cl;
        try {
            if (cmd == "-link1") cl.parseLinks(player1links, argv[i + 1]);
            else if (cmd == "-link2") cl.parseLinks(player2links, argv[i + 1]);
            else if (cmd == "-ability1") cl.parseAbilities(player1abilities, argv[i + 1], 1);
            else if (cmd == "-ability2") cl.parseAbilities(player2abilities, argv[i + 1], 2);
            else if (cmd == "-graphics") graphicsOn = true;
        } catch(logic_error &e) {
            cerr << e.what();
        }
    }

    Game game;
    unique_ptr<Xwindow> xw;
    if (graphicsOn) {
    xw = make_unique<Xwindow>();
    game = Game{xw.get()};
    }
    // Pass these game parameters into init, via command flags
    game.init(player1links, player2links, player1abilities, player2abilities);
    game.display(graphicsOn);
    bool enhancementsOn = false;

    string command;
    while (cin >> command) {
        if (command == "board") {
            game.display(graphicsOn);
        }

        else if (command == "quit") {
            break;
        }

        else if (command == "move") {
            char whichLink;
            string direction;
            cin >> whichLink >> direction;
            Link *linkToMove = game.getActivePlayer()->getLinkByID(whichLink);

            if (linkToMove == nullptr) {
                cout << "This is not one of your links!" << endl;
                continue;
            } else if (linkToMove->getIsDead()) {
                cout << "This link no longer exists." << endl;
                continue;
            }
            try {
                int x = 0;
                int y = 0;
                if (direction == "up") y = 1;
                else if (direction == "right") x = 1;
                else if (direction == "left") x = -1;
                else if (direction == "down") y = -1;
                else {
                    cout << "Invalid direction." << endl;
                    continue;
                }
    
                game.move(linkToMove, x, y);
                
            } catch(logic_error &e) {
                cerr << e.what();
                continue;
            }

            game.switchActivePlayer();
            game.display(graphicsOn);
            if (game.checkWin() == true) {
                cout << "Player " << game.getWinningPlayer()->getplayerID() << " Wins!" << endl;
                break;
            }
        }

        else if (command == "enhancements") {
            enhancementsOn = !enhancementsOn;
            game.toggleEnhancementsOn();
        }

        else if (command == "abilities") {
            game.getActivePlayer()->printAbilities();
        }
        else if (command == "ability") {
            int id, x, y;
            cin >> id;
            char whichLink;
            char abilityType = game.getActivePlayer()->getAbility(id)->getType();

            if (!game.getActivePlayer()->getHasAbilityTurn()) {
                cout << "You've already used an ability this turn!" << endl;
                continue;
            }

            if (game.getActivePlayer()->getNumAbilitiesLeft() == 0) {
                cout << "You're out of abilities!" << endl;
                continue;
            }
            try {
                // target is only active player's link
                if (abilityType == 'L' || abilityType == 'A') {
                    cin >> whichLink;
                    Link *targetLink = game.getActivePlayer()->getLinkByID(whichLink);
                    if (targetLink == nullptr) {
                        cout << "This is not one of your links!" << endl;
                        continue;
                    } else if (targetLink->getIsDead()) {
                        cout << "This link no longer exists." << endl;
                        continue;
                    }
                    x = targetLink->getX();
                    y = targetLink->getY();
                    game.getActivePlayer()->useAbility(id, x, y);
                }
                // target is only inactive player's link
                else if (abilityType == 'D' || abilityType == 'T') {
                    cin >> whichLink;
                    Link *targetLink = game.getInactivePlayer()->getLinkByID(whichLink);
                    if (targetLink == nullptr) {
                        cout << "This is not one of your opponent's links!" << endl;
                        continue;
                    } else if (targetLink->getIsDead()) {
                        cout << "This link no longer exists." << endl;
                        continue;
                    }
                    x = targetLink->getX();
                    y = targetLink->getY();
                    game.getActivePlayer()->useAbility(id, x, y);
                }
                // target is either active or inactive player's link
                else if (abilityType == 'P' || abilityType == 'S') {
                    cin >> whichLink;
                    Link *targetLink = game.getActivePlayer()->getLinkByID(whichLink);
                    // not your own links, so check if it's opponent's
                    if (targetLink == nullptr) {
                        targetLink = game.getInactivePlayer()->getLinkByID(whichLink);
                    }
                    if (targetLink == nullptr) {
                        cout << "This is not neither one of your own or your opponent's links!" << endl;
                        continue;
                    } else if (targetLink->getIsDead()) {
                        cout << "This link no longer exists." << endl;
                        continue;
                    }
                    if (abilityType == 'S' && targetLink->getIsRevealed()) {
                        cout << "This link is already revealed... cannot be scanned." << endl;
                        continue;
                    }

                    x = targetLink->getX();
                    y = targetLink->getY();
                    game.getActivePlayer()->useAbility(id, x, y);
                }
                // target is coord
                else if (abilityType == 'F') {
                    cin >> x >> y;
                    game.getActivePlayer()->useAbility(id, x, y);
                }
                // target is not a coord or link
                else if (abilityType == 'H' || abilityType == 'C') {
                    game.getActivePlayer()->useAbility(id, -1, -1);
                }
            } catch(logic_error &e) {
                cerr << e.what();
                continue;
            }
            game.display(graphicsOn);
            cout << "Ability " << id << " used!" << endl;
            if (game.checkWin() == true) {
                cout << "Player " << game.getWinningPlayer()->getplayerID() << " Wins!" << endl;
                return 0;
            }
        }
        else if (command == "sequence") {
            string filename;
            cin >> filename;
            game.runSequence(filename, graphicsOn);
        }
    }
    
    return 0;
}

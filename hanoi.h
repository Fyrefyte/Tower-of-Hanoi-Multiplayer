#pragma once

#ifndef HANOI_H
#define HANOI_H

#include "stack.h"
#include <sstream>

template <typename T = size_t, T maxSize = 20, T numTowers = 3>
class Hanoi {
private:
    T size;
    Stack<T, maxSize> towers[numTowers];
    T pieces[maxSize];
    T goal;
    const T towerCount = numTowers;
public:
    Hanoi(T = 5, T = 2);
    bool canMoveFrom(T);
    bool canMoveTo(T);
    bool canMove(T, T);
    bool checkWin();
    bool movePiece(T, T);
    void reset();
    std::ostringstream computeSegment(const T, const T);

    friend void clearConsole(Hanoi<T, maxSize, numTowers>& towers) {
        std::cout << "\x1b[H\x1b[2J" << std::flush;
    }
    friend void printTowers(Hanoi<T, maxSize, numTowers>& towers) {
        #define TOWER_AT(tower, x) (x < towers.towers[tower].length() ? *towers.towers[tower].at(x) : 0)
        #define TOWER_SEPARATION " ";

        std::ostringstream stream;

        // Top layer
        std::cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++)
            std::cout << towers.computeSegment(0, TOWER_AT(i, towers.size - 1)).str() << TOWER_SEPARATION;
        std::cout << std::endl;

        // Middle layers
        for (T i = towers.size - 1; i > 0; i--) {
            std::cout << TOWER_SEPARATION;
            for (T j = 0; j < towers.towerCount; j++)
                std::cout << towers.computeSegment(TOWER_AT(j, i), TOWER_AT(j, i-1)).str() << TOWER_SEPARATION;
            std::cout << std::endl;
        }

        // Bottom layer
        std::cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++)
            std::cout << towers.computeSegment(TOWER_AT(i, 0), 0).str() << TOWER_SEPARATION;
        std::cout << std::endl;

        std::cout << stream.str();
    }

    friend void gameStep(Hanoi<T, maxSize, numTowers>& towers) {

        // Clear console and display current tower setup
        clearConsole(towers);
        printTowers(towers);

        // Loop for checking invalid move states
        while (true) {
            // Get the tower to move from this step
            T choice1;
            T choice2;
            char inpChar;
            // Stack<T, maxSize>* tower1;
            // Stack<T, maxSize>* tower2;
            while (true) {
                std::cout << std::endl << "Select a tower to move from (1 to " << static_cast<size_t>(numTowers) << "): " << std::flush;
                try {
                    std::cin >> inpChar;
                    choice1 = static_cast<T>(inpChar - '0');
                    break;
                    // if (choice1 >= 1 && choice1 <= numTowers) {
                    //     // string choiceStr = (choice1 == 1 ? "Left" : (choice1 == 2 ? "Center" : "Right"));
                    //     // tower1 = (choice1 == 1 ? &towers.towerL : (choice1 == 2) ? &towers.towerC : &towers.towerR);
                    //     // cout << choiceStr << " tower chosen" << endl;
                    //     break;
                    // }
                }
                catch (std::exception e) {}
            }

            // Get the tower to move to this step
            while (true) {
                std::cout << std::endl << "Select a tower to move to (1 to " << static_cast<size_t>(numTowers) << "): " << std::flush;
                try {
                    std::cin >> inpChar;
                    choice2 = static_cast<T>(inpChar - '0');
                    break;
                    // if (choice2 >= 1 && choice2 <= numTowers && choice2 != choice1) {
                    //     // string choiceStr = (choice2 == 1 ? "Left" : (choice2 == 2 ? "Center" : "Right"));
                    //     // tower2 = (choice2 == 1 ? &towers.towerL : (choice2 == 2) ? &towers.towerC : &towers.towerR);
                    //     // cout << choiceStr << " tower chosen" << endl;
                    //     break;
                    // }
                }
                catch (std::exception e) {}
            }

            // Actually move the piece
            if (towers.movePiece(choice1 - 1, choice2 - 1)) break;
            else std::cout << "Invalid move, try again" << std::endl;
        }
    }

    friend void gameLoop(Hanoi<T, maxSize, numTowers>& towers) {
        while (!towers.checkWin()) gameStep(towers);
        clearConsole(towers);
        printTowers(towers);
        std::cout << "You win!" << std::endl;
        // TODO add play again option
    }
};

template <typename T = size_t, T maxSize = 20, T numTowers = 5, T numPlayers = 2, T numGoals = 2>
class HanoiMultiplayer {
private:
    T size;
    StackPaired<T, T, maxSize> towers[numTowers];
    T pieces[maxSize];
    T players[numPlayers];
    T goals[numPlayers][numGoals];
    T starts[numPlayers];
    const T towerCount = numTowers;
    T turn = 0;
public:
    HanoiMultiplayer(T, T(&)[numPlayers], T(&)[numPlayers][numGoals]);
    bool canMoveFrom(T);
    bool canMoveTo(T);
    bool canMove(T, T);
    bool checkOwns(T, T);
    T checkWin();
    bool movePiece(T, T);
    void reset();
    static char getPlayerChar(const T);
    std::ostringstream computeSegment(const T, const T, const char);

    friend void clearConsole(HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>& towers) {
        std::cout << "\x1b[H\x1b[2J" << std::flush;
    }

    friend void printTurn(HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>& towers) {
        std::cout << "Player " << static_cast<size_t>(towers.turn)+1 << "'s turn" << std::endl;
    }

    friend void printTowers(HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>& towers) {
        #define TOWER_AT1(tower, x) (x < towers.towers[tower].length() ? *towers.towers[tower].at1(x) : 0)
        #define TOWER_AT2(tower, x) (x < towers.towers[tower].length() ? *towers.towers[tower].at2(x) : -1)
        #define TOWER_SEPARATION " ";

        std::ostringstream stream;

        // Top layer
        std::cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++)
            std::cout << towers.computeSegment(0, TOWER_AT1(i, towers.size - 1), getPlayerChar(TOWER_AT2(i, towers.size - 1))).str() << TOWER_SEPARATION;
        std::cout << std::endl;

        // Middle layers
        for (T i = towers.size - 1; i > 0; i--) {
            std::cout << TOWER_SEPARATION;
            for (T j = 0; j < towers.towerCount; j++)
                std::cout << towers.computeSegment(TOWER_AT1(j, i), TOWER_AT1(j, i-1), getPlayerChar(TOWER_AT2(j, i-1))).str() << TOWER_SEPARATION;
            std::cout << std::endl;
        }

        // Bottom layer
        std::cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++)
            std::cout << towers.computeSegment(TOWER_AT1(i, 0), 0, getPlayerChar(TOWER_AT2(i, 0))).str() << TOWER_SEPARATION;
        std::cout << std::endl;

        std::cout << stream.str();
    }

    friend void gameStep(HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>& towers) {

        // Clear console and display current tower setup
        clearConsole(towers);
        printTowers(towers);
        printTurn(towers);

        // Loop for checking invalid move states
        while (true) {
            // Get the tower to move from this step
            T choice1;
            T choice2;
            char inpChar;
            while (true) {
                std::cout << std::endl << "Select a tower to move from (1 to " << static_cast<size_t>(numTowers) << "): " << std::flush;
                try {
                    std::cin >> inpChar;
                    choice1 = static_cast<T>(inpChar - '0');
                    /*if (towers.canMoveFrom(choice1)) */break;
                }
                catch (std::exception e) {}
            }

            // Get the tower to move to this step
            while (true) {
                std::cout << std::endl << "Select a tower to move to (1 to " << static_cast<size_t>(numTowers) << "): " << std::flush;
                try {
                    std::cin >> inpChar;
                    choice2 = static_cast<T>(inpChar - '0');
                    /*if (towers.canMove(choice1, choice2)) */break;
                }
                catch (std::exception e) {}
            }

            // Actually move the piece
            if (towers.movePiece(choice1 - 1, choice2 - 1)) break;
            else std::cout << "Invalid move, try again" << std::endl;
        }
    }

    friend void gameLoop(HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>& towers) {
        while (towers.checkWin() == static_cast<T>(-1)) {
            gameStep(towers);
            ++towers.turn %= numPlayers;
        }
        clearConsole(towers);
        printTowers(towers);
        std::cout << "Player " << towers.checkWin() + 1 << " wins!" << std::endl;
        // TODO add play again option
    }
};

#endif // HANOI_H

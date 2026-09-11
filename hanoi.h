#pragma once

#ifndef HANOI_H
#define HANOI_H

#include "stack.h"
#include "queue.h"
#include <sstream>
#include <limits>
#include <iostream>
#include <fstream>

#define CLEAR_CONSOLE std::cout << "\x1b[H\x1b[2J" << std::flush;
#define CONT_REMINDER "\n(Enter to continue)"
#define ENTER_TO_CONT(prevInput) std::cout << CONT_REMINDER; \
                                 std::cin.clear(); \
                                 if (prevInput) std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); \
                                 std::cin.get(); \
                                 CLEAR_CONSOLE; \

template <typename T = size_t>
class Move
{
private:
    const T from;
    const T to;
public:
    Move(T fromTower, T toTower) : from(fromTower), to(toTower) {}
    const T getFrom() const { return from; }
    const T getTo() const { return to; }
    const std::ostringstream printable() const { return std::ostringstream() << "(" << static_cast<size_t>(from) + 1 << " -> " << static_cast<size_t>(to) + 1 << ")"; }
};

template <typename T = size_t>
class MoveSeq
{
private:
    ContainerQueue<Move, T> moves;
public:
    MoveSeq() {}
    void addMove(const T from, const T to) {
        Move<T>* newMove = new Move<T>(from, to);
        addMove(newMove);
    }
    void addMove(Move<T>* move) {
        moves.enqueue(move);
    }
    const Move<T>* getNextMove() {
        return moves.dequeue();
    }
    size_t movesLeft() { return moves.length(); }
    void save(const std::string saveSignature) {
        std::ofstream csvFile("savedSeq" + saveSignature + ".csv");
        if (!csvFile.is_open()) throw std::runtime_error("New save file with the given signature failed to open: savedSeq" + saveSignature + ".csv");
        csvFile << "From,To\n";
        while (moves.length() > 0) {
            const Move<T>* next = getNextMove();
            csvFile << next->getFrom() << ',' << next->getTo() << "\n";
            delete next;
        }
    }
    void load(const std::string saveSignature) {
        std::ifstream csvFile("savedSeq" + saveSignature + ".csv");
        if (!csvFile.is_open()) throw std::runtime_error("Save file with the given signature failed to open: savedSeq" + saveSignature + ".csv");
        std::string line;
        std::getline(csvFile, line); // Remove header
        while (std::getline(csvFile, line)) {
            size_t delimiter = line.find(",");
            addMove(std::stoull(line.substr(0, delimiter)), std::stoull(line.substr(delimiter + 1)));
        }
    }
};

template <typename T = size_t, T maxSize = 20, T numTowers = 3>
class Hanoi
{
private:
    T size;
    Stack<T, maxSize> towers[numTowers];
    T pieces[maxSize];
    T goal;
    const T towerCount = numTowers;
    size_t moveCount = 0;
public:
    Hanoi(T = 5, T = 2);
    bool canMoveFrom(T);
    bool canMoveTo(T);
    bool canMove(T, T);
    bool checkWin();
    bool movePiece(T, T);
    void reset();
    std::ostringstream computeSegment(const T, const T);

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

        // Goal indicators
        std::cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++) {
            std::cout << std::string(towers.size + 1, ' ');
            if (towers.goal == i) std::cout << "^^";
            else std::cout << "  ";
            std::cout << std::string(towers.size + 2, ' ');
        }
        std::cout << std::endl;

        // std::cout << stream.str();
    }

    friend void gameStep(Hanoi<T, maxSize, numTowers>& towers) {

        // Clear console and display current tower setup
        CLEAR_CONSOLE;
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
        CLEAR_CONSOLE;
        printTowers(towers);
        std::cout << "You win!" << std::endl;
        std::cout << "You took " << towers.moveCount << " moves, which is ";
        if (towers.moveCount == (1LL << towers.size) - 1) {
            std::cout << "optimal!" << std::endl;
        }
        else if (towers.moveCount == (1LL << towers.size)) {
            std::cout << "1 move more than optimal." << std::endl;
        }
        else {
            std::cout << (towers.moveCount - (1LL << towers.size) + 1) << " moves more than optimal." << std::endl;
        }
        // TODO add play again option
    }
};

template <typename T = size_t, T maxSize = 20, T numTowers = 3>
class HanoiAuto
{
private:
    Hanoi<T, maxSize, numTowers> hanoi;
    MoveSeq<T> moves;
    size_t currentMove = 0;
public:
    HanoiAuto() {}
    void setMoves(MoveSeq<T> newMoves) { moves = newMoves; }
    T movesLeft() { return moves.movesLeft(); }
    friend void showCurrentMove(HanoiAuto<T, maxSize, numTowers>& hanoiAuto, bool prevInput = false) {
        printTowers(hanoiAuto.hanoi);
        ENTER_TO_CONT(prevInput);
    }
    friend bool playNextMove(HanoiAuto<T, maxSize, numTowers>& hanoiAuto, bool prevInput = false) {
        if (hanoiAuto.movesLeft() == 0) return false;
        const Move<T>* move = hanoiAuto.moves.getNextMove();
        hanoiAuto.hanoi.movePiece(move->getFrom(), move->getTo());
        printTowers(hanoiAuto.hanoi);
        hanoiAuto.currentMove++;
        std::cout << "Move " << hanoiAuto.currentMove << ": " << move->printable().str() << std::endl;
        ENTER_TO_CONT(prevInput);
        return true;
    }
    friend void showFullMoveset(HanoiAuto<T, maxSize, numTowers>& hanoiAuto, bool prevInput = false) {
        for (size_t i = 1; hanoiAuto.movesLeft() > 0; i++) {
            std::cout << i << ". " << hanoiAuto.moves.getNextMove()->printable().str() << std::endl;
        }
    }
};

template <typename T = size_t, T maxSize = 20, T numTowers = 5, T numPlayers = 2, T numGoals = 2>
class HanoiMultiplayer
{
private:
    T size;
    StackPaired<T, T, maxSize> towers[numTowers];
    T pieces[maxSize];
    T players[numPlayers];
    T goals[numPlayers][numGoals];
    T starts[numPlayers];
    const T towerCount = numTowers;
    T turn = 0;
    size_t moveCount = 0;
    bool pattern;
public:
    HanoiMultiplayer(T, T(&)[numPlayers], T(&)[numPlayers][numGoals], bool patterned = true);
    bool canMoveFrom(T);
    bool canMoveTo(T);
    bool canMove(T, T);
    bool checkOwns(T, T);
    T goalAt(T);
    T checkWin();
    bool movePiece(T, T);
    void reset();
    static char getPlayerChar(const T);
    std::ostringstream computeSegment(const T, const T, const char);

    friend void printTurn(HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>& towers) {
        std::cout << "Player " << static_cast<size_t>(towers.turn)+1 << "'s turn";
        if (towers.pattern && towers.moveCount < numPlayers) std::cout << std::string(15, ' ') << "____" << std::endl << "(Your pieces look like this: |" << std::string(4, towers.getPlayerChar(towers.turn)) << "|)" << std::endl;
    }

    friend void printTowers(HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>& towers) {
#define TOWER_AT1(tower, x) (x < towers.towers[tower].length() ? *towers.towers[tower].at1(x) : 0)
#define TOWER_AT2(tower, x) (x < towers.towers[tower].length() ? *towers.towers[tower].at2(x) : -1)
#define TOWER_SEPARATION " ";
#define PLAYER_CHAR(tower, x) towers.pattern ? getPlayerChar(TOWER_AT2(tower, x)) : static_cast<size_t>(TOWER_AT2(tower, x)) + '1'

        std::ostringstream stream;

        // Tower numbers
        std::cout << TOWER_SEPARATION;
        for (size_t i = 1; i <= towers.towerCount; i++) {
            std::cout << std::string(towers.size + 1, ' ');
            std::cout << i;
            if (i < 10) std::cout << ' ';
            std::cout << std::string(towers.size + 1, ' ') << TOWER_SEPARATION;
        }
        std::cout << std::endl;

        // Top layer
        std::cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++)
            std::cout << towers.computeSegment(0, TOWER_AT1(i, towers.size - 1), PLAYER_CHAR(i, towers.size - 1)).str() << TOWER_SEPARATION;
        std::cout << std::endl;

        // Middle layers
        for (T i = towers.size - 1; i > 0; i--) {
            std::cout << TOWER_SEPARATION;
            for (T j = 0; j < towers.towerCount; j++)
                std::cout << towers.computeSegment(TOWER_AT1(j, i), TOWER_AT1(j, i-1), PLAYER_CHAR(j, i)).str() << TOWER_SEPARATION;
            std::cout << std::endl;
        }

        // Bottom layer
        std::cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++)
            std::cout << towers.computeSegment(TOWER_AT1(i, 0), 0, PLAYER_CHAR(i, 0)).str() << TOWER_SEPARATION;
        std::cout << std::endl;

        // Goal indicators
        std::cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++) {
            std::cout << std::string(towers.size + 1, ' ');
            T goal = towers.goalAt(i);
            if (goal != static_cast<T>(-1)) std::cout << static_cast<size_t>(goal + 1) << '^';
            else std::cout << "  ";
            std::cout << std::string(towers.size + 1, ' ') << TOWER_SEPARATION;
        }
        std::cout << std::endl;

        // std::cout << stream.str();
    }

    friend void gameStep(HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>& towers) {

        // Clear console and display current tower setup
        CLEAR_CONSOLE;
        printTowers(towers);
        printTurn(towers);

        // Loop for checking invalid move states
        while (true) {
            // Get the tower to move from this step
            T choice1;
            T choice2;
            while (true) {
                std::cout << std::endl << "Select a tower to move from (1 to " << static_cast<size_t>(numTowers) << "): " << std::flush;
                try {
                    if (numTowers < 10) {
                        char inpChar;
                        std::cin >> inpChar;
                        choice1 = static_cast<T>(inpChar - '0');
                    }
                    else {
                        std::string inpStr;
                        std::getline(std::cin, inpStr);
                        choice1 = static_cast<T>(std::stoi(inpStr));
                    }
                    /*if (towers.canMoveFrom(choice1)) */break;
                }
                catch (std::exception e) {}
            }

            // Get the tower to move to this step
            while (true) {
                std::cout << std::endl << "Select a tower to move to (1 to " << static_cast<size_t>(numTowers) << "): " << std::flush;
                try {
                    if (numTowers < 10) {
                        char inpChar;
                        std::cin >> inpChar;
                        choice2 = static_cast<T>(inpChar - '0');
                    }
                    else {
                        std::string inpStr;
                        std::getline(std::cin, inpStr);
                        choice2 = static_cast<T>(std::stoi(inpStr));
                    }
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
        CLEAR_CONSOLE;
        printTowers(towers);
        std::cout << "Player " << towers.checkWin() + 1 << " wins!" << std::endl;
        // TODO add play again option
    }
};

#endif // HANOI_H

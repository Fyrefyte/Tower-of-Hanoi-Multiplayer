#pragma once

#ifndef HANOI_H
#define HANOI_H

#include "stack.h"
#include "queue.h"
#include <sstream>
#include <limits>
#include <iostream>
#include <fstream>
#include <iostream>
#include <cstdint>

#include <charconv>

#define CLEAR_CONSOLE std::cout << "\x1b[H\x1b[2J" << std::flush;
#define CONT_REMINDER "\n(Enter to continue)"
#define ENTER_TO_CONT(prevInput) std::cout << CONT_REMINDER; \
                                 std::cin.clear(); \
                                 if (prevInput) std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); \
                                 std::cin.get(); \
                                 CLEAR_CONSOLE;
#define CONFIRM_COND(inp) (inp == 't' || inp == 'T' || inp == 'y' || inp == 'Y' || inp == '1')
#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define BOLD        "\033[1m"
#define END_BOLD    "\033[21m"
#define ITALIC      "\033[3m"
#define END_ITALIC  "\033[23m"
#define UNDERLINE   "\033[4m"
#define END_UNDERLINE "\033[24m"

template <typename T>
T stringToNumericFast(std::string_view str) {
    T result;
    // std::from_chars takes a raw pointer range
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);

    // Check if the conversion succeeded and consumed the entire string
    if (ec == std::errc{} && ptr == str.data() + str.size()) {
        return result;
    }

    return -1; // Conversion failed or partial match
}

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
    void clear() {
        moves.clear();
    }
    size_t movesLeft() { return moves.length(); }
    std::string save(const std::string saveSignature) {
        std::ofstream csvFile("savedSeq" + saveSignature + ".csv");
        if (!csvFile.is_open()) throw std::runtime_error("New save file with the given signature failed to open: savedSeq" + saveSignature + ".csv");
        csvFile << "From,To\n";
        while (moves.length() > 0) {
            const Move<T>* next = getNextMove();
            csvFile << static_cast<size_t>(next->getFrom()) << ',' << static_cast<size_t>(next->getTo()) << "\n";
            delete next;
        }
        return "savedSeq" + saveSignature + ".csv";
    }
    void load(const std::string saveSignature) {
        std::ifstream csvFile("savedSeq" + saveSignature + ".csv");
        if (!csvFile.is_open()) throw std::runtime_error("Save file with the given signature failed to open: savedSeq" + saveSignature + ".csv");
        std::string line;
        std::getline(csvFile, line); // Remove header
        while (std::getline(csvFile, line)) {
            size_t delimiter = line.find(",");
            addMove(stringToNumericFast<T>(line.substr(0, delimiter)), stringToNumericFast<T>(line.substr(delimiter + 1)));
        }
    }
};

template <typename T = size_t, T maxSize = 20, T numTowers = 3>
class Hanoi
{
private:
    T size;
    Stack<T, maxSize> towers[numTowers];
    const bool useSeq;
    MoveSeq<T>* moveSeq;
    T pieces[maxSize];
    T goal;
    const T towerCount = numTowers;
    size_t moveCount = 0;
    bool randomized = false;
public:
    Hanoi(T = 5, T = 2);
    Hanoi(MoveSeq<T>*, T = 5, T = 2);
    bool canMoveFrom(T);
    bool canMoveTo(T);
    bool canMove(T, T);
    bool checkWin();
    bool movePiece(T, T);
    void reset();
    void randomize();
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
        if (towers.useSeq) {
            std::cout << "Save this sequence (y/n)? ";
            char inpChar;
            std::cin >> inpChar;
            if (CONFIRM_COND(inpChar)) {
                std::string fileString = std::to_string(static_cast<unsigned int>(towers.size));
                std::cout << "Saved to " << towers.moveSeq->save(fileString) << std::endl;
            }
        }
    }
};

template <typename T = size_t, T maxSize = 20, T numTowers = 3>
class HanoiAuto
{
private:
    Hanoi<T, maxSize, numTowers> hanoi;
    MoveSeq<T> moves;
    size_t currentMove = 0;
    const T size;
public:
    HanoiAuto(T size = 5, T goalTower = 2) : size(size), hanoi(size, goalTower) {}
    void setMoves(MoveSeq<T> newMoves) { moves = newMoves; }
    T movesLeft() { return moves.movesLeft(); }
    void setBestMoves() {
        moves.clear();
        hanoi.reset();
        // Alternate between:
        // - Moving the top piece "clockwise" if size is even or "counterclockwise" if size is odd
        // - Performing the only other legal move
        // The first and last moves must both be top piece moves, so we check win only after the top piece move
        if (size % 2 == 0) { // Even case
            hanoi.movePiece(0, 1);
            moves.addMove(0, 1);
            T topPieceFrom = 1;
            T topPieceTo = 2;
            T otherFrom = 0;
            while (!hanoi.checkWin()) {
                // Other legal move
                if (hanoi.canMove(topPieceTo, otherFrom)) {
                    hanoi.movePiece(topPieceTo, otherFrom);
                    moves.addMove(topPieceTo, otherFrom);
                }
                else {
                    hanoi.movePiece(otherFrom, topPieceTo);
                    moves.addMove(otherFrom, topPieceTo);
                }
                // Top piece rotation
                hanoi.movePiece(topPieceFrom, topPieceTo);
                moves.addMove(topPieceFrom, topPieceTo);
                topPieceFrom = (topPieceFrom + 1) % 3;
                topPieceTo = (topPieceTo + 1) % 3;
                otherFrom = (otherFrom + 1) % 3;
            }
        }
        else { // Odd case
            hanoi.movePiece(0, 2);
            moves.addMove(0, 2);
            T topPieceFrom = 2;
            T topPieceTo = 1;
            T otherFrom = 0;
            while (!hanoi.checkWin()) {
                // Other legal move
                if (hanoi.canMove(topPieceTo, otherFrom)) {
                    hanoi.movePiece(topPieceTo, otherFrom);
                    moves.addMove(topPieceTo, otherFrom);
                }
                else {
                    hanoi.movePiece(otherFrom, topPieceTo);
                    moves.addMove(otherFrom, topPieceTo);
                }
                // Top piece rotation
                hanoi.movePiece(topPieceFrom, topPieceTo);
                moves.addMove(topPieceFrom, topPieceTo);
                topPieceFrom = (topPieceFrom + 2) % 3;
                topPieceTo = (topPieceTo + 2) % 3;
                otherFrom = (otherFrom + 2) % 3;
            }
        }
        hanoi.reset();
    }
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
    const T size;
    const T towerCount = numTowers;
    const uint8_t diskMode;
    StackPaired<T, T, maxSize> towers[numTowers];
    T pieces[maxSize];
    T players[numPlayers];
    T goals[numPlayers][numGoals];
    T starts[numPlayers];
    T turn = 0;
    size_t moveCount = 0;
public:
    HanoiMultiplayer(T, T(&)[numPlayers], T(&)[numPlayers][numGoals], uint8_t = 0);
    bool canMoveFrom(T);
    bool canMoveTo(T);
    bool canMove(T, T);
    bool checkOwns(T, T);
    T goalAt(T);
    T checkWin();
    bool movePiece(T, T);
    void reset();
    static char getPlayerChar(const T);
    static std::string getPlayerColor(const T);
    std::ostringstream computeSegment(const T, const T, const char, const std::string = "", const std::string = "");

    friend void printTurn(HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>& towers) {
        std::cout << towers.getPlayerColor(towers.turn) << "Player " << static_cast<size_t>(towers.turn)+1 << "'s turn" << RESET;
        if (towers.moveCount < numPlayers) {
            if (towers.diskMode == 0)
                std::cout << std::string(15, ' ') << "____" << std::endl << "(Your pieces look like this: |" << UNDERLINE
                          << std::string(4, towers.getPlayerChar(towers.turn)) << END_UNDERLINE << "|)" << std::endl;
        }
    }

    friend void printTowers(HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>& towers) {
#define TOWER_AT1(tower, x) (x < towers.towers[tower].length() ? *towers.towers[tower].at1(x) : 0)
#define TOWER_AT2(tower, x) (x < towers.towers[tower].length() ? *towers.towers[tower].at2(x) : -1)
#define TOWER_SEPARATION " ";
#define PLAYER_CHAR(tower, x) towers.diskMode == 0 ? getPlayerChar(TOWER_AT2(tower, x)) : (towers.diskMode == 1 ? static_cast<size_t>(TOWER_AT2(tower, x)) + '1' : '_')
#define PLAYER_COLOR(tower, x) getPlayerColor(TOWER_AT2(tower, x))

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
            std::cout << towers.computeSegment(0, TOWER_AT1(i, towers.size - 1), PLAYER_CHAR(i, towers.size - 1), "", PLAYER_COLOR(i, towers.size - 1)).str() << TOWER_SEPARATION;
        std::cout << std::endl;

        // Middle layers
        for (T i = towers.size - 1; i > 0; i--) {
            std::cout << TOWER_SEPARATION;
            for (T j = 0; j < towers.towerCount; j++)
                std::cout << towers.computeSegment(TOWER_AT1(j, i), TOWER_AT1(j, i-1), PLAYER_CHAR(j, i), PLAYER_COLOR(j, i), PLAYER_COLOR(j, i-1)).str() << TOWER_SEPARATION;
            std::cout << std::endl;
        }

        // Bottom layer
        std::cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++)
            std::cout << towers.computeSegment(TOWER_AT1(i, 0), 0, PLAYER_CHAR(i, 0), PLAYER_COLOR(i, 0), "").str() << TOWER_SEPARATION;
        std::cout << std::endl;

        // Goal indicators
        std::cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++) {
            std::cout << std::string(towers.size + 1, ' ');
            T goal = towers.goalAt(i);
            if (goal != static_cast<T>(-1)) std::cout << getPlayerColor(goal) << static_cast<size_t>(goal + 1) << '^' << RESET;
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
    }
};

#endif // HANOI_H

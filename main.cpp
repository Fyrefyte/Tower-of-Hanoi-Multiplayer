#include <iostream>
#include "stack.h"
#include "stack.cpp"
#include "hanoi.h"
#include "hanoi.cpp"
#include <cstdint>

using namespace std;

class StackUnitTest {
private:
    static bool failMessage() {
        cout << "Unit test failed" << endl;
        return false;
    }

    static bool successMessage() {
        cout << "Unit test success" << endl;
        return true;
    }
public:
    static bool unitTest(bool verbose = false) {

        cout << "Starting unit test..." << endl;

        Stack<char, 5> testStack;

        char a = 'a';
        char b = 'b';
        char c = 'c';
        char d = 'd';

        testStack.push(&a, &b, &c, &d);

        if (testStack.length() != 4) return failMessage();
        if (verbose) cout << "   Items on stack: " << testStack.length() << endl;

        char currentItem = *testStack.pop();
        if (currentItem != d) return failMessage();
        if (verbose) cout << "      Top: " << currentItem << endl;

        currentItem = *testStack.pop();
        if (currentItem != c) return failMessage();
        if (verbose) cout << "      2nd: " << currentItem << endl;

        currentItem = *testStack.pop();
        if (currentItem != b) return failMessage();
        if (verbose) cout << "      3rd: " << currentItem << endl;

        currentItem = *testStack.pop();
        if (currentItem != a) return failMessage();
        if (verbose) cout << "      4th: " << currentItem << endl;

        if (testStack.length() != 0) return failMessage();
        if (verbose) cout << "   Items on the stack now: " << testStack.length() << endl;

        try {
            currentItem = *testStack.pop();
            return false;
        }
        catch (exception e) {
            if (verbose) cout << "   Underflow caught" << endl;
        }

        testStack.push(&a, &a, &a, &a, &a);
        try {
            testStack.push(&a);
        }
        catch (exception e) {
            if (verbose) cout << "   Overflow caught" << endl;
        }

        return successMessage();
    }
};

void help() {
#define EXAMPLE_TOWERS "\n   _[]_        []         []\n  |____|       []         []\n |______|      []         []\n|________|     []         []"
#define EXAMPLE_WIN "\n    []         []        _[]_\n    []     ----[]--->   |____|\n    []         []      |______|\n    []         []     |________|"
#define EXAMPLE_MOVES "\n   _[]_        []         []\n  |////|  -->  []         []  Valid move\n |______|     _[]_        []\n|________|   |////|       []"
#define EXAMPLE_SIZE_RULE "\n    []         []         []\n  __[]__ -X> __[]__       []  Invalid move\n |//////|   |//////|      []\n|________|   |____|       []"
#define EXAMPLE_2PLAYER "\n   _[]_        []         []         []        _[]_\n  |____|       []         []         []       |////|\n |______|      []         []         []      |//////|\n|________|     []         []         []     |////////|\n    2^         2^                    1^         1^"
    CLEAR_CONSOLE
    cout << "This program is an adaptation of the popular mathematical puzzle called the Tower of Hanoi." << endl;
    ENTER_TO_CONT(true);
    cout << "In this puzzle, there are 3 poles on which you can stack disks.\nThe first pole starts with a stack of disks in decending order, so the smallest on top and largest on bottom." << endl;
    cout << EXAMPLE_TOWERS << endl;
    ENTER_TO_CONT(false);
    cout << "The goal is to transfer the entire tower from the leftmost pole to the rightmost pole." << endl;
    cout << EXAMPLE_WIN << endl;
    ENTER_TO_CONT(false);
    cout << "In order to do so, you may move one disk at a time, without ever placing a disk on top of one smaller than itself." << endl;
    cout << EXAMPLE_MOVES << endl << endl;
    cout << EXAMPLE_SIZE_RULE << endl;
    ENTER_TO_CONT(false);
    cout << "An interesting property of this ruleset is that any size of tower is solvable with only 3 poles in just 2^n - 1 moves, where n is the number of disks." << endl;
    cout << "Check out https://en.wikipedia.org/wiki/Tower_of_Hanoi for more on the mathematical puzzle." << endl;
    cout << "I recommend trying to mess around find your own solution before looking here, as it contains the general optimal solution." << endl;
    ENTER_TO_CONT(false);
    cout << "In addition to the 1-player mode, which mirrors the original puzzle, there are 2, 4, and 6-player modes available." << endl;
    cout << "All the multiplayer modes are similar in nature, where several players are racing to complete their tower before the others." << endl;
    ENTER_TO_CONT(false);
    cout << "The rules are the same as the 1-player rule, except you can only move your own pieces and you must get your tower to one of the opposing poles, as indicated." << endl;
    cout << "Building a complete tower on any of the indicated goal poles is considered a win." << endl;
    cout << EXAMPLE_2PLAYER << endl;
    ENTER_TO_CONT(false);
    cout << "A couple tips:" << endl;
    cout << " - When entering the to-from values in modes with fewer than 10 poles, you can enter both values simultaneously (e.g. entering \"12\" to mean \"from 1 to 2\")" << endl;
    cout << " - Try any algorithm you come up with on both a 3-size tower and a 4-size tower during testing. Even and odd-height towers work slightly differently." << endl;
    cout << " - Any tower over the size of 6 takes at least 100 moves to complete. You have been warned." << endl;
    cout << " - Don't jump right into the 20-size tower. It takes over a million moves to solve. Honestly, don't even bother." << endl << endl;
    cout << "Good luck!" << endl;
    ENTER_TO_CONT(false);
}

void intro() {
    // #define NUM_GAMEMODES 2
    cout << "Welcome to the Tower of Hanoi!\nSelect number of players:\n(1) 1-Player\n(2) 2-Player\n(4) 4-Player\n(6) 6-Player\n(0) View/Replay Solution\n\nEnter anything else for help and details" << endl;
    uint8_t inp;
    while (true) {
        char inpChar;
        cin >> inpChar;
        try {
            inp = static_cast<uint8_t>(inpChar - '0');
            break;
            // if (inp >= 1 && inp <= NUM_GAMEMODES) break;
        }
        catch (exception e) {}
    }
    uint8_t size;
    bool pattern;
    if (inp == 0 || inp == 1 || inp == 2 || inp == 4 || inp == 6) {
        cout << "What size of towers do you want? (min 3, max 20)" << endl;
        while (true) {
            string inpString;
            getline(cin, inpString);
            try {
                size = static_cast<uint8_t>(stoi(inpString));
                if (size >= 3 && size <= 20) break;
            }
            catch (exception e) {}
        }
        if (inp > 1) {
            cout << "Would you rather have:\n(1) Patterned disks (like |////|) or\n(2) Numbered disks (like |1111|)?" << endl;
            while (true) {
                char inpChar;
                cin >> inpChar;
                try {
                    if (inpChar == '1' || inpChar == '2') {
                        pattern = inpChar == '1';
                        break;
                    }
                }
                catch (exception e) {}
            }
        }
    }
    switch (inp) {
        case 0: {
            MoveSeq<uint8_t> seq;
            seq.load(to_string(size));
            HanoiAuto<uint8_t> hanoiAuto;
            hanoiAuto.setMoves(seq);
        }
        case 1: {
            MoveSeq<uint8_t> seq;
            Hanoi<uint8_t> hanoi = Hanoi<uint8_t>(size);
            gameLoop(hanoi);
            break;
        }
        case 2: {
            uint8_t startTowers[2] = {0, 4};
            uint8_t goalTowers[2][2] = {{3, 4}, {0, 1}};
            HanoiMultiplayer<uint8_t> hanoi = HanoiMultiplayer<uint8_t>(size, startTowers, goalTowers, pattern);
            gameLoop(hanoi);
            break;
        }
        case 4: {
            uint8_t startTowers[4] = {0, 2, 6, 8};
            uint8_t goalTowers[4][2] = {{5, 6}, {7, 8}, {2, 3}, {0, 1}};
            HanoiMultiplayer<uint8_t, 20, 9, 4> hanoi = HanoiMultiplayer<uint8_t, 20, 9, 4>(size, startTowers, goalTowers, pattern);
            gameLoop(hanoi);
            break;
        }
        case 6: {
            uint8_t startTowers[6] = {0, 2, 4, 8, 10, 12};
            uint8_t goalTowers[6][2] = {{7, 8}, {11, 12}, {9, 10}, {4, 5}, {2, 3}, {0, 1}};
            HanoiMultiplayer<uint8_t, 20, 13, 6> hanoi = HanoiMultiplayer<uint8_t, 20, 13, 6>(size, startTowers, goalTowers, pattern);
            gameLoop(hanoi);
            break;
        }
        default: { // help page
            help();
        }
    }
}

int main()
{
    intro();

    // MoveSeq seq;
    // seq.addMove(0, 1);
    // seq.addMove(1, 2);
    // seq.addMove(0, 4);

    // seq.save("Test");

    // seq.load("Test");

    // HanoiAuto aut;
    // aut.setMoves(seq);

    // while (playNextMove(aut));

    return 0;
}

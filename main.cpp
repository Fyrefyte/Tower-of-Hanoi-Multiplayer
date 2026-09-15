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

#define RESET       "\033[0m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define BOLD        "\033[1m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"

template <typename... Args>
void print(string str, Args... modifiers) {
    ((cout << modifiers), ...);
    cout << str << RESET << flush;
}

template <typename... Args>
void println(string str = "", Args... modifiers) {
    ((cout << modifiers), ...);
    cout << str << RESET << endl;
}

void help() {
#define EXAMPLE_TOWERS "\n   _[]_        []         []\n  |____|       []         []\n |______|      []         []\n|________|     []         []"
#define EXAMPLE_WIN "\n    []         []        _[]_\n    []     ----[]--->   |____|\n    []         []      |______|\n    []         []     |________|"
#define EXAMPLE_MOVES "\n   _[]_        []         []\n  |////|  -->  []         []  Valid move\n |______|     _[]_        []\n|________|   |////|       []"
#define EXAMPLE_SIZE_RULE "\n    []         []         []\n  __[]__ -X> __[]__       []  Invalid move\n |//////|   |//////|      []\n|________|   |____|       []"
#define EXAMPLE_2PLAYER "\n   _[]_        []         []         []        _[]_\n  |____|       []         []         []       |////|\n |______|      []         []         []      |//////|\n|________|     []         []         []     |////////|\n    2^         2^                    1^         1^"
    CLEAR_CONSOLE
    println("This program is an adaptation of the popular mathematical puzzle called the Tower of Hanoi.");
    ENTER_TO_CONT(true);
    println("In this puzzle, there are 3 poles on which you can stack disks.\nThe first pole starts with a stack of disks in decending order, so the smallest on top and largest on bottom.");
    println(EXAMPLE_TOWERS);
    ENTER_TO_CONT(false);
    println("The goal is to transfer the entire tower from the leftmost pole to the rightmost pole.");
    println(EXAMPLE_WIN);
    ENTER_TO_CONT(false);
    println("In order to do so, you may move one disk at a time, without ever placing a disk on top of one smaller than itself.");
    println(EXAMPLE_MOVES);
    println();
    println(EXAMPLE_SIZE_RULE);
    ENTER_TO_CONT(false);
    println("An interesting property of this ruleset is that any size of tower is solvable with only 3 poles in just 2^n - 1 moves, where n is the number of disks.");
    println("Check out https://en.wikipedia.org/wiki/Tower_of_Hanoi for more on the mathematical puzzle.");
    println("I recommend trying to mess around find your own solution before looking here, as it contains the general optimal solution.");
    ENTER_TO_CONT(false);
    println("In addition to the 1-player mode, which mirrors the original puzzle, there are 2, 4, and 6-player modes available.");
    println("All the multiplayer modes are similar in nature, where several players are racing to complete their tower before the others.");
    ENTER_TO_CONT(false);
    println("The rules are the same as the 1-player rule, except you can only move your own pieces and you must get your tower to one of the opposing poles, as indicated.");
    println("Building a complete tower on any of the indicated goal poles is considered a win.");
    println(EXAMPLE_2PLAYER);
    ENTER_TO_CONT(false);
    println("A couple tips:");
    println(" - When entering the to-from values in modes with fewer than 10 poles, you can enter both values simultaneously (e.g. entering \"12\" to mean \"from 1 to 2\")");
    println(" - Try any algorithm you come up with on both a 3-size tower and a 4-size tower during testing. Even and odd-height towers work slightly differently.");
    println(" - Any tower over the size of 6 takes at least 100 moves to complete. You have been warned.");
    println(" - Don't jump right into the 20-size tower. It takes over a million moves to solve. Honestly, don't even bother.");
    println();
    println("Good luck!");
    ENTER_TO_CONT(false);
}

void game() {
    println("Welcome to the Tower of Hanoi!", RED, BOLD);
    println("Created by Leo Canales, MIT License 2026\nEnter anything not listed below for help and details\n", RED);
    println("Select number of players:", BLUE, BOLD);
    println("(1) \033[4m1\033[24m-Player\n(2) \033[4m2\033[24m-Player\n(4) \033[4m4\033[24m-Player\n(6) \033[4m6\033[24m-Player\n", BLUE);
    println("Or, choose an autoplay mode:", GREEN, BOLD);
    println("(V) \033[4mV\033[24miew Saved Solution\n(R) \033[4mR\033[24meplay Saved Solution", GREEN);
    println("(A) View \033[4mA\033[24muto Solve Solution\n(P) \033[4mP\033[24mlay Auto Solve Solution\n", GREEN);
    // println("Or, check out an experiment:", YELLOW, BOLD);
    // println("(B) \033[4mB\033[24micolor Hanoi\n", YELLOW);
    print("> ");
    char mode;
    cin >> mode;
    uint8_t size;
    uint8_t pattern;
    if (mode == '1' || mode == '2' || mode == '4' || mode == '6' ||
        mode == 'R' || mode == 'r' || mode == 'V' || mode == 'v' ||
        mode == 'A' || mode == 'a' || mode == 'P' || mode == 'p') {
        CLEAR_CONSOLE
        print("Enter tower size", BOLD);
        println(" (min 3, max 20)");
        print("> ");
        while (true) {
            string inpString;
            getline(cin, inpString);
            try {
                size = static_cast<uint8_t>(stoi(inpString));
                if (size >= 3 && size <= 20) break;
            }
            catch (exception e) {}
        }
        if (mode == '2' || mode == '4' || mode == '6') {
            print("\nColors? ", BOLD);
            print("(y/n) ");
            char inp;
            cin >> inp;
            bool colors = CONFIRM_COND(inp);
            if (colors) {
                print("Choose disk patterns:   ");
                print("_", BLUE);
                print("[]");
                println("_", BLUE);
                print("(1) Patterned disks -> ");
                print("|", BLUE);
                print("////", BLUE, UNDERLINE);
                println("|", BLUE);
                print("(2) Numbered disks -> ");
                print("|", BLUE);
                print("222222", BLUE, UNDERLINE);
                println("|", BLUE);
                print("(3) Plain disks ---> ");
                print("|", BLUE);
                print("________", BLUE, UNDERLINE);
                println("|", BLUE);
                print("> ");
                while (true) {
                    char inpChar;
                    cin >> inpChar;
                    try {
                        if (inpChar == '1' || inpChar == '2' || inpChar == '3') {
                            pattern = inpChar - '1';
                            break;
                        }
                    } catch (exception e) {}
                }
            }
            else {
                print("Choose disk patterns:   ");
                println("_[]_");
                print("(1) Patterned disks -> ");
                print("|");
                print("////", UNDERLINE);
                println("|");
                print("(2) Numbered disks -> ");
                print("|");
                print("222222", UNDERLINE);
                print("|\n> ");
                while (true) {
                    char inpChar;
                    cin >> inpChar;
                    try {
                        if (inpChar == '1' || inpChar == '2') {
                            pattern = inpChar - '1';
                            break;
                        }
                    } catch (exception e) {}
                }
            }
        }
    }
    switch (mode) {
        case 'v':
        case 'V': {
            MoveSeq<uint8_t> seq;
            try {
                seq.load(to_string(size));
                HanoiAuto<uint8_t> hanoiAuto = HanoiAuto<uint8_t>(size);
                hanoiAuto.setMoves(seq);
                CLEAR_CONSOLE
                showFullMoveset(hanoiAuto, true);
            } catch (exception e) {
                println("No solution has been saved for this tower size", RED);
            }
            break;
        }
        case 'r':
        case 'R': {
            MoveSeq<uint8_t> seq;
            try {
                seq.load(to_string(size));
                HanoiAuto<uint8_t> hanoiAuto = HanoiAuto<uint8_t>(size);
                hanoiAuto.setMoves(seq);
                CLEAR_CONSOLE
                showCurrentMove(hanoiAuto);
                while (playNextMove(hanoiAuto));
            } catch (exception e) {
                println("No solution has been saved for this tower size", RED);
            }
            break;
        }
        case 'a':
        case 'A': { // TODO build this
            HanoiAuto<uint8_t> hanoiAuto = HanoiAuto<uint8_t>(size);
            hanoiAuto.setBestMoves();
            CLEAR_CONSOLE
            showFullMoveset(hanoiAuto, true);
            break;
        }
        case 'p':
        case 'P': {
            HanoiAuto<uint8_t> hanoiAuto = HanoiAuto<uint8_t>(size);
            hanoiAuto.setBestMoves();
            CLEAR_CONSOLE
            showCurrentMove(hanoiAuto);
            while (playNextMove(hanoiAuto));
            break;
        }
        case '1': {
            MoveSeq<uint8_t> seq;
            Hanoi<uint8_t> hanoi = Hanoi<uint8_t>(&seq, size);
            gameLoop(hanoi);
            break;
        }
        case '2': {
            uint8_t startTowers[2] = {0, 4};
            uint8_t goalTowers[2][2] = {{3, 4}, {0, 1}};
            HanoiMultiplayer<uint8_t> hanoi = HanoiMultiplayer<uint8_t>(size, startTowers, goalTowers, pattern);
            gameLoop(hanoi);
            break;
        }
        case '4': {
            uint8_t startTowers[4] = {0, 2, 6, 8};
            uint8_t goalTowers[4][2] = {{5, 6}, {7, 8}, {2, 3}, {0, 1}};
            HanoiMultiplayer<uint8_t, 20, 9, 4> hanoi = HanoiMultiplayer<uint8_t, 20, 9, 4>(size, startTowers, goalTowers, pattern);
            gameLoop(hanoi);
            break;
        }
        case '6': {
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
    do {
        CLEAR_CONSOLE
        game();
        print("Go again (y/n)? ", BLUE);
        char cont;
        cin >> cont;
        if (!CONFIRM_COND(cont)) break;
    } while (true);

    return 0;
}

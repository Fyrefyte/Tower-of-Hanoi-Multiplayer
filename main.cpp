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

void intro() {
    #define NUM_GAMEMODES 2
    cout << "Welcome to the Tower of Hanoi!\nSelect number of players:\n(1) 1-Player\n(2) 2-Player" << endl;
    uint8_t inp;
    while (true) {
        char inpChar;
        cin >> inpChar;
        try {
            inp = static_cast<uint8_t>(inpChar - '0');
            if (inp >= 1 && inp <= NUM_GAMEMODES) break;
        }
        catch (exception e) {}
    }
    cout << "What size of towers do you want? (min 3, max 20)" << endl;
    uint8_t size;
    while (true) {
        string inpString;
        getline(cin, inpString);
        try {
            size = static_cast<uint8_t>(stoi(inpString));
            if (size >= 3 && size <= 20) break;
        }
        catch (exception e) {}
    }
    switch (inp) {
        case 2: {
            uint8_t startTowers[2] = {0, 4};
            uint8_t goalTowers[2][2] = {{3, 4}, {0, 1}};
            HanoiMultiplayer<uint8_t> hanoi = HanoiMultiplayer<uint8_t>(size, startTowers, goalTowers);
            gameLoop(hanoi);
            break;
        }
        default: { // 1 player mode
            Hanoi<uint8_t> hanoi = Hanoi<uint8_t>(size);
            gameLoop(hanoi);
            break;
        }
    }
}

int main()
{
    // StackUnitTest::unitTest(false);

    Hanoi<size_t, 5> hanoi;

    intro();

    // gameLoop(hanoi);

    // cout << hanoi.computeSegment(3, 4).str() << "|" << hanoi.computeSegment(0, 0).str() << "|" << hanoi.computeSegment(0, 0).str() << endl;

    // cout << HanoiSinglePlayer<>::computeSegment(0, 3) << HanoiSinglePlayer<>::computeSegment(0, 1) << endl;
    // cout << HanoiSinglePlayer<>::computeSegment(3, 4) << HanoiSinglePlayer<>::computeSegment(1, 2) << endl;
    // cout << HanoiSinglePlayer<>::computeSegment(4, 2) << HanoiSinglePlayer<>::computeSegment(2, 3) << endl;
    // cout << HanoiSinglePlayer<>::computeSegment(2, 1) << HanoiSinglePlayer<>::computeSegment(3, 4) << endl;
    // cout << HanoiSinglePlayer<>::computeSegment(1, 5) << HanoiSinglePlayer<>::computeSegment(4, 5) << endl;
    // cout << HanoiSinglePlayer<>::computeSegment(5, 0) << HanoiSinglePlayer<>::computeSegment(5, 0) << endl;

    return 0;
}

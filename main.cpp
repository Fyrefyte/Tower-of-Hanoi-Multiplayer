#include <iostream>
#include "stack.h"
#include "stack.cpp"
#include <sstream>
#include <type_traits>
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

template <typename T = size_t, T maxSize = 20/*, T numTowers = 3*/>
class Hanoi {
private:
    T size;
    Stack<T, maxSize> towerL;
    Stack<T, maxSize> towerC;
    Stack<T, maxSize> towerR;
    //Stack<T, maxSize> towers[numTowers];
    T pieces[maxSize];
public:
    Hanoi() {
        Hanoi(5);
    }
    Hanoi(T towerSize) {
        static_assert(is_arithmetic_v<T>, "Typename T must be numeric");
        size = towerSize;
        for (T i = 0; i < size; i++) {
            pieces[i] = i + 1;
        }
        reset();
    }
    bool isLegalMove(Stack<T, maxSize>* tower1, Stack<T, maxSize>* tower2) {
        if (tower1->isEmpty() || tower2->length() == size || !(tower2->isEmpty() || tower1->top() < tower2->top())) return false;
        else return true;
    }
    bool checkWin() {
        if (towerR.length() == size) return true;
        return false;
    }
    bool movePiece(Stack<T, maxSize>* tower1, Stack<T, maxSize>* tower2) {
        if (!isLegalMove(tower1, tower2)) return false;
        tower2->push(tower1->pop());
        return true;
    }
    void reset() {
        towerL.empty();
        towerC.empty();
        towerR.empty();

        for (T i = size; i > 0; i--) {
            towerL.push(&pieces[i-1]);
        }
    }
    Stack<T, maxSize>& getTowerL() {return towerL;}
    Stack<T, maxSize>& getTowerC() {return towerC;}
    Stack<T, maxSize>& getTowerR() {return towerR;}
    ostringstream computeSegment(const T thisLayerWidth, const T belowLayerWidth) {
        #define MIDDLE_SPACING 2
        #define SPACE_CHAR ' '
        #define H_LINE_CHAR '_'
        #define V_LINE_CHAR '|'
        #define POLE_STR "[]"
        #define POLE_CAP_STR "[]"

        ostringstream out;

        if (thisLayerWidth == 0 && belowLayerWidth == 0) {
            // Spacing layer
            out << string(size + 1, SPACE_CHAR)                                 // Space to the left of the pole
                << POLE_STR                                                     // Pole
                << string(size + 1, SPACE_CHAR);                                // Space to the right of the pole
        }
        else if (thisLayerWidth == 0 && belowLayerWidth != 0) {
            // Cap layer
            out << string(size - belowLayerWidth + 1, SPACE_CHAR)               // Space to the left of the cap
                << string(belowLayerWidth, H_LINE_CHAR)                         // Left cap
                << POLE_CAP_STR                                                 // The pole connecting to the other bits
                << string(belowLayerWidth, H_LINE_CHAR)                         // Right cap
                << string(size - belowLayerWidth + 1, SPACE_CHAR);              // Space to the right of the cap
        }
        else if (thisLayerWidth + 1 >= belowLayerWidth) {
            // Normal layer
            out << string(size - thisLayerWidth, SPACE_CHAR)                    // Space to the left of the left wall
                << string(1, V_LINE_CHAR)                                       // Left wall
                << string(MIDDLE_SPACING + thisLayerWidth*2, H_LINE_CHAR)       // The inner bottom surface
                << string(1, V_LINE_CHAR)                                       // Right wall
                << string(size - thisLayerWidth, SPACE_CHAR);                   // Space to the right of the right wall
        }
        else {
            // Layer with skirt
            out << string(size - belowLayerWidth + 1, SPACE_CHAR)               // Space to the left of the skirt
                << string(belowLayerWidth - thisLayerWidth - 1, H_LINE_CHAR)    // Left skirt
                << string(1, V_LINE_CHAR)                                       // Left wall
                << string(MIDDLE_SPACING + thisLayerWidth*2, H_LINE_CHAR)       // The inner bottom surface
                << string(1, V_LINE_CHAR)                                       // Right wall
                << string(belowLayerWidth - thisLayerWidth - 1, H_LINE_CHAR)    // Right skirt
                << string(size - belowLayerWidth + 1, SPACE_CHAR);              // Space to the right of the skirt
        }

        return out;
    }

    friend void clearConsole(Hanoi<T, maxSize>& towers) {
        cout << "\x1b[H\x1b[2J" << flush;
    }
    friend void printTowers(Hanoi<T, maxSize>& towers) {
        #define TOWER_AT(tower, x) (x < tower->length() ? *tower->at(x) : 0)
        #define TOWER_L_AT(x) TOWER_AT(towerL, x)
        #define TOWER_C_AT(x) TOWER_AT(towerC, x)
        #define TOWER_R_AT(x) TOWER_AT(towerR, x)
        #define TOWER_SEPARATION " ";

        ostringstream stream;

        Stack<T, maxSize>* towerL = &towers.towerL;
        Stack<T, maxSize>* towerC = &towers.towerC;
        Stack<T, maxSize>* towerR = &towers.towerR;

        // Top layer
        cout << TOWER_SEPARATION;
        cout << towers.computeSegment(0, TOWER_L_AT(towers.size - 1)).str() << TOWER_SEPARATION;
        cout << towers.computeSegment(0, TOWER_C_AT(towers.size - 1)).str() << TOWER_SEPARATION;
        cout << towers.computeSegment(0, TOWER_R_AT(towers.size - 1)).str() << TOWER_SEPARATION;
        cout << endl;

        // Middle layers
        for (T i = towers.size - 1; i > 0; i--) {
            cout << TOWER_SEPARATION;
            cout << towers.computeSegment(TOWER_L_AT(i), TOWER_L_AT(i-1)).str() << TOWER_SEPARATION;
            cout << towers.computeSegment(TOWER_C_AT(i), TOWER_C_AT(i-1)).str() << TOWER_SEPARATION;
            cout << towers.computeSegment(TOWER_R_AT(i), TOWER_R_AT(i-1)).str() << TOWER_SEPARATION;
            cout << endl;
        }

        // Bottom layer
        cout << TOWER_SEPARATION;
        cout << towers.computeSegment(TOWER_L_AT(0), 0).str() << TOWER_SEPARATION;
        cout << towers.computeSegment(TOWER_C_AT(0), 0).str() << TOWER_SEPARATION;
        cout << towers.computeSegment(TOWER_R_AT(0), 0).str() << TOWER_SEPARATION;
        cout << endl;

        cout << stream.str();
    }

    friend void gameStep(Hanoi<T, maxSize>& towers) {

        // Clear console and display current tower setup
        clearConsole(towers);
        printTowers(towers);

        // Loop for checking invalid move states
        while (true) {
            // Get the tower to move from this step
            uint8_t choice1;
            uint8_t choice2;
            char inpChar;
            Stack<T, maxSize>* tower1;
            Stack<T, maxSize>* tower2;
            while (true) {
                cout << endl << "Select a tower to move from (1, 2, or 3): " << flush;
                try {
                    cin >> inpChar;
                    choice1 = static_cast<uint8_t>(inpChar - '0');
                    if (choice1 >= 1 && choice1 <= 3) {
                        // string choiceStr = (choice1 == 1 ? "Left" : (choice1 == 2 ? "Center" : "Right"));
                        tower1 = (choice1 == 1 ? &towers.towerL : (choice1 == 2) ? &towers.towerC : &towers.towerR);
                        // cout << choiceStr << " tower chosen" << endl;
                        break;
                    }
                }
                catch (exception e) {}
            }

            // Get the tower to move to this step
            while (true) {
                cout << endl << "Select a tower to move to (1, 2, or 3): " << flush;
                try {
                    cin >> inpChar;
                    choice2 = static_cast<uint8_t>(inpChar - '0');
                    if (choice2 >= 1 && choice2 <= 3 && choice2 != choice1) {
                        string choiceStr = (choice2 == 1 ? "Left" : (choice2 == 2 ? "Center" : "Right"));
                        tower2 = (choice2 == 1 ? &towers.towerL : (choice2 == 2) ? &towers.towerC : &towers.towerR);
                        cout << choiceStr << " tower chosen" << endl;
                        break;
                    }
                }
                catch (exception e) {}
            }

            // Actually move the piece
            if (towers.movePiece(tower1, tower2)) break;
            // else cout << "Invalid move, try again" << endl;
        }
    }

    friend void gameLoop(Hanoi<T, maxSize>& towers) {
        while (!towers.checkWin()) gameStep(towers);
        clearConsole(towers);
        printTowers(towers);
        cout << "You win!" << endl;
        // TODO add play again option
    }
};

void intro() {
    #define NUM_GAMEMODES 2
    cout << "Welcome to the Tower of Hanoi!\nSelect a game mode:\n(1) 1-Player\n(2) 2-Player" << endl;
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
        case 2:
            break;
        default: // 1 player mode
            Hanoi<uint8_t> hanoi = Hanoi<uint8_t>(size);
            gameLoop(hanoi);
            break;
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

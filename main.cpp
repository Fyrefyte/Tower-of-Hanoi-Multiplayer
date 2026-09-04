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

template <typename T = size_t, T maxSize = 20, T numTowers = 3>
class Hanoi {
private:
    T size;
    Stack<T, maxSize> towers[numTowers];
    T pieces[maxSize];
    T goal;
    const T towerCount = numTowers;
public:
    Hanoi(T towerSize = 5, T goalTower = 2) {
        static_assert(is_arithmetic_v<T>, "Typename T must be numeric");
        size = towerSize;
        goal = goalTower;
        for (T i = 0; i < size; i++) {
            pieces[i] = i + 1;
        }
        for (T i = 0; i < numTowers; i++) {
            towers[i] = Stack<T, maxSize>();
        }
        reset();
    }
    bool isLegalMove(T tower1, T tower2) {
        if (towers[tower1].isEmpty()
            || towers[tower2].length() == size
            || !(towers[tower2].isEmpty()
                 || towers[tower1].top() < towers[tower2].top())) return false;
        else return true;
    }
    bool checkWin() {
        if (towers[goal].length() == size) return true;
        return false;
    }
    bool movePiece(T tower1, T tower2) {
        if (!isLegalMove(tower1, tower2)) return false;
        towers[tower2].push(towers[tower1].pop());
        return true;
    }
    void reset() {
        for (T i = 0; i < numTowers; i++) {
            towers[i].empty();
        }
        for (T i = size; i > 0; i--) {
            towers[0].push(&pieces[i-1]);
        }
    }
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

    friend void clearConsole(Hanoi<T, maxSize, numTowers>& towers) {
        cout << "\x1b[H\x1b[2J" << flush;
    }
    friend void printTowers(Hanoi<T, maxSize, numTowers>& towers) {
        #define TOWER_AT(tower, x) (x < towers.towers[tower].length() ? *towers.towers[tower].at(x) : 0)
        #define TOWER_SEPARATION " ";

        ostringstream stream;

        // Top layer
        cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++)
            cout << towers.computeSegment(0, TOWER_AT(i, towers.size - 1)).str() << TOWER_SEPARATION;
        cout << endl;

        // Middle layers
        for (T i = towers.size - 1; i > 0; i--) {
            cout << TOWER_SEPARATION;
            for (T j = 0; j < towers.towerCount; j++)
                cout << towers.computeSegment(TOWER_AT(j, i), TOWER_AT(j, i-1)).str() << TOWER_SEPARATION;
            cout << endl;
        }

        // Bottom layer
        cout << TOWER_SEPARATION;
        for (T i = 0; i < towers.towerCount; i++)
            cout << towers.computeSegment(TOWER_AT(i, 0), 0).str() << TOWER_SEPARATION;
        cout << endl;

        cout << stream.str();
    }

    friend void gameStep(Hanoi<T, maxSize, numTowers>& towers) {

        // Clear console and display current tower setup
        clearConsole(towers);
        printTowers(towers);

        // Loop for checking invalid move states
        while (true) {
            // Get the tower to move from this step
            uint8_t choice1;
            uint8_t choice2;
            char inpChar;
            // Stack<T, maxSize>* tower1;
            // Stack<T, maxSize>* tower2;
            while (true) {
                cout << endl << "Select a tower to move from (1 to " << static_cast<size_t>(numTowers) << "): " << flush;
                try {
                    cin >> inpChar;
                    choice1 = static_cast<uint8_t>(inpChar - '0');
                    if (choice1 >= 1 && choice1 <= numTowers) {
                        // string choiceStr = (choice1 == 1 ? "Left" : (choice1 == 2 ? "Center" : "Right"));
                        // tower1 = (choice1 == 1 ? &towers.towerL : (choice1 == 2) ? &towers.towerC : &towers.towerR);
                        // cout << choiceStr << " tower chosen" << endl;
                        break;
                    }
                }
                catch (exception e) {}
            }

            // Get the tower to move to this step
            while (true) {
                cout << endl << "Select a tower to move to (1 to " << static_cast<size_t>(numTowers) << "): " << flush;
                try {
                    cin >> inpChar;
                    choice2 = static_cast<uint8_t>(inpChar - '0');
                    if (choice2 >= 1 && choice2 <= numTowers && choice2 != choice1) {
                        // string choiceStr = (choice2 == 1 ? "Left" : (choice2 == 2 ? "Center" : "Right"));
                        // tower2 = (choice2 == 1 ? &towers.towerL : (choice2 == 2) ? &towers.towerC : &towers.towerR);
                        // cout << choiceStr << " tower chosen" << endl;
                        break;
                    }
                }
                catch (exception e) {}
            }

            // Actually move the piece
            if (towers.movePiece(choice1 - 1, choice2 - 1)) break;
            else cout << "Invalid move, try again" << endl;
        }
    }

    friend void gameLoop(Hanoi<T, maxSize, numTowers>& towers) {
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

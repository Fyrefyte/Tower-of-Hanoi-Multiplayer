#include "hanoi.h"
#include "stack.h"
#include <sstream>
#include <type_traits>
#include <algorithm>

template <typename T, T maxSize, T numTowers>
Hanoi<T, maxSize, numTowers>::Hanoi(T towerSize, T goalTower) {
    static_assert(std::is_arithmetic_v<T>, "Typename T must be numeric");
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

template <typename T, T maxSize, T numTowers, T numPlayers, T numGoals>
HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>::HanoiMultiplayer(T towerSize, T (&startTowers)[numPlayers], T (&goalTowers)[numPlayers][numGoals]) {
    static_assert(std::is_arithmetic_v<T>, "Typename T must be numeric");
    size = towerSize;
    for (T i = 0; i < numPlayers; i++) std::copy(goalTowers[i], goalTowers[i] + numPlayers, goals[i]);
    std::copy(startTowers, startTowers + numPlayers, starts);
    for (T i = 0; i < size; i++) {
        pieces[i] = i + 1;
    }
    for (T i = 0; i < numTowers; i++) {
        towers[i] = StackPaired<T, T, maxSize>();
    }
    for (T i = 0; i < numPlayers; i++) {
        players[i] = i;
    }
    reset();
}

template <typename T, T maxSize, T numTowers>
bool Hanoi<T, maxSize, numTowers>::canMoveFrom(T tower) {
    return (
        tower >= 0 &&
        tower < numTowers &&
        !towers[tower].isEmpty()
    );
}

template <typename T, T maxSize, T numTowers, T numPlayers, T numGoals>
bool HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>::canMoveFrom(T tower) {
    return (
        tower >= 0 &&
        tower < numTowers &&
        !towers[tower].isEmpty() &&
        *towers[tower].top2() == turn
    );
}

template <typename T, T maxSize, T numTowers>
bool Hanoi<T, maxSize, numTowers>::canMoveTo(T tower) {
    return (
        tower >= 0 &&
        tower < numTowers &&
        towers[tower].length() != size
    );
}

template <typename T, T maxSize, T numTowers, T numPlayers, T numGoals>
bool HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>::canMoveTo(T tower) {
    return (
        tower >= 0 &&
        tower < numTowers &&
        towers[tower].length() != size
    );
}

template <typename T, T maxSize, T numTowers>
bool Hanoi<T, maxSize, numTowers>::canMove(T tower1, T tower2) {
    return (
        canMoveFrom(tower1) &&
        canMoveTo(tower2) &&
        (
            towers[tower2].isEmpty() ||
            towers[tower1].top() < towers[tower2].top()
        )
    );
}

template <typename T, T maxSize, T numTowers, T numPlayers, T numGoals>
bool HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>::canMove(T tower1, T tower2) {
    return (
        canMoveFrom(tower1) &&
        canMoveTo(tower2) &&
        (
            towers[tower2].isEmpty() ||
            towers[tower1].top1() < towers[tower2].top1()
        )
    );
}

template <typename T, T maxSize, T numTowers>
bool Hanoi<T, maxSize, numTowers>::checkWin() {
    return (towers[goal].length() == size);
}

template <typename T, T maxSize, T numTowers, T numPlayers, T numGoals>
bool HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>::checkOwns(T tower, T player) {
    for (T i = 0; i < towers[tower].length(); i++) {
        if (*towers[tower].at2(i) != player) return false;
    }
    return true;
}

template <typename T, T maxSize, T numTowers, T numPlayers, T numGoals>
T HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>::checkWin() {
    for (T i = 0; i < numPlayers; i++) {
        for (T j = 0; j < numGoals; j++) {
            if (towers[goals[i][j]].length() == size && checkOwns(goals[i][j], i)) return i;
        }
    }
    return -1; //TODO this might be an issue
}

template <typename T, T maxSize, T numTowers>
bool Hanoi<T, maxSize, numTowers>::movePiece(T tower1, T tower2) {
    if (!canMove(tower1, tower2)) return false;
    towers[tower2].push(towers[tower1].pop());
    return true;
}

template <typename T, T maxSize, T numTowers, T numPlayers, T numGoals>
bool HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>::movePiece(T tower1, T tower2) {
    if (!canMove(tower1, tower2)) return false;
    towers[tower2].push(towers[tower1].top1(), towers[tower1].top2());
    towers[tower1].pop1();
    return true;
}

template <typename T, T maxSize, T numTowers>
void Hanoi<T, maxSize, numTowers>::reset() {
    for (T i = 0; i < numTowers; i++) {
        towers[i].empty();
    }
    for (T i = size; i > 0; i--) {
        towers[0].push(&pieces[i-1]);
    }
}

template <typename T, T maxSize, T numTowers, T numPlayers, T numGoals>
void HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>::reset() {
    // std::cout << "Starts: " << static_cast<size_t>(starts[0]) << ", " << static_cast<size_t>(starts[1]) << std::endl;
    for (T i = 0; i < numTowers; i++) {
        towers[i].empty();
    }
    for (T i = size; i > 0; i--) {
        for (T j = 0; j < numPlayers; j++) {
            // std::cout << "Pushing piece " << static_cast<size_t>(pieces[i-1]) << " for player " << static_cast<size_t>(j) << " at tower " << static_cast<size_t>(starts[j]) << std::endl;
            towers[starts[j]].push(&pieces[i-1], &players[j]);
            // std::cout << "Piece pushed: " << static_cast<size_t>(*towers[starts[j]].top1()) << " for player " << static_cast<size_t>(*towers[starts[j]].top2()) << std::endl;
        }
    }
}

template <typename T, T maxSize, T numTowers>
std::ostringstream Hanoi<T, maxSize, numTowers>::computeSegment(const T thisLayerWidth, const T belowLayerWidth) {
    #define MIDDLE_SPACING 2
    #define SPACE_CHAR ' '
    #define H_LINE_CHAR '_'
    #define V_LINE_CHAR '|'
    #define POLE_STR "[]"
    #define POLE_CAP_STR "[]"

    std::ostringstream out;

    if (thisLayerWidth == 0 && belowLayerWidth == 0) {
        // Spacing layer
        out << std::string(size + 1, SPACE_CHAR)                                 // Space to the left of the pole
            << POLE_STR                                                          // Pole
            << std::string(size + 1, SPACE_CHAR);                                // Space to the right of the pole
    }
    else if (thisLayerWidth == 0 && belowLayerWidth != 0) {
        // Cap layer
        out << std::string(size - belowLayerWidth + 1, SPACE_CHAR)               // Space to the left of the cap
            << std::string(belowLayerWidth, H_LINE_CHAR)                         // Left cap
            << POLE_CAP_STR                                                      // The pole connecting to the other bits
            << std::string(belowLayerWidth, H_LINE_CHAR)                         // Right cap
            << std::string(size - belowLayerWidth + 1, SPACE_CHAR);              // Space to the right of the cap
    }
    else if (thisLayerWidth + 1 >= belowLayerWidth) {
        // Normal layer
        out << std::string(size - thisLayerWidth, SPACE_CHAR)                    // Space to the left of the left wall
            << std::string(1, V_LINE_CHAR)                                       // Left wall
            << std::string(MIDDLE_SPACING + thisLayerWidth*2, H_LINE_CHAR)       // The inner bottom surface
            << std::string(1, V_LINE_CHAR)                                       // Right wall
            << std::string(size - thisLayerWidth, SPACE_CHAR);                   // Space to the right of the right wall
    }
    else {
        // Layer with skirt
        out << std::string(size - belowLayerWidth + 1, SPACE_CHAR)               // Space to the left of the skirt
            << std::string(belowLayerWidth - thisLayerWidth - 1, H_LINE_CHAR)    // Left skirt
            << std::string(1, V_LINE_CHAR)                                       // Left wall
            << std::string(MIDDLE_SPACING + thisLayerWidth*2, H_LINE_CHAR)       // The inner bottom surface
            << std::string(1, V_LINE_CHAR)                                       // Right wall
            << std::string(belowLayerWidth - thisLayerWidth - 1, H_LINE_CHAR)    // Right skirt
            << std::string(size - belowLayerWidth + 1, SPACE_CHAR);              // Space to the right of the skirt
    }

    return out;
}

template <typename T, T maxSize, T numTowers, T numPlayers, T numGoals>
char HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>::getPlayerChar(const T player) {
    switch (player) {
        case static_cast<T>(0): return '_';
        case static_cast<T>(1): return '/';
        case static_cast<T>(2): return 'L';
        case static_cast<T>(3): return '#';
        case static_cast<T>(4): return '+';
        default: return '?';
    }
}

template <typename T, T maxSize, T numTowers, T numPlayers, T numGoals>
std::ostringstream HanoiMultiplayer<T, maxSize, numTowers, numPlayers, numGoals>::computeSegment(const T thisLayerWidth, const T belowLayerWidth, const char playerChar) {
    #define MIDDLE_SPACING 2
    #define SPACE_CHAR ' '
    #define H_LINE_CHAR '_'
    #define V_LINE_CHAR '|'
    #define POLE_STR "[]"
    #define POLE_CAP_STR "[]"

    std::ostringstream out;

    if (thisLayerWidth == 0 && belowLayerWidth == 0) {
        // Spacing layer
        out << std::string(size + 1, SPACE_CHAR)                                 // Space to the left of the pole
            << POLE_STR                                                          // Pole
            << std::string(size + 1, SPACE_CHAR);                                // Space to the right of the pole
    }
    else if (thisLayerWidth == 0 && belowLayerWidth != 0) {
        // Cap layer
        out << std::string(size - belowLayerWidth + 1, SPACE_CHAR)               // Space to the left of the cap
            << std::string(belowLayerWidth, H_LINE_CHAR)                         // Left cap
            << POLE_CAP_STR                                                      // The pole connecting to the other bits
            << std::string(belowLayerWidth, H_LINE_CHAR)                         // Right cap
            << std::string(size - belowLayerWidth + 1, SPACE_CHAR);              // Space to the right of the cap
    }
    else if (thisLayerWidth + 1 >= belowLayerWidth) {
        // Normal layer
        out << std::string(size - thisLayerWidth, SPACE_CHAR)                    // Space to the left of the left wall
            << std::string(1, V_LINE_CHAR)                                       // Left wall
            << std::string(MIDDLE_SPACING + thisLayerWidth*2, playerChar)        // The inner bottom surface (defined by player)
            << std::string(1, V_LINE_CHAR)                                       // Right wall
            << std::string(size - thisLayerWidth, SPACE_CHAR);                   // Space to the right of the right wall
    }
    else {
        // Layer with skirt
        out << std::string(size - belowLayerWidth + 1, SPACE_CHAR)               // Space to the left of the skirt
            << std::string(belowLayerWidth - thisLayerWidth - 1, H_LINE_CHAR)    // Left skirt
            << std::string(1, V_LINE_CHAR)                                       // Left wall
            << std::string(MIDDLE_SPACING + thisLayerWidth*2, playerChar)        // The inner bottom surface (defined by player)
            << std::string(1, V_LINE_CHAR)                                       // Right wall
            << std::string(belowLayerWidth - thisLayerWidth - 1, H_LINE_CHAR)    // Right skirt
            << std::string(size - belowLayerWidth + 1, SPACE_CHAR);              // Space to the right of the skirt
    }

    return out;
}

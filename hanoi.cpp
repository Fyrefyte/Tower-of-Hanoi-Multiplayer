#pragma once

#include "hanoi.h"
#include "stack.h"
#include <sstream>
#include <type_traits>

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

template <typename T, T maxSize, T numTowers>
bool Hanoi<T, maxSize, numTowers>::isLegalMove(T tower1, T tower2) {
    if (towers[tower1].isEmpty()
        || towers[tower2].length() == size
        || !(towers[tower2].isEmpty()
             || towers[tower1].top() < towers[tower2].top())) return false;
    else return true;
}

template <typename T, T maxSize, T numTowers>
bool Hanoi<T, maxSize, numTowers>::checkWin() {
    if (towers[goal].length() == size) return true;
    return false;
}

template <typename T, T maxSize, T numTowers>
bool Hanoi<T, maxSize, numTowers>::movePiece(T tower1, T tower2) {
    if (!isLegalMove(tower1, tower2)) return false;
    towers[tower2].push(towers[tower1].pop());
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
            << POLE_STR                                                     // Pole
            << std::string(size + 1, SPACE_CHAR);                                // Space to the right of the pole
    }
    else if (thisLayerWidth == 0 && belowLayerWidth != 0) {
        // Cap layer
        out << std::string(size - belowLayerWidth + 1, SPACE_CHAR)               // Space to the left of the cap
            << std::string(belowLayerWidth, H_LINE_CHAR)                         // Left cap
            << POLE_CAP_STR                                                 // The pole connecting to the other bits
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

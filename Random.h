//
// Created by Christina Nguyen on 12/1/22.
//

#pragma once
#include <random>
class Random {
    static std::mt19937 random;

public:
    static int Int(int min, int max);
    static float Float(float min, float max);
};
#ifndef PROJECT3MINESWEEPER_RANDOM_H
#define PROJECT3MINESWEEPER_RANDOM_H

#endif //PROJECT3MINESWEEPER_RANDOM_H

//
// Created by Pablos on 23.01.2026.
//

#ifndef SNEK_H
#define SNEK_H

struct MapSize {
    uint32_t width;
    uint32_t height;
};

enum SnekDirection {
    SNEK_LEFT = 1,
    SNEK_RIGHT,
    SNEK_UP,
    SNEK_DOWN,
};

struct SnekPiece {
    double x, y;
    enum SnekDirection direction;
};



#endif
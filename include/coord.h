#ifndef COORD_H
#define COORD_H

#include <iostream>
using namespace std;

/**
 * Standard class for a 2D coordinate.
 *
 * @author José Tomás Tocino García <theom3ga@gmail.com>
 *
 */
struct coord{
    /// Horizontal position
    int x;

    /// Vertical position
    int y;

    /// Default constructor
    coord(int x = -1, int y = -1) : x(x), y(y) { }

    bool operator ==(const coord & c) const{
        return (c.x == x && c.y == y);
    }

    friend ostream& operator << (ostream& out, coord & C){
        out << "(" << C.x << "," << C.y << ")";

        return out;
    }
};

#endif

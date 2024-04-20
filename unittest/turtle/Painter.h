/**
 * @file Painter.h
 * @brief Painter class
 * @author [Jason Bao](https://github.com/jbaoys)
 */

#pragma once

#include "Turtle.h"

class Painter {
   public:
    Painter(Turtle& turtle) : turtle_(turtle) {}

    /**
     * Draw a circle
     * @param x center x coordinate
     * @param y center y coordinate
     * @param radius radius of the circle
     * @return true if the circle is drawn successfully
     * @return false if the circle is not drawn
     */
    bool DrawCircle(int x, int y, int radius) {
        turtle_.PenUp();
        turtle_.GoTo(x, y);
        turtle_.PenDown();
        turtle_.Circle(radius);
        return true;
    }
   private:
    Turtle& turtle_; //< turtle
};


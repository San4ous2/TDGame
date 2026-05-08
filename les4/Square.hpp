#include <iostream>
#include "Shape.hpp"

class Square : public Shape{
public:
    double h;
    double w;
    Square();
    void area();
    void Resize();
    void draw();
};

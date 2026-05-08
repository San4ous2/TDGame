#include <iostream>
#include "Shape.hpp"

class Circle : public Shape{
public:
    double r;
    Circle();
    void area();
    void Resize();
    void draw();
};

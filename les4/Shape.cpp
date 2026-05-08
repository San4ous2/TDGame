#include "Shape.hpp"
#include <iostream>
#include "AllegroUtil.hpp"

using namespace std;

class Shape{
public:
    double x;
    double y;
    void Move();
    virtual void area();
    virtual void Resize();
    virtual void draw();

    ~Shape() = default;
};

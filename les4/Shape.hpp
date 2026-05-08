#include <iostream>
#include "IDrawable.hpp"
#include "IResizable.hpp"

class Shape : public IDrawable, public IResizable {
public:
    double x;
    double y;

    void Move();
    virtual void area();

    ~Shape() = default;
};

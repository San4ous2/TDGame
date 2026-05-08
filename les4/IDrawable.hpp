#include <iostream>

class IDrawable{
public:
    virtual void draw();
    ~IDrawable() = default;
};

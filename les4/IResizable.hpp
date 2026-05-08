#include <iostream>

class IResizable{
public:
    virtual void Resize();
    ~IResizable() = default;
};

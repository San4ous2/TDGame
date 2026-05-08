#include "Circle.hpp"
#include <iostream>
#include "AllegroUtil.hpp"

using namespace std;
class Circle{
public:
    double r;
    Circle(int v){
         r = v;
    }
    void area(){
        cout<<r*r*3.14
    };
    void Resize();
    void draw();

};

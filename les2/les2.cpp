#include <iostream>
#include <cmath>
using namespace std;

class Figure{
    protected:
        double x;
        double y;
    void Move(){}

};
class Triangle: public Figure{
    protected:
        double a;
        double b;
        double c;
    public:
        Triangle(double val , double val2, double val3){
            a=val;
            b=val2;
            c=val3;
        }
        double Area(){
            double p = (a+b+c)/2;
            return pow(p*(p-a)*(p-b)*(p-c),1/2);
            }
};

class Rectangle{
protected:
    double a;
    double b;
    public:
        Rectangle(double val, double val2){
            a=val;
            b=val2;
        }
        double Area(){
            return a*b;
        }

};
class Square: public Rectangle{
    protected:
        double size;
    public:
        Square(double val){
            size = val;
        }
        double Area(){
            return pow(size,2);
        }
};

class Circle: public Figure{
    protected:
        double r;
    public:
        Circle(double val){
            r = val;
        }
        double Area(){
            return 2.0*3.14*pow(r,2);
        }
};

int main(){
Square p = Square(15.0) ;
cout << p.Area();

}


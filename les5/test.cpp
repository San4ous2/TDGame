#include <iostream>

using namespace std;

enum RGB{
    RED,
    GREEN,
    BLUE,
    WHITE = 12,
    WALTER,
};


class Lohoped_single{
private:
    static Lohoped_single* inst;
    Lohoped_single(){}

public:
    static Lohoped_single* get_inst(){
        if (inst == nullptr){
            inst = new Lohoped_single();
        }
        return inst;
    }

};

Lohoped_single* Lohoped_single::inst = nullptr;

int main(){
Lohoped_single* loh = Lohoped_single::get_inst();
cout << loh;
}


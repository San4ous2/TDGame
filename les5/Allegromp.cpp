#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "engine.hpp"
#include <unistd.h>

using namespace std;

void draw3()
{
    al_draw_filled_triangle(rand()%Engine::WIDTH, rand()%Engine::HEIGHT, rand()%Engine::WIDTH, rand()%Engine::HEIGHT, rand()%Engine::WIDTH, rand()%Engine::HEIGHT, al_map_rgb(rand()% 255,rand()% 255, rand()% 255));
}

void square(){
    double x = rand()%(Engine::WIDTH-100);
    double y = rand()%(Engine::HEIGHT-100);
    double n = rand()% 100+50;
    al_draw_filled_rectangle(x,y,x+n,y+n,al_map_rgb(rand()% 255 +100,rand()% 255+100, rand()% 255+100));
}

void fps (){
    al_clear_to_color(al_map_rgb(0,0,0));
    sleep(1);
}

class Square{
private:
    double x;
    double  y;
    ALLEGRO_COLOR color;
    friend class Fabric;

public:
    static Square* fps_upd(){
    al_clear_to_color(al_map_rgb(0,0,0))
    x+=5;
    y+=5;
    }
    static Square* draw_upd(){
    al_draw_filled_rectangle(x,y,x+50,y+50,color)

    }
}
class Fabric{
    public:
        void make_new_rec(){

        }



};
int main(int argc, char **argv)
{

    srand(time(0));
    Engine::start(fps,square);
}

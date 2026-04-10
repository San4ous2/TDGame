#include "engine.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <vector>
#include <allegro5/allegro.h>
#include <unistd.h>

using namespace std;

class Player{
public:
    double x = 320;
    double y = 360-70;
    double height = 70;
    double width = 50;
    double speed = 5;
    double jumpspeed = -20;
    double grav = 5;
    void gravity(){
        y +=grav;

    }
    void moving(){
        ALLEGRO_KEYBOARD_STATE k_state;
        al_get_keyboard_state(&k_state);
        if (al_key_down(&k_state, ALLEGRO_KEY_LEFT)) {
            x -= speed;
        }
        if (al_key_down(&k_state, ALLEGRO_KEY_RIGHT)) {
            x += speed;
        }

    }

    void physics_process(){
        ALLEGRO_KEYBOARD_STATE k_state;
        al_get_keyboard_state(&k_state);
        cout<< "x:" << x + width << " y:" << y+height << endl;
        if(x+width >= 640){
            x = 590;
        }
        if(x <= 0){
            x = 0;
        }
        if(y+height >= 360){
            y = 290-grav;
        }
        if(y <= 0){
            y = 0;
        }
        moving();

                if (al_key_down(&k_state, ALLEGRO_KEY_SPACE)){
                        y+=jumpspeed;
                }


        gravity();

    }

    void render_process(){
        al_draw_filled_rectangle(x,y,x+width,y+height,al_map_rgb(255,255,255));
    }

};


class Screen : public Engine {
public:
    Player player;

    void physics_process() override {
        player.physics_process();
    }

    void render_process() override {
        player.render_process();
    }
};

int main() {
    Screen screen;
    screen.start();
}

#include "tower_defense.hpp"


int main() {
    TowerDefense game;
    game.start();

}

/*void Menu(){

        ALLEGRO_TRANSFORM tr;

        al_draw_filled_rectangle(0,0,640,360,al_map_rgb(189, 107, 0));
        al_draw_rectangle(3,3,125,69,al_map_rgb(94, 53, 0),3);
        if(!is_playing){
        al_identity_transform(&tr);
        al_scale_transform(&tr, 5,5);
        al_use_transform(&tr);
        }
        al_draw_text(font, al_map_rgb(255,255,255),65,10, ALLEGRO_ALIGN_CENTER, "TD++");
        al_draw_filled_rounded_rectangle(50, 30, 80, 40, 4, 4, al_map_rgb(0,255,0));
        al_draw_filled_rounded_rectangle(50, 50, 80, 60, 4, 4, al_map_rgb(255,0,0));

        ALLEGRO_MOUSE_STATE ms;
        al_get_mouse_state(&ms);
        my = al_get_mouse_state_axis(&ms,1);
        mx = al_get_mouse_state_axis(&ms,0);
        static bool prevL = false, prevR = false;
        bool curL = ms.buttons & 1;
        bool curR = ms.buttons & 2;
        cout<< "x"<<mx <<","<< "y"<< my<< endl;

        if(my>150 && my< 200){
            if(mx>250 && mx<400){
                al_draw_rounded_rectangle(50,30,80,40,4,4,al_map_rgb(255,255,255),2);
                if(curL && !prevL){
                    al_identity_transform(&tr);
                    al_scale_transform(&tr, 1,1);
                    al_use_transform(&tr);
                    is_playing = true;
                }
            }
        }
        if(my > 250 && my<300){
            if(mx>250 && mx<400){
                al_draw_rounded_rectangle(50,50,80,60,4,4,al_map_rgb(255,255,255),2);
                if(curL && !prevL){
                    Engine::stop();
                }
            }
        }

        if (curL && !prevL){
            al_draw_rectangle(121,93,221,123,al_map_rgb(255,255,255),4);
            cout<<"left";
        }
        if (curR && !prevR) {
            cout<< "right";
        }
        prevL = curL; prevR = curR;


    }
*/

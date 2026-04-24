#include "engine.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <ctime>
#include <thread>
#include <random>
#include <limits>
using namespace std;

ALLEGRO_BITMAP* dirt = nullptr;
ALLEGRO_BITMAP* grass = nullptr;
ALLEGRO_BITMAP* vrag_img = nullptr;
ALLEGRO_BITMAP* wizard_img = nullptr;
ALLEGRO_BITMAP* archer_img = nullptr;
ALLEGRO_BITMAP* farm_img = nullptr;
ALLEGRO_BITMAP* summoner_img = nullptr;
ALLEGRO_BITMAP* cat_img = nullptr;
ALLEGRO_BITMAP* menu = nullptr;
ALLEGRO_FONT* font = nullptr;
bool is_playing = false;
static const int CELL = 32;
static const int GCOLS = 20;
static const int GROWS = 8;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> dist(1,100);

static const int MAP1[GROWS][GCOLS] = {
    {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

static const int MAP2[GROWS][GCOLS] = {
    {1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

static const int MAP3[GROWS][GCOLS] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};


static const vector<pair<int,int>> WAYPOINTS1 = {
    {0,0},{1,0},{2,0},{3,0},
    {3,1},
    {3,2},{4,2},{5,2},{6,2},{7,2},{8,2},
    {8,3},
    {8,4},{9,4},{10,4},{11,4},{12,4},
    {12,5},
    {12,6},{13,6},{14,6},{15,6},{16,6},{17,6},{18,6},{19,6}
};

static const vector<pair<int,int>> WAYPOINTS2 = {
    {0,0},{1,0},{2,0},{3,0},{4,0},
    {4,1},
    {4,2},{3,2},{2,2},{1,2},
    {1,3},
    {1,4},{2,4},{3,4},{4,4},{5,4},{6,4},{7,4},{8,4},{9,4},
    {9,5},
    {9,6},{10,6},{11,6},{12,6},{13,6},{14,6},{15,6},{16,6},{17,6},{18,6},{19,6}
};

static const vector<pair<int,int>> WAYPOINTS3 = {
    {0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},
    {10,0},{11,0},{12,0},{13,0},{14,0},{15,0},{16,0},{17,0},{18,0},
    {18,1},
    {18,2},{17,2},{16,2},{15,2},{14,2},{13,2},{12,2},{11,2},{10,2},{9,2},{8,2},{7,2},{6,2},
    {6,3},
    {6,4},{7,4},{8,4},{9,4},{10,4},{11,4},{12,4},{13,4},{14,4},{15,4},{16,4},
    {16,5},
    {16,6},{17,6},{18,6},{19,6}
};

const int (*CURRENT_MAP)[GCOLS] = MAP1;
const vector<pair<int,int>>* CURRENT_WAYPOINTS = &WAYPOINTS1;


static float cell_cx(int c) { return c * CELL + CELL * 0.5f; }
static float cell_cy(int r) { return r * CELL + CELL * 0.5f; }
static float dist2(float ax, float ay, float bx, float by) {
    return (ax-bx)*(ax-bx) + (ay-by)*(ay-by);
}


struct Vrag {
    float x, y;
    float skorost;
    int tochkaIdx;
    bool zhiv;
    int hp;
    int maxHp;

    Vrag(int lvl = 1) {
        x = -32;
        y = cell_cy(0);
        tochkaIdx = 0;
        zhiv = true;

        if (lvl == 1) {
            skorost = 1.5; hp = 5; maxHp = 5;
        } else if (lvl == 2) {
            skorost = 2.0; hp = 8; maxHp = 8;
        } else {
            skorost = 2.5; hp = 12; maxHp = 12;
        }
    }

    void update() {
        if (!zhiv) return;
        if (tochkaIdx >= (int)CURRENT_WAYPOINTS->size()) { zhiv = false; return; }

        float tx = cell_cx((*CURRENT_WAYPOINTS)[tochkaIdx].first);
        float ty = cell_cy((*CURRENT_WAYPOINTS)[tochkaIdx].second);
        float dx = tx - x, dy = ty - y;
        float d = sqrt(dx*dx + dy*dy);
        if (d < skorost) { x = tx; y = ty; tochkaIdx++; }
        else { x += (dx/d)*skorost; y += (dy/d)*skorost; }
    }

    void draw() {
        if (!zhiv) return;
        al_draw_bitmap(vrag_img, x-16, y-16, 0);
        float ratio = (float)hp / maxHp;
        al_draw_filled_rectangle(x-14, y-20, x-14+28*ratio, y-16, al_map_rgb(255,80,80));
        al_draw_rectangle(x-14, y-20, x+14, y-16, al_map_rgb(200,200,200), 1);
    }
};


struct Shar {
    float x, y, tx, ty;
    float skorost;
    bool zhiv;
    int uron;
    float radius;

    Shar(float sx, float sy, float ex, float ey, int u, float r) {
        x = sx; y = sy; tx = ex; ty = ey;
        skorost = 4.0; zhiv = true; uron = u; radius = r;
    }

    void update(vector<Vrag>& vragi) {
        if (!zhiv) return;
        float dx = tx-x, dy = ty-y;
        float d = sqrt(dx*dx+dy*dy);
        if (d < skorost) {
            for (auto& v : vragi) {
                if (!v.zhiv) continue;
                if (dist2(v.x, v.y, tx, ty) < radius*radius) {
                    v.hp -= uron;
                    if (v.hp <= 0) v.zhiv = false;
                }
            }
            zhiv = false;
        } else {
            x += (dx/d)*skorost;
            y += (dy/d)*skorost;
        }
    }

    void draw() {
        if (!zhiv) return;
        al_draw_filled_circle(x, y, 6, al_map_rgb(100,100,255));
        al_draw_filled_circle(x+2, y-2, 2, al_map_rgb(200,200,255));
    }
};


struct Strela {
    float x, y, tx, ty;
    float skorost;
    bool zhiv;
    int uron;
    Vrag* cel;

    Strela(float sx, float sy, Vrag* v, int u) {
        x = sx; y = sy; cel = v;
        tx = v->x; ty = v->y;
        skorost = 6.0; zhiv = true; uron = u;
    }

    void update() {
        if (!zhiv) return;
        if (cel && cel->zhiv) { tx = cel->x; ty = cel->y; }
        float dx = tx-x, dy = ty-y;
        float d = sqrt(dx*dx+dy*dy);
        if (d < skorost) {
            if (cel && cel->zhiv) {
                cel->hp -= uron;
                if (cel->hp <= 0) cel->zhiv = false;
            }
            zhiv = false;
        } else {
            x += (dx/d)*skorost;
            y += (dy/d)*skorost;
        }
    }

    void draw() {
        if (!zhiv) return;
        float dx = tx-x, dy = ty-y;
        float d = sqrt(dx*dx+dy*dy)+0.001;
        float nx = dx/d, ny = dy/d;
        al_draw_line(x, y, x+nx*10, y+ny*10, al_map_rgb(200,180,100), 2);
        al_draw_filled_circle(x, y, 2, al_map_rgb(255,200,50));
    }
};


struct Kot {
    float x, y;
    float skorost;
    int tochkaIdx;
    bool zhiv;
    int hp;
    float atacTimer;

    Kot(int startWp) {
        tochkaIdx = startWp;
        x = cell_cx((*CURRENT_WAYPOINTS)[startWp].first);
        y = cell_cy((*CURRENT_WAYPOINTS)[startWp].second);
        skorost = 1.0; zhiv = true; hp = 8; atacTimer = 0;
    }

    void update(vector<Vrag>& vragi) {
        if (!zhiv) return;
        atacTimer -= 1.0/60.0;

        Vrag* cel = nullptr;
        float minD = 60*60;
        for (auto& v : vragi) {
            if (!v.zhiv) continue;
            float d = dist2(x, y, v.x, v.y);
            if (d < minD) { minD = d; cel = &v; }
        }

        if (cel && atacTimer <= 0) {
            cel->hp -= 1;
            if (cel->hp <= 0) cel->zhiv = false;
            atacTimer = 1.0;
        }

        if (!cel) {
            if (tochkaIdx > 0) {
                float tx = cell_cx((*CURRENT_WAYPOINTS)[tochkaIdx].first);
                float ty = cell_cy((*CURRENT_WAYPOINTS)[tochkaIdx].second);
                float dx = tx-x, dy = ty-y;
                float d = sqrt(dx*dx+dy*dy);
                if (d < skorost) { x = tx; y = ty; tochkaIdx--; }
                else { x += (dx/d)*skorost; y += (dy/d)*skorost; }
            } else {
                zhiv = false;
            }
        }
    }

    void draw() {
        if (!zhiv) return;
        al_draw_bitmap(cat_img, x-16, y-16, 0);
    }
};


enum TowerType { NONE=-1, WIZARD, ARCHER, FARM, SUMMONER };

struct Tower {
    int col, row;
    TowerType type;
    float atacTimer;
    float farmTimer;
    float summonTimer;

    Tower(int c, int r, TowerType t) : col(c), row(r), type(t) {
        atacTimer = 0; farmTimer = 0; summonTimer = 0;
    }

    float cx() { return col*CELL + CELL*0.5; }
    float cy() { return row*CELL + CELL*0.5; }

    Vrag* blizhaishiy(vector<Vrag>& vragi, float rad) {
        Vrag* res = nullptr;
        float minD = rad*rad;
        for (auto& v : vragi) {
            if (!v.zhiv) continue;
            float d = dist2(cx(), cy(), v.x, v.y);
            if (d < minD) { minD = d; res = &v; }
        }
        return res;
    }

    void update(vector<Vrag>& vragi, vector<Shar>& shary,
                vector<Strela>& streli, vector<Kot>& koty,
                int& dengi) {
        atacTimer  -= 1.0/60.0;
        farmTimer  -= 1.0/60.0;
        summonTimer -= 1.0/60.0;

        if (type == WIZARD) {
            if (atacTimer <= 0) {
                Vrag* v = blizhaishiy(vragi, 100);
                if (v) {
                    shary.emplace_back(cx(), cy(), v->x, v->y, 2, 40.0);
                    atacTimer = 1.5;
                }
            }
        } else if (type == ARCHER) {
            if (atacTimer <= 0) {
                Vrag* v = blizhaishiy(vragi, 120);
                if (v) {
                    streli.emplace_back(cx(), cy(), v, 3);
                    atacTimer = 0.8;
                }
            }
        } else if (type == FARM) {
            if (farmTimer <= 0) {
                if(dist(gen) == 67){
                    dengi +=67;
                }
                dengi += 6;
                farmTimer = 5.0;
            }
        } else if (type == SUMMONER) {
            if (summonTimer <= 0) {
                int lastWp = (int)CURRENT_WAYPOINTS->size() - 1;
                koty.emplace_back(lastWp);
                summonTimer = 8.0;
            }
        }
    }

    void draw() {
        ALLEGRO_BITMAP* img = nullptr;
        if (type == WIZARD)   img = wizard_img;
        else if (type == ARCHER)   img = archer_img;
        else if (type == FARM)     img = farm_img;
        else if (type == SUMMONER) img = summoner_img;
        if (img) al_draw_bitmap(img, col*CELL, row*CELL, 0);
    }
};


class TowerDefense : public Engine {
private:
    vector<Vrag>  vragi;
    vector<Tower> bashni;
    vector<Shar>  shary;
    vector<Strela> streli;
    vector<Kot>   koty;

    float taymerSpavna  = 2.0;
    bool  assets_loaded = false;

    TowerType vybrannyType = NONE;
    int dengi  = 50;
    int zhizni = 10;

    int uroven           = 1;
    int vragov_spavneno  = 0;
    float tekst_taymer   = 3.0;
    bool pobeda          = false;
    bool proigrysh       = false;

    const int   maxVragov[3]     = {10, 15, 20};
    const float intervalSpavna[3]= {2.0, 1.7, 1.4};

    const int  STOIMOST[4]  = {30, 20, 25, 40};
    const char* NAZVANIYA[4] = {"Wizzard", "Archer", "Farm", "Summoner"};

    bool isTowerAt(int c, int r) {
        for (auto& t : bashni)
            if (t.col == c && t.row == r) return true;
        return false;
    }

    void sleduyushiy_uroven() {
        uroven++;
        vragov_spavneno = 0;
        taymerSpavna    = 4.0;
        tekst_taymer    = 3.0;

        bashni.clear();
        shary.clear();
        streli.clear();
        koty.clear();
        vragi.clear();

        dengi += 50;

        if (uroven == 2) {
            CURRENT_MAP       = MAP2;
            CURRENT_WAYPOINTS = &WAYPOINTS2;
        } else if (uroven == 3) {
            CURRENT_MAP       = MAP3;
            CURRENT_WAYPOINTS = &WAYPOINTS3;
        }
    }

public:
    TowerDefense() {
        bg_color    = al_map_rgb(15, 20, 15);
        full_redraw = true;
    }

    void load_assets() {
        if (assets_loaded) return;
        vrag_img    = al_load_bitmap("Game/sprites/Vrag.bmp");
        dirt        = al_load_bitmap("Game/sprites/Dirt.bmp");
        grass       = al_load_bitmap("Game/sprites/Grass.bmp");
        wizard_img  = al_load_bitmap("Game/sprites/Wizard.bmp");
        archer_img  = al_load_bitmap("Game/sprites/Archer.bmp");
        farm_img    = al_load_bitmap("Game/sprites/Farm.bmp");
        summoner_img= al_load_bitmap("Game/sprites/Summoner.bmp");
        cat_img     = al_load_bitmap("Game/sprites/Summon.bmp");
        menu     = al_load_bitmap("Game/Menu.bmp");
        font        = al_create_builtin_font();
        assets_loaded = true;
    }

    void handle_click(int mx, int my, int btn) {
        if (pobeda || proigrysh) return;

        if (btn == 1) {
            for (int i = 0; i < 4; i++) {
                int bx = 10 + i*110;
                if (mx >= bx && mx <= bx+100 && my >= 262 && my <= 298) {
                    vybrannyType = (TowerType)i;
                    return;
                }
            }

            int c = mx / CELL;
            int r = my / CELL;
            if (r < GROWS && c < GCOLS && CURRENT_MAP[r][c] == 0
                && vybrannyType != NONE && !isTowerAt(c, r)) {
                int cost = STOIMOST[(int)vybrannyType];
                if (dengi >= cost) {
                    bashni.emplace_back(c, r, vybrannyType);
                    dengi -= cost;
                }
            }
        }

        if (btn == 2) {
            vybrannyType = NONE;
        }
    }

    void physics_process() override {
        ALLEGRO_MOUSE_STATE ms;
        al_get_mouse_state(&ms);
        static bool prevL = false, prevR = false;
        bool curL = ms.buttons & 1;
        bool curR = ms.buttons & 2;
        if (curL && !prevL) handle_click(ms.x, ms.y, 1);
        if (curR && !prevR) handle_click(ms.x, ms.y, 2);
        prevL = curL; prevR = curR;

        if (pobeda || proigrysh) return;

        if (tekst_taymer > 0) tekst_taymer -= 1.0 / Engine::FPS;

        taymerSpavna -= 1.0 / Engine::FPS;
        if (taymerSpavna <= 0 && vragov_spavneno < maxVragov[uroven-1]) {
            vragi.emplace_back(uroven);
            vragov_spavneno++;
            taymerSpavna = intervalSpavna[uroven-1];
        }

        for (auto& v : vragi) v.update();
        for (auto& t : bashni) t.update(vragi, shary, streli, koty, dengi);
        for (auto& s : shary)  s.update(vragi);
        for (auto& a : streli) a.update();
        for (auto& k : koty)   k.update(vragi);

        for (int i = (int)vragi.size()-1; i >= 0; i--) {
            if (!vragi[i].zhiv) {
                if (vragi[i].tochkaIdx >= (int)CURRENT_WAYPOINTS->size())
                    zhizni--;
                else
                    dengi += 3;
                vragi.erase(vragi.begin()+i);
            }
        }

        shary.erase(remove_if(shary.begin(), shary.end(),
            [](Shar& s)  { return !s.zhiv; }), shary.end());
        streli.erase(remove_if(streli.begin(), streli.end(),
            [](Strela& a){ return !a.zhiv; }), streli.end());
        koty.erase(remove_if(koty.begin(), koty.end(),
            [](Kot& k)   { return !k.zhiv; }), koty.end());

        bool vsehSpavnili = vragov_spavneno >= maxVragov[uroven-1];
        if (vsehSpavnili && vragi.empty()) {
            if (uroven < 3) {
                sleduyushiy_uroven();
            } else {
                pobeda = true;
            }
        }

        if (zhizni <= 0) {
            proigrysh = true;
        }
    }
    void MGame(){
        for (int r = 0; r < GROWS; r++)
            for (int c = 0; c < GCOLS; c++)
                if (CURRENT_MAP[r][c] == 1)
                    al_draw_bitmap(dirt, c*CELL, r*CELL, 0);
                else
                    al_draw_bitmap(grass, c*CELL, r*CELL, 0);

        for (auto& t : bashni)  t.draw();
        for (auto& s : shary)   s.draw();
        for (auto& a : streli)  a.draw();
        for (auto& k : koty)    k.draw();
        for (auto& v : vragi)   v.draw();

        al_draw_filled_rectangle(0, 256, 640, 360, al_map_rgb(30,20,10));
        al_draw_rectangle(0, 256, 640, 360, al_map_rgb(100,70,30), 2);

        ALLEGRO_COLOR colors[4] = {
            al_map_rgb(80,80,200),
            al_map_rgb(100,180,80),
            al_map_rgb(220,180,50),
            al_map_rgb(180,80,200)
        };

        for (int i = 0; i < 4; i++) {
            int bx = 10 + i*110;
            bool vybran = (vybrannyType == (TowerType)i);
            al_draw_filled_rounded_rectangle(bx, 262, bx+100, 298, 4, 4, colors[i]);
            if (vybran)
                al_draw_rounded_rectangle(bx, 262, bx+100, 298, 4, 4,
                    al_map_rgb(255,255,255), 3);

            ALLEGRO_BITMAP* img = nullptr;
            if (i==0) img = wizard_img;
            else if (i==1) img = archer_img;
            else if (i==2) img = farm_img;
            else img = summoner_img;
            if (img) al_draw_scaled_bitmap(img, 0,0,32,32, bx+4,265,28,28, 0);

            if (font) {
                al_draw_text(font, al_map_rgb(255,255,255), bx+35, 265, 0, NAZVANIYA[i]);
                char buf[16]; sprintf(buf, "%d$", STOIMOST[i]);
                al_draw_text(font, al_map_rgb(255,230,100), bx+35, 278, 0, buf);
            }
        }

        if (font) {
            char buf[64];

            sprintf(buf, "Money: %d$", dengi);
            al_draw_text(font, al_map_rgb(255,230,100), 10, 308, 0, buf);

            sprintf(buf, "Lives: %d", zhizni);
            al_draw_text(font, al_map_rgb(255,100,100), 10, 325, 0, buf);

            int total = maxVragov[uroven-1];
            int ubito  = vragov_spavneno - (int)vragi.size();
            if (ubito < 0) ubito = 0;
            sprintf(buf, "Level: %d/3   Enemies: %d/%d", uroven, ubito, total);
            al_draw_text(font, al_map_rgb(180,220,180), 200, 308, 0, buf);

            al_draw_text(font, al_map_rgb(150,150,150), 200, 325, 0,
                "LKM = Place, PKM = Cancel");
        }

        if (tekst_taymer > 0 && font) {
            al_draw_filled_rounded_rectangle(180, 95, 460, 165, 6, 6,
                al_map_rgb(10,10,10));
            al_draw_rounded_rectangle(180, 95, 460, 165, 6, 6,
                al_map_rgb(255,200,50), 3);

            char buf[32];
            sprintf(buf, "Level %d", uroven);
            al_draw_text(font, al_map_rgb(255,200,50),
                320, 115, ALLEGRO_ALIGN_CENTER, buf);

            const char* podpisi[3] = {
                "10 enemies",
                "15 faster enemies  +50$",
                "20 strong enemies  +50$"
            };
            al_draw_text(font, al_map_rgb(200,200,200),
                320, 135, ALLEGRO_ALIGN_CENTER, podpisi[uroven-1]);
        }

        if (proigrysh) {
            al_draw_filled_rectangle(130, 90, 510, 185, al_map_rgb(0,0,0));
            al_draw_rectangle(130, 90, 510, 185, al_map_rgb(255,50,50), 3);
            if (font) {
                al_draw_text(font, al_map_rgb(255,50,50),
                    320, 115, ALLEGRO_ALIGN_CENTER, "Game Over");
                char buf[32];
                sprintf(buf, "Level reached: %d / 3", uroven);
                al_draw_text(font, al_map_rgb(200,200,200),
                    320, 145, ALLEGRO_ALIGN_CENTER, buf);
            }
            al_draw_filled_rectangle(0, 0, 640, 360, al_map_rgb(0,0,0));
            this_thread::sleep_for(chrono::seconds(5));
            Engine::stop();
        }

        if (pobeda) {
            al_draw_filled_rectangle(130, 90, 510, 185, al_map_rgb(0,0,0));
            al_draw_rectangle(130, 90, 510, 185, al_map_rgb(50,255,100), 3);
            if (font){
                al_draw_text(font, al_map_rgb(50,255,100),
                    320, 110, ALLEGRO_ALIGN_CENTER, "You Win!");
                al_draw_text(font, al_map_rgb(200,200,200),
                    320, 135, ALLEGRO_ALIGN_CENTER, "All 3 levels cleared!");
                char buf[32];
                sprintf(buf, "Money left: %d$", dengi);
                al_draw_text(font, al_map_rgb(255,230,100),
                    320, 155, ALLEGRO_ALIGN_CENTER, buf);
            }
            al_draw_filled_rectangle(0, 0, 640, 360, al_map_rgb(0,0,0));
            this_thread::sleep_for(chrono::seconds(5));
            Engine::stop();
        }


    }
    void Menu(){
        al_draw_bitmap(menu,0, 0, 0);

        /*ALLEGRO_TRANSFORM tr;
        al_draw_filled_rectangle(0,0,640,360,al_map_rgb(189, 107, 0));
        al_draw_rectangle(3,3,125,69,al_map_rgb(94, 53, 0),3);
        al_identity_transform(&tr);

        al_scale_transform(&tr, 5,5);
        al_use_transform(&tr);
        al_draw_text(font, al_map_rgb(255,255,255),65,10, ALLEGRO_ALIGN_CENTER, "TD++");
        al_draw_filled_rounded_rectangle(50, 30, 80, 40, 4, 4, al_map_rgb(0,0,0));*/

    }
    void render_process() override {
        load_assets();
        Menu();
        if(is_playing){
            MGame();
        }


    }
};

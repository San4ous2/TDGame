#include "engine.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;

ALLEGRO_BITMAP* dirt = nullptr;
ALLEGRO_BITMAP* grass = nullptr;
ALLEGRO_BITMAP* vrag_img = nullptr;
ALLEGRO_BITMAP* wizard_img = nullptr;
ALLEGRO_BITMAP* archer_img = nullptr;
ALLEGRO_BITMAP* farm_img = nullptr;
ALLEGRO_BITMAP* summoner_img = nullptr;
ALLEGRO_BITMAP* cat_img = nullptr;
ALLEGRO_FONT* font = nullptr;

static const int CELL = 32;
static const int GCOLS = 20;
static const int GROWS = 8;

static const int Level1[GROWS][GCOLS] = {
    {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

static const int Level2[GROWS][GCOLS] = {
    {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

static const int Level3[GROWS][GCOLS] = {
    {1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};


static const std::vector<std::pair<int,int>> WAYPOINTS = {
    {0,0},{1,0},{2,0},{3,0},
    {3,1},
    {3,2},{4,2},{5,2},{6,2},{7,2},{8,2},
    {8,3},
    {8,4},{9,4},{10,4},{11,4},{12,4},
    {12,5},
    {12,6},{13,6},{14,6},{15,6},{16,6},{17,6},{18,6},{19,6}
};

static float cell_cx(int c) { return c * CELL + CELL * 0.5; }
static float cell_cy(int r) { return r * CELL + CELL * 0.5; }
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

    Vrag() {
        x = -32; y = cell_cy(0);
        skorost = 1.5;
        tochkaIdx = 0;
        zhiv = true;
        hp = 5; maxHp = 5;
    }

    void update() {
        if (!zhiv) return;
        if (tochkaIdx >= (int)WAYPOINTS.size()) { zhiv = false; return; }
        float tx = cell_cx(WAYPOINTS[tochkaIdx].first);
        float ty = cell_cy(WAYPOINTS[tochkaIdx].second);
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

    void update(std::vector<Vrag>& vragi) {
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
        al_draw_line(x, y, x+nx*10, y+ny*10, al_map_rgb(200,180,100),2);
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
        x = cell_cx(WAYPOINTS[startWp].first);
        y = cell_cy(WAYPOINTS[startWp].second);
        skorost = 1.0; zhiv = true; hp = 8; atacTimer = 0;
    }

    void update(std::vector<Vrag>& vragi) {
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
            if (tochkaIdx >= 0) {
                float tx = cell_cx(WAYPOINTS[tochkaIdx].first);
                float ty = cell_cy(WAYPOINTS[tochkaIdx].second);
                float dx = tx-x, dy = ty-y;
                float d = sqrt(dx*dx+dy*dy);
                if (d < skorost) { x = tx; y = ty; tochkaIdx--; }
                else { x += (dx/d)*skorost; y += (dy/d)*skorost; }
            } else { zhiv = false; }
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

    float cx() { return col*CELL + CELL*0.5f; }
    float cy() { return row*CELL + CELL*0.5f; }

    Vrag* blizhaishiy(std::vector<Vrag>& vragi, float rad) {
        Vrag* res = nullptr;
        float minD = rad*rad;
        for (auto& v : vragi) {
            if (!v.zhiv) continue;
            float d = dist2(cx(), cy(), v.x, v.y);
            if (d < minD) { minD = d; res = &v; }
        }
        return res;
    }

    void update(std::vector<Vrag>& vragi, std::vector<Shar>& shary,
                std::vector<Strela>& streli, std::vector<Kot>& koty,
                int& dengi) {
        atacTimer -= 1.0/60.0;
        farmTimer -= 1.0/60.0;
        summonTimer -= 1.0/60.0;

        if (type == WIZARD) {
            if (atacTimer <= 0) {
                Vrag* v = blizhaishiy(vragi, 100);
                if (v) {
                    shary.emplace_back(cx(), cy(), v->x, v->y, 2, 40.0f);
                    atacTimer = 1.5;
                }
            }
        }
        else if (type == ARCHER) {
            if (atacTimer <= 0) {
                Vrag* v = blizhaishiy(vragi, 120);
                if (v) {
                    streli.emplace_back(cx(), cy(), v, 1);
                    atacTimer = 0.8;
                }
            }
        }
        else if (type == FARM) {
            if (farmTimer <= 0) {
                dengi += 5;
                farmTimer = 5.0;
            }
        }
        else if (type == SUMMONER) {
            if (summonTimer <= 0) {
                int wp = (int)WAYPOINTS.size()-1;
                koty.emplace_back(wp);
                summonTimer = 8.0;
            }
        }
    }

    void draw() {
        ALLEGRO_BITMAP* img = nullptr;
        if (type == WIZARD) img = wizard_img;
        else if (type == ARCHER) img = archer_img;
        else if (type == FARM) img = farm_img;
        else if (type == SUMMONER) img = summoner_img;
        if (img) al_draw_bitmap(img, col*CELL, row*CELL, 0);
    }
};

class TowerDefense : public Engine {
private:
    std::vector<Vrag> vragi;
    std::vector<Tower> bashni;
    std::vector<Shar> shary;
    std::vector<Strela> streli;
    std::vector<Kot> koty;
    float taymerSpavna = 0;
    bool assets_loaded = false;
    TowerType vybrannyType = NONE;
    int dengi = 50;
    int zhizni = 10;

    const int STOIMOST[4] = {30, 20, 25, 40};
    const char* NAZVANIYA[4] = {"Wizzard", "Archer", "Farm", "Summoner"};

    bool isTowerAt(int c, int r) {
        for (auto& t : bashni)
            if (t.col == c && t.row == r) return true;
        return false;
    }

public:
    TowerDefense() {
        bg_color = al_map_rgb(15,20,15);
        full_redraw = true;
    }

    void load_assets() {
        if (assets_loaded) return;
        vrag_img = al_load_bitmap("Game/sprites/Vrag.bmp");
        dirt = al_load_bitmap("Game/sprites/Dirt.bmp");
        grass = al_load_bitmap("Game/sprites/Grass.bmp");
        wizard_img = al_load_bitmap("Game/sprites/Wizard.bmp");
        archer_img = al_load_bitmap("Game/sprites/Archer.bmp");
        farm_img = al_load_bitmap("Game/sprites/Farm.bmp");
        summoner_img = al_load_bitmap("Game/sprites/Summoner.bmp");
        cat_img = al_load_bitmap("Game/sprites/Summon.bmp");
        font = al_create_builtin_font();
        assets_loaded = true;
    }

    void handle_click(int mx, int my, int btn) {
        if (btn == 1) {
            for (int i = 0; i < 4; i++) {
                int bx = 10 + i*110;
                if (mx >= bx && mx <= bx+90 && my >= 262 && my <= 298) {
                    vybrannyType = (TowerType)i;
                    return;
                }
            }

            int c = mx / CELL;
            int r = my / CELL;
            if (r < GROWS && c < GCOLS && MAP[r][c] == 0 && vybrannyType != NONE && !isTowerAt(c,r)) {
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

        taymerSpavna -= 1.0/Engine::FPS;
        if (taymerSpavna <= 0) {
            vragi.emplace_back();
            taymerSpavna = 2.0;
        }

        for (auto& v : vragi) v.update();

        for (auto& t : bashni)
            t.update(vragi, shary, streli, koty, dengi);

        for (auto& s : shary) s.update(vragi);
        for (auto& a : streli) a.update();
        for (auto& k : koty) k.update(vragi);

        for (int i = (int)vragi.size()-1; i >= 0; i--)
            if (!vragi[i].zhiv) {
                if (vragi[i].tochkaIdx >= (int)WAYPOINTS.size()) zhizni--;
                else dengi += 3;
                vragi.erase(vragi.begin()+i);
            }

        shary.erase(remove_if(shary.begin(), shary.end(), [](Shar& s){ return !s.zhiv; }), shary.end());
        streli.erase(remove_if(streli.begin(), streli.end(), [](Strela& a){ return !a.zhiv; }), streli.end());
        koty.erase(remove_if(koty.begin(), koty.end(), [](Kot& k){ return !k.zhiv; }), koty.end());
    }

    void render_process() override {
        load_assets();

        for (int r = 0; r < GROWS; r++)
            for (int c = 0; c < GCOLS; c++)
                if (MAP[r][c] == 1) al_draw_bitmap(dirt, c*CELL, r*CELL, 0);
                else al_draw_bitmap(grass, c*CELL, r*CELL, 0);

        for (auto& t : bashni) t.draw();
        for (auto& s : shary) s.draw();
        for (auto& a : streli) a.draw();
        for (auto& k : koty) k.draw();
        for (auto& v : vragi) v.draw();

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
            if (vybran) al_draw_rounded_rectangle(bx, 262, bx+100, 298, 4, 4, al_map_rgb(255,255,255), 3);

            ALLEGRO_BITMAP* img = nullptr;
            if (i==0) img = wizard_img;
            else if (i==1) img = archer_img;
            else if (i==2) img = farm_img;
            else img = summoner_img;
            if (img) al_draw_scaled_bitmap(img, 0, 0, 32, 32, bx+4, 265, 28, 28, 0);

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
            al_draw_text(font, al_map_rgb(180,180,180), 200, 308, 0, "LKM = Place, PKM = Cancel");
        }

        if (zhizni <= 0) {
            al_draw_filled_rectangle(150, 100, 490, 180, al_map_rgb(0,0,0));
            al_draw_rectangle(150, 100, 490, 180, al_map_rgb(255,50,50), 3);
            if (font) al_draw_text(font, al_map_rgb(255,50,50), 320, 130, ALLEGRO_ALIGN_CENTER, "Game Over");
            Engine::stop();
        }
    }
};

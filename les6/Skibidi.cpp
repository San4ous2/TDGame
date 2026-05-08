#include "engine.hpp"
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <vector>
#include <algorithm> // for std::min
#include <allegro5/allegro.h>

using namespace std;

struct Point {
    float x, y;
};

class Square {
protected:
    float size;
    Point pos;
    ALLEGRO_COLOR color;

public:
    int dirx = 1;
    int diry = 1;

    Square(Point pos_, double size_, ALLEGRO_COLOR color_) {
        size = size_;
        pos = pos_;
        color = color_;
    };

    // Keep your AABB check
    bool is_colliding(Square& other) {
        return (pos.x < other.pos.x + other.size &&
                pos.x + size > other.pos.x &&
                pos.y < other.pos.y + other.size &&
                pos.y + size > other.pos.y);
    }

    void physics_process(Square& other) {
        // 1. Update movement on BOTH axes
        pos.x += dirx;
        pos.y += diry;

        // 2. Screen Boundary Logic
        if (pos.x <= 0 || pos.x + size >= 640) {
            dirx *= -1;
        }
        if (pos.y <= 0 || pos.y + size >= 360) {
            diry *= -1;
        }

        // 3. Side-Specific Collision Logic
        if (is_colliding(other)) {
            // Calculate overlaps
            float overlap_left   = (pos.x + size) - other.pos.x;
            float overlap_right  = (other.pos.x + other.size) - pos.x;
            float overlap_top    = (pos.y + size) - other.pos.y;
            float overlap_bottom = (other.pos.y + other.size) - pos.y;

            // Find the smallest overlap to determine the collision side
            float min_overlap_x = min(overlap_left, overlap_right);
            float min_overlap_y = min(overlap_top, overlap_bottom);

            if (min_overlap_x < min_overlap_y) {
                // Horizontal collision (Hit from Left or Right)
                dirx *= -1;
                // Simple "push" to resolve the overlap so they don't stick
                pos.x += dirx * 2;
            } else {
                // Vertical collision (Hit from Top or Bottom)
                diry *= -1;
                pos.y += diry * 2;
            }

            cout << "Bounced! New Dir: (" << dirx << "," << diry << ")" << endl;
        }
    }

    void render_process() {
        al_draw_filled_rectangle(pos.x, pos.y, pos.x + size, pos.y + size, color);
    }
};

class Screen : public Engine {
public:
    vector<Square> objects = {};
    void add_object(Square obj) { objects.push_back(obj); }

    void physics_process() override {
        // Only run collision check once for the pair to avoid double-bouncing
        objects[0].physics_process(objects[1]);
        objects[1].physics_process(objects[0]);
    }

    void render_process() override {
        objects[0].render_process();
        objects[1].render_process();
    }
};

int main() {
    Screen screen;
    // Set them on paths to collide
    Square Square1({100, 100}, 50, al_map_rgb(255, 0, 0));
    Square Square2({160, 100}, 50, al_map_rgb(0, 255, 0));

    screen.add_object(Square1);
    screen.add_object(Square2);
    screen.start();
}

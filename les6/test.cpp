#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>
#include <vector>
#include <cmath>

// Game object class
class GameObject {
public:
    float x, y;       // Position
    float w, h;       // Width and height
    ALLEGRO_COLOR color;

    GameObject(float px, float py, float pw, float ph, ALLEGRO_COLOR c)
        : x(px), y(py), w(pw), h(ph), color(c) {}

    // Draw the object
    void draw() const {
        al_draw_filled_rectangle(x, y, x + w, y + h, color);
    }

    // Check collision with another GameObject (AABB)
    bool collidesWith(const GameObject& other) const {
        return !(x + w < other.x ||   // this is left of other
                 x > other.x + other.w || // this is right of other
                 y + h < other.y ||   // this is above other
                 y > other.y + other.h);  // this is below other
    }
};

int main() {
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!\n";
        return -1;
    }
    al_init_primitives_addon();

    ALLEGRO_DISPLAY* display = al_create_display(800, 600);
    if (!display) {
        std::cerr << "Failed to create display!\n";
        return -1;
    }

    // Create some objects
    std::vector<GameObject> objects;
    objects.emplace_back(100, 100, 50, 50, al_map_rgb(255, 0, 0));
    objects.emplace_back(130, 130, 50, 50, al_map_rgb(0, 255, 0));
    objects.emplace_back(300, 300, 50, 50, al_map_rgb(0, 0, 255));

    // Detect collisions between all pairs
    for (size_t i = 0; i < objects.size(); ++i) {
        for (size_t j = i + 1; j < objects.size(); ++j) {
            if (objects[i].collidesWith(objects[j])) {
                std::cout << "Collision detected between object " << i
                          << " and object " << j << "\n";
            }
        }
    }

    // Draw objects
    al_clear_to_color(al_map_rgb(0, 0, 0));
    for (const auto& obj : objects) {
        obj.draw();
    }
    al_flip_display();

    al_rest(3.0);
    al_destroy_display(display);
    return 0;
}

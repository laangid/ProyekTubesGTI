#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include "Utils.h"

struct Building {
    float x, z;
    float width, depth, height;
    float r, g, b;
    int   type;
};

class City {
private:
    std::vector<Building> buildings;
    void drawStreetLamp(float x, float z);

public:
    void init();
    void drawProceduralRoads(float carX, float carZ);
    void drawScenery(float carX, float carZ); 
    void drawBuildings(float carX, float carZ);
    void drawBuildingShadows(float carX, float carZ, GLfloat* lightPos);
    void drawPerimeterFences();
    bool isColliding(float testX, float testZ, float carRadius);
};

class Car {
public:
    float x, z;
    float angle, speed;
    float maxSpeed;
    float width, length;
    
    // Physics
    float wheelRot; 
    float bobbing;  // Efek ayun bodi per mobil jalan
    float bodyRoll; // Efek bodi miring saat belok
    std::vector<Particle> exhaust; // Asap

    Car();
    void reset();
    void update(bool keys[256], City& city);
    void draw();
    void drawShadow(GLfloat* lightPos);
    
    // Handler Asap
    void updateParticles();
    void drawParticles();
};

class Checkpoint {
public:
    float x, z;
    float radius, rot;

    Checkpoint();
    void spawn(City& city, float carX, float carZ);
    void draw(float carX, float carZ);
    bool checkCollision(float carX, float carZ);
};

#endif

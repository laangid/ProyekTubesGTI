#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "GameObjects.h"
#include "Imageloader.h"

class GameManager {
private:
    City city;
    Car car;
    Checkpoint checkpoint;

    int winW, winH;
    int score;
    float timeLeft;
    bool gameStarted, gameOver;
    int lastTimeMs;
    bool keys[256];

    GLfloat lightPos[4];
    
    float camYaw;       
    float camPitch;     
    float targetCamYaw;   
    float targetCamPitch; 
    int lastMouseX;
    int lastMouseY;
    bool isDragging;
    
    GLuint bgTexture;

    void drawHUD();
    void setCamera();
    void drawSkyBackground();
    void drawMiniMap();

public:
    GameManager();
    void loadTexture();
    void reset();
    void update();
    void render();
    void reshape(int w, int h);
    
    void handleKeyDown(unsigned char key);
    void handleKeyUp(unsigned char key);
    
    void handleMouse(int button, int state, int x, int y);
    void handleMotion(int x, int y);
};

#endif

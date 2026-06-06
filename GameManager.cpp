#include "GameManager.h"

GameManager::GameManager() : winW(1000), winH(700) {
    lightPos[0] = 15.0f; lightPos[1] = 30.0f; lightPos[2] = 10.0f; lightPos[3] = 0.0f;
    reset();
}

void GameManager::reset() {
    for (int i = 0; i < 256; i++) keys[i] = false;
    score = 0; timeLeft = 20.0f;
    gameStarted = false; gameOver = false;
    
    camYaw = 0.0f; targetCamYaw = 0.0f;
    camPitch = 25.0f; targetCamPitch = 25.0f;
    isDragging = false;

    car.reset();
    city.init();
    checkpoint.spawn(city, car.x, car.z);
}

void GameManager::update() {
    int cur = glutGet(GLUT_ELAPSED_TIME);
    float dt = (cur - lastTimeMs) / 1000.0f;
    lastTimeMs = cur;

    if (!gameStarted || gameOver) return;

    timeLeft -= dt;
    if (timeLeft <= 0.0f) { timeLeft = 0.0f; gameOver = true; }

    car.update(keys, city);

    if (checkpoint.checkCollision(car.x, car.z)) {
    	score += 10;
	    timeLeft = 20.0f;
	
	    checkpoint.spawn(city, car.x, car.z);
	}
    
    if (!isDragging) {
        targetCamYaw *= 0.95f; 
        targetCamPitch += (25.0f - targetCamPitch) * 0.05f;
    }

    camYaw += (targetCamYaw - camYaw) * 0.15f;
    camPitch += (targetCamPitch - camPitch) * 0.15f;
}

void GameManager::handleMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDragging = true; lastMouseX = x; lastMouseY = y;
        } else if (state == GLUT_UP) {
            isDragging = false;
        }
    }
}

void GameManager::handleMotion(int x, int y) {
    if (isDragging) {
        float dx = (float)(x - lastMouseX);
        float dy = (float)(y - lastMouseY);
        targetCamYaw -= dx * 0.15f; targetCamPitch += dy * 0.15f;
        if (targetCamPitch < 5.0f) targetCamPitch = 5.0f;
        if (targetCamPitch > 80.0f) targetCamPitch = 80.0f;
        lastMouseX = x; lastMouseY = y;
    }
}

void GameManager::setCamera() {
    float totalYaw = car.angle + camYaw; 
    float radYaw = totalYaw * PI / 180.0f;
    float radPitch = camPitch * PI / 180.0f;
    
    float distance = 12.0f; 
    float hDist = distance * cosf(radPitch);
    float vDist = distance * sinf(radPitch);
    
    float camX = car.x - hDist * sinf(radYaw);
    float camY = 0.5f + vDist;
    float camZ = car.z - hDist * cosf(radYaw);
    
    gluLookAt(camX, camY, camZ, car.x, 3.0f, car.z, 0.0f, 1.0f, 0.0f);
}

void GameManager::drawSkyBackground() {
    // Matikan efek agar langit terlihat jelas dan dirender paling belakang
    glDisable(GL_LIGHTING); 
    glDisable(GL_DEPTH_TEST); 
    glDisable(GL_FOG); 
    glDepthMask(GL_FALSE); // Kunci Z-buffer agar kota/mobil merender di depannya

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    glColor3f(1.0f, 1.0f, 1.0f); 

    // --- TRIK TEKSTUR: Biar gambar gunung tidak pecah/melar ---
    // Karena tabungnya raksasa, kita ulang gambar gunungnya 3x melingkar
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glScalef(3.0f, 1.0f, 1.0f); 
    glMatrixMode(GL_MODELVIEW);
    // --------------------------------------------------------

    glPushMatrix();
    // 1. Posisikan tabung langit mengikuti mobil, tapi agak turun ke bawah (-50.0f)
    glTranslatef(car.x, -50.0f, car.z);
    
    // 2. Berdirikan tabungnya (default OpenGL tabung itu tiduran horizontal)
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    // 3. Gambar Tabung 3D (Sky Cylinder)
    GLUquadric* quad = gluNewQuadric();
    gluQuadricOrientation(quad, GLU_INSIDE); // Tekstur menghadap ke dalam menghadap kita
    gluQuadricTexture(quad, GL_TRUE);        // Aktifkan mode tekstur
    
    // Radius bawah 200, radius atas 200, tinggi 250, kehalusan lingkaran 32
    gluCylinder(quad, 200.0f, 200.0f, 250.0f, 32, 1); 
    gluDeleteQuadric(quad);

    glPopMatrix();

    // --- KEMBALIKAN TEKSTUR KE NORMAL ---
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    // ------------------------------------

    // Nyalakan kembali settingan normal untuk kota dan mobil
    glDisable(GL_TEXTURE_2D);
    glDepthMask(GL_TRUE); 
    glEnable(GL_FOG); 
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_LIGHTING);
}

void GameManager::drawMiniMap() {
    int mapSize = 180; 
    int padding = 20;
    // Map ditaruh tepat di bawah panel UI atas
    glViewport(winW - mapSize - padding, winH - mapSize - padding - 70, mapSize, mapSize);
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glDisable(GL_FOG);
    
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(-MAP_LIMIT, MAP_LIMIT, MAP_LIMIT, -MAP_LIMIT); 
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    
    // Background Minimap Hitam Transparan
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.05f, 0.05f, 0.1f, 0.7f);
    glBegin(GL_QUADS);
        glVertex2f(-MAP_LIMIT, -MAP_LIMIT); glVertex2f(MAP_LIMIT, -MAP_LIMIT);
        glVertex2f(MAP_LIMIT, MAP_LIMIT); glVertex2f(-MAP_LIMIT, MAP_LIMIT);
    glEnd();
    glDisable(GL_BLEND);

    // Border Map (Garis Emas)
    glColor3f(1.0f, 0.8f, 0.1f);
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(-MAP_LIMIT, -MAP_LIMIT); glVertex2f(MAP_LIMIT, -MAP_LIMIT);
        glVertex2f(MAP_LIMIT, MAP_LIMIT); glVertex2f(-MAP_LIMIT, MAP_LIMIT);
    glEnd();
    glLineWidth(1.0f);

    // Checkpoint (Kotak Emas Berkedip di Map)
    float flash = (sinf(glutGet(GLUT_ELAPSED_TIME) * 0.01f) + 1.0f) * 0.5f;
    glColor3f(1.0f, 0.8f + 0.2f*flash, flash); 
    glPushMatrix(); glTranslatef(checkpoint.x, checkpoint.z, 0);
    glBegin(GL_POLYGON);
    for(int i=0; i<360; i+=45) glVertex2f(cosf(i*PI/180.0f)*7.0f, sinf(i*PI/180.0f)*7.0f);
    glEnd();
    glPopMatrix();

    // Posisi Player (Segitiga Cyan Terang)
    glColor3f(0.0f, 1.0f, 1.0f);
    glPushMatrix(); glTranslatef(car.x, car.z, 0); glRotatef(-car.angle, 0, 0, 1);
    glBegin(GL_TRIANGLES); glVertex2f(0.0f, 7.0f); glVertex2f(-5.0f, -5.0f); glVertex2f( 5.0f, -5.0f); glEnd();
    glPopMatrix();

    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, winW, winH); 
    glEnable(GL_FOG); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
}

void GameManager::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

	setCamera();
    drawSkyBackground(); 
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    city.drawProceduralRoads(car.x, car.z);
    city.drawScenery(car.x, car.z);     
    city.drawPerimeterFences();         
    
    city.drawBuildingShadows(car.x, car.z, lightPos);
    car.drawShadow(lightPos);
    
    city.drawBuildings(car.x, car.z);
    checkpoint.draw(car.x, car.z);
    car.draw();
    
    drawHUD();
    drawMiniMap();
    
    glutSwapBuffers();
}

void GameManager::drawHUD() {
    char buf[256];
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glDisable(GL_FOG);

    // PANEL ATAS (Dashboard Hitam Transparan)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1f, 0.1f, 0.15f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2f(0, winH); glVertex2f(winW, winH);
        glVertex2f(winW, winH - 60); glVertex2f(0, winH - 60);
    glEnd();

    // GARIS AKSEN EMAS DI BAWAH PANEL
    glColor3f(1.0f, 0.8f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(0, winH - 60); glVertex2f(winW, winH - 60);
        glVertex2f(winW, winH - 64); glVertex2f(0, winH - 64);
    glEnd();
    glDisable(GL_BLEND);

    // TEKS SCORE
    glColor3f(1.0f, 1.0f, 1.0f);
    sprintf(buf, "SCORE : %d", score);
    drawString(30, winH - 40, GLUT_BITMAP_TIMES_ROMAN_24, buf);

    // TEKS WAKTU (Peringatan Merah jika < 10 detik)
    if (timeLeft <= 10.0f) glColor3f(1.0f, 0.2f, 0.2f); 
    else glColor3f(0.2f, 1.0f, 0.2f); 
    sprintf(buf, "TIME : %.1f s", timeLeft);
    drawString(250, winH - 40, GLUT_BITMAP_TIMES_ROMAN_24, buf);

    // TEKS KONTROL DI POJOK KIRI BAWAH
    glColor3f(1, 1, 1);
    drawString(20, 20, GLUT_BITMAP_HELVETICA_12, "W A S D: Gas & Belok   |   Drag Mouse: Lihat Kamera   |   R: Restart Game");

    if (!gameStarted) {
        glEnable(GL_BLEND); glColor4f(0,0,0,0.5f);
        glBegin(GL_QUADS); glVertex2f(0,0); glVertex2f(winW,0); glVertex2f(winW,winH); glVertex2f(0,winH); glEnd();
        glDisable(GL_BLEND);
        
        glColor3f(1.0f, 0.8f, 0.1f); 
        drawString(winW/2-140, winH/2, GLUT_BITMAP_TIMES_ROMAN_24, "PRESS SPACE TO START");
    } else if (gameOver) {
        glEnable(GL_BLEND); glColor4f(0,0,0,0.7f);
        glBegin(GL_QUADS); glVertex2f(0,0); glVertex2f(winW,0); glVertex2f(winW,winH); glVertex2f(0,winH); glEnd();
        glDisable(GL_BLEND);

        glColor3f(1,0.2f,0.2f); drawString(winW/2-60, winH/2+40, GLUT_BITMAP_TIMES_ROMAN_24, "TIME'S UP!");
        glColor3f(1,1,1); sprintf(buf, "Final Score: %d", score); drawString(winW/2-60, winH/2-10, GLUT_BITMAP_HELVETICA_18, buf); 
        glColor3f(1.0f, 0.8f, 0.1f); drawString(winW/2-80, winH/2-45, GLUT_BITMAP_HELVETICA_18, "Press R to Restart");
    }

    glEnable(GL_FOG); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}

void GameManager::reshape(int w, int h) {
    if (!h) h = 1; winW = w; winH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(60.0, (double)w/h, 0.1, 500.0); 
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

void GameManager::handleKeyDown(unsigned char key) {
    keys[key] = true;
    if (!gameStarted && key==' ') { gameStarted = true; lastTimeMs = glutGet(GLUT_ELAPSED_TIME); }
    if (key=='r'||key=='R') reset();
}
void GameManager::handleKeyUp(unsigned char key) { keys[key] = false; }

void GameManager::loadTexture() {
    Image* image = loadBMP("bg.bmp");
    
    glGenTextures(1, &bgTexture);
    glBindTexture(GL_TEXTURE_2D, bgTexture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
    delete image;
}

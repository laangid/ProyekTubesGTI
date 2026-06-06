#include "GameObjects.h"

// Kota
void City::init() {
    buildings.clear();
    srand(12345);
    for (float x = -90.0f; x < 90.0f; x += 15.0f) {
        for (float z = -90.0f; z < 90.0f; z += 15.0f) {
            if (abs(x) < 20.0f && abs(z) < 20.0f) continue;

            Building b;
            b.x = x + 7.5f;
            b.z = z + 7.5f;
            b.type = rand() % 3;
            
            if (b.type == 1) { 
                b.width = 4.0f + (rand()%2); b.depth = 4.0f + (rand()%2); b.height = 20.0f + (rand()%15);
            } else if (b.type == 2) {
                b.width = 6.0f + (rand()%3); b.depth = 5.0f + (rand()%3); b.height = 10.0f + (rand()%8);
            } else { 
                b.width = 5.0f + (rand()%3); b.depth = 5.0f + (rand()%3); b.height = 12.0f + (rand()%10);
            }

            int color = rand() % 4;
            if (color == 0)      { b.r=0.55f; b.g=0.55f; b.b=0.60f; } 
            else if (color == 1) { b.r=0.35f; b.g=0.50f; b.b=0.70f; } 
            else if (color == 2) { b.r=0.65f; b.g=0.40f; b.b=0.30f; } 
            else                 { b.r=0.75f; b.g=0.70f; b.b=0.55f; }

            buildings.push_back(b);
        }
    }
}

void City::drawStreetLamp(float x, float z) {
    glColor3f(0.4f, 0.4f, 0.4f);
    glPushMatrix(); glTranslatef(x, 2.5f, z); drawBox(0.15f, 5.0f, 0.15f); glPopMatrix();
    glColor3f(0.8f, 0.8f, 0.4f);
    glPushMatrix(); glTranslatef(x, 5.1f, z); drawBox(0.6f, 0.2f, 0.6f); glPopMatrix();
    glColor3f(0.9f, 0.9f, 0.7f);
    glPushMatrix(); glTranslatef(x, 4.9f, z); glutSolidSphere(0.15f, 8, 8); glPopMatrix();
}

void City::drawProceduralRoads(float carX, float carZ) {
    glDisable(GL_LIGHTING);

    glColor3f(0.18f, 0.32f, 0.15f); 
    float outDist = MAP_LIMIT + 80.0f; 
    glBegin(GL_QUADS);
        glVertex3f(-outDist, 0.005f, -outDist); glVertex3f( outDist, 0.005f, -outDist);
        glVertex3f( outDist, 0.005f,  outDist); glVertex3f(-outDist, 0.005f,  outDist);
    glEnd();

    glColor3f(0.18f, 0.18f, 0.18f); 
    glBegin(GL_QUADS);
        glVertex3f(-MAP_LIMIT, 0.01f, -MAP_LIMIT); glVertex3f( MAP_LIMIT, 0.01f, -MAP_LIMIT);
        glVertex3f( MAP_LIMIT, 0.01f,  MAP_LIMIT); glVertex3f(-MAP_LIMIT, 0.01f,  MAP_LIMIT);
    glEnd();

    glColor3f(0.50f, 0.50f, 0.50f); 
    glBegin(GL_QUADS);
    for (float x = -90.0f; x < 90.0f; x += 15.0f) {
        for (float z = -90.0f; z < 90.0f; z += 15.0f) {
            if (abs(x) < 20.0f && abs(z) < 20.0f) continue;
            float cx = x + 7.5f; float cz = z + 7.5f;
            if (abs(cx - carX) > RENDER_DISTANCE || abs(cz - carZ) > RENDER_DISTANCE) continue;
            glVertex3f(cx - 4.5f, 0.02f, cz - 4.5f); glVertex3f(cx + 4.5f, 0.02f, cz - 4.5f);
            glVertex3f(cx + 4.5f, 0.02f, cz + 4.5f); glVertex3f(cx - 4.5f, 0.02f, cz + 4.5f);
        }
    }
    glEnd();

    glColor3f(0.9f, 0.85f, 0.1f); 
    glBegin(GL_QUADS);
    float dashLen = 2.0f, gapLen = 2.0f;
    for (float i = -90.0f; i <= 90.0f; i += 15.0f) {
        if (abs(i - carX) > RENDER_DISTANCE && abs(i - carZ) > RENDER_DISTANCE) continue;
        for (float z = -90.0f; z < 90.0f; z += dashLen + gapLen) {
            if (abs(z - carZ) > RENDER_DISTANCE) continue;
            glVertex3f(i - 0.1f, 0.03f, z);           glVertex3f(i + 0.1f, 0.03f, z);
            glVertex3f(i + 0.1f, 0.03f, z + dashLen); glVertex3f(i - 0.1f, 0.03f, z + dashLen);
        }
        for (float x = -90.0f; x < 90.0f; x += dashLen + gapLen) {
            if (abs(x - carX) > RENDER_DISTANCE) continue;
            glVertex3f(x,           0.03f, i - 0.1f); glVertex3f(x + dashLen, 0.03f, i - 0.1f);
            glVertex3f(x + dashLen, 0.03f, i + 0.1f); glVertex3f(x,           0.03f, i + 0.1f);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);

    for (float x = -90.0f; x < 90.0f; x += 15.0f) {
        for (float z = -90.0f; z < 90.0f; z += 15.0f) {
            if (abs(x) < 20.0f && abs(z) < 20.0f) continue;
            float cx = x + 7.5f; float cz = z + 7.5f;
            if (abs(cx - carX) > RENDER_DISTANCE || abs(cz - carZ) > RENDER_DISTANCE) continue;
            drawStreetLamp(cx - 3.8f, cz - 3.8f); 
        }
    }
}

void City::drawScenery(float carX, float carZ) {
    glDisable(GL_LIGHTING);
    float dist = MAP_LIMIT + 35.0f; 
    float base = -5.0f;             
    float step = 5.0f;
    
    glBegin(GL_TRIANGLE_STRIP);
    for(float x = -dist; x <= dist; x += step) {
        float h = 25.0f + 15.0f * sinf(x * 0.05f) + 10.0f * cosf(-dist * 0.08f) + 5.0f * sinf((x - dist) * 0.15f);
        glColor3f(0.12f, 0.25f, 0.12f); glVertex3f(x, base, -dist);
        glColor3f(0.35f, 0.50f, 0.35f); glVertex3f(x, h, -dist);
    }
    for(float z = -dist; z <= dist; z += step) {
        float h = 25.0f + 15.0f * sinf(dist * 0.05f) + 10.0f * cosf(z * 0.08f) + 5.0f * sinf((dist + z) * 0.15f);
        glColor3f(0.12f, 0.25f, 0.12f); glVertex3f(dist, base, z);
        glColor3f(0.35f, 0.50f, 0.35f); glVertex3f(dist, h, z);
    }
    for(float x = dist; x >= -dist; x -= step) {
        float h = 25.0f + 15.0f * sinf(x * 0.05f) + 10.0f * cosf(dist * 0.08f) + 5.0f * sinf((x + dist) * 0.15f);
        glColor3f(0.12f, 0.25f, 0.12f); glVertex3f(x, base, dist);
        glColor3f(0.35f, 0.50f, 0.35f); glVertex3f(x, h, dist);
    }
    for(float z = dist; z >= -dist; z -= step) {
        float h = 25.0f + 15.0f * sinf(-dist * 0.05f) + 10.0f * cosf(z * 0.08f) + 5.0f * sinf((-dist + z) * 0.15f);
        glColor3f(0.12f, 0.25f, 0.12f); glVertex3f(-dist, base, z);
        glColor3f(0.35f, 0.50f, 0.35f); glVertex3f(-dist, h, z);
    }
    float h_end = 25.0f + 15.0f * sinf(-dist * 0.05f) + 10.0f * cosf(-dist * 0.08f) + 5.0f * sinf((-dist - dist) * 0.15f);
    glColor3f(0.12f, 0.25f, 0.12f); glVertex3f(-dist, base, -dist); 
    glColor3f(0.35f, 0.50f, 0.35f); glVertex3f(-dist, h_end, -dist);
    glEnd();
    glEnable(GL_LIGHTING);

    for (float tx = -MAP_LIMIT - 40; tx <= MAP_LIMIT + 40; tx += 20.0f) {
        for (float tz = -MAP_LIMIT - 40; tz <= MAP_LIMIT + 40; tz += 20.0f) {
            if (abs(tx) > MAP_LIMIT + 5.0f || abs(tz) > MAP_LIMIT + 5.0f) {
                float offsetX = sinf(tx * 1.3f + tz) * 6.0f;
                float offsetZ = cosf(tz * 1.7f + tx) * 6.0f;
                float fx = tx + offsetX; float fz = tz + offsetZ;
                
                if (abs(fx - carX) < RENDER_DISTANCE + 40 && abs(fz - carZ) < RENDER_DISTANCE + 40) {
                    glPushMatrix();
                    glTranslatef(fx, 0.0f, fz);
                    glColor3f(0.35f, 0.20f, 0.10f);
                    glPushMatrix(); glTranslatef(0.0f, 1.0f, 0.0f); drawBox(0.5f, 2.0f, 0.5f); glPopMatrix();
                    glColor3f(0.15f, 0.35f, 0.15f);
                    glPushMatrix(); glTranslatef(0.0f, 3.0f, 0.0f); drawBox(3.0f, 2.5f, 3.0f); glPopMatrix();
                    glPushMatrix(); glTranslatef(0.0f, 5.0f, 0.0f); drawBox(1.5f, 2.0f, 1.5f); glPopMatrix();
                    glPopMatrix();
                }
            }
        }
    }
}

void City::drawPerimeterFences() {
    glEnable(GL_LIGHTING);
    glColor3f(0.6f, 0.6f, 0.6f); 
    float height = 2.0f; float thickness = 1.0f; float length = MAP_LIMIT * 2.0f + thickness;
    
    glPushMatrix(); glTranslatef(0.0f, height/2.0f, -MAP_LIMIT); drawBox(length, height, thickness); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, height/2.0f, MAP_LIMIT); drawBox(length, height, thickness); glPopMatrix();
    glPushMatrix(); glTranslatef(-MAP_LIMIT, height/2.0f, 0.0f); drawBox(thickness, height, length); glPopMatrix();
    glPushMatrix(); glTranslatef(MAP_LIMIT, height/2.0f, 0.0f); drawBox(thickness, height, length); glPopMatrix();
    
    glColor3f(0.85f, 0.75f, 0.1f);
    float h2 = height + 0.2f; float rThick = thickness * 0.4f;
    glPushMatrix(); glTranslatef(0.0f, h2/2.0f, -MAP_LIMIT); drawBox(length, h2, rThick); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, h2/2.0f, MAP_LIMIT); drawBox(length, h2, rThick); glPopMatrix();
    glPushMatrix(); glTranslatef(-MAP_LIMIT, h2/2.0f, 0.0f); drawBox(rThick, h2, length); glPopMatrix();
    glPushMatrix(); glTranslatef(MAP_LIMIT, h2/2.0f, 0.0f); drawBox(rThick, h2, length); glPopMatrix();
}

void City::drawBuildings(float carX, float carZ) {
    for (size_t i = 0; i < buildings.size(); i++) {
        Building& b = buildings[i];
        if (abs(b.x - carX) > RENDER_DISTANCE || abs(b.z - carZ) > RENDER_DISTANCE) continue;

        glColor3f(b.r, b.g, b.b);
        glPushMatrix(); glTranslatef(b.x, b.height/2.0f, b.z); drawBox(b.width, b.height, b.depth); glPopMatrix();

        glColor3f(b.r*0.7f, b.g*0.7f, b.b*0.7f);
        glPushMatrix(); glTranslatef(b.x, b.height + 0.3f, b.z); drawBox(b.width*0.95f, 0.6f, b.depth*0.95f); glPopMatrix();

        if (b.type == 1) {
            glColor3f(0.5f, 0.5f, 0.5f);
            glPushMatrix(); glTranslatef(b.x, b.height + 1.5f, b.z); drawBox(0.2f, 2.0f, 0.2f); glPopMatrix();
        }

        glColor3f(0.5f, 0.65f, 0.8f); 
        int floors = (int)(b.height / 3.0f);
        for (int f = 1; f < floors; f++) {
            float fy = f * 3.0f;
            for (int w = 0; w < 2; w++) {
                float wx = b.x - b.width/4.0f + w * b.width/2.0f;
                glPushMatrix(); glTranslatef(wx, fy, b.z + b.depth/2.0f + 0.01f);
                glBegin(GL_QUADS);
                    glVertex3f(-0.3f, 0, 0); glVertex3f( 0.3f, 0, 0);
                    glVertex3f( 0.3f, 0.8f, 0); glVertex3f(-0.3f, 0.8f, 0);
                glEnd();
                glPopMatrix();
                
                glPushMatrix(); glTranslatef(wx, fy, b.z - b.depth/2.0f - 0.01f);
                glBegin(GL_QUADS);
                    glVertex3f(-0.3f, 0, 0); glVertex3f( 0.3f, 0, 0);
                    glVertex3f( 0.3f, 0.8f, 0); glVertex3f(-0.3f, 0.8f, 0);
                glEnd();
                glPopMatrix();
            }
        }
    }
}

void City::drawBuildingShadows(float carX, float carZ, GLfloat* lightPos) {
    GLfloat groundPlane[4] = {0.0f, 1.0f, 0.0f, -0.01f};
    GLfloat shadowMat[4][4];
    buildShadowMatrix(shadowMat, groundPlane, lightPos);

    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.35f); 
    glEnable(GL_POLYGON_OFFSET_FILL); glPolygonOffset(-1.5f, -1.5f);

    GLfloat mat[16] = {
        shadowMat[0][0], shadowMat[0][1], shadowMat[0][2], shadowMat[0][3],
        shadowMat[1][0], shadowMat[1][1], shadowMat[1][2], shadowMat[1][3],
        shadowMat[2][0], shadowMat[2][1], shadowMat[2][2], shadowMat[2][3],
        shadowMat[3][0], shadowMat[3][1], shadowMat[3][2], shadowMat[3][3],
    };

    for (size_t i = 0; i < buildings.size(); i++) {
        Building& b = buildings[i];
        if (abs(b.x - carX) > RENDER_DISTANCE || abs(b.z - carZ) > RENDER_DISTANCE) continue;
        glPushMatrix();
        glMultMatrixf(mat);
        glTranslatef(b.x, b.height/2.0f, b.z);
        drawBox(b.width, b.height, b.depth);
        glPopMatrix();
    }
    glDisable(GL_BLEND); glDisable(GL_POLYGON_OFFSET_FILL);
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
}

bool City::isColliding(float testX, float testZ, float carRadius) {
    if (testX < -MAP_LIMIT || testX > MAP_LIMIT || testZ < -MAP_LIMIT || testZ > MAP_LIMIT) return true;
    for (size_t i = 0; i < buildings.size(); i++) {
        Building& b = buildings[i];
        if (abs(b.x - testX) > 15.0f || abs(b.z - testZ) > 15.0f) continue;

        float cx = testX < b.x - b.width/2  ? b.x - b.width/2  : testX > b.x + b.width/2  ? b.x + b.width/2  : testX;
        float cz = testZ < b.z - b.depth/2  ? b.z - b.depth/2  : testZ > b.z + b.depth/2  ? b.z + b.depth/2  : testZ;
        
        float dx = testX - cx; float dz = testZ - cz;
        // Jarak dari titik tes (moncong/buntut) ke kotak bangunan diukur.
        if (dx*dx + dz*dz < carRadius*carRadius) return true;
    }
    return false;
}

// Mobil
Car::Car() : maxSpeed(0.5f), width(1.6f), length(3.6f) { reset(); }

void Car::reset() {
    x = 0.0f; z = 0.0f; angle = 0.0f; speed = 0.0f; wheelRot = 0.0f; bobbing = 0.0f; bodyRoll = 0.0f; exhaust.clear();
}

void Car::updateParticles() {
    if (fabsf(speed) > 0.01f) {
        for (int i = 0; i < 2; i++) { 
            Particle p;
            float rad = angle * PI / 180.0f;
            float side = (i == 0) ? 0.4f : -0.4f;
            p.x = x + cosf(rad)*side - sinf(rad)*1.85f;
            p.y = 0.1f;
            p.z = z - sinf(rad)*side - cosf(rad)*1.85f;
            p.vx = ((rand() % 10) - 5) * 0.003f;
            p.vy = 0.01f + (rand() % 10) * 0.003f; 
            p.vz = ((rand() % 10) - 5) * 0.003f;
            p.maxLife = 20.0f + (rand() % 10);
            p.life = p.maxLife; p.size = 0.15f;
            exhaust.push_back(p);
        }
    }

    for (size_t i = 0; i < exhaust.size(); ) {
        exhaust[i].x += exhaust[i].vx; exhaust[i].y += exhaust[i].vy; exhaust[i].z += exhaust[i].vz;
        exhaust[i].life -= 1.0f; exhaust[i].size += 0.015f; 
        if (exhaust[i].life <= 0.0f) exhaust.erase(exhaust.begin() + i);
        else i++;
    }
}

void Car::drawParticles() {
    glDisable(GL_LIGHTING); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glDepthMask(GL_FALSE); 
    for (size_t i = 0; i < exhaust.size(); i++) {
        float alpha = (exhaust[i].life / exhaust[i].maxLife) * 0.4f;
        glColor4f(0.5f, 0.5f, 0.5f, alpha); 
        glPushMatrix(); glTranslatef(exhaust[i].x, exhaust[i].y, exhaust[i].z); glutSolidSphere(exhaust[i].size, 5, 5); glPopMatrix();
    }
    glDepthMask(GL_TRUE); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}

void Car::update(bool keys[256], City& city) {
    if (keys['w']||keys['W']) speed += 0.02f;
    else if (keys['s']||keys['S']) speed -= 0.02f;
    else speed *= 0.93f;

    if (speed > maxSpeed) speed = maxSpeed;
    if (speed < -maxSpeed/2.0f) speed = -maxSpeed/2.0f;

    if (fabsf(speed) > 0.05f) {
        float ts = (speed < 0) ? -3.0f : 3.0f;
        if (keys['a']||keys['A']) angle += ts;
        if (keys['d']||keys['D']) angle -= ts;
    }

    float rad = angle * PI / 180.0f;
    float nx = x + speed * sinf(rad); float nz = z + speed * cosf(rad);

    // KOLISI 2-TITIK: Mengecek moncong depan dan pantat belakang secara terpisah.
    float frontX = nx + sinf(rad) * (length * 0.35f);
    float frontZ = nz + cosf(rad) * (length * 0.35f);
    float backX  = nx - sinf(rad) * (length * 0.35f);
    float backZ  = nz - cosf(rad) * (length * 0.35f);

    if (!city.isColliding(frontX, frontZ, width * 0.5f) && 
        !city.isColliding(backX, backZ, width * 0.5f)) { 
        x = nx; z = nz; wheelRot += speed * 150.0f; 
        if (wheelRot > 360.0f) wheelRot -= 360.0f; if (wheelRot < 0.0f) wheelRot += 360.0f;
    } else {
        speed = -speed * 0.4f; // Efek terpental saat menabrak
    }
    
    if (fabsf(speed) > 0.05f) bobbing = sinf(glutGet(GLUT_ELAPSED_TIME) * 0.02f) * (fabsf(speed)/maxSpeed) * 0.06f;
    else bobbing = 0.0f;

    float targetRoll = 0.0f;
    if (fabsf(speed) > 0.1f) {
        if (keys['a']||keys['A']) targetRoll = -5.0f;
        if (keys['d']||keys['D']) targetRoll = 5.0f;
    }
    bodyRoll += (targetRoll - bodyRoll) * 0.1f;
    
    updateParticles(); 
}

void Car::draw() {
    drawParticles(); 

    glPushMatrix();
    glTranslatef(x, 0.5f, z);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    float wheelY = -0.1f; float wheelX = 0.95f; float wheelZ = 1.1f;
    float wheelPos[4][2] = {{ wheelX, wheelZ}, {-wheelX, wheelZ}, { wheelX, -wheelZ}, {-wheelX, -wheelZ}};
    
    for (int r = 0; r < 4; r++) {
        glPushMatrix(); glTranslatef(wheelPos[r][0], wheelY, wheelPos[r][1]); glRotatef(wheelRot, 1, 0, 0); 
        glPushMatrix(); glRotatef(90, 0, 1, 0); glColor3f(0.15f, 0.15f, 0.15f); glutSolidTorus(0.12f, 0.25f, 12, 18);
        glColor3f(0.6f, 0.6f, 0.6f); glutSolidTorus(0.04f, 0.16f, 8, 12); glPopMatrix();
        glColor3f(0.7f, 0.7f, 0.7f); drawBox(0.15f, 0.32f, 0.05f); drawBox(0.15f, 0.05f, 0.32f); glPopMatrix();
    }
    
    glPushMatrix(); glTranslatef(0.0f, bobbing, 0.0f); glRotatef(bodyRoll, 0, 0, 1); 
    glColor3f(0.85f, 0.2f, 0.2f); drawBox(1.6f, 0.5f, 3.6f);

    glBegin(GL_QUADS);
        glColor3f(0.15f, 0.15f, 0.15f); glNormal3f(0, 0.7f, 0.7f);
        glVertex3f(-0.75f, 0.25f, 0.5f);  glVertex3f( 0.75f, 0.25f, 0.5f); glVertex3f( 0.6f,  0.75f, 0.0f);  glVertex3f(-0.6f,  0.75f, 0.0f);
        glNormal3f(0, 0.7f, -0.7f);
        glVertex3f( 0.75f, 0.25f, -1.3f); glVertex3f(-0.75f, 0.25f, -1.3f); glVertex3f(-0.6f,  0.75f, -0.8f); glVertex3f( 0.6f,  0.75f, -0.8f);
        glNormal3f(-1, 0, 0);
        glVertex3f(-0.75f, 0.25f,  0.5f); glVertex3f(-0.6f,  0.75f,  0.0f); glVertex3f(-0.6f,  0.75f, -0.8f); glVertex3f(-0.75f, 0.25f, -1.3f);
        glNormal3f(1, 0, 0);
        glVertex3f(0.75f, 0.25f, -1.3f);  glVertex3f(0.6f,  0.75f, -0.8f); glVertex3f(0.6f,  0.75f,  0.0f);  glVertex3f(0.75f, 0.25f,  0.5f);
        glColor3f(0.85f, 0.2f, 0.2f); glNormal3f(0, 1, 0);
        glVertex3f(-0.6f, 0.75f,  0.0f);  glVertex3f( 0.6f, 0.75f,  0.0f); glVertex3f( 0.6f, 0.75f, -0.8f);  glVertex3f(-0.6f, 0.75f, -0.8f);
    glEnd();

    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(-0.3f, 0.251f, 1.8f); glVertex3f(-0.1f, 0.251f, 1.8f); glVertex3f(-0.1f, 0.251f, 0.5f); glVertex3f(-0.3f, 0.251f, 0.5f);
        glVertex3f(0.1f, 0.251f, 1.8f);  glVertex3f(0.3f, 0.251f, 1.8f);  glVertex3f(0.3f, 0.251f, 0.5f);  glVertex3f(0.1f, 0.251f, 0.5f);
        glVertex3f(-0.3f, 0.751f, 0.0f); glVertex3f(-0.1f, 0.751f, 0.0f); glVertex3f(-0.1f, 0.751f, -0.8f); glVertex3f(-0.3f, 0.751f, -0.8f);
        glVertex3f(0.1f, 0.751f, 0.0f);  glVertex3f(0.3f, 0.751f, 0.0f);  glVertex3f(0.3f, 0.751f, -0.8f);  glVertex3f(0.1f, 0.751f, -0.8f);
        glVertex3f(-0.3f, 0.251f, -1.3f); glVertex3f(-0.1f, 0.251f, -1.3f); glVertex3f(-0.1f, 0.251f, -1.8f); glVertex3f(-0.3f, 0.251f, -1.8f);
        glVertex3f(0.1f, 0.251f, -1.3f);  glVertex3f(0.3f, 0.251f, -1.3f);  glVertex3f(0.3f, 0.251f, -1.8f);  glVertex3f(0.1f, 0.251f, -1.8f);
    glEnd();

    glColor3f(0.85f, 0.2f, 0.2f); 
    glPushMatrix(); glTranslatef(0.0f, 0.30f, 1.2f); drawBox(0.8f, 0.15f, 0.6f); glPopMatrix();
    glColor3f(0.1f, 0.1f, 0.1f); 
    glPushMatrix(); glTranslatef(0.0f, 0.30f, 1.51f); drawBox(0.6f, 0.1f, 0.05f); glPopMatrix();
    glColor3f(0.4f, 0.4f, 0.4f); 
    glPushMatrix(); glTranslatef(0.0f, -0.1f, 1.8f); drawBox(1.65f, 0.2f, 0.2f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, -0.1f, -1.8f); drawBox(1.65f, 0.2f, 0.2f); glPopMatrix();
    
    // Headlights
    glColor3f(0.9f, 0.9f, 0.9f);
    glPushMatrix(); glTranslatef(0.55f, 0.1f, 1.82f); drawBox(0.35f, 0.25f, 0.1f); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.55f, 0.1f, 1.82f); drawBox(0.35f, 0.25f, 0.1f); glPopMatrix();
    
    // Taillights
    glColor3f(1.0f, 0.2f, 0.1f); 
    glPushMatrix(); glTranslatef(0.5f, 0.05f, -1.81f); drawBox(0.3f, 0.2f, 0.1f); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.5f, 0.05f, -1.81f); drawBox(0.3f, 0.2f, 0.1f); glPopMatrix();

    // Knalpot
    glColor3f(0.3f, 0.3f, 0.3f); 
    glPushMatrix(); glTranslatef(0.4f, -0.2f, -1.85f); drawBox(0.15f, 0.1f, 0.2f); glPopMatrix();
    glPushMatrix(); glTranslatef(-0.4f, -0.2f, -1.85f); drawBox(0.15f, 0.1f, 0.2f); glPopMatrix();

    glPopMatrix(); 
    glPopMatrix(); 
}

void Car::drawShadow(GLfloat* lightPos) {
    GLfloat groundPlane[4] = {0.0f, 1.0f, 0.0f, -0.01f}; GLfloat shadowMat[4][4]; buildShadowMatrix(shadowMat, groundPlane, lightPos);
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST); glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.35f);  
    glEnable(GL_POLYGON_OFFSET_FILL); glPolygonOffset(-1.5f, -1.5f);
    glPushMatrix(); GLfloat mat[16] = {shadowMat[0][0], shadowMat[0][1], shadowMat[0][2], shadowMat[0][3], shadowMat[1][0], shadowMat[1][1], shadowMat[1][2], shadowMat[1][3], shadowMat[2][0], shadowMat[2][1], shadowMat[2][2], shadowMat[2][3], shadowMat[3][0], shadowMat[3][1], shadowMat[3][2], shadowMat[3][3],}; glMultMatrixf(mat);
    glTranslatef(x, 0.5f, z); glRotatef(angle, 0.0f, 1.0f, 0.0f); drawBox(1.6f, 0.5f, 3.6f);
    glBegin(GL_QUADS);
        glVertex3f(-0.75f, 0.25f, 0.5f);  glVertex3f( 0.75f, 0.25f, 0.5f); glVertex3f( 0.6f,  0.75f, 0.0f);  glVertex3f(-0.6f,  0.75f, 0.0f);
        glVertex3f( 0.75f, 0.25f, -1.3f); glVertex3f(-0.75f, 0.25f, -1.3f); glVertex3f(-0.6f,  0.75f, -0.8f); glVertex3f( 0.6f,  0.75f, -0.8f);
        glVertex3f(-0.75f, 0.25f,  0.5f); glVertex3f(-0.6f,  0.75f,  0.0f); glVertex3f(-0.6f,  0.75f, -0.8f); glVertex3f(-0.75f, 0.25f, -1.3f);
        glVertex3f(0.75f, 0.25f, -1.3f);  glVertex3f(0.6f,  0.75f, -0.8f); glVertex3f(0.6f,  0.75f,  0.0f);  glVertex3f(0.75f, 0.25f,  0.5f);
        glVertex3f(-0.6f, 0.75f,  0.0f);  glVertex3f( 0.6f, 0.75f,  0.0f); glVertex3f( 0.6f, 0.75f, -0.8f);  glVertex3f(-0.6f, 0.75f, -0.8f);
    glEnd();
    float wheelPos[4][2] = {{ 0.95f, 1.1f}, {-0.95f, 1.1f}, { 0.95f, -1.1f}, {-0.95f, -1.1f}};
    for (int r = 0; r < 4; r++) {
        glPushMatrix(); glTranslatef(wheelPos[r][0], -0.1f, wheelPos[r][1]); glRotatef(wheelRot, 1, 0, 0); 
        glPushMatrix(); glRotatef(90, 0, 1, 0); glutSolidTorus(0.12f, 0.25f, 8, 12); glPopMatrix(); glPopMatrix();
    }
    glPopMatrix();
    glDisable(GL_BLEND); glDisable(GL_POLYGON_OFFSET_FILL); glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
}

// Checkpoint
Checkpoint::Checkpoint() : radius(1.5f), rot(0.0f) { }

void Checkpoint::spawn(City& city, float carX, float carZ) {
    bool valid = false;
    while (!valid) {
        float gridX = ((rand() % 10) - 5) * 15.0f; float gridZ = ((rand() % 10) - 5) * 15.0f;
        x = gridX + ((rand() % 3) - 1) * 3.0f; z = gridZ + ((rand() % 3) - 1) * 3.0f;
        float dx = x - carX; float dz = z - carZ;
        if (sqrt(dx*dx + dz*dz) > 25.0f && x > -MAP_LIMIT+5.0f && x < MAP_LIMIT-5.0f && z > -MAP_LIMIT+5.0f && z < MAP_LIMIT-5.0f && !city.isColliding(x, z, radius)) valid = true;
    }
}

void Checkpoint::draw(float carX, float carZ) {
    if (abs(x - carX) > RENDER_DISTANCE || abs(z - carZ) > RENDER_DISTANCE) return;
    rot += 3.0f;
    
    // PILAR CAHAYA EMAS TRANSPARAN DI BAWAH CHECKPOINT
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE); // Biar nggak bentrok render transparan lain
    
    glColor4f(1.0f, 0.8f, 0.1f, 0.3f); // Emas tembus pandang
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(-90, 1, 0, 0); // Tegak lurus ke atas
    GLUquadric* quad = gluNewQuadric();
    gluCylinder(quad, 0.6f, 0.6f, 15.0f, 16, 1);
    gluDeleteQuadric(quad);
    glPopMatrix();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);

    // BENTUK RODA GIGI (GEAR) EMAS BERPUTAR
    glColor3f(1.0f, 0.75f, 0.1f); // Warna Emas Metalik
    glPushMatrix();
    glTranslatef(x, 1.5f + sinf(rot*PI/180.0f)*0.5f, z); // Melayang naik turun
    glRotatef(rot, 0, 1, 0); // Putar di sumbu Y
    glRotatef(90, 1, 0, 0);  // Tidurkan bentuk gear
    
    // Lingkaran utama gear
    glutSolidTorus(0.25f, 0.8f, 16, 32); 

    // Mata gigi gear luar
    for(int i = 0; i < 8; i++) {
        glPushMatrix();
        glRotatef(i * 45.0f, 0, 0, 1);
        glTranslatef(0.9f, 0.0f, 0.0f);
        drawBox(0.4f, 0.3f, 0.4f);
        glPopMatrix();
    }
    
    // Jari-jari poros dalam
    for(int i = 0; i < 4; i++) {
        glPushMatrix();
        glRotatef(i * 45.0f, 0, 0, 1);
        drawBox(1.6f, 0.2f, 0.2f);
        glPopMatrix();
    }
    glPopMatrix();

    // Base Pad Abu-abu Gelap di lantai
    glColor3f(0.2f, 0.2f, 0.2f);
    glPushMatrix(); glTranslatef(x, 0.05f, z); drawBox(2.2f, 0.1f, 2.2f); glPopMatrix();
}

bool Checkpoint::checkCollision(float carX, float carZ) {
    float dx = carX - x, dz = carZ - z; 
    return (sqrtf(dx*dx + dz*dz) < radius + 1.0f);
}

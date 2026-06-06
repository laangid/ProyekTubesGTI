#ifndef UTILS_H
#define UTILS_H

#include <windows.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>

#define PI 3.14159265359f

// Konstanta Global
const float MAP_LIMIT = 100.0f;
const float RENDER_DISTANCE = 150.0f; 

// Partikel Asap
struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float life, maxLife;
    float size;
};

inline void drawString(float x, float y, void *font, const char *str) {
    glRasterPos2f(x, y);
    for (int i = 0; str[i]; i++) glutBitmapCharacter(font, str[i]);
}

inline void drawQuadFace(float w, float h, float nx, float ny, float nz) {
    glNormal3f(nx, ny, nz);
    glBegin(GL_QUADS);
        glVertex3f(-w/2, -h/2, 0); glVertex3f( w/2, -h/2, 0);
        glVertex3f( w/2,  h/2, 0); glVertex3f(-w/2,  h/2, 0);
    glEnd();
}

inline void drawBox(float w, float h, float d) {
    glPushMatrix(); glTranslatef(0,0,d/2); drawQuadFace(w,h, 0,0,1); glPopMatrix();
    glPushMatrix(); glTranslatef(0,0,-d/2); glRotatef(180,0,1,0); drawQuadFace(w,h, 0,0,-1); glPopMatrix();
    glPushMatrix(); glTranslatef(0,h/2,0); glRotatef(-90,1,0,0); drawQuadFace(w,d, 0,1,0); glPopMatrix();
    glPushMatrix(); glTranslatef(0,-h/2,0); glRotatef(90,1,0,0); drawQuadFace(w,d, 0,-1,0); glPopMatrix();
    glPushMatrix(); glTranslatef(-w/2,0,0); glRotatef(-90,0,1,0); drawQuadFace(d,h, -1,0,0); glPopMatrix();
    glPushMatrix(); glTranslatef(w/2,0,0); glRotatef(90,0,1,0); drawQuadFace(d,h, 1,0,0); glPopMatrix();
}

inline void buildShadowMatrix(GLfloat shadowMat[4][4], GLfloat groundPlane[4], GLfloat lightPos[4]) {
    GLfloat dot = groundPlane[0] * lightPos[0] + groundPlane[1] * lightPos[1] + groundPlane[2] * lightPos[2] + groundPlane[3] * lightPos[3];
    shadowMat[0][0] = dot - lightPos[0] * groundPlane[0]; shadowMat[1][0] = - lightPos[0] * groundPlane[1];
    shadowMat[2][0] = - lightPos[0] * groundPlane[2]; shadowMat[3][0] = - lightPos[0] * groundPlane[3];
    shadowMat[0][1] = - lightPos[1] * groundPlane[0]; shadowMat[1][1] = dot - lightPos[1] * groundPlane[1];
    shadowMat[2][1] = - lightPos[1] * groundPlane[2]; shadowMat[3][1] = - lightPos[1] * groundPlane[3];
    shadowMat[0][2] = - lightPos[2] * groundPlane[0]; shadowMat[1][2] = - lightPos[2] * groundPlane[1];
    shadowMat[2][2] = dot - lightPos[2] * groundPlane[2]; shadowMat[3][2] = - lightPos[2] * groundPlane[3];
    shadowMat[0][3] = - lightPos[3] * groundPlane[0]; shadowMat[1][3] = - lightPos[3] * groundPlane[1];
    shadowMat[2][3] = - lightPos[3] * groundPlane[2]; shadowMat[3][3] = dot - lightPos[3] * groundPlane[3];
}

#endif

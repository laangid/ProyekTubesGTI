#include <GL/glut.h>
#include "GameManager.h"

GameManager game;

void display() { game.render(); }
void reshape(int w, int h) { game.reshape(w, h); }
void keyboard(unsigned char key, int x, int y) { game.handleKeyDown(key); }
void keyboardUp(unsigned char key, int x, int y) { game.handleKeyUp(key); }
void mouse(int button, int state, int x, int y) { game.handleMouse(button, state, x, y); }
void motion(int x, int y) { game.handleMotion(x, y); }

void timerFunc(int v) {
    game.update();
    glutPostRedisplay();
    glutTimerFunc(16, timerFunc, 0); 
}

void initLighting() {
    // === KEMBALI KE SIANG HARI CERAH ===
    GLfloat sky[] = {0.52f, 0.68f, 0.78f, 1.0f}; 
    glClearColor(sky[0], sky[1], sky[2], sky[3]);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);

    // Cahaya Matahari Terang
    const GLfloat light_ambient[]  = { 0.4f, 0.4f, 0.4f, 1.0f };
    const GLfloat light_diffuse[]  = { 0.85f, 0.85f, 0.85f, 1.0f };
    const GLfloat light_specular[] = { 1.0f,  1.0f,  1.0f,  1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Pastikan lampu sorot malam dimatikan
    glDisable(GL_LIGHT1);

    // Efek Kabut Siang
    glEnable(GL_FOG);
    glFogfv(GL_FOG_COLOR, sky);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 30.0f);
    glFogf(GL_FOG_END,   120.0f); 
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 700);
    glutCreateWindow("Mobil-Mobilan");

    initLighting();

	game.loadTexture();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutTimerFunc(16, timerFunc, 0);

    glutMainLoop();
    return 0;
}

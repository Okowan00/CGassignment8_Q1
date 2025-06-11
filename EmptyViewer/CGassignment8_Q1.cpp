#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include "load_mesh.cpp"
#include "frame_timer.cpp"

float eye[3] = { 0, 0, 0 };
int nx = 1280, ny = 1280;

void set_viewport() {
    glViewport(0, 0, nx, ny);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void set_light() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    float global_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glShadeModel(GL_SMOOTH);

    float light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float light_position[] = { -1.0f, -1.0f, -1.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void draw_bunny_immediate() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < gTriangles.size(); ++i) {
        for (int j = 0; j < 3; ++j) {
            int idx = gTriangles[i].indices[j];
            Vector3 n = gNormals[idx];
            Vector3 p = gPositions[idx];
            float len = std::sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
            if (len > 0.00001f) {
                n.x /= len;
                n.y /= len;
                n.z /= len;
            }

            n.x = -n.x;
            n.y = -n.y;
            n.z = -n.z;

            glNormal3f(n.x, n.y, n.z);
            glVertex3f(p.x, p.y, p.z);
        }
    }
    glEnd();
}


void my_display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], 0, 0, -1, 0, 1, 0);
    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    start_timing();
    draw_bunny_immediate();
    float timeElapsed = stop_timing();

    gTotalFrames++;
    gTotalTimeElapsed += timeElapsed;
    float fps = gTotalFrames / gTotalTimeElapsed;
    char string[1024] = { 0 };
    sprintf(string, "OpenGL Bunny: %0.2f FPS", fps);
    glutSetWindowTitle(string);

    glutSwapBuffers();
    glutPostRedisplay();
}

void initialize() {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    set_viewport();
    set_light();
    load_mesh("bunny.obj");
    init_timer();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(nx, ny);
    glutCreateWindow("OpenGL Bunny");
    glewInit();
    initialize();
    glutDisplayFunc(my_display);
    glutMainLoop();
    return 0;
}

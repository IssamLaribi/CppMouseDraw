#include <GL/glut.h>
#include <vector>

struct Point {
    float x, y;
    float r, g, b; // store color for each point
};
std::vector<Point> points;

bool drawing = false;
bool erasing = false;

// current drawing color (default white)
float currR = 1.0f, currG = 1.0f, currB = 1.0f;

Point toOpenGLCoords(int x, int y) {
    Point p;
    p.x = (2.0f * x) / glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
    p.y = 1.0f - (2.0f * y) / glutGet(GLUT_WINDOW_HEIGHT);
    p.r = erasing ? 0.0f : currR;
    p.g = erasing ? 0.0f : currG;
    p.b = erasing ? 0.0f : currB;
    return p;
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            drawing = true;
            points.push_back(toOpenGLCoords(x, y));
        } else if (state == GLUT_UP) {
            drawing = false;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        points.clear(); // clear screen
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (drawing) {
        points.push_back(toOpenGLCoords(x, y));
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'r': case 'R': currR = 1.0f; currG = 0.0f; currB = 0.0f; erasing = false; break;
        case 'g': case 'G': currR = 0.0f; currG = 1.0f; currB = 0.0f; erasing = false; break;
        case 'b': case 'B': currR = 0.0f; currG = 0.0f; currB = 1.0f; erasing = false; break;
        case 'w': case 'W': currR = 1.0f; currG = 1.0f; currB = 1.0f; erasing = false; break;
        case 'e': case 'E': erasing = true; break; // eraser (black)
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (auto &p : points) {
        glColor3f(p.r, p.g, p.b);
        glVertex2f(p.x, p.y);
    }
    glEnd();

    glutSwapBuffers();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Mini Paint - C++ OpenGL");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}

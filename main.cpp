#include <GL/glut.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include <GL/glui.h>


#include "GWindow.h"
#include <iostream>
using namespace std;

#define P_RELEASE
#ifdef P_RELEASE
int main(int argc,char *argv[])
{
    GWindow::display_init(argc,argv);
    GWindow::create_menu();
    glutMainLoop();
    return 0;
}
#endif


//
// Created by liududu on 18-10-29.
//

#include <GL/gl.h>
#include <cmath>
#include "BezierCurve.h"
#include "common/values.h"
//#include "common/helper.h"
unsigned int fact(unsigned int n);
unsigned int C(int m, int n);

void BezierCurve::draw() {
    Point *p;
    if (control_points.empty()) return ;
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) {
        p = drawPoint(1.0/n*i);
        glVertex2d(p->getX(),p->getY());
        delete p;
    }
    glEnd();
}

Point* BezierCurve::drawPoint(double t) {
    auto cnt = control_points.size();
    Point* r = new Point(0,0);
    if (cnt == 0) return NULL;
    for(int i = 0; i < cnt; i++) {
        *r =  control_points[i] *C(i,cnt - 1) * (pow(1-t,cnt-1 - i)*pow(t,i)) + *r;
    }
    return r;
}




void BezierCurve::display() {
    glColor3f(1.0,1.0,0);
    if (control_line_visible)
        draw_control_line();
    glColor3fv(curve_focus_color);
    draw();
}
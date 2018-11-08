//
// Created by liududu on 18-10-21.
//

#include <GL/gl.h>
#include <cmath>
#include "Curve.h"
#include "common/helper.h"
#include "common/values.h"

double Point::getX() const {
    return x;
}

void Point::setX(double x) {
    Point::x = x;
}

double Point::getY() const {
    return y;
}

void Point::setY(double y) {
    Point::y = y;
}

Point::Point(double x, double y) : x(x), y(y) {}

Point::Point() {
    x = 0;
    y = 0;
}

Point Point::operator+(Point &p) {
    Point r(this->x + p.x, this->y + p.y );
    return r;
}

Point Point::operator-(Point &p) {
    Point r(this->x - p.x, this->y - p.y);
    return r;
}

Point Point::operator*(double p) {
    Point r(this->x * p, this->y * p);
    return r;
}

Point operator + (Point& p1, Point& p2) {
    Point r(p1.getX() + p2.getX(),p1.getY() + p2.getY());
    return r;
}

void Point::draw() {
    glBegin(GL_POINTS);
    glVertex2d(x,y);
    glEnd();
}

void Point::display() {
    glColor3fv(color);
    draw();
}

bool Point::touch(Point p) {
    return  abs( p.x - x) < size && abs(p.y - y) < size;
}

int Point::getSize() const {
    return size;
}

void Point::setSize(int size) {
    Point::size = size;
}

const float *Point::getColor() const {
    return color;
}

int Shape::getFocus() const {
    return focus;
}

void Shape::setFocus(int focus) {
    Shape::focus = focus;
}

void Curve::setFocus() {
    Curve::focus = control_points.size() - 1;
}

vector<Curve> Curve::read_curve(string file_name) {
    vector<Curve> curves;
    FILE* fp;
    fp = fopen(file_name.c_str(),"r");
    return curves;
}

void Curve::append_control_point(Point& p){
    control_points.push_back(p);
}

void Curve::rm_control_point(int index) {
    control_points.erase(control_points.begin() + index);
}

void Curve::insert_control_point(Point &p) {
    if (focus == -1){
        return;
    }
    auto it =control_points.insert(control_points.begin() + focus+1,p);
    this->Shape::setFocus(it - control_points.begin());
}


int Curve::find_touch_point(Point& p) {
    int index = -1;
    for(int i = 0; i < control_points.size(); i++) {
        if (p.touch(control_points[i])){
            index = i;
            break;
        }
    }
    return index;
}

void Curve::ch_control_point(int index,Point &p){
    control_points[index] = p;
}





void Curve::write_curve(ostream& ost) {
    return;
}




int Curve::getN() const {
    return n;
}

void Curve::setN(int n) {
    Curve::n = n;
}

unsigned int Curve::getPnum() {
    return control_points.size();
}

void Curve::setControl_line_visible(bool control_line_visible) {
    Curve::control_line_visible = control_line_visible;
}

void Curve::draw_control_line() {
    glColor3fv(control_line_color);
    glBegin(GL_LINE_STRIP);
    for(auto it = control_points.begin(); it != control_points.end(); it++) {
        glVertex2d((*it).getX(),(*it).getY());
    }
    glEnd();
    glPointSize(control_points[0].getSize());
    glBegin(GL_POINTS);
    for(auto it = control_points.begin(); it != control_points.end(); it++) {
        glVertex2d((*it).getX(),(*it).getY());
    }
    glEnd();
}

Display_mode Shape::getDisplay_state() const {
    return display_state;
}

void Shape::setDisplay_state(Display_mode display_state) {
    Shape::display_state = display_state;
}

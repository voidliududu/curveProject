//
// Created by liududu on 18-10-25.
//

#include "BsplineCurve.h"
#include "common/values.h"
#include <GL/gl.h>
#include <iostream>
#include <fstream>

//初始化T
void BsplineCurve::init_T(int k) {
    T.clear();
    double num = control_points.size() + k + 0.0;
    for(int i = 0; i <= num; i++) {
        T.push_back(i / num );
    }
}

void BsplineCurve::put_T(double t) {
    T.push_back(t);
}


//计算N
double BsplineCurve::N(int i, int k, double t) {
    if (k == 0) {
        return t - T[i] >= 0 && t - T[i+1] < 0 ? 1:0;
    }
    double R1,R2,temp;
    temp = T[i+k] - T[i];
    R1 = temp == 0 ? 0 : (t - T[i])*N(i,k-1,t) / temp;
    temp = T[i+k+1] - T[i+1];
    R2 = temp == 0 ? 0  : (T[i+k+1] - t) * N(i+1,k-1,t) /temp;
    return R1 + R2;
}

//计算曲线上点
Point* BsplineCurve::drawPoint(double t) {
    Point* p = new Point;
    for(int i = 0; i < control_points.size(); i++) {
        *p = control_points[i] * N(i,k,t) + *p;
    }
    return p;
}

void BsplineCurve::append_control_point(Point &p) {
    cout<<"append called\n";
    Curve::append_control_point(p);
    cout<<"control_point num" <<control_points.size()<<endl;
    init_T(k);
    cout<<"T "<<T.size()<<endl;
}

void BsplineCurve::insert_control_point(Point &p) {
    if (focus == -1){
        return;
    }
    auto it =control_points.insert(control_points.begin() + focus+1,p);
    this->Shape::setFocus(it - control_points.begin());
    init_T(k);
}

void BsplineCurve::rm_control_point(int index) {
    Curve::rm_control_point(index);
    init_T(k);
}

void BsplineCurve::display() {
    if (control_line_visible)
        draw_control_line();
    if (display_state == FOCUS) {
        glColor3fv(curve_focus_color);
    } else if (display_state == DISPLAY) {
        glColor3fv(curve_display_color);
    }
    draw();
}
//绘图
void BsplineCurve::draw() {
    Point *p;
    if (control_points.size() >= k ){
        double domain = T[control_points.size()] - T[k];
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < n; i++) {
            p = drawPoint(T[k] + domain/n*i);
            glVertex2d(p->getX(),p->getY());
            delete p;
        }
        glEnd();
    }
}

vector<BsplineCurve*> BsplineCurve::read_curve(string filename) {
    cout<<filename<<endl;
    vector<BsplineCurve*> v;
    int n,k,t_num;
    double x,y,t;
    Point p;
    ifstream fs(filename);
    if (!fs) {
        return v;
    }
    while (!fs.eof()) {
        fs>>k>>n;
        if (!fs) {
            fs.close();
            return v;
        }
        t_num = n + k + 1;
        BsplineCurve* cur = new BsplineCurve(k);
        for (int i = 0; i < t_num; i++) {
            fs>>t;
            cur->put_T(t);
        }
        for (int i = 0; i< n; i++) {
            fs>>x>>y;
            p.setX(x);
            p.setY(y);
            cur->Curve::append_control_point(p);
        }
        v.push_back(cur);
    }
    fs.close();
    cout<<"read "<<v.size()<<endl;
    return v;
}

void BsplineCurve::write_curve(ostream& ost) {
    if (!ost) return;
    ost<<this->getK()<<endl;
    ost<<this->getPnum()<<endl;
    for (auto it = this->getT().begin(); it < this->getT().end(); it ++) {
        ost<<(*it)<<" ";
    }
    ost<<endl;
    for (auto it = this->control_points.begin(); it < this->control_points.end(); it ++) {
        ost<<(*it).getX()<<" "<<(*it).getY()<<endl;
    }
}

int BsplineCurve::getK() const {
    return k;
}

void BsplineCurve::setK(int k) {
    this->k = k;
    init_T(k);
}

const vector<double> &BsplineCurve::getT() const {
    return T;
}

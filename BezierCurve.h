//
// Created by liududu on 18-10-29.
//

#ifndef CURVEPROJECT_BEZIERCURVE_H
#define CURVEPROJECT_BEZIERCURVE_H


#include "Curve.h"

class BezierCurve : public Curve{
    //绘制曲线
    void draw();
    //显示图案
    virtual void display();
    //参数描点
    Point* drawPoint(double t);

};


#endif //CURVEPROJECT_BEZIERCURVE_H

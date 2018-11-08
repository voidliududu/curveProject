//
// Created by liududu on 18-10-25.
//

#ifndef CURVEPROJECT_BSPLINECURVE_H
#define CURVEPROJECT_BSPLINECURVE_H


#include "Curve.h"

class BsplineCurve : public Curve{
private:
    int k;
    //节点向量
    vector<double> T;
    // xxx系数
    double N(int i,int k,double t);
public:
    BsplineCurve(int k = 2) {
        this->k = k;
    }
    static vector<BsplineCurve*> read_curve(string filename);
    void write_curve(ostream& ost);
    virtual ~BsplineCurve(){};
    //均匀初始化节点矢量
    void init_T(int k);
    void put_T(double t);

    int getK() const;

    const vector<double> &getT() const;

    // 计算曲线上点
    Point* drawPoint(double t);
    //绘图
    void draw();
    //更新k值
    void setK(int k);
    //显示
    virtual void display();
    virtual void append_control_point(Point &p);
    virtual void insert_control_point(Point &p);
    virtual void rm_control_point(int index);

};


#endif //CURVEPROJECT_BSPLINECURVE_H

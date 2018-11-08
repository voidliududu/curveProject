//
// Created by liududu on 18-10-21.
//

#ifndef CURVEPROJECT_CURVE_H
#define CURVEPROJECT_CURVE_H

#include <vector>
#include <string>

using namespace std;

enum Display_mode{HIDE,DISPLAY,FOCUS};

class Shape{
public:
    Display_mode display_state = DISPLAY;

    //焦点索引
    int focus = -1;

    int getFocus() const;

    void setFocus(int focus);


    Display_mode getDisplay_state() const;

    void setDisplay_state(Display_mode display_state);

    virtual void display() = 0;
    virtual ~Shape() = default;
};

class Point:public Shape{
private:
    //点坐标
    double x;
    double y;
    //点尺寸
    int size = 5;
    // 点颜色
    float color[3] = {0};

public:
    Point();

    Point(double x, double y);

    virtual ~Point() = default;

    double getX() const;

    void setX(double x);

    double getY() const;
    //  判断重合
    bool touch(Point p);

    void setY(double y);

    int getSize() const;

    void setSize(int size);

    const float *getColor() const;

    //绘图函数
    void draw();
    //显示函数
    virtual void display();

    Point operator - (Point& p);
    Point operator + (Point& p);
    Point operator * (double p);
};

class Curve :public Shape{
protected:
    //曲线控制点集
    vector<Point> control_points;
    //曲线颜色
    float curve_color[3] = {0};
    //控制线颜色
    float line_color[3] = {0};
    //插值密度
    int n = 100;
    //控制多边形可见性
    bool control_line_visible = true;

public:
    Curve() {
    }

    virtual ~Curve() = default;

    void setFocus();

    int getN() const;

    void setN(int n);

    void setControl_line_visible(bool control_line_visible);

    unsigned int getPnum();

    static vector<Curve> read_curve(string file_name);

    virtual void write_curve(ostream& ost);
    //添加控制点
    virtual void append_control_point(Point& p);
    //插入控制点
    virtual void insert_control_point(Point& p);
    //删除控制点
    virtual void rm_control_point(int index);
    // 修改控制点
    virtual void ch_control_point(int index,Point &p);
    //画控制线
    void draw_control_line();
    //显示图案
    virtual void display() = 0;
    //查找点
    int find_touch_point(Point& p);

};


#endif //CURVEPROJECT_CURVE_H

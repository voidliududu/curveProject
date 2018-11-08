//
// Created by liududu on 18-10-20.
//

#ifndef CURVEPROJECT_GWINDOW_H
#define CURVEPROJECT_GWINDOW_H


#include <string>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include "common/values.h"
#include <vector>
#include "Curve.h"

using namespace std;

class GWindow {
    vector<Curve*> display_obj;
public:
    static GWindow* gWindow;
    static int window_width;
    static int window_height;
    static int window_id;
    static std::string window_name;

    //鼠标指针坐标
    static int cursor_x;
    static int cursor_y;

    //鼠标按键标志
    int pressed_mouse_button = -1;

    //窗口编辑模式
    int edit_mode = EDIT_NORMAL;

    static int global_n;

    static int focus_k;

    static GLUI_FileBrowser* fb;

    static bool show_control_point;

    static string file4write;

    static int focus_shape;
    //获取实例
    static GWindow* get_instance();
    //窗口初始化
    static void display_init(int argc,char *argv[]);
    //窗口空闲
    static void idle();
    //窗口形变
    static void reshape(int x, int y);
    //键盘事件
    static void keyboard();
    //鼠标事件
    static void mouse_click(int button,int state,int x,int y);
    //鼠标移动
    static void mouse_motion(int x, int y);
    //菜单
    static void create_menu();
    //绘图
    static void display();

    //添加对象
    int  add_obj(Curve* shape);
    //删除对象
    void rm_obj(Curve* shape);

    void clear_obj();
    //获取对象列表
    vector<Curve*>& get_display_object();

    bool refresh_focus(Point& p);

    static void handle_normal_mode(int button,int x,int y);
    static void handle_bezier_mode(int x,int y);
    static void handle_bspline_mode(int x, int y);

};

#endif //CURVEPROJECT_GWINDOW_H

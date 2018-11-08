//
// Created by liududu on 18-10-20.
//

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glui.h>

#include "GWindow.h"
#include "Curve.h"
#include "BsplineCurve.h"
#include "BezierCurve.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

std::string GWindow::window_name = "hello world";
int GWindow::window_height = 600;
int GWindow::window_width = 800;
int GWindow::window_id = 0;
GWindow* GWindow::gWindow = NULL;

int GWindow::cursor_x = 0;
int GWindow::cursor_y = 0;

//Shape* GWindow::focus_shape = new Curve();
int GWindow::focus_shape = -1;

int GWindow::global_n = 100;

int GWindow::focus_k = 2;

GLUI_FileBrowser* GWindow::fb = NULL;
bool GWindow::show_control_point = true;

string GWindow::file4write = "";

// 交互控制回调函数
void glui_control(int control);


GWindow* GWindow::get_instance() {
    if (gWindow == NULL){
        gWindow = new GWindow();
        return gWindow;
    } else{
        return gWindow;
    }
}


//初始化窗口
void GWindow::display_init(int argc,char *argv[]){
    get_instance();
    glutInit(&argc,argv);
    glutInitDisplayMode( GLUT_RGB);
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize(window_width, window_height);

    window_id = glutCreateWindow(window_name.c_str());

    glClearColor(0, 0, 0, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluOrtho2D(0.0, window_width, 0.0, window_height);

    //fixme set_glutDisplayFunc
    GLUI_Master.set_glutDisplayFunc(display);
    GLUI_Master.set_glutIdleFunc(idle);
    GLUI_Master.set_glutReshapeFunc(reshape);
    GLUI_Master.set_glutMouseFunc(mouse_click);
    glutMotionFunc(mouse_motion);


}
//窗口空闲
void GWindow::idle(){
    if (glutGetWindow() != window_id)
        glutSetWindow(window_id);
    glutPostRedisplay();
}
//窗口形变
void GWindow::reshape(int x, int y){
//    float xy_aspect;
//    xy_aspect = (float)x / (float)y;
//    glViewport( 0, 0, x, y );
//    glMatrixMode( GL_PROJECTION );
//    glLoadIdentity();
//    glFrustum( -xy_aspect*.08, xy_aspect*.08, -.08, .08, .1, 15.0 );
//    glutPostRedisplay();

    window_width = x;
    window_height = y;
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluOrtho2D(0.0, window_width, 0.0, window_height);
}

//菜单
void GWindow::create_menu(){
    GLUI *glui = GLUI_Master.create_glui_subwindow(window_id,GLUI_SUBWINDOW_RIGHT);
    GLUI_Panel* file_panel = new GLUI_Panel(glui,"file",1);
    fb = new GLUI_FileBrowser(file_panel,"fileBrowser",1,CTL_FILE_SELECT,glui_control);
    fb->fbreaddir("./save");
    fb->set_allow_change_dir(true);
    new GLUI_EditText(file_panel,"file name",GWindow::file4write,CTL_FN_CHANGE,glui_control);
    auto bpanel = new GLUI_Panel(glui,"",1);
    new GLUI_Button(bpanel,"open",CTL_FILE_OPEN,glui_control);
    new GLUI_Column(bpanel,false);
    new GLUI_Button(bpanel,"save",CTL_FILE_WRITE,glui_control);
    GLUI_Panel* curve_panel = new GLUI_Panel(glui,"curve",1);
    new GLUI_Button(curve_panel,"Bezier",CTL_CREATE_BESIZER,glui_control);
    new GLUI_Button(curve_panel,"Bspline",CTL_CREATE_BSPLINE,glui_control);
    new GLUI_Button(curve_panel,"toggle",CTL_TOGGLE_CTLP,glui_control);
    new GLUI_Button(curve_panel,"clear",CTL_CLEAR_SCREEN,glui_control);
    new GLUI_Spinner(curve_panel,"n",GLUI_SPINNER_INT, &global_n,CTL_N_SPINNER_CHANGE,glui_control);
    new GLUI_Spinner(curve_panel,"k",&focus_k, CTL_K_VALUE,glui_control);
    glui->set_main_gfx_window(window_id);
}
//绘图
void GWindow::display(){
    glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT );
//    Curve* curve = new Curve;
//    GWindow::get_instance()->add_obj(curve);
    if (gWindow->display_obj.empty()) {
        glFlush();
        return;
    }
    for(auto it =gWindow->display_obj.begin(); it != gWindow->display_obj.end();it ++) {
        (*it) -> display();
    }
    glFlush();
}

//添加形状
int GWindow::add_obj(Curve *curve) {
    display_obj.push_back(curve);
    return display_obj.size() - 1;
}

//删除形状
void GWindow::rm_obj(Curve *shape) {
    auto it = display_obj.begin();
    while (it != display_obj.end()){
        if (*it == shape) {
            display_obj.erase(it);
            break;
        }
    }
    delete(shape);
}
void GWindow::clear_obj() {
    display_obj.clear();
    focus_shape = -1;
}

//获取对象列表
vector<Curve*>& GWindow::get_display_object(){
    return display_obj;
}

//事件处理

/*
 * 如果鼠标点击处有点，则使用拖动处理
 * 如果没点，则添加点
 *
 * */

/*下面是一揽子事件处理逻辑
 * 鼠标左键添加、移动控制点，中间选择控制点，右键删除控制点
 *
 * 首先判断当前模式（创建Bezier模式、创建Bspline模式、普通模式）
 *
 * */

void GWindow::mouse_click(int button, int state, int x, int y) {

    if (state != GLUT_DOWN) return;
    switch (GWindow::get_instance()->edit_mode){
        case EDIT_CREATE_BESIZER:
            handle_bezier_mode(x,y);
            break;
        case EDIT_CREATE_BSPLINE:
            handle_bspline_mode(x,y);
            break;
        case EDIT_NORMAL:
            handle_normal_mode(button,x,y);
            break;
        default:
            break;
    }
}

/*
 * 左键 能找到焦点就移动，找不到就插入（找到焦点就插入，找不到就追加）
 * 右键 能找到焦点就删除，焦点置-1。。。找不到不做处理
 * 中间 能找到焦点就设置焦点，找不到就不做处理
 * */
void GWindow::handle_normal_mode(int button,int x,int y) {
    Point p(x,GWindow::window_height -  y);
    switch (button){
        case GLUT_LEFT_BUTTON:
            if (!GWindow::get_instance()->refresh_focus(p)){
                GWindow* gWindow = GWindow::get_instance();
                auto display_obj = gWindow->get_display_object();
                //曲线有焦点
                if (focus_shape != -1) {
                    Curve* focus_shape =  display_obj[GWindow::focus_shape];
                    focus_shape->insert_control_point(p);

                }
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (GWindow::get_instance()->refresh_focus(p)){
                GWindow* gWindow = GWindow::get_instance();
                auto display_obj = gWindow->get_display_object();
                //曲线有焦点
                if (focus_shape != -1) {
                    Curve* focus_shape =  display_obj[GWindow::focus_shape];
                    if(focus_shape->getFocus() != -1) {
                        focus_shape->rm_control_point(focus_shape->getFocus());
                    }

                }
            }
            break;
        case GLUT_MIDDLE_BUTTON:
            break;
        default:
            break;
    }
}

void GWindow::handle_bezier_mode(int x, int y) {
    Point p(x,GWindow::window_height -  y);
    focus_shape = GWindow::get_instance()->add_obj(new BezierCurve);
    GWindow::get_instance()->display_obj[focus_shape]->append_control_point(p);
    GWindow::get_instance()->display_obj[focus_shape]->setFocus();
    GWindow::get_instance()-> edit_mode = EDIT_NORMAL;
}

void GWindow::handle_bspline_mode(int x,int y){
    Point p(x,GWindow::window_height -  y);
    focus_shape = GWindow::get_instance()->add_obj(new BsplineCurve);
    GWindow::get_instance()->display_obj[focus_shape]->append_control_point(p);
    GWindow::get_instance()->display_obj[focus_shape]->setFocus();
    GWindow::get_instance()-> edit_mode = EDIT_NORMAL;
}

bool GWindow::refresh_focus(Point &p) {
    //焦点可能不存在
   Curve* focus_obj ;
    int cur_focus_index ;
    int obj_num = display_obj.size();
    if (focus_shape == -1) {
        for(int i = 0; i < obj_num;i++) {
            cur_focus_index = display_obj[i]->find_touch_point(p);
            if (cur_focus_index != -1) {
                display_obj[i]->Shape::setFocus(cur_focus_index);
                GWindow::focus_shape = i;
                return true;
            }
        }
        return false;
    }else{
        focus_obj = display_obj[focus_shape];
        cur_focus_index = focus_obj->find_touch_point(p);
        if (cur_focus_index != -1) {
            focus_obj->Shape::setFocus(cur_focus_index);
            return true;
        }else{
            for(int i = 0; i < obj_num;i++) {
                if (i == GWindow::focus_shape) continue;
                cur_focus_index = display_obj[i]->find_touch_point(p);
                if (cur_focus_index != -1) {
                    display_obj[i]->Shape::setFocus(cur_focus_index);
                    GWindow::focus_shape = i;
                    return true;
                }
            }
            return false;
        }
    }
}

void GWindow::mouse_motion(int x, int y) {
     Curve* t = GWindow::get_instance()->get_display_object()[focus_shape];
     Point p(x,GWindow::window_height -  y);
     if (t->getFocus() != -1) {
         t->ch_control_point(t->getFocus(),p);
     }
}


void glui_control(int control) {
    auto gWindow = GWindow::get_instance();
    auto obj = gWindow->get_display_object();
    string filename;
    ofstream ost;
    switch (control) {
        case CTL_CREATE_BESIZER:
            GWindow::get_instance()->edit_mode = EDIT_CREATE_BESIZER;
            cout<<"edit besizer"<<endl;
            break;
        case CTL_CREATE_BSPLINE:
            GWindow::get_instance()->edit_mode = EDIT_CREATE_BSPLINE;
            cout<<"edit bspline"<<endl;
            break;
        case CTL_N_SPINNER_CHANGE:
            for (int i = 0; i< obj.size(); i++) {
                obj[i]->setN(gWindow->global_n);
            }
            break;
        case CTL_FILE_OPEN:
            filename = GWindow::fb->get_file();
            if (!filename.empty()) {
                gWindow->clear_obj();
                auto obj = BsplineCurve::read_curve( +"./save/" + filename);
                for(auto it = obj.begin(); it != obj.end();it++) {
                    gWindow->add_obj(*it);
                }
            }
            break;
        case CTL_FILE_WRITE:
            cout <<gWindow->file4write<<endl;
            ost.open("./save/" + gWindow->file4write);
            if (!ost) return;
            for(auto it = obj.begin(); it != obj.end(); it ++) {
                (*it)->write_curve(ost);
            }
            ost.close();
            gWindow->fb->fbreaddir("./save");
            break;
        case CTL_TOGGLE_CTLP:
            GWindow::show_control_point = ! GWindow::show_control_point;
            for (auto it = obj.begin(); it != obj.end(); it ++) {
                (*it)->setControl_line_visible(GWindow::show_control_point);
            }
            break;
        case CTL_K_VALUE:
            if (gWindow->focus_shape != -1) {
                //fixme 判断类型
                ((BsplineCurve*)obj[gWindow->focus_shape]) -> setK(GWindow::focus_k);
            }
            break;
        case CTL_CLEAR_SCREEN:
//            for (auto it = obj.begin(); it != obj.end(); it ++) {
//                delete (*it);
//            }
            cout<<"clear"<<endl;
            gWindow->clear_obj();
            break;
        default:
            break;
    }
}


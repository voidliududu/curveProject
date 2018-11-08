cmake_minimum_required(VERSION 3.12)
project(CurveProject)

set(CMAKE_CXX_STANDARD 11)

find_package(GLUT)
find_package(OpenGL)

add_executable(Curve

#        test.h
#        test.cpp
        main.cpp
        GWindow.h
        GWindow.cpp
        Curve.h
        Curve.cpp
        common/helper.h BsplineCurve.cpp BsplineCurve.h common/values.h BezierCurve.cpp BezierCurve.h)

add_executable(Test
        test.cpp)

target_link_libraries(Curve
        ${GLUT_LIBRARIES}
        ${OPENGL_LIBRARY}
        libglui.so)

target_link_libraries(Test
        ${GLUT_LIBRARIES}
        ${OPENGL_LIBRARY}
        libglui.so)
#include "map_widget.h"

map_widget::map_widget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void map_widget::initializeGL()
{
    // This function gets called before rendering starts. Any initialization of
    // the OpenGL context should be done here.
}

void map_widget::paintGL()
{
    // This function implements the main rendering logic.
}

void map_widget::resizeGL(int w, int h)
{
    // This function is called whenever the widget is resized, including on
    // startup.

    Q_UNUSED(w);
    Q_UNUSED(h);
}

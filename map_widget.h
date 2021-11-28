#ifndef MAP_WIDGET_H
#define MAP_WIDGET_H

#include <QOpenGLWidget>

class map_widget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit map_widget(QWidget *parent = nullptr);
};

#endif // MAP_WIDGET_H

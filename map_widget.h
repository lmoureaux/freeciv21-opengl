#ifndef MAP_WIDGET_H
#define MAP_WIDGET_H

#include <QOpenGLWidget>

class map_widget : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit map_widget(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

#endif // MAP_WIDGET_H

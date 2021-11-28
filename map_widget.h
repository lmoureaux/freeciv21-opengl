#ifndef MAP_WIDGET_H
#define MAP_WIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

class sprite_provider;

class map_widget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit map_widget(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    sprite_provider *m_provider;
};

#endif // MAP_WIDGET_H

#ifndef MAP_WIDGET_H
#define MAP_WIDGET_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

#include <memory>
#include <vector>

class sprite_provider;

class map_widget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit map_widget(QWidget *parent = nullptr);
    virtual ~map_widget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private:
    sprite_provider *m_provider;

    QMatrix4x4 m_projection_matrix;
    std::unique_ptr<QOpenGLVertexArrayObject> m_vao;
    std::unique_ptr<QOpenGLBuffer> m_xy_vbo, m_uv_vbo;
    QOpenGLShaderProgram *m_program = nullptr;
    std::vector<QOpenGLTexture *> m_textures;
};

#endif // MAP_WIDGET_H

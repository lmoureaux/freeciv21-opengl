#ifndef MAP_WIDGET_H
#define MAP_WIDGET_H

#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QPropertyAnimation>

#include <memory>
#include <vector>

class sprite_provider;

class map_widget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
    Q_PROPERTY(QPointF origin READ origin WRITE setOrigin)

public:
    explicit map_widget(QWidget *parent = nullptr);
    virtual ~map_widget();

    QPointF origin() const { return m_origin; }
    void setOrigin(const QPointF &origin, bool animate = false);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    sprite_provider *m_provider;

    QMatrix4x4 m_projection_matrix;
    std::unique_ptr<QOpenGLVertexArrayObject> m_vao;
    std::unique_ptr<QOpenGLBuffer> m_xy_vbo, m_offset_vbo,
                                   m_uv_vbo, m_sprite_index_vbo;
    QOpenGLShaderProgram *m_program = nullptr;
    std::unique_ptr<QOpenGLTexture> m_atlas;

    QPointF m_origin;
    std::unique_ptr<QPropertyAnimation> m_center_animation;
};

#endif // MAP_WIDGET_H

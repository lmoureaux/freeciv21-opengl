#include "sprite_provider.h"

#include "map_widget.h"

#include <QOpenGLBuffer>

map_widget::map_widget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_provider(new sprite_provider(this))
{
    // Identity matrix is a good start
    m_projection_matrix.setToIdentity();
}

map_widget::~map_widget()
{
    makeCurrent();

    // Destroy everything
    delete m_program;
    for (auto tex: m_textures) {
        delete tex;
    }
    m_vao = nullptr;
    m_xy_vbo = nullptr;
    m_uv_vbo = nullptr;

    doneCurrent();
}

void map_widget::initializeGL()
{
    // This function gets called before rendering starts. Any initialization of
    // the OpenGL context should be done here.
    initializeOpenGLFunctions();

    // Create an OpenGL program, add our shaders and link
    m_program = new QOpenGLShaderProgram(this);
    if (!m_program->addShaderFromSourceFile(
                QOpenGLShader::Vertex, "shaders/vertex.vert")) {
        qCritical() << m_program->log();
        qFatal("Could not compile vertex shader");
    }
    if (!m_program->addShaderFromSourceFile(
                QOpenGLShader::Fragment, "shaders/fragment.frag")) {
        qCritical() << m_program->log();
        qFatal("Could not compile fragment shader");
    }
    // Bind attributes
    m_program->bindAttributeLocation("xy", 0);
    m_program->bindAttributeLocation("uv", 1);

    if (!m_program->link()) {
        qCritical() << m_program->log();
        qFatal("Could not link GL program");
    }

    // Bind the program so every other call below uses it
    // (especially the VAO creation)
    m_program->bind();

    // Create textures
    for (const auto &icon: m_provider->sprites()) {
        // Create one 2D texture per sprite
        auto tex = new QOpenGLTexture(QOpenGLTexture::Target2D);
        // Set the data. The size is hard coded for simplicity...
        tex->setData(icon.pixmap(96, 48).toImage());
        // Save it for later
        m_textures.push_back(tex);
    }

    // Build the VAO, which records which memory buffers are used where
    m_vao.reset(new QOpenGLVertexArrayObject);
    QOpenGLVertexArrayObject::Binder vao_binder(m_vao.get());

    // First buffer: xy coordinates
    static const GLfloat xy_data[] = {
        // Triangle 1
         0.0f,  0.0f,
        96.0f,  0.0f,
        96.0f, 48.0f,
        // Triangle 2
         0.0f,  0.0f,
        96.0f, 48.0f,
         0.0f, 48.0f,
    };

    // Send to the GPU
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, xy_data);
    // This will be the attribute at location 0
    glEnableVertexAttribArray(0);

    // Second buffer: uv coordinates (coordinates within the sprite)
    static GLfloat uv_data[] = {
        // Triangle 1
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        // Triangle 2
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };
    // This will be the attribute at location 1
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, uv_data);
    glEnableVertexAttribArray(1);

    // Enable alpha blending (otherwise we get into big trouble)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void map_widget::paintGL()
{
    // This function implements the main rendering logic.

    // Make the VAO active. This recalls the buffers we've set in initializeGL
    QOpenGLVertexArrayObject::Binder vao_binder(m_vao.get());

    // Make the program current
    m_program->bind();

    // Render the map
    const auto sprites = m_provider->render();
    for (std::size_t i = 0; i < sprites.size(); ++i) {
        // Set uniforms that won't change per sprite
        // Should change it to reflect sprites[i].x and .y
        auto matrix = m_projection_matrix;
        matrix.translate(sprites[i].x, sprites[i].y);
        m_program->setUniformValue("projection_matrix", matrix);

        // Make the texture active
        // It has texture ID 0 because it's the first... I think
        auto &tex = m_textures[sprites[i].sprite_index];
        tex->bind();
        m_program->setUniformValue("sprite", 0);

        // This does the actual drawing...
        glDrawArrays(GL_TRIANGLES,  // ...of triangles
                     0,             // ...starting from index 0 of the buffers
                     2 * 3);        // ...and using 6 vertices (two triangles)

        // Disable the texture
        tex->release();
    }

    m_program->release();
}

void map_widget::resizeGL(int w, int h)
{
    // This function is called whenever the widget is resized, including on
    // startup.

    // The OpenGL coordinate system has (-1, -1) at the bottom left and
    // (1, 1) at the top right. Change this to be pixel coordinates with
    // (0, 0) at the bottom left and whatever window width and height at
    // the top right.
    auto trans = QTransform();
    trans.translate(-1, -1);
    trans.scale(2.0 / w, 2.0 / h);
    m_projection_matrix = trans;
}

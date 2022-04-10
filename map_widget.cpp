#include "sprite_provider.h"

#include "map_widget.h"

#include <QOpenGLBuffer>
#include <QOpenGLPixelTransferOptions>

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
    m_atlas = nullptr;
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

    // Create the texture atlas
    auto icons = m_provider->sprites();
    m_atlas = std::make_unique<QOpenGLTexture>(QOpenGLTexture::Target2DArray);
    m_atlas->setFormat(QOpenGLTexture::RGBA8_UNorm);
    m_atlas->setSize(96, 48); // Size hard coded for simplicity
    m_atlas->setLayers(icons.size());
    m_atlas->allocateStorage();
    for (std::size_t i = 0; i < icons.size(); ++i) {
        auto image = icons[i].pixmap(96, 48).toImage().convertToFormat(QImage::Format_RGBA8888);
        QOpenGLPixelTransferOptions options;
        options.setAlignment(1);
        m_atlas->setData(0, i, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, image.constBits(), &options);
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

    // Create a Vertex Buffer Object (VBO) to move all this to the GPU
    m_xy_vbo.reset(new QOpenGLBuffer);
    m_xy_vbo->create();
    m_xy_vbo->bind(); // Make it the current VBO
    // Send to the GPU
    m_xy_vbo->allocate(xy_data, sizeof(xy_data));

    // This will be the attribute at location 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
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
    m_uv_vbo.reset(new QOpenGLBuffer);
    m_uv_vbo->create();
    m_uv_vbo->bind(); // Make it the current VBO
    // Send to the GPU
    m_uv_vbo->allocate(uv_data, sizeof(uv_data));

    // This will be the attribute at location 1
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
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

    // Make the atlas active
    // It has texture ID 0 because it's the first... I think
    m_atlas->bind();
    m_program->setUniformValue("atlas", 0);

    // Set uniforms that won't change per sprite
    auto matrix = m_projection_matrix;
    m_program->setUniformValue("projection_matrix", matrix);

    // Render the map
    const auto sprites = m_provider->render();
    for (std::size_t i = 0; i < sprites.size(); ++i) {
        // Should change it to reflect sprites[i].x and .y
        m_program->setUniformValue("offset", QVector2D(sprites[i].x, sprites[i].y));
        m_program->setUniformValue("index", static_cast<GLint>(sprites[i].sprite_index));

        // This does the actual drawing...
        glDrawArrays(GL_TRIANGLES,  // ...of triangles
                     0,             // ...starting from index 0 of the buffers
                     2 * 3);        // ...and using 6 vertices (two triangles)
    }

    // Disable our stuff
    m_atlas->release();
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

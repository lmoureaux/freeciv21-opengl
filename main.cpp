#include "mainwindow.h"

#include <QApplication>
#include <QOpenGLContext>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*
     * Copyright (C) 2016 The Qt Company Ltd.
     * Contact: https://www.qt.io/licensing/
     *
     * BSD License Usage
     * Alternatively, you may use this file under the terms of the BSD license
     * as follows:
     *
     * "Redistribution and use in source and binary forms, with or without
     * modification, are permitted provided that the following conditions are
     * met:
     *   * Redistributions of source code must retain the above copyright
     *     notice, this list of conditions and the following disclaimer.
     *   * Redistributions in binary form must reproduce the above copyright
     *     notice, this list of conditions and the following disclaimer in
     *     the documentation and/or other materials provided with the
     *     distribution.
     *   * Neither the name of The Qt Company Ltd nor the names of its
     *     contributors may be used to endorse or promote products derived
     *     from this software without specific prior written permission.
     */
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);

    // Request OpenGL 3.3 core or OpenGL ES 3.0.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        qDebug("Requesting 3.3 core context");
        fmt.setVersion(3, 3);
        fmt.setProfile(QSurfaceFormat::CoreProfile);
    } else {
        qDebug("Requesting 3.0 context");
        fmt.setVersion(3, 0);
    }

    QSurfaceFormat::setDefaultFormat(fmt);
    /*
     * End of Qt Company licensed code
     */

    MainWindow w;
    w.show();
    return a.exec();
}

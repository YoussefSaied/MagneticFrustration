#include <QKeyEvent>
#include <QTimerEvent>
#include <QMatrix4x4>
#include "glwidget.h"

// ======================================================================
void GLWidget::initializeGL()
{
    vue.init();
    if (dt > 0.001) {
        timerId = startTimer(dt * 1000);
    } else {
        timerId = startTimer(1);
    }
    chronometre.restart();
    pause();
}

// ======================================================================
void GLWidget::resizeGL(int width, int height)
{
    /* We start by specifying where OpenGL will draw in the window
     * Here we will ask him to use all the window
     */
    glViewport(0, 0, width, height);

    /* Then we modify the projection matrix of the shadder.
     * More details:
     *     http://www.songho.ca/opengl/gl_projectionmatrix.html
     * Then we update the shadder using an OpenGL method
     */
    QMatrix4x4 matrice;
    matrice.perspective(70.0, qreal(width) / qreal(height ? height : 1.0), 1e-3, 1e5);
    vue.setProjection(matrice);
}

// ======================================================================
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (!system_tab.empty()) { system_tab.back()->dessine(); }
}

// ======================================================================
void GLWidget::keyPressEvent(QKeyEvent * event)
{
    constexpr double petit_angle(5.0); // en degrés
    constexpr double petit_pas(5.0e-1);

    switch (event->key()) {
        case Qt::Key_Left:
            vue.rotate(petit_angle, 0.0, -1.0, 0.0);
            break;

        case Qt::Key_Right:
            vue.rotate(petit_angle, 0.0, +1.0, 0.0);
            break;

        case Qt::Key_Up:
            vue.rotate(petit_angle, -1.0, 0.0, 0.0);
            break;

        case Qt::Key_Down:
            vue.rotate(petit_angle, +1.0, 0.0, 0.0);
            break;

        case Qt::Key_PageUp:
        case Qt::Key_W:
            vue.translate(0.0, 0.0, petit_pas);
            break;

        case Qt::Key_PageDown:
        case Qt::Key_S:
            vue.translate(0.0, 0.0, -petit_pas);
            break;

        case Qt::Key_A:
            vue.translate(petit_pas, 0.0, 0.0);
            break;

        case Qt::Key_D:
            vue.translate(-petit_pas, 0.0, 0.0);
            break;

        case Qt::Key_R:
            vue.translate(0.0, -petit_pas, 0.0);
            break;

        case Qt::Key_F:
            vue.translate(0.0, petit_pas, 0.0);
            break;

        case Qt::Key_Q:
            vue.rotate(petit_angle, 0.0, 0.0, -1.0);
            break;

        case Qt::Key_E:
            vue.rotate(petit_angle, 0.0, 0.0, +1.0);
            break;

        case Qt::Key_Home:
            vue.initializePosition();
            break;

        case Qt::Key_Space:
            pause();
            break;
    }
    ;

    updateGL(); // redessine
} // GLWidget::keyPressEvent

// ======================================================================
void GLWidget::mousePressEvent(QMouseEvent * event)
{
    lastMousePosition = event->pos();
}

// ======================================================================
void GLWidget::mouseMoveEvent(QMouseEvent * event)
{
    /* If mouse tracking is disabled (the default), the widget only receives
     * mouse move events when at least one mouse button is pressed while the
     * mouse is being moved.
     *
     * Pour activer le "mouse tracking" if faut lancer setMouseTracking(true)
     * par exemple dans le constructeur de cette classe.
     */

    if (event->buttons() & Qt::LeftButton) {
        constexpr double petit_angle(.4); // en degrés

        // Récupère le mouvement relatif par rapport à la dernière position de la souris
        QPointF d = event->pos() - lastMousePosition;
        lastMousePosition = event->pos();

        vue.rotate(petit_angle * d.manhattanLength(), d.y(), d.x(), 0);

        updateGL();
    }
}

//
// ======================================================================

void GLWidget::timerEvent(QTimerEvent * event)
{
    Q_UNUSED(event);

    if (dt > 0.001) {
        if (!system_tab.empty()) { system_tab.back()->evolue(dt); }
        (system_tab.back()->time) += dt;
    } else {
        int n(0.001 / dt);
        if (!system_tab.empty()) { system_tab.back()->evolue(dt, n); }
        (system_tab.back()->time) += n * dt;
    }
    emit evolved(dt);
    updateGL();
}

// ======================================================================
void GLWidget::pause()
{
    if (timerId == 0) {
        // dans ce cas le timer ne tourne pas alors on le lance
        if (dt > 0.001) {
            timerId = startTimer(dt * 1000);
        } else { timerId = startTimer(1); }
        chronometre.restart();
    } else {
        // le timer tourne alors on l'arrête
        killTimer(timerId);
        timerId = 0;
    }
}

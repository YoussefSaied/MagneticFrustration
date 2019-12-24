#ifndef VUEOPENGL_H
#define VUEOPENGL_H

#include <QGLShaderProgram> // Class that regroups OpenGL functions related to shaders
#include <QMatrix4x4>
#include "SupportADessin.h"
#include "Vecteur3D.h"
#include "glsphere.h"
#include "glcylinder.h"


class VueOpenGL : public SupportADessin
{
public:
    // méthode(s) de dessin (héritée(s) de SupportADessin)

    virtual void dessine(Systeme const& S) override;
    virtual void dessine(Plan const& P) override;
    virtual void dessine(Dalle const& d) override;
    virtual void dessine(Sphere const& s) override;
    virtual void dessine(Brique const& b) override;
    virtual void dessine(Dodec const& d) override;
    virtual void dessine(Cube const& q) override;
    virtual void dessine(Cylinder const& c) override;
    virtual void dessine(MagnetA const& M) override;
    virtual void dessine(MagnetH const &M) override;

    virtual ~VueOpenGL(){ }

    // méthodes de (ré-)initialisation
    void init();
    void initializePosition();

    // méthode set
    void setProjection(QMatrix4x4 const& projection)
    {
        prog.setUniformValue("projection", projection);
    }

    // Méthodes set
    void translate(double x, double y, double z);
    void rotate(double angle, double dir_x, double dir_y, double dir_z);

    // method to draw the dodecahedron
    void dessineDodec(QMatrix4x4 const& point_de_vue = QMatrix4x4() );
    void dessineCube(QMatrix4x4 const& point_de_vue  = QMatrix4x4() );
    // method to draw a pentagon (dodecahedron face) 
    void dessinePenta(
        QMatrix4x4 const& point_de_vue = QMatrix4x4(), Vecteur3D const& x1 = Vecteur3D(0, 0, 0), Vecteur3D const& x2 = Vecteur3D(0, 0, 0), Vecteur3D const& x3 = Vecteur3D(0, 0, 0), Vecteur3D const& x4 = Vecteur3D(0, 0, 0), Vecteur3D const& x5 = Vecteur3D(0, 0, 0));
    void dessineSquare(
        QMatrix4x4 const& point_de_vue = QMatrix4x4(), Vecteur3D const& x1 = Vecteur3D(0, 0, 0), Vecteur3D const& x2 = Vecteur3D(0, 0, 0), Vecteur3D const& x3 = Vecteur3D(0, 0, 0), Vecteur3D const& x4 = Vecteur3D(0, 0, 0));


    void dessineDalle(QMatrix4x4 const& point_de_vue, Dalle const& dalle, double x = 1.0, double y = 1.0, double z = 0.0);
    void dessinePlan(QMatrix4x4 const& point_de_vue, Plan p);
    void dessineCross(QMatrix4x4 const& point_de_vue, Vecteur3D const& v);
    void dessineSphere(QMatrix4x4 const& point_de_vue,
      double rouge = 1.0, double vert = 0.0, double bleu = 0.0);
    void dessineCylinder(QMatrix4x4 const& point_de_vue,
      double rouge = 1.0, double vert = 0.0, double bleu = 0.0);
    void dessineLine(QMatrix4x4 const& point_de_vue, Vecteur3D const& v1, Vecteur3D const& v2);

    // alpha
    bool get_alpha()
    {
        return alpha;
    }

    void set_alpha(bool a)
    {
        alpha = a;
    }

private:
    // Un shader OpenGL encapsulé dans une classe Qt
    QGLShaderProgram prog;
    // QGLShaderProgram prog2;
    GLSphere sphere;
    GLCylinder cylinder;
    bool alpha;

    // Caméra
    QMatrix4x4 matrice_vue;
};

#endif // ifndef VUEOPENGL_H

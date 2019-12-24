#include "vue_opengl.h"
#include "vertex_shader.h" // Identifiants Qt de nos différents attributs
#include "Systeme.h"
#include "Dalle.h"
#include "Sphere.h"
#include "Brique.h"
#include "Cylinder.h"
using namespace std;

//               = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


void VueOpenGL::dessine(MagnetA const& M)
{
    // rotate
    Vecteur3D axer(M.orientationV() ^ Vecteur3D(0, 0, 1));
    double angle = acos(M.orientationV() * Vecteur3D(0, 0, 1));
    Vecteur3D axer2(M.axis ^ Vecteur3D(0, 0, 1));
    double angle2 = acos(M.axis * Vecteur3D(0, 0, 1));

    // axe drawing:
    QMatrix4x4 matrice3;

    
    matrice3.setToIdentity();
    matrice3.translate((M.get_position()).get_x(), (M.get_position()).get_y(), (M.get_position()).get_z());
    if (axer2 != 0) {
        matrice3.rotate(-1 * angle2 * 180 / M_PI, axer2.get_x(), axer2.get_y(), axer2.get_z());
    }

    matrice3.translate(0, 0, -0.5 * M.get_axerheight());
    prog.setUniformValue("vue_modele", matrice_vue * matrice3);
    // if (!M.selected)
    prog.setAttributeValue(CouleurId, 0.4, 0.25, 0.12);
    // else
    //    prog.setAttributeValue(CouleurId, 0.7, 0.7, 0.7);  // met la couleur
    cylinder.set_dimension(M.get_axerradius(), M.get_axerheight());
    cylinder.initialize();
    cylinder.draw(prog, SommetId);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // magnet body:

    QMatrix4x4 matrice2;
    matrice2.setToIdentity();
    matrice2.translate((M.get_position()).get_x(), (M.get_position()).get_y(), (M.get_position()).get_z());
    if (axer != 0) {
        matrice2.rotate(-1 * angle * 180 / M_PI, axer.get_x(), axer.get_y(), axer.get_z());
    }
    else if (M.orientationV() == Vecteur3D(0, 0, -1)) {
        matrice2.rotate(-1 * 180, (M.axis).get_x(), (M.axis).get_y(), (M.axis).get_z());
    }

    matrice2.translate(0, 0, -1 * M.length / 2);
    prog.setUniformValue("vue_modele", matrice_vue * matrice2);
    prog.setAttributeValue(CouleurId, 0.7, 0, 0);
    cylinder.set_dimension(M.radius, M.length / 2);
    cylinder.initialize();
    cylinder.draw(prog, SommetId);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    QMatrix4x4 matrice;
    matrice.setToIdentity();
    matrice.translate((M.get_position()).get_x(), (M.get_position()).get_y(), (M.get_position()).get_z());
    if (axer != 0) {
        matrice.rotate(-1 * angle * 180 / M_PI, axer.get_x(), axer.get_y(), axer.get_z());
    }
    else if (M.orientationV() == Vecteur3D(0, 0, -1)) {
        matrice2.rotate(-1 * 90, (M.axis).get_x(), (M.axis).get_y(), (M.axis).get_z());
    }

    if (!M.selected) {
        dessineCylinder(matrice, 0, 0, 0.7);
    }
    else {
        dessineCylinder(matrice, 0.75, 0.75, 0.75);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void VueOpenGL::dessine(const MagnetH &M)
{
    // rotate
    Vecteur3D axer(M.orientationV() ^ Vecteur3D(0, 0, 1));
    double angle = acos(M.orientationV() * Vecteur3D(0, 0, 1));


    // magnet body:

    QMatrix4x4 matrice2;

    matrice2.setToIdentity();
    matrice2.translate((M.get_position()).get_x(), (M.get_position()).get_y(), (M.get_position()).get_z());
    if (axer != 0) {
        matrice2.rotate(-1 * angle * 180 / M_PI, axer.get_x(), axer.get_y(), axer.get_z());
    }

    matrice2.translate(0, 0, -1 * M.length / 2);
    prog.setUniformValue("vue_modele", matrice_vue * matrice2);
    prog.setAttributeValue(CouleurId, 0.7, 0, 0);

    cylinder.set_dimension(M.radius, M.length / 2);
    cylinder.initialize();
    cylinder.draw(prog, SommetId);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    QMatrix4x4 matrice;
    matrice.setToIdentity();
    matrice.translate((M.get_position()).get_x(), (M.get_position()).get_y(), (M.get_position()).get_z());
    if (axer != 0) {
        matrice.rotate(-1 * angle * 180 / M_PI, axer.get_x(), axer.get_y(), axer.get_z());
    }

    if (!M.selected) {
        dessineCylinder(matrice, 0, 0, 0.7);
    }
    else {
        dessineCylinder(matrice, 0.75, 0.75, 0.75);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
} // VueOpenGL::dessine

void VueOpenGL::dessine(Sphere const& sphere)
{
    QMatrix4x4 matrice;
    matrice.translate((sphere.get_position()).get_x(), (sphere.get_position()).get_y(),
      (sphere.get_position()).get_z());
    matrice.scale(sphere.get_radius());
    dessineSphere(matrice, 1, 0, 1);
}

void VueOpenGL::dessine(Plan const& P)
{
    QMatrix4x4 matrice;
    dessinePlan(matrice, P);
}

void VueOpenGL::dessine(Dalle const& d)
{
    QMatrix4x4 matrice;
    dessineDalle(matrice, d);
}

void VueOpenGL::dessine(Brique const& b)
{
    std::vector<Dalle> d6;
    d6 = (b.dalle6());
    for (auto d : d6) {
        d.set_support(b.get_support());
        d.dessine();
    }
}

void VueOpenGL::dessine(Dodec const& d)
{
    double a = (1 + sqrt(5)) / 2;
    Vecteur3D v1(1 / a, 0, a);
    Vecteur3D v2(-1 / a, 0, a);
    Vecteur3D v3(-1, -1, 1);
    Vecteur3D o1(v1 - v2);
    Vecteur3D o2(v2 - v3);

    QMatrix4x4 matrice;
    matrice.setToIdentity();

    matrice.translate((d.get_position()).get_x(), (d.get_position()).get_y(), (d.get_position()).get_z());
    matrice.scale(d.get_edge());
    // rotation
    Vecteur3D axer0(o1 ^ o2);
    Vecteur3D axer1(d.get_vecteur_1() ^ axer0);
    double angle = acos((d.get_vecteur_1() * axer0) / ((d.get_vecteur_1().norme()) * axer0.norme()));

    if (axer1 != 0) {
        matrice.rotate(-1 * angle * 180 / M_PI, axer1.get_x(), axer1.get_y(), axer1.get_z());
    }

    dessineDodec(matrice);
}

void VueOpenGL::dessine(Cube const& q)
{
    double a = (1 + sqrt(5)) / 2;
    Vecteur3D v1(1 / a, 0, a);
    Vecteur3D v2(-1 / a, 0, a);
    Vecteur3D v3(-1, -1, 1);
    Vecteur3D o1(v1 - v2);
    Vecteur3D o2(v2 - v3);

    QMatrix4x4 matrice;
    matrice.setToIdentity();

    matrice.translate((q.get_position()).get_x(), (q.get_position()).get_y(), (q.get_position()).get_z());
    matrice.scale(q.get_edge());
    // rotation
    Vecteur3D axer0(o1 ^ o2);
    Vecteur3D axer1(q.get_vecteur_1() ^ axer0);
    double angle = acos((q.get_vecteur_1() * axer0) / ((q.get_vecteur_1().norme()) * axer0.norme()));

    if (axer1 != 0) {
        matrice.rotate(-1 * angle * 180 / M_PI, axer1.get_x(), axer1.get_y(), axer1.get_z());
    }

    dessineCube(matrice);
}

void VueOpenGL::dessine(Cylinder const& c)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    QMatrix4x4 matrice;
    Vecteur3D axer(Vecteur3D(0, 0, 1) ^ (c.get_vecteur_1()).normalise());
    double angle = acos((c.get_vecteur_1() * Vecteur3D(0, 0, 1)) / ((c.get_vecteur_1()).norme()));

    matrice.translate((c.get_position()).get_x(), (c.get_position()).get_y(), (c.get_position()).get_z());

    if (axer != 0) {
        matrice.rotate(1 * angle * 180 / M_PI, axer.get_x(), axer.get_y(), axer.get_z());
    }

    GLCylinder cylinderm;
    prog.setUniformValue("vue_modele", matrice_vue * matrice);
    prog.setAttributeValue(CouleurId, 0.5, 0.5, 0.5); // assigning color
    cylinderm.set_dimension(c.get_radius(), c.get_hauteur());
    cylinderm.initialize();
    cylinderm.draw(prog, SommetId);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void VueOpenGL::dessine(Systeme const& S)
{
    if (!(S.tab_ptr_Magnets).empty()) {
        for (auto const& ptr_Magnet : (S.tab_ptr_Magnets)) {
            (*ptr_Magnet).dessine();
        }
    }

    if ((S.tab_ptr_obstacles).size() != 0) {
        for (auto const& ptr_obstacle : (S.tab_ptr_obstacles)) {
            (*ptr_obstacle).dessine();
        }
    }
}

//               = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
void VueOpenGL::init()
{
    /* Initialise notre vue OpenGL.
     * Dans cet exemple, nous créons et activons notre shader.
     *
     * En raison du contenu des fichiers *.glsl, le shader de cet exemple
     * NE permet QUE de dessiner des primitives colorées
     * (pas de textures, brouillard, reflets de la lumière ou autres).
     *
     * Il est séparé en deux parties VERTEX et FRAGMENT.
     * Le VERTEX :
     * - récupère pour chaque sommet des primitives de couleur (dans
     * l'attribut couleur) et de position (dans l'attribut sommet)
     * - multiplie l'attribut sommet par les matrices 'vue_modele' et
     * 'projection' et donne le résultat à OpenGL
     * - passe la couleur au shader FRAGMENT.
     *
     * Le FRAGMENT :
     * - applique la couleur qu'on lui donne
     */

    prog.addShaderFromSourceFile(QGLShader::Vertex, ":/vertex_shader.glsl");
    prog.addShaderFromSourceFile(QGLShader::Fragment, ":/fragment_shader.glsl");

    /* Identifie les deux attributs du shader de cet exemple
     * (voir vertex_shader.glsl).
     *
     * L'attribut identifié par 0 est particulier, il permet d'envoyer un
     * nouveau "point" à OpenGL
     *
     * C'est pourquoi il devra obligatoirement être spécifié et en dernier
     * (après la couleur dans cet exemple, voir plus bas).
     */

    prog.bindAttributeLocation("sommet", SommetId);
    prog.bindAttributeLocation("couleur", CouleurId);

    // Compilation du shader OpenGL
    prog.link();

    // Activation du shader
    prog.bind();

    /* Activation du "Test de profondeur" et du "Back-face culling"
     * Le Test de profondeur permet de dessiner un objet à l'arrière-plan
     * partielement caché par d'autres objets.
     *
     * Le Back-face culling consiste à ne dessiner que les face avec ordre
     * de déclaration dans le sens trigonométrique.
     */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /*if (alpha){
     * glDisable(GL_DEPTH_TEST);
     * glEnable(GL_BLEND);
     * glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     * } */

    sphere.initialize();   // initialise la sphère
    cylinder.initialize(); // initialise cylinder

    initializePosition();
}

//               = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
void VueOpenGL::initializePosition()
{
    // position initiale
    matrice_vue.setToIdentity();
    // matrice_vue.translate(0.0, 0.0, -15.0);
    matrice_vue.scale(100);
    matrice_vue.rotate(-90.0, 1.0, 0.0, 0.0);
}

//               = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
void VueOpenGL::translate(double x, double y, double z)
{
    /* Multiply the vue matrix by the left
     * This way the last modification to the matrix
     * is applied last (composition of functions).
     */
    QMatrix4x4 translation_supplementaire;
    translation_supplementaire.translate(x, y, z);
    matrice_vue = translation_supplementaire * matrice_vue;
}

//               = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
void VueOpenGL::rotate(double angle, double dir_x, double dir_y, double dir_z)
{
    // Multiplie la matrice de vue par LA GAUCHE
    QMatrix4x4 rotation_supplementaire;
    rotation_supplementaire.rotate(angle, dir_x, dir_y, dir_z);
    matrice_vue = rotation_supplementaire * matrice_vue;
}

void VueOpenGL::dessineSquare(QMatrix4x4 const& point_de_vue, Vecteur3D const& x1, Vecteur3D const& x2, Vecteur3D const& x3, Vecteur3D const& x4)
{
    // draw lines glBegin(GL_LINES);
    dessineLine(point_de_vue, x1, x2);
    dessineLine(point_de_vue, x2, x3);
    dessineLine(point_de_vue, x3, x4);
    dessineLine(point_de_vue, x4, x1);
}

//               = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
void VueOpenGL::dessineCube(QMatrix4x4 const& point_de_vue)
{
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

    glBegin(GL_QUADS);
    QMatrix4x4 m;
    m.setToIdentity();

    dessineSquare(point_de_vue, Vecteur3D(-1, -1, -1), Vecteur3D(-1, -1, 1), Vecteur3D(-1, 1, 1), Vecteur3D(-1, 1, -1)); // 1
    dessineSquare(point_de_vue, Vecteur3D(1, -1, -1), Vecteur3D(1, -1, 1), Vecteur3D(1, 1, 1), Vecteur3D(1, 1, -1));     // 1
    dessineSquare(point_de_vue, Vecteur3D(-1, -1, 1), Vecteur3D(-1, 1, 1), Vecteur3D(1, 1, 1), Vecteur3D(1, -1, 1));     // 1
    dessineSquare(point_de_vue, Vecteur3D(-1, -1, -1), Vecteur3D(1, -1, -1), Vecteur3D(1, 1, -1), Vecteur3D(-1, 1, -1)); // 1
    dessineSquare(point_de_vue, Vecteur3D(-1, -1, -1), Vecteur3D(-1, -1, 1), Vecteur3D(1, -1, 1), Vecteur3D(1, -1, -1)); // 1
    dessineSquare(point_de_vue, Vecteur3D(-1, 1, -1), Vecteur3D(1, 1, -1), Vecteur3D(1, 1, 1), Vecteur3D(-1, 1, 1));     // 1

} // VueOpenGL::dessineCube

void VueOpenGL::dessinePenta(QMatrix4x4 const& point_de_vue, Vecteur3D const& x1, Vecteur3D const& x2, Vecteur3D const& x3, Vecteur3D const& x4,
  Vecteur3D const& x5)
{

    dessineLine(point_de_vue, x1, x2);
    dessineLine(point_de_vue, x2, x3);
    dessineLine(point_de_vue, x3, x4);
    dessineLine(point_de_vue, x4, x5);
    dessineLine(point_de_vue, x5, x1);
}

void VueOpenGL::dessineDodec(QMatrix4x4 const& point_de_vue)
{
    double a = (1 + sqrt(5)) / 2;
    QMatrix4x4 m;
    m.setToIdentity();
    dessinePenta(point_de_vue, Vecteur3D(1 / a, 0, a), Vecteur3D(-1 / a, 0, a), Vecteur3D(-1, -1, 1),
      Vecteur3D(0, -a, 1 / a), Vecteur3D(1, -1, 1)); // 1
    dessinePenta(point_de_vue, Vecteur3D(-1, -1, 1), Vecteur3D(-a, -1 / a, 0), Vecteur3D(-1, -1, -1),
      Vecteur3D(0, -a, -1 / a), Vecteur3D(0, -a, 1 / a)); // 2
    dessinePenta(point_de_vue, Vecteur3D(0, -a, 1 / a), Vecteur3D(0, -a, -1 / a), Vecteur3D(1, -1, -1),
      Vecteur3D(a, -1 / a, 0), Vecteur3D(1, -1, 1)); // 3
    dessinePenta(point_de_vue, Vecteur3D(1 / a, 0, a), Vecteur3D(1, -1, 1), Vecteur3D(a, -1 / a, 0),
      Vecteur3D(a, 1 / a, 0), Vecteur3D(1, 1, 1)); // 4
    dessinePenta(point_de_vue, Vecteur3D(1 / a, 0, -a), Vecteur3D(1, 1, -1), Vecteur3D(a, 1 / a, 0),
      Vecteur3D(a, -1 / a, 0), Vecteur3D(1, -1, -1)); // 5
    dessinePenta(point_de_vue, Vecteur3D(1 / a, 0, -a), Vecteur3D(1, -1, -1), Vecteur3D(0, -a, -1 / a),
      Vecteur3D(-1, -1, -1), Vecteur3D(-1 / a, 0, -a)); // 6
    dessinePenta(point_de_vue, Vecteur3D(1 / a, 0, -a), Vecteur3D(-1 / a, 0, -a), Vecteur3D(-1, 1, -1),
      Vecteur3D(0, a, -1 / a), Vecteur3D(1, 1, -1)); // 7
    dessinePenta(point_de_vue, Vecteur3D(-1, -1, -1), Vecteur3D(-a, -1 / a, 0), Vecteur3D(-a, 1 / a, 0),
      Vecteur3D(-1, 1, -1), Vecteur3D(-1 / a, 0, -a)); // 8
    dessinePenta(point_de_vue, Vecteur3D(-1, 1, 1), Vecteur3D(-a, 1 / a, 0), Vecteur3D(-a, -1 / a, 0),
      Vecteur3D(-1, -1, 1), Vecteur3D(-1 / a, 0, a)); // 9
    dessinePenta(point_de_vue, Vecteur3D(-1, 1, 1), Vecteur3D(-1 / a, 0, a), Vecteur3D(1 / a, 0, a), Vecteur3D(1, 1,
      1),
      Vecteur3D(0, a, 1 / a)); // 10
    dessinePenta(point_de_vue, Vecteur3D(-1, 1, 1), Vecteur3D(0, a, 1 / a), Vecteur3D(0, a, -1 / a), Vecteur3D(-1, 1,
      -1),
      Vecteur3D(-a, 1 / a, 0)); // 11
    dessinePenta(point_de_vue, Vecteur3D(1, 1, -1), Vecteur3D(0, a, -1 / a), Vecteur3D(0, a, 1 / a), Vecteur3D(1, 1,
      1),
      Vecteur3D(a, 1 / a, 0)); // 12
}

void VueOpenGL::dessineDalle(QMatrix4x4 const& point_de_vue, Dalle const& dalle, double x, double y, double z)
{
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

    glBegin(GL_QUADS);
    prog.setAttributeValue(CouleurId, x, y, z); // jaune
    prog.setAttributeValue(SommetId, (dalle.Sommet1()).get_x(), (dalle.Sommet1()).get_y(), (dalle.Sommet1()).get_z());
    prog.setAttributeValue(SommetId, (dalle.Sommet2()).get_x(), (dalle.Sommet2()).get_y(), (dalle.Sommet2()).get_z());
    prog.setAttributeValue(SommetId, (dalle.Sommet3()).get_x(), (dalle.Sommet3()).get_y(), (dalle.Sommet3()).get_z());
    prog.setAttributeValue(SommetId, (dalle.Sommet4()).get_x(), (dalle.Sommet4()).get_y(), (dalle.Sommet4()).get_z());
    glEnd();
    glBegin(GL_LINES);
    prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
    prog.setAttributeValue(SommetId, (dalle.Sommet1()).get_x(), (dalle.Sommet1()).get_y(), (dalle.Sommet1()).get_z());
    prog.setAttributeValue(SommetId, (dalle.Sommet2()).get_x(), (dalle.Sommet2()).get_y(), (dalle.Sommet2()).get_z());
    glEnd();
    glBegin(GL_LINES);
    prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
    prog.setAttributeValue(SommetId, (dalle.Sommet1()).get_x(), (dalle.Sommet1()).get_y(), (dalle.Sommet1()).get_z());
    prog.setAttributeValue(SommetId, (dalle.Sommet4()).get_x(), (dalle.Sommet4()).get_y(), (dalle.Sommet4()).get_z());
    glEnd();
    glBegin(GL_LINES);
    prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
    prog.setAttributeValue(SommetId, (dalle.Sommet3()).get_x(), (dalle.Sommet3()).get_y(), (dalle.Sommet3()).get_z());
    prog.setAttributeValue(SommetId, (dalle.Sommet2()).get_x(), (dalle.Sommet2()).get_y(), (dalle.Sommet2()).get_z());
    glEnd();
    glBegin(GL_LINES);
    prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
    prog.setAttributeValue(SommetId, (dalle.Sommet3()).get_x(), (dalle.Sommet3()).get_y(), (dalle.Sommet3()).get_z());
    prog.setAttributeValue(SommetId, (dalle.Sommet4()).get_x(), (dalle.Sommet4()).get_y(), (dalle.Sommet4()).get_z());
    glEnd();
}

void VueOpenGL::dessinePlan(QMatrix4x4 const& point_de_vue, Plan p)
{
    Dalle d(
        p.get_position() - 2 * 100 * ((p.get_vecteur_1()).GS1() + ((p.get_vecteur_1()).GS1()).GS1() ),
        p.get_vecteur_1(), ((p.get_vecteur_1()).GS1()), 4 * 100, 4 * 100);
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
    dessineDalle(point_de_vue, d, 0.5, 0.3, 0.6);
    glBegin(GL_LINES);
    prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
    prog.setAttributeValue(SommetId, (p.get_position()).get_x(), (p.get_position()).get_y(),
      (p.get_position()).get_z());
    prog.setAttributeValue(SommetId, (p.get_position()).get_x() + (p.get_vecteur_1()).get_x() * 20,
      (p.get_position()).get_y() + (p.get_vecteur_1()).get_y() * 20,
      (p.get_position()).get_z() + (p.get_vecteur_1()).get_z() * 20);

    glEnd();
}

void VueOpenGL::dessineLine(QMatrix4x4 const& point_de_vue, Vecteur3D const& v1, Vecteur3D const& v2)
{
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

    glBegin(GL_LINES);
    prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
    prog.setAttributeValue(SommetId, v1.get_x(), v1.get_y(), v1.get_z());
    prog.setAttributeValue(SommetId, v2.get_x(), v2.get_y(), v2.get_z());
    glEnd();
}

void VueOpenGL::dessineCross(QMatrix4x4 const& point_de_vue, Vecteur3D const& v)
{
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

    glBegin(GL_TRIANGLES);
    prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
    prog.setAttributeValue(SommetId, v.get_x(), v.get_y(), v.get_z());
    prog.setAttributeValue(SommetId, 1.25 * (v.get_x()), 1.25 * (v.get_y()), 1.25 * (v.get_z()));
    prog.setAttributeValue(SommetId, 0.75 * (v.get_x()), 0.75 * (v.get_y()), 0.75 * (v.get_z()));

    glEnd();
}

void VueOpenGL::dessineSphere(QMatrix4x4 const& point_de_vue,
  double rouge, double vert, double bleu)
{
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
    prog.setAttributeValue(CouleurId, rouge, vert, bleu); // met la couleur
    sphere.draw(prog, SommetId);                          // dessine la sphère
}

void VueOpenGL::dessineCylinder(QMatrix4x4 const& point_de_vue,
  double rouge, double vert, double bleu)
{
    prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
    prog.setAttributeValue(CouleurId, rouge, vert, bleu); // met la couleur
    cylinder.draw(prog, SommetId);                        // dessine la cylinder
}

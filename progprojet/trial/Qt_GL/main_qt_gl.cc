#include <QApplication>
#include "glwidget.h"
#include "Systeme.h"
#include "Vecteur3D.h"
#include "Plan.h"
#include "Dalle.h"
#include <vector>
#include "Sphere.h"
// #include "Tetra.h"
#include "Brique.h"
#include "Dodec.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Window.h"
#include "Magnet.h"
#include "MagnetA.h"
#include "MagnetH.h"
#include <iostream>
#include <algorithm>

using namespace std;

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);
    Window w;

    /* Dodec is the regular dodec with MagnetA
     * Dodec2 is the regular dodec with MagnetA but with 10 times as large edges and the same size magnets
     * DodecH is the dodec with MagnetH
     */

    string desiredconfig = "Dodec";

    // coordinate system:
    Cylinder c1(Position(0, 10, 0), Vecteur3D(0, 7, 0), 1, 1);
    Cylinder c2(Position(10, 0, 0), Vecteur3D(7, 0, 0), 1, 1);
    Cylinder c3(Position(0, 0, 10), Vecteur3D(0, 0, 1), 1, 1);
    (w.glWidget)->addObstacle(c1);
    (w.glWidget)->addObstacle(c2);
    (w.glWidget)->addObstacle(c3);

    if (desiredconfig == "simple") {
        MagnetA M4(Position(0, 0, 0), Vecteur3D(0, 1, 0), 0);
        MagnetA M5(Position(0, 0, 0 + M4.length * 2.2), Vecteur3D(0, 1, 0), 1);
        (w.glWidget)->addMagnet(M4);
        (w.glWidget)->addMagnet(M5);
        w.show();
    }



    // Dodec displaying saved angles:
    if (desiredconfig == "Dodecdisplay") {
        // Dodec dode(Vecteur3D(0, 0, 3), 4, Vecteur3D(0, -0.763932, 1.23607), false);
        Dodec dode(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0, 1.0), false);
        for (size_t i = 0; i < (dode.vertipositions()).size(); ++i) {
            size_t si = ((dode.vertipositions())[i]).size();
            for (size_t j = 0; j < si; ++j) {
                Vecteur3D pos( ((dode.vertipositions())[i][j] + (dode.vertipositions())[i][(j + 1) % si]) / 2);


                // axis CALCULATIONS:
                Vecteur3D polaraxis = (dode.vertipositions())[i][j] - (dode.vertipositions())[i][(j + 1) % si];
                Vecteur3D v2        =
                  ((dode.vertipositions())[i][(j + 1) % si] - (dode.vertipositions())[i][(j + 2) % si]);
                Vecteur3D v3 = polaraxis ^ v2;

                double alph1  = -1 * atan(2) / 2;
                Vecteur3D v4  = v3.rotate(alph1, polaraxis);
                Vecteur3D axe = (v4 ^ polaraxis).normalise();

                MagnetA M(pos, axe, 1, 0, polaraxis); // position pos, axis axe, movable yes, angle_0 0, polaraxis polaraxis
                (w.glWidget)->addMagnet(M);
            }
        }
        (w.glWidget)->system_tab[0]->randominitial();
        (w.glWidget)->system_tab[0]->setangles("finalangles.in");
        (w.glWidget)->addObstacle(dode);
        w.show();
    }


    // Dodec:
    if (desiredconfig == "Dodec") {
        Dodec dode(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0, 1.0), false);
        for (size_t i = 0; i < (dode.vertipositions()).size(); ++i) {
            size_t si = ((dode.vertipositions())[i]).size();
            for (size_t j = 0; j < si; ++j) {
                Vecteur3D pos( ((dode.vertipositions())[i][j] + (dode.vertipositions())[i][(j + 1) % si]) / 2);

                // axis CALCULATIONS:
                Vecteur3D polaraxis = (dode.vertipositions())[i][j] - (dode.vertipositions())[i][(j + 1) % si];
                Vecteur3D v2        =
                  ((dode.vertipositions())[i][(j + 1) % si] - (dode.vertipositions())[i][(j + 2) % si]);
                Vecteur3D v3 = polaraxis ^ v2;

                // angle for v3 to average between two faces
                double alph1  = -1 * atan(2) / 2;
                Vecteur3D v4  = v3.rotate(alph1, polaraxis);
                Vecteur3D axe = (v4 ^ polaraxis).normalise();

                MagnetA M(pos, axe, 1, 0, polaraxis); // position pos, axis axe, movable yes, angle_0 0, polaraxis polaraxis
                (w.glWidget)->addMagnet(M);
            }
        }
        (w.glWidget)->system_tab[0]->randominitial();
        (w.glWidget)->addObstacle(dode);
        w.show();
    }

    // Dodec2:
    if (desiredconfig == "Dodec2") {
        Dodec dode(Vecteur3D(0, 0, 0), 30e-2, Vecteur3D(0, 0, 1.0), false);
        for (size_t i = 0; i < (dode.vertipositions()).size(); ++i) {
            size_t si = ((dode.vertipositions())[i]).size();
            for (size_t j = 0; j < si; ++j) {
                Vecteur3D pos( ((dode.vertipositions())[i][j] + (dode.vertipositions())[i][(j + 1) % si]) / 2);

                // axis CALCULATIONS:
                Vecteur3D polaraxis = (dode.vertipositions())[i][j] - (dode.vertipositions())[i][(j + 1) % si];
                Vecteur3D v2        =
                  ((dode.vertipositions())[i][(j + 1) % si] - (dode.vertipositions())[i][(j + 2) % si]);
                Vecteur3D v3 = polaraxis ^ v2;

                // angle for v3 to average between two faces
                double alph1  = -1 * atan(2) / 2;
                Vecteur3D v4  = v3.rotate(alph1, polaraxis);
                Vecteur3D axe = (v4 ^ polaraxis).normalise();

                MagnetA M(pos, axe, 1, 0, polaraxis); // position pos, axis axe, movable yes, angle_0 0, polaraxis polaraxis
                (w.glWidget)->addMagnet(M);
            }
        }
        (w.glWidget)->system_tab[0]->randominitial();
        (w.glWidget)->addObstacle(dode);
        w.show();
    }


    // DodecH:
    if (desiredconfig == "DodecH") {
        // Dodec dode(Vecteur3D(0, 0, 3), 4, Vecteur3D(0, -0.763932, 1.23607), false);
        Dodec dode(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0, 1.0), false);
        for (size_t i = 0; i < (dode.vertipositions()).size(); ++i) {
            size_t si = ((dode.vertipositions())[i]).size();
            for (size_t j = 0; j < si; ++j) {
                Vecteur3D pos( ((dode.vertipositions())[i][j] + (dode.vertipositions())[i][(j + 1) % si]) / 2);
                MagnetH M(pos); // position pos, axis axe, movable yes, angle_0 0, polaraxis polaraxis
                (w.glWidget)->addMagnet(M);
            }
        }
        // (w.glWidget)->system_tab[0]->randominitial();
        (w.glWidget)->addObstacle(dode);
        w.show();
    }


    // DodecPartial:
    if (desiredconfig == "Dodecpartial") {
        // Dodec dode(Vecteur3D(0, 0, 3), 4, Vecteur3D(0, -0.763932, 1.23607), false);
        Dodec dode(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0, 1.0), false);
        for (size_t i = 0; i < (dode.vertipositions()).size(); ++i) {
            size_t si = ((dode.vertipositions())[i]).size();
            for (size_t j = 0; j < si; ++j) {
                Vecteur3D pos( ((dode.vertipositions())[i][j] + (dode.vertipositions())[i][(j + 1) % si]) / 2);

                // axis CALCULATIONS:
                Vecteur3D polaraxis = (dode.vertipositions())[i][j] - (dode.vertipositions())[i][(j + 1) % si];
                Vecteur3D v2        =
                  ((dode.vertipositions())[i][(j + 1) % si] - (dode.vertipositions())[i][(j + 2) % si]);
                Vecteur3D v3 = polaraxis ^ v2;

                // angle for v3 to average between two faces
                double alph1  = -1 * atan(2) / 2;
                Vecteur3D v4  = v3.rotate(alph1, polaraxis);
                Vecteur3D axe = (v4 ^ polaraxis).normalise();

                MagnetA M(pos, axe, 1, 0, polaraxis); // position pos, axis axe, movable yes, angle_0 0, polaraxis polaraxis
                if ( (pos
                  - Vecteur3D(0, 0.0255195,
                  0.0412915)) < 0.001 or (pos - Vecteur3D(-0.0242705, 0.00788597, 0.0412915)) < 0.001
                  or (pos - Vecteur3D(-0.0242705, 0.0334055, 0.0255195)) < 0.001
                  or (pos - Vecteur3D(-0.0392705, 0.0285317, 0.0)) < 0.001
                  or (pos - Vecteur3D(-0.015, 0.0461653, 0.0)) < 0.001)
                {
                    (w.glWidget)->addMagnet(M);
                }
            }
        }
        (w.glWidget)->system_tab[0]->randominitial();
        (w.glWidget)->addObstacle(dode);
        w.show();
    }


    // OldDodec:
    // new averaged axis
    if (desiredconfig == "OldDodec") {
        // Dodec dode(Vecteur3D(0, 0, 3), 4, Vecteur3D(0, -0.763932, 1.23607), false);
        Dodec dode(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0, 1.23607), false);
        for (size_t i = 0; i < (dode.vertipositions()).size(); ++i) {
            size_t si = ((dode.vertipositions())[i]).size();
            for (size_t j = 0; j < si; ++j) {
                Vecteur3D p(0, 0, 0);
                p = ((dode.vertipositions())[i][j] + (dode.vertipositions())[i][(j + 1) % si]) / 2;


                // axis CALCULATIONS:
                Vecteur3D v1 = ((dode.vertipositions())[i][j] - (dode.vertipositions())[i][(j + 1) % si]);
                Vecteur3D v2 = ((dode.vertipositions())[i][(j + 1) % si] - (dode.vertipositions())[i][(j + 2) % si]);
                Vecteur3D v3 = v1 ^ v2;          // vector perpindicular to face
                double alph  = -1 * atan(2) / 2; // angle between two faces
                alph = 0;
                Vecteur3D v4  = v3.rotate(alph, v1);
                Vecteur3D axe = v4 ^ v1;
                // calculate polaraxis:
                Vecteur3D polaraxis = (dode.vertipositions())[i][j] - (dode.vertipositions())[i][(j + 1) % si];


                // add magnet here.
                MagnetA M(p, axe, 1, 0, polaraxis); // position p, axis a, movable yes, angle_0 0, polaraxis polaraxis
                (w.glWidget)->addMagnet(M);
            }
        }
        (w.glWidget)->addObstacle(dode);
        w.show();
    }

    // Honeycomb:
    if (desiredconfig == "HC") {
        size_t Honeycombsize = 1;
        for (size_t i(0); i < Honeycombsize; ++i) {
            for (size_t j(0); j < Honeycombsize; ++j) {
                Vecteur3D v1(1, 0, 0);
                Vecteur3D v2(1, 0, 0);
                Vecteur3D v3(1, 0, 0);
                v2 = v1.rotate(M_PI / 3 * 2, Vecteur3D(0, 0, 1));
                v3 = v1.rotate(M_PI / 3 * 4, Vecteur3D(0, 0, 1));
                double r = 1.0;
                MagnetA M0(Position(0, 0, 0));
                double translationalunit(1.4 * M0.length * sqrt(2 - 2 * cos(M_PI * 2 / 3) ) );
                MagnetA M2(( Position(translationalunit * (i + cos(M_PI / 3) * j),
                  translationalunit * ( j * cos(M_PI / 6)), 0)), v2, 1, 1.57);
                MagnetA M1(( Position(translationalunit * (i + cos(M_PI / 3) * j) + r * M0.length * 1.2,
                  translationalunit * ( j *  cos(M_PI / 6)), 0)), v3, 1, 1.57);
                MagnetA M3(( Position(translationalunit * (i + cos(M_PI / 3) * j) + r * M0.length * 0.6,
                  translationalunit * ( j *  cos(M_PI / 6)) + r * M1.length * 1.039, 0)), v1, 1, -1.57);

                (w.glWidget)->addMagnet(M1);
                (w.glWidget)->addMagnet(M2);
                (w.glWidget)->addMagnet(M3);
            }
        }
        w.show();
    }


    if (desiredconfig == "testrotation") {
        double x(0), y, z;
        while (x != 1.11) {
            cout << "Vector before rotation: "
                 << " x: ";
            cin >> x;
            cout << " y: ";
            cin >> y;
            cout << " z: ";
            cin >> z;
            Vecteur3D vectortoberotated(x, y, z);
            cout << vectortoberotated
                 << " axe of rotation: "
                 << " x: ";
            cin >> x;
            cout << " y: ";
            cin >> y;
            cout << " z: ";
            cin >> z;
            Vecteur3D rotationaxe(x, y, z);
            cout << " angle: ";
            double angleofrotation;
            cin >> angleofrotation;
            cout << " Vector after: " << (vectortoberotated.rotate(angleofrotation, rotationaxe)) << endl;
        }
    }

    /*
        // Cube:
        if (desiredconfig == "Cube") {
            // Cube dode(Vecteur3D(0, 0, 3), 4, Vecteur3D(0, -0.763932, 1.23607), false);
            Cube qube(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0, 1.0));
            for (size_t i = 0; i < (qube.vertipositions()).size(); ++i) {
                size_t si = ((qube.vertipositions())[i]).size();
                for (size_t j = 0; j < si; ++j) {
                    Vecteur3D pos(0,0,0);
                    pos = ((qube.vertipositions())[i][j] + (qube.vertipositions())[i][(j + 1) % si]) / 2;
                    // axis CALCULATIONS:
                    Vecteur3D polaraxis = (qube.vertipositions())[i][j] - (qube.vertipositions())[i][(j + 1) % si]; //edge
                    Vecteur3D v2        =
                      ((qube.vertipositions())[i][(j + 1) % si] - (qube.vertipositions())[i][(j + 2) % si]); //next edge
                    //double alph1 = 3*M_PI/4;
                    Vecteur3D axe = Vecteur3D(0.5,0.5,0).normalise();  //v2.rotate(alph1, polaraxis);

                     Magnet M(pos, axe, 1, 0, polaraxis);
                      (w.glWidget)->addMagnet(M);

                     // position pos, axis axe, movable yes, angle_0 0, polaraxis polaraxis

                }
            }
            (w.glWidget)->addObstacle(qube);
            w.show();
        }

        //Tetra:
        if (desiredconfig == "Tetra") {
            // Cube dode(Vecteur3D(0, 0, 3), 4, Vecteur3D(0, -0.763932, 1.23607), false);
            Cube qube(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0, 1.0));
            vector<Vecteur3D> poss;
            for (size_t i = 0; i < (qube.vertipositions()).size(); ++i) {
                size_t si = ((qube.vertipositions())[i]).size();
                for (size_t j = 0; j < si; ++j) {
                    Vecteur3D pos( ((qube.vertipositions())[i][j] + (qube.vertipositions())[i][(j + 1) % si]) / 2);
                    // axis CALCULATIONS:
                    Vecteur3D v1 = (qube.vertipositions())[i][j] - (qube.vertipositions())[i][(j + 1) % si]; //edge
                    Vecteur3D v2 =((qube.vertipositions())[i][(j + 1) % si] - (qube.vertipositions())[i][(j + 2) % si]); //next edge
                    Vecteur3D v3 = v1^v2 //perpendicular
                    double alph1 = M_PI/6;
                    Vecteur3D axe  = v3.rotate(alph1, v1);

                    if (std::find(poss.begin(), poss.end(), pos) != poss.end()) {}
                    else {Magnet M(pos, axe, 1, 0, polaraxis);
                      poss.push_back(pos);
                      (w.glWidget)->addMagnet(M);
                    }
                     // position pos, axis axe, movable yes, angle_0 0, polaraxis polaraxis

                }
            }
            (w.glWidget)->addObstacle(qube);
            w.show();
        }

     */
    return a.exec();
} // main

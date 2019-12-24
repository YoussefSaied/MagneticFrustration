#include "Systeme.h"
#include "Vecteur3D.h"
#include "Plan.h"
#include "Dalle.h"
#include <vector>
#include "Sphere.h"
#include "Brique.h"
#include "Dodec.h"
#include "Cylinder.h"
#include <iostream>

#include "ConfigFile.tpp"

using namespace std;

int main(int argc, char * argv[])
{
    // intitial configuration:
    Systeme s;

    // Dodec dode(Vecteur3D(0, 0, 3), 4, Vecteur3D(0, -0.763932, 1.23607), false);
    Dodec dode(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0, 1.0), false);
    for (size_t i = 0; i < (dode.vertipositions()).size(); ++i) {
        size_t si = ((dode.vertipositions())[i]).size();
        for (size_t j = 0; j < si; ++j) {
            Vecteur3D pos( ((dode.vertipositions())[i][j] + (dode.vertipositions())[i][(j + 1) % si]) / 2);
            Vecteur3D polaraxis = (dode.vertipositions())[i][j] - (dode.vertipositions())[i][(j + 1) % si];
            Vecteur3D v2        =
              ((dode.vertipositions())[i][(j + 1) % si] - (dode.vertipositions())[i][(j + 2) % si]);
            Vecteur3D v3 = polaraxis ^ v2;
            double alph1  = -1 * atan(2) / 2;
            Vecteur3D v4  = v3.rotate(alph1, polaraxis);
            Vecteur3D axe = (v4 ^ polaraxis).normalise();

            Magnet M(pos, axe, 1, 0, polaraxis); // position pos, axis axe, movable yes, angle_0 0, polaraxis
            s.addMagnet(M);
        }
    }

    // input configuration file:
    string inputPath("configuration.in");
    if (argc > 1) { // Input file
        inputPath = argv[1];
    }
    ConfigFile configFile(inputPath);
    for (int i(2); i < argc; ++i) { // Additional input arguments
        configFile.process(argv[i]);
    }

    double dt      = configFile.get<double>("dt");
    double timesim = configFile.get<double>("timesim");
    double f       = configFile.get<double>("f");
    int n          = configFile.get<int>("nb_repet");

    // output text file:
    string output = configFile.get<string>("output");
    unique_ptr<ofstream> tfile(new ofstream(output.c_str()));
    tfile->precision(20);
    SupportADessinTexte tsupport(*tfile);
    s.setfriction(f);
    s.randominitial();
    s.n = n; // number of times display() is used when running a text sim.
    s.set_support(&tsupport);
    s.evolue1(dt, timesim, 1,0);

    return 0;
}

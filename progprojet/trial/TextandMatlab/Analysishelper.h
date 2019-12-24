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


using namespace std;


// functions and structures and variables:
Dodec dode(Vecteur3D(0, 0, 0), 3e-2, Vecteur3D(0, 0, 1.23607), false);
// The system:
Systeme s;
// functions and structures are easily generalisable to all solids, would be better with classes

struct vertixmagnets {
    Vecteur3D position;
    int       magnet1; // index
    int       magnet2; // index
    int       magnet3; // index
};

struct doublevertix {
    int           centralmagnet;
    vertixmagnets v1;
    vertixmagnets v2;
};

struct facemagnets {
    vector<doublevertix> doubleVertixVector;
    double               orientation;
};


// tables:
vector<vertixmagnets> VM;
vector<doublevertix> DV;
vector<facemagnets> FM;


// Matlab stuff :( / functions ::

// Vertix strength (f/w):

int Oddoneout(vertixmagnets vm)
{
    double p12 = ((s.tab_ptr_Magnets[vm.magnet1])->orientation()) * ((s.tab_ptr_Magnets[vm.magnet2])->orientation());
    double p13 = ((s.tab_ptr_Magnets[vm.magnet1])->orientation()) * ((s.tab_ptr_Magnets[vm.magnet3])->orientation());
    double p23 = ((s.tab_ptr_Magnets[vm.magnet2])->orientation()) * ((s.tab_ptr_Magnets[vm.magnet3])->orientation());
    if (p12 > 0 and p13 > 0) { return vm.magnet1; }
    if (p12 > 0 and p23 > 0) { return vm.magnet2; }
    if (p23 > 0 and p13 > 0) { return vm.magnet3; }
    return -1;
} // sends back oddmagnet index

int doublevertixstrength(doublevertix dv)
{
    if (Oddoneout(dv.v1) == dv.centralmagnet and Oddoneout(dv.v2) == dv.centralmagnet) { return 3; } // ff
    else if (Oddoneout(dv.v1) != dv.centralmagnet and Oddoneout(dv.v2) == dv.centralmagnet) {
        return 2; // fw
    } else if (Oddoneout(dv.v1) == dv.centralmagnet and Oddoneout(dv.v2) !=
      dv.centralmagnet)
    {
        return 1; // wf
    } else if (Oddoneout(dv.v1) != dv.centralmagnet and Oddoneout(dv.v2) !=
      dv.centralmagnet)
    {
        return 0; // ww
    }
    return -1;
}

bool doublevertixup(doublevertix dv, int magnetindex)
{
    Vecteur3D barycenterofdv = s.tab_ptr_Magnets[dv.centralmagnet]->position;
    Vecteur3D upwardOrientation(barycenterofdv - dode.position);
    double seperationAfterorientation1 =
      (s.tab_ptr_Magnets[magnetindex]->position
      + s.tab_ptr_Magnets[magnetindex]->orientation() * s.tab_ptr_Magnets[magnetindex]->length / 2
      - barycenterofdv ).norme();
    double seperationAfterorientation2 = // reorienting the central magnet orientation
      (-1 * s.tab_ptr_Magnets[magnetindex]->position
      + s.tab_ptr_Magnets[dv.centralmagnet]->orientation() * s.tab_ptr_Magnets[magnetindex]->length / 2
      + barycenterofdv ).norme();
    double seperationBetweencenters =
      (s.tab_ptr_Magnets[magnetindex]->position
      - barycenterofdv ).norme();
    int fixer = 1;
    if (seperationAfterorientation1 < seperationBetweencenters) { fixer *= -1; } // fixing branch magnet
    if (seperationAfterorientation2 > seperationBetweencenters) { fixer *= -1; } // fixing centralmagnet
    if (fixer
      * ((s.tab_ptr_Magnets[magnetindex]->orientation() ^ s.tab_ptr_Magnets[dv.centralmagnet]->orientation())
      * upwardOrientation) > 0) { return 1; }
    return 0;
}

int finddoublevertix(int magnetindex)
{
    for (size_t i = 0; i < DV.size(); i++) {
        if (DV[i].centralmagnet == magnetindex) {  return i; }
    }
    return -1;
}

int doublevertixstrengthRelative(doublevertix dv, int magnetindex)
{
    int strength = 0;
    int doublevertixindexofmagnet = finddoublevertix(magnetindex);
    if (doublevertixstrength(DV[doublevertixindexofmagnet]) ==
      0 or doublevertixstrength(DV[doublevertixindexofmagnet]) == 3)
    {
        strength = doublevertixstrength(DV[doublevertixindexofmagnet]);
    } else {
        if (Oddoneout(dv.v1) == DV[doublevertixindexofmagnet].centralmagnet) {
            strength = 2;
        } else { strength = 1; }
    }
    return strength;
}

int doublevertixstrengthDetailed(doublevertix dv)
{
    int detailed(doublevertixstrength(dv) * 100);
    if (doublevertixup(dv, (dv.v1).magnet1)) {
        // v1m1
        detailed += doublevertixstrengthRelative(dv, (dv.v1).magnet1);
        // v1m2
        detailed += doublevertixstrengthRelative(dv, (dv.v1).magnet2) * 10;
    } else {
        // v1m1
        detailed += doublevertixstrengthRelative(dv, (dv.v1).magnet1) * 10;
        // v1m2
        detailed += doublevertixstrengthRelative(dv, (dv.v1).magnet2);
    }

    if (doublevertixup(dv, (dv.v2).magnet1)) {
        // v2m1
        detailed += doublevertixstrengthRelative(dv, (dv.v2).magnet1) * 1000;
        // v2m2
        detailed += doublevertixstrengthRelative(dv, (dv.v2).magnet2) * 10000;
    } else {
        // v2m1
        detailed += doublevertixstrengthRelative(dv, (dv.v2).magnet1) * 10000;
        // v2m2
        detailed += doublevertixstrengthRelative(dv, (dv.v2).magnet2) * 1000;
    }
    return detailed;
}

// orientation of face
int getFaceOrientaion(facemagnets fm)
{
    // we can do it with vector product; seems elegant?
    vector<double> scalarproducts;
    Vecteur3D barycenterofface(0, 0, 0);
    size_t facesize = fm.doubleVertixVector.size();
    for (size_t i = 0; i < facesize; ++i) {
        double scalarp(s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->orientation()
          * s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % facesize].centralmagnet]->orientation());
        scalarproducts.push_back(scalarp);
        barycenterofface += s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->position;
    }
    barycenterofface = barycenterofface / facesize;
    int streaksize     = 0;
    int maxstreaksize  = 1;
    int maxstreakindex = 0;
    for (size_t i = 0; i < 2 * scalarproducts.size(); ++i) {
        if (scalarproducts[i % scalarproducts.size()] > 0) { streaksize += 1; }
        if (streaksize >= maxstreaksize) {
            maxstreaksize  = streaksize;
            maxstreakindex = i;
        }
        if (scalarproducts[i] < 0) { streaksize = 0; }
    }
    // for orientation:
    Vecteur3D upwardorientation(barycenterofface - dode.position);
    double orientation =
      (s.tab_ptr_Magnets[fm.doubleVertixVector[maxstreakindex % facesize].centralmagnet]->orientation()
      ^ s.tab_ptr_Magnets[fm.doubleVertixVector[(maxstreakindex + 1) % facesize].centralmagnet]->orientation())
      * upwardorientation;
    if (orientation > 0) { return maxstreaksize % (facesize + 1); } else {
        return -1 * maxstreaksize % (facesize + 1);
    }
} // getFaceOrientaion

// energies:
double vertixEnergy(vertixmagnets vm)
{
    double Energy = 0;
    Energy += s.tab_ptr_Magnets[vm.magnet1]->spotB(s.tab_ptr_Magnets[vm.magnet2]);
    Energy += s.tab_ptr_Magnets[vm.magnet1]->spotB(s.tab_ptr_Magnets[vm.magnet3]);
    Energy += s.tab_ptr_Magnets[vm.magnet2]->spotB(s.tab_ptr_Magnets[vm.magnet3]);
    return Energy;
}

double doublevertixEnergy(doublevertix dv)
{
    return vertixEnergy(dv.v1) + vertixEnergy(dv.v2);
}

double faceEnergy(facemagnets fm)
{
    double Energy = 0;
    for (size_t i = 0; i < fm.doubleVertixVector.size(); i++) {
        Energy +=
          s.tab_ptr_Magnets[fm.doubleVertixVector[i].centralmagnet]->
          spotB(s.tab_ptr_Magnets[fm.doubleVertixVector[(i + 1) % fm.doubleVertixVector.size()].centralmagnet]);
    }
    return Energy;
}

#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include "Vecteur3D.h"
#include <cmath>
#include "MagnetH.h"
#include <armadillo>

using namespace std;
using namespace arma;
MagnetH :: MagnetH(Position const& position, bool movable,
  Vecteur3D orientation1, Vecteur3D oldorientation1, Vecteur3D omega,
  double charge, double mass, double radius, double length,
  bool selected, Vecteur3D torque, Vecteur3D newtorque, Vecteur3D Bfield,
  SupportADessin * support, double f) :
    Magnet(position, movable, charge, mass, radius, length, selected, Bfield, support, f),
    omega(omega),
    torque(torque), oldtorque(Vecteur3D(0, 0, 0)), newtorque(newtorque), BTorque(0)
{
    orientation    = orientation1.normalise();
    oldorientation = oldorientation1.normalise();
}

ostream& MagnetH:: display(std :: ostream& c) const
{
    c << "Fixed Attributes" << endl
      << "Position of centre: " << position << endl
      << "inertia: " << inertia() << endl
      << "gamma: " << gamma() << endl
      << endl
      << "Motion" << endl
      << "orientation: " << orientationV() << endl
      << endl
      << "omega: " << omega << endl
      << endl
      << "Torque: " << torque << endl
      << "BTorque: " << BTorque << endl
      << endl
      << "Energy (*1e7)" << endl
      << "Kinetic Energy: " << Kinetic() * 1e7 << endl
      << "Potential Energy: " << (potBN + potBS) * 1e7 << endl;

    return c;
}

void MagnetH :: reset()
{
    // reinitialise
    torque    = Vecteur3D(0, 0, 0);
    newtorque = Vecteur3D(0, 0, 0);
    Bfield    = Vecteur3D(0, 0, 0);
    potBN     = 0;
    potBS     = 0;
}

void MagnetH :: resetnew()
{
    // reinitialise
    torque    = newtorque;
    newtorque = Vecteur3D(0, 0, 0);
    Bfield    = Vecteur3D(0, 0, 0);
    potBN     = 0;
    potBS     = 0;
}

// Torque3D
Vecteur3D MagnetH :: torquecalc3D(Vecteur3D r, double chargeM2, double chargeM1) const
{
    double pow = 1e-7;
    return ((length / 2) * pow) * (chargeM2 * chargeM1)
           * (  (orientationV() ^ r)) / (r.norme() * r.norme() * r.norme());
}

// torque and torquecalc inconsistent I think; Oh I understand now, you take S and N relative vector positions into account and
// N is positive while S is negative. Wow how did we miss that :(
void MagnetH :: addTorque(unique_ptr<Magnet> const& MagnetH2)
{
    Vecteur3D rNN = positionN() - MagnetH2->positionN(); // Npole i Npole j seperation
    Vecteur3D rNS = positionN() - MagnetH2->positionS(); // Npole i Spole j seperation
    Vecteur3D rSN = positionS() - MagnetH2->positionN(); // Spole i Npole j seperation
    Vecteur3D rSS = positionS() - MagnetH2->positionS(); // Spole i Spole j seperation

    torque += torquecalc3D(rNN, MagnetH2->chargeN(), chargeN());
    torque += torquecalc3D(rNS, MagnetH2->chargeS(), chargeN());
    torque -= torquecalc3D(rSN, MagnetH2->chargeN(), chargeS());
    torque -= torquecalc3D(rSS, MagnetH2->chargeS(), chargeS());
}

void MagnetH :: addnewTorque(unique_ptr<Magnet> const& MagnetH2)
{
    Vecteur3D rNN = positionN() - MagnetH2->positionN(); // Npole i Npole j seperation
    Vecteur3D rNS = positionN() - MagnetH2->positionS(); // Npole i Spole j seperation
    Vecteur3D rSN = positionS() - MagnetH2->positionN(); // Spole i Npole j seperation
    Vecteur3D rSS = positionS() - MagnetH2->positionS(); // Spole i Spole j seperation

    newtorque += torquecalc3D(rNN, MagnetH2->chargeN(), chargeN());
    newtorque += torquecalc3D(rNS, MagnetH2->chargeS(), chargeN());
    newtorque -= torquecalc3D(rSN, MagnetH2->chargeN(), chargeS());
    newtorque -= torquecalc3D(rSS, MagnetH2->chargeS(), chargeS());
}

void MagnetH :: addTorque(Vecteur3D extfield)
{
    double pow = 1;
    torque += pow * (moment() ^ extfield);
}

void MagnetH :: addnewTorque(Vecteur3D extfield)
{
    double pow = 1;
    newtorque += pow * (moment() ^ extfield);
    BTorque    = pow * (moment() ^ extfield);
}

// MAGNETIC FIELD
void MagnetH :: addpotBN(unique_ptr<Magnet> const& MagnetH2)
{
    Vecteur3D rN = MagnetH2->positionN() - positionN();
    Vecteur3D rS = MagnetH2->positionS() - positionN();
    potBN += 1e-7 * chargeN() * MagnetH2->chargeN() / rN.norme();
    potBN += 1e-7 * chargeN() * MagnetH2->chargeS() / rS.norme();
}

void MagnetH :: addpotBS(unique_ptr<Magnet> const& MagnetH2)
{
    Vecteur3D rN = MagnetH2->positionN() - positionS();
    Vecteur3D rS = MagnetH2->positionS() - positionS();
    potBS += 1e-7 * chargeS() * MagnetH2->chargeN() / rN.norme();
    potBS += 1e-7 * chargeS() * MagnetH2->chargeS() / rS.norme();
}

double MagnetH :: spotB(std::unique_ptr<Magnet> const& MagnetH2)
{
    double potential = 0;
    Vecteur3D rNN    = MagnetH2->positionN() - positionN();
    Vecteur3D rSN    = MagnetH2->positionS() - positionN();
    Vecteur3D rNS    = MagnetH2->positionN() - positionS();
    Vecteur3D rSS    = MagnetH2->positionS() - positionS();
    potential += 1e-7 * chargeN() * MagnetH2->chargeN() / rNN.norme();
    potential += 1e-7 * chargeN() * MagnetH2->chargeS() / rNS.norme();
    potential += 1e-7 * chargeS() * MagnetH2->chargeN() / rSN.norme();
    potential += 1e-7 * chargeS() * MagnetH2->chargeS() / rSS.norme();
    return potential;
}

// VERLET CALCULATIONS


void MagnetH :: moveomega(double delta_t)
{
    if (movable) {
        Vecteur3D omega_half = omega + 0.5 * delta_t * accel(torque, omega, delta_t);
        omega += 0.5 * delta_t * (torqueaccel() + newtorqueaccel() + 2 * dampingaccel(omega_half, delta_t));
    }
}

void MagnetH :: addBfield(unique_ptr<Magnet> const& MagnetH2)
{
    // for consistency maybe use the same model as addforce even beter would be
    // having addBfieldN and addBfieldS and use these in addforce
    Vecteur3D rN = MagnetH2->positionN() - position;
    Vecteur3D rS = MagnetH2->positionS() - position;
    Bfield += 1e-7 * MagnetH2->chargeN() * rN / (rN.norme() * rN.norme() * rN.norme());
    Bfield += 1e-7 * MagnetH2->chargeS() * rS / (rS.norme() * rS.norme() * rS.norme());
}

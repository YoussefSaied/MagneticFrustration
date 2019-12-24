#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include "Vecteur3D.h"
#include <cmath>
#include "Magnet.h"
#include "MagnetA.h"

using namespace std;
MagnetA :: MagnetA(Position const& position, Vecteur3D axis, bool movable, double angle,
  Vecteur3D polaraxis1, double charge, double mass, double radius, double length,
  bool selected, double torque, double newtorque, Vecteur3D Bfield,
  double omega, SupportADessin * support, double f) :
    Magnet(position, movable, charge, mass, radius, length, selected, Bfield, support, f), axis(axis.normalise()),
    angle(angle), omega(omega),
    torque(torque), oldtorque(0), newtorque(newtorque)

{
    if (polaraxis1.normalise() * axis.normalise() <= 0.01) {
        polaraxis = polaraxis1.normalise();
    }
    else {
        polaraxis = (polaraxis1.GS1(axis)).normalise();
    }
}

ostream& MagnetA:: display(std :: ostream& c) const
{
    c << "Fixed Attributes" << endl
      << "Position of centre: " << position << endl
      << "axis: " << axis << endl
      << "inertia: " << inertia() << endl
      << "gamma: " << gamma() << endl
      << endl
      << "Motion" << endl
      << "angle: " << std::fmod(angle, M_PI) << endl
      << "orientation: " << orientationV() << endl
      << endl
      << "omega: " << omega << endl
      << endl
      << "Torque: " << torque << endl
      << "realTorque: " << displ_accel(newtorque, omega) * inertia() << endl
      << "BTorque: " << BTorque << endl
      << endl
      << "acc(T): " << torqueaccel(newtorque) << endl
      << "acc(W): " << dampingaccel(omega) << endl
      << "acc(T,W): " << displ_accel(newtorque, omega) << endl
      << endl
      << "Energy (*1e7)" << endl
      << "Kinetic Energy: " << Kinetic() * 1e7 << endl
      << "Potential Energy: " << (potBN + potBS) * 1e7 << endl;

    return c;
}

void MagnetA :: reset()
{
    // reinitialise
    torque    = 0.0;
    newtorque = 0.0;
    Bfield    = Vecteur3D(0, 0, 0);
    potBN     = 0;
    potBS     = 0;
}

void MagnetA :: resetnew()
{
    // reinitialise
    torque    = newtorque;
    newtorque = 0.0;
    Bfield    = Vecteur3D(0, 0, 0);
    potBN     = 0;
    potBS     = 0;
}

// plane vectors
Vecteur3D MagnetA :: planevec1() const
{
    if (axis != Vecteur3D(1, 0, 0)) {
        return (axis ^ Vecteur3D(1, 0, 0)).normalise();
    }
    else {
        return (axis ^ Vecteur3D(0, 1, 0)).normalise();
    }
}

Vecteur3D MagnetA :: planevec2() const
{
    return (axis ^ polaraxis).normalise();
}

// Torque3D
Vecteur3D MagnetA :: torquecalc3D(Vecteur3D r, double chargeM2, double chargeM1) const
{
    double pow = 1e-7;
    return ((length / 2) * pow) * (chargeM2 * chargeM1)
           * (  (orientationV() ^ r)) / (r.norme() * r.norme() * r.norme());
}

// TORQUE, one degree of freedom
double MagnetA :: torquecalc(Vecteur3D r, double chargeM2, double chargeM1) const
{
    return torquecalc3D(r, chargeM2, chargeM1) * zaxis(); // projecting the 3D torque on the axis
}

// torque and torquecalc inconsistent I think; Oh I understand now, you take S and N relative vector positions into account and
// N is positive while S is negative. Wow how did we miss that :(
void MagnetA :: addTorque(unique_ptr<Magnet> const& Magnet2)
{
    Vecteur3D rNN = positionN() - Magnet2->positionN(); // Npole i Npole j
    Vecteur3D rNS = positionN() - Magnet2->positionS(); // Npole i Spole j
    Vecteur3D rSN = positionS() - Magnet2->positionN(); // Spole i Npole j
    Vecteur3D rSS = positionS() - Magnet2->positionS(); // Spole i Spole j

    torque += torquecalc(rNN, Magnet2->chargeN(), chargeN());
    torque += torquecalc(rNS, Magnet2->chargeS(), chargeN());
    // -= because S
    torque -= torquecalc(rSN, Magnet2->chargeN(), chargeS());
    torque -= torquecalc(rSS, Magnet2->chargeS(), chargeS());
}

void MagnetA :: addnewTorque(unique_ptr<Magnet> const& Magnet2)
{
    Vecteur3D rNN = positionN() - Magnet2->positionN(); // Npole i Npole j
    Vecteur3D rNS = positionN() - Magnet2->positionS(); // Npole i Spole j
    Vecteur3D rSN = positionS() - Magnet2->positionN(); // Spole i Npole j
    Vecteur3D rSS = positionS() - Magnet2->positionS(); // Spole i Spole j

    newtorque += torquecalc(rNN, Magnet2->chargeN(), chargeN());
    newtorque += torquecalc(rNS, Magnet2->chargeS(), chargeN());
    // -= because S
    newtorque -= torquecalc(rSN, Magnet2->chargeN(), chargeS());
    newtorque -= torquecalc(rSS, Magnet2->chargeS(), chargeS());
}

void MagnetA :: addTorque(Vecteur3D extfield)
{
    double pow = 1;
    torque += pow * zaxis() * (moment() ^ extfield);
}

void MagnetA :: addnewTorque(Vecteur3D extfield)
{
    double pow = 1;
    newtorque += pow * zaxis() * (moment() ^ extfield);
    BTorque    = pow * zaxis() * (moment() ^ extfield);
}

// MAGNETIC FIELD
void MagnetA :: addpotBN(unique_ptr<Magnet> const& Magnet2)
{
    Vecteur3D rN = Magnet2->positionN() - positionN();
    Vecteur3D rS = Magnet2->positionS() - positionN();
    potBN += 1e-7 * chargeN() * Magnet2->chargeN() / rN.norme();
    potBN += 1e-7 * chargeN() * Magnet2->chargeS() / rS.norme();
}

void MagnetA :: addpotBS(unique_ptr<Magnet> const& Magnet2)
{
    Vecteur3D rN = Magnet2->positionN() - positionS();
    Vecteur3D rS = Magnet2->positionS() - positionS();
    potBS += 1e-7 * chargeS() * Magnet2->chargeN() / rN.norme();
    potBS += 1e-7 * chargeS() * Magnet2->chargeS() / rS.norme();
}

double MagnetA :: spotB(std::unique_ptr<Magnet> const& Magnet2)
{
    double potential = 0;
    Vecteur3D rNN    = Magnet2->positionN() - positionN();
    Vecteur3D rSN    = Magnet2->positionS() - positionN();
    Vecteur3D rNS    = Magnet2->positionN() - positionS();
    Vecteur3D rSS    = Magnet2->positionS() - positionS();
    potential += 1e-7 * chargeN() * Magnet2->chargeN() / rNN.norme();
    potential += 1e-7 * chargeN() * Magnet2->chargeS() / rNS.norme();
    potential += 1e-7 * chargeS() * Magnet2->chargeN() / rSN.norme();
    potential += 1e-7 * chargeS() * Magnet2->chargeS() / rSS.norme();
    return potential;
}

// VERLET CALCULATIONS
void MagnetA :: VerletBU(double delta_t)
{
    if (movable) {
        angle += delta_t * omega + 1 / 2 * delta_t * delta_t * accel(torque, omega);
    }

    double zeta = (delta_t * gamma()) / (2 * inertia());
    omega = (1 / (1 + zeta)) * ((omega * (1 - zeta)) + (delta_t / (2 * inertia())) * (oldtorque + torque));
}

void MagnetA :: move(double delta_t) // works!!
{
    if (movable) {
        double omega_half = omega + 0.5 * delta_t * accel(oldtorque, omega);
        omega += 0.5 * delta_t * (torqueaccel(torque) + torqueaccel(oldtorque) + 2 * dampingaccel(omega_half));
        angle += delta_t * omega + 0.5 * delta_t * delta_t * accel(torque, omega);
    }
}

void MagnetA :: moveomega(double delta_t)
{
    if (movable) {
        double omega_half = omega + 0.5 * delta_t * accel(torque, omega);
        omega += 0.5 * delta_t * (torqueaccel(torque) + torqueaccel(newtorque) + 2 * dampingaccel(omega_half));
    }
}

void MagnetA :: addBfield(unique_ptr<Magnet> const& Magnet2)
{
    // for consistency maybe use the same model as addforce even beter would be
    // having addBfieldN and addBfieldS and use these in addforce
    Vecteur3D rN = Magnet2->positionN() - position;
    Vecteur3D rS = Magnet2->positionS() - position;
    Bfield += 1e-7 * Magnet2->chargeN() * rN / (rN.norme() * rN.norme() * rN.norme());
    Bfield += 1e-7 * Magnet2->chargeS() * rS / (rS.norme() * rS.norme() * rS.norme());
}

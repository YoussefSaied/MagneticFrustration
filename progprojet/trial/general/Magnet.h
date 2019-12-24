#pragma once
#define _USE_MATH_DEFINES
#include "Vecteur3D.h"
#include <iostream>
#include "Dessinable.h"
#include "SupportADessin.h"
#include "Obstacle.h"
#include <memory>
#include <cmath>

/* This is the general abstract magnet class, this class will be inherited by >=2 other classes:
MagnetA which is the single degree of freedom type of magnet. These magnets can only rotate about one axe.
MagnetH which has 2 degrees of freedom. These magnets can rotate freely but there center cannot move.
*/
// Paula's magnet: charge = 2, radius = 0.75e-3, mass = 0.3e-3, length = 1.9e-2
// IMPORTANT: T:= normally torque; W:= normally omega/ angular acceleration
class Magnet : public Dessinable
{
public:
    // Constructor and destructor
    Magnet(Position const & position, bool movable = 1,
      double charge = 2.0, double mass = 0.3e-3,
      double radius = 0.75e-3, double length = 1.9e-2, bool selected = 0,
      Vecteur3D const& Bfield = Vecteur3D(0, 0, 0), SupportADessin * support = &Texte1, double f = 0) :
        Dessinable(position, support), movable(movable), charge(charge), mass(mass), radius(radius), length(length), selected(selected),
        f(f), potBN(0), potBS(0), Bfield(Bfield)
    { }


    virtual ~Magnet(){ }

    // Non-pure Virtual Methods
    double inertia() const
    { return mass * length * length / 12; }

    double gamma() const
    { return f * inertia(); }


    virtual double chargeN() const
    { return charge; }

    virtual double chargeS() const
    { return charge * -1; }

    double get_axerheight() const { return radius * 6; }

    double get_axerradius() const { return length / 20; }

    void set_support(SupportADessin * s){ support = s; }

    double get_length() const { return length; }

    // Virtual Methods
    virtual double omegaS() const = 0;

    virtual double Kinetic() const = 0;

    virtual Vecteur3D orientationV() const = 0;

    virtual Vecteur3D moment() const = 0;

    virtual Vecteur3D positionN() const = 0;

    virtual Vecteur3D positionS() const = 0;


    // CALC METHODS

    virtual void addTorque(std::unique_ptr<Magnet> const& Magnet2) = 0;

    virtual void addnewTorque(std::unique_ptr<Magnet> const& Magnet2) = 0;

    virtual void addBfield(std::unique_ptr<Magnet> const& Magnet2) = 0;

    virtual void addpotBN(std::unique_ptr<Magnet> const& Magnet2) = 0;

    virtual void addpotBS(std::unique_ptr<Magnet> const& Magnet2) = 0;

    virtual void addpotB(std::unique_ptr<Magnet> const& Magnet2) = 0;

    virtual double spotB(std::unique_ptr<Magnet> const& Magnet2) = 0;


    double potB() const { return potBN + potBS; }

    virtual void addTorque(Vecteur3D extfield) = 0;

    virtual void addnewTorque(Vecteur3D extfield) = 0;

    virtual void addBfield(Vecteur3D const&  extfield){ Bfield += extfield; }

    virtual void reset() = 0;

    virtual void resetnew() = 0;

    virtual void moveangle(double delta_t) = 0;

    virtual void moveomega(double delta_t) = 0;

    virtual void setangle(double a)=0;

    virtual double getangle() =0;

    virtual void setomega(double a)=0;

    // 3D methodes
    virtual Vecteur3D torquecalc3D(Vecteur3D r, double chargeM2, double chargeM1) const = 0;

    virtual std::unique_ptr<Magnet> copie() const = 0;

    // method to display the Magnet's data
    virtual std :: ostream& display(std :: ostream& sortie) const = 0;


    // attributes
    bool movable;
    double charge;
    double mass;
    double radius;
    double length;
    bool selected;
    double f;
    double potBN;
    double potBS;
    Vecteur3D Bfield;
};

#pragma once
#define _USE_MATH_DEFINES
#include "Vecteur3D.h"
#include <iostream>
#include "Dessinable.h"
#include "SupportADessin.h"
#include "Obstacle.h"
#include "Magnet.h"
#include <memory>
#include <cmath>

// typedef Vecteur3D Position, Vitesse;

// Paula's magnet: charge = 2, radius = 0.75e-3, mass = 0.3e-3, length = 1.9e-2
// IMPORTANT: T:= normally torque; W:= normally omega/ angular acceleration
class MagnetA : public Magnet
{
public:
  // Constructor and destructor
    MagnetA(Position const & position, Vecteur3D axis = Vecteur3D(0, 0, 1), bool movable = 1, double angle = 0.0001,
      Vecteur3D polaraxis1 = Vecteur3D(0, 0, 1), double charge = 2.0, double mass = 0.3e-3,
      double radius = 0.75e-3, double length = 1.9e-2, bool selected = 0, double torque = 0,
      double newtorque = 0, Vecteur3D Bfield = Vecteur3D(0, 0, 0),
      double omega = 0, SupportADessin * support = &Texte1, double f = 0);
    virtual ~MagnetA(){ }


    virtual double omegaS() const override { return omega; }

    double Kinetic() const override
    { return 0.5 * inertia() * omegaS() * omegaS(); }


    // ACCELERATION
    double accel(double T, double W) const// angular acceleration
    { return (1 / inertia()) * (T - gamma() * W); }

    double torqueaccel(double T) const// angular acceleration
    { return (1 / inertia()) * T; }

    double dampingaccel(double W) const// angular acceleration
    { return -1 * f * W; }

    double displ_accel(double T, double W) const// angular acceleration
    { return (T / inertia()) - f * W; }

    // orientation attributes (unit vectors)
    Vecteur3D planevec1() const; // polaraxis
    Vecteur3D planevec2() const;
    Vecteur3D zaxis() const
    { return polaraxis ^ planevec2(); }


    Vecteur3D orientationV() const override
    { return polaraxis * cos(angle) + planevec2() * sin(angle); }

    Vecteur3D moment() const override
    { return orientationV() * chargeN() * length; }

    // charge attibutes:

    //North charge position
    Vecteur3D positionN() const override
    { return position + orientationV() * length / 2; }

    //South charge position
    Vecteur3D positionS() const override
    { return position - orientationV() * length / 2; }

    double chargeN() const override
    { return charge; }

    double chargeS() const override
    { return charge * -1; }

    //The height as a function of the radius if radius is provided (Paula's)
    double get_axerheight() const { return radius * 6; }

    //The radius as a function of the height if height is provided (Paula's)
    double get_axerradius() const { return length / 20; }

    // The class that would 'draw' the magnet (textual or graphical)
    void set_support(SupportADessin * s){ support = s; }

    virtual void setangle(double a) override {angle =a;}

    virtual double getangle() override {return angle;}

    virtual void setomega(double o) override {omega =o;}

    double get_length() const { return length; }

    // CALC METHODS
    double torquecalc(Vecteur3D r, double chargeM2, double chargeM1) const;

    virtual void addTorque(std::unique_ptr<Magnet> const& MagnetA2) override;

    virtual void addnewTorque(std::unique_ptr<Magnet> const& MagnetA2) override;

    virtual void addBfield(std::unique_ptr<Magnet> const& MagnetA2) override;

    virtual void addpotBN(std::unique_ptr<Magnet> const& MagnetA2) override;

    virtual void addpotBS(std::unique_ptr<Magnet> const& MagnetA2) override;

    virtual void addpotB(std::unique_ptr<Magnet> const& MagnetA2) override
    {
        addpotBN(MagnetA2);
        addpotBS(MagnetA2);
    }

    virtual double spotB(std::unique_ptr<Magnet> const& MagnetA2) override;


    virtual void addTorque(Vecteur3D extfield) override;

    virtual void addnewTorque(Vecteur3D extfield) override;

    virtual void reset() override;

    virtual void resetnew() override;

    virtual void VerletBU(double delta_t);
    virtual void move(double delta_t);
    virtual void moveangle(double delta_t) override
    {
        if (movable) {
            angle += delta_t * omega + 0.5 * delta_t * delta_t * accel(torque, omega);
        }
    }

    virtual void moveomega(double delta_t) override;

    virtual void dessine() const override
    {
        if (support != nullptr) { support->dessine(*this); }
    }

    // 3D methodes
    virtual Vecteur3D torquecalc3D(Vecteur3D r, double chargeM2, double chargeM1) const override;

    // method to display the MagnetA's data
    std :: ostream& display(std :: ostream& sortie) const override;

    // copie polymorphique
    std:: unique_ptr<MagnetA> cloneMe() const { return std::unique_ptr<MagnetA>(new MagnetA(*this)); }

    virtual std::unique_ptr<Magnet> copie() const override { return cloneMe(); }

    // attributes
    Vecteur3D axis;
    double angle;
    double omega;
    Vecteur3D polaraxis;
    double torque;
    double oldtorque;
    double newtorque;
    double BTorque;
};

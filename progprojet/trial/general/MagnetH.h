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
#include <armadillo>

// Paula's magnet: charge = 2, radius = 0.75e-3, mass = 0.3e-3, length = 1.9e-2
// IMPORTANT: T:= normally torque; W:= normally omega/ angular acceleration
class MagnetH : public Magnet
{
public:
  // Constructor and destructor
    MagnetH(Position const& position, bool movable = 1,
      // double angle1 = 0.0001, double angle2 = 0.00001,
      Vecteur3D orientation = Vecteur3D(0, 0, 1), Vecteur3D oldorientation = Vecteur3D(0, 0, 1),
      Vecteur3D omega = Vecteur3D(0, 0, 0),
      double charge = 2.0, double mass = 0.3e-3,
      double radius = 0.75e-3, double length = 1.9e-2, bool selected = 0,
      Vecteur3D torque = Vecteur3D(0, 0, 0), Vecteur3D newtorque = Vecteur3D(0, 0, 0),
      Vecteur3D Bfield = Vecteur3D(0, 0, 0),
      SupportADessin * support = &Texte1, double f = 0);
    virtual ~MagnetH(){ }

    // derived attributes

    virtual arma::Mat<double> inertiaMatrix() const
    {
        arma::Mat<double> CartesianInertia(3, 3, arma::fill::eye);
        CartesianInertia(0, 0) = inertia();
        CartesianInertia(1, 1) = inertia();
        CartesianInertia(2, 2) = inertia()*6.0;
        arma::Mat<double> rotationMatrix = Vecteur3D(0, 0, 1).rotationMatrix(orientation);
        return rotationMatrix * CartesianInertia * rotationMatrix.i();
    }

    virtual arma::Mat<double> OldinertiaMatrix() const
    {
        arma::Mat<double> CartesianInertia(3, 3, arma::fill::eye);
        CartesianInertia(0, 0) = inertia();
        CartesianInertia(1, 1) = inertia();
        CartesianInertia(2, 2) = inertia()*6.0;
        arma::Mat<double> rotationMatrix = Vecteur3D(0, 0, 1).rotationMatrix(oldorientation);
        return rotationMatrix * CartesianInertia * rotationMatrix.i();
    }

    virtual arma::Mat<double> gammaMatrix() const
    {
        arma::Mat<double> CartesianGamma(3, 3, arma::fill::eye);
        CartesianGamma(0, 0) = gamma();
        CartesianGamma(1, 1) = gamma();
        CartesianGamma(2, 2) = gamma();
        arma::Mat<double> rotationMatrix = Vecteur3D(0, 0, 1).rotationMatrix(orientation);
        return rotationMatrix * CartesianGamma * rotationMatrix.i();
    }

    //scalar value of omega
    double omegaS() const override
    {
        return omega.norme();
    }

    //Vector form of the orientation
    Vecteur3D orientationV() const override
    {
        return orientation;
    }

    //kinetic energy of the magnet
    double Kinetic() const override
    { return 0.5 * inertia() * omegaS() * omegaS(); }

    ////////////////////

    // ACCELERATION
    Vecteur3D accel(Vecteur3D const& T, Vecteur3D const& W, double dt) const// angular acceleration
    { return (inertiaMatrix()).i() * (T - ((inertiaMatrix() - OldinertiaMatrix()) / dt + gammaMatrix()) * W); }

    Vecteur3D torqueaccel() const// angular acceleration
    { return (OldinertiaMatrix()).i() * torque; }

    Vecteur3D newtorqueaccel() const// angular acceleration
    { return (inertiaMatrix()).i() * newtorque; }

    Vecteur3D dampingaccel(Vecteur3D const& W, double dt) const// angular acceleration
    {  return (inertiaMatrix()).i() * (-1 * ((inertiaMatrix() - OldinertiaMatrix()) / dt + gammaMatrix()) * W); }

    double displ_accel(double T, double W) const// angular acceleration
    { return (T / inertia()) - f * W; }


    virtual Vecteur3D moment() const override
    { return orientation * chargeN() * length; }

    // charge attibutes
    virtual Vecteur3D positionN() const override
    { return position + orientation * length / 2; }

    virtual Vecteur3D positionS() const override
    { return position - orientation * length / 2; }


    // CALC METHODS

    virtual void addTorque(std::unique_ptr<Magnet> const& MagnetH2) override;

    virtual void addnewTorque(std::unique_ptr<Magnet> const& MagnetH2) override;

    virtual void addBfield(std::unique_ptr<Magnet> const& MagnetH2) override;

    virtual void addpotBN(std::unique_ptr<Magnet> const& MagnetH2) override;

    virtual void addpotBS(std::unique_ptr<Magnet> const& MagnetH2) override;

    virtual void addpotB(std::unique_ptr<Magnet> const& MagnetH2) override
    {
        addpotBN(MagnetH2);
        addpotBS(MagnetH2);
    }

    virtual double spotB(std::unique_ptr<Magnet> const& MagnetH2) override;


    virtual double potB() const { return potBN + potBS; }

    virtual void addTorque(Vecteur3D extfield) override;

    virtual void addnewTorque(Vecteur3D extfield) override;

    virtual void reset() override;

    virtual void resetnew() override;

    virtual void moveangle(double delta_t) override
    {
        if (movable) {
            orientation += delta_t * (omega ^ orientation) + 0.5 * delta_t * delta_t
              * (accel(torque, omega, delta_t) ^ orientation);
            oldorientation = orientation;
        }
    }

    virtual void moveomega(double delta_t) override;

    virtual void dessine() const override
    {
        if (support != nullptr) { support->dessine(*this); }
    }

    // 3D methodes
    virtual Vecteur3D torquecalc3D(Vecteur3D r, double chargeM2, double chargeM1) const override; // r is the position

    // method to display the MagnetH's data
    std :: ostream& display(std :: ostream& sortie) const override;

    // copie polymorphique
    std:: unique_ptr<MagnetH> cloneMe() const { return std::unique_ptr<MagnetH>(new MagnetH(*this)); }

    virtual std::unique_ptr<Magnet> copie() const override { return cloneMe(); }

    virtual void setangle(double a) override{}

    virtual double getangle() override{return 0;}

    virtual void setomega(double o) override{omega =Vecteur3D(o,o,o);}


    // 3D attributes
    Vecteur3D orientation;
    Vecteur3D oldorientation;
    Vecteur3D omega;
    Vecteur3D torque;
    Vecteur3D oldtorque;
    Vecteur3D newtorque;
    Vecteur3D BTorque;
};


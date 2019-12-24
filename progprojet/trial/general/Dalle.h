#pragma once
#include "Vecteur3D.h"
#include "Plan.h"
#include "Obstacle.h"
#include <cmath>
#include <iostream>
#include <memory>

class Dalle : public Plan
{
public:
    //constructeur et destructeur

    Dalle(Vecteur3D position, Vecteur3D v1, Vecteur3D v2, double longueur = 1.0, double largeur = 1.0, bool m =false);

    virtual ~Dalle() {}


	//methodes

    virtual void dessine() const override { if (support != nullptr) support->dessine(*this); }

    //indiquant le vecteur de longueur
    Vecteur3D get_vecteur_2() const { return vecteur_2; }

    //vecteur indiquant le largeur
    Vecteur3D vecteur_3() const { return (vecteur_1^vecteur_2).normalise(); }

    double get_longueur() const { return longueur; }

    double get_largeur() const { return largeur; }

    Vecteur3D PointPlusProche(Vecteur3D const& x_i) const override; // point sur la dalle qui est le plus proche au point x_i

    //methodes qui retournent les 4 sommets de la dalle
    Vecteur3D Sommet1() const { return position-longueur/2*vecteur_2- largeur/2*vecteur_3(); }

    Vecteur3D Sommet3() const { return position + longueur/2*vecteur_2 + largeur/2*vecteur_3(); }

    Vecteur3D Sommet4() const { return position -longueur/2*vecteur_2 + largeur/2*vecteur_3(); }

    Vecteur3D Sommet2() const { return position + longueur/2*vecteur_2 - largeur/2*vecteur_3(); }

    std:: unique_ptr<Dalle> cloneMe() const;

    virtual std::unique_ptr<Obstacle> copie() const override;

    virtual std::ostream& display (std::ostream& c) const override;

protected:
    Vecteur3D vecteur_2; //indiquant le vecteur de longueur
    double longueur;
    double largeur;
};

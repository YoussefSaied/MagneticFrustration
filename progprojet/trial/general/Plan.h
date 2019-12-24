#pragma once
#include "Vecteur3D.h"
#include "Obstacle.h"
#include <iostream>
#include <memory>

class Plan : public Obstacle
{
public:
	//constructeur et destructeur
	Plan(Position const& position, Vecteur3D const& vector1, bool m= false): Obstacle(position,m), vecteur_1(vector1.normalise()) { }
    virtual ~Plan() {}


	//methodes
    virtual void dessine() const override { if (support != nullptr) support->dessine(*this); }

    virtual Vecteur3D PointPlusProche(Vecteur3D const& x_i) const override; // le point le plus proche dans le plan à x_i

    virtual std::ostream& display (std::ostream& c) const override;

    Vecteur3D get_vecteur_1() const { return vecteur_1; }

    std:: unique_ptr<Plan> cloneMe() const;

    virtual std::unique_ptr<Obstacle> copie() const override;


protected:
    Vecteur3D vecteur_1; // vecteur unitaire perpendiculaire au plan
};

//externes
std :: ostream& operator<<(std :: ostream& c, Plan const& plan1 );

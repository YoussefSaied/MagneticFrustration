#pragma once
#include "Vecteur3D.h"
#include "Obstacle.h"
#include <iostream>
#include <memory>

class Cylinder : public Obstacle{
public:
	//constructeur et destructeur
	Cylinder(Position position, Vecteur3D vector1, double h, double r, bool m= false);
	virtual ~Cylinder() {}

	//methodes
	virtual void dessine() const override {if (support != nullptr) support->dessine(*this);}

	virtual Vecteur3D PointPlusProche(Vecteur3D const& x_i) const override; // The closest point on the cylinder to x_i

    virtual std::ostream& display (std::ostream& sortie) const override;

	Vecteur3D get_vecteur_1() const { return vector1n; }

	double get_radius() const {	return rayon; }

	double get_hauteur() const { return hauteur; }

	std:: unique_ptr<Cylinder> cloneMe() const;

	virtual std::unique_ptr<Obstacle> copie() const override;


protected:
	Vecteur3D vector1n; // unitary perpendiculaire vector au plan
	double hauteur;
	double rayon;
};

//externes
std :: ostream& operator<<(std :: ostream& c, Cylinder const& cyl1 );

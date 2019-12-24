#pragma once
#include "Vecteur3D.h"
#include "Plan.h"
#include "Dalle.h"
#include "Obstacle.h"
#include <cmath>
#include <vector>

class Brique: public Obstacle{
	public:
	//constructeur et destructeur
	Brique(Dalle const& base, double hauteur, bool m= false): Obstacle(base.get_position(), m), base(base), hauteur(hauteur) {}
	virtual ~Brique(){}

	//methodes
	virtual void dessine() const override {if (support != nullptr) support->dessine(*this);}
	Vecteur3D get_vecteur_1() const {return base.get_vecteur_1();}
	Vecteur3D get_vecteur_2() const {return base.get_vecteur_2();}
	Vecteur3D get_vecteur_3() const {return base.vecteur_3();}
	Vecteur3D get_position() const {return base.get_position() + hauteur/2* base.get_vecteur_1();}
	double get_longueur() const {return base.get_longueur();}
	double get_largeur() const {return base.get_largeur();}
	bool is_inside(Vecteur3D const& x_i) const; // methode qui verifie si un point est  l'interieur de la brique
	virtual Vecteur3D PointPlusProche(Vecteur3D const& x_i) const override; // returne le point sur la dalle qui est le plus proche du point x_i
	std::vector<Dalle> dalle6() const; // methode qui retourne un vecteur de 6 dalles qui forment la brique
	std:: unique_ptr<Brique> cloneMe() const;
	virtual std::unique_ptr<Obstacle> copie() const override;
    virtual std::ostream& display(std::ostream& sortie) const override{ sortie<<"I am a brick"; return sortie;};


protected:
	Dalle base;
	double hauteur; // hauteur de la brique
};

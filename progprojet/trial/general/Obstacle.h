#pragma once

#include "Vecteur3D.h"
#include "Dessinable.h"
#include <iostream>
#include <memory>
#include <cmath>
class Obstacle : public Dessinable
{
public:
    //constructeur et destructeur
    Obstacle(Position const& position, bool m= false): Dessinable(position), xinit(position), movable(m) {
		}
    virtual ~Obstacle() {}

	//methodes
    void set_support(SupportADessin* s) { support = s; }

    virtual Vecteur3D PointPlusProche(Vecteur3D const& x_i) const = 0; // retourne le point le plus proche de x_i qui est dans le plan

    virtual std::unique_ptr<Obstacle> copie() const = 0;

    virtual std::ostream& display (std::ostream& c) const =0;

		void set_position(Vecteur3D v){
			position = v;
		}
		void set_vitesse(Vecteur3D v){
			speed = v;
		}
		Vecteur3D get_vitesse(){
			return speed;
		}
		void move_harmonically(Vecteur3D dir= Vecteur3D(0,0,1.0), double d= 0.1, double f= 10.0){
			speed= (f*pow( d*d - (position-xinit)*(position-xinit), 0.5))* dir.normalise();
		}
		void bouger(double temps){
			if(movable){
			position += (temps*speed);
			//move_harmonically();
		}
		}

protected:

	Vecteur3D speed;
	Vecteur3D xinit;
	bool movable;
};

#include "Dalle.h"
#include "Vecteur3D.h"
#include "Plan.h"
#include <cmath>
#include <memory>
#include <iostream>
using namespace std;

Dalle:: Dalle(Vecteur3D position, Vecteur3D v1, Vecteur3D v2, double longueur, double largeur, bool m): Plan(position, v1, m),
    longueur(longueur), largeur(largeur)
{
    vecteur_2 = v2.normalise();
}

Vecteur3D Dalle :: PointPlusProche(Vecteur3D const& x_i) const
{
    Vecteur3D x0 = x_i +((Sommet1()-x_i)*vecteur_1)*vecteur_1;
    double i((x0-Sommet1())*vecteur_2);
    double j((x0-Sommet1())*vecteur_3());
    if(i > longueur) x0-= (i-longueur)*vecteur_2;
    else if(i < 0) x0-= i*vecteur_2;
    if(j > largeur) x0-= (j-largeur)*vecteur_3();
    else if(j < 0) x0-= i*vecteur_3();
    return x0;
}

unique_ptr<Dalle> Dalle:: cloneMe() const
{
    return unique_ptr<Dalle>(new Dalle(*this));
}

unique_ptr<Obstacle> Dalle:: copie() const
{
    return cloneMe();
}

ostream& Dalle::display (ostream& c) const
{
    c<<"Dalle d'origine "<<position<<" de normale " << vecteur_1<< " Largeur : "<<largeur<< " Longueur : "<<longueur<<endl;
    return c;
}

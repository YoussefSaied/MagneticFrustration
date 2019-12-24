#include "Brique.h"
#include "Vecteur3D.h"
#include "Plan.h"
#include "Dalle.h"
#include "Obstacle.h"
#include <cmath>
#include <vector>
#include <memory>
using namespace std;

unique_ptr<Brique> Brique:: cloneMe() const
{
    return unique_ptr<Brique>(new Brique(*this));
}

unique_ptr<Obstacle> Brique:: copie() const
{
    return cloneMe();
}

vector<Dalle> Brique:: dalle6() const
{
    vector<Dalle> d6;
    //1
    d6.push_back(base);

    //2
    d6.push_back(Dalle((get_position()+hauteur/2*get_vecteur_1()) , -1*get_vecteur_1()
                       , get_vecteur_2(),get_longueur(), get_largeur()));
    //3
    d6.push_back(Dalle( (get_position() + (get_longueur()/2)*get_vecteur_2()) , -1*get_vecteur_2()
                       , -1*get_vecteur_3(),get_largeur(), hauteur));

    //4
    d6.push_back(Dalle((get_position()+ (get_largeur()/2)*get_vecteur_3()) , get_vecteur_3()
                       , -1*get_vecteur_1(),hauteur, get_longueur()));

    //5
    d6.push_back(Dalle((get_position() +(-1 *get_longueur()/2)*get_vecteur_2()) , get_vecteur_2()
                       , -1*get_vecteur_3(),get_largeur(), hauteur));

    //6
    d6.push_back(Dalle((get_position() + (get_largeur()/2)*get_vecteur_3()) , -1*get_vecteur_3()
                       , -1*get_vecteur_1(),hauteur, get_longueur()));

    return d6;
}

bool Brique:: is_inside(Vecteur3D const& x_i) const
{
    if(position.distance(PointPlusProche(x_i)) > position.distance(x_i)) return true;
    return false;
}

Vecteur3D Brique:: PointPlusProche(Vecteur3D const& x_i) const
{
    Vecteur3D x0(base.PointPlusProche(x_i));
    double d(x0.distance(x_i));
    for(auto dl : dalle6()) {
        Vecteur3D xi(dl.PointPlusProche(x_i));
        double di(xi.distance(x_i));
        if(di < d) {
            d=di;
            x0=xi;
        }
    }
    return x0;
}

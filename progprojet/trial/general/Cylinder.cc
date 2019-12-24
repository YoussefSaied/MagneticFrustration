#include "Obstacle.h"
#include "Vecteur3D.h"
#include "Cylinder.h"
#include <iostream>
#include <cmath>
using namespace std;
//internes

Cylinder:: Cylinder(Position position, Vecteur3D vector1, double h, double r, bool m): Obstacle(position, m), hauteur(h), rayon(r)
{
    vector1n = vector1.normalise();
}

Vecteur3D Cylinder :: PointPlusProche(Vecteur3D const& x_i) const
{
    double d(x_i * vector1n);
    Vecteur3D relatpos(x_i-position); //relative position
    Vecteur3D ep(d*vector1n);
    Vecteur3D epn(ep.normalise());
    Vecteur3D eu(relatpos-ep); //eu and ep form a right angled triangle
    Vecteur3D eun(eu.normalise());
    if(d < hauteur/2) return position + ep + rayon*eun;
    return position + hauteur/2 * epn + min(rayon,eu.norme()) * eun;
}

ostream& Cylinder:: display (ostream& sortie) const
{
    sortie<<"I am a cylinder! " << vector1n<<endl;
    return sortie;
}

unique_ptr<Cylinder> Cylinder:: cloneMe() const
{
    return unique_ptr<Cylinder>(new Cylinder(*this));
}

unique_ptr<Obstacle> Cylinder:: copie() const
{
    return cloneMe();
}

//externes

ostream& operator<<(ostream& sortie, Cylinder const& cyl1)
{
    cyl1.display(sortie);
    return sortie;
}

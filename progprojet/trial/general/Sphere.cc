#include "Sphere.h"
#include "Vecteur3D.h"
#include "Obstacle.h"
#include <cmath>
#include <memory>

using namespace std;

ostream& Sphere:: display (ostream& c) const
{
    c<<"Sphere d'originewith origin at "<<position<<" and radius: " << radius<<endl;
    return c;
}

Vecteur3D Sphere :: PointPlusProche(Vecteur3D const& v) const
{
    return ((v-position).normalise())*radius + position;
}

unique_ptr<Sphere> Sphere:: cloneMe() const
{
    return unique_ptr<Sphere>(new Sphere(*this));
}
unique_ptr<Obstacle> Sphere:: copie() const
{
    return cloneMe();
}

#include "Tetra.h"
#include "Vecteur3D.h"
#include "Plan.h"
#include "Dalle.h"
#include "Obstacle.h"
#include <cmath>
#include <vector>
#include <memory>
using namespace std;

unique_ptr<Tetra> Tetra:: cloneMe() const
{
    return unique_ptr<Tetra>(new Tetra(*this));
}

unique_ptr<Obstacle> Tetra:: copie() const
{
    return cloneMe();
}

bool Tetra:: is_inside(Vecteur3D const& x_i) const
{
    if (position.distance(PointPlusProche(x_i)) > position.distance(x_i)) { return true; }

    return false;
}

Vecteur3D Tetra:: PointPlusProche(Vecteur3D const& x_i) const
{
    return x_i;
    // wrong but not IMP for this simulation; just to remove warning;
}

vector<vector<Vecteur3D> > Tetra:: vertipositions() const
{
    // They are clockwise
    vector<vector<Vecteur3D> > vp; // vertixes positions
    Vecteur3D v1 =  Vecteur3D(-1, 0, -1/sqrt(2))
    Vecteur3D v2 =  Vecteur3D(1, 0, -1/sqrt(2))
    Vecteur3D v3 =  Vecteur3D(0,1, -1/sqrt(2))
    Vecteur3D v4 =  Vecteur3D(0,-1, 1*sqrt(2))
    vp.push_back({v1,v2,v3}); // clockwise looking from the last vertex
    vp.push_back({v1,v4,v2}); // 1
    vp.push_back({v2,v4,v3}); // 1
    vp.push_back({v1,v3,v4}); // 1

    // scaling
    for (auto& i : vp) {
        for (auto& j: i) {
            j *= edge/2;
        }
    }

    // rotate depending vecteur_1 (orientation vector)
    Vecteur3D axer1(Vecteur3D(0,0,1) ^ vecteur_1); // rotation axis
    double angle = acos((vecteur_1 * axer1) / ((vecteur_1.norme()) * axer1.norme()));
    for (auto& i : vp) {
        for (auto& j: i) {
            j = j.rotate(angle, axer1);
        }
    }

    // translate depending on position;

    for (auto& i : vp) {
        for (auto& j: i) {
            j += position;
        }
    }

    return vp;
} // Dodec::vertipositions

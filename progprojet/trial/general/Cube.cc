#include "Cube.h"
#include "Vecteur3D.h"
#include "Plan.h"
#include "Dalle.h"
#include "Obstacle.h"
#include <cmath>
#include <vector>
#include <memory>
using namespace std;

unique_ptr<Cube> Cube:: cloneMe() const
{
    return unique_ptr<Cube>(new Cube(*this));
}

unique_ptr<Obstacle> Cube:: copie() const
{
    return cloneMe();
}

bool Cube:: is_inside(Vecteur3D const& x_i) const
{
    if (position.distance(PointPlusProche(x_i)) > position.distance(x_i)) { return true; }

    return false;
}

Vecteur3D Cube:: PointPlusProche(Vecteur3D const& x_i) const
{
    return x_i;
    // wrong but not IMP for this simulation; just to remove warning;
}

vector<vector<Vecteur3D> > Cube:: vertipositions() const
{
    // They are clockwise
    vector<vector<Vecteur3D> > vp; // vertixes positions
    vp.push_back({ Vecteur3D(-1, -1, -1), Vecteur3D(-1, -1, 1), Vecteur3D(-1,1,1), Vecteur3D(-1,1,-1)}); // 1
    vp.push_back({ Vecteur3D(1, -1, -1), Vecteur3D(1, -1, 1), Vecteur3D(1,1,1), Vecteur3D(1,1,-1)}); // 1
    vp.push_back({ Vecteur3D(-1, -1, 1), Vecteur3D(-1, 1, 1), Vecteur3D(1,1,1), Vecteur3D(1,-1,1)}); // 1
    vp.push_back({ Vecteur3D(-1, -1, -1), Vecteur3D(1, -1,-1), Vecteur3D(1,1,-1), Vecteur3D(-1,1,-1)}); // 1
    vp.push_back({ Vecteur3D(-1, -1, -1), Vecteur3D(-1, -1, 1), Vecteur3D(1,-1,1), Vecteur3D(1,-1,-1)}); // 1
    vp.push_back({ Vecteur3D(-1, 1, -1), Vecteur3D(1, 1, -1), Vecteur3D(1,1,1), Vecteur3D(-1,1,1)}); // 1

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

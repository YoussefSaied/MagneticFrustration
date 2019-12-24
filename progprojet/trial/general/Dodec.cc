#include "Dodec.h"
#include "Vecteur3D.h"
#include "Plan.h"
#include "Dalle.h"
#include "Obstacle.h"
#include <cmath>
#include <vector>
#include <memory>
using namespace std;

unique_ptr<Dodec> Dodec:: cloneMe() const
{
    return unique_ptr<Dodec>(new Dodec(*this));
}

unique_ptr<Obstacle> Dodec:: copie() const
{
    return cloneMe();
}

bool Dodec:: is_inside(Vecteur3D const& x_i) const
{
    if (position.distance(PointPlusProche(x_i)) > position.distance(x_i)) { return true; }

    return false;
}

Vecteur3D Dodec:: PointPlusProche(Vecteur3D const& x_i) const
{
    return x_i;
    // wrong but not IMP for this simulation; just to remove warning;
}

vector<vector<Vecteur3D> > Dodec:: vertipositions() const
{
    // They are clockwise
    vector<vector<Vecteur3D> > vp; // vertixes positions
    double a = (1 + sqrt(5)) / 2;
    vp.push_back({ Vecteur3D(1 / a, 0, a), Vecteur3D(-1 / a, 0, a), Vecteur3D(-1, -1, 1), Vecteur3D(0, -a,
                   1 / a), Vecteur3D(1, -1, 1) }); // 1
    vp.push_back({ Vecteur3D(-1, -1, 1), Vecteur3D(-a, -1 / a, 0), Vecteur3D(-1, -1, -1), Vecteur3D(0, -a,
                   -1 / a),
                   Vecteur3D(0, -a, 1 / a) }); // 2
    vp.push_back({ Vecteur3D(0, -a, 1 / a), Vecteur3D(0, -a, -1 / a), Vecteur3D(1, -1, -1), Vecteur3D(a, -1 / a,
                   0),
                   Vecteur3D(1, -1, 1) }); // 3
    vp.push_back({ Vecteur3D(1 / a, 0, a), Vecteur3D(1, -1, 1), Vecteur3D(a, -1 / a, 0), Vecteur3D(a, 1 / a,
                   0), Vecteur3D(1, 1,
                   1) }); // 4
    vp.push_back({ Vecteur3D(1 / a, 0, -a), Vecteur3D(1, 1, -1), Vecteur3D(a, 1 / a, 0),
                   Vecteur3D(a, -1 / a, 0), Vecteur3D(1, -1, -1) }); // 5
    vp.push_back({ Vecteur3D(1 / a, 0, -a), Vecteur3D(1, -1, -1), Vecteur3D(0, -a, -1 / a),
                   Vecteur3D(-1, -1, -1), Vecteur3D(-1 / a, 0, -a) }); // 6

    vp.push_back({ Vecteur3D(1 / a, 0, -a), Vecteur3D(-1 / a, 0, -a), Vecteur3D(-1, 1, -1),
                   Vecteur3D(0, a, -1 / a), Vecteur3D(1, 1, -1) }); // 7

    vp.push_back({ Vecteur3D(-1, -1, -1), Vecteur3D(-a, -1 / a, 0), Vecteur3D(-a, 1 / a, 0),
                   Vecteur3D(-1, 1, -1), Vecteur3D(-1 / a, 0, -a) }); // 8
    vp.push_back({ Vecteur3D(-1, 1, 1), Vecteur3D(-a, 1 / a, 0), Vecteur3D(-a, -1 / a, 0), Vecteur3D(-1, -1, 1),
                   Vecteur3D(-1 / a, 0, a) }); // 9
    vp.push_back({ Vecteur3D(-1, 1, 1), Vecteur3D(-1 / a, 0, a), Vecteur3D(1 / a, 0, a), Vecteur3D(1, 1, 1),
                   Vecteur3D(0, a, 1 / a) }); // 10
    vp.push_back({ Vecteur3D(-1, 1, 1), Vecteur3D(0, a, 1 / a), Vecteur3D(0, a, -1 / a), Vecteur3D(-1, 1, -1),
                   Vecteur3D(-a, 1 / a, 0) }); // 11
    vp.push_back({ Vecteur3D(1, 1, -1), Vecteur3D(0, a, -1 / a), Vecteur3D(0, a, 1 / a), Vecteur3D(1, 1, 1),
                   Vecteur3D(a, 1 / a, 0) }); // 12

    /*for(int i =-1; i<2; i+=2){
        for(int j =-1; j<2; j+=2){
            for(int k =-1; k<2; k+=2){
                vp.push_back(Vecteur3D(i,j,k));
                vp.push_back(Vecteur3D(0,j*a,k/a));
                vp.push_back(Vecteur3D(i/a,0,k*a));
                vp.push_back(Vecteur3D(i*a,j/a,0));
            }
        }
       }*/
    // scaling
    for (auto& i : vp) {
        for (auto& j: i) {
            j *= edge;
        }
    }

    // rotate depending vecteur_1 (orientation vector)
    Vecteur3D v1(1 / a, 0, a);
    Vecteur3D v2(-1 / a, 0, a);
    Vecteur3D v3(-1, -1, 1);
    Vecteur3D o1(v1 - v2);
    Vecteur3D o2(v2 - v3);
    Vecteur3D axer0(o1 ^ o2);           // orthogonal vector to canonical dodec face
    Vecteur3D axer1(axer0 ^ vecteur_1); // rotation axis
    double angle = acos((vecteur_1 * axer0) / ((vecteur_1.norme()) * axer0.norme()));
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

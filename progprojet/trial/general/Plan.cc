#include "Obstacle.h"
#include "Vecteur3D.h"
#include "Plan.h"
#include <iostream>
using namespace std;
//internes

Vecteur3D Plan :: PointPlusProche(Vecteur3D const& x_i) const
{
    return x_i +((position-x_i)*vecteur_1)*vecteur_1;
}

ostream& Plan :: display (ostream& c) const
{
        c<<"Plan with center at "<<position<<" and normal vector: " << vecteur_1<<endl;
        return c;
}

unique_ptr<Plan> Plan:: cloneMe() const
{
    return unique_ptr<Plan>(new Plan(*this));
}

unique_ptr<Obstacle> Plan:: copie() const
{
    return cloneMe();
}

//externes

ostream& operator<<(ostream& c, Plan const& plan1)
{
    plan1.display(c);
    return c;
}

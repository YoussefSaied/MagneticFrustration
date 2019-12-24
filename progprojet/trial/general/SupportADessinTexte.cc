#include "SupportADessinTexte.h"
#include <iostream>

#include "Plan.h"
#include "Dalle.h"

#include "Systeme.h"
#include "Brique.h"
#include "Dodec.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Magnet.h"
#include "MagnetH.h"
#include "MagnetA.h"

using namespace std;


void SupportADessinTexte :: dessine( MagnetA const& M)
{
    M.MagnetA::display(flot);
}

void SupportADessinTexte :: dessine(MagnetH const& M)
{
    M.MagnetH::display(flot);
}

void SupportADessinTexte :: dessine(Plan const& P)
{
    P.display(flot);
}

void SupportADessinTexte :: dessine(Dalle const& d)
{
    d.display(flot);
}

void SupportADessinTexte :: dessine(Systeme const& S)
{
    S.display(flot);
}

void SupportADessinTexte :: dessine(Sphere const& s)
{
    s.display(flot);
}

void SupportADessinTexte :: dessine(Brique const& b)
{
    vector<Dalle> d6;
    d6 = (b.dalle6());
    for (auto d : d6) {
        d.dessine();
    }
}

void SupportADessinTexte :: dessine(Dodec const& d)
{
    d.display(flot);
}

void SupportADessinTexte :: dessine(Cube const& q)
{
    q.display(flot);
}

void SupportADessinTexte :: dessine(Cylinder const& c)
{
    c.display(flot);
}

#pragma once
class Plan;
class Systeme;
class Dalle;
class Sphere;
class Brique;
class Dodec;
class Cube;
class Cylinder;
class Magnet;
class MagnetH;
class MagnetA;


class SupportADessin
{
public:
    virtual ~SupportADessin(){ }


    virtual void dessine(Systeme const& S)  = 0;
    virtual void dessine(Plan const& P)     = 0;
    virtual void dessine(Dalle const& d)    = 0;
    virtual void dessine(Sphere const& s)   = 0;
    virtual void dessine(Brique const& b)   = 0;
    virtual void dessine(Dodec const& d)    = 0;
    virtual void dessine(Cube const& q)     = 0;
    virtual void dessine(Cylinder const& c) = 0;
    // virtual void dessine(Magnet const& M) = 0;
    virtual void dessine(MagnetA const& M)  = 0;
    virtual void dessine(MagnetH const& M) = 0;
};

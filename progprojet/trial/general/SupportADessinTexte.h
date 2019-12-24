#pragma once
#include "SupportADessin.h"
#include <iostream>
#include <vector>


class SupportADessinTexte : public SupportADessin
{
public:
    // constructeur et destructeur
    explicit SupportADessinTexte(std::ostream& flott) : flot(flott){ }

    virtual ~SupportADessinTexte() override { }

    // methodes

    virtual void dessine(Systeme const& S) override;
    virtual void dessine(Plan const& P) override;
    virtual void dessine(Dalle const& d) override;
    virtual void dessine(Sphere const& s) override;
    virtual void dessine(Brique const& b) override;
    virtual void dessine(Dodec const& d) override;
    virtual void dessine(Cube const& q) override;
    virtual void dessine(Cylinder const& c) override;
    virtual void dessine(MagnetA const& M) override;
    virtual void dessine(MagnetH const& M) override;

private:
    std::ostream& flot;
};

static SupportADessinTexte Texte1(std::cout);

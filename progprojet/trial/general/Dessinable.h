#pragma once
#include "Vecteur3D.h"
#include "SupportADessin.h"
#include "SupportADessinTexte.h"
class Dessinable
{
public:
    // constructeur et destructeur
    Dessinable(Position const& position = Position(), SupportADessin * support = &Texte1) : position(position),
        support(support){ }

    virtual ~Dessinable(){ }

    // methodes
    virtual void dessine() const = 0;

    Vecteur3D get_position() const { return position; }

    void set_position(Position const& newPosition){ position = newPosition; }

    SupportADessin * get_support() const { return support; }

    void set_support(SupportADessin * s){ support = s; }


    Vecteur3D position; // possition du centre de l'objet dessinable
    SupportADessin * support;
};

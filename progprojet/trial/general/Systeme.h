#pragma once
#include <QDataStream>
#include <vector>
#include <cmath>
#include "Obstacle.h"
#include <iostream>
#include "Dessinable.h"
#include <memory>
#include "SupportADessin.h"
#include "SupportADessinTexte.h"
#include "Vecteur3D.h"
#include "Sphere.h"
#include "Dodec.h"
#include "Cube.h"
#include "Magnet.h"
#include "MagnetA.h"
#include "MagnetH.h"
#include <fstream>
#include <sstream>
#include <iostream>


class Systeme : public Dessinable
{
    friend class VueOpenGL; // simple solution
public:

    // constructeurs et destructeurs
    Systeme(SupportADessin * support = &Texte1, double f = 1.0, int selectmagnet = 0) : Dessinable(Position(), support),
        B(0.000, 0, 0), selectedmagnet(selectmagnet), KineticEnergy(0), PotentialEnergy(0), time(0), f(f), n(1), eq(
            false),
        energyunit(1e4), NearestCorrelationv(0.0), MagnetType(0)
    { }

    virtual ~Systeme(){ }

    Vecteur3D B; // external magnetic field
    std:: vector<std::unique_ptr<Magnet> > tab_ptr_Magnets;
    int selectedmagnet;
    double KineticEnergy;
    double PotentialEnergy;
    double time;
    double f; //friction
    size_t n; // number of times display() is used when running a text sim.
    bool eq;
    int energyunit;
    double NearestCorrelationv;
    int MagnetType;
    // methodes

    std::vector<double> vertexangles(Vecteur3D vertexposition, double thresholddistance);

    virtual double Energy() const { return KineticEnergy + PotentialEnergy; }

    virtual double NearestCorrelation() const;

    virtual double totalangle() const;

    virtual void dessine() const override { support->dessine(*this); }



    // evolve: all magnet magnet interactions
    virtual void evolue(double dt = 0.01);
    virtual void evolue(double dt, unsigned int nb_repet);
    virtual void evolue(double dt, double t, bool d, bool final); // evolution du système selon le 1er temps t

    // evolve1: nearest neighbours only
    virtual void evolue1(double dt = 0.01);
    virtual void evolue1(double dt, unsigned int nb_repet);
    virtual void evolue1(double dt, double t, bool d, bool final); // evolution du système selon le 1er temps t

    // evolve2: 2nd nearest and nearest neighbours only
    virtual void evolue2(double dt = 0.01);
    virtual void evolue2(double dt, unsigned int nb_repet);
    virtual void evolue2(double dt, double t, bool d, bool final);


    virtual std:: ostream& display(std:: ostream& c) const;
    virtual QDataStream& display(QDataStream& c) const;
    virtual std:: ostream& displaypos(std:: ostream& c) const;

    virtual void setangles(std::string filename);

    virtual int addMagnet(Magnet const& new_Magnet);

    virtual void setfriction(double friction);

    virtual void randominitial();


    void addObstacle(Obstacle const& nouveau_obstacle);

    virtual void selected(int nm)
    {
        tab_ptr_Magnets[(nm % tab_ptr_Magnets.size())]->selected = 1;
        tab_ptr_Magnets[selectedmagnet]->selected = 0;
        selectedmagnet = (nm % tab_ptr_Magnets.size());
    }

    virtual std::unique_ptr<Systeme> copie() const;
    virtual std::unique_ptr<Systeme> cloneme() const;
    // delete
    Systeme (Systeme const&) = delete;


    // attributs
    std:: vector<std::unique_ptr<Obstacle> > tab_ptr_obstacles;
};

// externe
std::ostream& operator << (std::ostream& sortie, Systeme const& s);

QDataStream& operator << (QDataStream& sortie, Systeme const& systeme);

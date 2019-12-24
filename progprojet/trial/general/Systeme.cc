#define _USE_MATH_DEFINES
#include "Systeme.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <stdio.h>

using namespace std;


void Systeme:: addObstacle(Obstacle const& nouveau_obstacle)
{
    // faire des tests de compatibilité
    tab_ptr_obstacles.push_back(nouveau_obstacle.copie());
    (*(tab_ptr_obstacles.back())).set_support(support);
}

// externe
ostream& operator << (ostream& sortie, Systeme const& systeme)
{
    systeme.display(sortie);
    return sortie;
}

QDataStream& operator << (QDataStream& sortie, Systeme const& systeme)
{
    systeme.display(sortie);
    return sortie;
}

int Systeme:: addMagnet(Magnet const & nouveau_Magnet)
{
    // faire des tests de compatibilité
    int index = -1;
    for (size_t i = 0; i < tab_ptr_Magnets.size(); i++) {
        if ((tab_ptr_Magnets[i]->position - nouveau_Magnet.position).norme() < 0.0001) {
            index = i;
        }
    }
    if (index == -1) {
        tab_ptr_Magnets.push_back(nouveau_Magnet.copie());
        (*(tab_ptr_Magnets.back())).set_support(support);
        (*(tab_ptr_Magnets.back())).f = f;
        index = tab_ptr_Magnets.size() - 1;
    }

    return index;
}

void Systeme:: setangles(std::string filename)
{
    unique_ptr<ifstream> tfile(new ifstream(filename));
    double value;
    for (auto& ptr_Magnet : tab_ptr_Magnets) {
        *tfile >> value;
        ptr_Magnet->setangle(value) ;
    }
}

void Systeme:: setfriction(double friction)
{
    for (auto const& ptr_Magnet : tab_ptr_Magnets) {
        ptr_Magnet->f = friction;
    }
    f = friction;
}

void Systeme:: randominitial()
{
    int i = 0;
    for (auto const& ptr_Magnet : tab_ptr_Magnets) {
        ++i;
        std::chrono::time_point<std::chrono::system_clock> noww = std::chrono::system_clock::now();
        int secnow = std::chrono::duration_cast<std::chrono::seconds>(
            noww.time_since_epoch()).count();
        srand(secnow + i);
        double randomval = M_PI * (rand() % 2);
        ptr_Magnet->setangle(randomval);
    }
}

ostream& Systeme:: display(ostream& c) const
{
    bool fric = 0;
    if (fric) {
        c << time << " ";
        c << NearestCorrelation() << " ";
        c << totalangle() << " ";
        c << Energy() << " ";
        c << KineticEnergy << " ";
        c << endl;
    }
    else {
        c << time << " ";
        c << Energy() << " ";
        c << KineticEnergy << " ";
        c << NearestCorrelation() << " ";
        for (auto& g: tab_ptr_Magnets) {
            c << g->getangle() << " ";
        }
        c << endl;
    }

    return c;
}

QDataStream& Systeme:: display(QDataStream& c) const
{
    bool fric = 0;
    if (fric) {
        c << time << " ";
        c << NearestCorrelation() << " ";
        c << totalangle() << " ";
        c << Energy() << " ";
        c << KineticEnergy << " ";
        c << "\n";
    }
    else {
        c << time << " ";
        c << Energy() << " ";
        c << KineticEnergy << " ";
        c << NearestCorrelation() << " ";
        for (auto& g: tab_ptr_Magnets) {
            c << g->getangle() << " ";
        }
        c << "\n";
    }

    return c;
}


ostream& Systeme:: displaypos(ostream& c) const
{
    for (auto& g: tab_ptr_Magnets) {
        c << g->position << endl;
    }
    return c;
}

unique_ptr<Systeme> Systeme :: cloneme() const
{
    unique_ptr<Systeme> P9(new Systeme(support, f, selectedmagnet));
    for (auto& g: tab_ptr_Magnets) {
        P9->addMagnet(*g);
    }
    for (auto& o : tab_ptr_obstacles) {
        P9->addObstacle(*o);
    }

    return P9;
}

unique_ptr<Systeme> Systeme ::copie() const
{
    return cloneme();
}


///Generalised EVOLVE


///////////EVOLVE//////////////////////
void Systeme:: evolue(double dt)
{
    time += dt;
    KineticEnergy   = 0;
    PotentialEnergy = 0;
    // CALC TORQUE
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        if (time > 0.1) {
            tab_ptr_Magnets[i]->resetnew();
        }
        else {
            tab_ptr_Magnets[i]->reset();
            tab_ptr_Magnets[i]->addTorque(B);
            for (size_t j(0); j < tab_ptr_Magnets.size(); ++j) {
                // magnet interactions
                if (i != j) {
                    tab_ptr_Magnets[i]->addTorque(tab_ptr_Magnets[j]);
                }
            }
        }
    }
    // MOVE Angle
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        tab_ptr_Magnets[i]->moveangle(dt);
    }
    // CALC NEWTORQUE
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        tab_ptr_Magnets[i]->addnewTorque(B);
        for (size_t j(0); j < tab_ptr_Magnets.size(); ++j) {
            if (i != j) {
                tab_ptr_Magnets[i]->addnewTorque(tab_ptr_Magnets[j]);
                tab_ptr_Magnets[i]->addpotB(tab_ptr_Magnets[j]);
            }
        }
    }
    // MOVE OMEGA & ENERGY
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        tab_ptr_Magnets[i]->moveomega(dt);
        KineticEnergy   += energyunit * tab_ptr_Magnets[i]->Kinetic();
        PotentialEnergy += energyunit * tab_ptr_Magnets[i]->potB() / 2;
    }
    if (std::abs(KineticEnergy / Energy()) < 1e-15) {
        eq = true;
    }
} // Systeme::evolue

void Systeme:: evolue(double dt, unsigned int nb_repet)
{
    for (unsigned int i(0); i < nb_repet; ++i) {
        evolue(dt);
    }
}

void Systeme:: evolue(double dt, double t, bool d, bool final)
{
    double targettime = t / n; // the output interval
    while (targettime <= t) {
        while (abs(time + dt - targettime) < abs(time - targettime) ) {
            evolue(dt);
        }
        targettime += t / n;
        if (!final and d) {
            dessine();
        }
    }
    if (d and final) {
        dessine();
        time = 0;
    }
}

/////////////EVOLVE1///////////////////
void Systeme:: evolue1(double dt)
{
    time += dt;
    KineticEnergy   = 0;
    PotentialEnergy = 0;

    // nearest neighbours
    double genconst = 2;
    // second nearest neighbours
    // double genconst = 4.5;

    // CALC TORQUE
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        if (time > 0.1) {
            tab_ptr_Magnets[i]->resetnew();
        }
        else {
            tab_ptr_Magnets[i]->reset();
            tab_ptr_Magnets[i]->addTorque(B);
            for (size_t j(0); j < tab_ptr_Magnets.size(); ++j) {
                // magnet interactions
                if ( (tab_ptr_Magnets[i]->position - tab_ptr_Magnets[j]->position).norme() <
                  genconst * tab_ptr_Magnets[i]->length and i != j)
                {
                    tab_ptr_Magnets[i]->addTorque(tab_ptr_Magnets[j]);
                }
            }
        }
    }
    // MOVE ANGLE
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        tab_ptr_Magnets[i]->moveangle(dt);
    }
    // CALC NEWTORQUE
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        tab_ptr_Magnets[i]->addnewTorque(B);
        for (size_t j(0); j < tab_ptr_Magnets.size(); ++j) {
            if ( (tab_ptr_Magnets[i]->position - tab_ptr_Magnets[j]->position).norme() <
              2 * tab_ptr_Magnets[i]->length and i != j)
            {
                tab_ptr_Magnets[i]->addnewTorque(tab_ptr_Magnets[j]);
                tab_ptr_Magnets[i]->addpotB(tab_ptr_Magnets[j]);
            }
        }
    }
    // MOVE OMEGA & ENERGY
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        tab_ptr_Magnets[i]->moveomega(dt);
        KineticEnergy   += energyunit * tab_ptr_Magnets[i]->Kinetic();
        PotentialEnergy += energyunit * tab_ptr_Magnets[i]->potB() / 2;
    }
    if (std::abs(KineticEnergy / Energy()) < 1e-15) {
        eq = true;
    }
} // Systeme::evolue1

void Systeme:: evolue1(double dt, unsigned int nb_repet)
{
    for (unsigned int i(0); i < nb_repet; ++i) {
        evolue1(dt);
    }
}

void Systeme:: evolue1(double dt, double t, bool d, bool final)
{
    double targettime = t / n; // the output interval
    while (targettime <= t) {
        while (abs(time + dt - targettime) < abs(time - targettime) ) {
            evolue1(dt);
        }
        targettime += t / n;
        if (!final and d) {
            dessine();
        }
    }
    if (d and final) {
        dessine();
        time = 0;
    }
}

/////////////EVOLVE2///////////////////
void Systeme:: evolue2(double dt)
{
    time += dt;
    KineticEnergy   = 0;
    PotentialEnergy = 0;

    // CALC TORQUE
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        if (time > 0.1) {
            tab_ptr_Magnets[i]->resetnew();
        }
        else {
            tab_ptr_Magnets[i]->reset();
            tab_ptr_Magnets[i]->addTorque(B);
            int count = 0;
            for (size_t j(0); j < tab_ptr_Magnets.size(); ++j) {
                // magnet interactions
                if ( (tab_ptr_Magnets[i]->position - tab_ptr_Magnets[j]->position).norme() <
                  2 * tab_ptr_Magnets[i]->length and i != j)
                {
                    for (size_t k(0); k < tab_ptr_Magnets.size(); ++k) {
                        if ( (tab_ptr_Magnets[j]->position - tab_ptr_Magnets[k]->position).norme() <
                          2 * tab_ptr_Magnets[j]->length and j != k and i != k)
                        {
                            tab_ptr_Magnets[i]->addTorque(tab_ptr_Magnets[k]);
                            count += 1;
                            // if (std::fmod(time,0.5)){cout << "["<<count <<" "<< i <<" "<< j<<" " << k<<"]";}
                        }
                    }
                }
            }
        }
    }

    // MOVE ANGLE
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        tab_ptr_Magnets[i]->moveangle(dt);
    }
    // CALC NEWTORQUE
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        tab_ptr_Magnets[i]->addnewTorque(B);
        for (size_t j(0); j < tab_ptr_Magnets.size(); ++j) {
            if ( (tab_ptr_Magnets[i]->position - tab_ptr_Magnets[j]->position).norme() <
              2 * tab_ptr_Magnets[i]->length and i != j)
            {
                tab_ptr_Magnets[i]->addnewTorque(tab_ptr_Magnets[j]);
                tab_ptr_Magnets[i]->addpotB(tab_ptr_Magnets[j]);
            }
        }
    }
    // MOVE OMEGA & ENERGY
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        tab_ptr_Magnets[i]->moveomega(dt);
        KineticEnergy   += energyunit * tab_ptr_Magnets[i]->Kinetic();
        PotentialEnergy += energyunit * tab_ptr_Magnets[i]->potB() / 2;
    }
    if (std::abs(KineticEnergy / Energy()) < 1e-15) {
        eq = true;
    }
} // Systeme::evolue2

void Systeme:: evolue2(double dt, unsigned int nb_repet)
{
    for (unsigned int i(0); i < nb_repet; ++i) {
        evolue2(dt);
    }
}

void Systeme:: evolue2(double dt, double t, bool d, bool final)
{
    double targettime = t / n; // the output interval
    while (targettime <= t) {
        while (abs(time + dt - targettime) < abs(time - targettime) ) {
            evolue2(dt);
        }
        targettime += t / n;
        if (!final and d) {
            dessine();
        }
    }
    if (d and final) {
        dessine();
        time = 0;
    }
}

double Systeme:: NearestCorrelation() const
{ // can we do it with angles?
    double correlation(0);
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        for (size_t j(0); j < tab_ptr_Magnets.size(); ++j) {
            if ( (tab_ptr_Magnets[i]->position - tab_ptr_Magnets[j]->position).norme() <
              2 * tab_ptr_Magnets[i]->length and i != j)
            {
                correlation += (tab_ptr_Magnets[i]->moment().normalise())
                  * (tab_ptr_Magnets[j]->moment().normalise());
            }
        }
    }
    if (!tab_ptr_Magnets.empty()) {
        correlation /= tab_ptr_Magnets.size();
    }

    return correlation;
}

double Systeme::totalangle() const
{
    double totalangle1 = 0;
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        double angle = tab_ptr_Magnets[i]->getangle();
        //int numberofrotations(angle / M_PI);
        angle = std::abs(std::fmod(angle, M_PI));
        if (angle > M_PI / 2) {
            angle = M_PI - angle;
        }

        // angle        = pow(-1, numberofrotations) * std::fmod(angle, M_PI);
        // angle        =  std::fmod(angle, M_PI/2);
        totalangle1 += angle;
    }
    // provisional:

    /*if (tab_ptr_Magnets.size() > 29) {
        totalangle1 += -2 * tab_ptr_Magnets[1]->angle;
        totalangle1 += -2 * tab_ptr_Magnets[2]->angle;
        totalangle1 += -2 * tab_ptr_Magnets[23]->angle;
        totalangle1 += -2 * tab_ptr_Magnets[28]->angle;
        totalangle1 += -2 * tab_ptr_Magnets[3]->angle;
       }*/
    return totalangle1;
}

std::vector<double> Systeme::vertexangles(Vecteur3D vertexposition, double thresholddistance)
{
    std::vector<double> angles;
    for (size_t i(0); i < tab_ptr_Magnets.size(); ++i) {
        if ((tab_ptr_Magnets[i]->position - vertexposition).norme() < thresholddistance) {
            angles.push_back(tab_ptr_Magnets[i]->getangle());
        }
    }
    return angles;
}

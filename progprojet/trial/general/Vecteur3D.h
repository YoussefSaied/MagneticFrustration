#pragma once
#include <iostream> // utilisé dans la methode affiche
#include <armadillo>

class Vecteur3D
{
public:
    // constructeurs

    // explicit car risque de mauvais usage par le compilateur (par example dans produit scalaire)
    explicit Vecteur3D(double x = 0.0, double y = 0.0, double z = 0.0);


    // manipulateurs
    void set_coord(double newX = 0.0, double newY = 0.0, double newZ = 0.0);

    double get_x() const;

    double get_y() const;

    double get_z() const;

    void setx(double xn)
    {
        x = xn;
    }

    void sety(double yn)
    {
        y = yn;
    }

    void setz(double zn)
    {
        z = zn;
    }

    // méthodes


    double operator * (Vecteur3D const& vecteur) const; // produit scalaire

    Vecteur3D operator ^ (Vecteur3D const& vecteur) const; // produit vectoriel

    double norme() const;

    double norme2() const; // norme au carré

    double distance(Vecteur3D const& vecteur2) const; // returne la distance au vecteur2

    Vecteur3D oppose() const; // retourne un vecteur opposé a celui qui utilise la méthode (-x,-y,-z)

    Vecteur3D normalise() const; // retourne un vecteur normalisé

    std:: ostream&affiche(std:: ostream& sortie, bool withoutbrackets = 1) const; // affiche le vecteur:  (x,y,z)

    Vecteur3D GS1() const; // Procédé de Gram-Schmitt

    Vecteur3D GS1(Vecteur3D excluded) const; // Procédé de Gram-Schmitt

    Vecteur3D rotate(double angle, Vecteur3D axe) const; // rotation of angle about an axe

    arma::Mat<double> rotationMatrix(Vecteur3D finalVector) const;

    // oppérateurs internes
    bool operator == (Vecteur3D const& autre) const;

    bool operator != (Vecteur3D const& autre) const;

    bool operator == (double c) const; // vérifie si la norme du vecteur = c

    bool operator != (double c) const; // vérifie si la norme du vecteur != c

    bool operator < (double c) const; // vérifie si la norme du vecteur < c

    bool operator <= (double c) const; // vérifie si la norme du vecteur <= c

    bool operator > (double c) const; // vérifie si la norme du vecteur > c

    bool operator >= (double c) const; // vérifie si la norme du vecteur >= c

    Vecteur3D&operator += (Vecteur3D const& autre);

    Vecteur3D&operator += (double scalaire); // vectuer = vecteur + (scalaire,scalaire,scalaire)

    Vecteur3D&operator *= (double scalaire);

    Vecteur3D&operator -= (Vecteur3D const& autre);

    Vecteur3D&operator = (double c); // transforme le vecteur en (c,c,c)

    void random_vector(); // change les coord aleatoirement

    double x;
    double y;
    double z;
};

// opérateurs externes

const Vecteur3D operator + (Vecteur3D vecteur, Vecteur3D const& autre);

const Vecteur3D operator - (Vecteur3D vecteur, Vecteur3D const& autre);

const Vecteur3D operator * (double scalaire, Vecteur3D vecteur);

const Vecteur3D operator * (Vecteur3D vecteur, double scalaire);

const Vecteur3D operator / (Vecteur3D vecteur, double scalaire);

const Vecteur3D operator * (arma::Mat<double> A, Vecteur3D vecteur);

std::ostream& operator << (std::ostream& sortie, Vecteur3D const& vecteur);


typedef Vecteur3D Position, Vitesse, Force;

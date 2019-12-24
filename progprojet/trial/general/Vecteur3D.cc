#include <iostream>
#include "Vecteur3D.h"
#include <cmath>
#include "Aleatoire.h"

using namespace std;

// random utilisé dans la méthode random_vector()
random_device generator;
uniform_int_distribution<int> distribution(0, 10);

// constructeurs

Vecteur3D:: Vecteur3D(double x, double y, double z) : x(x), y(y), z(z){ }

// manipulateurs

void Vecteur3D :: set_coord(double newX, double newY, double newZ)
{
    // x,y,z sont les coord du vecteur qui utilise la méthode
    x = newX;
    y = newY;
    z = newZ;
}

double Vecteur3D:: get_x() const
{
    return x;
}

double Vecteur3D:: get_y() const
{
    return y;
}

double Vecteur3D:: get_z() const
{
    return z;
}

// methodes

ostream&Vecteur3D:: affiche(ostream &sortie, bool withoutbrackets) const
{
    if (withoutbrackets) {
        sortie << "(" << x << ", " << y << ", " << z << ") ";
    }
    else {
        sortie << x << " " << y << " " << z;
    }

    return sortie;
}

Vecteur3D Vecteur3D :: oppose() const
{
    return Vecteur3D(x * -1, y * -1, z * -1);
}

double Vecteur3D :: norme2() const
{
    return x * x + y * y + z * z;
}

double Vecteur3D :: norme() const
{
    return sqrt(norme2());
}

void Vecteur3D :: random_vector() // pour test
{
    (*this).set_coord(distribution(generator), distribution(generator), distribution(generator));
    cout << "Nouveau Vecteur3D aléatoire : ";
    cout << (*this) << endl;
}

Vecteur3D Vecteur3D :: GS1() const
{
    if ( (*this).normalise() !=
      Vecteur3D(1, 0, 0) )
    {
        return Vecteur3D(1, 0, 0) - (Vecteur3D(1, 0, 0) * (*this)) * (*this) / norme2();
    }
    else {
        return Vecteur3D(0, 1, 0);
    }
}

Vecteur3D Vecteur3D :: GS1(Vecteur3D excluded) const
{
    if ( ((*this) ^ excluded) != 0) {
        return (*this) - (excluded * (*this)) * excluded / (norme2());
    }
    else {
        return Vecteur3D(1, 10, 100) ^ excluded;
    }
}

Vecteur3D Vecteur3D :: normalise() const
{
    if (norme() != 0) {
        return (*this) / ((*this).norme());
    }

    return (*this);
}

double Vecteur3D :: distance(Vecteur3D const& v2) const
{
    return ((*this) - v2).norme();
}

Vecteur3D Vecteur3D :: rotate(double angle, Vecteur3D axe1) const
{
    Vecteur3D axe = axe1.normalise();
    double x1     = x * (cos(angle) + axe.x * axe.x * (1 - cos(angle)))
      + y * (axe.x * axe.y * (1 - cos(angle)) - axe.z * sin(angle))
      + z * (axe.x * axe.z * (1 - cos(angle)) + axe.y * sin(angle));
    double y1 = y * (cos(angle) + axe.y * axe.y * (1 - cos(angle)))
      + x * (axe.x * axe.y * (1 - cos(angle)) + axe.z * sin(angle))
      + z * (axe.y * axe.z * (1 - cos(angle)) - axe.x * sin(angle));
    double z1 = z * (cos(angle) + axe.z * axe.z * (1 - cos(angle)))
      + x * (axe.x * axe.z * (1 - cos(angle)) - axe.y * sin(angle))
      + y * (axe.y * axe.z * (1 - cos(angle)) + axe.x * sin(angle));

    return Vecteur3D(x1, y1, z1);
}

arma::Mat<double> Vecteur3D :: rotationMatrix(Vecteur3D finalVector) const
{
    Vecteur3D initial      = normalise();
    Vecteur3D finalVectorN = finalVector.normalise();
    arma::Mat<double> I(3, 3, arma::fill::eye);
    double C    = initial * finalVectorN;
    Vecteur3D v = initial ^ finalVectorN;
    //double S    = (v).norme();
    arma::Mat<double> Skew(3, 3, arma::fill::zeros);
    Skew(0, 1) = -1 * v.z;
    Skew(1, 0) = +1 * v.z;
    Skew(0, 2) = v.y;
    Skew(2, 0) = -1 * v.y;
    Skew(1, 2) = -1 * v.x;
    Skew(2, 1) = +1 * v.x;
    return I + Skew + (1 / (1 + C)) * Skew*Skew;
}

// operateurs interne

bool Vecteur3D:: operator == (Vecteur3D const& autre) const
{
    return (autre.x == x && autre.y == y && autre.z == z);
}

bool Vecteur3D :: operator != (Vecteur3D const& autre) const
{
    return !((*this) == autre);
}

bool Vecteur3D :: operator == (double c) const
{
    return ((*this).norme() == c);
}

bool Vecteur3D :: operator != (double c) const
{
    return !((*this) == c);
}

bool Vecteur3D :: operator < (double c) const
{
    return ((*this).norme() < c);
}

bool Vecteur3D :: operator <= (double c) const
{
    return ((*this) < c or (*this) == c);
}

bool Vecteur3D :: operator > (double c) const
{
    return !((*this) < c);
}

bool Vecteur3D :: operator >= (double c) const
{
    return ((*this) > c or (*this) == c);
}

Vecteur3D&Vecteur3D :: operator += (Vecteur3D const& autre)
{
    x += autre.x;
    y += autre.y;
    z += autre.z;
    return (*this);
}

Vecteur3D&Vecteur3D :: operator -= (Vecteur3D const& autre)
{
    x -= autre.x;
    y -= autre.y;
    z -= autre.z;
    return (*this);
}

Vecteur3D&Vecteur3D :: operator *= (double scalaire)
{
    (*this).set_coord(x * scalaire, y * scalaire, z * scalaire);
    return (*this);
}

Vecteur3D&Vecteur3D :: operator += (double scalaire)
{
    (*this).set_coord(x + scalaire, y + scalaire, z + scalaire);
    return (*this);
}

Vecteur3D&Vecteur3D :: operator = (double c)
{
    set_coord(c, c, c);
    return (*this);
}

double Vecteur3D :: operator * (Vecteur3D const& vecteur) const // prod scalaire
{
    return x * vecteur.x + y * vecteur.y + z * vecteur.z;
}

Vecteur3D Vecteur3D :: operator ^ (Vecteur3D const& vecteur) const // prod vectoriel
{
    return Vecteur3D((y * vecteur.z - z * vecteur.y), (z * vecteur.x - x * vecteur.z), (x * vecteur.y - y * vecteur.x));
}

// operateurs externe

ostream&operator << (ostream &sortie, Vecteur3D const& vecteur)
{
    vecteur.affiche(sortie);
    return sortie;
}

const Vecteur3D operator + (Vecteur3D vecteur, Vecteur3D const& autre)
{
    return vecteur += autre;
}

const Vecteur3D operator * (double scalaire, Vecteur3D vecteur)
{
    return vecteur *= scalaire;
}

const Vecteur3D operator * (Vecteur3D vecteur, double scalaire)
{
    return vecteur *= scalaire;
}

const Vecteur3D operator / (Vecteur3D vecteur, double scalaire)
{
    return vecteur *= (1 / scalaire);
}

const Vecteur3D operator - (Vecteur3D vecteur, Vecteur3D const& autre)
{
    return vecteur -= autre;
}

const Vecteur3D operator * (arma::Mat<double> A, Vecteur3D vecteur)
{
    double a1 = A(0, 0) * vecteur.x + A(0, 1) * vecteur.y + A(0, 2) * vecteur.z;
    double a2 = A(1, 0) * vecteur.x + A(1, 1) * vecteur.y + A(1, 2) * vecteur.z;
    double a3 = A(2, 0) * vecteur.x + A(2, 1) * vecteur.y + A(2, 2) * vecteur.z;

    return Vecteur3D(a1, a2, a3);
}

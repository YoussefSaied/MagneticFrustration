#ifndef ALEATOIRE_H
#define ALEATOIRE_H

#include <ctime>
#include <random>

class Aleatoire
{
	public:
	// constructeurs
	Aleatoire(int graine)
      : generateur(graine)
	{}

    Aleatoire()
      : generateur(rd())
			//generateur(time(nullptr))
    {}

	// méthodes
	double uniforme(double min = 0.0, double max = 1.0) {
      return distribution_uniforme(generateur,
                 std::uniform_real_distribution<double>::param_type{min, max});
    }


    double gaussienne(double moyenne, double ecart_type) {
      return distribution_normale(generateur,
                 std::normal_distribution<double>::param_type{moyenne, ecart_type});
	};

 private:
    static std::uniform_real_distribution<double> distribution_uniforme;
    static std::normal_distribution<double> distribution_normale;
    static std::random_device rd;
    std::mt19937 generateur;

  // suppression des copies
  Aleatoire(Aleatoire const&) = delete;
  Aleatoire& operator=(Aleatoire const&) = delete;

};

#endif // ALEATOIRE_H

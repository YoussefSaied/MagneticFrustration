#pragma once
#define _USE_MATH_DEFINES
#include "Vecteur3D.h"
#include <iostream>
#include <memory>
#include <cmath>
#include <vector>

class Matrix3D {
	public:
		std::vector<std::vector<double>> Vectorform;
		double operator [](int n){
			return Vectorform[n];
		}
};

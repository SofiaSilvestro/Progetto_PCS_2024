#pragma once
#include <iostream>
#include"Polygons.hpp"
using namespace std;

namespace FracturesLib{

bool importazione(const string& filename, Fractures& frattura);
double distanza_al_quadrato(Vector3d& v1, Vector3d& v2);
void esportazione(Traces& traccia, Fractures& frattura);
bool valuta_intersezione (Fractures& frattura, unsigned int& Id1, unsigned int& Id2);

}

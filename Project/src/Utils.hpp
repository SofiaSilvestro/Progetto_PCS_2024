#pragma once
#include <iostream>
#include"Polygons.hpp"
using namespace std;

namespace FracturesLib{

bool importazione(const string& filename, Fractures& frattura);
double distanza_al_quadrato(Vector3d& v1, Vector3d& v2);
void esportazione(Traces& traccia, Fractures& frattura);
bool valuta_intersezione (Fractures& frattura, unsigned int& Id1, unsigned int& Id2);
array<double,6> Retta_tra_piani(Fractures& frattura, unsigned int& id1, unsigned int& id2);
array<double,6> Retta_per_due_vertici_della_frattura(Fractures& frattura, unsigned int& id, unsigned int& i,unsigned int& j);
Vector2d alpha_di_intersezione(array<double,6> r_intersez,array<double,6> r_fratt);
}

#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

namespace FracturesLib{

struct Fractures{
    unsigned int NumberFractures = 0;
    unsigned int Id;
    map<unsigned int, Matrix<double, 3, Dynamic>> Vertices;
};

struct Traces{
    unsigned int Id;
    map<unsigned int, array<unsigned int, 2>> FracturesId;
    map<unsigned int, array<Vector3d,2>> Vertices;
    bool Tips;
};

bool importazione(const string& filename, Fractures& fracture);
double distanza_al_quadrato(Vector3d& v1, Vector3d& v2);
void esportazione(Traces& traccia, Fractures& frattura);
bool valuta_intersezione (Fractures& frattura, unsigned int& Id1, unsigned int& Id2);
}


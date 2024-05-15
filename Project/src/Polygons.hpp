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
    map<unsigned int, array<double,4>> Piano;
};

struct Traces{
    unsigned int Id;
    map<unsigned int, array<unsigned int, 2>> FracturesId;
    map<unsigned int, array<Vector3d,2>> Vertices;
    map<unsigned int, array<bool,2>> Tips;
    // il primo bool per il poligono FracturesId[i][0] e il secondo per FracturesId[i][1]
};

}


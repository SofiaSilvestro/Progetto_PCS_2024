#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

namespace FracturesLib{

struct Fractures{

    // Variabile che tiene traccia del numero di fratture
    unsigned int NumberFractures = 0;

    // Identificatore univoco per ogni frattura
    unsigned int Id;

    // Mappa che associa un id di frattura a una matrice di vertici
    // La matrice ha 3 righe(coord x, y,z) e un numero dinamico di colonne (il numero dei vertici è variabile)
    map<unsigned int, Matrix<double, 3, Dynamic>> Vertices;

    // Mappa che associa un id di frattura a un array di 4 elementi di tipo double (che rappresentano a,b,c,d dell'eq. del piano ax+by+cz+d=0 su cui è appoggiato il poligono)
    map<unsigned int, array<double,4>> Piano;
};

struct Traces{

    // Identificatore univoco per ogni traccia
    unsigned int Id;

    // Mappa che associa un id di traccia a un array di due id di frattura.
    map<unsigned int, array<unsigned int, 2>> FracturesId;

    // Mappa che associa un id di traccia a un array di due vettori tridimensionali (che rappresentano le coordinate dei 2 vertici della traccia)
    map<unsigned int, array<Vector3d,2>> Vertices;

    // Mappa che associa un id di traccia a un array di due valori booleani (che ci dicono se la traccia è passante [false] oppure non-passante [true]) per ciascuno delle due fratture che definiscono la traccia)
    // il primo bool per il poligono FracturesId[i][0] e il secondo per FracturesId[i][1]
    map<unsigned int, array<bool,2>> Tips;

    // Vettore che indica la lunghezza delle tracce
    vector<double> Lenght;
};

}


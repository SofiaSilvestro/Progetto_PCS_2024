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

//!\\ da capire se vogliamo spostare questo namespace in un altro file
namespace PolygonalLibrary {

struct PolygonalMesh
{
    ///CELLOD
    unsigned int NumberCell0D = 0; //number of Cell0D
    vector<unsigned int> Cell0DId = {}; //Cell0D id, size 1 x NumberCell1D, size 1 x NumberCell0D
    vector<Vector3d> Cell0DCoordinates = {}; // Cell0D coordinates, size 3 x NumberCell0D

    ///CELL1D
    unsigned int NumberCell1D = 0; // number of Cell1D
    vector<unsigned int> Cell1DId = {}; // Cell1D id, size 1 x NumberCell1D
    vector<Vector2i> Cell1DVertices = {}; // Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)

    ///CELL2D
    unsigned int NumberCell2D = 0; // number of Cell2D
    vector<unsigned int> Cell2DId = {}; // Cell2D id, size 1 x NumberCell2D
    vector<vector<unsigned int>> Cell2DVertices = {}; // Cell2D Vertices indices, size 1 x NumberCell2DVertices[NumberCell2D]
    vector<vector<unsigned int>> Cell2DEdges = {};// Cell2D Cell1D indices, size 1 x NumberCell2DEdges[NumberCell2D]
};

}


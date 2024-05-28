#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

namespace FracturesLib{

struct Fractures
{
    // Variabile di tipo intero senza segno che salva il numero di fratture
    unsigned int NumberFractures = 0;

    // Identificatore univoco per ogni frattura
    unsigned int Id = 0;

    // Vettore di matrici contenente tutti i vertici
    // La matrice ha 3 righe(coord x, y,z) e un numero dinamico di colonne
    // (il numero dei vertici è variabile)
    vector<Matrix<double, 3, Dynamic>> Vertices = {};

    // Vettore di array formati da 4 elementi di tipo double
    // (che rappresentano a,b,c,d dell'eq. del piano ax+by+cz+d=0 su cui è appoggiato il poligono)
    vector<array<double, 4>> Piano = {};
};


struct Traces
{
    // Variabile di tipo intero senza segno che salva il numero di tracce
    unsigned int Number = 0;

    // Vettore di array di dimensione 2 che contiene gli identificatori delle fratture che generano la traccia
    vector<array<unsigned int, 2>> FracturesId = {};

    // Vettore di array di dimensione 2 contenente 2 vettori tridimensionali
    // (che rappresentano le coordinate dei 2 vertici della traccia)
    vector<array<Vector3d, 2>> Vertices = {};

    // Vettore di array formato da due valori booleani
    // (che contengono informazione rigurado se la traccia è passante [false] oppure non-passante [true]
    // per ciascuna delle due fratture che definiscono la traccia)
    // il primo bool per il poligono FracturesId[i][0] e il secondo per FracturesId[i][1]
    vector<array<bool, 2>> Tips = {};
};
} // chiusura namespace FracturesLib

namespace PolygonalLibrary{

struct PolygonalMesh
{
    ///CELLOD
    unsigned int NumberCell0D = 0; //number of Cell0D
    vector<unsigned int> Cell0DId = {}; //Cell0D id, size 1 x NumberCell1D, size 1 x NumberCell0D
    vector<Vector3d> Cell0DCoordinates = {}; // Cell0D coordinates, size 3 x NumberCell0D

    ///CELL1D
    unsigned int NumberCell1D = 0; // number of Cell1D
    vector<unsigned int> Cell1DId = {}; // Cell1D id, size 1 x NumberCell1D
    vector<array<unsigned int,2>> Cell1DIdVertices = {};

    ///CELL2D
    unsigned int NumberCell2D = 0; // number of Cell2D
    vector<unsigned int> Cell2DId = {}; // Cell2D id, size 1 x NumberCell2D
    vector<vector<unsigned int>> Cell2DVertices = {}; // Cell2D Vertices indices, size 1 x NumberCell2DVertices[NumberCell2D]
    vector<vector<unsigned int>> Cell2DEdges = {};// Cell2D Cell1D indices, size 1 x NumberCell2DEdges[NumberCell2D]
};
}// chiusura namespace PolygonalLibrary


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
    // Variabile che tiene traccia del numero di fratture
    unsigned int NumberFractures = 0;

    // Identificatore univoco per ogni frattura
    unsigned int Id = 0;

    // Mappa che associa un id di frattura a una matrice di vertici
    // La matrice ha 3 righe(coord x, y,z) e un numero dinamico di colonne (il numero dei vertici è variabile)
    map<unsigned int, Matrix<double, 3, Dynamic>> Vertices = {};

    // Mappa che associa un id di frattura a un array di 4 elementi di tipo double (che rappresentano a,b,c,d dell'eq. del piano ax+by+cz+d=0 su cui è appoggiato il poligono)
    map<unsigned int, array<double, 4>> Piano = {};
};


struct Traces
{
    // numero tracce
    unsigned int Number = {};

    // Mappa che associa un id di traccia a un array di due id di frattura.
    map<unsigned int, array<unsigned int, 2>> FracturesId = {};

    // Mappa che associa un id di traccia a un array di due vettori tridimensionali (che rappresentano le coordinate dei 2 vertici della traccia)
    map<unsigned int, array<Vector3d, 2>> Vertices = {};

    // Mappa che associa un id di traccia a un array di due valori booleani (che ci dicono se la traccia è passante [false] oppure non-passante [true]) per ciascuno delle due fratture che definiscono la traccia)
    // il primo bool per il poligono FracturesId[i][0] e il secondo per FracturesId[i][1]
    map<unsigned int, array<bool, 2>> Tips = {};

    // Vettore che indica la lunghezza delle tracce
    map<unsigned int, double> Lenght = {};
};

} // chiusura namespace FracturesLib

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

//!\\ da capire se vogliamo spostare questo namespace in un altro file
namespace SortLibrary {

template<typename T>
void Merge(vector<T>& v,
           const unsigned int& sx,
           const unsigned int& cx,
           const unsigned int& dx){

    unsigned int i = sx;
    unsigned int j = cx + 1;

    vector<T> b;
    b.reserve(dx - sx + 1);

    while( i <= cx && j <= dx)
    {
        if (v[i] <= v[j])
            b.push_back(v[i++]);
        else
            b.push_back(v[j++]);
    }

    if (i <= cx)
        b.insert(b.end(), v.begin() + i, v.begin() + cx + 1);
    if ( j <= dx)
        b.insert(b.end(), v.begin() + j, v.begin() + dx + 1);

    copy(b.begin(), b.end(), v.begin() + sx);

}

template<typename T>
void MergeSort(vector<T>& v,
               const unsigned int& sx,
               const unsigned int& dx){

    if (sx < dx)
    {
        unsigned int cx = (sx + dx) / 2;
        MergeSort(v, sx, cx);
        MergeSort(v, cx + 1, dx);

        Merge(v, sx, cx, dx);
    }

}

template<typename T>
void MergeSort(vector<T>& v){
    MergeSort(v, 0, v.size()-1);
}


template<typename T>
void BubbleSort(std::vector<T>& data)
{
    size_t rem_size = data.size();
    size_t last_seen = rem_size;
    bool swapped = true;

    while (swapped) {
        swapped = false;
        for (size_t i = 1; i < rem_size; i++) {
            if (data[i-1] > data[i]) {
                std::swap(data[i-1], data[i]);
                swapped = true;
                last_seen = i;
            }
        }
        //        rem_size = rem_size - 1;
        rem_size = last_seen;
    }
}

}


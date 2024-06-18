#pragma once
#include <iostream>
#include "Polygons.hpp"

using namespace std;
using namespace Eigen;

namespace DFNLibrary{

// Import per lettura dati da file
bool importazione(const string& filename, Fractures& frattura);

// Calcolo equazione piano su cui giace ogni frattura
array<double, 4> EqPiano(Fractures& frattura, unsigned int& Id);

// Distanza euclidea al quadrato tra due vettori di dimensione 3
double distanza_al_quadrato(Vector3d& vec1, Vector3d& vec2);

// Calcolo baricentro poligono
Vector3d baricentro (Fractures& frattura, unsigned int& Id1);

// Determina se due fratture potrebbero intersecarsi con calcolo distanza tra baricentri e sfere circosritte
bool valuta_intersezione (Fractures& frattura, unsigned int& Idfrac1, unsigned int& Idfrac2);

// Retta di intersezione tra due fratture
array<double,6> Retta_tra_piani(Fractures& frattura, unsigned int& idfrac1, unsigned int& idfrac2);

// Retta passante per due vertici di una frattura
array<double,6> Retta_per_due_vertici_della_frattura(Fractures& frattura, unsigned int& id, unsigned int& vert1,unsigned int& vert2);

// Calcolo ascisse curvilinee rette che si intersecano per trovare punto di intersezione
Vector2d alpha_di_intersezione(array<double, 6>& r_intersez,array<double, 6>& r_fratt);

// Carico dati nella struttura
void caricamento_dati(Traces& traccia, Fractures& frattura);

// Ordinamento array<double, 6>& r_fratt
bool compare(array<double, 2> arr1, array<double, 2> arr2);

// Esportazione dei dati in un file di testo
void esportazione(Traces& traccia, Fractures& frattura);
}


//-----PARTE 2------------------------------------------------------------------------------------------------
using namespace DFNLibrary;

namespace PolygonalLibrary{
void caricamento_dati_2(Traces& traccia, Fractures& frattura, PolygonalMesh& mesh);
}

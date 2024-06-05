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
double distanza_al_quadrato(Vector3d& v1, Vector3d& v2);

// Calcolo baricentro poligono
Vector3d baricentro (Fractures& frattura, unsigned int& Id1);

// Determina se due fratture potrebbero intersecarsi con calcolo distanza tra baricentri e sfere circosritte
bool valuta_intersezione (Fractures& frattura, unsigned int& Id1, unsigned int& Id2);

// Retta di intersezione tra due fratture
array<double,6> Retta_tra_piani(Fractures& frattura, unsigned int& id1, unsigned int& id2);

// Retta passante per due vertici di una frattura
array<double,6> Retta_per_due_vertici_della_frattura(Fractures& frattura, unsigned int& id, unsigned int& i,unsigned int& j);

// Calcolo ascisse curvilinee rette che si intersecano per trovare punto di intersezione
Vector2d alpha_di_intersezione(array<double, 6> r_intersez, array<double, 6> r_fratt);

// Carico dati nella struttura
void caricamento_dati(Traces& traccia, Fractures& frattura);

// Ordinamento tracce
bool compare(array<double,2> a, array<double,2> b);

// Esportazione dei dati in un file di testo
void esportazione(Traces& traccia, Fractures& frattura);

Vector3d intersezione_rette(Vector3d& V1, Vector3d& V2, Vector3d& V3, Vector3d& V4);

array<double, 6> coord_retta_tra2punti(Vector3d& V1, Vector3d& V2);
}


//-----PARTE 2------------------------------------------------------------------------------------------------
using namespace DFNLibrary;

namespace PolygonalLibrary{
void caricamento_dati_2(Traces& traccia, Fractures& frattura);
Vector3d intersezione_rette(array<double, 6>& r1, array<double, 6>& r2);
array<double, 6> coord_retta_tra2punti(Vector3d& V1, Vector3d& V2);

}

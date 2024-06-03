// Sviluppiamo la seconda parte del progetto
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "Polygons.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;
using namespace DFNLibrary;

namespace PolygonalLibrary{

double distanza_al_quadrato_2(Vector3d& v1, Vector3d& v2)
{
    return (v1[0]-v2[0])*(v1[0]-v2[0]) + (v1[1]-v2[1])*(v1[1]-v2[1]) + (v1[2]-v2[2])*(v1[2]-v2[2]);
}

bool compare_2(array<double, 2> a, array<double, 2> b)
{
    return (a[1] > b[1]);
}

void caricamento_dati_2(Traces& traccia, Fractures& frattura)
{
    unsigned int conta_vertici = 0;
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        cout << "Poligono " << i << endl;
        // Salvo i vertici che già ci sono, cioè i vertici dei poligoni presi dal file
        for(unsigned int v = 0; v < 4; v++)
        {
            Vector3d vertice = frattura.Vertices[i].col(v);
            cout << "Vertice " << conta_vertici << ": " << setprecision(16) << scientific << vertice[0] << " " << vertice[1] << " " << vertice[2] << endl;
            conta_vertici++;
        }
        if(traccia.frattura_traccia[i] != 0)
        {
            int contatore = 0;
            while(contatore < 2)
            {
                unsigned int conta_per_tipo = 0;
                vector<array<double, 2>> ordinamento = {};
                array<double, 2> ord = {};
                for(unsigned int j = 0; j < traccia.FracturesId.size(); j++)
                {
                    // Se il primo o il secondo id è i, cioè la frattura i è coinvolta nella traccia j
                    if((i == traccia.FracturesId[j][0] || i == traccia.FracturesId[j][1]) && traccia.Tips[j][0] == contatore)
                    {
                        conta_per_tipo++;
                        ord[0] = j;
                        ord[1] = sqrt(distanza_al_quadrato_2(traccia.Vertices[j][0], traccia.Vertices[j][1]));
                        //ordinamento.push_back(ord);
                    }
                }
                // Condizioni per ordinamento vettore usare sort
                sort(ordinamento.begin(), ordinamento.end(), compare_2);
                // Caso di traccia passante
                /*if(contatore == 0)
                {
                    for(unsigned int k = 0; k < conta_per_tipo; k++)
                    {
                        int id_traccia = 0;
                        id_traccia = int(ordinamento[k][0]);
                        cout << id_traccia;
                        Vector3d punto1 = traccia.Vertices[id_traccia][0];
                        Vector3d punto2 = traccia.Vertices[id_traccia][1];
                        cout << "Vertice " << conta_vertici << ":" << punto1[0] << " " << punto1[1] << " " << punto1[2] << endl;
                        conta_vertici++;
                        cout << "Vertice " << conta_vertici << ":" << punto2[0] << " " << punto2[1] << " " << punto2[2] << endl;
                        conta_vertici++;
                    }
                }*/
                //Caso di traccia non passante da sviluppare

                contatore++;
            }
        }
    }
}



}

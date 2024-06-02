// Sviluppiamo la seconda parte del progetto in modo indipendente dalla prima parte
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
using namespace FracturesLib;

namespace PolygonalLibrary{
double distanza_al_quadrato(Vector3d& v1, Vector3d& v2)
{
    return (v1[0]-v2[0])*(v1[0]-v2[0]) + (v1[1]-v2[1])*(v1[1]-v2[1]) + (v1[2]-v2[2])*(v1[2]-v2[2]);
}
bool compare(array<double, 2> a, array<double, 2> b)
{
    return (a[1] > b[1]);
}

void caricamento_dati_2(Traces& traccia, Fractures& frattura)
{
    unsigned int conta_vertici=0;
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        for(unsigned int v=0;v<4;v++){
            Vector3d vertice=frattura.Vertices[i].col(v);
            cout<<"Vertice"<<conta_vertici<<":"<<setprecision(16)<<scientific<<vertice[0]<<" "<<vertice[1]<<" "<<vertice[2]<<endl;
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
                    // Se il primo o il secondo id è i
                    if((i == traccia.FracturesId[j][0] || i == traccia.FracturesId[j][1]) && traccia.Tips[j][0] == contatore)
                    {
                        conta_per_tipo++;
                        ord[0] = j;
                        ord[1] = sqrt(distanza_al_quadrato(traccia.Vertices[j][0], traccia.Vertices[j][1]));
                    }
                }
                // Condizioni per ordinamento vettore usare sort
                sort(ordinamento.begin(), ordinamento.end(), compare);
                // Stampiamo dall'inizio alla fine
                for(unsigned int k = 0; k < conta_per_tipo; k++)
                {
                    //ofs << int(ordinamento[k][0]) << "; " << contatore << "; " << ordinamento[k][1] << endl;
                }
                contatore++;
            }
        }
    }

}
}

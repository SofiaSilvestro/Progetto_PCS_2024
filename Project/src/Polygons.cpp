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
    unsigned int conta_vertici=0;
    double tol = 1e-10;
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        unsigned int conta_tracce_per_fratt=0;
        for(unsigned int j = 0; j < traccia.FracturesId.size(); j++)
        {
            // se il primo o il secondo id è i allora incremento di 1 il numero delle tracce
            if(i == traccia.FracturesId[j][0] || i == traccia.FracturesId[j][1])
            {
                conta_tracce_per_fratt = conta_tracce_per_fratt + 1;
            }
        }
        traccia.frattura_traccia.push_back(conta_tracce_per_fratt);
    }
    //cout<<traccia.frattura_traccia.size();
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        cout<<"POLIGONO : "<<i<<endl;
        //salvo i vertici che già ci sono
        for(unsigned int v=0;v<4;v++){
            Vector3d vertice=frattura.Vertices[i].col(v);
            cout<<"Vertice "<<conta_vertici<<":"<<setprecision(16)<<scientific<<vertice[0]<<" "<<vertice[1]<<" "<<vertice[2]<<endl;
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
                        ord[1] = sqrt(distanza_al_quadrato_2(traccia.Vertices[j][0], traccia.Vertices[j][1]));
                        ordinamento.push_back(ord);
                    }
                }
                // Condizioni per ordinamento vettore usare sort
                sort(ordinamento.begin(), ordinamento.end(), compare_2);
                // Caso di traccia passante
                if(contatore==0){
                    for(unsigned int k = 0; k < conta_per_tipo; k++)
                    {
                        int id_traccia=0;
                        id_traccia=int(ordinamento[k][0]);
                        Vector3d punto1=traccia.Vertices[id_traccia][0];
                        Vector3d punto2=traccia.Vertices[id_traccia][1];
                        cout<<"Vertice* "<<conta_vertici<<":"<<punto1[0]<<" "<<punto1[1]<<" "<<punto1[2]<<endl;
                        conta_vertici++;
                        cout<<"Vertice* "<<conta_vertici<<":"<<punto2[0]<<" "<<punto2[1]<<" "<<punto2[2]<<endl;
                        conta_vertici++;
                    }
                }
                //Caso di traccia non passante
                else{
                    for(unsigned int k = 0; k < conta_per_tipo; k++)
                    {
                        int id_traccia=0;
                        id_traccia=int(ordinamento[k][0]);
                        unsigned int id_frattura1=traccia.FracturesId[id_traccia][0];
                        unsigned int id_frattura2=traccia.FracturesId[id_traccia][1];
                        if(i==id_frattura1){
                            array<double, 6> r_piano = Retta_tra_piani(frattura, i, id_frattura2);
                            unsigned int h = 0; // Usato per accedere a tutti i vertici della frattura
                            unsigned int k = 1; // Usato per accedere a tutti i vertici della stessa frattura a partire dal secondo
                            while(h < frattura.Vertices[i].cols())
                            {
                                // Con l'if gestisco il caso dell'ultimo vertice con il primo del poligono
                                if(k == frattura.Vertices[i].cols())
                                {
                                    k = 0;
                                }
                                array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, i, h, k);
                                // Escludo il parallelismo: calcolo prodotto vettoriale e verifico che sia maggiore di tol
                                double parallelo = (r_piano[1] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[1]) - ((r_piano[0] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[0])) + (r_piano[0] * r_tra_punti[1]) - (r_piano[1] * r_tra_punti[0]);
                                if (abs(parallelo) > tol)
                                {
                                    Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                                    Vector3d punto_intersezione = {};
                                    punto_intersezione[0] = r_tra_punti[0] * x[0] + r_tra_punti[3];
                                    punto_intersezione[1] = r_tra_punti[1] * x[0] + r_tra_punti[4];
                                    punto_intersezione[2] = r_tra_punti[2] * x[0] + r_tra_punti[5];
                                    cout<<"Vertice* "<<conta_vertici<<":"<<punto_intersezione[0]<<" "<<punto_intersezione[1]<<" "<<punto_intersezione[2]<<endl;
                                    conta_vertici++;
                                }
                                h++;
                                k++;
                            }
                        }
                        if(i==id_frattura2){
                            array<double, 6> r_piano = Retta_tra_piani(frattura, i, id_frattura1);
                            unsigned int h = 0; // Usato per accedere a tutti i vertici della frattura
                            unsigned int k = 1; // Usato per accedere a tutti i vertici della stessa frattura a partire dal secondo
                            while(h < frattura.Vertices[i].cols())
                            {
                                // Con l'if gestisco il caso dell'ultimo vertice con il primo del poligono
                                if(k == frattura.Vertices[i].cols())
                                {
                                    k = 0;
                                }
                                array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, i, h, k);
                                // Escludo il parallelismo: calcolo prodotto vettoriale e verifico che sia maggiore di tol
                                double parallelo = (r_piano[1] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[1]) - ((r_piano[0] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[0])) + (r_piano[0] * r_tra_punti[1]) - (r_piano[1] * r_tra_punti[0]);
                                if (abs(parallelo) > tol)
                                {
                                    Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                                    Vector3d punto_intersezione = {};
                                    punto_intersezione[0] = r_tra_punti[0] * x[0] + r_tra_punti[3];
                                    punto_intersezione[1] = r_tra_punti[1] * x[0] + r_tra_punti[4];
                                    punto_intersezione[2] = r_tra_punti[2] * x[0] + r_tra_punti[5];
                                    cout<<"Vertice* "<<conta_vertici<<":"<<punto_intersezione[0]<<" "<<punto_intersezione[1]<<" "<<punto_intersezione[2]<<endl;
                                    conta_vertici++;
                                }
                                h++;
                                k++;
                            }
                        }
                    }
                }//chiusura if contatore
                contatore++;
            } //chiusura while
        }
        cout<<endl;
    }
}



}

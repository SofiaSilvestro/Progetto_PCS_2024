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

void caricamento_dati_2(Traces& traccia, Fractures& frattura, PolygonalMesh& mesh)
{
    double tol = 1e-10;
    vector<unsigned int> frattura_traccia = {};

    array<unsigned int , 2> aggiorna_1 = {};
    array<unsigned int , 2> aggiorna_2 = {};
    array<Vector3d, 2> punti_nuovi = {};
    unsigned int conta = 0;
  
    frattura_traccia.reserve(traccia.NumberTraces);
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        unsigned int conta_tracce_per_fratt = 0;
        unsigned int conta_tracce = 0;
        for(unsigned int j = 0; j < traccia.FracturesId.size(); j++)
        {
            // se il primo o il secondo id è i allora incremento di 1 il numero delle tracce
            if(i == traccia.FracturesId[j][0] || i == traccia.FracturesId[j][1])
            {
                conta_tracce_per_fratt = conta_tracce_per_fratt + 1;
            }
        }
        frattura_traccia.push_back(conta_tracce_per_fratt);
    }
    unsigned int conta_0d=0;
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        cout<<"POLIGONO: "<< i <<endl;
        if(frattura_traccia[i] != 0)
        {
            for(unsigned int v = 0; v < 4; v++)
            {
            Vector3d vertice = frattura.Vertices[i].col(v);
                cout<<setprecision(16)<<scientific<<vertice[0]<<" "<<vertice[1]<<" "<<vertice[2]<<endl;
                cout<<conta_0d<<endl;
                mesh.Cell0DId.push_back(conta_0d);
                mesh.Cell0DCoordinates.push_back(vertice);
                conta_0d++;
            }
            array<MatrixXd,2> sottopoligoni;
            int contatore = 0;
            unsigned int conta=0;
            while(contatore < 2)
            {
                unsigned int conta_per_tipo = 0;
                unsigned int giro = 0;
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
                        ordinamento.push_back(ord);
                    }
                }
                // Condizioni per ordinamento vettore usare sort
                sort(ordinamento.begin(), ordinamento.end(), compare);

                // I sottopoligoni
                for(unsigned int k = 0; k < conta_per_tipo; k++)
                {

                    array<unsigned int, 2> posizione = {};
                    array<unsigned int, 2> posizione_bis = {};
                    unsigned int posto = 0;
                    int id_traccia = 0;
                    id_traccia = int(ordinamento[k][0]);
                    unsigned int id_frattura1 = traccia.FracturesId[id_traccia][0];
                    unsigned int id_frattura2 = traccia.FracturesId[id_traccia][1];
                    if (conta!=0 && conta!=frattura_traccia[i]){
                        // punti tutti a sinistra rispetto alla traccia precedente
                        if(max(traccia.Vertices[id_traccia][0][0],traccia.Vertices[id_traccia][1][0])<= min(punti_nuovi[0][0],punti_nuovi[1][0])-tol)
                        {
                            frattura.Vertices[i]=sottopoligoni[0];
                            cout<<endl;
                            cout<<sottopoligoni[1](0,0)<<" "<<sottopoligoni[1](1,0)<<" "<<sottopoligoni[1](2,0)<<" "<<endl;
                            cout<<sottopoligoni[1](0,1)<<" "<<sottopoligoni[1](1,1)<<" "<<sottopoligoni[1](2,1)<<" "<<endl;
                            cout<<sottopoligoni[1](0,2)<<" "<<sottopoligoni[1](1,2)<<" "<<sottopoligoni[1](2,2)<<" "<<endl;
                            cout<<sottopoligoni[1](0,3)<<" "<<sottopoligoni[1](1,3)<<" "<<sottopoligoni[1](2,3)<<" "<<endl<<endl;
                        }
                        // punti tutti a destra rispetto alla traccia precedente
                        if(min(traccia.Vertices[id_traccia][0][0],traccia.Vertices[id_traccia][1][0])>= min(punti_nuovi[0][0],punti_nuovi[1][0])-tol)
                        {
                            frattura.Vertices[i]=sottopoligoni[1];
                            cout<<endl;
                            cout<<sottopoligoni[1](0,0)<<" "<<sottopoligoni[1](1,0)<<" "<<sottopoligoni[1](2,0)<<" "<<endl;
                            cout<<sottopoligoni[1](0,1)<<" "<<sottopoligoni[1](1,1)<<" "<<sottopoligoni[1](2,1)<<" "<<endl;
                            cout<<sottopoligoni[1](0,2)<<" "<<sottopoligoni[1](1,2)<<" "<<sottopoligoni[1](2,2)<<" "<<endl;
                            cout<<sottopoligoni[1](0,3)<<" "<<sottopoligoni[1](1,3)<<" "<<sottopoligoni[1](2,3)<<" "<<endl<<endl;
                        }
                    }
                    if(i == id_frattura1)
                    {
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
                                cout<<x[0]<<" "<<x[1]<<endl;
                                Vector3d punto_intersezione = {};
                                punto_intersezione[0] = r_tra_punti[0] * x[0] + r_tra_punti[3];
                                punto_intersezione[1] = r_tra_punti[1] * x[0] + r_tra_punti[4];
                                punto_intersezione[2] = r_tra_punti[2] * x[0] + r_tra_punti[5];
                                punti_nuovi[posto] = punto_intersezione;
                                cout<<"**"<<punto_intersezione[0]<<" "<<punto_intersezione[1]<<" "<<punto_intersezione[2]<<endl;
                                cout<<conta_0d<<endl;
                                conta_0d++;
                                if(giro == 0)
                                {
                                    posizione[0] = k;
                                    posizione[1] = k + 1;
                                    posizione_bis[0] = k - 1;
                                    posizione_bis[1] = k + 2;
                                    if(posizione_bis[1] == 4)
                                    {
                                        posizione_bis[1] = 0;
                                    }
                                    if(posizione[1] == 4)
                                    {
                                        posizione_bis[1] = 0;
                                    }
                                    giro++;
                                }
                                posto++;
                            }
                            h++;
                            k++;
                        } // Chiusura while
                        //SOTTOPOLIGONO 1
                        for(unsigned int v = 0; v < 4; v++)
                        {
                            if(v != posizione[0] && v != posizione[1])
                            {
                                Vector3d vertice = frattura.Vertices[i].col(v);
                            }
                            if(v == posizione[0])
                            {
                                aggiorna_1[0] = v;
                            }
                            if(v == posizione[1])
                            {
                                aggiorna_1[1] = v;
                            }
                        }
                        //SOTTOPOLIGONO 2
                        for(unsigned int v = 0; v < 4; v++)
                        {
                            if(v != posizione_bis[0] && v != posizione_bis[1])
                            {
                                Vector3d vertice = frattura.Vertices[i].col(v);
                            }
                            if(v == posizione_bis[0])
                            {
                                aggiorna_2[0] = v;
                            }
                            if(v == posizione_bis[1])
                            {
                                aggiorna_2[1] = v;
                            }
                        }
                        //Quale poligono devo aggiornare per il taglio??
                        MatrixXd copiafrattura_1=frattura.Vertices[i];
                        MatrixXd copiafrattura_2=frattura.Vertices[i];
                        copiafrattura_1.col(aggiorna_1[0]) = punti_nuovi[0];
                        copiafrattura_1.col(aggiorna_1[1]) = punti_nuovi[1];
                        copiafrattura_2.col(aggiorna_2[0]) = punti_nuovi[0];
                        copiafrattura_2.col(aggiorna_2[1]) = punti_nuovi[1];
                        sottopoligoni[0]=copiafrattura_1;
                        sottopoligoni[1]=copiafrattura_2;
                    }

                    if(i == id_frattura2)
                    {
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
                                punti_nuovi[posto] = punto_intersezione;
                                if(giro == 0)
                                {
                                    posizione[0] = k;
                                    posizione[1] = k + 1;
                                    posizione_bis[0] = k - 1;
                                    posizione_bis[1] = k + 2;
                                    if(posizione_bis[1] == 4)
                                    {
                                        posizione_bis[1] = 0;
                                    }
                                    if(posizione[1] == 4)
                                    {
                                        posizione_bis[1] = 0;
                                    }
                                    giro++;
                                }
                                posto++;
                            }
                            h++;
                            k++;
                        }
                        //SOTTOPOLIGONO 1
                        for(unsigned int v = 0; v < 4; v++)
                        {
                            if(v != posizione[0] && v != posizione[1])
                            {
                                Vector3d vertice = frattura.Vertices[i].col(v);
                            }
                            if(v == posizione[0])
                            {
                                aggiorna_1[0] = v;
                            }
                            if(v == posizione[1])
                            {
                                aggiorna_1[1] = v;
                            }
                        }
                        //SOTTOPOLIGONO 2
                        for(unsigned int v = 0; v < 4; v++)
                        {
                            if(v != posizione_bis[0] && v != posizione_bis[1])
                            {
                                Vector3d vertice = frattura.Vertices[i].col(v);
                            }
                            if(v == posizione_bis[0])
                            {
                               aggiorna_2[0] = v;
                            }
                            if(v == posizione_bis[1])
                            {
                                aggiorna_2[1] = v;
                            }
                        }
                        //Quale poligono devo aggiornare per il taglio??
                        MatrixXd copiafrattura_1=frattura.Vertices[i];
                        MatrixXd copiafrattura_2=frattura.Vertices[i];
                        copiafrattura_1.col(aggiorna_1[0]) = punti_nuovi[0];
                        copiafrattura_1.col(aggiorna_1[1]) = punti_nuovi[1];
                        copiafrattura_2.col(aggiorna_2[0]) = punti_nuovi[0];
                        copiafrattura_2.col(aggiorna_2[1]) = punti_nuovi[1];
                        sottopoligoni[0]=copiafrattura_1;
                        sottopoligoni[1]=copiafrattura_2;
                    }
                    conta++;
                }
                contatore++;
            }
            cout<<endl;
            cout<<sottopoligoni[0](0,0)<<" "<<sottopoligoni[0](1,0)<<" "<<sottopoligoni[0](2,0)<<" "<<endl;
            cout<<sottopoligoni[0](0,1)<<" "<<sottopoligoni[0](1,1)<<" "<<sottopoligoni[0](2,1)<<" "<<endl;
            cout<<sottopoligoni[0](0,2)<<" "<<sottopoligoni[0](1,2)<<" "<<sottopoligoni[0](2,2)<<" "<<endl;
            cout<<sottopoligoni[0](0,3)<<" "<<sottopoligoni[0](1,3)<<" "<<sottopoligoni[0](2,3)<<" "<<endl<<endl;
            cout<<sottopoligoni[1](0,0)<<" "<<sottopoligoni[1](1,0)<<" "<<sottopoligoni[1](2,0)<<" "<<endl;
            cout<<sottopoligoni[1](0,1)<<" "<<sottopoligoni[1](1,1)<<" "<<sottopoligoni[1](2,1)<<" "<<endl;
            cout<<sottopoligoni[1](0,2)<<" "<<sottopoligoni[1](1,2)<<" "<<sottopoligoni[1](2,2)<<" "<<endl;
            cout<<sottopoligoni[1](0,3)<<" "<<sottopoligoni[1](1,3)<<" "<<sottopoligoni[1](2,3)<<" "<<endl<<endl;
        }
    }
}
}

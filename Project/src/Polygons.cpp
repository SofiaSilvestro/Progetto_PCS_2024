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
bool importazione_2(const string& filename, Fractures& frattura, PolygonalMesh& mesh)
{
    ifstream file(filename);
    if (!file.is_open())
        return false;
    string header;
    getline(file, header); // Leggo la prima riga da trascurare
    string line;
    getline(file,line); // Leggo la riga che contiene il numero di fratture
    frattura.NumberFractures = stoi(line);
    char separatore;
    int numVertices = 0;
    int contatore_vertici = 0; // Bisogna poi aggiungere 1 per avere il numero esatto di vertici perchè parto a contare da zero
    int contatore_lati = 0;
    array<unsigned int, 2> id_vertici = {};
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        getline(file, header); // Leggo la riga da trascurare
        getline(file, line);
        stringstream ss(line);
        ss >> frattura.Id >> separatore >> numVertices;
        // Creo la tabella che contiene le coordinate dei punti
        Vector3d vertice = {};
        Matrix<double, 3, Dynamic> Tab_coord_vertici(3, numVertices);
        getline(file, header); // Leggo la riga da trascurare
        string val = "";
        // Lettura delle coordinate dei vertici
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < numVertices; j++)
            {
                file >> val;
                Tab_coord_vertici(i, j) = stod(val);
            }
        }
        file >> separatore;
        for (int k = 0; k < numVertices; k++)
        {
            vertice = Tab_coord_vertici.col(k);
            // cout << "Vertice  " << contatore_vertici << setprecision(16) << scientific
                // << " " << vertice[0] << " " << vertice[1] << " " << vertice[2] << endl;
            mesh.Cell0DId.push_back(contatore_vertici);
            mesh.Cell0DCoordinates.push_back(vertice);
            if(k == numVertices - 1)
            {
                // cout <<"Lato  " << contatore_lati << " " << contatore_vertici << " "
                    // << contatore_vertici - numVertices + 1 << endl;
                mesh.Cell1DId.push_back(contatore_lati);
                id_vertici[0] = contatore_vertici;
                id_vertici[1] = contatore_vertici - numVertices + 1;
            }
            else
            {
                // cout << "Lato  " << contatore_lati << " " << contatore_vertici
                    // << " " << contatore_vertici + 1 << endl;
                mesh.Cell1DId.push_back(contatore_lati);
                id_vertici[0] = contatore_vertici;
                id_vertici[1] = contatore_vertici+1;
                mesh.Cell1DIdVertices.push_back(id_vertici);
            }
            contatore_lati++;
            contatore_vertici++;
        }
    }
    mesh.NumberCell0D = contatore_vertici + 1;
    mesh.NumberCell1D = contatore_lati + 1;
    file.close();
    return true;
}

/*void caricamento_dati_2(Traces& traccia, Fractures& frattura)
{
    unsigned int NumberTraces = 0;
    array<unsigned int, 2> Id = {};
    array<Vector3d, 2> Vertici = {};
    array<bool, 2> Tipo = {};

    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        unsigned int j = i + 1;
        while(j < frattura.NumberFractures)
        {
            // Verifichiamo che i poligoni abbiano distanza minore della somma dei due raggi delle palle
            if(valuta_intersezione(frattura, i, j))
            {
                array<double, 4> coeff = {};
                // calcolo la retta passante tra i due piani
                array<double, 6> r_piano = Retta_tra_piani(frattura, i, j);
                // sulla carta sappiamo che se il prodotto vettoriale delle due normali ai piani è zero allora sono paralleli
                // le coordinate del risultato sono memorizzate nei primi tre spazi dell'array r_piano
                if(abs(r_piano[0]) < 1e-7 && abs(r_piano[1]) < 1e-7 && abs(r_piano[2]) < 1e-7)
                {
                    //piani paralleli
                }
                else
                {
                    //piani non paralleli
                    //calcolo la retta tra i lati adiacenti del poligono 1
                    unsigned int h = 0;
                    unsigned int k = 1;
                    unsigned int cont = 0;
                    while(h < frattura.Vertices[i].cols())
                    {
                        //con l'if gestisco il caso dell'ultimo punto con il primo del poligono
                        if(k == frattura.Vertices[i].cols())
                        {
                            k = 0;
                        }
                        array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, i, h, k);
                        // Escludo il parallelismo: calcolo prodotto vettoriale
                        double parallelo = (r_piano[1] * r_tra_punti[2])
                                           - (r_piano[2] * r_tra_punti[1])
                                           - ((r_piano[0] * r_tra_punti[2])
                                              - (r_piano[2] * r_tra_punti[0]))
                                           + (r_piano[0] * r_tra_punti[1])
                                           - (r_piano[1] * r_tra_punti[0]);
                        if (abs(parallelo) < 1e-5)
                        {
                            // non fare niente
                        }
                        else
                        {
                            Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                            // CONDIZIONI : verifichiamo che appartenga al segmento
                            double tol = 1e-15;
                            if(x[0] >= -tol && x[0] <= 1 + tol)
                            {
                                coeff[cont] = x[1];
                                cont++;
                            }
                        }
                        h++;
                        k++;
                    }
                    //calcolo la retta tra i lati adiacenti del poligono 2
                    h = 0;
                    k = 1;
                    while(h < frattura.Vertices[j].cols())
                    {
                        //con l'if gestisco il caso dell'ultimo punto con il primo del poligono
                        if(k == frattura.Vertices[j].cols())
                        {
                            k = 0;
                        }
                        array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, j, h, k);
                        // Escludo il parallelismo: calcolo il prodotto vettoriale
                        double parallelo = (r_piano[1] * r_tra_punti[2])
                                           - (r_piano[2] * r_tra_punti[1])
                                           - ((r_piano[0] * r_tra_punti[2])
                                              - (r_piano[2] * r_tra_punti[0]))
                                           + (r_piano[0] * r_tra_punti[1])
                                           - (r_piano[1] * r_tra_punti[0]);
                        if (abs(parallelo) < 1e-5)
                        {
                            // non fare niente
                        }
                        else
                        {
                            Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                            double tol = 1e-15;
                            if(x[0] >= -tol && x[0] <= 1+tol){
                                coeff[cont] = x[1];
                                cont++;
                            }
                        }
                        h++;
                        k++;
                    }
                    if(cont == 4)
                    {
                        // Trovare ora l'intervallo di intersezione
                        double max_el_sx = max(coeff[0], coeff[1]);
                        double min_el_sx = min(coeff[0], coeff[1]);
                        double max_el_dx = max(coeff[2], coeff[3]);
                        double min_el_dx = min(coeff[2], coeff[3]);
                        double sx = min(min_el_sx, min_el_dx); // Estremo sinistro
                        double dx = max(max_el_sx, max_el_dx); // Estremo sinistro
                        // gli intervalli si sovrappongono se:
                        if (sx < dx)
                        {
                            NumberTraces++;
                            cout << endl << "I piani " << i << " " << j << " potrebbero intersecarsi" << endl;
                            Id[0] = i;
                            Id[1] = j;
                            //calcolo la retta tra i lati adiacenti del poligono 1
                            cout << "PRIMO POLIGONO " << endl;
                            unsigned int conta_p1 = 0;
                            unsigned int h = 0;
                            unsigned int k = 1;
                            while(h < frattura.Vertices[i].cols())
                            {
                                //con l'if gestisco il caso dell'ultimo punto con il primo del poligono
                                if(k == frattura.Vertices[i].cols())
                                {
                                    k = 0;
                                }
                                array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, i, h, k);
                                // Escludo il parallelismo: calcolo prodotto vettoriale
                                double parallelo = (r_piano[1] * r_tra_punti[2])
                                                   - (r_piano[2] * r_tra_punti[1])
                                                   - ((r_piano[0] * r_tra_punti[2])
                                                      - (r_piano[2] * r_tra_punti[0]))
                                                   + (r_piano[0] * r_tra_punti[1])
                                                   - (r_piano[1] * r_tra_punti[0]);
                                if (abs(parallelo) < 1e-5)
                                {
                                    // non fare niente
                                }
                                else
                                {
                                    Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                                    double tol = 1e-5;
                                    // CONDIZIONI : verifichiamo che appartenga al segmento e all'altro poligono
                                    if(x[0] >= -tol && x[0] <= 1 + tol)
                                    {
                                        if(x[1] > sx - tol && x[1] < dx + tol)
                                        {
                                            //Calcolo il punto di intersezione
                                            Vector3d punto_intersezione = {};
                                            punto_intersezione[0] = r_tra_punti[0] * x[0] + r_tra_punti[3];
                                            punto_intersezione[1] = r_tra_punti[1] * x[0] + r_tra_punti[4];
                                            punto_intersezione[2] = r_tra_punti[2] * x[0] + r_tra_punti[5];
                                            Vertici[conta_p1] = punto_intersezione;
                                            cout << "Il punto di intersezione e' " << setprecision(16) << punto_intersezione[0]
                                                 << " " << punto_intersezione[1] << " " << punto_intersezione[2] << endl;
                                            conta_p1++;
                                        }
                                    }
                                }
                                h++;
                                k++;
                            }


                            // Non mi serve calcolare altri punti
                            if(conta_p1 == 2)
                            {
                                // calcolo la retta tra i lati adiacenti del poligono 2
                                cout << endl << "SECONDO POLIGONO " << endl;
                                unsigned int conta_p2 = 0;
                                h = 0;
                                k = 1;
                                while(h < frattura.Vertices[j].cols())
                                {
                                    // con l'if gestisco il caso dell'ultimo punto con il primo del poligono
                                    if(k == frattura.Vertices[j].cols())
                                    {
                                        k = 0;
                                    }
                                    array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, j, h, k);
                                    // Escludo il parallelismo: calcolo il prodotto vettoriale
                                    double parallelo = (r_piano[1] * r_tra_punti[2])
                                                       - (r_piano[2] * r_tra_punti[1])
                                                       - ((r_piano[0] * r_tra_punti[2])
                                                          - (r_piano[2] * r_tra_punti[0]))
                                                       + (r_piano[0] * r_tra_punti[1])
                                                       - (r_piano[1] * r_tra_punti[0]);
                                    if (abs(parallelo) < 1e-5)
                                    {
                                        // non fare niente
                                    }
                                    else
                                    {
                                        Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                                        // CONDIZIONI: verifichiamo che appartenga al segmento
                                        double tol = 1e-15;
                                        if(x[0] >= -tol && x[0] <= 1 + tol)
                                        {
                                            if(x[1] > sx - tol && x[1] < dx + tol)
                                            {
                                                conta_p2++;
                                            }
                                        }
                                    }
                                    h++;
                                    k++;
                                }
                            }


                            if(conta_p1 == 0)
                            {
                                // calcolo la retta tra i lati adiacenti del poligono 2
                                cout << endl << "SECONDO POLIGONO " << endl;
                                unsigned int conta_p2 = 0;
                                h = 0;
                                k = 1;
                                while(h < frattura.Vertices[j].cols())
                                {
                                    // con l'if gestisco il caso dell'ultimo punto con il primo del poligono
                                    if(k == frattura.Vertices[j].cols())
                                    {
                                        k = 0;
                                    }
                                    array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, j, h, k);
                                    // Escludo il parallelismo: calcolo il prodotto vettoriale
                                    double parallelo = (r_piano[1] * r_tra_punti[2])
                                                       - (r_piano[2] * r_tra_punti[1])
                                                       - ((r_piano[0] * r_tra_punti[2])
                                                          - (r_piano[2] * r_tra_punti[0]))
                                                       + (r_piano[0] * r_tra_punti[1])
                                                       - (r_piano[1] * r_tra_punti[0]);
                                    if (abs(parallelo) < 1e-5)
                                    {
                                        // non fare niente
                                    }
                                    else
                                    {
                                        Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                                        // CONDIZIONI: verifichiamo che appartenga al segmento
                                        double tol = 1e-15;
                                        if(x[0] >= -tol && x[0] <= 1 + tol)
                                        {
                                            if(x[1] > sx - tol && x[1] < dx + tol)
                                            {
                                                //Calcolo il punto di intersezione
                                                Vector3d punto_intersezione = {};
                                                punto_intersezione[0] = r_tra_punti[0] * x[0] + r_tra_punti[3];
                                                punto_intersezione[1] = r_tra_punti[1] * x[0] + r_tra_punti[4];
                                                punto_intersezione[2] = r_tra_punti[2] * x[0] + r_tra_punti[5];
                                                Vertici[conta_p2] = punto_intersezione;
                                                cout << "Il punto di intersezione e' " << setprecision(16) << punto_intersezione[0]
                                                     << " " << punto_intersezione[1] << " " << punto_intersezione[2] << endl;
                                                conta_p2++;
                                            }
                                        }
                                    }
                                    h++;
                                    k++;
                                }
                            }


                            if(conta_p1 == 1)
                            {
                                //calcolo la retta tra i lati adiacenti del poligono 2
                                cout << endl << "SECONDO POLIGONO " << endl;
                                h = 0;
                                k = 1;
                                while(h < frattura.Vertices[j].cols())
                                {
                                    //con l'if gestisco il caso dell'ultimo punto con il primo del poligono
                                    if(k == frattura.Vertices[j].cols())
                                    {
                                        k = 0;
                                    }
                                    array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, j, h, k);
                                    // Escludo il parallelismo: calcolo il prodotto vettoriale
                                    double parallelo = (r_piano[1] * r_tra_punti[2])
                                                       - (r_piano[2] * r_tra_punti[1])
                                                       - ((r_piano[0] * r_tra_punti[2])
                                                          - (r_piano[2] * r_tra_punti[0]))
                                                       + (r_piano[0] * r_tra_punti[1])
                                                       - (r_piano[1] * r_tra_punti[0]);
                                    if (abs(parallelo) < 1e-5)
                                    {
                                        // non fare niente
                                    }
                                    else
                                    {
                                        Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                                        // CONDIZIONI: verifichiamo che appartenga al segmento
                                        double tol = 1e-15;
                                        if(x[0] >= -tol && x[0] <= 1 + tol){
                                            if(x[1] > sx - tol && x[1] < dx + tol)
                                            {
                                                //Calcolo il punto di intersezione
                                                Vector3d punto_intersezione = {};
                                                punto_intersezione[0] = r_tra_punti[0] * x[0] + r_tra_punti[3];
                                                punto_intersezione[1] = r_tra_punti[1] * x[0] + r_tra_punti[4];
                                                punto_intersezione[2] = r_tra_punti[2] * x[0] + r_tra_punti[5];
                                                Vertici[1] = punto_intersezione;
                                                cout << "Il punto di intersezione e' " << setprecision(16) << punto_intersezione[0]
                                                     << " " << punto_intersezione[1] << " " << punto_intersezione[2] << endl;
                                            }
                                        }
                                    }
                                    h++;
                                    k++;
                                }
                            }
                        }
                    }
                } // chiusura if
            }
            j++;
        }
    }
}
*/

}

#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include "Polygons.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;

namespace DFNLibrary{

bool importazione(const string& filename, Fractures& frattura)
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
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        getline(file, header); // Leggo la riga da trascurare
        getline(file, line);
        stringstream ss(line);
        ss >> frattura.Id >> separatore >> numVertices;
        // Creo la tabella che contiene le coordinate dei vertici
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
        // Aggiungo quanto trovato all'interno del vettore di matrici della struttura
        frattura.Vertices.push_back(Tab_coord_vertici);
    }
    // Calcolo i coefficienti del piano per ciascuna frattura, sfruttando le formule del piano passante per tre punti
    frattura.Plane.reserve(frattura.NumberFractures);
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        array<double, 4> param_piano = EqPiano(frattura, i);
        frattura.Plane.push_back(param_piano);
    }
    file.close();
    return true;
}

array<double, 4> EqPiano(Fractures& frattura, unsigned int& Id)
{
    array<double, 4> param_piano = {};
    Vector3d point1 = frattura.Vertices[Id].col(1);
    Vector3d point2 = frattura.Vertices[Id].col(2);
    Vector3d point3 = frattura.Vertices[Id].col(3);
    // Calcolo indiretto dei coefficienti mediante il determinante uguale a zero e relativo inserimento nel vettore di array
    // (x-x1  y-y1  z-z1
    //  x2-x1 y2-y1 z2-z1
    //  x3-x1 y3-y1 z3-z1)
    param_piano[0] = (point2[1] - point1[1]) * (point3[2] - point1[2]) - (point2[2] - point1[2]) * (point3[1] - point1[1]);
    param_piano[1] = - ((point2[0] - point1[0]) * (point3[2] - point1[2]) - (point3[0] - point1[0]) * (point2[2] - point1[2]));
    param_piano[2] = (point2[0] - point1[0]) * (point3[1] - point1[1]) - (point3[0] - point1[0]) * (point2[1] - point1[1]);
    param_piano[3] = - param_piano[0] * point1[0] - param_piano[1] * point1[1] - param_piano[2] * point1[2];
    return param_piano;
}

double distanza_al_quadrato(Vector3d& v1, Vector3d& v2)
{
    return (v1[0]-v2[0])*(v1[0]-v2[0]) + (v1[1]-v2[1])*(v1[1]-v2[1]) + (v1[2]-v2[2])*(v1[2]-v2[2]);
}

Vector3d baricentro (Fractures& frattura, unsigned int& Id)
{
    Vector3d coord_bar = {};
    unsigned int n = frattura.Vertices[Id].cols(); // Numero di vertici della frattura
    for(unsigned int h = 0; h < 3; h++)
    {
        for (unsigned int k = 0; k < n; k++)
        {
            coord_bar[h] = coord_bar[h] + frattura.Vertices[Id](h, k);
        }
        coord_bar[h] = coord_bar[h] / n;
    }
    return coord_bar;
}

bool valuta_intersezione (Fractures& frattura, unsigned int& Id1, unsigned int& Id2)
{
    // Definisco due vettori che contengono le coordinate del mio baricentro
    Vector3d coord_bar_1 = baricentro(frattura, Id1);
    Vector3d coord_bar_2 = baricentro(frattura, Id2);

    unsigned int n1 = frattura.Vertices[Id1].cols(); // Numero di vertici della prima frattura
    unsigned int n2 = frattura.Vertices[Id2].cols(); // Numero di vertici della seconda frattura

    // Calcolo i possibili raggi delle due palle avente centro nei baricentri precedentemente calcolati
    VectorXd raggi_candidati1 = {};
    raggi_candidati1.resize(n1);
    for(unsigned int i = 0; i < n1; i++)
    {
        Vector3d point = frattura.Vertices[Id1].col(i);
        raggi_candidati1(i) = distanza_al_quadrato(coord_bar_1, point);
    }
    VectorXd raggi_candidati2 = {};
    raggi_candidati2.resize(n2);
    for(unsigned int i = 0; i < n2; i++)
    {
        Vector3d point = frattura.Vertices[Id2].col(i);
        raggi_candidati2(i) = distanza_al_quadrato(coord_bar_2, point);
    }

    // Scelgo il massimo tra i raggi trovati
    double raggio1 = *max_element(raggi_candidati1.begin(), raggi_candidati1.end());
    double raggio2 = *max_element(raggi_candidati2.begin(), raggi_candidati2.end());
    double tol = 1e-10;
    if (distanza_al_quadrato(coord_bar_1, coord_bar_2) <= (((raggio1 + raggio2) * (raggio1 + raggio2)) - tol))
        return true; // le fratture potrebbero intersecarsi
    else
        return false; // le fratture sicuramente non si intersecano
}

array<double, 6> Retta_tra_piani(Fractures& frattura, unsigned int& id1, unsigned int& id2)
{
    // Data l'equazione della retta:
    // x = coord_retta[0] * t + coord_retta[3]
    // y = coord_retta[1] * t + coord_retta[4]
    // z = coord_retta[2] * t + coord_retta[5]
    array<double, 6> coord_retta = {};
    // Calcolo la direzione della retta intersecante i piani mediante prodotto vettoriale
    coord_retta[0] = frattura.Plane[id1][1] * frattura.Plane[id2][2] - frattura.Plane[id1][2] * frattura.Plane[id2][1];
    coord_retta[1] = frattura.Plane[id1][2] * frattura.Plane[id2][0] - frattura.Plane[id1][0] * frattura.Plane[id2][2];
    coord_retta[2] = frattura.Plane[id1][0] * frattura.Plane[id2][1] - frattura.Plane[id1][1] * frattura.Plane[id2][0];
    // Calcolo un punto che appartiene alla retta intersecante
    Matrix<double, 3, 3> A = {};
    A << frattura.Plane[id1][0], frattura.Plane[id1][1], frattura.Plane[id1][2],
         frattura.Plane[id2][0], frattura.Plane[id2][1], frattura.Plane[id2][2],
         coord_retta[0], coord_retta[1], coord_retta[2];
    Vector3d b = {- frattura.Plane[id1][3], - frattura.Plane[id2][3], 0};
    Vector3d x = A.lu().solve(b);
    coord_retta[3] = x[0];
    coord_retta[4] = x[1];
    coord_retta[5] = x[2];
    return coord_retta;
}

array<double, 6> Retta_per_due_vertici_della_frattura(Fractures& frattura, unsigned int& id, unsigned int& i,unsigned int& j)
{
    // Data l'equazione parametrica X = a t + P sdella retta
    // t: (x2 - x1, y2 - y1, z2 - z1)
    // P: (x1, y1, z1)
    // Salviamo i relativi valori in un array
    array<double, 6> coord_retta_vertici = {};
    coord_retta_vertici[0] = frattura.Vertices[id](0, j) - frattura.Vertices[id](0, i);
    coord_retta_vertici[1] = frattura.Vertices[id](1, j) - frattura.Vertices[id](1, i);
    coord_retta_vertici[2] = frattura.Vertices[id](2, j) - frattura.Vertices[id](2, i);
    coord_retta_vertici[3] = frattura.Vertices[id](0, i);
    coord_retta_vertici[4] = frattura.Vertices[id](1, i);
    coord_retta_vertici[5] = frattura.Vertices[id](2, i);
    return coord_retta_vertici;
}

Vector2d alpha_di_intersezione(array<double, 6> r_intersez, array<double, 6> r_fratt)
{
    // Imposto un sistema lineare per la ricerca dei parametri alpha e beta
    // Imposto i coefficienti della matrice
    MatrixXd A = MatrixXd::Zero(3, 2);
    // Retta generata da un lato della frattura
    Vector3d t1 = {};
    t1[0] = r_fratt[0];
    t1[1] = r_fratt[1];
    t1[2] = r_fratt[2];
    A.col(0) = t1;
    // Retta di intersezione tra i piani
    Vector3d t2 = {};
    t2[0] = r_intersez[0];
    t2[1] = r_intersez[1];
    t2[2] = r_intersez[2];
    A.col(1) = -t2;
    // Imposto i coefficienti del termine noto
    Vector3d b = Vector3d::Zero();
    b[0] = r_intersez[3] - r_fratt[3];
    b[1] = r_intersez[4] - r_fratt[4];
    b[2] = r_intersez[5] - r_fratt[5];
    Vector2d x = A.householderQr().solve(b); // x = [alpha; beta]
    // Dove beta è l'ascissa curvilinea della retta generata dall'intersezione tra piani e
    // alpha è l'ascissa curvilinea della retta passante per due punti
    // Tale parametro deve essere controllato tra zero e uno per il segmento
    return x;
}

void caricamento_dati(Traces& traccia, Fractures& frattura)
{
    double tol = 1e-10;
    unsigned int NumberTrac = 0;
    array<unsigned int, 2> Id = {};
    array<Vector3d, 2> Vertici = {};
    array<bool, 2> Tipo = {};
    traccia.Vertices.reserve(10*frattura.NumberFractures);
    traccia.FracturesId.reserve(10*frattura.NumberFractures);
    traccia.Tips.reserve(10*frattura.NumberFractures);
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        unsigned int j = i + 1; // Valuta la frattura successiva
        while(j < frattura.NumberFractures)
        {
            // Verifichiamo che i poligoni abbiano distanza minore della somma dei due raggi delle palle
            if(valuta_intersezione(frattura, i, j))
            {
                array<double, 4> coeff = {};
                // Calcolo la retta di intersezione tra i piani dei poligoni i e j
                array<double, 6> r_piano = Retta_tra_piani(frattura, i, j);
                // Sulla carta sappiamo che se il prodotto vettoriale delle due normali ai piani è zero allora sono paralleli. Le coordinate del risultato sono memorizzate nei primi tre spazi dell'array r_piano
                if(abs(r_piano[0]) < tol && abs(r_piano[1]) < tol && abs(r_piano[2]) < tol)
                {
                    cout << "I piani sono paralleli, nessuna intersezione possibile" << endl;
                }
                else
                {
                    // Piani non paralleli
                    // Calcolo la retta tra i vertici adiacenti del poligono 1
                    unsigned int h = 0; // Usato per accedere a tutti i vertici della frattura
                    unsigned int k = 1; // Usato per accedere a tutti i vertici della stessa frattura a partire dal secondo
                    unsigned int cont = 0; // Usato per inserire l'ascissa curvilinea in coeff, poi per trovare l'intervallo di intersezione
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
                            // CONDIZIONI : verifichiamo che appartenga al segmento
                            if(x[0] >= -tol && x[0] <= 1 + tol)
                            {
                                coeff[cont] = x[1]; // Memorizziamo in coeff l'ascissa curvilinea della retta tra i piani : beta
                                cont++;
                            }
                        }
                        h++;
                        k++;
                    }
                    // Calcolo la retta tra i lati adiacenti del poligono 2
                    h = 0;
                    k = 1;
                    while(h < frattura.Vertices[j].cols())
                    {
                        // Con l'if gestisco il caso dell'ultimo vertice con il primo del poligono
                        if(k == frattura.Vertices[j].cols())
                        {
                            k = 0;
                        }
                        array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, j, h, k);
                        // Escludo il parallelismo: calcolo il prodotto vettoriale
                        double parallelo = (r_piano[1] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[1]) - ((r_piano[0] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[0])) + (r_piano[0] * r_tra_punti[1]) - (r_piano[1] * r_tra_punti[0]);
                        if (abs(parallelo) > tol)
                        {
                            Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                            if(x[0] >= -tol && x[0] <= 1 + tol)
                            {
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
                        double sx = max(min_el_sx, min_el_dx); // Estremo sinistro dell'intersezione
                        double dx = min(max_el_sx, max_el_dx); // Estremo destro dell'intersezione
                        // Gli intervalli si sovrappongono se:
                        if (sx < dx)
                        {
                            NumberTrac++;
                            Id[0] = i;
                            Id[1] = j;
                            // Rianalizziamo ora il poligono 1
                            unsigned int conta_p1 = 0;
                            unsigned int h = 0;
                            unsigned int k = 1;
                            while(h < frattura.Vertices[i].cols())
                            {
                                // Con l'if gestisco il caso dell'ultimo vertice con il primo del poligono
                                if(k == frattura.Vertices[i].cols())
                                {
                                    k = 0;
                                }
                                array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, i, h, k);
                                // Escludo il parallelismo: calcolo prodotto vettoriale
                                double parallelo = (r_piano[1] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[1]) - ((r_piano[0] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[0])) + (r_piano[0] * r_tra_punti[1]) - (r_piano[1] * r_tra_punti[0]);
                                if (abs(parallelo) > tol)
                                {
                                    Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                                    // CONDIZIONI : verifichiamo che appartenga al segmento (=lato poligono 1) e al poligono 2
                                    if(x[0] >= -tol && x[0] <= 1 + tol)
                                    {
                                        if(x[1] > sx - tol && x[1] < dx + tol)
                                        {
                                            // Calcolo il punto di intersezione
                                            Vector3d punto_intersezione = {};
                                            punto_intersezione[0] = r_tra_punti[0] * x[0] + r_tra_punti[3];
                                            punto_intersezione[1] = r_tra_punti[1] * x[0] + r_tra_punti[4];
                                            punto_intersezione[2] = r_tra_punti[2] * x[0] + r_tra_punti[5];
                                            Vertici[conta_p1] = punto_intersezione;
                                            conta_p1++;
                                        }
                                    }
                                }
                                h++;
                                k++;
                            }
                            //------------------------------------------------------------------------------------------
                            // ANALIZZIAMO LE VARIE CASISTICHE PER IL POLIGONO 2
                            // CASO 1: Traccia passante per il primo (passante o non passante per il secondo)
                            if(conta_p1 == 2)
                            {
                                Tipo[0] = 0; // Traccia passante per il poligono 1
                                // Rianalizziamo il poligono 2
                                unsigned int conta_p2 = 0;
                                h = 0;
                                k = 1;
                                while(h < frattura.Vertices[j].cols())
                                {
                                    // Con l'if gestisco il caso dell'ultimo vertice con il primo del poligono
                                    if(k == frattura.Vertices[j].cols())
                                    {
                                        k = 0;
                                    }
                                    array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, j, h, k);
                                    // Escludo il parallelismo: calcolo il prodotto vettoriale
                                    double parallelo = (r_piano[1] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[1]) - ((r_piano[0] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[0])) + (r_piano[0] * r_tra_punti[1]) - (r_piano[1] * r_tra_punti[0]);
                                    if (abs(parallelo) > tol)
                                    {
                                        Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                                        // CONDIZIONI: verifichiamo che appartenga al segmento e all'altro poligono
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
                                if(conta_p2 == 2)
                                {
                                    Tipo[1] = 0; // Traccia passante per il secondo poligono considerato
                                }
                                else
                                {
                                    Tipo[1] = 1; // Traccia non passante per il secondo poligono considerato
                                }
                            }
                            // CASO 2 : non passante per il primo e passante per il secondo
                            if(conta_p1 == 0)
                            {
                                Tipo[0] = 1; // Traccia non passante per il poligono 1
                                // Rianalizziamo il poligono 2
                                unsigned int conta_p2 = 0;
                                h = 0;
                                k = 1;
                                while(h < frattura.Vertices[j].cols())
                                {
                                    // Con l'if gestisco il caso dell'ultimo vertice con il primo del poligono
                                    if(k == frattura.Vertices[j].cols())
                                    {
                                        k = 0;
                                    }
                                    array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, j, h, k);
                                    // Escludo il parallelismo: calcolo il prodotto vettoriale
                                    double parallelo = (r_piano[1] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[1]) - ((r_piano[0] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[0])) + (r_piano[0] * r_tra_punti[1]) - (r_piano[1] * r_tra_punti[0]);
                                    if (abs(parallelo) > tol)
                                    {
                                        Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                                        // CONDIZIONI: verifichiamo che appartenga al segmento e all'altro poligono
                                        if(x[0] >= -tol && x[0] <= 1 + tol)
                                        {
                                            if(x[1] > sx - tol && x[1] < dx + tol)
                                            {
                                                // Calcolo il punto di intersezione
                                                Vector3d punto_intersezione = {};
                                                punto_intersezione[0] = r_tra_punti[0] * x[0] + r_tra_punti[3];
                                                punto_intersezione[1] = r_tra_punti[1] * x[0] + r_tra_punti[4];
                                                punto_intersezione[2] = r_tra_punti[2] * x[0] + r_tra_punti[5];
                                                Vertici[conta_p2] = punto_intersezione;
                                                conta_p2++;
                                            }
                                        }
                                    }
                                    h++;
                                    k++;
                                }
                                Tipo[1] = 0; // Traccia passante per il secondo poligono considerato
                            }
                            // CASO 3: non passante per il primo con 1 punto e non passante per il secondo con 1 punto
                            if(conta_p1 == 1)
                            {
                                Tipo[0] = 1; // Traccia non passante per il primo poligono considerato
                                // Rianalizzo poligono 2
                                h = 0;
                                k = 1;
                                while(h < frattura.Vertices[j].cols())
                                {
                                    // Con l'if gestisco il caso dell'ultimo vertice con il primo del poligono
                                    if(k == frattura.Vertices[j].cols())
                                    {
                                        k = 0;
                                    }
                                    array<double, 6> r_tra_punti = Retta_per_due_vertici_della_frattura(frattura, j, h, k);
                                    // Escludo il parallelismo: calcolo il prodotto vettoriale
                                    double parallelo = (r_piano[1] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[1]) - ((r_piano[0] * r_tra_punti[2]) - (r_piano[2] * r_tra_punti[0])) + (r_piano[0] * r_tra_punti[1]) - (r_piano[1] * r_tra_punti[0]);
                                    if (abs(parallelo) > tol)
                                    {
                                        Vector2d x = alpha_di_intersezione(r_piano, r_tra_punti);
                                        // CONDIZIONI: verifichiamo che appartenga al segmento e all'altro poligono
                                        if(x[0] >= -tol && x[0] <= 1 + tol)
                                        {
                                            if(x[1] > sx - tol && x[1] < dx + tol)
                                            {
                                                // Calcolo il punto di intersezione
                                                Vector3d punto_intersezione = {};
                                                punto_intersezione[0] = r_tra_punti[0] * x[0] + r_tra_punti[3];
                                                punto_intersezione[1] = r_tra_punti[1] * x[0] + r_tra_punti[4];
                                                punto_intersezione[2] = r_tra_punti[2] * x[0] + r_tra_punti[5];
                                                Vertici[1] = punto_intersezione;
                                            }
                                        }
                                    }
                                    h++;
                                    k++;
                                }
                                Tipo[1] = 1; // Traccia non passante per il secondo poligono considerato
                            }
                            traccia.Vertices.push_back(Vertici);
                            traccia.FracturesId.push_back(Id);
                            traccia.Tips.push_back(Tipo);
                        }
                    }
                    else
                    {
                        cout << "Numero insufficiente di rette che generano intersezione" << endl;
                    }
                } // chiusura else piani non paralleli
            }
            j++;
        }
    }
    traccia.NumberTraces = NumberTrac;
}

bool compare(array<double, 2> a, array<double, 2> b)
{
    return (a[1] > b[1]);
}

void esportazione(Traces& traccia, Fractures& frattura)
{
    string fileoutput = "Traces.txt";
    ofstream ofs(fileoutput);
    if (ofs.fail())
    {
        cout << "Impossibile creare il file di output" << endl;
        return;
    }
    ofs << "# Number of Traces" << endl;
    // Considero il numero di tracce presenti nell'intero sistema
    ofs << traccia.NumberTraces<< endl;
    ofs << "# TraceId; FracturesId1; FracturesId2; X1; Y1; Z1; X2; Y2; Z2" << endl;
    for(unsigned int i = 0; i < traccia.FracturesId.size(); i++)
    {
        ofs << i << "; " << traccia.FracturesId[i][0] << "; " << traccia.FracturesId[i][1] << "; "
            << setprecision(16) << scientific << traccia.Vertices[i][0][0] << "; " << traccia.Vertices[i][0][1] << "; " << traccia.Vertices[i][0][2]
            << "; " << traccia.Vertices[i][1][0] << "; " << traccia.Vertices[i][1][1] << "; " << traccia.Vertices[i][1][2] << endl;
    }
    ofs << endl;

    // Organizzo un vettore che memorizza il numero complessivo di tracce per ogni poligono
    vector<unsigned int> frattura_traccia = {};
    frattura_traccia.reserve(traccia.NumberTraces);
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        unsigned int conta_tracce_per_fratt = 0;
        for(unsigned int j = 0; j < traccia.NumberTraces; j++)
        {
            // se il primo o il secondo id è i allora incremento di 1 il numero delle tracce
            if(i == traccia.FracturesId[j][0] || i == traccia.FracturesId[j][1])
            {
                conta_tracce_per_fratt = conta_tracce_per_fratt + 1;
            }
        }
        frattura_traccia.push_back(conta_tracce_per_fratt);
    }
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        if(frattura_traccia[i] != 0)
        {
            ofs << endl;
            ofs << "# FractureId; NumTraces" << endl;
            ofs << i << "; " << frattura_traccia[i] << endl;
            ofs << "# TraceId; Tips; Length" << endl;
            unsigned int contatore = 0;
            while(contatore < 2)
            {
                unsigned int conta_per_tipo = 0;
                vector<array<double, 2>> ordinamento = {};
                ordinamento.reserve(frattura_traccia[i]);
                array<double, 2> ord = {};
                for(unsigned int j = 0; j < traccia.NumberTraces; j++)
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
                // Condizioni per ordinamento vettore di array sfruttando sort
                sort(ordinamento.begin(), ordinamento.end(), compare);
                // Stampiamo dall'inizio alla fine
                for(unsigned int k = 0; k < conta_per_tipo; k++)
                {
                    ofs << int(ordinamento[k][0]) << "; " << contatore << "; " << ordinamento[k][1] << endl;
                }
                contatore++;
            }
            ofs << endl;
        }
    }
}

array<double, 6> coord_retta_tra2punti(Vector3d& V1, Vector3d& V2)
{
    array<double, 6> coord_retta = {};
    coord_retta[0] = V2[0] - V1[0];
    coord_retta[1] = V2[1] - V1[1];
    coord_retta[2] = V2[2] - V1[2];
    coord_retta[3] = V1[0];
    coord_retta[4] = V1[1];
    coord_retta[5] = V1[2];

    return coord_retta;
}

Vector3d intersezione_rette(Vector3d& V1, Vector3d& V2, Vector3d& V3, Vector3d& V4)
{
    Vector3d punto_int = {};
    array<double, 6> r1 = coord_retta_tra2punti(V1, V2);
    array<double, 6> r2 = coord_retta_tra2punti(V3, V4);
    Vector2d axc = alpha_di_intersezione(r1, r2);
    punto_int[0] = r2[0] * axc[0] + r2[3];
    punto_int[1] = r2[1] * axc[0] + r2[4];
    punto_int[2] = r2[2] * axc[0] + r2[5];

    return punto_int;
}

}

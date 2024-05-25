#include <Polygons.hpp>
#include <Utils.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace Eigen;

namespace FracturesLib{

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
        // Creo la tabella che contiene le coordinate dei punti
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
        // Aggiungo quanto trovato all'interno della mappa
        frattura.Vertices.insert(make_pair(frattura.Id, Tab_coord_vertici));
    }
    // Calcolo i coefficienti del piano per ciascuna frattura
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        Vector3d point1 = frattura.Vertices[i].col(1);
        Vector3d point2 = frattura.Vertices[i].col(2);
        Vector3d point3 = frattura.Vertices[i].col(3);
        // Calcolo indiretto dei coefficienti mediante il determinante uguale a zero e relativo inserimento nella mappa
        //(x-x1  x-y1  x-z1
        // x2-x1 y2-y1 z2-z1
        // x3-x1 y3-y1 z3-z1)
        frattura.Piano[i][0] = (point2[1]-point1[1])*(point3[2]-point1[2]) - (point2[2]-point1[2])*(point3[1]-point1[1]);
        frattura.Piano[i][1] = -((point2[0]-point1[0])*(point3[2]-point1[2]) - (point3[0]-point1[0])*(point2[2]-point1[2]));
        frattura.Piano[i][2] = (point2[0]-point1[0])*(point3[1]-point1[1]) - (point3[0]-point1[0])*(point2[1]-point1[1]);
        frattura.Piano[i][3] = -frattura.Piano[i][0]*point1[0]-frattura.Piano[i][1]*point1[1]-frattura.Piano[i][2]*point1[2];
    }
    file.close();
    return true;
}


double distanza_al_quadrato(Vector3d& v1, Vector3d& v2)
{
    return (v1[0]-v2[0])*(v1[0]-v2[0]) + (v1[1]-v2[1])*(v1[1]-v2[1]) + (v1[2]-v2[2])*(v1[2]-v2[2]);
}

Vector3d baricentro (Fractures& frattura, unsigned int& Id)
{
    Vector3d coord_bar = {};
    unsigned int n = frattura.Vertices[Id].cols(); // numero di vertici della frattura
    for(unsigned int h = 0; h < 3; h++)
    {
        for (unsigned int k = 0; k < n; k++)
        {
            coord_bar[h] = coord_bar[h] + frattura.Vertices[Id](h,k);
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

    unsigned int n1 = frattura.Vertices[Id1].cols(); // numero di vertici della prima frattura
    unsigned int n2 = frattura.Vertices[Id2].cols(); // numero di vertici della seconda frattura

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
    // Calcolo il raggio delle due palle avente centro nei baricentri precedentemente calcolati
    double raggio1 = *max_element(raggi_candidati1.begin(), raggi_candidati1.end());
    double raggio2 = *max_element(raggi_candidati2.begin(), raggi_candidati2.end());
    double tol = 1e-10;
    if (distanza_al_quadrato(coord_bar_1,coord_bar_2) <= (((raggio1+raggio2)*(raggio1+raggio2)) - tol))
        return true; // le fratture potrebbero intersecarsi
    else
        return false; // le fratture sicuramente non si intersecano
}

array<double,6> Retta_tra_piani(Fractures& frattura, unsigned int& id1, unsigned int& id2)
{
    // Data l'equazione della retta:
    // x = coord_retta[0] * t + coord_retta[3]
    // y = coord_retta[1] * t + coord_retta[4]
    // z = coord_retta[2] * t + coord_retta[5]
    array<double, 6> coord_retta = {};
    // Calcolo la direzione della retta intersecante mediante prodotto vettoriale
    coord_retta[0] = frattura.Piano[id1][1] * frattura.Piano[id2][2] - frattura.Piano[id1][2] * frattura.Piano[id2][1];
    coord_retta[1] = frattura.Piano[id1][2] * frattura.Piano[id2][0] - frattura.Piano[id1][0] * frattura.Piano[id2][2];
    coord_retta[2] = frattura.Piano[id1][0] * frattura.Piano[id2][1] - frattura.Piano[id1][1] * frattura.Piano[id2][0];
    //Calcolo un punto che appartiene alla retta intersecante
    Matrix<double, 3, 3> A = {};
    A << frattura.Piano[id1][0], frattura.Piano[id1][1], frattura.Piano[id1][2],
        frattura.Piano[id2][0], frattura.Piano[id2][1], frattura.Piano[id2][2],
        coord_retta[0], coord_retta[1], coord_retta[2];
    Vector3d b = {-frattura.Piano[id1][3], -frattura.Piano[id2][3], 0};
    Vector3d x = A.lu().solve(b);
    coord_retta[3] = x[0];
    coord_retta[4] = x[1];
    coord_retta[5] = x[2];
    return coord_retta;
}

array<double,6> Retta_per_due_vertici_della_frattura(Fractures& frattura, unsigned int& id, unsigned int& i,unsigned int& j)
{
    //data l'equazione parametrica è X = at+P trovo direttrice e punto di partenza della retta
    // t:(x2-x1,y2-y1,z2-z1)
    // P:(x1,y1,z1)
    // salviamo i relativi valori in un array
    array<double, 6> coord_retta_vertici = {};
    coord_retta_vertici[0] = frattura.Vertices[id](0,j) - frattura.Vertices[id](0,i);
    coord_retta_vertici[1] = frattura.Vertices[id](1,j) - frattura.Vertices[id](1,i);
    coord_retta_vertici[2] = frattura.Vertices[id](2,j) - frattura.Vertices[id](2,i);
    coord_retta_vertici[3] = frattura.Vertices[id](0,i);
    coord_retta_vertici[4] = frattura.Vertices[id](1,i);
    coord_retta_vertici[5] = frattura.Vertices[id](2,i);
    return coord_retta_vertici;
}

Vector2d alpha_di_intersezione(array<double, 6> r_intersez, array<double, 6> r_fratt)
{
    // imposto un sistema lineare per la ricerca dei parametri alpha e beta
    // imposto i coefficienti della matrice
    MatrixXd A = MatrixXd::Zero(3,2);
    // retta di intersezione tra i piani
    Vector3d t1 = {};
    t1[0] = r_fratt[0];
    t1[1] = r_fratt[1];
    t1[2] = r_fratt[2];
    A.col(0) = t1;
    // retta di intersezione tra i vertici del poligono della stessa frattura
    Vector3d t2 = {};
    t2[0] = r_intersez[0];
    t2[1] = r_intersez[1];
    t2[2] = r_intersez[2];
    A.col(1) = -t2;
    // imposto i coefficienti del termine noto
    Vector3d b = Vector3d::Zero();
    b[0] = r_intersez[3] - r_fratt[3];
    b[1] = r_intersez[4] - r_fratt[4];
    b[2] = r_intersez[5] - r_fratt[5];
    Vector2d x = A.householderQr().solve(b); // x =[alpha;beta]
    // dove beta è il valore del parametro nell'equazione del piano e
    // alpha è il valore del parametro nell'equazione della retta passante per due punti
    // tale parametro deve essere controllato tra zero e uno per il segmento
    return x;
}

void caricamento_dati(Traces& traccia, Fractures& frattura)
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
                        double sx = max(min_el_sx, min_el_dx); // Estremo sinistro dell'intersezione
                        double dx = min(max_el_sx, max_el_dx); // Estremo destro dell'intersezione
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
                                cout << "Traccia passante" << endl;
                                Tipo[0] = 0;
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
                                if(conta_p2 == 2)
                                {
                                    cout << "Traccia passante" << endl;
                                    Tipo[1] = 0;
                                }
                                else
                                {
                                    cout << "Traccia non passante" << endl;
                                    Tipo[1] = 1;
                                }
                            }


                            if(conta_p1 == 0)
                            {
                                cout << "Traccia non passante" << endl;
                                Tipo[0] = 1;
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
                                if(conta_p2 == 2)
                                {
                                    cout << "Traccia passante" << endl;
                                    Tipo[1] = 0;
                                }
                                else
                                {
                                    cout << "Traccia non passante" << endl;
                                    Tipo[1] = 1;
                                }
                            }


                            if(conta_p1 == 1)
                            {
                                cout << "Traccia non passante" << endl;
                                Tipo[0] = 1;
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
                                cout << "Traccia non passante" << endl;
                                Tipo[1] = 1;
                            }
                        }
                    }
                } // chiusura if
                traccia.Vertices.insert(make_pair(NumberTraces-1, Vertici));
                traccia.FracturesId.insert(make_pair(NumberTraces-1, Id));
                traccia.Tips.insert(make_pair(NumberTraces-1, Tipo));
            }
            j++;
        }
    }
    traccia.Number = NumberTraces;
    cout << NumberTraces;

    for (const auto& [id, coord] : traccia.Vertices){
        Vector3d v1 = coord[0];
        Vector3d v2 = coord[1];
        traccia.Lenght[id] = sqrt(distanza_al_quadrato(v1, v2));
    }


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
    // considero il numero di mappature
    ofs << traccia.FracturesId.size() << endl;
    ofs << "# TraceId; FracturesId1; FracturesId2; X1; Y1; Z1; X2; Y2; Z2" << endl;
    for(unsigned int i = 0; i < traccia.FracturesId.size(); i++)
    {
        ofs << i << ";" << traccia.FracturesId[i][0] << ";" << traccia.FracturesId[i][1] << ";"
            << setprecision(16) << scientific << traccia.Vertices[i][0][0] << ";" << traccia.Vertices[i][0][1] << ";" << traccia.Vertices[i][0][2]
            << ";" << traccia.Vertices[i][1][0] << ";" << traccia.Vertices[i][1][1] << ";" << traccia.Vertices[i][1][2] << endl;
    }
    ofs << endl;

    // Organizzo una mappa che associa l'Id della frattura al numero complessivo di tracce
    map<unsigned int, unsigned int> frattura_traccia = {};
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        frattura_traccia[i] = 0;
        for(unsigned int j = 0; j < traccia.FracturesId.size(); j++)
        {
            // se il primo o il secondo id è i allora incremento di 1 il numero delle tracce
            if(i == traccia.FracturesId[j][0] || i == traccia.FracturesId[j][1])
            {
                frattura_traccia[i] = frattura_traccia[i] + 1;
            }
        }
    }
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        if(frattura_traccia[i] != 0)
        {
            ofs << endl;
            ofs << "# FractureId; NumTraces" << endl;
            ofs << i << ";" << frattura_traccia[i] << endl;
            ofs << "# TraceId; Tips; Length" << endl;
            for(unsigned int j = 0; j < traccia.FracturesId.size(); j++)
            {
                // se il primo o il secondo id è i stampo le relative informazioni
                if(i == traccia.FracturesId[j][0])
                {
                    ofs << j << ";" << traccia.Tips[j][0] << ";"
                        << sqrt(distanza_al_quadrato(traccia.Vertices[j][0], traccia.Vertices[j][1])) << endl;
                }
                if(i == traccia.FracturesId[j][1])
                {
                    ofs << j << ";" << traccia.Tips[j][1] << ";"
                        << sqrt(distanza_al_quadrato(traccia.Vertices[j][0], traccia.Vertices[j][1])) << endl;
                }
            }
        }
    }
    // CAPIRE COME ORDINARE IN MODO DECRESCENTE RAGGRUPPANDO PER TIPS
}

vector<tuple<unsigned int, array<Vector3d, 2>, array<bool, 2>>> OrdinamentoTracce (Traces& traccia)
{
    // es elt del vettore: <id, {(x1, y1, z1), (x2, y2, z2)}, {true/false, true/false}>
    vector<tuple<unsigned int, array<Vector3d, 2>, array<bool, 2>>> tracceOrdinate = {};
    // vettore dove mettiamo gli id delle tracce che sono passanti per entrambi i poligoni
    vector<unsigned int> both = {};
    // vettore dove mettiamo gli id delle tracce che sono passanti per un solo poligon
    vector<unsigned int> one = {};
    // vettore dove mettiamo gli id delle tracce che sono non passanti per entrambi i poligoni
    vector<unsigned int> none = {};

    //mettiamo in ordine gli id delle tracce nel vettore ordinamentoTips
    //prima mettiamo le tracce passanti per entrambi i poligoni, poi quelle passanti per un solo poligono e poi quelle non passanti per entrambi i poligoni
    for (const auto& [id, tips] : traccia.Tips)
    {
        if (tips[0] == false && tips[1] == false)
        {
            both.push_back(id);
        }
        if((tips[0] == true && tips[1] == false) || (tips[0] == false && tips[1] == true))
        {
            one.push_back(id);
        }
        else
        {
            both.push_back(id);
        }
    }

    //vettore dove mettiamo la lunghezza della traccia a cui associamo l'id
    vector<pair<double, unsigned int>> vec;

    for (const auto& it : traccia.Lenght)
    {
        vec.push_back(make_pair(it.second, it.first));
    }

    //ordina il vettore in base alla lunghezza delle tracce
    if (vec.size() < 10)
    {
        SortLibrary::BubbleSort(vec);
    }
    else
    {
        SortLibrary::MergeSort(vec);
    }

    // scorriamo il vettore delle lunghezze ordinato e quando troviamo un id di una traccia passante per entrambi i poligoni lo mettiamo nel vettore tracce ordinate
    for (const auto& it2 : vec)
    {
        for (const auto& it1 : both)
        {
            if (it2. second == it1)
            {
                tracceOrdinate.push_back(make_tuple(it1, traccia.Vertices[it1], traccia.Tips[it1]));
            }
        }
    }

    for (const auto& it2 : vec)
    {
        for (const auto& it1 : one)
        {
            if (it2. second == it1)
            {
                tracceOrdinate.push_back(make_tuple(it1, traccia.Vertices[it1], traccia.Tips[it1]));
            }
        }
    }

    for (const auto& it2 : vec)
    {
        for (const auto& it1 : none)
        {
            if (it2. second == it1)
            {
                tracceOrdinate.push_back(make_tuple(it1, traccia.Vertices[it1], traccia.Tips[it1]));
            }
        }
    }

    return tracceOrdinate;

}
}



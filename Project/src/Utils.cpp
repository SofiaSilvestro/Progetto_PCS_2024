#include <Polygons.hpp>
#include <Utils.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <vector>
#include <cmath>
#include<iomanip>
#include <algorithm>
using namespace std;
using namespace Eigen;

namespace FracturesLib{

bool importazione(const string& filename, Fractures& frattura) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    string header;
    getline(file, header); // Leggo la prima riga da trascurare
    string line;
    getline(file,line); // Leggo la riga che contiene il numero di fratture
    frattura.NumberFractures = stoi(line);
    char separatore;
    int numVertices;
    for(unsigned int i=0; i<frattura.NumberFractures;i++){
        getline(file, header); // Leggo la riga da trascurare
        getline(file, line);
        stringstream ss(line);
        ss >> frattura.Id >> separatore >> numVertices;
        // Creo la tabella che contiene le coordinate dei punti
        Matrix<double, 3, Dynamic> Tab_coord_vertici(3, numVertices);
        getline(file, header); // Leggo la riga da trascurare
        string val;
        // Lettura delle coordinate dei vertici
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < numVertices; j++){
                file >> val;
                Tab_coord_vertici(i, j) = stod(val);
            }
        }
        file >> separatore;
        // Aggiungo quanto trovato all'interno della mappa
        frattura.Vertices.insert(make_pair(frattura.Id, Tab_coord_vertici));
    }

    /*//Righe di stampa da commentare
    cout << frattura.NumberFractures << endl;
    for (const auto& coppia : frattura.Vertices) {
        cout << "Chiave: " << coppia.first <<endl<<"Valore: " <<endl<<setprecision(16)<<scientific<< coppia.second <<endl<<endl;
    }*/
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
        frattura.Piano[i][1] = -((point2[0]-point1[0])*(point3[2]-point1[2])-(point3[0]-point1[0])*(point2[2]-point1[2]));
        frattura.Piano[i][2] = (point2[0]-point1[0])*(point3[1]-point1[1])-(point3[0]-point1[0])*(point2[1]-point1[1]);
        frattura.Piano[i][3] = -frattura.Piano[i][0]*point1[0]-frattura.Piano[i][1]*point1[1]-frattura.Piano[i][2]*point1[2];
    }
    /*//Da commentare :stampiamo i coefficienti del piano
    cout<<"ID, coeff1,coeff2,coeff3,coeff4"<<endl;
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        cout<<i<<" "<<frattura.Piano[i][0]<<" "<<frattura.Piano[i][1]<<" "<<frattura.Piano[i][2]<<" "<<frattura.Piano[i][3]<<endl;
    }*/
    file.close();
    return true;
}


double distanza_al_quadrato(Vector3d& v1, Vector3d& v2){
    return (v1[0]-v2[0])*(v1[0]-v2[0]) + (v1[1]-v2[1])*(v1[1]-v2[1]) +(v1[2]-v2[2])*(v1[2]-v2[2]);
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
    //considero il numero di mappature
    ofs << traccia.FracturesId.size() << endl;
    ofs << "# TraceId; FracturesId1; FracturesId2; X1; Y1; Z1; X2; Y2; Z2" << endl;
    for(unsigned int i = 0; i < traccia.FracturesId.size();i++)
    {
        ofs << i << ";" << traccia.FracturesId[i][0] << ";" << traccia.FracturesId[i][1] << ";"
            << traccia.Vertices[i][0][0]<< ";" << traccia.Vertices[i][0][1] << ";" << traccia.Vertices[i][0][2]
            << traccia.Vertices[i][1][0] << ";" << traccia.Vertices[i][1][1] << ";" << traccia.Vertices[i][1][2] << endl;
    }

    // Organizzo una mappa che associa l'Id della frattura al numero complessivo di tracce
    map<unsigned int, unsigned int> frattura_traccia;
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        frattura_traccia[i]=0;
        for(unsigned int j = 0; j < traccia.FracturesId.size(); j++)
        {
            // se il primo o il secondo id è i allora incremento di 1 il numero delle tracce
            if(i == traccia.FracturesId[j][0] || i == traccia.FracturesId[j][1])
            {
                frattura_traccia[i]=frattura_traccia[i]+ 1;
            }
        }
    }
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        if(frattura_traccia[i]!=0)
        {
            ofs << "# FractureId; NumTraces" << endl;
            ofs << i << ";" << frattura_traccia[i] << endl;
            ofs << "# TraceId; Tips; Length" << endl;
            for(unsigned int j = 0; j < traccia.FracturesId.size(); j++)
            {
                // se il primo o il secondo id è i stampo le relative informazioni
                if(i == traccia.FracturesId[j][0])
                {
                    ofs << i << ";" << traccia.Tips[i][0] << sqrt(distanza_al_quadrato(traccia.Vertices[i][0],traccia.Vertices[i][1])) << endl<<endl;
                }
                if(i == traccia.FracturesId[j][1])
                {
                    ofs << i << ";" << traccia.Tips[i][1] << sqrt(distanza_al_quadrato(traccia.Vertices[i][0],traccia.Vertices[i][1])) << endl<<endl;
                }
            }
        }
    }
    // CAPIRE COME ORDINARE IN MODO DECRESCENTE RAGGRUPPANDO PER TIPS
}


bool valuta_intersezione (Fractures& frattura, unsigned int& Id1, unsigned int& Id2){
    // Definisco due vettori che contengono le coordinate del mio baricentro
    Vector3d coord_bar_1;
    unsigned int n1 = frattura.Vertices[Id1].cols(); //numero di colonne della prima frattura
    Vector3d coord_bar_2;
    unsigned int n2 = frattura.Vertices[Id2].cols(); //numero di colonne della seconda frattura
    for(unsigned int i=0; i<3; i++){
        for (unsigned int j=0; j<n1; j++){
            coord_bar_1[i]=coord_bar_1[i]+frattura.Vertices[Id1](i,j);
        }
        coord_bar_1[i] =coord_bar_1[i]/n1;
        for (unsigned int j=0; j<n2; j++){
            coord_bar_2[i] =coord_bar_2[i]+frattura.Vertices[Id2](i,j);
        }
        coord_bar_2[i] = coord_bar_2[i]/n2;
    }
    // Calcolo i possibili raggi delle due palle avente centro nei baricentri precedentemente calcolati
    VectorXd raggi_candidati1;
    raggi_candidati1.resize(n1);
    for(unsigned int i=0; i<n1; i++){
        Vector3d point = frattura.Vertices[Id1].col(i);
        raggi_candidati1(i) = distanza_al_quadrato(coord_bar_1,point);
    }
    VectorXd raggi_candidati2;
    raggi_candidati2.resize(n2);
    for(unsigned int i=0; i<n2; i++){
        Vector3d point = frattura.Vertices[Id2].col(i);
        raggi_candidati2(i) = distanza_al_quadrato(coord_bar_2,point);
    }
    // Calcolo il raggio delle due palle avente centro nei baricentri precedentemente calcolati
    double raggio1 = *max_element(raggi_candidati1.begin(), raggi_candidati1.end());
    double raggio2 = *max_element(raggi_candidati2.begin(), raggi_candidati2.end());
    double tol=pow(10,-10);
    if (distanza_al_quadrato(coord_bar_1,coord_bar_2) <=(pow(raggio1+raggio2,2)-tol))
        return true; // le fratture potrebbero intersecarsi
    else
        return false; // le fratture sicuramente non si intersecano
}


array<double,6> Retta_tra_piani(Fractures& frattura, unsigned int& id1, unsigned int& id2)
{
    // Data l'equazione della retta:
    // x= coord_retta[0]*t+coord_retta[3]
    // y= coord_retta[1]*t+coord_retta[4]
    // z= coord_retta[2]*t+coord_retta[5]
    array<double,6> coord_retta;
    // Calcolo la direzione della retta intersecante mediante prodotto vettoriale
    coord_retta[0] = frattura.Piano[id1][1]*frattura.Piano[id2][2] - frattura.Piano[id1][2]*frattura.Piano[id2][1];
    coord_retta[1] = frattura.Piano[id1][2]*frattura.Piano[id2][0] - frattura.Piano[id1][0]*frattura.Piano[id2][2];
    coord_retta[2] = frattura.Piano[id1][0]*frattura.Piano[id2][1] - frattura.Piano[id1][1]*frattura.Piano[id2][0];
    //Calcolo un punto che appartiene alla retta intersecante
    Matrix<double,3,3> A;
    A << frattura.Piano[id1][0], frattura.Piano[id1][1],frattura.Piano[id1][2],
        frattura.Piano[id2][0],frattura.Piano[id2][1],frattura.Piano[id2][2],
        coord_retta[0],coord_retta[1],coord_retta[2];
    Vector3d b = {-frattura.Piano[id1][3],-frattura.Piano[id2][3],0};
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
    array<double,6> coord_retta_vertici;
    coord_retta_vertici[0]=frattura.Vertices[id](0,j)-frattura.Vertices[id](0,i);
    coord_retta_vertici[1]=frattura.Vertices[id](1,j)-frattura.Vertices[id](1,i);
    coord_retta_vertici[2]=frattura.Vertices[id](2,j)-frattura.Vertices[id](2,i);
    coord_retta_vertici[3]=frattura.Vertices[id](0,i);
    coord_retta_vertici[4]=frattura.Vertices[id](1,i);
    coord_retta_vertici[5]=frattura.Vertices[id](2,i);
    return coord_retta_vertici;
}

Vector2d alpha_di_intersezione(array<double,6> r_intersez,array<double,6> r_fratt)
{
    //imposto un sistema lineare per la ricerca dei parametri alpha e beta
    //imposto i coefficienti della matrice
    MatrixXd A = MatrixXd::Zero(3,2);
    //retta di intersezione tra i piani
    Vector3d t1 ;
    t1[0]=r_fratt[0];
    t1[1]=r_fratt[1];
    t1[2]=r_fratt[2];
    A.col(0)=t1;
    //retta di intersezione tra i vertici del poligono della stessa frattura
    Vector3d t2 ;
    t2[0]=r_intersez[0];
    t2[1]=r_intersez[1];
    t2[2]=r_intersez[2];
    A.col(1) =-t2;
    //imposto i coefficienti del termine noto
    Vector3d b = Vector3d::Zero();
    b[0] = r_intersez[3]-r_fratt[3];
    b[1] = r_intersez[4]-r_fratt[4];
    b[2] = r_intersez[5]-r_fratt[5];
    Vector2d x = A.householderQr().solve(b); //x =[alpha;beta]
    //dove beta è il valore del parametro nell'equazione del piano e
    //alpha è il valore del parametro nell'equazione della retta passante per due punti
    // tale parametro deve essere controllato tra zero e uno per il segmento
    return x;
}

Vector3d Punto_intersezione_rette_piano_frattura(array<double,6> r_intersez, array<double,6> r_fratt){
    MatrixXd A = MatrixXd::Zero(3,2);
    A << r_intersez[0], -r_fratt[0],
        r_intersez[1], -r_fratt[1],
        r_intersez[2], -r_fratt[2];

    Vector3d b = Vector3d::Zero();
    b << r_intersez[3] -r_fratt[3],
        r_intersez[4] -r_fratt[4],
        r_intersez[5] -r_fratt[5];

    Vector2d ts = Vector2d::Zero();

    ts = A.householderQr().solve(b);
    double t = ts[0];

    Vector3d punto_intersezione;
    punto_intersezione << r_intersez[0] * t + r_intersez[3],
        r_intersez[1] * t + r_intersez[4],
        r_intersez[2] * t + r_intersez[5];

    VectorXd r = b - A * ts;

    // Soglia di tolleranza per il residuo
    double tol = 1e-15;

    // Verifica se il residuo è sufficientemente piccolo
    if (r.norm() >= tol) {
        return Vector3d(NAN, NAN, NAN);
    }

    return punto_intersezione;

}

bool Controllo_puntoIntersezione_segmentoFrattura (Fractures& frattura, unsigned int& id, unsigned int& i,unsigned int& j, Vector3d pt){

    double t1, t2, t3;

    double tol = 1e-15;

    t1 = (pt[0] - frattura.Vertices[id](0,i))/(frattura.Vertices[id](0,j)-frattura.Vertices[id](0,i));
    t2 = (pt[1] - frattura.Vertices[id](1,i))/(frattura.Vertices[id](1,j)-frattura.Vertices[id](1,i));
    t3 = (pt[2] - frattura.Vertices[id](2,i))/(frattura.Vertices[id](2,j)-frattura.Vertices[id](2,i));

    if (t1 >= 0-tol &&  t1 <= 1+tol){
        if (t2 >= 0-tol &&  t2 <= 1+tol){
            if (t3 >= 0-tol &&  t3 <= 1+tol){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }else{
        return false;
    }
}

}

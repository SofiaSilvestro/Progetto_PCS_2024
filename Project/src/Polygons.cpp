#include <Polygons.hpp>
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

    //Righe di stampa da commentare
    cout << frattura.NumberFractures << endl;
    for (const auto& coppia : frattura.Vertices) {
        cout << "Chiave: " << coppia.first <<endl<<"Valore: " <<endl<<setprecision(16)<<scientific<< coppia.second <<endl<<endl;
    }
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
        for(unsigned int j = 0; j < traccia.FracturesId.size(); j++)
        {
            // se il primo o il secondo id è i allora incremento di 1 il numero delle tracce
            if(i == traccia.FracturesId[j][0] || i == traccia.FracturesId[j][1])
            {
                frattura_traccia[i]=frattura_traccia[i]+ 1;
            }
        }
    }
    ofs << "# FractureId; NumTraces" << endl;
    for(unsigned int i = 0; i < frattura.NumberFractures; i++)
    {
        ofs << i << ";" << frattura_traccia[i] << endl;
    }

    ofs << "# TraceId; Tips; Length" << endl;
    for(unsigned int i = 0; i < traccia.FracturesId.size();i++)
    {
        ofs << i << ";" << traccia.Tips << sqrt(distanza_al_quadrato(traccia.Vertices[i][0],traccia.Vertices[i][1])) << endl;
        // NECESSARIO FARE L'ORDINAMENTO SUCCESSIVAMENTE E CAPIRE COME GESTIRE TIPS
    }
}

}

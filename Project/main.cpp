#include <iostream>
#include <Polygons.hpp>
#include <Utils.hpp>
#include <iomanip>

using namespace std;
using namespace FracturesLib;
using namespace PolygonalLibrary;

int main()
{
    Fractures frattura;
    PolygonalMesh mesh;
    Traces traccia;
    string filepath = "./DFN/FR3_data.txt";
    if(!importazione(filepath, frattura))
    {
        return 1;
    }
    if(!importazione_2(filepath, frattura,mesh))
    {
        return 2;
    }
    else
    {
        caricamento_dati(traccia, frattura);
        //caricamento_dati_2(traccia,frattura);
        esportazione(traccia, frattura);


        //!\\PROVA
        //lascio così vedete cosa ho fatto, ma poi si può togliere

        cout << endl;
        cout << endl;
        cout << "PROVA"<< endl;

        //stampa vertici delle tracce
        if(!traccia.Vertices.empty()){
            for (const auto& [id, vertices] : traccia.Vertices) {
                cout << "ID: " << id << endl;
                cout << "Vertices: ";
                for (const auto& vec : vertices) {
                    cout << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ") ";
                }
                cout << endl;
                cout << "---------------------" << endl;
            }
        }else{
            cout << "map<unsigned int, array<Vector3d, 2>> Vertices e' vuoto"<< endl;
        }

        //stampa se è passante o non passante per le tracce
        if(!traccia.Tips.empty()){
            for (const auto& [id, tips] : traccia.Tips) {
                cout << "ID: " << id << endl;
                cout << "Tips: ";
                for (bool tip : tips) {
                    cout << (tip ? "true" : "false") << " ";
                }
                cout << endl;
                cout << "---------------------" << endl;
            }
        }else{
            cout << "map<unsigned int, array<bool, 2>> Tips e' vuoto"<< endl;
        }

        //stampa la lunghezza delle tracce
        if(!traccia.Lenght.empty()){
            for (const auto& [id, length] : traccia.Lenght) {
                cout << "ID: " << id << ", Length: " << length << endl;
                cout << "---------------------" << endl;
            }
        }else{
            cout << "map<unsigned int, double> Lenght e' vuoto"<< endl;
        }

        vector<tuple<unsigned int, array<Vector3d, 2>, array<bool, 2>>> tracceOrdinate = OrdinamentoTracce(traccia);

        if (!tracceOrdinate.empty()) {

            // Stampa il contenuto del vettore di tuple
            for (const auto& t : tracceOrdinate) {
                unsigned int id = get<0>(t);
                array<Vector3d, 2> vertices = get<1>(t);
                array<bool, 2> tips = get<2>(t);

                cout << "ID: " << id << endl;
                cout << "Vertices: ";
                for (const auto& vec : vertices) {
                    cout << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ") ";
                }
                cout << endl;
                cout << "Tips: ";
                for (bool tip : tips) {
                    cout << (tip ? "true" : "false") << " ";
                }
                cout << endl;
                cout << "---------------------" << endl;
            }
        }else{
            cout << "Il vettore di tuple e' vuoto." << endl;
        }

        return 0;
    }
}

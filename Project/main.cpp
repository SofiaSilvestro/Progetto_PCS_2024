#include <iostream>
#include <iomanip>
#include "Utils.hpp"
#include "Polygons.hpp"

using namespace std;
using namespace FracturesLib;
using namespace PolygonalLibrary;

int main()
{
    Fractures frattura;
    PolygonalMesh mesh;
    Traces traccia;
    string filepath = "./DFN/FR10_data.txt";
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
        return 0;
    }
}

//------------------------------- utils.cpp
/*
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

}*/

//----------------------------------------- main
/*    //!\\PROVA
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
*/


//-------------------------------utils.hpp
/*///La funzione OrdinamentoTracce ordina le tracce per passanti per entrambi i poligoni,
/// passanti solo per un poligono, non passante per entrambi i poligoni.
/// Inoltre le ordina in lunghezza decrescente
//es elt del vettore: <id, {(x1, y1, z1), (x2, y2, z2)}, {true/false, true/false}>
vector<tuple<unsigned int, array<Vector3d, 2>, array<bool, 2>>> OrdinamentoTracce (Traces& traccia);*/

/*//!\\ da capire se vogliamo spostare questo namespace in un altro file
namespace SortLibrary {

template<typename T>
void Merge(vector<T>& v,
           const unsigned int& sx,
           const unsigned int& cx,
           const unsigned int& dx){

    unsigned int i = sx;
    unsigned int j = cx + 1;

    vector<T> b;
    b.reserve(dx - sx + 1);

    while( i <= cx && j <= dx)
    {
        if (v[i] <= v[j])
            b.push_back(v[i++]);
        else
            b.push_back(v[j++]);
    }

    if (i <= cx)
        b.insert(b.end(), v.begin() + i, v.begin() + cx + 1);
    if ( j <= dx)
        b.insert(b.end(), v.begin() + j, v.begin() + dx + 1);

    copy(b.begin(), b.end(), v.begin() + sx);

}

template<typename T>
void MergeSort(vector<T>& v,
               const unsigned int& sx,
               const unsigned int& dx){

    if (sx < dx)
    {
        unsigned int cx = (sx + dx) / 2;
        MergeSort(v, sx, cx);
        MergeSort(v, cx + 1, dx);

        Merge(v, sx, cx, dx);
    }

}

template<typename T>
void MergeSort(vector<T>& v){
    MergeSort(v, 0, v.size()-1);
}


template<typename T>
void BubbleSort(std::vector<T>& data)
{
    size_t rem_size = data.size();
    size_t last_seen = rem_size;
    bool swapped = true;

    while (swapped) {
        swapped = false;
        for (size_t i = 1; i < rem_size; i++) {
            if (data[i-1] > data[i]) {
                std::swap(data[i-1], data[i]);
                swapped = true;
                last_seen = i;
            }
        }
        //        rem_size = rem_size - 1;
        rem_size = last_seen;
    }
}

}
*/

//---------------------polygons.hpp
/*
// Vettore che indica la lunghezza delle tracce
vector<double> Lenght = {};*/

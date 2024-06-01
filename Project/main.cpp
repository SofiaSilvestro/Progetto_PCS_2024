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
    string filepath = "./DFN/FR50_data.txt";
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

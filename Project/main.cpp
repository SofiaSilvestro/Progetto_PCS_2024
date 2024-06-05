#include <iostream>
#include <iomanip>
#include "Utils.hpp"
#include "Polygons.hpp"

using namespace std;
using namespace DFNLibrary;
using namespace PolygonalLibrary;

int main()
{
    Fractures frattura;
    PolygonalMesh mesh;
    Traces traccia;
    string filepath = "./DFN/FR200_data.txt";
    if(!importazione(filepath, frattura))
    {
        return 1;
    }
    else
    {
        caricamento_dati(traccia, frattura);
        caricamento_dati_2(traccia,frattura);
        esportazione(traccia, frattura);
        return 0;
    }
}

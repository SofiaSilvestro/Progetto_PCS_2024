#include <iostream>
#include <Polygons.hpp>
#include <Utils.hpp>
#include<iomanip>
using namespace std;
using namespace FracturesLib;
int main()
{
    Fractures frattura;
    Traces traccia;
    string filepath = "./DFN/FR3_data.txt";
    if(!importazione(filepath, frattura))
    {
        return 1;
    }
    else{
        caricamento_dati(traccia,frattura);
        esportazione(traccia,frattura);
        return 0;
    }
}



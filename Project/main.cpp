#include <iostream>
#include <Polygons.hpp>
#include <Utils.hpp>

using namespace std;
using namespace FracturesLib;
int main()
{
    Fractures frattura;
    string filepath = "./DFN/FR3_data.txt";
    if(!importazione(filepath, frattura))
    {
        return 1;
    }
    return 0;
}


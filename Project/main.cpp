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


/*    for(unsigned int i=0;i<frattura.NumberFractures;i++){
        unsigned int j=i+1;
        if(valuta_intersezione(frattura,i,j)&&j<frattura.NumberFractures){
            cout<<i<<" "<<j<<" si potrebbero intersecare";
        }}*/

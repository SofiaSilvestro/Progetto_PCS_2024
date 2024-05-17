#include <iostream>
#include <Polygons.hpp>
#include <Utils.hpp>
#include<iomanip>
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
    else{
        for(unsigned int i=0;i<frattura.NumberFractures;i++){
            unsigned int j=i+1;
            while(j<frattura.NumberFractures){
                // Verifichiamo che i poligoni abbiano distanza minore della somma dei due raggi delle palle
                if(valuta_intersezione(frattura,i,j)){
                    cout<<endl<<"I piani "<<i<<" "<<j<<" potrebbero intersecarsi"<<endl;
                    // calcolo la retta passante tra i due piani
                    array<double,6> r_piano=Retta_tra_piani(frattura,i,j);
                    //sulla carta sappiamo che se il prodotto vettoriale delle due normali ai piani è zero allora sono paralleli
                    // le coordinate del risultato sono memorizzate nei primi tre spazi dell'array r_piano
                    if(abs(r_piano[0])<pow(10,-10)&& abs(r_piano[1])<pow(10,-10) && abs(r_piano[2])<pow(10,-10)){
                        //piani paralleli
                    }
                    else{
                        //piani non paralleli
                        //calcolo la retta tra i lati adiacenti del poligono 1
                        cout<<"PRIMO POLIGONO "<<endl;
                        unsigned int conta_p1=0;
                        unsigned int h=0;
                        unsigned int k=1;
                        while(h<frattura.Vertices[i].cols()){
                            //con l'if gestisco il caso dell'ultimo punto con il primo del poligono
                            if(k==frattura.Vertices[i].cols()){
                                k=0;
                            }
                            array<double,6> r_tra_punti=Retta_per_due_vertici_della_frattura(frattura,i,h,k);
                            // Inserisco una condizione per scartare i lati che non intersecano fissando una tolleranza: sulla carta i determinanti da scartare dovrebbero essere tutti zero
                            double det_1=r_piano[0]*r_tra_punti[2]-r_piano[2]*r_tra_punti[0];
                            double det_2=r_piano[1]*r_tra_punti[2]-r_piano[2]*r_tra_punti[1];
                            double det_3=r_piano[0]*r_tra_punti[1]-r_piano[1]*r_tra_punti[0];
                            if (abs(det_1)>pow(10,-10) || abs(det_2)>pow(10,-10) || abs(det_3)>pow(10,-10)){
                                Vector2d x=alpha_di_intersezione(r_piano,r_tra_punti);
                                cout<<x[0]<<" "<<x[1]<<endl;
                                //Calcolo il punto di intersezione
                                Vector3d punto_intersezione;
                                /*punto_intersezione[0]=r_piano[0]*x[0]+r_piano[3];
                                punto_intersezione[1]=r_piano[1]*x[0]+r_piano[4];
                                punto_intersezione[2]=r_piano[2]*x[0]+r_piano[5];*/
                                punto_intersezione[0]=r_tra_punti[0]*x[1]+r_tra_punti[3];
                                punto_intersezione[1]=r_tra_punti[1]*x[1]+r_tra_punti[4];
                                punto_intersezione[2]=r_tra_punti[2]*x[1]+r_tra_punti[5];
                                // CONDIZIONI : verifichiamo che appartenga al segmento
                                double tol=pow(10,-15);
                                if(x[1]>=-tol && x[1]<=1-tol){
                                    cout<<"Il punto di intersezione e'"<<setprecision(16)<< punto_intersezione[0]<<" "<<punto_intersezione[1]<<" "<<punto_intersezione[2]<<endl;
                                    conta_p1++;
                                }
                            }
                            h++;
                            k++;
                        }
                        if(conta_p1==2){
                            cout<<"Traccia passante";
                        }
                        else{
                            cout<<"Traccia non passante";
                        }
                        //calcolo la retta tra i lati adiacenti del poligono 2
                        cout<<endl<<"SECONDO POLIGONO "<<endl;
                        unsigned int conta_p2=0;
                        h=0;
                        k=1;
                        while(h<frattura.Vertices[j].cols()){
                            //con l'if gestisco il caso dell'ultimo punto con il primo del poligono
                            if(k==frattura.Vertices[j].cols()){
                                k=0;
                            }
                            array<double,6> r_tra_punti=Retta_per_due_vertici_della_frattura(frattura,j,h,k);
                            // Inserisco una condizione per scartare i lati che non intersecano fissando una tolleranza:sulla carta i determinanti da scartare dovrebbero essere tutti zero
                            double det_1=r_piano[0]*r_tra_punti[2]-r_piano[2]*r_tra_punti[0];
                            double det_2=r_piano[1]*r_tra_punti[2]-r_piano[2]*r_tra_punti[1];
                            double det_3=r_piano[0]*r_tra_punti[1]-r_piano[1]*r_tra_punti[0];
                            if (abs(det_1)>pow(10,-10) || abs(det_2)>pow(10,-10) || abs(det_3)>pow(10,-10)){
                                Vector2d x=alpha_di_intersezione(r_piano,r_tra_punti);
                                cout<<x[0]<<" "<<x[1]<<endl;
                                //Calcolo il punto di intersezione
                                Vector3d punto_intersezione;
                                /*punto_intersezione[0]=r_piano[0]*x[0]+r_piano[3];
                                punto_intersezione[1]=r_piano[1]*x[0]+r_piano[4];
                                punto_intersezione[2]=r_piano[2]*x[0]+r_piano[5];*/
                                punto_intersezione[0]=r_tra_punti[0]*x[1]+r_tra_punti[3];
                                punto_intersezione[1]=r_tra_punti[1]*x[1]+r_tra_punti[4];
                                punto_intersezione[2]=r_tra_punti[2]*x[1]+r_tra_punti[5];
                                // CONDIZIONI: verifichiamo che appartenga al segmento
                                double tol=pow(10,-15);
                                if(x[1]>=-tol && x[1]<=1+tol){
                                    cout<<"Il punto di intersezione e'"<<setprecision(16)<< punto_intersezione[0]<<" "<<punto_intersezione[1]<<" "<<punto_intersezione[2]<<endl;
                                    conta_p2++;
                                }
                            }
                            h++;
                            k++;
                        }
                        if(conta_p2==2){
                            cout<<"Traccia passante";
                        }
                        else{
                            cout<<"Traccia non passante";
                        }
                    }
                }
                j++;
            }
        }
        return 0;
    }
}


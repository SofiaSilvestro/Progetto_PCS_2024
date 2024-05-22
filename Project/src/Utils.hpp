#pragma once
#include <iostream>
#include"Polygons.hpp"
using namespace std;

namespace FracturesLib{

///La funzione importazione legge i dati da un file e popola una struttura Fractures con questi dati
//Apre un file di testo
//Legge il numero di fratture presenti nel file e lo memorizza in frattura.NumberFractures
//Per ogni frattura:
    //Legge l'id della frattura e il numero di vertici
    //Legge le coordinate dei vertici e le memorizza in una matrice
    //Aggiunge la matrice dei vertici alla mappa Vertices della struttura frattura
//Calcola i coefficienti del piano per ciascuna frattura utilizzando tre dei suoi vertici e memorizza questi coefficienti nella mappa Piano della struttura frattura
//Chiude il file e ritorna true se l'operazione è avvenuta con successo, false altrimenti
bool importazione(const string& filename, Fractures& frattura);

//La funzione distanza_al_quadrato calcola la distanza euclidea al quadrato tra due vettori tridimensionali v1 e v2
double distanza_al_quadrato(Vector3d& v1, Vector3d& v2);

///La funzione esportazione salva i dati delle tracce e delle fratture in un file di testo chiamato Traces.txt
//Il file Traces.txt viene aperto in modalità di scrittura. Se il file non può essere aperto, viene visualizzato un messaggio di errore e la funzione termina
//Scrive nel file il numero totale di tracce
//Per ogni traccia, scrive l'ID della traccia, gli ID delle due fratture associate e le coordinate dei due vertici che definiscono la traccia
//Crea una mappa (frattura_traccia) che associa l'ID di ogni frattura al numero di tracce che la coinvolgono
//Per ogni frattura, se ha almeno una traccia associata, scrive l'ID della frattura e il numero di tracce che la coinvolgono
//Per ogni traccia associata alla frattura, scrive l'ID della traccia, se è passante o no e la lunghezza della traccia
void esportazione(Traces& traccia, Fractures& frattura);

///La funzione valuta_intersezione determina se due fratture potrebbero intersecarsi basandosi su una verifica geometrica preliminare
//Per entrambe le fratture, si calcola il baricentro sommando le coordinate di tutti i vertici e dividendo per il numero totale di vertici
//Calcolo dei raggi:
    //Si calcolano le distanze al quadrato tra ogni vertice e il baricentro per entrambe le fratture
    //Si determinano i raggi prendendo la distanza massima tra i vertici e il baricentro per ciascuna frattura
//Si verifica se la distanza al quadrato tra i due baricentri è inferiore o uguale alla somma dei raggi delle due sfere al quadrato
    //Se questo è vero le fratture potrebbero intersecarsi, ma se questo è falso sicuramente non si intersecano
bool valuta_intersezione (Fractures& frattura, unsigned int& Id1, unsigned int& Id2);

///La funzione Retta_tra_piani restituisce un array di 6 elementi che descrive la retta di intersezione tra due fratture
// I primi tre elementi dell'array coord_retta rappresentano la direzione della retta di intersezione e vengono calcolati come il prodotto vettoriale delle normali dei due piani
// Risoluzione sistema lineare A*x=b con A matrice costruita con i coefficienti dei piani e la direzione della retta e b contenente i termini costanti delle equazioni dei piani e uno zero, trovando così un punto che appartiene alla retta intersecante
array<double,6> Retta_tra_piani(Fractures& frattura, unsigned int& id1, unsigned int& id2);

///La funzione Retta_per_due_vertici_della_frattura calcola la retta passante per due vertici di una frattura
// Calcolo della direzione della retta (x2 - x1, y2 - y1, z2 - z1)
//Calcolo del punto P sulla retta (x1, y1, z1)
array<double,6> Retta_per_due_vertici_della_frattura(Fractures& frattura, unsigned int& id, unsigned int& i,unsigned int& j);

Vector2d alpha_di_intersezione(array<double,6> r_intersez,array<double,6> r_fratt);
Vector3d baricentro (Fractures& frattura, unsigned int& Id1);
double raggio(Fractures& frattura, unsigned int& Id1, Vector3d& coord_bar_1);

///La funzione Punto_intersezione_rette_piano_frattura trova il punto di intersezione (se esiste) fra la retta generata dal piano e la retta generata dal segmento
//Restituirà il punto di intersezione se trovato con successo, altrimenti restituirà un vettore con valori NaN
//Viene risolto il sistema lineare usando la decomposizione QR per trovare i valori dei parametri t
//Con il valore di t, viene calcolato il punto di intersezione sulla retta
//Viene calcolato il residuo r come differenza tra il termine noto originale e il risultato ottenuto moltiplicando A per t
//Si verifica se il residuo è sufficientemente piccolo rispetto a una soglia di tolleranza. Se il residuo supera questa soglia, significa che il punto calcolato non è un'intersezione accurata, quindi viene restituito un vettore con valori NaN
Vector3d Punto_intersezione_rette_piano_frattura(array<double,6> r_intersez, array<double,6> r_fratt);

///La funzione Controllo_puntoIntersezione_segmentoFrattura controlla se il punto pt appartiene al segmento definito dai vertici della frattura identificata dall'id, utilizzando l'indice dei due vertici i e j
//Calcola i parametri t1,t2,t3 che rappresentano le coordinate parametriche del punto pt rispetto al segmento formato dai due vertici della frattura
//Verifica se ciascun parametro t è compreso tra 0 e 1 (con una tolleranza di 10^-15)
//Se tutti e tre i parametri sono all'interno di questo intervallo, allora il punto pt è considerato all'interno del segmento e la funzione restituirà true, altrimenti restituirà false
bool Controllo_puntoIntersezione_segmentoFrattura (Fractures& frattura, unsigned int& id, unsigned int& i,unsigned int& j, Vector3d pt);

}

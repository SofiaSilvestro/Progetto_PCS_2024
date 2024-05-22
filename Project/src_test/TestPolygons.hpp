#ifndef __TestPolygons_HPP__
#define __TestPolygons_HPP__

#include <gtest/gtest.h>
// #include <math.h>
#include <array>
#include <vector>
#include <iostream>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

/** PROVA: TEST SU FUNZIONE CHE CALCOLA IL BARICENTRO **/
array<double, 3> PolyCentroid(vector<array<double, 3>> &Vertices)
{
    double iCent = 0.;
    array<double, 3> CoordCent = {};
    unsigned int NumVert = Vertices.size();
    for(unsigned int i = 0; i < 3; i++) // ciclo sulle coordinate
    {
        iCent = 0;
        // ciclo sui vertici considerando la coord i (i = 0: x, i = 1: y, i = 2: z)
        for(unsigned int j = 0; j < NumVert; j++)
        {
            iCent += Vertices[j][i]; // iCent = iCent + Vertices[j][i]
        }
    CoordCent[i] = iCent / NumVert;
    }
    return CoordCent;
}

TEST(CentroidTest, Quadrangle)
{
    vector<array<double, 3>> A = {{3, 0, 0}, {0, 0, 3}, {0, -3, 3}, {3, -3, 0}};
    array<double, 3> result = PolyCentroid(A);
    array<double, 3> expected = {3./2., -3./2., 3./2.};
    ASSERT_EQ(result, expected);
}

/** TEST SU FUNZIONE CHE CALCOLA LA DISTANZA (AL QUADRATO) TRA DUE PUNTI **/
double distanza_al_quadrato(Vector3d& v1, Vector3d& v2)
{
    return (v1[0] - v2[0]) * (v1[0] - v2[0])
           + (v1[1] - v2[1]) * (v1[1] - v2[1])
           + (v1[2] - v2[2]) * (v1[2] - v2[2]);
}

TEST(DistanceTest, TwoPoints)
{
    Vector3d firstP = {2, 2, 2};
    Vector3d secondP = {3, 3, 3};
    double result = distanza_al_quadrato(firstP, secondP);
    double expected = 3.;
    ASSERT_DOUBLE_EQ(result, expected);
}

// ///TEST FUNZIONE PER TROVARE PUNTO DI INTERSEZIONE TRA LA RETTA FORMATA DALLE 2 FRATTURE E LA RETTA GENERATA DAL SEGMENTO DI UNA FRATTURA
//     //poi da togliere
// cout << endl;
// cout << "TEST INTERSEZIONE" <<endl;
// array<double, 6> r_intersez = {1, 0, 0, 0, 0, 0}; // x = t, y = 0, z = 0
// array<double, 6> r_fratt = {0, 1, 0, 1, 1, 0}; // x = 1, y = s + 1, z = 0
// Vector3d intersezione = Punto_intersezione_rette_piano_frattura(r_intersez, r_fratt);
// if (isnan(intersezione[0])) {
//     cout << "Le rette non si intersecano" << endl;
// } else {
//     cout << "Il punto di intersezione e': " << intersezione.transpose() << endl;
// }

// return 0;








#endif

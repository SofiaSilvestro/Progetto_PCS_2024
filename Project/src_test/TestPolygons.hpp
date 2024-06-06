#ifndef __TestPolygons_HPP__
#define __TestPolygons_HPP__

#include <gtest/gtest.h>
#include <array>
#include <vector>
#include <iostream>
#include <Eigen/Eigen>
#include "Utils.hpp"
// #include "UCDUtilities.hpp"
#include "Polygons.hpp"

using namespace std;
using namespace Eigen;

namespace DFNLibrary{

/** TEST SULLA FUNZIONE CHE CALCOLA L'EQUAZIONE DEL PIANO PASSANTE PER 3 PUNTI **/
TEST(TestRetteEPiani, EqPiano3Punti)
{
    Fractures frattura;
    unsigned int Id = 0;
    Matrix<double, 3, 4> Vert = {};
    Vert << 3., 0. , 0., 3.,
            0., 0., -3., -3.,
            0., 3., 3., 0.;

    frattura.Vertices.push_back(Vert);

    array<double, 4> result = EqPiano(frattura, Id);
    array<double, 4> expected = {9, 0, 9, -27};
    ASSERT_EQ(result, expected);
}


/** TEST SULLA FUNZIONE CHE CALCOLA LA DISTANZA (AL QUADRATO) TRA DUE PUNTI **/
TEST(TestDistanze, TraDuePunti)
{
    Vector3d firstP = {2, 2, 2};
    Vector3d secondP = {3, 3, 3};
    double result = distanza_al_quadrato(firstP, secondP);
    double expected = 3.;
    ASSERT_DOUBLE_EQ(result, expected);
}

/** TEST SULLA FUNZIONE CHE CALCOLA IL BARICENTRO **/
TEST(TestBaricentro, Quadrilatero)
{
    Fractures frattura;
    unsigned int Id = 0;
    Matrix<double, 3, 4> Vert = {};
    Vert << 3., 0. , 3., 0.,
            0., 0., -3., -3.,
            0., 3., 0., 3.;
    frattura.Vertices.push_back(Vert);

    Vector3d result = baricentro(frattura, Id);
    Vector3d expected = {3./2., -3./2., 3./2.};
    ASSERT_EQ(result, expected);
}

/** TEST SULLA FUNZIONE CHE VALUTA LA DISTANZA TRA DUE POLIGONI PER DETERMINARE PRELIMINARMENTE LA POSSIBILITA' DELL'INTERSEZIONE **/
TEST(TestDistanze, TraQuadrilateri)
{
    Fractures frattura;
    unsigned int Id1 = 0;
    unsigned int Id2 = 1;
    Matrix<double, 3, 4> Vert1 = {};
    Matrix<double, 3, 4> Vert2 = {};
    Vert1 << 0., 1., 1., 0.,
             0., 0., 1., 1.,
             0., 0., 0., 0.;
    Vert2 << 0.8, 0.8, 0.8, 0.8,
             0., 0., 1., 1.,
             -0.1, 0.3, 0.3, -0.1;

    frattura.Vertices.push_back(Vert1);
    frattura.Vertices.push_back(Vert2);

    bool result = valuta_intersezione(frattura, Id1, Id2);
    ASSERT_TRUE(result);
}

/** TEST SULLA FUNZIONE CHE VALUTA L'INTERSEZIONE TRA DUE PIANI GENERATI RISPETTIVAMENTE DA DUE POLIGONI DISTINTI **/
TEST(TestRetteEPiani, RettaTraPiani)
{
    Fractures frattura;
    unsigned int Id1 = 0;
    unsigned int Id2 = 1;
    array<double, 4> Arr1 = {3, 2, 1, -2};
    array<double, 4> Arr2 = {6, 5, 4, -11};

    frattura.Plane.push_back(Arr1);
    frattura.Plane.push_back(Arr2);

    array<double, 6> result = Retta_tra_piani(frattura, Id1, Id2);
    array<double, 6> expected = {3, -6, 3, -1, 1, 3};
    ASSERT_EQ(result, expected);
}

/** TEST SULLA FUNZIONE CHE CALCOLA I COEFFICIENTI DELLA RETTA PASSANTE PER DUE VERTICI DI UN POLIGONO **/
TEST(TestRetteEPiani, RettaTraVertici)
{
    Fractures frattura;
    unsigned int Id = 0;
    unsigned int i = 0;
    unsigned int j = 1;
    Matrix<double, 3, 4> Vert = {};
    Vert << 0.8, 0.8, 0.8, 0.8,
            0., 0., 1., 1.,
            -0.1, 0.3, 0.3, -0.1;

    frattura.Vertices.push_back(Vert);

    array<double, 6> result = Retta_per_due_vertici_della_frattura(frattura, Id, i, j);
    array<double, 6> expected = {0., 0., 0.4, 0.8, 0., -0.1};
    ASSERT_EQ(result, expected);
}

/** TEST SULLA FUNZIONE CHE CALCOLA L'ASCISSA CURVILINEA DATE DUE RETTE IN FORMA PARAMETRICA **/
TEST(TestRetteEPiani, AscissaCurvilinea)
{
    array<double, 6> r1 = {4, 1, 5, 0, 18, 0};
    array<double, 6> r2 = {2, 6, 3, 2, 2, 1};

    Vector2d result = alpha_di_intersezione(r1, r2);
    Vector2d expected = {3, 2};
    ASSERT_EQ(result, expected);
}

/** TEST SULLA FUNZIONE CHE CALCOLA LA RETTA PASSANTE PER DUE PUNTI **/
TEST(TestRetteEPiani, RettaPerDuePunti)
{
    Vector3d vert1 = {1, 2, 3};
    Vector3d vert2 = {4, 5, 6};

    array<double, 6> result = coord_retta_tra2punti(vert1, vert2);
    array<double, 6> expected = {3, 3, 3, 1, 2, 3};
    ASSERT_EQ(result, expected);
}

/** TEST CHE CALCOLA L'INTERSEZIONE TRA DUE RETTE NELLO SPAZIO **/
TEST(TestRetteEPiani, IntersezioneRette)
{
    Vector3d vertice1 = {1, 2, 3};
    Vector3d vertice2 = {4, 5, 6};
    Vector3d vertice3 = {1, 5, 3};
    Vector3d vertice4 = {4, 2, 6};

    Vector3d result = intersezione_rette(vertice1, vertice2, vertice3, vertice4);
    Vector3d expected = {2.5, 3.5, 4.5};
    ASSERT_EQ(result, expected);
}

}
#endif

#ifndef __TestPolygons_HPP__
#define __TestPolygons_HPP__

#include <gtest/gtest.h>
// #include <math.h>
#include <array>
#include <vector>
#include <iostream>
#include <Eigen/Eigen>
#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include "Polygons.hpp"

using namespace std;
using namespace Eigen;

namespace FracturesLib{

/** TEST SU FUNZIONE CHE CALCOLA LA DISTANZA (AL QUADRATO) TRA DUE PUNTI **/
TEST(TestDistanze, Tra2Punti)
{
    Vector3d firstP = {2, 2, 2};
    Vector3d secondP = {3, 3, 3};
    double result = distanza_al_quadrato(firstP, secondP);
    double expected = 3.;
    ASSERT_DOUBLE_EQ(result, expected);
}

/** TEST SU FUNZIONE CHE CALCOLA IL BARICENTRO **/
TEST(TestBaricentro, Quadrilatero)
{
    Fractures frattura;
    unsigned int Id = 0;
    Matrix<double, 3, 4> Mat;
    Mat << 3., 0. , 3., 0.,
           0., 0., -3., -3.,
           0., 3., 0., 3.;
    frattura.Vertices[Id] = Mat;

    Vector3d result = baricentro(frattura, Id);
    Vector3d expected = {3./2., -3./2., 3./2.};
    ASSERT_EQ(result, expected);
}

/** TEST SU FUNZIONE CHE VALUTA LA DISTANZA TRA DUE POLIGONI PER
 *  DETERMINARE PRELIMINARMENTE LA POSSIBILITA' DELL'INTERSEZIONE **/
TEST(RetteEPiani, Distanza_Tra_Poligoni)
{
    Fractures frattura;
    unsigned int Id1 = 0;
    unsigned int Id2 = 1;
    frattura.Vertices[Id1] << 0., 1., 1., 0.,
                              0., 0., 1., 1.,
                              0., 0., 0., 0.;
    frattura.Vertices[Id2] << 0.8, 0.8, 0.8, 0.8,
                              0., 0., 1., 1.,
                              -0.1, 0.3, 0.3, -0.1;

    bool result = valuta_intersezione (frattura, Id1, Id2);
    bool expected = true;
    ASSERT_EQ(result, expected);
}





/** TEST SU FUNZIONE CHE CALCOLA L'ASCISSA CURVILINEA DATE DUE RETTE IN FORMA PARAMETRICA **/
TEST(RetteEPiani, Ascissa_Curvilinea)
{
    array<double, 6> r1 = {2, 6, 3, 2, 2, 1};
    array<double, 6> r2 = {4, 1, 5, 0, 18, 0};

    Vector2d result = alpha_di_intersezione(r2, r1);
    Vector2d expected = {3, 2};
    ASSERT_EQ(result, expected);
}








}
#endif

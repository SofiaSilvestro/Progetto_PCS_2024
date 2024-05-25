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

/** TEST SU FUNZIONE CHE CALCOLA IL BARICENTRO **/

// Vector3d baricentro (Fractures& frattura, unsigned int& Id)
// {
//     Vector3d coord_bar = {};
//     unsigned int n = frattura.Vertices[Id].cols(); // numero di vertici della frattura
//     for(unsigned int h = 0; h < 3; h++)
//     {
//         for (unsigned int k = 0; k < n; k++)
//         {
//             coord_bar[h] = coord_bar[h] + frattura.Vertices[Id](h,k);
//         }
//         coord_bar[h] = coord_bar[h] / n;
//     }
//     return coord_bar;
// }

// TEST(CentroidTest, Quadrangle)
// {
//     Fractures frattura;
//     unsigned int Id = 0;

//     map<unsigned int, Matrix<double, 3, 4>> Vertices = {};
//     Matrix<double, 3, 4> mat;
//     mat [0][0] = 3; mat [0][1] = 0; mat [0][2] = 3; mat [0][3] = 0;
//     mat [1][0] = 0; mat [1][1] = 0; mat [1][2] = -3; mat [1][3] = -3;
//     mat [2][0] = 0; mat [2][1] = 3; mat [2][2] = 0; mat [2][3] = 3;
//     Vertices[Id] = mat;

//     Vector3d result = baricentro(frattura, Id);
//     Vector3d expected = {3./2., -3./2., 3./2.};
//     ASSERT_EQ(result, expected);
// }

/** TEST SU FUNZIONE CHE CALCOLA LA DISTANZA (AL QUADRATO) TRA DUE PUNTI **/

TEST(DistanceTest, TwoPoints)
{
    Vector3d firstP = {2, 2, 2};
    Vector3d secondP = {3, 3, 3};
    double result = distanza_al_quadrato(firstP, secondP);
    double expected = 3.;
    ASSERT_DOUBLE_EQ(result, expected);
}

/** TEST SU FUNZIONE CHE CALCOLA L'ASCISSA CURVILINEA DATE DUE RETTE IN FORMA PARAMETRICA **/

TEST(parametri_rette, ascissa_curvilinea)
{
    array<double, 6> r1 = {2, 6, 3, 2, 2, 1};
    array<double, 6> r2 = {4, 1, 5, 0, 18, 0};

    Vector2d result = alpha_di_intersezione(r2, r1);
    Vector2d expected = {3, 2};
    ASSERT_EQ(result, expected);
}

}
#endif

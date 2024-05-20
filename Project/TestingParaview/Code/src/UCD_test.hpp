#ifndef __UCD_test_HPP__
#define __UCD_test_HPP__

#include "UCDUtilities.hpp"
#include <gtest/gtest.h>

using namespace std;
using namespace Eigen;

// ***************************************************************************
TEST(TestUCDUtilities, UCDUtilities_Test0Ds)
{
  string exportFolder = "./";

  Gedim::UCDUtilities exporter;
  const MatrixXd points = (MatrixXd(3, 4)<< 0.0, 1.0, 1.0, 0.0,
                                  0.0, 0.0, 1.0, 1.0,
                                  2.0, 2.0, 2.0, 2.0).finished();

  exporter.ExportPoints(exportFolder + "/Geometry0Ds.inp",
                        points);

}
// ***************************************************************************
TEST(TestUCDUtilities, UCDUtilities_Test1Ds)
{
  string exportFolder = "./";

  Gedim::UCDUtilities exporter;
  const MatrixXd points = (MatrixXd(3, 4)<< 0.0, 1.0, 1.0, 0.0,
                                  0.0, 0.0, 1.0, 1.0,
                                  2.0, 2.0, 2.0, 2.0).finished();
  const MatrixXi edges = (MatrixXi(2, 5)<< 0, 1, 2, 3, 0,
                                 1, 2, 3, 0, 2).finished();

  exporter.ExportSegments(exportFolder + "/Geometry1Ds.inp",
                          points,
                          edges);
}
// ***************************************************************************
TEST(TestUCDUtilities, UCDUtilities_Test2Ds)
{
  string exportFolder = "./";

  Gedim::UCDUtilities exporter;
  const MatrixXd points = (MatrixXd(3, 8)<< 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0,
                                  0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0,
                                  2.0, 2.0, 2.0, 2.0, 4.0, 4.0, 4.0, 4.0).finished();
  const vector<vector<unsigned int>> polygons =
  {
    { 0, 1, 2 },
    { 0, 2, 3 },
    { 4, 5, 6, 7 }
  };

  exporter.ExportPolygons(exportFolder + "/Geometry2Ds.inp",
                          points,
                          polygons);
}
// ***************************************************************************

#endif // __UCD_test_HPP__

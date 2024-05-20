// ------------------------------------------------------------------------------
//
// Copyright (C) 2024 by the GeDiM authors.
// This file is part of GeDiM.
//
// GeDiM is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GeDiM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with GeDiM.  If not, see <http://www.gnu.org/licenses/>.
// ------------------------------------------------------------------------------

// Author: Fabio Vicini <fabio.vicini@polito.it>

#ifndef __UCDUtilities_H
#define __UCDUtilities_H

#include "Eigen/Eigen"
#include <list>
#include <string>

using namespace std;
using namespace Eigen;

namespace Gedim
{
  template <typename T>
  struct UCDProperty final
  {
      string Label;
      string UnitLabel;

      unsigned int Size;
      unsigned int NumComponents;
      const T* Data;
  };


  class UCDCell final
  {
    public:
      enum struct Types
      {
        Unknown = -1,
        Point = 0,
        Line = 1,
        Triangle = 2,
        Quadrilateral = 3,
        Hexahedron = 4,
        Prism = 5,
        Tetrahedron = 6,
        Pyramid = 7
      };

      const Types Type;
      const vector<unsigned int> PointIds;
      const unsigned int MaterialId;

      UCDCell(const Types type,
              const vector<unsigned int> pointIds,
              const unsigned int materialId) :
        Type(type),
        PointIds(pointIds),
        MaterialId(materialId)
      { }

      const std::string CellLabel(const UCDCell::Types type) const;
  };

  class UCDUtilities final
  {
    public:
      enum ExportFormats
      {
        Ascii = 0
      };

    private:
      vector<UCDCell> CreatePointCells(const MatrixXd& points,
                                            const VectorXi& materials) const;
      vector<UCDCell> CreateLineCells(const MatrixXi& lines,
                                           const VectorXi& materials) const;
      vector<UCDCell> CreatePolygonCells(const vector<vector<unsigned int>>& polygons_vertices,
                                              const VectorXi& materials) const;
      vector<UCDCell> CreatePolyhedraCells(const vector<vector<unsigned int>>& polyhedra_vertices,
                                                const VectorXi& materials) const;

      void ExportUCDAscii(const MatrixXd& points,
                          const vector<UCDProperty<double>>& point_properties,
                          const vector<UCDCell>& cells,
                          const vector<UCDProperty<double>>& cell_properties,
                          const string& filePath) const;

    public:
      UCDUtilities() { }
      virtual ~UCDUtilities() { }

      void ExportPoints(const string& filePath,
                        const MatrixXd& points,
                        const vector<UCDProperty<double>>& points_properties = {},
                        const VectorXi& materials = {}) const;

      void ExportSegments(const string& filePath,
                          const MatrixXd& points,
                          const MatrixXi& segments,
                          const vector<UCDProperty<double>>& points_properties = {},
                          const vector<UCDProperty<double>>& segmnents_properties = {},
                          const VectorXi& materials = {}) const;

      void ExportPolygons(const string& filePath,
                          const MatrixXd& points,
                          const vector<vector<unsigned int>>& polygons_vertices,
                          const vector<UCDProperty<double>>& points_properties = {},
                          const vector<UCDProperty<double>>& polygons_properties = {},
                          const VectorXi& materials = {}) const;

      void ExportPolyhedra(const string& filePath,
                           const MatrixXd& points,
                           const vector<vector<unsigned int>>& polyhedra_vertices,
                           const vector<UCDProperty<double>>& points_properties = {},
                           const vector<UCDProperty<double>>& polyhedra_properties = {},
                           const VectorXi& materials = {}) const;
  };
}

#endif // __UCDUtilities_H

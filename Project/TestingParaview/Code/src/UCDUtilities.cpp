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

#include "UCDUtilities.hpp"
#include <fstream>

using namespace std;
using namespace Eigen;

namespace Gedim
{
  // ***************************************************************************
  void UCDUtilities::ExportPoints(const string& filePath,
                                  const MatrixXd& points,
                                  const vector<UCDProperty<double>>& points_properties,
                                  const VectorXi& materials) const
  {
    ExportFormats format = ExportFormats::Ascii;

    switch (format)
    {
      case ExportFormats::Ascii:
        ExportUCDAscii(points,
                       { },
                       CreatePointCells(points,
                                        materials),
                       points_properties,
                       filePath);
        break;
      default:
        throw std::runtime_error("Unknown format");
    }
  }
  // ***************************************************************************
  void UCDUtilities::ExportSegments(const string& filePath,
                                    const MatrixXd& points,
                                    const MatrixXi& segments,
                                    const vector<UCDProperty<double> >& points_properties,
                                    const vector<UCDProperty<double> >& segmnents_properties,
                                    const VectorXi& materials) const
  {
    ExportFormats format = ExportFormats::Ascii;

    switch(format)
    {
      case ExportFormats::Ascii:
        ExportUCDAscii(points,
                       points_properties,
                       CreateLineCells(segments,
                                       materials),
                       segmnents_properties,
                       filePath);
        break;
      default:
        throw runtime_error("Unknown format");
    }
  }
  // ***************************************************************************
  void UCDUtilities::ExportPolygons(const string& filePath,
                                    const MatrixXd& points,
                                    const vector<vector<unsigned int> >& polygons_vertices,
                                    const vector<UCDProperty<double> >& points_properties,
                                    const vector<UCDProperty<double> >& polygons_properties,
                                    const VectorXi& materials) const
  {
    ExportFormats format = ExportFormats::Ascii;

    switch (format)
    {
      case ExportFormats::Ascii:
        ExportUCDAscii(points,
                       points_properties,
                       CreatePolygonCells(polygons_vertices,
                                          materials),
                       polygons_properties,
                       filePath);
        break;
      default:
        throw runtime_error("Unknown format");
    }
  }
  // ***************************************************************************
  void UCDUtilities::ExportPolyhedra(const string& filePath,
                                     const MatrixXd& points,
                                     const vector<vector<unsigned int>>& polyhedra_vertices,
                                     const vector<UCDProperty<double>>& points_properties,
                                     const vector<UCDProperty<double>>& polyhedra_properties,
                                     const VectorXi& materials) const
  {
    ExportFormats format = ExportFormats::Ascii;

    switch(format)
    {
      case ExportFormats::Ascii:
        ExportUCDAscii(points,
                       points_properties,
                       CreatePolyhedraCells(polyhedra_vertices,
                                            materials),
                       polyhedra_properties,
                       filePath);
        break;
      default:
        throw std::runtime_error("Unknown format");
    }
  }
  // ***************************************************************************
  vector<UCDCell> UCDUtilities::CreatePointCells(const MatrixXd& points,
                                                 const VectorXi& materials) const
  {
    vector<UCDCell> cells;
    cells.reserve(points.cols());

    for (unsigned int p = 0; p < points.cols(); p++)
    {
      cells.push_back(UCDCell(UCDCell::Types::Point,
                              { p },
                              materials.size() == points.cols() ?
                                materials[p] :
                                0));
    }

    return cells;
  }
  // ***************************************************************************
  vector<UCDCell> UCDUtilities::CreateLineCells(const MatrixXi& lines,
                                                const VectorXi& materials) const
  {
    vector<UCDCell> cells;
    cells.reserve(lines.cols());

    for (unsigned int l = 0; l < lines.cols(); l++)
    {
      cells.push_back(UCDCell(UCDCell::Types::Line,
                              {
                                static_cast<unsigned int>(lines(0, l)),
                                static_cast<unsigned int>(lines(1, l))
                              },
                              materials.size() == lines.cols() ?
                                materials[l] :
                                0));
    }

    return cells;
  }
  // ***************************************************************************
  vector<UCDCell> UCDUtilities::CreatePolygonCells(const vector<vector<unsigned int>>& polygons_vertices,
                                                        const VectorXi& materials) const
  {
    vector<UCDCell> cells;
    cells.reserve(polygons_vertices.size());

    for (unsigned int l = 0; l < polygons_vertices.size(); l++)
    {
      UCDCell::Types polygon_type = UCDCell::Types::Unknown;

      if (polygons_vertices[l].size() == 3)
        polygon_type = UCDCell::Types::Triangle;
      else if (polygons_vertices[l].size() == 4)
        polygon_type = UCDCell::Types::Quadrilateral;
      else
        throw runtime_error("Polygon type not supported");

      cells.push_back(UCDCell(polygon_type,
                              polygons_vertices[l],
                              materials.size() == static_cast<unsigned int>(polygons_vertices.size()) ?
                                materials[l] :
                                0));
    }

    return cells;
  }
  // ***************************************************************************
  vector<UCDCell> UCDUtilities::CreatePolyhedraCells(const vector<vector<unsigned int> >& polyhedra_vertices,
                                                          const VectorXi& materials) const
  {
    vector<UCDCell> cells;
    cells.reserve(polyhedra_vertices.size());

    for(unsigned int l = 0; l < polyhedra_vertices.size(); l++)
    {
      UCDCell::Types polyhedra_type = UCDCell::Types::Unknown;

      if(polyhedra_vertices[l].size() == 4)
        polyhedra_type = UCDCell::Types::Tetrahedron;
      else
        throw runtime_error("Polygon type not supported");

      cells.push_back(UCDCell(polyhedra_type,
                              polyhedra_vertices[l],
                              materials.size() == static_cast<unsigned int>(polyhedra_vertices.size()) ?
                                materials[l] :
                                0));
    }

    return cells;
  }
  // ***************************************************************************
  void UCDUtilities::ExportUCDAscii(const MatrixXd& points,
                                    const vector<UCDProperty<double> >& point_properties,
                                    const vector<UCDCell>& cells,
                                    const vector<UCDProperty<double>>& cell_properties,
                                    const string& filePath) const
  {
    ofstream file;

    file.open(filePath.c_str());

    if(file.fail())
      throw runtime_error("File '" + filePath + "' cannot be opened");

    const char sep = ' ';
    file.precision(16);

    // export full info
    file << points.cols() << sep;
    file << cells.size() << sep;
    file << point_properties.size() << sep;
    file << cell_properties.size() << sep;
    file << 0 << endl; // model not supported

    // export points
    for(unsigned int p = 0; p < points.cols(); p++)
    {
      file << (p + 1) << sep;
      file << scientific << points(0, p) << sep;
      file << scientific << points(1, p) << sep;
      file << scientific << points(2, p) << endl;
    }

    // export cells
    for(unsigned int c = 0; c < cells.size(); c++)
    {
      file << (c + 1) << sep;
      file << scientific << cells[c].MaterialId << sep;
      file << scientific << cells[c].CellLabel(cells[c].Type);
      for(unsigned int p = 0; p < cells[c].PointIds.size(); p++)
      {
        file << sep << (cells[c].PointIds[p] + 1);
      }
      file << endl;
    }

    // export points properties
    if(point_properties.size() > 0)
    {
      file << point_properties.size();
      for(unsigned int pr = 0; pr < point_properties.size(); pr++)
      {
        file << sep << point_properties[pr].NumComponents;
      }
      file << endl;

      for(unsigned int pr = 0; pr < point_properties.size(); pr++)
      {
        file<< point_properties[pr].Label<< ','<< sep;
        file<< point_properties[pr].UnitLabel<< std::endl;
      }

      for(unsigned int p = 0; p < points.cols(); p++)
      {
        file << (p + 1);
        for(unsigned int pr = 0; pr < point_properties.size(); pr++)
        {
          for(unsigned int cp = 0; cp < point_properties[pr].NumComponents; cp++)
          {
            file << sep << point_properties[pr].Data[point_properties[pr].NumComponents * p + cp];
          }
        }
        file << endl;
      }
    }

    // export cells properties
    if(cell_properties.size() > 0)
    {
      file << cell_properties.size();
      for (unsigned int pr = 0; pr < cell_properties.size(); pr++)
      {
        file << sep << cell_properties[pr].NumComponents;
      }
      file << endl;

      for (unsigned int pr = 0; pr < cell_properties.size(); pr++)
      {
        file << cell_properties[pr].Label << ',' << sep;
        file << cell_properties[pr].UnitLabel << endl;
      }

      for (unsigned int c = 0; c < cells.size(); c++)
      {
        file << (c + 1);
        for (unsigned int pr = 0; pr < cell_properties.size(); pr++)
        {
          for (unsigned int cp = 0; cp < cell_properties[pr].NumComponents; cp++)
          {
            file << sep << cell_properties[pr].Data[cell_properties[pr].NumComponents * c + cp];
          }
        }
        file << endl;
      }
    }


    file.close();
  }
  // ***************************************************************************
  const string UCDCell::CellLabel(const UCDCell::Types type) const
  {
    switch (type)
    {
      case UCDCell::Types::Line:
        return "line";
      case UCDCell::Types::Triangle:
        return "tri";
      case UCDCell::Types::Quadrilateral:
        return "quad";
      case UCDCell::Types::Hexahedron:
        return "hex";
      case UCDCell::Types::Prism:
        return "prism";
      case UCDCell::Types::Tetrahedron:
        return "tet";
      case UCDCell::Types::Pyramid:
        return "pyr";
      case UCDCell::Types::Point:
        return "pt";
      default:
        throw runtime_error("Type not supported");
    }
  }

  // ***************************************************************************
}

//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MeshGenerator.h"

#include "libmesh/face_quad4.h"
#include "libmesh/cell_hex8.h"
#include "libmesh/edge_edge2.h"

/**
 * This class works by first creating a "distributed ual graph" of the element connectivity based
 * on a linear partition of mesh before ever building an elements. It then uses
 * PetscExternalPartitioner to partition that graph - assigning elements to processors. Then, each
 * processor can read the partition map and build only the elements that need to be on that
 * processor. Final steps include adding in "ghosted" elements and making sure that boundary
 * conditions are right.
 */
class DistributedRectilinearMeshGenerator : public MeshGenerator
{
public:
  static InputParameters validParams();

  DistributedRectilinearMeshGenerator(const InputParameters & parameters);

  std::unique_ptr<MeshBase> generate() override;

  /**
   * Build a cube mesh
   *
   * @param mesh Distributed UnstructuredMesh
   * @param nx The number of elements in the x direction
   * @param ny The number of elements in the y direction
   * @param nz The number of elements in the z direction
   * @param xmin Lower X Coordinate of the generated mesh
   * @param xmax Upper X Coordinate of the generated mesh
   * @param ymin Lower Y Coordinate of the generated mesh
   * @param ymax Upper Y Coordinate of the generated mesh
   * @param zmin Lower Z Coordinate of the generated mesh
   * @param zmax Upper Z Coordinate of the generated mesh
   * @param type The element type
   */
  template <typename T>
  void buildCube(UnstructuredMesh & /*mesh*/,
                 const unsigned int /*nx*/,
                 unsigned int /*ny*/,
                 unsigned int /*nz*/,
                 const Real /*xmin*/,
                 const Real /*xmax*/,
                 const Real /*ymin*/,
                 const Real /*ymax*/,
                 const Real /*zmin*/,
                 const Real /*zmax*/,
                 const ElemType /*type*/);

  /**
   * Get the element ID for a given hex
   *
   * @param nx The number of elements in the x direction
   * @param ny The number of elements in the y direction
   * @param i The x index of this element
   * @param j The y index of this element
   * @param k The z index of this element
   * @return The ID of the i,j element
   */
  template <typename T>
  dof_id_type elemId(const dof_id_type /*nx*/,
                     const dof_id_type /*ny*/,
                     const dof_id_type /*i*/,
                     const dof_id_type /*j*/,
                     const dof_id_type /*k*/)
  {
    mooseError(
        "elemId not implemented for this element type in DistributedRectilinearMeshGenerator");
  }

  template <>
  dof_id_type elemId<Edge2>(const dof_id_type nx,
                            const dof_id_type ny,
                            const dof_id_type i,
                            const dof_id_type j,
                            const dof_id_type k);

  template <>
  dof_id_type elemId<Quad4>(const dof_id_type nx,
                            const dof_id_type ny,
                            const dof_id_type i,
                            const dof_id_type j,
                            const dof_id_type k);

  template <>
  dof_id_type elemId<Hex8>(const dof_id_type nx,
                           const dof_id_type ny,
                           const dof_id_type i,
                           const dof_id_type j,
                           const dof_id_type k);

  /**
   * Get the number of neighbors this element will have
   *
   * @param nx The number of elements in the x direction
   * @param ny The number of elements in the y direction
   * @param nz The number of elements in the z direction
   * @param i The x index of this element
   * @param j The y index of this element
   * @param k The z index of this element
   * @return The number of neighboring elements
   */
  template <typename T>
  dof_id_type numNeighbors(const dof_id_type /*nx*/,
                           const dof_id_type /*ny*/,
                           const dof_id_type /*nz*/,
                           const dof_id_type /*i*/,
                           const dof_id_type /*j*/,
                           const dof_id_type /*k*/)
  {
    mooseError("numNeighbors not implemented for this element type in "
               "DistributedRectilinearMeshGenerator");
  }

  template <>
  dof_id_type numNeighbors<Edge2>(const dof_id_type nx,
                                  const dof_id_type ny,
                                  const dof_id_type nz,
                                  const dof_id_type i,
                                  const dof_id_type j,
                                  const dof_id_type k);

  template <>
  dof_id_type numNeighbors<Quad4>(const dof_id_type nx,
                                  const dof_id_type ny,
                                  const dof_id_type nz,
                                  const dof_id_type i,
                                  const dof_id_type j,
                                  const dof_id_type k);

  template <>
  dof_id_type numNeighbors<Hex8>(const dof_id_type nx,
                                 const dof_id_type ny,
                                 const dof_id_type nz,
                                 const dof_id_type i,
                                 const dof_id_type j,
                                 const dof_id_type k);

  /**
   * Get the IDs of the neighbors of a given element
   *
   * @param nx The number of elements in the x direction
   * @param nx The number of elements in the y direction
   * @param nz The number of elements in the z direction
   * @param i The x index of this element
   * @param j The y index of this element
   * @param k The z index of this element
   * @param neighbors This will be filled with the IDs of the two neighbors or invalid_dof_id if
   * there is no neighbor.  THIS MUST be of size 6 BEFORE calling this function
   */
  template <typename T>
  void getNeighbors(const dof_id_type /*nx*/,
                    const dof_id_type /*ny*/,
                    const dof_id_type /*nz*/,
                    const dof_id_type /*i*/,
                    const dof_id_type /*j*/,
                    const dof_id_type /*k*/,
                    std::vector<dof_id_type> & /*neighbors*/,
                    const bool /*corner = false*/)
  {
    mooseError("getNeighbors not implemented for this element type in "
               "DistributedRectilinearMeshGenerator");
  }

  template <>
  void getNeighbors<Edge2>(const dof_id_type nx,
                           const dof_id_type ny,
                           const dof_id_type nz,
                           const dof_id_type i,
                           const dof_id_type j,
                           const dof_id_type k,
                           std::vector<dof_id_type> & neighbors,
                           const bool corner);

  template <>
  void getNeighbors<Quad4>(const dof_id_type nx,
                           const dof_id_type ny,
                           const dof_id_type nz,
                           const dof_id_type i,
                           const dof_id_type j,
                           const dof_id_type k,
                           std::vector<dof_id_type> & neighbors,
                           const bool corner);

  template <>
  void getNeighbors<Hex8>(const dof_id_type nx,
                          const dof_id_type ny,
                          const dof_id_type nz,
                          const dof_id_type i,
                          const dof_id_type j,
                          const dof_id_type k,
                          std::vector<dof_id_type> & neighbors,
                          const bool corner);

  /**
   * The ID of the i,j,k node
   *
   * @param type The element type
   * @param nx The number of elements in the x direction
   * @param nx The number of elements in the y direction
   * @param nz The number of elements in the z direction
   * @param i The x index of this node
   * @param j The y index of this node
   * @param k The z index of this node
   */
  template <typename T>
  dof_id_type nodeId(const ElemType /*type*/,
                     const dof_id_type /*nx*/,
                     const dof_id_type /*ny*/,
                     const dof_id_type /*i*/,
                     const dof_id_type /*j*/,
                     const dof_id_type /*k*/)
  {
    mooseError(
        "nodeId not implemented for this element type in DistributedRectilinearMeshGenerator");
  }

  template <>
  dof_id_type nodeId<Edge2>(const ElemType type,
                            const dof_id_type nx,
                            const dof_id_type ny,
                            const dof_id_type i,
                            const dof_id_type j,
                            const dof_id_type k);

  template <>
  dof_id_type nodeId<Quad4>(const ElemType type,
                            const dof_id_type nx,
                            const dof_id_type ny,
                            const dof_id_type i,
                            const dof_id_type j,
                            const dof_id_type k);

  template <>
  dof_id_type nodeId<Hex8>(const ElemType type,
                           const dof_id_type nx,
                           const dof_id_type ny,
                           const dof_id_type i,
                           const dof_id_type j,
                           const dof_id_type k);

  /**
   * Add a node to the mesh
   *
   * @param nx The number of elements in the x direction
   * @param nx The number of elements in the y direction
   * @param nz The number of elements in the z direction
   * @param i The x index of this node
   * @param j The y index of this node
   * @param k The z index of this node
   * @param type The element type
   * @param mesh The mesh to add it to
   */
  template <typename T>
  Node * addPoint(const dof_id_type /*nx*/,
                  const dof_id_type /*ny*/,
                  const dof_id_type /*nz*/,
                  const dof_id_type /*i*/,
                  const dof_id_type /*j*/,
                  const dof_id_type /*k*/,
                  const ElemType /*type*/,
                  MeshBase & /*mesh*/)
  {
    mooseError(
        "addPoint not implemented for this element type in DistributedRectilinearMeshGenerator");
  }

  template <>
  Node * addPoint<Edge2>(const dof_id_type nx,
                         const dof_id_type ny,
                         const dof_id_type nz,
                         const dof_id_type i,
                         const dof_id_type j,
                         const dof_id_type k,
                         const ElemType type,
                         MeshBase & mesh);

  template <>
  Node * addPoint<Quad4>(const dof_id_type nx,
                         const dof_id_type ny,
                         const dof_id_type nz,
                         const dof_id_type i,
                         const dof_id_type j,
                         const dof_id_type k,
                         const ElemType type,
                         MeshBase & mesh);

  template <>
  Node * addPoint<Hex8>(const dof_id_type nx,
                        const dof_id_type ny,
                        const dof_id_type nz,
                        const dof_id_type i,
                        const dof_id_type j,
                        const dof_id_type k,
                        const ElemType type,
                        MeshBase & mesh);

  /**
   * Adds an element to the mesh
   *
   * @param nx The number of elements in the x direction
   * @param ny The number of elements in the y direction
   * @param nz The number of elements in the z direction
   * @param i The x index of this element
   * @param j The y index of this element
   * @param k The z index of this element
   * @param elem_id The element ID of the element to add
   * @param pid The processor ID to assign it to
   * @param type The type of element to add
   * @param mesh The mesh to add it to
   */
  template <typename T>
  void addElement(const dof_id_type /*nx*/,
                  const dof_id_type /*ny*/,
                  const dof_id_type /*nz*/,
                  const dof_id_type /*i*/,
                  const dof_id_type /*j*/,
                  const dof_id_type /*k*/,
                  const dof_id_type /*elem_id*/,
                  const processor_id_type /*pid*/,
                  const ElemType /*type*/,
                  MeshBase & /*mesh*/)
  {
    mooseError(
        "addElement not implemented for this element type in DistributedRectilinearMeshGenerator");
  }

  template <>
  void addElement<Edge2>(const dof_id_type nx,
                         const dof_id_type ny,
                         const dof_id_type nz,
                         const dof_id_type i,
                         const dof_id_type j,
                         const dof_id_type k,
                         const dof_id_type elem_id,
                         const processor_id_type pid,
                         const ElemType type,
                         MeshBase & mesh);

  template <>
  void addElement<Quad4>(const dof_id_type nx,
                         const dof_id_type ny,
                         const dof_id_type nz,
                         const dof_id_type i,
                         const dof_id_type j,
                         const dof_id_type k,
                         const dof_id_type elem_id,
                         const processor_id_type pid,
                         const ElemType type,
                         MeshBase & mesh);

  template <>
  void addElement<Hex8>(const dof_id_type nx,
                        const dof_id_type ny,
                        const dof_id_type nz,
                        const dof_id_type i,
                        const dof_id_type j,
                        const dof_id_type k,
                        const dof_id_type elem_id,
                        const processor_id_type pid,
                        const ElemType type,
                        MeshBase & mesh);

  /**
   * Compute the i,j,k indices of a given element ID
   *
   * @param nx The number of elements in the x direction
   * @param ny The number of elements in the y direction
   * @param elem_id The ID of the element
   * @param i Output: The index in the x direction
   * @param j Output: The index in the y direction
   * @param k Output: The index in the z direction
   */
  template <typename T>
  void getIndices(const dof_id_type /*nx*/,
                  const dof_id_type /*ny*/,
                  const dof_id_type /*elem_id*/,
                  dof_id_type & /*i*/,
                  dof_id_type & /*j*/,
                  dof_id_type & /*k*/)
  {
    mooseError(
        "getIndices not implemented for this element type in DistributedRectilinearMeshGenerator");
  }

  template <>
  void getIndices<Edge2>(const dof_id_type nx,
                         const dof_id_type ny,
                         const dof_id_type elem_id,
                         dof_id_type & i,
                         dof_id_type & j,
                         dof_id_type & k);

  template <>
  void getIndices<Quad4>(const dof_id_type nx,
                         const dof_id_type ny,
                         const dof_id_type elem_id,
                         dof_id_type & i,
                         dof_id_type & j,
                         dof_id_type & k);

  template <>
  void getIndices<Hex8>(const dof_id_type nx,
                        const dof_id_type ny,
                        const dof_id_type elem_id,
                        dof_id_type & i,
                        dof_id_type & j,
                        dof_id_type & k);

  /**
   * Find the elements and sides that need ghost elements
   *
   * @param nx The number of elements in the x direction
   * @param ny The number of elements in the y direction
   * @param mesh The mesh - without any ghost elements
   * @param ghost_elems The ghost elems that need to be added
   */
  template <typename T>
  void getGhostNeighbors(const dof_id_type /*nx*/,
                         const dof_id_type /*ny*/,
                         const dof_id_type /*nz*/,
                         const MeshBase & /*mesh*/,
                         std::set<dof_id_type> & /*ghost_elems*/)
  {
    mooseError("getGhostNeighbors not implemented for this element type in "
               "DistributedRectilinearMeshGenerator");
  }

  template <>
  void getGhostNeighbors<Edge2>(const dof_id_type nx,
                                const dof_id_type ny,
                                const dof_id_type nz,
                                const MeshBase & mesh,
                                std::set<dof_id_type> & ghost_elems);

  template <>
  void getGhostNeighbors<Quad4>(const dof_id_type nx,
                                const dof_id_type ny,
                                const dof_id_type nz,
                                const MeshBase & mesh,
                                std::set<dof_id_type> & ghost_elems);

  template <>
  void getGhostNeighbors<Hex8>(const dof_id_type nx,
                               const dof_id_type ny,
                               const dof_id_type nz,
                               const MeshBase & mesh,
                               std::set<dof_id_type> & ghost_elems);

  /**
   * Set the boundary names for any added boundary ideas
   *
   * @boundary_info The BoundaryInfo object to set the boundary names on
   */
  template <typename T>
  void setBoundaryNames(BoundaryInfo & /*boundary_info*/)
  {
    mooseError("setBoundaryNames not implemented for this element type in "
               "DistributedRectilinearMeshGenerator");
  }

  template <>
  void setBoundaryNames<Edge2>(BoundaryInfo & boundary_info);

  template <>
  void setBoundaryNames<Quad4>(BoundaryInfo & boundary_info);

  template <>
  void setBoundaryNames<Hex8>(BoundaryInfo & boundary_info);

  /**
   * All meshes are generated on the unit square.  This function stretches the mesh
   * out to fill the correct area.
   *
   * @param nx The number of elements in the x direction
   * @param ny The number of elements in the y direction
   * @param nz The number of elements in the z direction
   * @param xmin Lower X Coordinate of the generated mesh
   * @param xmax Upper X Coordinate of the generated mesh
   * @param ymin Lower Y Coordinate of the generated mesh
   * @param ymax Upper Y Coordinate of the generated mesh
   * @param zmin Lower Z Coordinate of the generated mesh
   * @param zmax Upper Z Coordinate of the generated mesh
   * @param mesh Distributed UnstructuredMesh
   */
  template <typename T>
  void scaleNodalPositions(dof_id_type /*nx*/,
                           dof_id_type /*ny*/,
                           dof_id_type /*nz*/,
                           Real /*xmin*/,
                           Real /*xmax*/,
                           Real /*ymin*/,
                           Real /*ymax*/,
                           Real /*zmin*/,
                           Real /*zmax*/,
                           MeshBase & /*mesh*/)
  {
    mooseError("scaleNodalPositions not implemented for this element type in "
               "DistributedRectilinearMeshGenerator");
  }

  template <>
  void scaleNodalPositions<Edge2>(dof_id_type nx,
                                  dof_id_type ny,
                                  dof_id_type nz,
                                  Real xmin,
                                  Real xmax,
                                  Real ymin,
                                  Real ymax,
                                  Real zmin,
                                  Real zmax,
                                  MeshBase & mesh);

  template <>
  void scaleNodalPositions<Quad4>(dof_id_type nx,
                                  dof_id_type ny,
                                  dof_id_type nz,
                                  Real xmin,
                                  Real xmax,
                                  Real ymin,
                                  Real ymax,
                                  Real zmin,
                                  Real zmax,
                                  MeshBase & mesh);

  template <>
  void scaleNodalPositions<Hex8>(dof_id_type nx,
                                 dof_id_type ny,
                                 dof_id_type nz,
                                 Real xmin,
                                 Real xmax,
                                 Real ymin,
                                 Real ymax,
                                 Real zmin,
                                 Real zmax,
                                 MeshBase & mesh);

protected:
  /// The dimension of the mesh
  MooseEnum _dim;

  /// Number of elements in x, y, z direction
  dof_id_type &_nx, &_ny, &_nz;

  /// The min/max values for x,y,z component
  Real &_xmin, &_xmax, &_ymin, &_ymax, &_zmin, &_zmax;

  /// The type of element to build
  ElemType _elem_type;

  /// The amount by which to bias the cells in the x,y,z directions.
  /// Must be in the range 0.5 <= _bias_x <= 2.0.
  /// _bias_x < 1 implies cells are shrinking in the x-direction.
  /// _bias_x==1 implies no bias (original mesh unchanged).
  /// _bias_x > 1 implies cells are growing in the x-direction.
  Real _bias_x, _bias_y, _bias_z;

  /// External partitioner
  std::string _part_package;

  /// Number of cores per compute node if hierarch partitioning is used
  dof_id_type _num_parts_per_compute_node;
};

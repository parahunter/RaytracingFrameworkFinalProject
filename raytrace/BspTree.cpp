// 02562 Rendering Framework
// Inspired by BSP tree in GEL (http://www.imm.dtu.dk/GEL/)
// BSP tree in GEL originally written by Bent Dalgaard Larsen.
// This file written by Jeppe Revall Frisvad, 2011
// Copyright (c) DTU Informatics 2011

#include <vector>
#include <optix_world.h>
#include "AccObj.h"
#include "Object3D.h"
#include "HitInfo.h"
#include "BspTree.h"

using namespace std;
using namespace optix;

namespace
{
  const float f_eps = 1.0e-6f;  
  const float d_eps = 1.0e-12f;
}

BspTree::~BspTree()
{
  delete_node(root);
}

void BspTree::init(const vector<Object3D*>& geometry, const std::vector<const Plane*>& scene_planes)
{
  root = new BspNode;
  Accelerator::init(geometry, scene_planes);
  for(unsigned int i = 0; i < geometry.size(); ++i)
    bbox.include(geometry[i]->compute_bbox());
  vector<AccObj*> objects = primitives;
  subdivide_node(*root, bbox, 0, objects);
}

bool BspTree::closest_hit(Ray& r, HitInfo& hit) const
{
  // Call closest_plane(...) and intersect_node(...) instead of
  // Accelerator::closest_hit(...) to use the BSP tree.
  return Accelerator::closest_hit(r, hit);
}

bool BspTree::any_hit(Ray& r, HitInfo& hit) const
{
  // Call any_plane(...) and intersect_node(...) instead of
  // Accelerator::any_hit(...) to use the BSP tree.
  return Accelerator::any_hit(r, hit);
}

void BspTree::subdivide_node(BspNode& node, Aabb& bbox, unsigned int level, vector<AccObj*>& objects) 
{
  // Use recursion with this function to build the BSP tree.
  //
  // Input:  node            (the node to be subdivided if it does not fulfil a stop criterion)
  //         bbox            (bounding box of the geometry to be stored in the node)
  //         level           (subdivision level of the node)
  //         objects         (array of pointers to primitive objects)
  //
  // Output: node.axis_leaf  (flag signalling if the node is a leaf or which axis it was split in, always set)
  //         node.plane      (displacement along the axis of the splitting plane, set if not leaf)
  //         node.left       (pointer to the left node in the next level of the tree, set if not leaf)
  //         node.right      (pointer to the right node in the next level of the tree, set if not leaf)
  //         node.id         (index pointing to the primitive objects associated with the node, set if leaf)
  //         node.count      (number of primitive objects associated with the node, set if leaf)
  //
  // Relevant data fields that are available (see BspTree.h)
  // max_objects             (maximum number of primitive objects in a leaf, stop criterion)
  // max_level               (maximum subdivision level, stop criterion)
  // tree_objects            (array for storing primitive objects associated with leaves)
  // 
  //
  // Hint: Finding a good way of positioning the splitting planes is hard.
  //       When positioning a plane, try a couple of places inside the bounding
  //       box along each axis and compute a cost for each tested placement.
  //       You can use the the number of primitives times the bounding box area
  //       as the cost of a node and take the sum of the left and right nodes
  //       to estimate the cost of a particular plane position. After all the
  //       tests, use the plane position with minimum cost.
}

bool BspTree::intersect_node(Ray& ray, HitInfo& hit, const BspNode& node) const 
{
  // Use recursion with this function to compute ray-scene intersection
  // using the BSP tree.
  //
  // Input:  ray       (ray to find the first intersection for)
  //         node      (node of the BSP tree to intersect with)
  //
  // Output: ray.tmin  (minimum distance to intersection after considering the node)
  //         ray.tmax  (maximum distance to intersection after considering the node)
  //         hit       (hit info retrieved from primitive intersection function)
  //
  // Relevant data fields that are available (see BspTree.h)
  // tree_objects      (array of primitive objects associated with leaves)
  // 
  //
  // Hint: Stop the recursion once a leaf node has been found and get
  //       access to the intersect function of a primitive object through
  //       the geometry field.

  return false;
}

void BspTree::delete_node(BspNode *node) 
{
  if(node)
  {
    if(node->left)
      delete_node(node->left);
    if(node->right)
      delete_node(node->right);
    delete node;
  }
}

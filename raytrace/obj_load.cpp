// 02562 Rendering Framework
// Code from GEL (http://www.imm.dtu.dk/GEL/)
// Inspired and partly lifted from Nate Robins' Obj loader.
// Modified to use CUDA/OptiX matrix-vector types.
// Copyright (c) DTU Informatics 2011

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <optix_world.h>
#include "TriMesh.h"
#include "ObjMaterial.h"

using namespace std;
using namespace optix;

namespace
{
	string get_path(const string& _filename)
	{
		// Make sure we only have slashes and not backslashes
		string filename = _filename;
		replace(filename.begin(),filename.end(),'\\','/');
		
		// Find the last occurrence of slash.
		// Everything before is path.
		unsigned int n = filename.rfind("/");
		if(n > filename.size())
			return "./";
		string pathname = "";
		pathname.assign(filename,0,n);
		pathname.append("/");
		return pathname;
	}
}

class TriMeshObjLoader
{
	TriMesh *mesh;
	std::string pathname;

	int get_vert(int i) {
		assert(i!=0);
		if (i<0) {
			return mesh->geometry.no_vertices()+i;
		} else
			return i-1;
	}

	int get_normal(int i) {
		if (i<0) {
			return mesh->normals.no_vertices()+i;
		} else
			return i-1;
	}

	int get_texcoord(int i) {
		if (i<0) {
			return mesh->texcoords.no_vertices()+i;
		} else
			return i-1;
	}

	void read_material_library(const string& filename, vector<ObjMaterial>& materials);

public:

	TriMeshObjLoader(TriMesh *_mesh): mesh(_mesh) {}
	
	void load(const std::string& filename);
  void load_material_library(const string& filename, vector<ObjMaterial>& materials)
  {
    pathname = get_path(filename);
    read_material_library(filename, materials);
  }
};

void TriMeshObjLoader::read_material_library(const string& filename, vector<ObjMaterial>& materials)
{
	string fn = pathname + filename;
	FILE* file = fopen(fn.data(), "r");
	if (!file) 
		{
			cerr << "Could not open " << filename << endl;
			return;
		}
		
	char  buf[128];
	unsigned int nummaterials=1;
		
	// count the number of materials in the file 
	while(fscanf(file, "%s", buf) != EOF) 
		{
			switch(buf[0]) 
				{
				case '#':				/* comment */
					/* eat up rest of line */
					fgets(buf, sizeof(buf), file);
					break;
				case 'n':				/* newmtl */
					fgets(buf, sizeof(buf), file);
					nummaterials++;
					sscanf(buf, "%s %s", buf, buf);
					break;
				default:
					/* eat up rest of line */
					fgets(buf, sizeof(buf), file);
					break;
				}
		}
	rewind(file);
	
	/* allocate memory for the materials */
	materials.resize(nummaterials);

	/* now, read in the data */
	nummaterials = 0;
	while(fscanf(file, "%s", buf) != EOF) {
		switch(buf[0]) {
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'n':				/* newmtl */
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s %s", buf, buf);
			nummaterials++;
			materials[nummaterials].name = buf;
			break;
		case 'N':
      switch(buf[1])
        {
        case 's':
			    fscanf(file, "%f", &materials[nummaterials].shininess);
          break;
        case 'i':
			    fscanf(file, "%f", &materials[nummaterials].ior);
          break;
				default:
					/* eat up rest of line */
					fgets(buf, sizeof(buf), file);
					break;
        }
			break;
		case 'K': 
			switch(buf[1]) 
				{
				case 'd':
					fscanf(file, "%f %f %f",
								 &materials[nummaterials].diffuse[0],
								 &materials[nummaterials].diffuse[1],
								 &materials[nummaterials].diffuse[2]);
					break;
				case 's':
					fscanf(file, "%f %f %f",
								 &materials[nummaterials].specular[0],
								 &materials[nummaterials].specular[1],
								 &materials[nummaterials].specular[2]);
					break;
				case 'a':
					fscanf(file, "%f %f %f",
								 &materials[nummaterials].ambient[0],
								 &materials[nummaterials].ambient[1],
								 &materials[nummaterials].ambient[2]);
					break;
				default:
					/* eat up rest of line */
					fgets(buf, sizeof(buf), file);
					break;
				}
			break;
    case 'T':
			fscanf(file, "%f %f %f",
             &materials[nummaterials].transmission[0],
             &materials[nummaterials].transmission[1],
             &materials[nummaterials].transmission[2]);
      break;
    case 'i':
			fscanf(file, "%d", &materials[nummaterials].illum);
      break;
		case 'm': // Map ... all maps are treated equally.
			{
				fscanf(file,"%s",buf);
				materials[nummaterials].tex_path = pathname;
				materials[nummaterials].tex_name = string(buf);
        materials[nummaterials].has_texture = true;
			}
			break;
		default:
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
  fclose(file);
}


void TriMeshObjLoader::load(const std::string& filename) 
{
	pathname = get_path(filename);
	FILE *fp = fopen(filename.data(), "r");
	if (fp==0) {
		cerr << "File " << filename << " does not exist" << endl;
    exit(0);
	}
	mesh->materials.resize(1);
	
	char buf[256];
	float3 v_geo;
	float3 v_normals;
	float3 v_texcoords;

	uint3 f_geo;
	uint3 f_normals;
	uint3 f_texcoords;
	int current_material=0;
	int v, n, t;
	while(fscanf(fp, "%s", buf) != EOF) 
		{
			switch(buf[0]) 
				{
				case '#': // A comment
					fgets(buf, sizeof(buf), fp);
					break;
				case 'm':
					fgets(buf, sizeof(buf), fp);
					sscanf(buf, "%s %s", buf, buf);
					read_material_library(buf, mesh->materials);
					break;
				case 'u':
					fgets(buf, sizeof(buf), fp);
					sscanf(buf, "%s %s", buf, buf);
					current_material = mesh->find_material(buf);
					break;
				case 'v': // v, vn, vt
					switch(buf[1]) 
						{
						case '\0': // vertex
							fscanf(fp, "%f %f %f", &v_geo.x, &v_geo.y, &v_geo.z);
							mesh->geometry.add_vertex(v_geo);
							break;
						case 'n': // normal
							fscanf(fp, "%f %f %f", &v_normals.x, &v_normals.y, &v_normals.z);
							mesh->normals.add_vertex(v_normals);
							break;
						case 't': // texcoord
							fscanf(fp, "%f %f", &v_texcoords.x, &v_texcoords.y);
							v_texcoords.z=1;
							mesh->texcoords.add_vertex(v_texcoords);
							break;
						}
					break;
				case 'f':
					v = n = t = 0;
					fscanf(fp, "%s", buf);
					// can be one of %d, %d//%d, %d/%d, %d/%d/%d 
					if(sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3)
						{ // v/t/n
							
							f_geo.x = get_vert(v); 
							f_texcoords.x = get_texcoord(t);
							f_normals.x = get_normal(n);

							fscanf(fp, "%d/%d/%d", &v, &t, &n); 
							f_geo.y = get_vert(v); 
							f_texcoords.y = get_texcoord(t);
							f_normals.y = get_normal(n);

							fscanf(fp, "%d/%d/%d", &v, &t, &n); 
							f_geo.z = get_vert(v); 
							f_texcoords.z = get_texcoord(t);
							f_normals.z = get_normal(n);

							int idx = mesh->geometry.add_face(f_geo);
							mesh->normals.add_face(f_normals, idx);
							mesh->texcoords.add_face(f_texcoords, idx);
							mesh->mat_idx.push_back(current_material);

							// Load a general polygon and convert to triangles
							while(fscanf(fp, "%d/%d/%d", &v, &t, &n) == 3) 
								{
									f_geo.y = f_geo.z;
									f_normals.y = f_normals.z;
									f_texcoords.y = f_texcoords.z;

									f_geo.z = get_vert(v);
									f_normals.z = get_normal(n);
									f_texcoords.z = get_texcoord(t);

									int idx = mesh->geometry.add_face(f_geo);
									mesh->normals.add_face(f_normals, idx);
									mesh->texcoords.add_face(f_texcoords, idx);
									mesh->mat_idx.push_back(current_material);
								}
						} 
					else if (sscanf(buf, "%d//%d", &v, &n) == 2)
						{// v//n 
							f_geo.x = get_vert(v);
							f_normals.x = get_normal(n);

							fscanf(fp, "%d//%d", &v, &n); 
							f_geo.y = get_vert(v);
							f_normals.y = get_normal(n);
							
							fscanf(fp, "%d//%d", &v, &n); 
							f_geo.z = get_vert(v);
							f_normals.z = get_normal(n);
							
							int idx = mesh->geometry.add_face(f_geo);
							mesh->normals.add_face(f_normals, idx);
							mesh->mat_idx.push_back(current_material);
							
							// Load a general polygon and convert to triangles
							while(fscanf(fp, "%d//%d", &v, &n) == 2) 
								{
									f_geo.y = f_geo.z;
									f_normals.y = f_normals.z;
									f_geo.z = get_vert(v);
									int idx = mesh->geometry.add_face(f_geo);
									mesh->normals.add_face(f_normals, idx);
									mesh->mat_idx.push_back(current_material);
								}
						} 
					else if (sscanf(buf, "%d/%d", &v, &t) == 2)
						{ // v/t 
							f_geo.x = get_vert(v);
							f_texcoords.x = get_texcoord(t);

							fscanf(fp, "%d/%d", &v, &t); 
							f_geo.y = get_vert(v);
							f_texcoords.y = get_texcoord(t);

							fscanf(fp, "%d/%d", &v, &t); 
							f_geo.z = get_vert(v);
							f_texcoords.z = get_texcoord(t);

							int idx = mesh->geometry.add_face(f_geo);
							mesh->texcoords.add_face(f_texcoords, idx);
							mesh->mat_idx.push_back(current_material);
					
							// Load a general polygon and convert to triangles
							while(fscanf(fp, "%d/%d", &v, &t)==2) 
								{
									f_geo.y = f_geo.z;
									f_texcoords.y = f_texcoords.z;
							
									f_geo.z = get_vert(v);
									f_texcoords.z = get_texcoord(t);

									int idx = mesh->geometry.add_face(f_geo);
									mesh->texcoords.add_face(f_texcoords, idx);
									mesh->mat_idx.push_back(current_material);
								}
						} 
					else if (sscanf(buf, "%d", &v)==1)
						{ // v 
							f_geo.x = get_vert(v);
							
							fscanf(fp, "%d", &v);
							f_geo.y = get_vert(v);
							
							fscanf(fp, "%d", &v);
							f_geo.z = get_vert(v);
							
							mesh->geometry.add_face(f_geo);
							mesh->mat_idx.push_back(current_material);

							// Load a general polygon and convert to triangles
							while(fscanf(fp, "%d", &v)==1) 
								{
									f_geo.y = f_geo.z;
									f_geo.z = get_vert(v);
									mesh->geometry.add_face(f_geo);
									mesh->mat_idx.push_back(current_material);
								}
						}
					break;
				default:
					fgets(buf, sizeof(buf), fp);
					break;
				}
		}
  fclose(fp);
}


void obj_load(const string& filename, TriMesh& mesh)
{
	TriMeshObjLoader loader(&mesh);
	loader.load(filename);
}

void mtl_load(const string& filename, vector<ObjMaterial>& materials)
{
  TriMeshObjLoader loader(0);
  loader.load_material_library(filename, materials);
}

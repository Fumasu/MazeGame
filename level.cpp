#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>

struct TriangleData
{
	unsigned int textureID;
	unsigned int indices[3];
	glm::vec2 uv[3];
	glm::vec3 normal;
};

bool loadFromFile (const std::string filename, std::string& levelTitle, std::vector<std::string>& textures, std::vector<glm::vec3>& vertices, std::vector<TriangleData>& triangles)
{
	std::ifstream Datei;
	std::string line;
	
	Datei.open (filename);
	
	if (!Datei.is_open())
	{
		std::cerr <<"ERROR::LEVEL::File not found\n";
		
		return false;
	}
	
	std::getline (Datei, line);
	if (line.compare ("FLF") !=0)
	{
		std::cerr <<"ERROR::LEVEL::Not a Valid Level File\n";
		
		return false;
	}
	
	std::getline (Datei, line);
	levelTitle =line;
	
	while (!Datei.eof())
	{
		std::getline (Datei, line);
		switch (line[0])
		{
			case 'T':
			{
				line.erase (0, 2);
				textures.push_back (line);
				break;
			}
			case 'V':
			{
				float x, y, z;
				line.erase (0, 2);
				std::stringstream sstr;

				sstr <<line;
				sstr >>x >>y >>z;
				vertices.push_back (glm::vec3(x, y, z));
				break;
			}
			case 'M':
			{
				unsigned int t, i1, i2, i3;
				float ux[3], uy[3], nx, ny, nz;
				TriangleData tmp;
				
				line.erase (0, 2);
				std::stringstream sstr;
				
				sstr <<line;
				sstr >>t >>i1 >>i2 >>i3 >>ux[0] >>uy[0] >>ux[1] >>uy[1] >>ux[2] >>uy[2] >>nx >>ny >>nz;
				tmp.textureID =t;
				tmp.indices[0] =i1;
				tmp.indices[1] =i2;
				tmp.indices[2] =i3;
				tmp.uv[0] =glm::vec2 (ux[0], uy[0]);
				tmp.uv[1] =glm::vec2 (ux[1], uy[1]);
				tmp.uv[2] =glm::vec2 (ux[2], uy[2]);
				tmp.normal =glm::vec3 (nx, ny, nz);
				triangles.push_back (tmp);
				break;
			}
		}
	}
	
	Datei.close();
	
	return true;
}

int main()
{
	std::string title;
	std::vector<std::string> textures;
	std::vector<glm::vec3> vertices;
	std::vector<TriangleData> triangles;
	
	if (!loadFromFile ("res/Maps/quad.flf", title, textures, vertices, triangles))
	{
		std::cout <<"Level nicht geladen\n";
		
		return EXIT_FAILURE;
	}
	
	std::cout <<"Level Title: " <<title <<"\n";
	std::cout <<"NumTextures: " <<textures.size() <<"\n";
	for (auto& t: textures)
		std::cout <<t <<"\n";
	std::cout <<"NumVertices: " <<vertices.size() <<"\n";
	for (auto& v: vertices)
		std::cout <<v.x <<" " <<v.y <<" " <<v.z <<"\n";
	std::cout <<"NumTriangles: " <<triangles.size() <<"\n";
	for (auto& t: triangles)
		std::cout <<t.textureID <<" " <<t.indices[0] <<" " <<t.indices[1] <<" " <<t.indices[2] <<" " <<t.uv[0].x <<" " <<t.uv[0].y <<" " <<t.uv[1].x <<" " <<t.uv[1].y <<" " <<t.uv[2].x <<" " <<t.uv[2].y <<" " <<t.normal.x <<" " <<t.normal.y <<" " <<t.normal.z <<"\n";
	
	return EXIT_SUCCESS;
}
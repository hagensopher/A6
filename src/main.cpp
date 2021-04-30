#include <cassert>
#include <cstring>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Object.h"
#include "Camera.h"
#include "Image.h"

//#include "MatrixStack.h"

#include "Light.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;
glm::vec3 ray[2];




vector<Hit> hits;
vector<Object> objects;

double RANDOM_COLORS[7][3] = {
	{0.0000,    0.4470,    0.7410},
	{0.8500,    0.3250,    0.0980},
	{0.9290,    0.6940,    0.1250},
	{0.4940,    0.1840,    0.5560},
	{0.4660,    0.6740,    0.1880},
	{0.3010,    0.7450,    0.9330},
	{0.6350,    0.0780,    0.1840},
};

void RayGenerator(float imageSize,Camera camera,vector<glm::vec3> &midpoints) {
	//this function should generate all the rays for a given image size
	//3x3 image would have 9 rays
	//5x5 image would have 25 rays etc
	float virtualPlane = 4;
	float planeDifference = camera.z - virtualPlane; //camrea 1 it is 5-4 =1
	float theta = ((camera.fov / 2)*M_PI)/180; //for camrea 1 it is 45/2
	
	float length = tan(theta) * planeDifference; //should give the hieght of height
	//cout << "lenth is " << length << endl;
	float yLength = length;
	for (int i = 0; i < imageSize; i++) {
		
		float xLength = (length * -1);
		for (int j = 0; j < imageSize; j++) {
			glm::vec3 midpoint;
			midpoint.x = xLength + (length / imageSize);
			midpoint.y = yLength - (length / imageSize);
			midpoint.z = -1;
			
			midpoint = glm::normalize(midpoint);
			
			midpoints.push_back(midpoint);
			xLength = xLength + ((2 * length) / imageSize);
			//cout << "new xlenth is" << xLength << endl;
		}
		//cout << "the new ylength" << yLength << endl;
		yLength = yLength - ((2 * length) / imageSize);
		
	}
}

Hit Hitter(Sphere object,glm::vec3 rayDir, glm::vec3 rayOri) {
	hits.clear();
	assert(hits.size() == 0);
	//cout << "the size of the hits vector is " << hits.size() << endl;
	//cout << "the ray direction is " << rayDir.x << ',' << rayDir.y << ','<<rayDir.z << endl;
	//cout << "the ray origion is " << rayOri.x << ',' << rayOri.y << ',' << rayOri.z << endl;
	for (int i = 0; i < objects.size(); i++) {
		object.SphereIntersection(objects[i], hits, rayDir,rayOri);
		
		
	}
	Hit smallestT;
	for (int k = 0; k < hits.size(); k++) {
		if (k == 0) {
			if (hits[k].t <= 0) {
				continue;
			}
			else {
				smallestT = hits[k];
				smallestT.hasHit = true;
			}
		}
		else {
			if (hits[k].t <= 0) {
				continue;
			}
			else if (hits[k].t <= smallestT.t) {
				smallestT = hits[k];
				smallestT.hasHit = true;
			}
		}
	}
	return smallestT;
	//cout << "the amount of hits found " << hits.size() << endl;
	
}

#include <iomanip> 
glm::vec3 ColorCalculator(Sphere object,glm::vec3 rayDir,glm::vec3 rayOri,vector<Light> lights,Camera camera) {
	//this here is the basic blinnphong shader from assigment 3/lab7
	Hit hit = Hitter(object,rayDir,rayOri);
	//cout << "the hit direciton is " << rayDir.x << ',' << rayDir.y << ',' << rayDir.z << ',' << endl;
	//cout << "the hit location: " << hit.x.x << ',' << hit.x.y << ',' << hit.x.z << ',' << endl;
	//cout << "the distance of the hit is " << hit.t << endl;
	glm::vec3 color;
	//glm::vec3 n = glm::normalize(hit.n);
	//cout << "temp 123132" <<  endl;
	//glm::vec3 cA = object.kA;
	if (hit.hasHit) {
		color = hit.kA;
		for (int i = 0; i < lights.size(); i++) {
			glm::vec3 lightNormalized = glm::normalize(lights[i].pos - hit.x);
			glm::vec3 eyeNormalized = glm::normalize(glm::vec3(0, 0, 5) - hit.x);
			glm::vec3 halfVector = glm::normalize(eyeNormalized + lightNormalized);

			Hit hit2;
			//cout << "start of shadow" << endl;
			//cout << "temp "<<hit2.hasHit << endl;
			//need an epsilion value to be slightly off the suface
			//hit2 = Hitter(object, lightNormalized, (hit.x + glm::vec3(.9,.9,.9))); 
			glm::vec3 temp = (hit.x + (glm::vec3(.001, .001, .001) * lightNormalized));
			//hits.clear();
			hit2 = Hitter(object, lightNormalized, hit.x);
			/*cout << "the new hit location: " << hit2.x.x << ',' << hit2.x.y << ',' << hit2.x.z << ',' << endl;
			std::cout << std::setprecision(16) << hit2.x.z << '\n';
			cout << "the new hit direction: " << lightNormalized.x << ',' << lightNormalized.y << ',' << lightNormalized .z << ',' << endl;
			cout << "the distance of the new hit is " << hit2.t << endl;*/
			//cout << "the shoadow hit location: "<<hit2.x.x <<','<<hit2.x.y << ','<< hit2.x.z << ',' << endl;

		
			if (!hit2.hasHit) {
				glm::vec3 cD = hit.kD * max(0.0f, glm::dot(lightNormalized, hit.n));
				glm::vec3 cS = hit.kS * pow(max(0.0f, glm::dot(halfVector, hit.n)), hit.shine);
				color += lights[i].color * (cD + cS);
			}
			
			//WORKING SCENE 1 Code
			/*
				glm::vec3 cD = hit.kD * max(0.0f, glm::dot(lightNormalized, n));
				glm::vec3 cS = hit.kS * pow(max(0.0f, glm::dot(halfVector, n)), hit.shine);
				color += lights[i].color * (cD + cS);*/
			//END OF WORKING SECNE 1 CODE
		}
		if (color.r > 1) {
			color.r = 1.0f;
		}
		if (color.g > 1) {
			color.g = 1.0f;
		}
		if (color.b > 1) {
			color.b = 1.0f;
		}
		return color;
	}
	else {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
}



int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Usage: A1 meshfile" << endl;
		return 0;
	}

	int imageSize = atoi(argv[2]);
	int scene = atoi(argv[1]);
	vector<glm::vec3> rayDir;
	Camera camera;
	camera.x = 0;
	camera.y = 0;
	camera.z = 5;
	camera.fov = 45.0;
	vector<Light> lights;
	RayGenerator(imageSize, camera, rayDir);
	//if (scene == 1 || scene == 2) {
		
		//make the light for scene 1
		Light light1;
		//light1.pos = glm::vec3(0.0f, 0.0f, 6.0f);
		light1.pos = glm::vec3(-2.0f, 1.0f, 1.0f);
		light1.color = glm::vec3(1.0f, 1.0f, 1.0f);
		lights.push_back(light1);
		//make the first red sphere with test case of
		Sphere sphere1;
		//sphere1.pos = glm::vec3(0.0f, 0.0f, 0.0f);
		sphere1.pos = glm::vec3(-.5f, -1.0f, 1.0f);
		sphere1.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		sphere1.kA = glm::vec3(.1f, .1f, .1f);
		sphere1.kD = glm::vec3(1.0f, 0.0f, 0.0f);
		sphere1.kS = glm::vec3(1.0f, 1.0f, .5f);
		sphere1.shine = 100.0;
		objects.push_back(sphere1);
		Sphere sphere2;
		sphere2.pos = glm::vec3(0.5f, -1.0f, -1.0f);
		sphere2.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		sphere2.kA = glm::vec3(.1f, .1f, .1f);
		sphere2.kD = glm::vec3(0.0f, 1.0f, 0.0f);
		sphere2.kS = glm::vec3(1.0f, 1.0f, .5f);
		sphere2.shine = 100.0;
		objects.push_back(sphere2);
		Sphere sphere3;
		sphere3.pos = glm::vec3(0.0f, 1.0f, 0.0f);
		sphere3.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		sphere3.kA = glm::vec3(.1f, .1f, .1f);
		sphere3.kD = glm::vec3(0.0f, 0.0f, 1.0f);
		sphere3.kS = glm::vec3(1.0f, 1.0f, .5f);
		sphere3.shine = 100.0;
		objects.push_back(sphere3);
	//}
	
	Image image(imageSize, imageSize);
	glm::vec3 pixelColor = glm::vec3(0, 0, 0);
	for (int i = 0; i<imageSize; i++) { //loop through each pixel
		for (int j = 0; j < imageSize; j++) {
			
				//calculate blinnphone color 
				pixelColor = ColorCalculator(sphere1,rayDir[(i * imageSize) + j],glm::vec3(0.0f,0.0f,5.0f) ,lights, camera);
				image.setPixel(j, imageSize-i, pixelColor.r * 255, pixelColor.g * 255, 255 * pixelColor.b); //this uses 0-255 NOT 0-1  

				
		}
	}
	image.writeToFile("output.png");

	//// Load geometry
	//vector<float> posBuf; // list of vertex positions
	//vector<float> norBuf; // list of vertex normals
	//vector<float> texBuf; // list of vertex texture coords

	//
	//tinyobj::attrib_t attrib;
	//std::vector<tinyobj::shape_t> shapes;
	//std::vector<tinyobj::material_t> materials;
	//string errStr;
	//bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	//if(!rc) {
	//	cerr << errStr << endl;
	//} else {
	//	// Some OBJ files have different indices for vertex positions, normals,
	//	// and texture coordinates. For example, a cube corner vertex may have
	//	// three different normals. Here, we are going to duplicate all such
	//	// vertices.
	//	// Loop over shapes
	//	for(size_t s = 0; s < shapes.size(); s++) {
	//		// Loop over faces (polygons)
	//		size_t index_offset = 0;
	//		for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
	//			size_t fv = shapes[s].mesh.num_face_vertices[f];
	//			// Loop over vertices in the face.
	//			for(size_t v = 0; v < fv; v++) {
	//				// access to vertex
	//				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
	//				posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
	//				posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
	//				posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
	//				if(!attrib.normals.empty()) {
	//					norBuf.push_back(attrib.normals[3*idx.normal_index+0]);
	//					norBuf.push_back(attrib.normals[3*idx.normal_index+1]);
	//					norBuf.push_back(attrib.normals[3*idx.normal_index+2]);
	//				}
	//				if(!attrib.texcoords.empty()) {
	//					texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+0]);
	//					texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+1]);
	//				}
	//			}
	//			index_offset += fv;
	//			// per-face material (IGNORE)
	//			shapes[s].mesh.material_ids[f];
	//		}
	//	}
	//}
	//cout << "Number of vertices: " << posBuf.size()/3 << endl;
	
	return 0;
}

// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "RayTracing.h"
#include "ReadInputFile.h"
#include "SphereIntersection.h"
#include <algorithm>

ReadInputFile readInputFile("test2.in");
RayTracing tracer;
SphereIntersection sphereIntersection;
vec3 backgroundColor = vec3(0.5, 0.5, 0.5);
LightSource ambient;
vector<LightSource> lightSources;
vec3 ambientIntensity;
vector<Surface> surfaces;
vector<Pigment> pigments;
Camera camera;
int main()
{
	int columns = readInputFile.getWidth();
	int rows = readInputFile.getHeight();
	lightSources = readInputFile.getLightSources();
	ambient = lightSources[0];
	ambientIntensity = vec3(ambient.Ir, ambient.Ig, ambient.Ib);
	surfaces = readInputFile.getSurfaces();
	pigments = readInputFile.getPigments();
	camera = readInputFile.getCamera();

	vec3** arr = new vec3*[rows];
	for (int i = 0; i < rows; i++) {
		arr[i] = new vec3[columns];
	}
	float h = 2 * tan(camera.fovy / 2);
	float a = columns / rows;
	float w = h * a;
	vec3 cz = -normalize(camera.at - camera.eye);
	vec3 cx = normalize(cross(camera.up, cz));
	vec3 cy = cross(cz, cx);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			float px = w * j / columns - w / 2;
			float py = -h * i / rows + h / 2;
			vec3 pixel = camera.eye + px * cx + py * cy + (-1.0)*cz;
			Ray ray(camera.eye, pixel);
			vec3 color = tracer.trace(ray);
			arr[i][j] = color;
		}
	}
	tracer.outputToPPM(arr);
}

vec3 RayTracing::trace(Ray &ray) {
	vector<SceneObj> sceneObjects = readInputFile.getSceneObjs();
	vector<float> tValues;
	vector<bool> inOut;
	vec3 localC = vec3(0, 0, 0);
	for (int i = 0; i < sceneObjects.size(); i++) {
		SceneObj sceneObj = sceneObjects[i];
		bool isInside = false;
		float t = sphereIntersection.intersect(sceneObj.center, sceneObj.radius, ray,isInside);
		tValues.push_back(t);
		inOut.push_back(isInside);
	}
	bool temp = false;
	for (int i = 0; i < tValues.size(); i++) {
		if (tValues[i] >= 0) {
			temp = true;
			break;
		}
	}
	if (!temp) {
		return backgroundColor;
	}
	else {
		int whichObj = 0;
		float min = FLT_MAX;
		for (int i = 0; i < tValues.size(); i++) {
			if (tValues[i] >= 0 && tValues[i] <= min) {
				min = tValues[i];
				whichObj = i;
			}
		}
		vec3 P = sphereIntersection.intersectionPoint(ray, min);
		//TODO check backward normals
		vec3 normal;

		
		
		if(inOut[whichObj]) 
			normal = -normalize(P - sceneObjects[whichObj].center);
		else {
			normal = normalize(P - sceneObjects[whichObj].center);
		}
		Surface surface = surfaces[sceneObjects[whichObj].surfaceNum];
		vec3 Ia = ambientIntensity * surface.ka;
		Pigment pigment =pigments[sceneObjects[whichObj].pigmentNum];
		vec3 pigmentColor = vec3(pigment.r, pigment.g, pigment.b);
		localC = Ia * pigmentColor;

		for (int i = 1; i < lightSources.size(); i++) {
			if (isVisible(P, lightSources[i], sceneObjects)) {
				localC += phong(P, lightSources[i], normal, sceneObjects[whichObj]);
			}
		}
	}
	return localC;
}

bool RayTracing::isVisible(vec3 point, LightSource lightSource, vector<SceneObj> sceneObjects) {

	vec3 dest = lightSource.LightPos;
	Ray ray(dest, point);
	for (int i = 0; i < sceneObjects.size(); i++) {
		SceneObj sceneObj = sceneObjects[i];
		bool isInside;
		float t = sphereIntersection.intersect(sceneObj.center, sceneObj.radius, ray, isInside);
		vec3 newPoint = sphereIntersection.intersectionPoint(ray, t);
		if (t > 0 && newPoint.x == point.x && newPoint.y == point.y && newPoint.z == point.z) {
			return true;
		}
		else if (t < 0 && newPoint.x == point.x && newPoint.y == point.y && newPoint.z == point.z) {
			return false;
		}
	}
	return true;
}

vec3 RayTracing::phong(vec3 P, LightSource lightSource, vec3 normal, SceneObj sceneObj) {

	Pigment pigment = pigments[sceneObj.pigmentNum];
	vec3 pigmentColor = vec3(pigment.r, pigment.g, pigment.b);
	Surface surface = surfaces[sceneObj.surfaceNum];

	vec3 lightIntensity = vec3(lightSource.Ir, lightSource.Ig, lightSource.Ib);
	float d = sqrt(dot(lightSource.LightPos - P, lightSource.LightPos - P));
	float attenuation = lightSource.a + d * lightSource.b + d * d*lightSource.c;
	vec3 l = normalize(lightSource.LightPos - P);
	vec3 v = normalize(camera.eye - P);
	vec3 h = normalize(l + v);
	vec3 Id = max((double)dot(l, normal), 0.0);
	vec3 Is = pow(max((double)dot(normal, h), 0.0), surface.shineness);

	return (lightIntensity / attenuation) * (Id*surface.kd*pigmentColor + Is * surface.ks);
}

void RayTracing::outputToPPM(vec3** arr) {
	int width = readInputFile.getWidth();
	int height = readInputFile.getHeight();
	int i, j;
	FILE *fp = fopen("first.ppm", "wb"); /* b - binary mode */
	fprintf(fp, "P6\n%d %d\n255\n", width, height);
	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			unsigned char color[3];
			color[0] = (unsigned char)(arr[j][i].x >= 1.0 ? 255 : (arr[j][i].x <= 0.0 ? 0 : (int)floor(arr[j][i].x  * 256.0)));  /* red */
			color[1] = (unsigned char)(arr[j][i].y >= 1.0 ? 255 : (arr[j][i].y <= 0.0 ? 0 : (int)floor(arr[j][i].y  * 256.0)));   /* green */
			color[2] = (unsigned char)(arr[j][i].z >= 1.0 ? 255 : (arr[j][i].z <= 0.0 ? 0 : (int)floor(arr[j][i].z  * 256.0)));  /* blue */
			(void)fwrite(color, 1, 3, fp);
		}
	}
	(void)fclose(fp);
}

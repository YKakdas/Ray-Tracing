// RayTracing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "RayTracing.h"
#include "ReadInputFile.h"
#include "SphereIntersection.h"
#include <algorithm>

ReadInputFile readInputFile("test1.in");
RayTracing tracer;
SphereIntersection sphereIntersection;
vec3 backgroundColor = vec3(0.5, 0.5, 0.5);
int main()
{
	int columns = readInputFile.getWidth();
	int rows = readInputFile.getHeight();
	Camera camera = readInputFile.getCamera();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			vec3 pixel = vec3(i, j, 0);
			Ray ray(camera.eye, pixel);
			vec3 color = tracer.trace(ray);
			writePixel(i, j, color);
		}
	}
	
	
}

vec3 RayTracing::trace(Ray &ray) {
	vector<SceneObj> sceneObjects = readInputFile.getSceneObjs();
	vector<float> tValues;
	vec3 localC = vec3(0, 0, 0);
	for (int i = 0; i < sceneObjects.size(); i++) {
		SceneObj sceneObj = sceneObjects[i];
		float t = sphereIntersection.intersect(sceneObj.center, sceneObj.radius, ray);
		tValues.push_back(t);
	}
	if (tValues.size() == 0) {
		return backgroundColor;
	}
	else {
		int whichObj = 0;
		float min = tValues[0];
		for (int i = 0; i < tValues.size(); i++) {
			if (tValues[i] >= 0 && tValues[i] < min) {
				min = tValues[i];
				whichObj = i;
			}
		}
		vec3 P = sphereIntersection.intersectionPoint(ray, min);
		//TODO normal tekrar bak eksi icin
		vec3 normal = normalize(P - sceneObjects[whichObj].center);
		vector<LightSource> lightSources = readInputFile.getLightSources();
		for (int i = 0; i < lightSources.size(); i++) {
			if (isVisible(P, lightSources[i])) {
				localC += phong(P, lightSources[i], normal, sceneObjects[whichObj]);
			}
		}
	}
	return localC;
}

bool RayTracing::isVisible(vec3 point, LightSource lightSource) {
	vector<SceneObj> sceneObjects = readInputFile.getSceneObjs();
	vec3 dest = lightSource.LightPos;
	Ray ray(point, dest);
	for (int i = 0; i < sceneObjects.size(); i++) {
		SceneObj sceneObj = sceneObjects[i];
		float t = sphereIntersection.intersect(sceneObj.center, sceneObj.radius, ray);
		if (t >= 0) {
			return false;
		}
	}
	return true;
}

vec3 RayTracing::phong(vec3 P, LightSource lightSource, vec3 normal, SceneObj sceneObj) {
	vector<LightSource> lightSources = readInputFile.getLightSources();
	LightSource ambient = lightSources[0];
	vec3 ambientIntensity = vec3(ambient.Ir, ambient.Ig, ambient.Ib);
	Pigment pigment = readInputFile.getPigments()[sceneObj.pigmentNum];
	Surface surface = readInputFile.getSurfaces()[sceneObj.surfaceNum];
	vec3 Ia = ambientIntensity * surface.ka;

	vec3 lightIntensity = vec3(lightSource.Ir, lightSource.Ig, lightSource.Ib);
	float d = dot(lightSource.LightPos - P, lightSource.LightPos - P);
	vec3 attenuation = lightSource.a + d * lightSource.b + d * d*lightSource.c;
	vec3 l = normalize(lightSource.LightPos - P);
	vec3 h = normalize(readInputFile.getCamera().eye - P + l);
	vec3 Id = max((double)dot(l, normal), 0.0);
	vec3 Is = pow(max((double)dot(normal, h), 0.0), surface.shineness);

	return Ia + attenuation * (Id*surface.kd + Is * surface.ks);
}

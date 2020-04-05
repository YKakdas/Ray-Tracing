#include <iostream>
#include "RayTracing.h"
#include "ReadInputFile.h"
#include <algorithm>

ReadInputFile readInputFile;
RayTracing tracer;
vec3 backgroundColor = vec3(0.5, 0.5, 0.5);
LightSource ambient;
vector<LightSource> lightSources;
vec3 ambientIntensity;
vector<Surface> surfaces;
vector<Pigment> pigments;
vector<SceneObj> sceneObjects;
int NumberOfObjects = 0;
Camera camera;
int main(int argc, char** argv)
{
	readInputFile.fillObjectsFromFile(argv[1]);
	int columns = readInputFile.getWidth();
	int rows = readInputFile.getHeight();
	lightSources = readInputFile.getLightSources();
	ambient = lightSources[0];
	ambientIntensity = vec3(ambient.Ir, ambient.Ig, ambient.Ib);
	surfaces = readInputFile.getSurfaces();
	pigments = readInputFile.getPigments();
	camera = readInputFile.getCamera();
	sceneObjects = readInputFile.getSceneObjs();
	NumberOfObjects = sceneObjects.size();
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
			vec3 pixel = camera.eye + px * cx + py * cy + (-1.35)*cz;
			vec3 direction = normalize(pixel - camera.eye);
			ray r = { camera.eye,pixel,direction };
			vec3 color = tracer.trace(r, 0);
			arr[i][j] = color;
		}
	}
	tracer.outputToPPM(arr);
}

vec3 RayTracing::trace(ray r, int depth) {
	if (depth > 4) return backgroundColor;
	int whichObj = 0;
	bool isPositive = false;
	bool isInside = false;
	vec3 localC = vec3(0, 0, 0);
	vec3 reflectedC = vec3(0, 0, 0);
	Surface surface;
	SceneObj sceneObj;
	float min = FLT_MAX;
	for (int i = 0; i < NumberOfObjects; i++) {
		sceneObj = sceneObjects[i];
		bool temp;
		float t = intersect(sceneObj.center, sceneObj.radius, r, temp);
		if (t > 0.001) {
			isPositive = true;
		}
		if (t > 0.001 && t - min < 0.001) {
			min = t;
			isPositive = true;
			isInside = temp;
			whichObj = i;
		}
	}

	if (!isPositive) {
		return backgroundColor;
	}
	else {

		sceneObj = sceneObjects[whichObj];
		surface = surfaces[sceneObj.surfaceNum];
		vec3 normal;
		vec3 P = r.origin + min * r.direction;
		if (isInside)
			normal = -normalize(P - sceneObj.center);
		else {
			normal = normalize(P - sceneObj.center);
		}
		localC += phong(P, normal, sceneObj);
		if (surface.kr > 0.001) {
			vec3 direction = 2 * dot(normal, (-normalize(r.direction)))*normal + normalize(r.direction);
			vec3 dest = P + direction;
			ray reflected = { P,dest,direction };
			reflectedC = trace(reflected, depth + 1);
		}
	}
	return localC + surface.kr*reflectedC;
}

bool RayTracing::isVisible(vec3 point, LightSource lightSource, vector<SceneObj> sceneObjects) {

	vec3 dest = lightSource.LightPos;
	vec3 direction = normalize(point - dest);
	ray r = { dest, point,direction };
	float esp = 0.001;
	bool isPositive = false;
	bool isInside;
	float min = FLT_MAX;
	for (int i = 0; i < NumberOfObjects; i++) {
		SceneObj sceneObj = sceneObjects[i];
		float t = intersect(sceneObj.center, sceneObj.radius, r, isInside);
		if (t > 0.001) {
			isPositive = true;
		}
		if (t > 0.001 && t - min < 0.001) {
			min = t;
			isPositive = true;
		}
	}

	if (!isPositive) {
		return false;
	}

	vec3 newPoint = r.origin + min * r.direction;
	if (min >= 0.001 && -esp < (newPoint.x - point.x) && (newPoint.x - point.x) < esp
		&& -esp < (newPoint.y - point.y) && (newPoint.y - point.y) < esp
		&& -esp < (newPoint.z - point.z) && (newPoint.z - point.z) < esp) {
		return true;
	}
	return false;
}

vec3 RayTracing::phong(vec3 P, vec3 normal, SceneObj sceneObj) {

	Pigment pigment = pigments[sceneObj.pigmentNum];
	vec3 pigmentColor = vec3(pigment.r, pigment.g, pigment.b);
	Surface surface = surfaces[sceneObj.surfaceNum];
	vec3 local = (0, 0, 0);
	for (int i = 1; i < lightSources.size(); i++) {
		LightSource lightSource = lightSources[i];
		if (!isVisible(P, lightSource, sceneObjects)) continue;
		vec3 lightIntensity = vec3(lightSource.Ir, lightSource.Ig, lightSource.Ib);
		float d = sqrt(dot(lightSource.LightPos - P, lightSource.LightPos - P));
		float attenuation = lightSource.a + d * lightSource.b + d * d*lightSource.c;
		vec3 l = normalize(lightSource.LightPos - P);
		vec3 v = normalize(camera.eye - P);
		vec3 h = normalize(l + v);
		vec3 Id = max((double)dot(l, normal), 0.0);
		vec3 Is = pow(max((double)dot(normal, h), 0.0), surface.shineness);
		local += (lightIntensity / attenuation) * (Id*surface.kd*pigmentColor + Is * surface.ks);
	}
	vec3 Ia = ambientIntensity * surface.ka;
	local += Ia * pigmentColor;
	return local;
}

float RayTracing::intersect(const vec3 &center, float radius, ray r, bool &isInside) {
	vec3 d = r.direction;
	vec3 o = r.origin;
	float a = dot(d, d);
	vec3 u = (center - o);
	float b = -2.0*dot(u, d);
	float c = dot(u, u) - radius * radius;
	isInside = false;
	vec3 m = u - dot(u, d)*d;
	float discriminant = 4 * (radius*radius - dot(m, m));
	if (discriminant < -0.001) return -1.0;
	else {
		float root1 = (-b - sqrt(discriminant)) / (2.0*a);
		float root2 = (-b + sqrt(discriminant)) / (2.0*a);

		if (root1 < 0.001 && root2 < 0.001) {
			return root1;
		}
		else if (root1 < 0.001 || root2 < 0.001) {
			isInside = true;
			if (root1 - root2 > 0.001) return root1;
			else return root2;
		}
		else if (root1 - root2 > 0.001) return root2;
		else return root1;
	}
}

vec3 RayTracing::intersectionPoint(ray r, float t) {
	return r.origin + t * r.direction;
}

void RayTracing::outputToPPM(vec3** arr) {
	int width = readInputFile.getWidth();
	int height = readInputFile.getHeight();
	string outputFileName = readInputFile.getOutputFileName();
	int i, j;
	FILE *fp = fopen(outputFileName.c_str(), "wb"); /* b - binary mode */
	fprintf(fp, "P6\n%d %d\n255\n", width, height);
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			unsigned char color[3];
			color[0] = (unsigned char)(arr[j][i].x >= 1.0 ? 255 : (arr[j][i].x <= 0.0 ? 0 : (int)floor(arr[j][i].x  * 256.0)));  /* red */
			color[1] = (unsigned char)(arr[j][i].y >= 1.0 ? 255 : (arr[j][i].y <= 0.0 ? 0 : (int)floor(arr[j][i].y  * 256.0)));   /* green */
			color[2] = (unsigned char)(arr[j][i].z >= 1.0 ? 255 : (arr[j][i].z <= 0.0 ? 0 : (int)floor(arr[j][i].z  * 256.0)));  /* blue */
			fwrite(color, 1, 3, fp);
		}
	}
	fclose(fp);
}

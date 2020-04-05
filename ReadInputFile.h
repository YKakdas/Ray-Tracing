#ifndef READ_INPUT_FILE_H
#define READ_INPUT_FILE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h> 
#include "ObjectTypes.h"

class ReadInputFile {

private:
	string outputFileName;
	int width;
	int height;
	int numberOfLightSources;
	int numberOfSurfaces;
	int numberOfSceneObjs;
	int numberOfPigments;
	Camera camera;
	vector<LightSource> lightSources;
	vector<Pigment> pigments;
	vector<Surface> surfaces;
	vector<SceneObj> sceneObjs;

public:
	ReadInputFile() {

	}
	ReadInputFile(const char* fileName) {
		fillObjectsFromFile(fileName);
	}
	const string getOutputFileName(void);
	const int getWidth(void);
	const int getHeight(void);
	const vector<LightSource> &getLightSources(void);
	const vector<Surface> &getSurfaces(void);
	const vector<SceneObj> &getSceneObjs(void);
	const vector<Pigment> &getPigments(void);
	const Camera &getCamera(void);
	void fillObjectsFromFile(const char* fileName);
};
#endif

#include "ReadInputFile.h"

const string ReadInputFile::getOutputFileName(void) {
	return this->outputFileName;
}
const int ReadInputFile::getWidth() {
	return this->width;
}
const int ReadInputFile::getHeight() {
	return this->height;
}
const vector<LightSource> &ReadInputFile::getLightSources() {
	return this->lightSources;
}
const vector<Surface> &ReadInputFile::getSurfaces() {
	return this->surfaces;
}
const vector<SceneObj> &ReadInputFile::getSceneObjs() {
	return this->sceneObjs;
}
const vector<Pigment> &ReadInputFile::getPigments() {
	return this->pigments;
}

const Camera &ReadInputFile::getCamera(void) {
	return this->camera;
}
void ReadInputFile::fillObjectsFromFile(const char* fileName)
{
	string line;
	ifstream inputFile(fileName);
	int count = 0;
	if (inputFile.is_open()) {
		while (getline(inputFile, line)) {
			std::stringstream ss(line);
			if (count == 0) {
				this->outputFileName.assign(line);
				count++;
			}
			else if (count == 1) {
				ss >> this->width;
				ss >> this->height;
				count++;
			}
			else if (count == 2) {
				vec3 eye, at, up;
				stringstream ss(line);
				ss >> eye.x >> eye.y >> eye.z;
				getline(inputFile, line);
				ss = stringstream(line);
				ss >> at.x >> at.y >> at.z;
				getline(inputFile, line);
				ss = stringstream(line);
				ss >> up.x >> up.y >> up.z;
				getline(inputFile, line);
				ss = stringstream(line);
				float fovy;
				ss >> fovy;
				this->camera.eye = eye;
				this->camera.at = at;
				this->camera.up = up;
				this->camera.fovy = fovy;
				count++;
			}
			else if (count == 3) {
				ss >> this->numberOfLightSources;
				count++;
			}
			else if (count == 4) {
				for (int i = 0; i < this->numberOfLightSources; i++) {
					stringstream ss(line);
					float x, y, z, Ir, Ig, Ib, a, b, c;
					ss >> x >> y >> z >> Ir >> Ig >> Ib >> a >> b >> c;
					this->lightSources.push_back({ {x,y,z},Ir,Ig,Ib,a,b,c });
					if (i != this->numberOfLightSources - 1) {
						getline(inputFile, line);
					}
				}
				count++;
			}
			else if (count == 5) {
				ss >> this->numberOfPigments;
				count++;
			}
			else if (count == 6) {
				for (int i = 0; i < this->numberOfPigments; i++) {
					std::stringstream ss(line);
					string type;
					float r, g, b;
					ss >> type;
					ss >> r >> g >> b;
					this->pigments.push_back({ type,r,g,b });
					if (i != this->numberOfPigments - 1) {
						getline(inputFile, line);
					}
				}
				count++;
			}
			else if (count == 7) {
				ss >> this->numberOfSurfaces;
				count++;
			}
			else if (count == 8) {
				for (int i = 0; i < this->numberOfSurfaces; i++) {
					std::stringstream ss(line);
					float ka, kd, ks, shineness, kr;
					ss >> ka >> kd >> ks >> shineness >> kr;
					this->surfaces.push_back({ ka,kd,ks,shineness,kr });
					if (i != this->numberOfSurfaces - 1) {
						getline(inputFile, line);
					}
				}
				count++;
			}
			else if (count == 9) {
				ss >> this->numberOfSceneObjs;
				count++;
			}
			else if (count == 10) {
				for (int i = 0; i < this->numberOfSceneObjs; i++) {
					std::stringstream ss(line);
					string type;
					int pigmentNum, surfaceNum;
					float x, y, z, radius;
					ss >> pigmentNum >> surfaceNum >> type >> x >> y >> z >> radius;
					this->sceneObjs.push_back({ pigmentNum,surfaceNum,type,{x,y,z},radius });
					if (i != this->numberOfSceneObjs - 1) {
						getline(inputFile, line);
					}
				}
				count++;
			}
		}
		inputFile.close();
	}
}

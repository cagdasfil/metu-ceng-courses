#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"

using namespace std;

class Scene
{
public:
	Color backgroundColor;
	bool cullingEnabled;
	int projectionType;

	vector< vector<Color> > image;
	vector< Camera* > cameras;
	vector< Vec3* > vertices;
	vector< Color* > colorsOfVertices;
	vector< Scaling* > scalings;
	vector< Rotation* > rotations;
	vector< Translation* > translations;
	vector< Model* > models;

	Scene(const char *xmlPath);

	void initializeImage(Camera* camera);
	void forwardRenderingPipeline(Camera* camera);
	int makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile(Camera* camera);
	void convertPPMToPNG(string ppmFileName, int osType);

	// HELPER FUNCTIONS

	void applyModelTransformations(vector<int>* vx);
    void applyCameraTransformation(Camera*, vector<Vec3*>*);
    void applyProjection(Camera*, vector<Vec3*>*);
    void applyViewport(Camera*, vector<Vec3*>*);
    void applyRasterization(Camera*, vector<Vec3*>*);
    void applyLineRasterization(Model*, vector<Vec3*>*);
    void applyTriangleRasterization(Model*, vector<Vec3*>*);
    void applyCulling(Model*, vector<Vec3*>*);
    double f01(int x,int y,int x0, int x1,int y0,int y1);
    double f12(int x,int y,int x1, int x2,int y1,int y2);
    double f20(int x,int y,int x0, int x2,int y0,int y2);
    void lineDraw(int x0,int y0,int x1,int y1,Color c0, Color c1);
    Vec3 perspectiveDivide(Vec4 v4);
    void backFaceCulling(Camera* camera);
	void translate(int id, vector<int>* vx);
	void rotate(int id, vector<int>* vx);
	void scale(int id, vector<int>* vx);
	int minElementOfVec3(Vec3 v);
};

#endif

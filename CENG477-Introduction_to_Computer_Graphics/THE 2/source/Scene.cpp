#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <algorithm>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;

/*
	Transformations, clipping, culling, rasterization are done here.
	You can define helper functions inside Scene class implementation.
*/
void Scene::forwardRenderingPipeline(Camera *camera)
{
	// TODO: Implement this function.
    vector<Vec3*>* transformedVertices = new vector<Vec3*>;
    vector<int>* vx = new vector<int>; // contains all unique vertex id of a model.


    if (camera->cameraId == this->cameras[0]->cameraId){
        applyModelTransformations(vx);
    }

	applyCameraTransformation(camera, transformedVertices);

    applyProjection(camera, transformedVertices);

    for (int j = 0; j < this->models.size(); ++j) {
        Model* model = this->models[j];
        if (model->type == 0) { //wireframe
            //applyClipping();
            if (this->cullingEnabled){
                applyCulling(model, transformedVertices);
            }
        }
        else{ //solid
            if (this->cullingEnabled){
                applyCulling(model, transformedVertices);
            }
        }
    }

    applyViewport(camera, transformedVertices);

    for (int k = 0; k < this->models.size(); ++k) {
        Model* model = this->models[k];
        if (model->type == 0) { //wireframe
            applyLineRasterization(model, transformedVertices);
            //lineDraw(200,100,100,600,Color(0,0,0),Color(0,0,0));
        }
        else{ //solid
            applyTriangleRasterization(model, transformedVertices);
        }
    }
}

void Scene::applyModelTransformations(vector<int>* vx){

    for (int i = 0; i < this->models.size(); ++i) { // iterate models.

        for (int k = 0; k < this->models[i]->numberOfTriangles; ++k) { // iterate triangles.

            for (int m = 0; m < 3; ++m) {
                int id = models[i]->triangles[k].vertexIds[m];
                if(std::find(vx->begin(), vx->end(), id) != vx->end()) {
                    continue;
                }
                else {
                    vx->push_back(id);
                }
            }
        }

        for (int j = 0; j < this->models[i]->transformationIds.size() ; ++j) {
            char type = this->models[i]->transformationTypes[j];
            int id = this->models[i]->transformationIds[j];
            if (type == 't') {
                translate(id, vx);
            }
            else if (type == 'r') {
                rotate(id, vx);
            }
            else{
                scale(id, vx);
            }
        }

        vx->clear();
    }
}

void Scene::applyCameraTransformation(Camera* camera, vector<Vec3*>* transformedVertices) {
    Vec3 e, u, v, w;
    e = camera->pos;
    u = camera->u;
    v = camera->v;
    w = camera->w;
    double mcam[4][4] = {
        {u.x, u.y, u.z, -(u.x*e.x + u.y*e.y + u.z*e.z)},
        {v.x, v.y, v.z, -(v.x*e.x + v.y*e.y + v.z*e.z)},
        {w.x, w.y, w.z, -(w.x*e.x + w.y*e.y + w.z*e.z)},
        {  0,   0,   0,                              1}
    };
    Matrix4 Mcam(mcam);
    for (int i = 0; i < this->vertices.size(); ++i) {
        Vec3 v3 = *(this->vertices[i]);
        Vec4 v4(v3.x, v3.y, v3.z, 1, v3.colorId);
        Vec4 newv4 = multiplyMatrixWithVec4(Mcam, v4);
        Vec3* newv3 = new Vec3(newv4.x, newv4.y, newv4.z, newv4.colorId);
        transformedVertices->push_back(newv3);
    }
}

void Scene::applyProjection(Camera* camera, vector<Vec3*>* transformedVertices) {
    double r,l,t,b,f,n;
    r = camera->right;
    l = camera->left;
    t = camera->top;
    b = camera->bottom;
    f = camera->far;
    n = camera->near;
    double morth[4][4] = {
        {2/(r-l),       0,        0, -((r+l)/(r-l))},
        {      0, 2/(t-b),        0, -((t+b)/(t-b))},
        {      0,       0, -2/(f-n), -((f+n)/(f-n))},
        {      0,       0,        0,              1}
    };
    Matrix4 Morth(morth);

    if (this->projectionType == 0){ // orthographic
        for (int i = 0; i < transformedVertices->size(); ++i) {
            Vec3* v3 = (*transformedVertices)[i];
            Vec4 v4(v3->x, v3->y, v3->z, 1, v3->colorId);
            Vec4 newv4 = multiplyMatrixWithVec4(Morth, v4);
            v3->x = newv4.x;
            v3->y = newv4.y;
            v3->z = newv4.z;
            v3->colorId = newv4.colorId;
        }
    }
    else{
        double mp2o[4][4] = {
            {n, 0,   0,   0},
            {0, n,   0,   0},
            {0, 0, f+n, f*n},
            {0, 0,  -1,   0}
        };
        Matrix4 Mp2o(mp2o);
        Matrix4 Mper = multiplyMatrixWithMatrix(Morth, Mp2o);
        for (int i = 0; i < transformedVertices->size(); ++i) {
            Vec3* v3 = (*transformedVertices)[i];
            Vec4 v4(v3->x, v3->y, v3->z, 1, v3->colorId);
            Vec4 newv4 = multiplyMatrixWithVec4(Mper, v4);
            double t = newv4.t;
            *v3 = perspectiveDivide(newv4);
        }
    }
}

void Scene::applyViewport(Camera* camera, vector<Vec3*>* transformedVertices) {
    double nx = camera->horRes;
    double ny = camera->verRes;
    double mvp[4][4] = {
        {nx/2,    0,   0, (nx-1)/2},
        {   0, ny/2,   0, (ny-1)/2},
        {   0,    0, 0.5,      0.5},
        {   0,    0,   0,        0}
    };
    Matrix4 Mvp(mvp);
    for (int i = 0; i < transformedVertices->size(); ++i) {
        Vec3* v3 = (*transformedVertices)[i];
        Vec4 v4(v3->x, v3->y, v3->z, 1, v3->colorId);
        Vec4 newv4 = multiplyMatrixWithVec4(Mvp, v4);
        v3->x = newv4.x;
        v3->y = newv4.y;
        v3->z = newv4.z;
        v3->colorId = newv4.colorId;
    }
}

void Scene::applyRasterization(Camera* camera, vector<Vec3*>* transformedVertices) {
    for (int i = 0; i < transformedVertices->size(); ++i) {
        Vec3* v3 = (*transformedVertices)[i];
        this->image[(int)v3->x][(int)v3->y] = *(this->colorsOfVertices[i]);
    }
}

void Scene::applyLineRasterization(Model* model, vector<Vec3*>* transformedVertices) {
    for (int i = 0; i < model->numberOfTriangles; ++i) {
        Triangle t = model->triangles[i];
        Vec3* v0 = (*transformedVertices)[t.vertexIds[0]-1];
        Vec3* v1 = (*transformedVertices)[t.vertexIds[1]-1];
        Vec3* v2 = (*transformedVertices)[t.vertexIds[2]-1];
        int x0 = round(v0->x);
        int y0 = round(v0->y);
        Color c0 = *(this->colorsOfVertices[t.vertexIds[0]-1]);
        int x1 = round(v1->x);
        int y1 = round(v1->y);
        Color c1 = *(this->colorsOfVertices[t.vertexIds[1]-1]);
        int x2 = round(v2->x);
        int y2 = round(v2->y);
        Color c2 = *(this->colorsOfVertices[t.vertexIds[2]-1]);
        lineDraw(x0,y0,x1,y1,c0,c1);
        lineDraw(x1,y1,x2,y2,c1,c2);
        lineDraw(x2,y2,x0,y0,c2,c0);
    }
}

void Scene::applyTriangleRasterization(Model* model, vector<Vec3*>* transformedVertices) {
    for (int i = 0; i < model->numberOfTriangles; ++i) {
        Triangle t = model->triangles[i];
        Vec3* v0 = (*transformedVertices)[t.vertexIds[0]-1];
        Vec3* v1 = (*transformedVertices)[t.vertexIds[1]-1];
        Vec3* v2 = (*transformedVertices)[t.vertexIds[2]-1];
        int x0 = round(v0->x);
        int y0 = round(v0->y);
        Color c0 = *(this->colorsOfVertices[t.vertexIds[0]-1]);
        int x1 = round(v1->x);
        int y1 = round(v1->y);
        Color c1 = *(this->colorsOfVertices[t.vertexIds[1]-1]);
        int x2 = round(v2->x);
        int y2 = round(v2->y);
        Color c2 = *(this->colorsOfVertices[t.vertexIds[2]-1]);
        for (int j = min(min(y0,y1), y2); j < max(max(y0,y1), y2); ++j) {
            for (int k = min(min(x0,x1), x2); k < max(max(x0,x1), x2); ++k) {
                double a = f12(k,j,x1,x2,y1,y2)/f12(x0,y0,x1,x2,y1,y2);
                double b = f20(k,j,x0,x2,y0,y2)/f20(x1,y1,x0,x2,y0,y2);
                double g = f01(k,j,x0,x1,y0,y1)/f01(x2,y2,x0,x1,y0,y1);
                if (a>=0 && b>=0 && g>=0) {
                    Color c;
                    c.r = a * c0.r + b * c1.r + g * c2.r;
                    c.g = a * c0.g + b * c1.g + g * c2.g;
                    c.b = a * c0.b + b * c1.b + g * c2.b;
                    this->image[k][j] = c;
                }
            }
        }
    }
}

double Scene::f01(int x,int y,int x0, int x1,int y0,int y1){
    return x*(y0-y1)+y*(x1-x0)+x0*y1-y0*x1;
}
double Scene::f12(int x,int y,int x1, int x2,int y1,int y2){
    return x*(y1-y2)+y*(x2-x1)+x1*y2-y1*x2;
}
double Scene::f20(int x,int y,int x0, int x2,int y0,int y2){
    return x*(y2-y0)+y*(x0-x2)+x2*y0-y2*x0;
}

void Scene::lineDraw(int x0,int y0,int x1,int y1,Color c0, Color c1){
    int x, y, p, r;
    double d, ydiff, xdiff;
    Color c, dc;

    if (x0 == x1){
        if (y0==y1) {
            this->image[x0][y0] = c0;
            return;
        }
        x = x0;
        c = c0;
        dc.r = (c1.r-c0.r)/abs(y1-y0);
        dc.g = (c1.g-c0.g)/abs(y1-y0);
        dc.b = (c1.b-c0.b)/abs(y1-y0);
        p = (y1-y0)>0 ? 1 : -1;
        for (y = y0; y != y1 ; y+=p ) {
            this->image[x][y] = c;
            c.r+=dc.r;
            c.g+=dc.g;
            c.b+=dc.b;
        }
    }

    else if (y0 == y1){
        if (x0==x1) {
            this->image[x0][y0] = c0;
            return;
        }
        y = y0;
        c = c0;
        dc.r = (c1.r-c0.r)/abs(x1-x0);
        dc.g = (c1.g-c0.g)/abs(x1-x0);
        dc.b = (c1.b-c0.b)/abs(x1-x0);
        p = (x1-x0)>0 ? 1 : -1;

        for (x = x0; x != x1 ; x+=p ) {
            this->image[x][y] = c;
            c.r+=dc.r;
            c.g+=dc.g;
            c.b+=dc.b;
        }
    }

    else{
        double m = abs(((double)(y1-y0))/(x1-x0));
        if(m>0 && m<=1){
            ydiff = -abs(y0-y1);
            xdiff = abs(x1-x0);
            y = y0;
            d = ydiff+0.5*xdiff;
            c = c0;
            dc.r = (c1.r-c0.r)/abs(x1-x0);
            dc.g = (c1.g-c0.g)/abs(x1-x0);
            dc.b = (c1.b-c0.b)/abs(x1-x0);
            p = (x1-x0)>0 ? 1 : -1;
            r = (y1-y0)>0 ? 1: -1;
            for (x = x0; x != x1 ; x+=p ) {
                this->image[x][y] = c;
                if (d<0) {
                    y+=r;
                    d+=ydiff+xdiff;
                }
                else{
                    d+=ydiff;
                }
                c.r+=dc.r;
                c.g+=dc.g;
                c.b+=dc.b;
            }
        }
        else {
            xdiff = -abs(x0-x1);
            ydiff = abs(y1-y0);
            x = x0;
            d = xdiff+0.5*ydiff;
            c = c0;
            dc.r = (c1.r-c0.r)/abs(y1-y0);
            dc.g = (c1.g-c0.g)/abs(y1-y0);
            dc.b = (c1.b-c0.b)/abs(y1-y0);
            p = (y1-y0)>0 ? 1 : -1;
            r = (x1-x0)>0 ? 1 : -1;
            for (y = y0; y != y1 ; y+=p ) {
                this->image[x][y] = c;
                if (d<0) {
                    x+=r;
                    d+=xdiff+ydiff;
                }
                else{
                    d+=xdiff;
                }
                c.r+=dc.r;
                c.g+=dc.g;
                c.b+=dc.b;
            }
        }
    }
}

Vec3 Scene::perspectiveDivide(Vec4 v4){
    double t = v4.t;
    Vec3 v3(v4.x/t, v4.y/t, v4.z/t, v4.colorId);
    return v3;
}

void Scene::applyCulling(Model* model, vector<Vec3*>* transformedVertices){
    for (int i = 0; i < model->numberOfTriangles; ++i) {
        Triangle t = model->triangles[i];
        Vec3 *v0 = (*transformedVertices)[t.vertexIds[0] - 1];
        Vec3 *v1 = (*transformedVertices)[t.vertexIds[1] - 1];
        Vec3 *v2 = (*transformedVertices)[t.vertexIds[2] - 1];
        Vec3 a = subtractVec3(*v2, *v0);
        Vec3 b = subtractVec3(*v1, *v0);
        Vec3 n = crossProductVec3(a, b);
        Vec3 sum = addVec3(*v2, (addVec3(*v0, *v1)));
        Vec3 v = multiplyVec3WithScalar(sum, (1/3));

    }
}











void Scene::backFaceCulling(Camera* camera){

}

void Scene::translate(int id, vector<int>* vx){
    Vec3 t;
    Vec3* vertex;
    t.x = this->translations[id-1]->tx;
    t.y = this->translations[id-1]->ty;
    t.z = this->translations[id-1]->tz;
    for (int i = 0; i < vx->size() ; ++i) {
        vertex = this->vertices[(*vx)[i]-1];
        *vertex = addVec3(*vertex, t);
    }
}

void Scene::rotate(int id, vector<int>* vx){
    Vec3 u;
    Vec3 v;
    Vec3 w;
    int min;
    double angle;

    angle = this->rotations[id-1]->angle;
    angle = angle*M_PI/180;

    u.x = this->rotations[id-1]->ux;
    u.y = this->rotations[id-1]->uy;
    u.z = this->rotations[id-1]->uz;

    min = minElementOfVec3(u);

    if (min == 0){
        v.x = 0;
        v.y = -u.z;
        v.z = u.y;
    }
    else if (min == 1) {
        v.x = -u.z;
        v.y = 0;
        v.z = u.x;
    }
    else if (min == 2) {
        v.x = -u.y;
        v.y = u.x;
        v.z = 0;
    }

    w = crossProductVec3(u, v);

    u = normalizeVec3(u);
    v = normalizeVec3(v);
    w = normalizeVec3(w);

    double m[4][4] = {
        {u.x, u.y, u.z, 0},
        {v.x, v.y, v.z, 0},
        {w.x, w.y, w.z, 0},
        {  0,   0,   0, 1}};
    Matrix4 M(m);

    double m_1[4][4] = {
        {u.x, v.x, w.x, 0},
        {u.y, v.y, w.y, 0},
        {u.z, v.z, w.z, 0},
        {  0,   0,   0, 1}};
    Matrix4 M_1(m_1);

    double r[4][4] = {
            {  1,          0,           0, 0},
            {  0, cos(angle), -sin(angle), 0},
            {  0, sin(angle),  cos(angle), 0},
            {  0,          0,           0, 1}};
    Matrix4 R(r);

    for (int i = 0; i < vx->size() ; ++i) {
        Vec3* vertex = this->vertices[(*vx)[i]-1];
        Vec4 vec4(vertex->x, vertex->y, vertex->z, 1, 1);
        vec4 = multiplyMatrixWithVec4(M, vec4);
        vec4 = multiplyMatrixWithVec4(R, vec4);
        vec4 = multiplyMatrixWithVec4(M_1, vec4);
        vertex->x = vec4.x;
        vertex->y = vec4.y;
        vertex->z = vec4.z;
    }
}

int Scene::minElementOfVec3(Vec3 v){
    int index;
    double min;
    v.x = abs(v.x);
    v.y = abs(v.y);
    v.z = abs(v.z);
    if(v.x < v.y) {
        min = v.x;
        index = 0;
    }
    else {
        min = v.y;
        index = 1;
    }
    if(v.z < min) {
        min = v.z;
        index = 2;
    }
    return index;
}


void Scene::scale(int id, vector<int>* vx){
    Vec3 s;
    s.x = this->scalings[id-1]->sx;
    s.y = this->scalings[id-1]->sy;
    s.z = this->scalings[id-1]->sz;
    for (int i = 0; i < vx->size() ; ++i) {
        Vec3* vertex = this->vertices[(*vx)[i]-1];
        vertex->x *= s.x;
        vertex->y *= s.y;
        vertex->z *= s.z;
    }

}

/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL)
		pElement->QueryBoolText(&cullingEnabled);

	// read projection type
	pElement = pRoot->FirstChildElement("ProjectionType");
	if (pElement != NULL)
		pElement->QueryIntText(&projectionType);

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read models
	pElement = pRoot->FirstChildElement("Models");

	XMLElement *pModel = pElement->FirstChildElement("Model");
	XMLElement *modelElement;
	while (pModel != NULL)
	{
		Model *model = new Model();

		pModel->QueryIntAttribute("id", &model->modelId);
		pModel->QueryIntAttribute("type", &model->type);

		// read model transformations
		XMLElement *pTransformations = pModel->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		pTransformations->QueryIntAttribute("count", &model->numberOfTransformations);

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			model->transformationTypes.push_back(transformationType);
			model->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		// read model triangles
		XMLElement *pTriangles = pModel->FirstChildElement("Triangles");
		XMLElement *pTriangle = pTriangles->FirstChildElement("Triangle");

		pTriangles->QueryIntAttribute("count", &model->numberOfTriangles);

		while (pTriangle != NULL)
		{
			int v1, v2, v3;

			str = pTriangle->GetText();
			sscanf(str, "%d %d %d", &v1, &v2, &v3);

			model->triangles.push_back(Triangle(v1, v2, v3));

			pTriangle = pTriangle->NextSiblingElement("Triangle");
		}

		models.push_back(model);

		pModel = pModel->NextSiblingElement("Model");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	// if image is filled before, just change color rgb values with the background color
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}
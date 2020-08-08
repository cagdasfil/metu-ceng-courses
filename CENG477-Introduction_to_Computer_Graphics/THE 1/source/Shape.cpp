#include "Shape.h"
#include "Scene.h"
#include <cstdio>
#include <cmath>

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}

Sphere::Sphere(void)
{}

float Sphere::detCalculator(float a, float d, float g,
                           float b, float e, float h,
                           float c, float f, float i) const{
    return a*(e*i-h*f) + b*(g*f-d*i) + c*(d*h-e*g);
}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    this->id = id;
    this->matIndex = matIndex;
    this->cIndex = cIndex;
    this->R = R;
    this->vertices = pVertices;
}

/* Sphere-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
    */
	ReturnVal ret;
	ret.materialID = matIndex;
	Vector3f o = ray.origin;
	Vector3f d = ray.direction;
	Vector3f c = (*vertices)[cIndex-1];
	float determinant = pow(d*(o-c),2)-(d*d)*(((o-c)*(o-c))-R*R);
	if(determinant<0){
        ret.isIntersect = 0;
	}
	else{
	    float t1 = ((-(d*(o-c)))+sqrt(determinant))/(d*d);
        float t2 = ((-(d*(o-c)))-sqrt(determinant))/(d*d);
        ret.isIntersect = 1;
        if(t1<t2){
            ret.t = t1;
            ret.intersectionPoint = ray.getPoint(t1);
            ret.surfaceNormal = ret.intersectionPoint - c;
        }
        else{
            ret.t = t2;
            ret.intersectionPoint = ray.getPoint(t2);
            ret.surfaceNormal = ret.intersectionPoint - c;
        }
	}
    return ret;
}

Triangle::Triangle(void)
{}

float Triangle::detCalculator(float a, float d, float g,
                           float b, float e, float h,
                           float c, float f, float i) const{
    return a*(e*i-h*f) + b*(g*f-d*i) + c*(d*h-e*g);
}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	this->id = id;
    this->matIndex = matIndex;
    this->p1Index = p1Index;
    this->p2Index = p2Index;
    this->p3Index = p3Index;
    this->vertices = pVertices;
}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    ReturnVal ret;
    ret.materialID = matIndex;
    Vector3f a = (*vertices)[p1Index-1];
    Vector3f b = (*vertices)[p2Index-1];
    Vector3f c = (*vertices)[p3Index-1];
    Vector3f o = ray.origin;
    Vector3f d = ray.direction;

    float detA = detCalculator(a.x-b.x, a.x-c.x, d.x,
                                a.y-b.y, a.y-c.y, d.y,
                                a.z-b.z, a.z-c.z, d.z);

    float beta = detCalculator(a.x-o.x, a.x-c.x, d.x,
                                a.y-o.y, a.y-c.y, d.y,
                                a.z-o.z, a.z-c.z, d.z) / detA;

    float gama = detCalculator(a.x-b.x, a.x-o.x, d.x,
                                a.y-b.y, a.y-o.y, d.y,
                                a.z-b.z, a.z-o.z, d.z) / detA;

    float t = detCalculator(a.x-b.x, a.x-c.x, a.x-o.x,
                            a.y-b.y, a.y-c.y, a.y-o.y,
                            a.z-b.z, a.z-c.z, a.z-o.z) / detA;

    if(beta>=0 && gama>=0 && beta+gama<=1){
        ret.isIntersect = 1;
        ret.intersectionPoint = ray.getPoint(t);
        ret.surfaceNormal = (c-b).cross(a-b);
        ret.t = t;
    }
    else{
        ret.isIntersect = 0;
    }
    return ret;
}

Mesh::Mesh()
{}

float Mesh::detCalculator(float a, float d, float g,
                           float b, float e, float h,
                           float c, float f, float i) const{
    return a*(e*i-h*f) + b*(g*f-d*i) + c*(d*h-e*g);
}


/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	this->id = id;
	this->matIndex = matIndex;
	this->faces = faces;
	this->pIndices = pIndices;
	this->vertices = pVertices;
}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	float t=-1;
	Triangle triangle;
	ReturnVal ret;
	ReturnVal finalret;
    finalret.materialID = matIndex;
    int len = faces.size();
    for (int i = 0; i < len; ++i) {
        triangle = faces[i];
        ret = triangle.intersect(ray);
        if(ret.isIntersect == 0){
            continue;
        }
        else{
            if(t==-1){
                t = ret.t;
                finalret.isIntersect = 1;
                finalret.t = ret.t;
                finalret.surfaceNormal = ret.surfaceNormal;
                finalret.intersectionPoint = ret.intersectionPoint;
            }
            else if(ret.t < t){
                t = ret.t;
                finalret.t = ret.t;
                finalret.surfaceNormal = ret.surfaceNormal;
                finalret.intersectionPoint = ret.intersectionPoint;
            }
        }
    }
    return finalret;
}



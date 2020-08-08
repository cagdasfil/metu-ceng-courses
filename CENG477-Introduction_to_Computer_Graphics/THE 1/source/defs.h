#ifndef _DEFS_H_
#define _DEFS_H_

#include <cmath>

class Scene;

/* 3 dimensional vector holding floating point numbers.
Used for both coordinates and color. 
Use x, y, z for coordinate computations, and use r, g, b for color computations. 
Note that you do not have to use this one if you use any vector computation library like Eigen. */
typedef struct Vector3f
{
	union 
	{
		float x;
		float r;
	};
	union
	{
		float y;
		float g;
	};
	union
	{
		float z;
		float b;
	};

	Vector3f operator+(Vector3f const& v2) const {
	    Vector3f vsum;
	    vsum.x = x + v2.x;
	    vsum.y = y + v2.y;
	    vsum.z = z + v2.z;
        return vsum;
	}

    Vector3f operator-(Vector3f const& v2) const {
        Vector3f vdif;
        vdif.x = x - v2.x;
        vdif.y = y - v2.y;
        vdif.z = z - v2.z;
        return vdif;
    }

    float operator*(Vector3f const& v2) const {
        return x*v2.x + y*v2.y + z*v2.z;
    }

    Vector3f operator*(float c) const {
	    Vector3f vprd;
	    vprd.x = c*x;
        vprd.y = c*y;
        vprd.z = c*z;
        return vprd;
    }

    Vector3f cross(Vector3f const& v2) const {
        Vector3f vcrs;
        vcrs.x = y*(v2.z) - z*(v2.y);
        vcrs.y = z*(v2.x) - x*(v2.z);
        vcrs.z = x*(v2.y) - y*(v2.x);
        return vcrs;
    }

    Vector3f operator/(float c) const {
	    Vector3f vdiv;
	    vdiv.x = x/c;
	    vdiv.y = y/c;
	    vdiv.z = z/c;
	    return vdiv;
	}

    float operator/(Vector3f const& v2) const {
        if(x!=0){
            return x/(v2.x);
        }
	    if(y!=0){
	        return y/(v2.y);
	    }
	    if(z!=0){
	        return z/(v2.z);
	    }
    }

    float len2(void) const {
        return x*x+y*y+z*z;
	}

    float len(void) const {
        return sqrt(x*x+y*y+z*z);
    }

    Vector3f unit(){
	    Vector3f v;
	    float len = sqrt(this->len2());
	    v.x = x/len;
	    v.y = y/len;
	    v.z = z/len;
        return v;
	}

} Vector3f;


/* Structure to hold return value from ray intersection routine.
This should hold information related to the intersection point,
for example, coordinate of the intersection point, surface normal at the intersection point etc.
Think about the variables you will need for this purpose and declare them here inside of this structure. */
typedef struct ReturnVal
{
    /***********************************************
     *                                             *
     * TODO: Implement this structure              *
     *                                             *
     ***********************************************
     */
    int isIntersect;
    Vector3f intersectionPoint;
    Vector3f surfaceNormal;
    float t;
    int materialID;
} ReturnVal;




//
// The global variable through which you can access the scene data
//
extern Scene* pScene;

#endif

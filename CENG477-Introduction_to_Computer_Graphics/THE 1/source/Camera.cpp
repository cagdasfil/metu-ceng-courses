#include "Camera.h"

Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    this->id = id;
    for(int i=0; i<32; i++){
        this->imageName[i] = imageName[i];
        if(imageName[i] == '\0'){
            break;
        }
    }
    this->pos = pos;
    this->gaze = gaze;
    this->up = up;
    this->imgPlane = imgPlane;
}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    Ray ray;
    Vector3f e = pos;
    Vector3f v = up;
    Vector3f w = gaze*(-1);
    Vector3f u = v.cross(w);
    Vector3f m = e + ((w*(-1))*(imgPlane.distance));
    Vector3f q = m + u*(imgPlane.left) + v*(imgPlane.top);
    float su = ((row+0.5)*(imgPlane.right-imgPlane.left)) / imgPlane.nx;
    float sv = ((col+0.5)*(imgPlane.top-imgPlane.bottom)) / imgPlane.ny;
    Vector3f s = q + ((u*su)-(v*sv));
    ray.direction = s-e;
    ray.origin = e;
    return ray;
}

Vector3f Camera::getPos(void){
    return pos;
}


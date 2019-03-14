#ifndef VBOTEAPOT_H
#define VBOTEAPOT_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "Loader.h"
#include "IObject.h"

using glm::vec4;
using glm::vec3;
using glm::mat3;
using glm::mat4;

class VBOTeapot : public IObject
{
private:
    unsigned int vaoHandle, VBO_position, VBO_normal, IBO;;
    unsigned int faces;

	void setup(float *v, float *n, unsigned int * el, int nbverts);
    void generatePatches(float * v, float * n, float *tc, unsigned int* el, int grid);
    void buildPatchReflect(int patchNum,
                           float *B, float *dB,
                           float *v, float *n, float *, unsigned int *el,
                           int &index, int &elIndex, int &, int grid,
                           bool reflectX, bool reflectY);
    void buildPatch(vec3 patch[][4],
                    float *B, float *dB,
                    float *v, float *n,float *, unsigned int *el,
                    int &index, int &elIndex, int &, int grid, mat3 reflect, bool invertNormal);
    void getPatch( int patchNum, vec3 patch[][4], bool reverseV );

    void computeBasisFunctions( float * B, float * dB, int grid );
    vec3 evaluate( int gridU, int gridV, float *B, vec3 patch[][4] );
    vec3 evaluateNormal( int gridU, int gridV, float *B, float *dB, vec3 patch[][4] );
    void moveLid(int,float *,mat4);

public:
    VBOTeapot(int grid, mat4 lidTransform);
	void draw();

};

#endif // VBOTEAPOT_H

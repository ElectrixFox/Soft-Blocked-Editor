#include "MathsOperations.h"

void OutputVec2(vec2 v)
{
printf("(%.2f, %.2f)", v.x, v.y);
}

void OutputVecn(int n, const float* v)
{
printf("(%.2f", v[0]);
for (int i = 1; i < n; i++)
    {
    printf(", %.2f", v[i]);
    }
printf(")");
}

void OutputMatn(int n, int m, const float** A)
{
for (int i = 0; i < n; i++)
    {
    OutputVecn(m, A[i]);
    printf("\n");
    }
}

void OutputMat3(mat3 A)
{
for (int i = 0; i < 3; i++)
    {
    printf("\n%.2f %.2f %.2f", A.mat[i].x, A.mat[i].y, A.mat[i].z);
    }
}

void ExpandArray(void** arr, unsigned int osize, unsigned int nsize, unsigned int elesize)
{
/*
void* ptr = (void*)malloc(nsize * elesize);
memcpy(ptr, *arr, nsize * sizeof(elesize));
*/
void* ptr = realloc(*arr, elesize * nsize);
if(ptr == NULL)
    {
    printf("\nERROR: The reallocation shat the bed");
    free(ptr);
    exit(1);
    }
else
    {
    *arr = ptr;
    }
}

void ExpandByOne(void **arr, const unsigned int size, unsigned int elesize) { ExpandArray(arr, size, size + 1, elesize); }

static void _ShrinkArray(void** arr, unsigned int osize, unsigned int nsize, unsigned int elesize)
{
// ExpandArray(arr, osize, nsize, elesize);

/*/
void* tmp = (void*)malloc(nsize * elesize);
memcpy(tmp, *arr, nsize * sizeof(elesize));
*arr = tmp;
*/
}

void ShrinkArrayByOne(void** arr, const unsigned int size, unsigned int elesize) { _ShrinkArray(arr, size, size - 1, elesize); }

static unsigned int findArrMin(unsigned int* arr, int size)
{
unsigned int min = arr[0];
for (int i = 0; i < size; i++)
    if(arr[i] < min)
        min = arr[i];
return min;
}

static unsigned int findArrMax(unsigned int* arr, int size)
{
unsigned int max = arr[0];
for (int i = 0; i < size; i++)
    if(arr[i] > max)
        max = arr[i];
return max;
}

static int isInArr(unsigned int* arr, int size, unsigned int tofind)
{
for (int i = 0; i < size; i++)
    if(arr[i] == tofind)
        return 1;
return 0;
}

int findNextIDAvailable(unsigned int* arr, int size)
{
int nid = -1;

for (int i = 0; i <= size; i++)
    {
    if(!isInArr(arr, size, i))
        {
        nid = i;
        break;
        }
    }

if(nid == -1)
    {
    printf("Error: Cannot find a new ID");
    exit(1);
    }
return nid;
}

vec2 ScalarMultVec2(vec2 v, float a) { return (vec2){a * v.x, a * v.y}; }

vec3 ScalarMultVec3(vec3 v, float a) { return (vec3){a * v.x, a * v.y, a * v.x}; }

vec2 addVec2(vec2 u, vec2 v) { return (vec2){u.x + v.x, u.y + v.y}; }

vec2 LeftCornerFromCentre(vec2 centre, vec2 scale)
{
vec2 pos;
pos.x = centre.x;
pos.y = centre.y;
return pos;
}

float getDistance(vec2 a, vec2 b) { return sqrtf(powf((a.x - b.x), 2.0f) + powf((a.y - b.y), 2.0f)); }

float getMagnitude(vec2 v) { return sqrtf(powf(v.x, 2.0f) + powf(v.y, 2.0f)); }

int PointInSquare(vec2 point, vec2 pos, vec2 scale)
{
if (abs(point.x - pos.x) < scale.x)
    if (abs(point.y - pos.y) < scale.y)
        return 1;

return 0;
}

int SquareTouchesSquare(vec2 p1, vec2 s1, vec2 p2, vec2 s2) { return (getDistance(p1, p2) < getMagnitude(s1) + getMagnitude(s2)) ? 0 : 1; }

static int equivVec3(vec3 u, vec3 v)
{
return !(u.x != v.x || u.y != v.y || u.z != v.z);
}

int equivMat3(mat3 A, mat3 B)
{
for (int i = 0; i < 3; i++)
    if(!equivVec3(A.mat[i], B.mat[i]))
        return 0;
return 1;
}

mat3 ScalarMultMat3(mat3 A, float a)
{
mat3 res;
for (int i = 0; i < 3; i++)
    res.mat[i] = ScalarMultVec3(A.mat[i], a);

return res;
}

float generalisedDot(float u[], float v[], unsigned int n)
{
float res = 0;  // setting up the result

for(int i = 0; i < n; i++)
    {
    res += u[i] * v[i]; // doing the dotting
    }

return res;
}

float dotprod(vec3 u, vec3 v)
{
float tu[3] = {u.x, u.y, u.z}; 
float tv[3] = {v.x, v.y, v.z}; 

float res = 0;

for (int i = 0; i < 3; i++)
    {
    res += tu[i] * tv[i];
    }

return res;
}

void transposeMatr3(float m[][3], unsigned int n)
{
float** res = (float**)malloc(sizeof(float) * n * n);  // setting up the result matrix

for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
        res[i][j] = m[j][i];    // doing the transposing

for (int i = 0; i < n; i++)
    memcpy(m[i], res[i], sizeof(m[i]) / sizeof(m[0][0]));   // copying into the input

}

void generalisedMatrMult3(float m1[][3], float m2[][3], unsigned int n, unsigned int m)
{
transposeMatr3(m2, 3);  // transposing the matrix

for (int i = 0; i < n; i++)
    {
    generalisedDot(m1[i], m2[i], n);    // getting the entries
    }
}

vec3 applyMatrix(mat3 matr, vec3 vec)
{
vec3 res;

res.x = dotprod(matr.mat[0], (vec3){vec.x, vec.y, vec.z});
res.y = dotprod(matr.mat[1], (vec3){vec.x, vec.y, vec.z});
res.z = dotprod(matr.mat[2], (vec3){vec.x, vec.y, vec.z});

return res;
}

void changeVerts(vec3 verts[], mat3 matr, unsigned int n)
{
for (int i = 0; i < n; i++)
    {
    verts[i] = applyMatrix(matr, verts[i]); // applying the transformation
    }
}

void TransformObject(vec3 vertices[], vec2 moveBy, unsigned int n)
{
mat3 matr = {
    (vec3){ 1.0f, 0.0f, moveBy.x },
    { 0.0f, 1.0f, moveBy.y },
    { 0.0f, 0.0f, 1.0f }
};

changeVerts(vertices, matr, n);
}

void ScaleObject(vec3 vertices[], vec2 scale, unsigned int n)
{
mat3 matr = {
    (vec3){ scale.x, 0.0f, 0.0f },
    { 0.0f, scale.y, 0.0f },
    { 0.0f, 0.0f, 1.0f }
};

changeVerts(vertices, matr, n);
}

m4 mat3Tomat4(mat3 matr)
{
return (m4){
    matr.mat[0].x, matr.mat[0].y, matr.mat[0].z, 0.0f,
    matr.mat[1].x, matr.mat[1].y, matr.mat[1].z, 0.0f,
    matr.mat[2].x, matr.mat[2].y, matr.mat[2].z, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};
}

m4 getProjection(unsigned int wid, unsigned int hig, unsigned int transformed)
{
return 
    (m4){
         2 / (float)wid, 0.0f, 0.0f, -(float)(transformed),
         0.0f, 2 / (float)hig, 0.0f, -(float)(transformed),
         0.0f, 0.0f, 1.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    };
}

m4 GetModelMatrix(vec2 pos, vec2 scale, float theta)
{
vec2 transformed = LeftCornerFromCentre(pos, scale);
return (m4){
        scale.x * cos(theta), scale.x * -sin(theta), 0.0f, transformed.x,
        scale.y * sin(theta), scale.y * cos(theta), 0.0f, transformed.y,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
};
}

m4 getM4ID()
{
return (m4){
    {1.0f, 0.0f, 0.0f, 0.0f,
     0.0f, 1.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 1.0f, 0.0f,
     0.0f, 0.0f, 0.0f, 1.0f}
};
}

vec2 GetMousePositionRelative(vec2 cursor, unsigned int width, unsigned int height)
{
return (vec2){cursor.x, height - cursor.y};
}

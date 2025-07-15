#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct vec2 { float x, y; } vec2;
typedef struct vec3 { float x, y, z; } vec3;

typedef struct mat3 { vec3 mat[3]; } mat3;

typedef float vec4[4];
typedef struct m4 { vec4 mat[4]; } m4;

# define M_PI          3.141592653589793238462643383279502884L

/**
 * Outputs a vec2
 * 
 * @param v The vector to output
 */
void OutputVec2(vec2 v);

/**
 * Outputs a vecn
 * 
 * @param n The number of elements
 * @param v The vector to output
 */
void OutputVecn(int n, const float* v);

/**
 * Outputs a matn
 * 
 * @param n The number of rows
 * @param m The number of columns
 * @param A The matrix to output
 */
void OutputMatn(int n, int m, const float** A);

/**
 * Outputs a matn
 * 
 * @param n The number of rows
 * @param m The number of columns
 * @param A The matrix to output
 */
void OutputMat3(mat3 A);

/**
 * Increases the size of the passed array to the new size
 * 
 * @param arr A pointer to the array to expand
 * @param osize The current size of the array
 * @param nsize The new size of the array
 * @param elesize The size of an element in the array
 * 
 * @warning Remember to pass the array as a pointer not just an array
 */
void ExpandArray(void** arr, unsigned int osize, unsigned int nsize, unsigned int elesize);

/**
 * Increases the size of the passed array by one
 * 
 * @param arr A pointer to the array to expand
 * @param size The size of the array
 * @param elesize The size of an element in the array
 * 
 * @warning Remember to pass the array as a pointer not just an array
 */
void ExpandByOne(void** arr, const unsigned int size, unsigned int elesize);

/**
 * Decreases the size of the passed array by one
 * 
 * @param arr A pointer to the array to shrink
 * @param size The size of the array
 * @param elesize The size of an element in the array
 * 
 * @warning Remember to pass the array as a pointer not just an array
 */
void ShrinkArrayByOne(void** arr, const unsigned int size, unsigned int elesize);

/**
 * Finds the next free ID slot starting from 0
 * 
 * @param arr The array
 * @param size The size of the array
 * 
 * @returns The lowest ID which is not used
 */
int findNextIDAvailable(unsigned int* arr, int size);

/**
 * Multiplies a vec2 by a scalar
 * 
 * @param v The vector to multiply
 * @param a The scalar to multiply
 * 
 * @returns The result of the multiplication
 */
vec2 ScalarMultVec2(vec2 v, float a);

/**
 * Multiplies a vec3 by a scalar
 * 
 * @param v The vector to multiply
 * @param a The scalar to multiply
 * 
 * @returns The result of the multiplication
 */
vec3 ScalarMultVec3(vec3 v, float a);

/**
 * Adds the first vector to the second
 * 
 * @param u The first vector
 * @param v The second vector
 * 
 * @returns The sum of both vectors
 */
vec2 addVec2(vec2 u, vec2 v);

vec2 LeftCornerFromCentre(vec2 centre, vec2 scale);

/**
 * Gets the distance between two points
 * 
 * @param a The first point
 * @param b The second point
 * 
 * @returns The distance as a float
 */
float getDistance(vec2 a, vec2 b);

/**
 * Gets the length of a vector
 * 
 * @param v The vector
 * 
 * @returns The magnitude (length) of the vector
 */
float getMagnitude(vec2 v);

/**
 * Checks if point is in the square at position pos with scale scale
 * 
 * @param point The point to check if it is the square
 * @param pos The position of the centre of the square
 * @param scale The scale of the square
 * 
 * @returns 1 if the point is in the square, 0 if not
 */
int PointInSquare(vec2 point, vec2 pos, vec2 scale);

/**
 * Checks if two squares touch
 * 
 * @param p1 The position of the centre of the first square
 * @param s1 The scale of the first square
 * @param p2 The position of the centre of the second square
 * @param s2 The scale of the second square
 * 
 * @returns 1 if they touch, 0 if they don't
 */
int SquareTouchesSquare(vec2 p1, vec2 s1, vec2 p2, vec2 s2);

/**
 * Tests to see if A and B are equivalent
 * 
 * @param A Matrix A
 * @param B Matrix B
 * 
 * @returns 1 if they are the same, 0 if not
 */
int equivMat3(mat3 A, mat3 B);

/**
 * Multiplies a mat3 by a scalar
 * 
 * @param A The matrix to multiply
 * @param a The scalar to multiply
 * 
 * @returns The result of the multiplication
 */
mat3 ScalarMultMat3(mat3 A, float a);

float generalisedDot(float u[], float v[], unsigned int n);
float dotprod(vec3 u, vec3 v);

void transposeMatr3(float m[][3], unsigned int n);

void generalisedMatrMult3(float m1[][3], float m2[][3], unsigned int n, unsigned int m);

vec3 applyMatrix(mat3 matr, vec3 vec);
void changeVerts(vec3 verts[], mat3 matr, unsigned int n);

void TransformObject(vec3 vertices[], vec2 moveBy, unsigned int n);
void ScaleObject(vec3 vertices[], vec2 scale, unsigned int n);

m4 mat3Tomat4(mat3 matr);

/**
 * Gets the projection matrix
 * 
 * @param wid The width of the window
 * @param hig The height of the window
 * @param transformed Should the origin be transformed so that it is in the bottom left
 * 
 * @returns 4x4 projection matrix to be passed to the shader
 */
m4 getProjection(unsigned int wid, unsigned int hig, unsigned int transformed);

/**
 * Gets the model matrix for the (square) entity
 * 
 * @param pos The centre of the entity
 * @param scale The scale vector of the entity
 * @param theta The angle of the object
 * 
 * @note This also gets the bottom left corner of the square within the function
 */
m4 GetModelMatrix(vec2 pos, vec2 scale, float theta);

m4 getM4ID();

vec2 GetMousePositionRelative(vec2 cursor, unsigned int width, unsigned int height);
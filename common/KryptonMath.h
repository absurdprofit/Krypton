/*
Author: Nathan Johnson
Date: August 11, 2020
Email: nahtanjohn1l@gmail.com

The contents of this file make up the Krypton Math Library. The reason I made the decision of writing my own
math library is because Krypton applications under the hood will rely on orthographic projections to render
simple quads to the screen. Therefore one can assume the math involved in such operations should not be overly
complex. As size is one of the platform constraints I decided to write my own lightweight math library instead
of bloating my codebase with a well established math library.
*/
#ifndef __KRYPTON_MATH_H__
#define __KRYPTON_MATH_H__

#include <stdlib.h>
#include <initializer_list>
#include <cstring>
#include <vector>
#include "KryptonUtil.h"

//class to represent the row of a matrix
template <class T, int w>
class Row
{
public:
	Row(T *row)
	{
		_data = row;
	}
	~Row()
	{
		free(_data);
	}
	T operator[](int col)
	{
		return _data[col];
	}
	const int width()
	{
		return _width;
	}

private:
	int _width = w;
	T *_data = (T *)calloc(_width, sizeof(T));
};

//matrix class to represent matrices eg. orthographic projection matrix
//matrix 2d indexing (i.e mat[0][0]) is done by returning a row object that also has a [] operator overload and
//represents a row in the matrix.
//Later idea is to add a transpose function so that matrices can both be represented as column major and row
//major as the user pleases.
template <class T, int w, int h>
class Matrix
{
public:
	Matrix()
	{
	}
	Matrix(T *data)
	{
		_data = data;
	}
	Matrix(std::initializer_list<T> l)
	{
		memcpy(_data, l.begin(), sizeof(T) * l.size());
	}
	~Matrix()
	{
		free(_data);
	}
	const T *data()
	{
		return _data;
	}
	const int width()
	{
		return _width;
	}
	const int height()
	{
		return _height;
	}
	Row<T, w> operator[](int _row)
	{
		T *tmp = (T *)calloc(_width, sizeof(T));
		for (int col = 0; col < _width; col++)
		{
			tmp[col] = _data[_width * _row + col];
		}
		Row<T, w> row(tmp);
		return row;
	}

private:
	int _height = h;
	int _width = w;
	T *_data = (T *)calloc(_width * _height, sizeof(T));
};

//represents vectors e.g. colours, positions etc.
template <class T, int components>
struct Vector
{
	T x;
	T y;
	T z;
	T w;

	Vector(T X = 0, T Y = 0, T Z = 0, T W = 0) : x(X), y(Y), z(Z), w(W)
	{
	}

	T *data()
	{

		switch (components)
		{
		case 4:
		{
			T *_data = (T *)calloc(4, sizeof(T));
			_data[0] = x;
			_data[1] = y;
			_data[2] = z;
			_data[3] = w;
			return _data;
		}

		case 3:
		{
			T *_data = (T *)calloc(3, sizeof(T));
			_data[0] = x;
			_data[1] = y;
			_data[2] = z;
			return _data;
		}

		case 2:
		{
			T *_data = (T *)calloc(2, sizeof(T));
			_data[0] = x;
			_data[1] = y;
			return _data;
		}
		}
	}

	Vector operator*(Matrix<T, 4, 4> &M)
	{
		Vector<T, 3> tmp;
		tmp.x = (M[0][0] * x) + (M[0][1] * y) + (M[0][2] * z) + (M[0][3] * w);
		tmp.y = (M[1][0] * x) + (M[1][1] * y) + (M[1][2] * z) + (M[1][3] * w);
		tmp.z = (M[2][0] * x) + (M[2][1] * y) + (M[2][2] * z) + (M[2][3] * w);
		return tmp;
	}

	bool operator==(const Vector& rhs) {
		if (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w) {
			return true;
		} else {
			return false;
		}
	}

	bool operator>(const Vector& rhs) {
		if (x > rhs.x || y > rhs.y || z > rhs.z || w > rhs.w) {
			return true;
		} else {
			return false;
		}
	}

	T operator[](int index)
	{
		switch (index)
		{
		case 0:
			return x;

		case 1:
			return y;

		case 2:
			return z;

		case 3:
			return w;
		}
	}
};

#define vec3f Vector<float, 3>
#define vec2f Vector<float, 2>
#define vec4f Vector<float, 4>
#define mat4f Matrix<float, 4, 4>
//they all have a x, y, z, w component, this macro is more of a semantic thing
//these macros are designed to be almost symantically symmetrical to the mat and vec types in GLSL.

//represents the vertices of a quad, later will be extended to also have texture IDs and texture
//coordinates for images
struct Vertex
{
	vec4f colour;
	vec3f position;
};

//creates an orthoganol projection matrix, mainly used to set our projection matrix uniform
inline Matrix<float, 4, 4> glOrtho(
	const float &b, const float &t, const float &l, const float &r,
	const float &n, const float &f)
{
	// set OpenGL perspective projection matrix
	Matrix<float, 4, 4> M = {
		2 / (r - l), 0.0f, 0.0f, 0.0f,
		0.0f, 2 / (t - b), 0.0f, 0.0f,
		0.0f, 0.0f, -2 / (f - 2), 0.0f,
		-(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1.0f};

	return M;
}

//manage vertices and indices
class Geometry
{
private:
	GLuint _numQuadVertices = 0;
	GLuint _numRoundVertices = 0;
	const GLuint _maxVertices = 15000;
	const GLuint _numRoundSegments = 499;
	GLuint _numQuadIndices = 0;
	GLuint _numRoundIndices = 0;
	GLuint _batchVBO;
	GLuint _batchIBO;
	std::vector<GLuint> _quad_indices;
	std::vector<Vertex> _quad_vertices;
	std::vector<Vertex> _round_vertices;
	std::vector<GLuint> _round_indices;
public:
	Geometry(ContextData*);
	void addQuadVertices(int);
	void addRoundVertices(int);
	void addQuadIndices(int);
	void addRoundIndices(int);
	void addRoundVertex(int, Vertex);
	void addQuadVertex(int, Vertex);
	const GLuint numQuadVertices();
	void numQuadVertices(GLuint);
	const GLuint numRoundVertices();
	void numRoundVertices(GLuint);
	const GLuint maxVertices();
	const GLuint numRoundSegments();
	const GLuint numQuadIndices();
	void numQuadIndices(GLuint);
	const GLuint numRoundIndices();
	void numRoundIndices(GLuint);
	const GLuint VBO();
	const GLuint IBO();
	const GLuint* quadIndicesData();
	const GLuint quadIndicesSize();
	const GLuint* roundIndicesData();
	const GLuint roundIndicesSize();
	const Vertex* quadVerticesData();
	const GLuint quadVerticesSize();
	const Vertex* roundVerticesData();
	const GLuint roundVerticesSize();
};

#endif
#pragma once
#include <ctime>
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>
#include <ostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak()

#ifdef _DEBUG
#define MDEBUG
#endif


#else
#define DEBUG_BREAK __builtin_trap()

#ifndef NDEBUG
#define MDEBUG
#endif

#endif

//#define _DEBUG

#define ASSERT(x) \
	if (!(x))     \
		DEBUG_BREAK;

//#ifdef _DEBUG
//#define GLcall_P(y, x) \
//	GLClearError();    \
//	y = x;             \
//	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
//#define GLcall(x)   \
//	GLClearError(); \
//	x;              \
//	ASSERT(GLLogCall(#x, __FILE__, __LINE__))
//#else
//#define GLcall(x) x
//#define GLcall_P(y, x) y = x
//#endif

#define GLcall(x) x
#define GLcall_P(y, x) y = x

inline void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
		;
}

inline bool GLLogCall(const char *function, const char *file, int line)
{
	while (GLenum errorcode = glGetError())
	{
		std::string error;
		switch (errorcode)
		{
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		std::cout << "[Opengl Error] ( Error code :" << errorcode << " : " << error << " )" << function << " " << file << " : " << line << std::endl;
		return false;
	}
	return true;
}

typedef unsigned int uint;

//template<typename T,unsigned M,unsigned N>
//struct mat {
//	std::array<std::array<T, M>, N> m_buffer;
//
//
//	mat() {
//		for (size_t i = 0; i < N; i++) {
//			for (size_t j = 0; j < M; j++) {
//				if (i == j)
//					m_buffer[i][j] = 1;
//				else
//					m_buffer[i][j] = 0;
//			}
//		}
//	}
//
//	mat(T value) {
//		for (size_t i = 0; i < N; i++) {
//			for (size_t j = 0; j < M; j++) {
//					m_buffer[i][j] = value;
//			}
//		}
//	}
//
//	static mat<T,N,M> transpose(mat<T,M,N> in) {
//
//		mat<T, N, M> out;
//
//		for (size_t i = 0; i < in.m_buffer.size(); i++) {
//			for (size_t j = 0; j < in.m_buffer[i].size(); j++) {
//				out.m_buffer[j][i] = in.m_buffer[i][j];
//			}
//		}
//		return out;
//
//	}
//
//	std::array<T, M>& operator[](unsigned index) { return m_buffer[index]; }
//
//
//	friend std::ostream& operator<< (std::ostream& out, const mat<T,M,N>& in) {
//
//
//
//		out << mat::transpose(in) << std::endl;
//
//		for (size_t i = 0; i < in.m_buffer.size(); i++) {
//			out << "( ";
//			for (size_t j = 0; j < in.m_buffer[i].size(); j++) {
//				std::cout << in.m_buffer[i][j] << "	";
//			}
//			out << " )\n";
//		}
//		return out;
//	}
//
//};

struct vec3
{
	float x, y, z;

	vec3() { x = y = z = 0; }
	vec3(float a) : x(a), y(a), z(a) {}
	vec3(float a, float b, float c) : x(a), y(b), z(c) {}
	vec3(const glm::vec3 &in) : x(in.x), y(in.y), z(in.z) {}
	vec3(glm::vec3 &in) : x(in.x), y(in.y), z(in.z) {}

	float getmag()
	{
		return sqrtf(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}

	operator glm::vec4()
	{
		return glm::vec4(x, y, z, 1.0);
	}

	operator glm::vec3()
	{
		return glm::vec3(x, y, z);
	}

	friend vec3 operator+(const vec3 &a, const vec3 &b)
	{
		return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	friend vec3 operator+(vec3 in, float a)
	{
		return vec3(in.x + a, in.y + a, in.z + a);
	}

	friend vec3 operator-(const vec3 &a, const vec3 &b)
	{
		return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	friend vec3 operator-(vec3 in, float a)
	{
		return vec3(in.x - a, in.y - a, in.z - a);
	}

	friend vec3 operator*(const vec3 &a, const vec3 &b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	friend vec3 operator*(vec3 in, float a)
	{
		return vec3(in.x * a, in.y * a, in.z * a);
	}

	friend vec3 operator/(const vec3 &a, const vec3 &b)
	{
		return vec3(a.x / b.x, a.y / b.y, a.z / b.z);
	}

	friend vec3 operator/(vec3 in, float a)
	{
		return vec3(in.x / a, in.y / a, in.z / a);
	}

	vec3 operator-()
	{
		return vec3(-x, -y, -z);
	}

	vec3 operator+=(vec3 in)
	{
		x += in.x;
		y += in.y;
		z += in.z;
		return *this;
	}

	vec3 operator-=(vec3 in)
	{
		x -= in.x;
		y -= in.y;
		z -= in.z;
		return *this;
	}

	vec3 operator*=(vec3 in)
	{
		x *= in.x;
		y *= in.y;
		z *= in.z;
		return *this;
	}

	vec3 operator/=(vec3 in)
	{
		x /= in.x;
		y /= in.y;
		z /= in.z;
		return *this;
	}

	friend std::ostream &operator<<(std::ostream &out, const vec3 &vec)
	{
		out << "( " << vec.x << ", " << vec.y << ", " << vec.z << " )\n";
		return out;
	}

	vec3 normalize()
	{
		float temp = sqrt(x * x + y * y + z * z);
		*this = *this / temp;
		return *this;
	}

	static vec3 normalize(const vec3 &in)
	{
		float temp = sqrt(in.x * in.x + in.y * in.y + in.z * in.z);
		return vec3(in / temp);
	}

	static float dist(const vec3 &a, const vec3 &b)
	{
		return sqrtf(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
	}

	static vec3 cross(const vec3 &a, const vec3 &b)
	{
		vec3 ret;
		ret.x = a.y * b.z - b.y * a.z;
		ret.y = b.x * a.z - a.x * b.z;
		ret.z = a.x * b.y - b.x * a.y;
		return ret.normalize();
	}
};

struct vec4
{
	float x, y, z, w;

	vec4() {}
	vec4(float a) : x(a), y(a), z(a), w(a) {}
	vec4(float a, float b) : x(a), y(a), z(a), w(b) {}
	vec4(float a, float b, float c) : x(a), y(b), z(c), w(1) {}
	vec4(float a, float b, float c, float d) : x(a), y(b), z(c), w(d) {}
	vec4(const vec3 &a) : x(a.x), y(a.y), z(a.z), w(1.0) {}

	operator glm::vec4()
	{
		return glm::vec4(x, y, z, w);
	}

	vec4 operator-()
	{
		return vec4(-x, -y, -z, -w);
	}
};

struct vec2
{
	float x, y;

	vec2() {}
	vec2(float a) : x(a), y(a) {}
	vec2(float a, float b) : x(a), y(b) {}
	vec2(glm::vec2 a) : x(a.x), y(a.y) {}

	float getmag()
	{
		return sqrtf(pow(x, 2) + pow(y, 2));
	}

	operator glm::vec2()
	{
		return glm::vec2(x, y);
	}

	operator glm::vec3()
	{
		return glm::vec3(x, y, 0);
	}
};

//struct mat4 {
//	vec4 m_buffer[4];
//
//	mat4() {
//		for (size_t i = 0; i < 4; i++) {
//			for (size_t j = 0; j < 4; j++) {
//				if (i == j)
//					m_buffer[i][j] = 1;
//				else
//					m_buffer[i][j] = 0;
//			}
//		}
//	}
//
//	vec4& operator[](unsigned index) { return m_buffer[index]; }
//
//
//	static mat4 translate(vec3 in) {
//		mat4 out;
//		for (size_t i = 0; i < 4; i++)	{
//			for (size_t j = 0; j < 4; j++) {
//				if(i==j)
//					out[i][j] = 1;
//				else
//					out[i][j] = 0;
//			}
//		}
//
//		out[3][0] = in[0];
//		out[3][1] = in[1];
//		out[3][2] = in[2];
//		out[3][3] = 1;
//		return out;
//	}
//
//	mat4 operator*(mat4 in) {
//		mat4 out;
//		float sum(0);
//
//		for (int i = 0; i < 4; i++)
//		{
//			for (int j = 0; j < 4; j++)
//			{
//				for (int k = 0; k < 4; k++)
//				{
//					sum = sum + m_buffer[k][j] * in.m_buffer[i][k];
//				}
//
//				out[i][j] = sum;
//				sum = 0;
//			}
//		}
//		return out;
//	}
//
//	friend std::ostream& operator<< (std::ostream& out, mat4 in) {
//		out << "( " << in[0][0] << "	" << in[1][0] << "	" << in[2][0] << "	" << in[3][0] << " )\n";
//		out << "( " << in[0][1] << "	" << in[1][1] << "	" << in[2][1] << "	" << in[3][1] << " )\n";
//		out << "( " << in[0][2] << "	" << in[1][2] << "	" << in[2][2] << "	" << in[3][2] << " )\n";
//		out << "( " << in[0][3] << "	" << in[1][3] << "	" << in[2][3] << "	" << in[3][3] << " )\n";
//		return out;
//	}
//
//};

//struct assimpVertex {
//	vec3 position;
//	vec3 normal;
//	vec2 tex_coord;
//	float tex_id;
//
//
//	assimpVertex(const vec3& pos,  const vec3& nor = 0, const vec2& tex = 0, const float texid = 0)
//		:position(pos), normal(nor), tex_coord(tex), tex_id(texid) {}
//
//	assimpVertex() {}
//
//	friend std::ostream& operator<< (std::ostream& out, assimpVertex in) {
//		out << "position =";
//		out << "( " << in.position.x << ", " << in.position.y << ", " << in.position.z << ")\n";
//		out << "normal =";
//		out << "( " << in.normal.x << ", " << in.normal.y << ", " << in.normal.z << ")\n";
//		out << "tex Coord =";
//		out << "( " << in.tex_coord.x << ", " << in.tex_coord.y << ")\n";
//		return out;
//	}
//};


struct Vertex2
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
	vec3 tangent;
	vec3 bitangent;

	Vertex2(const vec3& pos, const vec3& nor = 0, const vec2& texcoord = 0, const vec3& tangent = 0, const vec3& bitangent = 0, const float texid = 0)
		: position(pos), normal(nor), tangent(tangent), bitangent(bitangent), texCoord(texcoord) {}
	Vertex2() {}
};


struct Vertex
{
	vec3 position;
	vec4 color;
	vec3 normal;
	vec2 texCoord;
	float tex_id;

	Vertex(const vec3 &pos, const vec4 &col = 0, const vec3 &nor = 0, const vec2 &tex = 0, const float texid = 0)
		: position(pos), color(col), normal(nor), texCoord(tex), tex_id(texid) {}
	Vertex() {}
	Vertex(const Vertex2 &in) :position(in.position),normal(in.normal),texCoord(in.texCoord),color(1,0,0){}



	friend std::ostream &operator<<(std::ostream &out, Vertex in)
	{
		out << "position =";
		out << "( " << in.position.x << ", " << in.position.y << ", " << in.position.z << ")\n";
		out << "color =";
		out << "( " << in.color.x << ", " << in.color.y << ", " << in.color.z << ", " << in.color.w << ")\n";
		return out;
	}
};


struct Indexdata
{
	std::vector<uint> primitive;
	vec4 color;
	std::vector<vec2> texCoord;
	float tex_id;

	Indexdata(std::vector<uint> indices, const vec4 col, std::vector<vec2> tex = std::vector<vec2>(0, 0),
			  float id = 0)
		: primitive(std::move(indices)), color(col), texCoord(std::move(tex)), tex_id(id)
	{
		if (texCoord.empty())
			tex_id = 0;
		else if (tex_id == 0)
			tex_id = 1;
	}
};

struct pointLight
{
	vec3 position;
	float intensity;
	vec3 diffusecolor;
	vec3 specularcolor;

	float constant;
	float linear;
	float quadratic;
	pointLight(const vec3 &pos, const float &intensity, const vec3 &diffcol = 1, const vec3 &speccol = 1) : position(pos), diffusecolor(diffcol), specularcolor(speccol), intensity(intensity) {}
};

struct DirLight
{
	vec3 direction;
	float intensity;

	vec3 diffusecolor;
	vec3 specularcolor;
	DirLight(const vec3 &dir, const float &intensity, const vec3 &diffcol = 1, const vec3 &speccol = 1) : direction(dir), diffusecolor(diffcol), specularcolor(speccol), intensity(intensity) {}
};

struct Material
{
	float AmbientStrength;
	float DiffuseStrength;
	float SpecularStrength;
	vec3 specularColor;
	float shininess;

	Material(const float shine, const vec3 &speccol = 1) : shininess(shine), AmbientStrength(0.1), DiffuseStrength(1), SpecularStrength(1), specularColor(speccol)
	{
	}
};

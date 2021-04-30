#pragma once

#include <utility>

#include "core.h"
#include "Vertexarray.h"
#include "buffer.h"
#include "Texture.h"
#include "Shader.h"

inline vec3 operator*(glm::mat4 mat, vec3 vec)
{
	glm::vec4 temp(vec.x, vec.y, vec.z, 1);
	temp = mat * temp;
	return vec3(temp.x, temp.y, temp.z);
}

class Lines;
template <class T> class Drawable;

template <class T>
class Drawable
{
protected:
	buffer<T, GL_ARRAY_BUFFER> m_vbo;
	Vertexarray m_vao;
	buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER> m_ibo;
	unsigned m_primitve = GL_TRIANGLES;
	std::vector<Texture> m_tex;
	Lines *normals = nullptr;

	virtual void preDraw(Shader &shader)
	{
		shader.Bind();
		matModel = translation * rotation * scaling;
		shader.SetUniform<glm::mat4 *>("model", &matModel);

		m_vao.Bind();
		m_ibo.Bind();
		for (int i = 0; i < m_tex.size(); i++)
			m_tex[i].Bind(i);
	}

	virtual void postDraw()
	{
		for (int i = 0; i < m_tex.size(); i++)
			m_tex[i].Unbind(i);
		Vertexarray::Unbind();
	}

	//void initNormals()
	//{
	//	std::vector<Vertex> lineVertices;
	//	std::vector<uint32_t> lineIndices;
	//	auto verts = m_vbo.getdata();
	//	lineVertices.reserve(verts.size() * 2);
	//	lineIndices.reserve(m_vbo.get_count() * 2);
	//	size_t indexCount = 0;
	//	for (size_t i = 0; i < verts.size(); i++)
	//	{
	//		lineVertices.emplace_back(verts[i].position, vec4(1));
	//		lineIndices.emplace_back(indexCount++);
	//		lineIndices.emplace_back(indexCount++);
	//		lineVertices.emplace_back(verts[i].position + verts[i].normal, vec4(1));
	//	}
	//	normals = new Lines(lineVertices, lineIndices);
	//}

public:
	glm::mat4 scaling = glm::mat4(1.0);
	glm::mat4 translation = glm::mat4(1.0);
	glm::mat4 rotation = glm::mat4(1.0);
	glm::mat4 matModel = glm::mat4(1.0);
	/*bool drawNormals = false;
	Shader *normalShader = nullptr;
	float normalLength = 1.0f;*/

	Drawable() = default;

	Drawable(const std::vector<T> &vertices, const std::vector<unsigned> &indices, const std::vector<Texture> &tex = std::vector<Texture>()) : m_vbo(vertices), m_ibo(indices), matModel(glm::mat4(1)), m_tex(std::move(tex))
	{
		m_vao.init();
		m_vao.AddBuffer(m_vbo, m_ibo);

#ifdef MDEBUG
		//initNormals();
#endif                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
	}

	Drawable(const std::vector<vec3> &positions, const std::vector<Indexdata> &indices) : m_vbo(std::vector<T>())
	{
		std::vector<T> m_vertices;
		std::vector<uint32_t> m_indices;

		uint totalindexcount = 0;
		uint totalvertexcount = 0;
		for (auto &index : indices)
		{
			if (index.primitive.size() < 3)
			{
				printf("index data not valid");
				assert(false);
			}
			totalindexcount += (index.primitive.size() - 2) * 3;
			for (size_t j = 0; j < index.primitive.size(); j++)
				totalvertexcount++;
		}

		m_vertices.reserve(totalvertexcount);
		m_indices.reserve(totalindexcount);

		for (auto &index : indices)
		{
			if (!index.texCoord.empty())
			{
				if (index.texCoord.size() != index.primitive.size())
				{
					std::cout << "texture coord not defined for all indices\n";
					assert(false);
				}
				if (index.tex_id == 0)
					std::cout << "texture id set to zero so texture not used";
			}
			else if (index.tex_id == 0)
			{
				std::cout << "the drawable doesnot have textures\n";
				//index.texCoord = std::vector<vec2>(size, 0);
			}

			for (size_t j = 0; j < index.primitive.size(); j++)
			{
				int pos_index = index.primitive[j];
				T v1(positions[pos_index], index.color, vec3(0), index.texCoord[j], index.tex_id);
				m_vertices.push_back(v1);
			}
		}

		unsigned count = 0;

		for (auto &index : indices)
		{
			int size = index.primitive.size();
			int temp = 0;
			for (uint32_t j = count; j < count + size - 2; j++)
			{
				m_indices.push_back(count);
				m_indices.push_back(j + 1);
				m_indices.push_back(j + 2);
				temp++;
				vec3 p1 = m_vertices[count].position;
				vec3 p2 = m_vertices[j + 1].position;
				vec3 p3 = m_vertices[j + 2].position;
				vec3 u = p2 - p1;
				vec3 v = p3 - p2;
				vec3 w = p1 - p3;
				m_vertices[count].normal = vec3::cross(v, w);
				m_vertices[j + 1].normal = vec3::cross(w, u);
				m_vertices[j + 2].normal = vec3::cross(u, v);
			}
			count = count + temp + 2;
		}

		m_vbo.AddData(m_vertices);
		m_ibo.AddData(m_indices);
		m_vao.init();
		m_vao.AddBuffer<T>(m_vbo, m_ibo);

#ifdef MDEBUG
		//initNormals();
#endif // MDEBUG

		Vertexarray::Unbind();
	}

	Drawable(Drawable &&in) noexcept : m_vao(std::move(in.m_vao)), m_vbo(std::move(in.m_vbo)), m_ibo(std::move(in.m_ibo)), m_tex(std::move(in.m_tex))
	{
		m_primitve = in.m_primitve;
	}

	Drawable(const Drawable &in) = delete;

	~Drawable()
	{
		for (auto &a : m_tex)
		{
			a.free();
		}
		
	}

	void Draw(Shader &shader)
	{
		preDraw(shader);
		GLcall(glDrawElements(m_primitve, m_ibo.get_count(), GL_UNSIGNED_INT, nullptr));
		postDraw();
		
	}

	const std::vector<T> &getvertices() { return m_vbo.getdata(); };

	const std::vector<uint32_t> &getindices() { return m_ibo.getdata(); }

	const std::vector<Texture> &getTextures() { return m_tex; }


	void addTexture(const Texture &in)
	{
		m_tex.emplace_back(in);
	}
};

class Lines : public Drawable<Vertex>
{
protected:
	virtual void preDraw(Shader &shader)
	{
		if (width != 1)
			glLineWidth(width);
		shader.Bind();
		m_vao.Bind();
		m_ibo.Bind();
		glLineWidth(1);
	}
	virtual void postDraw(Shader &shader)
	{
		if (width != 1)
			glLineWidth(1);
		Vertexarray::Unbind();
	}

public:
	unsigned width = 1;

	Lines() = default;

	Lines(const std::vector<Vertex> &vertices, std::vector<uint32_t> indices = std::vector<uint32_t>())
	{

		m_vbo.setup(vertices);
		if (indices.empty())
		{
			indices.resize(vertices.size() * 2);
			indices[0] = 0;
			for (int i = 1; i < indices.size() * 2; i++)
			{
				indices[i] = (i + 1) / 2;
			}
		}

		m_ibo.setup(indices);
		m_vao.init();
		m_vao.AddBuffer(m_vbo, m_ibo);

		m_vao.Unbind();

		Drawable::m_primitve = GL_LINES;
	}
};
#pragma once

#include <vector>
#include "VertexBufferLayout.h"
#include "buffer.h"

class Vertexarray
{
private:
	unsigned m_arrayID = 0;
	unsigned attrib_count;
	VertexBufferLayout m_layout;

	void m_AddBuffer(const unsigned &vbo_id, const unsigned &ibo_id)
	{
		if (!m_arrayID)
		{
			init();
		}
		Bind();
		glBindVertexArray(m_arrayID);
		GLcall(glBindBuffer(GL_ARRAY_BUFFER, vbo_id));
		GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_id));
		const std::vector<VertexBufferElement> &elements = m_layout.GetElements();
		unsigned offset = 0;
		unsigned i = 0;
		while (i < elements.size())
		{
			const VertexBufferElement &element = elements[i];
			GLcall(glEnableVertexAttribArray(i));
			GLcall(glVertexAttribPointer(i, element.count, element.type, element.normalized, m_layout.GetStride(), (const void *)offset));
			offset += element.count * element.GetTypeSize();
			i++;
		}
		attrib_count = i;
		glBindVertexArray(0);
		GLcall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLcall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	static void createarray(unsigned &id)
	{

		// std::cout << "vao created\n";
		GLcall(glGenVertexArrays(1, &id));
	}

	static void deletearray(unsigned &id)
	{

		// std::cout << "vao freed\n";
		GLcall(glDeleteVertexArrays(1, &id));
	}

public:
	//explicit Vertexarray() : attrib_count(0), m_arrayID(0)
	//{
	//	//std::cout << "vao created\n";
	//	GLcall(glGenVertexArrays(1, &m_arrayID));
	//	//GLcall(glBindVertexArray(m_arrayID));
	//}

	Vertexarray() = default;

	void init()
	{
		glDeleteVertexArrays(1, &m_arrayID);
		GLcall(glGenVertexArrays(1, &m_arrayID));
	}

	~Vertexarray()
	{
		// std::cout << "vao freed\n";
		GLcall(glDeleteVertexArrays(1, &m_arrayID));
		m_arrayID = 0;
	}

	template <class T>
	void AddBuffer(const buffer<T, GL_ARRAY_BUFFER> &vbo, const buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER> &ibo) { assert(false); }

#ifdef _MSC_VER
	template <>
	void AddBuffer<Vertex>(const buffer<Vertex, GL_ARRAY_BUFFER> &vbo, const buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER> &ibo);
#endif

	void AddBufferf(const buffer<float, GL_ARRAY_BUFFER> &vb, const VertexBufferLayout &layout)
	{
		Bind();
		vb.Bind();
		const std::vector<VertexBufferElement> &elements = layout.GetElements();
		unsigned offset = 0;
		unsigned i = 0;
		while (i < elements.size())
		{

			const VertexBufferElement &element = elements[i];
			GLcall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));
			GLcall(glEnableVertexAttribArray(i));
			offset += element.count * element.GetTypeSize();
			i++;
		}
		attrib_count = i;
		vb.Unbind();
		Unbind();
	}

	void Bind() const
	{

		GLcall(glBindVertexArray(m_arrayID));
	}

	static void Unbind()
	{
		GLcall(glBindVertexArray(0));
		/*for (unsigned i = 0; i < attrib_count; i++)
		{
		GLcall(glDisableVertexAttribArray(i));
		}*/
	}

	void Unbind2(const unsigned &position) const
	{
		GLcall(glBindVertexArray(0));
		GLcall(glDisableVertexAttribArray(position));
	}

	unsigned get() const { return m_arrayID; }

	Vertexarray(Vertexarray &&in) noexcept : m_layout(std::move(in.m_layout))
	{
		m_arrayID = in.m_arrayID;
		attrib_count = in.attrib_count;
		in.m_arrayID = 0;
	}

	Vertexarray(Vertexarray &in) = delete;
};

template <>
inline void Vertexarray::AddBuffer<Vertex>(const buffer<Vertex, GL_ARRAY_BUFFER> &vbo, const buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER> &ibo)
{
	m_layout.push<float>(3, GL_FALSE);
	m_layout.push<float>(4, GL_FALSE);
	m_layout.push<float>(3, GL_FALSE);
	m_layout.push<float>(2, GL_FALSE);
	m_layout.push<float>(1, GL_FALSE);
	m_AddBuffer(vbo.getID(), ibo.getID());
}

//template<>
//inline void Vertexarray::AddBuffer<assimpVertex>(const buffer<assimpVertex, GL_ARRAY_BUFFER>& vbo, const buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER>& ibo) {
//	m_layout.push<float>(3, GL_FALSE);
//	m_layout.push<float>(3, GL_FALSE);
//	m_layout.push<float>(2, GL_FALSE);
//	m_AddBuffer(vbo.getID(), ibo.getID());
//}

template <>
inline void Vertexarray::AddBuffer<Vertex2>(const buffer<Vertex2, GL_ARRAY_BUFFER> &vbo, const buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER> &ibo)
{
	m_layout.push<float>(3, GL_FALSE);
	m_layout.push<float>(3, GL_FALSE);
	m_layout.push<float>(2, GL_FALSE);
	m_layout.push<float>(3, GL_FALSE);
	m_layout.push<float>(3, GL_FALSE);
	m_AddBuffer(vbo.getID(), ibo.getID());
}

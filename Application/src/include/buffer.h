#pragma once
#include <vector>
#include <cstring>
#include "core.h"

template <class T, uint32_t TYPE>
class buffer
{
private:
	unsigned m_bufferID;

#ifdef MDEBUG
	std::vector<T> m_data;
#endif

	uint32_t m_count;

	void createbuffer(unsigned &id)
	{
		//std::cout << "buffer created\n";
		GLcall(glGenBuffers(1, &id));
	}

	void deletebuffer(unsigned &id)
	{
		// std::cout << "buffer freed\n";
		GLcall(glDeleteBuffers(1, &id));
	}

public:

	void setup(const std::vector<T> &data)
	{

		m_count = data.size();
#ifdef MDEBUG

		m_data = data;
		/* m_data.resize(count);
		std::copy(data, data + count, m_data.begin());*/
		m_data.shrink_to_fit();

#endif

		if (m_bufferID)
		{
			deletebuffer(m_bufferID);
		}

		createbuffer(m_bufferID);
		GLcall(glBindBuffer(TYPE, m_bufferID));

		GLcall(glBufferData(TYPE, sizeof(T) * data.size(), &data[0], GL_DYNAMIC_DRAW));
		GLcall(glBindBuffer(TYPE, 0));
	}

	const std::vector<T> &getdata()
	{

#ifdef MDEBUG
		return m_data;
#else
		throw std::exception("Vertexbuffer data not availabele in release");
		return std::vector<T>();
#endif

	}

	buffer() : m_bufferID(0), m_count(0) {}

	buffer(const std::vector<T> &data)
	{
		if (data.empty())
		{
			std::cout << "buffer tried to initialize with empty vertex data\n";
			m_bufferID = 0;
			m_count = 0;
			return;
		}

		setup(data);
	}

	buffer(buffer &&in) noexcept
	{
		m_bufferID = std::move(in.m_bufferID);
		in.m_bufferID = 0;

#ifdef MDEBUG
		m_data = std::move(in.m_data);
#endif

		m_count = std::move(in.m_count);
	}

	buffer(buffer &in) = delete;

	~buffer()
	{
		deletebuffer(m_bufferID);
		m_bufferID = 0;
	}

	void Bind() const
	{
		GLcall(glBindBuffer(TYPE, m_bufferID));
	}
	void Unbind() const
	{
		GLcall(glBindBuffer(TYPE, 0));
	}

	void AddData(const std::vector<T> &data)
	{
		if (data.empty())
		{
			std::cout << "data tried to be added into buffer with empty data\n";
			assert(false);
			return;
		}

		if (m_bufferID == 0)
		{
			setup(data);
		}
		else
		{
			auto newsize = m_count + data.size();

#ifdef MDEBUG
			m_data.resize(m_data.size() + data.size());
			m_data.insert(m_data.end(), data.begin(), data.end());
			m_count = m_data.size();
#endif

			GLcall(glBindBuffer(TYPE, m_bufferID));
			GLcall(void *pointer = glMapBuffer(TYPE, GL_READ_ONLY));
			int buffersize;
			GLcall(glGetBufferParameteriv(TYPE, GL_BUFFER_SIZE, &buffersize));

#ifdef _MSC_VER
			void *tempdata = _malloca(m_count * sizeof(T));
			memcpy_s(tempdata, m_count * sizeof(T), pointer, m_count * sizeof(T));
#else
			void *tempdata = malloc(buffersize);
			memcpy(tempdata, pointer, buffersize);
#endif
			deletebuffer(m_bufferID);
			createbuffer(m_bufferID);
			GLcall(glBindBuffer(TYPE, m_bufferID));
			GLcall(glBufferData(TYPE, buffersize + data.size() * sizeof(T), nullptr, GL_DYNAMIC_DRAW));
			GLcall(glBufferSubData(TYPE, 0, buffersize, tempdata));
			GLcall(glBufferSubData(TYPE, buffersize, sizeof(T) * data.size(), &data[0]));
			GLcall(glBindBuffer(TYPE, 0));
		}
	}

	inline uint32_t get_count() const { return m_count; }

	[[nodiscard]] unsigned getID() const { return m_bufferID; }
	[[nodiscard]] bool isempty() const { return m_count == 0; }
};

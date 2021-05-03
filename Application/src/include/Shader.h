#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include "core.h"


class Shader
{
private:
	std::string m_vertex;
	std::string m_fragment;
	std::string m_path;
	std::string m_vertpath;
	std::string m_fragpath;
	unsigned m_programID;
	std::unordered_map<std::string, int> m_UniformLocationCache;


	static void getstring(const std::string& path, std::string& buffer)
	{
		FILE* filePointer;
		char c;
		int c_count = 0;

#ifdef _MSC_VER
		fopen_s(&filePointer, path.c_str(), "r");
#else
		filePointer = fopen(path.c_str(), "r");
#endif
		if (filePointer == NULL)
		{
			printf("shader file failed to open: %s\n", path.c_str());
			DEBUG_BREAK;
			assert(false);
		}
		else
		{
			while ((c = fgetc(filePointer)) != EOF)
			{
				c_count++;
			}
			buffer.reserve(c_count);
			fseek(filePointer, 0, SEEK_SET);

			while ((c = fgetc(filePointer)) != EOF)
				buffer.push_back(c);

			fclose(filePointer);
		}
	}
	
	unsigned CreateShader()
	{
		GLcall(unsigned int programID = glCreateProgram());
		unsigned vs = CompileShader(GL_VERTEX_SHADER, m_vertex);
		unsigned fs = CompileShader(GL_FRAGMENT_SHADER, m_fragment);

		GLcall(glAttachShader(programID, vs));
		GLcall(glAttachShader(programID, fs));
		GLcall(glLinkProgram(programID));

		int link_status;
		GLcall(glGetProgramiv(programID, GL_LINK_STATUS, &link_status));
		if (link_status == GL_FALSE)
		{
			int length;
			int retlength;
			GLcall(glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &length));
			//char* message = (char*)_malloca(length * sizeof(char));
			std::string msg;
			GLcall(glGetProgramInfoLog(programID, length, &retlength, &msg[0]));
			std::cout << "Failed to link program" << std::endl;
			//std::cout << message << std::endl;
			std::cout << msg << std::endl;
			GLcall(glDeleteProgram(programID));
			DEBUG_BREAK;
			assert(false);
		}

		GLcall(glValidateProgram(programID));

		GLcall(glDeleteShader(vs));
		GLcall(glDeleteShader(fs));
		return programID;
	}
	
	static unsigned CompileShader(unsigned int type, const std::string& source)
	{
		GLcall(unsigned id = glCreateShader(type));
		const char* src = source.c_str();
		GLcall(glShaderSource(id, 1, &src, nullptr));
		GLcall(glCompileShader(id));

		int result;
		GLcall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

		if (result == GL_FALSE)
		{
			int length;
			GLcall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
#ifdef _MSC_VER
			char* message = (char*)_malloca(length * sizeof(char));
#else
			char* message = (char*)malloc(length * sizeof(char));
#endif
			GLcall(glGetShaderInfoLog(id, length, &length, message));
			std::cout << "falied to comiple" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
			std::cout << message << std::endl;
			GLcall(glDeleteShader(id));
			DEBUG_BREAK;
			assert(false);
		}

		return id;
	}
	
	void setshader(const std::string& path)
	{
		enum class status
		{
			vertex,
			fragment,
			idle
		};

		FILE* filePointer;

		char c;
		char previous_c = '\0';

		int w_count = 0;
		int c_count = 0;
		int w_size = 0;

		int temp_space = 0;

		status s = status::idle;

#ifdef _MSC_VER
		fopen_s(&filePointer, path.c_str(), "r");
#else
		filePointer = fopen(path.c_str(), "r");
#endif

		std::string test1 = "#shader vertex\n";
		std::string test2 = "#shader fragment\n";

		std::string temp;

		temp.reserve(sizeof(char) * test2.size());

		if (filePointer == nullptr)
		{
			printf("shader file failed to open\n");
			DEBUG_BREAK;
			assert(false);
		}
		else
		{
			while ((c = fgetc(filePointer)) != EOF)
			{

				c_count++;

				if (w_size <= temp.size())
					temp.push_back(c);

				w_size++;

				if (s == status::vertex)
				{
					m_vertex.push_back(c);
				}
				else if (s == status::fragment)
				{
					m_fragment.push_back(c);
				}

				if (c == ' ' || c == '\n')
				{

					if (previous_c != ' ' && previous_c != '\n')
						temp_space++;
					else
					{
						if (s == status::vertex)
						{
							m_vertex.pop_back();
							if (!temp.empty())
								temp.pop_back();
						}
						else if (s == status::fragment)
						{
							m_fragment.pop_back();
							if (!temp.empty())
								temp.pop_back();
						}
					}
				}

				if (temp_space == 2 || c == '\n')
				{

					if (temp == test2)
					{
						m_vertex.erase(m_vertex.find(test2));
						s = status::fragment;
					}
					else if (temp == test1)
					{
						s = status::vertex;
					}
					temp.clear();

					w_size = 0;
					w_count++;
					temp_space = 0;
				}

				previous_c = c;
			}

			fclose(filePointer);
		}
	}
	
	void setshader2(const std::string& vert, const std::string& frag)
	{
		getstring(vert, m_vertex);
		getstring(frag, m_fragment);
	}
	
public:
	int GetUniformLocation(const char* name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];

		GLcall(int location = glGetUniformLocation(m_programID, name));
		if (location == -1)
			std::cout << "Warning: uniform " << name << " does not exist !" << std::endl;

		m_UniformLocationCache[name] = location;
		return location;
	}
	
	void SetUniform1i(const char* name, int value)
	{
		GLcall(glUniform1i(GetUniformLocation(name), value));
	}
	
	Shader(const std::string& path) : m_programID(0), m_path(path)
	{
		setshader(path);
		m_programID = CreateShader();
	}
	
	Shader(const std::string& vert, const std::string& frag) : m_path(vert), m_programID(0)
	{
		setshader2(vert, frag);
		m_programID = CreateShader();
	}
	
	Shader(Shader&& in) noexcept
	{
		m_programID = in.m_programID;
		in.m_programID = 0;
		m_UniformLocationCache = std::move(in.m_UniformLocationCache);
		m_vertex = std::move(in.m_vertex);
		m_fragment = std::move(in.m_fragment);
		m_path = std::move(in.m_path);
		m_vertpath = std::move(in.m_vertpath);
		m_fragpath = std::move(in.m_fragpath);
	}
	
	~Shader()
	{
		// std::cout << "shader deleted\n";
		GLcall(glUseProgram(0));
		GLcall(glDeleteProgram(m_programID));
		m_programID = 0;
	}
	
	void Bind() const { GLcall(glUseProgram(m_programID)); }
	
	static void Unbind() { GLcall(glUseProgram(0)); }

	template<class T>
	void SetUniform(const char* name, T value, unsigned count = 1);

#ifdef _MSC_VER
	template<>
	void SetUniform<int*>(const char* name, int* value, unsigned count);

	template<>
	void SetUniform<int>(const char* name, int value, unsigned count);

	template<>
	void SetUniform<float*>(const char* name, float* value, unsigned count);

	template<>
	void SetUniform<glm::mat4*>(const char* name, glm::mat4* value, unsigned count);

	template<>
	void SetUniform<vec3>(const char* name, vec3 value, unsigned count);

	/*template<>
	void SetUniform<mat4*>(const char* name, mat4* value, unsigned count);*/

	template<>
	void SetUniform<float>(const char* name, float value, unsigned count);

	template<>
	void SetUniform<uint32_t>(const char* name, uint32_t value, unsigned count);

	template<>
	void SetUniform<uint32_t*>(const char* name, uint32_t* value, unsigned count);

#endif

	void SetUniform4f(const char* name, float* value) {
		glUniform1fv(GetUniformLocation(name), 4, value);
	}
};


template<class T>
inline void Shader::SetUniform(const char* name, T value, unsigned count) {
	ASSERT(false);
}

template<>
inline void Shader::SetUniform<float*>(const char* name, float* value, unsigned count){
	GLcall(glUniform1fv(GetUniformLocation(name), count, value));
}

template<>
inline void Shader::SetUniform<float>(const char* name, float value, unsigned count) {
	GLcall(glUniform1f(GetUniformLocation(name), value));
}

template<>
inline void Shader::SetUniform<int*>(const char* name, int* value, unsigned count) {
	GLcall(glUniform1iv(GetUniformLocation(name), count, value));
}

template<>
inline void Shader::SetUniform<int>(const char* name, int value, unsigned count) {
	GLcall(glUniform1i(GetUniformLocation(name), value));
}


template<>
inline void Shader::SetUniform<uint32_t>(const char* name, uint32_t value, unsigned count) {
	GLcall(glUniform1ui(GetUniformLocation(name), value));
}

template<>
inline void Shader::SetUniform<uint32_t*>(const char* name, uint32_t* value, unsigned count) {
	GLcall(glUniform1uiv(GetUniformLocation(name), count, value));
}

template<>
inline void Shader::SetUniform<glm::mat4*>(const char* name, glm::mat4* value, unsigned count) {
	GLcall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &(*value)[0][0]));
}

//template<>
//inline void Shader::SetUniform<mat4*>(const char* name, mat4* value, unsigned count) {
//	GLcall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &(*value)[0][0]));
//}

template<>
inline void Shader::SetUniform<vec3>(const char* name, vec3 value, unsigned count) {
	GLcall (glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
}
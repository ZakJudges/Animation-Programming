#include "Shader.h"
#include "glad.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "Timer.h"

Shader::Shader()
{
	m_handle = glCreateProgram();
}

Shader::Shader(const std::string& vertex, const std::string& fragment)
{
	m_handle = glCreateProgram();
	Load(vertex, fragment);
}

Shader::~Shader()
{
	glDeleteProgram(m_handle);
}

void Shader::Load(const std::string& vertex, const std::string& fragment)
{
	std::ifstream f(vertex.c_str());
	bool vertFile = f.good();
	f.close();

	f = std::ifstream(fragment.c_str()); //std::ifstream f(vertex.c_str());
	bool fragFile = f.good();
	f.close();

	std::string vertexSource = vertex;
	if (vertFile)
	{
		vertexSource = ReadFile(vertex);
	}
	
	std::string fragmentSource = fragment;
	if (fragFile)
	{
		fragmentSource = ReadFile(fragment);
	}

	unsigned int vertexShader = CompileVertexShader(vertexSource);
	unsigned int fragmentShader = CompileFragmentShader(fragmentSource);

	if (LinkShaders(vertexShader, fragmentShader))
	{
		PopulateAttributes();
		PopulateUniforms();
	}
}

void Shader::Bind()
{
	glUseProgram(m_handle);
}

void Shader::UnBind()
{
	glUseProgram(0);
}

//	Retrieve the bind slot for an attribute.
unsigned int Shader::GetAttribute(const std::string& name)
{
	std::map<std::string, unsigned int>::iterator it = m_attributes.find(name);

	if (it == m_attributes.end())
	{
		std::cout << "Retrieving bad attribute index: " << name << "\n";
		return 0;
	}

	return it->second;
}

//	Retrieve the bind slot for a uniform.
unsigned int Shader::GetUniform(const std::string& name)
{
	std::map<std::string, unsigned int>::iterator it = m_uniforms.find(name);

	if (it == m_uniforms.end())
	{
		std::cout << "Retrieving bad attribute index: " << name << "\n";
		return 0;
	}

	return it->second;
}

unsigned int Shader::GetHandle()
{
	return m_handle;
}

//	Open a file and use the string stream to return the file contents as a string.
std::string Shader::ReadFile(const std::string& path)
{
	std::ifstream file;
	file.open(path);
	std::stringstream contents;
	contents << file.rdbuf();
	file.close();
	return contents.str();
}

unsigned int Shader::CompileVertexShader(const std::string& vertex)
{
	//	Compile the vertex shader.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vSource = vertex.c_str();
	glShaderSource(vertexShader, 1, &vSource, NULL);
	glCompileShader(vertexShader);

	//	Check for compilation errors.
	int success = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);

		std::cout << "Vertex Shader compilation failed. \n";
		std::cout << "\t" << infoLog << "\n";
		glDeleteShader(vertexShader);
		return 0;
	}//;

	return vertexShader;
}

unsigned int Shader::CompileFragmentShader(const std::string& fragment)
{
	//	Compile the fragment shader.
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fSource = fragment.c_str();
	glShaderSource(fragmentShader, 1, &fSource, NULL);
	glCompileShader(fragmentShader);

	//	Check for compilation errors.
	int success = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, sizeof(infoLog), NULL, infoLog);

		std::cout << "Fragment Shader compilation failed. \n";
		std::cout << "\t" << infoLog << "\n";
		glDeleteShader(fragmentShader);
		return 0;
	}//;

	return fragmentShader;
}

//	Attach the shaders to the program handle created at construction.
bool Shader::LinkShaders(unsigned int vertex, unsigned int fragment)
{
	glAttachShader(m_handle, vertex);
	glAttachShader(m_handle, fragment);
	glLinkProgram(m_handle);

	//	Check for errors.
	int success = 0;
	glGetProgramiv(m_handle, GL_LINK_STATUS, &success);

	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(m_handle, sizeof(infoLog), NULL, infoLog);
		std::cout << "Shader linking failed.\n";
		std::cout << "\t" << infoLog << "\n";

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return false;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return true;
}

void Shader::PopulateAttributes()
{
	int count = -1;
	int length;
	char name[128];
	int size; 
	GLenum type;

	glUseProgram(m_handle);
	glGetProgramiv(m_handle, GL_ACTIVE_ATTRIBUTES, &count);

	for (int i = 0; i < count; ++i)
	{
		memset(name, 0, sizeof(char) * 128);
		glGetActiveAttrib(m_handle, (GLuint)i, 128, &length, &size, &type, name);
		int attrib = glGetAttribLocation(m_handle, name);
		if (attrib >= 0)
		{
			m_attributes[name] = attrib;
		}
	}

	glUseProgram(0);
}

void Shader::PopulateUniforms()
{
	int count = -1;
	int length;
	char name[128];
	int size;
	GLenum type;
	char testName[256];

	glUseProgram(m_handle);
	glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count);

	for (int i = 0; i < count; ++i) {
		memset(name, 0, sizeof(char) * 128);
		glGetActiveUniform(m_handle, (GLuint)i, 128, &length, &size, &type, name);

		int uniform = glGetUniformLocation(m_handle, name);
		if (uniform >= 0) {
			std::string uniformName = name;
			std::size_t found = uniformName.find('[');
			if (found != std::string::npos) {
				uniformName.erase(uniformName.begin() + found, uniformName.end());
				// Populate subscripted names too
				unsigned int uniformIndex = 0;
				while (true) {
					memset(testName, 0, sizeof(char) * 256);
					sprintf_s(testName, "%s[%d]", uniformName.c_str(), uniformIndex++);//sprintf
					int uniformLocation = glGetUniformLocation(m_handle, testName);
					if (uniformLocation < 0) {
						break;
					}
					m_uniforms[testName] = uniformLocation;
				}
			}
			m_uniforms[uniformName] = uniform;
		}
	}

	glUseProgram(0);
}

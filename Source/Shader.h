#ifndef _H_SHADER
#define _H_SHADER

#include <map>
#include <string>

class Shader
{
public:
	Shader();
	Shader(const std::string& vertex, const std::string& fragment);
	~Shader();
	void Load(const std::string& vertex, const std::string& fragment);
	void Bind();
	void Unbind();
	unsigned int GetAttribute(const std::string& name);
	unsigned int GetUniform(const std::string& name);
	unsigned int GetHandle();


private:
	//	Non-copyable (since it holds a handle to a GPU resource).
	Shader(const Shader&);
	Shader& operator=(const Shader&) {}
	std::string ReadFile(const std::string& path);
	unsigned int CompileVertexShader(const std::string& vertex);
	unsigned int CompileFragmentShader(const std::string& fragment);
	bool LinkShaders(unsigned int vertex, unsigned int fragment);
	void PopulateAttributes();
	void PopulateUniforms();

private:
	//	Handle to the OpenGL shader object.
	unsigned int m_handle;
	//	Uniform and attribute indices.
	std::map<std::string, unsigned int> m_attributes;
	std::map<std::string, unsigned int> m_uniforms;
};

#endif

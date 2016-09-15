#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include "gl_core_3_3.h"

namespace fumasu
{

class Shader
{
public:
	enum Type
	{
		VertexShader,
		FragmentShader,
		GeometryShader
	};
	
	Shader();
	Shader(std::string vertexFile, std::string fragmentFile);
	~Shader();
	
	std::string GetErrorLog() const;
	void Compile();
	void LoadFromFile(Type type, std::string filename);
	void LoadFromString(Type type, std::string src);
	void UseShader() const;
	void RemoveShader(Type type);
	
	GLint GetAttribLocation(std::string attrib) const;
	GLint GetUniformLocation(std::string uniform) const;
	
private:
	std::string mVertexSrc;
	std::string mFragmentSrc;
	std::string mGeometrySrc;
	GLuint mVertex;
	GLuint mFragment;
	GLuint mGeometry;
	GLuint mProgram;
};

}

#endif // SHADER_HPP

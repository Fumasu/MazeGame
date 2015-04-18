#include "Resources/Shader.hpp"

#include <iostream>
#include <fstream>

namespace fumasu
{

Shader::Shader() : mVertex{0}, mFragment{0}, mGeometry{0}, mProgram{0}
{
}

Shader::Shader(std::string vertexFile, std::string fragmentFile) : Shader()
{
	mVertex =glCreateShader(GL_VERTEX_SHADER);
	mFragment =glCreateShader(GL_FRAGMENT_SHADER);
	mProgram =glCreateProgram();
	
	LoadFromFile(FragmentShader, fragmentFile);
	LoadFromFile(VertexShader, vertexFile);
}

Shader::~Shader()
{
	if (mVertex !=0)
		glDeleteShader(mVertex);
	if (mFragment !=0)
		glDeleteShader(mFragment);
	if (mGeometry !=0)
		glDeleteShader(mGeometry);
	if (mProgram !=0)
		glDeleteProgram(mProgram);
}

void Shader::LoadFromFile(Type type, std::string filename)
{
	std::string src;
	std::ifstream File(filename, std::ifstream::in);
	
	if (!File.is_open())
	{
		std::cerr <<"Shader Source File does not exist: " <<filename <<"\n";
		return;
	}
	
	while (!File.eof())
	{
		char buf[1024];
		
		File.getline(buf, sizeof(buf));
		src +=buf + std::string("\n");
	}
	
	LoadFromString(type, src);
}

void Shader::LoadFromString(Type type, std::string src)
{
	switch (type)
	{
		case VertexShader:
			mVertexSrc.clear();
			mVertexSrc =src;
			break;
		case FragmentShader:
			mFragmentSrc.clear();
			mFragmentSrc =src;
			break;
		case GeometryShader:
			mGeometrySrc.clear();
			mGeometrySrc =src;
	}
	
	Compile();
}

void Shader::Compile()
{
	if (mVertex ==0)
		mVertex =glCreateShader(GL_VERTEX_SHADER);
	if (mFragment ==0)
		mFragment =glCreateShader(GL_FRAGMENT_SHADER);
	if (mProgram ==0)
		mProgram =glCreateProgram();
		
	{
		const char* src =mVertexSrc.c_str();
		glShaderSource(mVertex, 1, &src, nullptr);
		
		glCompileShader(mVertex);
	}
	
	{
		const char* src =mFragmentSrc.c_str();
		glShaderSource(mFragment, 1, &src, nullptr);
		
		glCompileShader(mFragment);
	}
	
	if (!mGeometrySrc.empty())
	{
		if (mGeometry ==0)
			mGeometry =glCreateShader(GL_GEOMETRY_SHADER);
			
		const char* src =mGeometrySrc.c_str();
		glShaderSource(mGeometry, 1, &src, nullptr);
		
		glCompileShader(mGeometry);
	}
	
	glAttachShader(mProgram, mVertex);
	glAttachShader(mProgram, mFragment);
	if (mGeometry !=0)
		glAttachShader(mProgram, mGeometry);
	
	glLinkProgram(mProgram);
	glUseProgram(mProgram);
}

void Shader::UseShader() const
{
	glUseProgram(mProgram);
}

GLint Shader::GetAttribLocation(std::string attrib) const
{
	return glGetAttribLocation(mProgram, attrib.c_str());
}

GLint Shader::GetUniformLocation(std::string uniform) const
{
	return glGetUniformLocation(mProgram, uniform.c_str());
}

std::string Shader::GetErrorLog() const
{
	GLint status;
	std::string out;
		
	glGetShaderiv(mVertex, GL_COMPILE_STATUS, &status);
	if (GL_TRUE !=status)
	{
		char buffer[512]{0};
		glGetShaderInfoLog(mVertex, sizeof(buffer), nullptr, buffer);
		out +=buffer + std::string("\n\n");
	}
	
	glGetShaderiv(mFragment, GL_COMPILE_STATUS, &status);
	if (GL_TRUE !=status)
	{
		char buffer[512]{0};
		glGetShaderInfoLog(mFragment, sizeof(buffer), nullptr, buffer);
		out +=std::string("\n") + buffer;
	}
	
	if (mGeometry !=0)
	{
		glGetShaderiv(mGeometry, GL_COMPILE_STATUS, &status);
		if (GL_TRUE !=status)
		{
			char buffer[512]{0};
			glGetShaderInfoLog(mGeometry, sizeof(buffer), nullptr, buffer);
			out +=std::string("\n") + buffer;
		}
	}
	
	return out;
}

void Shader::RemoveShader(Type type)
{
	switch (type)
	{
		case VertexShader:
			mVertexSrc.clear();
			glDetachShader(mProgram, mVertex);
			glDeleteShader(mVertex);
			mVertex =0;
			break;
		case FragmentShader:
			mFragmentSrc.clear();
			glDetachShader(mProgram, mFragment);
			glDeleteShader(mFragment);
			mFragment =0;
			break;
		case GeometryShader:
			mGeometrySrc.clear();
			glDetachShader(mProgram, mGeometry);
			glDeleteShader(mGeometry);
			mGeometry =0;
			break;
	}
	
	Compile();
}

}


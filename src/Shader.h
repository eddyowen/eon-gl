#pragma once

#include <Renderer.h>

#include <string>

namespace eon
{
	class Shader
	{
	public:
		struct ShaderSource
		{
			std::string vertexSource;
			std::string fragmentSource;
		};

		enum class ShaderType
		{
			INVALID = -1, VERTEX = 0, FRAGMENT = 1
		};

		Shader(const std::string& filepath);
		~Shader();

		void Use() { glUseProgram(m_shaderId); }

		template<typename T>
		void SetUniform(const std::string& name, T value)
		{
			static_assert(true);
		}

		template<>
		void SetUniform(const std::string& name, float value)
		{
			glUniform1f(glGetUniformLocation(m_shaderId, name.c_str()), value);
		}

		template<>
		void SetUniform(const std::string& name, int value)
		{
			EON_TRACE("Asignando un int");
		}

		template<>
		void SetUniform(const std::string& name, bool value)
		{
			EON_TRACE("Asignando un bool");
		}
	private:
		unsigned int m_shaderId;
		
		ShaderSource ParseShader(const std::string& filepath);
		unsigned int CreateProgram(const std::string& vertexSource, const std::string& fragmentSource);
		unsigned int CompileShader(unsigned int shaderType, const std::string& source);
	};
}

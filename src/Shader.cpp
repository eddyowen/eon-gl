#include <Shader.h>

#include <fstream>
#include <sstream>

eon::Shader::Shader(const std::string& filepath)
{
	ShaderSource programSource = ParseShader(filepath);
	m_shaderId = CreateProgram(programSource.vertexSource, programSource.fragmentSource);
}

eon::Shader::~Shader()
{
	
}

eon::Shader::ShaderSource eon::Shader::ParseShader(const std::string& filepath)
{
	ShaderSource result;

	std::ifstream ifs(filepath);
	if (ifs.is_open())
	{
		using eon::Shader;
		ShaderType type = ShaderType::INVALID;

		std::string line;
		std::stringstream ss[2];
		while (std::getline(ifs, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else
			{
				ss[int(type)] << line << "\n";
			}
		}

		result.vertexSource = ss[0].str();
		result.fragmentSource = ss[1].str();
	}

	return result;
}

unsigned int eon::Shader::CreateProgram(const std::string& vertexSource, const std::string& fragmentSource)
{
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

	// Shader Linking:
	// - "When linking the shaders into a program it links the outputs 
	//    of each shader to the inputs of the next shader. This is also 
	//    where you'll get linking errors if your outputs and inputs do not match.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	int  success;
	char errorMessage[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, errorMessage);
		EON_ERROR("SHADER:: Linking error encountered!");
		EON_ERROR(errorMessage);

		glDeleteProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	// No longer needed
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);;
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

unsigned int eon::Shader::CompileShader(unsigned int shaderType, const std::string& source)
{
	const char* src = source.c_str();

	int  success;
	char errorMessage[512];

	unsigned int shaderId;
	shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &src, NULL);
	glCompileShader(shaderId);
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		std::string shaderTypeName = shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
		glGetShaderInfoLog(shaderId, 512, NULL, errorMessage);
		EON_ERROR("SHADER::" + shaderTypeName + " Compilation failed!");
		EON_ERROR(errorMessage);

		glDeleteShader(shaderId);
	}

	return shaderId;
}



//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{

}
ShaderProgram::~ShaderProgram()
{
	if (m_ID != 0)
		glDeleteProgram(m_ID);
}

bool ShaderProgram::AttachShader(const std::wstring& assetLocation, ShaderType type)
{
	//If there is no handle to a shaderprogram yet make one
	if (!m_ID)
		m_ID = glCreateProgram();

	//Find the shader type
	unsigned int shader = 0;
	switch (type)
	{
	case ShaderType::VertexShader:
		shader = glCreateShader(GL_VERTEX_SHADER);
		break;
	case ShaderType::FragmentShader:
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	case ShaderType::GeometryShader:
		shader = glCreateShader(GL_GEOMETRY_SHADER);
		break;
	default:
		break;
	}

	//Load the shader file
	std::string source =  ShaderSourceFromFile(assetLocation);
	const char* shaderSourceSTR = source.c_str();

	//Set the shader source to compile
	glShaderSource(shader, 1, &shaderSourceSTR, nullptr);

	//Compile the shader
	glCompileShader(shader);
		
	if ( !CheckCompileErrors(shader, type) )
	{
		//Could not compile the shader -> return false
		return false;
	}

	//Attach the shader
	glAttachShader(m_ID, shader);

	return true;
}

void ShaderProgram::Use() const
{
	if (!IsInUse())
		glUseProgram(m_ID);
}

bool ShaderProgram::IsInUse() const
{
	//Get the current program
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

	//Compare current program againt this.m_Handle
	return ((GLuint)currentProgram == m_ID);
}

void ShaderProgram::CheckInUse() const
{
	if (!IsInUse())
		Logger::LogWarning(L"ShaderProgram not in use.");
}

void ShaderProgram::StopUsing() const
{
	if (!IsInUse())
		glUseProgram(0);
}


bool ShaderProgram::Link()
{
	if (!m_ID)
		m_ID = glCreateProgram();

	//If the handle is not linked link the handle
	if (!m_IsLinked)
	{
		glLinkProgram(m_ID);

		//Check if the link succeeded
		if (!CheckLinkErrors(m_ID) )
		{
			//Could not compile the shader -> return false
			m_IsLinked = false;
			return false;
		}

		m_IsLinked = true;
	}

	return m_IsLinked;
}

void ShaderProgram::BindAttribute(int location, const std::string& name)
{
	glBindAttribLocation(m_ID, location, name.c_str() );
}

int ShaderProgram::GetAttribute(const std::string& name) const
{
	return glGetAttribLocation(m_ID, name.c_str());
}

int ShaderProgram::GetUniform(const std::string& name) const
{
	return glGetUniformLocation(m_ID, name.c_str());
}

void ShaderProgram::SetUniform(const std::string& name, float x) const
{
	CheckInUse();

	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniform1f(uniform, x);
}

void ShaderProgram::SetUniform(const std::string& name, float x, float y) const
{
	CheckInUse();

	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniform2f(uniform, x, y);
}
void ShaderProgram::SetUniform(const std::string& name, float x, float y, float z) const
{
	CheckInUse();

	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniform3f(uniform, x, y, z);
}

void ShaderProgram::SetUniform(const std::string& name, float x, float y, float z, float w) const
{
	CheckInUse();
	
	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniform4f(uniform, x, y, z, w);
}

void ShaderProgram::SetUniform(const std::string& name, unsigned int x) const
{
	CheckInUse();

	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniform1ui(uniform, x);
}

void ShaderProgram::SetUniform(const std::string& name, int x) const
{
	CheckInUse();

	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniform1i(uniform, x);
}

void ShaderProgram::SetUniform(const std::string& name, bool x) const
{
	CheckInUse();

	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniform1i(uniform, x);
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec2& v) const
{
	CheckInUse();

	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniform2fv(uniform, 1, glm::value_ptr(v));
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& v) const
{
	CheckInUse();

	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniform3fv(uniform, 1, glm::value_ptr(v) );
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec4& v) const
{
	CheckInUse();

	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniform4fv(uniform, 1, glm::value_ptr(v) );
}


void ShaderProgram::SetUniform(const std::string & name, const glm::mat4 & m, bool transpose) const
{
	CheckInUse();

	int uniform = GetUniform(name);
	if (uniform == -1)
		return;

	glUniformMatrix4fv(uniform, 1 ,static_cast<GLboolean>(transpose), glm::value_ptr(m) );
}


std::string ShaderProgram::ShaderSourceFromFile(const std::wstring & assetLocation)
{
	std::ifstream file;
	file.open(assetLocation, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		std::wstringstream wstream;
		wstream << L"ShaderProgram::ShaderSourceFromFile : Could not open file at location (" << assetLocation.c_str() << ")";
		Logger::LogError(wstream.str());
		return{};
	}

	std::string output;
	std::string line;
	
	//Loop through all lines in the file
	while (file.good())
	{
		getline(file, line);

		//Trim the leading spaces from the line
		size_t first = line.find_first_not_of(' ');
		size_t last = line.find_last_not_of(' ');
		line = line.substr(first, (last - first + 1));

		output.append(line);
		output.append("\n");
	}
	
	//Close the file
	file.close();

	return output;
}

bool ShaderProgram::CheckCompileErrors(GLuint object, ShaderType type)
{
	std::string typestr;
	switch (type)
	{
	case ShaderType::VertexShader:
		typestr = "VERTEX SHADER";
		break;
	case ShaderType::FragmentShader:
		typestr = "FRAGMENT SHADER";
		break;
	case ShaderType::GeometryShader:
		typestr = "GEOMETRY SHADER";
		break;
	default:
		break;
	}

	GLint success;
	GLchar infoLog[1024];

	glGetShaderiv(object, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(object, 1024, NULL, infoLog);
		std::cout << "| ERROR::SHADER: Compile-time error: Type: " << typestr << "\n"
			<< infoLog << "\n -- --------------------------------------------------- -- "
			<< std::endl;

		glDeleteShader(object);
		return false;
	}
	return true;
}

bool ShaderProgram::CheckLinkErrors(GLuint object)
{

	GLint success;
	GLchar infoLog[1024];
	glGetProgramiv(object, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(object, 1024, NULL, infoLog);
		std::cout << "| ERROR::Shader: Link-time error: Type: " << "LINK" << "\n"
			<< infoLog << "\n -- --------------------------------------------------- -- "
			<< std::endl;
		return false;
	}
	return true;
}

#pragma once
#include <map>

enum class ShaderType
{
	VertexShader,
	FragmentShader,
	GeometryShader
};

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();

	bool AttachShader(const std::wstring& assetLocation, ShaderType type);
	GLuint GetID() const { return m_ID; }

	void Use() const;
	bool IsInUse() const;
	void StopUsing() const;
	void CheckInUse() const;

	bool Link();

	void BindAttribute(int location,const std::string& name);

	int GetAttribute(const std::string& name) const;
	int GetUniform(const std::string& name) const;

	void SetUniform(const std::string& name, float x) const;
	void SetUniform(const std::string& name, float x, float y) const;
	void SetUniform(const std::string& name, float x, float y, float z) const;
	void SetUniform(const std::string& name, float x, float y, float z, float w) const;
	void SetUniform(const std::string& name, unsigned int x) const;
	void SetUniform(const std::string& name, int x) const;
	void SetUniform(const std::string& name, bool x) const;
	void SetUniform(const std::string& name, const glm::vec2& v) const;
	void SetUniform(const std::string& name, const glm::vec3& v) const;
	void SetUniform(const std::string& name, const glm::vec4& v) const;
	void SetUniform(const std::string& name, const glm::mat4& m, bool transpose = false) const;
private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//
	bool CheckCompileErrors(GLuint object, ShaderType type);
	bool CheckLinkErrors(GLuint object);
	std::string ShaderSourceFromFile(const std::wstring & assetLocation);

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//
	GLuint m_ID = 0;
	bool m_IsLinked = false;
};

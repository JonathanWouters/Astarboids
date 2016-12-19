#pragma once

#include <map>
#include "../Graphics/Texture2D.h"

class Texture2D;
class ShaderProgram;
class SpriteFont;


class ContentManager
{
public:
	static unsigned int LoadTexture(const std::wstring & file, bool hasAlpha,const TextureSettings & settings = TextureSettings() );
	static Texture2D* GetTexture(const std::wstring & file);
	static Texture2D* GetTexture(unsigned int texID);

	static ShaderProgram* LoadShader(const std::wstring & VertexShaderfile, const std::wstring & FragmentShaderfile, const std::wstring & GeometryShaderfile, std::wstring name);
	static ShaderProgram* GetShader(const std::wstring & name);

	static unsigned int LoadSpriteFont(const std::wstring & file);
	static SpriteFont* GetSpriteFont(const std::wstring & file);
	static SpriteFont* GetSpriteFont(unsigned int id);
	
	static void Release();
private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//
	ContentManager() { }

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//

	static Texture2D* LoadTextureFromFile(const std::wstring & file, bool hasAlpha,TextureSettings settings);
	static SpriteFont* LoadSpriteFontFromFile(const std::wstring & file);
	static std::map<unsigned int, Texture2D* > m_Textures;
	static std::map<unsigned int, SpriteFont* > m_SpriteFonts;
	static std::map<std::wstring, ShaderProgram* > m_Shaders;

};


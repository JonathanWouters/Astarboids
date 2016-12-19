//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "ContentManager.h"

#include "../Graphics/ShaderProgram.h"
#include "../Graphics/SpriteFont.h"

#include "../Helpers/BinaryReader.h"

#include <SOIL.h>
#include <functional>

// Instantiate static variables
std::map<unsigned int, Texture2D*> ContentManager::m_Textures;
std::map<unsigned int, SpriteFont*> ContentManager::m_SpriteFonts;
std::map<std::wstring, ShaderProgram*> ContentManager::m_Shaders;

void ContentManager::Release()
{
	//delete all textures
	for (auto iterator : m_Textures)
	{
		delete(iterator.second);
	}

	//delete all fonts
	for (auto iterator : m_SpriteFonts)
	{
		delete(iterator.second);
	}

	//delete all Shaders
	for (auto iterator : m_Shaders)
	{
		delete(iterator.second);
	}
}

unsigned int ContentManager::LoadTexture(const std::wstring & file, bool hasAlpha, const TextureSettings & settings)
{
	// Hash the file path string so it can be used as an ID
	// Each path can only refer to 1 file -> unique ID's :)
	std::hash<std::wstring> hasher;
	unsigned int texID = hasher(file);

	//Check if the file already exists if not add it to m_Textures
	if (!m_Textures.count(texID))
		m_Textures[texID] = LoadTextureFromFile(file, hasAlpha, settings);

	return texID;
}

Texture2D* ContentManager::GetTexture(const std::wstring  & file)
{
	// Hash the file path string so it can be used as an ID
	// Each path can only refer to 1 file -> unique ID's :)
	std::hash<std::wstring> hasher;
	unsigned int texID = hasher(file);

	return GetTexture(texID);
}

Texture2D* ContentManager::GetTexture(unsigned int texID)
{
	//Check if texture exists in m_Textures else log warning 
	if (m_Textures.count(texID))
		return m_Textures[texID];

	Logger::LogWarning(L"ContentManager::GetTexture() -> Texture not loaded! returning nullptr");
	return nullptr;
}

ShaderProgram * ContentManager::LoadShader(const std::wstring & VertexShaderfile, const std::wstring & FragmentShaderfile, const std::wstring & GeometryShaderfile, std::wstring name)
{
	ShaderProgram* shader = new ShaderProgram();

	//Attach vertex shader
	if (VertexShaderfile != L"")
		shader->AttachShader(VertexShaderfile, ShaderType::VertexShader);

	//Attach geometry shader
	if (GeometryShaderfile != L"")
		shader->AttachShader(GeometryShaderfile, ShaderType::GeometryShader);

	//Attach fragment shader
	if (FragmentShaderfile != L"")
		shader->AttachShader(FragmentShaderfile, ShaderType::FragmentShader);

	m_Shaders[name] = shader;
	return m_Shaders[name];
}

ShaderProgram * ContentManager::GetShader(const std::wstring & name)
{
	//Check if Shader exists in m_Shaders else log warning 
	if (m_Shaders.count(name))
		return m_Shaders[name];

	Logger::LogWarning(L"ContentManager::GetShader() -> Shader not loaded! returning nullptr");
	return nullptr;

}

unsigned int ContentManager::LoadSpriteFont(const std::wstring & file)
{
	// Hash the file path string so it can be used as an ID
	// Each path can only refer to 1 file -> unique ID's :)
	std::hash<std::wstring> hasher;
	unsigned int fntID = hasher(file);

	//Check if font already exists
	if (!m_SpriteFonts.count(fntID))
		m_SpriteFonts[fntID] = LoadSpriteFontFromFile(file);

	return fntID;
}

SpriteFont * ContentManager::GetSpriteFont(const std::wstring & file)
{
	//hash filepath to get the id
	std::hash<std::wstring> hasher;
	unsigned int fntID = hasher(file);

	return GetSpriteFont(fntID);
}

SpriteFont * ContentManager::GetSpriteFont(unsigned int id)
{
	//Check if font exists else log a warning
	if (m_SpriteFonts.count(id))
		return m_SpriteFonts[id];

	Logger::LogWarning(L"ContentManager::GetSpriteFont() -> Font not loaded! returning nullptr");
	return nullptr;
}

Texture2D* ContentManager::LoadTextureFromFile(const std::wstring & file, bool hasAlpha, TextureSettings settings)
{
	// Create Texture object
	Texture2D* texture = new Texture2D();
	if (hasAlpha)
	{
		settings.Image_Format = GL_RGBA;
		settings.Internal_Format = GL_RGBA;
	}
	texture->SetTextureSettings(settings);

	// Load image
	int width = 0 , height = 0;
	std::string fileStr(file.begin(), file.end());

	unsigned char* image = SOIL_load_image(fileStr.c_str(), &width, &height, 0, hasAlpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	
	//generate texture
	texture->Generate(width, height, image );
	
	//Free image data
	SOIL_free_image_data(image);
	
	return texture;
}

 
SpriteFont * ContentManager::LoadSpriteFontFromFile(const std::wstring & file)
{
	auto pBinReader = new BinaryReader(); //Prevent memory leaks
	pBinReader->Open(file);
	
	if (!pBinReader->Exists())
	{
		delete pBinReader;
		Logger::LogError(L"ContentManager::LoadSpriteFontFromFile > Failed to read the assetFile!\nPath: " +  file );
	
		return nullptr;
	}
	
	//See BMFont Documentation for Binary Layout
	
	//Parse the Identification bytes (B,M,F)
	//If Identification bytes doesn't match B|M|F,
	//Log Error (ContentManager::LoadSpriteFontFromFile > Not a valid .fnt font) &
	//return nullptr
	char IdByte1 = pBinReader->Read<char>();
	char IdByte2 = pBinReader->Read<char>();
	char IdByte3 = pBinReader->Read<char>();
	
	if (IdByte1 != 'B' && IdByte2 != 'M' && IdByte3 != 'F')
	{
		Logger::LogError( L"ContentManager::LoadSpriteFontFromFile > Not a valid .fnt font " + file );
		return nullptr;
	}
		
	//Parse the version (version 3 required)
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	
	if (pBinReader->Read<char>() - '0' >= 3)
	{
		Logger::LogError(L"ContentManager::LoadSpriteFontFromFile > Only .fnt version 3 is supported");
		return nullptr;
	}
	
	//Valid .fnt file
	auto pSpriteFont = new SpriteFont();
	//SpriteFontLoader is a friend class of SpriteFont
	//That means you have access to its privates (pSpriteFont->m_FontName = ... is valid)
	
	//**********
	// BLOCK 0 *
	//**********
	//Retrieve the blockId and blockSize
	//Retrieve the FontSize (will be -25, BMF bug) [SpriteFont::m_FontSize]
	//Move the pBinReader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [SpriteFont::m_FontName]
	
	int blockId = pBinReader->Read<char>();
	int blockSize = pBinReader->Read<int>();
	//Retrieve the FontSize (will be -25, BMF bug) [SpriteFont::m_FontSize]
	
	pSpriteFont->m_FontSize = pBinReader->Read<char>();
	//Move the pBinReader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	pBinReader->MoveBufferPosition(13);
	//Retrieve the FontName [SpriteFont::m_FontName]
	pSpriteFont->m_FontName = pBinReader->ReadNullString();
	
	
	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	//int bufferpos = pBinReader->GetBufferPosition();
	blockId = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();
	
	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	
	pBinReader->MoveBufferPosition(4);
	
	pSpriteFont->m_TextureWidth = pBinReader->Read<unsigned short>();
	pSpriteFont->m_TextureHeight = pBinReader->Read<unsigned short>();
	
	
	//Retrieve PageCount
	int PageCount = pBinReader->Read<unsigned short>();
	//> if pagecount > 1
	//> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed)
	//Advance to Block2 (Move Reader)
	
	if (PageCount > 1)
	{
		Logger::LogError(L"ContentManager::LoadSpriteFontFromFile > SpriteFont (.fnt): Only one texture per font allowed");
	}
	
	pBinReader->MoveBufferPosition(5);
	
	
	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();
	
	//Retrieve the PageName (store Local)
	std::wstring PageName = pBinReader->ReadNullString();
	
	//	> If PageName is empty
	//	> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])	
	
	if (PageName == L"")
	{
		Logger::LogError(L"ContentManager::LoadSpriteFontFromFile > SpriteFont (.fnt): Invalid Font Sprite [Empty]");
	}
	
	//>Retrieve texture filepath from the assetFile path
	//> (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	std::wstring FilePath = file.substr(0, file.rfind(L"/") + 1);
	
	//>Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	//> (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)
	//...
	pSpriteFont->m_TextureID = ContentManager::LoadTexture(FilePath + PageName,true);
	
	
	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pBinReader->Read<char>();
	blockSize = pBinReader->Read<int>();
	//Retrieve Character Count (see documentation)
	int characterCount = blockSize / 20;
	
	//Retrieve Every Character, For every Character:
	for (int i = 0; i < characterCount; i++)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		wchar_t CharacterId = (wchar_t)pBinReader->Read<unsigned long>();
		//> Check if CharacterId is valid (SpriteFont::IsCharValid), Log Warning and advance to next character if not valid
		if (!pSpriteFont->IsCharValid(CharacterId))
		{
			pBinReader->MoveBufferPosition(16);
			continue;
		}
	
		//> Retrieve the corresponding FontMetric (SpriteFont::GetMetric) [REFERENCE!!!]
		FontMetric *pFontMetric = &pSpriteFont->GetMetric(CharacterId);
		//> Set IsValid to true [FontMetric::IsValid]
		pFontMetric->IsValid = true;
		//> Set Character (CharacterId) [FontMetric::Character]
		pFontMetric->Character = CharacterId;
		//> Retrieve Xposition (store Local)
		float xPos = pBinReader->Read<unsigned short>();
		//> Retrieve Yposition (store Local)
		float yPos = pBinReader->Read<unsigned short>();
	
		//> Retrieve & Set Width [FontMetric::Width]
		pFontMetric->Width = pBinReader->Read<unsigned short>();
		//> Retrieve & Set Height [FontMetric::unsigned short]
		pFontMetric->Height = pBinReader->Read<unsigned short>();
		//> Retrieve & Set OffsetX [FontMetric::OffsetX]
		pFontMetric->OffsetX = pBinReader->Read<short>();
		//> Retrieve & Set OffsetY [FontMetric::OffsetY]
		pFontMetric->OffsetY = pBinReader->Read<short>();
		//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
		pFontMetric->AdvanceX = pBinReader->Read<unsigned short>();
		//> Retrieve & Set Page [FontMetric::Page]
		pFontMetric->Page = pBinReader->Read<char>();
		//> Retrieve Channel (BITFIELD!!!) 
		//	> See documentation for BitField meaning [FontMetrix::Channel]
		unsigned char channel = pBinReader->Read<char>();
		if (channel == 1)pFontMetric->Channel = 2;
		if (channel == 2)pFontMetric->Channel = 1;
		if (channel == 4)pFontMetric->Channel = 0;
		if (channel == 8)pFontMetric->Channel = 3;
		//> Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
		pFontMetric->TexCoord = glm::vec2(xPos / pSpriteFont->m_TextureWidth, yPos / pSpriteFont->m_TextureHeight);
	}
		
	delete pBinReader;
	return pSpriteFont;
}


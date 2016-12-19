#pragma once

struct TextCache
{
public:
	TextCache(const std::wstring& text,const glm::vec2& pos,const glm::vec4& col) :
		Text(text), Position(pos), Color(col) {}

	const std::wstring Text;
	glm::vec2 Position;
	glm::vec4 Color;

private:
	TextCache& operator=(const TextCache &tmp);
};

struct FontMetric
{
	//BMFONT
	bool IsValid;
	wchar_t Character;
	unsigned short Width;
	unsigned short Height;
	short OffsetX;
	short OffsetY;
	short AdvanceX;
	unsigned char Page;
	unsigned char Channel;
	glm::vec2 TexCoord;
};

class TextRenderer;

class SpriteFont
{
public:
	SpriteFont();
	virtual ~SpriteFont();

	static bool IsCharValid(const wchar_t& character);

	static const int MAX_CHAR_ID = 255;
	static const int MIN_CHAR_ID = 0;
	static const int CHAR_COUNT = MAX_CHAR_ID - MIN_CHAR_ID + 1;

private:

	friend class ContentManager;
	friend class TextRenderer;

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//
	FontMetric& GetMetric(const wchar_t& character) { return m_CharTable[character - MIN_CHAR_ID]; };

private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//
	FontMetric m_CharTable[CHAR_COUNT];
	std::vector<TextCache> m_TextCache;

	short m_FontSize;
	std::wstring m_FontName;
	int m_CharacterCount;
	int m_CharacterSpacing;
	int m_TextureWidth;
	int m_TextureHeight;
	int m_BufferStart, m_BufferSize;
	unsigned int m_TextureID;
	bool m_IsAddedToRenderer;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpriteFont(const SpriteFont& t);
	SpriteFont& operator=(const SpriteFont& t);
};


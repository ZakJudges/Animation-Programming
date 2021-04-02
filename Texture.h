#ifndef _H_TEXTURE
#define _H_TEXTURE

class Texture
{
public:
	Texture();
	Texture(const char* path);
	~Texture();
	void Load(const char* path);
	void Set(unsigned int uniform, unsigned int texIndex);
	void UnSet(unsigned int textureIndex);
	unsigned int GetHandle();

private:
	//	Non-copyable.
	Texture(const Texture& other);
	Texture& operator=(const Texture& other);

protected:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_channels;
	unsigned int m_handle;
};

#endif

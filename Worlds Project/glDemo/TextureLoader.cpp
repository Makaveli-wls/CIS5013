
#include "TextureLoader.h"

using namespace std;


//load image using FreeImage
GLuint loadTexture(string filename, FREE_IMAGE_FORMAT srcImageType) {

	//Load bitmap
	FIBITMAP* loadedBitmap = FreeImage_Load(srcImageType, filename.c_str(), BMP_DEFAULT);

	if (!loadedBitmap) {

		cout << "FreeImage: Could not load image " << filename << endl;
		return 0;
	}

	// Convert to RGBA
	FIBITMAP* bitmap32bpp = FreeImage_ConvertTo32Bits(loadedBitmap);
	FreeImage_Unload(loadedBitmap);

	if (!bitmap32bpp) {

		cout << "FreeImage: Conversion to 32 bits unsuccessful for image " << filename << endl;
		return 0;
	}

	GLuint newTexture = 0;

	//If image loaded generate texture
	glGenTextures(1, &newTexture); 

	if (newTexture) 
	{
		glBindTexture(GL_TEXTURE_2D, newTexture);

		//Setup texture image properties
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			FreeImage_GetWidth(bitmap32bpp),
			FreeImage_GetHeight(bitmap32bpp),
			0,
			GL_BGRA,
			GL_UNSIGNED_BYTE,
			FreeImage_GetBits(bitmap32bpp));

		//Setup texture filter and wrap properties
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

	//Copy image data into OpenGL
	FreeImage_Unload(bitmap32bpp);

	return newTexture;
}
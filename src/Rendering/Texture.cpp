#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb/stb_image.h"

static char fps[32][64]; // array of file paths

static unsigned int SetTextureVariable(unsigned int, const char*);

static unsigned int SetTextureVariable(unsigned int tex, const char* fp)
{
unsigned int mask = 0b1111000000000000U;  // the mask for the active texture

int nfps = 0;
for (int i = 0; i < 32; i++)
    if(strlen(fps[i]) != 0)
        nfps++;

for (int i = 0; i < nfps; i++)
    if(strcmp(fps[i], fp) == 0)    // if the file path is already in the array
        return ((tex & ~mask) | (i << 12)); // clears the tex variable part then appends the new active number

strcpy(fps[nfps], fp);  // copy the new file path into the filepaths array
return ((tex & ~mask) | (nfps << 12));
}

unsigned int getActiveTexture(unsigned int texture)
{
unsigned int mask = 0b1111000000000000U;  // the mask for the active texture
return ((texture & mask) >> 12); // ignores all the other bits then shifts the active part to the front
}

unsigned int getTexture(unsigned int texture)
{
unsigned int mask = 0b1111000000000000U;  // the mask for the active texture
return (texture & ~mask);   // everything that isn't the mask
}

unsigned int CreateTexture(const char* path)
{
unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

int width, height, nrChannels;
// stbi_set_flip_vertically_on_load(1);
unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
if(!data)
    {
    printf("ERROR: Failed to load texture\n");
    stbi_image_free(data);
    return -1;
    }

glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
glGenerateMipmap(GL_TEXTURE_2D);

stbi_image_free(data);

texture = SetTextureVariable(texture, path);

return texture;
}

void BindTexture(unsigned int texture)
{
// GL_TEXTURE0 is the first of many consecutive numbers referring to active textures so if we just add the active we get the correct thing
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, getTexture(texture));  // binds the texture
}

const char* getTextureFilePath(unsigned int texture) { return fps[getTexture(texture)]; }
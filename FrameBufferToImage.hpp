#include <string>

#include "GL/glew.h"

//http://www.libpng.org/pub/png/libpng.html
#include "png.h"

void writePngFile(const char *filename, int width, int height, GLubyte* pixels );
void saveFramePNG( std::string file_name );


//https://github.com/nothings/stb
#include "stb.h"

void saveFrameJpegSTB( std::string file_name );

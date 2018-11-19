#include <string>

#include "GL/glew.h"

#include "png.h"

void writePngFile(const char *filename, int width, int height, GLubyte* pixels );
void saveFramePNG( std::string file_name );


#include "stb.h"

void saveFrameJpegSTB( std::string file_name );

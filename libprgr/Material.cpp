#include "Material.h"
#include "common.h"


void Material::bind(int textureUnit)
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);//activamos unidad de texturado 0, esto significa que va a contener la textura que va a contener.
	glBindTexture(GL_TEXTURE_2D, this->texture->textureID); //cargamos la textura en unidad de texturado 0
}

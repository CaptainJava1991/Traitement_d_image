#ifndef PILE_IMAGE
#define PILE_IMAGE

#include "image.h"
#include "pgm_image.h"

void pile_new_image(image_t *new_image);
void pile_free_images();
void pile_undo();
void pile_redo();
image_t *charg(char *nom_fichier,char *nom_fichier2);
image_t *c2332_1m4g3(image_t *im, image_t *im2);
typedef struct
	{
	image_t *im;
	image_t *prev;
	image_t *next;
	} lifo;
#endif

#ifndef PGM_IMAGE
#define PGM_IMAGE

image_t *charger_image_pgm(char *nom_fichier);

int sauver_image_pgm(char *nom_fichier, image_t *img);

image_t *charg(char *nom_fichier,char *nom_fichier2);

image_t *c2332_1m4g3(image_t *im, image_t *im2);

#endif

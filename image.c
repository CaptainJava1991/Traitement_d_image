#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "image.h"

//initialise une structure image et retourne son adresse
image_t *creer_image(){
	image_t *im=malloc(sizeof(image_t));
	if(im==NULL){
		fprintf(stderr,"Erreur, impossible d'initialiser l'image\n");
		return NULL;	
	}
	im->w=0;
	im->h=0;
	im->path=NULL;
	im->buff=NULL;
	return im;
}

//crée une copie d'une image et retourne son adresse
image_t *copier_image(image_t *src){
	image_t *new=creer_image();
	if(new==NULL){
		return NULL;
	}
	new->w=src->w;
	new->h=src->h;
	new->path=strdup(src->path);
	new->buff=malloc(src->w*src->h*sizeof(unsigned char));
	if(new->buff==NULL){
		fprintf(stderr,"Copie d'image impossible\n");
		detruire_image(new);
		return NULL;
	}
	memcpy(new->buff,src->buff,src->w*src->h*sizeof(unsigned char));
	return new;
}

//libère la mémoire allouée à la structure
void detruire_image(image_t *p){
	if(p!=NULL){
		free(p->path);
		free(p->buff);
		free(p);
	}
}

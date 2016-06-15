#include <stdlib.h>
#include "noyau.h"
#include <stdio.h>
#include <string.h>

#define TMP_STR_SIZE 128

noyau_t *creer_noyau(unsigned int dim)
	{
	noyau_t *h=malloc(sizeof(noyau_t));
	if(h==NULL){
		fprintf(stderr,"Impossible de creer le noyau\n");
		return NULL;
	}
	h->dim=dim;
	h->coeffs=malloc(sizeof(int)*dim*dim);
	if(h->coeffs==NULL){
		fprintf(stderr,"Erreur d'allocation\n");
		free(h);
		return NULL;
	}
	return h;
	}

void detruire_noyau(noyau_t *h)
	{
		if(h!=NULL){
			free(h->coeffs);
			free(h);
		}
	}


noyau_t *charger_noyau(char *nom_fichier)
	{
	FILE *f=fopen(nom_fichier,"r");
	if(f==NULL){
		fprintf(stderr,"Impossible de charger le noyau\n");
		return NULL;
	}
	unsigned int dim;
	if(fscanf(f,"%d\n",&dim)!=1){
		fprintf(stderr,"Erreur noyau\n");
		return NULL;
	}
	noyau_t *h=creer_noyau(dim);
	if(h!=NULL){
		unsigned int k;
		for (k = 0; k < dim*dim; k++)
		{
			if(fscanf(f," %d ",&(h->coeffs[k]))!=1){
				fprintf(stderr,"Impossible de charger les donnees\n");
				detruire_noyau(h);
				fclose(f);
				return NULL;
			}
		}
	}
	fclose(f);
	return h;
	}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "image.h"
#include "noyau.h"
#define VAL(img,i,j) (img)->buff[(i)*(img)->w+(j)]


image_t *negatif(image_t *im)
	{
	//création d'une nouvelle image
	image_t *neg=creer_image();
	if(neg==NULL) return NULL;
	neg->w=im->w;
	neg->h=im->h;
	neg->path=strdup(im->path);
	neg->buff=malloc(im->w*im->h*sizeof(char));
	if(neg->buff==NULL)
	{
		fprintf(stderr,"Impossible de creer une nouvelle image\n");
	}
	unsigned long k;
	//copie de l'inverse pour chaque pixel de l'image
	for (k = 0; k < im->w*im->h; k++)
	{
		neg->buff[k]=255-im->buff[k];
	}
	return neg;
	}

image_t *rotation(image_t *src, int angle)
	{
	//creation d'une nouvelle image avec la largeur égale à la hauteur de l'image rentrante et vice-versa
	image_t *rot=creer_image();
	if(rot==NULL) return NULL;
	rot->w=src->h;
	rot->h=src->w;
	rot->path=strdup(src->path);
	rot->buff=malloc(src->w*src->h*sizeof(unsigned char));
	angle=angle/90; //simplification des angles, 1=90, 2=180, 3=270
	int i,j;
	for(j=0;j<(src->h);j++){
		for (i = 0; i < (src->w); i += 1)
		{
			VAL(rot,src->w-i-1,j)=VAL(src,j,i);
		}
	}
	
	//Faire une rotation supplémentaire (180°)
	if (angle>1)
	{
		if (angle>2) //Encore un rotation (180+90=270) peut être facilement amélioré (en une transformation au lieu de 3)
		{
			image_t *cop=rotation(rot,180);
			detruire_image(rot);
			return cop;
		}
		image_t *cp=creer_image();  //nouvelle image tampon crée, libérée avant la fin de la fonction
		if(cp==NULL) return NULL;
		cp->w=rot->h;
		cp->h=rot->w;
		cp->path=strdup(rot->path);
		cp->buff=malloc(rot->w*rot->h*sizeof(unsigned char));
		for(j=0;j<(rot->h);j++){
			for (i = 0; i < (rot->w); i += 1)
			{
				VAL(cp,rot->w-i-1,j)=VAL(rot,j,i);
			}
		}
		detruire_image(rot);
		return cp;
	}
	//fin de fct en cas de rotation à 90°
	return rot;
}

image_t *modifier_lumin(image_t *src, int pourcent)
	{
	image_t *lum=creer_image();
	if(lum==NULL) return NULL;
	lum->w=src->w;
	lum->h=src->h;
	lum->path=strdup(src->path);
	lum->buff=malloc(src->w*src->h*sizeof(char));
	if(lum->buff==NULL)
	{
		fprintf(stderr,"Impossible de creer une nouvelle image\n");
	}
	int prc=(int)(255*((pourcent-100)/100.0));
	unsigned long k;
	for (k = 0; k < src->w*src->h; k++)
	{
		if ((src->buff[k]+prc)>255)
		{
			lum->buff[k]=255;
		}
		else{
			if ((src->buff[k]+prc)<0)
			{
				lum->buff[k]=0;
			}
			else{
				lum->buff[k]=(src->buff[k])+prc;
			}
		}
	}
	return lum;
	}

image_t *bruiter_image(image_t *src, int pourcent)
	{
	image_t *br=creer_image();
	if(br==NULL) return NULL;
	br->w=src->w;
	br->h=src->h;
	br->path=strdup(src->path);
	br->buff=malloc(src->w*src->h*sizeof(char));
	if(br->buff==NULL)
	{
		fprintf(stderr,"Impossible de creer une nouvelle image\n");
	}
	srand(time(NULL));
	unsigned long k;
	for (k = 0; k < src->w*src->h; k++)
	{
		if ((rand()%100)<=pourcent)
		{
			br->buff[k]=rand()%255;
		}
		else
		{
			br->buff[k]=src->buff[k];
		}
	}
	return br;
	}

int tri(unsigned char tab[], int n){
	int i, change=0, end=0;
	while (end!=1)
	{
	end=1;
		for (i = 0; i < n-1; i++)
		{
			if (tab[i]>tab[i+1])
			{
				end=0;
				change=tab[i+1];
				tab[i+1]=tab[i];
				tab[i]=change;
			}
		}
	}
	return (tab[9]+tab[10])/2;
}


image_t *convoluer(image_t *im, noyau_t *h)
	{
	int i,j,k,di,dj,vi,vj;
	image_t *dst=copier_image(im);
	if(dst==NULL || h==NULL) return NULL;
	
	//facteur de normalisation
	int norm=0;
	for(k=0;k<h->dim*h->dim;k++){
		norm+=((h->coeffs[k]>0)?h->coeffs[k]:-h->coeffs[k]);
	}
	
	
	//Parcours de l'image
	for (i = 0; i < im->h; i++)
	{
		for (j = 0; j < im->w; j++)
		{
			//Boucles d'application des modifications crées par le noyau
			int somme=0;
			for (di = 0; di < h->dim; di++)
			{
				for (dj = 0; dj < h->dim; dj++)
				{
					vi=i+di-h->dim/2;
					vj=j+dj-h->dim/2;
					if(vi<0) vi=0;
					if(vi>=im->h) vi=im->h-1;
					if(vj<0) vj=0;
					if(vj>=im->w) vj=im->w-1;
					somme += VAL(im,vi,vj)*h->coeffs[di*h->dim+dj];
				}
			}
			if(norm!=0) VAL(dst,i,j)=somme/norm;
		}
	}
	return dst;
	}
	
image_t *puzzle(image_t *src, int div)
	{
	printf("Pas\n");
	//creation d'une nouvelle image morcelée
	int dw=src->w/2, dh=src->h/2;
	printf("%d %d\n",dw,dh);
	
	//création des quarts d'image
	image_t *puz1=creer_image();
	if(puz1==NULL) return NULL;
	puz1->w=(src->w)-dw;
	puz1->h=dh;
	puz1->path=NULL;
	puz1->buff=malloc(puz1->w*puz1->h*sizeof(unsigned char));
	printf("Pas\n");
	//remplir les quarts d'image	
	int i,j;
	for(j=0;j<dh;j++){
		for (i = dw; i < (src->w); i += 1)
		{
			VAL(puz1,j,i)=VAL(src,j,i);
		}
	}
	printf("Passe\n");
	printf("Passe End\n");
/*	puz0->path=strdup(src->path);*/
	return puz1;
}

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <gtk/gtk.h>
#include "guimpe_callback.h"

#include "image.h"
#define TMP_STR_SIZE 256

//crée une structure image et aloue la taille mémoire spécifique au fichier lu, et recopie les données dans le buffer
image_t *charger_image_pgm(char *nom_fichier)
	{
	FILE *f=fopen(nom_fichier,"r");
	if (f==NULL)
	{
		fprintf(stderr,"Impossible de charger l'image %s\n",nom_fichier);
		return NULL;
	}
	//creation de la nouvelle image par copie de chaque donnée
	image_t *im=creer_image();
	im->path=malloc(sizeof(char)*(strlen(nom_fichier)+1));
	strcpy (im->path,nom_fichier);
		
	
	char ligne[TMP_STR_SIZE];
	//mettre dans 'ligne' la prochaine ligne non commentée
	do{
		fgets(ligne,TMP_STR_SIZE,f);
	}while(ligne[0]=='#');
	//vérifier le format de fichier
	int binaire=0;
	char s[]={'P','2'};
	if(sscanf(ligne,"%s",s)==1){
		
	if(strcmp(s,"P2")==0){//Image en codée en ASCII
		binaire=1;
	}
	if(strcmp(s,"P5")==0){//Image codée en binaire
		binaire=0;
	}
	}
	else{
		fprintf(stderr,"Format du fichier incorrect\n");
		detruire_image(im);
		fclose(f);
		return NULL;
	}
	
	
	do{
		fgets(ligne,TMP_STR_SIZE,f);
	}while(ligne[0]=='#');	
	//lire la largeur et la hauteur
	long w;
	long h;
	if(sscanf(ligne,"%ld %ld",&w,&h)!=2){
		fprintf(stderr,"Contenu du fichier incorrect\n");
		return NULL;
	}
	im->w=w;
	im->h=h;
	
	do{
		fgets(ligne,TMP_STR_SIZE,f);
	}while(ligne[0]=='#');	
	//lire les niveaux de gris
	long int niv_gris=255;
	if(sscanf(ligne,"%ld",&niv_gris)!=1){
		fprintf(stderr,"Contenu du fichier incorrect\n");
		return NULL;
	}
	if(niv_gris!=255){
		fprintf(stderr,"Niveau de gris incorrect\n");
		return NULL;
	}
	
	//copier l'image dans le buffer en fonction du format
	im->buff=malloc(sizeof(unsigned char)*w*h);
	if(binaire==0){
		fread(im->buff,im->w*sizeof(char),im->h,f);
	}
	int k;
	if(binaire==1){
		for (k = 0; k < im->w*im->h; k++)
		{
			if (fscanf(f," %hhu ",&im->buff[k])!=1)
			{
				fprintf(stderr,"Erreur de la partie ASCII de l'image'\n");
				return NULL;
			}
		}
	}
	
	fclose(f);
	return im;
	}

int sauver_image_pgm(char *nom_fichier, image_t *im)
	{
	FILE *f=fopen(nom_fichier,"w");
	if(f==NULL||im==NULL){
	fprintf(stderr,"Impossible de charger l'image %s\n",nom_fichier);
	return 1;
	}
	fprintf(f,"P5\n#Image en .pgm generee par Paul Willot\n");
	fprintf(f,"%ld %ld\n",im->w,im->h);
	fprintf(f,"255\n");
	fwrite(im->buff,sizeof(unsigned char),im->w*im->h,f);
	fclose(f);
	return 0;
	}
	
image_t *charg(char *nom_fichier,char *nom_fichier2)
	{
	image_t *im = charger_image_pgm(nom_fichier);
	image_t *im2=charger_image_pgm(nom_fichier2);
	
	image_t *stg=creer_image();
	if(stg==NULL) return NULL;
	stg->w=im->w;
	stg->h=im->h;
	stg->path=strdup(im->path);
	stg->buff=malloc(im->w*im->h*sizeof(char));
	if(stg->buff==NULL)
	{
		fprintf(stderr,"Impossible de creer une nouvelle image\n");
	}
	unsigned long k;
	//copie de l'inverse pour chaque pixel de l'image
	for (k = 0; k < im->w*im->h; k++)
	{
		stg->buff[k]=im->buff[k]-im2->buff[k];
	}
	return stg;
	}


image_t *c2332_1m4g3(image_t *im, image_t *im2)
	{
	if ((im->w < im2->w)||(im->h < im2->h))
	{
		fprintf(stderr,"Images de largeur/hauteur différentes\n");

		GtkWidget *dialog;
		dialog = gtk_message_dialog_new (NULL,
                                 GTK_DIALOG_DESTROY_WITH_PARENT,
                                 GTK_MESSAGE_ERROR,
                                 GTK_BUTTONS_CLOSE,
                                 "Images de hauteur/largeur différentes!\nVeuillez choisir un image masque de \ntaille suffisante");
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		
		
		return im;
	}
	image_t *br=creer_image();
	if(br==NULL) return NULL;
	br->w=im->w;
	br->h=im->h;
	br->path=strdup(im->path);
	br->buff=malloc(im->w*im->h*sizeof(char));
	if(br->buff==NULL)
	{
		fprintf(stderr,"Impossible de creer une nouvelle image\n");
	}
	unsigned long k;
	for (k = 0; k < im2->w*im2->h; k++)
	{
			
		if (im2->buff[k]>=125)
		{
			br->buff[k]=im->buff[k]-1;
		}
		else
		{
			br->buff[k]=im->buff[k];
		}
	}
	return br;
	}

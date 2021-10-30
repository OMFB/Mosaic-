/*
 * comparim.c
 *
 *  Created on: 5 juin 2009
 *      Author: monga
 */

#include <stdio.h>

#include <errno.h>

/* NEW */

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>  /* unlink */

#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include "geometrie.h"



/* calcul du nombre de points d'une image */

int nbpointim(char *nom1,int dimx,int dimy,int dimz,int tentete)

{

	int i,j,k,n;

	image *ptnom1;



	ptnom1 = lecima(nom1,dimx,dimy,dimz,tentete);

	n = 0;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	{

		if(ptnom1->tab[i][j][k] == 0)

			n++;

	}

	return(n);

}



void testnbpointim(void)

{



	char nom1[50];

	int tentete,dimx,dimy,dimz,n;



	fprintf(stderr,"\n nom de l'image :");

	scanf("%s",nom1);

	fprintf(stderr,"\n dimx dimy dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n taille de l'entete = ");

	scanf("%d",&tentete);

	n = nbpointim(nom1,dimx,dimy,dimz,tentete);

	fprintf(stderr,"\n nb de points a 0 de l'image = %d",n);

}



/* comparaison d'images :

entree : nom1 et nom2 les deux images a comparer, dimx, dimy, dimz

les dimensions des images, tentete taille de l'entete, entete fichier entete



sortie : nom12 (1-2), nom21 (2-1), fichier de statistiques */



void comparim(char *nom1,char *nom2,int dimx,int dimy,int dimz,int tentete, char *entete,char *nom12,char *nom21,char *ficres)

{

	int i,j,k,n12,n21,nb,n1,n2;

	image *ptnom1,*ptnom2,*ptnom;

	float p12,p21;

	FILE *ptfic;



	/* lecture des deux images */

	fprintf(stderr,"\n 1");

	ptnom1 = lecima(nom1,dimx,dimy,dimz,tentete);

	fprintf(stderr,"\n 2");

	ptnom2 = lecima(nom2,dimx,dimy,dimz,tentete);

	fprintf(stderr,"\n 3");



	ptnom = creimage(dimx,dimy,dimz,1);

	fprintf(stderr,"\n 4");





	/* calcul des differences d'images */

	n12 = 0;

	n21 = 0;

	n1 = 0;

	n2 = 0;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	{

		if(ptnom1->tab[i][j][k] == 0)

		{

			n1++;

			if(ptnom2->tab[i][j][k] != 0)

			{

				ptnom->tab[i][j][k] = 0;

				n12++;

			}

			else

				ptnom->tab[i][j][k] = 1;

		}

		else

			ptnom->tab[i][j][k] = 1;

	}

	nb = ecrimage(entete,nom12,ptnom);



	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	{

		if(ptnom2->tab[i][j][k] == 0)

		{

			n2++;

			if(ptnom1->tab[i][j][k] != 0)

			{

				ptnom->tab[i][j][k] = 0;

				n21++;

			}

			else

				ptnom->tab[i][j][k] = 1;

		}

		else

			ptnom->tab[i][j][k] = 1;

	}

	nb = ecrimage(entete,nom21,ptnom);



	/* ecriture des stats dans le fichier */



	ptfic = fopen(ficres,"w");

	fprintf(ptfic," \n nombre de points de la forme 1 = %d",n1);

	fprintf(ptfic," \n nombre de points de la forme 2 = %d",n2);

	fprintf(ptfic," \n nombre de points de la forme 1-2 = %d",n12);

	fprintf(ptfic," \n nombre de points de la forme 2-1 = %d",n21);

	p21 = (float)(n21*100)/(float)n1;

	p12 = (float)(n12*100)/(float)n1;

	fprintf(ptfic,"\n pourcentage de points de la forme	1 non inclus dans la forme 2 = %f",p12);

	fprintf(ptfic,"\n pourcentage de points de la forme	2 non inclus dans la forme 1 = %f",p21);



	fprintf(stderr," \n nombre de points de la forme 1 = %d",n1);

	fprintf(stderr," \n nombre de points de la forme 2 = %d",n2);

	fprintf(stderr," \n nombre de points de la forme 1-2 = %d",n12);

	fprintf(stderr," \n nombre de points de la forme 2-1 = %d",n21);

	fprintf(stderr,"\n pourcentage de points de la forme	1 non inclus dans la forme 2 = %f",p12);

	fprintf(stderr,"\n pourcentage de points de la forme	2 non inclus dans la forme 1 = %f",p21);



	fclose(ptfic);

}



void testcomparim(void)

{

	char nom1[50],nom2[50],nom12[50],nom21[50],stat[50];

	char entete[50];

	int tentete,dimx,dimy,dimz;



	fprintf(stderr,"\n nom des 2 images a comparer :");

	scanf("%s %s",nom1,nom2);

	fprintf(stderr,"\n dimx dimy dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n taille de l'entete = ");

	scanf("%d",&tentete);

	fprintf(stderr,"nom du fichier entete :");

	scanf("%s",entete);

	fprintf(stderr,"\n 1-2 2-1 :");

	scanf("%s %s",nom12,nom21);

	fprintf(stderr,"\n fichier de stats :");

	scanf("%s",stat);



	comparim(nom1,nom2,dimx,dimy,dimz,tentete,entete,nom12,nom21,stat);

}



void testcomparfic(void)

{

	char nom1[50],nom2[50];

	char entete[50];

	int tentete,entier1,entier2,i,k,iok;

	FILE *ptim1,*ptim2;



	fprintf(stderr,"\n nom des 2 fichiers images opendx a comparer (image originale, image de boules):");

	scanf("%s %s",nom1,nom2);

	fprintf(stderr,"\n taille de l'entete = ");

	scanf("%d",&tentete);

	ptim1 = fopen(nom1,"r");

	ptim2 = fopen(nom2,"r");



	/* lecture des entetes */



	fprintf(stderr,"\n lecture des entetes ");

	entier1 = fgetc(ptim1);

	entier2 = fgetc(ptim2);

	if(entier1 != entier2)fprintf(stderr,"\n entier1 = %d ; entier2 = %d",entier1,entier2);

	i = 1;

	while(i <= tentete)

	{

		entier1 = fgetc(ptim1);

		entier2 = fgetc(ptim2);

		if(entier1 != entier2)fprintf(stderr,"\n entier1 = %d ; entier2 = %d",entier1,entier2);

		i++;

	}

	fprintf(stderr,"\n fin de la lecture des entetes ");



	/* les pointeurs sont positionnes sur les premiers octets des images */



	/* lecture des octets des images */



	entier1 = fgetc(ptim1);

	entier2 = fgetc(ptim2);

	k = 1;

	while((entier1 != EOF) || (entier2 != EOF))



	{

		if((entier1 == EOF) || (entier2 == EOF))

		{

			fprintf(stderr,"\n TAILLE DES FICHIERS DIFFERENTES !!!");

			exit(1);

		}

		if(entier2 == 0)

			if(entier1 != 0)

			{

				fprintf(stderr,"\n k = %d ; pour continuer taper 1",k);

				scanf("%d",&iok);

			}

		if((entier1 != 0) && (entier1 != 1))fprintf(stderr,"\n k = %d ; entier1 = %d",k,entier1);

		if((entier2 != 0) && (entier2 != 1))fprintf(stderr,"\n k = %d ; entier2 = %d",k,entier2);

		entier1 = fgetc(ptim1);

		entier2 = fgetc(ptim2);

		k++;

	}







}

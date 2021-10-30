/*
 * squelette.c
 *
 *  Created on: 5 juin 2009
 *      Author: monga
 */

#include <stdio.h>

#include <errno.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>  /* unlink */

#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include "geometrie.h"

/* NEW */



/* cette proc�dure prend en entr�e un fichier de boules de Delaunay et

donne en sortie une image contenant les centres des boules de rayon au

moins �gal � un certain seuil */



void squelette(char *ficboul,char *nomima,char *entete,int dimx,int dimy,int dimz,float seuil)

{

	image *ptimage;

	FILE *ptficboul;

	int nboules,nptsec,npoints,i;

	float xc,yc,zc,r,a,u,v,w;



	ptimage = creimage(dimx,dimy,dimz,1);

	ptficboul = fopen(ficboul,"r");

	fscanf(ptficboul,"%d",&nboules);

	fprintf(stderr,"\n nombre de boules = %d",nboules);

	nptsec = 0;

	for(i=0;i<nboules;i++)

	{

		fscanf(ptficboul,"%f %f %f %f %f %f %f %f",&xc,&yc,&zc,&r,&a,&u,&v,&w);

		if(r >= seuil)

		{

			nptsec++;

			ptimage->tab[neari(xc)][neari(yc)][neari(zc)] = 0;

		}

	}

	fprintf(stderr,"\n nb de points selectionn�s = %d",nptsec);

	fclose(ptficboul);



	/* ecriture de l'image dans le fichier image */

	npoints = ecrimage(entete,nomima,ptimage);

}



/* calcul du squelette filtre en selectionnant les maximas locaux

de l'ouverture et en seuillant sur l'ouverture */



void filtresq(char *ficboul,char *nomima,char *entete,int dimx,int dimy,int dimz,float seuil)

{

	image *ptimage;

	FILE *ptficboul;

	int nboules,nptsec,npoints,i,j,k,l,m,n;

	float xc,yc,zc,r,a,u,v,w,***rayons;



	ptimage = creimage(dimx,dimy,dimz,1);

	rayons = creertab3f(dimx,dimy,dimz);

	ptficboul = fopen(ficboul,"r");

	fscanf(ptficboul,"%d",&nboules);

	fprintf(stderr,"\n nombre de boules = %d",nboules);

	nptsec = 0;

	for(i=0;i<nboules;i++)

	{

		fscanf(ptficboul,"%f %f %f %f %f %f %f %f",&xc,&yc,&zc,&r,&a,&u,&v,&w);

		nptsec++;

		ptimage->tab[neari(xc)][neari(yc)][neari(zc)] = 0;

		rayons[neari(xc)][neari(yc)][neari(zc)] = r;

	}

	fprintf(stderr,"\n nb de points du squelette dans l'image = %d",nptsec);



	/* determination des maxima locaux */

	nptsec = 0;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	if(ptimage->tab[i][j][k] == 0)

	{

		/* regarder si c'est un maxima local */

		for(l=-1;l<=1;l++)

		for(m=-1;m<=1;m++)

		for(n=-1;n<=1;n++)

			if(l != 0 || m != 0 || n != 0)

			if(rayons[i][j][k] < rayons[i+l][j+m][k+n])

			{

				ptimage->tab[i][j][k] = 1;

				break;

				break;

				break;

			}

	}

}









void testsquel(void)

{

	char ficboul[50],nomima[50],entete[50];

	int dimx,dimy,dimz;

	float seuil;



	fprintf(stderr,"\n fichier de boules de Delaunay : ");

	scanf("%s",ficboul);

	fprintf(stderr,"\n minorant pour le rayon = ");

	scanf("%f",&seuil);

	fprintf(stderr,"\n image de sortie du squelette : ");

	scanf("%s",nomima);

	fprintf(stderr,"\n entete de l'image de sortie : ");

	scanf("%s",entete);

	fprintf(stderr,"\n dimx = ");

	scanf("%d",&dimx);

	fprintf(stderr,"\n dimy = ");

	scanf("%d",&dimy);

	fprintf(stderr,"\n dimz = ");

	scanf("%d",&dimz);



	squelette(ficboul,nomima,entete,dimx,dimy,dimz,seuil);

}

/*
 * reduboul.c
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


/* cette procedure prend en entree le fichier des boules tries (sortie de triboules2)

selon les rayons (nb de boules suivi de : coordonnees du centre;

rayon, alllongement, coordonnees du vecteur orientation) et renvoie un

fichier de boules dans la grille discr�te */



void reduboul(char *ficboul,char *ficboulm)

{

	FILE *ptficboul,*ptficboulm;

	int nboules,i,dimx,dimy,dimz,nsquel,j,k,l,m,n;

	float xmax,ymax,zmax,u,v,w,a,*along,*oru,*orv,*orw;

	float xc,yc,zc,r;

	boule **boules;

	image *ptsquel;



	ptficboul = fopen(ficboul,"r");

	/* fichier de boule en entr�e */



	ptficboulm = fopen(ficboulm,"w");

	/* fichier de boules en sortie */



	fscanf(ptficboul,"%d",&nboules);

	printf("\n nboules = %d",nboules);



	/*rangement des boules et des directions et allongements

	correspondant dans les tableaux "boules", "oru", "orv", "orw", "along" */

	boules = (boule**)mycalloc(nboules,sizeof(boule*));

	oru = (float*)mycalloc(nboules,sizeof(float));

	orv = (float*)mycalloc(nboules,sizeof(float));

	orw = (float*)mycalloc(nboules,sizeof(float));

	along = (float*)mycalloc(nboules,sizeof(float));



	xmax = 0;

	ymax = 0;

	zmax = 0;

	for(i=0;i<nboules;i++)

	{

		boules[i] = (boule*)mymalloc(sizeof(boule));

	}

	/* on range dans 'boules' les boules du fichier 'ficboul' dans l'ordre */

	for(i=0;i<nboules;i++)

	{

		fscanf(ptficboul,"%f %f %f %f %f %f %f %f",&xc,&yc,&zc,&r,&a,&u,&v,&w);

		boules[i]->xc = xc;

		boules[i]->yc = yc;

		boules[i]->zc = zc;

		if(xc > xmax)xmax=xc;

		if(yc > ymax)ymax=yc;

		if(zc > zmax)zmax=zc;

		boules[i]->rayon = r;

		along[i] = a;

		oru[i] = u;

		orv[i] = v;

		orw[i] = w;

	}



	fprintf(stderr,"\n fin du rangement des boules");

	fprintf(stderr,"\n xmax = %f ; ymax = %f ; zmax = %f",xmax,ymax,zmax);



	dimx = ceil(xmax) + 1;

	dimy = ceil(ymax) + 1;

	dimz = ceil(zmax) + 1;

	fprintf(stderr,"\n dimx = %d ; dimy = %d ; dimz = %d",dimx,dimy,dimz);



	ptsquel = creimage(dimx,dimy,dimz,0);



	/* ptsquel pointe sur l'image du squelette qui est initialise a 0 */

	for(i=0;i<nboules;i++)

	{

		xc = boules[i]->xc;

		yc = boules[i]->yc;

		zc = boules[i]->zc;

		r = boules[i]->rayon;

		ptsquel->tab[neari(xc)][neari(yc)][neari(zc)] = r;

	}

	/* ptsquel est une image � 0 pour les points qui sont des centres de

	Delaunay et � la valeur du rayon de la boule de Delaunay correspondante sinon */



	/* on compte le nombre de boules de Delaunay dans la grille discr�te */

	nsquel = 0;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

		if(ptsquel->tab[i][j][k] != 0)nsquel++;



	fprintf(stderr,"\n nb de boules dans la grille discrete = %d",nsquel);



	/* on ecrit le nb de boules de Delaunay de la grille discrete dans le fichier de boules en sortie */

	fprintf(ptficboulm,"%d",nsquel);



	for(i=0;i<nboules;i++)

	{

		xc = boules[i]->xc;

		yc = boules[i]->yc;

		zc = boules[i]->zc;

		l = neari(xc) ;

		m = neari(yc) ;

		n = neari(zc) ;

		if(ptsquel->tab[l][m][n] != 0)

		{

			r = boules[i]->rayon;

			a = along[i];

			u = oru[i];

			v = orv[i];

			w = orw[i];

			/* ecriture de la boule dans le fichier de sortie */

			fprintf(ptficboulm,"\n %f %f %f %f %f %f %f %f",xc,yc,zc,r,a,u,v,w);

			ptsquel->tab[l][m][n] = 0;

		}



	}

	fclose(ptficboulm);

	fclose(ptficboul);

}



/* entree : fichier de boules avec les centres dans la grille relle

sortie : fichier de boules avec les centres dans la grille discrete */

void testreduboul(void)

{

	char nom1[50],nom2[50];



	printf("nom du fichier de boules dans la grille continue (x0,y0,z0,r,a,u,v,w): ");

	scanf("%s",nom1);

	printf("\n nom du fichier de boules dans la grille discrete (x0,y0,z0,r,a,u,v,w): ");

	scanf("%s",nom2);

	reduboul(nom1,nom2);

}

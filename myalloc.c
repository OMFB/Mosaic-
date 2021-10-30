/*
 * myalloc.c
 *
 *  Created on: 5 juin 2009
 *      Author: monga
 */
/* definition de fonctions d'allocation de memoire plus sures !! */

/* NEW */

#include <errno.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>  /* unlink */

#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include "geometrie.h"



char *mymalloc(taille)

int taille;

{

	static char *pt;



	pt = (char*)malloc(taille);

	if (pt == NULL) {

	    fprintf(stderr,"\n Probleme de place memoire ... (1) \n");

	    exit(67);

	}

	else return(pt);

}



/* libere la place de l'image pointee par im */

void freeim(image *im)

{

	/* liberation de la place du tableau */

	freet3(im->tab,im->dimx,im->dimy,im->dimz);

	free(im);

}



char *mycalloc(nb,taille)

int taille,nb;

{





	static char *pt;



	pt = (char*)malloc(taille*nb);

	if (pt == NULL) {

	    fprintf(stderr,"\n Probleme de place memoire dans mycalloc ... (2) \n");

	    exit(67);

	}

	else return(pt);

}



void myfreec(pt)

char *pt;

{

	free(pt);

	pt = NULL;

}



void myfreei(pt)

int *pt;

{

	free(pt);

	pt = NULL;

}



void myfreef(pt)
float *pt;

{

	free(pt);

	pt = NULL;

}

void myfreeLf(pt)
long double *pt;

{

	free(pt);

	pt = NULL;

}

void myfreeb(pt)

bool *pt;

{

	free(pt);

	pt = NULL;

}



void myfreed(pt)

double *pt;

{

	free(pt);

	pt = NULL;

}





/* retourne un pointeur sur un tableau d'entiers tab[nli +1][nco + 1]

l'adressage commence � 1 pour la compatibilit� avec les programmes

de creation-graphe */



int **allotab2i(int nli,int nco)

{

	int **tab,i ;



	tab = (int**)mycalloc(nli+1,sizeof(int*));



    for(i=0;i<=nli;i++)

	*(tab + i) = (int*)mycalloc(nco+1,sizeof(int));



	return(tab);

}



/* retourne un pointeur sur un tableau d'entiers tab[dimx][dimy]

l'adressage commence � 0  */



int **allot2i(int dimx,int dimy)

{

	int **tab,i ;



	tab = (int**)mycalloc(dimx,sizeof(int*));



    for(i=0;i<dimx;i++)

	*(tab + i) = (int*)mycalloc(dimy,sizeof(int));



	return(tab);

}

float **allot2f(int dimx,int dimy)

{

	float **tab;

	int i ;



	tab = (float**)mycalloc(dimx,sizeof(float*));



    for(i=0;i<dimx;i++)

	*(tab + i) = (float*)mycalloc(dimy,sizeof(float));



	return(tab);

}





bool **allot2ic(int dimx,int dimy)

{

	bool **tab;

	int i;



	tab = (bool**)mycalloc(dimx,sizeof(bool*));



    for(i=0;i<dimx;i++)

	*(tab + i) = (bool*)mycalloc(dimy,sizeof(bool));



	return(tab);

}

/* retourne un pointeur sur un tableau d'entiers tab[dimx][dimy][dimz] */



int ***creertab3(int dimx,int dimy,int dimz)

{

	int ***tab,i;



	tab = (int***)mycalloc(dimx,sizeof(int**));



    for(i=0;i<dimx;i++)

	*(tab + i) = allot2i(dimy,dimz);



	return(tab);

}



bool ***creertab3c(int dimx,int dimy,int dimz)

{

	bool ***tab;

	int i;



	tab = (bool***)mycalloc(dimx,sizeof(bool**));



    for(i=0;i<dimx;i++)

	*(tab + i) = allot2ic(dimy,dimz);



	return(tab);

}



float ***creertab3f(int dimx,int dimy,int dimz)

{

	float ***tab;

	int i;



	tab = (float***)mycalloc(dimx,sizeof(float**));



    for(i=0;i<dimx;i++)

	*(tab + i) = allot2f(dimy,dimz);



	return(tab);

}



/* libere la place du tableau d'entiers tab[nli + 1][nco + 1] */



void freeti(int **tab,int nli,int nco)

{

	int i;



	for(i=0;i<=nli;i++)

	myfreei(*(tab + i));

	free(tab);

	tab = NULL ;

}



/* libere la place du tableau d'entiers tab[nli][nco] */



void freetin(int **tab,int dimx,int dimy)

{

	int i;



	for(i=0;i<dimx;i++)

	myfreei(*(tab + i));

	free(tab);

	tab = NULL ;

}

/* libere la place du tableau de booleens tab[nli][nco] */



void freet2b(bool **tab,int dimx,int dimy)

{

	int i;



	for(i=0;i<dimx;i++)

	myfreeb(*(tab + i));

	free(tab);

	tab = NULL ;

}

void freetfloat(float **tab,int dimx,int dimy)

{

	int i;


	for(i=0;i<dimx;i++)

	myfreef(*(tab + i));

	free(tab);

	tab = NULL ;

}


/* libere la place du tableau d'entiers tab[dimx][dimy][dimz] */



void freet3(int ***tab,int dimx,int dimy,int dimz)

{

	int i;



	for(i=0;i<dimx;i++)

	freetin(*(tab + i),dimy,dimz);

	free(tab);

	tab = NULL ;

}

/* libere la place du tableau de booleens tab[dimx][dimy][dimz] */

void freet3b(bool ***tab,int dimx,int dimy,int dimz)

{

	int i;



	for(i=0;i<dimx;i++)

	freet2b(*(tab + i),dimy,dimz);

	free(tab);

	tab = NULL ;

}


/*  retourne un pointeur sur un tableau de floats tab[nli + 1][nco + 1] */

float **allotab2f(int nli,int nco)

{



float **tab ;

int i ;



	tab = (float**)mycalloc(nli+1,sizeof(float*));



    for(i=0;i<=nli;i++)

	*(tab + i) = (float*)mymalloc((nco+1)*sizeof(float));



	return(tab);

}





/* libere la place du tableau de floats tab[nli + 1][nco + 1] */



void freetf(float **tab,int nli,int nco)

{

int i;



	for(i=0;i<=nli;i++){

	free(*(tab + i));

	*(tab + i) = NULL;

	}

	free(tab);

	tab = NULL;

}





/*  retourne un pointeur sur un tableau de doubles tab[nli + 1][nco + 1] */

double **allotab2d(int nli,int nco)

{

double **tab;

int i ;



	tab = (double**)mycalloc(nli+1,sizeof(double*));



    for(i=0;i<=nli;i++)

	*(tab + i) = (double*)mycalloc(nco+1,sizeof(double));



	return(tab);

}



/* libere la place du tableau de doubles tab[nli + 1][nco + 1] */



void freetd(double **tab,int nli,int nco)

{

int i;



	for(i=0;i<=nli;i++)

	free(*(tab + i));

	*(tab+i)= NULL ;

	free(tab);

	tab = NULL;

}

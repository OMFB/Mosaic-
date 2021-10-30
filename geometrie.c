/*
 * geometrie.c
 *
 *  Created on: 5 juin 2009
 *      Author: monga
 */



/* Boules, cylindres, cones : initialisation du graphe, gestion des structures geometriques

, squellettisation */

/* NEW */

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

#include <stdbool.h>


void testedp(void)

{

	char nom[50];

	int **points,nbpts;

	char boulmax[50];


	fprintf(stderr,"\n fichier en entree des boules pour obtenir les centres : ");

	scanf("%s",boulmax);

	fprintf(stderr,"\n nom du fichier des boules : %s",boulmax);

	fprintf(stderr,"\n nom du fichier de sortie pour trianguler : ");

	scanf("%s",nom);

	points = centres(boulmax,&nbpts);

	fprintf(stderr,"\n nb de boules = %d",nbpts);

	fprintf(stderr,"\n creation d'un fichier de points pour delaunay");

	creerfic(nom,points,nbpts);

}

void testedpnew(void)

{

	char nom[50];

	int **points,nbpts;

	char boulmax[50];


	fprintf(stderr,"\n fichier en entree des boules pour obtenir les centres : ");

	scanf("%s",boulmax);

	fprintf(stderr,"\n nom du fichier des boules : %s",boulmax);

	fprintf(stderr,"\n nom du fichier de sortie pour trianguler : ");

	scanf("%s",nom);

	points = centresnew(boulmax,&nbpts);

	fprintf(stderr,"\n nb de boules = %d",nbpts);

	fprintf(stderr,"\n creation d'un fichier de points pour delaunay");

	creerfic(nom,points,nbpts);

}

/* Prend en entr�e  un fichier de boules maximales et donne en sortie les centres des boules et

leur nombre */



int **centres(char *bmax,int *nbpts)

{

	FILE *f1file;

	int **points,entier,i,nmaxecor;
	float x,y,z,r,a,b,c,d;
	int ix,iy,iz;


	/* ouverture du fichier des boules */

	f1file = fopen(bmax,"r");

	fscanf(f1file,"%d",&nmaxecor);
	fprintf(stderr,"nb de boules = %d",nmaxecor);

	/* allocation pour le tableau des centres des boules */

	points = allot2i(3,nmaxecor);

	*nbpts = nmaxecor;

	for(i=0;i<nmaxecor;i++)
	{
		fscanf(f1file,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);

		points[0][i] = neari(x);
		points[1][i] = neari(y);
		points[2][i] = neari(z);

	}


	fprintf(stderr,"\n fin de l'ecriture du tableau des centres");


	fclose(f1file);

	return(points);

}



int **centresnew(char *bmax,int *nbpts)

{

	FILE *f1file;

	int **points,entier,i,nmaxecor;
	float x,y,z,r,a,b,c,d;
	int ix,iy,iz;


	/* ouverture du fichier des boules */

	f1file = fopen(bmax,"r");

	fscanf(f1file,"%d",&nmaxecor);
	fprintf(stderr,"nb de boules = %d",nmaxecor);

	/* allocation pour le tableau des centres des boules */

	points = allot2i(3,nmaxecor);

	*nbpts = nmaxecor;

	for(i=0;i<nmaxecor;i++)
	{
		fscanf(f1file,"%f %f %f %f",&x,&y,&z,&r);

		points[0][i] = neari(x);
		points[1][i] = neari(y);
		points[2][i] = neari(z);

	}


	fprintf(stderr,"\n fin de l'ecriture du tableau des centres");


	fclose(f1file);

	return(points);

}



/* g�n�ration d'un ensemble de boules dont les centres sont

port�s par un segment "(x1,y1,z1)-(x2,y2,z2)" et dont les rayons

sont dans le tableau "rayons" qui comprend "nbrayons" floats.

on renvoie un tableau contenant les boules ainsi que leur nombre

"nbboules" */



boule **generboules(float x1,float y1,float z1,float x2,float y2,

float z2,float *rayons,int nbrayons,int *nbboules)

{

	boule **boules;

	float x,y,z,lambda,d,scal;

	int i ;



	fprintf(stderr,"\n x1=%f y1=%f z1=%f nbrayons = %d",x1,y1,z1,nbrayons);

	x = x1;

	y= y1 ;

	z = z1 ;

	boules = (boule**)mycalloc(nbrayons,sizeof(boule*));



	for(i=0;i<nbrayons;i++)

	boules[i] = (boule*)mymalloc(sizeof(boule));



	*nbboules = 0;

	scal = (x-x1)*(x-x2) + (y-y1)*(y-y2) + (z-z1)*(z-z2);

	fprintf(stderr,"\n scal = %f",scal);

	d = 0 ;

	while (scal <= 0)

	{

		lambda = d/sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));

		fprintf(stderr,"\n lambda = %f",lambda);

		x = x1 + lambda*(x2-x1);

		y = y1 + lambda*(y2-y1);

		z = z1 + lambda*(z2-z1);

		scal = (x-x1)*(x-x2) + (y-y1)*(y-y2) + (z-z1)*(z-z2);

		fprintf(stderr,"\n scal = %f",scal);

		if(scal > 0)break;

		(boules[*nbboules])->xc = x;

		(boules[*nbboules])->yc = y;

		(boules[*nbboules])->zc = z;

		(boules[*nbboules])->rayon = rayons[*nbboules];

		fprintf(stderr,"\n x = %f ; y = %f ; z = %f ; rayon= %f",x,y,z,rayons[*nbboules]);

		d = d + rayons[*nbboules];

		fprintf(stderr,"\n d = %f",d);

		(*nbboules)++;

		fprintf(stderr,"\n *nbboules = %d",*nbboules);

		if((*nbboules) >= nbrayons)break;

		d = d + rayons[*nbboules];

		fprintf(stderr,"\n d = %f",d);



	}

	return(boules);

}







/* allocation memoire pour un tableau tab[dimx][dimy][dimz] */

int ***allotab3di(int dimx,int dimy,int dimz)

{

	int ***tab,i ;



	tab = (int***)mycalloc(dimx,sizeof(int**));



    for(i=0;i< dimx;i++)

	*(tab + i) = allotab2i(dimy - 1,dimz -1);



	return(tab);

}





/* creation d'une image d'entiers de dimensions

dimx,dimy,dimz initialement a 0 */

image *cimage3dint(int dimx,int dimy,int dimz)

{

	int i,j,k ;

	image *image3 ;



	image3 = (image*)mymalloc(sizeof(image));

	image3->dimx = dimx ;

	image3->dimy = dimy ;

	image3->dimz = dimz ;



	image3->tab = allotab3di(dimx,dimy,dimz);



	for(i = 0 ; i<dimx ; i++)

	for(j = 0 ; j<dimy ; j++)

	for(k = 0 ; k<dimz ; k++)

	image3->tab[i][j][k] = 0 ;



	return(image3);

}





/* calcul d'un determinant 3 par 3 d'une matrice lu

par colonnes */



float det3(float a1,float a2,float a3,float a4,float a5,float a6,

float a7,float a8,float a9)

{

	float det ;



	det = a1*(a5*a9 - a8*a6) - a2*(a4*a9 - a7*a6) + a3*(a8*a4 - a5*a7);

	return(det);

}



long double det3d(long double a1,long double a2,long double a3,long double a4,long double a5,long double a6,

long double a7,long double a8,long double a9)

{

	long double det ;



	det = a1*(a5*a9 - a8*a6) - a2*(a4*a9 - a7*a6) + a3*(a8*a4 - a5*a7);

	return(det);

}





/* resolution d'un systeme de 3 equations a 3 inconnues

a1.x + b1.y + c1.z = d1.... si une solution sol est mis a 1 et

la solution est x,y,z sinon sol est mis a 0

les entrees et les sorties sont des floats mais les calculs

sont effectues avec des doubles */



void resyst3(float a1,float b1,float c1,float d1,float a2,

float b2,float c2,float d2,float a3,float b3,float c3,float d3,

int *sol,float *x,float *y,float *z)

{

	long double d,dx,dy,dz ;



	d = det3d((long double)a1,(long double)a2,(long double)a3,(long double)b1,

	(long double)b2,(long double)b3,(long double)c1,(long double)c2,(long double)c3);

	if(d == 0)*sol = 0;

	else

	{





		dx = det3d((long double)d1,(long double)d2,(long double)d3,

		(long double)b1,(long double)b2,(long double)b3,(long double)c1,

		(long double)c2,(long double)c3);

		dy = det3d((long double)a1,(long double)a2,(long double)a3,(long double)d1,

		(long double)d2,(long double)d3,(long double)c1,(long double)c2,(long double)c3);

		dz = det3d((long double)a1,(long double)a2,(long double)a3,(long double)b1

		,(long double)b2,(long double)b3,(long double)d1,(long double)d2,(long double)d3);

		*x = (float)(dx/d) ;

		*y = (float)(dy/d) ;

		*z = (float)(dz/d) ;

		*sol = 1 ;

	}

}

/* renvoie 1 si le point (xi,yi,zi) appartient au cylindre fini

defini par cyl et 0 sinon */



int appcyl(cylindre *cyl,int xi,int yi,int zi)

{

	int sol ;

	float x,y,z,x1,y1,z1,x2,y2,z2,r,a,b,c,scal,dis ;



	x1 = cyl->x1;

	y1 = cyl->y1;

	z1 = cyl->z1;

	x2 = cyl->x2;

	y2 = cyl->y2;

	z2 = cyl->z2;

	r = cyl->rayon;

	r = r*r;

	x = xi;

	y =yi;

	z = zi;



	/* calcul de l'intersection de la droite qui porte le

	segment definissant l'axe du cylindre et du plan passant par

	(i,j,k) et perpendiculaire a l'axe du cylindre */



	if(y1-y2 != 0)

	resyst3(y1-y2,x2-x1,0,x1*(y1-y2)+(x2-x1)*y1,0,z1-z2,y2-y1,

	y1*(z1-z2)+z1*(y2-y1),x2-x1,y2-y1,z2-z1,x*(x2-x1)+y*(y2-y1)

	+z*(z2-z1),&sol,&a,&b,&c);

	else

	if(x1-x2 != 0)

	resyst3(y1-y2,x2-x1,0,x1*(y1-y2)+(x2-x1)*y1,z1-z2,0,x2-x1,

	x1*(z1-z2)+z1*(x2-x1),x2-x1,y2-y1,z2-z1,x*(x2-x1)+y*(y2-y1)

	+z*(z2-z1),&sol,&a,&b,&c);

	else

	resyst3(z1-z2,0,x2-x1,x1*(z1-z2)+z1*(x2-x1),0,z1-z2,y2-y1,

	y1*(z1-z2)+z1*(y2-y1),x1-x2,y1-y2,z1-z2,x*(x1-x2)+y*(y1-y2)+z*(z1-z2),

	&sol,&a,&b,&c);



	if(sol != 1)exit(9);



	/* on regarde si (a,b,c) appartient au segment axe du cylindre */

	scal = (x1-a)*(x2-a) + (y1-b)*(y2-b) + (z1-c)*(z2-c) ;

	if(scal > 0)return(0);

	else

	{

		dis = (x-a)*(x-a)+(y-b)*(y-b)+(z-c)*(z-c) ;

		if(dis <= r)return(1);

		else

		return(0);

	}

}





/* calcul de la projection orthogonale d'un point (x,y,z) sur une droite

(x1,y1,z1)-(x2,y2,z2) ; rend 1 si le calcul a reussi et 0 sinon  */

int projorth(float x,float y,float z,float x1,float y1,float z1,float x2,

float y2,float z2,float *a,float *b,float *c)

{

	int sol ;



	/* calcul de l'intersection de la droite qui porte le

	segment definissant l'axe du cylindre et du plan passant par

	(x,y,z) et perpendiculaire a (x1,y1,z1)-(x2,y2,z2)*/



	if(y1-y2 != 0)

	resyst3(y1-y2,x2-x1,0,x1*(y1-y2)+(x2-x1)*y1,0,z1-z2,y2-y1,

	y1*(z1-z2)+z1*(y2-y1),x2-x1,y2-y1,z2-z1,x*(x2-x1)+y*(y2-y1)

	+z*(z2-z1),&sol,a,b,c);

	else

	if(x1-x2 != 0)

	resyst3(y1-y2,x2-x1,0,x1*(y1-y2)+(x2-x1)*y1,z1-z2,0,x2-x1,

	x1*(z1-z2)+z1*(x2-x1),x2-x1,y2-y1,z2-z1,x*(x2-x1)+y*(y2-y1)

	+z*(z2-z1),&sol,a,b,c);

	else

	resyst3(z1-z2,0,x2-x1,x1*(z1-z2)+z1*(x2-x1),0,z1-z2,y2-y1,

	y1*(z1-z2)+z1*(y2-y1),x1-x2,y1-y2,z1-z2,x*(x1-x2)+y*(y1-y2)+z*(z1-z2),

	&sol,a,b,c);



	if(sol != 1)return(0);

	else return(1);

}









/* insertion d'un cylindre fini avec le niveau de gris

color dans une image */

int inscylind(image *ima,cylindre *cyl,int color)

{

	int dimx,dimy,dimz,i,j,k,npoints ;

	int xc,yc,zc,dx,dy,dz,a,b,c;





	dimx = ima->dimx ;

	dimy = ima->dimy ;

	dimz = ima->dimz ;

	npoints = 0 ;

	boitecyl(cyl,&xc,&yc,&zc,&dx,&dy,&dz);

	if(xc < 0)xc=0;

	if(yc < 0)yc=0;

	if(zc < 0)zc=0;

	a = xc + dx;

	b = yc + dy;

	c = zc + dz;

	if(a >= dimx)a=dimx-1;

	if(b >= dimy)b=dimy-1;

	if(c >= dimz)c=dimz-1;

	for(i=xc;i <= a;i++)

	for(j=yc;j <= b;j++)

	for(k=zc;k <= c;k++)

	{

		/* fprintf(stderr,"i=%d;j=%d;k=%d",i,j,k); */

		if(appcyl(cyl,i,j,k) == 1)

		{

			ima->tab[i][j][k] = color;

			/* fprintf(stderr,"[%d,%d,%d]",i,j,k); */

			npoints++;

		}

	}

	return(npoints);

}

/* cette procedure prend en entree un cylindre et rend les

points a coordonnees entieres incluse dans ce cylindre ainsi

que le nb de ces points, PROCEDURE NON TESTEE */

int **ptcylind(cylindre *cyl,int *npoints)

{

	int i,j,k,**points,nptmax ;

	int xc,yc,zc,dx,dy,dz,a,b,c;

	float r,x1,y1,z1,x2,y2,z2,vol,pie;



	*npoints = 0;

	pie = 3.14116;

	x1 = cyl->x1;

	y1 = cyl->y1;

	z1 = cyl->z1;

	x2 = cyl->x2;

	y2 = cyl->y2;

	z2 = cyl->z2;

	r = cyl->rayon;

	vol = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2))*pie*r*r;

	nptmax = neari(vol)*2;

	points = allotab2i(2,nptmax-1);

	boitecyl(cyl,&xc,&yc,&zc,&dx,&dy,&dz);

	a = xc + dx;

	b = yc + dy;

	c = zc + dz;

	for(i=xc;i <= a;i++)

	for(j=yc;j <= b;j++)

	for(k=zc;k <= c;k++)

	{

		if(appcyl(cyl,i,j,k) == 1)

		{

			if(*npoints >= nptmax)

			{

				fprintf(stderr,"\n pb dans ptcylind !!");

				exit(33);

			}

			points[0][*npoints] = i;

			points[1][*npoints] = j;

			points[2][*npoints] = k;

			(*npoints)++;

		}

	}

	return(points);

}





/* renvoie 1 si le point (xi,yi,zi) appartient au cone fini

defini par con et 0 sinon */



int appcone(cone *con,int xi,int yi,int zi)

{

	int sol ;

	float x,y,z,x1,y1,z1,x2,y2,z2,r1,r2,a,b,c,scal,dis ;

	float r,m1m,m1m2 ;



	x1 = con->x1;

	y1 = con->y1;

	z1 = con->z1;

	x2 = con->x2;

	y2 = con->y2;

	z2 = con->z2;

	r1 = con->rayon1;

	r2 = con->rayon2;

	x = xi;

	y = yi;

	z = zi;



	if(y1-y2 != 0)

	resyst3(y1-y2,x2-x1,0,x1*(y1-y2)+(x2-x1)*y1,0,z1-z2,y2-y1,

	y1*(z1-z2)+z1*(y2-y1),x2-x1,y2-y1,z2-z1,x*(x2-x1)+y*(y2-y1)

	+z*(z2-z1),&sol,&a,&b,&c);

	else

	if(x1-x2 != 0)

	resyst3(y1-y2,x2-x1,0,x1*(y1-y2)+(x2-x1)*y1,z1-z2,0,x2-x1,

	x1*(z1-z2)+z1*(x2-x1),x2-x1,y2-y1,z2-z1,x*(x2-x1)+y*(y2-y1)

	+z*(z2-z1),&sol,&a,&b,&c);

	else

	resyst3(z1-z2,0,x2-x1,x1*(z1-z2)+z1*(x2-x1),0,z1-z2,y2-y1,

	y1*(z1-z2)+z1*(y2-y1),x1-x2,y1-y2,z1-z2,x*(x1-x2)+y*(y1-y2)+z*(z1-z2),

	&sol,&a,&b,&c);



	if(sol != 1)exit(9);



	/* on regarde si (a,b,c) appartient au segment axe du cone */

	scal = (x1-a)*(x2-a) + (y1-b)*(y2-b) + (z1-c)*(z2-c) ;

	if(scal > 0)return(0);

	else

	{

		dis = sqrt((x-a)*(x-a)+(y-b)*(y-b)+(z-c)*(z-c)) ;

		m1m = sqrt((a-x1)*(a-x1)+(b-y1)*(b-y1)+(c-z1)*(c-z1));

		m1m2 = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1)+(z2-z1)*(z2-z1));

		r = r1 + ((r2-r1)*m1m)/m1m2;

		if(dis <= r)return(1);

		else return(0);







	}

}





/* insertion d'un cone dans une image avec le niveau de gris "color" */



int inscone(image *ima,cone *con,int color)

{

	int dimx,dimy,dimz,i,j,k,npoints ;





	dimx = ima->dimx ;

	dimy = ima->dimy ;

	dimz = ima->dimz ;

	npoints = 0 ;



	for(i = 0 ; i < dimx ; i++)

	for(j = 0 ; j < dimy ; j++)

	for(k = 0 ; k < dimz ; k++)

	if(appcone(con,i,j,k) == 1)

	{

		ima->tab[i][j][k] = color;

		npoints++;

	}

	return(npoints);

}



/* creation d'une image initialisee a "valeur" de dimension

dimx.dimy.dimz */



image *creimage(int dimx,int dimy,int dimz,int valeur)

{

	image *pt;

	int i,j,k ;



	pt = (image*)mymalloc(sizeof(image));



	pt->dimx = dimx ;

	pt->dimy = dimy ;

	pt->dimz = dimz ;

	pt->tab = creertab3(dimx,dimy,dimz);

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	(pt->tab)[i][j][k] = valeur;



	return(pt);



}

/* creation d'une image de floats initialisee a "valeur" de dimension

dimx.dimy.dimz */



imagef *creimagef(int dimx,int dimy,int dimz,float valeur)

{

	imagef *pt;

	int i,j,k ;



	pt = (imagef*)mymalloc(sizeof(imagef));



	pt->dimx = dimx ;

	pt->dimy = dimy ;

	pt->dimz = dimz ;

	pt->tab = creertab3f(dimx,dimy,dimz);

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	(pt->tab)[i][j][k] = valeur;

	return(pt);

}


/* creation d'une image initialisee de caracteres a "valeur" de dimension

dimx.dimy.dimz */



imagec *creimagec(int dimx,int dimy,int dimz,bool valeur)

{

	imagec *pt;

	int i,j,k ;

	fprintf(stderr,"\n dimx = %d , dimy = %d , dimz = %d , bool = %d",dimx,dimy,dimz,valeur); 

	pt = (imagec*)mymalloc(sizeof(imagec));

	pt->dimx = dimx ;

	pt->dimy = dimy ;

	pt->dimz = dimz ;

	fprintf(stderr,"\n entree dans creertab3c"); 

	pt->tab = creertab3c(dimx,dimy,dimz);

	fprintf(stderr,"\n sortie de creertab3c");

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	(pt->tab)[i][j][k] = valeur;

	return(pt);
}



/* creation d'une boule de centre C(x0,y0,z0) et de rayon r dans une

image d'entiers pointee par "ptimage". les points de la boule sont mis a la valeur

seuil. On renvoie le nb de points de la boule dans l'image

CETTE PROCEDURE EQUIVALENT DE BOULIMAG N'A PAS ETE TESTEE */



int boulimage(float x0,float y0,float z0,float r,image *ptimage,int seuil)

{

	float d,r2,nbpoints ;

	int dimx,dimy,dimz ;

	int i,j,k;



	dimx = ptimage->dimx;

	dimy = ptimage->dimy;

	dimz = ptimage->dimz;



	r2 = r*r ;

	nbpoints = 0 ;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	{

		d = (x0-i)*(x0-i)+(y0-j)*(y0-j)+(z0-k)*(z0-k) ;

		if(d <= r2)

		{

			(ptimage->tab)[i][j][k] = seuil;

			nbpoints++;

		}

	}

	return(nbpoints);

}





/* insertion d'une boule dans une image, les points de la boule

sont colories avec le niveau de gris 'seuil' */



int boulimag(boule *ptboule,image *ptimage,int seuil)

{

	float d,r2,x0,y0,z0,r ;

	int dimx,dimy,dimz,dx,dy,dz,xc,yc,zc ;

	int i,j,k,a,b,c,nbpoints;



	dimx = ptimage->dimx;

	dimy = ptimage->dimy;

	dimz = ptimage->dimz;

	x0 = ptboule->xc ;

	y0 = ptboule->yc ;

	z0 = ptboule->zc ;

	r = ptboule->rayon;

	boiteboul(ptboule,&xc,&yc,&zc,&dx,&dy,&dz);

	r2 = r*r ;

	nbpoints = 0 ;

	if(xc < 0)xc=0;

	if(yc < 0)yc=0;

	if(zc < 0)zc=0;

	a = xc + dx;

	b = yc + dy;

	c = zc + dz;

	if(a >= dimx)a=dimx-1;

	if(b >= dimy)b=dimy-1;

	if(c >= dimz)c=dimz-1;

	for(i=xc;i <= a;i++)

	for(j=yc;j <= b;j++)

	for(k=zc;k <= c;k++)

	{

		d = (x0-i)*(x0-i)+(y0-j)*(y0-j)+(z0-k)*(z0-k) ;

		if(d <= r2)

		{

			(ptimage->tab)[i][j][k] = seuil;

			nbpoints++;

		}

	}

	return(nbpoints);

}


int boulimag2(boule *ptboule,image *ptimage,int seuil,int *nbint)

{

	float d,r2,x0,y0,z0,r ;

	int dimx,dimy,dimz,dx,dy,dz,xc,yc,zc ;

	int i,j,k,a,b,c,nbpoints;


	*nbint = 0;

	dimx = ptimage->dimx;

	dimy = ptimage->dimy;

	dimz = ptimage->dimz;

	x0 = ptboule->xc ;

	y0 = ptboule->yc ;

	z0 = ptboule->zc ;

	r = ptboule->rayon;

	boiteboul(ptboule,&xc,&yc,&zc,&dx,&dy,&dz);

	r2 = r*r ;

	nbpoints = 0 ;

	if(xc < 0)xc=0;

	if(yc < 0)yc=0;

	if(zc < 0)zc=0;

	a = xc + dx;

	b = yc + dy;

	c = zc + dz;

	if(a >= dimx)a=dimx-1;

	if(b >= dimy)b=dimy-1;

	if(c >= dimz)c=dimz-1;

	for(i=xc;i <= a;i++)

	for(j=yc;j <= b;j++)

	for(k=zc;k <= c;k++)

	{

		d = (x0-i)*(x0-i)+(y0-j)*(y0-j)+(z0-k)*(z0-k) ;

		if(d <= r2)

		{
			if((ptimage->tab)[i][j][k] == seuil)((*nbint)++);


			nbpoints++;

		}

	}

	return(nbpoints);

}


/* insertion d'une boule dans une image, les points de la boule

sont colories avec le niveau de gris 'seuil' */



int boulimagf(boule *ptboule,imagef *ptimage,float seuil)

{

	float d,r2,x0,y0,z0,r ;

	int dimx,dimy,dimz,dx,dy,dz,xc,yc,zc ;

	int i,j,k,a,b,c,nbpoints;



	dimx = ptimage->dimx;

	dimy = ptimage->dimy;

	dimz = ptimage->dimz;

	x0 = ptboule->xc ;

	y0 = ptboule->yc ;

	z0 = ptboule->zc ;

	r = ptboule->rayon;

	boiteboul(ptboule,&xc,&yc,&zc,&dx,&dy,&dz);

	r2 = r*r ;

	nbpoints = 0 ;

	if(xc < 0)xc=0;

	if(yc < 0)yc=0;

	if(zc < 0)zc=0;

	a = xc + dx;

	b = yc + dy;

	c = zc + dz;

	if(a >= dimx)a=dimx-1;

	if(b >= dimy)b=dimy-1;

	if(c >= dimz)c=dimz-1;

	for(i=xc;i <= a;i++)

	for(j=yc;j <= b;j++)

	for(k=zc;k <= c;k++)

	{

		d = (x0-i)*(x0-i)+(y0-j)*(y0-j)+(z0-k)*(z0-k) ;

		if(d <= r2)

		{

			(ptimage->tab)[i][j][k] = seuil;

			nbpoints++;

		}

	}

	return(nbpoints);

}


/* ecriture d'une image d'octets pointee par "ptimage" dans le "fichier opendx" a la suite de ce qui est deja

dans le fichier (entete en general). On renvoie le nombre de points

ecrits dans le fichier ; 'nomfic' est le nom du fichier image

en tete et 'nomfic2' le nom du fichier image resultat */



int ecrimage(char *nomfic,char *nomfic2,image *ptimage)

{



	FILE *f1file,*f2file;

	int i,j,k,entier,nbpoints,dimx,dimy,dimz ;

	int nvide,nforme ;



	f1file = fopen(nomfic,"r");

	f2file = fopen(nomfic2,"w");

	fprintf(stderr,"\n fichier entete ouvert : %s \n",nomfic);

	fprintf(stderr,"\n fichier image cree : %s \n",nomfic2);

	nbpoints = 0 ;

	dimx = ptimage->dimx;

	dimy = ptimage->dimy;

	dimz = ptimage->dimz;

	entier = fgetc(f1file);

	while(entier != EOF)

	{

		fputc(entier,f2file);

		entier = fgetc(f1file);

	}

	fprintf(stderr,"\n fin de l'ecriture de l'en tete");

	nvide = 0;

	nforme = 0;

	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	for(i=0;i<dimx;i++)

	{

		entier = (ptimage->tab)[i][j][k];

		if(entier == 0)nforme++;

		else nvide++;

		fputc(entier,f2file);

		nbpoints++;

	}

	fclose(f1file);

	fclose(f2file);

	return(nbpoints);

}



int ecrimagec(char *nomfic,char *nomfic2,imagec *ptimage)

{



	FILE *f1file,*f2file;

	int i,j,k,entier,nbpoints,dimx,dimy,dimz ;

	int nvide,nforme ;



	f1file = fopen(nomfic,"r");

	f2file = fopen(nomfic2,"w");

	fprintf(stderr,"\n fichier entete ouvert : %s \n",nomfic);

	fprintf(stderr,"\n fichier image cree : %s \n",nomfic2);

	nbpoints = 0 ;

	dimx = ptimage->dimx;

	dimy = ptimage->dimy;

	dimz = ptimage->dimz;

	entier = fgetc(f1file);

	while(entier != EOF)

	{

		fputc(entier,f2file);

		entier = fgetc(f1file);

	}

	fprintf(stderr,"\n fin de l'ecriture de l'en tete");

	nvide = 0;

	nforme = 0;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	{

		if(ptimage->tab[i][j][k] == true)

		entier = 1;

		else

		entier=0;



		if(entier == 0)nforme++;

		else nvide++;

		fputc(entier,f2file);

		nbpoints++;

	}

	fclose(f1file);

	fclose(f2file);

	return(nbpoints);

}



/* lit une image dans un fichier de format "native fila data opendx"

et ecrit la meme image dans un autre fichier */



int testim(void)

{

	char nomimage[50],nomim[50];

	int i;

	int entier,nbitun,nbitzero,nbit,nbentete;

	FILE *f1file,*f2file;



	fprintf(stderr,"nom de l'image en format opendx : ");

	scanf("%s",nomimage);

	fprintf(stderr,"%s",nomimage);

	fprintf(stderr,"\n nom du fichier : ");

	scanf("%s",nomim);

	fprintf(stderr,"%s",nomim);

	fprintf(stderr,"\n nombre de caracteres de l'entete (690 pour 'originale.dx', 689 pour cylind) = ");

	scanf("%d",&nbentete);

	f2file = fopen(nomim,"w");

	f1file = fopen(nomimage,"r");

	nbitun = 0;

	nbitzero = 0;

	nbit = 0;

	fprintf(stderr,"\n lecture des octets de l image ");

	entier = fgetc(f1file);

	i = 1;

	while(entier != EOF)

	{

		fputc(entier,f2file);

		if(i > nbentete)

		{

			nbit++;

			if(entier == 1)nbitun++;

			else if(entier == 0)

			nbitzero++;

		}

		entier = fgetc(f1file);

		i++;



	}

	fprintf(stderr,"nb octets = %d ; nb octets a 1 = %d ; nb octets a zero = %d",nbit,nbitun,nbitzero);

	fprintf(stderr,"\n fin de la lecture de l image ");

	fclose(f1file);

	fclose(f2file);

	return 0;

}



/* Prend en entr�e une image volumique contenant une forme etiquetee a etiq

sur un  fond a 1 et donne en sortie les points de l'ecorce de la forme et

le nombre de points de l'ecorce ; 'tentete' est la taille de l'entete (691 pour

originale.dx, 690 pour cylind.dx) */



int **ecorce(char *nomim,int tentete,int etiq,int dimx,int dimy,int dimz,int nmaxecor,int *nbpts)

{

	FILE *f1file;

	image *ptimage;

	int **points,entier,i,j,k,d1,d2,d3,x,y,z,nbecorce,nbforme;

	int nbuns, nbzeros;



	/* fprintf(stderr,"\n etiq = %d",etiq); */

	/* allocation pour le tableau des points de l'ecorce */

	points = allot2i(3,nmaxecor);



	/* ouverture du fichier image */

	f1file = fopen(nomim,"r");

	ptimage = creimage(dimx,dimy,dimz,1);



	/* ecriture de l'image contenue dans le fichier .dx : nomim dans l'image

	pointee par ptimage */



	/* lecture de l'entete */



	fprintf(stderr,"\n lecture de l'entete ");

	entier = fgetc(f1file);

	i = 1;

	while(i <= tentete)

	{

		entier = fgetc(f1file);

		i++;

	}



	fprintf(stderr,"\n fin de la lecture de l'entete ");



	/* le pointeur est positionne sur le premier octet de l'image */



	/* lecture des octets de l'image */

	nbuns = 0;

	nbzeros =0;

	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	for(i=0;i<dimx;i++)

	{

		entier = fgetc(f1file);

		if(entier == EOF)

		{

			fprintf(stderr,"\n 11 ; i = %d, j=%d, k=%d",i,j,k);

			exit(67);

		}

		(ptimage->tab)[i][j][k] = entier;

		if(entier == etiq)nbzeros++;

		else nbuns++;

		/* fprintf(stderr,"%d;",entier);  */

	}

	entier = fgetc(f1file);

	if(entier != EOF)

	{

		printf("\n 12 ");

		exit(12);

	}



	fprintf(stderr,"\n fin de l'ecriture du tableau des points de l'image");

	fprintf(stderr,"\n nb de pts de la forme = %d nb de pts du fond = %d",nbzeros,nbuns);



	/* les octets de l'image sont dans le tableau ptimage->tab */





	/* recherche des points de l'ecorce */

	nbecorce = 0;

	nbforme=0;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	{

		if(ptimage->tab[i][j][k] == etiq)

		{

			nbforme++;

			/* on regarde si le point (i,j,k) appartient a l'ecorce */

			for(d1=-1;d1 <= 1;d1++)

			for(d2=-1;d2 <= 1;d2++)

			for(d3=-1;d3 <= 1;d3++)

			if((d1==0 && d2==0)||(d1==0 && d3==0)||(d2==0 && d3==0))

			{

				x = i + d1;

				y = j + d2;

				z = k + d3;

				if((x>=0)&&(x<dimx)&&(y>=0)&&(y<dimy)&&(z>=0)&&(z<dimz))

				{

					if((ptimage->tab)[x][y][z] != etiq)

					{

						if(nbecorce >= nmaxecor)

						{

							fprintf(stderr,"\n nb max de pts de l'ecorce depasse , i=%d,j=%d,k=%d",i,j,k);

							exit(15);

						}

						points[0][nbecorce] = i ;

						points[1][nbecorce] = j ;

						points[2][nbecorce] = k ;

						nbecorce++;

						goto un;



					}

				}

			}

		}



un:	/*---------------------------------------*/ ;

	}

	*nbpts = nbecorce;

	fprintf(stderr,"\n nombre de points de la forme = %d",nbforme);

	fprintf(stderr,"\n nombre de points de l'ecorce = %d",nbecorce);

	fprintf(stderr,"\n fin de l'ecriture du tableau des points de l'ecorce");



	/* liberation de la place occupee par l'image */



	freet3(ptimage->tab,ptimage->dimx,ptimage->dimy,ptimage->dimz);

	free(ptimage);

	fclose(f1file);

	return(points);

}


int **ecorce2(char *nomim,int tentete,int etiq,int dimx,int dimy,int dimz,

int nmaxecor,int *nbpts)

{

	FILE *f1file;

	image *ptimage;

	int **points,entier,i,j,k,d1,d2,d3,x,y,z,nbecorce,nbforme;

	int nbuns, nbzeros;



	/* fprintf(stderr,"\n etiq = %d",etiq); */

	/* allocation pour le tableau des points de l'ecorce */

	points = allot2i(3,nmaxecor);



	/* ouverture du fichier image */

	f1file = fopen(nomim,"r");

	ptimage = creimage(dimx,dimy,dimz,1);



	/* ecriture de l'image contenue dans le fichier .dx : nomim dans l'image

	pointee par ptimage */



	/* lecture de l'entete */



	fprintf(stderr,"\n lecture de l'entete ");

	entier = fgetc(f1file);

	i = 1;

	while(i <= tentete)

	{

		entier = fgetc(f1file);

		i++;

	}



	fprintf(stderr,"\n fin de la lecture de l'entete ");



	/* le pointeur est positionne sur le premier octet de l'image */



	/* lecture des octets de l'image */

	nbuns = 0;

	nbzeros =0;

	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	for(i=0;i<dimx;i++)

	{

		entier = fgetc(f1file);

		if(entier == EOF)

		{

			fprintf(stderr,"\n 11 ; i = %d, j=%d, k=%d",i,j,k);

			exit(67);

		}

		(ptimage->tab)[i][j][k] = entier;

		if(entier == etiq)nbzeros++;

		else nbuns++;

		/* fprintf(stderr,"%d;",entier);  */

	}

	entier = fgetc(f1file);

	if(entier != EOF)

	{

		printf("\n 12 ");

		exit(12);

	}



	fprintf(stderr,"\n fin de l'ecriture du tableau des points de l'image");

	fprintf(stderr,"\n nb de pts de la forme = %d nb de pts du fond = %d",nbzeros,nbuns);



	/* les octets de l'image sont dans le tableau ptimage->tab */





	/* recherche des points de l'ecorce */

	nbecorce = 0;

	nbforme=0;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	{

		if(ptimage->tab[i][j][k] != etiq)

		{

			nbforme++;

			/* on regarde si le point (i,j,k) appartient a l'ecorce */

			for(d1=-1;d1 <= 1;d1++)

			for(d2=-1;d2 <= 1;d2++)

			for(d3=-1;d3 <= 1;d3++)

			if((d1==0 && d2==0)||(d1==0 && d3==0)||(d2==0 && d3==0))

			{

				x = i + d1;

				y = j + d2;

				z = k + d3;

				if((x>=0)&&(x<dimx)&&(y>=0)&&(y<dimy)&&(z>=0)&&(z<dimz))

				{

					if((ptimage->tab)[x][y][z] == etiq)

					{

						if(nbecorce >= nmaxecor)

						{

							fprintf(stderr,"\n nb max de pts de l'ecorce depasse , i=%d,j=%d,k=%d",i,j,k);

							exit(15);

						}

						points[0][nbecorce] = i ;

						points[1][nbecorce] = j ;

						points[2][nbecorce] = k ;

						nbecorce++;

						goto un;



					}

				}

			}

		}



un:	/*---------------------------------------*/ ;

	}

	*nbpts = nbecorce;

	fprintf(stderr,"\n nombre de points hors de la forme = %d",nbforme);

	fprintf(stderr,"\n nombre de points de l'ecorce = %d",nbecorce);

	fprintf(stderr,"\n fin de l'ecriture du tableau des points de l'ecorce");



	/* liberation de la place occupee par l'image */



	freet3(ptimage->tab,ptimage->dimx,ptimage->dimy,ptimage->dimz);

	free(ptimage);

	fclose(f1file);

	return(points);

}

void testverification()
{
	char ficxavier[50],imageval[50],boulesmax[50];

	fprintf(stderr,"\n fichier format xavier ; fichier image espace poral (512x512x512) ; fichier boules maximales");
	scanf("%s %s %s",ficxavier,imageval,boulesmax);
	verification(ficxavier,imageval,boulesmax);
}

/* Cette procedure verifie que les points du fichier de Xavier sont bien dans l'image de l'espace poral 
ficxavier est le fichier contenant les positions des spots de bacteries, boulesmax est le fichier des boules maximum
on calcule aussi le pourcentage de l'espace poral non couvert par les boules */
void verification(char *ficxavier,char *imageval,char *boulesmax)
{
	FILE *ptficxavier, *ptimageval, *ptboulesmax; 
	imagec *ptimage;
	float x,y,z,r,sommevol; 
	int k,j,i,dimx,dimy,dimz,nbuns,nbzeros,etiq,nbpatchs,x1,y1,z1,nbmic,nboules,entier; 

	ptficxavier = fopen(ficxavier,"r"); 
	ptimageval = fopen(imageval,"r");
	ptboulesmax = fopen(boulesmax,"r");

	dimx = 512;
	dimy = 512;
	dimz = 512; 
	etiq = 0;

	/* on met l'image de l'espace poral dans une image de booleens */
	ptimage = creimagec(dimx,dimy,dimz,true);

	nbuns = 0;

	nbzeros =0;

	/* comptage des entiers dans le fichier */

	fprintf(stderr,"\n entree dans la boucle de comptage, dimx = %d, dimy = %d , dimz = %d",dimx,dimy,dimz);

	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	for(i=0;i<dimx;i++)

	{

		fscanf(ptimageval,"%d",&entier);

		if(entier == EOF)

			{

				fprintf(stderr,"\n 11 ; i = %d, j=%d, k=%d, fichier incomplet",i,j,k);

				exit(67);

			}
		else
		if(entier == etiq)
		{
			(ptimage->tab)[i][j][k] = false;
			nbzeros++;
		}
		else 
		{
			(ptimage->tab)[i][j][k] = true;
			nbuns++;
		}

	}

	fprintf(stderr,"\n fin de l'ecriture du tableau des points de l'image");

	fprintf(stderr,"\n nb de pts de la forme = %d nb de pts du fond = %d",nbzeros,nbuns);

	/* on regarde si les points du fichier de Xavier sont bien dans l'image */
	fscanf(ptficxavier,"%d",&nbpatchs); 

	for(i=0;i<nbpatchs;i++)
	{
		fscanf(ptficxavier,"%d %d %d %d",&x1,&y1,&z1,&nbmic); 
		if(ptimage->tab[x1][y1][z1] != false)
		{
			fprintf(stderr,"\n point (%d,%d,%d) de Xavier pas dans l'espace poral",x1,y1,z1); 
		}
	}

	/* calcul de la somme des volumes des boules */
	sommevol = 0; 
	fscanf(ptboulesmax,"%d",&nboules); 
	for(i=0;i<nboules;i++)
	{
		fscanf(ptboulesmax,"%f %f %f %f",&x,&y,&z,&r); 
		sommevol += (4.0/3.0)*3.1416*r*r*r; 
	}
	fprintf(stderr,"\n somme du volume des boules / volume espace poral = %f ",sommevol/(float)nbzeros);

}



int **ecorceval(char *nomim,int etiq,int dimx,int dimy,int dimz,int nmaxecor,int *nbpts)

{

	FILE *f1file;

	imagec *ptimage;

	int **points,entier,i,j,k,d1,d2,d3,x,y,z,nbecorce,nbforme,nbent;

	int nbuns, nbzeros;

	fprintf(stderr,"\n entree dans ecorceval"); 
	/* allocation pour le tableau des points de l'ecorce */

	points = allot2i(3,nmaxecor);

	/* ouverture du fichier image */

	f1file = fopen(nomim,"r");

	/*creation d'une image binaire (de booleens) de dimensions dimx.dimy.dimz que l'on initialise à 1 */

	fprintf(stderr,"\n entree dans creimagec"); 

	ptimage = creimagec(dimx,dimy,dimz,true);

	fprintf(stderr,"\n sortie de creimagec "); 


	/* ecriture de l'image contenue dans le fichier nomim (suite d'entiers sur 4 octets) dans l'image

	pointee par ptimage */


	/* le pointeur est positionne sur le premier entier de l'image */


	/* lecture des entiers de l'image */

	nbuns = 0;

	nbzeros =0;

	/* comptage des entiers dans le fichier */

	fprintf(stderr,"\n entree dans la boucle de comptage, dimx = %d, dimy = %d , dimz = %d",dimx,dimy,dimz);

	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	for(i=0;i<dimx;i++)

	{

		fscanf(f1file,"%d",&entier);

		if(entier == EOF)

			{

				fprintf(stderr,"\n 11 ; i = %d, j=%d, k=%d, fichier incomplet",i,j,k);

				exit(67);

			}
		else
		if(entier == etiq)
		{
			(ptimage->tab)[i][j][k] = false;
			nbzeros++;
		}
		else 
		{
			(ptimage->tab)[i][j][k] = true;
			nbuns++;
		}

	}

	fprintf(stderr,"\n fin de l'ecriture du tableau des points de l'image");

	fprintf(stderr,"\n nb de pts de la forme = %d nb de pts du fond = %d",nbzeros,nbuns);



	/* les valeurs boolennes de l'image sont dans le tableau ptimage->tab */

	/* recherche des points de l'ecorce, on considere que les points frontieres sont en dehors de la forme */

	nbecorce = 0;

	nbforme=0;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	{

		if(ptimage->tab[i][j][k] == true)

		{

			nbforme++;

			/* on regarde si le point (i,j,k) appartient a l'ecorce */

			for(d1=-1;d1 <= 1;d1++)

			for(d2=-1;d2 <= 1;d2++)

			for(d3=-1;d3 <= 1;d3++)

			if((d1==0 && d2==0)||(d1==0 && d3==0)||(d2==0 && d3==0))

			{

				x = i + d1;

				y = j + d2;

				z = k + d3;

				if((x>=0)&&(x<dimx)&&(y>=0)&&(y<dimy)&&(z>=0)&&(z<dimz))

				{

					if((ptimage->tab)[x][y][z] == false)

					{

						if(nbecorce >= nmaxecor)

						{

							fprintf(stderr,"\n nb max de pts de l'ecorce depasse , i=%d,j=%d,k=%d",i,j,k);

							exit(15);

						}
						else
						{

							points[0][nbecorce] = i ;

							points[1][nbecorce] = j ;

							points[2][nbecorce] = k ;

							nbecorce++;

							goto un;
						}



					}

				}

			}

		}



un:	/*---------------------------------------*/ ;

	}

	*nbpts = nbecorce;

	fprintf(stderr,"\n nombre de points hors de la forme = %d",nbforme);

	fprintf(stderr,"\n nombre de points de l'ecorce = %d",nbecorce);

	fprintf(stderr,"\n fin de l'ecriture du tableau des points de l'ecorce");



	/* liberation de la place occupee par l'image */

	freet3b(ptimage->tab,ptimage->dimx,ptimage->dimy,ptimage->dimz);

	free(ptimage);

	fclose(f1file);

	fprintf(stderr,"\n sortie de ecorceval"); 

	return(points);

}


void testecor(void)

{

	char nomimage[50],nomfic[50],nomfic2[50],nom[50];

	int nbentete,dimx,dimy,dimz ;

	int **points,nbpts,nmaxecor,npoints,etiq ;

	image *ptimage ;



	fprintf(stderr,"\n nom de l'image de la forme volumique en format opendx : ");

	scanf("%s",nomimage);

	fprintf(stderr,"\n nombre de caracteres de l'entete (690 pour 'originale.dx', 689 pour cylind) = ");

	scanf("%d",&nbentete);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n nombre max de pts de l'ecorce = ");

	scanf("%d",&nmaxecor);

	fprintf(stderr,"\n niveau de gris des points de la forme = ");

	scanf("%d",&etiq);

	points = ecorce(nomimage,nbentete,etiq,dimx,dimy,dimz,nmaxecor,&nbpts);

	fprintf(stderr,"\n nb de pts de l'ecorce = %d",nbpts);



	fprintf(stderr,"\n creation d'une image avec l'ecorce ");

	fprintf(stderr,"nom du fichier entete en format opendx (image d'octets): ");

	scanf("%s",nomfic);

	fprintf(stderr,"\n %s",nomfic);

	fprintf(stderr,"\n nom du fichier image en format opendx (image d'octets): ");

	scanf("%s",nomfic2);

	fprintf(stderr,"\n %s",nomfic2);

	ptimage = cimagtab(points,nbpts,0,1,dimx,dimy,dimz);

	npoints = ecrimage(nomfic,nomfic2,ptimage);



	fprintf(stderr,"\n creation d'un fichier de points pour delaunay");

	fprintf(stderr,"\n nom du fichier pour trianguler : ");

	scanf("%s",nom);

	creerfic(nom,points,nbpts);

}

void testecor2(void)

{

	char nomimage[50],nomfic[50],nomfic2[50],nom[50];

	int nbentete,dimx,dimy,dimz ;

	int **points,nbpts,nmaxecor,npoints,etiq ;

	image *ptimage ;



	fprintf(stderr,"\n nom de l'image de la forme volumique en format opendx : ");

	scanf("%s",nomimage);

	fprintf(stderr,"\n nombre de caracteres de l'entete (690 pour 'originale.dx', 689 pour cylind) = ");

	scanf("%d",&nbentete);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n nombre max de pts de l'ecorce = ");

	scanf("%d",&nmaxecor);

	fprintf(stderr,"\n niveau de gris des points de la forme = ");

	scanf("%d",&etiq);

	points = ecorce2(nomimage,nbentete,etiq,dimx,dimy,dimz,nmaxecor,&nbpts);

	fprintf(stderr,"\n nb de pts de l'ecorce = %d",nbpts);



	fprintf(stderr,"\n creation d'une image avec l'ecorce ");

	fprintf(stderr,"nom du fichier entete en format opendx (image d'octets): ");

	scanf("%s",nomfic);

	fprintf(stderr,"\n %s",nomfic);

	fprintf(stderr,"\n nom du fichier image en format opendx (image d'octets): ");

	scanf("%s",nomfic2);

	fprintf(stderr,"\n %s",nomfic2);

	ptimage = cimagtab(points,nbpts,0,1,dimx,dimy,dimz);

	npoints = ecrimage(nomfic,nomfic2,ptimage);



	fprintf(stderr,"\n creation d'un fichier de points pour delaunay");

	fprintf(stderr,"\n nom du fichier pour trianguler : ");

	scanf("%s",nom);

	creerfic(nom,points,nbpts);

}



/* creation d'une image a partir d'un tableau de points , l'image

cree met a 'forme' les points du tableau de points et a 'vide'

les autres */



image *cimagtab(int **points,int nbpts,int forme,int vide,

int dimx,int dimy,int dimz)

{

	int i,j,k ;

	image *image3 ;



	image3 = (image*)mymalloc(sizeof(image));

	image3->dimx = dimx ;

	image3->dimy = dimy ;

	image3->dimz = dimz ;



	image3->tab = allotab3di(dimx,dimy,dimz);



	for(i = 0 ; i<dimx ; i++)

	for(j = 0 ; j<dimy ; j++)

	for(k = 0 ; k<dimz ; k++)

	image3->tab[i][j][k] = vide ;



	for(i = 0 ; i < nbpts ; i++)

		image3->tab[points[0][i]][points[1][i]][points[2][i]] = forme ;



	return(image3);

}



/* creation d'un fichier de points de nom 'nom' de format

compatible avec le Delaunay de PLG a partir d'un tableau de

points */



void creerfic(char *nom,int **points, int nbpts)

{

	FILE *ptnom;

	int i,entier;

	float x,y,z;



	/* creation et ouverture du fichier resultat en ecriture */

	ptnom = fopen(nom,"w");



	/* ecriture des donnees dans le fichier */

	fprintf(ptnom,"%d",nbpts);

	for(i=0;i<nbpts;i++)

	{

		x = (float)(points[0][i]);

		y = (float)(points[1][i]);

		z = (float)(points[2][i]);

		entier = 0;



		fprintf(ptnom,"\n %f %f %f %d",x,y,z,entier);

	}



	fclose(ptnom);

}





/* determination de l'equation d'un tetraedre et stockage

dans la structure tetraedre, renvoie 1 si c'est un tetraedre

et 0 si c'est un triangle ou si les points sont coplanaires */



int eqtetra(tetra *tet)

{

	int x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;

	int res ;

	float a123,b123,c123,d123;

	float a234,b234,c234,d234;

	float a134,b134,c134,d134;

	float a124,b124,c124,d124;





	x1 = tet->x1;

	x2 = tet->x2;

	x3 = tet->x3;

	x4 = tet->x4;

	y1 = tet->y1;

	y2 = tet->y2;

	y3 = tet->y3;

	y4 = tet->y4;

	z1 = tet->z1;

	z2 = tet->z2;

	z3 = tet->z3;

	z4 = tet->z4;



	/* on verifie que les points ne sont pas coplanaires */

	if(det3((float)(x2-x1),(float)(y2-y1),(float)(z2-z1),

	(float)(x3-x1),(float)(y3-y1),(float)(z3-z1),

	(float)(x4-x1),(float)(y4-y1),(float)(z4-z1))==0)

	{

		tet->a1 = 0;

		tet->b1 = 0;

		tet->c1 = 0;

		tet->d1 = 0;

		tet->a2 = 0;

		tet->b2 = 0;

		tet->c2 = 0;

		tet->d2 = 0;

		tet->a3 = 0;

		tet->b3 = 0;

		tet->c3 = 0;

		tet->d3 = 0;

		tet->a4 = 0;

		tet->b4 = 0;

		tet->c4 = 0;

		tet->d4 = 0;

		return(0);

	}





	/* determination des equations des plans des 4 faces

	du tetraedre */

	res = eqplan(x1,y1,z1,x2,y2,z2,x3,y3,z3,&a123,&b123,&c123,

	&d123);

	if(res == 0)

	{

		tet->a1 = 0;

		tet->b1 = 0;

		tet->c1 = 0;

		tet->d1 = 0;

		tet->a2 = 0;

		tet->b2 = 0;

		tet->c2 = 0;

		tet->d2 = 0;

		tet->a3 = 0;

		tet->b3 = 0;

		tet->c3 = 0;

		tet->d3 = 0;

		tet->a4 = 0;

		tet->b4 = 0;

		tet->c4 = 0;

		tet->d4 = 0;

		return(0);

	}

	if(a123*x4 + b123*y4 + c123*z4 - d123 < 0)

	{

		a123 = -a123;

		b123 = -b123;

		c123 = -c123;

	}



	res = eqplan(x2,y2,z2,x3,y3,z3,x4,y4,z4,&a234,&b234,&c234,

	&d234);

	if(res == 0)

	{

		tet->a1 = 0;

		tet->b1 = 0;

		tet->c1 = 0;

		tet->d1 = 0;

		tet->a2 = 0;

		tet->b2 = 0;

		tet->c2 = 0;

		tet->d2 = 0;

		tet->a3 = 0;

		tet->b3 = 0;

		tet->c3 = 0;

		tet->d3 = 0;

		tet->a4 = 0;

		tet->b4 = 0;

		tet->c4 = 0;

		tet->d4 = 0;

		return(0);

	}

	if(a234*x1 + b234*y1 + c234*z1 - d234 < 0)

	{

		a234 = -a234;

		b234 = -b234;

		c234 = -c234;

		d234 = -d234;

	}



	res = eqplan(x1,y1,z1,x3,y3,z3,x4,y4,z4,&a134,&b134,&c134,

	&d134);

	if(res == 0)

	{

		tet->a1 = 0;

		tet->b1 = 0;

		tet->c1 = 0;

		tet->d1 = 0;

		tet->a2 = 0;

		tet->b2 = 0;

		tet->c2 = 0;

		tet->d2 = 0;

		tet->a3 = 0;

		tet->b3 = 0;

		tet->c3 = 0;

		tet->d3 = 0;

		tet->a4 = 0;

		tet->b4 = 0;

		tet->c4 = 0;

		tet->d4 = 0;

		return(0);

	}

	if(a134*x2 + b134*y2 + c134*z2 - d134 < 0)

	{

		a134 = -a134;

		b134 = -b134;

		c134 = -c134;

		d134 = -d134;

	}



	res = eqplan(x1,y1,z1,x2,y2,z2,x4,y4,z4,&a124,&b124,&c124,

	&d124);

	if(res == 0)

	{

		tet->a1 = 0;

		tet->b1 = 0;

		tet->c1 = 0;

		tet->d1 = 0;

		tet->a2 = 0;

		tet->b2 = 0;

		tet->c2 = 0;

		tet->d2 = 0;

		tet->a3 = 0;

		tet->b3 = 0;

		tet->c3 = 0;

		tet->d3 = 0;

		tet->a4 = 0;

		tet->b4 = 0;

		tet->c4 = 0;

		tet->d4 = 0;

		return(0);

	}

	if(a124*x3 + b124*y3 + c124*z3 - d124 < 0)

	{

		a124 = -a124;

		b124 = -b124;

		c124 = -c124;

		d124 = -d124;

	}



	tet->a1 = a123;

	tet->b1 = b123;

	tet->c1 = c123;

	tet->d1 = d123;



	tet->a2 = a234;

	tet->b2 = b234;

	tet->c2 = c234;

	tet->d2 = d234;



	tet->a3 = a134;

	tet->b3 = b134;

	tet->c3 = c134;

	tet->d3 = d134;



	tet->a4 = a124;

	tet->b4 = b124;

	tet->c4 = c124;

	tet->d4 = d124;



	return(1);



}





/* renvoie 1 si le point (x,y,z) appartient au teraedre pointe

par tetra et 0 sinon */

int aptetra(int x,int y,int z,tetra *tet)

{

	float a1,b1,c1,d1,a2,b2,c2,d2,a3,b3,c3,d3,a4,b4,c4,d4;



	a1 = tet->a1;

	b1 = tet->b1;

	c1 = tet->c1;

	d1 = tet->d1;



	a2 = tet->a2;

	b2 = tet->b2;

	c2 = tet->c2;

	d2 = tet->d2;



	a3 = tet->a3;

	b3 = tet->b3;

	c3 = tet->c3;

	d3 = tet->d3;



	a4 = tet->a4;

	b4 = tet->b4;

	c4 = tet->c4;

	d4 = tet->d4;



	if((a1*x+b1*y+c1*z-d1 >= 0)&&(a2*x+b2*y+c2*z-d2 >= 0)&&

	(a3*x+b3*y+c3*z-d3 >= 0)&&(a4*x+b4*y+c4*z-d4 >= 0))

		return(1);

	else

		return(0);



}



/* prend en entree un tetraedre et donne en sortie une boite

minimale contenant le tetraedre ; cette boite est definie par

un point origine et des dimensions en X,Y,Z */



void boitetra(tetra *tet,int *x0,int *y0,int *z0,int *dx,

int *dy, int *dz)



{

	int x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;

	int xmax,ymax,zmax,xmin,ymin,zmin;



	x1 = tet->x1;

	x2 = tet->x2;

	x3 = tet->x3;

	x4 = tet->x4;

	y1 = tet->y1;

	y2 = tet->y2;

	y3 = tet->y3;

	y4 = tet->y4;

	z1 = tet->z1;

	z2 = tet->z2;

	z3 = tet->z3;

	z4 = tet->z4;



	if(x1>=x2 && x1>=x3 && x1>=x4)

	xmax = x1;

	else

	if(x2>=x1 && x2>=x3 && x2>=x4)

	xmax = x2;

	else

	if(x3>=x1 && x3>=x2 && x3>=x4)

	xmax = x3;

	else

	xmax = x4;

	if(x1<=x2 && x1<=x3 && x1<=x4)

	xmin = x1;

	else

	if(x2<=x1 && x2<=x3 && x2<=x4)

	xmin = x2;

	else

	if(x3<=x1 && x3<=x2 && x3<=x4)

	xmin = x3;

	else

	xmin = x4;



	if(y1>=y2 && y1>=y3 && y1>=y4)

	ymax = y1;

	else

	if(y2>=y1 && y2>=y3 && y2>=y4)

	ymax = y2;

	else

	if(y3>=y1 && y3>=y2 && y3>=y4)

	ymax = y3;

	else

	ymax = y4;

	if(y1<=y2 && y1<=y3 && y1<=y4)

	ymin = y1;

	else

	if(y2<=y1 && y2<=y3 && y2<=y4)

	ymin = y2;

	else

	if(y3<=y1 && y3<=y2 && y3<=y4)

	ymin = y3;

	else

	ymin = y4;



	if(z1>=z2 && z1>=z3 && z1>=z4)

	zmax = z1;

	else

	if(z2>=z1 && z2>=z3 && z2>=z4)

	zmax = z2;

	else

	if(z3>=z1 && z3>=z2 && z3>=z4)

	zmax = z3;

	else

	zmax = z4;

	if(z1<=z2 && z1<=z3 && z1<=z4)

	zmin = z1;

	else

	if(z2<=z1 && z2<=z3 && z2<=z4)

	zmin = z2;

	else

	if(z3<=z1 && z3<=z2 && z3<=z4)

	zmin = z3;

	else

	zmin = z4;



	*x0 = xmin;

	*y0 = ymin;

	*z0 = zmin;

	*dx = xmax - xmin;

	*dy = ymax - ymin;

	*dz = zmax - zmin;

}





/* prend en entree une boule et donne en sortie la boite minimale la

contenant */



void boiteboul(boule *ptboule,int *x0,int *y0,int *z0,int *dx,

int *dy,int *dz)

{

	float xc,yc,zc,r;



	xc = ptboule->xc ;

	yc = ptboule->yc ;

	zc = ptboule->zc ;

	r = ptboule->rayon;



	*x0 = floor(xc - r);

	*y0 = floor(yc - r);

	*z0 = floor(zc - r);

	*dx = ceil(2*r);

	*dy = ceil(2*r);

	*dz = ceil(2*r);

}



/* prend en entree un cylindre et donne en sortie la boite minimale le

contenant */



void boitecyl(cylindre *ptcyl,int *x0,int *y0,int *z0,int *dx,

int *dy,int *dz)

{

	float x1,y1,z1,x2,y2,z2,r,l;



	x1 = ptcyl->x1 ;

	y1 = ptcyl->y1 ;

	z1 = ptcyl->z1 ;

	x2 = ptcyl->x2 ;

	y2 = ptcyl->y2 ;

	z2 = ptcyl->z2 ;

	r = ptcyl->rayon;

	l = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));

	if(x1-r < x2-r)*x0=floor(x1-r); else *x0 = floor(x2-r);

	if(y1-r < y2-r)*y0=floor(y1-r); else *y0 = floor(y2-r);

	if(z1-r < z2-r)*z0=floor(z1-r); else *z0 = floor(z2-r);

	*dx = ceil(l + 2*r);

	*dy = ceil(l + 2*r);

	*dz = ceil(l + 2*r);

}





/* prend en entree un tetraedre et renvoie la sphere circonscrite (passant

par ces 4 points) */



boule *tetrasphere(tetra *tet)

{

	int x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4;

	float xc,yc,zc,r;

	double er1,er2,er3,er4;

	float a1,b1,c1,d1,a2,b2,c2,d2,a3,b3,c3,d3;

	float epsilon;

	int sol;

	boule *boul;



	boul = (boule*)mymalloc(sizeof(boule));

	x1 = tet->x1;

	x2 = tet->x2;

	x3 = tet->x3;

	x4 = tet->x4;

	y1 = tet->y1;

	y2 = tet->y2;

	y3 = tet->y3;

	y4 = tet->y4;

	z1 = tet->z1;

	z2 = tet->z2;

	z3 = tet->z3;

	z4 = tet->z4;



	a1 = -2*x1 + 2*x2 ;

	b1 = -2*y1 + 2*y2 ;

	c1 = -2*z1 + 2*z2 ;

	d1 = x2*x2 + y2*y2 + z2*z2 - x1*x1 - y1*y1 - z1*z1;



	a2 = -2*x2 + 2*x3 ;

	b2 = -2*y2 + 2*y3 ;

	c2 = -2*z2 + 2*z3 ;

	d2 = x3*x3 + y3*y3 + z3*z3 - x2*x2 - y2*y2 - z2*z2 ;



	a3 = -2*x3 + 2*x4 ;

	b3 = -2*y3 + 2*y4 ;

	c3 = -2*z3 + 2*z4 ;

	d3 = x4*x4 + y4*y4 + z4*z4 - x3*x3 - y3*y3 - z3*z3 ;



	resyst3(a1,b1,c1,d1,a2,b2,c2,d2,a3,b3,c3,d3,&sol,&xc,&yc,&zc);





	if(sol != 1)

	{

		fprintf(stderr,"\n sortie dans tetrasphere !");

		fprintf(stderr,"\n %d %d %d %d %d %d %d %d %d %d %d %d",

		x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4);

		fprintf(stderr,"\n %f %f %f %f %f %f %f %f %f %f %f %f",

		a1,b1,c1,d1,a2,b2,c2,d2,a3,b3,c3,d3);

		exit(40);

	}

	r = sqrt((x1-xc)*(x1-xc)+(y1-yc)*(y1-yc)+(z1-zc)*(z1-zc));



	boul->rayon = r;

	boul->xc = xc;

	boul->yc = yc;

	boul->zc = zc;

	epsilon = 0.001;

	/* verification du resultat */

	er1=fabs(((xc-(float)x1)*(xc-(float)x1)+(yc-(float)y1)*(yc-(float)y1)+(zc-(float)z1)*(zc-(float)z1)-(r*r))/(r*r));

	er2=fabs(((xc-(float)x2)*(xc-(float)x2)+(yc-(float)y2)*(yc-(float)y2)+(zc-(float)z2)*(zc-(float)z2)-(r*r))/(r*r));

	er3=fabs(((xc-(float)x3)*(xc-(float)x3)+(yc-(float)y3)*(yc-(float)y3)+(zc-(float)z3)*(zc-(float)z3)-(r*r))/(r*r));

	er4=fabs(((xc-(float)x4)*(xc-(float)x4)+(yc-(float)y4)*(yc-(float)y4)+(zc-(float)z4)*(zc-(float)z4)-(r*r))/(r*r));

	if(er1<epsilon && er2<epsilon && er3<epsilon && er4<epsilon)

	return(boul);

	else

	{

		fprintf(stderr,"\n pb dans 'tetrasphere'");

		fprintf(stderr,"\n %f %f %f %f",xc,yc,zc,r);

		fprintf(stderr,"\n %d %d %d %d %d %d %d %d %d %d %d %d",

		x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4);

		fprintf(stderr,"\n %f %f %f %f",er1,er2,er3,er4);

		exit(30);

	}

	return(0);

}







/* determination de l'equation d'un plan : az + by + cz = d

a partir de trois points non alignes, rend 0 si les points sont alignes */



int eqplan(int x1,int y1,int z1,int x2,int y2,int z2,int x3,

int y3,int z3,float *a,float *b,float *c,float *d)

{

	*a = y2*z3 - y3*z2 - z3*y1 + z2*y1 + y3*z1 - y2*z1 ;

	*b = x1*z3 - x1*z2 - x2*z3 + z2*x3 + x2*z1 - x3*z1 ;

	*c = x1*y2 - x1*y3 - x2*y1 + x3*y1 + x2*y3 - y2*x3 ;

	*d = x1*y2*z3 - x1*z2*y3 - x2*z3*y1 + z2*x3*y1 +

		 x2*z1*y3 - y2*x3*z1 ;

	if(*a == 0 && *b == 0 && *c == 0)return(0);

	else

	return(1);







}





/* renvoie 1 si le point (x,y,z) appartient au plan d'equation

ax + by +cz = d et 0 sinon */



int aplan(float x,float y,float z,float a,float b,float c,

float d)



{

	float epsilon;



	epsilon = 0.0001;

	if(fabs(a*x+b*y+c*z-d) < epsilon)return(1);

	else return(0);

}



void ecrtetra(tetra *tet)

{

	fprintf(stderr,"\n %d %d %d %d %d %d %d %d %d %d %d %d",tet->x1,tet->y1,tet->z1,tet->x2,tet->y2,

	tet->z2,tet->x3,tet->y3,tet->z3,tet->x4,tet->y4,tet->z4);



}



/* insertion d'un tetraedre avec le niveau de gris

color dans une image, retourne le nb de points ecrits

et -1 si le tetraedre est triangle

et donc n'a pas ete insere */

int instetra(image *ima,tetra *tet,int color)

{

	int dimx,dimy,dimz,i,j,k,npoints ;

	int x0,y0,z0,dx,dy,dz,s;





	dimx = ima->dimx ;

	dimy = ima->dimy ;

	dimz = ima->dimz ;

	npoints = 0 ;

	s = eqtetra(tet);

	if(s == 1)

	{

		boitetra(tet,&x0,&y0,&z0,&dx,&dy,&dz);

		for(i = x0 ; i <= x0 + dx ; i++)

		for(j = y0 ; j <= y0 + dy ; j++)

		for(k = z0 ; k <= z0 + dz ; k++)

		{

			if(aptetra(i,j,k,tet) == 1)

			{

				ima->tab[i][j][k] = color;

				npoints++;

			}

		}

		return(npoints);

	}

	else return(-1);

}





/* test de procedures et en particulier celles des tetraedres */



void testetra(void)



{

	int x1,y1,z1,x2,y2,z2,x3,y3,z3,x4,y4,z4 ;

	int iok;

	tetra *ptetra;



	/* test de aptetra */

	fprintf(stderr,"\n coordonnes entieres des 4 sommets du tetraedre : ");

	scanf("%d %d %d %d %d %d %d %d %d %d %d %d",

	&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3,&x4,&y4,&z4);

	ptetra = (tetra*)mymalloc(sizeof(tetra));

	ptetra->x1 = x1;

	ptetra->x2 = x2;

	ptetra->x3 = x3;

	ptetra->x4 = x4;

	ptetra->y1 = y1;

	ptetra->y2 = y2;

	ptetra->y3 = y3;

	ptetra->y4 = y4;

	ptetra->z1 = z1;

	ptetra->z2 = z2;

	ptetra->z3 = z3;

	ptetra->z4 = z4;

	eqtetra(ptetra);

	fprintf(stderr,"\n tester un point (1 si oui 0 sinon) : ");

	scanf("%d",&iok);

	while(iok == 1)

	{

		fprintf(stderr,"\n taper les coordonnees entieres d'un point : ");

		scanf("%d %d %d",&x1,&y1,&z1);

		if(aptetra(x1,y1,z1,ptetra) == 1)

		fprintf(stderr,"\n le point appartient au tetraedre");

		else fprintf(stderr,"\n le point n'appartient pas au tetraedre");



		fprintf(stderr,"\n test de aptetra (1 si oui 0 sinon) :");

		scanf("%d",&iok);

	}



	/* insertion de teraedres dans une image */

}



/* lecture d'un fichier de tetraedres ; on renvoie un pointeur sur un

tableau de tetraedres, le tableau des points ainsi que le nb de tetraedres

et le nombre de points */



void lfictet(char *nomft,float ***points,int ***ltet,int *nbtet,

int *nbs,float *xmax,float *ymax,float *zmax)

{

	FILE *ptfic;

	char chaine[100];

	int nbt,nbp;

	float xm,ym,zm;

	int i,j,l;

	float r;



	ptfic = fopen(nomft,"r");

	fscanf(ptfic,"%s",chaine);

	fprintf(stderr,"%s",chaine);

	while(strcmp(chaine,"Tetrahedra") != 0)

	{

		fscanf(ptfic,"%s",chaine);

		fprintf(stderr,"%s",chaine);

	}

	fscanf(ptfic,"%d",&nbt);

	fprintf(stderr,"\n nb de tetrahedres = %d",nbt);

	*nbtet = nbt;

	(*ltet) = allot2i(4,nbt);

	fprintf(stderr,"\n fin de l'allocation");

	for(i = 0;i < nbt;i++)

	{

		for(j=0;j<4;j++)

		{

			fscanf(ptfic,"%d",&l);

			if(l == EOF)

			{

				fprintf(stderr,"\n pb dans fichier de tetrahedres");

				fprintf(stderr,"\n i = %d ; nbt = %d ; j = %d ",i,nbt,j);

				exit(54);

			}

			(*ltet)[j][i] = l;

		}

		fscanf(ptfic,"%d",&l);

		if(l == EOF)

		{

			fprintf(stderr,"\n pb dans fichier de tetrahedres");

			fprintf(stderr,"\n i = %d ; nbt = %d ",i,nbt);

			exit(55);

		}

	}

	fscanf(ptfic,"%s",chaine);

	while(strcmp(chaine,"Vertices") != 0)fscanf(ptfic,"%s",chaine);

	fscanf(ptfic,"%d",&nbp);

	*nbs = nbp;

	fprintf(stderr,"\n nb de sommets = %d",nbp);

	(*points) = allotab2f(2,nbp-1);

	xm = 0;

	ym = 0;

	zm = 0;

	for(i = 0;i < nbp;i++)

	{

		for(j=0;j<3;j++)

		{

			fscanf(ptfic,"%f",&r);

			(*points)[j][i] = r;

		}

		if(xm < (*points)[0][i])xm=(*points)[0][i];

		if(ym < (*points)[1][i])ym=(*points)[1][i];

		if(zm < (*points)[2][i])zm=(*points)[2][i];

		fscanf(ptfic,"%d",&l);

	}

	*xmax = xm;

	*ymax = ym;

	*zmax = zm;

	fclose(ptfic);

}







/* renvoie la valeur entiere la plus proche d'un float */

int neari(float val)

{



	if((val-floor(val)) <= 0.5)return((int)(floor(val)));

	else return((int)ceil(val));

}



/* renvoie la valeur minimum des 6 floats en entree */

float calmin(float x1,float x2,float x3,float x4,float x5,float x6)

{

	if(x1 <= x2 && x1 <= x3 && x1 <= x4 && x1 <= x5 && x1 <= x6)return(x1);

	else if(x2 <= x1 && x2 <= x3 && x2 <= x4 && x2 <= x5 && x2<=x6)return(x2);

	else if (x3<=x1 && x3 <= x2 && x3 <= x4 && x3<=x5 && x3<=x6)return(x3);

	else if (x4<=x1 && x4 <= x2 && x4 <= x3 && x4<=x5 && x4<=x6)return(x4);

	else if (x5<=x1 && x5 <= x2 && x5 <= x3 && x5<=x4 && x5<=x6)return(x5);

	else return(x6);

}



/* renvoie la valeur minimum des 4 floats en entree */

float calmin4(float x1,float x2,float x3,float x4)

{

	if(x1 <= x2 && x1 <= x3 && x1 <= x4 )return(x1);

	else if(x2 <= x1 && x2 <= x3 && x2 <= x4)return(x2);

	else if (x3<=x1 && x3 <= x2 && x3 <= x4)return(x3);

	else return(x4);

}



/* renvoie la valeur maximale des 4 floats en entree */

float calmax4(float x1,float x2,float x3,float x4)

{



	if(x1 >= x2 && x1 >= x3 && x1 >= x4)return(x1);

	else if(x2 >= x1 && x2 >= x3 && x2 >= x4)return(x2);

	else if (x3>=x1 && x3 >= x2 && x3 >= x4)return(x3);

	else return(x4);



}



/* renvoie la valeur maximale des 6 floats en entree */

float calmax(float x1,float x2,float x3,float x4,float x5,float x6)

{



	if(x1 >= x2 && x1 >= x3 && x1 >= x4 && x1 >= x5 && x1 >= x6)return(x1);

	else if(x2 >= x1 && x2 >= x3 && x2 >= x4 && x2 >= x5 && x2>=x6)return(x2);

	else if (x3>=x1 && x3 >= x2 && x3 >= x4 && x3>=x5 && x3>=x6)return(x3);

	else if (x4>=x1 && x4 >= x2 && x4 >= x3 && x4>=x5 && x4>=x6)return(x4);

	else if (x5>=x1 && x5 >= x2 && x5 >= x3 && x5>=x4 && x5>=x6)return(x5);

	else return(x6);

}

/* routine du 18 juin 2020 */


void testecorval(void)

{

	char nomimage[50],nomfic[50],nomfic2[50],nom[50];

	int nbentete,dimx,dimy,dimz ;

	int **points,nbpts,nmaxecor,npoints,etiq ;

	image *ptimage ;

	fprintf(stderr,"\n entree dans testecorval");

	fprintf(stderr,"\n nom du fichier d'entiers codes sur 4 octets contenant l'image volumique : ");

	scanf("%s",nomimage);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n nombre max de pts de l'ecorce = ");

	scanf("%d",&nmaxecor);

	fprintf(stderr,"\n niveau de gris des points de la forme = ");

	scanf("%d",&etiq);

	fprintf(stderr,"\n nom du fichier pour trianguler : ");

	scanf("%s",nom);

	fprintf(stderr,"\n entree dans ecorceval");

	points = ecorceval(nomimage,etiq,dimx,dimy,dimz,nmaxecor,&nbpts);

	fprintf(stderr,"\n nb de pts de l'ecorce = %d",nbpts);

	fprintf(stderr,"\n creation d'un fichier de points pour delaunay");

	creerfic(nom,points,nbpts);

	fprintf(stderr,"\n sortie de testecorval"); 
}
















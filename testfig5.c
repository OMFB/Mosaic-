/*
 * testfig5.c
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



void testfig5(void)

{

	char nomft[50],nomima[50],nomficbou[50];

	float x0,y0,z0,r ;

	float x1,y1,z1,x2,y2,z2;

	float x4,y4,z4,x3,y3,z3;

	int a1,b1,c1,a2,b2,c2,a3,b3,c3,a4,b4,c4;

	int dimx,dimy,dimz,nbtet,nbs;

	image *ptima;

	boule *ptboule;

	int i,nin,nout;

	float **lpoints;

	int **ltet,xi,yi,zi,dx,dy,dz,ntetras;

	float xm,ym,zm,spla;

	tetra *ptetra;

	int j,k,tentete,entier,ix,iy,iz;

	float pourcentage;

	FILE *ptim,*ptficbou;

	short int *sculpt;

	float a12,a14,a13,a24,a34,a23,rap,amax,amin,s1,s2,s3,s4,smax;

	float m1m2,m1m3,m1m4,m2m3,m2m4,m3m4,xe,ye,ze,u,v,w,norme;

	int ind,nboul,boulforme,pourcent,nretir,npla;



	fprintf(stderr,"\n fichier de tetraedres : ");

	scanf("%s",nomft);

	lfictet(nomft,&lpoints,&ltet,&nbtet,&nbs,&xm,&ym,&zm);

	fprintf(stderr,"\n nb de tetrahedres= %d  nb de sommets=%d xm=%f ym=%f zm=%f",

	nbtet,nbs,xm,ym,zm);

	sculpt = (short int*)mycalloc(nbtet,sizeof(short int));

	fprintf(stderr,"\n nom du fichier image contenant la forme pleine (image d'octets): ");

	scanf("%s",nomima);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n nombre de caracteres de l'entete (690 pour 'originale.dx', 689 pour cylind) = ");

	scanf("%d",&tentete);

	fprintf(stderr,"\n valeur maximale du rapport 'nb de pts du tetra en dehors de la forme'/'nb de pts du tetra (0.2 raisonnable)'");

	scanf("%f",&pourcentage);

	fprintf(stderr,"\n valeur minimale du pourcentage de points de la boule dans la forme (70 conseille) : ");

	scanf("%d",&pourcent);

	fprintf(stderr,"\n taper la valeur maximale du determinant pour le filtrage des tetras plats (minimum 0) : ");

	scanf("%f",&spla);

	fprintf(stderr,"\n fichier de sortie des boules circonscrites :");

	scanf("%s",nomficbou);

	ptficbou = fopen(nomficbou,"w");





	/* ouverture du fichier image */

	ptim = fopen(nomima,"r");



	/* ecriture de l'image contenue dans le fichier .dx : nomim dans l'image

		pointee par ptimage */



	/* lecture de l'entete */



	fprintf(stderr,"\n lecture de l'entete ");

	entier = fgetc(ptim);

	i = 1;

	while(i <= tentete)

	{

		entier = fgetc(ptim);

		i++;

	}

	fprintf(stderr,"\n fin de la lecture de l'entete ");



	/* le pointeur est positionne sur le premier octet de l'image */



	/* lecture des octets de l'image */



	ptima = creimage(dimx,dimy,dimz,1);

	fprintf(stderr,"\n fin de creimage");



	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	for(i=0;i<dimx;i++)



	{

		entier = fgetc(ptim);

		if(entier == EOF)

		{

			fprintf(stderr,"\n 11 ; i = %d, j=%d, k=%d",i,j,k);

			exit(11);

		}

		(ptima->tab)[i][j][k] = entier;

	}

	entier = fgetc(ptim);

	if(entier != EOF)

	{

		fprintf(stderr,"\n 12 ");

		exit(12);

	}

	/* l'image pointee par 'ptima' contient l'image du fichier

	'nomima' */

	fprintf(stderr,"\n fin de la lecture de l'image ");

	fclose(ptim);



	/* sculpture du fichier de tetraedres; on met a 0 dans le tableau

	'sculpt' les indices des tetraedres qui ne sont pas dans la

	forme et a 1 les autres */

	ntetras = 0;

	ind = 1;

	npla = 0;

	ptetra = (tetra*)mymalloc(sizeof(tetra));

	for(i =0;i<nbtet;i++)

	{

		if(ind == 100000)

		{

			fprintf(stderr,"\n i=%d ntetras=%d ; npla = %d",i,ntetras,npla);

			ind = 1;

		}

		else ind++;



		ptetra->x1 = neari(lpoints[0][ltet[0][i]-1]);

		ptetra->x2 = neari(lpoints[0][ltet[1][i]-1]);

		ptetra->x3 = neari(lpoints[0][ltet[2][i]-1]);

		ptetra->x4 = neari(lpoints[0][ltet[3][i]-1]);

		ptetra->y1 = neari(lpoints[1][ltet[0][i]-1]);

		ptetra->y2 = neari(lpoints[1][ltet[1][i]-1]);

		ptetra->y3 = neari(lpoints[1][ltet[2][i]-1]);

		ptetra->y4 = neari(lpoints[1][ltet[3][i]-1]);

		ptetra->z1 = neari(lpoints[2][ltet[0][i]-1]);

		ptetra->z2 = neari(lpoints[2][ltet[1][i]-1]);

		ptetra->z3 = neari(lpoints[2][ltet[2][i]-1]);

		ptetra->z4 = neari(lpoints[2][ltet[3][i]-1]);

		/* on teste si le tetrahedre est plat */

		if(plan(ptetra,spla)==1)

		{

			npla++;

			sculpt[i] = 0;

		}

		else

		{

			entier = eqtetra(ptetra);

			if(entier == 1)

			{

				/* on regarde si le tetrahedre est dans la forme */

				boitetra(ptetra,&xi,&yi,&zi,&dx,&dy,&dz);

				nin = 0;

				nout = 0;

				for(ix = xi;ix<= xi+dx;ix++)

				for(iy = yi;iy<= yi+dy;iy++)

				for(iz = zi;iz<= zi+dz;iz++)

					if(aptetra(ix,iy,iz,ptetra)==1)
					{

							if((ptima->tab)[ix][iy][iz]==0)
							{
								nin++;
							}

							else
							{
									nout++;
							}
					}

					if((nin + nout) == 0)rap = 1;
					else rap = (float)nout/((float)nin + (float)nout);

				if(rap > pourcentage)


				{

					sculpt[i] = 0;

					ntetras++;

				}

				else

				{

					sculpt[i] = 1;

				}

			}



			else

			{

				sculpt[i] = 0;

				ntetras++;

			}

		}

	}

	fprintf(stderr,"\n nb de tetrahedres retires = %d",ntetras);

	fprintf(stderr,"\n nb de tetrahedres plans = %d",npla);

	/* ecriture des tetraedres de la forme dans l'image en sortie */

	nboul = 0;

	nretir = 0;

	for(i =0;i<nbtet;i++)

	{

		if(sculpt[i] == 1)

		{

			ptetra->x1 = neari(lpoints[0][ltet[0][i]-1]);

			ptetra->x2 = neari(lpoints[0][ltet[1][i]-1]);

			ptetra->x3 = neari(lpoints[0][ltet[2][i]-1]);

			ptetra->x4 = neari(lpoints[0][ltet[3][i]-1]);

			ptetra->y1 = neari(lpoints[1][ltet[0][i]-1]);

			ptetra->y2 = neari(lpoints[1][ltet[1][i]-1]);

			ptetra->y3 = neari(lpoints[1][ltet[2][i]-1]);

			ptetra->y4 = neari(lpoints[1][ltet[3][i]-1]);

			ptetra->z1 = neari(lpoints[2][ltet[0][i]-1]);

			ptetra->z2 = neari(lpoints[2][ltet[1][i]-1]);

			ptetra->z3 = neari(lpoints[2][ltet[2][i]-1]);

			ptetra->z4 = neari(lpoints[2][ltet[3][i]-1]);

			/* calcul de la sphere cirsconcrite */

			ptboule = tetrasphere(ptetra);

			x0 = ptboule->xc;

			y0 = ptboule->yc;

			z0 = ptboule->zc;

			r = ptboule->rayon;

			/* on regarde si la boule est incluse dans la forme*/

			boulforme = bouleform(ptboule,ptima,0,pourcent);

			free(ptboule);

			if(boulforme == 0)

			{

				nretir++;

			}

			else

			{

				x1 = ptetra->x1;

				x2 = ptetra->x2;

				x3 = ptetra->x3;

				x4 = ptetra->x4;

				y1 = ptetra->y1;

				y2 = ptetra->y2;

				y3 = ptetra->y3;

				y4 = ptetra->y4;

				z1 = ptetra->z1;

				z2 = ptetra->z2;

				z3 = ptetra->z3;

				z4 = ptetra->z4;

				m1m2 = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2);

				m1m3 = (x1-x3)*(x1-x3)+(y1-y3)*(y1-y3)+(z1-z3)*(z1-z3);

				m1m4 = (x1-x4)*(x1-x4)+(y1-y4)*(y1-y4)+(z1-z4)*(z1-z4);

				m2m3 = (x2-x3)*(x2-x3)+(y2-y3)*(y2-y3)+(z2-z3)*(z2-z3);

				m2m4 = (x2-x4)*(x2-x4)+(y2-y4)*(y2-y4)+(z2-z4)*(z2-z4);

				m3m4 = (x3-x4)*(x3-x4)+(y3-y4)*(y3-y4)+(z3-z4)*(z3-z4);

				s1 = m1m2 + m1m3 + m1m4;

				s2 = m1m2 + m2m3 + m2m4;

				s3 = m2m3 + m3m4 + m1m3;

				s4 = m2m4 + m3m4 + m1m4;

				smax = calmax4(s1,s2,s3,s4);

				if(smax == s1)

				{

					xe = x1;

					ye = y1;

					ze = z1;

				}

				else if(smax == s2)

				{

					xe = x2;

					ye = y2;

					ze = z2;

				}

				else if(smax == s3)

				{

					xe = x3;

					ye = y3;

					ze = z3;

				}

				else if(smax == s4)

				{

					xe = x4;

					ye = y4;

					ze = z4;

				}

					u = xe-x0;

					v = ye-y0;

					w = ze-z0;

					norme = sqrt(u*u + v*v + w*w);

				if(norme == 0)

				{

					fprintf(stderr,"\n norme = 0 !");

					norme = 1;

				}

				u = u/norme;

				v = v/norme;

				w = w/norme;

				/* calcul des longueurs des aretes de plus petite

				et de plus grande longueur */

				a1 = ptetra->x1 ;

				a2 = ptetra->x2 ;

				a3 = ptetra->x3 ;

				a4 = ptetra->x4 ;

				b1 = ptetra->y1 ;

				b2 = ptetra->y2 ;

				b3 = ptetra->y3 ;

				b4 = ptetra->y4 ;

				c1 = ptetra->z1 ;

				c2 = ptetra->z2 ;

				c3 = ptetra->z3 ;

				c4 = ptetra->z4 ;

				a12 = (a1-a2)*(a1-a2)+(b1-b2)*(b1-b2)+(c1-c2)*(c1-c2);

				a13 = (a1-a3)*(a1-a3)+(b1-b3)*(b1-b3)+(c1-c3)*(c1-c3);

				a14 = (a1-a4)*(a1-a4)+(b1-b4)*(b1-b4)+(c1-c4)*(c1-c4);

				a23 = (a3-a2)*(a3-a2)+(b3-b2)*(b3-b2)+(c3-c2)*(c3-c2);

				a24 = (a4-a2)*(a4-a2)+(b4-b2)*(b4-b2)+(c4-c2)*(c4-c2);

				a34 = (a3-a4)*(a3-a4)+(b3-b4)*(b3-b4)+(c3-c4)*(c3-c4);

				a12 = (float)sqrt((double)(a12));

				a13 = (float)sqrt((double)(a13));

				a14 = (float)sqrt((double)(a14));

				a23 = (float)sqrt((double)(a23));

				a24 = (float)sqrt((double)(a24));

				a34 = (float)sqrt((double)(a34));

				amax = calmax(a12,a13,a14,a23,a24,a34);

				amin = calmin(a12,a13,a14,a23,a24,a34);

				rap = amin/amax;

				if(amax == 0)fprintf(stderr,"\n amax = %f amin = %f",amin,amax);

				fprintf(ptficbou,"\n %f %f %f %f %f %f %f %f %f %f",x0,y0,z0,r,amax,amin,rap,u,v,w);

				nboul++;

			}

		}

	}

	fprintf(ptficbou,"\n %d",nboul);

	fprintf(stderr,"\n nb de boules = %d",nboul);

	fprintf(stderr,"\n nb de boules retirees = %d",nretir);

	fclose(ptficbou);

}

/* FONCTION EN COURS D'ECRITURE CORRESPONDANT A TESTFIG5 SIMPLIFIEE */
void testfig8(void)

{

	char nomft[50],nomima[50],nomficbou[50];

	float x0,y0,z0,r ;

	float x1,y1,z1,x2,y2,z2;

	float x4,y4,z4,x3,y3,z3;

	int a1,b1,c1,a2,b2,c2,a3,b3,c3,a4,b4,c4;

	int dimx,dimy,dimz,nbtet,nbs;

	image *ptima;

	boule *ptboule;

	int i,nin,nout;

	float **lpoints;

	int **ltet,xi,yi,zi,dx,dy,dz,ntetras;

	float xm,ym,zm,spla;

	tetra *ptetra;

	int j,k,tentete,entier,ix,iy,iz;

	float pourcentage;

	FILE *ptim,*ptficbou;

	short int *sculpt;

	float a12,a14,a13,a24,a34,a23,rap,amax,amin,s1,s2,s3,s4,smax;

	float m1m2,m1m3,m1m4,m2m3,m2m4,m3m4,xe,ye,ze,u,v,w,norme;

	int ind,nboul,boulforme,pourcent,nretir,npla;



	fprintf(stderr,"\n fichier de tetraedres : ");

	scanf("%s",nomft);

	lfictet(nomft,&lpoints,&ltet,&nbtet,&nbs,&xm,&ym,&zm);

	fprintf(stderr,"\n nb de tetrahedres= %d  nb de sommets=%d xm=%f ym=%f zm=%f",

	nbtet,nbs,xm,ym,zm);

	sculpt = (short int*)mycalloc(nbtet,sizeof(short int));

	fprintf(stderr,"\n nom du fichier image contenant la forme pleine (image d'octets): ");

	scanf("%s",nomima);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n nombre de caracteres de l'entete (690 pour 'originale.dx', 689 pour cylind) = ");

	scanf("%d",&tentete);

	fprintf(stderr,"\n valeur minimale du pourcentage de points de la boule dans la forme (70 conseille) : ");

	scanf("%d",&pourcent);

	fprintf(stderr,"\n taper la valeur maximale du determinant pour le filtrage des tetras plats (minimum 0) : ");

	scanf("%f",&spla);

	fprintf(stderr,"\n fichier de sortie des boules circonscrites :");

	scanf("%s",nomficbou);

	ptficbou = fopen(nomficbou,"w");





	/* ouverture du fichier image */

	ptim = fopen(nomima,"r");



	/* ecriture de l'image contenue dans le fichier .dx : nomim dans l'image

		pointee par ptimage */



	/* lecture de l'entete */



	fprintf(stderr,"\n lecture de l'entete ");

	entier = fgetc(ptim);

	i = 1;

	while(i <= tentete)

	{

		entier = fgetc(ptim);

		i++;

	}

	fprintf(stderr,"\n fin de la lecture de l'entete ");



	/* le pointeur est positionne sur le premier octet de l'image */



	/* lecture des octets de l'image */



	ptima = creimage(dimx,dimy,dimz,1);

	fprintf(stderr,"\n fin de creimage");



	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	for(i=0;i<dimx;i++)



	{

		entier = fgetc(ptim);

		if(entier == EOF)

		{

			fprintf(stderr,"\n 11 ; i = %d, j=%d, k=%d",i,j,k);

			exit(11);

		}

		(ptima->tab)[i][j][k] = entier;

	}

	entier = fgetc(ptim);

	if(entier != EOF)

	{

		fprintf(stderr,"\n 12 ");

		exit(12);

	}

	/* l'image pointee par 'ptima' contient l'image du fichier

	'nomima' */

	fprintf(stderr,"\n fin de la lecture de l'image ");

	fclose(ptim);




	/* ecriture des tetraedres de la forme dans l'image en sortie */

	nboul = 0;

	nretir = 0;

	for(i =0;i<nbtet;i++)

	{

		if(sculpt[i] == 1)

		{

			ptetra->x1 = neari(lpoints[0][ltet[0][i]-1]);

			ptetra->x2 = neari(lpoints[0][ltet[1][i]-1]);

			ptetra->x3 = neari(lpoints[0][ltet[2][i]-1]);

			ptetra->x4 = neari(lpoints[0][ltet[3][i]-1]);

			ptetra->y1 = neari(lpoints[1][ltet[0][i]-1]);

			ptetra->y2 = neari(lpoints[1][ltet[1][i]-1]);

			ptetra->y3 = neari(lpoints[1][ltet[2][i]-1]);

			ptetra->y4 = neari(lpoints[1][ltet[3][i]-1]);

			ptetra->z1 = neari(lpoints[2][ltet[0][i]-1]);

			ptetra->z2 = neari(lpoints[2][ltet[1][i]-1]);

			ptetra->z3 = neari(lpoints[2][ltet[2][i]-1]);

			ptetra->z4 = neari(lpoints[2][ltet[3][i]-1]);

			/* calcul de la sphere cirsconcrite */

			ptboule = tetrasphere(ptetra);

			x0 = ptboule->xc;

			y0 = ptboule->yc;

			z0 = ptboule->zc;

			r = ptboule->rayon;

			/* on regarde si la boule est incluse dans la forme*/

			boulforme = bouleform(ptboule,ptima,0,pourcent);

			free(ptboule);

			if(boulforme == 0)

			{

				nretir++;

			}

			else

			{

				x1 = ptetra->x1;

				x2 = ptetra->x2;

				x3 = ptetra->x3;

				x4 = ptetra->x4;

				y1 = ptetra->y1;

				y2 = ptetra->y2;

				y3 = ptetra->y3;

				y4 = ptetra->y4;

				z1 = ptetra->z1;

				z2 = ptetra->z2;

				z3 = ptetra->z3;

				z4 = ptetra->z4;

				m1m2 = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2);

				m1m3 = (x1-x3)*(x1-x3)+(y1-y3)*(y1-y3)+(z1-z3)*(z1-z3);

				m1m4 = (x1-x4)*(x1-x4)+(y1-y4)*(y1-y4)+(z1-z4)*(z1-z4);

				m2m3 = (x2-x3)*(x2-x3)+(y2-y3)*(y2-y3)+(z2-z3)*(z2-z3);

				m2m4 = (x2-x4)*(x2-x4)+(y2-y4)*(y2-y4)+(z2-z4)*(z2-z4);

				m3m4 = (x3-x4)*(x3-x4)+(y3-y4)*(y3-y4)+(z3-z4)*(z3-z4);

				s1 = m1m2 + m1m3 + m1m4;

				s2 = m1m2 + m2m3 + m2m4;

				s3 = m2m3 + m3m4 + m1m3;

				s4 = m2m4 + m3m4 + m1m4;

				smax = calmax4(s1,s2,s3,s4);

				if(smax == s1)

				{

					xe = x1;

					ye = y1;

					ze = z1;

				}

				else if(smax == s2)

				{

					xe = x2;

					ye = y2;

					ze = z2;

				}

				else if(smax == s3)

				{

					xe = x3;

					ye = y3;

					ze = z3;

				}

				else if(smax == s4)

				{

					xe = x4;

					ye = y4;

					ze = z4;

				}

					u = xe-x0;

					v = ye-y0;

					w = ze-z0;

					norme = sqrt(u*u + v*v + w*w);

				if(norme == 0)

				{

					fprintf(stderr,"\n norme = 0 !");

					norme = 1;

				}

				u = u/norme;

				v = v/norme;

				w = w/norme;

				/* calcul des longueurs des aretes de plus petite

				et de plus grande longueur */

				a1 = ptetra->x1 ;

				a2 = ptetra->x2 ;

				a3 = ptetra->x3 ;

				a4 = ptetra->x4 ;

				b1 = ptetra->y1 ;

				b2 = ptetra->y2 ;

				b3 = ptetra->y3 ;

				b4 = ptetra->y4 ;

				c1 = ptetra->z1 ;

				c2 = ptetra->z2 ;

				c3 = ptetra->z3 ;

				c4 = ptetra->z4 ;

				a12 = (a1-a2)*(a1-a2)+(b1-b2)*(b1-b2)+(c1-c2)*(c1-c2);

				a13 = (a1-a3)*(a1-a3)+(b1-b3)*(b1-b3)+(c1-c3)*(c1-c3);

				a14 = (a1-a4)*(a1-a4)+(b1-b4)*(b1-b4)+(c1-c4)*(c1-c4);

				a23 = (a3-a2)*(a3-a2)+(b3-b2)*(b3-b2)+(c3-c2)*(c3-c2);

				a24 = (a4-a2)*(a4-a2)+(b4-b2)*(b4-b2)+(c4-c2)*(c4-c2);

				a34 = (a3-a4)*(a3-a4)+(b3-b4)*(b3-b4)+(c3-c4)*(c3-c4);

				a12 = (float)sqrt((double)(a12));

				a13 = (float)sqrt((double)(a13));

				a14 = (float)sqrt((double)(a14));

				a23 = (float)sqrt((double)(a23));

				a24 = (float)sqrt((double)(a24));

				a34 = (float)sqrt((double)(a34));

				amax = calmax(a12,a13,a14,a23,a24,a34);

				amin = calmin(a12,a13,a14,a23,a24,a34);

				rap = amin/amax;

				if(amax == 0)fprintf(stderr,"\n amax = %f amin = %f",amin,amax);

				fprintf(ptficbou,"\n %f %f %f %f %f %f %f %f %f %f",x0,y0,z0,r,amax,amin,rap,u,v,w);

				nboul++;

			}

		}

	}

	fprintf(ptficbou,"\n %d",nboul);

	fprintf(stderr,"\n nb de boules = %d",nboul);

	fprintf(stderr,"\n nb de boules retirees = %d",nretir);

	fclose(ptficbou);

}



/* cette procedure prend en entree un fichier de "nboules" boules (

avec l'allongement et la direction sortie de testfig5) et rend le fichier

classe selon les rayons par ordre decroissant et un autre fichier classe selon les allongements

par ordre croissant */



void triboules2(char *ficboul,char *ficboul1,char *ficboul2,int nboules)

{

	boule **taboules;

	FILE *ptboul,*ptboul1,*ptboul2;

	int i,*itas,n,iout;

	float xc,yc,zc,r,amin,amax,rap,*along,u,v,w,*oru,*orv,*orw;

	double *val,valout;



	taboules = (boule**)mycalloc(nboules,sizeof(boule*));

	for(i=0;i<nboules;i++)

	taboules[i] = (boule*)mymalloc(sizeof(boule));

	along = (float*)mycalloc(nboules,sizeof(float));

	oru = (float*)mycalloc(nboules,sizeof(float));

	orv = (float*)mycalloc(nboules,sizeof(float));

	orw = (float*)mycalloc(nboules,sizeof(float));

	itas = (int*)mycalloc(nboules+1,sizeof(int));

	val = (double*)mycalloc(nboules+1,sizeof(double));

	ptboul = fopen(ficboul,"r");

	ptboul1 = fopen(ficboul1,"w");

	ptboul2 = fopen(ficboul2,"w");



	printf("\n fin des allocations");



	/* lecture de ficboul et remplissage de taboules, de along et de oru,orv,orw*/

	for(i=0;i<nboules;i++)

	{

		/* printf("\n i = %d",i); */

		fscanf(ptboul,"%f %f %f %f %f %f %f %f %f %f",&xc,&yc,&zc,&r,&amin,&amax,&rap,&u,&v,&w);

		taboules[i]->xc = xc;

		taboules[i]->yc = yc;

		taboules[i]->zc = zc;

		taboules[i]->rayon = r;

		along[i] = rap;

		oru[i] = u;

		orv[i] = v;

		orw[i] = w;

	}

	printf("\n fin de la lecture de ficboul");



	/* tri des boules selon les rayons et selon les allongements des

	tetraedres correspondants */



	/* tri des boules selon les rayons */



	for(i=1;i<=nboules;i++)

	{

		itas[i] = i-1;

		val[i] = -(taboules[i-1]->rayon);

	}

	printf("\n fin de l'ecriture de itas et de val");

	cretas(val,itas,nboules);

	/* ecriture des boules triees selon les rayons par ordre decroissant */

	printf("\n debut de l'ecriture des boules");

	n = nboules;

	fprintf(ptboul1,"%d",nboules);

	for(i=0;i<nboules;i++)

	{

		outas(val,itas,&n,&iout,&valout);

		fprintf(ptboul1,"\n %f %f %f %f %f %f %f %f",taboules[iout]->xc,taboules[iout]->yc,taboules[iout]->zc,taboules[iout]->rayon,along[iout],oru[iout],orv[iout],orw[iout]);

		if(taboules[iout]->rayon != -valout)

		{

			fprintf(stderr,"\n probleme 1 dans triboules !! %f %lf",taboules[iout]->rayon

			,valout);

			exit(40);

		}

	}



	/* tri des boules selon les allongements */



	for(i=1;i<=nboules;i++)

	{

		itas[i] = i-1;

		val[i] = along[i-1];

	}

	cretas(val,itas,nboules);



	/* ecriture des boules triees selon les allongements par ordre decroissant */

	n = nboules;

	fprintf(ptboul2,"%d",nboules);

	for(i=0;i<nboules;i++)

	{

		outas(val,itas,&n,&iout,&valout);

		fprintf(ptboul2,"\n %f %f %f %f %f %f %f %f",taboules[iout]->xc,taboules[iout]->yc,taboules[iout]->zc,taboules[iout]->rayon,(float)valout,oru[iout],orv[iout],orw[iout]);

		if(along[iout] != valout)

		{

			printf("\n probleme 2 dans triboules !!%f %lf",along[iout],valout);

			exit(40);

		}

	}

	fclose(ptboul);

	fclose(ptboul1);

	fclose(ptboul2);



}


void triboules3(char *ficboul,char *ficboul1,int nboules)

{

	boule **taboules;

	FILE *ptboul,*ptboul1;

	int i,*itas,n,iout;

	float xc,yc,zc,r,amin,amax,rap,u,v,w,zero;

	double *val,valout;


	itas = (int*)mycalloc(nboules+1,sizeof(int));

	val = (double*)mycalloc(nboules+1,sizeof(double));

	fprintf(stderr,"\n fin de l'allocation pour le tas ");

	taboules = (boule**)mycalloc(nboules,sizeof(boule*));

	fprintf(stderr,"\n fin de l'allocation pour le tableau des boules");

	for(i=0;i<nboules;i++)

	taboules[i] = (boule*)mymalloc(sizeof(boule));


	ptboul = fopen(ficboul,"r");

	ptboul1 = fopen(ficboul1,"w");



	fprintf(stderr,"\n fin des allocations");



	/* lecture de ficboul et remplissage de taboules, de along et de oru,orv,orw*/

	for(i=0;i<nboules;i++)

	{


		fscanf(ptboul,"%f %f %f %f %f %f %f %f %f %f",&xc,&yc,&zc,&r,&amin,&amax,&rap,&u,&v,&w);

		taboules[i]->xc = xc;

		taboules[i]->yc = yc;

		taboules[i]->zc = zc;

		taboules[i]->rayon = r;

	}

	fprintf(stderr,"\n fin de la lecture de ficboul");



	/* tri des boules selon les rayons et selon les allongements des

	tetraedres correspondants */



	/* tri des boules selon les rayons */



	for(i=1;i<=nboules;i++)

	{

		itas[i] = i-1;

		val[i] = -(taboules[i-1]->rayon);

	}

	fprintf(stderr,"\n fin de l'ecriture de itas et de val");

	cretas(val,itas,nboules);

	/* ecriture des boules triees selon les rayons par ordre decroissant */

	fprintf(stderr,"\n debut de l'ecriture des boules");

	n = nboules;
	zero = 0.0;

	fprintf(ptboul1,"%d",nboules);

	for(i=0;i<nboules;i++)

	{

		outas(val,itas,&n,&iout,&valout);

		fprintf(ptboul1,"\n %f %f %f %f %f %f %f %f",taboules[iout]->xc,taboules[iout]->yc,taboules[iout]->zc,taboules[iout]->rayon,zero,zero,zero,zero);

		if(taboules[iout]->rayon != -valout)

		{

			fprintf(stderr,"\n probleme 1 dans triboules !! %f %lf",taboules[iout]->rayon

			,valout);

			exit(40);

		}

	}




	fclose(ptboul);

	fclose(ptboul1);

}


void testtri3(void)

{

	char ficboul[50],ficboul1[50];

	int nboules;



	printf("\n nom du fichier de boules en entree (sortie de testfig5) : ");

	scanf("%s",ficboul);

	printf("\n nombre de boules = ");

	scanf("%d",&nboules);

	printf("\n nom du fichier de boules en sortie trie selon les rayons: ");

	scanf("%s",ficboul1);

	triboules3(ficboul,ficboul1,nboules);

}



/* cette procedure prend en entree le fichier des boules tries (sortie de triboules2)

selon les rayons (nb de boules suivi de : coordonnees du centre;

rayon, alllongement, coordonnees du vecteur orientation) et renvoie un

fichier de boules maximales disjointes deux � deux du meme format si 'iopt=1'

avec seulement l'allongement sans l'orientation si 'opt=2'*/



void selecboules2(char *ficboul,char *ficboulm,float seuil,int iopt)

{

	FILE *ptficboul,*ptficboulm;

	int nboules,nbm,nbv,i,j,sel,ind;

	boule **boules,**boulesm;

	float xc,yc,zc,r,a,*along,*alongm,*oru,*orv,*orw,u,v,w,*orum,*orvm,*orwm;



	ptficboul = fopen(ficboul,"r");

	ptficboulm = fopen(ficboulm,"w");

	fscanf(ptficboul,"%d",&nboules);

	fprintf(stderr,"\n nboules = %d",nboules);

	boules = (boule**)mycalloc(nboules,sizeof(boule*));

	boulesm = (boule**)mycalloc(nboules,sizeof(boule*));

	along = (float*)mycalloc(nboules,sizeof(float));

	alongm = (float*)mycalloc(nboules,sizeof(float));

	oru = (float*)mycalloc(nboules,sizeof(float));

	orv = (float*)mycalloc(nboules,sizeof(float));

	orw = (float*)mycalloc(nboules,sizeof(float));

	orum = (float*)mycalloc(nboules,sizeof(float));

	orvm = (float*)mycalloc(nboules,sizeof(float));

	orwm = (float*)mycalloc(nboules,sizeof(float));

	nbm = 0;

	nbv = 0;

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

		boules[i]->rayon = r;

		along[i] = a;

		oru[i] = u;

		orv[i] = v;

		orw[i] = w;

	}

	fprintf(stderr,"\n fin du rangement des boules");

	ind = 1;

	/* selection des boules maximales */

	for(i=0;i<nboules;i++)

	{

		/* on regarde si la boule a une intersection vide avec

		les boules deja selectionnees et celles rejetees */

		if(ind >= 100000)

		{

			fprintf(stderr,"\n i = %d",i);
			fprintf(stderr,"\n nbm = %d",nbm);

			ind = 1;

		}

		else

		ind++;

		j = 0;

		sel=1;

		while(j<nbm && sel==1)

		{

			if(connex(boules[i],boulesm[j],seuil)==1)sel=0;

			else j++;

		}

		if(sel==1)

		{

			boulesm[nbm] = boules[i];

			alongm[nbm] = along[i];

			orum[nbm] = oru[i];

			orvm[nbm] = orv[i];

			orwm[nbm] = orw[i];

			nbm++;

			/* fprintf(stderr," %d;",nbm); */

		}

	}

	printf("\n nb de boules maximales = %d",nbm);



	/* rangement des boules selectionnees dans le fichier en sortie */

	fprintf(ptficboulm,"%d",nbm);

	for(i=0;i<nbm;i++)

	if(iopt==1)

	fprintf(ptficboulm,"\n %f %f %f %f %f %f %f %f",boulesm[i]->xc,

	boulesm[i]->yc,boulesm[i]->zc,boulesm[i]->rayon,alongm[i],orum[i],orvm[i],orwm[i]);

	else if(iopt==2)

	fprintf(ptficboulm,"\n %f %f %f %f",boulesm[i]->xc,

	boulesm[i]->yc,boulesm[i]->zc,boulesm[i]->rayon);

	else

	{

		fprintf(stderr,"erreur dans selectboules2 !!");

		exit(32);

	}

	fclose(ptficboulm);

	fclose(ptficboul);



}


void triboul5(void)

{

	char nom1[50],nom2[50];

	float seuil;

	int iopt,dimx,dimy,dimz;



	printf("nom du fichier de boules trie selon les rayons (sortie de testtri2): ");

	scanf("%s",nom1);

	printf("\n nom du fichier de boules maximales en sortie : ");

	scanf("%s",nom2);

	printf("\n %s %s",nom1,nom2);

	printf("\n dimx, dimy, dimz :");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	printf("\n pourcentage de points des boules dans les autres boules =");

	scanf("%f",&seuil);

	fprintf(stderr,"\n taper 1 pour fichier de sortie (x0,y0,z0,r,a,u,v,w) et 2 pour (x0,y0,z0,r) : ");

	scanf("%d",&iopt);

	selecboules5(nom1,nom2,seuil,iopt,dimx,dimy,dimz);

}

void triboul4(void)

{

	char nom1[50],nom2[50];

	float seuil;

	int iopt;



	printf("nom du fichier de boules trie selon les rayons (sortie de testtri2): ");

	scanf("%s",nom1);

	printf("\n nom du fichier de boules maximales en sortie : ");

	scanf("%s",nom2);

	printf("\n %s %s",nom1,nom2);

	printf("\n seuil pour la connexite (d(C1,C2)/(r1+r2)>=seui pour ttes les paires de boules ; 0.7 recommand�) : ");

	scanf("%f",&seuil);

	seuil = seuil*seuil;

	fprintf(stderr,"\n taper 1 pour fichier de sortie (x0,y0,z0,r,a,u,v,w) et 2 pour (x0,y0,z0,r) : ");

	scanf("%d",&iopt);

	selecboules4(nom1,nom2,seuil,iopt);


}



/* selection des boules maximales avec prise en compte du squelette */

void triboul3(void)

{

	char nom1[50],nom2[50];

	float seuil;

	int iopt;



	printf("nom du fichier de boules trie selon les rayons (sortie de testtri2): ");

	scanf("%s",nom1);

	printf("\n nom du fichier de boules maximales en sortie : ");

	scanf("%s",nom2);

	printf("\n %s %s",nom1,nom2);

	printf("\n seuil pour la connexite (d(C1,C2)/(r1+r2)>=seuil pour ttes les paires de boules) : ");

	seuil = seuil*seuil;

	scanf("%f",&seuil);

	fprintf(stderr,"\n taper 1 pour fichier de sortie (x0,y0,z0,r,a,u,v,w) et 2 pour (x0,y0,z0,r) : ");

	scanf("%d",&iopt);

	selecboules3(nom1,nom2,seuil,iopt);



}



/* prend en entree un fichier de boules (xc,yc,zc,r,a,u,v,w) et filtre

selon la valeur de a allongement du tetraedre correspondant,

seuil est le seuil de filtrage pour l'allongement et seuil2 pour le rayon, le resultat est un fichier de

boules filtrees */



void filtral2(char *ficboul,char *ficfiltre,float seuil,float seuil2)

{

	FILE *ptfic1,*ptfic2;

	int nboules,i,nboulsec;

	float xc,yc,zc,r,a,u,v,w;



	ptfic1 = fopen(ficboul,"r");

	ptfic2 = fopen(ficfiltre,"w");

	fscanf(ptfic1,"%d",&nboules);

	nboulsec = 0;

	for(i=0;i<nboules;i++)

	{

		fscanf(ptfic1,"%f %f %f %f %f %f %f %f",&xc,&yc,&zc,&r,&a,&u,&v,&w);

		if(a < seuil && r >= seuil2)nboulsec++;

	}

	fprintf(ptfic2,"%d"	,nboulsec);

	fclose(ptfic1);

	ptfic1 = fopen(ficboul,"r");

	fscanf(ptfic1,"%d",&nboules);

	for(i=0;i<nboules;i++)

	{

		fscanf(ptfic1,"%f %f %f %f %f %f %f %f",&xc,&yc,&zc,&r,&a,&u,&v,&w);

		if(a < seuil && r >= seuil2)fprintf(ptfic2,"\n %f %f %f %f %f %f %f %f",xc,yc,zc,r,a,u,v,w);

	}

	printf("\n nb de boules du fichier de sortie = %d ",nboulsec);

	fclose(ptfic2);

	fclose(ptfic2);

}



void filtre(void)

{

	char fic1[50],fic2[50];

	float seuil,seuil2;



	printf("\n fichier de boules maximales en entree (x0,y0,z0,r,a,u,v,w): ");

	scanf("%s",fic1);

	printf("\n fichier de boules en sortie : ");

	scanf("%s",fic2);

	printf("\n valeur du seuil pour l'allongement '0 <= arete min/arete max <= seuil' raisonnable entre 0.2 et 0.3 -conseillee 0.3-) =");

	scanf("%f",&seuil);

	printf("\n valeur minimale pour le rayon = ");

	scanf("%f",&seuil2);

	filtral2(fic1,fic2,seuil,seuil2);

}



/* cree une image de dimensions 'dimx dimy dimz'

avec le fond a 1 et les boules du fichier "ficboul" (sortie de selecbules2)

a 0 ; l'entete de l'image est le fichier 'entete'*/



void visuboul2(char *boules,char *imaboul,char *entete,int dimx,int dimy,int dimz)

{



	FILE *ptboules;

	int nboules,i,npoints;

	float x0,y0,z0,r,a,u,v,w;

	image *ptimage;

	boule *ptboule;



	ptboules = fopen(boules,"r");

	ptimage = creimage(dimx,dimy,dimz,1);

	ptboule = (boule*)mymalloc(sizeof(boule));



	fscanf(ptboules,"%d",&nboules);

	fprintf(stderr,"\n nboules = %d",nboules);



	for(i=0;i<nboules;i++)

	{

		fscanf(ptboules,"%f %f %f %f %f %f %f %f",&x0,&y0,&z0,&r,&a,&u,&v,&w);

		ptboule->xc = x0;

		ptboule->yc = y0;

		ptboule->zc = z0;

		ptboule->rayon = r;

		npoints = boulimag(ptboule,ptimage,0);

	}

	npoints = ecrimage(entete,imaboul,ptimage);

	fprintf(stderr,"\n nb total de points ecrits = %d",npoints);

}



/* cree une image de dimensions 'dimx dimy dimz'

avec le fond a 1 et les vecteurs orientation du fichier "ficboul" (sortie de selecbules2)

a 0 ; l'entete de l'image est le fichier 'entete'

un vecteur orientation est visualis� par un segment passant par le centre de

la boule de longueur le diam�tre de la boule de direction la direction du t�tra�dre associ�

et tel que le centre de la boule soit le milieu du segment */



void visorient(char *boules,char *imaboul,char *entete,int dimx,int dimy,int dimz,float ep)

{



	FILE *ptboules;

	int nboules,i,npoints;

	float x0,y0,z0,r,a,u,v,w,x1,y1,z1,x2,y2,z2;

	image *ptimage;

	boule *ptboule;

	cylindre *ptcyl;



	ptboules = fopen(boules,"r");

	ptimage = creimage(dimx,dimy,dimz,1);

	ptboule = (boule*)mymalloc(sizeof(boule));

	ptcyl = (cylindre*)mymalloc(sizeof(cylindre));

	fscanf(ptboules,"%d",&nboules);



	for(i=0;i<nboules;i++)

	{

		fscanf(ptboules,"%f %f %f %f %f %f %f %f",&x0,&y0,&z0,&r,&a,&u,&v,&w);

		ptboule->xc = x0;

		ptboule->yc = y0;

		ptboule->zc = z0;

		ptboule->rayon = r;

		x1 = x0 + r*u;

		y1 = y0 + r*v;

		z1 = z0 + r*w;

		x2 = x0 - r*u;

		y2 = y0 - r*v;

		z2 = z0 - r*w;

		ptcyl->x1 = x1;

		ptcyl->y1 = y1;

		ptcyl->z1 = z1;

		ptcyl->x2 = x2;

		ptcyl->y2 = y2;

		ptcyl->z2 = z2;

		ptcyl->rayon = ep;

		npoints = inscylind(ptimage,ptcyl,0);

	}

	npoints = ecrimage(entete,imaboul,ptimage);

}



void testvisorient(void)

{



	char boules[50],imaboul[50],entete[50];

	int dimx,dimy,dimz;

	float ep;



	printf("nom du fichier des boules (nb de boules,xc,yc,zc,r,a,u,v,w) : ");

	scanf("%s",boules);

	printf("\n nom du fichier entete de l'image de sortie en format opendx (image d'octets): ");

	scanf("%s",entete);

	printf("\n %s",entete);

	printf("\n nom du fichier image de sortie en format opendx (image d'octets): ");

	scanf("%s",imaboul);

	printf("\n %s",imaboul);

	printf("\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	printf("\n epaisseur du trait = ");

	scanf("%f",&ep);

	visorient(boules,imaboul,entete,dimx,dimy,dimz,ep);

}



void testvisboul2(void)

{



	char boules[50],imaboul[50],entete[50];

	int dimx,dimy,dimz;



	printf("nom du fichier des boules (nb de boules,xc,yc,zc,r,a,u,v,w) : ");

	scanf("%s",boules);

	printf("\n nom du fichier entete de l'image de sortie en format opendx (image d'octets): ");

	scanf("%s",entete);

	printf("\n %s",entete);

	printf("\n nom du fichier image de sortie en format opendx (image d'octets): ");

	scanf("%s",imaboul);

	printf("\n %s",imaboul);

	printf("\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	visuboul2(boules,imaboul,entete,dimx,dimy,dimz);

}



/* prend en entree une boule et une image ou les points d'une forme sont etiquetees a la

valeur seuil ; rend en sortie 1 si la boule est completement incluse dans la forme

(pourcentage - 0-100- est le pourcentage de points de la boule qui doivent

etre dans la forme) et zero sinon */

int bouleform(boule *ptboule,image *ptimage,int seuil,int pourcentage)

{

	float d,r2,nbpoints,x0,y0,z0,r,coeff ;

	int dimx,dimy,dimz,dx,dy,dz,xc,yc,zc ;

	int i,j,k,a,b,c,nf,nd;



	dimx = ptimage->dimx;

	dimy = ptimage->dimy;

	dimz = ptimage->dimz;

	x0 = ptboule->xc ;

	y0 = ptboule->yc ;

	z0 = ptboule->zc ;

	r = ptboule->rayon;

	if(x0+r<dimx && x0-r>=0 && y0+r<dimy && y0-r>=0 && z0+r<dimz && z0-r>=0)

	{

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

		nf = 0; /* nb de points de la boule dans la forme */

		nd = 0; /* nb de points de la boule en dehors de la forme */

		for(i=xc;i <= a;i++)

		for(j=yc;j <= b;j++)

		for(k=zc;k <= c;k++)

		{

			d = (x0-i)*(x0-i)+(y0-j)*(y0-j)+(z0-k)*(z0-k) ;

			if(d <= r2)

			{

				if(ptimage->tab[i][j][k] != seuil)nd++;

				else nf++;

			}

		}

		if(nd + nf == 0)

		{

			fprintf(stderr,"\n Bizarre !!");

			return(0);

		}

		else

		{

			coeff = (float)(100*nf)/(float)(nd+nf);

			if( coeff >= (float)(pourcentage))

			{

				return(1);

			}

			else return(0);

		}

	}

	else

	{

		return(0);

	}

}



/* cette proc�dure teste si un tetrahedre est plan.Elle renvoie 1 si il

est plan et zero sinon */



int plan(tetra *ptetra,float seuil)

{

	float det;

	float x1,y1,z1,x2,y2,z2;

	float x4,y4,z4,x3,y3,z3;



	x1 = (float)ptetra->x1;

	x2 = (float)ptetra->x2;

	x3 =(float) ptetra->x3;

	x4 = (float)ptetra->x4;

	y1 = (float)ptetra->y1;

	y2 = (float)ptetra->y2;

	y3 = (float)ptetra->y3;

	y4 = (float)ptetra->y4;

	z1 = (float)ptetra->z1;

	z2 = (float)ptetra->z2;

	z3 = (float)ptetra->z3;

	z4 = (float)ptetra->z4;



	/* les sommets du tetrahedre sont (x1,y1,z1), (x2,y2,z2), (x3,y3,z3),

	(x4,y4,z4) */

	det = det3((float)(x2-x1),(float)(y2-y1),(float)(z2-z1),

	(float)(x3-x1),(float)(y3-y1),(float)(z3-z1),

	(float)(x4-x1),(float)(y4-y1),(float)(z4-z1));

	if(det < 0)det = -det;

	if(det <= seuil)

	{

		return(1);

	}

	else return(0);

}



/* cette procedure prend en entree le fichier des boules tries (sortie de triboules2)

selon les rayons (nb de boules suivi de : coordonnees du centre;

rayon, alllongement, coordonnees du vecteur orientation) et renvoie un

fichier de boules maximales disjointes deux � deux du meme format si 'iopt=1'

avec seulement l'allongement sans l'orientation si 'opt=2'

PAR RAPPORT A SELECTBOULES2 ON PREND EN CONSIDERATION LE

RECOUVREMENT DU SQUELETTE */



void selecboules3(char *ficboul,char *ficboulm,float seuil,int iopt)

{

	FILE *ptficboul,*ptficboulm;

	int nboules,nbm,nbv,i,j,sel,ind,dimx,dimy,dimz,nsquel;

	int nsqbm,l,k;

	float xmax,ymax,zmax;

	boule **boules,**boulesm;

	image *ptsquel;

	float xc,yc,zc,r,a,*along,*alongm,*oru,*orv,*orw,u,v,w,*orum,*orvm,*orwm;



	ptficboul = fopen(ficboul,"r");

	ptficboulm = fopen(ficboulm,"w");

	fscanf(ptficboul,"%d",&nboules);

	fprintf(stderr,"\n nboules = %d",nboules);

	boules = (boule**)mycalloc(nboules,sizeof(boule*));

	boulesm = (boule**)mycalloc(nboules,sizeof(boule*));

	along = (float*)mycalloc(nboules,sizeof(float));

	alongm = (float*)mycalloc(nboules,sizeof(float));

	oru = (float*)mycalloc(nboules,sizeof(float));

	orv = (float*)mycalloc(nboules,sizeof(float));

	orw = (float*)mycalloc(nboules,sizeof(float));

	orum = (float*)mycalloc(nboules,sizeof(float));

	orvm = (float*)mycalloc(nboules,sizeof(float));

	orwm = (float*)mycalloc(nboules,sizeof(float));

	nbm = 0;

	nbv = 0;

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

	ptsquel = creimage(dimx,dimy,dimz,1);

	/* ptsquel pointe sur l'image du squelette */

	/* on met a 0 les points du squelette dans l'image du squelette */

	nsquel = 0;

	for(i=0;i<nboules;i++)

	{

		xc = boules[i]->xc;

		yc = boules[i]->yc;

		zc = boules[i]->zc;

		ptsquel->tab[neari(xc)][neari(yc)][neari(zc)] = 0;

	}

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

		if(ptsquel->tab[i][j][k] == 0)nsquel++;



	fprintf(stderr,"\n nb de boules = %d",nboules);

	fprintf(stderr,"\n nb de points du squelette = %d",nsquel);



	ind = 1;

	/* selection des boules maximales */

	for(i=0;i<nboules;i++)

	{

		/* on regarde si la boule a une intersection vide avec

		les boules deja selectionnees */

		if(ind >= 100000)

		{

			fprintf(stderr,"\n i = %d;",i);
			fprintf(stderr,"\n nbm = %d;",nbm);

			ind = 1;

		}

		else

		ind++;

		j = 0;

		sel=1;

		while(j<nbm && sel==1)

		{

			if(connex(boules[i],boulesm[j],seuil)==1)sel=0;

			else j++;

		}

		if(sel==1)

		{

			boulesm[nbm] = boules[i];

			alongm[nbm] = along[i];

			orum[nbm] = oru[i];

			orvm[nbm] = orv[i];

			orwm[nbm] = orw[i];

			nbm++;

		}

	}

	printf("\n nb de boules maximales = %d",nbm);



	/* calcul du recouvrement du squelette par les boules maximales */

	nsqbm = 0;

	/* nsqbm est le nb de points du squelette dans les boules maximales */

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	if(ptsquel->tab[i][j][k] == 0)

	{

		for(l = 0;l<nbm;l++)

		{

			xc = boulesm[l]->xc;

			yc = boulesm[l]->yc;

			zc = boulesm[l]->zc;

			r = boulesm[l]->rayon;

			if((xc-i)*(xc-i)+(yc-j)*(yc-j)+(zc-k)*(zc-k) <= r*r)

			{

				nsqbm++;

				break;

			}

		}

	}

	fprintf(stderr,"\n nb de points du squelette dans les boules maximales : %d",nsqbm);

	/* rangement des boules selectionnees dans le fichier en sortie */

	fprintf(ptficboulm,"%d",nbm);

	for(i=0;i<nbm;i++)

	if(iopt==1)

	fprintf(ptficboulm,"\n %f %f %f %f %f %f %f %f",boulesm[i]->xc,

	boulesm[i]->yc,boulesm[i]->zc,boulesm[i]->rayon,alongm[i],orum[i],orvm[i],orwm[i]);

	else if(iopt==2)

	fprintf(ptficboulm,"\n %f %f %f %f",boulesm[i]->xc,

	boulesm[i]->yc,boulesm[i]->zc,boulesm[i]->rayon);

	else

	{

		fprintf(stderr,"erreur dans selectboules2 !!");

		exit(32);

	}

	fclose(ptficboulm);

	fclose(ptficboul);



}

void triboul2(void)

{

	char nom1[50],nom2[50];

	float seuil;

	int iopt;



	printf("nom du fichier de boules trie selon les rayons (sortie de testtri2): ");

	scanf("%s",nom1);

	printf("\n nom du fichier de boules maximales en sortie : ");

	scanf("%s",nom2);

	printf("\n %s %s",nom1,nom2);

	printf("\n seuil pour la connexite (d(C1,C2)/(r1+r2)>=seuil pour ttes les paires de boules) : ");

	seuil = seuil*seuil;

	scanf("%f",&seuil);

	fprintf(stderr,"\n taper 1 pour fichier de sortie (x0,y0,z0,r,a,u,v,w) et 2 pour (x0,y0,z0,r) : ");

	scanf("%d",&iopt);

	selecboules2(nom1,nom2,seuil,iopt);



}



/*
 * testfig6.c
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

/* cette procedure prend en entree un fichier de boules et rend un fichier avec les commandes matlab pour tracer l'histogramme
 de repartition des rayons des boules, "boules" est le fichier des boules (x,y,z,r,0,0,0,0), "commandes" est le fichier matlab, dimax est la
 taille maximale d'un pore
 */

void histoboules(char *boules,char *commandes,int rmax)

{

	int i,nbboules;

	int *tabou,dimax;

	float x,y,z,r,a,b,c,d,s;

	double volboules,*vtabou,v;

	FILE *ptboules,*ptcommandes;



	ptboules = fopen(boules,"r");

	ptcommandes = fopen(commandes,"w");

	dimax = 2*rmax ;

	vtabou = (double*)mymalloc((dimax+1)*sizeof(double));

	for(i=0;i<dimax+1;i++)vtabou[i] = 0.0;

	tabou = (int*)mymalloc((dimax+1)*sizeof(int));

	for(i=0;i<dimax+1;i++)tabou[i]=0;

	volboules = 0.0;

	fscanf(ptboules,"%d",&nbboules);

	/* lecture du fichier des boules */
	for(i=0;i<nbboules;i++)
	{
		fscanf(ptboules,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);
		v= (4.0/3.0)*3.14159*ceil(r)*ceil(r)*ceil(r);
		tabou[(int)(ceil(2*r))] += 1;
		vtabou[(int)(ceil(2*r))] += v;
		volboules += (double)(v);
	}
	/* tabou contient pour chaque diametre entier le nombre de pores de ce diametre, vtabou contient pour chaque diametre
	 la somme des volumes des boules associes */
	s = 0;
	for(i=0;i<dimax+1;i++)
	{

		vtabou[i] = (vtabou[i]/volboules);
		s += vtabou[i];
	}

	fprintf(stderr,"\n somme des fractions = %f",s);

	/* ecriture du fichier matlab pour la courbe associant à chaque rayon le rapport entre la somme des volumes des boules
	 avec ce rayon et la somme totale du volume des boules exprime en pourcentage
	 */
	fprintf(ptcommandes,"X = [");
	for(i = 0;i<dimax+1;i++)fprintf(ptcommandes,"%f ",(float)(vtabou[i]));
	fprintf(ptcommandes,"];");

	fprintf(ptcommandes,"\n");

	fprintf(ptcommandes,"Y=[");
	for(i = 0;i<dimax+1;i++)fprintf(ptcommandes,"%f ",(float)(i));
	fprintf(ptcommandes,"];");

	fprintf(ptcommandes,"\n");

	fprintf(ptcommandes,"plot(Y,X,'b');");

	fclose(ptcommandes);
	fclose(ptboules);

}

void testhistoboules(void)
{
	char boules[50], commandes[50];

	int dimax;


	fprintf(stderr,"\n fichier de boules : ");

	scanf("%s",boules);

	fprintf(stderr,"\n nom du fichier matlab pour la courbe ");

	scanf("%s",commandes);

	fprintf(stderr,"\n valeur maximale du rayon des boules = ");
	scanf("%d",&dimax);

	histoboules(boules,commandes,dimax);
}
void testfig6(void)

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

	int **ltet,xi,yi,zi,dx,dy,dz,ntetras,**ltetscu;

	float xm,ym,zm;

	tetra *ptetra;

	int j,k,tentete,entier,ix,iy,iz;

	float pourcentage;

	FILE *ptim,*ptficbou;

	short int *sculpt;

	float a12,a14,a13,a24,a34,a23,rap,amax,amin,s1,s2,s3,s4,smax;

	float m1m2,m1m3,m1m4,m2m3,m2m4,m3m4,xe,ye,ze,u,v,w,norme;

	int ind,nboul,boulforme,pourcent,nretir,ntscu,fts;

	char tetrascu[50];





	fprintf(stderr,"\n fichier de tetraedres : ");

	scanf("%s",nomft);

	fprintf(stderr,"\n nom du fichier image contenant la forme pleine (image d'octets): ");

	scanf("%s",nomima);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n nombre de caracteres de l'entete (690 pour 'originale.dx', 689 pour cylind) = ");

	scanf("%d",&tentete);

	fprintf(stderr,"\n valeur maximale du rapport 'nb de pts du tetra en dehors de la forme'/'nb de pts du tetra dans la forme (0.1 raisonnable)'");

	scanf("%f",&pourcentage);

	fprintf(stderr,"\n valeur minimale du pourcentage de points de la boule dans la forme (80 conseille) : ");

	scanf("%d",&pourcent);

	fprintf(stderr,"\n fichier de sortie des boules circonscrites :");

	scanf("%s",nomficbou);

	ptficbou = fopen(nomficbou,"w");

	fprintf(stderr,"\n fichier de sortie des tetraedres apres sculpture : ");

	scanf("%s",tetrascu);

	fprintf(stderr,"\n pour filtre des tetras en fonctions des spheres taper 1 sinon 0 :");

	scanf("%d",&fts);

	lfictet(nomft,&lpoints,&ltet,&nbtet,&nbs,&xm,&ym,&zm);

	fprintf(stderr,"\n nb de tetrahedres= %d  nb de sommets=%d xm=%f ym=%f zm=%f",

	nbtet,nbs,xm,ym,zm);

	sculpt = (short int*)mycalloc(nbtet,sizeof(short int));


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

	ptetra = (tetra*)mymalloc(sizeof(tetra));

	for(i =0;i<nbtet;i++)

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

			if((ptima->tab)[ix][iy][iz]==0)nin++;

			else 
			{
				nout++;
			}

			if((float)nout/(float)nin > pourcentage)

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

	fprintf(stderr,"\n nb de tetrahedres retires = %d",ntetras);
	fprintf(stderr,"\n");

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

			if(boulforme == 0)

			{

				nretir++;

				if(fts==1)sculpt[i] = 0;

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



	/* ecriture des tetraedres restant apres la sculpture */

	fprintf(stderr,"\n ecriture du fichier des tetraedres sculptes");

	ntscu = nbtet - ntetras;

	fprintf(stderr,"\n ntscu = %d",ntscu);

	ltetscu = allot2i(4,ntscu);

	ind = 1;

	k = 0;

	for(i = 0; i<nbtet ; i++)

	{

		if(sculpt[i] == 1)

		{

			for(j = 0;j <4;j++)

			{

				if(k >= ntscu)exit(432);

				ltetscu[j][k] = ltet[j][i];

			}

			k++;

		}

	}

	fprintf(stderr,"\n nb de tetradedres sculptes = %d",k);

	ntscu = k;

	efictet(tetrascu,lpoints,ltetscu,ntscu,nbs);



	fprintf(ptficbou,"\n %d",nboul);

	fprintf(stderr,"\n nb de boules = %d",nboul);

	fprintf(stderr,"\n nb de boules retirees = %d",nretir);

	fclose(ptficbou);

}



/* ecriture d'un fichier de t�tra�dres

entrees : nom du fichier ou ecrire, tableau des points, tableau des tertraedres

, nb de tetraedres, nb de sommets */



void efictet(char *nomft,float **points,int **ltet,int nbtet,int nbs)

{

	FILE *ptfic;

	int i,j,l;



	ptfic = fopen(nomft,"w");

	fprintf(ptfic,"MeshVersionFormatted 1");

	fprintf(ptfic,"\n Dimension");

	fprintf(ptfic,"\n 3");

	fprintf(ptfic,"\n Tetrahedra");

	fprintf(ptfic,"\n %d",nbtet);



	for(i = 0;i < nbtet;i++)

	{

		fprintf(ptfic,"\n");

		for(j=0;j<4;j++)

		{

			fprintf(ptfic,"%d ",ltet[j][i]);

		}

		fprintf(ptfic," %d",0);

	}



	fprintf(ptfic,"\n Vertices");

	fprintf(ptfic,"\n %d",nbs);

	for(i = 0;i < nbs;i++)

	{

		fprintf(ptfic,"\n");

		for(j=0;j<3;j++)

		{

			fprintf(ptfic,"%f ",points[j][i]);

		}

		fprintf(ptfic," %d",0);

	}

	fclose(ptfic);

}



void testboule(void)

{

	char nomft[50],nomima[50],nomficbou[50];

	float x0,y0,z0,r ;

	float x1,y1,z1,x2,y2,z2;

	float x4,y4,z4,x3,y3,z3;

	int a1,b1,c1,a2,b2,c2,a3,b3,c3,a4,b4,c4;

	int dimx,dimy,dimz,nbtet,nbs;

	image *ptima;

	boule *ptboule;

	int i,nin,nout,iok;

	float **lpoints;

	int **ltet,xi,yi,zi,dx,dy,dz,ntetras,**ltetscu;

	float xm,ym,zm;

	tetra *ptetra;

	int j,k,tentete,entier,ix,iy,iz;

	float pourcentage;

	FILE *ptim,*ptficbou;

	short int *sculpt;

	float a12,a14,a13,a24,a34,a23,rap,amax,amin,s1,s2,s3,s4,smax;

	float m1m2,m1m3,m1m4,m2m3,m2m4,m3m4,xe,ye,ze,u,v,w,norme,r2,a;

	int ind,nboul,boulforme,pourcent,nretir,ntscu,fts,nptboul,nptboulf;

	char tetrascu[50];







	fprintf(stderr,"\n nom du fichier image contenant la forme pleine (image d'octets): ");

	scanf("%s",nomima);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n nombre de caracteres de l'entete (690 pour 'originale.dx', 689 pour cylind) = ");

	scanf("%d",&tentete);

	printf("nom du fichier de boules (x0,y0,z0,r,a,u,v,w): ");

	scanf("%s",nomficbou);

	ptficbou = fopen(nomficbou,"r");

	fscanf(ptficbou,"%d",&nboul);

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



	/* comptage des points a 0 de l'image qui sont dans la boule */





	for(ind=0;ind<nboul;ind++)

	{

		fscanf(ptficbou,"%f %f %f %f %f %f %f %f",&x0,&y0,&z0,&r,&a,&u,&v,&w);

		r2 = r*r ;

		nptboul = 0 ;

		nptboulf = 0 ;

		for(k=0;k<dimz;k++)

		for(j=0;j<dimy;j++)

		for(i=0;i<dimx;i++)



		{

			if(((x0-i)*(x0-i) + (y0-j)*(y0-j) + (z0-k)*(z0-k)) <= r2)

			{

				nptboul++;

				if(ptima->tab[i][j][k] == 0)nptboulf++;

			}

		}

		pourcentage = ((float)(nptboulf)/(float)(nptboul))*100;

		if(pourcentage < 100 || (nptboul - nptboulf) != 0)

		{

			fprintf(stderr,"\n x0 = %f ; y0 = %f ; z0 = %f ; r = %f",x0,y0,z0,r);

			fprintf(stderr,"\n nb total de pts de la boule dans la grille = %d",nptboul);

			fprintf(stderr,"\n nb total de pts de la boule dans la forme = %d",nptboulf);

			fprintf(stderr,"\n pourcentage des points de la boule dans la forme = %f",pourcentage);

		}

	}



}



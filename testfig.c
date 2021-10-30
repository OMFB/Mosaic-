/*
 * testfig.c
 *
 *  Created on: 5 juin 2009
 *      Author: monga
 */

#include <stdio.h>

#include <errno.h>

#include <stdio.h>

#include <stdlib.h>
/* NEW */
#include <unistd.h>  /* unlink */

#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include "geometrie.h"





void testfig(void)

{

	char nomfic[50],nomfic2[50],nomft[50],nomima[50],nomficbou[50],fsquel[50];

	float x0,y0,z0,r,vol ;

	float x1,y1,z1,x2,y2,z2,r1,r2;

	float x4,y4,z4,x3,y3,z3;

	int a1,b1,c1,a2,b2,c2,a3,b3,c3,a4,b4,c4;

	int dimx,dimy,dimz,zero,un,npoints,npts,nbboules,nbtet,nbs;

	image *ptimage,*ptima;

	boule *ptboule,**ptboules;

	cylindre *ptcyl;

	cone *ptcone;

	int iok,i,nb,nin,nout;

	float **lpoints,x,y,z;

	int **ltet,nbtriang,nbiz,xi,yi,zi,dx,dy,dz,ntetras,nsquel,nfsquel;

	float xm,ym,zm;

	float *rayons,epsilon;

	double d;

	tetra *ptetra;

	int j,k,npboul,ndelaunay,nsq,iopt,tentete,entier,ix,iy,iz;

	float moyenne,dmax,d1,d2,d3,d4,dmin,pourcentage;

	FILE *ptim,*ptficbou,*ptfsquel;

	short int *sculpt;

	float a12,a14,a13,a24,a34,a23,rap,amax,amin;



	zero = 0 ;

	epsilon = 0.0001;

	un = 1 ;

	printf("nom du fichier entete en format opendx (image d'octets): ");

	scanf("%s",nomfic);

	printf("\n %s",nomfic);

	printf("\n nom du fichier image de sortie en format opendx (image d'octets): ");

	scanf("%s",nomfic2);

	printf("\n %s",nomfic2);

	printf("\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	/* creation d'une image 3d d'entiers de dimensions dimx.dimy.dimz initialisee a un */

	ptimage = creimage(dimx,dimy,dimz,un);

	ptboule = (boule*)mymalloc(sizeof(boule));

	ptcyl = (cylindre*)mymalloc(sizeof(cylindre));

	ptcone = (cone*)mymalloc(sizeof(cone));

	ptetra = (tetra*)mymalloc(sizeof(tetra));





	printf("\n voulez vous inserer des figures dans l'image (1:boule,2:cylindre,3:cone,4:tetraedre,5 : fichier de tetraedres,6: suite de boules, 7:fichier de tetras sculpte 0:rien)") ;

	scanf("%d",&iok);

	while(iok != 0)

	{

		if(iok == 1)

		{

			printf("\n coordonnees du centre de la boule ; rayon de la boule : ");

			scanf("%f %f %f %f",&x0,&y0,&z0,&r);

			vol = (4*3.14116*r*r*r)/3;

			printf("\n volume de la boule = %f",vol);

			ptboule->xc = x0;

			ptboule->yc = y0;

			ptboule->zc = z0;

			ptboule->rayon = r;

			npoints = boulimag(ptboule,ptimage,zero);

			printf("\n nombre de points de la boule ecrits dans l'image = %d ",npoints);

		}else

		if(iok == 2)

		{



			printf("\n coordonnes des points centres extremites \n du cylindre [x1,y1,z1,x2,y2,z2] et rayon :");

			scanf("%f %f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2,&r);

			ptcyl->x1 = x1;

			ptcyl->y1 = y1;

			ptcyl->z1 = z1;

			ptcyl->x2 = x2;

			ptcyl->y2 = y2;

			ptcyl->z2 = z2;

			ptcyl->rayon = r;

			printf("\n entree dans inscylind");

			npoints = inscylind(ptimage,ptcyl,zero);

			printf("\n sortie de inscylind");

			printf("\n nb de points du cylindre ecrits dans l'image = %d",npoints);

			vol = (3.14116*r*r)*sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));

			printf("\n volume du cylindre = %f",vol);

		}else

		if(iok == 3)

		{

			printf("\n coordonnes des points centres extremites \n [x1,y1,z1,x2,y2,z2] et rayons :");

			scanf("%f %f %f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2,&r1,&r2);

			ptcone->x1 = x1;

			ptcone->y1 = y1;

			ptcone->z1 = z1;

			ptcone->x2 = x2;

			ptcone->y2 = y2;

			ptcone->z2 = z2;

			ptcone->rayon1 = r1;

			ptcone->rayon2 = r2;

			npoints = inscone(ptimage,ptcone,zero);

			vol = (3.14116/3)*sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2))

			*(r1*r1 + r2*r2 + r1*r2);

			printf("\n nb de points du cone ecrits dans l'image = %d",npoints);

			printf("\n volume du cone = %f",vol);

		}else

		if(iok == 4)

		{

			printf("\n coordonnes entieres des 4 sommets du tetraedre : ");

			scanf("%f %f %f %f %f %f %f %f %f %f %f %f",

			&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3,&x4,&y4,&z4);

			ptetra->x1 = (int)x1;

			ptetra->x2 = (int)x2;

			ptetra->x3 = (int)x3;

			ptetra->x4 = (int)x4;

			ptetra->y1 = (int)y1;

			ptetra->y2 = (int)y2;

			ptetra->y3 = (int)y3;

			ptetra->y4 = (int)y4;

			ptetra->z1 = (int)z1;

			ptetra->z2 = (int)z2;

			ptetra->z3 = (int)z3;

			ptetra->z4 = (int)z4;



			ptboule = tetrasphere(ptetra);

			printf("\n %f %f %f %f",ptboule->xc,ptboule->yc,

			ptboule->zc,ptboule->rayon);



		}else

		if(iok == 5)

		{

			printf("\n fichier de tetraedres : ");

			scanf("%s",nomft);

			lfictet(nomft,&lpoints,&ltet,&nbtet,&nbs,&xm,&ym,&zm);

			printf("\n nb de tetrahedres= %d  nb de sommets=%d xm=%f ym=%f zm=%f",

			nbtet,nbs,xm,ym,zm);

			/* ecriture des tetrahedres dans l'image */

			nbtriang = 0;

			ndelaunay = 0;

			nbiz = 0;

			nsq = 0;

			moyenne = 0;



			dmax = 0;

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

				npoints =  instetra(ptimage,ptetra,zero);

				if(npoints == -1)nbtriang++;

				else

				{

					/* on verifie que le tetrahedre est bien

					un tetrahedre de Delaunay */





					ptboule = tetrasphere(ptetra);

					x0 = ptboule->xc ;

					y0 = ptboule->yc ;

					z0 = ptboule->zc ;

					r = ptboule->rayon;



					if(x0 >= 0 && x0 < 100 && y0 >= 0 && y0 < 100

					&& z0 >= 0 && z0 < 100)nsq++;

					/* calcul du nombre de sommets inclus dans la boule */

					npboul = 0;

					dmin = 0;

					for(j=0;j<nbs;j++)

					{

						x = (float)neari(lpoints[0][j]);

						y = (float)neari(lpoints[1][j]);

						z = (float)neari(lpoints[2][j]);

						d = fabs(((x0-x)*(x0-x)+(y0-y)*(y0-y)+(z0-z)*(z0-z)-(r*r))/(r*r));



						if(d < epsilon)

						{

							/* cas ou le sommet est dans la boule de Delaunay */

							npboul++;

							d1=(ptetra->x1-x)*(ptetra->x1-x)+(ptetra->y1-y)*(ptetra->y1-y)

							+(ptetra->z1-z)*(ptetra->z1-z);

							dmin = d1;

							d2=(ptetra->x2-x)*(ptetra->x2-x)+(ptetra->y2-y)*(ptetra->y2-y)

							+(ptetra->z2-z)*(ptetra->z2-z);

							if(d2 < dmin)dmin=d2;

							d3=(ptetra->x3-x)*(ptetra->x3-x)+(ptetra->y3-y)*(ptetra->y3-y)

							+(ptetra->z3-z)*(ptetra->z3-z);

							if(d3 < dmin)dmin=d3;

							d4=(ptetra->x4-x)*(ptetra->x4-x)+(ptetra->y4-y)*(ptetra->y4-y)

							+(ptetra->z4-z)*(ptetra->z4-z);

							if(d4 < dmin)dmin=d4;

						}



					}

					dmin = sqrt(dmin);

					if(npboul == 4)ndelaunay++;

					else if(npboul <4)

					{

						nbiz++;

						printf("\n i = %d npboul = %d",i,npboul);

					}

					moyenne = moyenne + npboul;

					dmax = dmax + dmin;

				}



			}

			moyenne = moyenne/(nbtet - nbtriang);

			dmax = dmax/(nbtet - nbtriang);

			printf("\n moyenne de la distance des points des spheres avec les sommets=%e",dmax);

			printf("\n nb de triangles et tetrahedres plans = %d",nbtriang);

			printf("\n nb de tetrahedres de Delaunay parfaits = %d",ndelaunay);

			printf("\n nb de tetrahedres de Delaunay bizarres = %d",nbiz);

			printf("\n nb de centres dans l'image = %d",nsq);

			printf("\n nb moyen de sommets dans les spheres de Delaunay = %f",moyenne);

		}

		else if(iok == 6)



		{

			printf("\n  creation d'une suite de boules dont les centres sont sur un segment") ;

			scanf("%d",&iok);

			printf("\n taper dans l'ordre : coordonnees des extremites;nb de boules : ");

			scanf("%f %f %f %f %f %f %d",&x1,&y1,&z1,&x2,&y2,&z2,&nbboules);

			rayons = (float*)mycalloc(nbboules,sizeof(float));

			printf("\n taper la suite des rayons des boules : ");

			for(i=0;i<nbboules;i++)

			{

				printf("\n rayon de la boule %d = ",i);

				scanf("%f",&r);

				rayons[i] = r;

			}

			ptboules = generboules(x1,y1,z1,x2,y2,z2,rayons,nbboules,&nb);

			printf("\n insertion des boules dans l'image");

			for(i=0;i<nb;i++)

			npoints = boulimag(ptboules[i],ptimage,zero);

		}

		else if(iok == 7)

		{

			printf("\n fichier de tetraedres : ");

			scanf("%s",nomft);

			lfictet(nomft,&lpoints,&ltet,&nbtet,&nbs,&xm,&ym,&zm);

			printf("\n nb de tetrahedres= %d  nb de sommets=%d xm=%f ym=%f zm=%f",

			nbtet,nbs,xm,ym,zm);

			sculpt = (short int*)mycalloc(nbtet,sizeof(short int));

			printf("\n nom du fichier image contenant la forme pleine (image d'octets): ");

			scanf("%s",nomima);

			printf("\n %s",nomima);

			printf("\n nombre de caracteres de l'entete (690 pour 'originale.dx', 689 pour cylind) = ");

			scanf("%d",&tentete);



			printf("\n valeur mimimale du rapport 'nb de pts du tetra en dehors de la forme'/'nb de pts du tetra dans la forme (0.1 raisonnable)'");

			scanf("%f",&pourcentage);



			printf("\n pour image du squelette en sortie taper 1 ; pour image des tetraedres taper 2 pour image du squelette avec les rayons et fichier du squelette taper 3 : ");

			scanf("%d",&iopt);



			/* ouverture du fichier image */

			ptim = fopen(nomima,"r");



			/* ecriture de l'image contenue dans le fichier .dx : nomim dans l'image

			pointee par ptimage */



			/* lecture de l'entete */



			printf("\n lecture de l'entete ");

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

				printf("\n 12 ");

				exit(12);

			}

			/* l'image pointee par 'ptima' contient l'image du fichier

			'nomima' */

			printf("\n fin de la lecture de l'image ");

			fclose(ptim);



			/* sculpture du fichier de tetraedres; on met a 0 dans le tableau

			'sculpt' les indices des tetraedres qui ne sont pas dans la

			forme et a 1 les autres */

			ntetras = 0;

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

					else nout++;

					if((float)nout/(float)nin > pourcentage)

						{

							sculpt[i] = 0;

							ntetras++;

						}

					else

						{

							sculpt[i] = 1;

						}

				}else

				{

					sculpt[i] = 0;

					ntetras++;

				}

			}

			printf("\n nb de tetrahedres retires = %d",ntetras);

			/* ecriture des tetraedres de la forme dans l'image en sortie */

			nsquel = 0;

			nfsquel = 0;

			if(iopt == 1 || iopt == 3)

			{

				printf("\n fichier de sortie des boules circonscrites :");

				scanf("%s",nomficbou);

				ptficbou = fopen(nomficbou,"w");

				if(iopt==3)

				for(i=0;i<dimx;i++)

				for(j=0;j<dimy;j++)

				for(k=0;k<dimz;k++)

				ptimage->tab[i][j][k] = 0;

				if(iopt == 3)

				{

					printf("\n fichier de sortie du squelette : ");

					scanf("%s",fsquel);

					ptfsquel = fopen(fsquel,"w");

				}

			}

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

					if(iopt==2)

					{

						npoints =  instetra(ptimage,ptetra,zero);

						if(npoints == -1)printf("\n anomalie !!!");

					}

					else if(iopt ==1 || iopt==3)

					{

						/* calcul de la sphere cirsconcrite */

						ptboule = tetrasphere(ptetra);

						x0 = ptboule->xc;

						y0 = ptboule->yc;

						z0 = ptboule->zc;

						r = ptboule->rayon;

						/* on regarde si le centre de la boule est inclus dans la forme*/

						if(neari(x0)>=0 && neari(x0)<dimx)

						if(neari(y0)>=0 && neari(y0)<dimy)

						if(neari(z0)>=0 && neari(z0)<dimz)

						if(ptima->tab[neari(x0)][neari(y0)][neari(z0)]==0)

						{

							if(iopt==1)ptimage->tab[neari(x0)][neari(y0)][neari(z0)] = 0;

							else

							{

								if(ptimage->tab[neari(x0)][neari(y0)][neari(z0)] == 0)

								{

									fprintf(ptfsquel,"\n %f %f %f %f",x0,y0,z0,r);

									nfsquel++;

								}

								ptimage->tab[neari(x0)][neari(y0)][neari(z0)] = neari(r);

							}

							nsquel++;

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

							if(amax == 0)printf("\n amax = %f amin = %f",amin,amax);

							fprintf(ptficbou,"\n %f %f %f %f %f %f %f",x0,y0,z0,r,amax,amin,rap);

						}



					}

				}

			}

			if(iopt==1 || iopt == 3)

			{

				printf("\n nb de points du squelette = %d",nsquel);

				fprintf(ptficbou,"\n %d",nsquel);

				fclose(ptficbou);

				if(iopt == 3)

				{

					printf("\n nb de points dans le fichier du squelette = %d",nfsquel);

					fprintf(ptfsquel,"\n %d",nfsquel);

					fclose(ptfsquel);

				}

			}

		}

		printf("\n voulez vous inserer des figures (boules,cones,cylindres) dans l'image (1:boule,2:cylindre,3:cone,4:tetraedre, 5:fichier de tetrahedres,6 : suite de boules, 7 : fichier de tetras sculpte, 0:rien :");

		scanf("%d",&iok);

	}

	/* ecriture de l'image resultat */

	npts = ecrimage(nomfic,nomfic2,ptimage);

}



/* cette procedure prend en entree un fichier de "nboules" boules et rend le fichier

classe selon les rayons par ordre decroissant et un autre fichier classe selon les allongements

par ordre croissant */



void triboules(char *ficboul,char *ficboul1,char *ficboul2,int nboules)

{

	boule **taboules;

	FILE *ptboul,*ptboul1,*ptboul2;

	int i,*itas,n,iout;

	float xc,yc,zc,r,amin,amax,rap,*along;

	double *val,valout;



	taboules = (boule**)mycalloc(nboules,sizeof(boule*));

	for(i=0;i<nboules;i++)

	taboules[i] = (boule*)mymalloc(sizeof(boule));

	along = (float*)mycalloc(nboules,sizeof(float));

	itas = (int*)mycalloc(nboules+1,sizeof(int));

	val = (double*)mycalloc(nboules+1,sizeof(double));

	ptboul = fopen(ficboul,"r");

	ptboul1 = fopen(ficboul1,"w");

	ptboul2 = fopen(ficboul2,"w");



	printf("\n fin des allocations");



	/* lecture de ficboul et remplissage de taboules et de along */

	for(i=0;i<nboules;i++)

	{

		/* printf("\n i = %d",i); */

		fscanf(ptboul,"%f %f %f %f %f %f %f",&xc,&yc,&zc,&r,&amin,&amax,&rap);

		taboules[i]->xc = xc;

		taboules[i]->yc = yc;

		taboules[i]->zc = zc;

		taboules[i]->rayon = r;

		along[i] = rap;

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

	/*	printf("\n i = %d",i); */

		outas(val,itas,&n,&iout,&valout);

		fprintf(ptboul1,"\n %f %f %f %f %f",taboules[iout]->xc,

		taboules[iout]->yc,taboules[iout]->zc,taboules[iout]->rayon,along[iout]);

		if(taboules[iout]->rayon != -valout)

		{

			printf("\n probleme 1 dans triboules !! %f %lf",taboules[iout]->rayon

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

		fprintf(ptboul2,"\n %f %f %f %f %f",taboules[iout]->xc,

		taboules[iout]->yc,taboules[iout]->zc,taboules[iout]->rayon,(float)valout);

		if(along[iout] != valout)

		{

			printf("\n probleme 2 dans triboules !!%f %lf",along[iout],valout);

			exit(40);

		}

	}

	fclose(ptboul);



}



void testtri(void)

{

	char ficboul[50],ficboul1[50],ficboul2[50];

	int nboules;



	printf("\n nom du fichier de boules en entree : ");

	scanf("%s",ficboul);

	printf("\n nombre de boules = ");

	scanf("%d",&nboules);

	printf("\n nom du fichier de boules en sortie trie selon les rayons: ");

	scanf("%s",ficboul1);

	printf("\n nom du fichier de boules en sortie trie selon les allongements : ");

	scanf("%s",ficboul2);

	triboules(ficboul,ficboul1,ficboul2,nboules);

}



/* cette procedure prend en entree le fichier des boules tries

selon les rayons (nb de boules suivi de : coordonnees du centre;

rayon et alllongement) et renvoie un fichier de boules maximales

disjointes deux � deux */



void selecboules(char *ficboul,char *ficboulm,float seuil)

{

	FILE *ptficboul,*ptficboulm;

	int nboules,nbm,nbv,i,j,sel;

	boule **boules,**boulesm,**boulesv;

	float xc,yc,zc,r,a,*along,*alongm;



	ptficboul = fopen(ficboul,"r");

	ptficboulm = fopen(ficboulm,"w");

	fscanf(ptficboul,"%d",&nboules);

	printf("\n nboules = %d",nboules);

	boules = (boule**)mycalloc(nboules,sizeof(boule*));

	boulesm = (boule**)mycalloc(nboules,sizeof(boule*));

	boulesv = (boule**)mycalloc(nboules,sizeof(boule*));

	along = (float*)mycalloc(nboules,sizeof(float));

	alongm = (float*)mycalloc(nboules,sizeof(float));

	nbm = 0;

	nbv = 0;

	for(i=0;i<nboules;i++)

	{

		boules[i] = (boule*)mymalloc(sizeof(boule));

	}

	/* on range dans 'boules' les boules du fichier 'ficboul' dans l'ordre */

	for(i=0;i<nboules;i++)

	{

		fscanf(ptficboul,"%f %f %f %f %f",&xc,&yc,&zc,&r,&a);

		boules[i]->xc = xc;

		boules[i]->yc = yc;

		boules[i]->zc = zc;

		boules[i]->rayon = r;

		along[i] = a;

	}



	/* selection des boules maximales */

	for(i=0;i<nboules;i++)

	{

		/* on regarde si la boule a une intersection vide avec

		les boules deja selectionnees et celles rejetees */

		j = 0;

		sel=1;

		while(j<nbm && sel==1)

		{

			if(connex(boules[i],boulesm[j],seuil)==1)sel=0;

			else j++;

		}

		/* if(sel==1)

		{

			j=0;

			while(j<nbv && sel==1)

			{

				if(connex(boules[i],boulesv[j],seuil)==1)sel=0;

				else j++;

			}

		}*/

		if(sel==1)

		{

			boulesm[nbm] = boules[i];

			alongm[nbm] = along[i];

			nbm++;

		}else

		{

			boulesv[nbv] = boules[i];

			nbv++;

		}



	}

	printf("\n nb de boules maximales = %d",nbm);



	/* rangement des boules selectionnees dans le fichier en sortie */

	fprintf(ptficboulm,"%d",nbm);

	for(i=0;i<nbm;i++)

	fprintf(ptficboulm,"\n %f %f %f %f %f",boulesm[i]->xc,

	boulesm[i]->yc,boulesm[i]->zc,boulesm[i]->rayon,alongm[i]);



	fclose(ptficboulm);

	fclose(ptficboul);



}





/* retourne 1 si les boules ont une intersection non vide ou sont incluses l'une dans l'autre

et 0 sinon. */



int connex(boule *boule1,boule *boule2,float seuil)

{

	float d2,d,r1,r2,r;



	d = (boule1->xc-boule2->xc)*(boule1->xc-boule2->xc)+

	(boule1->yc-boule2->yc)*(boule1->yc-boule2->yc)+

	(boule1->zc-boule2->zc)*(boule1->zc-boule2->zc);



	r1 = boule1->rayon;

	r2 = boule2->rayon;

	/* r = (r1-r2)*(r1-r2);  */



	/* on retourne 1 si les boules sont incluses l'une dans  l'autre */



	/* if ((r1<=r2 && d <= r) || (r1>=r2 && d <= r)) return(1); */



	if(d <= r1*r1 || d <= r2*r2)return(1);

	d2 = d/((r1 + r2)*(r1 + r2));



	if(d2 < seuil)return(1);

	else return(0);



}



void triboul(void)

{

	char nom1[50],nom2[50];

	float seuil;



	printf("nom du fichier de boules trie selon les rayons : ");

	scanf("%s",nom1);

	printf("\n nom du fichier de boules maximales en sortie : ");

	scanf("%s",nom2);

	printf("\n %s %s",nom1,nom2);

	printf("\n seuil pour la connexite (entre 0.5 et 1 -conseillee 0.5-) : ");

	scanf("%f",&seuil);



	selecboules(nom1,nom2,seuil);



}



/* cree une image de dimensions 'dimx dimy dimz'

avec le fond a 1 et les boules du fichier "ficboul" a 0

l'entete de l'image est le fichier 'entete'*/



void visuboul(char *boules,char *imaboul,char *entete,int dimx,int dimy,int dimz)

{



	FILE *ptboules;

	int nboules,i,npoints;

	float x0,y0,z0,r,a;

	image *ptimage;

	boule *ptboule;



	ptboules = fopen(boules,"r");

	ptimage = creimage(dimx,dimy,dimz,1);

	ptboule = (boule*)mymalloc(sizeof(boule));



	fscanf(ptboules,"%d",&nboules);



	for(i=0;i<nboules;i++)

	{

		fscanf(ptboules,"%f %f %f %f %f",&x0,&y0,&z0,&r,&a);

		ptboule->xc = x0;

		ptboule->yc = y0;

		ptboule->zc = z0;

		ptboule->rayon = r;

		npoints = boulimag(ptboule,ptimage,0);

	}

	npoints = ecrimage(entete,imaboul,ptimage);

}



/* cree une image de dimensions 'dimx dimy dimz'

avec le fond a 1 et les cylindres du fichier "cyl" a 0

l'entete de l'image est le fichier 'entete'*/



void visucyl(char *cyl,char *imacyl,char *entete,int dimx,int dimy,int dimz)

{



	FILE *ptcylind;

	int i,nbc,npoints;

	float x1,y1,z1,x2,y2,z2,r,nboul,er1,er2;

	image *ptimage;

	cylindre *ptcyl;

	boule *ptboule;



	ptcylind = fopen(cyl,"r");

	ptimage = creimage(dimx,dimy,dimz,1);

	ptcyl = (cylindre*)mymalloc(sizeof(cylindre));

	ptboule = (boule*)mymalloc(sizeof(boule));



	fscanf(ptcylind,"%d",&nbc);



	for(i=0;i<nbc;i++)

	{

		fscanf(ptcylind,"%f %f %f %f %f %f %f %f %f %f",

		&x1,&y1,&z1,&x2,&y2,&z2,&r,&nboul,&er1,&er2);

		if(er1 >= 0)

		{

			ptcyl->x1 = x1;

			ptcyl->y1 = y1;

			ptcyl->z1 = z1;

			ptcyl->x2 = x2;

			ptcyl->y2 = y2;

			ptcyl->z2 = z2;

			ptcyl->rayon = r;

			npoints = inscylind(ptimage,ptcyl,0);

		}else

		{

			ptboule->xc = x1;

			ptboule->yc = y1;

			ptboule->zc = z1;

			ptboule->rayon = r;

			npoints = boulimag(ptboule,ptimage,0);

		}

	}

	npoints = ecrimage(entete,imacyl,ptimage);

}



void testvisboul(void)

{



	char boules[50],imaboul[50],entete[50];

	int dimx,dimy,dimz;



	printf("nom du fichier des boules (nb de boules,xc,yc,zc,r,a) : ");

	scanf("%s",boules);

	printf("\n nom du fichier entete de l'image de sortie en format opendx (image d'octets): ");

	scanf("%s",entete);

	printf("\n %s",entete);

	printf("\n nom du fichier image de sortie en format opendx (image d'octets): ");

	scanf("%s",imaboul);

	printf("\n %s",imaboul);

	printf("\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	visuboul(boules,imaboul,entete,dimx,dimy,dimz);

}



void testviscyl(void)

{



	char cyl[50],imacyl[50],entete[50];

	int dimx,dimy,dimz;



	printf("nom du fichier des cylindres (nb de cylindres,xc,yc,zc,r,a) : ");

	scanf("%s",cyl);

	printf("\n nom du fichier entete de l'image de sortie en format opendx (image d'octets): ");

	scanf("%s",entete);

	printf("\n %s",entete);

	printf("\n nom du fichier image de sortie en format opendx (image d'octets): ");

	scanf("%s",imacyl);

	printf("\n %s",imacyl);

	printf("\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	visucyl(cyl,imacyl,entete,dimx,dimy,dimz);

}

/* prend en entree un fichier de boules (xc,yc,zc,r,a) et filtre

selon la valeur de a allongement du tetraedre correspondant,

seuil est le seuil de filtrage , le resultat est un fichier de

boules filtrees */



void filtral(char *ficboul,char *ficfiltre,float seuil)

{

	FILE *ptfic1,*ptfic2;

	int nboules,i,nboulsec;

	float xc,yc,zc,r,a;



	ptfic1 = fopen(ficboul,"r");

	ptfic2 = fopen(ficfiltre,"w");

	fscanf(ptfic1,"%d",&nboules);

	nboulsec = 0;

	for(i=0;i<nboules;i++)

	{

		fscanf(ptfic1,"%f %f %f %f %f",&xc,&yc,&zc,&r,&a);

		if(a < seuil)nboulsec++;

	}

	fprintf(ptfic2,"%d"	,nboulsec);

	fclose(ptfic1);

	ptfic1 = fopen(ficboul,"r");

	fscanf(ptfic1,"%d",&nboules);

	for(i=0;i<nboules;i++)

	{

		fscanf(ptfic1,"%f %f %f %f %f",&xc,&yc,&zc,&r,&a);

		if(a < seuil)fprintf(ptfic2,"\n %f %f %f %f %f",xc,yc,zc,r,a);

	}

	printf("\n nb de boules du fichier de sortie = %d ",nboulsec);

	fclose(ptfic2);

	fclose(ptfic2);

}



void filtallong(void)

{

	char fic1[50],fic2[50];

	float seuil;



	printf("\n fichier de boules maximales en entree : ");

	scanf("%s",fic1);

	printf("\n fichier de boules en sortie : ");

	scanf("%s",fic2);

	printf("\n valeur maximale pour l'allongement (raisonnable entre 0.2 et 0.3 -conseillee 0.3-) =");

	scanf("%f",&seuil);

	filtral(fic1,fic2,seuil);

}



/* prend en entree le nom d'une image opendx 'ima' ses dimensions

et la taille de son entete, et ecrit dans l'image de nom 'imas'

avec l'entete de nom 'entete' l'image originale ou tous les

pixels entre 'b1' et 'b2' ont ete mis a la valeur 'val' */



void seuilim(char *ima,int dimx,int dimy,int dimz,int tentete,int b1,int b2,

			int val,char *imas,char *entete)

{

	int i,j,k,npts;

	image *im,*ptimas;



	im = lecima(ima,dimx,dimy,dimz,tentete);

	ptimas = creimage(dimx,dimy,dimz,0);



	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	if(im->tab[i][j][k] >= b1 && im->tab[i][j][k] <= b2)

	ptimas->tab[i][j][k] = val;

	else ptimas->tab[i][j][k] = im->tab[i][j][k];



	npts = ecrimage(entete,imas,ptimas);



}



void testseuil(void)

{

	char ima[50],entete[50],imas[50];

	int dimx,dimy,dimz,tentete,b1,b2,val;



	printf("\n image,dimx,dimy,dimz,taille de l'entete (689 ou 690 -originale.dx-),borne 1,borne 2, valeur, image de sortie, image de l'entete : ");

	scanf("%s %d %d %d %d %d %d %d %s %s",ima,&dimx,&dimy,&dimz,&tentete,&b1,&b2,&val,imas,entete);



	seuilim(ima,dimx,dimy,dimz,tentete,b1,b2,val,imas,entete);



}



/* prend en entree une image et rend en sortie une image seuillee ou

les points selectionnes ont ete mis a 0 et les autres a 1 */

void testseuil2(void)

{

	char ima[50],entete[50],imas[50];

	int dimx,dimy,dimz,tentete,seuil;

	int i,j,k,npts;

	image *im,*ptimas;



	printf("\n nom de l'image a seuiller : ");

	scanf("%s",ima);

	printf("\n dimx, dimy, dimz :");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	printf("\n nom de l'image d'entete : ");

	scanf("%s",entete);

	printf("\n nb de caracteres de l'entete (689 ou 690 -originale.dx-) : ");

	scanf("%d",&tentete);

	printf("\n nom de l'image resultat : ");

	scanf("%s",imas);

	printf("\n seuil = ");

	scanf("%d",&seuil);



	im = lecima(ima,dimx,dimy,dimz,tentete);

	ptimas = creimage(dimx,dimy,dimz,0);



	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	if(im->tab[i][j][k] == 0)ptimas->tab[i][j][k] = 1;

	else if(im->tab[i][j][k] > seuil) ptimas->tab[i][j][k] = 0;

	else ptimas->tab[i][j][k] = 1;



	npts = ecrimage(entete,imas,ptimas);



}



/* cette procedure compte le nombre de caracteres d'un entete d'image */

int taillent(char *entete)

{

	FILE *ptim;

	int i,entier;



	ptim = fopen(entete,"r");

	entier = fgetc(ptim);

	i = 0;

	while(entier != EOF)

	{

				i++;

				entier = fgetc(ptim);

	}

	i--;

	return(i);

}



void testtaillent(void)

{

	char nom[100];

	int tentete;



	fprintf(stderr,"nom du fichier entete : ");

	scanf("%s",nom);

	tentete = taillent(nom);

	fprintf(stderr,"\n taille du fichier = %d",tentete);

}





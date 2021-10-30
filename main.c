/*
 * main.c
 *
 *  Created on: 5 juin 2009
 *      Author: monga
 */





/* fonctions de gestion d'un tas */



/* test des fonctions de gestion du tas */

#include <stdio.h>

#include <errno.h>0


#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>  /* unlink */

#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include "geometrie.h"


int main(void)

{
int iok;

fprintf(stderr,"\n pour passer d'un fichier .raw venant de MATLAB a une image opendx utiliser 'testcreerim' ou 'testcreerim2");

fprintf(stderr,"\n pour passer d'une image volumique a un fichier de points pour la triangulation 'testecor'");

fprintf(stderr,"\n pour obtenir des cylindres a partir d'un decoupage en rondelles");

fprintf(stderr,"\n pour passer d'un fichier de tetrahedres a un fichier contenant les points du squelette avec les rayons et les directions 'testfig3' options 7 et 3");

fprintf(stderr,"\n pour passer d'un fichier du squelette avec diametres a un fichier de cylindres initiaux 'finitcyl3'");

fprintf(stderr,"\n pour visualiser les cylindres initiaux et les seuiller selon les rayons lancer 'testviscylini2'");

fprintf(stderr,"\n pour lancer le regroupement hierarchique lancer 'testcgraf_t'");

fprintf(stderr,"\n pour visualiser lancer 'testviscylini2'");

fprintf(stderr,"\n pour obtenir des fichiers d'histogrammes lancer 'testhiso'");

fprintf(stderr,"\n \n");

fprintf(stderr,"\n pour obtenir des boules ");

fprintf(stderr,"\n pour obtenir un fichier boules+allongement+direction a partir d'un fichier de Delaunay : 'testfig5' 'testtri3' 'triboul5' 'visuboul2' 'filtre'");

fprintf(stderr,"\n pour obtenir un fichier de tetraedres sculptes : testfig6");

fprintf(stderr,"\n pour obtenir un fichier de boules maximales en prenant en compte le recouvrement duè squelette : 'triboul3'");

fprintf(stderr,"\n pour visualiser le squelette 'testsquel'");

fprintf(stderr,"\n pour sortir tapez 0");
fprintf(stderr,"\n pour lancer une simulation mepsom avec diffusion et options taper 1   ");
fprintf(stderr,"\n pour 'testtrans2' taper 2  ");
fprintf(stderr,"\n pour lancer une simulation mepsom avec diffusion methode 2 taper 3  ");
fprintf(stderr,"\n pour lancer une diffusion iterative avec le graphe taper 4  ");
fprintf(stderr,"\n pour lancer une simulation mepsom taper 5  ");
fprintf(stderr,"\n pour lancer une diffusion iterative taper 6  ");
fprintf(stderr,"\n pour lancer une diffusion taper 7  ");
fprintf(stderr,"\n pour generation du fichier des attributs taper 8  ");
fprintf(stderr,"\n pour simulation dynamique biologique taper 9  ");
fprintf(stderr,"\n pour calcul des coefficients de diffusion taper 10  ");
fprintf(stderr,"\n pour 'determination des chaines simples par regroupement' taper 11  ");
fprintf(stderr,"\n pour 'visuboul2_s' taper 12  ");
fprintf(stderr,"\n pour 'testapp' taper 13  ");
fprintf(stderr,"\n pour test de l'approximmation par un plan taper 14  ");
fprintf(stderr,"\n pour 'testsquel' taper 15  ");
fprintf(stderr,"\n pour 'testboule' taper 16  ");
fprintf(stderr,"\n pour 'testtaillent' taper 17  ");
fprintf(stderr,"\n pour 'testnbpointim' taper 18  ");
fprintf(stderr,"\n pour 'testcomparim' taper 19  ");
fprintf(stderr,"\n pour 'testcomparfic' taper 20  ");
fprintf(stderr,"\n pour 'testlecfic' taper 21  ");
fprintf(stderr,"\n pour 'testtrans' taper 22  ");
fprintf(stderr,"\n pour 'testcgraf3' taper 23  ");
fprintf(stderr,"\n pour 'testfig5' taper 24  ");
fprintf(stderr,"\n pour 'testfig6' taper 25  ");
fprintf(stderr,"\n pour 'testtri3' taper 26  ");
fprintf(stderr,"\n pour 'testreduboul' taper 27  ");
fprintf(stderr,"\n pour 'triboul5' taper 28  ");
fprintf(stderr,"\n pour 'triboul4' taper 29  ");
fprintf(stderr,"\n pour 'triboul3' taper 30  ");
fprintf(stderr,"\n pour 'triboul2' taper 31  ");
fprintf(stderr,"\n pour 'filtre' taper 32  ");
fprintf(stderr,"\n pour 'visuboul2' taper 33  ");
fprintf(stderr,"\n pour 'visorient' taper 34 ");
fprintf(stderr,"\n pour 'testfic2scilab' taper 35  ");
fprintf(stderr,"\n pour 'testhisto' taper 36  ");
fprintf(stderr,"\n pour 'testcreerim2' taper 37  ");
fprintf(stderr,"\n pour 'testecor' taper 38  ");
fprintf(stderr,"\n pour 'testecor2' taper 39  ");
fprintf(stderr,"\n pour 'testcgraf_t' taper 40  ");
fprintf(stderr,"\n pour 'testviscylini2' taper 41  ");
fprintf(stderr,"\n pour 'testfinitcyl4' taper 42  ");
fprintf(stderr,"\n pour 'testfinitcyl3' taper 43  ");
fprintf(stderr,"\n pour 'testfinitcyl2' taper 44  ");
fprintf(stderr,"\n pour 'testfinitcyl' taper 45  ");
fprintf(stderr,"\n pour 'testinitcyl' taper 46  ");
fprintf(stderr,"\n pour 'testviscylini' taper 47  ");
fprintf(stderr,"\n pour 'testseuil2' taper 48  ");
fprintf(stderr,"\n pour 'testseuil' taper 49  ");
fprintf(stderr,"\n pour 'testcgraf' taper 50  ");
fprintf(stderr,"\n pour 'testcgraf2' taper 51  ");
fprintf(stderr,"\n pour 'testviscyl' taper 52  ");
fprintf(stderr,"\n pour 'testrac3' taper 53  ");
fprintf(stderr,"\n pour 'testequa3' taper 54  ");
fprintf(stderr,"\n pour 'testfig' taper 55  ");
fprintf(stderr,"\n pour 'testfig3' taper 56  ");
fprintf(stderr,"\n pour 'testim' taper 57  ");
fprintf(stderr,"\n pour 'testetra' taper 58  ");
fprintf(stderr,"\n pour 'testcreerim' taper 59  ");
fprintf(stderr,"\n pour 'testtri' taper 60  ");
fprintf(stderr,"\n pour 'triboul' taper 61  ");
fprintf(stderr,"\n pour 'filtallong' taper 62  ");
fprintf(stderr,"\n pour 'testvisboul' taper 63  ");
fprintf(stderr,"\n taper le choix : ")
scanf("%d",&iok);

while(iok != 0)

switch(iok)

{

case 0:
	break;

case 1:
	simulmepsom3();
	break;

case 2:
	testtrans2();
	break;

case 3:
	simulmepsom2();
	break;

case 4:
	lancdifgr();
	break;

case 5:
	simulmepsom();
	break;

case 6:
	lancdifit();
	break;

case 7:
	lancdif();
	break;

case 8:
	intercalatt();
	break;

case 9:
	testsim();
	break;

case 10:
	testdif();
	break;

case 11:
	testcgraf1_s();
	break;

case 12:
	testvisboul2_s();
	break;

case 13:
	testapp();
	break;

case 14:
	testapplan2();
	break;

case 15:
	testsquel();
	break;

case 16:
	testboule();
	break;

case 17:
	testtaillent();
	break;

case 18:
	testnbpointim();
	break;

case 19:
	testcomparim();
	break;

case 20:
	testcomparfic();
	break;

case 21:
	testlecfic();
	break;

case 22:
	testtrans();
	break;

case 23:
	testcgraf3();
	break;

case 24:
	testfig5();
	break;

case 25:
	testfig6();
	break;

case 26:
	testtri3();
	break;

case 27:
	testreduboul();
	break;

case 28:
	triboul5();
	break;

case 29:
	triboul4();
	break;

case 30:
	triboul3();
	break;

case 31:
	triboul2();
	break;

case 32:
	filtre();
	break;

case 33:
	testvisboul2();
	break;

case 34:
	testvisorient();
	break;

case 35:
	testfic2scilab();
	break;

case 36:
	testhisto();
	break;

case 37:
	testcreerim2();
	break;

case 38:
	testecor();
	break;

case 39:
	testecor2();
	break;

case 40:
	testcgraf_t();
	break;

case 41:
	testviscylini2();
	break;

case 42:
	testfinitcyl4();
	break;

case 43:
	testfinitcyl3();
	break;

case 44:
	testfinitcyl2();
	break;

case 45:
	testfinitcyl();
	break;

case 46:
	testinitcyl();
	break;

case 47:
	testviscylini();
	break;

case 48:
	testseuil2();
	break;

case 49:
	testseuil();
	break;

case 50:
	testcgraf();
	break;

case 51:
	testcgraf2();
	break;

case 52:
	testviscyl();
	break;

case 53:
	testrac3();
	break;

case 54:
	testequa3();
	break;

case 55:
	testfig();
	break;

case 56:
	testfig3();
	break;

case 57:
	testim();
	break;

case 58:
	testetra();
	break;

case 59:
	testcreerim();
	break;

case 60:
	testtri();
	break

case 61:
	triboul();
	break;

case 62:
	filtallong();
	break;

case 63:
	testvisboul();
	break;

}

	fprintf(stderr,"\n pour sortir tapez 0");
	fprintf(stderr,"\n pour lancer une simulation mepsom avec diffusion et options taper 1   ");
	fprintf(stderr,"\n pour 'testtrans2' taper 2  ");
	fprintf(stderr,"\n pour lancer une simulation mepsom avec diffusion methode 2 taper 3  ");
	fprintf(stderr,"\n pour lancer une diffusion iterative avec le graphe taper 4  ");
	fprintf(stderr,"\n pour lancer une simulation mepsom taper 5  ");
	fprintf(stderr,"\n pour lancer une diffusion iterative taper 6  ");
	fprintf(stderr,"\n pour lancer une diffusion taper 7  ");
	fprintf(stderr,"\n pour generation du fichier des attributs taper 8  ");
	fprintf(stderr,"\n pour simulation dynamique biologique taper 9  ");
	fprintf(stderr,"\n pour calcul des coefficients de diffusion taper 10  ");
	fprintf(stderr,"\n pour 'determination des chaines simples par regroupement' taper 11  ");
	fprintf(stderr,"\n pour 'visuboul2_s' taper 12  ");
	fprintf(stderr,"\n pour 'testapp' taper 13  ");
	fprintf(stderr,"\n pour test de l'approximmation par un plan taper 14  ");
	fprintf(stderr,"\n pour 'testsquel' taper 15  ");
	fprintf(stderr,"\n pour 'testboule' taper 16  ");
	fprintf(stderr,"\n pour 'testtaillent' taper 17  ");
	fprintf(stderr,"\n pour 'testnbpointim' taper 18  ");
	fprintf(stderr,"\n pour 'testcomparim' taper 19  ");
	fprintf(stderr,"\n pour 'testcomparfic' taper 20  ");
	fprintf(stderr,"\n pour 'testlecfic' taper 21  ");
	fprintf(stderr,"\n pour 'testtrans' taper 22  ");
	fprintf(stderr,"\n pour 'testcgraf3' taper 23  ");
	fprintf(stderr,"\n pour 'testfig5' taper 24  ");
	fprintf(stderr,"\n pour 'testfig6' taper 25  ");
	fprintf(stderr,"\n pour 'testtri3' taper 26  ");
	fprintf(stderr,"\n pour 'testreduboul' taper 27  ");
	fprintf(stderr,"\n pour 'triboul5' taper 28  ");
	fprintf(stderr,"\n pour 'triboul4' taper 29  ");
	fprintf(stderr,"\n pour 'triboul3' taper 30  ");
	fprintf(stderr,"\n pour 'triboul2' taper 31  ");
	fprintf(stderr,"\n pour 'filtre' taper 32  ");
	fprintf(stderr,"\n pour 'visuboul2' taper 33  ");
	fprintf(stderr,"\n pour 'visorient' taper 34 ");
	fprintf(stderr,"\n pour 'testfic2scilab' taper 35  ");
	fprintf(stderr,"\n pour 'testhisto' taper 36  ");
	fprintf(stderr,"\n pour 'testcreerim2' taper 37  ");
	fprintf(stderr,"\n pour 'testecor' taper 38  ");
	fprintf(stderr,"\n pour 'testecor2' taper 39  ");
	fprintf(stderr,"\n pour 'testcgraf_t' taper 40  ");
	fprintf(stderr,"\n pour 'testviscylini2' taper 41  ");
	fprintf(stderr,"\n pour 'testfinitcyl4' taper 42  ");
	fprintf(stderr,"\n pour 'testfinitcyl3' taper 43  ");
	fprintf(stderr,"\n pour 'testfinitcyl2' taper 44  ");
	fprintf(stderr,"\n pour 'testfinitcyl' taper 45  ");
	fprintf(stderr,"\n pour 'testinitcyl' taper 46  ");
	fprintf(stderr,"\n pour 'testviscylini' taper 47  ");
	fprintf(stderr,"\n pour 'testseuil2' taper 48  ");
	fprintf(stderr,"\n pour 'testseuil' taper 49  ");
	fprintf(stderr,"\n pour 'testcgraf' taper 50  ");
	fprintf(stderr,"\n pour 'testcgraf2' taper 51  ");
	fprintf(stderr,"\n pour 'testviscyl' taper 52  ");
	fprintf(stderr,"\n pour 'testrac3' taper 53  ");
	fprintf(stderr,"\n pour 'testequa3' taper 54  ");
	fprintf(stderr,"\n pour 'testfig' taper 55  ");
	fprintf(stderr,"\n pour 'testfig3' taper 56  ");
	fprintf(stderr,"\n pour 'testim' taper 57  ");
	fprintf(stderr,"\n pour 'testetra' taper 58  ");
	fprintf(stderr,"\n pour 'testcreerim' taper 59  ");
	fprintf(stderr,"\n pour 'testtri' taper 60  ");
	fprintf(stderr,"\n pour 'triboul' taper 61  ");
	fprintf(stderr,"\n pour 'filtallong' taper 62  ");
	fprintf(stderr,"\n pour 'testvisboul' taper 63  ");
	fprintf(stderr,"\n taper le choix : ")
	scanf("%d",&iok);
}

}



void testfig3(void)

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

	float a12,a14,a13,a24,a34,a23,rap,amax,amin,s1,s2,s3,s4,smax;

	float m1m2,m1m3,m1m4,m2m3,m2m4,m3m4,xe,ye,ze,u,v,w,norme;

	int ind;



	zero = 0 ;

	epsilon = 0.0001;

	un = 1 ;

	fprintf(stderr,"nom du fichier entete en format opendx (image d'octets): ");

	scanf("%s",nomfic);

	fprintf(stderr,"\n %s",nomfic);

	fprintf(stderr,"\n nom du fichier image de sortie en format opendx (image d'octets): ");

	scanf("%s",nomfic2);

	fprintf(stderr,"\n %s",nomfic2);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	/* creation d'une image 3d d'entiers de dimensions dimx.dimy.dimz initialisee a un */

	ptimage = creimage(dimx,dimy,dimz,un);

	ptboule = (boule*)mymalloc(sizeof(boule));

	ptcyl = (cylindre*)mymalloc(sizeof(cylindre));

	ptcone = (cone*)mymalloc(sizeof(cone));

	ptetra = (tetra*)mymalloc(sizeof(tetra));





	fprintf(stderr,"\n voulez vous inserer des figures dans l'image (1:boule,2:cylindre,3:cone,4:tetraedre,5 : fichier de tetraedres,6: suite de boules, 7:fichier de tetras sculpte 0:rien)") ;

	scanf("%d",&iok);

	while(iok != 0)

	{

		if(iok == 1)

		{

			fprintf(stderr,"\n coordonnees du centre de la boule ; rayon de la boule : ");

			scanf("%f %f %f %f",&x0,&y0,&z0,&r);

			vol = (4*3.14116*r*r*r)/3;

			fprintf(stderr,"\n volume de la boule = %f",vol);

			ptboule->xc = x0;

			ptboule->yc = y0;

			ptboule->zc = z0;

			ptboule->rayon = r;

			npoints = boulimag(ptboule,ptimage,zero);

			fprintf(stderr,"\n nombre de points de la boule ecrits dans l'image = %d ",npoints);

		}else

		if(iok == 2)

		{



			fprintf(stderr,"\n coordonnes des points centres extremites \n du cylindre [x1,y1,z1,x2,y2,z2] et rayon :");

			scanf("%f %f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2,&r);

			ptcyl->x1 = x1;

			ptcyl->y1 = y1;

			ptcyl->z1 = z1;

			ptcyl->x2 = x2;

			ptcyl->y2 = y2;

			ptcyl->z2 = z2;

			ptcyl->rayon = r;

			fprintf(stderr,"\n entree dans inscylind");

			npoints = inscylind(ptimage,ptcyl,zero);

			fprintf(stderr,"\n sortie de inscylind");

			fprintf(stderr,"\n nb de points du cylindre ecrits dans l'image = %d",npoints);

			vol = (3.14116*r*r)*sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));

			fprintf(stderr,"\n volume du cylindre = %f",vol);

		}else

		if(iok == 3)

		{

			fprintf(stderr,"\n coordonnes des points centres extremites \n [x1,y1,z1,x2,y2,z2] et rayons :");

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

			fprintf(stderr,"\n nb de points du cone ecrits dans l'image = %d",npoints);

			fprintf(stderr,"\n volume du cone = %f",vol);

		}else

		if(iok == 4)

		{

			fprintf(stderr,"\n coordonnes entieres des 4 sommets du tetraedre : ");

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

			fprintf(stderr,"\n %f %f %f %f",ptboule->xc,ptboule->yc,

			ptboule->zc,ptboule->rayon);



		}else

		if(iok == 5)

		{

			fprintf(stderr,"\n fichier de tetraedres : ");

			scanf("%s",nomft);

			lfictet(nomft,&lpoints,&ltet,&nbtet,&nbs,&xm,&ym,&zm);

			fprintf(stderr,"\n nb de tetrahedres= %d  nb de sommets=%d xm=%f ym=%f zm=%f",

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

						fprintf(stderr,"\n i = %d npboul = %d",i,npboul);

					}

					moyenne = moyenne + npboul;

					dmax = dmax + dmin;

				}



			}

			moyenne = moyenne/(nbtet - nbtriang);

			dmax = dmax/(nbtet - nbtriang);

			fprintf(stderr,"\n moyenne de la distance des points des spheres avec les sommets=%e",dmax);

			fprintf(stderr,"\n nb de triangles et tetrahedres plans = %d",nbtriang);

			fprintf(stderr,"\n nb de tetrahedres de Delaunay parfaits = %d",ndelaunay);

			fprintf(stderr,"\n nb de tetrahedres de Delaunay bizarres = %d",nbiz);

			fprintf(stderr,"\n nb de centres dans l'image = %d",nsq);

			fprintf(stderr,"\n nb moyen de sommets dans les spheres de Delaunay = %f",moyenne);

		}

		else if(iok == 6)



		{

			fprintf(stderr,"\n  creation d'une suite de boules dont les centres sont sur un segment") ;

			scanf("%d",&iok);

			fprintf(stderr,"\n taper dans l'ordre : coordonnees des extremites;nb de boules : ");

			scanf("%f %f %f %f %f %f %d",&x1,&y1,&z1,&x2,&y2,&z2,&nbboules);

			rayons = (float*)mycalloc(nbboules,sizeof(float));

			fprintf(stderr,"\n taper la suite des rayons des boules : ");

			for(i=0;i<nbboules;i++)

			{

				fprintf(stderr,"\n rayon de la boule %d = ",i);

				scanf("%f",&r);

				rayons[i] = r;

			}

			ptboules = generboules(x1,y1,z1,x2,y2,z2,rayons,nbboules,&nb);

			fprintf(stderr,"\n insertion des boules dans l'image");

			for(i=0;i<nb;i++)

			npoints = boulimag(ptboules[i],ptimage,zero);

		}

		else if(iok == 7)

		{

			fprintf(stderr,"\n fichier de tetraedres : ");

			scanf("%s",nomft);

			lfictet(nomft,&lpoints,&ltet,&nbtet,&nbs,&xm,&ym,&zm);

			fprintf(stderr,"\n nb de tetrahedres= %d  nb de sommets=%d xm=%f ym=%f zm=%f",

			nbtet,nbs,xm,ym,zm);

			sculpt = (short int*)mycalloc(nbtet,sizeof(short int));

			fprintf(stderr,"\n nom du fichier image contenant la forme pleine (image d'octets): ");

			scanf("%s",nomima);

			fprintf(stderr,"\n %s",nomima);

			fprintf(stderr,"\n nombre de caracteres de l'entete (690 pour 'originale.dx', 689 pour cylind) = ");

			scanf("%d",&tentete);



			fprintf(stderr,"\n valeur mimimale du rapport 'nb de pts du tetra en dehors de la forme'/'nb de pts du tetra dans la forme (0.1 raisonnable)'");

			scanf("%f",&pourcentage);



			fprintf(stderr,"\n pour image du squelette en sortie taper 1 ; pour image des tetraedres taper 2 pour image du squelette avec les rayons et fichier du squelette avec diametres taper 3 : ");

			scanf("%d",&iopt);



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

			for(i =0;i<nbtet;i++)

			{

				if(ind == 100000)

				{

					fprintf(stderr,"\n i=%d ntetras=%d",i,ntetras);

					ind = 1;

				}

				else

				ind++;



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

			fprintf(stderr,"\n nb de tetrahedres retires = %d",ntetras);

			/* ecriture des tetraedres de la forme dans l'image en sortie */

			nsquel = 0;

			nfsquel = 0;

			if(iopt == 1 || iopt == 3)

			{

				fprintf(stderr,"\n fichier de sortie des boules circonscrites :");

				scanf("%s",nomficbou);

				ptficbou = fopen(nomficbou,"w");

				if(iopt==3)

				for(i=0;i<dimx;i++)

				for(j=0;j<dimy;j++)

				for(k=0;k<dimz;k++)

				ptimage->tab[i][j][k] = 0;

				if(iopt == 3)

				{

					fprintf(stderr,"\n fichier de sortie du squelette : ");

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

						if(npoints == -1)fprintf(stderr,"\n anomalie !!!");

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

							else if(iopt ==3)

							{

								if(ptimage->tab[neari(x0)][neari(y0)][neari(z0)] == 0)

								/* ATTENTION DANS LE CAS OU PLUSIEURS RAYONS SONT ASSOCIES A UN MEME

								CENTRE IL FAUDRAIT PRIVILEGIE LES PLUS GRANDS RAYONS DONC COMPARER L'EXPRESSION

								PRECEDENTE NON PAS A 0 MAIS A r */

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

									fprintf(ptfsquel,"\n %f %f %f %f %f %f %f",x0,y0,z0,r,u,v,w);

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

							if(amax == 0)fprintf(stderr,"\n amax = %f amin = %f",amin,amax);

							fprintf(ptficbou,"\n %f %f %f %f %f %f %f",x0,y0,z0,r,amax,amin,rap);

						}



					}

				}

			}

			if(iopt==1 || iopt == 3)

			{

				fprintf(stderr,"\n nb de points du squelette = %d",nsquel);

				fprintf(ptficbou,"\n %d",nsquel);

				fclose(ptficbou);

				if(iopt == 3)

				{

					fprintf(stderr,"\n nb de points dans le fichier du squelette = %d",nfsquel);

					fprintf(ptfsquel,"\n %d",nfsquel);

					fclose(ptfsquel);

				}

			}

		}

		fprintf(stderr,"\n voulez vous inserer des figures (boules,cones,cylindres) dans l'image (1:boule,2:cylindre,3:cone,4:tetraedre, 5:fichier de tetrahedres,6 : suite de boules, 7 : fichier de tetras sculpte, 0:rien :");

		scanf("%d",&iok);

	}

	/* ecriture de l'image resultat */

	npts = ecrimage(nomfic,nomfic2,ptimage);

}




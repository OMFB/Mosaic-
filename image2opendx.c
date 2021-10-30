/*
 * image2opendx.c
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

/* cree une image opendx avec un entete (nomfic), un fichier de voxels

(nomfic2) et les dimensions de l'image dimx,dimy,dimz

l'image opendx est dans nomfic3 */

void creerim(char *nomfic,char *nomfic2,char *nomfic3,int dimx,int dimy,int dimz)

{



	FILE *f1file,*f2file,*f3file,*ptentete,*ptimsur;

	int i,j,k,entier,iopt,iech;

	image *im,*im2;

	int nzeros,nuns,naut,l;

	char entete[50],imsur[50];





	f1file = fopen(nomfic,"r");

	f2file = fopen(nomfic2,"r");

	f3file = fopen(nomfic3,"w");

	fprintf(stderr,"\n pour traitement des bords taper 1 sinon 0 : ");

	scanf("%d",&iopt);

	fprintf(stderr,"\n pour sur-echantillonnage en z taper le coeff multipicateur des plans sinon 0 : ");

	scanf("%d",&iech);

	if(iech > 1)

	{

		fprintf(stderr,"\n entete de l'image surechantillonnee en z (dz,dy,dx): ");

		scanf("%s",entete);

		fprintf(stderr,"\n nom de l'image de sortie surechantillonnee :");

		scanf("%s",imsur);

		ptentete = fopen(entete,"r");

		ptimsur = fopen(imsur,"w");

	}

	nzeros = 0;

	nuns = 0;

	naut = 0;

	im = creimage(dimx,dimy,dimz,1);

	/* ecriture de l'entete */

	entier = fgetc(f1file);

	while(entier != EOF)

	{

		fputc(entier,f3file);

		entier = fgetc(f1file);

	}

	fprintf(stderr,"\n fin de l'ecriture de l'entete");

	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	for(i=0;i<dimx;i++)

	{

		entier = fgetc(f2file);

		if(entier == 0)nzeros++;

		else if (entier == 1)nuns++;

		else naut++;

		im->tab[i][j][k] = entier;

	}

	fprintf(stderr,"\n nb de 0 =%d nb de 1 =%d nb autres =%d",nzeros,nuns,naut);

	if(iopt==1)

	{

		im2 = fondaun(im);

		fprintf(stderr,"\n fin du traitement des bords");

	}

	else

	{

		im2 = creimage(dimx,dimy,dimz,1);

		for(i=0;i<dimx;i++)

		for(j=0;j<dimy;j++)

		for(k=0;k<dimz;k++)

		{

			im2->tab[i][j][k] = im->tab[i][j][k];

		}

	}

	freet3(im->tab,im->dimx,im->dimy,im->dimz);

	/* im2 contient l'image avec les bords traites */

	nzeros = 0;

	nuns = 0;

	naut = 0;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	{

		entier = im2->tab[i][j][k];

		if(entier == 0)nzeros++;

		else if (entier == 1)nuns++;

		else naut++;

	}

	fprintf(stderr,"\n nb de 0 = %d nb de 1 = %d nb d'autres = %d",nzeros,nuns,naut);

	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	for(i=0;i<dimx;i++)

	{

		entier = im2->tab[i][j][k];

		fputc(entier,f3file);

	}

	fprintf(stderr,"\n fin de l'ecriture de l'image non surechantillonnee en z");

	if(iech > 1)

	{

		fprintf(stderr,"\n ecriture de l'image surechantillonnee");

		entier = fgetc(ptentete);

		while(entier != EOF)

		{

			fputc(entier,ptimsur);

			entier = fgetc(ptentete);

		}

		for(k=0;k<dimz;k++)

		{

			for(l = 0;l<iech;l++)

			{

				for(j=0;j<dimy;j++)

				for(i=0;i<dimx;i++)

				{

					entier = im2->tab[i][j][k];

					fputc(entier,ptimsur);

				}

			}

		}

		fprintf(stderr,"\n fin de l'ecriture de l'image surechantillonnee");

	}

	fprintf(stderr,"\n entree dans freet3");

	freet3(im2->tab,im2->dimx,im2->dimy,im2->dimz);

	fprintf(stderr,"\n sortie de freet3");

	fclose(f1file);

	fclose(f2file);

	fclose(f3file);

	if(iech > 1)

	{

		fclose(ptentete);

		fclose(ptimsur);

	}

	return;



}



void testcreerim()

{

	char entete[50],voxels[50],opendx[50];

	int dimx,dimy,dimz;



	printf("\n fichier entete : ");

	scanf("%s",entete);

	printf("\n fichier des valeurs des voxels : ");

	scanf("%s",voxels);

	printf("\n fichier image opendx en sortie : ");

	scanf("%s",opendx);

	printf("\n dimx dimy dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	creerim(entete,voxels,opendx,dimx,dimy,dimz);

}



void testcreerim2()

{

	char entete[50],voxels[50],opendx[50];

	int dimx,dimy,dimz,xo,yo,zo,dx,dy,dz;





	printf("\n fichier des valeurs des voxels : ");

	scanf("%s",voxels);

	printf("\n dimensions de l'image contenue dans ce fichier (dimx dimy dimz) : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	printf("\n fichier image opendx en sortie : ");

	scanf("%s",opendx);

	printf("\n coordonnees du point origine : ");

	scanf("%d %d %d",&xo,&yo,&zo);

	printf("\n dimensions de l'image a extraire (dx,dy,dz) : ");

	scanf("%d %d %d",&dx,&dy,&dz);

	printf("\n fichier entete pour l'image resultat (dz,dy,dx): ");

	scanf("%s",entete);



	creerim2(entete,voxels,opendx,dimx,dimy,dimz,xo,yo,zo,dx,dy,dz);

}

void testcreerim3()

{

	char entete[50],voxels[50],opendx[50];

	int dimx,dimy,dimz,xo,yo,zo,dx,dy,dz;





	printf("\n fichier des valeurs des voxels : ");

	scanf("%s",voxels);

	printf("\n dimensions de l'image contenue dans ce fichier (dimx dimy dimz) : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	printf("\n fichier image opendx en sortie : ");

	scanf("%s",opendx);

	printf("\n coordonnees du point origine (� partir de (0,0,0)): ");

	scanf("%d %d %d",&xo,&yo,&zo);

	printf("\n dimensions de l'image a extraire (dx,dy,dz) : ");

	scanf("%d %d %d",&dx,&dy,&dz);

	printf("\n fichier entete pour l'image resultat (dz,dy,dx): ");

	scanf("%s",entete);



	creerim3(entete,voxels,opendx,dimx,dimy,dimz,xo,yo,zo,dx,dy,dz);

}


/* cette procedure met le fond d'une image a 1 */

image *fondaun(image *im)

{

	int dimx,dimy,dimz;

	int i,j,k;

	image *im2;



	fprintf(stderr,"\n entree dans fondaun");

	dimx = im->dimx;

	dimy = im->dimy;

	dimz = im->dimz;

	im2 = creimage(dimx,dimy,dimz,1);

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	{

		im2->tab[i][j][k] = im->tab[i][j][k];

	}

	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	{

		/* traitement de la ligne definie par k et j */

		/* parcours de la ligne dans un sens */

		i = 0;

		while(im->tab[i][j][k] == 0 && i<dimx)

		{

			im2->tab[i][j][k] = 1;

			i++;

			if(i>=dimx)break;

		}

		/* parcours de la ligne dans l'autre sens */

		i = dimx-1;

		while(im->tab[i][j][k] == 0 && i>=0)

		{

			im2->tab[i][j][k] = 1;

			i--;

			if(i<0)break;

		}

	}

	fprintf(stderr,"\n sortie de fondaun");

	return(im2);

}



/* cree une image opendx avec un entete (nomfic), un fichier de voxels

(nomfic2) et les dimensions de l'image dimx,dimy,dimz

l'image opendx est dans nomfic3 */

void creerim2(char *nomfic,char *nomfic2,char *nomfic3,

int dimx,int dimy,int dimz,int xo,int yo,int zo,int dx,int dy,int dz)

{



	FILE *f1file,*f2file,*f3file,*ptentete,*ptimsur;

	int i,j,k,entier,iopt,iech;

	image *im,*im2;

	int nzeros,nuns,naut,l;

	char entete[50],imsur[50];





	f1file = fopen(nomfic,"r");

	f2file = fopen(nomfic2,"r");

	f3file = fopen(nomfic3,"w");

	fprintf(stderr,"\n pour traitement des bords taper 1 sinon 0 : ");

	scanf("%d",&iopt);

	fprintf(stderr,"\n pour sur-echantillonnage en z taper le coeff multipicateur des plans sinon 0 : ");

	scanf("%d",&iech);

	if(iech > 1)

	{

		fprintf(stderr,"\n entete de l'image surechantillonnee en z (dz,dy,dx): ");

		scanf("%s",entete);

		fprintf(stderr,"\n nom de l'image de sortie surechantillonnee :");

		scanf("%s",imsur);

		ptentete = fopen(entete,"r");

		ptimsur = fopen(imsur,"w");

	}

	nzeros = 0;

	nuns = 0;

	naut = 0;

	fprintf(stderr,"\n entree dans creimage");

	im = creimage(dx,dy,dz,1);

	fprintf(stderr,"\n sortie de creimage");

	/* ecriture de l'entete */

	entier = fgetc(f1file);

	while(entier != EOF)

	{

		fputc(entier,f3file);

		entier = fgetc(f1file);

	}

	fprintf(stderr,"\n fin de l'ecriture de l'entete");

	for(k=0;k<dimz;k++)
	{
		fprintf(stderr,"\n k = %d",k);

		for(j=0;j<dimy;j++)

			for(i=0;i<dimx;i++)

			{

				entier = fgetc(f2file);

				if(i>=xo && j>=yo && k>=zo && i<dx+xo && j<dy+yo && k<dz+zo)

				{

					im->tab[i-xo][j-yo][k-zo] = entier;

					if(entier == 0)nzeros++;

					else if (entier == 1)nuns++;

					else naut++;

				}

			}
	}

	fprintf(stderr,"\n nb de 0 =%d nb de 1 =%d nb autres =%d",nzeros,nuns,naut);

	if(iopt==1)

	{

		im2 = fondaun(im);

		fprintf(stderr,"\n fin du traitement des bords");

	}

	else

	{

		im2 = creimage(dx,dy,dz,1);

		for(i=0;i<dx;i++)

		for(j=0;j<dy;j++)

		for(k=0;k<dz;k++)

		{

			im2->tab[i][j][k] = im->tab[i][j][k];

		}

	}

	freet3(im->tab,im->dimx,im->dimy,im->dimz);

	/* im2 contient l'image avec les bords traites */

	nzeros = 0;

	nuns = 0;

	naut = 0;

	for(i=0;i<dx;i++)

	for(j=0;j<dy;j++)

	for(k=0;k<dz;k++)

	{

		entier = im2->tab[i][j][k];

		if(entier == 0)nzeros++;

		else if (entier == 1)nuns++;

		else naut++;

	}

	fprintf(stderr,"\n nb de 0 = %d nb de 1 = %d nb d'autres = %d",nzeros,nuns,naut);

	for(k=0;k<dz;k++)

	for(j=0;j<dy;j++)

	for(i=0;i<dx;i++)

	{

		entier = im2->tab[i][j][k];

		fputc(entier,f3file);

	}

	fprintf(stderr,"\n fin de l'ecriture de l'image non surechantillonnee en z");

	if(iech > 1)

	{

		fprintf(stderr,"\n ecriture de l'image surechantillonnee");

		entier = fgetc(ptentete);

		while(entier != EOF)

		{

			fputc(entier,ptimsur);

			entier = fgetc(ptentete);

		}

		for(k=0;k<dz;k++)

		{

			for(l = 0;l<iech;l++)

			{

				for(j=0;j<dy;j++)

				for(i=0;i<dx;i++)

				{

					entier = im2->tab[i][j][k];

					fputc(entier,ptimsur);

				}

			}

		}

		fprintf(stderr,"\n fin de l'ecriture de l'image surechantillonnee");

	}

	fprintf(stderr,"\n entree dans freet3");

	freet3(im2->tab,im2->dimx,im2->dimy,im2->dimz);

	fprintf(stderr,"\n sortie de freet3");

	fclose(f1file);

	fclose(f2file);

	fclose(f3file);

	if(iech > 1)

	{

		fclose(ptentete);

		fclose(ptimsur);

	}

	return;



}

/* cree une image opendx avec un entete (nomfic), un fichier de voxels sous forme de suite d'entiers

(nomfic2) et les dimensions de l'image dimx,dimy,dimz

l'image opendx est dans nomfic3 */

void creerim3(char *nomfic,char *nomfic2,char *nomfic3, int dimx,int dimy,int dimz,int xo,int yo,int zo,int dx,int dy,int dz)

{



	FILE *f1file,*f2file,*f3file,*ptentete,*ptimsur;

	int i,j,k,entier,iopt,iech;

	image *im,*im2;

	int nzeros,nuns,naut,l;

	char entete[50],imsur[50];





	f1file = fopen(nomfic,"r");

	f2file = fopen(nomfic2,"r");

	f3file = fopen(nomfic3,"w");

	fprintf(stderr,"\n pour traitement des bords taper 1 sinon 0 : ");

	scanf("%d",&iopt);

	fprintf(stderr,"\n pour sur-echantillonnage en z taper le coeff multipicateur des plans sinon 0 : ");

	scanf("%d",&iech);

	if(iech > 1)

	{

		fprintf(stderr,"\n entete de l'image surechantillonnee en z (dz,dy,dx): ");

		scanf("%s",entete);

		fprintf(stderr,"\n nom de l'image de sortie surechantillonnee :");

		scanf("%s",imsur);

		ptentete = fopen(entete,"r");

		ptimsur = fopen(imsur,"w");

	}

	nzeros = 0;

	nuns = 0;

	naut = 0;

	fprintf(stderr,"\n entree dans creimage");

	im = creimage(dx,dy,dz,1);

	fprintf(stderr,"\n sortie de creimage");

	/* ecriture de l'entete */

	entier = fgetc(f1file);

	while(entier != EOF)

	{

		fputc(entier,f3file);

		entier = fgetc(f1file);

	}

	fprintf(stderr,"\n fin de l'ecriture de l'entete");

	for(k=0;k<dimz;k++)
	{
		fprintf(stderr,"\n k = %d",k);

		for(j=0;j<dimy;j++)

			for(i=0;i<dimx;i++)

			{

				fscanf(f2file,"%d",&entier);

				if(i>=xo && j>=yo && k>=zo && i<dx+xo && j<dy+yo && k<dz+zo)

				{

					im->tab[i-xo][j-yo][k-zo] = entier;

					if(entier == 0)nzeros++;

					else if (entier == 1)nuns++;

					else naut++;

				}

			}
	}

	fprintf(stderr,"\n nb de 0 =%d nb de 1 =%d nb autres =%d",nzeros,nuns,naut);

	if(iopt==1)

	{

		im2 = fondaun(im);

		fprintf(stderr,"\n fin du traitement des bords");

	}

	else

	{

		im2 = creimage(dx,dy,dz,1);

		for(i=0;i<dx;i++)

		for(j=0;j<dy;j++)

		for(k=0;k<dz;k++)

		{

			im2->tab[i][j][k] = im->tab[i][j][k];

		}

	}

	freet3(im->tab,im->dimx,im->dimy,im->dimz);

	/* im2 contient l'image avec les bords traites */

	nzeros = 0;

	nuns = 0;

	naut = 0;

	for(i=0;i<dx;i++)

	for(j=0;j<dy;j++)

	for(k=0;k<dz;k++)

	{

		entier = im2->tab[i][j][k];

		if(entier == 0)nzeros++;

		else if (entier == 1)nuns++;

		else naut++;

	}

	fprintf(stderr,"\n nb de 0 = %d nb de 1 = %d nb d'autres = %d",nzeros,nuns,naut);

	for(k=0;k<dz;k++)

	for(j=0;j<dy;j++)

	for(i=0;i<dx;i++)

	{

		entier = im2->tab[i][j][k];

		fputc(entier,f3file);

	}

	fprintf(stderr,"\n fin de l'ecriture de l'image non surechantillonnee en z");

	if(iech > 1)

	{

		fprintf(stderr,"\n ecriture de l'image surechantillonnee");

		entier = fgetc(ptentete);

		while(entier != EOF)

		{

			fputc(entier,ptimsur);

			entier = fgetc(ptentete);

		}

		for(k=0;k<dz;k++)

		{

			for(l = 0;l<iech;l++)

			{

				for(j=0;j<dy;j++)

				for(i=0;i<dx;i++)

				{

					entier = im2->tab[i][j][k];

					fputc(entier,ptimsur);

				}

			}

		}

		fprintf(stderr,"\n fin de l'ecriture de l'image surechantillonnee");

	}

	fprintf(stderr,"\n entree dans freet3");

	freet3(im2->tab,im2->dimx,im2->dimy,im2->dimz);

	fprintf(stderr,"\n sortie de freet3");

	fclose(f1file);

	fclose(f2file);

	fclose(f3file);

	if(iech > 1)

	{

		fclose(ptentete);

		fclose(ptimsur);

	}

	return;
}


/* prend en entree un fichier de cylindres et sort des fichiers pour les

histogrammes :

	- directions des axes (entre 0 et 180)/volume des cylindres

	- direction des axes (entre 0 et 180)/ longueur des cylindres

	- rayons des cylindres / longueur des axes



Le pas de discretisation des orientations est "pasdis" (entre 10 et 100 dans

la pratique)

*/



void histo(char *ficyl,char *axevol,char *axelong,char *axeray,char

*raylong, int pasdis)

{

	FILE *ptficyl,*ptaxevol,*ptaxelong,*ptraylong,*ptaxeray;

	float **vollongor,***orvolaxe,*raylongu;

	int ncyl,i,j,k,ue,ve;

	float x1,y1,z1,x2,y2,z2,r,laxe,u,v,w,vol,pie,un,vn,wn,rmax;



	ptficyl = fopen(ficyl,"r");

	ptaxevol = fopen(axevol,"w");

	ptaxelong = fopen(axelong,"w");

	ptaxeray = fopen(axeray,"w");

	ptraylong = fopen(raylong,"w");



	fscanf(ptficyl,"%d",&ncyl);

	vollongor = allot2f(5,ncyl);

	pie = 3.14116;

	rmax = 0;

	for(i=0;i<ncyl;i++)

	{

		fscanf(ptficyl,"%f %f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2,&r);

		laxe = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));

		vol = laxe*pie*r*r;

		vollongor[0][i] = vol;

		vollongor[1][i] = laxe;

		vollongor[2][i] = r;

		if(r>rmax)rmax=r;

		if(laxe==0)

		{

			fprintf(stderr,"\n laxe=0 !!!");

			exit(18);

		}

		u = (x1-x2)/laxe;

		v = (y1-y2)/laxe;

		w = (z1-z2)/laxe;

		if(w > 0)

		{

			un = u;

			vn = v;

			wn = w;

		}

		else if(w < 0)

		{

			un = -u;

			vn = -v;

			wn = -w;

		}

		else if(w == 0)

		if(v > 0)

		{

			un = u;

			vn = v;

			wn = w;

		}

		else if(v < 0)

		{

			un = -u;

			vn = -v;

			wn = -w;

		}else if(v==0)

		if(u > 0)

		{

			un = u;

			vn = v;

			wn = w;

		}else if(u < 0)

		{

			un = -u;

			vn = -v;

			wn = -w;

		}

		/* la direction de l'axe est completement define par deux coordonnees

		prises dans (un,vn,wn) ; on considere par exemple les deux premieres */

		vollongor[3][i] = un;

		vollongor[4][i] = vn;

	}

	fclose(ptficyl);



	orvolaxe = creertab3f(2*pasdis + 1,2*pasdis + 1,4);



	for(i=0;i<2*pasdis+1;i++)

	for(j=0;j<2*pasdis+1;j++)

	for(k=0;k<4;k++)

	orvolaxe[i][j][k] = 0;



	/* u,v,rayon,longueur,volume */

	for(i = 0; i<ncyl; i++)

	{

		u = vollongor[3][i];

		v = vollongor[4][i];

		ue = neari(pasdis*u + pasdis);

		ve = neari(pasdis*v + pasdis);

		if(ue>2*pasdis || ve>2*pasdis || ue<0 || ve<0)

		{

			fprintf(stderr,"u=%f v=%f ue=%d ve=%d pasdis=%d",u,v,ue,ve,pasdis);

			exit(43);

		}

		orvolaxe[ue][ve][3]++;

		vol = vollongor[0][i];

		laxe = vollongor[1][i];

		r = vollongor[2][i];

		/* on ecrit la somme des rayons des cylindres */

		orvolaxe[ue][ve][0] += r;

		/* on ecrit la somme des longueurs des axes */

		orvolaxe[ue][ve][1] += laxe;

		/* on ecrit la somme des volumes des cylindres */

		orvolaxe[ue][ve][2] += vol;

	}

	for(i=0;i<2*pasdis+1;i++)

	for(j=0;j<2*pasdis+1;j++)

	{

		if(orvolaxe[i][j][3] != 0)

			orvolaxe[i][j][0] = orvolaxe[i][j][0]/orvolaxe[i][j][3];

		else

			orvolaxe[i][j][0] = 0;

	}

	/* generation des fichiers permettant de tracer les histogrammes

	a pour les directions */

	fprintf(stderr,"\n dx=dy=%d",2*pasdis);



	for(i=0;i<2*pasdis+1;i++)

	for(j=0;j<2*pasdis+1;j++)

	{

		fprintf(ptaxevol,"\n %d %d %f",i,j,orvolaxe[i][j][2]);

		fprintf(stderr," $ %d %d %f",i,j,orvolaxe[i][j][2]);

	}



	fprintf(stderr,"\n %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");

	fprintf(stderr,"\n");

	for(i=0;i<2*pasdis+1;i++)

	for(j=0;j<2*pasdis+1;j++)

	{

		fprintf(ptaxelong,"\n %d %d %f",i,j,orvolaxe[i][j][1]);

		fprintf(stderr," $ %d %d %f",i,j,orvolaxe[i][j][1]);

	}



	fprintf(stderr,"\n %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");

	fprintf(stderr,"\n");



	for(i=0;i<2*pasdis+1;i++)

	for(j=0;j<2*pasdis+1;j++)

	{

		fprintf(ptaxeray,"\n %d %d %f",i,j,orvolaxe[i][j][0]);

		fprintf(stderr," $ %d %d %f",i,j,orvolaxe[i][j][0]);

	}



	fprintf(stderr,"\n %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%");

	fprintf(stderr,"\n");





	/* generation du fichier permettant de tracer l'histogramme

	des rayons et des longueurs */

	raylongu = (float*)mycalloc(ceil(rmax)+1,sizeof(float));



	for(i=0;i<ceil(rmax)+1;i++)

	raylongu[i] = 0;



	for(i=0;i<ncyl;i++)

	{



		raylongu[neari(vollongor[2][i])] += vollongor[1][i];

	}



	fprintf(stderr,"\n rayon max = %f",rmax);



	for(i=0;i<ceil(rmax)+1;i++)

	{

		fprintf(ptraylong,"\n %d %f",i,raylongu[i]);

		fprintf(stderr,"$ %d %f",i,raylongu[i]);

	}

	fclose(ptaxevol);

	fclose(ptaxelong);

	fclose(ptaxeray);

	fclose(ptraylong);



}





void testhisto(void)

{

	char ficyl[50],axevol[50],axelong[50],axeray[50],raylong[50];

	int pasdis;



	fprintf(stderr,"\n fichier des cylindres, axe-vol, axe-long, axe-ray, ray-long ");

	scanf("%s %s %s %s %s",ficyl,axevol,axelong,axeray,raylong);

	fprintf(stderr,"%s ; %s ; %s ; %s ; %s",ficyl,axevol,axelong,axeray,raylong);

	fprintf(stderr,"\n pas d'echantillonage de l'orientation des axes et des rayons :");

	scanf("%d",&pasdis);

	histo(ficyl,axevol,axelong,axeray,raylong, pasdis);

}



/* prend en entr�e un fichier contenant une suite de triplets separes par des

blancs et rend en sortie la matrice correspondante en format scilab

valmax correspond � la valeur maximum de la sortie histogramme */



void fic2scilab(char *fic,char *scilab,int dimx,int dimy,int valmax)

{

	FILE *ptfic,*ptscilab;

	float **matrice,r,rmax,*mat2;

	int i,j,k,nlig;



	ptfic = fopen(fic,"r");

	ptscilab = fopen(scilab,"w");

	if(dimy != 1)

	{

		/* on rentre dans 'matrice' les donnees */

		nlig = dimx*dimy;

		matrice = allot2f(dimx,dimy);

		rmax = 0;

		for(k=0;k<nlig;k++)

		{

			fscanf(ptfic,"%d %d %f",&i,&j,&r);

			matrice[i][j] = r;

			if(r > rmax)rmax=r;

		}

		fclose(ptfic);

		if(valmax < 0)rmax=valmax;

		/* on ecrit dans le fichier en sortie les donnees pour scilab */

		fprintf(ptscilab,"%c",'[');

		for(j=0;j<dimy;j++)

		{

			for(i=0;i<dimx;i++)

			{

				fprintf(ptscilab,"%d ",neari((matrice[i][j]*valmax)/rmax));

			}

			fprintf(ptscilab,"%c",'.');

			fprintf(ptscilab,"%c",'.');

			fprintf(ptscilab,"%c",'.');

			fprintf(ptscilab,"\n");

			if(j != dimy-1)fprintf(ptscilab," %c ",';');

		}

		freetf(matrice,dimx-1,dimy-1);

		fprintf(ptscilab,"%c",']');

		fclose(ptscilab);

	}

	else

	{

		mat2 = (float*)mycalloc(dimx,sizeof(float));

		rmax = 0;

		for(k=0;k<dimx;k++)

		{

			fscanf(ptfic,"%d %f",&i,&r);

			mat2[i] = r;

			if(r > rmax)rmax=r;

		}

		fclose(ptfic);

		if(valmax < 0)rmax=valmax;

		/* on ecrit dans le fichier en sortie les donnees pour scilab */

		fprintf(ptscilab,"%c",'[');

		for(i=0;i<dimx;i++)

			fprintf(ptscilab,"%d ",neari((mat2[i]*valmax)/rmax));

		myfreef(mat2);

		fprintf(ptscilab,"%c",']');

		fclose(ptscilab);

	}

}



void testfic2scilab(void)

{

	char fic[50],scilab[50];

	int dimx,dimy,valmax;



	fprintf(stderr,"\n fichier en entree : ");

	scanf("%s",fic);

	fprintf(stderr,"\n fichier en sortie : ");

	scanf("%s",scilab);

	fprintf(stderr,"\n dimx = ");

	scanf("%d",&dimx);

	fprintf(stderr,"\n dimy (1 si histogramme 2D) = ");

	scanf("%d",&dimy);

	fprintf(stderr,"\n valeur maximum (si -1 pas de traitement) = ");

	scanf("%d",&valmax);

	fic2scilab(fic,scilab,dimx,dimy,valmax);

}



void testlecfic(void)

{

	int ncar,entier,i ;

	char fic[50];

	FILE *ptfic;



	fprintf(stderr,"\n nom du fichier a lire : ");

	scanf("%s",fic);

	fprintf(stderr,"\n nb de caracteres a lire = ");

	scanf("%d",&ncar);

	ptfic = fopen(fic,"r");

	entier = fgetc(ptfic);



	for(i = 0;i<ncar;i++)

	{

			if(entier != EOF)

			{

				fprintf(stderr,"%d;",entier);

				entier = fgetc(ptfic);

			}

	}

}







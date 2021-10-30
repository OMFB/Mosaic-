/*
 * init-cyl.c
 *
 *  Created on: 5 juin 2009
 *      Author: monga
 */

/* definition des structures utilisees pour le graphe et pour le tas, tous les taleaux utilises seront indexes

a partir de 1 car ces programmes s'inspirent de programmes

ecrits precedemment en fortran. Pour des raisons de rapidite de

programmation, on a pris les memes structures de donnees que celles utlises dans les

programmes ecrits en fortan 77 */

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





/* prend en entree une image ou tous les points du squelette

sont etiquetes avec la valeur du rayon et les autres points

a 0 et ecrit les cylindres initiaux correspondants dans un fichier

de cylindres au format suivant : nb de cylindres, x1,y1,z1,x2,y2,z2,rayon */



void initcyl(char *imsquel,int dimx,int dimy,int dimz,

int tentete,char *ficyl)

{

	image *ptsquel;

	cylindre **tabcyl,*cylind;

	int nbsquel,i,j,k,nmaxcyl;

	int *itas,*itas2,ntas,nbcyl,iok;

	double *val,valout,valout2,*val2;

	int *pointx,*pointy,*pointz,*pointx2,*pointy2,

	*pointz2,npt2,iout,iout2,ix,iy,iz,x,y,z,x0,y0,z0,dx,dy,dz;

	int a,b,c,nbptint,crecyl,nb,n,ii,a1,b1,c1,a2,b2,c2;

	FILE *ptficyl;



	ptsquel = lecima(imsquel,dimx,dimy,dimz,tentete);

	/* printf("\n fin de lecima "); */

	/* comptage du nb de points du squelette */

	nbsquel = 0;

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	if(ptsquel->tab[i][j][k] != 0)nbsquel++;

	printf("\n nb de points du squelette = %d",nbsquel);



	nmaxcyl = nbsquel/2 + 1;

	printf("\n nb maximum de cylindres = %d",nmaxcyl);



	tabcyl = (cylindre**)mycalloc(nmaxcyl,sizeof(cylindre*));

	cylind = (cylindre*)mymalloc(sizeof(cylindre));

	for(i=0;i<nmaxcyl;i++)

	*(tabcyl + i) = (cylindre*)mymalloc(sizeof(cylindre));



	itas = (int*)mycalloc(nbsquel+1,sizeof(int));

	val = (double*)mycalloc(nbsquel+1,sizeof(double));

	itas2 = (int*)mycalloc(28,sizeof(int));

	val2 = (double*)mycalloc(28,sizeof(double));

	pointx = (int*)mycalloc(nbsquel,sizeof(int));

	pointy = (int*)mycalloc(nbsquel,sizeof(int));

	pointz = (int*)mycalloc(nbsquel,sizeof(int));

	pointx2 = (int*)mycalloc(27,sizeof(int));

	pointy2 = (int*)mycalloc(27,sizeof(int));

	pointz2 = (int*)mycalloc(27,sizeof(int));

	ntas = 1;

	/* printf("\n fin des allocations"); */

	for(i=0;i<dimx;i++)

	for(j=0;j<dimy;j++)

	for(k=0;k<dimz;k++)

	if(ptsquel->tab[i][j][k] != 0)

	{

		itas[ntas] = ntas-1;

		pointx[ntas-1] = i;

		pointy[ntas-1] = j;

		pointz[ntas-1] = k;

		val[ntas] = (double)(-(ptsquel->tab[i][j][k]));

		ntas++;

	}

	cretas(val,itas,nbsquel);

	/* printf("\n fin de la creation du tas principal"); */

	n = nbsquel;

	nbcyl = 0;

	/* printf("\n nb de points du squelette = %d",nbsquel); */

	for(ii=0;ii<nbsquel;ii++)

	{

		/* printf("\n ii = %d",ii); */

		outas(val,itas,&n,&iout,&valout);

		/* printf("\n n = %d iout = %d valout = %f",n,iout,valout);*/

		i = pointx[iout];

		j = pointy[iout];

		k = pointz[iout];

		/* printf("\n i = %d j = %d k = %d",i,j,k); */

		/* on regarde si le point n'est pas deja dans un cylindre */

		if(ptsquel->tab[i][j][k] != -1)

		{



			/* on selectionne le voisin dont le rayon est le plus

			grand et qui n'est pas deja dans un cylindre */



			/* tri des voisins non inclus dans un cylindre existant

			en fonction de la valeur du rayon */

			/* printf("\n ptsquel = %d",ptsquel->tab[i][j][k]); */

			printf("\n i = %d j = %d k = %d rayon = %d ",i,j,k,ptsquel->tab[i][j][k]);

			npt2 = 0;

			for(ix=i-1;ix<=i+1;ix++)

			for(iy=j-1;iy<=j+1;iy++)

			for(iz=k-1;iz<=k+1;iz++)

			if(ix != i || iy != j || iz != k)

			if(ptsquel->tab[ix][iy][iz] > 0)

			{

				pointx2[npt2] = ix;

				pointy2[npt2] = iy;

				pointz2[npt2] = iz;

				itas2[npt2+1] = npt2;

				val2[npt2+1] = (double)(-ptsquel->tab[ix][iy][iz]);

				printf("\n ix = %d iy = %d iz = %d rayon = %f ",ix,iy,iz,

				-val2[npt2+1]);

				/* printf("\n val2 = %f",val2[npt2+1]); */

				npt2++;

			}

			/* printf("\n npt2 = %d",npt2); */

			cretas(val2,itas2,npt2);

			crecyl = 0;

			/* printf("\n fin de la creation du tas secondaire"); */

			while(crecyl != 1 && npt2 != 0)

			{

				outas(val2,itas2,&npt2,&iout2,&valout2);

				/* printf("\n npt2 = %d iout2 = %d valout2 = %f",npt2,iout2,valout2);*/



				/* le point pointe par iout2 est voisin

				du point (x,y,z) pointe par iout et est

				un point du squelette non inclu dans un

				cylindre deja forme.

				On verifie maintenant que le cylindre

				iout-iout2 a une intersection vide avec

				les cylindres deja formes */

				printf("\n x1 = %d y1 = %d z1 = %d rayon = %f",pointx2[iout2],

				pointy2[iout2],pointz2[iout2],-valout2);

				cylind->x1 = pointx2[iout2];

				cylind->y1 = pointy2[iout2];

				cylind->z1 = pointz2[iout2];

				cylind->x2 = i;

				cylind->y2 = j;

				cylind->z2 = k;

				cylind->rayon = (-valout2-valout)/2;

				/* printf("\n rayon = %f",cylind->rayon); */

				boitecyl(cylind,&x0,&y0,&z0,&dx,&dy,&dz);

				if(x0 < 0)x0=0;

				if(y0 < 0)y0=0;

				if(z0 < 0)z0=0;

				a = x0 + dx;

				b = y0 + dy;

				c = z0 + dz;

				if(a >= dimx)a=dimx-1;

				if(b >= dimy)b=dimy-1;

				if(c >= dimz)c=dimz-1;

				nbptint = 0;

				for(x=x0;x <= a;x++)

				for(y=y0;y <= b;y++)

				for(z=z0;z <= c;z++)

				{

					if(appcyl(cylind,x,y,z) == 1)

					if(ptsquel->tab[x][y][z] == -1)

					nbptint++;

				}

				printf("\n nbpint = %d",nbptint);

				/* if(nbptint != 0)printf("\n nbptint = %d",nbptint); */

				if(nbptint == 0)

				crecyl = 1;

			}

			if(crecyl == 1)

			{

				/* on cree un cylindre pointe par cylind */

				/* printf("\n nb de cylindres = %d",nbcyl); */

				printf("\n creation du cylindre : %f %f %f %f %f %f %f",cylind->x1,

				cylind->y1,cylind->z1,cylind->x2,cylind->y2,cylind->z2,cylind->rayon);

				/* printf("%f",cylind->rayon);*/

				tabcyl[nbcyl]->x1 = cylind->x1;

				tabcyl[nbcyl]->y1 = cylind->y1;

				tabcyl[nbcyl]->z1 = cylind->z1;

				tabcyl[nbcyl]->x2 = cylind->x2;

				tabcyl[nbcyl]->y2 = cylind->y2;

				tabcyl[nbcyl]->z2 = cylind->z2;

				tabcyl[nbcyl]->rayon = cylind->rayon;

				nbcyl++;

				/* on met a -1 le cylindre cree dans l'image du squelette*/

				nb = inscylind(ptsquel,cylind,-1);

				a1 = (int)(cylind->x1);

				b1 = (int)(cylind->y1);

				c1 = (int)(cylind->z1);

				a2 = (int)(cylind->x2);

				b2 = (int)(cylind->y2);

				c2 = (int)(cylind->z2);

				ptsquel->tab[a1][b1][c1] = -1;

				ptsquel->tab[a2][b2][c2] = -1;

				fprintf(stderr,"\n taper 1 pour continuer :");

				scanf("%d",&iok);

			}



		}



	}

	printf("\n nb de cylindres crees = %d",nbcyl);

	/* le tableau tabcyl contient les cylindres initiaux */



	/* ecriture des cylindres initiaux dans le fichier des cylindres */

	printf("\n ecriture du fichier des cylindres ");

	ptficyl = fopen(ficyl,"w");

	fprintf(ptficyl,"%d",nbcyl);

	/* printf("\n rayons : "); */

	for(i = 0;i<nbcyl;i++)

	{

		/* printf("\n ecriture du cylindre : %f %f %f %f %f %f %f",tabcyl[i]->x1,tabcyl[i]->y1,tabcyl[i]->z1,tabcyl[i]->x2,tabcyl[i]->y2,tabcyl[i]->z2,tabcyl[i]->rayon);*/

		/* printf("%f",tabcyl[i]->rayon);*/

		fprintf(ptficyl,"\n %f %f %f %f %f %f %f",tabcyl[i]->x1,tabcyl[i]->y1,tabcyl[i]->z1,tabcyl[i]->x2,tabcyl[i]->y2,tabcyl[i]->z2,tabcyl[i]->rayon);

	}



	fclose(ptficyl);

}



void viscylini(char *ficyl,char *imacyl,char *entete,

int dimx,int dimy,int dimz,float ray1,float ray2)

{

	image *ptimage;

	FILE *ptficyl;

	int nbcyl,i,npoints,ncyl,ncylv;

	float x1,y1,z1,x2,y2,z2,r;

	cylindre *ptcyl;



	ptimage = creimage(dimx,dimy,dimz,1);

	ptcyl = (cylindre*)mymalloc(sizeof(cylindre));

	ptficyl = fopen(ficyl,"r");

	fscanf(ptficyl,"%d",&nbcyl);

	printf("\n nb de cylindres du fichier = %d",nbcyl);

	ncyl = 0;

	ncylv = 0;

	for(i=0;i<nbcyl;i++)

	{

		fscanf(ptficyl,"%f %f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2,&r);

		ptcyl->x1 = x1;

		ptcyl->y1 = y1;

		ptcyl->z1 = z1;

		ptcyl->x2 = x2;

		ptcyl->y2 = y2;

		ptcyl->z2 = z2;

		ptcyl->rayon = r;



		if(r >= ray1 && r <= ray2)

		{

			npoints = inscylind(ptimage,ptcyl,0);

			if(npoints >0)ncylv++;

			ncyl++;



		}

	}

	npoints = ecrimage(entete,imacyl,ptimage);

	printf("\n nb de cylindres ecrits dans l'image = %d",ncylv);



}



void viscylini2(char *ficyl,char *imacyl,char *entete,

int dimx,int dimy,int dimz,float ray1,float ray2,float lmin,float volmin)

{

	image *ptimage;

	FILE *ptficyl,*ptficyl2;

	char ficyl2[50];

	int nbcyl,i,npoints,ncyl,ncylv,iok,n;

	float x1,y1,z1,x2,y2,z2,r,d2,lminc;

	cylindre *ptcyl;



	ptimage = creimage(dimx,dimy,dimz,1);

	ptcyl = (cylindre*)mymalloc(sizeof(cylindre));

	ptficyl = fopen(ficyl,"r");

	fscanf(ptficyl,"%d",&nbcyl);

	printf("\n nb de cylindres du fichier = %d",nbcyl);

	ncyl = 0;

	ncylv = 0;

	lminc = lmin*lmin;

	printf("\n si vous voulez mettre le resultat dans un fichier de cylindres tapez 1 sinon 0 : ");

	scanf("%d",&iok);

	if(iok == 1)

	{

		printf("\n nom du fichier de sortie des cylindres : ");

		scanf("%s",ficyl2);

		ptficyl2 = fopen(ficyl2,"w");

	}



	for(i=0;i<nbcyl;i++)

	{

		fscanf(ptficyl,"%f %f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2,&r);

		ptcyl->x1 = x1;

		ptcyl->y1 = y1;

		ptcyl->z1 = z1;

		ptcyl->x2 = x2;

		ptcyl->y2 = y2;

		ptcyl->z2 = z2;

		ptcyl->rayon = r;



		if(r >= ray1 && r<= ray2)

		{

			d2 = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2);

			if(d2 >= lminc)

				if(d2*3.14116*r*r >= volmin)

				{

					npoints = inscylind(ptimage,ptcyl,0);

					if(npoints >0)ncylv++;

					ncyl++;

				}



		}

	}

	if(iok == 1)

	{

		fprintf(ptficyl2,"%d",ncyl);

		fclose(ptficyl);

		ptficyl = fopen(ficyl,"r");

		fscanf(ptficyl,"%d",&n);

		for(i=0;i<nbcyl;i++)

		{

			fscanf(ptficyl,"%f %f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2,&r);

			if(r >= ray1 && r <= ray2)

			{

				d2 = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2);

				if(d2 >= lminc)

					if(d2*3.14116*r*r >= volmin)

					{

						fprintf(ptficyl2,"\n %f %f %f %f %f %f %f",x1,y1,z1,x2,y2,z2,r);

					}

			}

		}

	}

	if(iok==1)fclose(ptficyl2);

	fclose(ptficyl);

	npoints = ecrimage(entete,imacyl,ptimage);

	printf("\n nb de cylindres ecrits dans l'image = %d",ncylv);

	printf("\n nb de cylindres = %d",ncyl);



}







void testviscylini2(void)

{

	char ima[50],ficyl[50],entete[50];

	int dimx,dimy,dimz;

	float ray1,ray2,ray3,raymax;



	printf("\n nom du fichier de cylindres : ");

	scanf("%s",ficyl);

	printf("\n nom de l'image de sortie visualisant les cylindres : ");

	scanf("%s",ima);

	printf("\n dimx, dimy, dimz :");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	printf("\n nom de l'entete : ");

	scanf("%s",entete);

	printf("\n valeur minimale des rayons = ");

	scanf("%f",&ray1);

	printf("\n valeur maximale des rayons = ");

	scanf("%f",&raymax);

	printf("\n valeur minimale des longueurs des axes = ");

	scanf("%f",&ray2);

	printf("\n valeur minimale du volume = ");

	scanf("%f",&ray3);

	viscylini2(ficyl,ima,entete,dimx,dimy,dimz,ray1,raymax,ray2,ray3);



}



void testviscylini(void)

{

	char ima[50],ficyl[50],entete[50];

	int dimx,dimy,dimz;

	float ray1,ray2;



	printf("\n nom du fichier de cylindres initiaux : ");

	scanf("%s",ficyl);

	printf("\n nom de l'image de sortie visualisant les cylindres : ");

	scanf("%s",ima);

	printf("\n dimx, dimy, dimz :");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	printf("\n nom de l'entete : ");

	scanf("%s",entete);

	printf("\n valeurs minimales et maximales des rayons : ");

	scanf("%f %f",&ray1,&ray2);

	viscylini(ficyl,ima,entete,dimx,dimy,dimz,ray1,ray2);



}



void testinitcyl(void)

{

	char ima[50],ficyl[50];

	int dimx,dimy,dimz,tentete;



	printf("\n nom du squelette avec les valeurs des rayons : ");

	scanf("%s",ima);

	printf("\n dimx, dimy, dimz :");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	printf("\n nb de caracteres de l'entete (689 ou 690 -originale.dx-) : ");

	scanf("%d",&tentete);

	printf("\n nom du fichier de cylindres resultat : ");

	scanf("%s",ficyl);

	initcyl(ima,dimx,dimy,dimz,tentete,ficyl);

}



/* prend en entree :

- un fichier contenant les points du squelette

avec le format suivant coordonn�es relles des points rayon

associe en format reel ; le nb de points du fichier est ecrit

a la fin du fichier en format entier

- le nb de points de ce fichier (entier ecrit a la fin du fichier)



 En sortie on ecrit les cylindres initiaux correspondants dans un fichier

de cylindres au format suivant : nb de cylindres, x1,y1,z1,x2,y2,z2,rayon */



void finitcyl(char *imsquel,int nbsquel,char *ficyl)

{

	FILE *ptsquel;

	cylindre **tabcyl,*cylind;

	image *ima;

	int i,j,k,nmaxcyl,dimx,dimy,dimz;

	int *itas,*itas2,ntas,nbcyl;

	double *val,valout,valout2,*val2;

	float *pointx,*pointy,*pointz,*pointx2,*pointy2,*pointz2;

	float x,y,z,r,xv,yv,zv;

	int npt2,iout,iout2,ix,x0,y0,z0,dx,dy,dz;

	int a,b,c,nbptint,crecyl,nb,n,ii,a1,b1,c1,a2,b2,c2,xx,yy,zz;

	FILE *ptficyl;

	int nmaxv;



	nmaxcyl = nbsquel/2 + 1;

	nmaxv = 100;



	tabcyl = (cylindre**)mycalloc(nmaxcyl,sizeof(cylindre*));

	cylind = (cylindre*)mymalloc(sizeof(cylindre));

	for(i=0;i<nmaxcyl;i++)

	*(tabcyl + i) = (cylindre*)mymalloc(sizeof(cylindre));

	itas = (int*)mycalloc(nbsquel+1,sizeof(int));

	val = (double*)mycalloc(nbsquel+1,sizeof(double));

	itas2 = (int*)mycalloc(nmaxv+1,sizeof(int));

	val2 = (double*)mycalloc(nmaxv+1,sizeof(double));

	pointx = (float*)mycalloc(nbsquel,sizeof(int));

	pointy = (float*)mycalloc(nbsquel,sizeof(int));

	pointz = (float*)mycalloc(nbsquel,sizeof(int));

	pointx2 = (float*)mycalloc(nmaxv,sizeof(int));

	pointy2 = (float*)mycalloc(nmaxv,sizeof(int));

	pointz2 = (float*)mycalloc(nmaxv,sizeof(int));

	ntas = 0;



	/* printf("\n fin des allocations"); */

	ptsquel = fopen(imsquel,"r");

	dimx = 0;

	dimy = 0;

	dimz = 0;

	for(i=0;i<nbsquel;i++)

	{

		ntas++;

		fscanf(ptsquel,"%f %f %f %f",&x,&y,&z,&r);

		itas[ntas] = ntas-1;

		pointx[ntas-1] = x;

		pointy[ntas-1] = y;

		pointz[ntas-1] = z;

		val[ntas] = -r;

		if(dimx < x+r)dimx=(int)ceil(x+r);

		if(dimy < y+r)dimy=(int)ceil(y+r);

		if(dimz < z+r)dimz=(int)ceil(z+r);

	}

	ima = creimage(dimx,dimy,dimz,0);

	/* printf("\n dimx = %d dimy = %d dimz = %d",dimx,dimy,dimz);*/



	cretas(val,itas,nbsquel);

	/* printf("\n fin de la creation du tas principal"); */

	n = nbsquel;

	nbcyl = 0;

	/* printf("\n nb de points du squelette = %d",nbsquel); */

	for(ii=0;ii<nbsquel;ii++)

	{

		/* printf("\n ii = %d",ii); */

		outas(val,itas,&n,&iout,&valout);

		/* printf("\n n = %d iout = %d valout = %f",n,iout,valout);*/

		x = pointx[iout];

		y = pointy[iout];

		z = pointz[iout];

		r = -valout;

		/* printf("\n x = %f y = %f z = %f",x,y,z); */

		/* on regarde si le point n'est pas deja dans un cylindre */

		/* deja = 0;

		num = 0;

		while(deja == 0 && num < nbcyl)

		{

			if(appcyl(tabcyl[num],x,y,z) == 1)deja=1;

			else num++;

		}*/

		i = neari(x);

		j = neari(y);

		k = neari(z);

		if(ima->tab[i][j][k] != -1)

		{



			/* on selectionne le voisin dont le rayon est le plus

			grand et qui n'est pas deja dans un cylindre */



			/* tri des voisins non inclus dans un cylindre existant

			en fonction de la valeur du rayon */

			/* printf("\n ptsquel = %d",ptsquel->tab[i][j][k]); */

			/* printf("\n x = %f y = %f z = %f rayon = %f ",x,y,z,r);*/

			npt2 = 0;

			for(ix = 0;ix <n;ix++)

			{

				xv = pointx[itas[ix+1]];

				yv = pointy[itas[ix+1]];

				zv = pointz[itas[ix+1]];

				if((x-xv)*(x-xv)+(y-yv)*(y-yv)+(z-zv)*(z-zv) <= 1.5

				&& (x!=xv || y!=yv || z!=zv) &&

				(x-xv)*(x-xv)+(y-yv)*(y-yv)+(z-zv)*(z-zv) >= 1)

				{

					if(npt2 >= nmaxv)exit(66);

					pointx2[npt2] = xv;

					pointy2[npt2] = yv;

					pointz2[npt2] = zv;

					itas2[npt2+1] = npt2;

					val2[npt2+1] = val[ix+1];

					npt2++;

				}



			}

			cretas(val2,itas2,npt2);

			crecyl = 0;

			/* printf("\n fin de la creation du tas secondaire"); */

			while(crecyl != 1 && npt2 != 0)

			{

				outas(val2,itas2,&npt2,&iout2,&valout2);

				/* printf("\n npt2 = %d iout2 = %d valout2 = %f",npt2,iout2,valout2);*/



				/* le point pointe par iout2 est voisin

				du point (x,y,z) pointe par iout et est

				un point du squelette non inclu dans un

				cylindre deja forme.

				On verifie maintenant que le cylindre

				iout-iout2 a une intersection vide avec

				les cylindres deja formes */

				/* printf("\n x1 = %f y1 = %f z1 = %f rayon = %f",pointx2[iout2],

				pointy2[iout2],pointz2[iout2],-valout2); */

				cylind->x1 = pointx2[iout2];

				cylind->y1 = pointy2[iout2];

				cylind->z1 = pointz2[iout2];

				cylind->x2 = x;

				cylind->y2 = y;

				cylind->z2 = z;

				cylind->rayon = (-valout2-valout)/2;

				/* printf("\n rayon = %f",cylind->rayon);*/

				boitecyl(cylind,&x0,&y0,&z0,&dx,&dy,&dz);

				if(x0 < 0)x0=0;

				if(y0 < 0)y0=0;

				if(z0 < 0)z0=0;

				a = x0 + dx;

				b = y0 + dy;

				c = z0 + dz;

				if(a >= dimx)a=dimx-1;

				if(b >= dimy)b=dimy-1;

				if(c >= dimz)c=dimz-1;

				nbptint = 0;

				for(xx=x0;xx <= a;xx++)

				for(yy=y0;yy <= b;yy++)

				for(zz=z0;zz <= c;zz++)

				{

					if(appcyl(cylind,xx,yy,zz) == 1)

					if(ima->tab[xx][yy][zz] == -1)

					nbptint++;

				}

				/* printf("\n nbpint = %d",nbptint); */

				/* if(nbptint != 0)printf("\n nbptint = %d",nbptint); */

				if(nbptint == 0)

				crecyl = 1;

			}

			if(crecyl == 1)

			{

				/* on cree un cylindre pointe par cylind */

				/* printf("\n nb de cylindres = %d",nbcyl); */

				if(nbcyl >= nmaxcyl)exit(55);

				tabcyl[nbcyl]->x1 = cylind->x1;

				tabcyl[nbcyl]->y1 = cylind->y1;

				tabcyl[nbcyl]->z1 = cylind->z1;

				tabcyl[nbcyl]->x2 = cylind->x2;

				tabcyl[nbcyl]->y2 = cylind->y2;

				tabcyl[nbcyl]->z2 = cylind->z2;

				tabcyl[nbcyl]->rayon = cylind->rayon;

				nbcyl++;



				/* on met a -1 le cylindre cree dans l'image du squelette*/

				/* if(cylind->rayon >= 15)printf("\n creation du cylindre : %f %f %f %f %f %f %f",cylind->x1,

				cylind->y1,cylind->z1,cylind->x2,cylind->y2,cylind->z2,cylind->rayon);*/



				nb = inscylind(ima,cylind,-1);



				/* if(cylind->rayon >= 15)

				{

					distance = sqrt((cylind->x1 - cylind->x2)*(cylind->x1 - cylind->x2)

					+ (cylind->y1 - cylind->y2)*(cylind->y1 - cylind->y2)+

					(cylind->z1 - cylind->z2)*(cylind->z1 - cylind->z2));

					printf("\n nb = %d distance = %f",nb,distance);

				}*/



				a1 = neari(cylind->x1);

				b1 = neari(cylind->y1);

				c1 = neari(cylind->z1);

				a2 = neari(cylind->x2);

				b2 = neari(cylind->y2);

				c2 = neari(cylind->z2);

				ima->tab[a1][b1][c1] = -1;

				ima->tab[a2][b2][c2] = -1;

				/* printf("\n taper 1 pour continuer :");

				scanf("%d",&iok);*/

			}



		}



	}

	printf("\n nb de cylindres crees = %d",nbcyl);

	/* le tableau tabcyl contient les cylindres initiaux */



	/* ecriture des cylindres initiaux dans le fichier des cylindres */

	printf("\n ecriture du fichier des cylindres ");

	ptficyl = fopen(ficyl,"w");

	fprintf(ptficyl,"%d",nbcyl);

	/* printf("\n rayons : "); */

	for(i = 0;i<nbcyl;i++)

	{

		/* printf("\n ecriture du cylindre : %f %f %f %f %f %f %f",tabcyl[i]->x1,tabcyl[i]->y1,tabcyl[i]->z1,tabcyl[i]->x2,tabcyl[i]->y2,tabcyl[i]->z2,tabcyl[i]->rayon);*/

		/* printf("%f",tabcyl[i]->rayon);*/

		fprintf(ptficyl,"\n %f %f %f %f %f %f %f",

		tabcyl[i]->x1,tabcyl[i]->y1,tabcyl[i]->z1,

		tabcyl[i]->x2,tabcyl[i]->y2,tabcyl[i]->z2,

		tabcyl[i]->rayon);

	}



	fclose(ptficyl);

}



/* meme chose que finitcyl mais on choisit le voisin de

rayon le plus proche et non de rayon le plus grand */

void finitcyl2(char *imsquel,int nbsquel,char *ficyl)

{

	FILE *ptsquel;

	cylindre **tabcyl,*cylind;

	image *ima;

	int i,j,k,nmaxcyl,dimx,dimy,dimz;

	int *itas,*itas2,ntas,nbcyl;

	double *val,valout,valout2,*val2;

	float *pointx,*pointy,*pointz,*pointx2,*pointy2,*pointz2;

	float x,y,z,r,xv,yv,zv;

	int npt2,iout,iout2,ix,x0,y0,z0,dx,dy,dz;

	int a,b,c,nbptint,nb,crecyl,n,ii,a1,b1,c1,a2,b2,c2,xx,yy,zz;

	FILE *ptficyl;

	int nmaxv;



	nmaxcyl = nbsquel/2 + 1;

	nmaxv = 100;



	tabcyl = (cylindre**)mycalloc(nmaxcyl,sizeof(cylindre*));

	cylind = (cylindre*)mymalloc(sizeof(cylindre));

	for(i=0;i<nmaxcyl;i++)

	*(tabcyl + i) = (cylindre*)mymalloc(sizeof(cylindre));

	itas = (int*)mycalloc(nbsquel+1,sizeof(int));

	val = (double*)mycalloc(nbsquel+1,sizeof(double));

	itas2 = (int*)mycalloc(nmaxv+1,sizeof(int));

	val2 = (double*)mycalloc(nmaxv+1,sizeof(double));

	pointx = (float*)mycalloc(nbsquel,sizeof(int));

	pointy = (float*)mycalloc(nbsquel,sizeof(int));

	pointz = (float*)mycalloc(nbsquel,sizeof(int));

	pointx2 = (float*)mycalloc(nmaxv,sizeof(int));

	pointy2 = (float*)mycalloc(nmaxv,sizeof(int));

	pointz2 = (float*)mycalloc(nmaxv,sizeof(int));

	ntas = 0;



	/* printf("\n fin des allocations"); */

	ptsquel = fopen(imsquel,"r");

	dimx = 0;

	dimy = 0;

	dimz = 0;

	for(i=0;i<nbsquel;i++)

	{

		ntas++;

		fscanf(ptsquel,"%f %f %f %f",&x,&y,&z,&r);

		itas[ntas] = ntas-1;

		pointx[ntas-1] = x;

		pointy[ntas-1] = y;

		pointz[ntas-1] = z;

		val[ntas] = -r;

		if(dimx < x+r)dimx=(int)ceil(x+r);

		if(dimy < y+r)dimy=(int)ceil(y+r);

		if(dimz < z+r)dimz=(int)ceil(z+r);

	}

	ima = creimage(dimx,dimy,dimz,0);

	/* printf("\n dimx = %d dimy = %d dimz = %d",dimx,dimy,dimz);*/



	cretas(val,itas,nbsquel);

	/* printf("\n fin de la creation du tas principal"); */

	n = nbsquel;

	nbcyl = 0;

	/* printf("\n nb de points du squelette = %d",nbsquel); */

	for(ii=0;ii<nbsquel;ii++)

	{

		/* printf("\n ii = %d",ii); */

		outas(val,itas,&n,&iout,&valout);

		/* printf("\n n = %d iout = %d valout = %f",n,iout,valout);*/

		x = pointx[iout];

		y = pointy[iout];

		z = pointz[iout];

		r = -valout;

		/* printf("\n x = %f y = %f z = %f",x,y,z); */

		/* on regarde si le point n'est pas deja dans un cylindre */

		/* deja = 0;

		num = 0;

		while(deja == 0 && num < nbcyl)

		{

			if(appcyl(tabcyl[num],x,y,z) == 1)deja=1;

			else num++;

		}*/

		i = neari(x);

		j = neari(y);

		k = neari(z);

		if(ima->tab[i][j][k] != -1)

		{



			/* on selectionne le voisin dont le rayon est le plus

			proche et qui n'est pas deja dans un cylindre */



			/* tri des voisins non inclus dans un cylindre existant

			en fonction de la valeur du rayon */

			/* printf("\n ptsquel = %d",ptsquel->tab[i][j][k]); */

			/* printf("\n x = %f y = %f z = %f rayon = %f ",x,y,z,r);*/

			npt2 = 0;

			for(ix = 0;ix <n;ix++)

			{

				xv = pointx[itas[ix+1]];

				yv = pointy[itas[ix+1]];

				zv = pointz[itas[ix+1]];

				if((x-xv)*(x-xv)+(y-yv)*(y-yv)+(z-zv)*(z-zv) <= 1.5

				&& (x!=xv || y!=yv || z!=zv) &&

				(x-xv)*(x-xv)+(y-yv)*(y-yv)+(z-zv)*(z-zv) >= 1)

				{

					if(npt2 >= nmaxv)exit(66);

					pointx2[npt2] = xv;

					pointy2[npt2] = yv;

					pointz2[npt2] = zv;

					itas2[npt2+1] = npt2;

					val2[npt2+1] = r+val[ix+1];

					if(val2[npt2+1] < 0)exit(88);

					npt2++;

				}



			}

			/* if(npt2 == 0)printf("\n pas de voisins !");

			else */ cretas(val2,itas2,npt2);

			crecyl = 0;

			/* printf("\n fin de la creation du tas secondaire"); */

			while(crecyl != 1 && npt2 != 0)

			{

				outas(val2,itas2,&npt2,&iout2,&valout2);

				/* printf("\n npt2 = %d iout2 = %d valout2 = %f",npt2,iout2,valout2);*/



				/* le point pointe par iout2 est voisin

				du point (x,y,z) pointe par iout et est

				un point du squelette non inclu dans un

				cylindre deja forme.

				On verifie maintenant que le cylindre

				iout-iout2 a une intersection vide avec

				les cylindres deja formes */

				/* printf("\n x1 = %f y1 = %f z1 = %f rayon = %f",pointx2[iout2],

				pointy2[iout2],pointz2[iout2],-valout2); */

				cylind->x1 = pointx2[iout2];

				cylind->y1 = pointy2[iout2];

				cylind->z1 = pointz2[iout2];

				cylind->x2 = x;

				cylind->y2 = y;

				cylind->z2 = z;

				cylind->rayon = (-valout2-2*valout)/2;

				/* printf("\n rayon = %f",cylind->rayon);*/

				boitecyl(cylind,&x0,&y0,&z0,&dx,&dy,&dz);

				if(x0 < 0)x0=0;

				if(y0 < 0)y0=0;

				if(z0 < 0)z0=0;

				a = x0 + dx;

				b = y0 + dy;

				c = z0 + dz;

				if(a >= dimx)a=dimx-1;

				if(b >= dimy)b=dimy-1;

				if(c >= dimz)c=dimz-1;

				nbptint = 0;

				for(xx=x0;xx <= a;xx++)

				for(yy=y0;yy <= b;yy++)

				for(zz=z0;zz <= c;zz++)

				{

					if(appcyl(cylind,xx,yy,zz) == 1)

					if(ima->tab[xx][yy][zz] == -1)

					nbptint++;

				}

				/* printf("\n nbpint = %d",nbptint); */

				/* if(nbptint != 0)printf("\n nbptint = %d",nbptint); */

				if(nbptint == 0)

				crecyl = 1;

			}

			if(crecyl == 1)

			{

				/* on cree un cylindre pointe par cylind */

				/* printf("\n nb de cylindres = %d",nbcyl); */

				if(nbcyl >= nmaxcyl)exit(55);

				tabcyl[nbcyl]->x1 = cylind->x1;

				tabcyl[nbcyl]->y1 = cylind->y1;

				tabcyl[nbcyl]->z1 = cylind->z1;

				tabcyl[nbcyl]->x2 = cylind->x2;

				tabcyl[nbcyl]->y2 = cylind->y2;

				tabcyl[nbcyl]->z2 = cylind->z2;

				tabcyl[nbcyl]->rayon = cylind->rayon;

				nbcyl++;

				/* on met a -1 le cylindre cree dans l'image du squelette*/

				/* if(cylind->rayon >= 15)printf("\n creation du cylindre : %f %f %f %f %f %f %f",cylind->x1,

				cylind->y1,cylind->z1,cylind->x2,cylind->y2,cylind->z2,cylind->rayon);*/



				nb = inscylind(ima,cylind,-1);



				/* if(cylind->rayon >= 15)

				{

					distance = sqrt((cylind->x1 - cylind->x2)*(cylind->x1 - cylind->x2)

					+ (cylind->y1 - cylind->y2)*(cylind->y1 - cylind->y2)+

					(cylind->z1 - cylind->z2)*(cylind->z1 - cylind->z2));

					printf("\n nb = %d distance = %f",nb,distance);

				}*/

				a1 = neari(cylind->x1);

				b1 = neari(cylind->y1);

				c1 = neari(cylind->z1);

				a2 = neari(cylind->x2);

				b2 = neari(cylind->y2);

				c2 = neari(cylind->z2);

				ima->tab[a1][b1][c1] = -1;

				ima->tab[a2][b2][c2] = -1;

				/* printf("\n taper 1 pour continuer :");

				scanf("%d",&iok);*/

			}



		}



	}

	printf("\n nb de cylindres crees = %d",nbcyl);

	/* le tableau tabcyl contient les cylindres initiaux */



	/* ecriture des cylindres initiaux dans le fichier des cylindres */

	printf("\n ecriture du fichier des cylindres ");

	ptficyl = fopen(ficyl,"w");

	fprintf(ptficyl,"%d",nbcyl);

	/* printf("\n rayons : "); */

	for(i = 0;i<nbcyl;i++)

	{

		/* printf("\n ecriture du cylindre : %f %f %f %f %f %f %f",tabcyl[i]->x1,tabcyl[i]->y1,tabcyl[i]->z1,tabcyl[i]->x2,tabcyl[i]->y2,tabcyl[i]->z2,tabcyl[i]->rayon);*/

		/* printf("%f",tabcyl[i]->rayon);*/

		fprintf(ptficyl,"\n %f %f %f %f %f %f %f",

		tabcyl[i]->x1,tabcyl[i]->y1,tabcyl[i]->z1,

		tabcyl[i]->x2,tabcyl[i]->y2,tabcyl[i]->z2,

		tabcyl[i]->rayon);

	}



	fclose(ptficyl);

}







/* meme chose que finitcyl3 mais avec test de la continuite

des orientations des cylindres */



void finitcyl4(char *imsquel,int nbsquel,char *ficyl,float scal,float cosang)

{

	FILE *ptsquel;

	cylindre **tabcyl,*cylind;

	image *ima;

	int i,j,k,nmaxcyl,dimx,dimy,dimz;

	int *itas,*itas2,ntas,nbcyl;

	double *val,valout,valout2,*val2;

	float *pointx,*pointy,*pointz,*pointx2,*pointy2,*pointz2;

	float *pointu,*pointv,*pointw;

	float x,y,z,r,xv,yv,zv;

	int npt2,iout,iout2,ix,x0,y0,z0,dx,dy,dz;

	int a,b,c,nbptint,crecyl,n,ii,a1,b1,c1,a2,b2,c2,xx,yy,zz;

	FILE *ptficyl;

	int nmaxv,nb;

	float u,v,w,sca1,sca2,u1,v1,w1,norm1,u2,v2,w2;

	float dv1,dv2,dv3,x1v,y1v,z1v,scala,norm3;

	int ndv,itamp;



	nmaxcyl = nbsquel/2 + 1;

	nmaxv = 100;



	tabcyl = (cylindre**)mycalloc(nmaxcyl,sizeof(cylindre*));

	cylind = (cylindre*)mymalloc(sizeof(cylindre));

	for(i=0;i<nmaxcyl;i++)

	*(tabcyl + i) = (cylindre*)mymalloc(sizeof(cylindre));

	itas = (int*)mycalloc(nbsquel+1,sizeof(int));

	val = (double*)mycalloc(nbsquel+1,sizeof(double));

	itas2 = (int*)mycalloc(nmaxv+1,sizeof(int));

	val2 = (double*)mycalloc(nmaxv+1,sizeof(double));

	pointx = (float*)mycalloc(nbsquel,sizeof(int));

	pointy = (float*)mycalloc(nbsquel,sizeof(int));

	pointz = (float*)mycalloc(nbsquel,sizeof(int));

	pointu = (float*)mycalloc(nbsquel,sizeof(int));

	pointv = (float*)mycalloc(nbsquel,sizeof(int));

	pointw = (float*)mycalloc(nbsquel,sizeof(int));

	pointx2 = (float*)mycalloc(nmaxv,sizeof(int));

	pointy2 = (float*)mycalloc(nmaxv,sizeof(int));

	pointz2 = (float*)mycalloc(nmaxv,sizeof(int));

	ntas = 0;



	/* printf("\n fin des allocations"); */

	ptsquel = fopen(imsquel,"r");

	dimx = 0;

	dimy = 0;

	dimz = 0;

	for(i=0;i<nbsquel;i++)

	{

		ntas++;

		fscanf(ptsquel,"%f %f %f %f %f %f %f",&x,&y,&z,&r,&u,&v,&w);

		itas[ntas] = ntas-1;

		pointx[ntas-1] = x;

		pointy[ntas-1] = y;

		pointz[ntas-1] = z;

		val[ntas] = -r;

		pointu[ntas-1] = u;

		pointv[ntas-1] = v;

		pointw[ntas-1] = w;

		if(dimx < x+r)dimx=(int)ceil(x+r);

		if(dimy < y+r)dimy=(int)ceil(y+r);

		if(dimz < z+r)dimz=(int)ceil(z+r);

	}

	ima = creimage(dimx,dimy,dimz,0);



	cretas(val,itas,nbsquel);

	n = nbsquel;

	nbcyl = 0;

	for(ii=0;ii<nbsquel;ii++)

	{

		outas(val,itas,&n,&iout,&valout);

		x = pointx[iout];

		y = pointy[iout];

		z = pointz[iout];

		u = pointu[iout];

		v = pointv[iout];

		w = pointw[iout];

		r = -valout;

		i = neari(x);

		j = neari(y);

		k = neari(z);

		if(ima->tab[i][j][k] == 0)

		{



			/* on selectionne le voisin dont le rayon est le plus

			proche et qui n'est pas deja dans un cylindre */



			/* tri des voisins non inclus dans un cylindre existant

			en fonction de la valeur du rayon */

			/* on met dans le tas uniquement les voisins dans le plan perpendiculaire

				au rayon defini par le tetraedre */

			npt2 = 0;

			dv1 = 0;

			dv2 = 0;

			dv3 = 0;

			ndv = 0;

			for(ix = 0;ix <n;ix++)

			{

				xv = pointx[itas[ix+1]];

				yv = pointy[itas[ix+1]];

				zv = pointz[itas[ix+1]];

				norm1 = sqrt((x-xv)*(x-xv)+(y-yv)*(y-yv)+(z-zv)*(z-zv));



				if(norm1 <= 1.5  && (x!=xv || y!=yv || z!=zv))

				{

					itamp = ima->tab[neari(xv)][neari(yv)][neari(zv)];

					if(itamp != 0)

					{

						x1v = tabcyl[-itamp - 1]->x1 - tabcyl[-itamp - 1]->x2 ;

						y1v = tabcyl[-itamp - 1]->y1 - tabcyl[-itamp - 1]->y2 ;

						z1v = tabcyl[-itamp - 1]->z1 - tabcyl[-itamp - 1]->z2 ;

						if(z1v < 0)

						{

							x1v = -x1v;

							y1v = -y1v;

							z1v = -z1v;

						}else if(z1v == 0)

						if(y1v < 0)

						{

							x1v = -x1v;

							y1v = -y1v;

							z1v = -z1v;

						}else if(y1v == 0)

						if(x1v < 0)

						{

							x1v = -x1v;

							y1v = -y1v;

							z1v = -z1v;

						}

						ndv++;

						dv1 = dv1 + x1v;

						dv2 = dv2 + y1v;

						dv3 = dv3 + z1v;

					}

				}

				if(norm1 <= 1.5 && (x!=xv || y!=yv || z!=zv) && norm1 >= 1)

				{

					/* on teste l'orientation du cylindre

					par rapport a l'orientation des deux tetraedres */

					u1 = (x-xv)/norm1;

					v1 =(y-yv)/norm1;

					w1 = (z-zv)/norm1;

					u2 = pointu[itas[ix+1]];

					v2 = pointv[itas[ix+1]];

					w2 = pointw[itas[ix+1]];

					sca1 = u1*u + v1*v + w1*w;

					sca2 = u1*u2 + v1*v2 + w1*w2;

					if(fabs(sca1) <= scal && fabs(sca2) <= scal)

					{

						if(npt2 >= nmaxv)exit(66);

						pointx2[npt2] = xv;

						pointy2[npt2] = yv;

						pointz2[npt2] = zv;

						itas2[npt2+1] = npt2;

						val2[npt2+1] = r+val[ix+1];

						if(val2[npt2+1] < 0)exit(88);

						npt2++;

					}

				}



			}

			if(ndv != 0)

			{

				dv1 = dv1/ndv;

				dv2 = dv2/ndv;

				dv3 = dv3/ndv;

				norm3 = sqrt(dv1*dv1 + dv2*dv2 + dv3*dv3);

				dv1 = dv1/norm3;

				dv2 = dv2/norm3;

				dv3 = dv3/norm3;

			}

			cretas(val2,itas2,npt2);

			crecyl = 0;

			while(crecyl != 1 && npt2 != 0)

			{

				outas(val2,itas2,&npt2,&iout2,&valout2);



				/* le point pointe par iout2 est voisin

				du point (x,y,z) pointe par iout et est

				un point du squelette non inclu dans un

				cylindre deja forme.

				On verifie maintenant que le cylindre

				iout-iout2 a une intersection vide avec

				les cylindres deja formes */



				cylind->x1 = pointx2[iout2];

				cylind->y1 = pointy2[iout2];

				cylind->z1 = pointz2[iout2];

				cylind->x2 = x;

				cylind->y2 = y;

				cylind->z2 = z;

				cylind->rayon = (-valout2-2*valout)/2;

				/* verification de la compatibilite de l'orientation

				du cylindre avec les cylindres vosins */

				scala = 0;

				if(ndv != 0)

				{

					norm3 = sqrt((cylind->x1-cylind->x2)*(cylind->x1-cylind->x2)

					+(cylind->y1-cylind->y2)*(cylind->y1-cylind->y2)+

					(cylind->z1-cylind->z2)*(cylind->z1-cylind->z2));

					scala = (dv1*(cylind->x1-cylind->x2)+dv2*(cylind->y1-cylind->y2)

					+dv3*(cylind->z1-cylind->z2))/norm3;

				}

				if(ndv==0 || ((ndv != 0)&&fabs(scala) >= cosang))

				{

					boitecyl(cylind,&x0,&y0,&z0,&dx,&dy,&dz);

					if(x0 < 0)x0=0;

					if(y0 < 0)y0=0;

					if(z0 < 0)z0=0;

					a = x0 + dx;

					b = y0 + dy;

					c = z0 + dz;

					if(a >= dimx)a=dimx-1;

					if(b >= dimy)b=dimy-1;

					if(c >= dimz)c=dimz-1;

					nbptint = 0;

					for(xx=x0;xx <= a;xx++)

					for(yy=y0;yy <= b;yy++)

					for(zz=z0;zz <= c;zz++)

					{

						if(appcyl(cylind,xx,yy,zz) == 1)

						if(ima->tab[xx][yy][zz] < 0)

						nbptint++;

					}

					if(nbptint == 0)crecyl=1;

				}

			}

			if(crecyl == 1)

			{

				/* on cree un cylindre pointe par cylind */

				if(nbcyl >= nmaxcyl)exit(55);

				tabcyl[nbcyl]->x1 = cylind->x1;

				tabcyl[nbcyl]->y1 = cylind->y1;

				tabcyl[nbcyl]->z1 = cylind->z1;

				tabcyl[nbcyl]->x2 = cylind->x2;

				tabcyl[nbcyl]->y2 = cylind->y2;

				tabcyl[nbcyl]->z2 = cylind->z2;

				tabcyl[nbcyl]->rayon = cylind->rayon;

				/* on met a -(place dans tabcyl +1) le cylindre cree dans l'image du squelette*/

				nb = inscylind(ima,cylind,-(nbcyl+1));

				a1 = neari(cylind->x1);

				b1 = neari(cylind->y1);

				c1 = neari(cylind->z1);

				a2 = neari(cylind->x2);

				b2 = neari(cylind->y2);

				c2 = neari(cylind->z2);

				ima->tab[a1][b1][c1] = -(nbcyl+1);

				ima->tab[a2][b2][c2] = -(nbcyl+1);

				nbcyl++;

			}



		}



	}

	printf("\n nb de cylindres crees = %d",nbcyl);

	/* le tableau tabcyl contient les cylindres initiaux */



	/* ecriture des cylindres initiaux dans le fichier des cylindres */

	printf("\n ecriture du fichier des cylindres ");

	ptficyl = fopen(ficyl,"w");

	fprintf(ptficyl,"%d",nbcyl);

	for(i = 0;i<nbcyl;i++)

	{

		fprintf(ptficyl,"\n %f %f %f %f %f %f %f",

		tabcyl[i]->x1,tabcyl[i]->y1,tabcyl[i]->z1,

		tabcyl[i]->x2,tabcyl[i]->y2,tabcyl[i]->z2,

		tabcyl[i]->rayon);

	}



	fclose(ptficyl);

}



void testfinitcyl(void)

{

	char ficyl[50],fsquel[50];

	int nsquel;



	printf("\n nom du fichier contenant le squelette avec les valeurs des rayons : ");

	scanf("%s",fsquel);

	printf("\n nb de points du squelette = ");

	scanf("%d",&nsquel);

	printf("\n nom du fichier de cylindres resultat : ");

	scanf("%s",ficyl);

	finitcyl(fsquel,nsquel,ficyl);

}



void testfinitcyl2(void)

{

	char ficyl[50],fsquel[50];

	int nsquel;



	printf("\n nom du fichier contenant le squelette avec les valeurs des rayons : ");

	scanf("%s",fsquel);

	printf("\n nb de points du squelette = ");

	scanf("%d",&nsquel);

	printf("\n nom du fichier de cylindres resultat : ");

	scanf("%s",ficyl);

	finitcyl2(fsquel,nsquel,ficyl);

}



void testfinitcyl3(void)

{

	char ficyl[50],fsquel[50];

	int nsquel;

	float scal;



	printf("\n nom du fichier contenant le squelette avec les valeurs des rayons et l'orientation : ");

	scanf("%s",fsquel);

	printf("\n nb de points du squelette = ");

	scanf("%d",&nsquel);

	printf("\n nom du fichier de cylindres resultat : ");

	scanf("%s",ficyl);

	printf("\n valeur maximale pour la valeur absolue du produit scalaire (0.2 raisonnable)= ");

	scanf("%f",&scal);

	finitcyl3(fsquel,nsquel,ficyl,scal);

}



void testfinitcyl4(void)

{

	char ficyl[50],fsquel[50];

	int nsquel;

	float scal,cosang;



	printf("\n nom du fichier contenant le squelette avec les valeurs des rayons et l'orientation : ");

	scanf("%s",fsquel);

	printf("\n nb de points du squelette = ");

	scanf("%d",&nsquel);

	printf("\n nom du fichier de cylindres resultat : ");

	scanf("%s",ficyl);

	printf("\n valeur maximale pour la valeur absolue du cosinus de l'angle entre le cylindre et les tetraedres (0.2 raisonnable) = ");

	scanf("%f",&scal);

	printf("\n valeur minimale pour la valeur absolue du cosinus de l'angle entre 2 cylindres voisins (0.9 raisonnable) = ");

	scanf("%f",&cosang);

	finitcyl4(fsquel,nsquel,ficyl,scal,cosang);

}

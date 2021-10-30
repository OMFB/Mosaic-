/*
 * finitcyl3.c
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



/* meme chose que finitcyl2 mais on utlise aussi l'orientation du teraedre

associe au point du squelette */



void finitcyl3(char *imsquel,int nbsquel,char *ficyl,float scal)

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

	int nmaxv,nb,trav;

	float u,v,w,sca1,sca2,u1,v1,w1,norm1,u2,v2,w2;



	nmaxcyl = nbsquel/5 + 1;

	nmaxv = 50;



	tabcyl = (cylindre**)mycalloc(nmaxcyl,sizeof(cylindre*));

	fprintf(stderr,"\n $");

	cylind = (cylindre*)mymalloc(sizeof(cylindre));

	fprintf(stderr,"\n $");

	for(i=0;i<nmaxcyl;i++)

	*(tabcyl + i) = (cylindre*)mymalloc(sizeof(cylindre));

	fprintf(stderr,"\n $");

	itas = (int*)mycalloc(nbsquel+1,sizeof(int));

	fprintf(stderr,"\n $");

	val = (double*)mycalloc(nbsquel+1,sizeof(double));

	fprintf(stderr,"\n $");

	itas2 = (int*)mycalloc(nmaxv+1,sizeof(int));

	fprintf(stderr,"\n $");

	val2 = (double*)mycalloc(nmaxv+1,sizeof(double));

	fprintf(stderr,"\n $");

	pointx = (float*)mycalloc(nbsquel,sizeof(float));

	fprintf(stderr,"\n $");

	pointy = (float*)mycalloc(nbsquel,sizeof(float));

	fprintf(stderr,"\n $");

	pointz = (float*)mycalloc(nbsquel,sizeof(float));

	fprintf(stderr,"\n $");

	pointu = (float*)mycalloc(nbsquel,sizeof(float));

	fprintf(stderr,"\n $");

	pointv = (float*)mycalloc(nbsquel,sizeof(float));

	fprintf(stderr,"\n $");

	pointw = (float*)mycalloc(nbsquel,sizeof(float));

	fprintf(stderr,"\n $");

	pointx2 = (float*)mycalloc(nmaxv,sizeof(float));

	fprintf(stderr,"\n $");

	pointy2 = (float*)mycalloc(nmaxv,sizeof(float));

	fprintf(stderr,"\n $");

	pointz2 = (float*)mycalloc(nmaxv,sizeof(float));

	fprintf(stderr,"\n $");

	ntas = 0;

	fprintf(stderr,"\n fin des allocations");



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

		if((float)dimx < x+r)dimx=(int)ceil(x+r);

		if((float)dimy < y+r)dimy=(int)ceil(y+r);

		if((float)dimz < z+r)dimz=(int)ceil(z+r);

	}

	fprintf(stderr,"\n dimx=%d dimy=%d dimz=%d",dimx,dimy,dimz);

	ima = creimage(dimx,dimy,dimz,0);

	fprintf(stderr,"\n sortie de creimage");

	cretas(val,itas,nbsquel);

	fprintf(stderr,"\n sortie de cretas");

	n = nbsquel;

	nbcyl = 0;

	trav=1;

	for(ii=0;ii<nbsquel;ii++)

	{

		if(trav < 10000)trav++;

		else

		{

			trav=1;

			fprintf(stderr,"\n ii=%d nbcyl=%d",ii,nbcyl);

		}

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

		if(ima->tab[i][j][k] != -1)

		{

			/* on selectionne le voisin dont le rayon est le plus

			proche et qui n'est pas deja dans un cylindre */



			/* tri des voisins non inclus dans un cylindre existant

			en fonction de la valeur du rayon */

			/* on met dans le tas uniquement les voisins dans le plan perpendiculaire

				au rayon defini par le tetraedre */

			npt2 = 0;

			for(ix = 0;ix <n;ix++)

			{

				xv = pointx[itas[ix+1]];

				yv = pointy[itas[ix+1]];

				zv = pointz[itas[ix+1]];

				norm1 = sqrt((x-xv)*(x-xv)+(y-yv)*(y-yv)+(z-zv)*(z-zv));

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

			if(npt2 >= nmaxv)exit(66);

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

				if(iout2 >= nmaxv)exit(45);

				cylind->x1 = pointx2[iout2];

				cylind->y1 = pointy2[iout2];

				cylind->z1 = pointz2[iout2];

				cylind->x2 = x;

				cylind->y2 = y;

				cylind->z2 = z;

				cylind->rayon = (-valout2-2*valout)/2;

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

				if(nbptint == 0)

				crecyl = 1;

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

				nbcyl++;

				/* on met a -1 le cylindre cree dans l'image du squelette*/



				nb = inscylind(ima,cylind,-1);



				a1 = neari(cylind->x1);

				b1 = neari(cylind->y1);

				c1 = neari(cylind->z1);

				a2 = neari(cylind->x2);

				b2 = neari(cylind->y2);

				c2 = neari(cylind->z2);

				ima->tab[a1][b1][c1] = -1;

				ima->tab[a2][b2][c2] = -1;

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




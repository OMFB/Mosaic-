#include <stdio.h>

#include <errno.h>

#include <stdlib.h>

#include <unistd.h>  /* unlink */
/* NEW */
#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include "geometrie.h"

void selecboules4(char *ficboul,char *ficboulm,float seuil,int iopt)

{

	FILE *ptficboul,*ptficboulm;

	int nboules,nbm,i,j,sel,ind;

	boule **boules,**boulesm;

	float xc,yc,zc,r,a,u,v,w,zero;



	ptficboul = fopen(ficboul,"r");

	ptficboulm = fopen(ficboulm,"w");

	fscanf(ptficboul,"%d",&nboules);

	boules = (boule**)mycalloc(nboules,sizeof(boule*));

	boulesm = (boule**)mycalloc(nboules,sizeof(boule*));

	nbm = 0;
	zero = 0;

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

			fprintf(stderr,"\n i = %d;",i);
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

			nbm++;

			/* fprintf(stderr," %d;",nbm); */

		}

	}

	fprintf(stderr,"\n nb de boules maximales = %d",nbm);



	/* rangement des boules selectionnees dans le fichier en sortie */

	fprintf(ptficboulm,"%d",nbm);

	for(i=0;i<nbm;i++)

	if(iopt==1)

	fprintf(ptficboulm,"\n %f %f %f %f %f %f %f %f",boulesm[i]->xc,

	boulesm[i]->yc,boulesm[i]->zc,boulesm[i]->rayon,zero,zero,zero,zero);

	else if(iopt==2)

	fprintf(ptficboulm,"\n %f %f %f %f",boulesm[i]->xc,

	boulesm[i]->yc,boulesm[i]->zc,boulesm[i]->rayon);

	else

	{

		fprintf(stderr,"erreur dans selectboules4 !!");

		exit(32);

	}

	fclose(ptficboulm);

	fclose(ptficboul);



}

void selecboules5(char *ficboul,char *ficboulm,float seuil,int iopt,int dimx, int dimy, int dimz)

{

	FILE *ptficboul,*ptficboulm;

	int nboules,nbm,i,ind;

	int nptboul,nptint;

	boule *boult,**boulesm;

	image *ptsquel;

	float xc,yc,zc,r,a,u,v,w;


	ptficboul = fopen(ficboul,"r");

	ptficboulm = fopen(ficboulm,"w");

	fscanf(ptficboul,"%d",&nboules);

	fprintf(stderr,"\n nboules = %d",nboules);

	boulesm = (boule**)mycalloc(nboules,sizeof(boule*));

	nbm = 0;

	boult = (boule*)mymalloc(sizeof(boule));

	ptsquel = creimage(dimx,dimy,dimz,1);


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

		fscanf(ptficboul,"%f %f %f %f %f %f %f %f",&xc,&yc,&zc,&r,&a,&u,&v,&w);

		boult->xc = xc;

		boult->yc = yc;

		boult->zc = zc;

		boult->rayon = r;


		nptboul = boulimag2(boult,ptsquel,0,&nptint);


		if(nptboul != 0)
		{
			if( ( ((float)nptint/(float)nptboul) *100 ) < seuil)
			{
				boulesm[nbm] = (boule*)mymalloc(sizeof(boule));
				boulesm[nbm]->xc = xc;

				boulesm[nbm]->yc = yc;

				boulesm[nbm]->zc = zc;

				boulesm[nbm]->rayon = r;
				nbm++;
				nptboul = boulimag(boult,ptsquel,0);
			}
		}


	}

	fprintf(stderr,"\n nb de boules maximales = %d",nbm);


	/* rangement des boules selectionnees dans le fichier en sortie */

	fprintf(ptficboulm,"%d",nbm);
	fprintf(stderr,"\n iopt = %d",iopt);

	for(i=0;i<nbm;i++)

	if(iopt==1)

	fprintf(ptficboulm,"\n %f %f %f %f %f %f %f %f",boulesm[i]->xc,

	boulesm[i]->yc,boulesm[i]->zc,boulesm[i]->rayon,0.0,0.0,0.0,0.0);

	else if(iopt==2)

	fprintf(ptficboulm,"\n %f %f %f %f",boulesm[i]->xc,

	boulesm[i]->yc,boulesm[i]->zc,boulesm[i]->rayon);

	fclose(ptficboulm);

	fclose(ptficboul);

}





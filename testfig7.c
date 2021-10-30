/*
 * testfig7.c
 *
 *  Created on: 19 juin 2020
 *      Author: monga
 */


#include <errno.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>  /* unlink */

#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include "geometrie.h"


/* ecriture de l'image dont les valeurs sont stockees sous forme d'entiers dans le fichier de nom nomim,
les dimensions de l'image sont dimx.dimy.dimz ; l'image est stocke par plans puis lignes 
la forme est constitue des points a zero que l'on met a false dans le tableau de l'image */

imagec *fic2imagb(char *nomim, int dimx, int dimy, int dimz)

{
    int k,j,i,entier; 
    FILE *ptnomim; 
    imagec *ptimage; 

    ptnomim = fopen(nomim,"r"); 
    ptimage = creimagec(dimx,dimy,dimz,true);

	for(k=0;k<dimz;k++)

	for(j=0;j<dimy;j++)

	for(i=0;i<dimx;i++)
	{

		fscanf(ptnomim,"%d",&entier);
		if(entier == 0)
		{
			(ptimage->tab)[i][j][k] = false;
		}
        else
        {
            (ptimage->tab)[i][j][k] = true;
        }
	}

	fclose(ptnomim);
    return(ptimage);
}


void testfig7(void)

{

	char nomft[50],nomima[50],nomficbou[50];

	float x0,y0,z0,r ;

	float x1,y1,z1,x2,y2,z2;

	float x4,y4,z4,x3,y3,z3;

	int a1,b1,c1,a2,b2,c2,a3,b3,c3,a4,b4,c4;

	int dimx,dimy,dimz,nbtet,nbs;

	imagec *ptimage;

	boule *ptboule;

	int i,nin,nout;

	float **lpoints;

	int **ltet,xi,yi,zi,dx,dy,dz,ntetras,**ltetscu;

	float xm,ym,zm;

	tetra *ptetra;

	int j,k,tentete,entier,ix,iy,iz;

	float pourcentage;

	FILE *ptim,*ptficbou,*ptficbou2;

	short int *sculpt;

	float a12,a14,a13,a24,a34,a23,rap,amax,amin,s1,s2,s3,s4,smax;

	float m1m2,m1m3,m1m4,m2m3,m2m4,m3m4,xe,ye,ze,u,v,w,norme;

	int ind,nboul,boulforme,pourcent,nretir,ntscu,fts,pourc,npla;

	char tetrascu[50],trav[] = "trav";

	fprintf(stderr,"\n fichier de tetraedres : ");

	scanf("%s",nomft);

	fprintf(stderr,"\n nom du fichier image contenant la forme pleine (suite d'entiers sur 4 octets): ");

	scanf("%s",nomima);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n valeur minimale du pourcentage de points de la boule dans la forme (80 conseille) : ");

	scanf("%d",&pourcent);

	fprintf(stderr,"\n fichier de sortie des boules circonscrites incluses dnas la forme, on met en tete le nombre de boules :");

	scanf("%s",nomficbou);

	ptficbou = fopen(trav,"w");

	lfictet(nomft,&lpoints,&ltet,&nbtet,&nbs,&xm,&ym,&zm);

	fprintf(stderr,"\n nb de tetrahedres= %d  nb de sommets=%d xm=%f ym=%f zm=%f",

	nbtet,nbs,xm,ym,zm);

	/* ecriture de l'image contenue dans le fichier nomim dans l'image de boolens pointee par ptimage 

	creation d'une image binaire (de booleens) de dimensions dimx.dimy.dimz que l'on initialise à 1 */

	ptimage = fic2imagb(nomima,dimx,dimy,dimz);

	nboul = 0;
    npla = 0; 
    ptetra = (tetra*)mymalloc(sizeof(tetra));

	for(i =0;i<nbtet;i++)

	{
        if(i%10000000 == 0 && i!=0)
        {
            pourc = neari( (((float)nboul)/i)*100 ); 
            fprintf(stderr,"\n i = %d , nombre de boules = %d, pourcentage de tetraedres selectionnes %d, npla = %d",i,nboul,pourc,npla);
        }

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

        /* regarder si le tetraedre est plat c'est a dire si ses points sont coplanaires */
        if(plat(ptetra) == false)
        {
		/* calcul de la sphere cirsconcrite */

		    ptboule = tetrasphere(ptetra);

		    x0 = ptboule->xc;

		    y0 = ptboule->yc;

		    z0 = ptboule->zc;

		    r = ptboule->rayon;

		    /* on regarde si la boule est incluse dans la forme*/

		    if(bouleformb(ptboule,ptimage,pourcent) == true)
            {
                
                fprintf(ptficbou,"%f %f %f %f \n",x0,y0,z0,r);

			    nboul++;

			}

		}
        else
        {
            npla++; 
        }
	}

    /* les boules sont dans le fichier pointé par ptficbou */

	fprintf(stderr,"\n nb de boules = %d",nboul);

    ptficbou2 = fopen(nomficbou,"w");

    fprintf(ptficbou2,"%d",nboul);

    fclose(ptficbou);

    ptficbou = fopen(trav,"r");

    for(i=0;i<nboul;i++)
    {
        fscanf(ptficbou,"%f %f %f %f",&x0,&y0,&z0,&r);
        if(i == 0)fprintf(stderr,"\n %f %f %f %f",x0,y0,z0,r);
        fprintf(ptficbou2,"\n%f %f %f %f",x0,y0,z0,r);
    }
    fprintf(stderr,"\n fin boucle d'ecriture"); 
	fclose(ptficbou);
    fclose(ptficbou2); 
    remove(trav); 
    /* IL FAUDRAIT LIBERER LA PLACE OCCUPEE PAR LES TABLEAUX DECRIVANT LES TETRAEDRES SI ON N'EXECUTE D'AUTRES PROCEDURES APRES 
    TESTFIG7 */
   /*remove(ficbou); */

    /* ecriture du nombre de boules en tete du fichier */

}


/* prend en entree une boule et une image ou les points d'une forme sont etiquetees a la

valeur false ; rend en sortie true si la boule est completement incluse dans la forme

(pourcentage - 0-100- est le pourcentage de points de la boule qui doivent

etre dans la forme) et false sinon */

bool bouleformb(boule *ptboule,imagec *ptimage,int pourcentage)

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

				if(ptimage->tab[i][j][k] == true)nd++;

				else nf++;

			}

		}

		if(nd + nf == 0)

		{

			fprintf(stderr,"\n Bizarre !!");

			return(false);

		}

		else

		{

			coeff = (float)(100*nf)/(float)(nd+nf);

			if( coeff >= (float)(pourcentage))

			{

				return(true);

			}

			else return(false);

		}

	}

	else

	{

		return(false);

	}

}

bool plat(tetra *ptetra)
{

	float det;

	float x1,y1,z1,x2,y2,z2;

	float x4,y4,z4,x3,y3,z3;

    float epsilon; 

    epsilon = 0.00000000000001; 

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

	if(det <= epsilon)

	{

		return(true);

	}

	else return(false);
}

/* cette procedure trie les boules contenus dans un fichier selon les rayons */
void testtri4(void)

{

	char ficboul[50],ficboul1[50];

	fprintf(stderr,"\n nom du fichier de boules en entree format (x,y,z,r) avec nombre de boules en tete format entier (sortie de testfig7) : ");

	scanf("%s",ficboul);

	fprintf(stderr,"\n nom du fichier de boules en sortie trie selon les rayons: ");

	scanf("%s",ficboul1);

	triboules4(ficboul,ficboul1);

}


void triboules4(char *ficboul,char *ficboul1)

{

	boule **taboules;

	FILE *ptboul,*ptboul1;

	int i,*itas,n,iout,nboules;

	float xc,yc,zc,r,amin,amax,rap,u,v,w,zero;

	double *val,valout;


    ptboul = fopen(ficboul,"r");

	ptboul1 = fopen(ficboul1,"w");

    fscanf(ptboul,"%d",&nboules);

    fprintf(stderr,"\n nombre de boules = %d",nboules); 

	itas = (int*)mycalloc(nboules+1,sizeof(int));

	val = (double*)mycalloc(nboules+1,sizeof(double));

	fprintf(stderr,"\n fin de l'allocation pour le tas ");

	taboules = (boule**)mycalloc(nboules,sizeof(boule*));

	fprintf(stderr,"\n fin de l'allocation pour le tableau des boules");

	for(i=0;i<nboules;i++)
	taboules[i] = (boule*)mymalloc(sizeof(boule));

	fprintf(stderr,"\n fin des allocations");

	/* lecture de ficboul et remplissage de taboules*/

	for(i=0;i<nboules;i++)

	{


		fscanf(ptboul,"%f %f %f %f ",&xc,&yc,&zc,&r);

		taboules[i]->xc = xc;

		taboules[i]->yc = yc;

		taboules[i]->zc = zc;

		taboules[i]->rayon = r;

	}

	fprintf(stderr,"\n fin de la lecture de ficboul");


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

		fprintf(ptboul1,"\n%f %f %f %f",taboules[iout]->xc,taboules[iout]->yc,taboules[iout]->zc,taboules[iout]->rayon);

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


void selecboulesval(char *ficboul,char *ficboulm,float seuil,int dimx, int dimy, int dimz)

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


	/* selection des boules maximales */

	for(i=0;i<nboules;i++)

	{

		/* on regarde si la boule a une intersection vide avec

		les boules deja selectionnees */

		if(i % 1000000 == 0)

		{

			fprintf(stderr,"\n i = %d;",i);

			fprintf(stderr,"\n nbm = %d;",nbm); 

		}

		fscanf(ptficboul,"%f %f %f %f",&xc,&yc,&zc,&r);

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

	for(i=0;i<nbm;i++)
    {
        fprintf(ptficboulm,"\n%f %f %f %f",boulesm[i]->xc,

	    boulesm[i]->yc,boulesm[i]->zc,boulesm[i]->rayon);
    }

    fclose(ptficboulm);

	fclose(ptficboul);

}


void triboulval(void)

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

	selecboulesval(nom1,nom2,seuil,dimx,dimy,dimz);

}


/* cree un fichier image de dimensions 'dimx dimy dimz'

avec le fond a 255 et les boules du fichier "boules"(sortie de selecboulesval)

a 0, le fichier image est "imaboul" */



void visuboulval(char *boules,char *imaboul,int dimx,int dimy,int dimz)

{

	FILE *ptboules;

	int nboules,i,npoints;

	float x0,y0,z0,r,a,u,v,w;

	imagec *ptimage;

	boule *ptboule;



	ptboules = fopen(boules,"r");

	ptimage = creimagec(dimx,dimy,dimz,true);

	ptboule = (boule*)mymalloc(sizeof(boule));

	fscanf(ptboules,"%d",&nboules);

	fprintf(stderr,"\n nboules = %d",nboules);


	for(i=0;i<nboules;i++)

	{

		fscanf(ptboules,"%f %f %f %f",&x0,&y0,&z0,&r);

		ptboule->xc = x0;

		ptboule->yc = y0;

		ptboule->zc = z0;

		ptboule->rayon = r;

		npoints = boulimagb(ptboule,ptimage,false);

	}

	npoints = ecrimageval(imaboul,ptimage);

	fprintf(stderr,"\n nb total de points ecrits = %d",npoints);

}


/* insertion d'une boule dans une image, les points de la boule

sont colories avec le niveau de gris 'seuil' */

int boulimagb(boule *ptboule,imagec *ptimage,bool seuil)

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



/* ecriture d'une image d'entiers pointee par "ptimage" dans un fichier. On renvoie le nombre de points

ecrits dans le fichier ; 'nomfic2' le nom du fichier image resultat ; l'image est une image binaire de booleens ;
on met les voxels à false à zero et ceux a true a 255 */



int ecrimageval(char *nomfic2,imagec *ptimage)

{



	FILE *f2file;

	int i,j,k,nbpoints,dimx,dimy,dimz ;

	int nvide,nforme ;

    bool entier ; 



	f2file = fopen(nomfic2,"w");


	fprintf(stderr,"\n fichier image cree : %s \n",nomfic2);

	nbpoints = 0 ;

	dimx = ptimage->dimx;

	dimy = ptimage->dimy;

	dimz = ptimage->dimz;

	nvide = 0;

	nforme = 0;

	for(k=0;k<dimz;k++)
    {

	    for(j=0;j<dimy;j++)
        {

	    
            for(i=0;i<dimx;i++)
	        {

		        entier = (ptimage->tab)[i][j][k];

		        if(entier == false)
                {
                    nforme++;
                    fprintf(f2file,"%d ",0);
                }
		        else 
                {
                    nvide++;
                    fprintf(f2file,"%d ",255);
                }
                    nbpoints++;
            }
            fprintf(f2file,"\n"); 
        }
    }

	fclose(f2file);
    fprintf(stderr,"\n nombre de voxels a 0 = %d",nforme);
    fprintf(stderr,"\n nombre de voxels a 255 = %d",nvide);

	return(nbpoints);
}

void testvisuboulval(void)

{

	char boules[50],imaboul[50];

	int dimx,dimy,dimz;



	fprintf(stderr,"nom du fichier des boules (nb de boules,xc,yc,zc,r) : ");

	scanf("%s",boules);

	fprintf(stderr,"\n nom du fichier image de sortie en format lignes ecrites a la suite, niveau de gris un entier: ");

	scanf("%s",imaboul);

	fprintf(stderr,"\n %s %s",boules,imaboul);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	visuboulval(boules,imaboul,dimx,dimy,dimz);
}

#include <stdio.h>

#include "geometrie.h"

#include <errno.h>

#include <stdlib.h>

#include <unistd.h>  /* unlink */

#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include <stdbool.h>

/* routines pour la simulation de la dynamique biologique */

/* cette fonction cree le graphe des boules 
entree: nom du fichier des boules maximums, attributs des boules maximum autres que leur centres et rayons (masses, biomasse...)
sortie: fichier contenant le graphe correspondant au fichier des boules et des attributs */

void testcregraphsoilmu3d()

{
	char ficboules[50],ficatt[50],racine[50],chaine[50];
	char ficgraphe[50],ficarcs[50];
	graphe_s *graphe;
	int nbarcs,dimx,dimy,dimz,nbmaxvois;


    /* on lit le fichier des boules maximum, les attributs noeuds initiaux (fichier), les parametres du modele biologique (fichier), l'intervalle de temps total
    ainsi que la discretisation voulue (fichier) */

    fprintf(stderr,"\n racine des fichiers : ");
	scanf("%s",racine);

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(ficboules,chaine);
	fprintf(stderr,"\n fichier de boules maximales en entree (x,y,z,r,0,0,0,0) : %s ",ficboules);

    strcpy(chaine,racine);
	strcat(chaine,".att");
	strcpy(ficatt,chaine);
	fprintf(stderr,"\n fichier des attributs complementaires en entree : %s",ficatt);

    fprintf(stderr,"\n dimx, dimy, dimz en entree  : ");
	scanf("%d %d %d",&dimx,&dimy,&dimz);
	fprintf(stderr,"\n nb max de voisins d'un noeud pour appeller la routine de creation des arcs en entree: ");
	scanf("%d",&nbmaxvois);

	strcpy(chaine,racine);
	strcat(chaine,".arcs");
	strcpy(ficarcs,chaine);
	fprintf(stderr,"\n fichier des arcs en sortie : %s ",ficarcs);

	strcpy(chaine,racine);
	strcat(chaine,".graphe");
	strcpy(ficgraphe,chaine);
	fprintf(stderr,"\n fichier du graphe en sortie : %s",ficgraphe);

    /* creation du fichier des arcs */
    arcsval(ficboules,ficarcs, dimx,dimy,dimz,&nbarcs,nbmaxvois);
	fprintf(stderr,"\n nombre d'arcs du graphe = %d",nbarcs);
    
    /* creation du graphe correspondant a ficboules et a ficatt, on a besoin de ficboules pour ecrire les caracteristiques des boules */
    graphe = cregraphe_val(ficboules,ficarcs,ficatt);
	fprintf(stderr,"\n graphe->nbnoeuds = %d graphe->nel = %d",graphe->nbnoeuds,graphe->nel);

    /* ecriture du graphe dans le fichier ficgraphe */ 
    ecrigraphe(graphe,ficgraphe);

}

void testcreeratt()
{
    char ficdonnees[50],ficboules[50],ficatt[50];
    FILE *ptficboules,*ptficatt,*ptficdonnees; 

    fprintf(stderr,"fichier de donnees (entree), fichier de boules (entree), fichier d'attributs (sortie) :  ");
    scanf("%s %s %s",ficdonnees,ficboules,ficatt); 
    fprintf(stderr,"\n fichier de donnees: %s , fichier de boules: %s, fichier d'attributs: %s ",ficdonnees,ficboules,ficatt);

    creeratt(ficdonnees,ficboules,ficatt);

}

/* Cette procedure cree un fichier d'attributs complementaires (masses de micro-organismes, masses de DOM, masses de CO2) en fonction d'un fichier de donnees */
/* les masses de carbone sont mises à zero */
/* les entrees sont le fichier de donnees et le fichier des boules */
/* le fichier de donnees comprend: la masse totale de microorganismes (%f), la masse totale de DOM (%f), le nombre de boules comrenant des micro-organismes (%d),
le nombre de boules comprenant de la DOM (%d), la liste des numeros des boules comprenant des micro-organismes (suite de %d), , la liste des numeros des boules
comprenant de la DOM */

void creeratt(char *ficdonnees, char *ficboules,char *ficatt)

{

    float massetmic,massetdom,x,y,z,r,sommevolmic,sommevoldom;
    int nbbmic,nbbdom,nboules,numboule,*boulmic,*bouldom,i,j; 
    float** attcopml,**trav;
    FILE *ptficboules,*ptficatt,*ptficdonnees; 

    ptficboules = fopen(ficboules,"r");
	ptficatt = fopen(ficatt,"w");
    ptficdonnees = fopen(ficdonnees,"r");

    fscanf(ptficboules,"%d",&nboules);

    fprintf(stderr,"\n nombre de boules = %d",nboules); 

    /* creation du tableau trav qui contiendra le volume de la boule, sa masse de micro-organismes, sa masse de DOM */
	/* les boules seront indexes a partir de 0 ainsi que les attributs */
    trav = allot2f(3,nboules); 

    /* initialisation du tableau trav a zero */
    for(i=0;i<3;i++)
    {
        for(j=0;j<nboules;j++)
        {
            trav[i][j] = 0; 
        }
    }

    fprintf(stderr,"\n fin initialisation du tableau trav"); 

    /* ecriture du volume des boules dans trav(0,i) */
    for(i=0;i<nboules;i++)
	{
		fscanf(ptficboules,"%f %f %f %f",&x,&y,&z,&r);
        trav[0][i] = (4.0/3.0)*3.1416*r*r*r; 

	}

    fprintf(stderr,"\n fin ecriture du volume des boules dans le tableau trav");

    /* le volume des boules est dans trav[0][.] */

    fscanf(ptficdonnees,"%f %f %d %d",&massetmic,&massetdom,&nbbmic,&nbbdom); 

    fprintf(stderr,"\n masse totale de microorganismes = %f ; masse totale de dom = %f ; nb de boules mic = %d ; nb de boules dom = %d",massetmic,massetdom,nbbmic,nbbdom);

     /* creation d'un tableau contenant les indices de toutes les boules contenant des micro-organismes */
     boulmic = (int*)mycalloc(nbbmic,sizeof(int));

     /* creation d'un tableau contenant les indices de toutes les boules contenant de la DOM */
     bouldom = (int*)mycalloc(nbbdom,sizeof(int));


    /* lecture des boules contenant des microorganismes */
    /* determination de la somme du volume des boules contenant des micro-organismes */

    fprintf(stderr,"\n lecture des boules contenant des microorganismes");

    sommevolmic = 0; 
    for(i=0;i<nbbmic;i++)
    {
        fscanf(ptficdonnees,"%d",&numboule);
        sommevolmic += trav[0][numboule];
        boulmic[i] = numboule; 
    }

    fprintf(stderr,"\n somme des volumes des boules avec des mic = %f",sommevolmic); 

    /* sommevolmic contient la somme du volume des boules contetant des micro-organismes */

    /* lecture des boules contenant de la DOM */
    /* determination de la somme du volume des boules contenant de la DOM */

    fprintf(stderr,"\n lecture des boules contenant de la dom");

    sommevoldom = 0; 
    for(i=0;i<nbbdom;i++)
    {
        fscanf(ptficdonnees,"%d",&numboule);
        sommevoldom += trav[0][numboule];
        bouldom[i] = numboule; 
    }

    fprintf(stderr,"\n somme des volumes des boules avec de la dom = %f",sommevoldom); 

    /* on met a jour trav avec les tableaux boulmic et bouldom */

    for(i=0;i<nbbmic;i++)
    {
        trav[1][boulmic[i]] = (trav[0][boulmic[i]]/sommevolmic)*massetmic ; 
    }

    for(i=0;i<nbbdom;i++)
    {
        trav[2][bouldom[i]] = (trav[0][bouldom[i]]/sommevoldom)*massetdom ; 
    }

    /* trav contient le volume des boules, la masse de microorganismes, la masse de DOM */

    /* remplissage de ficatt a partir de trav */

    fprintf(stderr,"\n remplissage de ficatt");

    fprintf(ptficatt,"%d \n",3);

    for(i=0;i<nboules;i++)
    {
        fprintf(ptficatt,"%f %f %f \n",trav[1][i+1],trav[2][i+1],0.0); 
    }

    fclose(ptficatt);
    fclose(ptficboules); 
    fclose(ptficdonnees);
}

void testcompconnexes()
{
    graphe_s *graphe ;
    char ficgraphe[50],ficcompo[50],ficcompcor[50]; 
    int nbcompco; 

    fprintf(stderr,"\n nom du fichier graphe (entree), nom du fichier 1 (sortie), nom du fichier 2 (sortie)"); 
    scanf("%s %s %s",ficgraphe,ficcompo,ficcompcor);

	graphe = lecgraphe(ficgraphe);
	fprintf(stderr,"\n fin de la lecture du graphe");

	compconnexesval(graphe,ficcompo,ficcompcor,&nbcompco);
	fprintf(stderr,"\n nb de composantes connexes = %d",nbcompco);
}


/* void voisins(int n,graphe_s *graphe1,int nvmax,int *nvois,int **vois) */

/* cette fonction determine les composantes connexes d'un graphe */
/* entree : un pointeur sur le graphe */
/* sortie : un fichier contenant le nombre de composantes connexes, chaque composante connexe : nb de boules, volume total des boules,
   pourcentage du volume total des boules, liste des boules (centre, rayon) ainsi que le nombre de composantes connexes */

void compconnexesval(graphe_s *graphe,char *fcompco,char *fcompcor,int *nbcompco)
{
	int *etiq,nboules,i,natraiter,*atraiter,*compco,ncompco,k,nvmax,nvois,*vois,p,l,numco, nbouleau,numax,nbcompmax;
	float vco,vtotal,somr,pourcmax;
	double vmoyen;
	FILE *ptcompco,*ptcompcor;

	nboules = graphe->nel;

	ptcompco = fopen(fcompco,"w");

	ptcompcor = fopen(fcompcor,"w");

	fprintf(stderr,"\n nb de boules = %d",nboules);

	etiq = (int*)mycalloc(nboules+1,sizeof(int));
	atraiter = (int*)mycalloc(nboules,sizeof(int));
	compco = (int*)mycalloc(nboules,sizeof(int));

	vtotal = 0;
	nvmax = 10000;

	vois = (int*)mycalloc(nvmax,sizeof(int));

	/* calcul du volume total en pixels cube */
	somr = 0;
	for(i=1;i<=nboules;i++)
	{
		vtotal += (4.0/3.0)*M_PI*(graphe->carac[4][i])*(graphe->carac[4][i])*(graphe->carac[4][i]);
		somr += graphe->carac[4][i];
	}
	somr = somr/nboules;


	fprintf(ptcompco,"\n volume total en voxels = %f",vtotal);
	fprintf(ptcompco,"\n");
	fprintf(ptcompco,"\n");

	numco = 0;
	/* numco contient le nombre de composantes connexes */

	/* initialisation de etiq a -1 ; les boules sont numerotees a partir de 1*/
	for(i=0;i<nboules+1;i++)etiq[i]=-1;
	vmoyen = 0;

    pourcmax = 0;
    numax = 0; 
    nbcompmax = 0; 

	for(i=1;i<=nboules;i++)
	{
		/* determination de la composante connexe de la boule i si elle n'est pas deja incluse dans une composante connexe */
		if(etiq[i] < 0)
		{
			numco++;
			/* on determine la composante connexe numco */

			/* cas ou le point n'est pas deja inclu dans une composante connexe */
			/* initialisation a 0 du volume de la composante connexe */
			vco = 0;
			/* determination de la composante connexe du noeud i et stokage dans le fichier */
			/* initialisation a 0 du nombre de noeuds a traiter et du nombre de noeud de la composante connexe */
			natraiter = 0;
			ncompco = 0;
			atraiter[natraiter] = i;
			natraiter++;
			etiq[i] = numco;

			while(natraiter > 0)
			{
				natraiter--;
				k = atraiter[natraiter];
				/* on range k dans le tableau contenant la composante connexe et on met dans le tableau atraiter tous les voisins de k
				 qui ne sont pas deja dans la composante connexe courante ou dans une autre composante connexe (ce dernier cas etant
				 normalement impossible)
				 */
				compco[ncompco] = k;
				/* fprintf(stderr,"\n noeud numero %d inclu",k);*/
				ncompco++;
				/* fprintf(stderr,"\n k = %d",k);*/
				vco += (4.0/3.0)*M_PI*(graphe->carac[4][k])*(graphe->carac[4][k])*(graphe->carac[4][k]);

				voisins(k,graphe,nvmax,&nvois,&vois);
				/* fprintf(stderr,"\n nb de voisins = %d",nvois);*/

				/* on inclut dans le tableau atraiter tous les voisins qui n'ont jamais ete dans le tableau */
				for(p=0;p<nvois;p++)
				{
					l = vois[p];
					/* fprintf(stderr,"\n l = %d",l);*/
					if(etiq[l] < 0)
					{
						/* cas ou le point n'a pas ete traite alors on l'inclut dans la liste a traiter */
						atraiter[natraiter] = l;
						/* fprintf(stderr,"\n inclusion de %d dans la liste a traiter",l);*/
						natraiter++;
						etiq[l] = numco;
					}
				}

			}
			/* a la fin de la boucle natraiter est egal a 0 et le tableau compco contient la composante connexe de la boule numero i */

			/* la composante connexe comprend ncompco noeuds et a un volume total de vco en pixels cube */

			/* stockage des informations dans le fichier de sortie */
			fprintf(ptcompco,"\n");
			fprintf(ptcompco,"\n %d",ncompco);
			fprintf(ptcompco,"\n %f",vco);
			vmoyen += vco;
			fprintf(ptcompco,"\n %f",(vco/vtotal)*100);
			fprintf(ptcompco,"\n");

			fprintf(ptcompcor,"\n");
			fprintf(ptcompcor,"\n composante connexe numero : %d",numco);
			fprintf(ptcompcor,"\n nombre de boules = %d",ncompco);
			fprintf(ptcompcor,"\n volume total des boules = %f",vco);
			fprintf(ptcompcor,"\n pourcentage du volume total de l'ensemble des boules = %f",(vco/vtotal)*100);
			fprintf(ptcompcor,"\n");

			for(p=0;p<ncompco;p++)
			{
				k = compco[p];
				fprintf(ptcompco,"\n %f %f %f %f",graphe->carac[1][k],graphe->carac[2][k],graphe->carac[3][k],graphe->carac[4][k]);
			}
            if(pourcmax < ((vco/vtotal)*100))
            {
                pourcmax = (vco/vtotal)*100;
                numax = numco;
                nbcompmax = ncompco; 
            }
		}

	}
	fprintf(ptcompcor,"\n nombre total de composantes connexes = %d",numco);
	fprintf(ptcompcor,"\n volume moyen des composantes connexes en voxels = %f",(float)(vmoyen/numco));
	fprintf(ptcompcor,"\n rayon moyen des boules = %f",somr);
    fprintf(ptcompcor,"\n numero composante connexe avec le plus grand volume: %d ; pourcentage du volume total: %f ; nombre de boules: %d",numax,pourcmax,nbcompmax);
    fprintf(stderr,"\n numero composante connexe avec le plus grand volume: %d ; pourcentage du volume total: %f ; nombre de boules: %d",numax,pourcmax,nbcompmax);
	*nbcompco = numco;
	free(vois);
	fclose(ptcompco);
	fclose(ptcompcor);
}


/* determine tous les arcs du graphe et les stocke dans le fichier cyl2 qui contient la suite des arcs avec les numeros de noeud
 * dans l'ordre croissant, le fichier des boules a le format: nb de boules, (x,y,z,r)*/

void arcsval(char *cyl,char *cyl2,int dimx, int dimy, int dimz,int *nbarcs,int nmaxv)

{

	FILE *ptboules,*ptarcs,*pttrav;

	int nboules,i,npoints,l;

	float x0,y0,z0,r,a2,u,v,w;

	image *ptimage;

	boule *ptboule;

	int dx,dy,dz,xc,yc,zc,m ;

	int j,k,a,b,c,i1,j1,k1,i2,j2,k2,*voisins,nv,evois,p,stop;


	*nbarcs = 0;

	voisins = (int*)mycalloc(nmaxv,sizeof(int));

	ptboules = fopen(cyl,"r");

	ptarcs = fopen(cyl2,"w");

	pttrav = fopen("arc.trav","w");

	fprintf(stderr,"\n debut des allocations de arcs");

	ptimage = creimage(dimx,dimy,dimz,0);

	fprintf(stderr,"\n fin de l'allocation de l'image dans arcs");

	ptboule = (boule*)mymalloc(sizeof(boule));

	fprintf(stderr,"\n fin de l'allocation de la boule");

	fscanf(ptboules,"%d",&nboules);

	fprintf(stderr,"\n nboules = %d",nboules);

	fprintf(stderr,"\n debut de l'ecriture de l'image");

	for(i=1;i<nboules+1;i++)

	{

		fscanf(ptboules,"%f %f %f %f",&x0,&y0,&z0,&r);

		ptboule->xc = x0;

		ptboule->yc = y0;

		ptboule->zc = z0;

		ptboule->rayon = r;

		npoints = boulimag(ptboule,ptimage,i);

	}

	fclose(ptboules);

	ptboules = fopen(cyl,"r");

	fscanf(ptboules,"%d",&nboules);

	fprintf(stderr,"\n nb de boules = %d",nboules);


	/* ptimage pointe sur une image contenant les boules etiquetees avec leurs numero de noeuds */
	/* parcours de l'image pointe par ptimage pour determiner les arcs du graphe */

	fprintf(stderr,"\n fin de l'ecriture de l'image");

	/* remplissage du fichier des arcs */


	for(l=1;l<nboules + 1;l++)
	{
		/* determination des arcs incluant la boule l */

		/* fprintf(stderr,"\n l = %d",l);*/

		fscanf(ptboules,"%f %f %f %f",&x0,&y0,&z0,&r);

		ptboule->xc = x0;

		ptboule->yc = y0;

		ptboule->zc = z0;

		ptboule->rayon = r;

		boiteboul(ptboule,&xc,&yc,&zc,&dx,&dy,&dz);


		if(xc < 0)xc=0;

		if(yc < 0)yc=0;

		if(zc < 0)zc=0;

		a = xc + dx;

		b = yc + dy;

		c = zc + dz;

		if(a >= dimx)a=dimx-1;

		if(b >= dimy)b=dimy-1;

		if(c >= dimz)c=dimz-1;

		nv = 0;


		for(i=xc;i <= a;i++)

		for(j=yc;j <= b;j++)

		for(k=zc;k <= c;k++)

		{


			if(ptimage->tab[i][j][k] == l)

			{

				/* (i,j,k) contient un point de la boite inclu dans la boule */

				for(i1 = -1 ; i1 <= 1 ; i1++)
				{
					for(j1 = -1 ; j1 <= 1 ; j1++)
					{
						for(k1 = -1 ; k1 <= 1 ; k1++)
						{

							if((i1 != 0 || j1 != 0 || k1 != 0))
							{
								/* (i2,j2,k2) est un point voisin de (i,j,k) different de (i,j,k) */
								i2 = i + i1;
								j2 = j + j1;
								k2 = k + k1;

								if( (i2>=0) &&(j2>=0 )&& (k2>=0) && (i2<dimx) && (j2<dimy) && (k2<dimz))
								{

									evois = ptimage->tab[i2][j2][k2];

									if(evois > l)
									{
										/* on regarde si la boule contenant (i2,j2,k2) est deja dans la liste des voisins */
										p = 0;
										stop = 0;

										while(stop == 0 && p < nv)
										{
											if(voisins[p] == evois)stop = 1;
											p++;
										}

										if(stop == 0)
										{
											if(nv < nmaxv)
												{
													voisins[nv] = evois;
													nv++;
												}
											else
											{
												fprintf(stderr,"\n trop de voisins dans arcs() !");
												exit(56);
											}

										}
									}

								}

							}
						}
					}
				}


			}

		}
/*		le tableau voisin contient toutes les boules voisines de la boule numero l de numero inferieur a l */
/*		stockage des arcs correspondants dans le fichier */

	/*	fprintf(stderr,"\n ecriture des voisins de la boule %d",l);
		fprintf(stderr,"\n nb d'arcs = %d",nv);*/
		for(m=0 ; m<nv ; m++)
		{
			fprintf(pttrav,"\n %d %d",l,voisins[m]);
			(*nbarcs)++;
		}
	}

	fprintf(stderr,"\n nombre total d'arcs = %d",*nbarcs);

	/* ecriture du fichier des arcs final avec le nombre d'arcs en tete du fichier */

	fclose(pttrav);

	pttrav = fopen("arc.trav","r");

	fprintf(ptarcs,"%d",*nbarcs);

	for(i = 0;i<*nbarcs;i++)
	{
		fscanf(pttrav,"%d %d",&j,&k);
		fprintf(ptarcs,"\n %d %d",j,k);
	}

	fclose(ptboules);

	fclose(ptarcs);

	fclose(pttrav);

	freeim(ptimage);

	free(voisins);
}



/* creation d'un graphe pour la simulation biologique a partir d'un fichier de boules, d'un fichier d'arcs,
 d'un fichier d'attributs des pores (nb d'attributs complementaires pour chaque pore en dehors du rayon de la boule,
 liste des attributs complementaires). Le graphe cree est pour la simulation donc
 comprend un tas vide */

graphe_s *cregraphe_val(char *cyl,char *arcs,char *attributs)

{

	int nbarmax,nbnomax,ncar,i,j;

	int k,nbarcs,n,natt ;

	float *car;

	float r,x1,y1,z1;

	float t1,t2,t3,t4;

	graphe_s *graphe1;

	FILE *ptcyl,*ptarcs,*ptatt;


	/* ouverture du fichier des boules */
	ptcyl = fopen(cyl,"r");

	/* ouverture du fichier des arcs */
	ptarcs = fopen(arcs,"r");

	/* ouverture du fichier des attributs en plus du centre et du rayon de la boule */
	ptatt = fopen(attributs,"r");

	/* lecture du nombre de boules */
	fscanf(ptcyl,"%d",&nbnomax);

	/* lecture du nombre d'arcs */
	fscanf(ptarcs,"%d",&nbarcs);

	/* lecture du nombre d'attributs */
	fscanf(ptatt,"%d",&natt);

	fprintf(stderr,"\n nb de boules= %d nb d'arcs = %d nb d'attributs complementaires par boules= %d",nbnomax,nbarcs,natt);

	nbarmax = nbarcs +1;

	ncar = natt + 4;

	fprintf(stderr,"\n debut de creation du graphe initial vide");

	graphe1 = creergraphe_s (nbnomax,nbarmax,ncar);

	fprintf(stderr,"\n fin de creation du graphe initial");



	/* creation des noeuds du graphe */

	fprintf(stderr,"\n debut de creation des noeuds du graphe");

	car = (float*)mycalloc(ncar + 1,sizeof(float));


	for(k=0;k<nbnomax;k++)

	{

		fscanf(ptcyl,"%f %f %f %f",&x1,&y1,&z1,&r);

		car[1] = x1;

		car[2] = y1;

		car[3] = z1;

		car[4] = r;

		for(j=0; j<natt;j++)
		{
			fscanf(ptatt,"%f",&t1);
			car[j+5] = t1;
		}

		crenoeud_s2(graphe1,car,ncar);

	}


	fprintf(stderr,"\n fin de creation des noeuds du graphe");

	/* creation des arcs du graphe */



	for(n=0;n< nbarcs;n++)

	{
		fscanf(ptarcs,"%d %d",&i,&k);
		incou2_s(graphe1,i,k);
	}

	fclose(ptcyl);

	myfreef(car);

	return(graphe1);
}

/* test de la diffusion avec le schéma explicite */

void testdiffusionexp()
{
	char ficgraphe1[50],ficgraphe2[50];
	int numcar, fraction, nvmax;
	float coeff,deltat;

	fprintf(stderr,"\n fichier du graphe initial ; fichier du graphe en sortie : ");
	scanf("%s %s",ficgraphe1,ficgraphe2);
	fprintf(stderr,"\n numero attribut ; coefficient de diffusion, intervalle de temps, fraction intervalle de temps, nombre max voisins: ");
	scanf("%d %f %f %d %d",&numcar,&coeff,&deltat,&fraction,&nvmax);
	diffusionexp(ficgraphe1,numcar,coeff,deltat,fraction,nvmax,ficgraphe2);

}


/* cette procedure realise la diffusion d'une caracteristique complementaire entre t0 et t0 + ∆t par la methode explicite */
/* entree: le nom du fichier graphe, le numero de la caracteristique complementaire, le coefficient de diffusion, l'intervalle de temps ∆T,
la fraction de l'intervalle de temps pour la methode explicite, nvmax est le nombre maximum de voisins pour un noeud */
/* sortie: le nom du fichier graphe du nouveau graphe (ficgraphe2) apres diffusion, seuls les attributs noeuds correspondant a la caracteristique diffusee ont change */

void diffusionexp(char *ficgraphe1,int numcar,float coeff,float deltat,int fraction,int nvmax,char *ficgraphe2)
{
	graphe_s *graphe;
	int nbnoeud,i,j,k,n,*tavois,nvois;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,*tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,massetot2,harm;

	graphe = lecgraphe(ficgraphe1);
	nbnoeud = graphe->nbnoeuds; 
	tavois = (int*)mycalloc(nvmax,sizeof(int));
	tabdelta = (float*)mycalloc(nbnoeud + 1,sizeof(float)); 

	/* calcul de la masse totale initiale de la caracteristique a diffuser */
	massetot = 0;
	for(i = 1 ; i <= nbnoeud ; i++)
	{
		massetot += graphe->carac[4+numcar][i];
	}
	fprintf(stderr,"\n masse totale de la caracteristique a diffuser = %f",massetot);

	petitdeltat = deltat/fraction ;
	fprintf(stderr,"\n petit delta t = %f",petitdeltat); 
	/* initialisation du tableau des deltas à 0, le tableau est indexe a partir de 1 comme les noeuds du graphe */
	for(n = 0 ; n < fraction ; n++)
	{
		for(i=1 ; i<= nbnoeud; i++)
		{
			tabdelta[i] = 0; 
		}
		/* pour chaque noeud on calcule la somme des deltas avec tous ces voisins pour la diffusion pour chaque intervalle (deltat/n) */

		for(i = 1 ; i <= nbnoeud ; i++)
		{
			rboul = graphe->carac[4][i];
			volboul = (4.0/3.0)*3.1416*rboul*rboul*rboul; 
			gdif = graphe->carac[4+numcar][i]; 
			conc = gdif/volboul; 
			/* volboul contient le volume de la boule, rboul son rayon, gdif la caractetristique a diffuser, conc la concentration */

			/* determination des voisins du noeud i */
			voisins(i,graphe,nvmax,&nvois,&tavois);
			fprintf(stderr,"\n nombre de voisins du noeud %d = %d",i,nvois); 
			/* les nvois voisins du noeud i sont ranges dans tavois */

			x1 = graphe->carac[1][i];
			y1 = graphe->carac[2][i];
			z1 = graphe->carac[3][i];

			for(j=0;j<nvois;j++)
			{
				k = tavois[j];
				fprintf(stderr,"\n voisin du noeud %d est %d",i,k); 
				x2 = graphe->carac[1][k];
				y2 = graphe->carac[2][k];
				z2 = graphe->carac[3][k];
				rboul2 = graphe->carac[4][k];

				fprintf(stderr,"\n rapport entre distance des centres et somme des rayons = %f",sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2))/(rboul+rboul2)); 

				/* traitement du flux entre i et k */
				rboul2 = graphe->carac[4][k];
				volboul2 = (4.0/3.0)*3.1416*rboul2*rboul2*rboul2; 
				gdif2 = graphe->carac[4+numcar][k]; 
				conc2 = gdif2/volboul2; 
				/* calcul de la surface de contact */
				/* scont = (rboul>rboul2) ? (4*3.1416*rboul2*rboul2):(4*3.1416*rboul*rboul);*/
				/* scont = 4.0*3.1416*((rboul+rboul2)/2.0) * ((rboul + rboul2)/2.0);*/
				harm = (2*rboul*rboul2)/(rboul+rboul2); 
				scont = 4.0*3.1416*harm*harm;
				
				distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
				if(distance == 0)
				{
					fprintf(stderr,"\n distance entre deux centres de boules a zero !!");
					exit(10); 
				}
				delta = -(coeff*(conc-conc2)*scont*petitdeltat)/distance;
				tabdelta[i] += delta;
			}
		}
		/* mise à jour de la caracteristique diffusee dans le graphe */
		masseneg = 0; 
		sommeconc = 0; 
		for(i = 1 ; i <= nbnoeud ; i++)
		{
			fprintf(stderr,"\n graphe.. = %f ; tabdelta.. = %f",graphe->carac[4+numcar][i],tabdelta[i]); 
			graphe->carac[4+numcar][i] += tabdelta[i]; 
			fprintf(stderr,"\n graphe.. = %f",graphe->carac[4+numcar][i]); 
			if(graphe->carac[4+numcar][i] < 0)
			{
				masseneg += -graphe->carac[4+numcar][i];
			}
			else
			{
				rboul = graphe->carac[4][i];
				volboul = (4.0/3.0)*3.1416*rboul*rboul*rboul; 
				gdif = graphe->carac[4+numcar][i]; 
				conc = gdif/volboul; 
				sommeconc += conc; 
			}
		}
		/* on traite les cas ou des valeurs de la caracteristique diffusee seraient negatives */
		fprintf(stderr,"\n proportion de masse negative = %f",(masseneg/massetot)*100); 

		/* rectification des masses negatives */
		massetot2 = 0;
		for(i = 1 ; i <= nbnoeud ; i++)
		{
			if(graphe->carac[4+numcar][i] <= 0)
			{
				graphe->carac[4+numcar][i] = 0;
			}
			else
			{
				rboul = graphe->carac[4][i];
				volboul = (4.0/3.0)*3.1416*rboul*rboul*rboul; 
				gdif = graphe->carac[4+numcar][i]; 
				conc = gdif/volboul; 
				graphe->carac[4+numcar][i] -= (conc/sommeconc)*masseneg;
				if(graphe->carac[4+numcar][i] < 0)
				{
					fprintf(stderr,"\n rectification des valeurs negatives impossible, reduire le pas de temps");
					exit(1); 
				}
				massetot2 += graphe->carac[4+numcar][i]; 
			}
		}
		fprintf(stderr,"\n rapport pour bilan des masses: %f",massetot2/massetot);

		/* la caracteristique a ete diffusee avec l'intervalle petitdeltat */
	}
	/* le tableau des caracteristiques du graphe contient les valeurs de la caracteristique diffusee apres diffusion */

	/* ecriture du nouvaeu graphe value apres diffusion, seules les valeurs de la caracteristique diffusee ont pu changer */
	ecrigraphe(graphe,ficgraphe2);
}

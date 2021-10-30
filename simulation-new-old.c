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
	fprintf(stderr,"\n fichier de boules maximales en entree (x,y,z,r) : %s ",ficboules);

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
	fprintf(stderr,"\n graphe->nbnoeuds = %d ; graphe->nel = %d ; graphe->ncar = %d",graphe->nbnoeuds,graphe->nel,graphe->ncar);

    /* ecriture du graphe dans le fichier ficgraphe */ 
    ecrigraphe(graphe,ficgraphe);

}

void testcreeratt()
{
    char ficdonnees[50],ficboules[50],ficatt[50];
    FILE *ptficboules,*ptficatt,*ptficdonnees; 

    fprintf(stderr,"fichier de donnees (entree,MB,DOM), fichier de boules (entree), fichier d'attributs (sortie) :  ");
    scanf("%s %s %s",ficdonnees,ficboules,ficatt); 
    fprintf(stderr,"\n fichier de donnees: %s , fichier de boules: %s, fichier d'attributs: %s ",ficdonnees,ficboules,ficatt);

    creeratt(ficdonnees,ficboules,ficatt);

}


void testcreeratt5()
{
    char ficdonnees[50],ficboules[50],ficatt[50];
    FILE *ptficboules,*ptficatt,*ptficdonnees; 

    fprintf(stderr,"fichier de donnees (entree,MB,DOM,SOM,FOM), fichier de boules (entree), fichier d'attributs (sortie) :  ");
    scanf("%s %s %s",ficdonnees,ficboules,ficatt); 
    fprintf(stderr,"\n fichier de donnees: %s , fichier de boules: %s, fichier d'attributs: %s ",ficdonnees,ficboules,ficatt);

    creeratt5(ficdonnees,ficboules,ficatt);

}

/* Cette procedure cree un fichier d'attributs complementaires (masses de micro-organismes, masses de DOM, masses de CO2) en fonction d'un fichier de donnees */
/* les masses de carbone sont mises à zero */
/* les entrees sont le fichier de donnees et le fichier des boules */
/* le fichier de donnees comprend: la masse totale de microorganismes (%f), la masse totale de DOM (%f), le nombre de boules comrenant des micro-organismes (%d),
le nombre de boules comprenant de la DOM (%d), la liste des numeros des boules comprenant des micro-organismes (suite de %d), , la liste des numeros des boules
comprenant de la DOM 
la sortie est le fichier des attributs complémentaires ficatt*/

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
	/* les boules seront indexes a partir de 1 ainsi que les attributs */
    trav = allot2f(3,nboules+1); 

    /* initialisation du tableau trav a zero */
    for(i=0;i<3;i++)
    {
        for(j=0;j<=nboules;j++)
        {
            trav[i][j] = 0; 
        }
    }

    fprintf(stderr,"\n fin initialisation du tableau trav"); 

    /* ecriture du volume des boules dans trav(0,i) */
    for(i=1;i<=nboules;i++)
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

/* Cette procedure cree un fichier initial d'attributs complementaires (masses de micro-organismes, masses de DOM, masses de
SOM, masses de FOM, masses de CO2) 
en fonction d'un fichier de donnees */
/* les masses de carbone sont mises à zero */
/* les entrees sont le fichier de donnees et le fichier des boules */
/* le fichier de donnees comprend: la masse totale de MB (%f), la masse totale de DOM (%f), 
la masse totale de SOM (%f), la masse totale de FOM (%f), le nombre de boules comprenant des MB (%d),
le nombre de boules comprenant de la DOM (%d), le nb de boules contenant de la SOM %d), le nb de boules contenant 
de la FOM (%d), la liste des numeros des boules comprenant des MB (suite de %d), la liste des numeros des boules
comprenant de la DOM, la liste des numeros des boules
comprenant de la SOM, la liste des numeros des boules
comprenant de la FOM 
si ce nombre de boules est -1 (seulement pour les MB ou la DOM) alors la masse est repartie de manière uniforme sur l'ensemble des boules proportionellement au volume */

void creeratt5(char *ficdonnees, char *ficboules,char *ficatt)

{

    float massetmic,massetdom,massetsom,massetfom,x,y,z,r,sommevolmic,sommevoldom,sommevolsom,sommevolfom,sommevolboules;
    int nbbmic,nbbdom,nbbsom,nbbfom,nboules,numboule,*boulmic,*bouldom,*boulsom,*boulfom,i,j; 
    float **trav,bilanmasse1,bilanmasse2;
    FILE *ptficboules,*ptficatt,*ptficdonnees; 

    ptficboules = fopen(ficboules,"r");
	ptficatt = fopen(ficatt,"w");
    ptficdonnees = fopen(ficdonnees,"r");

    fscanf(ptficboules,"%d",&nboules);

    fprintf(stderr,"\n nombre de boules = %d",nboules); 

    /* creation du tableau trav qui contiendra le volume de la boule, sa masse de micro-organismes, sa masse de DOM, sa
	masse de SOM et sa masse de FOM */
	/* les boules seront indexes a partir de 0 ainsi que les attributs */
    trav = allot2f(5,nboules+1); 

    /* initialisation du tableau trav a zero */
    for(i=0;i<5;i++)
    {
        for(j=0;j<=nboules;j++)
        {
            trav[i][j] = 0; 
        }
    }

    fprintf(stderr,"\n fin initialisation du tableau trav"); 

    /* ecriture du volume des boules dans trav(0,i) */
    for(i=1;i<=nboules;i++)
	{
		fscanf(ptficboules,"%f %f %f %f",&x,&y,&z,&r);
        trav[0][i] = (4.0/3.0)*3.1416*r*r*r; 

	}

    fprintf(stderr,"\n fin ecriture du volume des boules dans le tableau trav");

    /* le volume des boules est dans trav[0][.] */

    fscanf(ptficdonnees,"%f %f %f %f %d %d %d %d",&massetmic,&massetdom,&massetsom,&massetfom,&nbbmic,&nbbdom,&nbbsom,&nbbfom); 

    fprintf(stderr,"\n masse totale MB= %f ; masse totale DOM= %f ; masse totale SOM= %f ; masse totale FOM= %f ; nb boules MB= %d ; nb boules DOM= %d ; nb boules SOM= %d ; nb boules FOM= %d ",massetmic,massetdom,massetsom,massetfom,nbbmic,nbbdom,nbbsom,nbbfom);

	/* calcul du bilan des masses initial */
	bilanmasse1 = massetdom + massetsom + massetfom + massetmic; 

	fprintf(stderr,"\n bilan des masses initial = %f",bilanmasse1); 

     /* creation d'un tableau contenant les indices de toutes les boules contenant des micro-organismes */
	 if(nbbmic >= 0)
	 {
     	boulmic = (int*)mycalloc(nbbmic,sizeof(int));
	 }else if(nbbmic == -1)
	 {
		boulmic = (int*)mycalloc(nboules,sizeof(int));
	 }
	 else
	 {
		 boulmic = (int*)mycalloc(-(nbbmic+1),sizeof(int));
	 }

     /* creation d'un tableau contenant les indices de toutes les boules contenant de la DOM */
	 if(nbbdom >= 0)
	 {
     	bouldom = (int*)mycalloc(nbbdom,sizeof(int));
	 }else if(nbbdom == -1)
	 {
		bouldom = (int*)mycalloc(nboules,sizeof(int));
	 }
	 else
	 {
		 bouldom = (int*)mycalloc(-(nbbdom+1),sizeof(int));
	 }

	/* creation d'un tableau contenant les indices de toutes les boules contenant de la SOM */
     boulsom = (int*)mycalloc(nbbsom,sizeof(int));

	 /* creation d'un tableau contenant les indices de toutes les boules contenant de la FOM */
     boulfom = (int*)mycalloc(nbbfom,sizeof(int));

    /* lecture des boules contenant des microorganismes */
    /* determination de la somme du volume des boules contenant des micro-organismes */

    fprintf(stderr,"\n lecture des boules contenant des microorganismes");

    sommevolmic = 0; 
	if(nbbmic >= 0)
	{
    	for(i=0;i<nbbmic;i++)
    	{
        	fscanf(ptficdonnees,"%d",&numboule);
        	sommevolmic += trav[0][numboule];
        	boulmic[i] = numboule; 
    	}
	}else if(nbbmic == -1)
	{
		for(i=0;i<nboules;i++)
		{
			sommevolmic += trav[0][i+1];
			boulmic[i] = i+1; 
		}
	}else
	{
		for(i=0;i< -(nbbmic+1);i++)
		{
			sommevolmic += trav[0][i+1];
			boulmic[i] = i+1; 
		}
	}
    fprintf(stderr,"\n somme des volumes des boules avec des mic = %f",sommevolmic); 

    /* sommevolmic contient la somme du volume des boules contetant des micro-organismes */

    /* lecture des boules contenant de la DOM */
    /* determination de la somme du volume des boules contenant de la DOM */

    fprintf(stderr,"\n lecture des boules contenant de la dom");

    sommevoldom = 0; 
	if(nbbdom >= 0)
	{
    	for(i=0;i<nbbdom;i++)
    	{
        	fscanf(ptficdonnees,"%d",&numboule);
        	sommevoldom += trav[0][numboule];
        	bouldom[i] = numboule; 
    	}
	}else if(nbbdom == -1)
	{
		for(i=0;i<nboules;i++)
		{
			sommevoldom += trav[0][i+1];
			bouldom[i] = i+1; 
		}
	}else
	{
		for(i=0;i< -(nbbdom+1);i++)
		{
			sommevoldom += trav[0][i+1];
			bouldom[i] = i+1; 
		}
	}

    fprintf(stderr,"\n somme des volumes des boules avec de la dom = %f",sommevoldom); 

	fprintf(stderr,"\n lecture des boules contenant de la som");

    sommevolsom = 0; 
    for(i=0;i<nbbsom;i++)
    {
        fscanf(ptficdonnees,"%d",&numboule);
        sommevolsom += trav[0][numboule];
        boulsom[i] = numboule; 
    }

    fprintf(stderr,"\n somme des volumes des boules avec de la som = %f",sommevolsom); 

	fprintf(stderr,"\n lecture des boules contenant de la fom");

    sommevolfom = 0; 
    for(i=0;i<nbbfom;i++)
    {
        fscanf(ptficdonnees,"%d",&numboule);
        sommevolfom += trav[0][numboule];
        boulfom[i] = numboule; 
    }

    fprintf(stderr,"\n somme des volumes des boules avec de la fom = %f",sommevolfom); 

	bilanmasse2 = 0;
    /* on met a jour trav avec les tableaux boulmic, bouldom, boulsom et boulfom */
	if(nbbmic >= 0)
	{
    	for(i=0;i<nbbmic;i++)
    	{
       	 	trav[1][boulmic[i]] = (trav[0][boulmic[i]]/sommevolmic)*massetmic ; 
			bilanmasse2 += trav[1][boulmic[i]]; 
   	 	}
	}else if(nbbmic == -1)
	{
		for(i=0;i<nboules;i++)
    	{
       	 	trav[1][boulmic[i]] = (trav[0][boulmic[i]]/sommevolmic)*massetmic ; 
			bilanmasse2 += trav[1][boulmic[i]];
   	 	}
	}else
	{
		for(i=0;i< -(nbbmic+1);i++)
    	{
       	 	trav[1][boulmic[i]] = (trav[0][boulmic[i]]/sommevolmic)*massetmic ; 
			bilanmasse2 += trav[1][boulmic[i]];
   	 	}
	}

	if(nbbdom >= 0)
	{
    	for(i=0;i<nbbdom;i++)
    	{
        	trav[2][bouldom[i]] = (trav[0][bouldom[i]]/sommevoldom)*massetdom ; 
			bilanmasse2 += trav[2][bouldom[i]];
    	}
	}else if(nbbdom == -1)
	{
		for(i=0;i<nboules;i++)
    	{
       	 	trav[2][bouldom[i]] = (trav[0][bouldom[i]]/sommevoldom)*massetdom ; 
			bilanmasse2 += trav[2][bouldom[i]];
   	 	}
	}else
	{
		for(i=0;i< -(nbbdom+1);i++)
    	{
       	 	trav[2][bouldom[i]] = (trav[0][bouldom[i]]/sommevoldom)*massetdom ; 
			bilanmasse2 += trav[2][bouldom[i]];
   	 	}
	}

	for(i=0;i<nbbsom;i++)
    {
        trav[3][boulsom[i]] = (trav[0][boulsom[i]]/sommevolsom)*massetsom ; 
		bilanmasse2 += trav[3][boulsom[i]];
    }

	for(i=0;i<nbbfom;i++)
    {
        trav[4][boulfom[i]] = (trav[0][boulfom[i]]/sommevolfom)*massetfom ;
		bilanmasse2 += trav[4][boulfom[i]];
    }

	fprintf(stderr,"\n bilan des masses apres distribution = %f",bilanmasse2); 

    /* trav contient le volume des boules, la masse de microorganismes, la masse de DOM, la masse de SOM, la masse de FOM */

    /* remplissage de ficatt a partir de trav */

    fprintf(stderr,"\n remplissage de ficatt");

    fprintf(ptficatt,"%d \n",5);

    for(i=1;i<=nboules;i++)
    {
        fprintf(ptficatt,"%f %f %f %f %f \n",trav[1][i],trav[2][i],trav[3][i],trav[4][i],0.0); 
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


	fprintf(ptcompco,"\n volume total en voxels = %f ",vtotal);
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
			fprintf(ptcompcor,"\n composante connexe numero : %d ",numco);
			fprintf(ptcompcor,"\n nombre de boules = %d ",ncompco);
			fprintf(ptcompcor,"\n volume total des boules = %f ",vco);
			fprintf(ptcompcor,"\n pourcentage du volume total de l'ensemble des boules = %f ",(vco/vtotal)*100);
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
    fprintf(ptcompcor,"\n numero composante connexe avec le plus grand volume: %d ; pourcentage du volume total: %f ; nombre de boules: %d ",numax,pourcmax,nbcompmax);
    fprintf(stderr,"\n numero composante connexe avec le plus grand volume: %d ; pourcentage du volume total: %f ; nombre de boules: %d ",numax,pourcmax,nbcompmax);
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

	float x0,y0,z0,r,a2,u,v,w,*Xb,*Yb,*Zb,*Rb,rap;

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

	Xb = (float*)mycalloc(nboules + 1,sizeof(float)); 
	Yb = (float*)mycalloc(nboules + 1,sizeof(float)); 
	Zb = (float*)mycalloc(nboules + 1,sizeof(float)); 
	Rb = (float*)mycalloc(nboules + 1,sizeof(float)); 


	for(i=1;i<nboules+1;i++)

	{

		fscanf(ptboules,"%f %f %f %f",&x0,&y0,&z0,&r);

		ptboule->xc = x0;

		ptboule->yc = y0;

		ptboule->zc = z0;

		ptboule->rayon = r;

		Xb[i] = x0;
		Yb[i] = y0;
		Zb[i] = z0;
		Rb[i] = r; 

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

		/*
		rap = sqrt((Xb[j]-Xb[k])*(Xb[j]-Xb[k])+(Yb[j]-Yb[k])*(Yb[j]-Yb[k])+(Zb[j]-Zb[k])*(Zb[j]-Zb[k]))/(Rb[j]+Rb[k]);
		if(rap >= 1.3)
		{
			fprintf(stderr,"\n rapport distance centre sur somme des rayons trop grand dans arcsval pour paire (%d,%d): %f ",j,k,rap); 
			fprintf(stderr,"\n x1=%f ; y1=%f ; z1=%f ; r1=%f ; x2=%f ; y2=%f ; z2=%f ; r2=%f ",Xb[j],Yb[j],Zb[j],Rb[j],Xb[k],Yb[k],Zb[k],Rb[k]); 
		}
		*/

	}

	fclose(ptboules);

	fclose(ptarcs);

	fclose(pttrav);

	freeim(ptimage);

	remove("arc.trav");

	free(voisins);
	free(Xb);
	free(Yb);
	free(Zb);
	free(Rb);
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

	fclose(ptarcs);

	fclose(ptatt);

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
	float massetot,masseneg,sommeconc,massetot2,prop;

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
	fprintf(stderr,"\n masse totale de la caracteristique a diffuser = %f ; deltat = %f ; fraction = %d ",massetot,deltat,fraction);

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
			/* fprintf(stderr,"\n nombre de voisins du noeud %d = %d",i,nvois); */
			/* les nvois voisins du noeud i sont ranges dans tavois */

			x1 = graphe->carac[1][i];
			y1 = graphe->carac[2][i];
			z1 = graphe->carac[3][i];

			for(j=0;j<nvois;j++)
			{
				k = tavois[j];
				/* fprintf(stderr,"\n voisin du noeud %d est %d",i,k); */
				x2 = graphe->carac[1][k];
				y2 = graphe->carac[2][k];
				z2 = graphe->carac[3][k];
				rboul2 = graphe->carac[4][k];

				/* fprintf(stderr,"\n rapport entre distance des centres et somme des rayons = %f",sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2))/(rboul+rboul2));*/ 

				/* traitement du flux entre i et k */
				rboul2 = graphe->carac[4][k];
				volboul2 = (4.0/3.0)*3.1416*rboul2*rboul2*rboul2; 
				gdif2 = graphe->carac[4+numcar][k]; 
				conc2 = gdif2/volboul2; 
				/* calcul de la surface de contact */
				/* scont = (rboul>rboul2) ? (4*3.1416*rboul2*rboul2):(4*3.1416*rboul*rboul);*/

				scont = (2.0*rboul*rboul2)/(rboul+rboul2); 
				
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
			/* fprintf(stderr,"\n graphe.. = %f ; tabdelta.. = %f",graphe->carac[4+numcar][i],tabdelta[i]); */
			graphe->carac[4+numcar][i] += tabdelta[i]; 
			/* fprintf(stderr,"\n graphe.. = %f",graphe->carac[4+numcar][i]); */
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
		prop = (masseneg/massetot)*100 ;
		/* fprintf(stderr,"\n masse totale = %f ; masse negative = %f ; proportion de masse negative = %f",massetot,masseneg,prop); */
		if(prop > 20)
		{
			fprintf(stderr,"\n reduire le petit pas de temps car proportion de valeurs negatives trop importante = %f",prop);
			exit(3); 
		}
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
		/* fprintf(stderr,"\n rapport pour bilan des masses: %f",massetot2/massetot);*/

		/* la caracteristique a ete diffusee avec l'intervalle petitdeltat */
	}
	/* le tableau des caracteristiques du graphe contient les valeurs de la caracteristique diffusee apres diffusion */

	/* ecriture du nouvaeu graphe value apres diffusion, seules les valeurs de la caracteristique diffusee ont pu changer */
	ecrigraphe(graphe,ficgraphe2);
}

/* cette procedure prend en entree un fichier de graphe associé a une image de dimensions: dimx,dimy,dimz, le numero d'un attribut
complementaire (on ne compte pas les attributs de la geometrie soit le centre de la boule et son rayon) et rend une image normalisee de dimensions dimx,dimy,dimz
ou les boules ont un niveau de gris correspondant a leur concentration par rapport a cet attribut, un fichier contenant: 
le nb de boules, (x,y,z,r,concentration) */

float *viscar(char *ficgraphe,int dimx, int dimy,int dimz,int num,char *ficimage,char *ficboulescar)
{
	graphe_s *graphe;
	int nbnoeud,i,c3,j,k,npoints; 
	float *conc,r,v,c,x,y,z,cmin,cmax,c2; 
	imagef *ptimage; 
	boule *ptboule; 
	FILE *ptficimage,*ptficboulescar; 

	ptficimage = fopen(ficimage,"w");
	ptficboulescar = fopen(ficboulescar,"w"); 
	graphe = lecgraphe(ficgraphe);
	/* graphe contient le graphe associe au fichier ficgraphe */
	nbnoeud = graphe->nbnoeuds;

	fprintf(ptficboulescar,"%d \n",nbnoeud); 

	conc = (float*)mycalloc(nbnoeud+1,sizeof(float));
	/* le tableau conc va contenir les concentrations correspondant a l'attribut num 
	ce tableau est revoye par la fonction et est indexe a partir de 1 numero du premier noeud */

	ptboule = (boule*)mymalloc(sizeof(boule));
	
	ptimage = creimagef(dimx,dimy,dimz,0);
	/* image va contenir l'image ou les voxels des boules sont colories avec la concentration de l'attribut numatt
	dans la boule */
	/* initialisation de cmin et de cmax */
	x = graphe->carac[1][1];
	y = graphe->carac[2][1];
	z = graphe->carac[3][1];
	r = graphe->carac[4][1];
	v = (4.0/3.0)*3.1416*r*r*r; 
	c = (graphe->carac[4+num][1])/v; 
	cmax = c;
	cmin = c; 

	for(i=1;i <= nbnoeud;i++)
	{
		x = graphe->carac[1][i];
		y = graphe->carac[2][i];
		z = graphe->carac[3][i];
		r = graphe->carac[4][i];

		v = (4.0/3.0)*3.1416*r*r*r; 

		c = (graphe->carac[4+num][i])/v; 

		if(c<cmin)cmin=c;
		if(c>cmax)cmax=c; 
		ptboule->xc = x;
		ptboule->yc = y;
		ptboule->zc = z;
		ptboule->rayon = r;
		/* c est la concentration de l'attribut complementaire num dans la boule */
		/* on colorie dans ptimage les points de la boule avec la concentration c */
		npoints = boulimagf(ptboule,ptimage,c);

		/* on ecrit la boule avec la concentration dans le fichier ficboulescar */
		fprintf(ptficboulescar,"%f %f %f %f %f \n",x,y,z,r,c); 
		conc[i] = c; 
	}
	fprintf(stderr,"\n cmin = %f ; cmax = %f",cmin,cmax); 
	/* on ecrit l'image des concentrations dans le fichier ficimage en ramenant les valeurs entre 0 et 255 */
	for(k=0;k<dimz;k++)
	{
		for(j=0;j<dimy;j++)
		{
			for(i=0;i<dimx;i++)
			{
				c = ptimage->tab[i][j][k];
				if(c==0)
				{
					fprintf(ptficimage,"%d ",0);
				}
				else
				{
					c2 = (c-cmin)/(cmax-cmin);
					c3 = neari(c2*200+55); 
					/* on a normalise entre 0 et 245 pour laisser le fond a 0 et la valeur min a 55 */
					fprintf(ptficimage,"%d ",c3);
				}
			
				
			}
			fprintf(ptficimage,"\n");
		}
	}
	fclose(ptficboulescar);
	fclose(ptficimage); 
	return(conc); 
}

void testviscar()
{
	char ficgraphe[50],ficimage[50],ficboulescar[50];
	int dimx,dimy,dimz,num; 
	float *conc; 

	fprintf(stderr,"\n fichier graphe(entree) ; fichier image (sortie) ; fichier boules attribut (sortie): ");
	scanf("%s %s %s",ficgraphe,ficimage,ficboulescar); 
	fprintf(stderr,"\n dimx, dimy, dimz, numcar: ");
	scanf("%d %d %d %d",&dimx,&dimy,&dimz,&num); 

	conc =viscar(ficgraphe,dimx,dimy,dimz,num,ficimage,ficboulescar); 
}

/* cette procedure calcule les delta des attributs du graphe apres transformation biologique du modele Soilmu3d/Mepsom
pour un intervalle de temps deltat */
/* elle rend un tableau delta[6][nbnoeuds +1] ou delta[i][j] correspond a l'attribut complementaire i du noeud j;
on indexe a partir de 1 pour rester compatible avec les conventions du graphe */
/* ce tableau contient les deltas correspondant a l'application du modele biologique sur les noeuds du graphe */
/* deltat est l'intervalle de temps ; ro: relative respiration rate ; mu : relative mortality rate ; rom: proportion
de MB retournant a la DOM (l'autre fraction se transforme en SOM), vfom et vsom les taux de decomposition relatifs de 
la FOM et de la SOM ; vdom taux relatif maximum de croissance des microorganismes ; kab: constante de demi saturation de DOM
*/
float **miseajourbio(graphe_s *graphe,float deltat,float ro,float mu,float rom,float vfom,float vsom,float vdom,float kab)
{
	float r,x,y,z,**deltas,somdeltas,x1,x2,x3,x4,x5,cdom,vart;
	int nboules,i,j;

	nboules = graphe->nbnoeuds;
	/* nboules contient le nombre de boules */

	deltas = allotab2f(5,nboules); 

	/* vart = 0;*/

	/* traitement iteratif des noeuds */
	for(i=1;i<=nboules;i++)
	{
		x = graphe->carac[1][i];
		y = graphe->carac[2][i];
		z = graphe->carac[3][i];
		r = graphe->carac[4][i];
		x1 = graphe->carac[5][i];
		x2 = graphe->carac[6][i];
		x3 = graphe->carac[7][i];
		x4 = graphe->carac[8][i];
		x5 = graphe->carac[9][i];

		cdom = x2/((4.0/3.0)*3.1416*r*r*r);

		/* cdom contient la concentration en DOM du noeud */

		deltas[1][i] = - ro*x1*deltat - mu*x1*deltat + (vdom*cdom*x1*deltat)/(kab+cdom);

		/* vart += (vdom*cdom*x1*deltat)/(kab+cdom);*/

		deltas[2][i] = rom*mu*x1*deltat - (vdom*cdom*x1*deltat)/(kab+cdom) + vsom*x3*deltat + vfom*x4*deltat ;

		deltas[3][i] = (1-rom)*mu*x1*deltat - vsom*x3*deltat ;

		deltas[4][i] = - vfom*x4*deltat ; 

		deltas[5][i] = ro*x1*deltat ; 

		/* x1 : masse de MB ; x2 : masse de DOM ; x3 : masse de SOM ; x4 : masse de FOM ; x5 : masse de CO2 */

		/* (y1,y2,y3,y4,y5) sont les nouvelles valeurs de (x1,x2,x3,x4,x5) apres transformation */

	}
	/* fprintf(stderr,"\n vart = %f ",vart); 
	exit(4);*/
	/* verification du blian des masses */
	/* somdeltas = 0; 
	for(i=1;i<=nboules;i++)
	{
		for(j=1;j<=5;j++)
		{
			somdeltas += deltas[j][i]; 
		}
	}*/
	/* fprintf(stderr,"\n somme des deltas des transformations biologiques = %f",somdeltas);*/
	return(deltas); 
}

/* cette procedure prend en entree un fichier contenant un graphe value de boules avec les attributs complementaires
(MB,DOM,SOM,FOM,CO2) (unite g), un fichier contenant les parametres du modele biologique
(vdom,kab,mu,ro,rom) (j-1,gCg-1,j-1,j-1), le coefficient de diffusion de la DOM, un intervalle de temps (en jours) 
une fraction de l'intervalle de temps pour le schema explicite, et rend en
sortie un fichier contenant le graphe apres simulation et un pointeur sur la structure de graphe correspondante */
/* on simule de maniere synchrone la diffusion et les processus de transformation en calculant les deltas correspondants */
/* nvmax est le nombre de vosins maximums d'un noeud dans le graphe */
/* on retourne un graphe correspondant au graphe value apres simulation  synchrone de la diffusion et de la decomposition microbienne 
on retourne aussi un fichier contenant de nouveau graphe */

graphe_s *simulsynch(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,char *ficgrapher)
{
	graphe_s *graphe;
	int nbnoeud,i,j,k,n,*tavois,nvois;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,**tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,prop,**deltatbio,*deltatdif;
	FILE *ptficparbio; 
	float ro,mu,rom,vfom,vsom,vdom,kab,somdelta,somdeltabio,somdeltadif; 
	double massetot1,massetot2,rapport; 
	float temps,tempstot; 
	clock_t t1,t2; 

	graphe = lecgraphe(ficgraphe);

	nbnoeud = graphe->nbnoeuds; 

	ptficparbio = fopen(ficparbio,"r");

	fscanf(ptficparbio,"%f %f %f %f %f %f %f",&ro,&mu,&rom,&vfom,&vsom,&vdom,&kab);

	fprintf(stderr,"\n ro = %f ; mu = %f ; rom = %f ; vfom = %f ; vsom = %f ; vdom = %f ; kab = %f ",ro,mu,rom,vfom,vsom,vdom,kab);
	fprintf(stderr,"\n nombre de noeuds = %d",nbnoeud); 

	tabdelta = allotab2f(5,nbnoeud); 

	petitdeltat = deltat/fraction ;

	fprintf(stderr,"\n delta t = %f jours ",deltat); 

	fprintf(stderr,"\n petit delta t = %f jours",petitdeltat); 
	
	/* calcul des masses totales pour bilan des masses */

	massetot1 = bilanmasses(graphe); 

	fprintf(stderr,"\n");
	t1 = clock(); 
	tempstot = 0; 
	for(n = 0 ; n < fraction ; n++)
	{
		if(n % 1000 == 0)
		{
			fprintf(stderr,"n = %d ",n);
			t2 = clock(); 
			temps = (float)(t2 - t1) / CLOCKS_PER_SEC;
			temps = temps/60.0; 
			fprintf(stderr,"temps = %f minutes ",temps);
			tempstot += temps; 
			t1= clock(); 
		}
		/* initialisation du tableau des deltas à 0, le tableau est indexe a partir de 1 comme les noeuds du graphe */
		for(i=0 ; i<= nbnoeud; i++)
		{
			for(j=0; j <= 5 ; j++)
			{
				tabdelta[j][i] = 0; 
			}
			
		}
		/* pour chaque noeud on calcule la somme des deltas pour la diffusion et la biologie pour chaque intervalle (deltat/n) */

		deltatbio = miseajourbio(graphe,petitdeltat,ro,mu,rom,vfom,vsom,vdom,kab);

		deltatdif = miseajourdif(graphe,2,coeffdom,petitdeltat,nvmax);

		/* calcul de la somme des deltas dif */
		somdeltadif = 0; 
		for(i=1 ; i<= nbnoeud; i++)
		{
			somdeltadif += deltatdif[i]; 
		}

		/* fprintf(stderr,"\n somme des deltadif a la sortie de deltatdif = %f",somdeltadif); */

		/* determination du tableau des deltas prenant en compte la diffusion de la DOM et les transformations */ 
		somdelta = 0; 
		somdeltabio = 0;
		somdeltadif = 0; 
		for(i=1 ; i<= nbnoeud; i++)
		{
			for(j=1;j<=5;j++)
			{
				tabdelta[j][i] += deltatbio[j][i];
				somdelta += deltatbio[j][i];
				somdeltabio += deltatbio[j][i];
			}
			tabdelta[2][i] += deltatdif[i]; 
			somdelta += deltatdif[i];
			somdeltadif += deltatdif[i]; 
		}

		/* fprintf(stderr,"\n somme des deltas biologie = %f ; somme des deltas dif = %f",somdeltabio,somdeltadif); */
		/* ecrire la somme des deltats pour tous les noeuds du graphe qui doit etre nulle pour la conservation des masses */
		/* fprintf(stderr,"\n somme totale des deltas pour tous les noeuds du graphe = %f",somdelta); */

		/* mise a jour des attributs du graphe avec le tableau tabdelta */
		for(i = 1 ; i <= nbnoeud ; i++)
		{
			/* mise a jour des attributs avec le tableau des deltas */
			for(j=1;j<=5;j++)
			{
				graphe->carac[4+j][i] += tabdelta[j][i];
			}
		}

		/* On regarde pour chaque attribut si il existe des valeurs negatives
		et si la proportion n'est pas trop importante on rectifie */
		/* traitement de l'attribut numero j */

		for(j=1;j<=5;j++)
		{
			/* rectification des attributs en cas de valeurs negatives */
			masseneg = 0; 
			massetot = 0; 
			sommeconc = 0; 

			for(i=1;i<=nbnoeud;i++)
			{

				if(graphe->carac[4+j][i] < 0)
				{
					masseneg += -graphe->carac[4+j][i];
				}
				else
				{
					rboul = graphe->carac[4][i];
					volboul = (4.0/3.0)*3.1416*rboul*rboul*rboul; 
					gdif = graphe->carac[4+j][i]; 
					conc = gdif/volboul; 
					sommeconc += conc; 
					massetot += graphe->carac[4+j][i];
		
				}
			}
			if(masseneg > 0)
			{
				/* on traite les cas ou des valeurs de la caracteristique j seraient negatives */
				prop = (masseneg/massetot)*100 ;
				/* fprintf(stderr,"\n masse totale = %f ; masse negative = %f ; proportion de masse negative = %f",massetot,masseneg,prop); */
				if(prop > 20)
				{
					fprintf(stderr,"\n reduire le petit pas de temps car proportion de valeurs negatives trop importante = %f",prop);
					exit(3); 
				}
				/* rectification des masses negatives */
				for(i = 1 ; i <= nbnoeud ; i++)
				{
					if(graphe->carac[4+j][i] <= 0)
					{
						graphe->carac[4+j][i] = 0;
					}
					else
					{
						rboul = graphe->carac[4][i];
						volboul = (4.0/3.0)*3.1416*rboul*rboul*rboul; 
						gdif = graphe->carac[4+j][i]; 
						conc = gdif/volboul; 
						graphe->carac[4+j][i] -= (conc/sommeconc)*masseneg;
						if(graphe->carac[4+j][i] < 0)
						{
							fprintf(stderr,"\n rectification des valeurs negatives impossible, reduire le pas de temps");
							exit(1); 
						}
					}
				}
			}
		}
		/*  liberation de l'espace pointe par deltatbio et deltatdif */
		freetf(deltatbio,5,nbnoeud); 
		myfreef(deltatdif); 
	}
	/* le tableau des caracteristiques du graphe contient les valeurs des caracteristiques apres diffusion et 
	decomposition microbienne synchrones */

	fprintf(stderr,"\n temps total pour la simulation synchrone explicite = %f minutes",tempstot); 

	massetot2 = bilanmasses(graphe); 

	/* fprintf(stderr,"\n bilan des masses apres transformation-diffusion = %lf",massetot2); */

	if(massetot1 > massetot2)
	{
		rapport = massetot2/massetot1; 
	}
	else
	{
		rapport = massetot1/massetot2; 
	}
	fprintf(stderr,"\n rapport des bilans de masse entre 0 et 1 = %lf",rapport); 
	/* ecriture du nouveau graphe value apres diffusion, seules les valeurs des attributs ont pu changer */
	
	ecrigraphe(graphe,ficgrapher);
	fclose(ptficparbio); 
	return(graphe); 
}

/* cette procedure calcule les delta pour la diffusion */
/* elle prend en entree un graphe value, le numero d'un attribut a diffuse, le coefficient de diffusion, l'intervalle de temps,
le nombre de voisins maximum d'un noeud */

float *miseajourdif(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax)
{
	int nbnoeud,i,j,k,n,*tavois,nvois;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,*tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,massetot2,prop;
	double somdelta; 

	nbnoeud = graphe->nbnoeuds; 
	tavois = (int*)mycalloc(nvmax,sizeof(int));
	tabdelta = (float*)mycalloc(nbnoeud + 1,sizeof(float)); 

	/* calcul de la masse totale initiale de la caracteristique a diffuser */
	massetot = 0;
	for(i = 1 ; i <= nbnoeud ; i++)
	{
		massetot += graphe->carac[4+numcar][i];
	}
	/* fprintf(stderr,"\n masse totale de la caracteristique a diffuser = %f ; deltat = %f",massetot,deltat);*/

	/* initialisation de tabdelta a 0 */
	for(i=0 ; i<= nbnoeud; i++)
	{
		tabdelta[i] = 0; 
	}
	/* pour chaque noeud on calcule la somme des deltas avec tous ces voisins pour la diffusion pour deltat */

	for(i = 1 ; i <= nbnoeud ; i++)
	{
		/* fprintf(stderr,"\n i = %d",i);*/
		x1 = graphe->carac[1][i];
		y1 = graphe->carac[2][i];
		z1 = graphe->carac[3][i];
		rboul = graphe->carac[4][i];
		volboul = (4.0/3.0)*3.1416*rboul*rboul*rboul; 
		gdif = graphe->carac[4+numcar][i]; 
		conc = gdif/volboul; 
		/* volboul contient le volume de la boule, rboul son rayon, gdif la caractetristique a diffuser, conc la concentration */

		/* determination des voisins du noeud i */
		voisins(i,graphe,nvmax,&nvois,&tavois);
		/* fprintf(stderr,"\n nombre de voisins du noeud %d = %d",i,nvois); */
		/* les nvois voisins du noeud i sont ranges dans tavois */

		for(j=0;j<nvois;j++)
		{
			k = tavois[j];
			/* fprintf(stderr,"\n voisin du noeud %d est %d",i,k);*/
			x2 = graphe->carac[1][k];
			y2 = graphe->carac[2][k];
			z2 = graphe->carac[3][k];
			rboul2 = graphe->carac[4][k];

			/* fprintf(stderr,"\n rapport entre distance des centres et somme des rayons = %f",sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2))/(rboul+rboul2));*/ 

			/* traitement du flux entre i et k */

			volboul2 = (4.0/3.0)*3.1416*rboul2*rboul2*rboul2; 
			gdif2 = graphe->carac[4+numcar][k]; 
			conc2 = gdif2/volboul2; 
			/* calcul de la surface de contact */
			/* scont = (rboul>rboul2) ? (4*3.1416*rboul2*rboul2):(4*3.1416*rboul*rboul);*/
			scont = (2.0*rboul*rboul2)/(rboul+rboul2); 
				
			distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
			if(distance == 0)
			{
				fprintf(stderr,"\n distance entre deux centres de boules a zero !!");
				exit(10); 
			}
			delta = -(coeff*(conc-conc2)*scont*deltat)/distance;
			/* fprintf(stderr,"\n coeff = %f ; conc = %f ; conc2 = %f ; scont = %f ; deltat = %f ; distance = %f ; delta = %f ;",coeff,conc,conc2,scont,deltat,distance,delta);*/
			tabdelta[i] += delta;
			/* if(delta == 0)fprintf(stderr,"\n delta est egal a zero"); 
			fprintf(stderr,"\n tabdelta[%d] = %f",i,tabdelta[i]); */
		}
		/* fprintf(stderr,"\n"); */
	}
	/* tabdelta contient les delta de la diffusion pour tous les noeuds */

	/* somdelta = 0; 
	for(i=1;i<=nbnoeud;i++)
	{
		fprintf(stderr,"\n tabdelta[%d] = %f",i,tabdelta[i]); 
		somdelta += (double)tabdelta[i];
	}
	fprintf(stderr,"\n somme des deltas a la fin de miseajourdif = %lf \n",somdelta); 
	exit(10);*/
	myfreei(tavois); 
	return(tabdelta);
}

void testsimulsynch()
{
	graphe_s *graphe ;
	char ficgraphe[50],ficparbio[50],ficgrapher[50];
	float coeffdom,deltat;
	int fraction,nvmax ; 

	fprintf(stderr,"\n fichier graphe en entree ; fichier parametres bio ; fichier graphe en sortie: ");
	scanf("%s %s %s",ficgraphe,ficparbio,ficgrapher); 

	fprintf(stderr,"\n coeff diffusion (104665 en voxels carre par jour) ; deltat (en jours) ; fraction ; nb max voisins: ");
	scanf("%f %f %d %d",&coeffdom,&deltat,&fraction,&nvmax);

	graphe = simulsynch(ficgraphe,ficparbio,coeffdom,deltat,fraction,nvmax,ficgrapher);
}

/* cette procedure appelle la procedure de simulation qui rend les courbes echantillonnees en heures */
void testsimulsynchcourb()
{
	graphe_s *graphe ;
	char ficgraphe[50],ficparbio[50],ficgrapher[50],ficmasses[50],ficmatlab[50],ficexcel[50];
	float coeffdom,deltat;
	int fraction,nvmax,pourcneg ; 

	fprintf(stderr,"\n fichier graphe en entree ; fichier parametres bio ; fichier graphe en sortie: ; fichier masses en sortie ; fichier matlab en sortie ; fichier excel en sortie \n");
	scanf("%s %s %s %s %s %s",ficgraphe,ficparbio,ficgrapher,ficmasses,ficmatlab,ficexcel); 

	fprintf(stderr,"\n coeff diffusion (104665 en voxels carre par jour) ; deltat (en jours) ; fraction ; nb max voisins ; pourcentage max valeurs negatives : \n");
	scanf("%f %f %d %d %d",&coeffdom,&deltat,&fraction,&nvmax,&pourcneg);

	graphe = simulsynchcourb(ficgraphe,ficparbio,coeffdom,deltat,fraction,nvmax,pourcneg,ficgrapher,ficmasses,ficmatlab,ficexcel);
}



/* cette procedure lance la procedure de drainage methode valerie et rend un nouveau fichier de boules et le pourcentatge de saturation 
en porosite boules et en porosite voxels */
void testdrainageval()
{
	char ficdrai[50],ficboules[50];
	int dimx,dimy,dimz,nbmaxvois,i,nboules,iok,j,k,entier1,entier2,nforme1,nforme2,npoints,nbeau;
	float seuil,dseuil,voltotb1,voltotb2,voltotv1,voltotv2,x,y,z,r,pourcb,pourcv,x0,y0,z0;
	FILE *ptficdrai,*ptficboules; 
	imagec *ptimage1,*ptimage2; 
	boule *ptboule; 

	fprintf(stderr,"fichier de boules initial ; fichier de boules en sortie ; dimx ; dimy ; dimz ; nb max de voisins par noeuds ; seuil rayon ; seuil bord \n");
	scanf("%s %s %d %d %d %d %f %f",ficdrai,ficboules,&dimx,&dimy,&dimz,&nbmaxvois,&seuil,&dseuil);
	drainageval(ficdrai,dimx,dimy,dimz,seuil,dseuil,nbmaxvois,ficboules);

	/* calcul de la saturation */
	ptficdrai = fopen(ficdrai,"r");
	ptficboules = fopen(ficboules,"r");
	voltotb1 = 0;
	voltotb2 = 0;
	fscanf(ptficdrai,"%d",&nboules); 
	fscanf(ptficboules,"%d",&nbeau); 
	
	for(i=0;i<nboules;i++)
	{
		fscanf(ptficdrai,"%f %f %f %f",&x,&y,&z,&r); 
		voltotb1 += (4.0/3.0)*3.1416*r*r*r; 
	}
	for(i=0;i<nbeau;i++)
	{
		fscanf(ptficboules,"%f %f %f %f",&x,&y,&z,&r); 
		voltotb2 += (4.0/3.0)*3.1416*r*r*r; 
	}
	fprintf(stderr,"volume total des boules initial = %f ; volume total des boules d'eau apres drainage = %f \n",voltotb1,voltotb2);
	pourcb = (voltotb2/voltotb1)*100;
	fprintf(stderr,"Pourcentage de saturation boules = %f \n",pourcb); 

	fprintf(stderr,"Pour calcul de saturation voxels taper 1 sinon 0 : \n");
	scanf("%d",&iok);
	if(iok == 1)
	{
		fclose(ptficdrai);
		fclose(ptficboules);
		ptficdrai = fopen(ficdrai,"r");
		ptficboules = fopen(ficboules,"r");

		ptimage1 = creimagec(dimx,dimy,dimz,true);
		ptimage2 = creimagec(dimx,dimy,dimz,true);

		fscanf(ptficdrai,"%d",&nboules);
		fscanf(ptficboules,"%d",&nbeau);

		ptboule = (boule*)mymalloc(sizeof(boule));

		fprintf(stderr,"\n nboules = %d nboules eau = %d",nboules,nbeau);


		for(i=0;i<nboules;i++)

		{
			fscanf(ptficdrai,"%f %f %f %f",&x0,&y0,&z0,&r);

			ptboule->xc = x0;

			ptboule->yc = y0;

			ptboule->zc = z0;

			ptboule->rayon = r;

			npoints = boulimagb(ptboule,ptimage1,false);
		}

		for(i=0;i<nbeau;i++)
		{
			fscanf(ptficboules,"%f %f %f %f",&x0,&y0,&z0,&r);

			ptboule->xc = x0;

			ptboule->yc = y0;

			ptboule->zc = z0;

			ptboule->rayon = r;

			npoints = boulimagb(ptboule,ptimage2,false);

		}
		/* ptimage1 pointe sur l'image contenant les boules initiales et ptimage2 sur l'image contenant les boules finales */

		nforme1 = 0;
		nforme2 = 0; 

		for(k=0;k<dimz;k++)
    	{
	   	 	for(j=0;j<dimy;j++)
        	{
				for(i=0;i<dimx;i++)
	        	{
		        	entier1 = (ptimage1->tab)[i][j][k];
					entier2 = (ptimage2->tab)[i][j][k];
		        	if(entier1 == false)
                	{
                    	nforme1++;
					}
					if(entier2 == false)
                	{
                    	nforme2++;
					}
				}       
            }
		}
		/* calcul de la saturation voxels */

		pourcv = ((float)nforme2/(float)nforme1)*100; 

		fprintf(stderr,"\n Saturation voxels = %f",pourcv);
    }

	fclose(ptficdrai);
	fclose(ptficboules); 

}

/* cette procedure draine avec la methode de Valerie.
entrees: le fichier de boules en format (x,y,z,r) ; les dimensions de l'image (dimx,dimy,dimz) ; le nombre max de voisins par noeuds 
sortie : le fichier ficboules contenant les boules pleines d'eau
*/
void drainageval(char *ficdrai,int dimx,int dimy, int dimz, float seuil, float dseuil,int nbmaxvois,char *ficboules)
{
			int nbarcs,**trait,optdif,i; 
			graphe_s *graphe; 


			/* la procedure arcs prend en entree le nom du fichier de boules, les dimensions de l'image
			 contenant les boules et ecrit en sortie le nombre d'arcs "nbarcs" et un fichier contenant le nombre d'arcs et la liste des arcs
			 */
		
			arcsval(ficdrai,"trav", dimx,dimy,dimz,&nbarcs,nbmaxvois);

			fprintf(stderr,"\n nombre d'arcs = %d ",nbarcs);

			graphe = cregraphe_s4_val(ficdrai,"trav");

			fprintf(stderr,"\n fin de la cr�ation du graphe");

			trait = drainit2(graphe,seuil,dseuil,dimx,dimy,dimz);

			verifdrain(graphe,trait,seuil,&optdif);

			fprintf(stderr,"\n nb d'anomalies = %d",optdif);

			ficdrain_val(graphe,trait,ficboules,seuil);

			remove("trav"); 
}

/* meme chose que cregraphe_s4 mais avec un fichier de boules format (x,y,z,r) */
graphe_s *cregraphe_s4_val(char *cyl,char *arcs)
{

	int nbarmax,nbnomax,ncar,i,j;

	int k,nbarcs,n,natt ;

	float *car;

	float r,x1,y1,z1;

	float t1,t2,t3,t4;

	graphe_s *graphe1;

	FILE *ptcyl,*ptarcs;


	/* ouverture du fichier des boules */
	ptcyl = fopen(cyl,"r");

	/* ouverture du fichier des arcs */
	ptarcs = fopen(arcs,"r");


	/* lecture du nombre de boules */
	fscanf(ptcyl,"%d",&nbnomax);

	/* lecture du nombre d'arcs */
	fscanf(ptarcs,"%d",&nbarcs);


	fprintf(stderr,"\n nb de boules= %d nb d'arcs = %d ",nbnomax,nbarcs);

	nbarmax = nbarcs +1;

	ncar = 4;

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

	fclose(ptarcs); 

	return(graphe1);

}


/* cette procedure cr�e un fichier de boules drain�es en fonction du tableau trait et du graphe 
meme chose que ficdrain mais rend un fichier de boules format (x,y,z,r) */
void ficdrain_val(graphe_s *graphe,int **trait,char *fic,float seuil)
{
	int i,nboules,nbouleau,nvtot;
	FILE *ptfic;

	nboules = graphe->nel;

	ptfic = fopen(fic,"w");

	nbouleau = 0;
	nvtot = 0;
	/* on compte le nombre de boules d'eau */
	for(i=1;i<=nboules;i++)
	{
		/* traitement de la boule i */
		if(trait[1][i] == 1)
		{
			nbouleau++;
			if(graphe->carac[4][i] > seuil)nvtot++;
		}

	}

	fprintf(stderr,"\n nombre de boules d'eau = %d nb de boules d'eau avec rayon > seuil = %d ",nbouleau,nvtot);

	/* on �crit le nombre de boules d'eau en tete du fichier */
	fprintf(ptfic,"%d \n",nbouleau);


	for(i=1;i<=nboules;i++)
	{
		/* traitement de la boule i */
		if(trait[1][i] == 1)
		{
			/* cas d'une boule remplie d'eau on l'ecrit dans le fichier pour la simulation biologique */
			fprintf(ptfic,"%f %f %f %f \n",(float)graphe->carac[1][i],(float)graphe->carac[2][i],(float)graphe->carac[3][i],(float)graphe->carac[4][i]);
		}


	}
	fclose(ptfic);
}

/* cette procedure effectue le drainage methode seuillage des rayons et rend un nouveau fichier de boules et le pourcentatge de saturation 
en porosite boules et en option en porosite voxels, les boules de rayon <= seuil sont conservees */
void testdrainagesimp()
{
	char ficdrai[50],ficboules[50];
	int dimx,dimy,dimz,nbmaxvois,i,nboules,iok,j,k,entier1,entier2,nforme1,nforme2,npoints,nbeau,nboulesdrai;
	float seuil,dseuil,voltotb1,voltotb2,voltotv1,voltotv2,x,y,z,r,pourcb,pourcv,x0,y0,z0;
	FILE *ptficdrai,*ptficboules; 
	imagec *ptimage1,*ptimage2; 
	boule *ptboule; 

	fprintf(stderr,"fichier de boules initial ; fichier de boules en sortie ; dimx ; dimy ; dimz ; seuil rayon \n");
	scanf("%s %s %d %d %d %f",ficdrai,ficboules,&dimx,&dimy,&dimz,&seuil);

	ptficdrai = fopen(ficdrai,"r");
	ptficboules = fopen(ficboules,"w");

	voltotb1 = 0;
	voltotb2 = 0;

	fscanf(ptficdrai,"%d",&nboules); 

	nboulesdrai = 0; 
	for(i=0;i<nboules;i++)
	{
		fscanf(ptficdrai,"%f %f %f %f",&x,&y,&z,&r); 
		if(r <= seuil)nboulesdrai++; 
	}
	fprintf(stderr,"\n nombre de boules d'eau apres drainage simplifie = %d ",nboulesdrai); 

	fprintf(ptficboules,"%d \n",nboulesdrai);

	fclose(ptficdrai);
	ptficdrai = fopen(ficdrai,"r");

	fscanf(ptficdrai,"%d",&nboules); 

	for(i=0;i<nboules;i++)
	{
		fscanf(ptficdrai,"%f %f %f %f",&x,&y,&z,&r); 
		voltotb1 += (4.0/3.0)*3.1416*r*r*r; 
		if(r <= seuil)
		{
			fprintf(ptficboules,"%f %f %f %f \n",x,y,z,r); 
			voltotb2 += (4.0/3.0)*3.1416*r*r*r; 
		}
	}

	/* le fichier de sortie des boules apres le drainage simplifie est rempli */

	fprintf(stderr,"volume total des boules initial = %f ; volume total des boules d'eau apres drainage simplifie = %f \n",voltotb1,voltotb2);
	pourcb = (voltotb2/voltotb1)*100;
	fprintf(stderr,"Pourcentage de saturation boules = %f \n",pourcb); 

	fprintf(stderr,"Pour calcul de saturation voxels taper 1 sinon 0 : \n");
	scanf("%d",&iok);
	if(iok == 1)
	{
		fclose(ptficdrai);
		fclose(ptficboules);
		ptficdrai = fopen(ficdrai,"r");
		ptficboules = fopen(ficboules,"r");

		ptimage1 = creimagec(dimx,dimy,dimz,true);
		ptimage2 = creimagec(dimx,dimy,dimz,true);

		fscanf(ptficdrai,"%d",&nboules);
		fscanf(ptficboules,"%d",&nbeau);

		ptboule = (boule*)mymalloc(sizeof(boule));

		fprintf(stderr,"\n nboules = %d nboules eau = %d",nboules,nbeau);


		for(i=0;i<nboules;i++)

		{
			fscanf(ptficdrai,"%f %f %f %f",&x0,&y0,&z0,&r);

			ptboule->xc = x0;

			ptboule->yc = y0;

			ptboule->zc = z0;

			ptboule->rayon = r;

			npoints = boulimagb(ptboule,ptimage1,false);
		}

		for(i=0;i<nbeau;i++)
		{
			fscanf(ptficboules,"%f %f %f %f",&x0,&y0,&z0,&r);

			ptboule->xc = x0;

			ptboule->yc = y0;

			ptboule->zc = z0;

			ptboule->rayon = r;

			npoints = boulimagb(ptboule,ptimage2,false);

		}
		/* ptimage1 pointe sur l'image contenant les boules initiales et ptimage2 sur l'image contenant les boules finales */

		nforme1 = 0;
		nforme2 = 0; 

		for(k=0;k<dimz;k++)
    	{
	   	 	for(j=0;j<dimy;j++)
        	{
				for(i=0;i<dimx;i++)
	        	{
		        	entier1 = (ptimage1->tab)[i][j][k];
					entier2 = (ptimage2->tab)[i][j][k];
		        	if(entier1 == false)
                	{
                    	nforme1++;
					}
					if(entier2 == false)
                	{
                    	nforme2++;
					}
				}       
            }
		}
		/* calcul de la saturation voxels */

		pourcv = ((float)nforme2/(float)nforme1)*100; 

		fprintf(stderr,"\n Saturation voxels = %f",pourcv);
    }

	fclose(ptficdrai);
	fclose(ptficboules); 

}



/* determine tous les arcs du graphe et les stocke dans le fichier cyl2 qui contient la suite des arcs avec les numeros de noeud
 * dans l'ordre croissant, le fichier des boules a le format: nb de boules, (x,y,z,r)
 la methode utilisee ne passe pas par l'ecriture des images mais par les boules directement */

void arcssimp(char *cyl,char *cyl2,int dimx, int dimy, int dimz,int *nbarcs,int nmaxv,float seuilr)

{

	FILE *ptboules,*ptarcs,*pttrav;

	int nboules,i,npoints,l;

	float x0,y0,z0,r,a2,u,v,w,*Xb,*Yb,*Zb,*Rb,rap;

	int dx,dy,dz,xc,yc,zc,m ;

	int j,k,a,b,c,i1,j1,k1,i2,j2,k2,*voisins,nv,evois,p,stop;


	*nbarcs = 0;


	ptboules = fopen(cyl,"r");

	ptarcs = fopen(cyl2,"w");

	pttrav = fopen("arc.trav","w");

	fscanf(ptboules,"%d",&nboules);

	fprintf(stderr,"\n nboules = %d",nboules);

	Xb = (float*)mycalloc(nboules + 1,sizeof(float)); 
	Yb = (float*)mycalloc(nboules + 1,sizeof(float)); 
	Zb = (float*)mycalloc(nboules + 1,sizeof(float)); 
	Rb = (float*)mycalloc(nboules + 1,sizeof(float)); 


	for(i=1;i<=nboules;i++)

	{

		fscanf(ptboules,"%f %f %f %f",&x0,&y0,&z0,&r);

		Xb[i] = x0;
		Yb[i] = y0;
		Zb[i] = z0;
		Rb[i] = r; 

	}

	fclose(ptboules); 

	fprintf(stderr,"\n fin remplissage de Xb,Yb,Zb,Rb");


	/* remplissage du fichier des arcs dans arc.trav */


	for(j=1;j<= (nboules - 1);j++)
	{
		/* determination des arcs incluant la boule l */
		for(k=j+1;k<=nboules;k++)
		{
			rap = sqrt((Xb[j]-Xb[k])*(Xb[j]-Xb[k])+(Yb[j]-Yb[k])*(Yb[j]-Yb[k])+(Zb[j]-Zb[k])*(Zb[j]-Zb[k]))/(Rb[j]+Rb[k]);
			if(rap <= seuilr)
			{
				/* creation d'un arc */
				fprintf(pttrav,"\n %d %d",j,k);
				(*nbarcs)++;
			}

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


	fclose(ptarcs);

	fclose(pttrav);


	remove("arc.trav");

	free(Xb);
	free(Yb);
	free(Zb);
	free(Rb);
}

/* routines pour la simulation de la dynamique biologique */

/* cette fonction cree le graphe des boules 
entree: nom du fichier des boules maximums, attributs des boules maximum autres que leur centres et rayons (masses, biomasse...)
sortie: fichier contenant le graphe correspondant au fichier des boules et des attributs */

void testcregraphsoilmu3d2()

{
	char ficboules[50],ficatt[50],racine[50],chaine[50];
	char ficgraphe[50],ficarcs[50];
	graphe_s *graphe;
	int nbarcs,dimx,dimy,dimz,nbmaxvois;
	float seuil; 


    /* on lit le fichier des boules maximum, les attributs noeuds initiaux (fichier), les parametres du modele biologique (fichier), l'intervalle de temps total
    ainsi que la discretisation voulue (fichier) */

    fprintf(stderr,"\n racine des fichiers : ");
	scanf("%s",racine);

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(ficboules,chaine);
	fprintf(stderr,"\n fichier de boules maximales en entree (x,y,z,r) : %s ",ficboules);

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
	fprintf(stderr,"\n seuil pour connexite: distance centres/somme rayons <= ");
	scanf("%f",&seuil); 
    arcssimp(ficboules,ficarcs, dimx,dimy,dimz,&nbarcs,nbmaxvois,seuil);
	fprintf(stderr,"\n nombre d'arcs du graphe = %d",nbarcs);
    
    /* creation du graphe correspondant a ficboules et a ficatt, on a besoin de ficboules pour ecrire les caracteristiques des boules */
    graphe = cregraphe_val(ficboules,ficarcs,ficatt);
	fprintf(stderr,"\n graphe->nbnoeuds = %d ; graphe->nel = %d ; graphe->ncar = %d",graphe->nbnoeuds,graphe->nel,graphe->ncar);

    /* ecriture du graphe dans le fichier ficgraphe */ 
    ecrigraphe(graphe,ficgraphe);
}

/* cette procedure genere un fichier de courbes des caracteristiques correspondant a un fichier de graphe, ce fichier contient
les valeurs des masses en pourcentage */
void calsommes3f(char *ficgraphe,char *ficcourbes)
{
	float *car1,somme,som;
	int nat,nboules,i,j;
	FILE *ptgraphe,*ptficcourbes;
	graphe_s *graphe; 

	graphe = lecgraphe(ficgraphe);

	nboules = graphe->nbnoeuds;
	/* nboules contient le nombre de boules */

	nat = graphe->ncar - 4;
	/* natt contient le nombre d'attributs complementaires */

	car1 = (float*)mycalloc(nat,sizeof(float));

	for(i=0;i<nat;i++)car1[i]=0;

	fprintf(stderr,"\n bilan des masses = %f",som); 
	/* traitement iteratif des noeuds */
	somme = 0.0; 
	for(i=1;i<=nboules;i++)
	{
		for(j=0;j<nat;j++)
		{
			car1[j] += graphe->carac[j+5][i];
			somme += graphe->carac[j+5][i]; 
		}
	}
	fprintf(stderr,"\n somme totale des masses = %f",somme);
	/* somme contient la somme totale des masses et car1 les sommes associees aux attributs */

	ptficcourbes = fopen(ficcourbes,"w"); 

	fprintf(ptficcourbes,"%d",nat);
	fprintf(ptficcourbes,"\n");

	/* calcul des pourcentages de la masse totale des attributs du graphe */
	
	for(j=0;j<nat;j++)
	{
		fprintf(ptficcourbes,"%f ",(car1[j]/somme)*100);
	}
	fclose(ptficcourbes);
}

void testcalsommes3f()
{
	char ficgraphe[50],ficcourbes[50]; 

	fprintf(stderr,"\n fichier graphe en entree ; fichier courbes en sortie : \n ");
	scanf("%s %s",ficgraphe,ficcourbes); 
	calsommes3f(ficgraphe,ficcourbes);
}


/* cette procedure prend en entree un fichier contenant un graphe value de boules avec les attributs complementaires
(MB,DOM,SOM,FOM,CO2) (unite g), un fichier contenant les parametres du modele biologique
(vdom,kab,mu,ro,rom) (j-1,gCg-1,j-1,j-1), le coefficient de diffusion de la DOM, un intervalle de temps (en jours) 
une fraction de l'intervalle de temps pour le schema explicite, et rend en
sortie un fichier contenant le graphe apres simulation, un pointeur sur la structure de graphe correspondante, un fichier contenant la suite
des valeurs des masses sans pourcentages, un fichier contenant les courbes
matlab de la variation des masses des attributs (en pourcentage de la masse totale, echantillonage en heures), un fichier contenant la meme chose mais pour excel */
/* on simule de maniere synchrone la diffusion et les processus de transformation en calculant les deltas correspondants */
/* nvmax est le nombre de vosins maximums d'un noeud dans le graphe */
/* on retourne un graphe correspondant au graphe value apres simulation  synchrone de la diffusion et de la decomposition microbienne 
on retourne aussi un fichier contenant le nouveau graphe
l'unite de temps est le jour et l'echantillonnage pour les courbes est l'heure */

graphe_s *simulsynchcourb(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel)
{
	graphe_s *graphe;
	int nbnoeud,i,j,k,n,*tavois,nvois,ech,natt;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,**tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,prop,**deltatbio,*deltatdif,car1[5],**car;
	FILE *ptficparbio,*ptficmatlab,*ptficexcel,*ptficmasses; 
	float ro,mu,rom,vfom,vsom,vdom,kab,somdelta,somdeltabio,somdeltadif,somtra; 
	double massetot1,massetot2,rapport,massetr; 
	float temps,tempstot,totacarb,massett; 
	clock_t t1,t2,t3,t4,t5; 
	int n1,n2,fracjour,nbels; 

	t3 = clock(); 

	graphe = lecgraphe(ficgraphe);

	nbnoeud = graphe->nbnoeuds; 

	ptficparbio = fopen(ficparbio,"r");

	ptficmatlab = fopen(ficmatlab,"w");

	ptficexcel = fopen(ficexcel,"w");

	ptficmasses = fopen(ficmasses,"w");

	fscanf(ptficparbio,"%f %f %f %f %f %f %f",&ro,&mu,&rom,&vfom,&vsom,&vdom,&kab);

	fprintf(stderr,"\n ro=%f ; mu=%f ; rom=%f ; vfom=%f ; vsom=%f ; vdom=%f ; kab=%f ",ro,mu,rom,vfom,vsom,vdom,kab);
	fprintf(stderr,"\n nombre de noeuds=%d",nbnoeud); 

	tabdelta = allotab2f(5,nbnoeud); 

	petitdeltat = deltat/fraction ;

	ech = neari((1.0/24.0)*(1/petitdeltat)); 

	/* ech represente le nombre d'iterations correspondant a une heure de temps */

	fprintf(stderr,"\n deltat=%f jours ",deltat); 

	fprintf(stderr,"\n petitdeltat=%f jours ; ech=%d",petitdeltat,ech); 
	
	/* calcul des masses totales pour bilan des masses */

	massetot1 = bilanmasses(graphe); 

	/* fprintf(stderr,"\n bilan total des masses au debut de simulsynchcourb = %lf",massetot1); */

	/* fprintf(stderr,"\n");*/
	t1 = clock(); 
	tempstot = 0; 

	natt = 5; 
	nbels = fraction/ech + 1; 
	car = allot2f(nbels + 1,natt);

	n1 =0; 

	for(n = 0 ; n < fraction ; n++)
	{
		if(n % 1000 == 0)
		{
			t2 = clock(); 
			temps = (float)(t2 - t1) / CLOCKS_PER_SEC;
			temps = temps/60.0; 
			tempstot += temps; 
			/* fprintf(stderr,"\n temps de calcul= %f minutes",tempstot);*/
			t1= clock(); 
		}
		/* initialisation du tableau des deltas à 0, le tableau est indexe a partir de 1 comme les noeuds du graphe */
		for(i=0 ; i<= nbnoeud; i++)
		{
			for(j=0; j <= 5 ; j++)
			{
				tabdelta[j][i] = 0; 
			}
			
		}
		/* pour chaque noeud on calcule la somme des deltas pour la diffusion et la biologie pour chaque intervalle (deltat/n) */

		deltatbio = miseajourbio(graphe,petitdeltat,ro,mu,rom,vfom,vsom,vdom,kab);

		/* calcul de la somme des deltas pour les MB */
		/* fprintf(stderr,"\n");*/
		/* somtra = 0; 
		for(i=1;i<=nbnoeud;i++)
		{
			somtra += deltatbio[1][i];
		}
		fprintf(stderr,"som=%f ",somtra); 
		fprintf(stderr,"\n");*/

		deltatdif = miseajourdif(graphe,2,coeffdom,petitdeltat,nvmax);

		/* deltatdif = (float*)mycalloc(nbnoeud + 1,sizeof(float)); 
		for(i=0;i<nbnoeud+1;i++)
		{
			deltatdif[i] = 0.0; 
		}*/

		/* calcul de la somme des deltas dif */
		somdeltadif = 0; 
		for(i=1 ; i<= nbnoeud; i++)
		{
			somdeltadif += deltatdif[i]; 
		}

		/* fprintf(stderr,"\n somme des deltadif a la sortie de deltatdif = %f",somdeltadif); */

		/* determination du tableau des deltas prenant en compte la diffusion de la DOM et les transformations */ 
		somdelta = 0; 
		somdeltabio = 0;
		somdeltadif = 0; 
		for(i=1 ; i<= nbnoeud; i++)
		{
			for(j=1;j<=5;j++)
			{
				tabdelta[j][i] += deltatbio[j][i];
				somdelta += deltatbio[j][i];
				somdeltabio += deltatbio[j][i];
			}
			tabdelta[2][i] += deltatdif[i]; 
			somdelta += deltatdif[i];
			somdeltadif += deltatdif[i]; 
		}

		/* fprintf(stderr,"\n somme des deltas biologie = %f ; somme des deltas dif = %f",somdeltabio,somdeltadif); */
		/* ecrire la somme des deltats pour tous les noeuds du graphe qui doit etre nulle pour la conservation des masses */
		/* fprintf(stderr,"\n somme totale des deltas pour tous les noeuds du graphe = %f",somdelta); */

		/* mise a jour des attributs du graphe avec le tableau tabdelta */
		for(i = 1 ; i <= nbnoeud ; i++)
		{
			/* mise a jour des attributs avec le tableau des deltas */
			for(j=1;j<=5;j++)
			{
				graphe->carac[4+j][i] += tabdelta[j][i];
			}
		}

		/* On regarde pour chaque attribut si il existe des valeurs negatives
		et si la proportion n'est pas trop importante on rectifie */
		/* traitement de l'attribut numero j */

		for(j=1;j<=5;j++)
		{
			/* rectification des attributs en cas de valeurs negatives */
			masseneg = 0; 
			massetot = 0; 
			massett = 0; 
			for(i=1;i<=nbnoeud;i++)
			{
				if(graphe->carac[4+j][i] < 0)
				{
					masseneg += -graphe->carac[4+j][i];
				}
				else
				{
					massetot += graphe->carac[4+j][i];
				}
			}
			massett = massetot - masseneg ; 
			if(massett < 0)
			{
				fprintf(stderr,"\n masse totale de l'attribut %d negative = %f",j,massett);
				exit(4);
			}
			if(masseneg > 0)
			{
				/* on traite les cas ou des valeurs de la caracteristique j seraient negatives */
				prop = (masseneg/massetot)*100 ;
				/* fprintf(stderr,"\n masse totale = %f ; masse negative = %f ; proportion de masse negative = %f",massetot,masseneg,prop); */
				if(prop > pourcneg)
				{
					fprintf(stderr,"\n reduire le petit pas de temps car proportion de valeurs negatives = %f",prop);
					fprintf(stderr,"\n"); 
					exit(3); 
				}
				/* rectification des masses negatives */
				for(i = 1 ; i <= nbnoeud ; i++)
				{
					if(graphe->carac[4+j][i] <= 0)
					{
						graphe->carac[4+j][i] = 0;
					}
					else
					{
						gdif = graphe->carac[4+j][i]; 
						graphe->carac[4+j][i] -= (gdif/massetot)*masseneg;
						if(graphe->carac[4+j][i] < 0)
						{
							fprintf(stderr,"\n rectification des valeurs negatives impossible, reduire le pas de temps");
							fprintf(stderr,"\n j = %d",j); 
							exit(1); 
						}
					}
				}
			}
		}
		/*  liberation de l'espace pointe par deltatbio et deltatdif */
		freetf(deltatbio,5,nbnoeud); 
		myfreef(deltatdif); 

		/* ecriture dans le fichier des courbes, on ecrit toutes les heures */
		if((n%ech) == 0)
		{
			/* calcul des sommmes des attributs pour le temps t0 + n*petitdeltat */
			for(j=0;j<5;j++)
			{
				car1[j] = 0; 
			}
			for(i=1;i<=nbnoeud;i++)
			{
				for(j=0;j<5;j++)
				{
					car1[j] += graphe->carac[j+5][i];
				}
			}
		
			/* car1 contient les sommmes des valeurs des attributs a l'instant n*petideltat, massetot1 contient le total des masses */

			/* remplissage du fichier des masses, sur une ligne on a la valeur des masses puis les pourcentages */
			fprintf(stderr,"\n temps de la simulation en jours =%f ",(n+1)*petitdeltat); 

			t5 = clock(); 
			temps = (float)(t5 - t3) / CLOCKS_PER_SEC;
			temps = temps/60.0;
			fprintf(stderr,"\n temps de calcul intermediaire pour la simulation synchrone explicite = %f' ",temps); 

			fprintf(ptficmasses,"%f \n",(n+1)*petitdeltat); 
			fprintf(stderr,"\n"); 
			for(j=0;j<5;j++)
			{
				fprintf(ptficmasses,"%f ",car1[j]);
				fprintf(stderr,"%f ",car1[j]);
				car[n1][j] = car1[j];
			}
			n1++; 
			fprintf(stderr,"\n"); 
			fprintf(ptficmasses,"\n");
			for(j=0;j<5;j++)
			{
				fprintf(ptficmasses,"%f ",(car1[j]/massetot1)*100);
				fprintf(stderr,"%f ",(car1[j]/massetot1)*100);
			}
			fprintf(ptficmasses,"\n");
			fprintf(stderr,"\n");
			fprintf(ptficmasses,"\n");
			fprintf(stderr,"\n");
			massetot1 = bilanmasses(graphe); 
			/* fprintf(stderr,"\n bilan total des masses = %lf",massetr); */
		}
	}
	/* a la fin de la boucle le tableau car[i][j] contient pour le temps numero i (en heures) la somme des attributs adresses par j */
	totacarb = massetot1; 
	fracjour = 24; 
	nbels = n1; 
	/* fprintf(stderr,"\n nbels = %d",nbels); */
	for(i=0;i<nbels;i++)
	{
		fprintf(ptficexcel,"%f ",(float)(i)/(float)(fracjour));
		for(j=0;j<natt;j++)
		{
			car[i][j] = (car[i][j]/totacarb)*100;
			fprintf(ptficexcel,"%f ",car[i][j]);
		}
		fprintf(ptficexcel,"\n");
	}

	/* on genere les instructions matlab */

	/* on ecrit la valeur de X */
	fprintf(ptficmatlab,"X = [");
	for(i = 0;i<nbels-1;i++)fprintf(ptficmatlab,"%f ",(float)(i));
	fprintf(ptficmatlab,"%f];",(float)(nbels-1));

	/* on ecrit Y0,Y1,Y2,Y3,Y4,Y5 */

	/* on ecrit la valeur de Y0 */
	fprintf(ptficmatlab,"\nY0 = [");
	for(i = 0;i<nbels-1;i++)fprintf(ptficmatlab,"%f ",car[i][0]);
	fprintf(ptficmatlab,"%f];",car[nbels-1][0]);

	fprintf(ptficmatlab,"\n");

	fprintf(ptficmatlab,"Y1 = [");
	for(i = 0;i<nbels-1;i++)fprintf(ptficmatlab,"%f ",car[i][1]);
	fprintf(ptficmatlab,"%f];",car[nbels-1][1]);

	fprintf(ptficmatlab,"\n");

	fprintf(ptficmatlab,"Y2 = [");
	for(i = 0;i<nbels-1;i++)fprintf(ptficmatlab,"%f ",car[i][2]);
	fprintf(ptficmatlab,"%f];",car[nbels-1][2]);

	fprintf(ptficmatlab,"\n");

	fprintf(ptficmatlab,"Y3 = [");
	for(i = 0;i<nbels-1;i++)fprintf(ptficmatlab,"%f ",car[i][3]);
	fprintf(ptficmatlab,"%f];",car[nbels-1][3]);

	fprintf(ptficmatlab,"\n");

	fprintf(ptficmatlab,"Y4 = [");
	for(i = 0;i<nbels-1;i++)fprintf(ptficmatlab,"%f ",car[i][4]);
	fprintf(ptficmatlab,"%f];",car[nbels-1][4]);

	fprintf(ptficmatlab,"\n");
	/* on a ecrit Y0,Y1,Y3,Y5 */

	/* on appelle la fonction plot */

	fprintf(ptficmatlab,"plot(X,Y0,'b',X,Y1,'g',X,Y2,'r',X,Y3,'c',X,Y4,'c');");
	fprintf(ptficmatlab,"\n");

	t4= clock(); 
	temps = (float)(t4 - t3) / CLOCKS_PER_SEC;
	temps = temps/60.0;

	fprintf(stderr,"\n temps total pour la simulation synchrone explicite = %f minutes",temps); 

	massetot2 = bilanmasses(graphe); 

	/* fprintf(stderr,"\n bilan des masses apres transformation-diffusion = %lf",massetot2); */

	if(massetot1 > massetot2)
	{
		rapport = massetot2/massetot1; 
	}
	else
	{
		rapport = massetot1/massetot2; 
	}
	fprintf(stderr,"\n rapport des bilans de masse entre 0 et 1 = %lf",rapport); 
	/* ecriture du nouveau graphe value apres diffusion, seules les valeurs des attributs ont pu changer */
	
	ecrigraphe(graphe,ficgrapher);

	fclose(ptficparbio); 
	fclose(ptficmatlab);
	fclose(ptficexcel);
	fclose(ptficmasses);

	return(graphe); 
}
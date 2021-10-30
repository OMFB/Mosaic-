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

void testcreerattvalbis()
{
    char ficdonnees[50],ficboules[50],ficatt[50];
    FILE *ptficboules,*ptficatt,*ptficdonnees; 
	float rmax,massedom; 

    fprintf(stderr,"fichier de donnees (entree,format 'Xavier'), fichier de boules (entree), fichier d'attributs (sortie) :  ; massedom en microng = ; rayon max en voxel = ");
    scanf("%s %s %s %f %f",ficdonnees,ficboules,ficatt,&massedom,&rmax); 
    fprintf(stderr,"\n fichier de donnees: %s , fichier de boules: %s, fichier d'attributs: %s ; masse dom = %f ; rayon max = %f",ficdonnees,ficboules,ficatt,massedom,rmax);

    creerattvalbis(ficdonnees,ficboules,ficatt,massedom,rmax);
}

void testcreerattval()
{
    char ficdonnees[50],ficboules[50],ficatt[50];
    FILE *ptficboules,*ptficatt,*ptficdonnees; 
	float rmax,massedom; 

    fprintf(stderr,"fichier de donnees (entree,format 'Xavier'), fichier de boules (entree), fichier d'attributs (sortie) :  ; massedom en microng = ; rayon max en voxel = ");
    scanf("%s %s %s %f %f",ficdonnees,ficboules,ficatt,&massedom,&rmax); 
    fprintf(stderr,"\n fichier de donnees: %s , fichier de boules: %s, fichier d'attributs: %s ; masse dom = %f ; rayon max = %f",ficdonnees,ficboules,ficatt,massedom,rmax);

    creerattval(ficdonnees,ficboules,ficatt,massedom,rmax);

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

void testcreeratt6()
{
    char ficdonnees[50],ficboules[50],ficatt[50];
    FILE *ptficboules,*ptficatt,*ptficdonnees; 

    fprintf(stderr,"fichier de donnees (entree,MB,DOM,SOM,FOM), fichier de boules (entree), fichier d'attributs (sortie) :  ");
    scanf("%s %s %s",ficdonnees,ficboules,ficatt); 
    fprintf(stderr,"\n fichier de donnees: %s , fichier de boules: %s, fichier d'attributs: %s ",ficdonnees,ficboules,ficatt);

    creeratt6(ficdonnees,ficboules,ficatt);

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
si ce nombre de boules est -1 (seulement pour les MB ou la DOM) alors la masse est repartie de manière uniforme sur l'ensemble des boules proportionellement au volume
Si ce nombre est negatif (seulement pour les MB ou la DOM) n repartit la masse sur les premieres boules (-nombre +1) 
*/

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


/* cette procedure fait la meme chose que creeratt5 mais en prenant les dernieres boules */ 

void creeratt6(char *ficdonnees, char *ficboules,char *ficatt)
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
			sommevolmic += trav[0][nboules-i];
			boulmic[i] = nboules - i; 
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

/* creation d'un graphe pour la simulation biologique a partir d'un fichier de boules, d'un fichier d'arcs,
 d'un fichier d'attributs des pores (nb d'attributs complementaires pour chaque pore en dehors du rayon de la boule,
 liste des attributs complementaires). Le graphe cree est pour la simulation donc
 comprend un tas vide */

 /* CREATION D'UN GRAPHE SANS ATTRIBUTS COMPLEMENTAIRES A PARTIR D'UN FICHIER DE BOULES ET DU FICHIER DES ARCS */

graphe_s *cregraphe_satt(char *cyl,char *arcs)

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

	fprintf(stderr,"\n fin ouverture du fichier des boules: %s",cyl); 

	/* ouverture du fichier des arcs */
	ptarcs = fopen(arcs,"r");

	fprintf(stderr,"\n fin ouverture du fichier des arcs: %s",arcs); 

	/* lecture du nombre de boules */
	fscanf(ptcyl,"%d",&nbnomax);

	fprintf(stderr,"\n fin lecture du nombre de boules"); 

	/* lecture du nombre d'arcs */
	fscanf(ptarcs,"%d",&nbarcs);

	fprintf(stderr,"\n nb de boules= %d nb d'arcs = %d ",nbnomax,nbarcs);

	nbarmax = nbarcs +1;

	ncar =  4;

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

	fclose(ptarcs);

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
	float massetot,masseneg,sommeconc,massetot2,prop,rs;
	double masset1,masset2,rapport;
	

	graphe = lecgraphe(ficgraphe1);
	masset1 = bilanmasses(graphe);
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

				rs = (2.0*rboul*rboul2)/(rboul+rboul2); 
				scont = 4*3.1416*rs*rs; 
				
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
	masset2 = bilanmasses(graphe); 
	if(masset1 > masset2)
	{
		rapport = masset2/masset1; 
	}
	else
	{
		rapport = masset1/masset2; 
	}
	fprintf(stderr,"\n rapport bilan des masses entre 0 et 1 dans diffexp = %lf",rapport);

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
	float r,x,y,z,**deltas,somdeltas,x1,x2,x3,x4,x5,cdom;
	int nboules,i,j;

	nboules = graphe->nbnoeuds;
	/* nboules contient le nombre de boules */

	deltas = allotab2f(5,nboules); 

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

		deltas[2][i] = rom*mu*x1*deltat - (vdom*cdom*x1*deltat)/(kab+cdom) + vsom*x3*deltat + vfom*x4*deltat ;

		deltas[3][i] = (1-rom)*mu*x1*deltat - vsom*x3*deltat ;

		deltas[4][i] = - vfom*x4*deltat ; 

		deltas[5][i] = ro*x1*deltat ; 

		/* x1 : masse de MB ; x2 : masse de DOM ; x3 : masse de SOM ; x4 : masse de FOM ; x5 : masse de CO2 */

		/* (y1,y2,y3,y4,y5) sont les nouvelles valeurs de (x1,x2,x3,x4,x5) apres transformation */

	}
	/* verification du bilan des masses */
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


/* CETTE PROCEDURE FAIT LA MEME CHOSE QUE MISEAJOURBIO EN ASYNCHRONE EN NE CREANT JAMAIS DE VALEURS NEGATIVES
cette procedure calcule les delta des attributs du graphe apres transformation biologique du modele Soilmu3d/Mepsom
pour un intervalle de temps deltat */
/* elle rend un tableau delta[6][nbnoeuds +1] ou delta[i][j] correspond a l'attribut complementaire i du noeud j;
on indexe a partir de 1 pour rester compatible avec les conventions du graphe */
/* ce tableau contient les deltas correspondant a l'application du modele biologique sur les noeuds du graphe */
/* deltat est l'intervalle de temps ; ro: relative respiration rate ; mu : relative mortality rate ; rom: proportion
de MB retournant a la DOM (l'autre fraction se transforme en SOM), vfom et vsom les taux de decomposition relatifs de 
la FOM et de la SOM ; vdom taux relatif maximum de croissance des microorganismes ; kab: constante de demi saturation de DOM
*/
float **miseajourbioas(graphe_s *graphe,float deltat,float ro,float mu,float rom,float vfom,float vsom,float vdom,float kab)
{
	float r,x,y,z,**deltas,somdeltas,x1,x2,x3,x4,x5,cdom,tr1,tr2;
	int nboules,i,j;

	nboules = graphe->nbnoeuds;
	/* nboules contient le nombre de boules */
	deltas = allotab2f(5,nboules); 
	/* traitement iteratif des noeuds */
	for(i=1;i<=nboules;i++)
	{
		r = graphe->carac[4][i];
		x1 = graphe->carac[5][i];
		x2 = graphe->carac[6][i];
		x3 = graphe->carac[7][i];
		x4 = graphe->carac[8][i];
		x5 = graphe->carac[9][i];

		if(x1 != 0)
		{
			/* croissance des microorganismes, on les fait croitre que si la masse de DOM est positive */
			if(x2 > 0)
			{
				cdom = x2/((4.0/3.0)*3.1416*r*r*r);
				tr1 = (vdom*cdom*x1*deltat)/(kab+cdom);
				if( (x2 - tr1) >= 0)
				{
					x1 += tr1;
					x2 -= tr1;
				}
				else
				{
					x1 += x2;
					x2 = 0;
				}
			}

				/* decomposition des micro-organismes */
			tr1 = mu*x1*deltat;
			if( (x1 - tr1) >= 0)
			{
				x1 -= tr1;
				x2 += rom*mu*x1*deltat;
				x3 += (1-rom)*mu*x1*deltat;
			}else
			{
				tr2 = x1; 
				x1 = 0;
				x2 += rom*tr2;
				x3 += (1-rom)*tr2;
			}
			/* IL FAUDRAIT PEUT ETRE METTRE A 0 LA BIOMASSE SI  (X1 - TR1) < 0 */
			/* respiration des micro-organismes */
			tr1 = ro*x1*deltat;
			if(x1 > 0)
			{
				if((x1-tr1) >= 0)
				{
					x1 -= tr1;
					x5 += tr1;
				}
				else
				{
					x5 += x1;
					x1 = 0;
				}
			}
		}
		/* transformation d'une partie de la SOM et de la FOM en DOM */

		/* transformation de la SOM */
		if(x3 > 0)
		{
			tr1 = vsom*x3*deltat;
			if(x3 >= tr1)
			{
				x2 += tr1;
				x3 -= tr1;
			}
			else
			{
				x2 += x3;
				x3 = 0; 
			}
		}

		/* transformation de la FOM */
		if(x4 > 0)
		{
			tr1 = vfom*x4*deltat;
			if(x4 >= tr1)
			{
				x2 += tr1;
				x4 -= tr1;
			}
			else
			{
				x2 += x4;
				x4 = 0; 
			}
		}


		/* calcul des deltas */

		deltas[1][i] = x1 - graphe->carac[5][i];
		
		deltas[2][i] = x2 - graphe->carac[6][i] ;

		deltas[3][i] = x3 - graphe->carac[7][i] ;

		deltas[4][i] =  x4 - graphe->carac[8][i]; 

		deltas[5][i] =  x5 - graphe->carac[9][i]; 

	}
	
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
	double massetot1,massetot2,rapport,rs; 
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
le nombre de voisins maximum d'un noeud. si le coefficient de diffusion est negatif, la diffusion est instantanee ce qui revient a prendre un coefficient de diffusion infini */

float *miseajourdif(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax)
{
	int nbnoeud,i,j,k,n,*tavois,nvois;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,*tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,massetot2,prop,vtot,sommedelta,vartr,rs;
	long double sommedeltad,massetotd,vtotd,rbould,volbould,massetot2d,vard,*tabdeltad; 

	nbnoeud = graphe->nbnoeuds; 
	tabdelta = (float*)mycalloc(nbnoeud + 1,sizeof(float)); 

	/* calcul de la masse totale initiale de la caracteristique a diffuser */
	massetot = 0;
	for(i = 1 ; i <= nbnoeud ; i++)
	{
		massetot += graphe->carac[4+numcar][i];
	}

	/* fprintf(stderr,"\n massetot = %f",massetot); */

	/* initialisation de tabdelta a 0 */
	for(i=0 ; i<= nbnoeud; i++)
	{
		tabdelta[i] = 0; 
	}

	/* cas de la diffusion instantanee, dans ce cas aucune valeur ne peut etre negative */
	/* IL Y A SANS DOUTE UN BUG DANS LE CODE DU CAS COEFF < 0 CAR LE BILAN DES MASSES PART A L'OUEST QUAND ON APPELLE SIMULSYNCHCOURBAS ET
	EN PLUS LA SOMME DES DELTAS EST EGALE A -1 ALORS QUE LE BILAN DES MASSES EN LOCAL EST BON (!!??), IL FAUDRAIT TESTER SUR DE PETITES DONNEES SYNTHETIQUES */
	if(coeff < 0)
	{
		/* fprintf(stderr,"\n ENTREE DANS MISE A JOUR DIF"); */
		massetotd = 0; 
		tabdeltad = (long double*)mycalloc(nbnoeud + 1,sizeof(long double));
		for(i=1 ; i<= nbnoeud; i++)
		{
			tabdeltad[i] = 0; 
			massetotd += (long double)graphe->carac[4+numcar][i]; 
		}
		/* fprintf(stderr,"\n masse totale de DOM = %Lf",massetotd);*/
		sommedeltad = 0; 
		vtotd = 0;
		for(i = 1 ; i <= nbnoeud ; i++)
		{
			rbould = (long double)graphe->carac[4][i];
			volbould = (4.0/3.0)*3.1416*rbould*rbould*rbould; 
			tabdeltad[i] = massetotd*volbould; 
			vtotd += volbould; 
		}
		/* fprintf(stderr,"\n somme des volumes = %Lf",vtotd); */
		vard = 0; 
		/* fprintf(stderr,"\n"); */
		for(i = 1 ; i <= nbnoeud ; i++)
		{
			rbould = (long double)graphe->carac[4][i];
			volbould = (4.0/3.0)*3.1416*rbould*rbould*rbould; 
			tabdeltad[i] = massetotd*(volbould/vtotd) - (long double)graphe->carac[4+numcar][i]; 
			sommedeltad += tabdeltad[i];
			massetot2d += tabdeltad[i] + (long double)graphe->carac[4+numcar][i]; 
			vard += volbould/vtotd; 
			/* fprintf(stderr,"\n i=%d ; rbould=%Lf ; volbould=%Lf ; tabdeltad=%Lf ; vard=%Lf ; massetot2d=%Lf ; graphe-carac=%f",i,rbould,volbould,tabdeltad[i],vard,massetot2d,graphe->carac[4+numcar][i]); */
			/* fprintf(stderr,"concentration de DOM de la boule %d = %Lf ; ",i,(tabdeltad[i] + (long double)graphe->carac[4+numcar][i])/volbould); */
		}
		sommedelta = 0;  
		for(i = 1 ; i <= nbnoeud ; i++)
		{
			tabdelta[i] = (float)tabdeltad[i]; 
			sommedelta += tabdelta[i];
		}
		/* fprintf(stderr,"\n somme des delta dif en long double = %Lf ; masse totale 2 = %Lf ; vartr = %Lf ; ",sommedeltad,massetot2d,vard);
		fprintf(stderr," somme des delta dif en float = %f",sommedelta);*/
		/* rectificationn des deltas pour que la somme soit zero, on met ou on enleve un epsilon a la boule 1 */
		myfreeLf(tabdeltad); 
		return(tabdelta);
	}

	tavois = (int*)mycalloc(nvmax,sizeof(int));
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
			rs = (2.0*rboul*rboul2)/(rboul+rboul2); 
			scont = 4*3.1416*rs*rs; 
				
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


/* cete procedure appelle la procedure de simulation qui rend les courbes echantillonnees en heures en utilisant la
transformation biologique asynchrone */

void testsimulsynchcourbas()
{
	graphe_s *graphe ;
	char ficgraphe[50],ficparbio[50],ficgrapher[50],ficmasses[50],ficmatlab[50],ficexcel[50];
	float coeffdom,deltat;
	int fraction,nvmax,pourcneg ; 

	fprintf(stderr,"\n fichier graphe en entree ; fichier parametres bio ; fichier graphe en sortie: ; fichier masses en sortie ; fichier matlab en sortie ; fichier excel en sortie \n");
	scanf("%s %s %s %s %s %s",ficgraphe,ficparbio,ficgrapher,ficmasses,ficmatlab,ficexcel); 

	fprintf(stderr,"\n coeff diffusion (104665 en voxels carre par jour) ; deltat (en jours) ; fraction ; nb max voisins ; pourcentage max valeurs negatives : \n");
	scanf("%f %f %d %d %d",&coeffdom,&deltat,&fraction,&nvmax,&pourcneg);

	graphe = simulsynchcourbas(ficgraphe,ficparbio,coeffdom,deltat,fraction,nvmax,pourcneg,ficgrapher,ficmasses,ficmatlab,ficexcel);
}

void testsimulsynchcourbasas()
{
	graphe_s *graphe ;
	char ficgraphe[50],ficparbio[50],ficgrapher[50],ficmasses[50],ficmatlab[50],ficexcel[50];
	float coeffdom,deltat;
	int fraction,nvmax,pourcneg ; 

	fprintf(stderr,"\n fichier graphe en entree ; fichier parametres bio ; fichier graphe en sortie: ; fichier masses en sortie ; fichier matlab en sortie ; fichier excel en sortie \n");
	scanf("%s %s %s %s %s %s",ficgraphe,ficparbio,ficgrapher,ficmasses,ficmatlab,ficexcel); 

	fprintf(stderr,"\n coeff diffusion (104665 en voxels carre par jour) ; deltat (en jours) ; fraction ; nb max voisins ; pourcentage max valeurs negatives : \n");
	scanf("%f %f %d %d %d",&coeffdom,&deltat,&fraction,&nvmax,&pourcneg);

	graphe = simulsynchcourbasas(ficgraphe,ficparbio,coeffdom,deltat,fraction,nvmax,pourcneg,ficgrapher,ficmasses,ficmatlab,ficexcel);
}

void testsimulsynchcourbimp()
{
	graphe_s *graphe ;
	char ficgraphe[50],ficparbio[50],ficgrapher[50],ficmasses[50],ficmatlab[50],ficexcel[50];
	float coeffdom,deltat,eps;
	int fraction,nvmax,pourcneg ; 

	fprintf(stderr,"\n fichier graphe en entree ; fichier parametres bio ; fichier graphe en sortie: ; fichier masses en sortie ; fichier matlab en sortie ; fichier excel en sortie \n");
	scanf("%s %s %s %s %s %s",ficgraphe,ficparbio,ficgrapher,ficmasses,ficmatlab,ficexcel); 

	fprintf(stderr,"\n coeff diffusion (104665 en voxels carre par jour) ; deltat (en jours) ; fraction ; nb max voisins ; numero de l'attribut a diffuser : ; valeur de epsilon pour gradient\n");
	scanf("%f %f %d %d %d %f",&coeffdom,&deltat,&fraction,&nvmax,&pourcneg,&eps);

	fprintf(stderr,"\n eps = %f",eps); 

	graphe = simulsynchcourbimp(ficgraphe,ficparbio,coeffdom,deltat,fraction,nvmax,pourcneg,ficgrapher,ficmasses,ficmatlab,ficexcel,eps);
}

void testsimulsynchcourbopt()
{
	graphe_s *graphe ;
	char ficgraphe[50],ficparbio[50],ficgrapher[50],ficmasses[50],ficmatlab[50],ficexcel[50];
	float coeffdom,deltat,eps;
	int fraction,nvmax,pourcneg ; 

	fprintf(stderr,"\n fichier graphe en entree ; fichier parametres bio ; fichier graphe en sortie: ; fichier masses en sortie ; fichier matlab en sortie ; fichier excel en sortie \n");
	scanf("%s %s %s %s %s %s",ficgraphe,ficparbio,ficgrapher,ficmasses,ficmatlab,ficexcel); 

	fprintf(stderr,"\n coeff diffusion (104665 en voxels carre par jour) ; deltat (en jours) ; fraction ; nb max voisins ; numero de l'attribut a diffuser : ; valeur de epsilon pour gradient\n");
	scanf("%f %f %d %d %d %f",&coeffdom,&deltat,&fraction,&nvmax,&pourcneg,&eps);

	fprintf(stderr,"\n eps = %f",eps); 

	graphe = simulsynchcourbopt(ficgraphe,ficparbio,coeffdom,deltat,fraction,nvmax,pourcneg,ficgrapher,ficmasses,ficmatlab,ficexcel,eps);
}

void testsimulsynchcourbasync()
{
	graphe_s *graphe ;
	char ficgraphe[50],ficparbio[50],ficgrapher[50],ficmasses[50],ficmatlab[50],ficexcel[50];
	float coeffdom,deltat;
	int fraction,nvmax,pourcneg,fracbio; 

	fprintf(stderr,"\n fichier graphe en entree ; fichier parametres bio ; fichier graphe en sortie: ; fichier masses en sortie ; fichier matlab en sortie ; fichier excel en sortie \n");
	scanf("%s %s %s %s %s %s",ficgraphe,ficparbio,ficgrapher,ficmasses,ficmatlab,ficexcel); 

	fprintf(stderr,"\n coeff diffusion (104665 en voxels carre par jour) ; deltat (en jours) ; fraction diffusion ; fraction bio ; nb max voisins ; pourcentage max valeurs negatives : \n");
	scanf("%f %f %d %d %d %d",&coeffdom,&deltat,&fraction,&fracbio,&nvmax,&pourcneg);

	graphe = simulsynchcourbasync(ficgraphe,ficparbio,coeffdom,deltat,fraction,fracbio,nvmax,pourcneg,ficgrapher,ficmasses,ficmatlab,ficexcel);
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
			rap = sqrt((Xb[j]-Xb[k])*(Xb[j]-Xb[k])+(Yb[j]-Yb[k])*(Yb[j]-Yb[k])+(Zb[j]-Zb[k])*(Zb[j]-Zb[k])) - (Rb[j]+Rb[k]);
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
	fprintf(stderr,"\n seuil pour connexite: distance centres - somme rayons <= ");
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
	float ro,mu,rom,vfom,vsom,vdom,kab,somdelta,somdeltabio,somdeltadif; 
	double massetot1,massetot2,rapport,massetr; 
	float temps,tempstot,totacarb; 
	clock_t t1,t2,t3,t4,t5; 
	int n1,n2,fracjour,nbels,l; 

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

	fprintf(stderr,"\n bilan total des masses au debut de simulsynchcourb = %lf",massetot1); 

	fprintf(stderr,"\n");
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

		deltatdif = miseajourdif(graphe,2,coeffdom,petitdeltat,nvmax);

		/* calcul de la somme des deltas dif */
		somdeltadif = 0; 
		for(i=1 ; i<= nbnoeud; i++)
		{
			somdeltadif += deltatdif[i]; 
		}

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
			/* on regarde si la rectification est possible et sinon on revient en arriere pour lancer la dynamique sans assimilation de la DOM */
			if(masseneg > massetot)
			{
				freetf(deltatbio,5,nbnoeud); 
				myfreef(deltatdif); 
				if(j != 2)
				{
					fprintf(stderr,"\n rectification impossible et j different de 2 ; j = %d",j); 
					exit(5); 
				}
				/* fprintf(stderr," & ");*/
				/* on remet le graphe dans l'etat initial avant la mise a jour */
				for(k=1 ; k <= nbnoeud ; k++)
				{
					for(l=1;l<=5;l++)
					{
						graphe->carac[4+l][k] -= tabdelta[l][k];
					}
				}
				/* le graphe est dans son etat initial avant la mise a jour */
				/* on effectue une nouvelle mise a jour sans assimilation de la DOM par les MB ce qui revient a mettre vdom a zero */
				deltatbio = miseajourbio(graphe,petitdeltat,ro,mu,rom,vfom,vsom,0.0,kab);
				deltatdif = miseajourdif(graphe,2,coeffdom,petitdeltat,nvmax);

				for(k=0 ; k<= nbnoeud; k++)
				{
					for(l=0; l <= 5 ; l++)
					{
						tabdelta[l][k] = 0; 
					}
			
				}
				/* on forme le nouveau tabdelta */
				for(k=1 ; k<= nbnoeud; k++)
				{
					for(l=1;l<=5;l++)
					{
						tabdelta[l][k] += deltatbio[l][k];
					}
					tabdelta[2][k] += deltatdif[k]; 
				}
				/* on calcule a nouveau les masses totales */
				massetot = 0; 
				masseneg = 0;
				for(k = 1 ; k <= nbnoeud ; k++)
				{
					for(l=1;l<=5;l++)
					{
						graphe->carac[4+l][k] += tabdelta[l][k];
						if(graphe->carac[4+l][k] < 0)
						{
							masseneg -= graphe->carac[4+l][k]; 
						}
						else
						{
							massetot += graphe->carac[4+l][k]; 
						}
					}
				}
				if(massetot < masseneg)
				{
					fprintf(stderr,"\n valeurs negatives plus importantes que valeurs positives ! ; j=%d ",j);
					fprintf(stderr,"\n masses negatives = %f ; masses positives = %f ; total des masses = %f",masseneg,massetot,massetot-masseneg);
					fprintf(stderr,"\n pourcentage du total des masses par rapport a la masse totale = %f",((-massetot+masseneg)/massetot1)*100); 
					exit(4);
				}
			}
			if(masseneg > 0)
			{
				/* on traite les cas ou des valeurs de la caracteristique j seraient negatives */
				prop = (masseneg/massetot)*100 ;
				/* fprintf(stderr,"\n masse totale = %f ; masse negative = %f ; proportion de masse negative = %f",massetot,masseneg,prop); */
				if(prop > pourcneg)
				{
					fprintf(stderr,"\n reduire le petit pas de temps car proportion de valeurs negatives trop importante = %f , j= %d",prop,j);
					exit(3); 
				}
				/* if(prop > 90)
				{
					fprintf(stderr,"\n proportion de valeurs negatives = %f ; j = %d ; ",prop,j); 
					fprintf(stderr,"masses negatives = %f ; masses positives = %f ; total des masses = %f ",masseneg,massetot,massetot-masseneg);
					fprintf(stderr," ; pourcentage du total des masses par rapport a la masse totale = %f",((massetot-masseneg)/massetot1)*100);
					fprintf(stderr,"\n");
				}*/
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
						graphe->carac[4+j][i] -= (graphe->carac[4+j][i]/massetot)*masseneg;
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
			fprintf(stderr,"\n bilan total des masses = %lf",massetot1); 
			t5 = clock(); 
			temps = (float)(t5 - t3) / CLOCKS_PER_SEC;
			temps = temps/60.0;
			fprintf(stderr,"\n temps de calcul intermediaire pour la simulation synchrone explicite = %f' ",temps); 
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

/* CETTE PROCEDURE FAIT LA MEME CHOSE QUE SIMULSYNCHCOURB MAIS AVEC UNE MISE A JOUR DE LA BIOLOGIE ASYNCHRONE QUI NE PROVOQUE JAMAIS
DE VALEURS NEGATIVES
cette procedure prend en entree un fichier contenant un graphe value de boules avec les attributs complementaires
(MB,DOM,SOM,FOM,CO2) (unite g), un fichier contenant les parametres du modele biologique
(vdom,kab,mu,ro,rom) (j-1,gCg-1,j-1,j-1), le coefficient de diffusion de la DOM, un intervalle de temps (en jours) 
une fraction de l'intervalle de temps pour le schema explicite, et rend en
sortie un fichier contenant le graphe apres simulation, un pointeur sur la structure de graphe correspondante, un fichier contenant la suite
des valeurs des masses sans pourcentages, un fichier contenant les courbes
matlab de la variation des masses des attributs (en pourcentage de la masse totale, echantillonage en heures), un fichier contenant la meme chose mais pour excel
on simule de maniere synchrone la diffusion et les processus de transformation en calculant les deltas correspondants
nvmax est le nombre de vosins maximums d'un noeud dans le graphe
on retourne un graphe correspondant au graphe value apres simulation  synchrone de la diffusion et de la decomposition microbienne 
on retourne aussi un fichier contenant le nouveau graphe
l'unite de temps est le jour et l'echantillonnage pour les courbes est l'heure */

graphe_s *simulsynchcourbas(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel)
{
	graphe_s *graphe;
	int nbnoeud,i,j,k,n,*tavois,nvois,ech,natt;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,**tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,prop,**deltatbio,*deltatdif,car1[5],**car,propmax;
	FILE *ptficparbio,*ptficmatlab,*ptficexcel,*ptficmasses; 
	float ro,mu,rom,vfom,vsom,vdom,kab,somdelta,somdeltabio,somdeltadif; 
	double massetot1,massetot2,rapport,massetr,massetotdebut,massetotfin; 
	float temps,tempstot,totacarb; 
	clock_t t1,t2,t3,t4,t5; 
	int n1,n2,fracjour,nbels,l; 

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

	massetotdebut = bilanmasses(graphe); 

	massetot1 = bilanmasses(graphe); 

	fprintf(stderr,"\n bilan total des masses au debut de simulsynchcourb = %lf",massetotdebut); 

	fprintf(stderr,"\n");
	t1 = clock(); 
	tempstot = 0; 

	natt = 5; 
	nbels = fraction/ech + 1; 
	car = allot2f(nbels + 1,natt);

	n1 =0; 

	propmax = 0; 

	for(n = 0 ; n < fraction ; n++)
	{
		if(n % 1000 == 0)
		{
			t2 = clock(); 
			temps = (float)(t2 - t1) / CLOCKS_PER_SEC;
			temps = temps/60.0; 
			tempstot += temps; 
			fprintf(stderr,"\n temps de calcul= %f minutes",tempstot);
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

		deltatbio = miseajourbioas(graphe,petitdeltat,ro,mu,rom,vfom,vsom,vdom,kab);

		deltatdif = miseajourdif(graphe,2,coeffdom,petitdeltat,nvmax);

		/* determination du tableau des deltas prenant en compte la diffusion de la DOM et les transformations */ 
		
		for(i=1 ; i<= nbnoeud; i++)
		{
			for(j=1;j<=5;j++)
			{
				tabdelta[j][i] += deltatbio[j][i];
			}
			tabdelta[2][i] += deltatdif[i]; 
		}

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
			/* on regarde si la rectification est possible et sinon on s'arrete */
			if(masseneg > massetot)
			{
				fprintf(stderr,"\n valeurs negatives plus importantes que valeurs positives ! ; j=%d ",j);
				fprintf(stderr,"\n masses negatives = %f ; masses positives = %f ; total des masses en algébrique = %f",masseneg,massetot,massetot-masseneg);
				fprintf(stderr,"\n pourcentage du total des masses par rapport a la masse totale = %f",((-massetot+masseneg)/massetot1)*100);
				exit(3); 
			}

			if(masseneg > 0)
			{
				/* on traite les cas ou des valeurs de la caracteristique j seraient negatives */
				prop = (masseneg/massetot)*100 ;
				if(prop > propmax)
				{
					propmax = prop; 
				}

				if(prop > pourcneg)
				{
					fprintf(stderr,"\n reduire le petit pas de temps car proportion de valeurs negatives trop importante = %f , j= %d \n",prop,j);
					exit(3); 
				}

				for(i = 1 ; i <= nbnoeud ; i++)
				{
					if(graphe->carac[4+j][i] <= 0)
					{
						graphe->carac[4+j][i] = 0;
					}
					else
					{
						graphe->carac[4+j][i] -= (graphe->carac[4+j][i]/massetot)*masseneg;
						if(graphe->carac[4+j][i] < 0)
						{
							fprintf(stderr,"\n rectification des valeurs negatives impossible niveau 2, reduire le pas de temps");
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
			massetot1 = bilanmasses(graphe); 
			fprintf(stderr,"\n temps de la simulation en jours =%f ",(n+1)*petitdeltat); 
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
			fprintf(stderr,"\n bilan total des masses = %lf",massetot1); 
			t5 = clock(); 
			temps = (float)(t5 - t3) / CLOCKS_PER_SEC;
			temps = temps/60.0;
			fprintf(stderr,"\n temps de calcul intermediaire pour la simulation synchrone explicite = %f' ",temps); 
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

	fprintf(ptficmatlab,"plot(X,Y0,'b',X,Y1,'g',X,Y2,'c',X,Y3,'c',X,Y4,'r');");
	fprintf(ptficmatlab,"\n");

	t4= clock(); 
	temps = (float)(t4 - t3) / CLOCKS_PER_SEC;
	temps = temps/60.0;

	fprintf(stderr,"\n temps total pour la simulation synchrone explicite = %f minutes",temps); 

	massetotfin = bilanmasses(graphe); 


	if(massetotfin > massetotdebut)
	{
		rapport = massetotdebut/massetotfin; 
	}
	else
	{
		rapport = massetotfin/massetotdebut; 
	}
	fprintf(stderr,"\n rapport des bilans de masse entre 0 et 1 = %lf ; proportion valeurs negatives maximum = %f",rapport,propmax); 
	/* ecriture du nouveau graphe value apres diffusion, seules les valeurs des attributs ont pu changer */
	
	ecrigraphe(graphe,ficgrapher);

	fclose(ptficparbio); 
	fclose(ptficmatlab);
	fclose(ptficexcel);
	fclose(ptficmasses);

	return(graphe); 
}

/* CETTE PROCEDURE FAIT LA MEME CHOSE QUE SIMULSYNCHCOURBAS AVEC UN SCHEMA ASYNCHRONE DIFFUSION-TRANSFORMATION */


graphe_s *simulsynchcourbasas(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel)
{
	graphe_s *graphe;
	int nbnoeud,i,j,k,n,*tavois,nvois,ech,natt;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,**tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,prop,**deltatbio,*deltatdif,car1[5],**car,propmax;
	FILE *ptficparbio,*ptficmatlab,*ptficexcel,*ptficmasses; 
	float ro,mu,rom,vfom,vsom,vdom,kab,somdelta,somdeltabio,somdeltadif; 
	double massetot1,massetot2,rapport,massetr,massetotdebut,massetotfin; 
	float temps,tempstot,totacarb; 
	clock_t t1,t2,t3,t4,t5; 
	int n1,n2,fracjour,nbels,l; 

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


	petitdeltat = deltat/fraction ;

	ech = neari((1.0/24.0)*(1/petitdeltat)); 

	/* ech represente le nombre d'iterations correspondant a une heure de temps */

	fprintf(stderr,"\n deltat=%f jours ",deltat); 

	fprintf(stderr,"\n petitdeltat=%f jours ; ech=%d",petitdeltat,ech); 
	
	/* calcul des masses totales pour bilan des masses */

	massetotdebut = bilanmasses(graphe); 

	massetot1 = bilanmasses(graphe); 

	fprintf(stderr,"\n bilan total des masses au debut de simulsynchcourb = %lf",massetotdebut); 

	fprintf(stderr,"\n");
	t1 = clock(); 
	tempstot = 0; 

	natt = 5; 
	nbels = fraction/ech + 1; 
	car = allot2f(nbels + 1,natt);

	n1 =0; 

	propmax = 0; 

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
		
		/* pour chaque noeud on calcule la somme des deltas pour la diffusion pour chaque intervalle (deltat/n) */
		/* ensuite on simule de maniere synchrone la diffusion de la DOM */

		deltatdif = miseajourdif(graphe,2,coeffdom,petitdeltat,nvmax);
		
		/* mise a jour des attributs du graphe avec le tableau tabdelta */
		for(i = 1 ; i <= nbnoeud ; i++)
		{
			/* mise a jour de la DOM qui diffuse avec le tableau des deltas */
				graphe->carac[6][i] += deltatdif[i];
		}

		/* rectification de la DOM diffusee en cas de valeurs negatives */
		masseneg = 0; 
		massetot = 0; 

		for(i=1;i<=nbnoeud;i++)
		{
			if(graphe->carac[6][i] < 0)
			{
				masseneg += -graphe->carac[6][i];
			}
			else
			{
				massetot += graphe->carac[6][i];
		
			}
		}

		/* on regarde si la rectification est possible et sinon on s'arrete */
		if(masseneg > massetot)
		{
			fprintf(stderr,"\n valeurs negatives plus importantes que valeurs positives !");
			fprintf(stderr,"\n masses negatives = %f ; masses positives = %f ; total des masses en algébrique = %f",masseneg,massetot,massetot-masseneg);
			fprintf(stderr,"\n pourcentage du total des masses par rapport a la masse totale = %f",((-massetot+masseneg)/massetot1)*100);
			exit(3); 
		}

		if(masseneg > 0)
		{
			/* on traite les cas ou des valeurs de la caracteristique 2 (DOM) seraient negatives */
			prop = (masseneg/massetot)*100 ;
			if(prop > propmax)
			{
				propmax = prop; 
			}

			if(prop > pourcneg)
			{
				fprintf(stderr,"\n reduire le petit pas de temps car proportion de valeurs negatives trop importante = %f  \n",prop);
				exit(3); 
			}

			for(i = 1 ; i <= nbnoeud ; i++)
			{
				if(graphe->carac[6][i] <= 0)
				{
					graphe->carac[6][i] = 0;
				}
				else
				{
					graphe->carac[6][i] -= (graphe->carac[6][i]/massetot)*masseneg;
					if(graphe->carac[6][i] < 0)
					{
						fprintf(stderr,"\n rectification des valeurs negatives impossible niveau 2, reduire le pas de temps");
						exit(1); 
					}
				}
			}
		}
		
		/* liberation de l'espace pointe par deltatdif */
		myfreef(deltatdif); 

		/* la diffusion a ete simulee, on simule maintenant les transformations ; 
		toutes les valeurs des attributs sont positives */ 

		deltatbio = miseajourbioas(graphe,petitdeltat,ro,mu,rom,vfom,vsom,vdom,kab);

		for(i = 1 ; i <= nbnoeud ; i++)
		{
			/* mise a jour des attributs avec le tableau des deltas de la biologie */
			for(j=1;j<=5;j++)
			{
				graphe->carac[4+j][i] += deltatbio[j][i];
			}
		}
		freetf(deltatbio,5,nbnoeud); 

		/* la diffusion puis les transformations ont ete simulees de maniere asynchrone */

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
			massetot1 = bilanmasses(graphe); 
			fprintf(stderr,"\n temps de la simulation en jours =%f ",(n+1)*petitdeltat); 
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
			fprintf(stderr,"\n bilan total des masses = %lf",massetot1); 
			t5 = clock(); 
			temps = (float)(t5 - t3) / CLOCKS_PER_SEC;
			temps = temps/60.0;
			fprintf(stderr,"\n temps de calcul intermediaire pour la simulation synchrone explicite = %f' ",temps); 
		}
	}
	/* a la fin de la boucle le tableau car[i][j] contient pour le temps numero i (en heures) la somme des attributs adresses par j */
	totacarb = massetot1; 
	fracjour = 24; 
	nbels = n1; 
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

	fprintf(ptficmatlab,"plot(X,Y0,'b',X,Y1,'g',X,Y2,'c',X,Y3,'c',X,Y4,'r');");
	fprintf(ptficmatlab,"\n");

	t4= clock(); 
	temps = (float)(t4 - t3) / CLOCKS_PER_SEC;
	temps = temps/60.0;

	fprintf(stderr,"\n temps total pour la simulation synchrone explicite = %f minutes",temps); 

	massetotfin = bilanmasses(graphe); 


	if(massetotfin > massetotdebut)
	{
		rapport = massetotdebut/massetotfin; 
	}
	else
	{
		rapport = massetotfin/massetotdebut; 
	}
	fprintf(stderr,"\n rapport des bilans de masse entre 0 et 1 = %lf ; proportion valeurs negatives maximum = %f",rapport,propmax); 
	/* ecriture du nouveau graphe value apres diffusion, seules les valeurs des attributs ont pu changer */
	
	ecrigraphe(graphe,ficgrapher);

	fclose(ptficparbio); 
	fclose(ptficmatlab);
	fclose(ptficexcel);
	fclose(ptficmasses);

	return(graphe); 
}

/* cette procedure cree un fichier d'attributs complementaire en fonction d'un fichier de donnees format "xavier"
on recherche pour chaque voxel la boule la plus proche de rayon inferieur a rmax 
la DOM est répartie de manière homogène avec une masse totale massedom*/

void creerattval(char *ficdonnees, char *ficboules,char *ficatt,float massedom,float rmax)
{

    float massetmic,massetdom,x,y,z,r,sommevolmic,sommevoldom,vol,biomasse,distance,dcarre;
    int nbbmic,nbbdom,nboules,numboule,*boulmic,*bouldom,i,j,npatchs,nbmb,x1,y1,z1; 
    float** attcopml,**trav,biomassetot,rayon;
    FILE *ptficboules,*ptficatt,*ptficdonnees; 
	
    ptficboules = fopen(ficboules,"r");
	ptficatt = fopen(ficatt,"w");
    ptficdonnees = fopen(ficdonnees,"r");

    fscanf(ptficboules,"%d",&nboules);

    fprintf(stderr,"\n nombre de boules = %d",nboules); 

    /* creation du tableau trav qui contiendra: les coordonnnes du centre de la boule, son rayon, sa masse de micro-organismes, sa masse de DOM */
	/* les boules seront indexes a partir de 1 ainsi que les attributs */
    trav = allot2f(6,nboules+1); 

    /* initialisation du tableau trav a zero */
    for(i=0;i<6;i++)
    {
        for(j=0;j<=nboules;j++)
        {
            trav[i][j] = 0; 
        }
    }

    fprintf(stderr,"\n fin initialisation du tableau trav"); 

    /* ecriture des centres des boules et de rayons dans trav(i,j) et mise de la DOM de maniere homogene dans les boules */

	sommevoldom = 0; 
    for(i=1;i<=nboules;i++)
	{
		fscanf(ptficboules,"%f %f %f %f",&x,&y,&z,&r);
        trav[0][i] = x; 
		trav[1][i] = y; 
		trav[2][i] = z; 
		trav[3][i] = r; 
		vol = (4.0/3.0)*3.1416*r*r*r;
		sommevoldom += vol;
		trav[5][i] = vol*massedom; 
	}
	massetdom = 0.0; 
	for(i=1;i<=nboules;i++)
	{
		trav[5][i] = trav[5][i]/sommevoldom; 
		massetdom += trav[5][i];
	}

	/* on place la biomasse dans les boules avec le fichier de donnees format Xavier */

	fscanf(ptficdonnees,"%d",&npatchs);

	/* npatchs contient le nombre de patchs de micro-organismes */
	biomassetot = 0; 
	for(i=0;i<npatchs;i++)
	{
		fscanf(ptficdonnees,"%d %d %d %d",&x1,&y1,&z1,&nbmb);
		biomasse = (nbmb*5.41)*0.000000001; 
		biomassetot += biomasse; 
		/* on recherche la boule la plus proche du voxel (x1,y1,z1) dont le rayon est plus petit que rmax */
		distance = 10000000;
		numboule = -1;
		rayon = -1; 
		for(j=1;j<=nboules;j++)
		{
			x = trav[0][j]; 
			y = trav[1][j]; 
			z = trav[2][j]; 
			r = trav[3][j]; 
			dcarre = sqrt((x1-x)*(x1-x) + (y1-y)*(y1-y) + (z1-z)*(z1-z)) - r;
			if((dcarre < distance) && (r < rmax)) 
			{
				distance = dcarre; 
				numboule = j; 
				rayon = r; 
			}
		}
		if(numboule == -1)
		{
			fprintf(stderr,"\n on ne trouve pas de boule !!");
			exit(5); 
		}else
		{
			/* fprintf(stderr,"\n distance = %f ; rayon = %f",distance,rayon); */
			trav[4][numboule] += biomasse; 
		}
	}
	fprintf(stderr,"\n biomasse totale initiale = %f",biomassetot); 
	fprintf(stderr,"\n masse totale de dom initiale = %f",massetdom); 


	/* on remplit ficcat avec les valeurs de la biomasse et de la matière organique qui sont dans trav */
    fprintf(stderr,"\n remplissage de ficatt");

    fprintf(ptficatt,"%d \n",5);

    for(i=0;i<nboules;i++)
    {
        fprintf(ptficatt,"%f %f %f %f %f\n",trav[4][i+1],trav[5][i+1],0.0,0.0,0.0); 
    }

    fclose(ptficatt);
    fclose(ptficboules); 
    fclose(ptficdonnees);
}

/* MEME CHOSE QUE CREERATTVAL MAIS EN GERANT LES COMPOSANTES CONNEXES
LES DIMENSIONS DE L'IMAGE SONT 512X512X512 AU PLUS ET LE NOMBRE MAX DE VOISINS 500 
CETTE PROCEDURE EST SPECIFIQUE POUR LES DONNEES DE VALERIE */
void creerattvalbis(char *ficdonnees, char *ficboules,char *ficatt,float massedom,float rmax)
{

    float massetmic,massetdom,x,y,z,r,sommevolmic,sommevoldom,vol,biomasse,distance,dcarre;
    int nbbmic,nbbdom,nboules,numboule,*boulmic,*bouldom,i,j,npatchs,nbmb,x1,y1,z1,nbarcs,**compboul,nbout; 
    float** attcopml,**trav,biomassetot,rayon;
    FILE *ptficboules,*ptficatt,*ptficdonnees; 
	graphe_s *graphe; 
	char chaine[] = "ficarc";

	/* creation du graphe correspondant au fichier de boules, on cree un graphe sans attributs complementaires */ 
	arcsval(ficboules,chaine, 512,512,512,&nbarcs,500);
	fprintf(stderr,"\n chaine = %s",chaine); 
	fprintf(stderr,"\n nombre d'arcs = %d",nbarcs); 
	fprintf(stderr,"nb arcs = %d",nbarcs); 
	fprintf(stderr,"\n sortie de arcval"); 
	graphe = cregraphe_satt(ficboules,chaine);
	fprintf(stderr,"\n sortie de cregraphe"); 
	compboul = compconnexesvalbis(graphe); 
	fprintf(stderr,"\n sortie de compconnexesvalbis"); 
	remove("ficarc"); 
	/* le tableau compboul contient pour chaque boule le numero de sa composante connexe et le pourcentage du volume total
	que cette composante connexe represente */

    ptficboules = fopen(ficboules,"r");
	ptficatt = fopen(ficatt,"w");
    ptficdonnees = fopen(ficdonnees,"r");

    fscanf(ptficboules,"%d",&nboules);

    fprintf(stderr,"\n nombre de boules = %d",nboules); 

	nbout = 0; 

    /* creation du tableau trav qui contiendra: les coordonnnes du centre de la boule, son rayon, sa masse de micro-organismes, sa masse de DOM */
	/* les boules seront indexes a partir de 1 ainsi que les attributs */
    trav = allot2f(6,nboules+1); 

    /* initialisation du tableau trav a zero */
    for(i=0;i<6;i++)
    {
        for(j=0;j<=nboules;j++)
        {
            trav[i][j] = 0; 
        }
    }

    fprintf(stderr,"\n fin initialisation du tableau trav"); 

    /* ecriture des centres des boules et de rayons dans trav(i,j) et mise de la DOM de maniere homogene dans les boules */

	sommevoldom = 0; 
    for(i=1;i<=nboules;i++)
	{
		fscanf(ptficboules,"%f %f %f %f",&x,&y,&z,&r);
        trav[0][i] = x; 
		trav[1][i] = y; 
		trav[2][i] = z; 
		trav[3][i] = r; 
		vol = (4.0/3.0)*3.1416*r*r*r;
		sommevoldom += vol;
		trav[5][i] = vol*massedom; 
	}
	massetdom = 0.0; 
	for(i=1;i<=nboules;i++)
	{
		trav[5][i] = trav[5][i]/sommevoldom; 
		massetdom += trav[5][i];
	}

	/* on place la biomasse dans les boules avec le fichier de donnees format Xavier */

	fscanf(ptficdonnees,"%d",&npatchs);

	/* npatchs contient le nombre de patchs de micro-organismes */
	biomassetot = 0; 
	for(i=0;i<npatchs;i++)
	{
		fscanf(ptficdonnees,"%d %d %d %d",&x1,&y1,&z1,&nbmb);
		biomasse = (nbmb*5.41)*0.000000001; 
		biomassetot += biomasse; 
		/* on recherche la boule la plus proche du voxel (x1,y1,z1) dont le rayon est plus petit que rmax */
		distance = 10000000;
		numboule = -1;
		rayon = -1; 
		for(j=1;j<=nboules;j++)
		{
			x = trav[0][j]; 
			y = trav[1][j]; 
			z = trav[2][j]; 
			r = trav[3][j]; 
			dcarre = sqrt((x1-x)*(x1-x) + (y1-y)*(y1-y) + (z1-z)*(z1-z)) - r;
			if((dcarre < distance) && (r < rmax)) 
			{
				distance = dcarre; 
				numboule = j; 
				rayon = r; 
			}
		}
		if(numboule == -1)
		{
			fprintf(stderr,"\n on ne trouve pas de boule !!");
			exit(5); 
		}else
		{
			trav[4][numboule] += biomasse; 
			if(distance > 0)nbout++; 
			fprintf(stderr,"\n distance = %f ; rayon = %f ; numero boule = %d",distance,rayon,numboule);
			fprintf(stderr,"\n numero de la composante connexe = %d ; pourcentage du volume total = %d",compboul[0][numboule],compboul[1][numboule]);
		}
	}
	fprintf(stderr,"\n biomasse totale initiale = %f",biomassetot); 
	fprintf(stderr,"\n masse totale de dom initiale = %f",massetdom); 


	/* on remplit ficcat avec les valeurs de la biomasse et de la matière organique qui sont dans trav */
    /* fprintf(stderr,"\n nombre de patch en dehors des boules = %d",nbout);*/

    fprintf(ptficatt,"%d \n",5);

    for(i=0;i<nboules;i++)
    {
        fprintf(ptficatt,"%f %f %f %f %f\n",trav[4][i+1],trav[5][i+1],0.0,0.0,0.0); 
    }

    fclose(ptficatt);
    fclose(ptficboules); 
    fclose(ptficdonnees);
}


/* MEME CHOSE QUE COMPCONNEXEVAL MAIS EN RETOURNANT SIMPLEMENT UN TABLEAU CONTENANT POUR CHAQUE BOULE LE NUMERO DE SA COMPOSANTE CONNEXE AINSI
QUE LE POURCENTAGE DU VOLUME TOTAL DES BOULES A LAQUELLE ELLE CORRESPOND */

int **compconnexesvalbis(graphe_s *graphe)
{
	int *etiq,nboules,i,j,natraiter,*atraiter,*compco,ncompco,k,nvmax,nvois,*vois,p,l,numco, nbouleau,numax,nbcompmax,**compboul;
	float vco,vtotal,somr,pourcmax;
	double vmoyen;

	nboules = graphe->nel;

	compboul = allot2i(2,nboules+1); 

	fprintf(stderr,"\n nb de boules = %d",nboules);

	etiq = (int*)mycalloc(nboules+1,sizeof(int));
	atraiter = (int*)mycalloc(nboules,sizeof(int));
	compco = (int*)mycalloc(nboules,sizeof(int));

	/* initialisation de compboul a zero */

	for(i=0;i < 2;i++)
	{
		for(j=0;j <= nboules;j++)
		{
			compboul[i][j] = 0; 
		}
	}


	vtotal = 0;
	nvmax = 1000;

	vois = (int*)mycalloc(nvmax,sizeof(int));

	/* calcul du volume total en pixels cube */
	somr = 0;
	for(i=1;i<=nboules;i++)
	{
		vtotal += (4.0/3.0)*M_PI*(graphe->carac[4][i])*(graphe->carac[4][i])*(graphe->carac[4][i]);
		somr += graphe->carac[4][i];
	}
	somr = somr/nboules;

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

			/* stockage des informations dans le tableau de sortie compboul */ 
			compboul[0][i] = i; 
			compboul[1][i] = neari((vco/vtotal)*100); 
			for(p=0;p<ncompco;p++)
			{
				k = compco[p];
				if(compboul[0][k] == 0)
				{
					compboul[0][k] = i; 
					compboul[1][k] = neari((vco/vtotal)*100); 
				}
			}
		}

	}
	return(compboul);
}


/* CETTE PROCEDURE FAIT LA MEME CHOSE QUE SIMULSYNCHCOURBASAS AVEC UN SCHEMA ASYNCHRONE DIFFUSION-TRANSFORMATION incluant deux pas de temps différents */


graphe_s *simulsynchcourbasync(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int fracbio, int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel)
{
	graphe_s *graphe;
	int nbnoeud,i,j,k,n,*tavois,nvois,ech,natt;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,**tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,prop,**deltatbio,*deltatdif,car1[5],**car,propmax;
	FILE *ptficparbio,*ptficmatlab,*ptficexcel,*ptficmasses; 
	float ro,mu,rom,vfom,vsom,vdom,kab,somdelta,somdeltabio,somdeltadif; 
	double massetot1,massetot2,rapport,massetr,massetotdebut,massetotfin; 
	float temps,tempstot,totacarb,tempssimul,biodeltat; 
	clock_t t1,t2,t3,t4,t5; 
	int n1,n2,fracjour,nbels,l; 

	t3 = clock(); 

	graphe = lecgraphe(ficgraphe);

	nbnoeud = graphe->nbnoeuds; 

	ptficparbio = fopen(ficparbio,"r");

	ptficmatlab = fopen(ficmatlab,"w");

	ptficexcel = fopen(ficexcel,"w");

	ptficmasses = fopen(ficmasses,"w");

	fscanf(ptficparbio,"%f %f %f %f %f %f %f",&ro,&mu,&rom,&vfom,&vsom,&vdom,&kab);

	fprintf(stderr,"\n coeffdom = %f ro=%f ; mu=%f ; rom=%f ; vfom=%f ; vsom=%f ; vdom=%f ; kab=%f ",coeffdom,ro,mu,rom,vfom,vsom,vdom,kab);
	fprintf(stderr,"\n nombre de noeuds=%d",nbnoeud); 


	petitdeltat = deltat/fraction ;

	ech = neari((1.0/24.0)*(1/petitdeltat)); 

	/* ech represente le nombre d'iterations correspondant a une heure de temps */

	fprintf(stderr,"\n deltat=%f jours fraction = %d fracbio = %d",deltat,fraction,fracbio); 

	fprintf(stderr,"\n petitdeltat=%f jours ; ech=%d",petitdeltat,ech); 
	
	/* calcul des masses totales pour bilan des masses */

	massetotdebut = bilanmasses(graphe); 

	massetot1 = bilanmasses(graphe); 

	fprintf(stderr,"\n bilan total des masses au debut de simulsynchcourb = %lf",massetotdebut); 

	fprintf(stderr,"\n");
	t1 = clock(); 
	tempstot = 0; 

	natt = 5; 
	nbels = fraction/ech + 1; 
	car = allot2f(nbels + 1,natt);

	n1 =0; 

	propmax = 0; 

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
		
		/* pour chaque noeud on calcule la somme des deltas pour la diffusion pour chaque intervalle (deltat/n) */
		/* ensuite on simule de maniere synchrone la diffusion de la DOM */

		deltatdif = miseajourdif(graphe,2,coeffdom,petitdeltat,nvmax);
		
		/* mise a jour des attributs du graphe avec le tableau tabdelta */
		for(i = 1 ; i <= nbnoeud ; i++)
		{
			/* mise a jour de la DOM qui diffuse avec le tableau des deltas */
				graphe->carac[6][i] += deltatdif[i];
		}

		/* rectification de la DOM diffusee en cas de valeurs negatives */
		masseneg = 0; 
		massetot = 0; 

		for(i=1;i<=nbnoeud;i++)
		{
			if(graphe->carac[6][i] < 0)
			{
				masseneg += -graphe->carac[6][i];
			}
			else
			{
				massetot += graphe->carac[6][i];
		
			}
		}

		/* on regarde si la rectification est possible et sinon on s'arrete */
		if(masseneg > massetot)
		{
			fprintf(stderr,"\n valeurs negatives plus importantes que valeurs positives !");
			fprintf(stderr,"\n masses negatives = %f ; masses positives = %f ; total des masses en algébrique = %f",masseneg,massetot,massetot-masseneg);
			fprintf(stderr,"\n pourcentage du total des masses par rapport a la masse totale = %f",((-massetot+masseneg)/massetot1)*100);
			exit(3); 
		}

		if(masseneg > 0)
		{
			/* on traite les cas ou des valeurs de la caracteristique 2 (DOM) seraient negatives */
			prop = (masseneg/massetot)*100 ;
			if(prop > propmax)
			{
				propmax = prop; 
			}

			if(prop > pourcneg)
			{
				fprintf(stderr,"\n reduire le petit pas de temps car proportion de valeurs negatives trop importante = %f  \n",prop);
				exit(3); 
			}

			for(i = 1 ; i <= nbnoeud ; i++)
			{
				if(graphe->carac[6][i] <= 0)
				{
					graphe->carac[6][i] = 0;
				}
				else
				{
					graphe->carac[6][i] -= (graphe->carac[6][i]/massetot)*masseneg;
					if(graphe->carac[6][i] < 0)
					{
						fprintf(stderr,"\n rectification des valeurs negatives impossible niveau 2, reduire le pas de temps");
						exit(1); 
					}
				}
			}
		}
		
		/* liberation de l'espace pointe par deltatdif */
		myfreef(deltatdif); 

		/* la diffusion a ete simulee, on simule maintenant les transformations tous les fracbio fois, fraction est divisible par fracbio; 
		toutes les valeurs des attributs sont positives */ 
		if( ((n+1)%fracbio) == 0)
		{
			biodeltat = fracbio*petitdeltat;
			deltatbio = miseajourbioas(graphe,biodeltat,ro,mu,rom,vfom,vsom,vdom,kab);
			fprintf(stderr,"\n n = %d ; biodeltat = %f minutes ",n,biodeltat*24*60); 

			for(i = 1 ; i <= nbnoeud ; i++)
			{
				/* mise a jour des attributs avec le tableau des deltas de la biologie */
				for(j=1;j<=5;j++)
				{
					graphe->carac[4+j][i] += deltatbio[j][i];
				}
			}
			freetf(deltatbio,5,nbnoeud);
		}

		/* la diffusion puis les transformations ont ete simulees de maniere asynchrone */

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
			massetot1 = bilanmasses(graphe); 
			fprintf(stderr,"\n temps de la simulation en jours =%f ",(n+1)*petitdeltat); 
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
			fprintf(stderr,"\n bilan total des masses = %lf",massetot1); 
			t5 = clock(); 
			temps = (float)(t5 - t3) / CLOCKS_PER_SEC;
			temps = temps/60.0;
			fprintf(stderr,"\n temps de calcul intermediaire pour la simulation synchrone explicite = %f' ",temps); 
		}
	}
	/* a la fin de la boucle le tableau car[i][j] contient pour le temps numero i (en heures) la somme des attributs adresses par j */
	totacarb = massetot1; 
	fracjour = 24; 
	nbels = n1; 
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

	fprintf(ptficmatlab,"plot(X,Y0,'b',X,Y1,'g',X,Y2,'c',X,Y3,'c',X,Y4,'r');");
	fprintf(ptficmatlab,"\n");

	t4= clock(); 
	temps = (float)(t4 - t3) / CLOCKS_PER_SEC;
	temps = temps/60.0;

	fprintf(stderr,"\n temps total pour la simulation synchrone explicite = %f minutes",temps); 

	massetotfin = bilanmasses(graphe); 


	if(massetotfin > massetotdebut)
	{
		rapport = massetotdebut/massetotfin; 
	}
	else
	{
		rapport = massetotfin/massetotdebut; 
	}
	fprintf(stderr,"\n rapport des bilans de masse entre 0 et 1 = %lf ; proportion valeurs negatives maximum = %f",rapport,propmax); 
	/* ecriture du nouveau graphe value apres diffusion, seules les valeurs des attributs ont pu changer */
	
	ecrigraphe(graphe,ficgrapher);

	fclose(ptficparbio); 
	fclose(ptficmatlab);
	fclose(ptficexcel);
	fclose(ptficmasses);

	return(graphe); 
}

/* CETTE FONCTION N'A PAS ETE TESTEE */
/* amelioration de la connectivite du graphe des boules en construisant un graphe de boules a partir de la triangulation des centres des boules 
on construit d'abord les arcs correspondant aux relations de connexite et ensuiste on ameliore avec delaunay */

void arcssimpdelau(char *cyl,char *cyl2,int dimx, int dimy, int dimz,int *nbarcs,int nmaxv,float seuilr,char *mesh,float seuilp)

{

	FILE *ptboules,*ptarcs,*pttrav;

	int nboules,i,npoints,l,**ltet,nbtet,nbs;

	float x0,y0,z0,r,a2,u,v,w,*Xb,*Yb,*Zb,*Rb,rap,**lpoints,xmax,ymax,zmax,dist,dist1,xm,ym,zm;

	int dx,dy,dz,xc,yc,zc,m,p1,p2,p3,p4,c1,c2,c3,c4,narcplus;

	int j,k,a,b,c,i1,j1,k1,i2,j2,k2,*voisins,nv,evois,p,stop,*corres;

    bool **arcs;


	*nbarcs = 0;

	ptboules = fopen(cyl,"r");

	ptarcs = fopen(cyl2,"w");

	pttrav = fopen("arc.trav","w");

	fscanf(ptboules,"%d",&nboules);

	fprintf(stderr,"\n nboules = %d",nboules);

    arcs = allot2ic(nboules+1,nboules+1);

    corres = (int*)mycalloc(nboules,sizeof(int)); 

    for(i=0;i<nboules+1;i++)
    {
        for(j=0;j<nboules+1;j++)
        {
            arcs[i][j] = false; 
        }
    }
    

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
		/* determination des arcs incluant la boule j */
		for(k=j+1;k<=nboules;k++)
		{
			rap = sqrt((Xb[j]-Xb[k])*(Xb[j]-Xb[k])+(Yb[j]-Yb[k])*(Yb[j]-Yb[k])+(Zb[j]-Zb[k])*(Zb[j]-Zb[k])) - (Rb[j]+Rb[k]);
			if(rap <= seuilr)
			{
				/* creation d'un arc */
				fprintf(pttrav,"\n %d %d",j,k);
                arcs[j][k] = true;
                arcs[k][j] = true; 
				(*nbarcs)++;
			}

		}
	}

	fprintf(stderr,"\n nombre total d'arcs apres premiere passe = %d",*nbarcs);


     /* amelioration de la cnnectivite avec Delaunay */

    /* lecture du fichier de tetraedres correspondant a la triangulation des centres des boules */

    lfictet(mesh,&lpoints,&ltet,&nbtet,&nbs,&xm,&ym,&zm);

    /* on definit les correspondances entre le fichier points du mesh et les indices des boules maximales */
    /* initialisation de corres */

    fprintf(stderr,"\n remplissage du tableau des correspondances");

    for(i=0;i<nboules;i++)
    {
        corres[i] = 0;
    }

    /* on remplit corres, corres[i] est le numero de la boule (à partir de 1) du point i+1 du fichier mesh */

    for(i=0;i<nboules;i++)
    {
        /* on remplit corres[i] */
        dist = 100000000;
        for(j=1;j<nboules+1;j++)
        {
            dist1 = (lpoints[0][i]-Xb[j])*(lpoints[0][i]-Xb[j]) + (lpoints[1][i]-Yb[j])*(lpoints[1][i]-Yb[j]) + (lpoints[2][i]-Zb[j])*(lpoints[2][i]-Zb[j]);
            if(dist1 < dist)
            {
                corres[i] = j; 
                dist = dist1; 
            }
        }
    }
    /* la boule referencee par i (a partir de 1) dans le tableau des tetraedres ltet correspond a la boule corres[i-1] dans le fichier des boules maximales (a partir de 1) */

    /* amelioration connectivite avec les tetraedres de Delaunay */
    narcplus = 0; 
    for(i=0;i<nbtet;i++)
    {
        p1 = ltet[0][i];
        p2 = ltet[1][i];
        p3 = ltet[2][i];
        p4 = ltet[3][i];
        /* p1,p2,p3,p4 sont indexes a partir de 1 */
        c1 = corres[p1-1]; 
        c2 = corres[p2-1]; 
        c3 = corres[p3-1]; 
        c4 = corres[p4-1]; 

        /* c1,c2,c3,c4 sont les numeros des boules correspondantes dans le fichier des boules max, les indices debutent a 1 */
        /* on regarde les aretes c1-c2, c1-c3, c1-c4, c3-c4, c2-c3, c2-c4 */
        
        if(arcs[c1][c2] == false)
        {
            dist = sqrt((Xb[c1]-Xb[c2])*(Xb[c1]-Xb[c2]) + (Yb[c1]-Yb[c2])*(Yb[c1]-Yb[c2]) + (Zb[c1]-Zb[c2])*(Zb[c1]-Zb[c2])) - Rb[c1] - Rb[c2];
            if(dist <= seuilp)
            {
                fprintf(pttrav,"\n %d %d",c1,c2);
                arcs[c1][c2] = true;
                arcs[c2][c1] = true; 
				(*nbarcs)++;
                narcplus++; 
            }
        }

        if(arcs[c1][c3] == false)
        {
            dist = sqrt((Xb[c1]-Xb[c3])*(Xb[c1]-Xb[c3]) + (Yb[c1]-Yb[c3])*(Yb[c1]-Yb[c3]) + (Zb[c1]-Zb[c3])*(Zb[c1]-Zb[c3])) - Rb[c1] - Rb[c3];
            if(dist <= seuilp)
            {
                fprintf(pttrav,"\n %d %d",c1,c3);
                arcs[c1][c3] = true;
                arcs[c3][c1] = true; 
				(*nbarcs)++;
                narcplus++; 
            }
        }

        if(arcs[c1][c4] == false)
        {
            dist = sqrt((Xb[c1]-Xb[c4])*(Xb[c1]-Xb[c4]) + (Yb[c1]-Yb[c4])*(Yb[c1]-Yb[c4]) + (Zb[c1]-Zb[c4])*(Zb[c1]-Zb[c4])) - Rb[c1] - Rb[c4];
            if(dist <= seuilp)
            {
                fprintf(pttrav,"\n %d %d",c1,c4);
                arcs[c1][c4] = true;
                arcs[c4][c1] = true; 
				(*nbarcs)++;
                narcplus++; 
            }
        }
            
    
        if(arcs[c3][c4] == false)
        {

            dist = sqrt((Xb[c4]-Xb[c3])*(Xb[c4]-Xb[c3]) + (Yb[c4]-Yb[c3])*(Yb[c4]-Yb[c3]) + (Zb[c4]-Zb[c3])*(Zb[c4]-Zb[c3])) - Rb[c4] - Rb[c3];
            if(dist <= seuilp)
            {
                fprintf(pttrav,"\n %d %d",c4,c3);
                arcs[c4][c3] = true;
                arcs[c3][c4] = true; 
				(*nbarcs)++;
            }
        }

        if(arcs[c2][c3] == false)
        {
            dist = sqrt((Xb[c2]-Xb[c3])*(Xb[c2]-Xb[c3]) + (Yb[c2]-Yb[c3])*(Yb[c2]-Yb[c3]) + (Zb[c2]-Zb[c3])*(Zb[c2]-Zb[c3])) - Rb[c2] - Rb[c3];
            if(dist <= seuilp)
            {
                fprintf(pttrav,"\n %d %d",c2,c3);
                arcs[c2][c3] = true;
                arcs[c3][c2] = true; 
				(*nbarcs)++;
                narcplus++; 
            }
        }

        if(arcs[c2][c4] == false)
        {
            dist = sqrt((Xb[c4]-Xb[c2])*(Xb[c4]-Xb[c2]) + (Yb[c4]-Yb[c2])*(Yb[c4]-Yb[c2]) + (Zb[c4]-Zb[c2])*(Zb[c4]-Zb[c2])) - Rb[c4] - Rb[c2];
            if(dist <= seuilp)
            {
                fprintf(pttrav,"\n %d %d",c4,c2);
                arcs[c4][c2] = true;
                arcs[c2][c4] = true; 
				(*nbarcs)++;
                narcplus++; 
            }
        }
            

    }
	/* ecriture du fichier des arcs final avec le nombre d'arcs en tete du fichier */

	fclose(pttrav);

	pttrav = fopen("arc.trav","r");

	fprintf(ptarcs,"%d",*nbarcs);

	for(i = 0;i<*nbarcs;i++)
	{
		fscanf(pttrav,"%d %d",&j,&k);
		fprintf(ptarcs,"\n %d %d",j,k);

		
	}

    fprintf(stderr,"\n nombre arcs rajoutes avec Delaunay = %d",narcplus);

	fclose(ptarcs);

	fclose(pttrav);

	remove("arc.trav");

	free(Xb);
	free(Yb);
	free(Zb);
	free(Rb);
}

/*CETTE FONCTION N'A PAS ETE TESTEE */
void testcregraphsoilmu3d3()

{
	char ficboules[50],ficatt[50],racine[50],chaine[50];
	char ficgraphe[50],ficarcs[50],mesh[50];
	graphe_s *graphe;
	int nbarcs,dimx,dimy,dimz,nbmaxvois;
	float seuil,seuilp; 


    /* on lit le fichier des boules maximum, les attributs noeuds initiaux (fichier), les parametres du modele biologique (fichier), l'intervalle de temps total
    ainsi que la discretisation voulue (fichier) */

    fprintf(stderr,"\n racine des fichiers : ");
	scanf("%s",racine);

    fprintf(stderr,"\n fichier de la triangulation de Delaunay des centres : ");
	scanf("%s",mesh);

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
	fprintf(stderr,"\n seuil pour connexite: distance centres - somme rayons <= ");
	scanf("%f",&seuil); 

    fprintf(stderr,"\n seuil pour connexite Delaunay: distance centres - somme rayons <= ");
	scanf("%f",&seuilp); 


    arcssimpdelau(ficboules,ficarcs, dimx,dimy,dimz,&nbarcs,nbmaxvois,seuil,mesh,seuilp);
	fprintf(stderr,"\n nombre d'arcs du graphe = %d",nbarcs);
    
    /* creation du graphe correspondant a ficboules et a ficatt, on a besoin de ficboules pour ecrire les caracteristiques des boules */
    graphe = cregraphe_val(ficboules,ficarcs,ficatt);
	fprintf(stderr,"\n graphe->nbnoeuds = %d ; graphe->nel = %d ; graphe->ncar = %d",graphe->nbnoeuds,graphe->nel,graphe->ncar);

    /* ecriture du graphe dans le fichier ficgraphe */ 
    ecrigraphe(graphe,ficgraphe);
}


/* cette fonction calcule les deltas avec la methode implicite et rend les nouvelles valeurs des concentrations en meoire vive et aussi dans un fichier */

float *miseajourdifimp(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax,char *ficent)
{
	int nbnoeud,i,j,k,n,*vois,nvois,narcs,*ligne,*colonne,nmat,ntotmat;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,*tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,massetot2,prop,vtot,sommedelta,vartr,theta,*sommetheta,*nconc1,*nconc2,diag,*volume,*mat;
	float rboul1,rs; 
	clock_t t1,t2,t3,t4;
	FILE *ptficent; 

	ptficent = fopen(ficent,"w"); 
	
	vois = (int*)mycalloc(nvmax,sizeof(int));
	nbnoeud = graphe->nbnoeuds; 
	narcs = graphe->nco;

	/* nconc1 et nconc2 contient les anciennes et les nouvelles concentrations de la caracteristique numcar */
	nconc2 = (float*)mycalloc(nbnoeud +1,sizeof(float)); 
	nconc1 = (float*)mycalloc(nbnoeud +1,sizeof(float)); 
	volume = (float*)mycalloc(nbnoeud +1,sizeof(float)); 

	/* initialisation de nconc1, de nconc2 et de volume a zero. CES INITIALISATIONS SONT INUTILES ET COUTENT DU TEMPS DE CALCUL */
	/* for(i=0;i<=nbnoeud;i++)
	{
		nconc1[i] = 0;
		nconc2[i] = 0; 
		volume[i] = 0;
	}*/

	/* mat(i) contient la valeur du ieme element non nul de la matrice et est indexe a partir de 0 */
	ntotmat = narcs + nbnoeud; 

	/* fprintf(stderr,"\n nombre d'elements a priori non nuls de la matrice = %d",ntotmat); */

	mat = (float*)mycalloc(ntotmat,sizeof(float)); 

	/* ligne(i) et colonne(i) contiennent les numeros de la ligne et de la colonne de la iemme valeur non nulle de la matrice et sont indexes a partir de 0 */
	ligne = (int*)mycalloc(ntotmat,sizeof(int)); 
	colonne = (int*)mycalloc(ntotmat,sizeof(int)); 

	/* initialisation de ligne, colonne et mat a zero LES INITIALISATIONS DES TABLEAUX A ZERO SONT INUTILES ET COUTENT DU TEMPS DE CALCUL */
	/* for(i=0;i< ntotmat;i++)
	{
		ligne[i] = 0;
		colonne[i] = 0;
		mat[i] = 0; 
	}*/

	/* DANS LA VERSION DEBUGEE IL FAUDRA ENLEVER LES INUTIALISATIONS DE NCONC1, NCONC2, VOLUME, LIGNE, COLONNE ET MAT QUI COUTENT DU TEMPS DE CALCUL */

	nmat = 0; 

	/* fprintf(stderr,"\n debut du remplissage des termes non diagonaux de la matrice"); */

	/* remplissage des elements non diagonaux de la matrice */
	for(i=1;i<=narcs;i++)
	{
		k = graphe->couple[1][i];
		n = graphe->couple[2][i];
		fprintf(stderr,"\n arc (%d,%d) ; noeuds indexes a partir de 1",k,n);
		/* l'arc (k,n) existe */
		/* calcul de la valeur de theta */
		x1 = graphe->carac[1][k];
		y1 = graphe->carac[2][k];
		z1 = graphe->carac[3][k];
		x2 = graphe->carac[1][n];
		y2 = graphe->carac[2][n];
		z2 = graphe->carac[3][n];
		rboul1 = graphe->carac[4][k];
		rboul2 = graphe->carac[4][n];
		fprintf(stderr,"\n x1=%f ; y1=%f ; z1=%f ; x2=%f ; y2=%f ; z2=%f ; rboul1=%f ; rboul2=%f",x1,y1,z1,x2,y2,z2,rboul1,rboul2); 
		distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
		rs = (2.0*rboul1*rboul2)/(rboul1+rboul2); 
		/* L'AIRE D'UN DISQUE EST PIE*R*R ET NON 4*PIE*R*R !! CE QU'ON A FAIT REVIENT A MULTIPLIER PAR 4 LE COEFFICIENT DE DIFFUSION */
		scont = 4*3.1416*rs*rs; 
		theta = -(coeff*scont*deltat)/distance; 

		fprintf(stderr,"\n distance = %f ; rs = %f ; scont = %f ; theta = %f ; nmat = %d ",distance,rs,scont,theta,nmat);

		/* on considere la partie superieure de la matrice symetrique c'est a dire telle que le numero de la ligne est plus pett que le numero de la colonne 
		comme le graphe a ete construit en regardant les voisinages des boules l'ordre des couples est lexicographique avec k < n */

		ligne[nmat] = k;
		colonne[nmat] = n;
		mat[nmat] = theta;
		nmat++; 
	}

	/* fprintf(stderr,"\n fin du remplissage des termes non diagonaux de la matrice");*/

	/* remplissage du tableau des volumes et de celui des concentrations initiales */

	/* fprintf(stderr,"\n nbnoeud = %d ; numcar = %d",nbnoeud,numcar); */

	for(i = 1;i<=nbnoeud;i++)
	{
		/* fprintf(stderr,"\n i = %d",i);*/ 

		rboul1 = graphe->carac[4][i];
		/* fprintf(stderr,"\n rboul1 = %f",rboul1); */
		volume[i] = (4.0/3.0)*3.1416*rboul1*rboul1*rboul1; 
		/* fprintf(stderr,"\n volume = %f ; carac = %f ",volume[i],graphe->carac[numcar + 4][i]); */
		nconc1[i] = (graphe->carac[numcar + 4][i])/volume[i]; 
		/* fprintf(stderr,"\n nconc1 = %f",nconc1[i]); */
	}
	

	/* fprintf(stderr,"\n debut du remplissage des termes diagonaux de la matrice");*/

	/* remplissage de la diagonale de la matrice */ 
	for(i = 1;i<=nbnoeud;i++)
	{
		diag = 0; 
		x1 = graphe->carac[1][i];
		y1 = graphe->carac[2][i];
		z1 = graphe->carac[3][i];
		rboul1 = graphe->carac[4][i];
		voisins(i,graphe,nvmax,&nvois,&vois);
		for(j=0;j<nvois;j++)
		{
			n = vois[j]; 
			/* le noeud n est voisin du noeud i */

			x2 = graphe->carac[1][n];
			y2 = graphe->carac[2][n];
			z2 = graphe->carac[3][n];
			rboul2 = graphe->carac[4][n];
			distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
			rs = (2.0*rboul1*rboul2)/(rboul1+rboul2); 
			scont = 4*3.1416*rs*rs; 
			theta = -(coeff*scont*deltat)/distance; 
			diag -= theta; 
		}
		diag += (4.0/3.0)*3.1416*rboul1*rboul1*rboul1;
		ligne[nmat] = i;
		colonne[nmat] = i;
		mat[nmat] = diag;
		nmat++; 
	}

	/* if(nmat != ntotmat)
	{
		fprintf(stderr,"\n erreur, nmat different de ntotmat !!");
		exit(1); 
	}*/
	/* entree de la procedure de resolution du systeme par le gradient conjugue: nbnoeud,nmat, ligne, colonne, mat, volume, nconc1 */
	/* la fonction de resolution du systeme donne nconc2 tableau des valeurs des concentrations a l'instant t+deltat */

	/* APPEL DE LA FONCTION QUI RESOUD LE SYSTEME AVEC LE GRADIENT CONJUGUE */

	/* on met les donnees necessaires a la resolution du systeme dans le fichier ficent*/

	fprintf(ptficent,"%d %d",nbnoeud,nmat);
	fprintf(ptficent,"\n"); 
	/* nbnoeud est le nombre de boules et nmat le nombre d'elements non nuls (a priori) de la matrice A */

	/* on met les tableaux ligne colonne et mat, ces tableaux sont en desordre, on pourrait rajouter un tri selon l'ordre lexicographique en fonction des lignes et colonnes */
	for(i=0;i<nmat;i++)
	{
		fprintf(ptficent,"\n%d %d %f",ligne[i],colonne[i],mat[i]); 
	}
	fprintf(ptficent,"\n"); 
	/* on met le tableau des valeurs des volumes et des valeurs initiales des concentrations */
	for(i=1;i<=nbnoeud;i++)
	{
		fprintf(ptficent,"\n%f %f ",volume[i],nconc1[i]); 
	}

	/* liberation de la place allouee dynamiquement pour les tableaux */
	myfreei(vois); 
	myfreei(ligne); 
	myfreei(colonne); 
	myfreef(mat); 
	myfreef(volume); 
	myfreef(nconc1); 
	fclose(ptficent); 
	return(nconc2); 
}

void testmiseajourdifimplic()
{
	float *nconc2,coeff,deltat,eps;
	int nvmax,numcar; 
	graphe_s *graphe; 
	char ficgraphe[50],ficent[50]; 

	fprintf(stderr,"\n fichier du graphe initial ; numero de l'attribut a diffuser ; coefficient de diffusion ; delta t en jour ; nb max de voisins par noeud ; fichier graphe final ; epsilon pour gradient \n");
	scanf("%s %d %f %f %d %s %f",ficgraphe,&numcar,&coeff,&deltat,&nvmax,ficent,&eps); 
	fprintf(stderr,"\n fichier graphe initial: %s ; numero attribut: %d ; coefficient de diffusion: %f ; pas de temps: %f ; nb max de vosiins: %d ; fichier graphe final: %s eps=%f \n",ficgraphe,numcar,coeff,deltat,nvmax,ficent,eps);

	graphe = lecgraphe(ficgraphe);
	fprintf(stderr,"\n fin de la lecture du graphe");

	miseajourdifimplic(graphe,numcar,coeff,deltat,nvmax,eps); 

	ecrigraphe(graphe,ficent); 

}

void testmiseajourdifimp()
{
	float *nconc2,coeff,deltat;
	int nvmax,numcar; 
	graphe_s *graphe; 
	char ficgraphe[50],ficent[50]; 

	fprintf(stderr,"\n fichier du graphe initial ; numero de l'attribut a diffuser ; coefficient de diffusion ; delta t en jour ; nb max de voisins par noeud ; fichier sortie \n");
	scanf("%s %d %f %f %d %s",ficgraphe,&numcar,&coeff,&deltat,&nvmax,ficent); 
	fprintf(stderr,"\n fichier graphe: %s ; numero attribut: %d ; coefficient de diffusion: %f ; pas de temps: %f ; nb max de vosiins: %d ; fichier sortie: %s \n",ficgraphe,numcar,coeff,deltat,nvmax,ficent);

	graphe = lecgraphe(ficgraphe);
	fprintf(stderr,"\n fin de la lecture du graphe");

	nconc2 = miseajourdifimp(graphe,numcar,coeff,deltat,nvmax,ficent); 
}

void triinsertion(int *t,int n)
{
    int i,j,aux,min;
    for (i=0;i<n-1;i++)
    {
        min=i;
        for (j=i+1;j<n;j++)
            if (t[j]<t[min]) min=j;
        if (min!=i)
        {
            aux=t[i];
            t[i]=t[min];
            t[min]=aux;
        }
    }
}


/* cette fonction calcule les deltas avec la methode implicite et rend les nouvelles valeurs des concentrations en memoire vive et aussi dans un fichier */
/* Il faut ajouter en entrée de cette fonction T la durée de la simulation */
/* Cette fonction effectue la diffusion avec schéma implicite de l'attribut numcar du graphe et met a jour le graphe */
void miseajourdifimplic(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax,float eps)
{
	int nbnoeud,i,j,k,n,*vois,nvois,narcs,*ptligne,*colonne,nmat,ntotmat,nbC,*colC,*ptC,nbitermax=500000;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,*tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,massetot2,prop,vtot,sommedelta,vartr,theta,*sommetheta,*nconc1,*nconc2,diag,*volume,*mat,*matC;
	float rboul1,rs,tsimul,*trav1,*trav2,norme,*trav3;
	double masset1,masset2,rapport;
	FILE *ptficsort;
	clock_t t1,t2,t3,t4; 

	t1= clock(); 
	/* ecrigraphe(graphe,"graphe2"); */
	/* masset1 = bilanmasses(graphe); */
	vois = (int*)mycalloc(nvmax,sizeof(int));
	nbnoeud = graphe->nbnoeuds;
	narcs = graphe->nco;

	/* nconc1 et nconc2 contient les anciennes et les nouvelles concentrations de la caracteristique numcar */
	nconc2 = (float*)mycalloc(nbnoeud,sizeof(float));
	nconc1 = (float*)mycalloc(nbnoeud,sizeof(float));
	volume = (float*)mycalloc(nbnoeud,sizeof(float));

	ntotmat = narcs + nbnoeud;

	/* mat(i) contient la valeur du ieme element non nul de la matrice A et est indexe a partir de 0
	   les coefficients sont rangés dans mat ligne par ligne et dans l'ordre
	   seule la partie triangulaire inférieure est stockée */
	mat = (float*)mycalloc(ntotmat,sizeof(float));

	/* colonne(i) contient le numeros de la colonne de la iemme valeur non nulle de la matrice mat et
       ptligne(i) contient la position dans mat du dernier element non nul de la ligne i (ie l'element diagonal)
       et ils sont indexes a partir de 0 */
	ptligne = (int*)mycalloc(nbnoeud,sizeof(int));
	colonne = (int*)mycalloc(ntotmat,sizeof(int));

	/* DANS LA VERSION DEBUGEE IL FAUDRA ENLEVER LES INUTIALISATIONS DE NCONC1, NCONC2, VOLUME, LIGNE, COLONNE ET MAT QUI COUTENT DU TEMPS DE CALCUL */

	nmat = 0;
	/* Fusion des trois boucles :
       remplissage de la matrice, des tableaux des volumes et des concentrations initiales */

	
	/* initialisation de ptligne */
	/* 	DANS L'EXECUTION ON A JUSTE BESOIN DE METTRE PTLIGNE[0] A ZERO */
	for(i = 0;i<nbnoeud;i++)
	{
		ptligne[i] = 0;
	}
	for(i = 0;i<ntotmat;i++)
	{
		mat[i] = 0;
		colonne[i] = 0; 
	}
	for(i = 0;i<nbnoeud;i++)
	{
		/* fprintf(stderr,"\n i = %d",i); */
		diag = 0;
		x1 = graphe->carac[1][i+1];
		y1 = graphe->carac[2][i+1];
		z1 = graphe->carac[3][i+1];
		rboul1 = graphe->carac[4][i+1]; 
		voisins(i+1,graphe,nvmax,&nvois,&vois);
		/* tri des voisins dans l'ordre croissant */
		/* fprintf(stderr,"\n nvois=%d avant triinsertion",nvois); */
		if(nvois>1) triinsertion(vois,nvois);
		/* for(k=0;k<nvois;k++)
		{
			fprintf(stderr,"\n vois(%d) = %d",k,vois[k]); 
		}
		fprintf(stderr,"\n nvois=%d apres appel de triinsertion",nvois); */
		for(j=0;(j<nvois)&&((vois[j]-1)<i);j++)
		{
			n = vois[j];
			/* fprintf(stderr,"\n j=%d ; n=%d ",j,n);*/
			/* le noeud n est voisin du noeud i */

			x2 = graphe->carac[1][n];
			y2 = graphe->carac[2][n];
			z2 = graphe->carac[3][n];
			rboul2 = graphe->carac[4][n];
			distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
			rs = (2.0*rboul1*rboul2)/(rboul1+rboul2);
			scont = 4*3.1416*rs*rs;
			theta = -(coeff*scont*deltat)/distance;
			diag += -theta;
            colonne[nmat] = n-1;
            mat[nmat] = theta;
            mat[ptligne[n-1]] += -theta;
			/*if((n-1)==0)
			{
				fprintf(stderr,"\n theta=%f ; i=%d ",theta,i); 
			}*/
            nmat++;
		}
		volume[i] = (4.0/3.0)*3.1416*rboul1*rboul1*rboul1;
		/* diag += volume[i];*/
		ptligne[i] = nmat;
		colonne[nmat] = i;
		mat[nmat] = diag;
        nconc1[i] = (graphe->carac[numcar + 4][i+1])/volume[i];
		nmat++;
	}
	
	for(i=0;i<nbnoeud;i++)
	{
		mat[ptligne[i]] += volume[i]; 
		/* fprintf(stderr,"\n volume = %f",volume[i]); */
	}
	/* fprintf(stderr,"\n nmat sortie boucle = %d",nmat); */
    /* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
    /* POINT D'ENTREE POUR LE GRADIENT CONJUGUE */
    /* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

	/* entree de la procedure de resolution du systeme par le gradient conjugue: nbnoeud,nmat, ligne, colonne, mat, volume, nconc1 */
	/* la fonction de resolution du systeme donne nconc2 tableau des valeurs des concentrations a l'instant t+deltat après
	   chaque pas de temps */

	/* APPEL DE LA FONCTION QUI RESOUD LE SYSTEME AVEC LE GRADIENT CONJUGUE */
    /* "*mat" CONTIENT LES "nmat" ELEMENTS NON NULS A PRIORI DE LA MATRICE A, "*mat" est indexe a partir de 0.
       on a "nbnoeud" noeuds du graphe et "narcs" arcs dans le graphe (en dehors de la diagonale on a narcs valeurs non nulles
       pour A).
       "*colonne" contient les coordonnees en colonne des elemnts de "*mat", cad colonne[i] est la colonne dans A de mat[i].
       "*ligne" contient la position dans "*mat" du dernier element de la ligne (ie de l'element diagonal): mat[ligne[i]]=A[i,i]
       Les valeurs non nulles de la matrice dans "*mat" sont rangees ligne par ligne et dans l'ordre et seule la partie
       triangulaire inférieure est stockée
       "*nconc1" qui contient les valeurs initiales de la concentration, "*conc1" est indexe a partir de 0 et contient les
       concentrations des "nbnoeud" boules multiplié par le volume de la boule correspondante
       "*volume" contient les valeurs du volume des boules et est aussi indexe a partir de 0.

    LA FONCTION miseajourdifimp (SERGE MOTO DOIT PRENDRE EN ENTREE: "*mat" *float,"*ligne" *int,"*colonne" *float,"*volume" *float,"*nconc1" *float, narcs int (nb arcs), nbnoeud int (nb boules),
Ax = V-1.b)
    appelle les fonctions precondmat qui calcule la matrice de précondionnement matC du système et GradienConjugué qui résout le
    système Ax = M-1.b (ie A*nconc2 = volume*nconc1 ) et elle rend dans "*nconc2" *float tel que nconc2[i] = concentration a la
    fin de la simulation */

/* initialisation de la matrice de préconditionnement matC, de sont pointeur ligne ptC et de son tableau de colonnes colonneC */
    nbC=nbnoeud;
    matC=(float *)mycalloc(nbC,sizeof(float));
    colC=(int *)mycalloc(nbC,sizeof(int));
    ptC=(int *)mycalloc(nbC,sizeof(int));




/* Appel du sous programme construisant la matrice de préconditionnement */
    precondmat(mat,matC,ptligne,colonne,nbnoeud,ntotmat,ptC,colC,nbC,1,0);

/* Faire une boucle sur le nombre de pas de temps contenant l'appel à GradienConjugué et dedant, la multiplication de nconc2 par
   le volume et sa sauvegarde dans nconc1 */

	/* ecriture des tableaux */
	/* initialisation de nbitermax pour debuging */
	/* fprintf(stderr,"\n nombre iterations gradient conjugue = ");*/
	/* scanf("%d",&nbitermax); */
	for(i=0;i<nbnoeud;i++)
	{
		nconc1[i] = volume[i]*nconc1[i]; 
		nconc2[i] = 0.0 ;
	}
	/* apres cette boulcle nconc1 devient le vecteur des masses au lieu de celui des concentrations */
	nbitermax = 250000; 
	/* fprintf(stderr,"\n nbnoeud=%d ; nmat=%d ; nbitermax=%d ; eps=%f ; nbC=%d \n",nbnoeud,nmat,nbitermax,eps,nbC);
	for(i=0;i<nmat;i++)
	{
		fprintf(stderr,"\n mat(%d)=%f ; colonne(%d)=%d ",i,mat[i],i,colonne[i]); 
	}
	fprintf(stderr,"\n "); 
	for(i=0;i<nbnoeud;i++)
	{
		fprintf(stderr,"\n matC(%d)=%f ; nconc1(%d)=%f ; ptligne(%d)=%d ; ptC(%d)=%d ; colC(%d)=%d",i,matC[i],i,nconc1[i],i,ptligne[i],i,ptC[i],i,colC[i]); 
	}*/

	t2=clock();
	/* fprintf(stderr,"\n temps de creation des tableaux pour le gradient conjugue = %f secondes \n",(float)(t2 - t1) / CLOCKS_PER_SEC);*/

	GradienConjugue(mat,matC,nconc1,nconc2,ptligne,colonne,nbnoeud,nmat,ptC,colC,nbC,nbitermax,eps,1,0);

	t3=clock(); 
	/* fprintf(stderr,"\n temps du gradient conjugue = %f secondes \n",(float)(t3 - t2) / CLOCKS_PER_SEC);*/

    /* mise à jour de la concentration initiale pour le pas de temps suivant */

	/* nconc2 contient les nouvelles valeurs de la concentration */

	/* mise a jour des masses dans le tableau carac */

	for(i = 1;i<nbnoeud+1;i++)
	{
		/* fprintf(stderr,"\n nconc2(%d)=%12.10f ; masse=%12.10f",i-1,nconc2[i-1],nconc2[i-1]*volume[i-1]);*/
		graphe->carac[numcar + 4][i] = nconc2[i-1]*volume[i-1]; 
	
	}
	/* fprintf(stderr,"\n ");

	masset2 = bilanmasses(graphe); 
	if(masset1 > masset2)
	{
		rapport = masset2/masset1; 
	}
	else
	{
		rapport = masset1/masset2; 
	}
	fprintf(stderr,"\n rapport des bilans de masse entre 0 et 1, debut dif. impl fin dif. impl = %lf",rapport); */

	/* liberation de la place allouee dynamiquement pour les tableaux */
    myfreei(vois);
	myfreei(ptligne);
	myfreei(colonne);
	myfreef(mat);
	myfreef(volume);
	myfreef(nconc1);
	myfreef(matC);
	myfreei(colC); 
	myfreei(ptC); 
	myfreef(nconc2);
	/* ecrigraphe(graphe,"graphe3"); */
}


/* fait le produit scalaire des vecteurs x et y de dimension n */
float prodscal (float *x, float *y, int n)
{   float a=0.0;
    int i;
    for (i=0; i<n; i++)
        a+= x[i]*y[i];
    return a;
}

/* Fait la somme des vecteurs x et y de dimension n et met le resultat dans z, z=x-y */
void sommevect(float *x, float *y, float *z, int n)
{   int i;
    for (i=0;i<n;i++)
        z[i]=x[i]+y[i];
}

/* Fait la difference des vectteurs x et y de dimension n et met le resultat dans z, z=x-y */
void diffvect(float *x, float *y, float *z, int n)
{   int i;
    for (i=0;i<n;i++)
        z[i]=x[i]-y[i];
}

/* fait l'operation y=-x, ou x est un vecteur de dimension n */
void opposevect(float *x, float *y, int n)
{   int i;
    for (i=0;i<n;i++)
        y[i]=-x[i];
}

/* multiplie le vecteur x de dimension n par le scalaire scal et met le resultat dans y */
void scalvect(float *x, float scal, float *y, int n)
{   int i;
    for (i=0;i<n;i++)
        y[i]=scal*x[i];
}

/* effectue l'operation z=alpha1*x+alpha2*y, ou x, y et z sont des vecteurs de dimension n, alpha1 et alpha2 des scalaires */
void combinaisonlineairevect(float *x, float *y, float alpha1,float alpha2, float *z, int n)
{   int i;
    for (i=0;i<n;i++)
        z[i]=alpha1*x[i]+alpha2*y[i];
}

void prodmatvect(float *matA,int *ptA,int *colA,float *x,float *y,int nblgA,int nbA,int itype)
{
	int i,j;
	switch(itype)
	{
		case 0: /*matrice diagonale. Seuls les elements diagonaux sont stockés */
			for (i=0;i<nblgA;i++)
				y[i]=matA[i]*x[i];
			break;
		case 1: /* matrice symetrique et creuse. Seuls les elements non nuls de la partie triangulaire inférieure 
			   sont stockés */
			/* contribution des termes diagonaux */
			for (i=0;i<nblgA;i++)
				y[i]=matA[ptA[i]]*x[i];
			/* contributions des termes extra-diagonaux */
			for (i=1;i<nblgA;i++)
				for (j=ptA[i-1]+1;j<ptA[i];j++)
				{
					y[i]+=matA[j]*x[colA[j]];
					y[colA[j]]+=matA[j]*x[i];
				}
			break;
		default: /* cas non encore traité */
			fprintf(stderr,"\n cas non traité dans produit matrice-vecteur \n");
			exit(5);
	}
}

/* resout le systeme Ax=b par la methode du gradient conjugue preconditionne par une matrice C(stoquee dans matC de dimension nbC)
   les elements non nuls de la matrice A sont stockes dans le tableau matA de dimension nbA
   les vecteurs x et b sont de dimension nblgA
   ptA et colA sont les tableaux d'indexes de matA alors que ptC et colC sont les tableaux d'indexes de matC
   iAtype et iCtype sont respectivement les types de matrice creuse de A et de C (diagonale, symetrique et creuse, etc) */
int GradienConjugue(float *matA,float *matC,float *b, float *x, int *ptA, int *colA, int nblgA, int nbA, int *ptC, int *colC, int nbC, int nbitermax,float eps,int iAtype,int iCtype)
{
    int n=nblgA,i;
    float  *g, *h, *Ah, *Cg,ro;

	/* fprintf(stderr,"\n nblgA=%d ; nbA=%d ; nbitermax=%d ; eps=%f ; nbC=%d \n",nblgA,nbA,nbitermax,eps,nbC);
	for(i=;i<10;i++)
	{
		fprintf(stderr,"\n matA(%d)=%f ; colA(%d)=%d ",i,matA[i],i,colA[i]); 
	}
	fprintf(stderr,"\n "); 
	for(i=0;i<10;i++)
	{
		fprintf(stderr,"\n matC(%d)=%f ; b(%d)=%f ; ptA(%d)=%d ; ptC(%d)=%d ; colC(%d)=%d",i,matC[i],i,b[i],i,ptA[i],i,ptC[i],i,colC[i]); 
	}*/

    g=(float *)mycalloc(n,sizeof(float));
    h=(float *)mycalloc(n,sizeof(float));
    Ah=(float *)mycalloc(n,sizeof(float));
    Cg=(float *)mycalloc(n,sizeof(float));
    prodmatvect(matA,ptA,colA,x,g,n,nbA,iAtype); /* g=Ax */
    /* fprintf(stderr,"g[0]=%f  g[1]=%f \n",g[0],g[1]);*/
    combinaisonlineairevect(g,b,1.0,-1.0,g,n);  /* g=g-b (ie g=Ax-b) */
    /* fprintf(stderr,"g[0]=%f  g[1]=%f \n",g[0],g[1]);*/
    /* fprintf(stderr,"\n");*/

    prodmatvect(matC,ptC,colC,g,Cg,n,nbC,iCtype); /* Cg = C*g */
    /* fprintf(stderr,"Cg[0]=%f  Cg[1]=%f \n",Cg[0],Cg[1]);
    fprintf(stderr,"\n");*/

    /* gradient preconditionn� e */
    scalvect(Cg,-1.0,h,n);  /* h = -Cg */
    /* fprintf(stderr,"h[0]=%f  h[1]=%f \n",h[0],h[1]);*/

    float g2 = prodscal(Cg,g,n);  /* g2 = (Cg,g) */

    /* fprintf(stderr," g2=%f  eps*eps=%f \n",g2,eps*eps);
    fprintf(stderr,"**************\n");*/
    if (g2 < eps*eps)
    /* la solution a d�ej`a converg�ee */
    {
        fprintf(stderr,"iter=0 ||g||�2 = %f \n", g2);
        return 1;
    }
    float reps2 = eps*eps*g2;  // epsilon relatif
    int iter;

    for (iter=0;iter<=nbitermax;iter++)
    {   
		
		prodmatvect(matA,ptA,colA,h,Ah,n,nbA,iAtype); 

	/* Ah= A*h */
        /* fprintf(stderr,"Ah[0]=%f Ah[1]=%f \n",Ah[0],Ah[1]);*/

        ro= -prodscal(g,h,n)/prodscal(h,Ah,n); 
		
		/* ro = - (g,h)/(h,Ah)  */

        /* fprintf(stderr," iter=%d ro=%f \n",iter,ro);
        fprintf(stderr,"x[0]=%f x[1]=%f \n",x[0],x[1]);*/
        combinaisonlineairevect(x,h,1.0,ro,x,n); 
		
		 /* x=x+ro*h  */
        /* fprintf(stderr,"x[0]=%f x[1]=%f \n",x[0],x[1]);*/
        combinaisonlineairevect(g,Ah,1.0,ro,g,n); 

		  /* g=g+ro*Ah */
        /* fprintf(stderr,"g[0]=%f g[1]=%f \n",g[0],g[1]);*/
        prodmatvect(matC,ptC,colC,g,Cg,n,nbC,iCtype);  /* Cg=C*g  */
        /* fprintf(stderr,"Cg[0]=%f Cg[1]=%f \n",Cg[0],Cg[1]);*/

        float g2p=g2;
        g2=prodscal(Cg,g,n);  /* g2 = (Cg,g) */

        if (g2 < reps2)
        {
            /* fprintf(stderr," Convergence iter=%d ro = %f ||g||^2 = %f x[0]=%f x[1]=%f\n",iter,ro,g2,x[0],x[1]);*/
            break; // ok, convergence
        }
        float gamma = g2/g2p;

        /* fprintf(stderr,"gamma=%f \n",gamma);*/
        combinaisonlineairevect(h,Cg,gamma,-1.0,h,n);    /* h = gamma*h - Cg; */
        /* fprintf(stderr,"gamma*h-Cg h[0]=%f h[1]=%f \n",h[0],h[1]);*/
    }
    if (iter>=nbitermax) fprintf(stderr,"Non-convergence de la m�ethode du gradient conjugu�e\n");
    myfreef(g);
    myfreef(h);
    myfreef(Ah);
    myfreef(Cg);

	/* fprintf(stderr,"\n fin de GradienConjugue "); 
	fprintf(stderr,"\n nblgA=%d ; nbA=%d ; nbitermax=%d ; eps=%f ; nbC=%d \n",nblgA,nbA,nbitermax,eps,nbC);
	for(i=0;i<nbA;i++)
	{
		fprintf(stderr,"\n matA(%d)=%f ; colA(%d)=%d ",i,matA[i],i,colA[i]); 
	}
	fprintf(stderr,"\n "); 
	for(i=0;i<nblgA;i++)
	{
		fprintf(stderr,"\n matC(%d)=%f ; b(%d)=%f ; ptA(%d)=%d ; ptC(%d)=%d ; colC(%d)=%d ; x(%d)=%f ",i,matC[i],i,b[i],i,ptA[i],i,ptC[i],i,colC[i],i,x[i]); 
	}*/
    return 0;
}

/* remplit la matrice C stockee dans le tableau matC de preconditionnement du systeme qui sera resolu par le gradient conjugue
   matA de taille nbA contient la matrice du systeme sous forme morse, ptA et colA sont les tableaux d'indexes de matA
   ptC et colC sont les tableaux d'indexe de matC dont la dimensin est nbC
   nblgA est le nombre de ligne des matrices A et C, iAtype et iCtype le type des matrices matA et matC
*/
void precondmat(float *matA,float *matC, int *ptA,int *colA,int nblgA,int nbA,int *ptC,int *colC,int nbC,int iAtype,int iCtype)
{   int i,j;
    switch (iCtype)
    {   case 0: /* pr�conditionneur diagonal; la matrice matA est sym�trique, creuse et stockee sous forme morse*/
            for (i=0;i<nbC;i++)
            {
                ptC[i]=i;
                colC[i]=i;
				/*if(matA[ptA[i]] == 0)
				{
					fprintf(stderr,"\n NAN !!");
					exit(5); 
				}*/
                matC[i]=1.0/matA[ptA[i]];
				/* matC[i]=1.0;*/
                /* fprintf(stderr,"precondmat ptc[%d]=%d colC[%d]=%d matC[%d]=%f\n",i,ptC[i],i,colC[i],i,matC[i]);*/
            }
            break;
        case 1:  /* preconditionneur par factorisation incomplete de crout; la matrice matA est sym�trique, creuse et stockee sous forme morse */
            ;
        case 2:  /* pr�conditionneur par factorisation incomplete de cholesky; la matrice matA est SDP, creuse et stockee sous forme morse */
            ;
    }
  }


/* CETTE PROCEDURE FAIT LA MEME CHOSE QUE SIMULSYNCHCOURBAAS AVEC UN SCHEMA implicite pour la diffusion */


graphe_s *simulsynchcourbimp(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel,float eps)
{
	graphe_s *graphe;
	int nbnoeud,i,j,k,n,*tavois,nvois,ech,natt;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,**tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,prop,**deltatbio,*deltatdif,car1[5],**car,propmax;
	FILE *ptficparbio,*ptficmatlab,*ptficexcel,*ptficmasses; 
	float ro,mu,rom,vfom,vsom,vdom,kab,somdelta,somdeltabio,somdeltadif; 
	double massetot1,massetot2,rapport,massetr,massetotdebut,massetotfin,massetest1,massetest2,massetest3; 
	float temps,tempstot,totacarb,temp; 
	clock_t t1,t2,t3,t4,t5,t6,t7; 
	int n1,n2,fracjour,nbels,l; 

	t3 = clock(); 

	graphe = lecgraphe(ficgraphe);

	nbnoeud = graphe->nbnoeuds; 

	ptficparbio = fopen(ficparbio,"r");

	ptficmatlab = fopen(ficmatlab,"w");

	ptficexcel = fopen(ficexcel,"w");

	ptficmasses = fopen(ficmasses,"w");

	fscanf(ptficparbio,"%f %f %f %f %f %f %f",&ro,&mu,&rom,&vfom,&vsom,&vdom,&kab);

	fprintf(stderr,"\n ro=%f ; mu=%f ; rom=%f ; vfom=%f ; vsom=%f ; vdom=%f ; kab=%f ; numero attribut = %d",ro,mu,rom,vfom,vsom,vdom,kab,pourcneg);
	fprintf(stderr,"\n nombre de noeuds=%d",nbnoeud); 


	petitdeltat = deltat/fraction ;

	ech = neari((1.0/24.0)*(1/petitdeltat)); 

	/* ech represente le nombre d'iterations correspondant a une heure de temps */

	fprintf(stderr,"\n deltat=%f jours ",deltat); 

	fprintf(stderr,"\n petitdeltat=%f jours ; ech=%d",petitdeltat,ech); 
	
	/* calcul des masses totales pour bilan des masses */

	massetotdebut = bilanmasses(graphe); 

	massetot1 = bilanmasses(graphe); 

	fprintf(stderr,"\n bilan total des masses au debut de simulsynchcourbimp = %lf",massetotdebut); 

	fprintf(stderr,"\n");
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

		/* la diffusion a ete simulee, on simule maintenant les transformations ; 
		toutes les valeurs des attributs sont positives */ 
		massetest1 = bilanmasses(graphe); 
		deltatbio = miseajourbioas(graphe,petitdeltat,ro,mu,rom,vfom,vsom,vdom,kab);

		for(i = 1 ; i <= nbnoeud ; i++)
		{
			/* mise a jour des attributs avec le tableau des deltas de la biologie */
			for(j=1;j<=5;j++)
			{
				graphe->carac[4+j][i] += deltatbio[j][i];
			}
		}
		freetf(deltatbio,5,nbnoeud); 

		massetest2 = bilanmasses(graphe); 

		/* POUR DEBUGGING */
		/* ecrigraphe(graphe,"graphe1"); */
		/* fprintf(stderr,"\n pourcneg=%d ; coeffdom=%f ; petitdeltat=%f ; nvmax=%d",pourcneg,coeffdom,petitdeltat,nvmax); */
		
		/* on realise la diffusion avec le schema implicite */
		t6=clock(); 
		miseajourdifimplic(graphe,pourcneg,coeffdom,petitdeltat,nvmax,eps);
		t7=clock();
		temp = (float)(t7 - t6) / CLOCKS_PER_SEC;
		/* fprintf(stderr,"\n temps de calcul= %f secondes pour iteration %d",temp,n); */
		


		massetest3 = bilanmasses(graphe); 

		/* fprintf(stderr,"\n massetest1 = %lf ; massetest2 = %lf ; massetest3 = %lf",massetest1,massetest2,massetest3); */
		/* ecrigraphe(graphe,"graphe4"); */
 
		/* la diffusion puis les transformations ont ete simulees de maniere asynchrone */

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
			massetot1 = bilanmasses(graphe); 
			fprintf(stderr,"\n temps de la simulation en jours =%f ",(n+1)*petitdeltat); 
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
			fprintf(stderr,"\n bilan total des masses = %lf",massetot1); 
			t5 = clock(); 
			temps = (float)(t5 - t3) / CLOCKS_PER_SEC;
			temps = temps/60.0;
			fprintf(stderr,"\n temps de calcul intermediaire pour la simulation synchrone implicite = %f' ",temps); 
		}
	}
	/* a la fin de la boucle le tableau car[i][j] contient pour le temps numero i (en heures) la somme des attributs adresses par j */
	totacarb = massetot1; 
	fracjour = 24; 
	nbels = n1; 
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

	fprintf(ptficmatlab,"plot(X,Y0,'b',X,Y1,'g',X,Y2,'c',X,Y3,'c',X,Y4,'r');");
	fprintf(ptficmatlab,"\n");

	t4= clock(); 
	temps = (float)(t4 - t3) / CLOCKS_PER_SEC;
	temps = temps/60.0;

	fprintf(stderr,"\n temps total pour la simulation synchrone implicite = %f minutes",temps); 

	massetotfin = bilanmasses(graphe); 


	if(massetotfin > massetotdebut)
	{
		rapport = massetotdebut/massetotfin; 
	}
	else
	{
		rapport = massetotfin/massetotdebut; 
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

graphe_s *simulsynchcourbopt(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel,float eps)
{
	graphe_s *graphe;
	int nbnoeud,i,j,k,n,*tavois,nvois,ech,natt;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,**tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,prop,**deltatbio,*deltatdif,car1[5],**car,propmax,rboul1;
	FILE *ptficparbio,*ptficmatlab,*ptficexcel,*ptficmasses; 
	float ro,mu,rom,vfom,vsom,vdom,kab,somdelta,somdeltabio,somdeltadif,diag,rs; 
	double massetot1,massetot2,rapport,massetr,massetotdebut,massetotfin,massetest1,massetest2,massetest3; 
	float temps,tempstot,totacarb,temp,*volume,*mat,theta,*matC; 
	clock_t t1,t2,t3,t4,t5,t6,t7; 
	int n1,n2,fracjour,nbels,l,*ptligne,*colonne,*vois,nmat,ntotmat,narcs,*ptC,*colC,nbC; 

	petitdeltat = deltat/fraction ;
	t3 = clock(); 

	graphe = lecgraphe(ficgraphe);

	nbnoeud = graphe->nbnoeuds;
	narcs = graphe->nco;
	ntotmat = nbnoeud + narcs; 

	/* initialisation des matrices A et de la matrice de preconditionnement */
	volume = (float*)mycalloc(nbnoeud,sizeof(float));
	vois = (int*)mycalloc(nvmax,sizeof(int));
	mat = (float*)mycalloc(ntotmat,sizeof(float));
	ptligne = (int*)mycalloc(nbnoeud,sizeof(int));
	colonne = (int*)mycalloc(ntotmat,sizeof(int));

	nmat = 0;
	for(i = 0;i<nbnoeud;i++)
	{
		ptligne[i] = 0;
	}
	for(i = 0;i<ntotmat;i++)
	{
		mat[i] = 0;
		colonne[i] = 0; 
	}
	for(i = 0;i<nbnoeud;i++)
	{
		diag = 0.0;
		x1 = graphe->carac[1][i+1];
		y1 = graphe->carac[2][i+1];
		z1 = graphe->carac[3][i+1];
		rboul1 = graphe->carac[4][i+1]; 
		voisins(i+1,graphe,nvmax,&nvois,&vois);
		if(nvois>1) triinsertion(vois,nvois);
		for(j=0;(j<nvois)&&((vois[j]-1)<i);j++)
		{
			n = vois[j];
			x2 = graphe->carac[1][n];
			y2 = graphe->carac[2][n];
			z2 = graphe->carac[3][n];
			rboul2 = graphe->carac[4][n];
			distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
			rs = (2.0*rboul1*rboul2)/(rboul1+rboul2);
			scont = 4*3.1416*rs*rs;
			theta = -(coeffdom*scont*petitdeltat)/distance;
			diag += -theta;
            colonne[nmat] = n-1;
            mat[nmat] = theta;
            mat[ptligne[n-1]] += -theta;
            nmat++;
		}
		volume[i] = (4.0/3.0)*3.1416*rboul1*rboul1*rboul1;
		ptligne[i] = nmat;
		colonne[nmat] = i;
		mat[nmat] = diag;
		nmat++;
	}
	
	for(i=0;i<nbnoeud;i++)
	{
		mat[ptligne[i]] += volume[i]; 
	}
	
	fprintf(stderr,"\n fin de la creation des tableaux mat,colonne,ptligne,volume");
	/* les tbleaux colonne,mat,ptligne et volume sont remplis */
    nbC=nbnoeud;
    matC=(float*)mycalloc(nbC,sizeof(float));
    colC=(int*)mycalloc(nbC,sizeof(int));
    ptC=(int*)mycalloc(nbC,sizeof(int));

    precondmat(mat,matC,ptligne,colonne,nbnoeud,ntotmat,ptC,colC,nbC,1,0);

	fprintf(stderr,"\n fin de precondmat"); 

	/* les tableaux matC,ptC,colC,nbC de la matrice de preconditionnement sont remplis */

	ptficparbio = fopen(ficparbio,"r");

	ptficmatlab = fopen(ficmatlab,"w");

	ptficexcel = fopen(ficexcel,"w");

	ptficmasses = fopen(ficmasses,"w");

	fscanf(ptficparbio,"%f %f %f %f %f %f %f",&ro,&mu,&rom,&vfom,&vsom,&vdom,&kab);

	fprintf(stderr,"\n ro=%f ; mu=%f ; rom=%f ; vfom=%f ; vsom=%f ; vdom=%f ; kab=%f ; numero attribut = %d",ro,mu,rom,vfom,vsom,vdom,kab,pourcneg);
	fprintf(stderr,"\n nombre de noeuds=%d",nbnoeud); 

	ech = neari((1.0/24.0)*(1/petitdeltat)); 

	/* ech represente le nombre d'iterations correspondant a une heure de temps */

	fprintf(stderr,"\n deltat=%f jours ",deltat); 

	fprintf(stderr,"\n petitdeltat=%f jours ; ech=%d",petitdeltat,ech); 
	
	/* calcul des masses totales pour bilan des masses */

	massetotdebut = bilanmasses(graphe); 

	massetot1 = bilanmasses(graphe); 

	fprintf(stderr,"\n bilan total des masses au debut de simulsynchcourbimp = %lf",massetotdebut); 

	fprintf(stderr,"\n");
	t1 = clock(); 
	tempstot = 0; 

	natt = 5; 
	nbels = fraction/ech + 1; 
	car = allot2f(nbels + 1,natt);

	n1 =0; 
	/* fprintf(stderr,"\n"); */
	for(n = 0 ; n < fraction ; n++)
	{
		/* fprintf(stderr,"n=%d ",n); */
		/* if(n % 1000 == 0)
		{
			t2 = clock(); 
			temps = (float)(t2 - t1) / CLOCKS_PER_SEC;
			temps = temps/60.0; 
			tempstot += temps; 
			fprintf(stderr,"\n temps de calcul pour 1000 iterations = %f minutes",tempstot);
			t1= clock(); 
		}*/

		/* la diffusion a ete simulee, on simule maintenant les transformations ; 
		toutes les valeurs des attributs sont positives */ 
		deltatbio = miseajourbioas(graphe,petitdeltat,ro,mu,rom,vfom,vsom,vdom,kab);

		for(i = 1 ; i <= nbnoeud ; i++)
		{
			/* mise a jour des attributs avec le tableau des deltas de la biologie */
			for(j=1;j<=5;j++)
			{
				graphe->carac[4+j][i] += deltatbio[j][i];
			}
		}
		freetf(deltatbio,5,nbnoeud); 

		massetest2 = bilanmasses(graphe); 

		/* POUR DEBUGGING */
		/* ecrigraphe(graphe,"graphe1"); */
		/* fprintf(stderr,"\n pourcneg=%d ; coeffdom=%f ; petitdeltat=%f ; nvmax=%d",pourcneg,coeffdom,petitdeltat,nvmax); */
		
		/* on realise la diffusion avec le schema implicite */
		/* t6=clock(); */
		/* fprintf(stderr,"\n entree dans miseajourdifopt; pourcneg=%d ; coeffdom=%f ; petitdeltat=%f ; nvmax=%d ; eps=%f ; nmat=%d ;nbnoeud=%d ; nbC=%d ",pourcneg,coeffdom,petitdeltat,nvmax,eps,nmat,nbnoeud,nbC);*/ 
		miseajourdifopt(graphe,pourcneg,coeffdom,petitdeltat,nvmax,eps,mat,ptligne,colonne,nmat,nbnoeud,matC,ptC,colC,nbC,volume);
		/* fprintf(stderr,"\n sortie de miseajourdifopt"); */
		/* t7=clock();
		temp = (float)(t7 - t6) / CLOCKS_PER_SEC;*/
		/* fprintf(stderr,"\n temps de calcul= %f secondes pour iteration %d",temp,n); */

		/* fprintf(stderr,"\n massetest1 = %lf ; massetest2 = %lf ; massetest3 = %lf",massetest1,massetest2,massetest3); */
		/* ecrigraphe(graphe,"graphe4"); */
 
		/* la diffusion puis les transformations ont ete simulees de maniere asynchrone */

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
			massetot1 = bilanmasses(graphe); 
			fprintf(stderr,"\n temps de la simulation en jours =%f ",(n+1)*petitdeltat); 
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
			fprintf(stderr,"\n bilan total des masses = %lf",massetot1); 
			t5 = clock(); 
			temps = (float)(t5 - t3) / CLOCKS_PER_SEC;
			temps = temps/60.0;
			fprintf(stderr,"\n temps de calcul intermediaire pour la simulation synchrone implicite = %f' ",temps); 
		}
	}
	/* a la fin de la boucle le tableau car[i][j] contient pour le temps numero i (en heures) la somme des attributs adresses par j */
	totacarb = massetot1; 
	fracjour = 24; 
	nbels = n1; 
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

	fprintf(ptficmatlab,"plot(X,Y0,'b',X,Y1,'g',X,Y2,'c',X,Y3,'c',X,Y4,'r');");
	fprintf(ptficmatlab,"\n");

	t4= clock(); 
	temps = (float)(t4 - t3) / CLOCKS_PER_SEC;
	temps = temps/60.0;

	fprintf(stderr,"\n temps total pour la simulation synchrone implicite = %f minutes",temps); 

	massetotfin = bilanmasses(graphe); 


	if(massetotfin > massetotdebut)
	{
		rapport = massetotdebut/massetotfin; 
	}
	else
	{
		rapport = massetotfin/massetotdebut; 
	}
	fprintf(stderr,"\n rapport des bilans de masse entre 0 et 1 = %lf",rapport); 
	/* ecriture du nouveau graphe value apres diffusion, seules les valeurs des attributs ont pu changer */
	
	ecrigraphe(graphe,ficgrapher);

	fclose(ptficparbio); 
	fclose(ptficmatlab);
	fclose(ptficexcel);
	fclose(ptficmasses);

	myfreei(vois);
	myfreei(ptligne);
	myfreei(colonne);
	myfreef(mat);
	myfreef(volume);
	myfreef(matC);
	myfreei(colC); 
	myfreei(ptC); 
	
	return(graphe); 
}


void miseajourdifopt(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax,float eps,float *mat,int *ptligne,int *colonne,int nmat,int nbnoeud,float *matC,int *ptC, int *colC, int nbC,float *volume)
{
	int i,j,k,n,narcs,ntotmat,nbitermax;
	float *nconc1,*nconc2;
	
	nconc2 = (float*)mycalloc(nbnoeud,sizeof(float));
	nconc1 = (float*)mycalloc(nbnoeud,sizeof(float));
	ntotmat = narcs + nbnoeud;

	for(i=0;i<nbnoeud;i++)
	{
		nconc1[i] = graphe->carac[numcar + 4][i+1]; 
		nconc2[i] = 0.0 ;
	}
	
	nbitermax = 250000; 

	GradienConjugue(mat,matC,nconc1,nconc2,ptligne,colonne,nbnoeud,nmat,ptC,colC,nbC,nbitermax,eps,1,0);

	for(i = 1;i<nbnoeud+1;i++)
	{
		graphe->carac[numcar + 4][i] = nconc2[i-1]*volume[i-1]; 
	}

	myfreef(nconc2);
	myfreef(nconc1);
}

/* generation des courbes de Valérie en format Matlab */
void courbesvalerie()
{
	char ficvalerie[50],ficmatlab[50]; 
	FILE *ptficvalerie,*ptficmatlab; 
	float car[121][5],x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,totacarb; 
	int nbels,i,j;

	fprintf(stderr,"\n fichier courbes de Valerie moins la premiere ligne: ");
	scanf("%s",ficvalerie); 
	fprintf(stderr,"\n fichier sortie courbes de Valerie matlab: ");
	scanf("%s",ficmatlab); 
	ptficvalerie = fopen(ficvalerie,"r"); 
	ptficmatlab = fopen(ficmatlab,"w");
	nbels = 120; 

	fscanf(ptficvalerie,"%f %f %f %f %f %f %f %f %f %f %f",&x1,&x2,&x3,&x4,&x5,&x6,&x7,&x8,&x9,&x10,&x11);
	totacarb = x2 + x10; 

	fprintf(stderr,"\n masse totale = %f",totacarb); 
	fclose(ptficvalerie);
	ptficvalerie = fopen(ficvalerie,"r"); 

	for(i=0;i<121;i++)
	{
		for(j=0;j<5;j++)
		{
			car[i][j] = 0; 
		}
	}


	for(i=0;i<nbels;i++)
	{
		fscanf(ptficvalerie,"%f %f %f %f %f %f %f %f %f %f %f",&x1,&x2,&x3,&x4,&x5,&x6,&x7,&x8,&x9,&x10,&x11);
		car[i][0] = (x2/totacarb)*100;
		car[i][1] = (x10/totacarb)*100;
		car[i][2] = (x8/totacarb)*100;
		car[i][4] = (x9/totacarb)*100;
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

	fprintf(stderr,"\n $");

	fprintf(ptficmatlab,"Y3 = [");
	for(i = 0;i<nbels-1;i++)fprintf(ptficmatlab,"%f ",car[i][3]);
	fprintf(ptficmatlab,"%f];",car[nbels-1][3]);

	fprintf(ptficmatlab,"\n");

	fprintf(stderr,"\n $$");

	fprintf(ptficmatlab,"Y4 = [");
	for(i = 0;i<nbels-1;i++)fprintf(ptficmatlab,"%f ",car[i][4]);
	fprintf(ptficmatlab,"%f];",car[nbels-1][4]);

	fprintf(ptficmatlab,"\n");

	fprintf(stderr,"\n $$$");
	/* on a ecrit Y0,Y1,Y3,Y5 */

	/* on appelle la fonction plot */

	fprintf(ptficmatlab,"plot(X,Y0,'b-.',X,Y1,'g-.',X,Y2,'c-.',X,Y3,'c-.',X,Y4,'r-.');");
	fprintf(ptficmatlab,"\n");

	fclose(ptficmatlab);
	fclose(ptficvalerie); 

}

/* cette procedure cree un fichier de donnees a envoyer a creeratt pour le calibrage du coefficient de diffusion avec la methode LBM 
massecarb est la masse totale de carbone mise sur les deux premiers plans de maniere homogene par rapport aux concentrations 
La sortie est le tableau des attributs complementaires qui contient: masses de micro-organismes (0 dans ce cas) , masses de DOM
(masse totale massedom), masses de
SOM (0 dans ce cas), masses de FOM (0 dans ce cas), masses de CO2 (0 dans ce cas) 
On repartit en homogeneisant les concentrations la masse totale massedom de DOM dans toutes les boules touchant les nbplans
premiers plans (les plans sont indexes a partir de 1 */

void creerattdiff(char *ficboules,char *ficatt,float massedom,int nbplans)
{

    double sommevoldom,massetot;
    int nbdom,nboules,i,j; 
    double **trav;
	float x,y,z,r; 
    FILE *ptficboules,*ptficatt; 

    ptficboules = fopen(ficboules,"r");
	ptficatt = fopen(ficatt,"w");

    fscanf(ptficboules,"%d",&nboules);

    fprintf(stderr,"\n nombre de boules = %d ; nb de plans = %d ",nboules,nbplans); 

    /* creation du tableau trav qui contiendra le volume de la boule, sa masse de DOM */
	/* les boules seront indexes a partir de 1 ainsi que les attributs */
    trav = allotab2d(2,nboules+1); 

    /* initialisation du tableau trav a zero */
    for(i=0;i<2;i++)
    {
        for(j=0;j<=nboules;j++)
        {
            trav[i][j] = 0; 
        }
    }

    fprintf(stderr,"\n fin initialisation du tableau trav"); 

	nbdom = 0;
	sommevoldom = 0; 

    /* ecriture du volume des boules dans trav(0,i) */
    for(i=1;i<=nboules;i++)
	{
		fscanf(ptficboules,"%f %f %f %f",&x,&y,&z,&r);
		/* on regarde si la boule est dans les nbplans premiers plans, Valerie met nb a 2 */
		if((z-r) <= (nbplans-1))
		{
			trav[0][i] = (4.0/3.0)*3.1416*(double)r*(double)r*(double)r; 
			nbdom++;
			sommevoldom += trav[0][i]; 
		}

	}

	fprintf(stderr,"\n nombre de boules des premiers plans donc avec de la DOM = %d ; somme du volume des boules = %f",nbdom,sommevoldom);

	for(i=1;i<=nboules;i++)
	{
		if(trav[0][i] != 0)
		{
			trav[1][i] = (trav[0][i]/sommevoldom)*massedom; 
		}
	}

    fprintf(stderr,"\n fin ecriture du volume des boules et de la masse de DOM dans le tableau trav");

	 /* remplissage de ficatt a partir de trav */

    fprintf(stderr,"\n remplissage de ficatt \n");

    fprintf(ptficatt,"%d \n",5);
	massetot = 0; 
    for(i = 1;i <= nboules;i++)
    {
        fprintf(ptficatt,"%f %f %f %f %f \n",0.0,(float)trav[1][i],0.0,0.0,0.0); 
		if(trav[1][i] != 0)
		{
			fprintf(stderr,"%d %lf %lf & ",i,trav[0][i],trav[1][i]); 
			massetot += trav[1][i];
		}
    }
	fprintf(stderr,"\n masse totale dans les boules = %lf",massetot); 
   
    fclose(ptficatt);
    fclose(ptficboules); 
}

void testcreerattdiff()
{
    char ficboules[50],ficatt[50];
    float massedom; 
	int nbplans; 

    fprintf(stderr," fichier de boules (entree), masse totale DOM (entree), nombre de plans (entree),fichier d'attributs (sortie) :  ");
    scanf("%s %f %d %s",ficboules,&massedom,&nbplans,ficatt); 
    fprintf(stderr,"\n fichier de boules: %s ; fichier d'attributs: %s ; nombre de plans = %d ; masse DOM = %f",ficboules,ficatt,nbplans,massedom);

    creerattdiff(ficboules,ficatt,massedom,nbplans);
}

/* Meme chose que simulsynchcourbopt mais sans simuler la biologie, on diffuse simplement la DOM 
On obtient en sortie le nouveau graphe apres diffusion et la courbe avec en abscisse le numero des plans et en ordonnee la
masse de DOM dans le plan, on donne cette courbe sous format matlab */
graphe_s *simulsynchcourboptdiff(char *ficgraphe,float coeffdom,float deltat,int fraction,int nvmax,char *ficmatlab,float eps,int dimz)
{
	graphe_s *graphe;
	int nbnoeud,i,j,k,n,*tavois,nvois,ech,natt,nbvox;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,**tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,prop,**deltatbio,*deltatdif,car1[5],**car,propmax,rboul1,masseval,*domplanval;
	FILE *ptficmatlab,*pttrav; 
	float ro,mu,rom,vfom,vsom,vdom,kab,somdelta,somdeltabio,somdeltadif,diag,rs,rcarre,massetotval; 
	double massetot1,massetot2,rapport,massetr,massetotdebut,massetotfin,massetest1,massetest2,massetest3; 
	float temps,tempstot,totacarb,temp,*volume,*mat,theta,*matC,dom,*domplan,massevox,intercor,xx,xcar,ycar; 
	clock_t t1,t2,t3,t4,t5,t6,t7; 
	int n1,n2,fracjour,nbels,l,*ptligne,*colonne,*vois,nmat,ntotmat,narcs,*ptC,*colC,nbC; 

	petitdeltat = deltat/fraction ;
	t3 = clock(); 

	graphe = lecgraphe(ficgraphe);

	nbnoeud = graphe->nbnoeuds;
	narcs = graphe->nco;
	ntotmat = nbnoeud + narcs; 

	/* initialisation des matrices A et de la matrice de preconditionnement */
	volume = (float*)mycalloc(nbnoeud,sizeof(float));
	vois = (int*)mycalloc(nvmax,sizeof(int));
	mat = (float*)mycalloc(ntotmat,sizeof(float));
	ptligne = (int*)mycalloc(nbnoeud,sizeof(int));
	colonne = (int*)mycalloc(ntotmat,sizeof(int));
	domplan = (float*)mycalloc(dimz+1,sizeof(float));
	domplanval = (float*)mycalloc(dimz+1,sizeof(float));

	nmat = 0;
	for(i = 0;i<nbnoeud;i++)
	{
		ptligne[i] = 0;
	}
	for(i = 0;i<ntotmat;i++)
	{
		mat[i] = 0;
		colonne[i] = 0; 
	}
	for(i = 0;i<nbnoeud;i++)
	{
		diag = 0.0;
		x1 = graphe->carac[1][i+1];
		y1 = graphe->carac[2][i+1];
		z1 = graphe->carac[3][i+1];
		rboul1 = graphe->carac[4][i+1]; 
		voisins(i+1,graphe,nvmax,&nvois,&vois);
		if(nvois>1) triinsertion(vois,nvois);
		for(j=0;(j<nvois)&&((vois[j]-1)<i);j++)
		{
			n = vois[j];
			x2 = graphe->carac[1][n];
			y2 = graphe->carac[2][n];
			z2 = graphe->carac[3][n];
			rboul2 = graphe->carac[4][n];
			distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
			rs = (2.0*rboul1*rboul2)/(rboul1+rboul2);
			scont = 4*3.1416*rs*rs;
			theta = -(coeffdom*scont*petitdeltat)/distance;
			diag += -theta;
            colonne[nmat] = n-1;
            mat[nmat] = theta;
            mat[ptligne[n-1]] += -theta;
            nmat++;
		}
		volume[i] = (4.0/3.0)*3.1416*rboul1*rboul1*rboul1;
		ptligne[i] = nmat;
		colonne[nmat] = i;
		mat[nmat] = diag;
		nmat++;
	}
	
	for(i=0;i<nbnoeud;i++)
	{
		mat[ptligne[i]] += volume[i]; 
	}
	
	fprintf(stderr,"\n fin de la creation des tableaux mat,colonne,ptligne,volume");
	/* les tableaux colonne,mat,ptligne et volume sont remplis */
    nbC=nbnoeud;
    matC=(float*)mycalloc(nbC,sizeof(float));
    colC=(int*)mycalloc(nbC,sizeof(int));
    ptC=(int*)mycalloc(nbC,sizeof(int));

    precondmat(mat,matC,ptligne,colonne,nbnoeud,ntotmat,ptC,colC,nbC,1,0);

	fprintf(stderr,"\n fin de precondmat"); 

	/* les tableaux matC,ptC,colC,nbC de la matrice de preconditionnement sont remplis */

	ptficmatlab = fopen(ficmatlab,"w");

	fprintf(stderr,"\n nombre de noeuds=%d",nbnoeud); 

	fprintf(stderr,"\n deltat=%f jours ",deltat); 

	fprintf(stderr,"\n petitdeltat=%f jours ",petitdeltat); 
	
	/* calcul des masses totales pour bilan des masses */

	massetotdebut = bilanmasses(graphe); 

	fprintf(stderr,"\n bilan total des masses au debut de simulsynchcourbimp = %lf",massetotdebut); 

	fprintf(stderr,"\n");
	
	for(n = 0 ; n < fraction ; n++)
	{

		miseajourdifopt(graphe,2,coeffdom,petitdeltat,nvmax,eps,mat,ptligne,colonne,nmat,nbnoeud,matC,ptC,colC,nbC,volume);
	}

	t4= clock(); 
	temps = (float)(t4 - t3) / CLOCKS_PER_SEC;
	temps = temps/60.0;

	fprintf(stderr,"\n temps total pour la simulation synchrone implicite = %f minutes",temps); 

	massetotfin = bilanmasses(graphe); 


	if(massetotfin > massetotdebut)
	{
		rapport = massetotdebut/massetotfin; 
	}
	else
	{
		rapport = massetotfin/massetotdebut; 
	}
	fprintf(stderr,"\n rapport des bilans de masse entre 0 et 1 = %lf",rapport); 
	/* ecriture du nouveau graphe value apres diffusion, seules les valeurs des attributs ont pu changer */
	
	/* initialisation domplan */
	for(i = 0;i <= dimz;i++)
	{
		domplan[i] = 0; 
	}
	/* ecriture de la courbe des masses totales sur les plans z=cste */
	fprintf(stderr,"\n nombre de boules = %d dimz = %d ",nbnoeud,dimz);
	massetot = 0;
	fprintf(stderr,"\n"); 
	for(i = 0;i<nbnoeud;i++)
	{
		if(i % 10000 == 0)fprintf(stderr,"i=%d ",i); 
		x1 = graphe->carac[1][i+1];
		y1 = graphe->carac[2][i+1];
		z1 = graphe->carac[3][i+1];
		rboul1 = graphe->carac[4][i+1]; 
		rcarre = rboul1*rboul1; 
		volboul = (4.0/3.0)*3.1416*rboul1*rboul1*rboul1; 
		dom = graphe->carac[6][i+1]; 
		massevox = dom / volboul; 
		massetot += dom;
		/* dom est la masse de DOM dans la boule i+1 */
		/* mise a jour des plans intersectant la boule */
		for(l = floor(x1-rboul1); l <= ceil(x1 + rboul1) ; l++ )
		{
			for(j = floor(y1-rboul1); j <= ceil(y1 + rboul1) ; j++ )
			{
				for(k = floor(z1-rboul1) ; k <= ceil(z1 + rboul1); k++)
				{
					if( ((l-x1)*(l-x1) + (j-y1)*(j-y1) + (k-z1)*(k-z1)) <=  rcarre )
					{
						/* le point est dans la boule */
						if(k <= dimz)
						{
							domplan[k] += massevox; 
						}
					}
				}
				
			}
		}
	}
	fprintf(stderr,"\n fin de boucle remplissage de domplan, massetot = %f \n",massetot);
	for(k=0;k<=dimz;k++)
	{
		domplan[k] = (domplan[k]*100)/massetot; 
		/* fprintf(stderr,"%f ",domplan[k]);*/
	}
	fprintf(stderr,"\n masse totale de DOM = %f",massetot);
	/* generation du fichier matlab avec le tableau domplan */
	fprintf(ptficmatlab,"X = [");
	for(i = 0;i<dimz;i++)fprintf(ptficmatlab,"%f ",(float)(i));
	fprintf(ptficmatlab,"%f];",(float)(dimz));

	/* on ecrit Y0 */

	/* on ecrit la valeur de Y0, mettre tout en pourcentage */
	fprintf(ptficmatlab,"\n Y0 = [");
	for(i = 0;i<dimz;i++)fprintf(ptficmatlab,"%f ",domplan[i]);
	fprintf(ptficmatlab,"%f];",domplan[dimz]);

	/* on calcule domplanval pour les images de valerie */
	pttrav = fopen("table_DOC_profileZ_BD12_Sw100_15000.txt","r");
	massetotval = 0; 
	for(k=0;k<300;k++)
	{
		fscanf(pttrav,"%f %d",&masseval,&nbvox);
		domplanval[k] = masseval;
		massetotval += masseval; 
	}
	for(k=0;k<300;k++)
	{
		domplanval[k] = (domplanval[k]*100)/massetotval;
	}

	/* on calcule l'intercorrelation normalisee entre les deux courbes */
	xx = 0;
	for(k=0;k<300;k++)
	{
		xx += domplanval[k]*domplan[k];
	}
	xcar = 0; 
	for(k=0;k<300;k++)
	{
		xcar += domplan[k]*domplan[k];
	}
	xcar = sqrt(xcar); 
	ycar = 0; 
	for(k=0;k<300;k++)
	{
		ycar += domplanval[k]*domplanval[k];
	}
	ycar = sqrt(ycar);
	intercor = xx/(xcar*ycar);
	fprintf(stderr,"\n intercorrelation normalisee = %f",intercor); 

	/* on ecrit la courbe de Valerie */
	fprintf(ptficmatlab,"\n Y1 = [");
	for(i = 0;i<dimz;i++)fprintf(ptficmatlab,"%f ",domplanval[i]);
	fprintf(ptficmatlab,"%f];",domplanval[dimz]);
	fprintf(ptficmatlab,"\n");
	
	fprintf(ptficmatlab,"plot(X,Y0,'b',X,Y1,'r');");
	fprintf(ptficmatlab,"\n");
	fclose(ptficmatlab);
	fclose(pttrav); 

	myfreei(vois);
	myfreei(ptligne);
	myfreei(colonne);
	myfreef(mat);
	myfreef(volume);
	myfreef(matC);
	myfreei(colC); 
	myfreei(ptC); 
	
	return(graphe); 
}

void testcalib()
{
	graphe_s *graphe; 
	float coeffdom, deltat, eps;
	int fraction,nvmax,dimz;
	char ficgraphe[50], ficmatlab[50];

	fprintf(stderr,"\n fichier graphe en entree ; fichier matlab en sortie \n");
	scanf("%s %s",ficgraphe,ficmatlab); 

	fprintf(stderr,"\n coeff diffusion (104665 en voxels carre par jour) ; deltat (en jours) ; fraction ; nb max voisins ; valeur de epsilon pour gradient ; dimz \n");
	scanf("%f %f %d %d %f %d",&coeffdom,&deltat,&fraction,&nvmax,&eps,&dimz);

	fprintf(stderr,"\n eps = %f",eps); 
	graphe = simulsynchcourboptdiff(ficgraphe,coeffdom,deltat,fraction,nvmax,ficmatlab,eps,dimz); 
}

/* algorithme de Dijkstra 
cette procedure prend en entree un graphe valué, un numéro de noeud 
elle rend en sortie un tableau qui associe à chaque noeud la longueur du plus court chemin entre le noeud et le noeud initial numnoeud 
la fonction de cout est la fonction cout qui à un couple de noeuds associe le cout */
float *dijkstra(graphe_s *graphe,int numnoeud,int nvmax,int opt)
{
	float *shortest,mindis,vtrav;
	float infini;
	int nbnoeud,minsg,nbsousgraphe,i,nvois,*vois,nv,ico;
	bool *sousgraphe; 
	float *val,valout;
	int *itas,n,iout,nmaxtas,nsousg;

	nbnoeud = graphe->nbnoeuds;
	shortest =  (float*)mycalloc(nbnoeud+1,sizeof(float));
	sousgraphe =  (bool*)mycalloc(nbnoeud+1,sizeof(float));
	vois = (int*)mycalloc(nvmax,sizeof(int));
	infini = 10e37; 
	nmaxtas = nbnoeud; 
	/* le tas est indexe a partir de 1 */
	itas = (int*)mycalloc(nmaxtas+1,sizeof(int));
	val = (float*)mycalloc(nmaxtas+1,sizeof(double));

	/* le tableau sousgraphe met a 1 les noeuds du sous graphe et a 0 les autres, le nombre des elements du sousgraphe est nbsousgraphe */
	/* le tableau shortest indexe a partir de 1 contiendra la longueur du plus court chemin de numnoeud au noeud shortest(i) */
	/* minsg contiendra le numero du noeud du sous graphe de plus petite distance (geodesique) avec numnoeud */
	/* on notera -1 une distance infini */
	/* l'algorithme s'arrete lorsque le sous graphe ne peut plus etre etendu */

	/* initialisation de shortest a l'infini */
	for(i=0;i<=nbnoeud;i++)
	{
		shortest[i] = infini;
		sousgraphe[i] = 0;
	}
	
	shortest[numnoeud] = 0;

	/* creation d'un tas contenant les noeuds du complementaire du sous graphe avec le cout associe,
	le cout associe n'est pas infini */
	/* au depart le tas contient le noeud initial */
	itas[1] = numnoeud ; 
	val[1] = 0 ;
	n = 1; 
	nsousg = 0; 
	while(n != 0)
	{
		/* on retire du tas le noeud du complementaire du sous graphe de plus petite distance au sous graphe */
		outas_s(val,itas,&n,&ico,&valout) ;
		/* on traite le point que si la valeur de son cout est bien la derniere valeur ecrite dans shortest et si le noeud est dans le complementaire du sous graphe */
		if((valout == shortest[ico]) && (sousgraphe[ico] == 0))
		{
			/* on ajoute le noeud ico au sous graphe */
			sousgraphe[ico] = 1; 
			nsousg += 1;
			/* on met a jour les voisins de ico qui ne sont pas deja dans le sous graphe */
			voisins(ico,graphe,nvmax,&nvois,&vois);
			for(i=0;i<nvois;i++)
			{
				nv = vois[i];
				/* on met a jour le voisin si il n'est pas deja dans le sous graphe */
				if(sousgraphe[nv] == 0)
				{
					/* si le chemin passant par ico est plus court que celui deja calcule, on met a jour shortest[nv] */
					vtrav = shortest[ico] + cout(nv,ico,graphe,opt); 

					/* normalement ce test peut etre enlevé */
					if(vtrav >= infini)
					{
						fprintf(stderr,"\n vtrav >= infini !!!!");
						exit(2); 
					}

					if(shortest[nv] > vtrav)
					{
						shortest[nv] = vtrav;
						/* on insere dans le tas nv avac la nouvelle valeur */ 
						if((n+1) <= nmaxtas)
						{
							intas_s(val,itas,&n,nv,vtrav); 	
						}else
						{
							fprintf(stderr,"\n debordement du tas, n = %d",n);
							exit(1); 
						}
					
					}
				}
			}
			/* fin de la mise a jour des voisins et de leur insertion dans le tas avec leur eventuelle nouvelle valeur */
		}
	}
	/* le tableau shortest contient la plus courte distance pour les noeuds de la composanta connexe
	du noeud initial et l'infini pour les autres */

	/* fprintf(stderr,"\n nombre d'elements du sous graphe = %d",nsousg);*/
	/* liberer les tableaux et retourner le tableau shortest */
	myfreei(itas);
	myfreef(val);
	myfreeb(sousgraphe);
	myfreei(vois);
	
	return(shortest); 
}

/* cette fonction associe un cout à l'arc (n1,n2) du graphe pointé par graphe 
on pourrait la mettre en parametre de la fonction dijkstra ce qui n'a pas ete fait par paresse */
float cout(int n1, int n2, graphe_s *graphe,int opt)
{
	float cost,x1,y1,z1,r1,x2,y2,z2,r2;

	x1 = graphe->carac[1][n1];
	y1 = graphe->carac[2][n1];
	z1 = graphe->carac[3][n1];
	x2 = graphe->carac[1][n2];
	y2 = graphe->carac[2][n2];
	z2 = graphe->carac[3][n2];
	r1 = graphe->carac[4][n1];
	r2 = graphe->carac[4][n2];

	/* if(opt == 1)
	{*/
	cost = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
	 
	/* }
	else
	if(opt == 2)
	{
		cost = r1 + r2; 
	}*/
	return(cost);
}

void testdijkstra()
{
	char ficgraphe[50];
	FILE *ptficgraphe; 
	graphe_s *graphe; 
	int numnoeud,nvmax,i,nsousg,nbnoeud,n1,n2,opt; 
	float *shortest,infini,dist,x1,y1,z1,x2,y2,z2,rap,temps; 
	clock_t t1,t2; 

	
	fprintf(stderr,"\n nom du fichier graphe: ");
	scanf("%s",ficgraphe);

	fprintf(stderr,"\n numero du noeud de depart, nb max de voisins: ");
	scanf("%d %d",&numnoeud,&nvmax);

	fprintf(stderr,"\n intervalle d'ecriture des plus courts chemins (2 entiers)");
	scanf("%d %d",&n1,&n2);

	fprintf(stderr,"\n option pour la fonction de cout des chemins (1:rayons, 2:volumes");
	scanf("%d",&opt);

	/* lecture du graphe */
	graphe = lecgraphe(ficgraphe);
	nbnoeud = graphe->nbnoeuds;
	t1 = clock(); 
	shortest = dijkstra(graphe,numnoeud,nvmax,opt);
	t2 = clock(); 
	temps = (float)(t2 - t1) / CLOCKS_PER_SEC;
	temps = temps/60.0;
	fprintf(stderr,"\n temps de calcul pour Dijkstra = %f minutes",temps); 
	infini = 10e37; 
	nsousg = 0;
	x1 = graphe->carac[1][numnoeud];
	y1 = graphe->carac[2][numnoeud];
	z1 = graphe->carac[3][numnoeud];
	for(i=1;i <= nbnoeud;i++)
	{
		if(shortest[i] < infini)
		{
			nsousg += 1;
		}
	}
	fprintf(stderr,"\n nombre de noeuds a distance fini = %d",nsousg);
	/* affichage des elements de shortest compris entre n1 et n2 */
	for(i=n1;i <= n2;i++)
	{
		x2 = graphe->carac[1][i];
		y2 = graphe->carac[2][i];
		z2 = graphe->carac[3][i];
		dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
		if( (dist==0) && (shortest[i]==0))
		{
			rap = 1;
		}else
		{
			rap = dist/shortest[i]; 
		}
		fprintf(stderr,"\n distance geodes entre noeud %d et noeud %d = %f ; distance euclidienne centres = %f ; rapport = %f",numnoeud,i,shortest[i],dist,rap);
	}
}

/* calcul de l'accessibilté de la matière organique avec la définition donné dans l'article de Patricia */
/* si opt ==1 alors le cout d'un arc est la distance entre les deux centres si opt == 2 la somme des volumes des deux boules */

float calculaccess(graphe_s *graphe,int nvmax,int opt)
{
	
	int numnoeud,i,nsousg,nbnoeud,n1,n2,j,nbbio,nbdom; 
	float *shortest,infini,dist,x1,y1,z1,x2,y2,z2,rap,temps,mj; 
	clock_t t1,t2; 
	float *acc,shj,sommemj,access,massebio; 

	t1 = clock(); 
	nbnoeud = graphe->nbnoeuds;
	infini = 10e37; 
	acc = (float*)mycalloc(nbnoeud+1,sizeof(float));

	sommemj = 0; 
	massebio = 0; 
	nbbio = 0;
	nbdom = 0; 
	for(i=1;i<=nbnoeud;i++)
	{
		sommemj += graphe->carac[6][i];
		massebio += graphe->carac[5][i];
		if(graphe->carac[6][i] != 0)nbdom++;
		if(graphe->carac[5][i] != 0)nbbio++;
	}
	fprintf(stderr,"\n biomasse totale avant calcul  = %f ; masse totale de matiere organique avant calcul = %f ; nb boules biomasse = %d ; nb boules DOM = %d",massebio,sommemj,nbbio,nbdom); 

	/* initialisation du tableau acc a 0 */ 
	for(i=0;i<=nbnoeud;i++)
	{
		acc[i] = 0; 
	}
	/* sommemj = 0; */
	nbbio = 0; 
	massebio = 0; 
	/* parcourir les noeuds pour mettre a jour le tableau acc avec l'inverse de la distance geodesique a un patch de microorganismes */
	for(i=1;i<=nbnoeud;i++)
	{
		/* on regarde si le noeud contient de la biomasse */
		if(graphe->carac[5][i] > 0)
		{
			nbbio++; 
			massebio += graphe->carac[5][i] ; 
			/* le noeud i contient de la biomasse, on regarde les distances geodesiques des noeuds contennat de la DOM avec lesquesls il est relie */
			shortest = dijkstra(graphe,i,nvmax,opt);
			/* mise a jour de acc avec les infos de shortest */
			for(j=1;j<=nbnoeud;j++)
			{
				/* on regarde si le noeud j et le noeud i sont relies et si le noeud j contient de la matiere organique */
				mj = graphe->carac[6][j];
				/* si le noeud j ne contient pas de matiere organique on ne fait rien */
				if(mj > 0)
				{
					/* cas ou le noeud j contient de la matiere organique */
					shj = shortest[j]; 
					/* on traite le cas ou le noeud i et le noeud j sont les memes
					dans ce cas on met la distance geodesique a la valeur du rayon de la boule i */
					if(shj == 0)
					{
						/* if(opt == 1)
						{*/
							shj = graphe->carac[4][i]; 
						/* }
						else
						if(opt == 2)
						{
							shj = (4.0/3.0)*M_PI*graphe->carac[4][i]*graphe->carac[4][i]*graphe->carac[4][i];
						}*/
					}

					if(shj < infini)
					{
						/* if(opt == 1)
						{*/
							acc[j] += mj/shj; 
						/* }
						else
						if(opt == 2)
						{
							acc[j] += mj/sqrt(shj);
						}*/
						
						/* sommemj += mj; */
					}
				}
			}
			myfreef(shortest); 
		}
	}
	fprintf(stderr,"\n nb de noeuds contenant de la biomasse = %d ; biomasse totale = %f ; masse totale de DOM = %f",nbbio,massebio,sommemj); 
	/* A la fin de la boucle on a rempli acc et on a calcule la somme des Mj (sommemj) */
	access = 0;
	for(i=1;i<nbnoeud;i++)
	{
		access += acc[i];
	}
	access = access/sommemj; 

	t2 = clock(); 
	temps = (float)(t2 - t1) / CLOCKS_PER_SEC;
	temps = temps/60.0;
	fprintf(stderr,"\n temps de calcul pour calcul accessibilite = %f minutes",temps); 
	/* liberation de l'espace memoire des tableaux */
	myfreef(acc);
	return(access); 
}

void testaccess()
{
	char ficgraphe[50];
	graphe_s *graphe; 
	int numnoeud,nvmax,i,nsousg,nbnoeud,n1,n2,opt; 
	float access; 
	

	
	fprintf(stderr,"\n nom du fichier graphe: ");
	scanf("%s",ficgraphe);

	fprintf(stderr,"\n nombre maximum de voisins = ");
	scanf("%d",&nvmax);

	fprintf(stderr,"\n option pour cout des arcs (1:rayons, 2: volumes) : ");
	scanf("%d",&opt);
	/* lecture du graphe */
	graphe = lecgraphe(ficgraphe);

	access = calculaccess(graphe,nvmax,opt);
	fprintf(stderr,"\n coefficient d'accessibilite de la matiere organique = %f",access); 
	
}

void testaccessval()
{
	char ficgraphe[50],ficgraphebio[50],ficmatlab[50];
	graphe_s *graphe,*graphebio; 
	int numnoeud,nvmax,i,nsousg,nbnoeud,n1,n2,opt; 
	float access; 
	
	fprintf(stderr,"\n nom du fichier graphe initial : ");
	scanf("%s",ficgraphe);

	fprintf(stderr,"\n nom du fichier graphe final : ");
	scanf("%s",ficgraphebio);

	fprintf(stderr,"\n nom du fichier matlab des courbes : ");
	scanf("%s",ficmatlab);


	fprintf(stderr,"\n nombre maximum de voisins = ");
	scanf("%d",&nvmax);

	
	/* lecture des graphes */
	graphe = lecgraphe(ficgraphe);
	fprintf(stderr,"\n fin lecture graphe initial, debut lecture graphe %s",ficgraphebio); 
	graphebio = lecgraphe(ficgraphebio);
	fprintf(stderr,"\n fin lecture des graphes");

	calculaccessval(graphe,graphebio,ficmatlab,nvmax); 
}

/* cette procedure prend en entree un fichier contenant un graphe valué avec une distribution de micro-organismes similaire
a celle de Valerie et rend en sortie un fichier contenant un graphe valué avec la meme distribution des micro-organismes
mais une autre distribution de matiere organique. On donne en entree:  le nombre de patchs,
un intervalle de choix pour les boules. Les patchs sont repartis de maniere aleatoire 
rand() genere un nombre aleatoire entre 0 et 1 
la masse totale de matiere organique est conservee */

void grapheaccess(char *ficgraphe1, char *ficgraphe2, int nbpatchs, int n1, int n2)
{
	FILE *ptficgraphe1, *ptficgraphe2 ; 
	graphe_s *graphe; 
	int nbnoeud,i,nbal,nb; 
	float masse1,massepatch,rebal,masse2; 

	graphe = lecgraphe(ficgraphe1);
	fprintf(stderr,"\n fin de la lecture du graphe");
	nbnoeud = graphe->nbnoeuds;
	
	masse1 = 0; 
	/* parcours des noeuds du graphe */
	/* mise a 0 de la matiere organique */ 
	for(i=1;i<=nbnoeud;i++)
	{
		masse1 += graphe->carac[6][i]; 
		if(masse1 < 0)fprintf(stderr,"\n masse1 = %f",masse1);
		graphe->carac[6][i] = 0; 
	}
	fprintf(stderr,"\n total masse DOM initiale = %f",masse1); 

	/* on met nbpatchs de matiere organique aleatoirement dans les noeuds compris entre n1 et n2 */
	massepatch = masse1/nbpatchs;
	srand(time(NULL));
	masse2 = 0; 
	for(i=1;i<=nbpatchs;i++)
	{
		nb = (rand() % (n2-n1+1)) + n1; 

		/* nb est un nombre aleatoire compris entre n1 et n2 */
		/* fprintf(stderr,"\n nb = %d",nb); */
		graphe->carac[6][nb] += massepatch;  
		masse2 += massepatch; 
	}
	fprintf(stderr,"\n total masse DOM finale = %f",masse2); 
	fprintf(stderr,"\n ecriture du graphe");
	ecrigraphe(graphe,ficgraphe2);
}

void testgrapheaccess()
{
	char fic1[50], fic2[50]; 
	int n1,n2,np; 

	fprintf(stderr,"\n nom du fichier graphe initial : ");
	scanf("%s",fic1);

	fprintf(stderr,"\n nom du fichier graphe resultat : ");
	scanf("%s",fic2);

	fprintf(stderr,"\n nombre de patchs ; intervalle des noeuds (n1,n2) indexes a partir de 1");
	scanf("%d %d %d",&np,&n1,&n2);

	grapheaccess(fic1, fic2, np, n1, n2);

}

void calculaccessval(graphe_s *graphe,graphe_s *graphebio,char *ficmatlab,int nvmax)
{
	
	int numnoeud,i,nsousg,nbnoeud,n1,n2,j,nbbio,nbdom,nb,n,*nbelements,*nbelem,ntra,*vois,nvois; 
	float *shortest,infini,dist,x1,y1,z1,x2,y2,z2,rap,temps,mj,access2,access3,access4; 
	clock_t t1,t2; 
	float *acc,shj,sommemj,access,massebio,*accessib,*rayons,*dom1,*dom2,massecarbfin,disntra,masseseuil; 
	FILE *ptficmatlab; 

	fprintf(stderr,"\n entree dans calculaccessval"); 
	t1 = clock(); 
	nbnoeud = graphe->nbnoeuds;
	infini = 10e37; 
	acc = (float*)mycalloc(nbnoeud+1,sizeof(float));
	nbelements = (int*)mycalloc(nbnoeud+1,sizeof(int));
	vois = (int*)mycalloc(nvmax,sizeof(int));
	ptficmatlab = fopen(ficmatlab,"w");
	/* traces complementaires */
	fprintf(stderr,"\n numero du noeud pour trace complementaire: ");
	scanf("%d",&ntra);
	
	fprintf(stderr,"\n trace pour diffusion matiere organique");
	nbdom = 0; 
	sommemj = 0; 
	massebio = 0; 
	/* ecrire les boules comprises entre n1 et n2 qui ont une masse de MO au moins egale a masseseuil */
	fprintf(stderr,"\n intervalle des boules, seuil de la masse: ");
	scanf("%d %d %f",&n1,&n2,&masseseuil); 

	for(i=1;i<=nbnoeud;i++)
	{
		if( (i>=n1) && (i<=n2) && (graphebio->carac[6][i] >= masseseuil) && graphebio->carac[5][i] == 0)
		{
			fprintf(stderr,"\n masse DOM pour boule %d dans graphe final = %f",i,graphebio->carac[6][i]); 
		}

		if(graphebio->carac[6][i] != 0)
		{
			if(graphebio->carac[5][i] != 0)
			{
				massebio += graphebio->carac[6][i];
			}
			nbdom++;
			sommemj += graphebio->carac[6][i];
		}
		
	}
	fprintf(stderr,"\n nombre de boules contenant de la DOM apres diffusion = %d masse totale de DOM restante = %f",nbdom,sommemj);
	fprintf(stderr,"\n masse de DOM dans les boules contenant des MO = %f",massebio);

	sommemj = 0; 
	massebio = 0; 
	nbbio = 0;
	nbdom = 0; 
	for(i=1;i<=nbnoeud;i++)
	{
		sommemj += graphe->carac[6][i];
		massebio += graphe->carac[5][i];
		if(graphe->carac[6][i] != 0)nbdom++;
		if(graphe->carac[5][i] != 0)nbbio++;
	}
	fprintf(stderr,"\n biomasse totale avant calcul  = %f ; masse totale de matiere organique avant calcul = %f ; nb boules biomasse = %d ; nb boules DOM = %d",massebio,sommemj,nbbio,nbdom); 

	/* initialisation du tableau acc a 0 */ 
	for(i=0;i<=nbnoeud;i++)
	{
		acc[i] = 0; 
		nbelements[i]=0; 
	}
	/* sommemj = 0; */
	nbbio = 0; 
	massebio = 0; 
	/* parcourir les noeuds pour mettre a jour le tableau acc avec l'inverse de la distance geodesique a un patch de microorganismes */
	fprintf(stderr,"\n trace pour noeud matiere organique: %d \n",ntra); 
	disntra = 0; 
	fprintf(stderr,"\n carac. de %d: %f %f %f %f %f %f",ntra,graphe->carac[1][ntra],graphe->carac[2][ntra],graphe->carac[3][ntra],graphe->carac[4][ntra],graphe->carac[5][ntra],graphe->carac[6][ntra]);
	fprintf(stderr,"\n carac. final: %f %f %f %f %f \n",graphebio->carac[5][i],graphebio->carac[6][i],graphebio->carac[7][i],graphebio->carac[8][i],graphebio->carac[9][i]);
	voisins(ntra,graphe,nvmax,&nvois,&vois);
	fprintf(stderr,"\n nombre de voisins de %d = %d",ntra,nvois);
	fprintf(stderr,"\n rayons des voisins :");
	for(i=0;i<nvois;i++)
	{
		fprintf(stderr," %f ",graphe->carac[4][vois[i]]); 
	}
	fprintf(stderr,"\n"); 
	for(i=1;i<=nbnoeud;i++)
	{
		/* on regarde si le noeud contient de la biomasse */
		if(graphe->carac[5][i] > 0)
		{
			nbbio++; 
			massebio += graphe->carac[5][i] ; 
			/* le noeud i contient de la biomasse, on regarde les distances geodesiques des noeuds contennat de la DOM avec lesquesls il est relie */
			shortest = dijkstra(graphe,i,nvmax,1);
			/* mise a jour de acc avec les infos de shortest */
			for(j=1;j<=nbnoeud;j++)
			{
				/* on regarde si le noeud j et le noeud i sont relies et si le noeud j contient de la matiere organique */
				mj = graphe->carac[6][j];
				/* si le noeud j ne contient pas de matiere organique on ne fait rien */
				if(mj > 0)
				{
					/* cas ou le noeud j contient de la matiere organique */
					shj = shortest[j]; 
					/* on traite le cas ou le noeud i et le noeud j sont les memes
					dans ce cas on met la distance geodesique a la valeur du rayon de la boule i */
					if(shj == 0)
					{
						shj = graphe->carac[4][i]; 
					}

					if(shj < infini)
					{
						acc[j] += mj/shj;
						nbelements[j]++; 
						if(j==ntra)
						{
							fprintf(stderr,"\n disgeo de %d: %f ; carac.: %f %f %f %f %f %f",i,shj,graphe->carac[1][i],graphe->carac[2][i],graphe->carac[3][i],graphe->carac[4][i],graphe->carac[5][i],graphe->carac[6][i]);
							fprintf(stderr,"\n carac. final: %f %f %f %f %f \n",graphebio->carac[5][i],graphebio->carac[6][i],graphebio->carac[7][i],graphebio->carac[8][i],graphebio->carac[9][i]);
							disntra += mj/shj;
						}
					}
				}
			}
			myfreef(shortest); 
		}
	}
	fprintf(stderr,"\n disntra = %f",disntra); 
	fprintf(stderr,"\n nb de noeuds contenant de la biomasse = %d ; biomasse totale = %f ; masse totale de DOM = %f",nbbio,massebio,sommemj); 
	/* A la fin de la boucle on a rempli acc et on a calcule la somme des Mj (sommemj) */
	access = 0;
	nb = 0;
	massecarbfin = 0; 
	access2 = 0; 
	access4 = 0; 
	for(i=1;i<nbnoeud;i++)
	{
		massecarbfin += graphebio->carac[9][i];
		access += acc[i];
		access2 += acc[i]/((4.0/3.0)*M_PI*(graphe->carac[4][i])*(graphe->carac[4][i])*(graphe->carac[4][i]));
		access3 += acc[i]*acc[i];
		access4 += sqrt(acc[i]); 
		if(acc[i] != 0)
		{
			nb++; 
		}
	}
	access = access/sommemj; 
	access2 = access2/sommemj; 
	access3 = access3/sommemj; 
	access4 = access4/sommemj; 
	fprintf(stderr,"\n accessibilite globale avec les carres = %f ; accessibilite globale avec les racines=%f ",access3,access4); 
	fprintf(stderr,"\n nombre de noeuds avec de la DOM = %d ; accessibilite masse = %f ; accessibilite concent= %f ; masse finale totale de CO2= %f",nb,access,access2,massecarbfin); 
	rayons = (float*)mycalloc(nb,sizeof(float));
	accessib = (float*)mycalloc(nb,sizeof(float));
	dom1 = (float*)mycalloc(nb,sizeof(float));
	dom2 = (float*)mycalloc(nb,sizeof(float));
	nbelem = (int*)mycalloc(nb,sizeof(int)); 
	/* acc contient pour chaque noeud l'accessibilité de la matière organique contenue dans le noeud par les micro-organismes */
	n = 0;
	for(i=1;i<nbnoeud;i++)
	{
		if(acc[i] != 0)
		{
			fprintf(stderr,"\n noeud:%d ; rayon=%f ;acc=%f ; DOM init=%f ; DOM fin=%f ; nb de connexions=%d",i,graphe->carac[4][i],acc[i],graphe->carac[6][i],graphebio->carac[6][i],nbelements[i]);
			rayons[n] = graphe->carac[4][i];
			dom1[n] = graphe->carac[6][i];
			dom2[n] = graphebio->carac[6][i];
			accessib[n] = acc[i];
			nbelem[n] = nbelements[i];
			n++; 
			if(graphe->carac[5][i] != 0)
			{
				fprintf(stderr,"\n MB init=%f ; MB fin=%f ; CO2=%f",graphe->carac[5][i],graphebio->carac[5][i],graphebio->carac[9][i]);
			}
		}
	}
	fprintf(stderr,"\n nombre de noeuds avec accessibilite non nulle = %d",n); 
	fprintf(stderr,"\n \n \n \n"); 
	/* ecriture fichiers matlab */
	fprintf(ptficmatlab,"X = [");
	for(i = 0;i<nb;i++)fprintf(ptficmatlab,"%f ",(float)(i));
	fprintf(ptficmatlab,"];");
	
	fprintf(ptficmatlab,"\nY0 = [");
	for(i = 0;i<nb;i++)fprintf(ptficmatlab,"%f ",accessib[i]);
	fprintf(ptficmatlab,"];");

	fprintf(ptficmatlab,"\n");

	fprintf(ptficmatlab,"Y1 = [");
	for(i = 0;i<nb;i++)fprintf(ptficmatlab,"%f ",rayons[i]/10.0);
	fprintf(ptficmatlab,"];");

	fprintf(ptficmatlab,"\n");

	fprintf(ptficmatlab,"Y2 = [");
	for(i = 0;i<nb;i++)fprintf(ptficmatlab,"%f ",dom1[i]);
	fprintf(ptficmatlab,"];");

	fprintf(ptficmatlab,"\n");

	fprintf(ptficmatlab,"Y3 = [");
	for(i = 0;i<nb;i++)fprintf(ptficmatlab,"%f ",dom2[i]*100);
	fprintf(ptficmatlab,"];");

	fprintf(ptficmatlab,"Y4 = [");
	for(i = 0;i<nb;i++)fprintf(ptficmatlab,"%f ",dom2[i]*100);
	fprintf(ptficmatlab,"];");

	fprintf(ptficmatlab,"Y5 = [");
	for(i = 0;i<nb;i++)fprintf(ptficmatlab,"%f ",(float)nbelem[i]);
	fprintf(ptficmatlab,"];");


	fprintf(ptficmatlab,"\n");

	/* on a ecrit Y0,Y1,Y3*/

	/* on appelle la fonction plot */

	fprintf(ptficmatlab,"plot(X,Y0,'r',X,Y1,'g',X,Y3,'b',X,Y5,'c');");
	fprintf(ptficmatlab,"\n");
	t2 = clock(); 
	temps = (float)(t2 - t1) / CLOCKS_PER_SEC;
	temps = temps/60.0;
	fprintf(stderr,"\n temps de calcul pour calcul accessibilite = %f minutes",temps); 
	/* liberation de l'espace memoire des tableaux */
	myfreef(acc);
	myfreef(accessib);
	myfreef(dom1);
	myfreef(dom2);
	myfreef(rayons);
	myfreei(nbelem);
	myfreei(nbelements);
	fclose(ptficmatlab); 
}

/* A partir d'un fichier d'entiers (fic1) contenant une image on génère un autre fichier d'entiers (fic2) ou les voxels a une
certaine valeur sont mis a 0 et les autres a 255 */

void transformval()
{
	FILE *ptfic1, *ptfic2 ; 
	char fic1[50],fic2[50];
	int i,nb,dimx,dimy,dimz,valeur,nbel,entier;
	float rapport; 

	fprintf(stderr,"\n nom fichier 1 , nom fichier 2 : ");
	scanf("%s %s",fic1,fic2);

	fprintf(stderr,"\n dimx, dimy, dimz: ");
	scanf("%d %d %d",&dimx,&dimy,&dimz); 

	fprintf(stderr,"\n valeur niveau de gris de la forme = ");
	scanf("%d",&valeur);

	nbel = dimx*dimy*dimz; 
	nb = 0; 

	ptfic1 = fopen(fic1,"r");
	ptfic2 = fopen(fic2,"w");

	for(i=0;i<nbel;i++)
	{
		if( (i%dimx == 0) && (i != 0))
		{
			fprintf(ptfic2,"\n"); 
		}
		fscanf(ptfic1,"%d",&entier);
		if(entier == valeur)
		{
			nb++;
			fprintf(ptfic2,"%d ",0);
		}
		else
		{
			fprintf(ptfic2,"%d ",255); 
		}
	}
	rapport = (float)nb / (float)nbel;
	fprintf(stderr,"\n nombre points de la forme =  %d ; rapport nombre points de la forme / nombre de points total = %f",nb,rapport);

	fclose(ptfic1);
	fclose(ptfic2); 
}

/* cette fonction compte les points d'une image sous forme de fichier de lignes avec une certaine valeur */
/* A partir d'un fichier d'entiers (fic1) contenant une image on génère un autre fichier d'entiers (fic2) ou les voxels a une
certaine valeur sont mis a 0 et les autres a 255 */

void comptepoints()
{
	FILE *ptfic1; 
	char fic1[50];
	int i,nb,dimx,dimy,dimz,valeur,nbel,entier;

	fprintf(stderr,"\n nom fichier image: ");
	scanf("%s",fic1);

	fprintf(stderr,"\n dimx, dimy, dimz: ");
	scanf("%d %d %d",&dimx,&dimy,&dimz); 

	fprintf(stderr,"\n valeur niveau de gris de la forme = ");
	scanf("%d",&valeur);

	nbel = dimx*dimy*dimz; 
	nb = 0; 

	ptfic1 = fopen(fic1,"r");

	for(i=0;i<nbel;i++)
	{
		fscanf(ptfic1,"%d",&entier);
		if(entier == valeur)
		{
			nb++;
		}
	}
	
	fprintf(stderr,"\n nombre points de la forme =  %d",nb);

	fclose(ptfic1);
}
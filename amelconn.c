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

    corres = mycalloc(nboules,sizeof(int)); 

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
    for(i=0;i<nbtet)
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

    fprintf(stderr,"\n nombre arcs rajoutes avec Delaunay = %d",narcplus)

	fclose(ptarcs);

	fclose(pttrav);

	remove("arc.trav");

	free(Xb);
	free(Yb);
	free(Zb);
	free(Rb);
}

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
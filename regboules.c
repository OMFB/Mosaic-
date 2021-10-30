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

void testcgraf1_s(void)

{

	char cyl1[50],cyl2[50],cyl3[50];

	graphe_s *graf;

	float seuil,rap;

	tas_s *tas1;

	int nbmaxcomp,opt,nbmaxvois;

	int nbarcs,dimx,dimy,dimz;



	fprintf(stderr,"\n nom du fichier de boules initiales en entree : ");

	scanf("%s",cyl1);

	fprintf(stderr,"\n nom du fichier de chaines de boules en sortie apres fusions : ");

	scanf("%s",cyl2);

	fprintf(stderr,"\n nom du fichier des arcs du graphe des boules : ");

	scanf("%s",cyl3);

	fprintf(stderr,"\n si fichier pour fatou taper 1 sinon 0 : ");

	scanf("%d",&opt);

	fprintf(stderr,"\n nombre maximal de boules dans une chaine (au moins 100) =");

	scanf("%d",&nbmaxcomp);

	fprintf(stderr,"\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	fprintf(stderr,"\n nb d'elements dans le tas = rap*nbarcs; rap =");

	scanf("%f",&rap);

	fprintf(stderr,"\n nb maximum de voisins par noeud =");

	scanf("%d",&nbmaxvois);

	arcs(cyl1,cyl3, dimx,dimy,dimz,&nbarcs,nbmaxvois);

	fprintf(stderr,"\n fin de arcgraf");


	graf = cregraphe_s2(cyl1,cyl3,&tas1,nbmaxcomp,rap);

	fprintf(stderr,"\n fin de la creation de graphe");


	fprintf(stderr,"\n nb de noeuds = %d",graf->nbnoeuds);

	fprintf(stderr,"\n nb d'arcs = %d",graf->nco);


	ctas1_s(graf,tas1,seuil);

	fprintf(stderr,"\n nb d'elements du tas = %d",tas1->nt);

	fprintf(stderr,"\n fin de la creation du tas ");


	hiera1_s(graf,tas1,seuil,nbmaxcomp);


	/* stockage des boules dans un fichier */

	fprintf(stderr,"\n fin des regroupements");

	fprintf(stderr,"\n nb de noeuds = %d",graf->nbnoeuds);


	fprintf(stderr,"\n stockage des chaines simplement connexes");

	stockboules_s(graf,cyl2,opt);

	fprintf(stderr,"\n fin du stockage des chaines simplement connexes");

}


/* cette proc�dure stocke les chaines du graphe dans l'ordre d�croissant de leur nombre de boules */

void stockboules_s(graphe_s *graf, char *cyl2, int opt)
{
	float **carac,*val,valout ;

	int *itas,nt,iout,n1,n2,na;

	int *liste,k,n,i,l,*label2,narcs;

	FILE *ptfic;

	fprintf(stderr,"\n entree dans stockboules");

	carac = graf->carac ;

	liste = graf->liste ;

	n = graf->nbnoeuds;

	narcs = 0;

	nt = 0;

	itas = (int*)mycalloc(graf->nel +1,sizeof(int));

	val = (float*)mycalloc(graf->nel + 1,sizeof(float));

	label2 = (int*)mycalloc(graf->nel +1,sizeof(int));


	fprintf(stderr,"\n fin des allocations, n = %d",n);

	for(i=1;i<=graf->nel;i++)
	{
		label2[i] = -1;
		if(liste[i] != -1)
		{
			nt++;
			itas[nt] = i;
			val[nt] = -carac[1][i];
		}
	  }


	 fprintf(stderr,"\n fin du remplissage de val et de itas ; nt = %d",nt);

	 cretas_s(val,itas,nt);

	 fprintf(stderr,"\n fin de la creation du tas, nt=%d",nt);

	 ptfic = fopen(cyl2,"w");

	 fprintf(ptfic,"\n %d",nt);

	 fprintf(stderr,"\n nombre de chaines = %d",nt);

	 k = 0;

	 while(nt > 0)
	 {
		 outas_s(val,itas,&nt,&iout,&valout);
		 k++;
		 i = (int)(floor(carac[1][iout]));
		 if(opt==0)
		 {
			 fprintf(ptfic,"\n %d",i);
		 }
		 else
		 {
			 fprintf(ptfic,"\n %d %d %d %d",i,0,0,0);
		 }
		 /* la chaine numero k dans le fichier des chaines a le numero iout dans le graphe des chaines */

		 label2[iout] = k;
		 l = 2;
		 for(i=1;i<= carac[1][iout];i++)
		 {
			 fprintf(ptfic,"\n %f %f %f %f",carac[l][iout],carac[l+1][iout],carac[l+2][iout],carac[l+3][iout]);
			 l = l + 4 ;
		 }

	 }

	 /* ecriture des arcs correspondant aux chaines dans le fichier apres les chaines */

	 /* on compte le nombre d'arcs du graphe */

	 for(i=1;i<= graf->nco;i++)
	 {
		 if(liste[graf->couple[1][i]] > 0 && liste[graf->couple[2][i]] > 0)
		 {
			 narcs++;
		 }
	 }

	 fprintf(stderr,"\n nb d'arcs du graphe des chaines = %d",narcs);

	 fprintf(ptfic,"\n %d",narcs);

	 /* ecriture des arcs dans le fichier */

	 na = 0;

	 for(i=1;i<= graf->nco;i++)
	 	 {
	 		 if(liste[graf->couple[1][i]] > 0 && liste[graf->couple[2][i]] > 0)
	 		 {
	 			 n1 = graf->couple[1][i];
	 			 n2 = graf->couple[2][i];
	 			 if(label2[n1] == -1 || label2[n2] == -1)
	 			 {
	 				 fprintf(stderr,"\n i = %d label2 a -1 n1 = %d n2 = %d label2(n1) = %d label2(n2) = %d",i,n1,n2,label2[n1],label2[n2]);
	 				 exit(56);
	 			 }
	 			 else
	 			 {
	 				 fprintf(ptfic,"\n %d %d",label2[n1],label2[n2]);
	 				 na++;
	 			 }
	 		 }
	 	 }

	 fprintf(stderr,"\n nb d'arcs ecrits dans le fichier = %d",na);
	 fclose(ptfic);
	 myfreei(itas);
	 myfreef(val);
	 myfreei(label2);
}


void cretas_s(float *val,int *itas,int n)

{

	int j,in ;

	float valin ;



	if(n > 1)

	{

	j=1 ;

un: valin=val[j+1] ;

    in=itas[j+1] ;
    intas_s(val,itas,&j,in,valin) ;

    if(j < n) goto un ;

	}

}


/* cette procedure prend en entree un fichier de boules

(nb des boules,x,y,z,r) et rend en sortie un pointeur

sur une structure de graphe contenant les boules

majrap definit la relation de connexite entre les boules. majrap est la valeur maximale du rapport entre la distance
entre les deux centres et la somme des deux rayons, on prendra typiquement majrap entre 1 et 1.2*/
/* nbmaxcomp est le nombre maximal de boules dans une composante connexe */

graphe_s *cregraphe_s(char *cyl,float majrap,tas_s **tas1,int nbmaxcomp)

{

	int nbarmax,nbnomax,ntasmax,ncar,i;

	int k ;

	float *car,rap;

	float r,s,x1,y1,z1,x2,y2,z2,r1,r2;

	float t1,t2,t3,t4;

	graphe_s *graphe1;

	FILE *ptcyl;



	ptcyl = fopen(cyl,"r");

	fscanf(ptcyl,"%d",&nbnomax);

	fprintf(stderr,"\n nb de noeuds du graphe initial = %d",nbnomax);

	nbarmax = nbnomax*7;

	ntasmax = floor(nbarmax);

	ncar = 1 + nbmaxcomp*4;

	s = majrap*majrap;

	fprintf(stderr,"\n debut de creation du graphe initial");

	graphe1 = creergraphe_s (nbnomax,nbarmax,ncar);

	fprintf(stderr,"\n fin de creation du graphe initial");

	*tas1 = creertas_s(ntasmax,nbarmax,nbnomax);



	/* creation des noeuds du graphe */

	fprintf(stderr,"\n debut de creation des noeuds du graphe");

	car = (float*)mycalloc(ncar + 1,sizeof(float));



	for(k=0;k<nbnomax;k++)

	{

		fscanf(ptcyl,"%f %f %f %f %f %f %f %f",&x1,&y1,&z1,&r,&t1,&t2,&t3,&t4);

		car[1] = 1.0;

		car[2] = x1;

		car[3] = y1;

		car[4] = z1;

		car[5] = r;

		crenoeud_s(graphe1,car,5);

	}


	fprintf(stderr,"\n fin de creation des noeuds du graphe");

	/* creation des arcs du graphe */



	for(k=1;k<= nbnomax;k++)

	{

		x1 = graphe1->carac[2][k];

		y1 = graphe1->carac[3][k];

		z1 = graphe1->carac[4][k];

		r1 = graphe1->carac[5][k];

		for(i=k+1;i<=nbnomax;i++)

		{

			x2 = graphe1->carac[2][i];

			y2 = graphe1->carac[3][i];

			z2 = graphe1->carac[4][i];

			r2 = graphe1->carac[5][i];

			if((r1 + r2) ==0)
			{
				fprintf(stderr,"/n r1 + r2 = 0 !!!");
				exit(56);
			}

			rap = ((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2))/((r1+r2)*(r1+r2));

			if(rap <= s)
			{
				incou2_s(graphe1,i,k);
			}

		}

	}

	fclose(ptcyl);

	myfreef(car);

	return(graphe1);

}

graphe_s *cregraphe_s2(char *cyl,char *arcs,tas_s **tas1,int nbmaxcomp,float rtas)

{

	int nbarmax,nbnomax,ntasmax,ncar,i;

	int k,nbarcs,n ;

	float *car;

	float r,x1,y1,z1;

	float t1,t2,t3,t4;

	graphe_s *graphe1;

	FILE *ptcyl,*ptarcs;



	ptcyl = fopen(cyl,"r");

	ptarcs = fopen(arcs,"r");

	fscanf(ptcyl,"%d",&nbnomax);

	fscanf(ptarcs,"%d",&nbarcs);

	fprintf(stderr,"\n nb de noeuds du graphe initial = %d",nbnomax);

	nbarmax = nbarcs + 1;

	ntasmax = floor(nbarmax*rtas);

	ncar = 1 + nbmaxcomp*4;

	fprintf(stderr,"\n debut de creation du graphe initial");

	graphe1 = creergraphe_s (nbnomax,nbarmax,ncar);

	fprintf(stderr,"\n fin de creation du graphe initial");

	*tas1 = creertas_s(ntasmax,nbarmax,nbnomax);



	/* creation des noeuds du graphe */

	fprintf(stderr,"\n debut de creation des noeuds du graphe");

	car = (float*)mycalloc(ncar + 1,sizeof(float));



	for(k=0;k<nbnomax;k++)

	{

		fscanf(ptcyl,"%f %f %f %f %f %f %f %f",&x1,&y1,&z1,&r,&t1,&t2,&t3,&t4);

		car[1] = 1.0;

		car[2] = x1;

		car[3] = y1;

		car[4] = z1;

		car[5] = r;

		crenoeud_s(graphe1,car,5);

	}


	fprintf(stderr,"\n fin de creation des noeuds du graphe");

	/* creation des arcs du graphe */



	for(n=0;n<= nbarcs;n++)

	{
		fscanf(ptarcs,"%d %d",&i,&k);
		incou2_s(graphe1,i,k);
	}

	fclose(ptcyl);

	myfreef(car);

	return(graphe1);

}

/* prend en entree un graphe qui sort de la simulation cad ou les noeuds
 * n'ont pas etes changes et remplit un fichier ou le graphe est ecrit, seules les tableaux liste, carac et
 * couple ainsi que le nombre de noeuds, le nombre de couples et le nombre d'attributs sont ecrits ; le graphe ne sera pas
 * utilise pour des regroupements mais simplement pour acceder aux noeuds et aux voisins */

/* la structure du fichier est la suivante : nb de noeuds, nb d'arcs, nb d'attributs, tableau liste, tableau carac, tableau couple */

void ecrigraphe(graphe_s *graphe1,char *fic)
{
	FILE *ptfic;

	int nbnoeuds,i,j,nbarcs,ncar ;


	ncar = graphe1->ncar;

	nbnoeuds = graphe1->nel;

	nbarcs = graphe1->nco;

	fprintf(stderr,"\n ecrigraphe: ncar = %d ; nbnoeuds = %d ; nbarcs = %d",ncar,nbnoeuds,nbarcs); 

	ptfic = fopen(fic,"w");

	fprintf(stderr,"\n nom du fichier graphe : %s",fic);

	/* ecriture du nombre de noeuds */
	fprintf(ptfic,"%d \n",nbnoeuds);

	/* ecriture du nombre d'arcs */
	fprintf(ptfic,"%d \n",nbarcs);

	/* ecriture du nombre d'attributs par noeud */
	fprintf(ptfic,"%d \n",ncar);

	/* ecriture de liste, de carac et de couple */

	/* ecriture de liste */


	for(i=1;i<=nbnoeuds;i++)
		fprintf(ptfic,"%d \n",graphe1->liste[i]);

	/* ecriture de carac */
	for(i=1;i<=nbnoeuds;i++)
		{
			fprintf(ptfic,"\n");
			for(j=1;j<=ncar;j++)
				fprintf(ptfic,"%f ",graphe1->carac[j][i]);
		}

	/* ecriture du tableau couple */

	for(i=1;i<=nbarcs;i++)
	{
		fprintf(ptfic,"\n");
		for(j=1;j<=6;j++)
			fprintf(ptfic,"%d ",graphe1->couple[j][i]);
	}

	fclose(ptfic);

}
 /* dual de la procedure ecrgrboul prend en entree un fichier contenant : le nombre de noeuds, le nombre d'arcs,

 le nombre d'attributs, les tableaux liste,carac, et couple et renvoue un pointeur sur un graphe contenant une

 structure de graphe correspondant au fichier */

graphe_s *lecgraphe(char *fic)
{
	FILE *ptfic;

	graphe_s *graphe1;

	float f1;

	int nbnoeuds,i,j,nbarcs,ncar,t1 ;


	ptfic = fopen(fic,"r");

	graphe1 = (graphe_s*)mymalloc(sizeof(graphe_s));

	/* lecture du nombre de noeuds */
	fscanf(ptfic,"%d",&nbnoeuds);

	/* lecture du nombre d'arcs */
	fscanf(ptfic,"%d",&nbarcs);

	/* lecture du nombre d'attributs par noeud */
	fscanf(ptfic,"%d",&ncar);

	fprintf(stderr,"\n procedure lecgraphe");

	fprintf(stderr,"\n nb de noeuds = %d ; nb d'arcs = %d ; nombre d'attributs par noeud = %d",nbnoeuds,nbarcs,ncar);

	graphe1->liste = (int*)mycalloc(nbnoeuds + 1,sizeof(int));

	graphe1->couple= allotab2i(6,nbarcs);

	graphe1->carac = allotab2f(ncar,nbnoeuds) ;

	graphe1->ncar = ncar;

	graphe1->nel = nbnoeuds;

	graphe1->nco = nbarcs;

	graphe1->nbnoeuds = nbnoeuds;


	/* ecriture de liste, de carac et de couple */

	/* ecriture de liste */


	for(i=1;i<=nbnoeuds;i++)
	{
		fscanf(ptfic,"%d",&t1);
		graphe1->liste[i] = t1;
	}

	/* ecriture de carac */
	for(i=1;i<=nbnoeuds;i++)
		{
			for(j=1;j<=ncar;j++)
			{
				fscanf(ptfic,"%f",&f1);
				graphe1->carac[j][i] = f1;
			}
		}

	/* ecriture du tableau couple */

	for(i=1;i<=nbarcs;i++)
	{
		for(j=1;j<=6;j++)
		{

			fscanf(ptfic,"%d",&t1);
			graphe1->couple[j][i] = t1;
		}
	}

fclose(ptfic);
return(graphe1);

}

/* creation d'un fichier contenant les noeuds d'un graphe avec tous les niveaux de voisinage a partir d'un graphe

le nom du fichier que l'on cree est fic et le graphe pris en entree est graphe1, les niveaux de voisinage sont ecrits jusau'au

niveau nivmax. nmaxvois est le nombre maximum de voisins par noeuds ; nmaxniv est le nombre maximum de noeuds pour un niveau de vosinage  */

void crevoisins(graphe_s *graphe1, char *fic, int nivmax,int nmaxvois,int nmaxniv)
{
	int i,*indic,nbnoeuds,j,*niv,nniv,k,l,etiq,n,test;

	FILE *ptfic;

	int *trav,*niv2,nniv2,nbv;

	/* on determine pour chacun des noeuds les voisins de niveaux 1 a nivmax et on les stocke dans le fichier. Le fichier est
	 structure de la maniere suivante : nombre de noeuds, nombre de niveaux, numero du noeud, niveau de voisinage (j)
	 , nb de voisins de niveau j , liste des voisins de niveau j */

	ptfic = fopen(fic,"w");

	/* ecriture du nombre de noeuds et du nombre de niveaux de voisinage pour chaque noeud */

	nbnoeuds = graphe1->nel;

	/* fprintf(stderr,"\n nbnoeuds = %d nivmax = %d fichier = %s",nbnoeuds,nivmax,fic);*/

	fprintf(ptfic,"\n %d %d",nbnoeuds,nivmax);

	/* fprintf(stderr,"\n debut des allocations de memoire dans crevoisins");*/


	/* determination des voisins de niveau 1 a nivmax des noeuds du graphe */

	indic = (int*)mycalloc(nbnoeuds+1,sizeof(int));

	trav = (int*)mycalloc(nmaxvois,sizeof(int));

	niv = (int*)mycalloc(nmaxniv,sizeof(int));

	niv2 = (int*)mycalloc(nmaxniv,sizeof(int));

	test = nbnoeuds;

	/* fprintf(stderr,"\n test des noeuds un a :");

	scanf("%d",&test);*/

	for(i=1;i<=test;i++)
	{
		if(i % 1000 == 0)fprintf(stderr,"\n nb de noeuds traites = %d",i);
		/* initialisatioo du tableau indic qui contient le niveau de vosinages des noeuds du graphe par rapport au noeud i */

		for(n=0;n<=nbnoeuds;n++)indic[n]=-1;

		/* on considere que i est un voisin de i de niveau 0 */
		indic[i] = 0;

		/* le tableau niv contient les nniv voisins du noeud i de niveau j-1 */
		/* initialisation du tableau niv avec le noeud initial i */

		nniv = 1;
		niv[0] = i;

		/* on ecrit le numero du noeud dans le fichier */

		fprintf(ptfic,"\n");

		/* fprintf(stderr,"\n i = %d",i);*/


		fprintf(ptfic,"\n %d",i);

		for(j=1;j<=nivmax;j++)
		/* determination des voisins du noeud i de niveau j */
		{
			/* le tableau niv contient les nniv voisins du noeud i de niveau j-1 si ce tableau est vide on s'arrete */

			if(nniv != 0)
			{

				/* determination des voisins de niveau j de i a l'aide des voisins de niveau j-1 */

				/* on initialise nniv2 a 0 car le tableau niv2 contiendra a la fin de la boucle
				 les voisins du noeud i de niveau j */
				nniv2 = 0;

				for(k=0;k<nniv;k++)
				{
					etiq = niv[k];
					/* determination des voisins du noeud etiq */
					voisins(etiq,graphe1,nmaxvois,&nbv,&trav);

					/* le tableau trav contient les nbv voisins du noeud etiqu dans le graphe graphe1 */

					/* on met dans l'ensemble des noeuds voisins de niveau j les voisins de etiq non inclus dans
					les niveaux precedents */

					for(l=0;l<nbv;l++)
					{
						if(indic[trav[l]] == -1)
						{
							indic[trav[l]] = j;
							if(nniv2 >= nmaxniv)
							{
								fprintf(stderr,"\n nombre maximum de noeuds par niveau de voisinage trop faible !");
								exit(4);
							}
							niv2[nniv2] = trav[l];
							nniv2++;
						}
					}
				}
				/* le tableau niv2 contient les voisinns du noeud i de niveau j */
				/* le tableau niv contient les voisins du noeud i de niveau j-1 */

				/* on range dans le fichier de sortie, le niveau de voisinage (j), le nombre des voisins de niveau j du noeud i,
				  ,les voisins du noeud i de niveau j */

				/* on met dans niv les voisins du noeud i de niveau j */

				/* fprintf(stderr,"\n nb de voisins de niveau %d du noeud %d = %d ",j,i,nniv2);*/

				fprintf(ptfic,"\n %d %d",j,nniv2);

				for(l=0;l<nniv2;l++)
				{
					/* fprintf(stderr,"\n ecriture du voisin %d ",niv2[l]);*/
					fprintf(ptfic,"\n %d",niv2[l]);
					niv[l] = niv2[l];
				}
				nniv = nniv2;

			}
			else
			{
				break;
			}
		}
	}
	free(niv);
	free(indic);
	free(niv2);
	free(trav);
	fclose(ptfic);
}



/* determination des voisins d'un noeud dans un graphe, on renvoie un tableau contenant
 les numeros des voisins (vois) ainsi que le nombre de voisins (nvois). On prend en entree le numero du noeud, le graphe et le
 nombre maximum de voisins
 en sortie le tableau des voisins est rempli. On choisit la solution statique pour ne pas multiplier les
 appels de malloc car cette procedure sera appellee des millions de fois; nvmax est le nombre maximum de voisins */

void voisins(int n,graphe_s *graphe1,int nvmax,int *nvois,int **vois)
{
	int **couple,*liste,voisin,icouple;

	couple = graphe1->couple;
	liste = graphe1->liste;

	*nvois = 0;
	icouple = liste[n];

	while(icouple > 0)
	{
		if(couple[1][icouple] == n)
		{
			voisin = couple[2][icouple];
			icouple = couple[3][icouple];
		}
		else
		{
			voisin = couple[1][icouple];
			icouple = couple[5][icouple];
		}
		(*vois)[*nvois] = voisin;
		(*nvois)++;
		if(*nvois >= nvmax)
		{
			fprintf(stderr,"\n trop de voisins dans voisins !!");
			exit(5);
		}
	}
	return;
}


/* creation d'un graphe pour la simulation biologique a partir d'un fichier de boules, d'un fichier d'arcs,
 d'un fichier d'attributs des pores (nb d'attributs complementaires pour chaque pore en dehors du rayon de la boule,
 liste des attributs complementaires). Le graphe cree est pour la simulation donc
 comprend un tas vide */

graphe_s *cregraphe_s3(char *cyl,char *arcs,char *attributs)

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

		fscanf(ptcyl,"%f %f %f %f %f %f %f %f",&x1,&y1,&z1,&r,&t1,&t2,&t3,&t4);

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

/* determine tous les arcs du graphe et les stocke dans le fichier cyl2 qui contient la suite des arcs avec les numeros de noeud
 * dans l'ordre croissant */

void arcs (char *cyl,char *cyl2,int dimx, int dimy, int dimz,int *nbarcs,int nmaxv)

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

		fscanf(ptboules,"%f %f %f %f %f %f %f %f",&x0,&y0,&z0,&r,&a2,&u,&v,&w);

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

		fscanf(ptboules,"%f %f %f %f %f %f %f %f",&x0,&y0,&z0,&r,&a2,&u,&v,&w);

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

void ctas1_s(graphe_s *graphe1,tas_s *tas1,float seuil)

{


float **carac,*val,*vacou,*car1,*car2 ;

int **couple,*liste,*itas,i ;

int ncar,nco,nel,nt ;

float valin ;

int iok,j,n1,n2,nel1,nel2;



    carac = graphe1->carac ;

    val = tas1->val ;

    vacou = tas1->vacou ;

    couple = graphe1->couple ;

    liste = graphe1->liste ;

    itas = tas1->itas ;

    ncar = graphe1->ncar ;

    nco = graphe1->nco ;

    nel = graphe1->nel ;

    nt = 0 ;

    tas1->nt = 0;


    car1 = (float*)mycalloc(ncar + 1,sizeof(float));

    car2 = (float*)mycalloc(ncar + 1,sizeof(float));



    i = 1 ;

un: if(couple[1][i] == 0)goto deux ;

         n1 = couple[1][i] ;

         n2 = couple[2][i] ;  ;

         if((liste[n1] == -1) || (liste[n2] == -1)){

            i = i + 1 ;

            goto un ;

         }

         for (j =1 ; j <= 4*carac[1][n1] + 1; j++)

            car1[j] = carac[j][n1];



         for (j =1 ; j <= 4*carac[1][n2] + 1 ; j++)

            car2[j] = carac[j][n2];



      	fta1_s(car1,car2,ncar,seuil,&iok,&valin) ;

      if(iok == 1){

      	 if(nt >= tas1->nmaxtas)exit(1);

         nel1 = couple[1][i];
         nel2 = couple[2][i];
         intas_s(val,itas,&nt,i,valin) ;

         tas1->nt = nt ;

         vacou[i] = valin ;

      }else{

         vacou[i] = -1 ;

      }

         i = i + 1 ;

      goto un ;

deux:   /*---------------------------------------*/ ;

myfreef(car1);
car1 = NULL;
myfreef(car2);
car2=NULL;

}


void fta1_s_k(float *car1,float *car2,int ncar,float seuil,int *regroup,float *cout)

{
	int i,indc,sel,j;
	float rmin,rmax,r;

	sel = 0;
	*regroup = 0;
	for(i=1;i<= car1[1];i++)
	{
		for(j=1;j<=car2[1];j++)
		{

			indc = connexb(i,j,car1,car2,1);

			/* si deux boules qui ne sont pas des extremites sont connexes la chaine
			 n'est pas simplement connexe et on ne regroupe pas */

			if(indc == 1)
			{
				if((i != 1 ) && (i != car1[1]))
				{
					goto fin;
				}
				else
				{
					if(j == 1)
					{
						if(sel >=1)goto fin;
						sel++;
					}
					else
					{
						if(j == car2[1])
						{
							if(sel >=1)goto fin;
							sel++;
						}
						else goto fin;
					}
				}
			}

		}
	}
	if(sel == 1)*regroup = 1;

	fin : /*--------------------*/;

	if(*regroup == 1)
	{
		/* calcul du cout du regroupement */
		rmax = car1[5];
		rmin = car1[5];
		for(j = 2;j <= car1[1] ; j++)
		{
			r = car1[5 + 4*(j-1)];
			if(r > rmax)rmax = r;
			if(r < rmin)rmin = r;
		}
		for(j = 1;j <= car2[1] ; j++)
		{
			r = car2[5 + 4*(j-1)];
			if(r > rmax)rmax = r;
			if(r < rmin)rmin = r;
		}
/*		*cout = rmax - rmin;*/
		*cout = -rmin;

	}



}

void fta1_s(float *car1,float *car2,int ncar,float seuil,int *regroup,float *cout)

{
	int i,indc,sel,j;
	float rmin,rmax,r;

	sel = 0;
	*regroup = 0;
	for(i=1;i<= car1[1];i++)
	{
		for(j=1;j<=car2[1];j++)
		{

			indc = connexb(i,j,car1,car2,1);

			/* si deux boules qui ne sont pas des extremites sont connexes la chaine
			 n'est pas simplement connexe et on ne regroupe pas */

			if(indc == 1)
			{
				if((i != 1 ) && (i != car1[1]))
				{
					goto fin;
				}
				else
				{
					if(j == 1)
					{
						if(sel >=1)goto fin;
						sel++;
					}
					else
					{
						if(j == car2[1])
						{
							if(sel >=1)goto fin;
							sel++;
						}
						else goto fin;
					}
				}
			}

		}
	}
	if(sel == 1)*regroup = 1;

	fin : /*--------------------*/;


	if(*regroup == 1)
	{
		/* calcul du cout du regroupement */
		rmax = car1[5];
		rmin = car1[5];
		for(j = 2;j <= car1[1] ; j++)
		{
			r = car1[5 + 4*(j-1)];
			if(r > rmax)rmax = r;
			if(r < rmin)rmin = r;
		}
		for(j = 1;j <= car2[1] ; j++)
		{
			r = car2[5 + 4*(j-1)];
			if(r > rmax)rmax = r;
			if(r < rmin)rmin = r;
		}
/*		*cout = rmax - rmin;*/
		*cout = (rmax - rmin)/rmax;
		if( *cout > 0.25)*regroup = 0;
	}



}

/* cette proc�dure prend en entr�e deux num�ro de boules de car1 et de car2 avec une num�rotation
 commen�ant � 1 et renvoie 1 si elles sont connexes et 0 sinon. seuil est classiquement mis � 1 */

int connexb(int n1, int n2, float *car1, float * car2, float seuil)
{
	int connexe,i;
	float x1,y1,z1,r1,x2,y2,z2,r2;

	x1 = car1[2 + 4*(n1-1)];
	y1 = car1[3 + 4*(n1-1)];
	z1 = car1[4 + 4*(n1-1)];
	r1 = car1[5 + 4*(n1-1)];

	x2 = car2[2 + 4*(n2-1)];
	y2 = car2[3 + 4*(n2-1)];
	z2 = car2[4 + 4*(n2-1)];
	r2 = car2[5 + 4*(n2-1)];

	if((r1 + r2) == 0)
	{
		fprintf(stderr,"\n erreur dans connexb !!");
		fprintf(stderr,"\n r1 = %f r2 = %f",r1,r2);
		fprintf(stderr,"\n");
		for(i=1 ; i<= 1 + 4*car1[1];i++)
			fprintf(stderr,"car1(%d)=%f",i,car1[i]);
		fprintf(stderr,"\n");
		for(i=1 ; i<= 1 + 4*car2[1];i++)
			fprintf(stderr,"car2(%d)=%f",i,car2[i]);
		fprintf(stderr,"\n");
			fprintf(stderr,"\n n1 = %d n2 = %d seuil = %f",n1,n2,seuil);
		exit(43);
	}
	if( (((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2))/((r1+r2)*(r1+r2))) <= (seuil*seuil) )connexe=1;
	else connexe=0;

	return(connexe);
}

/* cette procedure cree un tas initialement vide

entree :

ntasmax : nombre maximum d'elements du tas



sortie : pointeur sur la structure de tas cree */



tas_s *creertas_s (int ntasmax,int nbarmax,int nbnodemax)

{

	tas_s *tas1 ;

	int *itas,*label,i ;

	float *val,*vacou;



	tas1 = (tas_s*)mymalloc(sizeof(tas));

	val = (float*)mycalloc(ntasmax + 1,sizeof(float));

	itas = (int*)mycalloc(ntasmax + 1,sizeof(int));

	label = (int*)mycalloc(nbnodemax + 1,sizeof(int));

	vacou = (float*)mycalloc(nbarmax +1,sizeof(float));



	tas1->nmaxtas = ntasmax ;

	tas1->nt = 0 ;

	tas1->val = val;

	tas1->itas = itas ;

	tas1->label = label ;

	tas1->vacou = vacou ;





	/* initialisation du tas a 0 */



	for(i=1 ; i <= ntasmax ; i++)

	{

		val[i] = 0 ;

		itas[i] = 0 ;



	}

	 /* c     initialisation de label */



	for (i=1 ; i <= nbnodemax ; i++)

		{

			label[i] = i ;

      	}



	for (i=1 ; i <= nbarmax ; i++)

	vacou[i] = -1 ;



	return(tas1);

}




/* cette proc�dure cree un graphe value initialement vide

entree :

nbnomax : nombre maximun de noeuds

nbarmax : nombre maximum d'arcs

ncarac : nombre de caracteristiques noeuds



sortie : pointeur sur la structure de graphe cree et initialise

les tableux du graphe */



graphe_s *creergraphe_s (int nbnomax,int nbarmax,int ncar)

{

	graphe_s *graphe1 ;

	float **carac;

	int **couple,*licou,*liste,i,j ;



	graphe1 = (graphe_s*)mymalloc(sizeof(graphe_s));



	graphe1->ncar = ncar;

	graphe1->nbnodemax = nbnomax;

	graphe1->nbarcmax = nbarmax;

	graphe1->nel = 0;

	graphe1->nco = 0;

	graphe1->ncou = 1;

	graphe1->nbnoeuds = 0;



	liste = (int*)mycalloc(graphe1->nbnodemax + 1,sizeof(int));

	couple = allotab2i(6,graphe1->nbarcmax);

	licou = (int*)mycalloc(graphe1->nbarcmax + 1,sizeof(int));

	carac = allotab2f(graphe1->ncar,graphe1->nbnodemax) ;



	graphe1->liste = liste;

	graphe1->couple = couple;

	graphe1->licou = licou;

	graphe1->carac = carac;


	/* c

c     initialisation de carac a 0 */



	for(i = 1; i <= ncar ; i++)

	for (j = 1 ; j <= nbnomax ; j++)carac[i][j] = 0 ;



/* c

c     initialisation de couple a 0 */



	for(i = 1; i <= 6 ; i++)

	for (j = 1 ; j <= nbarmax ; j++)couple[i][j] = 0 ;





/* c

c     initialisation de licou */





	licou[nbarmax] = 0 ;



	for (i=1 ; i <= nbarmax-1 ; i++)licou[i] = i + 1;







/* c     initialisation de liste */



	for (i=1 ; i <= nbnomax ; i++)

      		liste[i] = -1 ;



	return(graphe1);



}



void crenoeud_s(graphe_s *graphe1,float *car1,int ncar)

{

	int i ;



	if((graphe1->nel) >= (graphe1->nbnodemax))exit(2);

	(graphe1->nel)++ ;

	(graphe1->nbnoeuds)++;

	(graphe1->liste)[graphe1->nel] = 0;

	for (i=1 ; i <= 1 + (int)(car1[1]*4) ; i++)

	(graphe1->carac)[i][graphe1->nel] = car1[i];



}

void crenoeud_s2(graphe_s *graphe1,float *car1,int ncar)

{

	int i ;



	if((graphe1->nel) >= (graphe1->nbnodemax))exit(2);

	(graphe1->nel)++ ;

	(graphe1->nbnoeuds)++;

	(graphe1->liste)[graphe1->nel] = 0;

	for (i=1 ; i <= ncar ; i++)

	(graphe1->carac)[i][graphe1->nel] = car1[i];



}


void hiera1_s(graphe_s *graphe1,tas_s *tas1,float seuil,int nmaxcomp)

{

float **carac,*val,*vacou ;

int **couple,*liste,*licou,*itas,*label;

int ncar,nco,nel,nt,iok;

int ico,n2,n1,nb1,nb2,i,debord;

float valout,*car1,*car2 ;


    carac = graphe1->carac ;

    val = tas1->val ;

    vacou = tas1->vacou ;

    couple = graphe1->couple ;

    liste = graphe1->liste ;

    licou = graphe1->licou ;

    itas = tas1->itas ;

    label = tas1->label ;

    ncar = graphe1->ncar ;

    nco = graphe1->nco ;

    nel = graphe1->nel ;

    nt = tas1->nt ;



/* c

c     on fusionne la tete du tas jusqu'a ce que soit le tas est vide

c     soit le couple en tete a une fonction d 'ordre trop importante */



      while (nt != 0) {

      outas_s(val,itas,&nt,&ico,&valout) ;

      tas1->nt = nt ;

      n1 = couple[1][ico] ;

      n2 = couple[2][ico] ;

      if((liste[n1] != -1)&&(liste[n2] != -1)&&(vacou[ico] == valout)){


    /*  fprintf(stderr,"\n fusion du noeud %d et du noeud %d",n1,n2);*/
    /* on verifie que le couple est toujours regroupable */
    	  nb1 = carac[1][n1];

    	  nb2 = carac[1][n2];

    	  car1 = (float*)mycalloc(ncar + 1,sizeof(float));

    	  car2 = (float*)mycalloc(ncar + 1,sizeof(float));


    	  for(i=1 ; i <= 1 + 4*nb1 ; i++)

    	   {

    	       	car1[i] = carac[i][n1] ;
    	   }

    	   for(i=1 ; i <= 1 + 4*nb2 ; i++)

    	    {

    	         car2[i] = carac[i][n2] ;

    	    }

      fta1_s(car1,car2,ncar,seuil,&iok,&valout) ;

      if(iok == 1)
      {
    	  fusion1_s(n1,n2,graphe1,tas1,seuil,nmaxcomp,&debord);
    	  if(debord == 1)
    	  {
    		  fprintf(stderr,"\n debordement du tas, creation d'un nouveau tas");
    		  ctas1_s(graphe1,tas1,seuil);
    		  fprintf(stderr,"\n fin de la creation du nouveau tas");
    	  }
      }

      nt = tas1->nt ;

      }

     }

      myfreef(car1);

      car1 = NULL ;

      myfreef(car2);

      car2 = NULL ;

}


/* si le tas deborde on met *debord a 1 */

void fusion1_s(int n1,int n2,graphe_s *graphe1,tas_s *tas1,float seuil,int nmaxcomp,int *debord)

{

	float **carac,*val,*vacou ;

	int **couple,*liste,*licou,*itas,*label,i ;

	int ncar,nco,nel,ncou,nt ;

    float *car1,*car2,*car3 ;

    int itrai,its,isits,ipits,idep,isn2,ipn2,iok,ipn1,it,isn1,iqueue,isu;

    float valin ;

    int nmaxtas,nb1,nb2,erreur,nel1,nel2 ;


    erreur = 0;
    *debord = 0;

    (graphe1->nbnoeuds)--;

    carac = graphe1->carac ;

    val = tas1->val ;

    vacou = tas1->vacou ;

    couple = graphe1->couple ;

    liste = graphe1->liste ;

    licou = graphe1->licou ;

    itas = tas1->itas ;

    label = tas1->label ;

    ncar = graphe1->ncar ;

    nco = graphe1->nco ;

    nel = graphe1->nel ;

    ncou = graphe1->ncou ;

    nt = tas1->nt ;

    nmaxtas = tas1->nmaxtas ;

    nb1 = carac[1][n1];

    nb2 = carac[1][n2];

    if((nb1+nb2) > nmaxcomp)
    {
    	fprintf(stderr,"\n nb de boules dans une chaine trop eleve !!");
    	exit(44);
    }

    car1 = (float*)mycalloc(ncar + 1,sizeof(float));

    car2 = (float*)mycalloc(ncar + 1,sizeof(float));

    car3 = (float*)mycalloc(ncar + 1,sizeof(float));



/* mise a jour du tableau des etiquettes label */



    label[n2] = n1 ;



/* mise a jour de carac */



    for(i=1 ; i <= 1 + 4*nb1 ; i++)

     {

     	car1[i] = carac[i][n1] ;
     }

    for(i=1 ; i <= 1 + 4*nb2 ; i++)

      {

            car2[i] = carac[i][n2] ;

      }


  	fcar1_s(car1,car2,car3,ncar) ;

  	if(car3[1] > nmaxcomp)
  	{
  		fprintf(stderr,"\n trop de boules dans une chaine");
  		exit(55);
  	}



  	 for (i=1 ; i <= 1 + 4*nb1 + 4*nb2 ; i++)

     {

     	carac[i][n1] = car3[i] ;

     }



/* mise a jour de couple */



      itrai = liste[n1];

trois :  if(couple[1][itrai] != n1){

            its = couple[1][itrai];}

         else{

            its = couple[2][itrai] ;

         }

         for (i=1 ; i <= 1 + carac[1][its]*4 ; i++)

     	{

     		car1[i] = carac[i][its];

    	 } ;



         if(its != n2){

/*           on regarde si its est voisin de n2 */

            idep = liste[n2] ;

cinq :       if((couple[1][idep]==its)||(couple[2][idep]==its)||(idep==0))goto six ;

              if(couple[1][idep]==n2){

                        idep = couple[3][idep];}

                  else{

                        idep = couple[5][idep] ;

                  };

            goto cinq ;

six:        /*----------------------------------------------------------*/



            if(idep!=0){

/*         si idep <> 0 alors its est aussi voisin de n2 il faut alors

           mettre a jour les parametres du couple (n1-n2,its) qui seront

           different des parametres du couple (n1,its).si its n'est pas

           voisin de n2 ces parametres ne changent pas.il faut aussi supprimer

           le couple (its,n2) de la liste "n2".

           suppression du couple n2-its de la liste "n2" et de la liste "its" */



               if(couple[1][idep]==n2){

                  isn2 = couple[3][idep] ;

                  ipn2 = couple[4][idep] ;

                  isits = couple[5][idep] ;

                  ipits = couple[6][idep];}

               else{

                  isn2 = couple[5][idep] ;

                  ipn2 = couple[6][idep] ;

                  isits = couple[3][idep] ;

                  ipits = couple[4][idep] ;

               }

/*             suppression de n2-its de la liste "n2" */

               if(ipn2==0){

/*              cas ou n2-its est la tete de la liste "n2" */

                  liste[n2] = isn2 ;

                  if(isn2!=0){

                     if(couple[1][isn2]==n2){

                        couple[4][isn2] = 0 ;

                     }else{

                        couple[6][isn2] = 0 ;

                     }

                  }

               }else{

/*             cas ou n2-its n'est pas tete de la liste "its" */

                  if(couple[1][ipn2]==n2){

                     couple[3][ipn2] = isn2 ;

                  }else{

                     couple[5][ipn2] = isn2 ;

                  }

                  if(isn2!=0){

                     if(couple[1][isn2]==n2){

                        couple[4][isn2] = ipn2 ;

                     }else{

                        couple[6][isn2] = ipn2 ;

                     }

                  }

               }

/*             suppression de n2-its de la liste "its' */

               if(ipits==0){

                  liste[its] = isits ;

                  if(isits!=0){

                     if(couple[1][isits]==its){

                        couple[4][isits] = 0 ;

                     }else{

                        couple[6][isits] = 0 ;

                     }

                  }

               }else{

                  if(couple[1][ipits]==its){

                     couple[3][ipits] = isits ;

                  }else{

                     couple[5][ipits] = isits ;

                  }

                  if(isits!=0){

                     if(couple[1][isits]==its){

                        couple[4][isits] = ipits ;

                     }else{

                        couple[6][isits] = ipits ;

                     }

                  }

               }



/*              rajout de la case idep du tableau couple dans la liste libre

c              licou en tete de la liste. */



               licou[idep] = ncou ;

               ncou = idep ;



/*             mise a jour du tas des couples */



               fta1_s(car3,car1,ncar,seuil,&iok,&valin) ;



/*             insertion du couple n1-its de nouvelle valeur valin et d 'indice

c              itrai dans le tas. */



               if(iok==1){

               	 if(nt >= nmaxtas)
               	 {
					*debord = 1;
               	 }

                 nel1 = couple[1][itrai];
                 nel2 = couple[2][itrai];

                 if(*debord == 0)intas_s(val,itas,&nt,itrai,valin) ;

                  tas1->nt = nt ;


                 vacou[itrai] = valin ;

               }else{

 /*           	 if(itrai==27156)fprintf(stderr,"\n VACOU(27156) A -1");*/

                 vacou[itrai] = -1 ;

               }

            }else{



/*           cas ou its n'est pas voisin de n2

c           dans ce cas on met a jour le tas des couples les parametres de

c           la frontiere restant inchanges.

c           on supprime le couple d'indice itrai du tas des couples pour

c           le reinserer avec    sa nouvelle valeur. */



                  fta1_s(car3,car1,ncar,seuil,&iok,&valin) ;


                  if(iok==1){

                  	if(nt >= nmaxtas)*debord=1;

                    nel1 = couple[1][itrai];
                    nel2 = couple[2][itrai];

                    if(*debord==0)intas_s(val,itas,&nt,itrai,valin) ;

                    tas1->nt = nt ;

                    vacou[itrai] = valin;}

                  else{
                    vacou[itrai] = -1 ;

                  }

            }

         }else{



/*        cas du couple n1-n2

c        on supprime n1-n2 des listes "n1" et "n2" */



            if(couple[1][itrai]==n1){

               isn1 = couple[3][itrai] ;

               ipn1 = couple[4][itrai] ;

               isn2 = couple[5][itrai] ;

               ipn2 = couple[6][itrai];}

            else{

               isn1 = couple[5][itrai] ;

               ipn1 = couple[6][itrai] ;

               isn2 = couple[3][itrai] ;

               ipn2 = couple[4][itrai] ;

            } ;



/*           suppression de n1-n2 de la liste "n1" */



            if(ipn1==0){



/*           cas ou n1-n2 est la tete de la liste "n1" */



               liste[n1] = isn1 ;

               if(isn1!=0){

                  if(couple[1][isn1]==n1){

                     couple[4][isn1] = 0;}

                  else{

                     couple[6][isn1] = 0 ;

                  }

               }

            }else{



/*           cas ou n1-n2 n'est pas la tete de la liste "n1" */



               if(couple[1][ipn1]==n1){

                  couple[3][ipn1] = isn1;}

               else{

                  couple[5][ipn1] = isn1 ;

               }

               if(isn1!=0){

                 if(couple[1][isn1]==n1){

                    couple[4][isn1] = ipn1;}

                 else{

                    couple[6][isn1] = ipn1 ;

                 }

               }

            }



/*           suppression de n1-n2 de la liste "n2" */



            if(ipn2==0){

               liste[n2]  = isn2 ;

               if(isn2!=0){

                  if(couple[1][isn2]==n2){

                     couple[4][isn2] = 0;}

                  else{

                     couple[6][isn2] = 0 ;

                  }

               }

            }else{

               if(couple[1][ipn2]==n2){

                  couple[3][ipn2] = isn2;}

               else{

                  couple[5][ipn2] = isn2 ;

               }

               if(isn2!=0){

                  if(couple[1][isn2]==n2){

                     couple[4][isn2] = ipn2;}

                  else{

                     couple[6][isn2] = ipn2 ;

                  }

               }

            }

/*           liberation de la case occuppe par n1-n2 itrai dans le tableau

c           licou representant la liste chainee des cases libres. */



            licou[itrai] = ncou ;

            ncou = itrai ;



         }



         if(liste[n2]==0)goto onze ;



/* c        si la liste "n2" est vide on ne fait plus rien */



         if(liste[n1]==0)goto quatre;



/* c        cas ou la liste "n1" ne contenait que n1-n2 */



         if(its!=n2)iqueue = itrai;



/* c        a la sortie de la boucle iqueue pointera sur le dernier

c        element de la liste "n1" */


 onze:   if(couple[1][itrai]==n1){

            itrai = couple[3][itrai];}

         else{

            itrai = couple[5][itrai];

         };



/*        determination du couple suivant de la liste "n1" a traiter */



         if(itrai==0)goto quatre;


      goto trois;

quatre:     /*----------------------------------------------------------*/;



      if(liste[n2]==0){



/* c     si la liste "n2" est vide il suffit de mettre a jour liste */



         liste[n2] = -1 ;



/* c        on rend caduc l'element "n2" */



      }else{



/* c     cas ou la liste "n2" n'est pas vide

c     on change dans la liste "n2" n2 en n1

c        et on met a jour le tas avec la nouvelle valeur associee au couple */



         itrai = liste[n2];

neuf:        if(itrai==0)goto dix;

            if(couple[1][itrai]==n2){

               couple[1][itrai] = n1;

               isu = couple[2][itrai];

               for(i=1;i<= 1 + 4*carac[1][isu] ;i++)

               car1[i] = carac[i][isu];

               fta1_s(car3,car1,ncar,seuil,&iok, &valin);



               if(iok==1){

               	  if(nt >= nmaxtas)*debord=1;

                  nel1 = couple[1][itrai];
                  nel2 = couple[2][itrai];
     /*             if((nel1==19293 && nel2==13421)||(nel1==13421 && nel2==19293))fprintf(stderr,"\n F3: inclusion du couple (%d,%d) dans couple()(%d)",nel1,nel2,itrai);*/

                  if(*debord==0)intas_s(val,itas,&nt,itrai,valin);

                  tas1->nt = nt ;

                  vacou[itrai] = valin;}

               else{

                  vacou[itrai] = -1;

               }

               itrai = couple[3][itrai];

            }else{

               couple[2][itrai] = n1;

               isu = couple[1][itrai];

               for(i=1;i<= 1 + 4*carac[1][isu] ;i++)

               car1[i] = carac[i][isu];

              fta1_s(car3,car1,ncar,seuil,&iok,&valin);




               if(iok==1){

               	  if(nt >= nmaxtas)*debord=1;

                  nel1 = couple[1][itrai];
                  nel2 = couple[2][itrai];

                  if(*debord==0)intas_s(val,itas,&nt,itrai,valin);

                  tas1->nt = nt ;

                  vacou[itrai] = valin;}

               else{

                  vacou[itrai] = -1;

               }

               itrai = couple[5][itrai];

            }

         goto neuf;

dix:       /*----------------------------------------------------------*/;

/* c        on relie la liste "n1" et la liste "n2" */

         if(liste[n1]==0){

            liste[n1] = liste[n2];

            liste[n2] = -1;}

         else{

            if(couple[1][iqueue]==n1){

               couple[3][iqueue] = liste[n2];}

            else{

               couple[5][iqueue] = liste[n2];

            }

           it = liste[n2];

           if(couple[1][it]==n1){

                couple[4][it] = iqueue;}

           else{

                couple[6][it] = iqueue;

           }

           liste[n2] = -1;

         }

      }



      /* lib�ration des allocations m�moire */



      myfreef(car1);

      car1 = NULL ;

      myfreef(car2);

      car2 = NULL ;

      myfreef(car3);

      car3 = NULL;



}





void intas_s(float *val,int *itas,int *n,int in,float valin)


{

	int i,j ;



	*n=*n+1 ;

    i=*n ;

un: if(i<= 1) goto deux ;

    j=i/2 ;

    if(valin >= val[j]) goto deux ;

    itas[i]=itas[j] ;

    val[i]=val[j] ;

    i=j ;

    goto un ;

deux: itas[i]=in ;

    val[i]=valin ;



}





/* assigne a iout l'indice du plus petit elt du tas (d'indice

itas(1))et valout la valeur correspondante */



void outas_s(float *val,int *itas,int *n,int *iout,float *valout)


{

	int in ;

	float valin ;



    valin=val[*n] ;

    in=itas[*n] ;

    swopta_s(val,itas,*n-1,in,iout,valin,valout) ;

    *n=*n-1 ;



}

void swopta_s(float *val,int *itas,int n,int in,int *iout,float valin,float *valout)


{

	int i,j,itemp1,itemp ;

	float temp,temp1 ;



if(valin <= val[1]) goto vingt ;

      i=1 ;

      itas[n+1]=in ;

      val[n+1]=valin ;

      *iout=itas[1] ;

      *valout=val[1] ;

un:     j=i+i ;

      if(j>n) goto dix ;

      itemp=itas[j] ;

      temp=val[j] ;

      itemp1=itas[j+1] ;

      temp1=val[j+1] ;

      if(temp1>=temp) goto cinq ;

      temp=temp1 ;

      itemp=itemp1 ;

      j=j+1 ;

cinq: if(temp>=valin) goto dix ;

      itas[i]=itemp ;

      val[i]=temp ;

      i=j ;

      goto un ;

dix:    itas[i]=in ;

      val[i]=valin ;

      return ;

vingt:*iout=in ;

      *valout=valin ;

 }




void fcar1_s (float *car1,float *car2,float *car3,int ncar)



{

int i,n1,n2,n3;
float x,y,z,r;

	/* GERER LES EXTREMITES */
	/* seules deux extremites sont voisines */

	n1 = car1[1];
	n2 = car2[1];


	car3[1] = n1 + n2;
	n3 = car3[1];

	/* cas ou la premiere extremite de la premiere chaine est voisine de la premiere extremite
	 * de la deuxieme chaine */

	if(connexb(1,1,car1,car2,1)==1)
	{
		/* remplissage de la premi�re partie de car3 avec car2 � l'envers*/
		/* fprintf(stderr,"\n ENTREE 1");*/
		for (i=1 ; i <= n2; i++)

			{
				x = car2[2+4*(i-1)];
				y = car2[3+4*(i-1)];
				z = car2[4+4*(i-1)];
				r = car2[5+4*(i-1)];
				car3[2+4*(n2-i)] = x;
				car3[3+4*(n2-i)] = y;
				car3[4+4*(n2-i)] = z;
				car3[5+4*(n2-i)] = r;
			}

			/* remplissage de la deuxi�me partie de car3 avec car1 */
		for (i=2 ; i <= 4*n1 + 1; i++)

			{

				car3[i + 4*n2] = car1[i];

			}
	}
	else if(connexb(1,car2[1],car1,car2,1) == 1)
	{
		/* remplissage de la deuxieme partie de car3 avec car1 */
		/*fprintf(stderr,"\n ENTREE 2");*/

		for (i=2+4*n2 ; i <= 2+4*n2+4*n1; i++)

		{

			car3[i] = car1[i-4*n2];

		}

		/* remplissage de la premi�re partie de car3 avec car2 */
		for (i=2 ; i <= 4*n2 + 1; i++)

		{

			car3[i] = car2[i];

		}
	}
	else if(connexb(car1[1],1,car1,car2,1) == 1)
	{
		/* remplissage de la premiere partie de car3 avec car1 */
		/* fprintf(stderr,"\n ENTREE 3");*/
		for (i=2 ; i <= 4*n1 + 1; i++)

		{

			car3[i] = car1[i];

		}
		/* remplissage de la deuxieme partie de car3 avec car2 */
		for (i=2+4*n1 ; i <= 2+4*n2+4*n1; i++)

		{

			car3[i] = car2[i-4*n1];

		}

	}
	else

	{
		/* fprintf(stderr,"\n ENTREE 4");*/
		if(connexb(car1[1],car2[1],car1,car2,1) == 1)
		{
			/* remplissage de la premiere partie de car3 avec car1 */
			for (i=2 ; i <= 4*n1 + 1; i++)
			{
				car3[i] = car1[i];
			}

			/* remplissage de la deuxi�me partie de car3 avec car2 � l'envers */
			for (i=1 ; i <= n2; i++)

			{
				x = car2[2+4*(i-1)];
				y = car2[3+4*(i-1)];
				z = car2[4+4*(i-1)];
				r = car2[5+4*(i-1)];
				car3[2+4*(n2-i)+4*n1] = x;
				car3[3+4*(n2-i)+4*n1] = y;
				car3[4+4*(n2-i)+4*n1] = z;
				car3[5+4*(n2-i)+4*n1] = r;
			}
		}
		else
		{
			fprintf(stderr,"\n Erreur dans fcar");
			ecrtab(car1);
			ecrtab(car2);
			exit(66);
		}

	}
}

/* ecriture des valeurs d'un tableau float de n elements significatifs a partir du rang 1*/
void ecrtab(float *tab)
{
	int i,n;

	n = 4*tab[1] + 1;

	fprintf(stderr,"\n");
	for(i=1;i<=n;i++)
	{
		fprintf(stderr,"tab(%d)=%f",i,tab[i]);
	}
	fprintf(stderr,"\n");
}

void incou2_s(graphe_s *graphe1,int nel1,int nel2)

{



	int **couple,*liste,ncou,*licou,itete,num;



    liste = graphe1->liste ;

    couple = graphe1->couple ;

    ncou = graphe1->ncou ;

    licou = graphe1->licou ;



	if(ncou == 0)exit(3) ;

	if((graphe1->nco) >= graphe1->nbarcmax)exit(4);

    (graphe1->nco)++;

	couple[1][ncou] = nel1 ;

    couple[2][ncou] = nel2 ;

    if(liste[nel1] == 0){

         liste[nel1] = ncou ;

         couple[3][ncou] = 0 ;

         couple[4][ncou] = 0 ;

/* c        le pointeur vide ("nil") est l 'entier 0 */

    }else{

         itete = liste[nel1] ;

         couple[3][ncou] = itete ;

         couple[4][ncou] = 0 ;

         if(couple[1][itete]==nel1){

            num = 4 ;

         }else{

            num = 6 ;

         }



/* c     les pointeurs correspondants a la liste "nel1" se trouvent en

c        couple[num-1][itete] et couple[num][itete] */



         couple[num][itete] = ncou ;

         liste[nel1] = ncou ;



/* c        le couple nel1-nel2 a ete rajoute en tete de la liste nel1 */



      }

      if(liste[nel2]==0){

         liste[nel2] = ncou ;

         couple[5][ncou] = 0 ;

         couple[6][ncou] = 0 ;

      }else{

         itete = liste[nel2] ;

         couple[5][ncou] = itete ;

         couple[6][ncou] = 0 ;

         if(couple[1][itete]==nel2){

            num = 4 ;

         }else{

            num = 6 ;

         }

         couple[num][itete] = ncou ;

         liste[nel2] = ncou ;

      }



/* c      mise a jour de la liste libre de couple licou */


     graphe1->ncou = licou[ncou];

}

/*visualisation de boules contenues dans un fichier */

void visuboul2_s(char *boules,char *imaboul,char *entete,int dimx,int dimy,int dimz)

{



	FILE *ptboules;

	int nboules,i,npoints;

	float x0,y0,z0,r,a,u,v,w;

	image *ptimage;

	boule *ptboule;



	ptboules = fopen(boules,"r");

	ptimage = creimage(dimx,dimy,dimz,1);

	ptboule = (boule*)mymalloc(sizeof(boule));

	fprintf(stderr,"\n entree dans fscanf");

	fscanf(ptboules,"%d",&nboules);

	fprintf(stderr,"\n sortie de fscanf");

	fprintf(stderr,"\n nboules = %d",nboules);



	for(i=0;i<nboules;i++)

	{

		fscanf(ptboules,"%f %f %f %f",&x0,&y0,&z0,&r);

		ptboule->xc = x0;

		ptboule->yc = y0;

		ptboule->zc = z0;

		ptboule->rayon = r;

		npoints = boulimag(ptboule,ptimage,0);

	}

	npoints = ecrimage(entete,imaboul,ptimage);

	fprintf(stderr,"\n nb total de points ecrits = %d",npoints);

}

void testvisboul2_s(void)

{



	char boules[50],imaboul[50],entete[50];

	int dimx,dimy,dimz;



	printf("nom du fichier des boules (nb de boules,xc,yc,zc,r) : ");

	scanf("%s",boules);

	printf("\n nom du fichier entete de l'image de sortie en format opendx (image d'octets): ");

	scanf("%s",entete);

	printf("\n %s",entete);

	printf("\n nom du fichier image de sortie en format opendx (image d'octets): ");

	scanf("%s",imaboul);

	printf("\n %s",imaboul);

	printf("\n dimx,dimy,dimz : ");

	scanf("%d %d %d",&dimx,&dimy,&dimz);

	visuboul2_s(boules,imaboul,entete,dimx,dimy,dimz);

}





/* recherche de l'arc (n1,n2) dans le graphe graphe1 ; sert a la creation

du graphe ; entree : n1,n2,le graphe ; sortie : place ("place") de l'arc (n1,n2)

dans le tableau couple et 0 si cet arc n'existe pas */



int itrouv_s(int nel1,int nel2,graphe_s *graphe1)



{

	int **couple,*liste ;

	int nco,nel,iitrouv,itra;



    couple = graphe1->couple ;

    liste = graphe1->liste ;

    nco = graphe1->nco ;

    nel = graphe1->nel ;



      if((liste[nel1]==0)||(liste[nel2]==0) || (liste[nel1]== -1)

      || (liste[nel2] == -1)){

         iitrouv = 0;

      }else{

         iitrouv = 0;

         itra = liste[nel1];

un:        if(itra==0)goto deux;
/*		   fprintf(stderr,"\n itra = %d",itra);*/

            if(couple[1][itra]==nel1){

               if(couple[2][itra]==nel2){

               iitrouv = itra;

                  goto deux;

               }else{

                  itra = couple[3][itra];

               }

            }else{

               if((couple[1][itra]==nel2)&&(couple[2][itra]==nel1))

            	{

                  iitrouv = itra;

                  goto deux;}

               else{

                  itra = couple[5][itra];

               }

            }

            goto un;

deux:       /*----------------------------*/ ;

      }

      return(iitrouv);
}

graphe_s *cregraphe_s4(char *cyl,char *arcs)

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

		fscanf(ptcyl,"%f %f %f %f %f %f %f %f",&x1,&y1,&z1,&r,&t1,&t2,&t3,&t4);

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

	return(graphe1);

}





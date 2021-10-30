/*
 * creation-graphe3.c
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







void fta3(double *car1,double *car2,int ncar,

float seuil,int *regroup,double *cout)

{

	double n,r,rc,sx,sxx,sy,syy,sz,szz,sxy,sxz,syz,erreur;

	double sx1,sxx1,sy1,syy1,sz1,szz1,sxy1,sxz1,syz1,n1,r1,rc1;

	double sx2,sxx2,sy2,syy2,sz2,szz2,sxy2,sxz2,syz2,n2,r2,rc2;

	int res;

	double xg,yg,zg,dx,dy,dz;

	double xg1,yg1,zg1,dx1,dy1,dz1;

	double xg2,yg2,zg2,dx2,dy2,dz2;

	double c1,c2,c,erreur1,erreur2,pie,s1,s2,s,d;



	n = car1[1] + car2[1];

	r = car1[2] + car2[2];

	rc = car1[3] + car2[3];

	c = rc - ((r*r)/n);



	if(c > seuil)

	{

		*regroup = 0;

	}

	else

	{

		*regroup = 1;

		*cout = c;

	}



}





void fusion3(int n1,int n2,graphe *graphe1,tas *tas1,float seuil)

{

	double **carac,*val,*vacou ;

	int **couple,*liste,*licou,*itas,*label,i ;

	int ncar,nco,nel,ncou,nt ;

    double *car1,*car2,*car3 ;

    int itrai,its,isits,ipits,idep,isn2,ipn2,iok,ipn1,it,isn1,iqueue,isu;

    double valin ;

    int nmaxtas ;



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

    car1 = (double*)mycalloc(ncar + 1,sizeof(double));

    car2 = (double*)mycalloc(ncar + 1,sizeof(double));

    car3 = (double*)mycalloc(ncar +1,sizeof(double));



/* mise a jour du tableau des etiquettes label */



    label[n2] = n1 ;



/* mise a jour de carac */



    for (i=1 ; i <= ncar ; i++)

     {

     	car1[i] = carac[i][n1] ;

        car2[i] = carac[i][n2] ;

     } ;

  	fcar1(car1,car2,car3,ncar) ;



  	 for (i=1 ; i <= ncar ; i++)

     {

     	carac[i][n1] = car3[i] ;

     } ;



/* mise a jour de couple */



      itrai = liste[n1];

trois :  if(couple[1][itrai] != n1){

            its = couple[1][itrai];}

         else{

            its = couple[2][itrai] ;

         }

         for (i=1 ; i <= ncar ; i++)

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



               fta3(car3,car1,ncar,seuil,&iok,&valin) ;



/*             insertion du couple n1-its de nouvelle valeur valin et d 'indice

c              itrai dans le tas. */



               if(iok==1){

               	 if(nt >= nmaxtas)exit(1);

                 intas(val,itas,&nt,itrai,valin) ;

                 tas1->nt = nt ;

                 vacou[itrai] = valin ;

               }else{

                 vacou[itrai] = -1 ;

               }

            }else{



/*           cas ou its n'est pas voisin de n2

c           dans ce cas on met a jour le tas des couples les parametres de

c           la frontiere restant inchanges.

c           on supprime le couple d'indice itrai du tas des couples pour

c           le reinserer avec    sa nouvelle valeur. */



                  fta3(car3,car1,ncar,seuil,&iok,&valin) ;

                  if(iok==1){

                  	if(nt >= nmaxtas)exit(1);

                    intas(val,itas,&nt,itrai,valin) ;

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

         if(liste[n2]==0)goto quatre ;



/* c        si la liste "n2" est vide on ne fait plus rien */



         if(liste[n1]==0)goto quatre;



/* c        cas ou la liste "n1" ne contenait que n1-n2 */



         if(its!=n2)iqueue = itrai;



/* c        a la sortie de la boucle iqueue pointera sur le dernier

c        element de la liste "n1" */



         if(couple[1][itrai]==n1){

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

               for(i=1;i<=ncar;i++)

               car1[i] = carac[i][isu];

               fta3(car3,car1,ncar,seuil,&iok, &valin);

               if(iok==1){

               	  if(nt >= nmaxtas)exit(1);

                  intas(val,itas,&nt,itrai,valin);

                  tas1->nt = nt ;

                  vacou[itrai] = valin;}

               else{

                  vacou[itrai] = -1;

               }

               itrai = couple[3][itrai];

            }else{

               couple[2][itrai] = n1;

               isu = couple[1][itrai];

               for(i=1;i<=ncar;i++)

               car1[i] = carac[i][isu];

               fta3(car3,car1,ncar,seuil,&iok,&valin);

               if(iok==1){

               	  if(nt >= nmaxtas)exit(1);

                  intas(val,itas,&nt,itrai,valin);

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



      free(car1);

      car1 = NULL ;

      free(car2);

      car2 = NULL ;

      free(car3);

      car3 = NULL;

}









void ctas3(graphe *graphe1,tas *tas1,float seuil)

{



double **carac,*val,*vacou,*car1,*car2 ;

int **couple,*liste,*itas,i ;

int ncar,nco,nel,nt ;

double valin ;

int iok,j,n1,n2;



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



    car1 = (double*)mycalloc(ncar + 1,sizeof(double));

    car2 = (double*)mycalloc(ncar + 1,sizeof(double));



    i = 1 ;

un: if(couple[1][i] == 0)goto deux ;

         n1 = couple[1][i] ;

         n2 = couple[2][i] ;  ;

         if((liste[n1] == -1) || (liste[n2] == -1)){

            i = i + 1 ;

            goto un ;

         }

         for (j =1 ; j <= ncar ; j++)

            car1[j] = carac[j][n1];



         for (j =1 ; j <= ncar ; j++)

            car2[j] = carac[j][n2];



      	fta3(car1,car2,ncar,seuil,&iok,&valin) ;

      if(iok == 1){

      	 if(nt >= tas1->nmaxtas)exit(1);

         intas(val,itas,&nt,i,valin) ;

         tas1->nt = nt ;

         vacou[i] = valin ;

      }else{

         vacou[i] = -1 ;

      }

         i = i + 1 ;

      goto un ;

deux:   /*---------------------------------------*/ ;



}





void hiera3(graphe *graphe1,tas *tas1,double seuil)

{



double **carac,*val,*vacou ;

int **couple,*liste,*licou,*itas,*label;

int ncar,nco,nel,nt ;

int ico,n2,n1;

double valout ;





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

      outas(val,itas,&nt,&ico,&valout) ;

      tas1->nt = nt ;

      n1 = couple[1][ico] ;

      n2 = couple[2][ico] ;

      if((liste[n1] != -1)&&(liste[n2] != -1)&&(vacou[ico] == valout)){

      fusion3(n1,n2,graphe1,tas1,seuil);

      nt = tas1->nt ;

      }

     }



}





/* creation d'un graphe d'adjacence des boules et regroupement de ces boules

en fonction des tailles des rayons pour obtenir des ensembles de boules

connectees de rayons homogenes */



void testcgraf3(void)

{

	char boules[50],ficgraf[50],cylindres[50];

	graphe *graf;

	float majrap,seuil;

	tas *tas1;

	int inser,it,nel,i,j;



	printf("\n nom du fichier de boules en entree (x0,y0,z0,r,a): ");

	scanf("%s",boules);

	printf("\n nom du fichier de groupes de boules en sortie : ");

	scanf("%s",cylindres);

	printf("\n seuil pour la connexite (entre 1 et 1.5 -1.2 raisonnable-)");

	scanf("%f",&majrap);

	graf = cregraphe(boules,majrap,&tas1);

	printf("\n nb de noeuds = %d",graf->nbnoeuds);

	printf("\n si vous voulez lancer une fusion hierarchique taper 1 sinon 0 : ");

	scanf("%d",&inser);



	while(inser == 1)

	{

		printf("\n regroupement en groupes de boules connectees de rayons homogenes ");

		printf("valeur maximale pour la somme du du carre des ecarts a la moyenne =  ");

		scanf("%f",&seuil);

		ctas3(graf,tas1,seuil);

		printf("\n nb d'elements du tas = %d",tas1->nt);

		printf("\n fin de la creation du tas ");

		hiera3(graf,tas1,seuil);

		printf("\n fin des regroupements");

		printf("\n nb de noeuds = %d",graf->nbnoeuds);

		printf("\n si vous voulez lancer une fusion hierarchique taper 1 sinon 0 : ");

		scanf("%d",&inser);

		if(inser == 1)graf = cregraphe(boules,majrap,&tas1);

	}

	stocker(graf,tas1,boules,cylindres);

}



/* stockage du resultat de la segmentation dans un fichier au format suivant :

nb de regions

nb de boules de la region 1 rayon moyen de la region 1 ecart type des rayons

x0 y0 z0 r pour la suite des boules de la region 1

......

*/

void stocker(graphe *graf,tas *tas1,char *boulin,char *segment)

{

	int nel,i,j,nbnoeuds,nboules,nbi,n,*itas,nt,iout;

	FILE *ptres,*ptboulin;

	boule **tboulin;

	float a,x0,y0,z0,r,moy,ect;

	double sr, sr2,nf,erreur,*val,valout;



	nel = graf->nel ;

	nbnoeuds = graf->nbnoeuds;

	ptres = fopen(segment,"w");

	ptboulin = fopen(boulin,"r");



	fscanf(ptboulin,"%d",&nbi);

	tboulin = (boule**)mycalloc(nbi,sizeof(boule*));



	for(i=0;i<nbi;i++)

	tboulin[i] = (boule*)mymalloc(sizeof(boule));



	/* remplissage du tableau des boules initiales */

	for(i=0;i<nbi;i++)

	{

		fscanf(ptboulin,"%f %f %f %f %f",&x0,&y0,&z0,&r,&a);

		tboulin[i]->xc = x0;

		tboulin[i]->yc = y0;

		tboulin[i]->zc = z0;

		tboulin[i]->rayon = r;

	}

	fclose(ptboulin);



	/* ecriture du nb de regions au debut du fichier */

	fprintf(ptres,"%d",nbnoeuds);



	 /* traitement du tableau des etiquettes */

	for(i=1;i<=nel;i++)

	{

		j = i;

		while(tas1->label[j] != j)

		j = tas1->label[j];

		tas1->label[i] = j;

	}



	/* tri des noeuds selon la moyenne des rayons des boules

	correspondantes par le heapsort algorithm */

	val = tas1->val;

	itas = tas1->itas;

	j = 1;

	for(i=1;i<nel;i++)

	{

		if(graf->liste[i] != -1)

		{

			itas[j] = i;

			val[j] = graf->carac[2][i]/graf->carac[1][i];

			j++;

		}

	}

	cretas(val,itas,nbnoeuds);

	nt = nbnoeuds;

	/* traitement de l'ensemble des noeuds */

	for(j=1;j<=nbnoeuds;j++)

	{

		outas(val,itas,&nt,&iout,&valout);

		n = (int)graf->carac[1][iout];

		nf = graf->carac[1][iout];

		sr = graf->carac[2][iout];

		sr2 = graf->carac[3][iout];



		/* ecriture du nb de boules de la region dans le fichier */

		fprintf(ptres,"\n");

		fprintf(ptres,"\n %d",n);



		/* calcul de la moyenne et de la somme du carre des erreurs */

		moy = sr/nf;

		erreur = sr2 - (sr*sr)/nf;



		/* ecriture de la moyenne et de l'erreur quadratique totale dans le fichier */

		fprintf(ptres," %f %f ",moy,erreur);



		for(j=1;j<=nel;j++)

		if(tas1->label[j] == i)

		/* ecriture de la boule j dans le fichier */

		fprintf(ptres,"\n %f %f %f %f",tboulin[j-1]->xc,tboulin[j-1]->yc,tboulin[j-1]->zc,tboulin[j-1]->rayon);

	}

	fclose(ptres);

}



/* transforme un fichier de boules du format :

- nb de boules

- x0 y0 z0 r a u v w

en un fichier au format :

- nb de boules

- x0 y0 z0 r a

*/

void transformer(char *entree,char *sortie)

{

	int nboules,j;

	FILE *ptentree,*ptsortie;

	float x0,y0,z0,r,a,u,v,w;



	ptentree = fopen(entree,"r");

	ptsortie = fopen(sortie,"w");



	fscanf(ptentree,"%d",&nboules);

	fprintf(ptsortie,"%d",nboules);

	j= 0;

	while(j<nboules)

	{

		fscanf(ptentree,"%f %f %f %f %f %f %f %f",&x0,&y0,&z0,&r,&a,&u,&v,&w);

		fprintf(ptsortie,"\n %f %f %f %f %f",x0,y0,z0,r,a);

		j++;

	}

	fclose(ptsortie);

	fclose(ptentree);

}

void transformer2(char *entree,char *sortie)

{

	int nboules,j;

	FILE *ptentree,*ptsortie;

	float x0,y0,z0,r,a,u,v,w;



	ptentree = fopen(entree,"r");

	ptsortie = fopen(sortie,"w");



	fscanf(ptentree,"%d",&nboules);

	fprintf(ptsortie,"%d",nboules);

	j= 0;

	while(j<nboules)

	{

		fscanf(ptentree,"%f %f %f %f",&x0,&y0,&z0,&r);

		fprintf(ptsortie,"\n %f %f %f %f %f %f %f %f",x0,y0,z0,r,0.0,0.0,0.0,0.0);

		j++;

	}

	fclose(ptsortie);

	fclose(ptentree);

}



void testtrans(void)

{

	char fic1[50],fic2[50];



	fprintf(stderr,"\n fichier entree fichier sortie :");

	scanf("%s %s",fic1,fic2);

	transformer(fic1,fic2);

}

void testtrans2(void)

{

	char fic1[50],fic2[50];



	fprintf(stderr,"\n fichier entree fichier sortie :");

	scanf("%s %s",fic1,fic2);

	transformer2(fic1,fic2);

}




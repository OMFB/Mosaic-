/*
 * creation-graphe2.c
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





/* fcar est la fonction de mise � jour des attributs-noeuds */

/* entree : car1 et car2 qui sont les tableaux de caracteristiques correspondant

aux deux noeuds et ncar la taille de ces tableaux -nb de caracteristiques -

sortie : car3 qui est le tableau de caracteristiques du noeud obtenu par fusion de deux noeuds ayant

car1 et car2 comme caracteristiques */

void fcar1_t (double *car1,double *car2,double *car3,int ncar)



{

int i ;



	for (i=1 ; i < ncar+1 ; i++)

	{

		car3[i] = car1[i] + car2[i];



	}

}









/* estimation du cout d'un regroupement, entree : car1,car2,cou soit trois tableaux contenant

les deux vecteurs attribut-noeud

a un couple de noeuds connexes (arc) ; sortie : regroup entier a 1 si les

deux noeuds verifient le predicat de regroupement et a 0 sinon ; cout soit le cout

du regroupement qui est un reel positif */



void fta1_t(double *car1,double *car2,int ncar,float lambda,

int *regroup,double *cout)

{

	double n,r,rc,sx,sxx,sy,syy,sz,szz,sxy,sxz,syz,erreur;

	double sx1,sxx1,sy1,syy1,sz1,szz1,sxy1,sxz1,syz1,n1,r1,rc1;

	double sx2,sxx2,sy2,syy2,sz2,szz2,sxy2,sxz2,syz2,n2,r2,rc2;

	int res;

	double xg,yg,zg,dx,dy,dz;

	double xg1,yg1,zg1,dx1,dy1,dz1;

	double xg2,yg2,zg2,dx2,dy2,dz2;

	double c1,c2,c,erreur1,erreur2,pie,s1,s2,s,d;



	pie = 3.14116;



	n = car1[1] + car2[1];

	r = car1[2] + car2[2];

	rc = car1[3] + car2[3];

	sx = car1[4] + car2[4];

	sxx = car1[5] + car2[5];

	sy = car1[6] + car2[6];

	syy = car1[7] + car2[7];

	sz = car1[8] + car2[8];

	szz = car1[9] + car2[9];

	sxy = car1[10] + car2[10];

	sxz = car1[11] + car2[11];

	syz = car1[12] + car2[12];

	res = rapprox(sx,sy,sz,sxx,syy,szz,sxy,sxz,syz,n,&xg,

	&yg,&zg,&dx,&dy,&dz,&erreur);

	c = rc - ((r*r)/n) + erreur;

	s = (n/2)*2*pie*(r/n) + 2*pie*(r/n)*(r/n);



	n1 = car1[1];

	r1 = car1[2];

	rc1 = car1[3];

	sx1 = car1[4];

	sxx1 = car1[5];

	sy1 = car1[6];

	syy1 = car1[7];

	sz1 = car1[8];

	szz1 = car1[9];

	sxy1 = car1[10];

	sxz1 = car1[11];

	syz1 = car1[12];

	res = rapprox(sx1,sy1,sz1,sxx1,syy1,szz1,sxy1,sxz1,syz1,n1,&xg1,

	&yg1,&zg1,&dx1,&dy1,&dz1,&erreur1);

	c1 = rc1 - ((r1*r1)/n1) + erreur1;

	s1 = (n1/2)*2*pie*(r1/n1) + 2*pie*(r1/n1)*(r1/n1);

	n2 = car2[1];

	r2 = car2[2];

	rc2 = car2[3];

	sx2 = car2[4];

	sxx2 = car2[5];

	sy2 = car2[6];

	syy2 = car2[7];

	sz2 = car2[8];

	szz2 = car2[9];

	sxy2 = car2[10];

	sxz2 = car2[11];

	syz2 = car2[12];

	res = rapprox(sx2,sy2,sz2,sxx2,syy2,szz2,sxy2,sxz2,syz2,n2,&xg2,

	&yg2,&zg2,&dx2,&dy2,&dz2,&erreur2);

	c2 = rc2 - ((r2*r2)/n2) + erreur2;

	s2 = (n2/2)*2*pie*(r2/n2) + 2*pie*(r2/n2)*(r2/n2);

	d = c - c1 - c2 + lambda*(s - s1 -s2);

	if(d < 0)

	{

		*regroup=1;

		*cout = 1.0/(-d);

	}

	else *regroup=0;



}





void fta2_t(double *car1,double *car2,int ncar,float lambda,int *regroup,double *cout)

{

	double n,r,rc,sx,sxx,sy,syy,sz,szz,sxy,sxz,syz,erreur;

	double c,pie,ect1,ect2;



	pie = 3.14116;



	n = car1[1] + car2[1];

	r = car1[2] + car2[2];

	rc = car1[3] + car2[3];

	sx = car1[4] + car2[4];

	sxx = car1[5] + car2[5];

	sy = car1[6] + car2[6];

	syy = car1[7] + car2[7];

	sz = car1[8] + car2[8];

	szz = car1[9] + car2[9];

	sxy = car1[10] + car2[10];

	sxz = car1[11] + car2[11];

	syz = car1[12] + car2[12];

	erreur = erapprox(sx,sy,sz,sxx,syy,szz,sxy,sxz,syz,n);

	ect1 = sqrt((rc - ((r*r)/n))/n);

	ect2 = sqrt(erreur/n);

	c = ect1 + ect2;

	/* printf("c=%f c1=%f c2=%f s=%f s1=%f s2=%f d=%f lambda=%f",c,c1,c2,s,s1,s2,d,lambda);*/

	if(lambda > 0)

		if(c < lambda)

		{

			*regroup=1;

			*cout = c;

		}

		else *regroup=0;

	else

		if(ect1 < -lambda && ect2 < -lambda)

		{

			*regroup = 1;

			*cout = c;

		}

		else *regroup = 0;



}





/* fusion effectue la mise a jour du graphe et du tas apres le regroupement

de deux noeuds ; fusion prend en entree les numeros des deux noeuds a

fusionner n1 et n2 ainsi que le graphe et le tas ;

fusion met a jour le graphe et le tas, les fonctions de mise a jour des attributs-noeuds

et de calcul du predicat et du cout du regroupement sont fcar1_t et fta1_t */



void fusion1_t(int n1,int n2,graphe *graphe1,tas *tas1,float seuil)

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

  	fcar1_t(car1,car2,car3,ncar) ;



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



               fta1_t(car3,car1,ncar,seuil,&iok,&valin) ;



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



                  fta1_t(car3,car1,ncar,seuil,&iok,&valin) ;

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

               fta1_t(car3,car1,ncar,seuil,&iok, &valin);

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

               fta1_t(car3,car1,ncar,seuil,&iok,&valin);

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





/* fusion effectue la mise a jour du graphe et du tas apres le regroupement

de deux noeuds ; fusion prend en entree les numeros des deux noeuds a

fusionner n1 et n2 ainsi que le graphe et le tas ;

fusion met a jour le graphe et le tas, les fonctions de mise a jour des attributs-noeuds

et de calcul du predicat et du cout du regroupement sont fcar1_t et fta1_t */



void fusion2_t(int n1,int n2,graphe *graphe1,tas *tas1,float seuil)

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

  	fcar1_t(car1,car2,car3,ncar) ;



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



               fta2_t(car3,car1,ncar,seuil,&iok,&valin) ;



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



                  fta2_t(car3,car1,ncar,seuil,&iok,&valin) ;

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

               fta2_t(car3,car1,ncar,seuil,&iok, &valin);

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

               fta2_t(car3,car1,ncar,seuil,&iok,&valin);

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



/* recherche de l'arc (n1,n2) dans le graphe graphe1 ; sert a la creation

du graphe ; entree : n1,n2,le graphe ; sortie : place ("place") de l'arc (n1,n2)

dans le tableau couple et 0 si cet arc n'existe pas */



int itrouv_t(int nel1,int nel2,graphe *graphe1)



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





/* initialisation du tas des couples de noeuds susceptibles d'etre regroupes

on utilise la fonction fta1_t */

/* c

c cette routine cree le tas des couples a partir des tableaux carac et

c couple dans le cas ou des cases de couple ne seraient plus

c d'actualite le couple est quand meme insere dans le tas .

c on suppose que le tableau couple a ete initialise a 0 et qu'il

a ete rempli sequentiellement sans trous , les fonctions de mise a jour des attributs-noeuds

et de calcul du predicat et du cout du regroupement sont fcar1_t et fta1_t

c */



void ctas2_t(graphe *graphe1,tas *tas1,float seuil)

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



      	fta2_t(car1,car2,ncar,seuil,&iok,&valin) ;

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



void ctas1_t(graphe *graphe1,tas *tas1,float seuil)

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



      	fta1_t(car1,car2,ncar,seuil,&iok,&valin) ;

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





/* regroupement hierarchique des paires de regions adjacentes, la hierarchie

de fusion est defini par la fonction fta1_t, la mise a jour des attributs des  noeuds

et des arcs par fcar1_t ; le tas est initialise par

la fonction ctas ; le graphe est mis a jour apres la suite de fusions

definie par fta1_t par appels successifs de la fonction fusion ; le couple de noeuds

a regrouper est determine en depilant le tas ; le tableau vacou permet

d'obtenir la bonne valeur des couts de fusion ; les fonctions de mise a jour des attributs-noeuds

et de calcul du predicat et du cout du regroupement sont fcar1_t et fta1  */



void hiera1_t(graphe *graphe1,tas *tas1,double seuil)

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

      fusion1_t(n1,n2,graphe1,tas1,seuil);

      nt = tas1->nt ;

      }

     }



}







void hiera2_t(graphe *graphe1,tas *tas1,double seuil)

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

      fusion2_t(n1,n2,graphe1,tas1,seuil);

      nt = tas1->nt ;

      }

     }



}



/* insertion d'arcs dans le graphe lors de sa creation

entree : graphe, arc a inserer ; sortie : graphe mis a jour apres

l'insertion de l'arc ; on suppose que l'arc n'a pas deja ete inserer

dans le graphe */



void incou2_t(graphe *graphe1,int nel1,int nel2)



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





/* initialisation du graphe */





/* cette proc�dure cree un graphe value initialement vide

entree :

nbnomax : nombre maximun de noeuds

nbarmax : nombre maximum d'arcs

ncarac : nombre de caracteristiques noeuds



sortie : pointeur sur la structure de graphe cree et initialise

les tableux du graphe */



graphe *creergraphe_t (int nbnomax,int nbarmax,int ncar)



{

	graphe *graphe1 ;

	double **carac;

	int **couple,*licou,*liste,i,j ;



	graphe1 = (graphe*)mymalloc(sizeof(graphe));



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

	carac = allotab2d(graphe1->ncar,graphe1->nbnodemax) ;



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

      		liste[i] = 0 ;







	return(graphe1);



}







/* cette procedure cree un tas initialement vide

entree :

ntasmax : nombre maximum d'elements du tas



sortie : pointeur sur la structure de tas cree */



tas *creertas_t (int ntasmax,int nbarmax,int nbnodemax)

{

	tas *tas1 ;

	int *itas,*label,i ;

	double *val,*vacou;



	tas1 = (tas*)mymalloc(sizeof(tas));

	val = (double*)mycalloc(ntasmax + 1,sizeof(double));

	itas = (int*)mycalloc(ntasmax + 1,sizeof(int));

	label = (int*)mycalloc(nbnodemax + 1,sizeof(int));

	vacou = (double*)mycalloc(nbarmax +1,sizeof(double));



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





/* creation d'un nouveau noeud du graphe avec ses attributs

entree : le graphe, les caracteristiques du nouveau noeud,

le nombre de caracteristiques noeuds

sortie : le graphe mis a jour apres insertion du nouveau noeud

*/



void crenoeud_t(graphe *graphe1,double *car1,int ncar)

{

	int i ;



	if((graphe1->nel) >= (graphe1->nbnodemax))exit(2);

	(graphe1->nel)++ ;

	(graphe1->nbnoeuds)++;

	(graphe1->liste)[graphe1->nel] = 0;

	for (i=1 ; i <= ncar ; i++)

	(graphe1->carac)[i][graphe1->nel] = car1[i];



}



void ecrgraphe_t(graphe *graphe1)

{



	double **carac;

	int **couple,*licou,*liste,i,j,ncar,nbnomax,nbarmax,nco,ncou,nel,itrai;

	int ivois ;



	ncar = graphe1->ncar;

	nbnomax = graphe1->nbnodemax;

	nbarmax = graphe1->nbarcmax;

	nel = graphe1->nel ;

	nco = graphe1->nco ;

	ncou = graphe1->ncou ;



	liste = graphe1->liste ;

	couple = graphe1->couple;

	licou = graphe1->licou;

	carac = graphe1->carac;



	printf("Ecriture du graphe \n");



	printf("nb max noeuds = %d - nb max arcs = %d \n",nbnomax,nbarmax);

	printf("nb de noeuds = %d - nb d'arcs = %d \n",nel,nco);

	printf("dimension du vecteur caracteristiques-noeuds = %d \n",ncar);





	/* c     impression de liste */



	for (i=1 ; i <= nel ; i++)

      		printf("liste[%d]=%d \n",i,liste[i]) ;



	/* c

c     impression de carac */



	printf("tableau carac \n");

	for(j = 1; j <= nel ; j++)

	{

		for (i=1;i <= ncar;i++)printf("carac[%d][%d]=%lf-",i,j,carac[i][j]) ;

		printf("\n");

	}



/* c

c     impression de couple */



	printf("tableau couple \n");

	for (j = 1 ; j <= nco ; j++)

	{

		for(i = 1; i <= 6 ; i++) printf("couple[%d][%d]=%d-",i,j,couple[i][j]);

		printf("\n");

	}





/* c

c     impression de ncou et du tableau licou */

	printf("tete de la liste libre de couple = %d \n",ncou);



	for (i=1 ; i <= nco ; i++)printf("licou[%d]=%d \n",i,licou[i]);





/* impression des noeuds actifs et de leurs voisins */



	for (i=1 ; i <= nel ; i++)

	{

		if((liste[i] != -1))

		{

			printf("\n noeud numero:%d",i);

			itrai = liste[i];

			printf(" \n voisins : ");

			while (itrai != 0)

			{

				ivois = ((couple[1][itrai] == i) ? 1 : 2);



				if(ivois == 1)

				{

					printf("- %d",couple[2][itrai]);

					itrai = couple[3][itrai];

				}

				else

				{

					printf("- %d ",couple[1][itrai]);

					itrai = couple[5][itrai];

				}

			}

			printf("\n");

		}

	}

}





/* ecriture de l'etat du tas, nel est l'etiquette maximale

attribuee */



void ecrtas_t(tas *tas1,int nel)

{

	int *itas,*label,i,ntasmax,nt ;

	double *val,*vacou;



	val = tas1->val;

	itas = tas1->itas;

	label = tas1->label;

	vacou = tas1->vacou;



	ntasmax = tas1->nmaxtas ;

	nt = tas1->nt ;



	printf("taille max du tas= %d -nb d'elements du tas= %d  \n",ntasmax,nt);



	/* impression de itas,val,vacou */



	for(i=1 ; i <= nt ; i++)

	{

		printf("itas[%d]=%d - val[%d]=%lf - vacou=%lf \n",i,itas[i],i,

		val[i],vacou[itas[i]]);



	}

	 /* c     impression de label */



	for (i=1 ; i <= nel ; i++)

		{

			printf("label[%d] = %d \n",i,label[i]) ;

      	}



}



/* cette procedure prend en entree un fichier de cylindres

(nb de cylindres,x1,y1,z1,x2,y2,z2,r) et rend en sortie un pointeur

sur une structure de graphe contenant les cylindres

majrap definit la distance maximale entre les centres de gravit�

des segments-axes de deux cylindres voisins */



graphe *cregraphe_t(char *cyl,float majrap,tas **tas1)

{

	int nbarmax,nbnomax,ntasmax,ncar,i;

	int k ;

	double *car,rap;

	float r,s,x1,y1,z1,x2,y2,z2;

	graphe *graphe1;

	FILE *ptcyl;



	ptcyl = fopen(cyl,"r");

	fscanf(ptcyl,"%d",&nbnomax);

	nbarmax = nbnomax*10;

	ntasmax = 2*nbarmax;

	ncar = 12;

	s = majrap*majrap;

	/* le nb de cylindres multipli� par 2, la somme des rayons, la somme du

	carre des rayons, la somme des abscisses des centres,

	la somme du carre des abscisses des centres, la somme

	des ordonnees des centres, la somme du carre des ordonnees

	des centres, la somme des cotes, la somme du carre des cotes,

	la somme des produits xy, la somme des produits xz,

	la somme des produits yz */



	graphe1 = creergraphe_t (nbnomax,nbarmax,ncar);

	*tas1 = creertas_t(ntasmax,nbarmax,nbnomax);



	/* creation des noeuds du graphe */



	car = (double*)mycalloc(ncar + 1,sizeof(double));



	for(k=0;k<nbnomax;k++)

	{

		fscanf(ptcyl,"%f %f %f %f %f %f %f",&x1,&y1,&z1,&x2,&y2,&z2,&r);

		car[1] = 2;

		car[2] = 2*r;

		car[3] = 2*r*r;

		car[4] = x1 + x2 ;

		car[5] = x1*x1 + x2*x2;

		car[6] = y1 + y2;

		car[7] = y1*y1 + y2*y2;

		car[8] = z1 + z2;

		car[9] = z1*z1 + z2*z2;

		car[10] = x1*y1 + x2*y2;

		car[11] = x1*z1 + x2*z2;

		car[12] = y1*z1 + y2*z2;

		crenoeud_t(graphe1,car,ncar);

	}



	/* creation des arcs du graphe */



	for(k=1;k<nbnomax;k++)

	{

		x1 = graphe1->carac[4][k]/2;

		y1 = graphe1->carac[6][k]/2;

		z1 = graphe1->carac[8][k]/2;

		for(i=k+1;i<=nbnomax;i++)

		{

			x2 = graphe1->carac[4][i]/2;

			y2 = graphe1->carac[6][i]/2;

			z2 = graphe1->carac[8][i]/2;

			rap = (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2);

			if(rap <= s)incou2_t(graphe1,i,k);

		}



	}

	fclose(ptcyl);

	return(graphe1);

}







void testcgraf_t(void)

{

	char cyl1[50],cyl2[50];

	graphe *graf,*grafinit;

	float majr,seuil,majrap;

	tas *tas1;

	int inser,it;



	printf("\n nom du fichier de cylindres initiaux en entree : ");

	scanf("%s",cyl1);

	printf("\n nom du fichier de cylindres en sortie apres fusions : ");

	scanf("%s",cyl2);

	printf("\n seuil pour la connexite =");

	scanf("%f",&majrap);

	fprintf(stderr,"\n rayon = rayon moyen + coeff*ecarttype, taper coeff : ");

	scanf("%f",&majr);

	graf = cregraphe_t(cyl1,majrap,&tas1);

	/* cet appel de cregraphe peut etre remplace par une copie du graphe graf dans la structure pointee par grafinit */

	grafinit = cregraphe_t(cyl1,majrap,&tas1);



	printf("\n nb de noeuds = %d",graf->nbnoeuds);

	printf("\n nb d'arcs = %d",graf->nco);

	printf("\n si vous voulez lancer une fusion hierarchique taper 1 sinon 0 : ");

	scanf("%d",&inser);



	while(inser == 1)

	{

		printf("\n pour fta1_t taper 1, pour fta2_t taper 2 : ");

		scanf("%d",&it);

		printf("seuil somme ecarts types (fta2_t) ou lambda (fta1_t) =  ");

		scanf("%f",&seuil);

		printf("\n seuil = %f",seuil);

		if(it==1)ctas1_t(graf,tas1,seuil);

		else ctas2_t(graf,tas1,seuil);



		printf("\n nb d'elements du tas = %d",tas1->nt);



		printf("\n fin de la creation du tas ");



		if(it==1)hiera1_t(graf,tas1,seuil);

		else hiera2_t(graf,tas1,seuil);



		/* stockage des cylindres dans un fichier */

		printf("\n fin des regroupements");

		printf("\n nb de noeuds = %d",graf->nbnoeuds);

		printf("\n si vous voulez lancer une nouvelle fusion hierarchique taper 1 sinon 0 : ");

		scanf("%d",&inser);

		if(inser == 1)graf = cregraphe_t(cyl1,majrap,&tas1);

	}

	printf("\n stockage des cylindres ");

	stockcyl_t(graf,tas1,grafinit,cyl2,majr);

}





/* genere un fichier de cylindres a partir d'un graphe initial,

d'un graphe final et du tableau label */

void stockcyl_t(graphe *graf,tas *tas1,graphe *grafinit,char *fic,float majr)

{

	int i,n;

	FILE *ptfic;

	double sx1,sxx1,sy1,syy1,sz1,szz1,sxy1,sxz1,syz1,n1,r1,rc1;

	double erreur1,r3;

	int ncar,nel,res,nbcyl,j;

	double xg1,yg1,zg1,dx1,dy1,dz1,x1,y1,z1,x2,y2,z2,r,xg2,yg2,zg2,t,ectype;

	float x,y,z,a,b,c;



	nel = graf->nel ;

	/* traitement du tableau label */

	for(i=1;i<=nel;i++)

	{

		j = i;

		while(tas1->label[j] != j)

		j = tas1->label[j];

		tas1->label[i] = j;

	}



	ptfic = fopen(fic,"w");

	nbcyl = 0;

	ncar = graf->ncar ;

    /* nel est le nombre de noeuds du graphe */



    /* on compte le nb de cylindres a ecrire dans le fichier de sortie */

	for(i=1;i <= nel;i++)

	if(graf->liste[i] != -1)

	{

		n1 = graf->carac[1][i];

		r1 = graf->carac[2][i];

		rc1 = graf->carac[3][i];

		sx1 = graf->carac[4][i];

		sxx1 = graf->carac[5][i];

		sy1 = graf->carac[6][i];

		syy1 = graf->carac[7][i];

		sz1 = graf->carac[8][i];

		szz1 = graf->carac[9][i];

		sxy1 = graf->carac[10][i];

		sxz1 = graf->carac[11][i];

		syz1 = graf->carac[12][i];

		res = rapprox(sx1,sy1,sz1,sxx1,syy1,szz1,sxy1,sxz1,syz1,n1,&xg1,

			&yg1,&zg1,&dx1,&dy1,&dz1,&erreur1);

		if(res != -1)nbcyl++;

	}

	fprintf(ptfic,"%d",nbcyl);



	/* on determine ces cylindres grace aux cylindres initiaux */

	for(i=1;i <= nel;i++)

	if(graf->liste[i] != -1)

	{

		n1 = graf->carac[1][i];

		r1 = graf->carac[2][i];

		rc1 = graf->carac[3][i];

		sx1 = graf->carac[4][i];

		sxx1 = graf->carac[5][i];

		sy1 = graf->carac[6][i];

		syy1 = graf->carac[7][i];

		sz1 = graf->carac[8][i];

		szz1 = graf->carac[9][i];

		sxy1 = graf->carac[10][i];

		sxz1 = graf->carac[11][i];

		syz1 = graf->carac[12][i];

		res = rapprox(sx1,sy1,sz1,sxx1,syy1,szz1,sxy1,sxz1,syz1,n1,&xg1,

		&yg1,&zg1,&dx1,&dy1,&dz1,&erreur1);

		if(res != -1)

		{

			/* calcul de l'ecart type des rayons */

			ectype = sqrt(rc1/n1 - (r1/n1)*(r1/n1));

			/* fprintf(stderr,"ecart type = %f;",ectype); */

			r = r1/n1;

			r = r + majr*ectype;

			xg2 = xg1 + 100*dx1;

			yg2 = yg1 + 100*dy1;

			zg2 = zg1 + 100*dz1;

			r3 = 0;

			/* l'axe du cylindre est (xg1,yg1,zg1)-(xg2,yg2,zg2) */

			for(j=1;j<=nel;j++)

			if(tas1->label[j]==i)

			{

				/* projeter le point initial */

				x = grafinit->carac[4][j]/2;

				y = grafinit->carac[6][j]/2;

				z = grafinit->carac[8][j]/2;

				n = projorth(x,y,z,xg1,yg1,zg1,xg2,yg2,zg2,&a,&b,&c);

				if(n != 1)

				{

					printf("\n pas de projection orthogonale !");

				}else

				{



					t = (x-xg1)*(x-xg1)+(y-yg1)*(y-yg1)+(z-zg1)*(z-zg1);

					if( t> r3)r3=t;

				}

			}

			r3 = sqrt(r3) + 1;

			x1 = xg1 + r3*dx1;

			y1 = yg1 + r3*dy1;

			z1 = zg1 + r3*dz1;

			x2 = xg1 - r3*dx1;

			y2 = yg1 - r3*dy1;

			z2 = zg1 - r3*dz1;

			fprintf(ptfic,"\n %f %f %f %f %f %f %f",x1,y1,z1,x2,y2,z2,r);

		}

	}

	fclose(ptfic);

}


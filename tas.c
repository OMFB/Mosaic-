/*
 * tas.c
 *
 *  Created on: 5 juin 2009
 *      Author: monga
 */

/* test des fonctions de gestion du tas

pour ces fonctions on utilise les tableaux a partir de l'indice 1 car elles ont ete ecrites initialement en

fortran */

/*NEW */

#include <stdio.h>

#include <errno.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>  /* unlink */

#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */



/* prototypes pour les fonctions liees au tas */

void testtas(void) ;

void ecrire(double *val,int *itas,int n) ;

void swopta(double *val,int *itas,int n,int in,int *iout,double valin,double *valout);

void intas(double *val,int *itas,int *n,int in,double valin);

void outas(double *val,int *itas,int *n,int *iout,double *valout);

void cretas(double *val,int *itas,int n);

void suptas(double *val,int *itas,int *n,int j);

void rech(int *itas,int n,int ind,int *iplace);

void verif(int *itas,double *val,int n);





void testtas(void)

{

double *val ;

char *mymalloc() ;

char *mycalloc();

int *itas ;

int n,nmax ;

int i ;

int inser ;

double valout,travlong ;

int travint,iout,iplace ;

void cretas(),intas(), ecrire(), outas(),suptas(),rech(),swopta() ;

void verif() ;

/* initialisation de variables */

	valout = -1 ;

	iplace = -1 ;

	iout = -1 ;

	travint = -1 ;

	travlong = -1 ;

	printf("Nombre maximun d'elements du tas = ") ;

	scanf("%d",&nmax);

	printf("\n");

	val = (double *)mycalloc(nmax+1,sizeof(double));

	itas = (int *)mycalloc(nmax+1,sizeof(int));



	/* initialisation des tableaux � 0 */

	for (i=1 ; i < nmax + 1 ; i++)

	{

		val[i] = 999 ;

		itas[i] = 999 ;

		}



	printf("Nombre d'elements du tas a construire = ") ;

	scanf("%d",&n);

	printf("\n");

	for (i = 1; i < n+1 ; i++)

	{

		printf("pour i egal a : %d ",i);

		printf("\n valeur pour itas : ");

		scanf("%d",&travint);

		itas[i] = travint ;

		printf("\n valeur pour val :");

		scanf("%lf",&travlong);

		val[i] = travlong ;

		}



	cretas(val,itas,n);

	ecrire(val,itas,n) ;

	verif(itas,val,n) ;



	printf("\n si vous voulez inserer un element taper 1 sinon 0 : ");

	scanf("%d",&inser);

	while(inser == 1)

	{

		printf("indice de l'element a inserer : ");

		scanf("%d",&travint);

		printf("\n valeur de l'element a inserer : ");

		scanf("%lf",&travlong);

		printf("\n");

		intas(val,itas,&n,travint,travlong) ;

		ecrire(val,itas,n) ;

		verif(itas,val,n) ;

		printf("\n si vous voulez inserer un element taper 1 sinon 0 : ");

		scanf("%d",&inser);

		}



	printf("voulez vous inserer un element et retirer le plus petit \n (si oui taper 1 sinon 0  : ");

	scanf("%d",&inser);

	while(inser == 1)

	{

		printf("indice de l'element a inserer : ");

		scanf("%d",&travint);

		printf("valeur de l'element a inserer : ");

		scanf("%lf",&travlong);

		swopta(val,itas,n,travint,&iout,travlong,&valout);

		printf("\n indice retire : %d ",iout);

		printf("\n valeur de l'element retire : %lf",valout);

		ecrire(val,itas,n);

		verif(itas,val,n) ;

		printf("voulez vous inserer un element et retirer le plus petit : ");

		scanf("%d",&inser);

	}



	printf("voulez vous retirer le plus petit element : ");

	scanf("%d",&inser);

	while(inser == 1)

	{

		outas(val,itas,&n,&iout,&valout),

		verif(itas,val,n) ;

		printf("indice retire : %d ",iout);

		printf("\n valeur de l'element retire : %lf",valout);

		ecrire(val,itas,n);

		printf("voulez vous retirer le plus petit element : ");

		scanf("%d",&inser);

	}



	printf("voulez vous retirer un element du tas : ");

	scanf("%d",&inser);

	while(inser == 1)

	{

		printf("indice de l'element a retirer : ");

		scanf("%d",&travint);

		rech(itas,n,travint,&iplace);

		printf("\n place de l'element dans le tableau : %d",iplace);

		suptas(val,itas,&n,iplace);

		verif(itas,val,n) ;

		ecrire(val,itas,n);

		printf("voulez vous retirer un element du tas : ");

		scanf("%d",&inser);

		}

}





void ecrire(val,itas,n)

double *val ;

int *itas ;

int n ;

{

	int i,itrav ;

	double dtrav ;



	printf("\n") ;

	for (i = 1 ; i < n+1 ; i++)

	{

	itrav = itas[i];

	dtrav = val[i];

	printf("%d -- %d -- %lf \n",i,itrav,dtrav) ;

	}



}







/* rajoute au tas de taille n d'indices itas et de valeurs val(1)...

val(n), l'elt. d'indice in et de valeur valin enleve du tas l'elt.

de plus petite valeur et donne en sortie

l'indice de celui ci iout et sa valeur valout */



void swopta(val,itas,n,in,iout,valin,valout)

double *val,valin,*valout ;

int *itas ;

int n,*iout ;



{

	int i,j,itemp1,itemp ;

	double temp,temp1 ;



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





/* ajoute l'elt. d'indice in et de valeur valin au tas de

taille n (n>=0), d'elts d'indices itas(1)...itas(n) et de

valeurs val(1)...val(n). n est incremente */



void intas(val,itas,n,in,valin)

double *val,valin ;

int *itas,in,*n ;

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



void outas(val,itas,n,iout,valout)

double *val,*valout ;

int *itas,*iout,*n ;

{

	int in ;

	double valin ;

	void swopta() ;



    valin=val[*n] ;

    in=itas[*n] ;

    swopta(val,itas,*n-1,in,iout,valin,valout) ;

    *n=*n-1 ;



}





/* rearrange itas(1)...itas(n) et val(1)...val(n) pour que les

 elts d'indices itas et de valeurs val forment un tas */



void cretas(val,itas,n)

double *val ;

int *itas ;

int n ;



{



	int j,in ;

	double valin ;

	void intas() ;



	if(n > 1)

	{

	j=1 ;

un: valin=val[j+1] ;

    in=itas[j+1] ;

    intas(val,itas,&j,in,valin) ;

    if(j < n) goto un ;

	}

}





/* cette routine supprime du tas itas(1)..itas(n) l'element

d'indice j et de valeur value ( en mettant n a jour ) */



void suptas(val,itas,n,j)

int *itas,*n,j ;

double *val ;



{

	double valout,tamp ;

	int iout,itamp,k ;

	void outas() ;



    if(j == 1)

    {

 		outas(val,itas,n,&iout,&valout);

        return ;

    }



    itas[j] = itas[*n] ;

    val[j] = val[*n] ;

    *n = *n - 1 ;

    if(val[j] <= val[j/2])

      {

un :     itas[j] = itamp ;

         itas[j] = itas[j/2] ;

         itas[j/2] = itamp ;

         val[j] = tamp ;

         val[j] = val[j/2] ;

         val[j/2] = tamp ;

         if(j < 2)return ;

         j = j/2 ;

         if(val[j]<= val[j/2])goto un ;

       }

      else

       {

deux :   if(2*j > *n)return ;

         if(2*j == *n)

           k = *n;

         else

         	if(val[2*j]< val[2*j+1])k = 2*j ;

         	else

            k = 2*j + 1 ;

         if(val[j] <= val[k])return ;

         itas[j] = itamp ;

         itas[j] = itas[k] ;

         itas[k] = itamp ;

         val[j] = tamp ;

         val[j] = val[k] ;

         val[k] = tamp ;

         j = k ;

         goto deux ;

      }

     }





/* cette routine recherche la place d'un element dans un tas de la

la facon la pire qui soit !!!!!

ind est l'indice de l'element cherche ,itab(1)----itab(n) est le tas

iplace est la place de ind dans le tas si il y est et -1 sinon */







void rech(itas,n,ind,iplace)

int *itas,*iplace,n,ind ;



{

	int i ;



    *iplace = -1 ;

    i = 1 ;

deux :   if(itas[i] == ind){

         *iplace = i ;

         return; }

      else

      	{

         if(i >= n)return ;

         else

           i = i + 1 ;

      }

      goto deux ;

 }





 /* cette procedure verifie que les tableaux val et itas definissent

 bien un tas avec les valeurs comprises entre 1 et n */



 void verif(itas,val,n)

 int *itas,n ;

 double *val ;



 {

 	int i ;



 	for (i = 1 ; i <= n ; i++)



 		{

 		if(2*i <= n)

 			if(val[2*i] < val[i])

 				{

 					printf("ce n'est pas un tas !!! %d \n",i);

 					return;

 					}



 		if(2*i + 1 <= n)

 			if(val[2*i + 1] < val[i])

 				{

 					printf("ce n'est pas un tas !!! %d \n",i);

 					return;

 				}

 		}



 }














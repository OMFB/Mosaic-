#include <stdio.h>
#include <stdlib.h>

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

/* fait le produit matrice vecteur de matA par x et met le resultat dens le vecteur y. matA est un tableau contenant
   le stockage des elements non nuls de la partie triangulaire inferieure d'une matrice creuse
   ptA et colA sont des tableaux contenant les index de la matrice A */
void prodmatvect(float *matA, int *ptA, int *colA, float *x, float *y, int nblgA, int nbA,int itype)
{   int i,j;
    float a,b;
    switch (itype)
    {   case 0: /* matrice diagonale. Seuls les éléments diagonaux sont stockés */
            for (i=0;i<nblgA;i++)
                y[i]=matA[i]*x[i];
            break;
        case 1:  /* matrice symetrique et creuse. Seuls les éléments non nuls de la partie triangulaire inférieure sont stockées sous forme morse*/
            for (i=0;i<nblgA;i++)
                y[i]=0.0;
            y[0]=matA[0]*x[0];
            for (i=1;i<nblgA;i++)
            {
                /* contribution des termes extra diagonaux de la ligne */
                for (j=ptA[i-1]+1;j<ptA[i];j++)
                {   a=matA[j]*x[colA[j]];
                    b=matA[j]*x[i];
                    y[i]+=a;
                    y[colA[j]]+=b;
                }
                /* contribution du terme diagonal de la ligne */
                if (colA[ptA[i]]==i)
                    y[i]+=matA[ptA[i]]*x[i];
                else
                    fprintf(stderr,"erreur dans le produit matrice/vecteur: %d %d\n",i,colA[ptA[i]]);
            }
            break;
        case 2:  /* matrice non symétrique et creuse. Stockage morse */
            for (i=0;i<nblgA-1;i++)
            {   y[i]=0.0;
                for (j=ptA[i];j<ptA[i+1]-1;j++)
                    y[i]+=matA[j]*x[colA[j]];
            }
            y[nblgA-1]=0.0;
            for (i=ptA[nblgA-1];i<nbA;i++)
                y[nblgA-1]+=matA[i]*x[colA[i]];
            break;
        default:
            printf("Cas non traité dans le produit matrice/vecteur");
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
    float  *g, *h, *Ah, *Cg;
    g=(float *)malloc(n*sizeof(float));
    h=(float *)malloc(n*sizeof(float));
    Ah=(float *)malloc(n*sizeof(float));
    Cg=(float *)malloc(n*sizeof(float));
    prodmatvect(matA,ptA,colA,x,g,n,nbA,iAtype); /* g=Ax */
    fprintf(stderr,"g[0]=%f  g[1]=%f \n",g[0],g[1]);
    combinaisonlineairevect(g,b,1.0,-1.0,g,n);  /* g=g-b (ie g=Ax-b) */
    fprintf(stderr,"g[0]=%f  g[1]=%f \n",g[0],g[1]);
    fprintf(stderr,"\n");

    prodmatvect(matC,ptC,colC,g,Cg,n,nbC,iCtype); /* Cg = C*g */
    fprintf(stderr,"Cg[0]=%f  Cg[1]=%f \n",Cg[0],Cg[1]);
    fprintf(stderr,"\n");

    /* gradient preconditionn´ e */
    scalvect(Cg,-1.0,h,n);  /* h = -Cg */
    fprintf(stderr,"h[0]=%f  h[1]=%f \n",h[0],h[1]);

    float g2 = prodscal(Cg,g,n);  /* g2 = (Cg,g) */

    fprintf(stderr," g2=%f  eps*eps=%f \n",g2,eps*eps);
    fprintf(stderr,"**************\n");
    if (g2 < eps*eps)
    /* la solution a d´ej`a converg´ee */
    {
        fprintf(stderr,"iter=0 ||g||ˆ2 = %f \n", g2);
        return 1;
    }
    float reps2 = eps*eps*g2;  // epsilon relatif
    int iter;

    for (iter=0;iter<=nbitermax;iter++)
    {   prodmatvect(matA,ptA,colA,h,Ah,n,nbA,iAtype); /* Ah= A*h */
        fprintf(stderr,"Ah[0]=%f Ah[1]=%f \n",Ah[0],Ah[1]);
        float ro=-prodscal(g,h,n)/prodscal(h,Ah,n); /* ro = - (g,h)/(h,Ah)  */

        fprintf(stderr," iter=%d ro=%f \n",iter,ro);
        fprintf(stderr,"x[0]=%f x[1]=%f \n",x[0],x[1]);
        combinaisonlineairevect(x,h,1.0,ro,x,n);  /* x=x+ro*h  */
        fprintf(stderr,"x[0]=%f x[1]=%f \n",x[0],x[1]);
        combinaisonlineairevect(g,Ah,1.0,ro,g,n);   /* g=g+ro*Ah */
        fprintf(stderr,"g[0]=%f g[1]=%f \n",g[0],g[1]);
        prodmatvect(matC,ptC,colC,g,Cg,n,nbC,iCtype);  /* Cg=C*g  */
        fprintf(stderr,"Cg[0]=%f Cg[1]=%f \n",Cg[0],Cg[1]);

        float g2p=g2;
        g2=prodscal(Cg,g,n);  /* g2 = (Cg,g) */

        fprintf(stderr,"%d ro = %f ||g||^2 = %f \n",iter,ro,g2);

        if (g2 < reps2)
        {
            fprintf(stderr," Convergence iter=%d ro = %f ||g||^2 = %f x[0]=%f x[1]=%f\n",iter,ro,g2,x[0],x[1]);
            break; // ok, convergence
        }
        float gamma = g2/g2p;
        fprintf(stderr,"gamma=%f \n",gamma);
        combinaisonlineairevect(h,Cg,gamma,-1.0,h,n);    /* h = gamma*h - Cg; */
        fprintf(stderr,"gamma*h-Cg h[0]=%f h[1]=%f \n",h[0],h[1]);
    }
    if (iter>=nbitermax) fprintf(stderr,"Non-convergence de la m´ethode du gradient conjugu´e\n");
    free(g);
    free(h);
    free(Ah);
    free(Cg);
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
    {   case 0: /* préconditionneur diagonal; la matrice matA est symétrique, creuse et stockee sous forme morse*/
            for (i=0;i<nbC;i++)
            {
                ptC[i]=i;
                colC[i]=i;
                matC[i]=1.0/matA[ptA[i]];
                fprintf(stderr,"precondmat ptc[%d]=%d colC[%d]=%d matC[%d]=%f\n",i,ptC[i],i,colC[i],i,matC[i]);
            }
            break;
        case 1:  /* preconditionneur par factorisation incomplete de crout; la matrice matA est symétrique, creuse et stockee sous forme morse */
            ;
        case 2:  /* préconditionneur par factorisation incomplete de cholesky; la matrice matA est SDP, creuse et stockee sous forme morse */
            ;
    }
  }


int main()
{
    int N=200000,i;//5000,i;

    /* remplissage de la matrice A */
    /* nblgA=2,nbA=3,nbC=2;*/ int nblgA=N+2,nbA=nblgA*2-1,nbC;
    int *ptA,*ptC,*colA,*colC;
    float *matA, *matC;
    float *x,*b,*volume;
    float h=1./(N+1);
    float h2=h*h;
    matA=(float *)malloc(nbA*sizeof(float));
    colA=(int *)malloc(nbA*sizeof(int));
    ptA=(int *)malloc(nblgA*sizeof(int));
    x=(float *)malloc(nblgA*sizeof(float));
    b=(float *)malloc(nblgA*sizeof(float));
    volume=(float *)malloc(nblgA*sizeof(float));
    ptA[0]=0;
    colA[0]=0;
//    matA[0]=314160.531250;
//    ptA[1]=2;
//    colA[1]=0;
//    matA[1]=-314160.000000;
//    colA[2]=1;
//    matA[2]=314160.531250;
    matA[0]=2./h2;
    for (i=1;i<nblgA;i++)
    {
        ptA[i]=ptA[i-1]+2;
        colA[ptA[i]-1]=i-1;
        colA[ptA[i]]=i;
        matA[ptA[i]-1]=-1./h2;
        matA[ptA[i]]=2./h2;
    }
    fprintf(stderr,"ptA[13]=%d\n",ptA[13]);
    /* conditions aux limites */
    matA[0]=1.;
    matA[1]=0.;
    matA[ptA[nblgA-1]]=1.;
    matA[ptA[nblgA-1]-1]=0.;
    int iAtype=1;
    int iCtype=0;
    nbA=ptA[nblgA-1]+1;
    int nbitermax=5000000;
    float eps=1.e-3;
    for (i=0;i<nblgA;i++)
    {
    //    b[0]=1.909955;////
        b[i]=1.;
        // x[0]=0.0;//
        x[i]=0.;
        //volume[0]=0.5236;
        //b[1]=0.00000;
        //x[1]=0.0;
        //volume[1]=0.5236;
    }
    b[0]=0.;
    b[nblgA-1]=0.;
    switch(iCtype)
    {   case 0:
            nbC=nblgA;
            matC=(float *)malloc(nbC*sizeof(float));
            colC=(int *)malloc(nbC*sizeof(int));
            ptC=(int *)malloc(nbC*sizeof(int));
            break;
    }
    fprintf(stderr," **** Avant precondmat **** %d %d %d\n",nblgA,nbA,nbC);

    precondmat(matA,matC,ptA,colA,nblgA,nbA,ptC,colC,nbC,iAtype,iCtype);
    fprintf(stderr," **** Avant GC **** \n");
    GradienConjugue(matA,matC,b,x,ptA,colA,nblgA,nbA,ptC,colC,nbC,nbitermax,eps,iAtype,iCtype);
    fprintf(stderr," **** Solution calculee **** \n");
    for (i=0;i<nblgA;i++)
    {
        fprintf(stderr,"x[%d]=%f \n",i,x[i]);//*volume[i]);
    }

    return 0;
}

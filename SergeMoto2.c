void triinsertion(int *t,int n)
{
    int i,j,aux,min;
    for (i=0;i<n-1;i++)
    {
        min=i;
        for (j=i+1;j<n;j++)
            if (t[j]<t[i]) min=j;
        if (min!=i)
        {
            aux=t[i];
            t[i]=t[min];
            t[min]=aux;
        }
    }
}


float prodscal (float *x, float *y, int n)
{   float a=0.0;
    int i;
    for (i=0; i<n; i++)
        a+= x[i]*y[i];
    return a;
}
void sommevect(float *x, float *y, float *z, int n)
{   int i;
    for (i=0;i<n;i++)
        z[i]=x[i]+y[i];
}
void opposevect(float *x, float *y, int n)
{   int i;
    for (i=0;i<n;i++)
        y[i]=-x[i];
}
void scalvect(float *x, float scal, float *y, int n)
{   int i;
    for (i=0;i<n;i++)
        y[i]=scal*x[i];
}
void prodmatvect(float *matA, int *ptA, int *colA, float *x, float *y, int nblgA, int nbA,int itype)
{   int i,j;
    float a,b;
    switch (itype)
    {   case 0: /* matrice diagonale. Seuls les �l�ments diagonaux sont stock�s */
            for (i=0;i<nblgA;i++)
                y[i]=matA[i]*x[i];
            break;
        case 1:  /* matrice symetrique et creuse. Seuls les �l�ments non nuls de la partie triangulaire inf�rieure sont stock�es sous forme morse*/
            for (i=0;i<nblgA;i++)
                y[i]=0.0;
            y[0]=matA[0]*x[0];
            for (i=1;i<nblgA;i++)
            {   for (j=ptA[i-1]+1;j<ptA[i];j++)
                {   a=matA[j]*x[colA[j]];
                    b=matA[j]*x[i];
                    y[i]+=a;
                    y[colA[j]]+=b;
                }
                if (colA[ptA[i]]==i) y[i]+=matA[ptA[i]]*x[i];
            }
            break;
        case 2:  /* matrice non sym�trique et creuse. Stockage morse */
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
            printf("Cas non trait� dans le produit matrice/vecteur");
    }
}

int GradienConjugue(float *matA,float *matC,float *b, float *x, int *ptA, int *colA, int nblgA, int nbA, int *ptC, int *colC, int nbC, int nbitermax,float eps,int iAtype,int iCtype)
{
    int n=nblgA;
    float  *g, *h, *Ah, *Cg,*x1,*g1,*h1;
    g=(float *)malloc(n*sizeof(float));
    h=(float *)malloc(n*sizeof(float));
    Ah=(float *)malloc(n*sizeof(float));
    Cg=(float *)malloc(n*sizeof(float));
    x1=(float *)malloc(n*sizeof(float));
    g1=(float *)malloc(n*sizeof(float));
    h1=(float *)malloc(n*sizeof(float));
    prodmatvect(matA,ptA,colA,x,g,n,nbA,iAtype);
    scalvect(b,-1.0,b,n);
    sommevect(g,b,g,n);

    prodmatvect(matC,ptC,colC,g,Cg,n,nbC,iCtype);
// gradient preconditionn� e
    scalvect(Cg,-1.0,h,n);
    float g2 = prodscal(Cg,g,n);
    if (g2 < eps*eps)
// solution d�ej`a converg�ee
    {   printf("iter=0 ||g||�2 = %f \n", g2);
        return 1;
    }
    float reps2 = eps*eps*g2;  // epsilon relatif
    int iter;
    for (iter=0;iter<=nbitermax;iter++)
    {   prodmatvect(matA,ptA,colA,h,Ah,n,nbA,iAtype);
        float ro=-prodscal(g,h,n)*prodscal(h,Ah,n);
        scalvect(h,ro,x1,n);
        sommevect(x,x1,x,n);
        scalvect(Ah,ro,g1,n);
        sommevect(g,g1,g,n);
        prodmatvect(matC,ptC,colC,g,Cg,n,nbC,iCtype);
        float g2p=g2;
        g2=prodscal(Cg,g,n);
        scalvect(Ah,ro,g1,n);
        sommevect(g1,g,g,n);
        prodmatvect(matC,ptC,colC,g,Cg,n,nbC,iCtype);
        g2p=g2;
        g2 = prodscal(Cg,g,n);
        printf("%d ro = %f ||g||^2 = %f \n",iter,ro,g2);

        if (g2 < reps2)
        {   printf("%d ro = %f ||g||^2 = %f \n",iter,ro,g2);
            return 1; // ok, convergence
        }
        float gamma = g2/g2p;
        scalvect(h,gamma,h,n);/* h *= gamma;*/
        opposevect(Cg,h1,n);
        sommevect(h,h1,h,n);    /* h -= Cg; */
    }
    free(g);
    free(h);
    free(Ah);
    free(Cg);
    free(x1);
    free(g1);
    free(h1);
    printf("Non-convergence de la m�ethode du gradient conjugu�e");
    return 0;
}

void precondmat(float *matA,float *matC, int *ptA,int *colA,int nblgA,int nbA,int *ptC,int *colC,int nbC,int iAtype,int iCtype)
{   int i,j;
    switch (iCtype)
    {   case 0: /* pr�conditionneur diagonal; la matrice matA est sym�trique, creuse et stockee sous forme morse*/
            for (i=0;i<nbC;i++)
            {   ptC[i]=i;
                colC[i]=i;
                matC[i]=1.0/matA[ptA[i]];
            }
            break;
        case 1:  /* preconditionneur par factorisation incomplete de crout; la matrice matA est sym�trique, creuse et stockee sous forme morse */
            ;
        case 2:  /* pr�conditionneur par factorisation incomplete de cholesky; la matrice matA est SDP, creuse et stockee sous forme morse */
            ;
    }
  }
}



/* cette fonction calcule les deltas avec la methode implicite et rend les nouvelles valeurs des concentrations en memoire vive et aussi dans un fichier */
/* Il faut ajouter en entrée de cette fonction T la durée de la simulation */
/* Cette fonction effectue la diffusion avec schéma implicite de l'attribut numcar du graphe et met a jour le graphe */
float *miseajourdifimplic(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax)
{
	int nbnoeud,i,j,k,n,*vois,nvois,narcs,*ptligne,*colonne,nmat,ntotmat,nbC,*colC,*ptC,nbitermax=500000;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,*tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,massetot2,prop,vtot,sommedelta,vartr,theta,*sommetheta,*nconc1,*nconc2,diag,*volume,*mat,*matC;
	float rboul1,rs,eps=1.e-2,tsimul;
	FILE *ptficsort;

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


	for(i = 0;i<nbnoeud;i++)
	{
		diag = 0;
		x1 = graphe->carac[1][i+1];
		y1 = graphe->carac[2][i+1];
		z1 = graphe->carac[3][i+1];
		rboul1 = graphe->carac[4][i+1];
		voisins(i+1,graphe,nvmax,&nvois,&vois);
		/* tri des voisins dans l'ordre croissant */
		if (nvois>1) triinsertion(vois,nvois);
		for(j=0;(j<nvois)&&(vois[j]<i);j++)
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
			theta = (coeff*scont*deltat)/distance;
			diag += theta;
            colonne[nmat] = n-1;
            mat[nmat] = -theta;
            mat[ptligne[n-1]] += theta;
            nmat++;
		}
		volume[i] = (4.0/3.0)*3.1416*rboul1*rboul1*rboul1;
		diag += volume[i];
		ligne[i] = nmat;
		colonne[nmat] = i;
		mat[nmat] = diag;
        nconc1[i] = (graphe->carac[numcar + 4][i+1])/volume[i];
		nmat++;
	}
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


    tsimul=0:
    while (tsimul<T)
    {
        GradienConjugue(mat,matC,nconc1,nconc2,ligne,colonne,nbnoeud,nmat,ptC,colC,nbC,nbitermax,eps,1,0);
    /* mise à jour de la concentration initiale pour le pas de temps suivant */
        for (i=0;i<nbnoeud;i++)
            nconc1[i]=nconc2[i]*volume[i];
        tsimul += deltat;
    }

	/* nconc2 contient les nouvelles valeurs de la concentration */

	/* mise a jour des masses dans le tableau carac */

	for(i = 1;i<nbnoeud+1;i++)
	{
		graphe->carac[numcar + 4][i] = nconc1[i-1]*volume[i-1]; 
	
	}


	/* liberation de la place allouee dynamiquement pour les tableaux */
    myfreei(vois);
	myfreei(ligne);
	myfreei(colonne);
	myfreef(mat);
	myfreef(volume);
	myfreef(nconc1);
	myfreef(matC);
	myfreei(colC); 
	myfreei(ptC); 
	myfreef(nconc2);
}


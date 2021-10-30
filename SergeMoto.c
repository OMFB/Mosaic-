/* cette fonction calcule les deltas avec la methode implicite et rend les nouvelles valeurs des concentrations en meoire vive et aussi dans un fichier */

float *miseajourdifimp(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax,char *ficent)
{
	int nbnoeud,i,j,k,n,*vois,nvois,narcs,*ligne,*colonne,nmat,ntotmat;
	float volboul,rboul,gdif,conc,volboul2,rboul2,gdif2,conc2,delta,*tabdelta,scont,petitdeltat,distance,x1,y1,z1,x2,y2,z2;
	float massetot,masseneg,sommeconc,massetot2,prop,vtot,sommedelta,vartr,theta,*sommetheta,*nconc1,*nconc2,diag,*volume,*mat;
	float rboul1,rs; 
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
		distance = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
		rs = (2.0*rboul1*rboul2)/(rboul1+rboul2); 
		scont = 4*3.1416*rs*rs; 
		theta = -(coeff*scont*deltat)/distance; 
		/* on considere la partie superieure de la matrice symetrique c'est a dire telle que le numero de la ligne est plus pett que le numero de la colonne 
		comme le graphe a ete construit en regardant les voisinages des boules l'ordre des couples est lexicographique avec k < n */
		ligne[nmat] = k;
		colonne[nmat] = n;
		mat[nmat] = -theta;
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
			diag += theta; 
		}
		diag += (4.0/3.0)*3.1416*rboul1*rboul1*rboul1;
		ligne[nmat] = i;
		colonne[nmat] = i;
		mat[nmat] = diag;
		nmat++; 
	}
    /* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
    /* POINT D'ENTREE POUR LE GRADIENT CONJUGUE */
    /* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

	/* entree de la procedure de resolution du systeme par le gradient conjugue: nbnoeud,nmat, ligne, colonne, mat, volume, nconc1 */
	/* la fonction de resolution du systeme donne nconc2 tableau des valeurs des concentrations a l'instant t+deltat */

	/* APPEL DE LA FONCTION QUI RESOUD LE SYSTEME AVEC LE GRADIENT CONJUGUE */
    /* "*mat" CONTIENT LES "nmat" ELEMENTS NON NULS A PRIORI DE LA MATRICE A, "*mat" est indexe a partir de 0. 
    on a "nbnoeud" noeuds du graphe et "narcs" arcs dans le graphe (en dehors de la diagonale on a narcs valeurs non nuls pour A). 
    "*ligne" et "*colonne" contiennent les coordonnees en ligne-colonne des elemnts de "*mat" cad mat[i] = A(ligne[i],colonne[i]). 
    Les valeurs non nulles de la matrice dans "*mat" sont rangees de cette maniere ; d'abord tous les elements non nuls dans l'ordre lexicographique
    ligne par ligne sans la diagonale, ensuite on stocke la diagonale. On a le cadran superieur dans le stockage "*ligne" et "*colonne". 
    "*nconc1" qui contient les valeurs initiales de la concentration, "*conc1" est indexe a partir de 1 et contient les concentrations des "nbnoeud" boules 
    "*volume" contient les valeurs du volume des boules et est aussi indexe a partir de 1. 

    LA FONCTION SERGE MOTO DOIT PRENDRE EN ENTREE: "*mat" *float,"*ligne" *int,"*colonne" *float,"*volume" *float,"*nconc1" *float, narcs int (nb arcs), nbnoeud int (nb boules),
    elle resoud le systeme : Ax = V-1.b
    et elle rend "*nconc2" *float tel que nconc2[i] = concentration a l'instant t + ∂t 


	/* on met les donnees necessaires a la resolution du systeme dans le fichier ficent */

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

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

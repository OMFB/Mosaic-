/*

 * calmat.c
 *
 *  Created on: 5 juin 2009
 *      Author: monga
 */

/* routines de calcul mathematiques de base */

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



int equa2(double a,double b,double *r1,double *r2);

void resyst3(float a1,float b1,float c1,float d1,float a2,

float b2,float c2,float d2,float a3,float b3,float c3,float d3,

int *sol,float *x,float *y,float *z);

double rac3(double x);

int diagonal(double a11,double a21,double a22,double a31,double

a32,double a33,double *l1,double *l2,double *l3,double *p1x,

double *p1y,double *p1z,double *p2x,double *p2y,double *p2z,

double *p3x,double *p3y,double *p3z);

int equa3(double a,double b,double c,double *r1,double *r2,double *r3);

void testequa3(void);

void testrac3(void);

int vecpropre(double a11,double a21,double a22,double a31,

double a32,double a33,double l,double *x,double *y, double *z);

int resyst2(double a,double b,double c,double d,double e,

double f,double *x,double *y);

int approx(float **points,int nbpoints,float *xg,float *yg,float *zg,

float *dx,float *dy,float *dz,float *erreur);

void testapp(void);

float **allotab2f(int nli,int nco);

int rapprox(double sx,double sy, double sz,double sxx,

double syy,double szz,double syx,double szx,double szy,double nbpoints,double *xg,double *yg,double *zg,

double *dx,double *dy,double *dz,double *erreur);

double erapprox(double sx,double sy, double sz,double sxx,

double syy,double szz,double syx,double szx,double szy,

double nbpoints);



/* resolution d'une equation du second degre (dans R) :

x2 +ay + b = 0; renvoie le nombre de solutions

et leurs valeurs */

int equa2(double a,double b,double *r1,double *r2)

{

	double delta;



	delta = a*a - 4*b;

	if(delta > 0)

	{

		*r1 = (-a + sqrt(delta))/2;

		*r2 = (-a - sqrt(delta))/2;

		return(2);

	}

	else

	if(delta == 0)

	{

		*r1 = -a/2;

		*r2 = *r1;

		return(1);

	}

	else

	return(0);

}



/* resolution d'une equation du troisieme degre (dans R)

x3 + ax2 + bx +c ; renvoie le nb de solutions et leurs valeurs

*/

int equa3(double a,double b,double c,double *r1,double *r2,double *r3)

{

	long double p,q,delta1,delta2,rdelta1,rdelta2;

	long double a1,b1,a2,b2,a3,b3,mx1,ax1,mu1,au1,pie,epsilon;

	int res;

	double rac1,rac2;

	/* on teste si l'equation n'est pas directement factorisable */

	if(c == 0)

	{

		*r1 = 0;

		res = equa2(a,b,&rac1,&rac2);

		if(res != 0)

		{

			*r2 = rac1;

			*r3 = rac2;

			return(3);

		}

		if(res == 0)

		{

			*r2 = 0;

			*r3 = 0;

			return(1);

		}

	}



	p = b - (a*a)/3.0;

	q = (a/27.0)*(2*a*a - 9*b) + c;

	/* printf("\n p = %f q = %f",p,q); */

	if(p==0)

	{

		*r1 = rac3(-q);

		*r1 = *r1 - a/3.0;

		return(1);

	}else

	if(q == 0)

	{

		*r1 = 0;

		*r1 = *r1 - a/3.0;

		if(p >= 0)return(1);

		else

		{

			*r2 = sqrt(-p);

			*r3 = -sqrt(-p);

			*r2 = *r2 - a/3.0;

			*r3 = *r3 - a/3.0;

			return(3);

		}

	}

	else

	{

		delta1 = (27*q*q + 4*p*p*p)/27.0;

		/* printf("\n delta1 = %f",delta1); */

		if(delta1 > 0)

		{

			rdelta1 = sqrt(delta1);

			*r1 = rac3(-q/2.0 - rdelta1/2.0) +

			rac3(-q/2.0 + rdelta1/2.0);

			delta2 = (*r1)*(*r1) + (4*q)/(*r1);

			/* printf("\n delta2 = %f",delta2); */

			if(delta2 > 0)

			{

				rdelta2 = sqrt(delta2);

				*r2 = (-(*r1) + rdelta2)/2.0;

				*r3 = (-(*r1) - rdelta2)/2.0;

				*r1 = *r1 - a/3.0;

				*r2 = *r2 - a/3.0;

				*r3 = *r3 - a/3.0;

				return(3);

			}

			else

			if(delta2 == 0)

			{

				*r2 = -(*r1)/2.0;

				*r1 = *r1 - a/3.0;

				*r2 = *r2 - a/3.0;

				return(2);

			}

			else

			{

				*r1 = *r1 - a/3.0;

				return(1);

			}

		}

		else

		if(delta1 == 0)

		{

			*r1 = rac3(-4*q);

			*r2 = rac3(q/2.0);

			*r1 = *r1 - a/3.0;

			*r2 = *r2 - a/3.0;

			return(2);

		}

		else

		{

			pie = 3.14116 ;

			epsilon = 0.0001;

			/* cas ou le discriminant est negatif on entre

			dans le complexe ! */

			mx1 = sqrt((q*q)/4 - delta1/4) ;

			/* mx1 est le module de X1 */

			ax1 = acos((-q/2)/mx1);

			if(sin(ax1) < 0)ax1 = -ax1;

			/* ax1 est l'argument de X1 */

			/* determination d'une racine cubique de X1 u1 */

			mu1 = rac3(mx1);

			au1 = ax1/3;

			/* printf("\n mu1 = %lf au1 = %lf",mu1,au1);*/

			/* determination des 3 racines de l'equation dans C

			en fonction de u1  */

			a1 = mu1*cos(au1) - (p/(3.0*mu1))*cos(-au1);

			b1 = mu1*sin(au1) - (p/(3.0*mu1))*sin(-au1);

			a2 = mu1*cos(au1+((2*pie)/3.0)) - (p/(3.0*mu1))*cos(((-2*pie)/3.0)-au1);

			b2 = mu1*sin(au1+((2*pie)/3.0)) - (p/(3.0*mu1))*sin(((-2*pie)/3.0)-au1);

			a3 = mu1*cos(au1+((4*pie)/3.0)) - (p/(3.0*mu1))*cos(((-4*pie)/3.0)-au1);

			b3 = mu1*sin(au1+((4*pie)/3.0)) - (p/(3.0*mu1))*sin(((-4*pie)/3.0)-au1);

			a1 = a1 - a/3.0;

			a2 = a2 - a/3.0;

			a3 = a3 - a/3.0;

			/* printf("\n a1=%f b1=%f a2=%f b2=%f a3=%f b3=%f",

			a1,b1,a2,b2,a3,b3); */



			if(fabsl(b1)<epsilon && fabsl(b2)<epsilon

			&& fabsl(b3)<epsilon)

			{

				*r1 = a1;

				*r2 = a2;

				*r3 = a3;

				return(3);

			}

			else if(fabsl(b1) < epsilon && fabsl(b2)<epsilon)

			{

				*r1 = a1;

				*r2 = a2;

				return(2);

			}

			else if(fabsl(b1) < epsilon && fabsl(b3)<epsilon)

			{

				*r1 = a1;

				*r2 = a3;

				return(2);

			}

			else if(fabsl(b2) < epsilon && fabsl(b3)<epsilon)

			{

				*r1 = a2;

				*r2 = a3;

				return(2);

			}

			else if (fabsl(b1) < epsilon)

			{

				*r1 = a1;

				return(1);

			}

			else if (fabsl(b2) < epsilon)

			{

				*r1 = a2;

				return(1);

			}

			else if (fabsl(b3) < epsilon)

			{

				*r3 = a3;

				return(1);

			}

		}

		return(0);

	}

}





double rac3(double x)

{

	double t;



	t = 1.0/3.0;

	if(x>0)return(pow(x,t));

	else if(x<0)return(-pow(-x,t));

	else return(0);

}





void testequa3(void)

{

	double a,b,c,r1,r2,r3,v1,v2,v3;

	int res;



	printf("\n a,b,c : ");

	scanf("%lf %lf %lf",&a,&b,&c);

	res = equa3(a,b,c,&r1,&r2,&r3);

	printf("\n res=%d r1=%lf r2=%lf r3=%lf",res,r1,r2,r3);

	v1 = r1*r1*r1 + a*r1*r1 + b*r1 + c;

	v2 = r2*r2*r2 + a*r2*r2 + b*r2 + c;

	v3 = r3*r3*r3 + a*r3*r3 + b*r3 + c;

	printf("\n v1=%lf v2=%lf v3=%lf",v1,v2,v3);

}



void testrac3(void)

{

	double r,r3,e;



	printf("\n reel = ");

	scanf("%lf",&r);

	r3 = rac3(r);

	printf("\n racine cubique = %lf",r3);

	printf("\n reel = ");

	scanf("%lf",&r);

	printf("\n exposant = ");

	scanf("%lf",&e);

	r3 = pow(r,e);

	printf("\n resultat = %lf",r3);

}



/* diagonalisation d'une matrice de covariance

retourne le nombre de valeurs propres et -1 si pb */



int diagonal(double a11,double a21,double a22,double a31,double

a32,double a33,double *l1,double *l2,double *l3,double *p1x,

double *p1y,double *p1z,double *p2x,double *p2y,double *p2z,

double *p3x,double *p3y,double *p3z)

{

	double a,b,c,r1,r2,r3;

	int res,res2;





	a = -a22-a33-a11;

	b = a22*a11+a33*a11+a22*a33-a32*a32-a21*a21-a31*a31;

	c= -a11*a22*a33+a32*a32*a11+a21*a21*a33-a21*a31*a32

		-a31*a21*a32+a22*a31*a31;



	/* fprintf(stderr,"\n a,b,c %f %f %f",a,b,c);*/

	res = equa3(a,b,c,&r1,&r2,&r3);

	/* fprintf(stderr,"\n r1 = %f; r2= %f ; r3 = %f",r1,r2,r3); */

	/* on considere que les racines non nulles */

	/* printf("\n res : %d racines : %f %f %f",res,r1,r2,r3);*/

	if(res == 3)

	{

		if(r1 == 0 || r2 == 0 || r3 == 0)

		{

			if(r1 == 0 && r2 != 0 && r3 != 0)

			{

				r1 = r2;

				r2 = r3;

				res = 2;

			}else if(r2 == 0 && r1 != 0 && r3 != 0)

			{

				r2 = r3;

				res = 2;

			}

			else if(r3 == 0 && r1 != 0 && r2 != 0)

			{

				res = 2;

			}else if(r1 == 0 && r2 == 0 && r3 != 0)

			{

				r1 = r3;

				res = 1;

			}

			else if(r1 == 0 && r3 == 0 && r2 != 0)

			{

				r1 = r2;

				res = 1;

			}

			else if(r2==0 && r3==0 && r1 != 0)

			{

				res = 1;

			}

			else

			{

				res = 0;

			}

		}

	}else if(res == 2)

	{

		if(r1 == 0 && r2 == 0)res = 0;

		else if(r1 == 0)

		{

			r1 = r2;

			res = 1;

		}else if(r2 == 0)

		{

			res = 1;

		}

	}

	else if(res == 1)

	{

		if(r1 == 0)res=0;

	}

	*l1 = r1;

	*l2 = r2;

	*l3 = r3;





	if(res == 3)

	{

		/* recherche des 3 vecteurs propres */

		/* printf("\n valeurs propres : %f %f %f",r1,r2,r3);*/

		res2 = vecpropre(a11,a21,a22,a31,a32,a33,r1,p1x,p1y,p1z);

		if(res2 == 0)

		{

			printf("\n erreur dans diagonal!");

			return(-1);

		}

		res2 = vecpropre(a11,a21,a22,a31,a32,a33,r2,p2x,p2y,p2z);

		if(res2 == 0)

		{

			printf("\n erreur dans diagonal!");

			return(-1);

		}

		res2 = vecpropre(a11,a21,a22,a31,a32,a33,r3,p3x,p3y,p3z);

		if(res2 == 0)

		{

			printf("\n erreur dans diagonal!");

			return(-1);

		}

		return(3);

	}

	else if(res == 2)

	{

		/* printf("\n valeurs propres : %f %f",r1,r2); */



		res2 = vecpropre(a11,a21,a22,a31,a32,a33,r1,p1x,p1y,p1z);

		if(res2 == 0)

		{

			printf("\n erreur dans diagonal!");

			return(-1);

		}

		res2 = vecpropre(a11,a21,a22,a31,a32,a33,r2,p2x,p2y,p2z);

		if(res2 == 0)

		{

			printf("\n erreur dans diagonal!");

			return(-1);

		}

		return(2);

	}

	else if(res == 1)

	{

		/* printf("\n valeurs propres : %f",r1); */

		res2 = vecpropre(a11,a21,a22,a31,a32,a33,r1,p1x,p1y,p1z);

		if(res2 == 0)

		{

			printf("\n erreur dans diagonal!");

			return(-1);

		}

		return(1);

	}

	else if(res == 0)

	return(0);

	return(0);

}





/* prend en entree une matrice de covariance 3 par 3 et une

valeur propre et donne en sortie un vecteur propre associe ; retourne 0 si probleme */



int vecpropre(double a11,double a21,double a22,double a31,

double a32,double a33,double l,double *x,double *y, double *z)

{

	int res;



	/* cas ou z different de 0 */

	*z = 1;

	res = resyst2(a11-l,a21,-a31,a21,a22-l,-a32,x,y);

	if(res == 1)return(1);

	else

	{

		res = resyst2(a21,a22-l,-a32,a31,a32,l-a33,x,y);

		if(res == 1)return(1);

		else

		{

			res = resyst2(a11-l,a21,-a31,a31,a32,l-a33,x,y);

			if(res == 1)return(1);

			else

			{

				/* cas ou y different de 0 */

				*y = 1;

				res = resyst2(a11-l,a31,-a21,a21,a32,l-a22,x,z);

				if(res == 1)return(1);

				else

				{

					res = resyst2(a21,a32,l-a22,a31,a33-l,-a32,x,z);

					if(res == 1)return(1);

					else

					{

						res = resyst2(a11-l,a31,-a21,a31,a33-l,-a32,x,z);

						if(res == 1) return(1);

						else

						{

							/* cas ou x different de 0 */

							*x = 1;

							res = resyst2(a21,a31,l-a11,a22-l,a32,-a21,y,z);

							if(res == 1)return(1);

							else

							{

								res = resyst2(a22-l,a32,-a21,a32,a33-l,-a31,y,z);

								if(res == 1)return(1);

								else

								{

									res = resyst2(a21,a31,l-a11,a32,a33-l,-a31,y,z);

									if(res == 1) return(1);

									else return(0);

								}

							}

						}

					}

				}

			}

		}



	}



}



/* prend en entree un systeme d'equations a 2 inconnues :

ax + by = c ; dx + ey = f et rend 1 et la solution si il

y a une solution unique et 0 sinon */

int resyst2(double a,double b,double c,double d,double e,

double f,double *x,double *y)

{

	double det,detx,dety;



	det = a*e - b*d;

	if(det != 0)

	{

		detx = c*e - b*f;

		dety = a*f - c*d;

		*x = detx/det;

		*y = dety/det;

		return(1);

	}

	else return(0);

}





/* prend en entree un tableau contenant les coordonnees

de points 3d, et leur nombre et rend en sortie l'approximation polygonale :

centre de gravite, vecteur directeur norme, et la somme

des erreurs, rend 1 si pas de probleme et 0 sinon

A CAUSE DES ERREURS D'ARRONDIS L'ERREUR PEUT ETRE NEGATIVE

(RAREMENT MAIS CA ARRIVE !!) */



int approx(float **points,int nbpoints,float *xg,float *yg,float *zg,

float *dx,float *dy,float *dz,float *erreur)

{

	double sx,sy,sz,sxx,syy,szz,syx,szx,szy;

	double a11,a21,a22,a31,a32,a33,l1,l2,l3;

	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,norme;

	int res,i;

	/* calcul de la matrice de covariance centree */

	sx = 0;

	sy = 0;

	sz = 0;

	sxx = 0;

	syy = 0;

	szz = 0;

	syx = 0;

	szx = 0;

	szy = 0;

	for(i = 0;i < nbpoints;i++)

	{

		sx = sx + points[0][i];

		sy = sy + points[1][i];

		sz = sz + points[2][i];

		sxx = sxx + points[0][i]*points[0][i];

		syy = syy + points[1][i]*points[1][i];

		szz = szz + points[2][i]*points[2][i];

		syx = syx + points[0][i]*points[1][i];

		szx = szx + points[0][i]*points[2][i];

		szy = szy + points[1][i]*points[2][i];

	}

	/* fprintf(stderr,"\n nbpoints = %d",nbpoints);

	fprintf(stderr,"\n sx=%f;sy=%f;sz=%f;sxx=%f;syy=%f;szz=%f;syx=%f;szx=%f;szy=%f",sx,sy,sz,sxx,syy,szz,syx,szx,szy);*/

	*xg = sx/nbpoints;

	*yg = sy/nbpoints;

	*zg = sz/nbpoints;

	a11 = sxx - (sx*sx)/nbpoints;

	a22 = syy - (sy*sy)/nbpoints;

	a33 = szz - (sz*sz)/nbpoints;

	a21 = syx - (sx*sy)/nbpoints;

	a31 = szx - (sz*sx)/nbpoints;

	a32 = szy - (sz*sy)/nbpoints;

	/* fprintf(stderr,"\n a11=%f;a22=%f;a33=%f;a21=%f;a31=%f;a32=%f",a11,a22,a33,a21,a31,a32);*/

	res = diagonal(a11,a21,a22,a31,a32,a33,&l1,&l2,&l3,&p1x,&p1y,&p1z,&p2x,&p2y,&p2z,&p3x,&p3y,&p3z);

	/* fprintf(stderr,"\n res = %d l1 = %f l2 = %f l3 = %f",res,l1,l2,l3);

	fprintf(stderr,"\n p1x=%f;p1y=%f;p1z=%f;p2x=%f;p2y=%f;p2z=%f;p3x=%f;p3y=%f;p3z=%f",p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z);*/

	if(res == 3)

	{

		/* determination de la plus petite valeur propre */



		if(l1 >= l2 && l1 >= l3)

		{

			norme = sqrt(p1x*p1x+p1y*p1y+p1z*p1z);

			*dx = p1x/norme;

			*dy = p1y/norme;

			*dz = p1z/norme;

			*erreur = l2 + l3;

			return(1);

		}

		else if(l2 >= l1 && l2 >= l3)

		{

			norme = sqrt(p2x*p2x+p2y*p2y+p2z*p2z);

			*dx = p2x/norme;

			*dy = p2y/norme;

			*dz = p2z/norme;

			*erreur = l1 + l3;

			return(1);

		}

		else

		{

			norme = sqrt(p3x*p3x+p3y*p3y+p3z*p3z);

			*dx = p3x/norme;

			*dy = p3y/norme;

			*dz = p3z/norme;

			*erreur = l1 + l2;

			return(1);

		}

	}

	else if(res == 2)

	if(l1 <= l2)

	{

		norme = sqrt(p2x*p2x+p2y*p2y+p2z*p2z);

		*dx = p2x/norme;

		*dy = p2y/norme;

		*dz = p2z/norme;

		*erreur = l1;

		return(1);

	}

	else

	{

		norme = sqrt(p1x*p1x+p1y*p1y+p1z*p1z);

		*dx = p1x/norme;

		*dy = p1y/norme;

		*dz = p1z/norme;

		*erreur = l2;

		return(1);

	}

	else if(res == 1)

	{

		norme = sqrt(p1x*p1x+p1y*p1y+p1z*p1z);

		*dx = p1x/norme;

		*dy = p1y/norme;

		*dz = p1z/norme;

		*erreur = 0;

		return(1);

	}

	else

	return(0);



}

/* prend en entree un tableau contenant les coordonnees

de points 3d, et leur nombre et rend en sortie le meilleur plan approximant les points :

point du plan (xg,yg,zg), vecteur normal (dx,dy,dz), et la racine carr�e de la somme

des erreurs quadratiques (erreur), rend 1 si pas de probleme et 0 sinon

A CAUSE DES ERREURS D'ARRONDIS L'ERREUR PEUT ETRE NEGATIVE

(RAREMENT MAIS CA ARRIVE !!) */



int approxplan(float **points,int nbpoints,float *xg,float *yg,float *zg,

float *dx,float *dy,float *dz,float *erreur)

{

	double sx,sy,sz,sxx,syy,szz,syx,szx,szy;

	double a11,a21,a22,a31,a32,a33,l1,l2,l3;

	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,norme;

	int res,i;


	/* calcul de la matrice de covariance centree */

	sx = 0;

	sy = 0;

	sz = 0;

	sxx = 0;

	syy = 0;

	szz = 0;

	syx = 0;

	szx = 0;

	szy = 0;

	for(i = 0;i < nbpoints;i++)

	{

		sx = sx + points[0][i];

		sy = sy + points[1][i];

		sz = sz + points[2][i];

		sxx = sxx + points[0][i]*points[0][i];

		syy = syy + points[1][i]*points[1][i];

		szz = szz + points[2][i]*points[2][i];

		syx = syx + points[0][i]*points[1][i];

		szx = szx + points[0][i]*points[2][i];

		szy = szy + points[1][i]*points[2][i];

	}

	*xg = sx/nbpoints;

	*yg = sy/nbpoints;

	*zg = sz/nbpoints;

	/* fprintf(stderr,"\n sxx=%f syy=%f szz=%f syx=%f szx=%f szy=%f nbpoints=%d",sxx,syy,szz,syx,szx,szy,nbpoints);*/

	a11 = sxx - (sx*sx)/nbpoints;

	a22 = syy - (sy*sy)/nbpoints;

	a33 = szz - (sz*sz)/nbpoints;

	a21 = syx - (sx*sy)/nbpoints;

	a31 = szx - (sz*sx)/nbpoints;

	a32 = szy - (sz*sy)/nbpoints;

	fprintf(stderr,"\n a11=%f a21=%f a22=%f a31=%f a32=%f a33=%f",a11,a21,a22,a31,a32,a33);

	res = diagonal(a11,a21,a22,a31,a32,a33,&l1,&l2,&l3,&p1x,&p1y,&p1z,&p2x,&p2y,&p2z,&p3x,&p3y,&p3z);

	fprintf(stderr,"\n res = %d",res);

	fprintf(stderr,"\n l1=%f l2=%f l3=%f p1x=%f p1y=%f p1z=%f p2x=%f p2y=%f p2z=%f p3x=%f p3y=%f p3z=%f",l1,l2,l3,p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z);

	if(res == 3)

	{

		/* determination de la plus petite valeur propre */



		if(l1 <= l2 && l1 <= l3)

		{

			norme = sqrt(p1x*p1x+p1y*p1y+p1z*p1z);

			*dx = p1x/norme;

			*dy = p1y/norme;

			*dz = p1z/norme;

			*erreur = l1;

			return(1);

		}

		else if(l2 <= l1 && l2 <= l3)

		{

			norme = sqrt(p2x*p2x+p2y*p2y+p2z*p2z);

			*dx = p2x/norme;

			*dy = p2y/norme;

			*dz = p2z/norme;

			*erreur = l2;

			return(1);

		}

		else

		{

			norme = sqrt(p3x*p3x+p3y*p3y+p3z*p3z);

			*dx = p3x/norme;

			*dy = p3y/norme;

			*dz = p3z/norme;

			*erreur = l3;

			return(1);

		}

	}

	else if(res == 2)

	if(l2 <= l1)

	{

		norme = sqrt(p2x*p2x+p2y*p2y+p2z*p2z);

		*dx = p2x/norme;

		*dy = p2y/norme;

		*dz = p2z/norme;

		*erreur = l2;

		return(1);

	}

	else

	{

		norme = sqrt(p1x*p1x+p1y*p1y+p1z*p1z);

		*dx = p1x/norme;

		*dy = p1y/norme;

		*dz = p1z/norme;

		*erreur = l1;

		return(1);

	}

	else if(res == 1)

	{

		norme = sqrt(p1x*p1x+p1y*p1y+p1z*p1z);

		*dx = p1x/norme;

		*dy = p1y/norme;

		*dz = p1z/norme;

		*erreur = l1;

		return(1);

	}

	else

	return(0);



}



/* test de l'approximation polygonale */

void testapp(void)

{

	float **points,x,y,z,xg,yg,zg,dx,dy,dz,erreur;

	int iok,npts,res;



	printf("\n voulez vous inserer un point (1 si oui 0 sinon)");

	scanf("%d",&iok);

	npts = 0;

	points = allotab2f(3,100);

	while(iok == 1)

	{

		printf("\n x,y,z : ");

		scanf("%f %f %f",&x,&y,&z);

		points[0][npts] = x;

		points[1][npts] = y;

		points[2][npts] = z;

		npts++;

		printf("\n voulez vous inserer un point (1 si oui 0 sinon)");

		scanf("%d",&iok);

	}

	res = approx(points,npts,&xg,&yg,&zg,&dx,&dy,&dz,&erreur);

	printf("\n %f %f %f %f %f %f %f",xg,yg,zg,dx,dy,dz,erreur);

}




/* test de l'approximation par un plan */

void testapplan(void)

{

	float **points,x,y,z,xg,yg,zg,dx,dy,dz,erreur;

	int iok,npts,res;





	printf("\n voulez vous inserer un point (1 si oui 0 sinon)");

	scanf("%d",&iok);

	npts = 0;

	points = allotab2f(3,100);

	while(iok == 1)

	{

		printf("\n x,y,z : ");

		scanf("%f %f %f",&x,&y,&z);

		points[0][npts] = x;

		points[1][npts] = y;

		points[2][npts] = z;

		npts++;

		printf("\n voulez vous inserer un point (1 si oui 0 sinon)");

		scanf("%d",&iok);

	}

	res = approxplan(points,npts,&xg,&yg,&zg,&dx,&dy,&dz,&erreur);

	fprintf(stderr,"\n %f %f %f %f %f %f %f",xg,yg,zg,dx,dy,dz,erreur);

}

/* test de l'approximation par un plan */

void testapplan2(void)

{

	float **points,x,y,z,xg,yg,zg,dx,dy,dz,erreur,r,a,b,c,d,dist;

	int npts,res,i;

	FILE *ptpoints;

	char ficpoints[50];


	fprintf(stderr,"\n fichier des coordonnees des points (nb de points, coordonnees des points: ");

	scanf("%s",ficpoints);

	ptpoints = fopen(ficpoints,"r");

	fscanf(ptpoints,"%d",&npts);

	points = allotab2f(3,npts);

	for(i=1;i<=npts;i++)

	{


		fscanf(ptpoints,"%f %f %f %f",&x,&y,&z,&r);

		points[0][i] = x;

		points[1][i] = y;

		points[2][i] = z;

	}

	res = approxplan(points,npts,&xg,&yg,&zg,&dx,&dy,&dz,&erreur);

	/* calcul des coefficients de l'equation du plan */

	a = dx;

	b = dy;

	c = dz;

	d = -dx*xg-dy*yg-dz*zg;

	fprintf(stderr,"\n a=%f b=%f c=%f d=%f",a,b,c,d);

	fprintf(stderr,"\n %f %f %f %f %f %f %f",xg,yg,zg,dx,dy,dz,erreur);

	fprintf(stderr,"\n distances entre le plan et les points");

	fprintf(stderr,"\n");

	for(i=1;i<=npts;i++)

		{

			x = points[0][i];

			y = points[1][i];

			z = points[2][i];

			dist = fabsf(a*x+b*y+c*z+d)/sqrt(a*a + b*b + c*c);

			fprintf(stderr,"\n (%f,%f,%f): %f",x,y,z,dist);

		}


}



/* prend en entree le nb de points et les 9 sommes et

rend le centre de gravite le vecteur directeur norme et l'erreur

rend 1 si OK et -1 si pb */



int rapprox(double sx,double sy, double sz,double sxx,

double syy,double szz,double syx,double szx,double szy,

double nbpoints,double *xg,double *yg,double *zg,

double *dx,double *dy,double *dz,double *erreur)

{

	double a11,a21,a22,a31,a32,a33,l1,l2,l3;

	double p1x,p1y,p1z,p2x,p2y,p2z,p3x,p3y,p3z,norme;

	int res;



	*xg = sx/nbpoints;

	*yg = sy/nbpoints;

	*zg = sz/nbpoints;

	a11 = sxx - (sx*sx)/nbpoints;

	a22 = syy - (sy*sy)/nbpoints;

	a33 = szz - (sz*sz)/nbpoints;

	a21 = syx - (sx*sy)/nbpoints;

	a31 = szx - (sz*sx)/nbpoints;

	a32 = szy - (sz*sy)/nbpoints;

	res = diagonal(a11,a21,a22,a31,a32,a33,&l1,&l2,&l3,&p1x,

	&p1y,&p1z,&p2x,&p2y,&p2z,&p3x,&p3y,&p3z);

	if(res == -1)return(-1);

	/* printf("\n res = %d l1 = %f l2 = %f l3 = %f",res,l1,l2,l3);*/

	if(res == 3)

	{

		/* determination de la plus petite valeur propre */



		if(l1 >= l2 && l1 >= l3)

		{

			norme = sqrt(p1x*p1x+p1y*p1y+p1z*p1z);

			*dx = p1x/norme;

			*dy = p1y/norme;

			*dz = p1z/norme;

			if(fabs(l2) <= fabs(l3))

			*erreur = fabs(l2);

			else *erreur=fabs(l3);

			return(1);

		}

		else if(l2 >= l1 && l2 >= l3)

		{

			norme = sqrt(p2x*p2x+p2y*p2y+p2z*p2z);

			*dx = p2x/norme;

			*dy = p2y/norme;

			*dz = p2z/norme;

			if(fabs(l1) <= fabs(l3))

			*erreur = fabs(l1);

			else *erreur=fabs(l3);

			return(1);

		}

		else

		{

			norme = sqrt(p3x*p3x+p3y*p3y+p3z*p3z);

			*dx = p3x/norme;

			*dy = p3y/norme;

			*dz = p3z/norme;

			if(fabs(l1) <= fabs(l2))

			*erreur = fabs(l1);

			else *erreur=fabs(l2);

			return(1);

		}

	}

	else if(res == 2)

	if(l1 <= l2)

	{

		norme = sqrt(p2x*p2x+p2y*p2y+p2z*p2z);

		*dx = p2x/norme;

		*dy = p2y/norme;

		*dz = p2z/norme;

		*erreur = fabs(l1);

		return(1);

	}

	else

	{

		norme = sqrt(p1x*p1x+p1y*p1y+p1z*p1z);

		*dx = p1x/norme;

		*dy = p1y/norme;

		*dz = p1z/norme;

		*erreur = fabs(l2);

		return(1);

	}

	else if(res == 1)

	{

		norme = sqrt(p1x*p1x+p1y*p1y+p1z*p1z);

		*dx = p1x/norme;

		*dy = p1y/norme;

		*dz = p1z/norme;

		*erreur = 0;

		return(1);

	}

	else

	return(0);



}



double erapprox(double sx,double sy, double sz,double sxx,

double syy,double szz,double syx,double szx,double szy,

double nbpoints)

{

	double a11,a21,a22,a31,a32,a33,l1,l2,l3;

	double erreur,r1,r2,r3,a,b,c;

	int res;



	a11 = sxx - (sx*sx)/nbpoints;

	a22 = syy - (sy*sy)/nbpoints;

	a33 = szz - (sz*sz)/nbpoints;

	a21 = syx - (sx*sy)/nbpoints;

	a31 = szx - (sz*sx)/nbpoints;

	a32 = szy - (sz*sy)/nbpoints;



	a = -a22-a33-a11;

	b = a22*a11+a33*a11+a22*a33-a32*a32-a21*a21-a31*a31;

	c= -a11*a22*a33+a32*a32*a11+a21*a21*a33-a21*a31*a32

		-a31*a21*a32+a22*a31*a31;



	res = equa3(a,b,c,&r1,&r2,&r3);

	/* on considere que les racines non nulles */



	if(res == 3)

	{

		if(r1 == 0 || r2 == 0 || r3 == 0)

		{

			if(r1 == 0 && r2 != 0 && r3 != 0)

			{

				r1 = r2;

				r2 = r3;

				res = 2;

			}else if(r2 == 0 && r1 != 0 && r3 != 0)

			{

				r2 = r3;

				res = 2;

			}

			else if(r3 == 0 && r1 != 0 && r2 != 0)

			{

				res = 2;

			}else if(r1 == 0 && r2 == 0 && r3 != 0)

			{

				r1 = r3;

				res = 1;

			}

			else if(r1 == 0 && r3 == 0 && r2 != 0)

			{

				r1 = r2;

				res = 1;

			}

			else if(r2==0 && r3==0 && r1 != 0)

			{

				res = 1;

			}

			else

			{

				res = 0;

			}

		}

	}else if(res == 2)

	{

		if(r1 == 0 && r2 == 0)res = 0;

		else if(r1 == 0)

		{

			r1 = r2;

			res = 1;

		}else if(r2 == 0)

		{

			res = 1;

		}

	}

	else if(res == 1)

	{

		if(r1 == 0)res=0;

	}

	l1 = r1;

	l2 = r2;

	l3 = r3;



	if(res == 3)

	{

		/* determination de la plus grande valeur propre */



		if(l1 >= l2 && l1 >= l3)

		{

			if(l2 <= l3)

			erreur = fabs(l2);

			else erreur = fabs(l3);



			return(erreur);

		}

		else if(l2 >= l1 && l2 >= l3)

		{

			if(l1 <= l3)

			erreur = fabs(l1);

			else erreur = fabs(l3);

			return(erreur);

		}

		else

		{

			if(l1 <= l2)

			erreur = fabs(l1);

			else erreur = fabs(l2);

			return(erreur);

		}

	}

	else if(res == 2)

	if(l1 <= l2)

	{

		erreur = fabs(l1);

		return(erreur);

	}

	else

	{

		erreur = fabs(l2);

		return(erreur);

	}

	else if(res == 1)

	{

		erreur = 0;

		return(erreur);

	}

	else

	return(0);



}

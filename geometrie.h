/*
 * geometrie.h
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

#define _USE_MATH_DEFINES


#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include<stdbool.h>



/* definition d'une boule */

void histoboules(char *boules,char *commandes,int dimax);

void testhistoboules(void);

void testfig8(void);



typedef struct bowl

{

	/* coordonnes du centre de la boule */

	float xc ;

	float yc ;

	float zc ;



	/* rayon de la boule */

	float rayon ;



}boule ;

float *viscar(char *ficgraphe,int dimx, int dimy,int dimz,int num,char *ficimage,char *ficboulescar);

void testviscar();

/* definition d'une image volumique de type entier */

typedef struct imag

{

	int dimx ;

	/* dimension en X */



	int dimy ;

	/* dimension en Y */



	int dimz ;

	/* dimension en Z */



	int ***tab;

	/* tableau contenant l'image */



}image ;



typedef struct imagc

{

	int dimx ;

	/* dimension en X */



	int dimy ;

	/* dimension en Y */



	int dimz ;

	/* dimension en Z */



	bool ***tab;

	/* tableau contenant l'image */



}imagec;

typedef struct imagf

{

	int dimx ;

	/* dimension en X */



	int dimy ;

	/* dimension en Y */



	int dimz ;

	/* dimension en Z */



	float ***tab;

	/* tableau contenant l'image */



}imagef;



/* definition d'un cylindre fini */

typedef struct cylinder

{

	/* rayon du cylindre */

	float rayon ;



	/* axe du cylindre defini par deux points */

	float x1,y1,z1 ;

	float x2,y2,z2 ;



}cylindre ;



/* definition d'un morceau de cone */

typedef struct cona

{

	/* segment formant l'axe du cone */

	float x1,y1,z1 ;

	float x2,y2,z2 ;



	/* rayons aux deux extremites, si ces rayons sont egaux

	on a un cylindre */

	float rayon1,rayon2 ;

}cone ;



/* definition d'un tetraedre avec des points de coordonnees

entieres */

typedef struct tetraedre

{

	/* coordonnees des 4 sommets du teraedre */

	int x1,y1,z1;

	int x2,y2,z2;

	int x3,y3,z3;

	int x4,y4,z4;



	/* equation du tetraedre : ax + by +cz - d > 0

	; si pas de calcul de l'equation tous les coeff. sont � 0 */

	float a1,b1,c1,d1;

	float a2,b2,c2,d2;

	float a3,b3,c3,d3;

	float a4,b4,c4,d4;



}tetra ;



/* definition de la structure de tas utilise pour le

regroupement hierarchique */

typedef struct heap

{

	int nmaxtas ;

	/* nombre d'elements maximal du tas */



	int nt ;

	/* nombre d'elements du tas */



	int *itas ;

	/* tableau contenant les indices dans le tableau arc des couples de

	noeuds inclus dans le tas ; les arcs d'indices itas(1),itas(2)... dans le

	tableau arc forment un tas de fonction d'ordre fta(caracteristiques des deux

	noeuds,caracteristiques de l'arc). cette fonction definit l'ordre de

	priorit� de fusion des couples */



	double *val ;

	/* tableau contenant les valeurs de la fonction de cout

	pour les elements du tas (des arcs du graphe)

	val(i) = fta(carac(couple(1,itas(i)),.),carac(couple(2,itas(i)),.) */



	double *vacou ;

	/* tableau contenant les valeurs courantes de la fonction de cout

	pour les arcs ; ce tableau est initialise a -1 */



	int *label ;

	/* ce tableau est initialise a label(i)=i et a chaque fusion si la reunion

	des noeuds i et j devient j alors on affecte i a label(j) : label(j) = i */



}tas ;


typedef struct heap_s

{

	int nmaxtas ;

	/* nombre d'elements maximal du tas */



	int nt ;

	/* nombre d'elements du tas */



	int *itas ;

	/* tableau contenant les indices dans le tableau arc des couples de

	noeuds inclus dans le tas ; les arcs d'indices itas(1),itas(2)... dans le

	tableau arc forment un tas de fonction d'ordre fta(caracteristiques des deux

	noeuds,caracteristiques de l'arc). cette fonction definit l'ordre de

	priorit� de fusion des couples */



	float *val ;

	/* tableau contenant les valeurs de la fonction de cout

	pour les elements du tas (des arcs du graphe)

	val(i) = fta(carac(couple(1,itas(i)),.),carac(couple(2,itas(i)),.) */



	float *vacou ;

	/* tableau contenant les valeurs courantes de la fonction de cout

	pour les arcs ; ce tableau est initialise a -1 */



	int *label ;

	/* ce tableau est initialise a label(i)=i et a chaque fusion si la reunion

	des noeuds i et j devient j alors on affecte i a label(j) : label(j) = i */



}tas_s ;




/* definition de la structure de graphe value utilisee pour le regroupement

hierarchique */

typedef struct graph

{

	int nbnodemax,nbarcmax ;

/* nombre maximum de noeuds et d'arcs du graphe */



	int nbnoeuds;

/* nombre de noeuds du graphe */



	int ncar;

/*	nombre d'attributs associes respectivement aux noeuds */



	int nel,nco;

/*	nombre de noeuds et nombre d'arcs effectifs au depart

des regroupements, ensuite ce nombre decroit mais n'est pas

mis a jour */





	double **carac ;

/* tableau contenant les attributs des noeuds, carac[i][j] est la i-ieme

composante du vecteur attribut du noeud j */



	int *liste;

/*	tableau contenant des pointeurs sur les tetes des listes d'arcs

correspondant aux noeuds ; liste[i] = tete de la liste des arcs contenant i (liste contenue

dans le tableau couple) ; si la liste est vide liste[i] = 0 ; si le noeud n'existe plus

liste[i] = -1 */



	int **couple ;

/* tableau contenant les listes doublement chainees de couples de noeuds

connexes (arcs), couple[1,p] = noeud numero 1 de l'arc p ;

couple[2,p] = noeud numero 2 de l'arc p ; couple(3,p) = pointeur sur l'arc suivant

dans la liste de l'element 1 ;

; couple(4,p) = pointeur sur l'arc precedent dans la liste de l'element 2 ;

couple(5,p) = pointeur sur l'arc suivant dans la liste de l'element 2 ;

couple(6,p) = pointeur sur l'arc precedent dans la liste de l'element 2 */





	int *licou ;

/* tableau contenant la liste chainee des cases libres du tableau couple ;

elle est initialisee avec arc en entier */



	int ncou ;

/* tete de la liste des cases libres du tableau couple contenue dans licou */





} graphe;

typedef struct graph_s

{

	int nbnodemax,nbarcmax ;

/* nombre maximum de noeuds et d'arcs du graphe */



	int nbnoeuds;

/* nombre de noeuds effectifs du graphe */



	int ncar;

/*	nombre d'attributs associes respectivement aux noeuds */



	int nel,nco;

/*	nombre de noeuds et nombre d'arcs effectifs au depart

des regroupements, ensuite ce nombre decroit mais n'est pas

mis a jour */





	float **carac ;

/* tableau contenant les attributs des noeuds, carac[i][j] est la i-ieme

composante du vecteur attribut du noeud j */



	int *liste;

/*	tableau contenant des pointeurs sur les tetes des listes d'arcs

correspondant aux noeuds ; liste[i] = tete de la liste des arcs contenant i (liste contenue

dans le tableau couple) ; si la liste est vide liste[i] = 0 ; si le noeud n'existe plus

liste[i] = -1 */



	int **couple ;

/* tableau contenant les listes doublement chainees de couples de noeuds

connexes (arcs), couple[1,p] = noeud numero 1 de l'arc p ;

couple[2,p] = noeud numero 2 de l'arc p ; couple(3,p) = pointeur sur l'arc suivant

dans la liste de l'element 1 ;

; couple(4,p) = pointeur sur l'arc precedent dans la liste de l'element 2 ;

couple(5,p) = pointeur sur l'arc suivant dans la liste de l'element 2 ;

couple(6,p) = pointeur sur l'arc precedent dans la liste de l'element 2 */





	int *licou ;

/* tableau contenant la liste chainee des cases libres du tableau couple ;

elle est initialisee avec arc en entier */



	int ncou ;

/* tete de la liste des cases libres du tableau couple contenue dans licou */





} graphe_s;


/* prototypes */

int taillent(char *entete);

void diffusiong3(graphe_s **graf,int numatt,float deltat,float cdif,float taillev,int *a,int *b,float *c,int opt,int ncor);

void testtaillent(void);

void ecrtab(float *tab);

double bilanmasses(graphe_s *graphe);

float *miseajourdif(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax); 

void testsimulsynch();

void transformer(char *entree,char *sortie);

void visuboul2_s(char *boules,char *imaboul,char *entete,int dimx,int dimy,int dimz);

void testvisboul2_s(void);

void testtrans(void);

void stocker(graphe *graf,tas *tas1,char *cylindres,char *cyl);

int itrouv_s(int nel1,int nel2,graphe_s *graphe1);

bool plat(tetra *ptetra);

void testdrainageval();

void drainageval(char *ficdrai,int dimx,int dimy, int dimz, float seuil, float dseuil,int nbmaxvois,char *ficboules);

void fta3(double *car1,double *car2,int ncar,float seuil,int *regroup,double *cout);

void voisins(int n,graphe_s *graphe1,int nvmax,int *nvois,int **vois);

graphe_s *cregraphe_s4_val(char *cyl,char *arcs);

void ficdrain_val(graphe_s *graphe,int **trait,char *fic,float seuil);

void swopta_s(float *val,int *itas,int n,int in,int *iout,float valin,float *valout);

void testcgraf1_s(void);

int approxplan(float **points,int nbpoints,float *xg,float *yg,float *zg,

float *dx,float *dy,float *dz,float *erreur);

void testapplan(void);

void crevoisins(graphe_s *graphe1, char *fic, int nivmax,int nmaxvois,int nmaxniv);

void testapplan2(void);

void stockboules_s(graphe_s *graf, char *cyl2, int opt);

void testtri4(void); 

void outas_s(float *val,int *itas,int *n,int *iout,float *valout);

void intas_s(float *val,int *itas,int *n,int in,float valin);

void testcgraf_s(void);

void ctas3(graphe *graphe1,tas *tas1,float seuil);

void hiera3(graphe *graphe1,tas *tas1,double seuil);

graphe_s *cregraphe_satt(char *cyl,char *arcs); 

void testcgraf3(void);

void fusion3(int n1,int n2,graphe *graphe1,tas *tas1,float seuil);

void testvisorient(void);

int **compconnexesvalbis(graphe_s *graphe);

int connexb(int n1, int n2, float *car1, float * car2, float seuil);

void cretas_s(float *val,int *itas,int n);

void visorient(char *boules,char *imaboul,char *entete,int dimx,int dimy,int dimz,float ep);

int bouleform(boule *ptboule,image *ptima,int etiq,int pourcentage);

void creerattval(char *ficdonnees, char *ficboules,char *ficatt,float massedom,float rmax);

void testcreerattval(); 

bool bouleformb(boule *ptboule,imagec *ptima,int pourcentage);

void testsimulsynchcourbopt();

graphe_s *simulsynchcourbopt(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel,float eps);

void testsimulsynchcourbas(); 

void testsimulsynchcourbasas();

graphe_s *simulsynchcourbasas(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel);

graphe_s *simulsynchcourbas(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel);

void creeratt5(char *ficdonnees, char *ficboules,char *ficatt);

void creeratt6(char *ficdonnees, char *ficboules,char *ficatt); 

void courbesvalerie(); 

void testcalib(); 

graphe_s *simulsynchas(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,char *ficgrapher);

void testcreeratt5(); 

void testcreeratt6(); 

void creerattdiff(char *ficboules,char *ficatt,float massedom,int nbplans); 

graphe_s *simulsynchcourboptdiff(char *ficgraphe,float coeffdom,float deltat,int fraction,int nvmax,char *ficmatlab,float eps,int dimz);

void testcreerattdiff(); 

void creeratt6(char *ficdonnees, char *ficboules,char *ficatt); 

void testsimulsynchcourbimp();

graphe_s *simulsynchcourbimp(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel,float eps);

void testmiseajourdifimplic(); 

int GradienConjugue(float *matA,float *matC,float *b, float *x, int *ptA, int *colA, int nblgA, int nbA, int *ptC, int *colC, int nbC, int nbitermax,float eps,int iAtype,int iCtype);

void testmiseajourdifimp();

void miseajourdifopt(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax,float eps,float *mat,int *ptligne,int *colonne,int nmat,int nbnoeud,float *matC,int *ptC, int *colC, int nbC,float *volume);

void miseajourdifimplic(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax,float eps); 

float prodscal (float *x, float *y, int n);

void sommevect(float *x, float *y, float *z, int n);

void opposevect(float *x, float *y, int n);

void scalvect(float *x, float scal, float *y, int n);

void prodmatvect(float *matA, int *ptA, int *colA, float *x, float *y, int nblgA, int nbA,int itype); 

void precondmat(float *matA,float *matC, int *ptA,int *colA,int nblgA,int nbA,int *ptC,int *colC,int nbC,int iAtype,int iCtype);

void visuboul2(char *boules,char *imaboul,char *entete,int dimx,int dimy,int dimz);

float *miseajourdifimp(graphe_s *graphe,int numcar,float coeff,float deltat,int nvmax,char *ficent);

void triboul2(void);

float **miseajourbio(graphe_s *graphe,float deltat,float ro,float mu,float rom,float vfom,float vsom,float vdom,float kab);

float **miseajourbioas(graphe_s *graphe,float deltat,float ro,float mu,float rom,float vfom,float vsom,float vdom,float kab);

void testvisboul2(void);

void triinsertion(int *t,int n);

void filtre(void);

void triboulval(void); 

void selecboules2(char *ficboul,char *ficboulm,float seuil,int iopt);

void filtral2(char *ficboul,char *ficfiltre,float seuil,float seuil2);

void arcs (char *cyl,char *cyl2,int dimx, int dimy, int dimz,int *nbarcs,int nmaxv);

void freet3b(bool ***tab,int dimx,int dimy,int dimz);

void triboules4(char *ficboul,char *ficboul1);

void testcregraphsoilmu3d();

void testcalsommes3f(); 

void testcregraphsoilmu3d2();

int boulimagf(boule *ptboule,imagef *ptimage,float seuil); 

void arcsval(char *cyl,char *cyl2,int dimx, int dimy, int dimz,int *nbarcs,int nmaxv);

void testcompconnexes();

void myfreeLf(long double *pt); 

void freet2b(bool **tab,int dimx,int dimy);

void creeratt(char *ficdonnees, char *ficboules,char *ficatt);

imagef *creimagef(int dimx,int dimy,int dimz,float valeur); 

void myfreeb(bool *pt); 

void testcreeratt(); 

void testtri2(void);

void triboules2(char *ficboul,char *ficboul1,char *ficboul2,int nboules);

void triboules4(char *fic1,char *fic2); 

void testfig5(void);

int appcyl(cylindre *cyl,int x,int y,int z);

void freeim(image *im);

void testdif(void);

void coeffdif(char *vois,char *boulesmax,float n, int nmax,char *coeff);

void calsommes3f(char *ficgraphe,char *ficcourbes); 

void comparim(char *nom1,char *nom2,int dimx,int dimy,int dimz,int tentete, char *entete,char *nom12,char *nom21,char *ficres);

void testcomparim(void);

void ecrat(float **attributs1,int nboules);

void arcssimp(char *cyl,char *cyl2,int dimx, int dimy, int dimz,int *nbarcs,int nmaxv,float seuilr);

void arcssimpdelau(char *cyl,char *cyl2,int dimx, int dimy, int dimz,int *nbarcs,int nmaxv,float seuilr,char *mesh,float seuilp);

void testcregraphsoilmu3d3();

imagec *fic2imagb(char *nomim, int dimx, int dimy, int dimz);

void testdrainagesimp();

void fdifenz(int n,int *tavois,int nvois,graphe_s **graphe);

void calcatt(char *ficboules, char *donnees, char *ficatt,void (*fonc)(float,float,float,float,float*,int,double,int*,float**));

void intercalatt();

graphe_s *simulsynch(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,char *ficgrapher);

float *calsommes(char *ficatt,char *boulesmax,int *natt);

float *calsommes2(graphe_s *graphe);

void simulmepsom2();

void testdiffusionexp();

void fdidom(int n,int *tavois,int nvois,graphe_s **graphe);

void diffusiong(char *ficgraphe,int nvmax,void (*fonc1)(int,int*,int,graphe_s**),int nbpas,char *ficatt2);

void diffusionexp(char *ficgraphe1,int numcar,float coeff,float deltat,int fraction,int nvmax,char *ficgraphe2);

float compar(float **attributs1,float **attributs2,int nboules,int natt);

void diffusion(char *ficcoeff,char *boulesmax,char *ficatt,float (*fonc1)(float**,float**,int), void (*fonc2)(float,float**,int),char *ficatt2);

void lancdif();

void transpboul(); 

void lancdifgr();

void verifdrain(graphe_s *graphe,int **trait,float seuil,int *res);

void simulmepsom3();

int **drainit2(graphe_s *graphe,float seuil,float dseuil,int dimx,int dimy,int dimz);

void draintrait2(graphe_s *graphe,int ***trait,float seuil,int *nvid);

graphe_s *cregraphe_s4(char *cyl,char *arcs);

float f1enz(float **car,float **car2,int ncar);

void testfig7(void); 

void f2enz(float m,float **car,int ncar);

void opdxtoml(char *nomima,char *nomfic,int tentete);

float f1dom(float **car,float **car2,int ncar);

void comparcarb(char *ficourbes,char *ficmatlab,int njours,int fracjour);

void f2dom(float m,float **car,int ncar);

void mangedom(float x,float y,float z,float ray,float **car,int natt);

void selecpores(char *boulesmax,char *ficatt,char *figraphe,int (*drain)(float,float,float,float,float*,int,graphe_s*),char *boules2,char *ficatt2);

float seuil_ext,nmax_ext,sombio_ext,k1_ext,k2_ext,fr_ext;

double som_ext;

void pesticides_sl2(float x,float y,float z,float ray,float **car,int natt);

int drainyoung(float x,float y,float z,float r,float *car,int natt,graphe_s *graphe);

void decompos(float x,float y,float z,float ray,float **car,int natt);

void prodenzy(float x,float y,float z,float r,float **car,int natt);

void miseajour(char *boulesmax,char *ficatt,void (*misaj)(float,float,float,float,float**,int),char *ficatt2);

void diffusiong4(graphe_s **graf,int numatt,int nbiter,float taillev);

void testmisaj();

void coeffdif1(char *vois,char *boulesmax,float n,int nmax,char *coeff);

void lancdifit();

float f12(float **car,float **car2,int ncar);

double r_ext,mu_ext,alpha_ext,cdom_ext,kb_ext;

void mangedom_sl2(float x,float y,float z,float ray,float **car,int natt);

void transform();

double *calsommes3(graphe_s *graphe);

void courbmatlab(char *ficourbes,char *ficmatlab,int njours,int fracjour,char *ficexcel);

float taillev_ext,cdif_ext,pourdom_ext;

int deltasim_ext,fracjour_ext,trace1_ext,trace2_ext;

void microorg_sl2(float x,float y,float z,float ray,float **car,int natt);

void fdidom_sl2(int n,int *tavois,int nvois,graphe_s **graphe);

void diffusionit(char *ficoeff,char *boulesmax,char *ficatt,float (*fonc1)(float**,float**,int), void (*fonc2)(float,float**,int),char *ficatt2,int nbit);

void f2(float m,float **car,int ncar);

void simulmepsom();

void testcreerim3();

void creerim3(char *nomfic,char *nomfic2,char *nomfic3, int dimx,int dimy,int dimz,int xo,int yo,int zo,int dx,int dy,int dz);

void respir(float x,float y,float z,float r,float **car,int natt);

void microorg(float x,float y,float z,float r,float **car,int natt);

void resyst3(float a1,float b1,float c1,float d1,float a2,

float b2,float c2,float d2,float a3,float b3,float c3,float d3,

int *sol,float *x,float *y,float *z);

void freetfloat(float **tab,int dimx,int dimy);

graphe *arcsjud (char *cyl,int rapan, int ncar);

void testarcsjud(void);

int fonction ( int (*fonc)() );

int test (int var);

int testmain();

int optcarb_ext;

char *mymalloc(int taille);

char *mycalloc(int nb,int taille);

void myfreec(char *pt);

void testtrans2(void);

void testaccessval();

void transformval();

void comptepoints();

void testgrapheaccess();

void grapheaccess(char *ficgraphe1, char *ficgraphe2, int nbpatchs, int n1, int n2);

void calculaccessval(graphe_s *graphe,graphe_s *graphebio,char *ficmatlab,int nvmax);

void transformer2(char *entree,char *sortie);

void calcatt2(char *ficboules, char *donnees, char *ficatt,void (*fonc)(float,float,float,float,float*,int,double,float,float,int*,float**));

void myfreei(int *pt);

void calattributs2(float x,float y,float z,float r,float *tadonnees,int nattg,double somboules,float sommme1,float somme2,int *natt,float** tabatta);

void myfreef(float *pt);

void myfreed(double *pt);

float *dijkstra(graphe_s *graphe,int numnoeud,int nvmax,int opt);

float cout(int n1, int n2, graphe_s *graphe,int opt);

void testdijkstra(); 

void testaccess();

float calculaccess(graphe_s *graphe,int nvmax,int opt); 

void calcatt3(char *ficboules, char *donnees, char *ficatt);

void freetd(double **tab,int nli,int nco);

int **allot2i(int dimx,int dimy);

void compconnexes(graphe_s *graphe,char *compco,char *compcor,int *nbcompco);

void compconnexesval(graphe_s *graphe,char *fcompco,char *fcompcor,int *nbcompco);

bool **allot2ic(int dimx,int dimy);

int **allotab2i(int nli,int nco);

void freeti(int **tab,int nli,int nco);

float **allotab2f(int nli,int nco);

void freetf(float **tab,int nli,int nco);

float x0_ext,y0_ext,z0_ext,dimx_ext,dimy_ext,dimz_ext;

double **allotab2d(int nli,int nco);

int ***allotab3di(int dimx,int dimy,int dimz) ;

image *cimage3dint(int dimx,int dimy,int dimz);

float det3(float a1,float a2,float a3,float a4,float a5,float a6,

float a7,float a8,float a9);

float inac_ext;

long double det3d(long double a1,long double a2,long double a3,long double a4,long double a5,long double a6,

long double a7,long double a8,long double a9);

int inscylind(image *ima,cylindre *cyl,int color);

int appcone(cone *con,int xi,int yi,int zi);

int testim(void);

void testtri3();

double ec_ext;

void courbcarb(char *ficourbes,char *ficarbone,int n,int p,int k,int j);

int boulimage(float x0,float y0,float z0,float r,image *ptimage,int seuil);

int ***creertab3(int dimx,int dimy,int dimz);

bool ***creertab3c(int dimx,int dimy,int dimz);

void freet3(int ***tab,int dimx,int dimy,int dimz);

int boulimagb(boule *ptboule,imagec *ptimage,bool seuil);

image *creimage(int dimx,int dimy,int dimz,int valeur);

imagec *creimagec(int dimx,int dimy,int dimz,bool valeur);

image *fondaun(image *im);

char *mymalloc(int taille);

int ecrimagec(char *nomfic,char *nomfic2,imagec *ptimage);

void testsimulsynchcourbasync();

int **centresnew(char *bmax,int *nbpts);

void testedpnew(void);

graphe_s *simulsynchcourbasync(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int fracbio, int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *ficexcel);

int ecrimageval(char *nomfic,imagec *ptimage);

void testvisuboulval(void); 

graphe_s *cregraphe_s3(char *cyl,char *arcs,char *attributs);

void crenoeud_s2(graphe_s *graphe1,float *car1,int ncar);

int derboule_ext;

int ecrimage(char *nomfic,char *nomfic2,image *ptimage);

int nbpointim(char *nom1,int dimx,int dimy,int dimz,int tentete);

void testnbpointim(void);

void testfig(void);

int boulimag(boule *ptboule,image *ptimage,int seuil);

int inscone(image *ima,cone *con,int color);

boule **generboules(float x1,float y1,float z1,float x2,float y2,

float z2,float *rayons,int nbrayons,int *nbboules);

void testecor(void);

void testecor2(void);

void testecorval(void); 

int **ecorce(char *nomim,int tentete,int etiq,int dimx,int dimy,int dimz,int nmaxecor,int *nbpts);

int **ecorce2(char *nomim,int tentete,int etiq,int dimx,int dimy,int dimz,int nmaxecor,int *nbpts);

int **ecorceval(char *nomim,int etiq,int dimx,int dimy,int dimz,int nmaxecor,int *nbpts);

image *cimagtab(int **points,int nbpts,int forme,int vide,

int dimx,int dimy,int dimz);

void creerfic(char *nom,int **points, int nbpts);

boule *tetrasphere(tetra *tet);

int eqplan(int x1,int y1,int z1,int x2,int y2,int z2,int x3,

int y3,int z3,float *a,float *b,float *c,float *d);

void boiteboul(boule *ptboule,int *x0,int *y0,int *z0,int *dx,

int *dy, int *dz);

void boitetra(tetra *tet,int *x0,int *y0,int *z0,int *dx,

int *dy, int *dz);

int aptetra(int x,int y,int z,tetra *tet);

int eqtetra(tetra *tet);

int aplan(float x,float y,float z,float a,float b,float c,

float d);

void testetra(void);

int instetra(image *ima,tetra *tet,int color);

void ecrtetra(tetra *tet);

void lfictet(char *nomft,float ***points,int ***ltet,int *nbtet,

int *nbs,float *xmax,float *ymax,float *zmax);

int neari(float val);

float calmin(float x1,float x2,float x3,float x4,float x5,float x6);

graphe_s *simulsynchcourb(char *ficgraphe,char *ficparbio,float coeffdom,float deltat,int fraction,int nvmax,int pourcneg,char *ficgrapher,char *ficmasses,char *ficmatlab,char *excel);

void calattributs(float x,float y,float z,float r,float *tadonnees,int nattg,double somboules,int *natt,float** tabatta);

float calmax(float x1,float x2,float x3,float x4,float x5,float x6);

void testsimulsynchcourb(); 

void boitecyl(cylindre *ptcyl,int *x0,int *y0,int *z0,int *dx,

int *dy,int *dz);

float calmin4(float x1,float x2,float x3,float x4);

float calmax4(float x1,float x2,float x3,float x4);

void fic2scilab(char *fic,char *scilab,int dimx,int dimy,int valmax);

void testfic2scilab(void);

void histo(char *ficyl,char *axevol,char *axelong,char *raylong,char *ray,int pasdis);

void viscylini2(char *ficyl,char *imacyl,char *entete,

int dimx,int dimy,int dimz,float ray1,float raymax,float lmin,float volmin);

void testviscylini2(void);



float **allot2f(int dimx,int dimy);





/* procedures de creation d'un graphe value et de regroupemant

hierarchique des noeuds */



/* prototypes */

void testcreerim2();

void testhisto(void);

float ***creertab3f(int dimx,int dimy,int dimz);

void creerim2(char *nomfic,char *nomfic2,char *nomfic3,

int dimx,int dimy,int dimz,int xo,int yo,int zo,int dx,int dy,int dz);

void fcar1 (double *car1,double *car2,double *car3,int ncar);

void fcar1_t (double *car1,double *car2,double *car3,int ncar);

void fusion1(int n1,int n2,graphe *graphe1,tas *tas1,

float seuil);

void fusion1_t(int n1,int n2,graphe *graphe1,tas *tas1,

float seuil);
int **drainit(graphe_s *graphe,float seuil,float dseuil,int dimx,int dimy,int dimz);

void draintrait(graphe_s *graphe,int ***trait,float seuil,int *nvid);

int *calbords(graphe_s *graphe,float dseuil,int dimx,int dimy,int dimz);

void fusion2(int n1,int n2,graphe *graphe1,tas *tas1,

float seuil);

void fusion2_t(int n1,int n2,graphe *graphe1,tas *tas1,

float seuil);



int itrouv(int n1,int n2,graphe *graphe1);

int itrouv_t(int n1,int n2,graphe *graphe1);



void fta1(double *car1,double *car2,int ncar,float seuil1,int

 *regroup,double *cout);



 void fta1_t(double *car1,double *car2,int ncar,float seuil1,int

 *regroup,double *cout);



void ctas1(graphe *graphe1,tas *tas1,float seuil);

void ctas1_t(graphe *graphe1,tas *tas1,float seuil);

void ctas2(graphe *graphe1,tas *tas1,float seuil);

void ctas2_t(graphe *graphe1,tas *tas1,float seuil);



void hiera1(graphe *graphe1,tas *tas1,double seuil);

void hiera1_t(graphe *graphe1,tas *tas1,double seuil);

void hiera2(graphe *graphe1,tas *tas1,double seuil);

void hiera2_t(graphe *graphe1,tas *tas1,double seuil);

void fta2(double *car1,double *car2,int ncar,float seuil,

int *regroup,double *cout);

void fta2_t(double *car1,double *car2,int ncar,float seuil,

int *regroup,double *cout);



void fta3(double *car1,double *car2,int ncar,float seuil,

int *regroup,double *cout);



void fta3_t(double *car1,double *car2,int ncar,float seuil,

int *regroup,double *cout);





void intas(double *val,int *itas,int *n,int in,double valin);



void outas(double *val,int *itas,int *n,int *iout,double *valout);



void incou2(graphe *graphe1,int nel1,int nel2);

void incou2_t(graphe *graphe1,int nel1,int nel2);



char *mymalloc(int taille);



char *mycalloc(int nb,int taille);

void testcgraf2_t(void);

void myfree(char *pt);

void testcgraf_t(void);

void testtas(void);

graphe *cregraphe2_t(image *squel,int nsquel,float majrap,tas **tas1);

void verification(char *ficxavier,char *imageval,char *boulesmax);

void  init(graphe *graphe1,tas *tas1);

graphe *cregraphe_t(char *boules,float majrap,tas **tas1);

void testverification(); 

void freetd(double **tab,int nli,int nco);

void ecrgraphe_t(graphe *graphe1);

int **allotab2i(int nli,int nco);

void hiera1bis_t(graphe *graphe1,tas *tas1,double seuil);

void creerattvalbis(char *ficdonnees, char *ficboules,char *ficatt,float massedom,float rmax);

void testcreerattvalbis();

void freeti(int **tab,int nli,int nco);

void crenoeud_t(graphe *graphe1,double *car1,int ncar);

float **allotab2f(int nli,int nco);

void ficdrain(graphe_s *graphe,int **trait,char *fic,float seuil);

void ecrtas_t(tas *tas1,int nel);

void freetf(float **tab,int nli,int nco);

void ctas1bis_t(graphe *graphe1,tas *tas1,float seuil);

double **allotab2d(int nli,int nco);

void fusion1bis_t(int n1,int n2,graphe *graphe1,tas *tas1,float seuil);

graphe *creergraphe (int nbnomax,int nbarmax,int ncar) ;

graphe *creergraphe_t (int nbnomax,int nbarmax,int ncar) ;

tas *creertas (int ntasmax,int nbarmax,int nbnodemax) ;

void fta1bis_t(double *car1,double *car2,int ncar,

float lambda,int *regroup,double *cout);

graphe_s *cregraphe_val(char *cyl,char *arcs,char *attributs);

void crenoeud(graphe *graphe1,double *car1,int ncar);

tas *creertas_t (int ntasmax,int nbarmax,int nbnodemax);

void ecrigraphe(graphe_s *graphe1,char *fic);

void ecrgraphe(graphe *graphe1);

void stockcyl_t(graphe *graf,tas *tas,graphe *grafinit,char *fic,float majr);

void ecrtas(tas *tas1,int nel);

void testedp(void);

void testgraphe(void);

int **centres(char *bmax,int *nbpts);


graphe *cregraphe(char *boules,float majrap,tas **tas1);



void ecrgraffic(graphe *graphe1,char *ficgraf);

graphe_s *lecgraphe(char *fic);

void testmbe();

void testmbe2();

void testcgraf(void);

void calcatt4(char *ficboules, char *donnees, char *ficatt);

void miseajour2(graphe_s **graf,void (*misaj)(float,float,float,float,float**,int));

void stockcyl(graphe *graf,char *fic);

void diffusiong2(graphe_s **graf,int nvmax,void (*fonc1)(int,int*,int,graphe_s**),int nbpas);

void fdifenz(int n,int *tavois,int nvois,graphe_s **graphe);

void bilanmatmic(graphe_s *graphe);

int rapprox(double sx,double sy, double sz,double sxx,

double syy,double szz,double syx,double szx,double szy,double nbpoints,double *xg,double *yg,double *zg,

double *dx,double *dy,double *dz,double *erreur);



void fta2old(double *car1,double *car2,int ncar,

float lambda,int *regroup,double *cout);



double erapprox(double sx,double sy, double sz,double sxx,

double syy,double szz,double syx,double szx,double szy,

double nbpoints);



graphe *cregraphe2(image *squel,int nsquel,float majrap,tas **tas1);



int neari(float val);



void testcgraf2(void);





void fta1bis(double *car1,double *car2,int ncar,

float lambda,int *regroup,double *cout);



void hiera1bis(graphe *graphe1,tas *tas1,double seuil);



void testfinitcyl(void);



void ctas1bis(graphe *graphe1,tas *tas1,float seuil);



void fusion1bis(int n1,int n2,graphe *graphe1,tas *tas1,float seuil);



image *lecima(char *nomima,int dimx,int dimy,int dimz, int tentete);

void testcomparfic(void);

imagec *lecimac(char *nomima,int dimx,int dimy,int dimz, int tentete);

void stockcyl2(graphe *graf,char *fic);

void testvisboul(void);

void filtral(char *ficboul,char *ficfiltre,float seuil);

void testlecfic(void);

void testfig6(void);

void efictet(char *nomft,float **points,int **ltet,int nbtet,int nbs);

int appcyl(cylindre *cyl,int x,int y,int z);

void filtallong(void);

void seuilim(char *ima,int dimx,int dimy,int dimz,int tentete,int b1,int b2,

int val,char *imas,char *entete);

void testseuil(void);

void testseuil2(void);



void boitecyl(cylindre *ptcyl,int *x0,int *y0,int *z0,int *dx,

int *dy,int *dz);

void finitcyl(char *imsquel,int nbsquel,char *ficyl);

int inscylind(image *ima,cylindre *cyl,int color);





void initcyl(char *imsquel,int dimx,int dimy,int dimz,

int tentete,char *ficyl);

void finitcyl2(char *imsquel,int nbsquel,char *ficyl);

void triboul(void);

void visuboul(char *boules,char *imaboul,char *entete,int dimx,int dimy,int dimz);

void testtri(void);

void visucyl(char *cyl,char *imacyl,char *entete,int dimx,int dimy,int dimz);

void testviscyl(void);

void testtas(void) ;

void ecrire(double *val,int *itas,int n) ;


void swopta(double *val,int *itas,int n,int in,int *iout,double valin,double *valout);

void intas(double *val,int *itas,int *n,int in,double valin);

void outas(double *val,int *itas,int *n,int *iout,double *valout);

void cretas(double *val,int *itas,int n);

void suptas(double *val,int *itas,int *n,int j);

void rech(int *itas,int n,int ind,int *iplace);

void verif(int *itas,double *val,int n);

void testinitcyl(void);

int **ptcylind(cylindre *cyl,int *npoints);

void testviscylini(void);

void viscylini(char *ficyl,char *imacyl,char *entete,

int dimx,int dimy,int dimz,float ray1,float ray2);

void selecboules(char *ficboul,char *ficboulm,float seuil);

void triboules(char *ficboul,char *ficboul1,char *ficboul2,int nboules);

int connex(boule *boule1,boule *boule2,float seuil);

void testgraphe(void);

void testsim(void);

void finitcyl4(char *imsquel,int nbsquel,char *ficyl,float scal,float cosang);

void testfig(void);

void testfinitcyl4(void);

void squelette(char *ficboul,char *nomima,char *entete,int dimx,int dimy,int dimz,float seuil);

void testecor(void);

void testetra(void);

void testtri(void);

void testsquel(void);

void testreduboul(void);

void triboul(void);

void testvisboul(void);

void filtallong(void);

void reduboul(char *ficboul,char *ficboulm);

void testcgraf(void);

void testcgraf2(void);

void testboule(void);

void testcreerim();

void testequa3(void);

void selecboules3(char *ficboul,char *ficboulm,float seuil,int iopt);

void selecboules4(char *ficboul,char *ficboulm,float seuil,int iopt);

int boulimag2(boule *ptboule,image *ptimage,int seuil,int *nbint);

void testrac3(void);

void triboul5(void);

void selecboules5(char *ficboul,char *ficboulm,float seuil,int iopt,int dimx,int dimy,int dimz);

void selecboulesval(char *ficboul,char *ficboulm,float seuil,int dimx,int dimy,int dimz);

void testapp(void);

void testfinitcyl3(void);

void finitcyl3(char *imsquel,int nbsquel,char *ficyl,float scal);

void testviscyl(void);

void testseuil(void);

graphe_s *cregraphe_s2(char *cyl,char *arcs,tas_s **tas1,int nbmaxcomp,float rtas);

graphe_s *cregraphe_s(char *cyl,float majrap,tas_s **tas1,int nbmaxcomp);

void ctas1_s(graphe_s *graphe1,tas_s *tas1,float seuil);

void fta2_s(float *car1,float *car2,int ncar,float lambda,int *regroup,float *cout);

graphe_s *creergraphe_s (int nbnomax,int nbarmax,int ncar);

void crenoeud_s(graphe_s *graphe1,float *car1,int ncar);

void hiera1_s(graphe_s *graphe1,tas_s *tas1,float seuil,int nbmaxcomp);

void fusion1_s(int n1,int n2,graphe_s *graphe1,tas_s *tas1,float seuil,int nmaxcomp,int *debord);

void fcar1_s (float *car1,float *car2,float *car3,int ncar);

void fta1_s(float *car1,float *car2,int ncar,float seuil,int *regroup,float *cout);

tas_s *creertas_s (int ntasmax,int nbarmax,int nbnodemax);

void swopta_s(float *val,int *itas,int n,int in,int *iout,float valin,float *valout);

void incou2_s(graphe_s *graphe1,int nel1,int nel2);

void testseuil2(void);

void testinitcyl(void);

void testviscylini(void);

int plan(tetra *pttetra,float seuil);

void testfinitcyl(void);

void testfinitcyl2(void);

void testtas(void) ;

void testfig3(void);

void creerim(char *nomfic,char *nomfic2,char *nomfic3,int dimx,int dimy,int dimz);

int projorth(float x,float y,float z,float x1,float y1,float z1,float x2,

float y2,float z2,float *a,float *b,float *c);

void freetin(int **tab,int dimx,int dimy);

void triboul3(void);


void triboul4(void);



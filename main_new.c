/*
 * main.c
 *
 *  Created on: 5 juin 2009
 *      Author: monga
 */
/* fonctions de gestion d'un tas */



/* test des fonctions de gestion du tas */

#include <errno.h>

#include <stdio.h>

#include <stdlib.h>

#include <unistd.h>  /* unlink */

#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include "geometrie.h"


int main(void)

{
int iok;

/* nouvelles procedures juin-juillet 2020 */

fprintf(stderr,"DEBUT NOUVELLES PROCEDURES JUIN-JUILLET 2020 \n"); 

fprintf(stderr,"\n pour passer d'une image en format fichier d'entiers a un fichier de points pour la triangulation utiliser testecorval"); 

fprintf(stderr,"\n pour obtenir le fichier des boules de Delaunay des points frontieres qui sont dans la forme a partir des tetraedres utiliser testfig7"); 

fprintf(stderr,"\n pour trier les boules contenus dans un fichier de format: nombre de boules sur un entier, coordonnees centre et rayon en float, utiliser testtri4");

fprintf(stderr,"\n pour obtenir le nombre minimal de boules maximales a partir du fichier des boules triees, utiliser triboulesval "); 

fprintf(stderr,"\n pour obtenir un fichier image correspondant a un fichier de boules (points des boules à 0) utiliser testvisuboulval"); 

fprintf(stderr,"\n pour creer un fichier d'attributs complementaires pour les noeuds du graphe (MB,DOM,CO2 a 0) utiliser testcreeratt ");

fprintf(stderr,"\n pour creer un fichier de graphe a partir d'un fichier de boules et d'attributs complementaires utiliser testcregraphsoilmu3D ");

fprintf(stderr,"\n pour analyse des composantes connexes du graphe utiliser testcompconnexes "); 

fprintf(stderr,"\n pour test diffusion methode explicite utiliser testdiffusionexp "); 

fprintf(stderr,"\n pour visualiser resultats de la diffusion avec fichier et image utiliser testviscar");

fprintf(stderr,"\n pour creer un fichier d'attributs complementaires pour les noeuds du graphe (MB,DOM,SOM,FOM,CO2 a 0) utiliser testcreeratt5 ou testcreeratt6");

fprintf(stderr,"\n pour simuler avec schéma explicite synchrone utiliser testsimulsynch");

fprintf(stderr,"\n pour drainer avec methode de graphe en vidant a partir des bords utiliser testdrainageval");

fprintf(stderr,"\n pour drainer avec methode simplifiee de seuillage des rayons testdrainagesimp");

fprintf(stderr,"\n pour creer un fichier de graphe a partir d'un fichier de boules et d'attributs complementaires avec calcul arcs methode directe utiliser testcregraphsoilmu3D2 ");

fprintf(stderr,"\n pour generer un fichier courbes correspondant a un fichier graphe testcalsommes3f");

fprintf(stderr,"\n pour simuler avec schema explicite synchrone avec courbes echantillonnees en heures testsimulsynchcourb");

fprintf(stderr,"\n pour simuler avec schema explicite synchrone incluant transformations biologiques asynchrone avec courbes echantillonnees en heures testsimulsynchcourbas");

fprintf(stderr,"\n pour simuler avec schema explicite asynchrone (diffusion-transformation) avec courbes echantillonnees en heures testsimulsynchcourbasas");

fprintf(stderr,"\n pour generer fichier d'attributs a partir du format 'xavier' testcreerattval ");

fprintf(stderr,"\n pour generer fichier d'attributs a partir du format 'xavier' avec test composante connexe testcreerattvalbis ");

fprintf(stderr,"\n pour verifier le fichier des pattchs de microorganismes format 'xavier' testverification ");

fprintf(stderr,"\n pour simuler avec schema explicite asynchrone (diffusion-transformation) avec pas de temps differents simulation et biologie testsimulsynchcourbasync");

fprintf(stderr,"\n pour creation fichier de points des centres des boules pour Delaunay testedpnew");

fprintf(stderr,"\n pour creer un fichier de graphe a partir d'un fichier de boules et d'attributs complementaires avec calcul arcs methode directe et augmentation Delaunay utiliser testcregraphsoilmu3D3 ");

fprintf(stderr,"\n pour tester la fonction de formation de la matrice pour le schema implicite utiliser testmiseajourdifimp");

fprintf(stderr,"\n pour tester la fonction de formation simulant la diffusion avec le schema implicite utiliser testmiseajourdifimplic");

fprintf(stderr,"\n pour tester la fonction de simulation avec diffusion schema implicite utliser testsimulsynchcourbimp ");

fprintf(stderr,"\n pour tester la fonction de simulation avec diffusion schema implicite optimise utliser testsimulsynchcourbopt ");

fprintf(stderr,"\n pour generer instructions matlab pour courbes de Valerie utiliser courbesvalerie");

fprintf(stderr,"\n pour generer un fichier d'attributs pour calibrage diffusion avec donnees Valerie utiliser testcreerattdiff");

fprintf(stderr,"\n pour calibration diffusion avec donnees Valerie utiliser testcalib"); 

fprintf(stderr,"\n pour test algorithme de Dijkstra  utiliser testdijkstra"); 

fprintf(stderr,"\n pour calcul coefficient accessibilite matiere organique testaccess"); 

fprintf(stderr,"\n pour generer graphes avec patchs de matiere organique testgrapheaccess"); 

fprintf(stderr,"\n pour analyser le critere d'accessibilite testaccessval"); 

fprintf(stderr,"\n pour transformer fichier image de Valerie transformval"); 

fprintf(stderr,"\n pour compter les points de la forme dans une image comptepoints"); 


fprintf(stderr,"FIN NOUVELLES PROCEDURES JUIN-JUILLET 2020 \n"); 

/* fin nouvelles procedures */

fprintf(stderr,"\n pour passer d'un fichier .raw venant de MATLAB a une image opendx utiliser 'testcreerim' ou 'testcreerim2");

fprintf(stderr,"\n pour passer d'une image volumique a un fichier de points pour la triangulation 'testecor'");

fprintf(stderr,"\n pour obtenir des cylindres a partir d'un decoupage en rondelles");

fprintf(stderr,"\n pour passer d'un fichier de tetrahedres a un fichier contenant les points du squelette avec les rayons et les directions 'testfig3' options 7 et 3");

fprintf(stderr,"\n pour passer d'un fichier du squelette avec diametres a un fichier de cylindres initiaux 'finitcyl3'");

fprintf(stderr,"\n pour visualiser les cylindres initiaux et les seuiller selon les rayons lancer 'testviscylini2'");

fprintf(stderr,"\n pour lancer le regroupement hierarchique lancer 'testcgraf_t'");

fprintf(stderr,"\n pour visualiser lancer 'testviscylini2'");

fprintf(stderr,"\n pour obtenir des fichiers d'histogrammes lancer 'testhiso'");

fprintf(stderr,"\n \n");

fprintf(stderr,"\n pour obtenir des boules ");

fprintf(stderr,"\n pour obtenir un fichier boules+allongement+direction a partir d'un fichier de Delaunay : 'testfig5' 'testtri3' 'triboul5' 'testvisboul2' 'filtre'");

fprintf(stderr,"\n pour obtenir un fichier de tetraedres sculptes : testfig6");

fprintf(stderr,"\n pour obtenir un fichier de boules maximales en prenant en compte le recouvrement du� squelette : 'triboul3'");

fprintf(stderr,"\n pour visualiser le squelette 'testsquel'");

fprintf(stderr,"\n pour sortir tapez 0");
fprintf(stderr,"\n DEBUT ANCIENNES PROCEDURES ");
fprintf(stderr,"\n pour lancer une simulation mepsom avec diffusion et options taper 1   ");
fprintf(stderr,"\n pour 'testtrans2' taper 2  ");
fprintf(stderr,"\n pour lancer une simulation mepsom avec diffusion methode 2 taper 3  ");
fprintf(stderr,"\n pour lancer une diffusion iterative avec le graphe taper 4  ");
fprintf(stderr,"\n pour lancer une simulation mepsom taper 5  ");
fprintf(stderr,"\n pour lancer une diffusion iterative taper 6  ");
fprintf(stderr,"\n pour lancer une diffusion taper 7  ");
fprintf(stderr,"\n pour generation du fichier des attributs taper 8  ");
fprintf(stderr,"\n pour simulation dynamique biologique taper 9  ");
fprintf(stderr,"\n pour calcul des coefficients de diffusion taper 10  ");
fprintf(stderr,"\n pour 'determination des chaines simples par regroupement' taper 11  ");
fprintf(stderr,"\n pour 'visuboul2_s' taper 12  ");
fprintf(stderr,"\n pour 'testapp' taper 13  ");
fprintf(stderr,"\n pour test de l'approximmation par un plan taper 14  ");
fprintf(stderr,"\n pour 'testsquel' taper 15  ");
fprintf(stderr,"\n pour 'testboule' taper 16  ");
fprintf(stderr,"\n pour 'testtaillent' taper 17  ");
fprintf(stderr,"\n pour 'testnbpointim' taper 18  ");
fprintf(stderr,"\n pour 'testcomparim' taper 19  ");
fprintf(stderr,"\n pour 'testcomparfic' taper 20  ");
fprintf(stderr,"\n pour 'testlecfic' taper 21  ");
fprintf(stderr,"\n pour 'testtrans' taper 22  ");
fprintf(stderr,"\n pour 'testcgraf3' taper 23  ");
fprintf(stderr,"\n pour 'testfig5' taper 24  ");
fprintf(stderr,"\n pour 'testfig6' taper 25  ");
fprintf(stderr,"\n pour 'testtri3' taper 26  ");
fprintf(stderr,"\n pour 'testreduboul' taper 27  ");
fprintf(stderr,"\n pour 'triboul5' taper 28  ");
fprintf(stderr,"\n pour 'triboul4' taper 29  ");
fprintf(stderr,"\n pour 'triboul3' taper 30  ");
fprintf(stderr,"\n pour 'triboul2' taper 31  ");
fprintf(stderr,"\n pour 'filtre' taper 32  ");
fprintf(stderr,"\n pour 'testvisboul2' taper 33  ");
fprintf(stderr,"\n pour 'testvisorient' taper 34 ");
fprintf(stderr,"\n pour 'testfic2scilab' taper 35  ");
fprintf(stderr,"\n pour 'testhisto' taper 36  ");
fprintf(stderr,"\n pour 'testcreerim2' taper 37  ");
fprintf(stderr,"\n pour 'testecor' taper 38  ");
fprintf(stderr,"\n pour 'testecor2' taper 39  ");
fprintf(stderr,"\n pour 'testcgraf_t' taper 40  ");
fprintf(stderr,"\n pour 'testviscylini2' taper 41  ");
fprintf(stderr,"\n pour 'testfinitcyl4' taper 42  ");
fprintf(stderr,"\n pour 'testfinitcyl3' taper 43  ");
fprintf(stderr,"\n pour 'testfinitcyl2' taper 44  ");
fprintf(stderr,"\n pour 'testfinitcyl' taper 45  ");
fprintf(stderr,"\n pour 'testinitcyl' taper 46  ");
fprintf(stderr,"\n pour 'testviscylini' taper 47  ");
fprintf(stderr,"\n pour 'testseuil2' taper 48  ");
fprintf(stderr,"\n pour 'testseuil' taper 49  ");
fprintf(stderr,"\n pour 'testcgraf' taper 50  ");
fprintf(stderr,"\n pour 'testcgraf2' taper 51  ");
fprintf(stderr,"\n pour 'testviscyl' taper 52  ");
fprintf(stderr,"\n pour 'testrac3' taper 53  ");
fprintf(stderr,"\n pour 'testequa3' taper 54  ");
fprintf(stderr,"\n pour 'testfig' taper 55  ");
fprintf(stderr,"\n pour 'testfig3' taper 56  ");
fprintf(stderr,"\n pour 'testim' taper 57  ");
fprintf(stderr,"\n pour 'testetra' taper 58  ");
fprintf(stderr,"\n pour 'testcreerim' taper 59  ");
fprintf(stderr,"\n pour 'testtri' taper 60  ");
fprintf(stderr,"\n pour 'triboul' taper 61  ");
fprintf(stderr,"\n pour 'filtallong' taper 62  ");
fprintf(stderr,"\n pour 'testvisboul' taper 63  ");
fprintf(stderr,"\n FIN ANCIENNES PROCEDURES ");
fprintf(stderr,"\n DEBUT NOUVELLES PROCEDURES ");
fprintf(stderr,"\n pour 'testecorval' taper 64");
fprintf(stderr,"\n pour 'testfig7' taper 65");
fprintf(stderr,"\n pour 'testtri4' taper 66");
fprintf(stderr,"\n pour 'triboulval' taper 67");
fprintf(stderr,"\n pour 'testvisuboulval' taper 68");
fprintf(stderr,"\n pour 'testcreeratt' taper 69");
fprintf(stderr,"\n pour 'testcregraphsoilmu3d' taper 70");
fprintf(stderr,"\n pour 'testcompconnexes' taper 71");
fprintf(stderr,"\n pour 'testdiffusionexp' taper 72");
fprintf(stderr,"\n pour 'testviscar' taper 73");
fprintf(stderr,"\n pour 'testcreeratt5' taper 74");
fprintf(stderr,"\n pour 'testsimulsynch' taper 75");
fprintf(stderr,"\n pour 'testdrainageval' taper 76");
fprintf(stderr,"\n pour 'testdrainagesimp' taper 77");
fprintf(stderr,"\n pour 'testcregraphsoilmu3d2' taper 78");
fprintf(stderr,"\n pour 'testcalsommes3f' taper 79");
fprintf(stderr,"\n pour 'testsimulsynchcourb' taper 80");
fprintf(stderr,"\n pour 'testsimulsynchcourbas' taper 81");
fprintf(stderr,"\n pour 'testcreeratt6' taper 82");
fprintf(stderr,"\n pour 'testsimulsynchcourbasas' taper 83");
fprintf(stderr,"\n pour 'testcreerattval' taper 84");
fprintf(stderr,"\n pour 'testcreerattvalbis' taper 85");
fprintf(stderr,"\n pour 'testverification' taper 86");
fprintf(stderr,"\n pour 'testsimulsynchcourbasynch' taper 87");
fprintf(stderr,"\n pour 'testedpnew' taper 88");
fprintf(stderr,"\n pour 'testcregraphsoilmu3d3' taper 89");
fprintf(stderr,"\n pour 'testmiseajourdifimp' taper 90");
fprintf(stderr,"\n pour 'testmiseajourdifimplic' taper 91");
fprintf(stderr,"\n pour 'testsimulsynchcourbimp' taper 92");
fprintf(stderr,"\n pour 'testsimulsynchcourbopt' taper 93");
fprintf(stderr,"\n pour 'courbesvalerie' taper 94");
fprintf(stderr,"\n pour 'testcreerattdiff' taper 95");
fprintf(stderr,"\n pour 'testcalib' taper 96");
fprintf(stderr,"\n pour 'testdijsktra' taper 97");
fprintf(stderr,"\n pour 'testaccess' taper 98");
fprintf(stderr,"\n pour 'testgrapheaccess' taper 99");
fprintf(stderr,"\n pour 'testaccessval' taper 100");
fprintf(stderr,"\n pour 'transformval' taper 101 ");
fprintf(stderr,"\n pour 'comptepoints' taper 102 ");


iok = -1; 
while(iok != 0)
{
	fprintf(stderr,"\n taper le choix : ");
	scanf("%d",&iok);
	switch(iok)

	{

		case 0:
		break;

		case 1:
		simulmepsom3();
		break;

		case 2:
		testtrans2();
		break;

		case 3:
		simulmepsom2();
		break;

		case 4:
		lancdifgr();
		break;

		case 5:
		simulmepsom();
		break;

		case 6:
			lancdifit();
			break;

		case 7:
			lancdif();
			break;

		case 8:
			intercalatt();
			break;

		case 9:
			testsim();
			break;

		case 10:
			testdif();
			break;

		case 11:
			testcgraf1_s();
			break;

		case 12:
			testvisboul2_s();
			break;

		case 13:
			testapp();
			break;

		case 14:
			testapplan2();
			break;

		case 15:
			testsquel();
			break;

		case 16:
			testboule();
			break;

		case 17:
			testtaillent();
			break;

		case 18:
			testnbpointim();
			break;

		case 19:
			testcomparim();
			break;

		case 20:
			testcomparfic();
			break;

		case 21:
			testlecfic();
			break;

		case 22:
			testtrans();
			break;

		case 23:
			testcgraf3();
			break;

		case 24:
			testfig5();
			break;

		case 25:
			testfig6();
			break;

		case 26:
			testtri3();
			break;

		case 27:
			testreduboul();
			break;

		case 28:
			triboul5();
			break;

		case 29:
			triboul4();
			break;

		case 30:
			triboul3();
			break;

		case 31:
			triboul2();
			break;

		case 32:
			filtre();
			break;

		case 33:
			testvisboul2();
			break;

		case 34:
			testvisorient();
			break;

		case 35:
			testfic2scilab();
			break;

		case 36:
			testhisto();
			break;

		case 37:
			testcreerim2();
			break;

		case 38:
			testecor();
			break;

		case 39:
			testecor2();
			break;

		case 40:
			testcgraf_t();
			break;

		case 41:
			testviscylini2();
			break;

		case 42:
			testfinitcyl4();
			break;

		case 43:
			testfinitcyl3();
			break;

		case 44:
			testfinitcyl2();
			break;

		case 45:
			testfinitcyl();
			break;

		case 46:
			testinitcyl();
			break;

		case 47:
			testviscylini();
			break;

		case 48:
			testseuil2();
			break;

		case 49:
			testseuil();
			break;

		case 50:
			testcgraf();
			break;

		case 51:
			testcgraf2();
			break;

		case 52:
			testviscyl();
			break;

		case 53:
			testrac3();
			break;

		case 54:
			testequa3();
			break;

		case 55:
			testfig();
			break;

		case 56:
			testfig3();
			break;

		case 57:
			testim();
			break;

		case 58:
			testetra();
			break;

		case 59:
			testcreerim();
			break;

		case 60:
			testtri();
			break;

		case 61:
			triboul();
			break;

		case 62:
			filtallong();
			break;

		case 63:
			testvisboul();
			break;

		case 64:
			testecorval(); 
			break;
		
		case 65:
			testfig7(); 
			break;

		case 66:
			testtri4(); 
			break;

		case 67:
			triboulval(); 
			break;
			
		case 68:
			testvisuboulval(); 
			break;

		case 69:
			testcreeratt(); 
			break;

		case 70:
			testcregraphsoilmu3d(); 
			break;

		case 71:
			testcompconnexes(); 
			break;

		case 72:
			testdiffusionexp(); 
			break;
		
		case 73:
			testviscar(); 
			break;

		case 74:
			testcreeratt5(); 
			break;

		case 75: 
			testsimulsynch();
			break; 

		case 76:
			testdrainageval();
			break;

		case 77:
			testdrainagesimp();
			break;

		case 78:
			testcregraphsoilmu3d2();
			break;

		case 79:
			testcalsommes3f();
			break;

		case 80:
			testsimulsynchcourb();
			break;

		case 81:
			testsimulsynchcourbas();
			break;

		case 82:
			testcreeratt6();
			break;

		case 83:
			testsimulsynchcourbasas();
			break;

		case 84:
			testcreerattval();
			break;

		case 85:
			testcreerattvalbis();
			break;

		case 86:
			testverification();
			break;

		case 87:
			testsimulsynchcourbasync();
			break; 

		case 88:
			testedpnew();
			break; 

		case 89:
			testcregraphsoilmu3d3();
			break; 

		case 90:
			testmiseajourdifimp(); 
			break; 	

		case 91:
			testmiseajourdifimplic(); 
			break;
		
		case 92:
			testsimulsynchcourbimp();
			break;

		case 93:
			testsimulsynchcourbopt();
			break;

		case 94:
			courbesvalerie();
			break; 

		case 95:
			testcreerattdiff();
			break; 

		case 96:
			testcalib();
			break; 

		case 97:
			testdijkstra();
			break; 
		
		case 98:
			testaccess();
			break; 

		case 99:
			testgrapheaccess();
			break;
		
		case 100:
			testaccessval();
			break;

		case 101:
			transformval();
			break;

		case 102:
			comptepoints();
			break;
		}
	}	
}



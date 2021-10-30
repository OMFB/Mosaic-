/*
 * simulation.c
 *
 *  Created on: 20 avr. 2010
 *      Author: monga
 */

#include <stdio.h>

#include "geometrie.h"

#include <errno.h>

#include <stdlib.h>

#include <unistd.h>  /* unlink */

#include <math.h>

#include <string.h> /* calloc ... */

#include <ctype.h> /* toupper ... */

#include <time.h>  /* rand ... */

#include <stdbool.h>
/* NEW */
/* routines pour la simulation de la dynamique biologique */

/* cette fonction effectue la simulation biologique avec le mod�le MEPSOM � partir des boules et des attributs initiaux ; elle
 rend les attributs finaux et leur evolution temporelle elle permet de combiner toutes les options */

void simulmepsom3()

{
	char ficboules[50],ficatt[50],ficatt2[50], ficcourbes[50],ficcoeff[50],racine[50],chaine[50],boules2[50],ficret[50];
	char ficgraphe[50],ficarcs[50],ficdonnees[50],ficpar[50],ficarbone[50],ficmatlab[50],ficmatlab2[50],ficcompco[50],ficcompcor[50],ficexcel[50];
	int nbjours,i,natt,j,nvmax,s,k,total,nbcompco,opteg;
	char ficdrai[50];
	double bilaninit;

	int nbinv,nbneg,tnbinv,tnbneg,optdif,nat,nboules ;
	float somneg,tsomneg,seuil,dseuil ;

	double *car,somme,bilan1;

	FILE *ptficcourbes,*ptficatt2,*ptficpar,*ptficmatlab,*ptficatt,*ptboules;

	graphe_s *graphe;

	float masse1,masse2,totmic,totmat;

	int nbarcs,dimx,dimy,dimz,nbmaxvois,iopt,ii,numatt,n,ncor,**trait,nbelc,retexp;

	float tempst,deltat,cdif,taillev,pas,rmax,vtot,porosite,r,veau,**courbret,pmin,pmax,rmin,resol;


	fprintf(stderr,"\n racine des fichiers : ");

	scanf("%s",racine);

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(ficboules,chaine);
	fprintf(stderr,"\n fichier de boules maximales en entree (x,y,z,r,0,0,0,0) : %s ",ficboules);

	strcpy(chaine,racine);
	strcat(chaine,".pardif");
	strcpy(ficpar,chaine);
	fprintf(stderr,"\n fichier des parametres pour la simulation %s ",ficpar);

	strcpy(chaine,racine);
	strcat(chaine,".arcs");
	strcpy(ficarcs,chaine);
	fprintf(stderr,"\n fichier des arcs : %s ",ficarcs);

	strcpy(chaine,racine);
	strcat(chaine,".att");
	strcpy(ficatt,chaine);
	fprintf(stderr,"\n fichier des attributs complementaires : %s",ficatt);

	strcpy(chaine,racine);
	strcat(chaine,".coeff");
	strcpy(ficcoeff,chaine);
	fprintf(stderr,"\n fichier des coefficients pour la diffusion : %s",ficcoeff);

	strcpy(chaine,racine);
	strcat(chaine,".gr");
	strcpy(ficgraphe,chaine);
	fprintf(stderr,"\n fichier du graphe : %s",ficgraphe);

	strcpy(chaine,racine);
	strcat(chaine,".carb");
	strcpy(ficarbone,chaine);
	fprintf(stderr,"\n fichier degagement de carbone : %s",ficarbone);

	strcpy(chaine,racine);
	strcat(chaine,".attsim");
	strcpy(ficatt2,chaine);
	fprintf(stderr,"\n nom du fichier des attributs complementaires apres decomposition en sortie: %s ",ficatt2);

	strcpy(chaine,racine);
	strcat(chaine,".courbes");
	strcpy(ficcourbes,chaine);
	fprintf(stderr,"\n nom du fichier contenant l'evolution des masses en sortie : %s",ficcourbes);

	strcpy(chaine,racine);
	strcat(chaine,".donnees");
	strcpy(ficdonnees,chaine);
	fprintf(stderr,"\n fichier des donnees : %s",ficdonnees);

	strcpy(chaine,racine);
	strcat(chaine,".matlab");
	strcpy(ficmatlab,chaine);
	fprintf(stderr,"\n fichier des courbes matlab  %s ",ficmatlab);

	strcpy(chaine,racine);
	strcat(chaine,".matlab_comp");
	strcpy(ficmatlab2,chaine);
	fprintf(stderr,"\n fichier de comparaison matlab  %s ",ficmatlab2);

	strcpy(chaine,racine);
	strcat(chaine,".compco");
	strcpy(ficcompco,chaine);
	fprintf(stderr,"\n fichier des composantes connexes  %s ",ficcompco);

	strcpy(chaine,racine);
	strcat(chaine,".r-compco");
	strcpy(ficcompcor,chaine);
	fprintf(stderr,"\n fichier des composantes connexes reduit %s ",ficcompcor);

	strcpy(chaine,racine);
	strcat(chaine,".excel");
	strcpy(ficexcel,chaine);
	fprintf(stderr,"\n fichier des courbes excel %s ",ficexcel);

	strcpy(chaine,racine);
	strcat(chaine,".bmaxd");
	strcpy(ficdrai,chaine);
	fprintf(stderr,"\n fichier original des boules pour drainage methode Valerie %s ",ficdrai);

	strcpy(chaine,racine);
	strcat(chaine,".cret");
	strcpy(ficret,chaine);
	fprintf(stderr,"\n fichier de la courbe de r�tention d'eau %s ",ficret);


	un : fprintf(stderr,"\n pour sortir taper 0");
	fprintf(stderr,"\n pour creation du fichier des arcs ('racine.arcs') en fonction du fichier des boules taper 1");
	fprintf(stderr,"\n pour creation du fichier du graphe ('racine.gr') a partir des fichiers de boules, des arcs et d'attributs taper 2");
	fprintf(stderr,"\n pour creation du fichier des boules apr�s drainage ('racineseuil.bmax') a partir du fichier initial de boules taper 3");
	fprintf(stderr,"\n pour creation du fichier d'attributs ('racine.att') en fonction du fichier de donnees (racine.donnees) methode aleatoire taper 4");
	fprintf(stderr,"\n pour simulation de la decomposition microbienne taper 5");
	fprintf(stderr,"\n pour creation du fichier d'attributs ('racine.att') en fonction du fichier de donnees (racine.donnees) methode kiel taper 6");
	fprintf(stderr,"\n pour creation du fichier d'attributs ('racine.att') en fonction du fichier de donnees (racine.donnees) methode paquets de micro-org. taper 7");
	fprintf(stderr,"\n pour creation du fichier des composantes connexes en fonction du fichier de graphe taper 8");
	fprintf(stderr,"\n pour test diffusion taper 9");
	fprintf(stderr,"\n pour transformation fichier des courbes en fichier pour excel avec pourcentages taper 10");
	fprintf(stderr,"\n pour drainage methode valerie taper 11");
	fprintf(stderr,"\n pour calcul de la courbe de r�tention d'eau taper 12");
	fprintf(stderr,"\n pour cr�ation du fichier d'attributs (racine.att) en fonction du fichier de donn�es (racine.donn�es) m�thode donn�es de Marc taper 13");

	fprintf(stderr,"\n");
	scanf("%d",&s);
	switch (s)
	{
		case 0:
			break;

		case 1:

			fprintf(stderr,"\n dimx = dimy = dimz = ");
			scanf("%d",&dimx);
			dimy = dimx;
			dimz = dimx;
			fprintf(stderr,"\n nb max de voisins d'un noeud = ");
			scanf("%d",&nbmaxvois);

			/* la procedure arcs prend en entree le nom du fichier de boules, les dimensions de l'image
			 contenant les boules et ecrit en sortie le nombre d'arcs "nbarcs" et un fichier contenant le nombre d'arcs et la liste des arcs
			 */
			arcs(ficboules,ficarcs, dimx,dimy,dimz,&nbarcs,nbmaxvois);
			fprintf(stderr,"\n fin de la determination du fichier des arcs");
			break;

		case 2:
			graphe = cregraphe_s3(ficboules,ficarcs,ficatt);
			fprintf(stderr,"\n graphe->nbnoeuds = %d graphe->nel = %d",graphe->nbnoeuds,graphe->nel);
			ecrigraphe(graphe,ficgraphe);
			break;

		case 3:
			fprintf(stderr,"\n Cette option suppose que les fichiers des attributs complementaires et du graphe ont ete crees");
			strcpy(chaine,racine);
			fprintf(stderr,"\n seuil pour le drainage selon les rayons (on selectionne les boules de rayon <= au seuil) = ");
			scanf("%f",&seuil_ext);
			fprintf(stderr,"\n extraction des boules dans la fenetre x0 y0 z0 dimx dimy dimz: ");
			scanf("%f %f %f %f %f %f",&x0_ext,&y0_ext,&z0_ext,&dimx_ext,&dimy_ext,&dimz_ext);
			sprintf(chaine,"%s_%f",chaine,seuil_ext);
			/* chaine contient la nouvelle racine */
			strcat(chaine,".bmax");
			strcpy(boules2,chaine);
			fprintf(stderr,"\n fichier de boules maximales en sortie (x,y,z,r,0,0,0,0) : %s ",boules2);
			strcpy(chaine,racine);
			sprintf(chaine,"%s_%f",chaine,seuil_ext);;
			strcat(chaine,".att");
			strcpy(ficatt2,chaine);
			fprintf(stderr,"\n fichier des attributs en sortie : %s",ficatt2);

			selecpores(ficboules,ficatt,ficgraphe,drainyoung,boules2,ficatt2);
			s = 0;
			break;

		case 4:
			/* le fichier des donnees contient: le nombre de donnees globales, la concentration en fructose en micron g par voxel cube
			  de sol et la concentration en BM;  les micro-organismes sont reparties aleatoirement et la matiere organique de
			 maniere homogene, on a 5 souches microbiennes differentes: MB1,MB2,MB3,MB4,MB5; dans un premier temps on regarde une souche
			 : Anthobacter sp 3R
			 1 mg/cm 3 = 0.00001 Ag / micron m cube
			 */
			fprintf(stderr,"\n coefficient multiplicatif maximal pour distribution aleatoire des MB (2 raisonnable) si -1 repartition homogene =");
			scanf("%f",&nmax_ext);
			sombio_ext = 0.0;
			/* sombio contient au long du processus la masse de MB deja attribuee */
			/* mise a jour du compteur pour l'appel de rand */
			srand(time(NULL));

			calcatt(ficboules,ficdonnees,ficatt,calattributs);
			break;

		case 5:

			som_ext = 0;
			ii =1;

			if(ii == 0)
			{
				fprintf(stderr,"\n nb de jours pour la simulation = (8 jours) ");
				scanf("%d",&nbjours);
				fprintf(stderr,"\n nb maximum de voisins par noeud =");
				scanf("%d",&nvmax);
				fprintf(stderr,"\n coefficient de production des enzymes (alpha) en jours -1 = (0) ");
				scanf("%lf",&alpha_ext);
				fprintf(stderr,"\n coefficient de respiration des micro-organismes (r) en jours -1 = (0,02) ");
				scanf("%lf",&r_ext);
				fprintf(stderr,"\n coefficient de mortalite des microorganismes (mu) en jours -1 = (0,001) ");
				scanf("%lf",&mu_ext);
				fprintf(stderr,"\n coefficient de diffusion de la DOM en (micron m carres par jours) -1 = (720) ");
				scanf("%f",&cdif_ext);
				fprintf(stderr,"\n croissance microbienne maximale en jours -1 (Cdom) = (7,68) ");
				scanf("%lf",&cdom_ext);
				fprintf(stderr,"\n coefficient pour l'assimilation des MB par la DOM en Angstrom g (Kb) = (0.264) ");
				scanf("%lf",&kb_ext);
				fprintf(stderr,"\n fraction de jour pour un cycle de simulation (nombre entier) = (24) ");
				scanf("%d",&deltasim_ext);
				fprintf(stderr,"\n nombre de pas pour pour la diffusion (5) = ");
				scanf("%d",&fracjour_ext);
				fprintf(stderr,"\n resolution de l'image en micron metre = ");
				scanf("%f",&taillev_ext);
				fprintf(stderr,"\n coefficient multiplicateur pour l'unte de la masse de MB (<1) = ");
				scanf("%lf",&ec_ext);
				fprintf(stderr,"\n pour inclure le bilan des masses dans le fichier des courbes taper 1 sinon 0");
				scanf("%d",&iopt);
				fprintf(stderr,"\n pour inclure tous les pourcentages dans le fichier carbone taper 1 sinon 0");
				scanf("%d",&total);

			}else
			{
				ptficpar = fopen(ficpar,"r");

				fscanf(ptficpar,"%d",&nbjours);
				fscanf(ptficpar,"%d",&nvmax);
				fscanf(ptficpar,"%lf",&alpha_ext);
				fscanf(ptficpar,"%lf",&r_ext);
				fscanf(ptficpar,"%lf",&mu_ext);
				fscanf(ptficpar,"%f",&cdif_ext);
				fscanf(ptficpar,"%lf",&cdom_ext);
				fscanf(ptficpar,"%lf",&kb_ext);
				fscanf(ptficpar,"%d",&deltasim_ext);
				fscanf(ptficpar,"%d",&fracjour_ext);
				fscanf(ptficpar,"%f",&taillev_ext);
				fscanf(ptficpar,"%lf",&ec_ext);
				fscanf(ptficpar,"%d",&iopt);
				fscanf(ptficpar,"%d",&total);
				/* pourdom_ext definit le pourcentage de matiere organique active */
				fscanf(ptficpar,"%f",&pourdom_ext);
				/* optcarb_ext definit la courbe experimentale de reference il peut etre egale a 1,2 */
				fscanf(ptficpar,"%d",&optcarb_ext);
				/* inac_ext definit le pourcentage de la biomasse transforme en matiere organique non consommable lors de la mortalite */
				fscanf(ptficpar,"%f",&inac_ext);
				/* optdif definit le type de diffusion voulu type 1 : mise a jour sequentielle ; type 2 : mise a jour parallele */
				fscanf(ptficpar,"%d",&optdif);
				/* opt definit si on egale les valeurs des densit�s de mani�re s�quentielle quand on trouve des inversions si optreg =
				 -1 alors diffusion instantan�e*/
				fscanf(ptficpar,"%d",&opteg);
				/* on definit le nombre d'iterations de la corerction le mettre a zero si on ne veut pas verifier pour aller plus vite*/
				fscanf(ptficpar,"%d",&ncor);
				/* on d�finit k1,k2,fr pour les pesticides */
				fscanf(ptficpar,"%f",&k1_ext);
				fscanf(ptficpar,"%f",&k2_ext);
				fscanf(ptficpar,"%f",&fr_ext);

				fprintf(stderr,"\n nb de jours pour la simulation = (8 jours) %d",nbjours);
				fprintf(stderr,"\n nb maximum de voisins par noeud = %d",nvmax);
				fprintf(stderr,"\n coefficient de production des enzymes (alpha) en jours -1 = (0) %lf ",alpha_ext);
				fprintf(stderr,"\n coefficient de respiration des micro-organismes (r) en jours -1 = (0,02) %lf",r_ext);
				fprintf(stderr,"\n coefficient de mortalite des microorganismes (mu) en jours -1 = (0,001) %lf ",mu_ext);
				fprintf(stderr,"\n coefficient de diffusion de la DOM en (micron m carres par jours) -1 = (720) si negatif diffusion iterative %f ",cdif_ext);
				fprintf(stderr,"\n croissance microbienne maximale en jours -1 (Cdom) = (7,68) %lf ",cdom_ext);
				fprintf(stderr,"\n coefficient pour l'assimilation de la DOM par les MB en Angstrom g (Kb) = (10000) %lf ",kb_ext);
				fprintf(stderr,"\n fraction de jour pour un cycle de simulation (nombre entier) = (24) %d ",deltasim_ext);
				fprintf(stderr,"\n nombre de pas pour pour la diffusion  = %d",fracjour_ext);
				fprintf(stderr,"\n resolution de l'image en micron metre = %f",taillev_ext);
				fprintf(stderr,"\n coefficient multiplicateur pour l'unte de la masse de MB (<1) = %lf",ec_ext);
				fprintf(stderr,"\n pourcentage de DOM active = %f",pourdom_ext);
				fprintf(stderr,"\n numero de la courbe experimentale de reference = %d",optcarb_ext);
				fprintf(stderr,"\n fraction de de la biomasse se transformant en mati�re organique non consommable (entre 0.3 et 0.7) = %f",inac_ext);
				fprintf(stderr,"\n type de diffusion : %d",optdif);
				fprintf(stderr,"\n rectification sequentielle (si oui 1 sinon 0, si -1 diffusion instantan�e): %d",opteg);
				fprintf(stderr,"\n nombre d'iterations de la correction : %d",ncor);
				fprintf(stderr,"\n k1 = %f k2 = %f fr = %f",k1_ext,k2_ext,fr_ext);
				fclose(ptficpar);
			}
			fprintf(stderr,"\n entree dans lecgraphe");
			graphe = lecgraphe(ficgraphe);
			fprintf(stderr,"\n sortie de lecgraphe");
			natt = graphe->ncar - 4;
			ptficcourbes = fopen(ficcourbes,"w");

			car = calsommes3(graphe);

			fprintf(ptficcourbes,"%d",natt);
			fprintf(ptficcourbes,"\n");

			somme = 0;

			for(j=0;j<natt;j++)
			{
				fprintf(ptficcourbes," %f",(float)(car[j]));
				if(j != 0)
				{
					somme = (float)(somme + (double)(car[j]));
				}
				else
				{
					somme = (float)(somme + (double)(car[j])*ec_ext);
				}
			}
			if(iopt == 1)fprintf(ptficcourbes," \n %f",(float)(somme));
			fprintf(ptficcourbes,"\n");


			/* on lance en cascade la simulation pour un jour en discretisant avec des fractions de jours deltasim_ext */
			tnbinv = 0;
			tnbneg = 0;
			tsomneg = 0;

			bilaninit = bilanmasses(graphe);
			bilanmatmic(graphe);

			for(i = 0;i<nbjours;i++)
			{
				fprintf(stderr,"\n jour : %d",i+1);

				/* on calcule pour chaque fraction de jour deltasim_ext */

				for(k=0;k<deltasim_ext;k++)
				{


					/* production d'enzymes, respiration, mortalite  des micro-organismes */
					miseajour2(&graphe,microorg_sl2);

					/* les micro-organismes mangent la DOM */
					miseajour2(&graphe,mangedom_sl2);

					/* pesticides */
					if(k1_ext != 0  || k2_ext != 0 || fr_ext !=0)miseajour2(&graphe,pesticides_sl2);

					/* diffusion de la DOM */
					/* si optdif == -2 pas de diffusion */
					if(optdif != -2)
					{
						if(cdif_ext > 0)
						{
							if(optdif == 1)
							{
								diffusiong2(&graphe,nvmax,fdidom_sl2,fracjour_ext);
							}
							else if(optdif == 2)
							{

								diffusiong3(&graphe,2,1.0/(float)(deltasim_ext),cdif_ext,taillev_ext,&nbinv,&nbneg,&somneg,opteg,ncor);
								if(nbinv == -1)
								{
									tnbinv = -1;
								}
								else
								{
									tnbinv += nbinv;
								}
								tnbneg += nbneg;
								tsomneg += somneg;
							}
						}
						else if(cdif_ext < 0)
						{
							diffusiong4(&graphe,2,-(int)(cdif_ext),taillev_ext);
						}
					}
					car = calsommes3(graphe);

					somme = 0;

					for(j=0;j<natt;j++)
					{
						fprintf(ptficcourbes," %f",car[j]);
						if(j != 0)
						{
							somme = (float)(somme + (double)car[j]);
						}
						else
						{
							somme = (float)(somme + (double)(car[j])*ec_ext);
						}
					}
						if(iopt == 1)fprintf(ptficcourbes," \n %f",(float)(somme));
						fprintf(ptficcourbes,"\n");
				}

				if(optdif == 2 && opteg != -1)fprintf(stderr,"\n total des inversions = %d ; nb total de valeurs negatives = %d ; somme totale des valeurs negatives = %f",tnbinv,tnbneg,tsomneg);
			}
			/* remplissage du fichier des attributs complementaires finaux */
			ptficatt2 = fopen(ficatt2,"w");
			fprintf(ptficatt2,"%d",graphe->ncar - 4);

			for(i=1;i<= graphe->nel;i++)
			{
				fprintf(ptficatt2,"\n");
				for(j=0;j<graphe->ncar - 4;j++)
				{
					fprintf(ptficatt2,"%f ",graphe->carac[j+5][i]);
				}
			}
			bilan1 = bilanmasses(graphe);
			bilanmatmic(graphe);
			fprintf(stderr,"\n rapport bilans avant et apr�s traitement total = %lf",bilan1/bilaninit);
			fprintf(stderr,"\n som_ext = %lf",som_ext);

			fclose(ptficcourbes);
			fclose(ptficatt2);
			/* remplissage du fichier carbone */
			courbcarb(ficcourbes,ficarbone,nbjours,deltasim_ext,iopt,total);
			courbmatlab(ficcourbes,ficmatlab,nbjours,deltasim_ext,ficexcel);
			comparcarb(ficcourbes,ficmatlab2,nbjours,deltasim_ext);
			break;

		case 6:
			/* le fichier de donnees contient le nombre des donnees l'encadrement des rayons des boules contenant la MO, l'encadrement
			 des rayons des boules contenant la BM la masse de MO et la masse de BM */

			calcatt2(ficboules,ficdonnees,ficatt,calattributs2);
			break;

		case 13:
			calcatt4(ficboules,ficdonnees,ficatt);
			break;

		case 7:
			/* le fichier de donnees contient le nombre des donnees l'encadrement des rayons des boules contenant la MO, l'encadrement
			 des rayons des boules contenant la BM la masse de MO et la masse de BM */
			fprintf(stderr,"\n Nombre de paquets de micro-organismes =  ");
			scanf("%f",&nmax_ext);

			/* mise a jour du compteur pour l'appel de rand */
			srand(time(NULL));
			calcatt3(ficboules,ficdonnees,ficatt);
			break;

		case 8:
			fprintf(stderr,"\n entr�e dans lecgraphe");
			graphe = lecgraphe(ficgraphe);
			fprintf(stderr,"\n fin de la lecture du graphe");
			compconnexes(graphe,ficcompco,ficcompcor,&nbcompco);
			fprintf(stderr,"\n nb de composantes connexes = %d",nbcompco);
			break;

		case 9:

			fprintf(stderr,"\n num�ro de l'attribut � diffuser ; coefficient de diffusion ; temps total (multiple du pas) ; pas de temps : taille voxels ; rectification ; nb d'iterations rect. ");
			scanf("%d %f %f %f %f %d %d",&numatt,&cdif,&tempst,&deltat,&taillev,&opteg,&ncor);

			graphe = lecgraphe(ficgraphe);

			/* on lance tempt/deltat fois la diffusion avec le pas de temps deltat */
			n = floor(tempst/deltat);
			fprintf(stderr,"\n nombre d'it�rations = %d",n);

			tnbinv = 0;
			tnbneg = 0;
			tsomneg = 0;
			for(i=0;i<n;i++)
			{
				diffusiong3(&graphe,numatt,deltat,cdif,taillev,&nbinv,&nbneg,&somneg,opteg,ncor);
				if(nbinv == -1)
				{
					tnbinv = -1;
				}
				else
				{
					tnbinv += nbinv;
				}
				tnbneg += nbneg;
				tsomneg += somneg;
			}
			fprintf(stderr,"\n total des inversions = %d ; nb total de valeurs negatives = %d ; somme totale des valeurs negatives = %f",tnbinv,tnbneg,tsomneg);

			/* ecriture des attributs finaux */

			ptficatt2 = fopen(ficatt2,"w");
			fprintf(ptficatt2,"%d",graphe->ncar - 4);

			for(i=1;i<= graphe->nel;i++)
			{
				fprintf(ptficatt2,"\n");
				for(j=0;j<graphe->ncar - 4;j++)
				{
					fprintf(ptficatt2,"%f ",graphe->carac[j+5][i]);
				}
			}
			fclose(ptficatt2);
			break;

		case 10:
			transform();
			break;

		case 11:


			fprintf(stderr,"\n dimx = dimy = dimz = ");
			scanf("%d",&dimx);
			dimy = dimx;
			dimz = dimx;

			fprintf(stderr,"\n seuil rayon, seuil bords : ");
			scanf("%f %f",&seuil,&dseuil);

			nbmaxvois = 1000;

			/* la procedure arcs prend en entree le nom du fichier de boules, les dimensions de l'image
			 contenant les boules et ecrit en sortie le nombre d'arcs "nbarcs" et un fichier contenant le nombre d'arcs et la liste des arcs
			 */
			arcs(ficdrai,ficarcs, dimx,dimy,dimz,&nbarcs,nbmaxvois);

			fprintf(stderr,"\n fin de la determination du fichier des arcs");

			graphe = cregraphe_s4(ficdrai,ficarcs);

			fprintf(stderr,"\n fin de la cr�ation du graphe");

			trait = drainit2(graphe,seuil,dseuil,dimx,dimy,dimz);

			verifdrain(graphe,trait,seuil,&optdif);

			fprintf(stderr,"\n nb d'anomalies = %d",optdif);

			ficdrain(graphe,trait,ficboules,seuil);
			break;

		case 12:


			fprintf(stderr,"\n dimx = dimy = dimz = ");
			scanf("%d",&dimx);
			dimy = dimx;
			dimz = dimx;

			fprintf(stderr,"\n seuil bords : ");
			scanf("%f",&dseuil);

			fprintf(stderr,"\n resolution de l'image en micron m (5 micron m pour le sable): ");

			scanf("%f",&resol);

			fprintf(stderr,"\n pas de discr�tisation des rayons en pixels (1), potentiel hydrique minimal en cm (1 sable), potentiel hydrique maximal en cm (294 sable) ");
			scanf("%f %f %f",&pas,&pmin,&pmax);

			fprintf(stderr,"\n courbe experimentale (sable : 1, pas de courbe 0 : ");

			scanf("%d",&retexp);

			/* calcul des rayons maximum et minimum ainsi que du pas de discr�tisation en pixels */
			rmin = (1456/pmax)/resol;
			rmax = (1456/pmin)/resol;

			nbmaxvois = 1000;

			fprintf(stderr,"\n rmin (pixels) = %f ; rmax (pixels) = %f ; pas (pixels) = %f",rmin,rmax,pas);

			/* la procedure arcs prend en entree le nom du fichier de boules, les dimensions de l'image
			 contenant les boules et ecrit en sortie le nombre d'arcs "nbarcs" et un fichier contenant le nombre d'arcs et la liste des arcs
			 */
			fprintf(stderr,"\n fichier des boules : %s",ficdrai);
			arcs(ficdrai,ficarcs, dimx,dimy,dimz,&nbarcs,nbmaxvois);

			fprintf(stderr,"\n fin de la determination du fichier des arcs");

			graphe = cregraphe_s4(ficdrai,ficarcs);

			fprintf(stderr,"\n fin de la cr�ation du graphe");

			/* calcul du volume total de l'ensemble des boules */

			vtot = 0;

			for(i=1;i<=graphe->nel;i++)
			{
				vtot += (4.0/3.0)*(graphe->carac[4][i])*(graphe->carac[4][i])*(graphe->carac[4][i])*3.14159265;
			}

			/* fprintf(stderr,"\n vtot = %f",vtot);*/

			porosite = (vtot/(dimx*dimy*dimz))*100.0;
			/* fprintf(stderr,"\n porosit� en pourcentage = %f",porosite);*/

			/* calcul de la courbe de r�tention avec un pas d'incr�ment des rayons de "pas" jusqu'� un rayon de "rmax */

			r = rmin;

			courbret = allot2f(2,ceil(rmax/pas)+1);

			j = 0;

			while(r <= rmax)
			{
				trait = drainit2(graphe,r,dseuil,dimx,dimy,dimz);

				verifdrain(graphe,trait,r,&optdif);

				/* fprintf(stderr,"\n nb d'anomalies = %d",optdif);*/

				veau = 0;

				for(i=1;i<=graphe->nel;i++)
				{
					/* traitement de la boule i */
					if(trait[1][i] == 1)
					{
						/* cas d'une boule remplie d'eau on met � jour le volume des boules d'eau */
						veau += (4.0/3.0)*(graphe->carac[4][i])*(graphe->carac[4][i])*(graphe->carac[4][i])*3.14159265;

					}

				}
				/* lib�ration de la place de trait */
				freetin(trait,2,graphe->nel + 1);

				courbret[0][j] = r;
				courbret[1][j] = (veau/vtot);
				/* fprintf(stderr,"\n r en micron m = %f ; r en pixels = %f ; p = %f ; rap = %f",courbret[0][j]*resol,courbret[0][j],1456/(r*resol),courbret[1][j]);*/

				j++;
				r+=pas;

			}

			nbelc = j;

			/* transformation de la courbe rayon / (volume eau / volume total des pores) en
			  potentiel hydrique / (volume eau / volume total des pores)
			 */
			/* fprintf(stderr,"\n nbelc = %d",nbelc);*/
			for(i=0;i<nbelc;i++)
			{
				courbret[0][i] = 1456/(courbret[0][i]*resol);
				/* fprintf(stderr,"\n potentiel = %f ; volume pores eau / volume total pores = %f",courbret[0][i],courbret[1][i]);*/
			}

			fprintf(stderr,"\n debut de la generation du fichier matlab");

			/* calcul de la courbe potentiels hydriques / (volume eau/volume total des pores) en fonction de la corbe en fonction des rayons */


			/* g�n�ration du fichier matlab pour la courbe de r�tention X:cm Y:sans dimension*/

			/* on genere les instructions matlab */

			ptficmatlab = fopen(ficret,"w");

			/* on ecrit la valeur de X */

			fprintf(ptficmatlab,"X = [");

			for(i = nbelc-1;i>=0;i--)fprintf(ptficmatlab,"%f ",courbret[0][i]);

			fprintf(ptficmatlab,"];");

			/* on ecrit R */

			/* on ecrit la valeur de R */

			fprintf(ptficmatlab,"R = [");

			for(i = nbelc-1;i>=0;i--)fprintf(ptficmatlab,"%f ",courbret[1][i]);

			fprintf(ptficmatlab,"];");

			fprintf(ptficmatlab,"\n");

			/* courbe experimentale */

			switch(retexp)
			{

				case 1 :
				/* cas du sable */
				fprintf(ptficmatlab,"Y = [1 10 31.62 100 251.19 1000]; O = [1 0.88 0.6 0.235 0.01 0];") ;
				fprintf(ptficmatlab,"plot(X,R,'b',Y,O,'r');");
				break;

				/* on appelle la fonction plot */

				case 0 :

				fprintf(ptficmatlab,"plot(X,R,'b');");
				fprintf(ptficmatlab,"\n");
				break;

			}

			fclose(ptficmatlab);

			break;


	}

	if (s != 0) goto un;

}

/* cette procedure transforme un fichier de courbes en un fichier avec les pourcentages lisible par excel */

void transform()
{
	char ficourbes[50],ficexcel[50];
	FILE *ptficourbes,*ptficexcel;
	int njours,fracjour;
	float totacarb;
	int i,j,natt,nbels;
	float **car;

	fprintf(stderr,"Fichier de courbes en entree : ");
	scanf("%s",ficourbes);
	fprintf(stderr,"Fichier de courbes pour excel en sortie :");
	scanf("%s",ficexcel);
	fprintf(stderr,"nombre de jours = ");
	scanf("%d",&njours);
	fprintf(stderr,"fraction de jour =");
	scanf("%d",&fracjour);

	ptficourbes = fopen(ficourbes,"r");
	ptficexcel = fopen(ficexcel,"w");

	fscanf(ptficourbes,"%d",&natt);

	nbels = njours*fracjour;

	car = allot2f(nbels + 1,natt);

	for(i=0;i<=nbels;i++)
		for(j=0;j<natt;j++)
		{
			fscanf(ptficourbes,"%f",&(car[i][j]));
		}
	/* car contient tous les elements du fichier des courbes */

	totacarb = car[0][1];

	/* on ramene tout en pourcentage de carbone initial */

	for(i=0;i<=nbels;i++)
	{
		fprintf(ptficexcel,"%f ",(float)(i)/(float)(fracjour));
		for(j=0;j<natt;j++)
		{
			car[i][j] = (car[i][j]/totacarb)*100;
			fprintf(ptficexcel,"%f ",car[i][j]);
		}
		fprintf(ptficexcel,"\n");
	}

	fclose(ptficourbes);
	fclose(ptficexcel);
	freetfloat(car,nbels,natt);

}


/* cette fonction prend en entree le fichier des courbes et fournit un fichier comprenant les instructions matlab
 pour tracer les courbes en fonction du pourcentage de carbone initial
 */
void courbmatlab(char *ficourbes,char *ficmatlab,int njours,int fracjour,char *ficexcel)
{
	FILE *ptficourbes,*ptficmatlab,*ptficexcel;
	float totacarb;
	int i,j,natt,nbels;
	float **car;

	ptficourbes = fopen(ficourbes,"r");
	ptficmatlab = fopen(ficmatlab,"w");
	ptficexcel = fopen(ficexcel,"w");
	fscanf(ptficourbes,"%d",&natt);

	nbels = njours*fracjour;
	car = allot2f(nbels + 1,natt);

	for(i=0;i<=nbels;i++)
		for(j=0;j<natt;j++)
		{
			fscanf(ptficourbes,"%f",&(car[i][j]));
		}
	/* car contient tous les elements du fichier des courbes */

	totacarb = car[0][1];

	/* on ramene la biomasse en angstrom grammes */
	for(i=0;i<=nbels;i++)
		car[i][0] = ec_ext*car[i][0];

	/* on ramene tout en pourcentage de carbone initial */
	for(i=0;i<=nbels;i++)
	{
		fprintf(ptficexcel,"%f ",(float)(i)/(float)(fracjour));
		for(j=0;j<natt;j++)
		{
			car[i][j] = (car[i][j]/totacarb)*100;
			fprintf(ptficexcel,"%f ",car[i][j]);
		}
		fprintf(ptficexcel,"\n");
	}

	/* on genere les instructions matlab */

	/* on ecrit la valeur de X */
	fprintf(ptficmatlab,"X = [");
	for(i = 0;i<nbels;i++)fprintf(ptficmatlab,"%f ",(float)(i));
	fprintf(ptficmatlab,"%f];",(float)(nbels));

	/* on ecrit Y0,Y1,Y2,Y3,Y4,Y5 */

	/* on ecrit la valeur de Y0 */
	fprintf(ptficmatlab,"Y0 = [");
	for(i = 0;i<nbels;i++)fprintf(ptficmatlab,"%f ",car[i][0]);
	fprintf(ptficmatlab,"%f];",car[i][0]);

	fprintf(ptficmatlab,"\n");

	fprintf(ptficmatlab,"Y1 = [");
	for(i = 0;i<nbels;i++)fprintf(ptficmatlab,"%f ",car[i][1]);
	fprintf(ptficmatlab,"%f];",car[nbels][1]);

	fprintf(ptficmatlab,"\n");

	fprintf(ptficmatlab,"Y3 = [");
	for(i = 0;i<nbels;i++)fprintf(ptficmatlab,"%f ",car[i][3]);
	fprintf(ptficmatlab,"%f];",car[nbels][3]);

	fprintf(ptficmatlab,"\n");

	fprintf(ptficmatlab,"Y5 = [");
	for(i = 0;i<nbels;i++)fprintf(ptficmatlab,"%f ",car[i][5]);
	fprintf(ptficmatlab,"%f];",car[nbels][5]);

	fprintf(ptficmatlab,"\n");
	/* on a ecrit Y0,Y1,Y3,Y5 */

	/* on appelle la fonction plot */

	fprintf(ptficmatlab,"plot(X,Y0,'b',X,Y1,'g',X,Y3,'r',X,Y5,'c');");
	fprintf(ptficmatlab,"\n");

	freetfloat(car,nbels,natt);
	fclose(ptficourbes);
	fclose(ptficmatlab);
	fclose(ptficexcel);
}

/* cette fonction prend en entree le fichier des courbes et fournit un fichier comprenant les instructions matlab
 pour comparer la courbe de degagement du carbone avec la courbe experimentale
 */
void comparcarb(char *ficourbes,char *ficmatlab,int njours,int fracjour)
{
	FILE *ptficourbes,*ptficmatlab;
	float totacarb;
	int i,j,natt,nbels;
	float **car;

	ptficourbes = fopen(ficourbes,"r");
	ptficmatlab = fopen(ficmatlab,"w");
	fscanf(ptficourbes,"%d",&natt);
	nbels = njours*fracjour;
	car = allot2f(nbels + 1,natt);

	for(i=0;i<=nbels;i++)
		for(j=0;j<natt;j++)
		{
			fscanf(ptficourbes,"%f",&(car[i][j]));
		}
	/* car contient tous les elements du fichier des courbes */

	totacarb = car[0][1];
	totacarb = (100.0/pourdom_ext)*totacarb;

	/* on ramene la biomasse en angstrom grammes */
	for(i=0;i<=nbels;i++)
		car[i][0] = ec_ext*car[i][0];

	/* on ramene tout en pourcentage de carbone initial */
	for(i=0;i<=nbels;i++)
		for(j=0;j<natt;j++)
		{
			car[i][j] = (car[i][j]/totacarb)*100;
		}

	/* on genere les instructions matlab */

	/* on ecrit la valeur de X */
	fprintf(ptficmatlab,"X = [");
	for(i = 0;i<njours;i++)
	{
		fprintf(ptficmatlab,"%f ",(float)(i));
	}
	fprintf(ptficmatlab,"%f];",(float)(njours));

	/* on ecrit Y5 */
	fprintf(ptficmatlab,"Y5 = [");
	for(i = 0;i<nbels;i++)
	{
		if(i % fracjour == 0)fprintf(ptficmatlab,"%f ",car[i][5]);
	}
	fprintf(ptficmatlab,"%f];",car[nbels][5]);
	if(optcarb_ext == 1)
	/* SL2_29 3R */
	{
		fprintf(ptficmatlab,"YE =[");
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",21.0);
		fprintf(ptficmatlab,"%f ",27.0);
		fprintf(ptficmatlab,"%f ",27.0);
		fprintf(ptficmatlab,"%f ",28.0);
		fprintf(ptficmatlab,"%f ",28.0);
		fprintf(ptficmatlab,"%f]; ",28.0);
	}
	else if(optcarb_ext == 2)
	{
		/* SL2_2.9 3R */
		fprintf(ptficmatlab,"YE =[");
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",1.0);
		fprintf(ptficmatlab,"%f ",10.0);
		fprintf(ptficmatlab,"%f ",23.0);
		fprintf(ptficmatlab,"%f ",25.0);
		fprintf(ptficmatlab,"%f ",27.0);
		fprintf(ptficmatlab,"%f ",28.0);
		fprintf(ptficmatlab,"%f]; ",29.0);
	}
	else if(optcarb_ext == 3)
	{
		/* anthro 7R pression hydrique 2.9; SL2_2.9 */
		fprintf(ptficmatlab,"YE =[");
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",12.0);
		fprintf(ptficmatlab,"%f ",29.0);
		fprintf(ptficmatlab,"%f ",44.0);
		fprintf(ptficmatlab,"%f ",48.0);
		fprintf(ptficmatlab,"%f ",52.0);
		fprintf(ptficmatlab,"%f]; ",56.0);
	}
	else if(optcarb_ext == 4)
	{
		/* anthro 7R pression hydrique 29; SL2_29 */
		fprintf(ptficmatlab,"YE =[");
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",19.0);
		fprintf(ptficmatlab,"%f ",33.0);
		fprintf(ptficmatlab,"%f ",46.0);
		fprintf(ptficmatlab,"%f ",54.0);
		fprintf(ptficmatlab,"%f ",56.0);
		fprintf(ptficmatlab,"%f]; ",59.0);
	}
	else if(optcarb_ext == 5) /* 6L pression hydrique 2.9 */
	{
		fprintf(ptficmatlab,"YE =[");
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",0.2);
		fprintf(ptficmatlab,"%f ",8.6);
		fprintf(ptficmatlab,"%f ",43.34);
		fprintf(ptficmatlab,"%f ",54.4);
		fprintf(ptficmatlab,"%f ",61.65);
		fprintf(ptficmatlab,"%f ",61.65);
		fprintf(ptficmatlab,"%f]; ",66.0);
	}
	else if(optcarb_ext == 6) /* 5L pression hydrique 2.9 */

	{
		fprintf(ptficmatlab,"YE =[");
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",1.0);
		fprintf(ptficmatlab,"%f ",13.0);
		fprintf(ptficmatlab,"%f ",37.0);
		fprintf(ptficmatlab,"%f ",43.0);
		fprintf(ptficmatlab,"%f ",46.0);
		fprintf(ptficmatlab,"%f]; ",49.0);
	}
	else if(optcarb_ext == 7) /* 5L pression hydrique 29 */
	{
		fprintf(ptficmatlab,"YE =[");
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",1.0);
		fprintf(ptficmatlab,"%f ",24.0);
		fprintf(ptficmatlab,"%f ",36.0);
		fprintf(ptficmatlab,"%f ",40.0);
		fprintf(ptficmatlab,"%f ",44.0);
		fprintf(ptficmatlab,"%f]; ",46.0);
	}
	else if(optcarb_ext == 8) /* 6L pression hydrique 29 */

	{
		fprintf(ptficmatlab,"YE =[");
		fprintf(ptficmatlab,"%f ",0.0);
		fprintf(ptficmatlab,"%f ",0.007);
		fprintf(ptficmatlab,"%f ",18.0);
		fprintf(ptficmatlab,"%f ",51.0);
		fprintf(ptficmatlab,"%f ",57.0);
		fprintf(ptficmatlab,"%f ",62.0);
		fprintf(ptficmatlab,"%f ",62.0);
		fprintf(ptficmatlab,"%f]; ",65.4);
	}
	fprintf(ptficmatlab,"\n");
	/* on a ecrit Y0,Y1,Y3,Y5 */

	/* on appelle la fonction plot */

	if(optcarb_ext != 0)
	{
		fprintf(ptficmatlab,"plot(X,YE,'r',X,Y5,'b');");
	}
	else
	{
		fprintf(ptficmatlab,"plot(X,Y5,'b')");
	}

	fprintf(ptficmatlab,"\n");

	freetfloat(car,nbels,natt);
	fclose(ptficourbes);
	fclose(ptficmatlab);
}

/* cette fonction prend en entree le fichier des courbes et fournit un fichier incluant les variations du carbone
 degage en fonction de la proportion de la matiere organique dissoute initiale, les lignes du fichier comprennent 'fracjour'
 valeurs sur une ligne, si opt = 1 on a aussi la somme totale des masses dans le fichier des courbes, si total = 1 on ecrit tout
 sinon on ecrit que pour chaque jour
 */
void courbcarb(char *ficourbes,char *ficarbone,int njours,int fracjour,int opt,int total)
{
	FILE *ptficourbes,*ptficarbone;
	double totacarb,carbone,pourcentage;
	int i,j,natt,k;
	float *car,somme;

	ptficourbes = fopen(ficourbes,"r");
	ptficarbone = fopen(ficarbone,"w");
	fscanf(ptficourbes,"%d",&natt);

	car = (float*)mycalloc(natt,sizeof(float));

	/* lire les donnees initiales pour le jour 0 et ecrire sur la premiere ligne du fichier carbone le pourcentage initial */

	for(j=0;j<natt;j++)
	{
		fscanf(ptficourbes,"%f",&(car[j]));
	}
	totacarb = (double)(car[1]);
	totacarb = (100.0/pourdom_ext)*totacarb;
	carbone = (double)(car[5]);
	pourcentage = (carbone/totacarb)*100.0;
	fprintf(ptficarbone,"%f ",(float)(pourcentage));
	fprintf(ptficarbone,"\n");
	if(opt == 1)fscanf(ptficourbes,"%f",&somme);

	/* lire tous les jours */

	for(i=0;i<njours;i++)
	{
		/* lire pour chaque fraction de jours */
		for(k=0;k<fracjour;k++)
		{
			/* lire les chiffres pour la fraction de jour k+1 du jour i+1 */
			for(j=0;j<natt;j++)
			{
				fscanf(ptficourbes,"%f",&(car[j]));
			}
			carbone = (double)(car[5]);
			pourcentage = (carbone/totacarb)*100;
			if(total == 1)fprintf(ptficarbone,"%f ",(float)(pourcentage));
			if(opt == 1)fscanf(ptficourbes,"%f",&somme);
		}
		/* on va a la ligne quand on change de jour */
		if(total ==1)fprintf(ptficarbone,"\n");
		if(total == 0)fprintf(ptficarbone,"\n%f",(float)(pourcentage));
	}
	free(car);
	fclose(ptficourbes);
	fclose(ptficarbone);
}

/* cette fonction effectue la simulation biologique avec le mod�le MEPSOM � partir des boules et des attributs initiaux ; elle
 rend les attributs finaux et leur evolution temporelle */

void simulmepsom2()

{
	char ficboules[50],ficatt[50],ficatt2[50], ficcourbes[50],ficcoeff[50],racine[50],chaine[50];
	char ficgraphe[50],ficarcs[50],ficdonnees[50];
	int nbjours,i,natt,j,nvmax;

	float *car,somme;

	FILE *ptficcourbes,*ptficatt2;

	graphe_s *graphe;


	fprintf(stderr,"\n racine des fichiers : ");

	scanf("%s",racine);

	fprintf(stderr,"\n 'racine.attsim': attributs complementaires en sortie ; 'racine.courbes': variations temporelles des masses");

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(ficboules,chaine);
	fprintf(stderr,"\n fichier de boules en entree (x,y,z,r,0,0,0,0) : %s ",ficboules);

	strcpy(chaine,racine);
	strcat(chaine,".arcs");
	strcpy(ficarcs,chaine);
	fprintf(stderr,"\n fichier des arcs en entree : %s ",ficarcs);

	strcpy(chaine,racine);
	strcat(chaine,".att");
	strcpy(ficatt,chaine);
	fprintf(stderr,"\n fichier des attributs complementaires en entree : %s",ficatt);

	strcpy(chaine,racine);
	strcat(chaine,".coeff");
	strcpy(ficcoeff,chaine);
	fprintf(stderr,"\n fichier des coefficients pour la diffusion en entree : %s",ficcoeff);

	strcpy(chaine,racine);
	strcat(chaine,".gr");
	strcpy(ficgraphe,chaine);
	fprintf(stderr,"\n fichier du graphe : %s",ficgraphe);

	strcpy(chaine,racine);
	strcat(chaine,".attsim");
	strcpy(ficatt2,chaine);
	fprintf(stderr,"\n nom du fichier des attributs complementaires apres decomposition en sortie : %s ",ficatt2);

	strcpy(chaine,racine);
	strcat(chaine,".courbes");
	strcpy(ficcourbes,chaine);
	fprintf(stderr,"\n nom du fichier contenant l'evolution des masses en sortie : %s",ficcourbes);

	strcpy(chaine,racine);
	strcat(chaine,".donnees");
	strcpy(ficdonnees,chaine);
	fprintf(stderr,"\n fichier des donnees : %s",ficdonnees);


	fprintf(stderr,"\n nb de jours pour la simulation = ");
	scanf("%d",&nbjours);

	fprintf(stderr,"\n nb maximum de voisins par noeud =");
	scanf("%d",&nvmax);

	fprintf(stderr,"\n pour creation du fichier graphe a l'aide des attributs, des boules et des arcs taper 1 sinon 0:");
	scanf("%d",&i);

	if(i == 1)
	{
		graphe = cregraphe_s3(ficboules,ficarcs,ficatt);
	/*	fprintf(stderr,"\n graphe->nbnoeuds = %d graphe->nel = %d",graphe->nbnoeuds,graphe->nel);*/
		ecrigraphe(graphe,ficgraphe);
	}
	else
	{
		graphe = lecgraphe(ficgraphe);;
	}
	natt = graphe->ncar - 4;
	ptficcourbes = fopen(ficcourbes,"w");

	car = calsommes(ficatt,ficboules,&natt);

	fprintf(ptficcourbes,"%d",natt);
	fprintf(ptficcourbes,"\n");

	somme = 0;

	for(j=0;j<natt;j++)
	{
		fprintf(ptficcourbes," %f",car[j]);
		somme += car[j];
	}
/*	fprintf(ptficcourbes," \n %f",somme);*/
	fprintf(ptficcourbes,"\n");

	/* creation du graphe avec le fichier de graphe*/


	/* on lance en cascade la simulation pour un jour */

	/* simulation pour nbjours jour */

	for(i = 0;i<nbjours;i++)
	{
		fprintf(stderr,"\n jour : %d",i+1);
		/* production d'enzymes, respiration, mortalite  des micro-organismes */
		miseajour2(&graphe,microorg);

		/* diffusion des enzymes */
		diffusiong2(&graphe,nvmax,fdifenz,5);

		/* decomposition de la matiere organique */
		miseajour2(&graphe,decompos);

		/* diffusion de la DOM */
		diffusiong2(&graphe,nvmax,fdidom,5);

	/* les micro-organismes mangent la DOM */
		miseajour2(&graphe,mangedom);

		car = calsommes2(graphe);

		somme = 0;

		for(j=0;j<natt;j++)
		{
			fprintf(ptficcourbes," %f",car[j]);
			somme += car[j];
		}
/*		fprintf(ptficcourbes," \n %f",somme);*/
		fprintf(ptficcourbes,"\n");

	}

	/* remplissage du fichier des attributs complementaires finaux */
	ptficatt2 = fopen(ficatt2,"w");
	fprintf(ptficatt2,"%d",graphe->ncar - 4);
/*	fprintf(stderr,"\n graphe->ncar = %d",graphe->ncar);*/
	for(i=1;i<= graphe->nel;i++)
	{
		fprintf(ptficatt2,"\n");
		for(j=0;j<graphe->ncar - 4;j++)
		{
			fprintf(ptficatt2,"%f ",graphe->carac[j+5][i]);
		}
	}
	fclose(ptficcourbes);
	fclose(ptficatt2);
}

/* cette fonction effectue la simulation biologique avec le mod�le MEPSOM � partir des boules et des attributs initiaux ; elle
 rend les attributs finaux et leur evolution temporelle */

void simulmepsom()

{
	char ficboules[50],ficatt[50],ficatt2[50], ficcourbes[50],ficcoeff[50],racine[50],chaine[50],fictrav[50],fictrav2[50];
	char ficgraphe[50];
	int nbjours,i,natt,j,nvmax;

	float *car,somme;

	FILE *ptficcourbes;



	fprintf(stderr,"\n racine des fichiers ('.bmax': boules d'eau en entree, '.att':attributs complementaires en entree, '.gr':graphe en entree, ' .coeff':coefficients pour diffusion en entree): ");

	scanf("%s",racine);

	fprintf(stderr,"\n 'racine.attsim': attributs complementaires en sortie ; 'racine.courbes': variations temporelles des masses");

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(ficboules,chaine);
	fprintf(stderr,"\n fichier de boules en entree (x,y,z,r,0,0,0,0) : %s ",ficboules);

	strcpy(chaine,racine);
	strcat(chaine,".att");
	strcpy(ficatt,chaine);
	fprintf(stderr,"\n fichier des attributs complementaires en entree : %s",ficatt);

	strcpy(chaine,racine);
	strcat(chaine,".coeff");
	strcpy(ficcoeff,chaine);
	fprintf(stderr,"\n fichier des coefficients pour la diffusion en entree : %s",ficcoeff);

	strcpy(chaine,racine);
	strcat(chaine,".gr");
	strcpy(ficgraphe,chaine);
	fprintf(stderr,"\n fichier du graphe : %s",ficgraphe);

	strcpy(chaine,racine);
	strcat(chaine,".attsim");
	strcpy(ficatt2,chaine);
	fprintf(stderr,"\n nom du fichier des attributs complementaires apres decomposition en sortie : %s ",ficatt2);



	strcpy(chaine,racine);
	strcat(chaine,".courbes");
	strcpy(ficcourbes,chaine);
	fprintf(stderr,"\n nom du fichier contenant l'evolution des masses en sortie : %s",ficcourbes);

	strcpy(chaine,racine);
	strcat(chaine,".trav");
	strcpy(fictrav,chaine);

	strcpy(chaine,racine);
	strcat(chaine,".trav2");
	strcpy(fictrav2,chaine);

	fprintf(stderr,"\n nb de jours pour la simulation = ");
	scanf("%d",&nbjours);

	fprintf(stderr,"\n nb maximum de voisins par noeud =");
	scanf("%d",&nvmax);

	ptficcourbes = fopen(ficcourbes,"w");

	car = calsommes(ficatt,ficboules,&natt);

	fprintf(ptficcourbes,"%d",natt);
	fprintf(ptficcourbes,"\n");

	somme = 0;

	for(j=0;j<natt;j++)
	{
		fprintf(ptficcourbes," %f",car[j]);
		somme += car[j];
	}
/*	fprintf(ptficcourbes," \n %f",somme);*/
	fprintf(ptficcourbes,"\n");


	/* on lance en cascade la simulation pour un jour */

	/* simulation pour nbjours jour */

	for(i = 0;i<nbjours;i++)
	{
		/* production d'enzymes, respiration, mortalite  des micro-organismes */
		if(i == 0)
		{
			miseajour(ficboules,ficatt,microorg,fictrav);
		}
		else
		{
			miseajour(ficboules,ficatt2,microorg,fictrav);
		}

		/* diffusion des enzymes */

		diffusion(ficcoeff,ficboules,fictrav,f1enz,f2enz,fictrav2);

		/* decomposition de la matiere organique */

		miseajour(ficboules,fictrav2,decompos,fictrav);

		/* diffusion de la DOM */

		diffusion(ficcoeff,ficboules,fictrav,f1dom,f2dom,fictrav2);

	/* les micro-organismes mangent la DOM */

		miseajour(ficboules,fictrav2,mangedom,ficatt2);

		car = calsommes(ficatt2,ficboules,&natt);

		somme = 0;

		for(j=0;j<natt;j++)
		{
			fprintf(ptficcourbes," %f",car[j]);
			somme += car[j];
		}
/*		fprintf(ptficcourbes," \n %f",somme);*/
		fprintf(ptficcourbes,"\n");

	}

	remove(fictrav);
	remove(fictrav2);
	fclose(ptficcourbes);
}

/* cette procedure cree de maniere interactive les fichiers necessaires a la simulation a partir d'un fichier de boules
 dans le format habituel avec les 3 zeros en trop */

/* on cree d'abord le graphe a partir des boules du fichier, ensuite on cree les fichiers contenant les vosins avec
 les differents niveau et les boules avec leurs attributs
 */

void testsim(void)
{
	char ficboules[50],ficarcs[50],ficatt[50],ficgraphe[50],ficvois[50],racine[50],chaine[50];

	int dimx,dimy,dimz,nbarcs,nbnvois,nbmaxniv,nbmaxvois;

	graphe_s *graf;

	fprintf(stderr,"\n racine des fichiers ('racine.bmax': fichier de boules en entree, 'racine.att':fichier des attributs complementaires): ");

	scanf("%s",racine);

	fprintf(stderr,"\n 'racine.arcs': fichier des arcs en sortie ; 'racine.gr': fichier du graphe des boules en sortie");

	fprintf(stderr,"\n 'racine.vois' : fichier contenant les voisins des boules aux differents niveaux en sortie ");

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(ficboules,chaine);

	fprintf(stderr,"\n fichier de boules en entree (x,y,z,r,0,0,0,0) : %s ",ficboules);

	strcpy(chaine,racine);
	strcat(chaine,".att");
	strcpy(ficatt,chaine);

	fprintf(stderr,"\n fichier des attributs complementaires des boules hormi le centre et le rayon (nb d'attibuts en tete) : %s",ficatt);

	strcpy(chaine,racine);
	strcat(chaine,".arcs");
	strcpy(ficarcs,chaine);
	fprintf(stderr,"\n nom du fichier des arcs du graphe des boules en sortie : %s ",ficarcs);



	strcpy(chaine,racine);
	strcat(chaine,".gr");
	strcpy(ficgraphe,chaine);

	fprintf(stderr,"\n nom du fichier contenant le graphe des boules en sortie : %s",ficgraphe);

	strcpy(chaine,racine);
	strcat(chaine,".vois");
	strcpy(ficvois,chaine);

	fprintf(stderr,"\n nom du fichier contenant les voisins des boules aux differents niveaux en sortie: %s",ficvois);

	fprintf(stderr,"\n dimx = dimy = dimz = ");
	scanf("%d",&dimx);
	dimy = dimx;
	dimz = dimx;

	fprintf(stderr,"\n nombre de niveaux de voisinage = ");
	scanf("%d",&nbnvois);


	fprintf(stderr,"\n nb max de noeud dans un niveau = nb max de voisins d'un noeud = ");
	scanf("%d",&nbmaxniv);

	nbmaxvois = nbmaxniv;


	/* la procedure arcs prend en entree le nom du fichier de boules, les dimensions de l'image
	 contenant les boules et ecrit en sortie le nombre d'arcs "nbarcs" et un fichier contenant le nombre d'arcs et la liste des arcs
	 */
	arcs(ficboules,ficarcs, dimx,dimy,dimz,&nbarcs,nbmaxvois);

	fprintf(stderr,"\n fin de la determination du fichier des arcs");

	/* graf contient un graphe contenant le graphe des boules */

	graf = cregraphe_s3(ficboules,ficarcs,ficatt);

	fprintf(stderr,"\n fin de la creation du graphe des boules avec les attributs complementaires");

	ecrigraphe(graf,ficgraphe);

	fprintf(stderr,"\n fin de l'ecriture du graphe dans le fichier ");

	crevoisins(graf,ficvois,nbnvois,nbmaxvois,nbmaxniv);

	fprintf(stderr,"\n fin de la creation du fichier des voisins avec les differents niveaux de voisinage");

	return;
}

/* cette procedure permet de realiser un processus de diffusion a partir d'un graphe */
/* le fichier de sortie est structure de la maniere suivante : nb de noeuds, puissance (0 proportionnel, 1 racine, 2 racine 4)
   numero du noeud, nb de noeuds connectes, numero du noeud, coefficient */

void testdif(void)
{
	char vois[50],coeff[50],boulesmax[50],racine[50],chaine[50];
	int nmax;
	float n;

	fprintf(stderr,"\n racine des fichiers ('racine.bmax': fichier de boules en entree,'racine.vois : fichier de voisinage en entree,'racine.coeff': fichier des coeffs en sortie):");
	scanf("%s",racine);

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(boulesmax,chaine);

	fprintf(stderr,"\n fichier de boules en entree (x,y,z,r,0,0,0,0) : %s ",boulesmax);

	strcpy(chaine,racine);
	strcat(chaine,".vois");
	strcpy(vois,chaine);

	fprintf(stderr,"\n fichier des voisinages : %s",vois);

	strcpy(chaine,racine);
	strcat(chaine,".coeff");
	strcpy(coeff,chaine);
	fprintf(stderr,"\n nom du fichier des coefficients en sortie : %s ",coeff);

	fprintf(stderr,"\n Puissance pour la diffusion ; n = (si n = 1 proportionnel) = ");
	scanf("%f",&n);

	fprintf(stderr,"\n nb maximum de noeuds dans la zone de voisinage = ");
	scanf("%d",&nmax);

	coeffdif1(vois,boulesmax,n,nmax,coeff);

}

/* cette procedure cree le fichier contenant les coefficients pour la diffusion a partir des fichiers de boules, de vosinage
 et de la "puissance" . Le resultat est dans le fichier coeff. "nmax" est le nombre maximum de points connectes a un point avec un
 niveau de voisinage <= nmax */

void coeffdif(char *vois,char *boulesmax,float n,int nmax,char *coeff)
{
	float *taboules,coeffniv;
	int nboules,k,nboules2,nbniv,l,m,num,q,p,ntavois,nivsign,s,**tavois,stop;
	FILE *ptvois,*ptboulesmax,*ptcoeff;
	float x,y,z,r,t1,t2,t3,t4,*tcoeffn,somme,*volboules,verif;

	ptvois = fopen(vois,"r");
	ptboulesmax = fopen(boulesmax,"r");
	ptcoeff = fopen(coeff,"w");

	fscanf(ptboulesmax,"%d",&nboules);

	fscanf(ptvois,"%d",&nboules2);

	fprintf(stderr,"\n nmax = %d ",nmax);

	if(nboules != nboules2)
	{
		fprintf(stderr,"\n nboules different de nboules2 !");
		exit(5);
	}

	fscanf(ptvois,"%d",&nbniv);

	taboules = (float*)mycalloc(nboules + 1,sizeof(float));

	tavois = allot2i(2,nmax);

	tcoeffn = (float*)mycalloc(nbniv+1,sizeof(float));

	volboules = (float*)mycalloc(nbniv+1,sizeof(float));

	/* remplissage du tableau taboules avec les boules maximales du fichier boulesmax, on indexe a partir de 1 pour
	 rester compatible avec le numerotage des noeuds a partir de 1 */

	for(k=1;k<= nboules;k++)

	{

		fscanf(ptboulesmax,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&t1,&t2,&t3,&t4);
		taboules[k] = r;
	}

	/* ecriture du nombre de noeuds et de la puissance dans le fichier de sortie */
	fprintf(ptcoeff,"%d %f",nboules,n);

	fprintf(ptcoeff,"\n");

	/* traitement de chaque noeud pour ecriture des voisins avec les coefficients */
	for(k=1;k <= nboules;k++)
	{
		if(k % 10000 == 0)fprintf(stderr,"\n k = %d",k);

		/* lecture du numero du noeud "num" */
		fscanf(ptvois,"%d",&num);
	/*	fprintf(stderr,"\n k = %d num = %d",k,num);*/

		if(num != k)
		{
			fprintf(stderr,"\n num != k !!");
			exit(5);
		}

		/* initialisation du tableau qui contiendra la somme des volumes des boules pour chaque niveau */
		for(s = 0;s<= nbniv;s++)
			volboules[s] = 0;

		ntavois = 0;

		nivsign = 0;

		/* on traite les voisins du noeud num=k */

		/* lecture du niveau de voisinage "l" et du nombre de voisins pour ce niveau "m" */
		fscanf(ptvois,"%d",&l);
		fscanf(ptvois,"%d",&m);

		stop = 0;

		while(m != 0 && stop != 1)
		{
			nivsign = l;
			if(l == nbniv)stop = 1;
			/* traitement de la boule */
			for(q=0;q<m;q++)
			{
				fscanf(ptvois,"%d",&p);

				if(ntavois >= nmax)
				{
					fprintf(stderr,"\n ntavois >= nmax");
					exit(4);
				}
				else
				{
					tavois[0][ntavois] = p;
					tavois[1][ntavois] = l;
					ntavois++;
					r = taboules[p];
					volboules[l] += (4.0/3.0)*M_PI*r*r*r;
				}
			}
			if(stop == 0)
			{
				fscanf(ptvois,"%d",&l);
				fscanf(ptvois,"%d",&m);
			}
		}
		/* on a rempli tavois avec les noeuds connectes a k dans les differents niveaux de voisinage
		   le dernier niveau significatif est nivsign <= nbniv
		 */
		/* on ecrit dans le fichier de sortie le noeud k, le nb de noeuds connectes et les noeuds connectes avec les coefficients */

		/* ecriture du numero du noeud et du nombre de noeuds connectes en incluant lui meme */
		fprintf(ptcoeff,"\n %d %d",k,ntavois+1);

	/*	fprintf(stderr,"\n k = %d ntavois = %d",k, ntavois);*/

		/* calcul des coefficients pour la normalisation */
/*		fprintf(stderr,"\n coefficients pour la normalisation pour le noeud %d , nb de niveaux significatifs = %d",k,nivsign);

		fprintf(stderr,"\n n = %f",n);

		fprintf(stderr,"\n");*/

		/* "somme" est la somme des coefficients */
		somme = 0;

		for(s=0;s<=nivsign;s++)
		{
			tcoeffn[s] = 1.0/pow(s+1,n);
		/*	fprintf(stderr,"%f ",tcoeffn[s]);*/
			somme += tcoeffn[s];
		}
	/*	fprintf(stderr,"\n");*/
		for(s=0;s<=nivsign;s++)
		{
			tcoeffn[s] = tcoeffn[s]/somme;
		/*	fprintf(stderr," %f",tcoeffn[s]);*/
		}

/*		fprintf(stderr,"\n somme = %f",somme);

		fprintf(stderr,"\n ntavois = %d",ntavois);

		fprintf(stderr,"\n");*/

		/* ecriture des noeuds connectes avec le coefficient */

		verif = tcoeffn[0];
		/* verif contient la somme des coefficients qui doit etre egale a 1 */

		/* ecriture du noeud avec le coefficient tcoeff[0] est le coefficient du niveau du noeud */
		coeffniv = tcoeffn[0];
		fprintf(ptcoeff,"\n %d %f",k,coeffniv);

		for (q=0;q<ntavois;q++)
		{
			/* calcul du coefficient */
			p = tavois[0][q];
			m = tavois[1][q];

			/* p et m sont respectivement le numero du noeud connecte et le niveau de voisinage */
			/* nivsign est le niveau de voisinage significatif */
			/* n est la puissance definissant la repartition entre les niveaux de voisinage */

			/* ecriture du numero du noeud et du coefficient */

			r = taboules[p];
			/* r est le rayon de la boule du noeud k */

			/* calcul du coefficient correspondant au niveau */
			coeffniv = ((tcoeffn[m])*(4.0/3.0)*M_PI*r*r*r)/volboules[m];
/*			fprintf(stderr,"noeud: %d r=%f niv=%d coeff=%f ",p,r,m,coeffniv);*/
			fprintf(ptcoeff,"\n %d %f",p,coeffniv);
			verif += coeffniv;
		}
/*		fprintf(stderr,"\n verif = %f",verif);*/
		if(ntavois >= 0)
		{
			if(verif > 1.1 || verif < 0.9)fprintf(stderr,"\n ALERTE verif = %f",verif);
		}
		fprintf(ptcoeff,"\n");
	}

	freetin(tavois,2,nmax);
	free(taboules);
	free(tcoeffn);
	free(volboules);
	fclose(ptvois);
	fclose(ptcoeff);
	fclose(ptboulesmax);
}

/* cette fonction determine un fichier d'attributs complementaires en fonction d'un fichier de boules et de donnees globales */

void intercalatt()
{
	char boulesmax[50],ficdonnees[50], ficatt[50],racine[50],chaine[50];


	fprintf(stderr,"\n racine des fichiers ('racine.bmax': fichier de boules en entree,'racine.donnees : fichier de donnees en entree,'racine.att': fichier des attributs en sortie):");
	scanf("%s",racine);

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(boulesmax,chaine);

	fprintf(stderr,"\n fichier de boules en entree (x,y,z,r,0,0,0,0) : %s ",boulesmax);

	strcpy(chaine,racine);
	strcat(chaine,".donnees");
	strcpy(ficdonnees,chaine);

	fprintf(stderr,"\n fichier des donnees : %s",ficdonnees);

	strcpy(chaine,racine);
	strcat(chaine,".att");
	strcpy(ficatt,chaine);
	fprintf(stderr,"\n nom du fichier des attributs en sortie : %s ",ficatt);

	calcatt(boulesmax,ficdonnees,ficatt,calattributs);
}

/* cette fonction prend en entree : le fichier des boules, des donnees globales sur le sol, une fonction calculant les attributs
 d'une boule en fonction de ses caracteristiques et des donnees globales et rend en sortie un fichier contenant
 les attributs des boules */
/* le fichier donnees globales sur le sol est structur� de la maniere suivante : nb de donnees, donnees */
/* la fonction de calcul des attributs prend en entree les coordonnees du centre de la boule et le rayon, le tableau des donnees globales
 et le nombre de ces donnees globales, elle rend en sortie un tableau d'attributs complementaires ainsi que le nombre d'attributs
 */

void calcatt(char *ficboules, char *donnees, char *ficatt,void (*fonc)(float,float,float,float,float*,int,double,int*,float**))
{
	FILE *ptficboules,*ptdonnees,*ptficatt;
	float *tabdonnees,v,x,y,z,r,a,b,c,d,*tabatt;
	double somboules,somdom,totdom,pourdom;
	int nattg,i,nboules,natt,j,nattmax;

	ptficboules = fopen(ficboules,"r");
	ptdonnees = fopen(donnees,"r");
	ptficatt = fopen(ficatt,"w");

	derboule_ext = 0;

	fscanf(ptdonnees,"%d",&nattg);
	/* nattg contient le nombre de donnees globales */
	/* remplissage du tableau tabdonnees avec les donnees globales */

	tabdonnees = (float*)mycalloc(nattg,sizeof(float));

	/* calcul de la somme des volumes des boules */
	fscanf(ptficboules,"%d",&nboules);
	somboules = 0;
	for(i=0;i<nboules;i++)
	{
		fscanf(ptficboules,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);
		somboules += (4.0/3.0)*(M_PI)*r*r*r;
	}
	fclose(ptficboules);
	fopen(ficboules,"r");

	for(i=0;i<nattg;i++)
	{
		fscanf(ptdonnees,"%f",&v);
		tabdonnees[i] = v;
	}

	/* remplissage du fichier ficatt avec les attributs complementaires */
	fscanf(ptficboules,"%d",&nboules);

	nattmax = 10000;

	tabatt = (float*)mycalloc(nattmax,sizeof(float));
	somdom = 0;
	totdom = 0;
	for(i=0;i<nboules;i++)
	{

		fscanf(ptficboules,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);

		if(i == nboules - 1)derboule_ext = 1;

		fonc(x,y,z,r,tabdonnees,nattg,somboules,&natt,&tabatt);

		if(natt >= nattmax)
		{
			exit(56);
		}


		if(i == 0)
		{
			fprintf(ptficatt,"%d",natt);
		}
		fprintf(ptficatt,"\n");
		for(j =0;j<natt;j++)
		{
			fprintf(ptficatt,"%f ",tabatt[j]);
		}
		totdom += tabatt[1];
		if(tabatt[0] == 0)somdom += tabatt[1];
	}
	pourdom = (somdom/totdom)*100;
	fprintf(stderr,"\n pourcentage de DOM non accessible directement = %f",(float)(pourdom));
	free(tabatt);
	fclose(ptficboules);
	fclose(ptdonnees);
	fclose(ptficatt);
	free(tabdonnees);
}

/* cette fonction prend en entree : le fichier des boules, des donnees globales sur le sol, une fonction calculant les attributs
 d'une boule en fonction de ses caracteristiques et des donnees globales et rend en sortie un fichier contenant
 les attributs des boules */
/* le fichier donnees globales sur le sol est structur� de la maniere suivante : nb de donnees, donnees */
/* la fonction de calcul des attributs prend en entree les coordonnees du centre de la boule et le rayon, le tableau des donnees globales
 et le nombre de ces donnees globales, elle rend en sortie un tableau d'attributs complementaires ainsi que le nombre d'attributs

 repartition des micro-organismes par paquets de maniere aleatoire en fonction du volume des boules
 */

void calcatt3(char *ficboules, char *donnees, char *ficatt)
{
	FILE *ptficboules,*ptdonnees,*ptficatt;
	float *tabdonnees,v,x,y,z,r,a,b,c,d,mpaq,massetotale,dmicro,rmin,vmin,ddom,**attrcomp,vmdom,vmmic,somdom;
	float somboules;
	int nattg,nboules,j,nbalea,*tabaleat,n,ntrav,i,nattcomp,nbaleatoire;

	ptficboules = fopen(ficboules,"r");
	ptdonnees = fopen(donnees,"r");
	ptficatt = fopen(ficatt,"w");

	fscanf(ptdonnees,"%d",&nattg);
	/* nattg contient le nombre de donnees globales */
	/* remplissage du tableau tabdonnees avec les donnees globales */

	tabdonnees = (float*)mycalloc(nattg,sizeof(float));

	for(i=0;i<nattg;i++)
	{
		fscanf(ptdonnees,"%f",&v);
		tabdonnees[i] = v;
	}
	dmicro = tabdonnees[1];
	ddom = tabdonnees[0];

	/* calcul de la somme des volumes des boules et du rayon minimum*/
	fscanf(ptficboules,"%d",&nboules);
	if(nboules == 0)
	{
		fprintf(stderr,"\n nboules = 0 !!!!!");
		exit(50);
	}

	fscanf(ptficboules,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);
	somboules = (4.0/3.0)*(M_PI)*r*r*r;
	rmin = r;

	for(i=1;i<nboules;i++)
	{
		fscanf(ptficboules,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);

		somboules += (4.0/3.0)*(M_PI)*r*r*r;

		if(r < rmin)rmin = r;
	}

	vmin = (4.0/3.0)*(M_PI)*rmin*rmin*rmin;
	nbalea = ceil(somboules/vmin);

	fprintf(stderr,"\n valeur maximale du nombre aleatoire = %d",nbalea);

	tabaleat = (int*)mycalloc(nbalea,sizeof(int));

	fclose(ptficboules);
	fopen(ficboules,"r");
	fscanf(ptficboules,"%d",&nboules);

	/* calcul du numero de boule en fonction du nombre aleatoire */
	/* repartition de la masse de fructose de maniere homogene en conservant la meme densite partout */

	nattcomp = 6;
	attrcomp = allot2f(nattcomp,nboules);

	/* initialisation de attrcomp � 0 */
	for(i=0;i<nattcomp;i++)
		for(j=0;j<nboules;j++)
		{
			attrcomp[i][j] = 0;
		}

	ntrav = 0;
	for(i=0;i<nboules;i++)
	{

		fscanf(ptficboules,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);

		n = floor(((4.0/3.0)*(M_PI)*r*r*r)/vmin);
		if(n == 0)n=1;

		/* fprintf(stderr,"\n n = %d",n);*/

		attrcomp[1][i] = ((4.0/3.0)*(M_PI)*r*r*r)*ddom;

		if(ntrav + n  > nbalea)
		{
			fprintf(stderr,"\n ntrav + n - 1 > nbalea ntrav = %d n = %d nbalea = %d!!",ntrav,n,nbalea);
			exit(51);
		}
		for(j=ntrav;j < ntrav + n;j++)tabaleat[j]=i;
		ntrav += n;
	}

	fprintf(stderr,"\n ntrav = %d",ntrav);
	/* tabaleat[i] contient la boule � considerer quand le nombre aleatoire est i */


	/* remplissage du fichier ficatt avec les attributs complementaires */
	/* on remplit d'abord le tableau attrcom avec les attributs complementaires */


	/* on repartit la bio-masse de micro-organismes par paquets de masse totale/nmax_ext */
	/* on traite sequentiellement les nmax_ext paquets */

	/* calcul de la masse totale de micro-organismes dans l'ensemble des pores */
	massetotale = somboules*dmicro;

	fprintf(stderr,"\n masse totale = %f nmax_ext = %f",massetotale,nmax_ext);

	mpaq = massetotale/nmax_ext;
	fprintf(stderr,"\n");

	for(i=0;i<nmax_ext;i++)
	{
		/* injection d'un paquet de bio-masse de mpaq micro g dans les pores */
		nbaleatoire = rand();

	/*	fprintf(stderr," 1= %d ntrav = %d",nbaleatoire,ntrav);*/

		nbaleatoire = nbaleatoire % ntrav;

		/* fprintf(stderr," 2= %d",nbaleatoire);*/

		/* nbaleatoire est un nombre entier compris entre 0 et ntrav-1 */
		attrcomp[0][tabaleat[nbaleatoire]] += mpaq;
	}

	/* remplissage du fichier des attributs complementaires */

	fprintf(ptficatt,"%d",nattcomp);

	vmdom = 0;
	vmmic = 0;

	for(i=0;i<nboules;i++)
	{
		fprintf(ptficatt,"\n");
		for(j =0;j<nattcomp;j++)
		{
			fprintf(ptficatt,"%f ",attrcomp[j][i]);
		}
		vmdom += attrcomp[1][i];
		vmmic += attrcomp[0][i];
	}
	somdom = ddom*somboules;
	fprintf(stderr,"\n masse initiale de DOM = %f ; masse de DOM distribuee = %f ; rapport = %f",somdom,vmdom,somdom/vmdom);
	fprintf(stderr,"\n masse initiale de micro-org. = %f ; masse de micro-org distribuee = %f ; rapport = %f",massetotale,vmmic,massetotale/vmmic);

	fclose(ptficboules);
	fclose(ptdonnees);
	fclose(ptficatt);
	free(tabdonnees);
	freetfloat(attrcomp,nattcomp,nboules);
}

/* on met une masse donn�e de mati�te organique et de micro organismes de mani�re homog�nes dans deux portions de l'image volumique
 le fichier des donn�es contient les deux masses globales e les deux fenetres 3D: x,y,z,dx,dy,dz
 */
/* cette fonction prend en entree : le fichier des boules, des donnees globales sur le sol, une fonction calculant les attributs
 d'une boule en fonction de ses caracteristiques et des donnees globales et rend en sortie un fichier contenant
 les attributs des boules */
/* le fichier donnees globales sur le sol est structur� de la maniere suivante : nb de donnees, donnees */
/* la fonction de calcul des attributs prend en entree les coordonnees du centre de la boule et le rayon, le tableau des donnees globales
 et le nombre de ces donnees globales, elle rend en sortie un tableau d'attributs complementaires ainsi que le nombre d'attributs
 */

void calcatt4(char *ficboules, char *donnees, char *ficatt)
{
	FILE *ptficboules,*ptdonnees,*ptficatt;
	float *tabdonnees,v,x,y,z,r,a,b,c,d,x1,y1,z1,dx1,dy1,dz1,x2,y2,z2,dx2,dy2,dz2,mic,mat,vboule,masse1,masse2;
	float somboules,somboules2;
	int nattg,i,nboules,natt,j,nb,nb2,compt,nrempmic,nbfmic,nbfmat,nrempmat;
	float totmic,totmat,**tab,mass1,mass2,zero;

	ptficboules = fopen(ficboules,"r");
	ptdonnees = fopen(donnees,"r");
	ptficatt = fopen(ficatt,"w");

	fprintf(stderr,"\n nombre de boules � remplir de micro organismes = ");
	scanf("%d",&nrempmic);

	fprintf(stderr,"\n nombre de boules � remplir de matiere organique = ");
	scanf("%d",&nrempmat);

	fscanf(ptdonnees,"%d",&nattg);
	/* nattg contient le nombre de donnees globales dans ce cas on a 14 donn�es globales */
	/* remplissage du tableau tabdonnees avec les donnees globales */

	fprintf(stderr,"\n nb de donn�es globales = %d",nattg);

	tabdonnees = (float*)mycalloc(nattg,sizeof(float));

	for(i=0;i<nattg;i++)
	{
		fscanf(ptdonnees,"%f",&v);
		tabdonnees[i] = v;
	}

	mic = tabdonnees[0];
	mat = tabdonnees[1];

	x1 = tabdonnees[2];
	y1 = tabdonnees[3];
	z1 = tabdonnees[4];
	dx1 = tabdonnees[5];
	dy1 = tabdonnees[6];
	dz1 = tabdonnees[7];

	x2 = tabdonnees[8];
	y2 = tabdonnees[9];
	z2 = tabdonnees[10];
	dx2 = tabdonnees[11];
	dy2 = tabdonnees[12];
	dz2 = tabdonnees[13];

	fprintf(stderr,"\n x1=%f y1= %f z1 = %f dx1=%f dy1=%f dz1=%f x2=%f y2=%f z2=%f dx2=%f dy2=%f dz2=%f",x1,y1,z1,dx1,dy1,dz1,x2,y2,z2,dx2,dy2,dz2);

	/* calcul de la somme des volumes des boules dans les deux fen�tres, la premi�re fenetre contient les micro organismes
	 et la deuxi�me la matiere organique */
	fscanf(ptficboules,"%d",&nboules);
	tab = allotab2f(2,nboules);
	somboules = 0;
	somboules2 = 0;
	nb = 0;
	nb2 = 0;
	nbfmic = 0;
	nbfmat = 0;
	fprintf(stderr,"\n nombre de boules = %d",nboules);
	fprintf(stderr,"\n");
	for(i=0;i<nboules;i++)
	{
		fscanf(ptficboules,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);

		/* on teste si le centre de la boule est dans la premiere fen�tre contenant les micro-organismes */

		if(x -r >= x1 && x +r <= x1 + dx1 && y-r >= y1 && y+r <= y1 + dy1 && z-r >= z1 && z+r <= z1 + dz1)
		{
			nbfmic++;
			if(nbfmic <= nrempmic)
			{
				somboules += (float)((4.0/3.0)*(M_PI)*r*r*r);
				nb++;
			}
		}

		/* on teste si le centre de la boule est dans la deuxi�me fen�tre contenant la matiere organique */
		if(x-r >= x2 && x+r <= x2 + dx2 && y-r >= y2 && y+r <= y2 + dy2 && z-r >= z2 && z+r <= z2 + dz2)
		{
			nbfmat++;
			if(nbfmat <= nrempmat)
			{
				somboules2 += (float)((4.0/3.0)*(M_PI)*r*r*r);
				nb2++;
			}
		}

	}

	/* somboules contient la somme des volumes des boules dans la fenetre contenant les micro organismes
		somboules2 contient la somme des volumes des boules dans la fenetre contenant la matiere organique
		mic est la masse totale de micro organismes et mat la masse totale de matiere organique
	 */
	fprintf(stderr,"\n somme des volumes des boules contenant les micro organismes = %f nombre de boules = %d",somboules,nb);
	fprintf(stderr,"\n somme des volumes des boules contenant la matiere organique = %f nombre de boules = %d",somboules2,nb2);
	fprintf(stderr,"\n masse totale initiale de micro organismes initiale = %f ; masse totale initiale de matiere organique = %f",mic,mat);
	if(somboules * somboules2 ==0)exit(449);

	fclose(ptficboules);
	fopen(ficboules,"r");


	/* remplissage du fichier ficatt avec les attributs complementaires */

	totmic = 0;
	totmat = 0;
	nbfmat = 0;
	nbfmic = 0;
	fscanf(ptficboules,"%d",&nboules);
	natt = 6;
	fprintf(stderr,"\n");
	compt = 0;
	for(i=0;i<nboules;i++)
	{

		fscanf(ptficboules,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);

		if(i == 0)
		{
			fprintf(ptficatt," %d ",natt);
		}
		fprintf(ptficatt,"\n");

		/* ecriture des attributs de la boule i */

		/* ecriture de la masse de micro organismes masse1 et de la masse de mati�re organique masse2 */

		masse1 = 0;
		masse2 = 0;

		/* ecriture de la masse de micro organismes */

		/* on teste si le centre de la boule est dans la fen�tre contenant les micro-organismes */

		if(x-r >= x1 && x+r <= x1 + dx1 && y-r >= y1 && y+r <= y1 + dy1 && z-r >= z1 && z+r <= z1 + dz1)
		{
			nbfmic++;
			if(nbfmic <= nrempmic)
			{
				vboule = (float)((4.0/3.0)*(M_PI)*r*r*r);
				masse1 = (float)(mic * (vboule/somboules));
			}
		}
		 /* on teste si la boule est dans la fenetre contenant la matiere organique */
		if(x-r >= x2 && x+r <= x2 + dx2 && y-r >= y2 && y+r <= y2 + dy2 && z-r >= z2 && z+r <= z2 + dz2)
		{
			nbfmat++;
			if(nbfmat <= nrempmat)
			{
				vboule = (float)((4.0/3.0)*(M_PI)*r*r*r);
				masse2 = (float)(mat * (vboule/somboules2));
			}
		}
		fprintf(ptficatt," %f ",masse1);
		fprintf(ptficatt," %f ",masse2);
		/* fprintf(stderr,"\n masse1 = %f masse2 = %f",masse1,masse2);*/

		tab[0][i] = masse1;
		tab[1][i] = masse2;

		/* if(masse1 - tab[0][i] != 0)
		{
			fprintf(stderr,"\n ALERTE ROUGE : %f",masse1 - tab[0][i]);
		}
		if(masse2 - tab[1][i] != 0)
		{
			fprintf(stderr,"\n ALERTE ROUGE : %f",masse2 - tab[1][i]);
		}*/

		totmic += masse1;
		totmat += masse2;

		for(j =0;j<4;j++)
		{
			zero = 0.0;
			fprintf(ptficatt," %f ",zero);
		}
	}

	fprintf(stderr,"\n masse totale de micro organismes = %f ; masse totale de matiere organique = %f",totmic,totmat);
	fclose(ptficboules);
	fclose(ptdonnees);
	fclose(ptficatt);
	free(tabdonnees);

	compt = 0;
	ptficatt = fopen(ficatt,"r");
	/* lecture du nombre d'attributs */
	fscanf(ptficatt,"%d",&natt);
	fprintf(stderr,"\n");
	totmic = 0;
	totmat = 0;
	fprintf(stderr,"\n nboules = %d",nboules);
	for(i=0;i<nboules;i++)
	{
		fscanf(ptficatt,"%f",&mass1);
		fscanf(ptficatt,"%f",&mass2);
	/*	if(mass1 != tab[0][i])
		{
			fprintf(stderr,"\n masse1 = %f tab = %f dif = %f i = %d",mass1,tab[0][i],mass1-tab[0][i],i);
		}
		if(mass2 != tab[1][i])
		{
			fprintf(stderr,"\n masse2 = %f tab = %f dif = %f i = %d",mass2,tab[1][i],mass2-tab[1][i],i);
		}*/
		totmic += mass1;
		totmat += mass2;
		for(j =0;j<4;j++)
		{
			fscanf(ptficatt,"%f",&mass1);
		}
	}
	fprintf(stderr,"\n total masse de mic dans le fichier att initial = %f total mat = %f",totmic,totmat);
	fclose(ptficatt);
	freetfloat(tab,2,nboules);
}


void calcatt2(char *ficboules, char *donnees, char *ficatt,void (*fonc)(float,float,float,float,float*,int,double,float,float,int*,float**))
{
	FILE *ptficboules,*ptdonnees,*ptficatt;
	float *tabdonnees,v,x,y,z,r,a,b,c,d,*tabatt,somme1,somme2,totmic,totmat;
	double somboules,somdom,totdom,pourdom;
	int nattg,i,nboules,natt,j,nattmax;

	ptficboules = fopen(ficboules,"r");
	ptdonnees = fopen(donnees,"r");
	ptficatt = fopen(ficatt,"w");

	derboule_ext = 0;

	fscanf(ptdonnees,"%d",&nattg);
	/* nattg contient le nombre de donnees globales */
	/* remplissage du tableau tabdonnees avec les donnees globales */

	tabdonnees = (float*)mycalloc(nattg,sizeof(float));

	/* calcul de la somme des volumes des boules */
	fscanf(ptficboules,"%d",&nboules);
	somboules = 0;
	somme1 = 0;
	somme2 = 0;
	for(i=0;i<nattg;i++)
	{
		fscanf(ptdonnees,"%f",&v);
		tabdonnees[i] = v;

	}
	/* somme1 contient la somme du volume des boules dont le rayon en voxels est compris entre tabdonnees[0] et tabdonnees[1} */
	/* somme2 contient la somme du volume des boules dont le rayon en voxels est compris entre tabdonnees[2] et tabdonnees[3] */
	/* tabdonnees[4] contient la masse totale de MO et tabdonnees[5] la masse totale de BM */

	for(i=0;i<nboules;i++)
	{
		fscanf(ptficboules,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);
		somboules += (4.0/3.0)*(M_PI)*r*r*r;
		if(r>=tabdonnees[0] && r<=tabdonnees[1])somme1 += (4.0/3.0)*(M_PI)*r*r*r;
		if(r>=tabdonnees[2] && r<=tabdonnees[3])somme2 += (4.0/3.0)*(M_PI)*r*r*r;
	}

	fclose(ptficboules);
	fopen(ficboules,"r");



	/* remplissage du fichier ficatt avec les attributs complementaires */
	fscanf(ptficboules,"%d",&nboules);

	/* fprintf(stderr,"\n nb de boules = %d",nboules);*/

	nattmax = 10000;

	tabatt = (float*)mycalloc(nattmax,sizeof(float));
	somdom = 0;
	totdom = 0;
	for(i=0;i<nboules;i++)
	{
	/*	if(i % 10000 == 0)fprintf(stderr,"\n i = %d",i);*/
		/* fprintf(stderr,"\n i = %d",i);*/

		fscanf(ptficboules,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&a,&b,&c,&d);

		if(i == nboules - 1)derboule_ext = 1;

		fonc(x,y,z,r,tabdonnees,nattg,somboules,somme1,somme2,&natt,&tabatt);

		if(natt >= nattmax)
		{
			exit(56);
		}

		if(i == 0)
		{
			fprintf(ptficatt,"%d",natt);
		}
		fprintf(ptficatt,"\n");
		for(j =0;j<natt;j++)
		{
			fprintf(ptficatt,"%f ",tabatt[j]);
		}
		totdom += tabatt[1];
		if(tabatt[0] == 0)somdom += tabatt[1];
	}
	pourdom = (somdom/totdom)*100;
	fprintf(stderr,"\n pourcentage de DOM non accessible directement = %f",(float)(pourdom));
	free(tabatt);
	fclose(ptficboules);
	fclose(ptdonnees);
	fclose(ptficatt);
	free(tabdonnees);
}

/* cette fonction calcule les attributs d'une boule en fonction de son centre, son rayon, des donnees globales (tadonnees et nattg),
 la somme des volumes des boules. Elle renvoie un pointeur sur un tbleau contenant les attributs compl�mentaires et un pointeur
 sur leur nombre (tabatta et natt)
 */
void calattributs(float x,float y,float z,float r,float *tadonnees,int nattg,double somboules,int *natt,float** tabatta)
{
	float *tabatt;
	float vpore,mfructose,dfructose,dmicro,mmicromax,massetotale,bmasse;
	int i,nbaleatoire;


	*natt = 6;

	/* on exprime la resolution en micron m */

	/* les attributs sont :
	 masse de micro-organismes (MB), masse de matiere organique dissoute (DOM),masse de matiere organique a decompositionn lente (SOM),
	 masse de matiere organique a decomposition rapide (FOM), masse d'enzyme (ENZ), masse de CO2 */

	tabatt = *tabatta;
	for(i = 0;i<*natt;i++)
		tabatt[i] = 0;

	/* calcul du volume du pore en voxel cube */
	vpore = ((4.0/3.0)*(M_PI)*r*r*r);


	dfructose = tadonnees[0];
	/* dfructose contient la masse volumique de fructose exprimee en micron g par voxel cube */

	/* calcul de la masse de fructose en micron g */
	mfructose = vpore*dfructose;

	tabatt[1] = mfructose;

	/* calcul de la masse de micro-organismes */

	/* calcul de la masse de MB qui serait presente avec une repartition homogene exprimee en 10E-6 Ang g par voxel cube */
	dmicro = tadonnees[1];

	if(nmax_ext == -1)
	{
		tabatt[0] = vpore*dmicro;
		goto fin;
	}

	mmicromax = vpore*dmicro*nmax_ext;
	/* mmicromax contient la masse maximum de micro-organismes dans le pore */

	/* calcul de la masse totale de micro-organismes dans l'ensemble des pores */
	massetotale = somboules*dmicro;

	nbaleatoire = rand();
	nbaleatoire = nbaleatoire % 1000;
	/* nb aleatoire est un nombre aleatoire compris entre 0 et 10000 */

	bmasse = ((nbaleatoire + 1)/1000.0)*mmicromax;


	if((massetotale - sombio_ext) >= bmasse)
	{
		tabatt[0] = bmasse;
		sombio_ext += bmasse;
	}
	else
	{
		if(massetotale > sombio_ext)
		{
			tabatt[0] = (massetotale - sombio_ext);
			sombio_ext = massetotale;
		}
		else
		{
			tabatt[0] = 0;
		}
	}
	if(derboule_ext == 1)
	{
		fprintf(stderr,"\n massetotale = %f sombio_ext = %f",(massetotale),sombio_ext);
		tabatt[0] = (massetotale) - sombio_ext;
		sombio_ext += tabatt[0];
		fprintf(stderr,"\n massetotale = %f sombio_ext = %f tabatt[0] = %f",massetotale,sombio_ext,tabatt[0]);
	}
fin: ;
}



/* cette fonction calcule les attributs d'une boule en fonction de son centre, son rayon, des donnees globales (tadonnees et nattg),
 la somme des volumes des boules. Elle renvoie un pointeur sur un tbleau contenant les attributs compl�mentaires et un pointeur
 sur leur nombre (tabatta et natt)
 */
void calattributs2(float x,float y,float z,float r,float *tadonnees,int nattg,double somboules,float somme1,float somme2,int *natt,float** tabatta)
{
	float *tabatt;
	float vpore;
	int i;


	*natt = 6;

	/* on exprime la resolution en micron m */

	/* les attributs sont :
	 masse de micro-organismes (MB), masse de matiere organique dissoute (DOM),masse de matiere organique a decompositionn lente (SOM),
	 masse de matiere organique a decomposition rapide (FOM), masse d'enzyme (ENZ), masse de CO2 */

	tabatt = *tabatta;
	for(i = 0;i<*natt;i++)
		tabatt[i] = 0;

	/* tadonnees contient les 4 rayons definissant les pores contenant la MO et les pores contenant la BM exprimes en voxels
	 ainsi que les masses globales de MO et de BM qui sont reparties proportionellement aux volumes des boules exprimes en micron g */

	/* tous les calculs sont effectu�s en voxels-cube */

	/* calcul du volume du pore en voxel cube */
	vpore = ((4.0/3.0)*(M_PI)*r*r*r);

	/* cas d'un pore contenant de la MO */
	if(r>=tadonnees[0] && r<=tadonnees[1])
	{
		tabatt[1] = (vpore/somme1)*tadonnees[4];
	}
	/* cas d'un pore contenant de la BM */
	if(r>=tadonnees[2] && r<=tadonnees[3])
	{
		tabatt[0] = (vpore/somme2)*tadonnees[5];
	}

}

/* diffusion d'une fonction des attributs des noeuds sur les noeuds connectes en utilisant les coefficients de connectivite (ficoeff)
 et les attributs (ficatt)
 ficoeff : fichier des coefficients de diffusion ; ficett : fichier des attributs ; boulesmax : fichier des boules
 fonc1 est une fonction prenant en entree le tableau des attributs d'un noeud, le nombre des attributs
  et qui rend la masse
 a repartir entre les noeuds connectes ainsi que les nouveaux attributs du noeud emetteur
 fonc2 est une fonction qui prend en entree la masse qui arrive sur le noeud (masse totale multipliee par le
 coefficient de diffusion) et qui met a jour le tableau des attributs du noeud recepteur
 la sortie de diffusion est un tableau avec les nouveaux attributs des noeuds hors centre et rayon
 on remplit le fichier ficatt2 avec les nouveaux attributs des boules apres diffusion */

void diffusion(char *ficoeff,char *boulesmax,char *ficatt,float (*fonc1)(float**,float**,int), void (*fonc2)(float,float**,int),char *ficatt2)
{
	float **attributs1,**attributs2,at,puissance,coef,*car1,*car2,masse,masse2;
	int natt,nboules,i,j,nboules2,n,num,nv,k;
	FILE *ptficoeff,*ptficatt,*ptboulesmax,*ptficatt2;

	/* remplissage du tableau des attributs complementaires par lecture du fichier ficatt */
	ptficatt = fopen(ficatt,"r");
	ptficoeff = fopen(ficoeff,"r");
	ptboulesmax = fopen(boulesmax,"r");
	ptficatt2 = fopen(ficatt2,"w");

	fscanf(ptboulesmax,"%d",&nboules);
	/* nboules contient le nombre de boules */

	fscanf(ptficatt,"%d",&natt);

	/* natt contient le nombre d'attributs complementaires */

	/* allocation de memoire pour le tableau des attributs */

	attributs1 = allot2f(natt,nboules);
	attributs2 = allot2f(natt,nboules);

	car1 = (float*)mycalloc(natt,sizeof(float));
	car2 = (float*)mycalloc(natt,sizeof(float));

	/* remplissage des tableaux des attributs */

	for(i=0;i<nboules;i++)
		for(j=0;j<natt;j++)
		{
			fscanf(ptficatt,"%f",&at);
			attributs1[j][i] = at;
			attributs2[j][i] = at;
		}



	fscanf(ptficoeff,"%d %f",&nboules2,&puissance);
	if(nboules2 != nboules)
	{
		fprintf(stderr,"\n nboules2 != nboules !!");
		exit(56);
	}

	for(i=0;i<nboules;i++)
	{
		if(i % 10000 == 0)fprintf(stderr,"\n i = %d",i);

		fscanf(ptficoeff,"%d %d",&num,&n);
		if(num != (i+1))
		{
			fprintf(stderr,"\n num != i+1");
			exit(30);
		}
		if(n != 0)
		{
			/* calcul de la masse a repartir venant de la boule i */
			for(k=0;k<natt;k++)
				car1[k] = attributs1[k][i];

			for(k=0;k<natt;k++)
			{
				car2[k] = attributs2[k][i];
			}

			masse = fonc1(&car1,&car2,natt);

		/* mise a jour des attributs */

			for(k=0;k<natt;k++)
			{
				attributs2[k][i] = car2[k];
			}
		}


		for(j=0;j<n;j++)
		{
			fscanf(ptficoeff,"%d %f",&nv,&coef);

			/* la boule i (en indexant a partir de 0) est connectee a la boule nv (en indexant a partir de 1, nv peut etre egale a i)
			   avec le coefficient coef
			 */

			/* calcul de la masse allouee a la boule nv (indexage a partir de 1) */
			masse2 = masse*coef;

			/* remplissage de car1 avec les attributs du noeud nv */

			for(k=0;k<natt;k++)
			car1[k] = attributs2[k][nv-1];

			/* mise a jour des attributs du noeud nv apres l'arrivee de la masse 'masse' */

			fonc2(masse2,&car1,natt);

			/* mise a jour des attributs  */
			for(k=0;k<natt;k++)
			{
				attributs2[k][nv-1] = car1[k];
			}
		}
	}

	/* attributs2 contient les nouveaux attributs des noeuds apres la diffusion */
	/* ecriture de ces nouveaux attributs dans un fichier d'attributs */

	/* ecriture du nombre d'attributs */
	fprintf(ptficatt2,"%d",natt);

	for(i=0;i<nboules;i++)
	{
		fprintf(ptficatt2,"\n");
		for(j=0;j<natt;j++)
		{
			at = attributs2[j][i];
			fprintf(ptficatt2," %f ",at);
		}
	}

	free(car1);
	free(car2);
	freetfloat(attributs1,natt,nboules);
	freetfloat(attributs2,natt,nboules);
	fclose(ptficatt);
	fclose(ptficoeff);
	fclose(ptboulesmax);
	fclose(ptficatt2);

}

/* cette fonction est similaire a "diffusion" avec en plus un nombre d'iterations 'it' pour discretiser dans le temps
 elle est equivalente a lancer it fois diffusion */
/* diffusion d'une fonction des attributs des noeuds sur les noeuds connectes en utilisant les coefficients de connectivite (ficoeff)
 et les attributs (ficatt)
 ficoeff : fichier des coefficients de diffusion ; ficett : fichier des attributs ; boulesmax : fichier des boules
 fonc1 est une fonction prenant en entree le tableau des attributs d'un noeud, le nombre des attributs
  et qui rend la masse
 a repartir entre les noeuds connectes ainsi que les nouveaux attributs du noeud emetteur
 fonc2 est une fonction qui prend en entree la masse qui arrive sur le noeud (masse totale multipliee par le
 coefficient de diffusion) et qui met a jour le tableau des attributs du noeud recepteur
 la sortie de diffusion est un tableau avec les nouveaux attributs des noeuds hors centre et rayon
 on remplit le fichier ficatt2 avec les nouveaux attributs des boules apres diffusion */

void diffusionit(char *ficoeff,char *boulesmax,char *ficatt,float (*fonc1)(float**,float**,int), void (*fonc2)(float,float**,int),char *ficatt2,int nbit)
{
	float **attributs1,**attributs2,at,puissance,coef,*car1,*car2,masse,masse2;
	int natt,nboules,i,j,nboules2,n,num,nv,k,l;
	FILE *ptficoeff,*ptficatt,*ptboulesmax,*ptficatt2;

	/* remplissage du tableau des attributs complementaires par lecture du fichier ficatt */
	ptficatt = fopen(ficatt,"r");
	ptficoeff = fopen(ficoeff,"r");
	ptboulesmax = fopen(boulesmax,"r");
	ptficatt2 = fopen(ficatt2,"w");

	fscanf(ptboulesmax,"%d",&nboules);
	/* nboules contient le nombre de boules */

	fscanf(ptficatt,"%d",&natt);
	/* natt contient le nombre d'attributs complementaires */

/*	fprintf(stderr,"\n natt = %d nboules = %d",natt,nboules);*/

	/* allocation de memoire pour le tableau des attributs */
	attributs1 = allot2f(natt,nboules);
	attributs2 = allot2f(natt,nboules);

	car1 = (float*)mycalloc(natt,sizeof(float));
	car2 = (float*)mycalloc(natt,sizeof(float));

	/* remplissage des tableaux des attributs */

	for(i=0;i<nboules;i++)
		for(j=0;j<natt;j++)
		{
			fscanf(ptficatt,"%f",&at);
			attributs1[j][i] = at;
			attributs2[j][i] = at;
		}

	/* processus de diffusion via la lecture du fichier des coefficients */


	fscanf(ptficoeff,"%d %f",&nboules2,&puissance);
	if(nboules2 != nboules)
	{
		fprintf(stderr,"\n nboules2 != nboules !!");
		exit(56);
	}

	/* on itere nbit fois la diffusion */

	for(l=0;l<nbit;l++)
	{
		fprintf(stderr,"\n iteration : %d",l);
		for(i=0;i<nboules;i++)
		{
			if(i % 10000 == 0)fprintf(stderr,"\n i = %d",i);

			fscanf(ptficoeff,"%d %d",&num,&n);
			if(num != (i+1))
			{
				fprintf(stderr,"\n num != i+1");
				exit(30);
			}
			/* calcul de la masse a repartir venant de la boule i */

			/* si le nombre des boules connectees est nul alors rien ne se diffuse et on ne fait rien */
			if(n != 0)
			{
				for(k=0;k<natt;k++)
					car1[k] = attributs1[k][i];

				for(k=0;k<natt;k++)
				{
					car2[k] = attributs2[k][i];
				}

				masse = fonc1(&car1,&car2,natt);

			/* mise a jour des attributs */


				for(k=0;k<natt;k++)
				{
					attributs2[k][i] = car2[k];
				}
			}
			for(j=0;j<n;j++)
			{
				fscanf(ptficoeff,"%d %f",&nv,&coef);

				masse2 = masse*coef;

				/* remplissage de car1 avec les attributs du noeud nv */

				for(k=0;k<natt;k++)
					car1[k] = attributs2[k][nv-1];

				fonc2(masse2,&car1,natt);

				/* mise a jour des attributs  */
				for(k=0;k<natt;k++)
				{
					attributs2[k][nv-1] = car1[k];
				}
			}
			/* verification de la conservation des masses  */
	/*		compar(attributs1,attributs2,nboules,natt);*/

			/* on met attributs2 dans attributs1 pour l'iteration suivante */
			if(n != 0)
			{
				for(i=0;i<nboules;i++)
					for(j=0;j<natt;j++)
					{
						attributs1[j][i] = attributs2[j][i];
					}
			}

		}
	}
	/* attributs2 contient les nouveaux attributs des noeuds apres la diffusion */
	/* ecriture de ces nouveaux attributs dans un fichier d'attributs */

	/* ecriture du nombre d'attributs */
	fprintf(ptficatt2,"%d",natt);

	for(i=0;i<nboules;i++)
	{
		fprintf(ptficatt2,"\n");
		for(j=0;j<natt;j++)
		{
			at = attributs2[j][i];
			fprintf(ptficatt2," %f ",at);
		}
	}

	free(car1);
	free(car2);
	freetfloat(attributs1,natt,nboules);
	freetfloat(attributs2,natt,nboules);
	fclose(ptficatt);
	fclose(ptficoeff);
	fclose(ptboulesmax);
	fclose(ptficatt2);

}

/* procedure uniquement pour verification */

float compar(float **attributs1,float **attributs2,int nboules,int natt)
{
	float car1[1000],car2[1000],verif,pourcent;
	int j,i;

	for(i=0;i<natt;i++)
	{
		car2[i] = 0;
		car1[i] = 0;
	}

	for(i = 0;i<nboules;i++)
	{
		for(j=0;j<natt;j++)
		{
			car2[j] += attributs2[j][i];
			car1[j] += attributs1[j][i];
		}
	}
	for(j=0;j<natt;j++)
	{
		if(car1[j] != 0)
		{
			pourcent = fabs(car1[j] - car2[j])/car1[j];
		}
	    else
	    {
	    	pourcent = 0;
		}
		fprintf(stderr,"\n somme initiale de la caracteristique %d = %f somme apres diffusion = %f pourcentage = %f",j,car1[j],car2[j],pourcent);
	}

	return(verif);
}

/* procedure de verification */

void ecrat(float **attributs1,int nboules)
{
	int i;
	float somme;

	somme = 0;
	fprintf(stderr,"\n");
	fprintf(stderr,"\n");
	for(i = 0;i<nboules;i++)
	{
		fprintf(stderr,"%d=%f ",i+1,attributs1[0][i]);
		somme += attributs1[0][i];
	}
	fprintf(stderr,"\n SOMME = %f",somme);
}

/* procedure pour la diffusion */

void lancdif()
{
	char boulesmax[50],ficoeff[50], ficatt[50],racine[50],chaine[50],ficatt2[50];


	fprintf(stderr,"\n racine ('.bmax':boules en entree,'.att':attributs en entree;'.coeff': diffusionen en entree;'.att_d': attributs en sortie);");
	scanf("%s",racine);

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(boulesmax,chaine);

	fprintf(stderr,"\n fichier de boules en entree (x,y,z,r,0,0,0,0) : %s ",boulesmax);

	strcpy(chaine,racine);
	strcat(chaine,".coeff");
	strcpy(ficoeff,chaine);

	fprintf(stderr,"\n fichier des coefficients de diffusion en entree : %s",ficoeff);

	strcpy(chaine,racine);
	strcat(chaine,".att");
	strcpy(ficatt,chaine);
	fprintf(stderr,"\n nom du fichier des attributs complementaires en entree : %s ",ficatt);

	strcpy(chaine,racine);
	strcat(chaine,".att_d");
	strcpy(ficatt2,chaine);
	fprintf(stderr,"\n nom du fichier des attributs complementaires apres diffusion en sortie : %s ",ficatt2);

	diffusion(ficoeff,boulesmax,ficatt,f1enz,f2enz,ficatt2);

	fprintf(stderr,"\n fin de la diffusion");

}

/* procedure pour la diffusion iterative */

void lancdifit()
{
	char boulesmax[50],ficoeff[50], ficatt[50],racine[50],chaine[50],ficatt2[50];
	int nbit;


	fprintf(stderr,"\n racine ('.bmax':boules en entree,'.att':attributs en entree;'.coeff': diffusionen en entree;'.att_d': attributs en sortie);");
	scanf("%s",racine);

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(boulesmax,chaine);

	fprintf(stderr,"\n fichier de boules en entree (x,y,z,r,0,0,0,0) : %s ",boulesmax);

	strcpy(chaine,racine);
	strcat(chaine,".coeff");
	strcpy(ficoeff,chaine);

	fprintf(stderr,"\n fichier des coefficients de diffusion en entree : %s",ficoeff);

	strcpy(chaine,racine);
	strcat(chaine,".att");
	strcpy(ficatt,chaine);
	fprintf(stderr,"\n nom du fichier des attributs complementaires en entree : %s ",ficatt);

	strcpy(chaine,racine);
	strcat(chaine,".att_d");
	strcpy(ficatt2,chaine);
	fprintf(stderr,"\n nom du fichier des attributs complementaires apres diffusion en sortie : %s ",ficatt2);

	fprintf(stderr,"\n nombre d'iterations = ");
	scanf("%d",&nbit);

	diffusionit(ficoeff,boulesmax,ficatt,f12,f2,ficatt2,nbit);

	fprintf(stderr,"\n fin de la diffusion");

}

/* procedure pour la diffusion de proche en proche */

/* procedure pour la diffusion iterative */

void lancdifgr()
{
	char ficgraphe[50],ficatt2[50],racine[50],chaine[50],ficboules[50],ficatt[50],ficarcs[50];
	int nbit,nvmax;
	graphe_s *graf;


	fprintf(stderr,"\n racine ('.bmax':boules en entree,'.att':attributs en entree,'.arcs': arcs en entree,'.att_d': attributs en sortie,'.gr':graphe en sortie);");
	scanf("%s",racine);

	strcpy(chaine,racine);
	strcat(chaine,".bmax");
	strcpy(ficboules,chaine);
	fprintf(stderr,"\n fichier de boules en entree (x,y,z,r,0,0,0,0) : %s ",ficboules);

	strcpy(chaine,racine);
	strcat(chaine,".att");
	strcpy(ficatt,chaine);
	fprintf(stderr,"\n nom du fichier des attributs complementaires en entree : %s ",ficatt);

	strcpy(chaine,racine);
	strcat(chaine,".arcs");
	strcpy(ficarcs,chaine);
	fprintf(stderr,"\n nom du fichier des arcs en entree : %s ",ficarcs);

	strcpy(chaine,racine);
	strcat(chaine,".gr");
	strcpy(ficgraphe,chaine);
	fprintf(stderr,"\n fichier du graphe en sortie: %s",ficgraphe);

	strcpy(chaine,racine);
	strcat(chaine,".att_d");
	strcpy(ficatt2,chaine);
	fprintf(stderr,"\n nom du fichier des attributs complementaires apres diffusion en sortie : %s ",ficatt2);

	fprintf(stderr,"\n nombre d'iterations = ");
	scanf("%d",&nbit);

	fprintf(stderr,"\n nombre maximum de voisins d'un noeud = ");
	scanf("%d",&nvmax);

	graf = cregraphe_s3(ficboules,ficarcs,ficatt);
	fprintf(stderr,"\n graf->nbnoeuds = %d graf->nel = %d",graf->nbnoeuds,graf->nel);

	fprintf(stderr,"\n fin de la creation du graphe des boules avec les attributs complementaires");

	ecrigraphe(graf,ficgraphe);


	diffusiong(ficgraphe,nvmax,fdifenz,nbit,ficatt2);

	fprintf(stderr,"\n fin de la diffusion");

}

/* cette procedure calcule la masse a repartir en fonction des caracteristiques et met a jour les carcteristiques */
float f12(float **car,float **car2,int ncar)
{
	float masse,taux;

	taux = 0.005;
	masse = taux*(*car)[0];
	/* le taux ne doit pas etre superieur a 1 */
	(*car2)[0] -= masse;
	return(masse);
}


/* cette procedure calcule la masse d'enzymes a repartir en fonction des caracteristiques pour diffusion
 avec methode des coefficients et met a jour les caracteristiques, on considere que la masse diffusee est de 25% par jour */

float f1enz(float **car,float **car2,int ncar)
{
	float masse,taux;

	taux = 0.25;
	masse = taux*(*car)[4];
	/* le taux ne doit pas etre superieur a 1 */
	(*car2)[4] -= masse;
	return(masse);
}

/* cette procedure met a jour les caracteristiques d'un pore qui recoit une mase m d'enzymes */

void f2enz(float m,float **car,int ncar)
{
	(*car)[4] += m;
}

/* cette procedure calcule la masse de DOM a repartir en fonction des caracteristiques pour diffusion
 avec methode des coefficients et met a jour les caracteristiques, on considere que la masse diffusee est de 25% par jour */

float f1dom(float **car,float **car2,int ncar)
{
	float masse,taux;

	taux = 0.25;
	masse = taux*(*car)[1];
	/* le taux ne doit pas etre superieur a 1 */
	(*car2)[1] -= masse;
	return(masse);
}

/* cette procedure met a jour les caracteristiques d'un pore qui recoit une mase m de DOM */

void f2dom(float m,float **car,int ncar)
{
	(*car)[1] += m;
}


/* cette procedure met a jour les caracteristiques en fonction de la masse a ajouter */
void f2(float m,float **car,int ncar)
{
	(*car)[0] += m;
}


/* cette procedure cree le fichier contenant les coefficients pour la diffusion a partir des fichiers de boules, de vosinage
 et de la "puissance" . Le resultat est dans le fichier coeff. "nmax" est le nombre maximum de points connectes a un point avec un
 niveau de voisinage <= nmax */

void coeffdif1(char *vois,char *boulesmax,float n,int nmax,char *coeff)
{
	float *taboules,coeffniv;
	int nboules,k,nboules2,nbniv,l,m,num,q,p,ntavois,nivsign,s,**tavois,stop;
	FILE *ptvois,*ptboulesmax,*ptcoeff;
	float x,y,z,r,t1,t2,t3,t4,*tcoeffn,somme,*volboules,verif;

	ptvois = fopen(vois,"r");
	ptboulesmax = fopen(boulesmax,"r");
	ptcoeff = fopen(coeff,"w");

	fscanf(ptboulesmax,"%d",&nboules);

	fscanf(ptvois,"%d",&nboules2);

	fprintf(stderr,"\n nmax = %d ",nmax);

	if(nboules != nboules2)
	{
		fprintf(stderr,"\n nboules different de nboules2 !");
		exit(5);
	}

	fscanf(ptvois,"%d",&nbniv);

	taboules = (float*)mycalloc(nboules + 1,sizeof(float));

	tavois = allot2i(2,nmax);

	tcoeffn = (float*)mycalloc(nbniv+1,sizeof(float));

	volboules = (float*)mycalloc(nbniv+1,sizeof(float));

	/* remplissage du tableau taboules avec les boules maximales du fichier boulesmax, on indexe a partir de 1 pour
	 rester compatible avec le numerotage des noeuds a partir de 1 */

	for(k=1;k<= nboules;k++)

	{

		fscanf(ptboulesmax,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&t1,&t2,&t3,&t4);
		taboules[k] = r;
	}

	/* ecriture du nombre de noeuds et de la puissance dans le fichier de sortie */
	fprintf(ptcoeff,"%d %f",nboules,n);

	fprintf(ptcoeff,"\n");

	/* traitement de chaque noeud pour ecriture des voisins avec les coefficients */
	for(k=1;k <= nboules;k++)
	{
		if(k % 1000 == 0)fprintf(stderr,"\n k = %d",k);

		/* lecture du numero du noeud "num" */
		fscanf(ptvois,"%d",&num);
	/*	fprintf(stderr,"\n k = %d num = %d",k,num);*/

		if(num != k)
		{
			fprintf(stderr,"\n num != k !!");
			exit(5);
		}

		/* initialisation du tableau qui contiendra la somme des volumes des boules pour chaque niveau */
		for(s = 0;s<= nbniv;s++)
			volboules[s] = 0;

		ntavois = 0;

		nivsign = 0;

		/* on traite les voisins du noeud num=k */

		/* lecture du niveau de voisinage "l" et du nombre de voisins pour ce niveau "m" */
		fscanf(ptvois,"%d",&l);
		fscanf(ptvois,"%d",&m);

		nivsign = 0;

		stop = 0;

		while(m != 0 && stop != 1)
		{
			nivsign = l;
			if(l == nbniv)stop = 1;
			for(q=0;q<m;q++)
			{
				fscanf(ptvois,"%d",&p);

				if(ntavois >= nmax)
				{
					fprintf(stderr,"\n ntavois >= nmax");
					exit(4);
				}
				else
				{
					tavois[0][ntavois] = p;
					tavois[1][ntavois] = l;
					ntavois++;
					r = taboules[p];
					volboules[l] += (4.0/3.0)*M_PI*r*r*r;
				}
			}
			if(stop == 0)
			{
				fscanf(ptvois,"%d",&l);
				fscanf(ptvois,"%d",&m);
			}
		}
		/* on a rempli tavois avec les noeuds connectes a k dans les differents niveaux de voisinage
		   le dernier niveau significatif est nivsign <= nbniv
		 */
		/* on ecrit dans le fichier de sortie le noeud k, le nb de noeuds connectes et les noeuds connectes avec les coefficients */

		/* ecriture du numero du noeud et du nombre de noeuds connectes */
		fprintf(ptcoeff,"\n %d %d",k,ntavois);
	/*	fprintf(stderr,"\n k = %d ntavois = %d",k, ntavois);*/

		/* calcul des coefficients pour la normalisation */
/*		fprintf(stderr,"\n coefficients pour la normalisation pour le noeud %d , nb de niveaux significatifs = %d",k,nivsign);

		fprintf(stderr,"\n n = %f",n);

		fprintf(stderr,"\n");*/

		/* "somme" est la somme des coefficients */
		somme = 0;

		for(s=1;s<=nivsign;s++)
		{
			tcoeffn[s] = 1.0/pow(s,n);
		/*	fprintf(stderr,"%f ",tcoeffn[s]);*/
			somme += tcoeffn[s];
		}
	/*	fprintf(stderr,"\n");*/
		for(s=1;s<=nivsign;s++)
		{
			tcoeffn[s] = tcoeffn[s]/somme;
		/*	fprintf(stderr," %f",tcoeffn[s]);*/
		}

/*		fprintf(stderr,"\n somme = %f",somme);

		fprintf(stderr,"\n ntavois = %d",ntavois);

		fprintf(stderr,"\n");*/

		/* ecriture des noeuds connectes avec le coefficient */

		verif = 0;
		/* verif contient la somme des coefficients qui doit etre egale a 1 */
		for (q=0;q<ntavois;q++)
		{
			/* calcul du coefficient */
			p = tavois[0][q];
			m = tavois[1][q];

			/* p et m sont respectivement le numero du noeud connecte et le niveau de voisinage */
			/* nivsign est le niveau de voisinage significatif */
			/* n est la puissance definissant la repartition entre les niveaux de voisinage */

			/* ecriture du numero du noeud et du coefficient */

			r = taboules[p];
			/* r est le rayon de la boule du noeud k */

			/* calcul du coefficient correspondant au niveau */
			coeffniv = ((tcoeffn[m])*(4.0/3.0)*M_PI*r*r*r)/volboules[m];
/*			fprintf(stderr,"noeud: %d r=%f niv=%d coeff=%f ",p,r,m,coeffniv);*/
			fprintf(ptcoeff,"\n %d %f",p,coeffniv);
			verif += coeffniv;
		}
/*		fprintf(stderr,"\n verif = %f",verif);*/
		if(ntavois >0)
		{
			if(verif > 1.1 || verif < 0.9)fprintf(stderr,"\n ALERTE verif = %f",verif);
		}
		fprintf(ptcoeff,"\n");
	}

	freetin(tavois,2,nmax);
	free(taboules);
	free(tcoeffn);
	free(volboules);
	fclose(ptvois);
	fclose(ptcoeff);
	fclose(ptboulesmax);
}


/* cette fonction effectue une mise a jour des attributs complementaires par la fonction en parametre 'misaj' qui prend en entree
 les caracteristiques de la boule et les attributs complementaires et rend les nouveaux attributs complementaires */
/* boulesmax est le nom du fichier des boules, ficatt celui des attributs coompl�mentaires, ficatt2 est le nom du fichier
 en sortie des nouveaux attributs complementaires
 */

void microorg_sl2(float x,float y,float z,float ray,float **car,int natt)
{
	double r,mu,mp,alpha,ech,trav;

	ech = ec_ext;
	r = r_ext/(double)(deltasim_ext); mu = mu_ext/(double)(deltasim_ext); alpha = alpha_ext/(double)(deltasim_ext);

	/* r est la fraction de la masse de micro-organismes qui est transformee en CO2 par respiration. Elle est egale a 0.02 en jour -1 */

	/* mp est la masse perdue par les micro-organismes par jour */
	mp = (alpha + r + mu)*(double)((*car)[0]);
	if((alpha + r + mu) > 1)
	{
		fprintf(stderr,"\n alpha + r + mu = %f rectification de alpha,r,mu",alpha+r+mu);
		trav = 1/(alpha + r + mu);
		alpha = alpha/trav;
		r = r/trav;
		mu = mu/trav;
		mp = (double)((*car)[0]);

	}
	/* mp est exprimee en 10E-6 Angstrom g */

	/* mise a jour des masses */
	(*car)[5] = (float)((double)((*car)[5]) + (ech*r*(double)((*car)[0])));
	(*car)[1] = (float)((double)((*car)[1]) + (ech*mu*(1-inac_ext)*(double)((*car)[0])));
	(*car)[3] = (float)((double)((*car)[3]) + (ech*mu*inac_ext*(double)((*car)[0])));
	(*car)[4] = (float)((double)((*car)[4]) + (ech*alpha*(double)((*car)[0])));
	(*car)[0] = (float)((double)((*car)[0]) - mp);
}

/* pour les donn�es de Marc on utilise les attributs normalement FOM (caract�ristique 3) et ENZ (caract�ristique 5) comme ADS et NER */
void pesticides_sl2(float x,float y,float z,float ray,float **car,int natt)
{
	double k1,k2,fr;
	float d2,d4,d1,car2,car4,car1;


	k1 = k1_ext/deltasim_ext;
	k2 = k2_ext/deltasim_ext;
	fr = fr_ext/deltasim_ext;

	car1 = (*car)[1];
	car2 = (*car)[2];
	car4 = (*car)[4];

	d2 = (float)(k1*((*car)[1] - k2*(*car)[2] - fr*(*car)[2]));
	d4 = (float)(fr*(*car)[2]);
	d1 = (float)(k2*(*car)[2] - k1*(*car)[1]);

	if( (*car)[2] + d2 >= 0 && (*car)[4] + d4 >= 0 && (*car)[1] + d1 >= 0)
	{
		/* mise � jour de ADS */
		(*car)[2] += (float)(k1*car1 - k2*car2 - fr*car2);

		/* mise � jour de NER */
		(*car)[4] += (float)(fr*car2);

		/* mise � jpour de la DOM */
		(*car)[1] += (float)(k2*car2 - k1*car1);
	}
	else fprintf(stderr,"$");

}

void miseajour(char *boulesmax,char *ficatt,void (*misaj)(float,float,float,float,float**,int),char *ficatt2)
{
	float *car1,r,x,y,z,at,t1,t2,t3,t4 ;
	int natt,nboules,i,j;
	FILE *ptficatt2,*ptficatt,*ptboulesmax;

	ptficatt = fopen(ficatt,"r");
	ptboulesmax = fopen(boulesmax,"r");
	ptficatt2 = fopen(ficatt2,"w");

	fscanf(ptboulesmax,"%d",&nboules);
	/* nboules contient le nombre de boules */

	fscanf(ptficatt,"%d",&natt);
	/* natt contient le nombre d'attributs complementaires */

	fprintf(ptficatt2,"%d",natt);

/*	fprintf(stderr,"\n natt = %d",natt);
	fprintf(stderr,"\n natt = %d nboules = %d",natt,nboules);*/

	car1 = (float*)mycalloc(natt,sizeof(float));

	/* traitement iteratif des noeuds */
	for(i=0;i<nboules;i++)
	{
		fscanf(ptboulesmax,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&t1,&t2,&t3,&t4);

	/*	fprintf(stderr,"\n %f %f %f %f",x,y,z,r);*/

		for(j=0;j<natt;j++)
		{
			fscanf(ptficatt,"%f",&at);
			car1[j] = at;
		}

		misaj(x,y,z,r,&car1,natt);

		fprintf(ptficatt2,"\n");

/*		fprintf(stderr,"\n boucle natt = %d",natt);
		fprintf(stderr,"\n");*/

		for(j=0;j<natt;j++)
		{
			fprintf(ptficatt2," %f",car1[j]);
	/*		fprintf(stderr," %f",car1[j]);*/
		}

	}
	fclose(ptficatt);
	fclose(ptficatt2);
	fclose(ptboulesmax);
}


void miseajour2(graphe_s **graf,void (*misaj)(float,float,float,float,float**,int))
{
	float *car1,r,x,y,z ;
	int natt,nboules,i,j;
	graphe_s *graphe;

	graphe = *graf;

	nboules = graphe->nel;
	/* nboules contient le nombre de boules */

	natt = graphe->ncar - 4;
	/* natt contient le nombre d'attributs complementaires */

/*	fprintf(stderr,"\n natt = %d nboules = %d",natt,nboules);*/

	car1 = (float*)mycalloc(natt,sizeof(float));

	/* traitement iteratif des noeuds */
	for(i=1;i<=nboules;i++)
	{
		x = graphe->carac[1][i];
		y = graphe->carac[2][i];
		z = graphe->carac[3][i];
		r = graphe->carac[4][i];

	/*	fprintf(stderr,"\n %f %f %f %f",x,y,z,r);*/

		for(j=0;j<natt;j++)
		{
			car1[j] = graphe->carac[j+5][i];
		}

		misaj(x,y,z,r,&car1,natt);

/*		fprintf(stderr,"\n boucle natt = %d",natt);
		fprintf(stderr,"\n");*/

		for(j=0;j<natt;j++)
		{
			graphe->carac[j+5][i] = car1[j];
	/*		fprintf(stderr," %f",car1[j]);*/
		}

	}
	free(car1);
}


/* cette fonction prend en entree un fichier d'attributs complementaires, un fichier de boules et rend en sortie la somme des attributs
 ainsi que le nombre de sommes (nombre d'attributs) */

float *calsommes(char *ficatt,char *boulesmax,int *natt)
{
	float *car1,at;
	int nat,nboules,i,j;
	FILE *ptficatt,*ptboulesmax;

	ptficatt = fopen(ficatt,"r");
	ptboulesmax = fopen(boulesmax,"r");

	fscanf(ptboulesmax,"%d",&nboules);
	/* nboules contient le nombre de boules */

	fscanf(ptficatt,"%d",&nat);
	*natt = nat;
	/* natt contient le nombre d'attributs complementaires */

	car1 = (float*)mycalloc(nat,sizeof(float));

	for(i=0;i<nat;i++)car1[i]=0;

	/* traitement iteratif des noeuds */
	for(i=0;i<nboules;i++)
	{
		for(j=0;j<nat;j++)
		{
			fscanf(ptficatt,"%f",&at);
			car1[j] += at;
		}
	}

	fclose(ptficatt);
	fclose(ptboulesmax);
	return(car1);
}

float *calsommes2(graphe_s *graphe)
{
	float *car1;
	int nat,nboules,i,j;


	nboules = graphe->nel;
	/* nboules contient le nombre de boules */

	nat = graphe->ncar - 4;
	/* natt contient le nombre d'attributs complementaires */

	car1 = (float*)mycalloc(nat,sizeof(float));

	for(i=0;i<nat;i++)car1[i]=0;

	/* traitement iteratif des noeuds */
	for(i=1;i<=nboules;i++)
	{
		for(j=0;j<nat;j++)
		{
			car1[j] += graphe->carac[j+5][i];
		}
	}
	return(car1);
}

double *calsommes3(graphe_s *graphe)
{
	double *car1;
	int nat,nboules,i,j;

	nboules = graphe->nel;
	/* nboules contient le nombre de boules */

	nat = graphe->ncar - 4;
	/* natt contient le nombre d'attributs complementaires */

	car1 = (double*)mycalloc(nat,sizeof(double));

	for(i=0;i<nat;i++)car1[i]=0;

	/* traitement iteratif des noeuds */
	for(i=1;i<=nboules;i++)
	{
		for(j=0;j<nat;j++)
		{
				car1[j] += (double)(graphe->carac[j+5][i]);
		}
	}
	return(car1);
}


/* fonction de mise a jour des attributs pour la production d'enzymes par les micro-organismes
 la structure du fichier des attributs complementaires est la suivante: MB,DOM,FOM,SOM,ENZ,CO2 */

void prodenzy(float x,float y,float z,float r,float **car,int natt)
{
	float mu,mp;

	mu = 0.01;
	/* mu est la fraction de la masse de micro-organismes qui est transformee en enzymes. Elle est egale a 0.01 en jour -1 */

	/* mp est la masse d'enzymes produite */
	mp = mu*(*car)[0];

	/* mise a jour des masses d'enzymes et de micro-organismes */
	(*car)[0] -= mp;
	(*car)[4] += mp;
}

/* fonction de mise a jour des attributs pour la respiration des micro-organismes. Le taux est egal a 0.02 en jour -1 */

void respir(float x,float y,float z,float ray,float **car,int natt)
{
	float r,mp;

	r = 0.02;
	/* r est la fraction de la masse de micro-organismes qui est transformee en CO2 par respiration. Elle est egale a 0.02 en jour -1 */

	/* mp est la masse de CO2 produite */
	mp = r*(*car)[0];

	/* mise a jour des masses d'enzymes et de micro-organismes */
	(*car)[0] -= mp;
	(*car)[5] += mp;
}

/* fonction de mise a jour pour la production d'enzymes, la respiration et la mortalite des micro-organismes pour un jour*/

void microorg(float x,float y,float z,float ray,float **car,int natt)
{
	float r,mu,mp,alpha;

	r = 0.02; mu = 0.001; alpha = 0.01;

	/* r est la fraction de la masse de micro-organismes qui est transformee en CO2 par respiration. Elle est egale a 0.02 en jour -1 */

	/* mp est la masse perdue par les micro-organismes par jour */
	mp = (alpha + r + mu)*(*car)[0];

	/* mise a jour des masses */
	(*car)[5] += r*(*car)[0];
	(*car)[1] += 0.5*(*car[0])*mu;
	(*car)[3] += 0.5*(*car[0])*mu;
	(*car)[4] += (*car[0])*alpha;
	(*car)[0] -= mp;
}

/* fonction de mise a jour pour la production d'enzymes, la respiration et la mortalite des micro-organismes pour un jour pour
 SL2 */

/* fonction de mise a jour pour l'assimilation de la DOM par les micro-organismes pour un jour*/

void mangedom(float x,float y,float z,float ray,float **car,int natt)
{
	float cdom,kb,mdom;

	cdom = 0.7; kb = 0.264;

	mdom = ((cdom*(*car)[1])/(kb+(*car)[1]))*(*car)[0];

	if(mdom > (*car)[1])mdom = (*car)[1];

	(*car)[1] -= mdom;

	(*car)[0] += mdom;

}

/* fonction de mise a jour pour l'assimilation de la DOM par les micro-organismes pour un jour*/

void mangedom_sl2(float x,float y,float z,float ray,float **car,int natt)
{
	double cdom,kb,mdom,mdom2;

/*	if((*car)[0] != 0 && (*car)[1] != 0)
	fprintf(stderr,"\n masse de micro organismes initiale = %f ; masse de mati�re organique initiale = %f",(*car)[0],(*car)[1]);*/

	kb = (double)(kb_ext);


	if((kb + (double)((*car)[1])) == 0)
	{
		mdom = 0;
		fprintf(stderr,"\n masse de DOM = 0");
	}
	else
	{

		cdom = cdom_ext/(double)(deltasim_ext);

/*		fprintf(stderr,"\n kb=%f (*car)[1]=%f (*car)[0]=%f cdom=%f ec_ext=%f",kb,(*car)[1],(*car)[0],cdom,ec_ext);

		rapport = (double)((*car)[1])/(kb + (double)((*car)[1]));*/

		mdom = ((cdom*(double)((*car)[1]))/(kb+(double)((*car)[1])))*((double)((*car)[0])*ec_ext);
		som_ext += mdom;


/*		fprintf(stderr,"\n rapport = %lf",rapport);*/

		if(mdom > (double)((*car)[1]))mdom = (*car)[1];
	}

	mdom2 = mdom/ec_ext;

/*	fprintf(stderr,"\n mdom2 = %f mdom = %f",mdom2,mdom);

	fprintf(stderr,"\n 1 : (*car)[1] (DOM) = %f (*car)[0] (MICRO) = %f",(*car)[1],(*car)[0]);*/

	(*car)[1] -= (float)(mdom);

	(*car)[0] += (float)(mdom2);

	/* fprintf(stderr,"\n 2 : (*car)[1] (DOM) = %f (*car)[0] (MICRO) = %f",(*car)[1],(*car)[0]);*/
/*	if((*car)[0] != 0 && (*car)[1] != 0)
	{
		fprintf(stderr,"\n masse de micro organismes finale = %f ; masse de mati�re organique finale = %f mdom = %lf ; mdom2 = %lf",(*car)[0],(*car)[1],mdom,mdom2);
		if(mdom == 0)
		{
			fprintf(stderr,"\n mdom = 0 ALERTE !");
			exit(12);
		}
	}*/

}
/* fonction de mise a jour pour la decomposition de la matiere organique par les enzymes l'unite de temps est le jour -1 */

void decompos(float x,float y,float z,float ray,float **car,int natt)
{
	float cfom,km,csom,mfom,msom;

	cfom = 0.1; km = 0.264; csom = 0.01;

	/* mfom et msom sont les masses de FOM et de SOM decomposes par les enzymes */

/*	fprintf(stderr,"\n (*car)[4] = %f (*car)[2] = %f (*car)[3] = %f",(*car)[4],(*car)[2],(*car)[3]);*/

	mfom = ((cfom*(*car)[4])/(km+(*car)[4]))*(*car)[2];

	msom = ((csom*(*car)[4])/(km+(*car)[4]))*(*car)[3];

/*	fprintf(stderr,"\n mfom = %f msom = %f",mfom,msom);*/

	(*car)[2] -= mfom;

	(*car)[3] -= msom;

	(*car)[1] += mfom + msom;
}


/* cette fonction implemente la production d'enzymes par les MB, la respiration des MB, la mortalite des MB, la decomposition
 de la FOM et de la SOM en DOM par les enzymes, l'assimilation de la DOM par les MB */

void testmisaj()
{

}

/* cette fonction implemente la selection des pores remplis d'eau pour la decomposition microbienne */
/* elle prend en entree les boules, les attributs complementaires, le fichier du graphe, une fonction de selection et
 rend en sortie un nouveau fichier de boules (fichier des boules d'eau) et un nouveau fichier d'attributs complementaires */

void selecpores(char *boulesmax,char *ficatt,char *figraphe,int (*drain)(float,float,float,float,float*,int,graphe_s*),char *boules2,char *ficatt2)
{
	float *car1,r,x,y,z,at,t1,t2,t3,t4 ;
	int natt,nboules,i,j,nboules2;
	FILE *ptboules2,*ptficatt,*ptboulesmax,*ptficatt2;
	graphe_s *graphe;

	fprintf(stderr,"\n entree dans selecpores ");

	ptficatt = fopen(ficatt,"r");
	ptboulesmax = fopen(boulesmax,"r");
	ptficatt2 = fopen(ficatt2,"w");
	ptboules2 = fopen(boules2,"w");

	graphe = lecgraphe(figraphe);

	fscanf(ptboulesmax,"%d",&nboules);
	/* nboules contient le nombre de boules */

	fscanf(ptficatt,"%d",&natt);
	/* natt contient le nombre d'attributs complementaires */

	fprintf(ptficatt2,"%d",natt);

	fprintf(stderr,"\n natt = %d nboules = %d",natt,nboules);

	car1 = (float*)mycalloc(natt,sizeof(float));

	/* traitement iteratif des noeuds pour compter les boules a selectionner */
	nboules2 = 0;
	for(i=0;i<nboules;i++)
	{
		fscanf(ptboulesmax,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&t1,&t2,&t3,&t4);

		for(j=0;j<natt;j++)
		{
			fscanf(ptficatt,"%f",&at);
			car1[j] = at;
		}

		if(drain(x,y,z,r,car1,natt,graphe) == 1)nboules2++;
	}
	fprintf(stderr,"\n nboules2 = %d",nboules2);

	fclose(ptboulesmax);
	ptboulesmax = fopen(boulesmax,"r");
	fclose(ptficatt);
	ptficatt = fopen(ficatt,"r");
	fscanf(ptficatt,"%d",&natt);
	fscanf(ptboulesmax,"%d",&nboules);

	/* ecriture du nombre de boules dans le nouveau fichier des boules */
	fprintf(ptboules2,"%d",nboules2);
	for(i=0;i<nboules;i++)
	{
		fscanf(ptboulesmax,"%f %f %f %f %f %f %f %f",&x,&y,&z,&r,&t1,&t2,&t3,&t4);

		for(j=0;j<natt;j++)
		{
			fscanf(ptficatt,"%f",&at);
			car1[j] = at;
		}

		if(drain(x,y,z,r,car1,natt,graphe) == 1)
		{
			fprintf(ptboules2,"\n %f %f %f %f %f %f %f %f",x,y,z,r,0.0,0.0,0.0,0.0);
			fprintf(ptficatt2,"\n");
			for(j=0;j<natt;j++)
			{
				fprintf(ptficatt2,"%f ",car1[j]);
			}

		}
	}
	fclose(ptboulesmax);
	fclose(ptficatt2);
	fclose(ptficatt);
	fclose(ptboules2);
}

/* fonction de selection des pores selon la loi de young renvoie 1 ou 0 */

int drainyoung(float x,float y,float z,float r,float *car,int natt,graphe_s *graphe)
{
/*	fprintf(stderr,"\n entree dans drainyoung");
	fprintf(stderr,"\n seuil_ext=%f x0_ext=%f dimx_ext=%f y0_ext=%f dimy_ext=%f z0_ext=%f dimz_ext=%f",seuil_ext,x0_ext,dimx_ext,y0_ext,dimy_ext,z0_ext,dimz_ext);*/

	if(r <= seuil_ext && x >= x0_ext && x <= x0_ext + dimx_ext && y >= y0_ext && y <= y0_ext + dimy_ext && z >= z0_ext && z <= z0_ext + dimz_ext)
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

/* procedure de diffusion de proche en proche
 entree: fichier contenant le graphe avec tous les attributs, nb max de voisins, fonction de mise a jour du graphe apres diffusion
 a partir d'un noeud, delta t,nb d'iteration pour discretiser le temps.
 en sortie: nouveau fichier des attributs compl�mentaires */


void diffusiong(char *ficgraphe,int nvmax,void (*fonc1)(int,int*,int,graphe_s**),int nbpas,char *ficatt2)
{
	float at;
	int natt,nboules,i,j,nvois,*tavois,l;
	FILE *ptficatt2;
	graphe_s *graphe;

	ptficatt2 = fopen(ficatt2,"w");

	graphe = lecgraphe(ficgraphe);
	/* graphe contient le graphe des boules avec tous les attributs y compris les rayons et centres des boules */

	nboules = graphe->nel;
/*	fprintf(stderr,"\n nb de noeuds = %d",nboules);*/

	natt = graphe->ncar - 4;
/*	fprintf(stderr,"\n nb d'attributs complementaires = %d",natt);*/

	tavois = (int*)mycalloc(nvmax,sizeof(int));

	/* traitement des boules pour application de la fonction fonc1 */

	/* on itere nbit fois la diffusion */

	for(l=0;l<nbpas;l++)
	{
		fprintf(stderr,"\n iteration : %d",l);
		for(i=1;i<=nboules;i++)
		{
			if(i % 10000 == 0)fprintf(stderr,"\n i = %d",i);

			/* determination des voisins du noeud i */
			voisins(i,graphe,nvmax,&nvois,&tavois);
			/* les nvois voisins du noeud i sont ranges dans tavois */

			/* on met a jour le graphe apres avoir diffuse a partir du noeud i */
			fonc1(i,tavois,nvois,&graphe);
		}
		/* graphe->carac contient les nouveaux attributs des noeuds apres la diffusion */

	}
		/* ecriture de ces nouveaux attributs dans un fichier d'attributs */

	/* ecriture du nombre d'attributs */
	fprintf(ptficatt2,"%d",natt);

	for(i=1;i<=nboules;i++)
	{
		fprintf(ptficatt2,"\n");
		for(j=1;j<=natt;j++)
		{
			at = graphe->carac[j+4][i];
			fprintf(ptficatt2," %f ",at);
		}
	}

	fclose(ptficatt2);
	free(tavois);
}

/* fonction de mise a jour des caracteristiques pour diffusion des enzymes */

void fdifenz(int n,int *tavois,int nvois,graphe_s **graphe)
{
	int i,j;
	float cdif,deltat,d1,d2,v1,v2,sc,m1,m2,dd1,dd2,deltam,s1,s2,fracjour,taillev;

	/* l'unite de temps est le jour et l'unite de longueur le micro m, l'unite de masse est le micron gramme */

	fracjour = 5;
	/* fraction de jour pour la simulation */

	taillev = 4;
	/* taille des voxels en microns m */

	/* deltat est exprime en jours */
	deltat = 1.0/fracjour;

	/* cdif est exprime en jour -1 par micron m -1 par micron m -1 */
	cdif = 720;
	/* cdif s'exprime en 1/mXmXT et est egal a 720 pour des Micron m carr�s jour pour la DOM et pour les enzymes */

	v1 = (4.0/3.0)*M_PI*pow((*graphe)->carac[4][n],3)*pow(taillev,3);
	/* v1 est exprime en microns m cube */

	s1 = 4*M_PI*pow((*graphe)->carac[4][n],2)*pow(taillev,2);
	/* s1 est la surfece de contact exprimee en micron m carre */

	for(i=0;i<nvois;i++)
	{
		m1 = (*graphe)->carac[9][n];
		/* m1 est la masse d'enzymes exprimee en micron g */

		d1 = m1/v1;
		/* d1 est exprime en microns grammes par micron metre cube */

		/* v1 et d1 contiennent respectivement le volume du pore emetteur a traiter et sa densite en enzymes*/



		j = tavois[i];
		/* j est le numero du pore voisin recepteur */

		m2 = (*graphe)->carac[9][j];
		v2 = (4.0/3.0)*M_PI*pow((*graphe)->carac[4][j],3)*pow(taillev,3);
		d2 = m2/v2;

		s2 = 4*M_PI*pow((*graphe)->carac[4][j],2)*pow(taillev,2);
		/* v2 et d2 contiennent respectivement le volume du pore voisin a traiter et sa densit� en enzymes */

		/* si sa densite en enzymes est < a celle du pore n on fait eventuellement quelque chose */

		if(d2 < d1)
		{
			/* calcul de la surface de contact */
			sc = (v1>v2) ? s2:s1;

			/* calcul du gradient de masse maximum a transferer */
			deltam = cdif*sc*deltat*(d1-d2)*v1;

			/* on regarde si la masse a ajouter n'augmenterait pas trop la densite du pore j */
			dd1 = (m1 - deltam)/v1;
			dd2 = (m2 + deltam)/v2;
			/* si c'est le cas on donne juste ce qu'il faut pour egaler les deux densites sinon on garde le delta calcule*/
			if(dd2 > dd1)
			{
				deltam = (d1-d2)/(1/v1 + 1/v2);
			}
			(*graphe)->carac[9][j] += deltam;
			(*graphe)->carac[9][n] -= deltam;
		}

	}
}

/* fonction de mise a jour des caracteristiques pour diffusion de la DOM */

void fdidom(int n,int *tavois,int nvois,graphe_s **graphe)
{
	int i,j;
	float cdif,deltat,d1,d2,v1,v2,sc,m1,m2,dd1,dd2,deltam,s1,s2,fracjour,taillev;

	/* l'unite de temps est le jour et l'unite de longueur le micro m, l'unite de masse est le micron gramme */

	fracjour = 5;
	/* fraction de jour pour la simulation */

	taillev = 4;
	/* taille des voxels en microns m */

	/* deltat est exprime en jours */
	deltat = 1.0/fracjour;

	/* cdif est exprime en jour -1 par micron m -1 par micron m -1 */
	cdif = 720;
	/* cdif s'exprime en 1/mXmXT et est egal a 720 pour des Micron m carr�s jour pour la DOM et pour les enzymes */

	v1 = (4.0/3.0)*M_PI*pow((*graphe)->carac[4][n],3)*pow(taillev,3);
	/* v1 est exprime en microns m cube */

	s1 = 4*M_PI*pow((*graphe)->carac[4][n],2)*pow(taillev,2);
	/* s1 est la surfece de contact exprimee en micron m carre */

	for(i=0;i<nvois;i++)
	{
		m1 = (*graphe)->carac[6][n];
		/* m1 est la masse d'enzymes exprimee en micron g */

		d1 = m1/v1;
		/* d1 est exprime en microns grammes par micron metre cube */

		/* v1 et d1 contiennent respectivement le volume du pore emetteur a traiter et sa densite en enzymes*/



		j = tavois[i];
		/* j est le numero du pore voisin recepteur */

		m2 = (*graphe)->carac[6][j];
		v2 = (4.0/3.0)*M_PI*pow((*graphe)->carac[4][j],3)*pow(taillev,3);
		d2 = m2/v2;

		s2 = 4*M_PI*pow((*graphe)->carac[4][j],2)*pow(taillev,2);
		/* v2 et d2 contiennent respectivement le volume du pore voisin a traiter et sa densit� en enzymes */

		/* si sa densite en enzymes est < a celle du pore n on fait eventuellement quelque chose */

		if(d2 < d1)
		{
			/* calcul de la surface de contact */
			sc = (v1>v2) ? s2:s1;

			/* calcul du gradient de masse maximum a transferer */
			deltam = cdif*sc*deltat*(d1-d2)*v1;

			/* on regarde si la masse a ajouter n'augmenterait pas trop la densite du pore j */
			dd1 = (m1 - deltam)/v1;
			dd2 = (m2 + deltam)/v2;
			/* si c'est le cas on donne juste ce qu'il faut pour egaler les deux densites sinon on garde le delta calcule*/
			if(dd2 > dd1)
			{
				deltam = (d1-d2)/(1/v1 + 1/v2);
			}
			(*graphe)->carac[6][j] += deltam;
			(*graphe)->carac[6][n] -= deltam;
		}

	}
}

/* fonction de mise a jour des caracteristiques pour diffusion de la DOM */

void fdidom_sl2(int n,int *tavois,int nvois,graphe_s **graphe)
{
	int i,j;
	float cdif,deltat,d1,d2,v1,v2,sc,m1,m2,dd1,dd2,deltam,s1,s2,fracjour,taillev,r1,r2;

	/* l'unite de temps est le jour et l'unite de longueur le micron m, l'unite de masse est le micron gramme */

	fracjour = fracjour_ext;
	/* fraction de jour pour la simulation */

	taillev = taillev_ext;
	/* taille des voxels en microns m */

	/* deltat est exprime en jours */
	deltat = 1.0/fracjour;
	deltat = deltat/(float)(deltasim_ext);

	r1 = ((*graphe)->carac[4][n])*taillev;

	/* cdif est exprime en jour -1 par micron m -1 par micron m -1 */
	cdif = cdif_ext;
	/* cdif s'exprime en 1/mXmXT et est egal a 720 pour des Micron m carr�s jour pour la DOM et pour les enzymes */

	v1 = (4.0/3.0)*M_PI*pow((*graphe)->carac[4][n],3)*pow(taillev,3);
	/* v1 est exprime en microns m cube */

	s1 = 4*M_PI*pow((*graphe)->carac[4][n],2)*pow(taillev,2);
	/* s1 est la surfece de contact exprimee en micron m carre */

	for(i=0;i<nvois;i++)
	{
		m1 = (*graphe)->carac[6][n];
		/* m1 est la masse d'enzymes exprimee en micron g */

		d1 = m1/v1;

		/* d1 est exprime en microns grammes par micron metre cube */

		/* v1 et d1 contiennent respectivement le volume du pore emetteur a traiter et sa densite en enzymes*/



		j = tavois[i];
		/* j est le numero du pore voisin recepteur */
		r2 = ((*graphe)->carac[4][j])*taillev;
		m2 = (*graphe)->carac[6][j];
		v2 = (4.0/3.0)*M_PI*pow((*graphe)->carac[4][j],3)*pow(taillev,3);
		d2 = m2/v2;
		s2 = 4*M_PI*pow((*graphe)->carac[4][j],2)*pow(taillev,2);
		/* v2 et d2 contiennent respectivement le volume du pore voisin a traiter et sa densit� en enzymes */

		/* si sa densite en enzymes est < a celle du pore n on fait eventuellement quelque chose */
		if(d2 < d1)
		{
			/* calcul de la surface de contact */
			sc = (v1>v2) ? s2:s1;

			/* calcul du gradient de masse maximum a transferer */

		/*	if((*graphe)->carac[4][n] < 1.2)fprintf(stderr,"\n sc*deltat*v1 = %f sc = %f deltat = %f v1 = %f",sc*deltat*v1,sc,deltat,v1);*/

		/* ANCIEN CALCUL DE DELTAM	deltam = cdif*sc*deltat*(d1-d2)*v1;*/

		/* NOUVEAU CALCUL DE DELTAM */

			deltam = cdif*sc*deltat*((d1-d2)/(r1+r2));

			/* on regarde si la masse a ajouter n'augmenterait pas trop la densite du pore j */
			dd1 = (m1 - deltam)/v1;
			dd2 = (m2 + deltam)/v2;
			/* si c'est le cas on donne juste ce qu'il faut pour egaler les deux densites sinon on garde le delta calcule*/
			if(dd2 > dd1)
			{
/*				trace1_ext++;*/
				deltam = (d1-d2)/(1/v1 + 1/v2);
			}
			else
			{
/*				trace2_ext++;*/
			}
			(*graphe)->carac[6][j] += deltam;
			(*graphe)->carac[6][n] -= deltam;
		}

	}
}

/* diffusion avec la methode "simultanee de l'attribut compl�mentaire num�ro numatt avec le pas de temps deltat
 * a la sortie le graphe est mis � jour apr�s la diffusion si opt = 1 rectification sequentielle sinon opt = 0 */

void diffusiong2(graphe_s **graf,int nvmax,void (*fonc1)(int,int*,int,graphe_s**),int nbpas)
{
	int natt,nboules,i,nvois,*tavois,l;
	graphe_s *graphe;

	graphe = *graf;

	/* graphe contient le graphe des boules avec tous les attributs y compris les rayons et centres des boules */

	nboules = graphe->nel;

	natt = graphe->ncar - 4;

	tavois = (int*)mycalloc(nvmax,sizeof(int));

	/* traitement des boules pour application de la fonction fonc1 */

	/* on itere nbit fois la diffusion */

	for(l=0;l<nbpas;l++)
	{
		for(i=1;i<=nboules;i++)
		{

			/* determination des voisins du noeud i */
			voisins(i,graphe,nvmax,&nvois,&tavois);
			/* les nvois voisins du noeud i sont ranges dans tavois */

			/* on met a jour le graphe apres avoir diffuse a partir du noeud i */
			fonc1(i,tavois,nvois,&graphe);
		}
		/* graphe->carac contient les nouveaux attributs des noeuds apres la diffusion */

	}
	free(tavois);
}

void diffusiong3(graphe_s **graf,int numatt,float deltat,float cdif,float taillev,int *nbvalinv,int *nbvalneg,float *sommeneg,int opt,int ncor)
{
	int natt,nboules,i,narcs,n1,n2,nbneg,ninvers,l;
	graphe_s *graphe;
	float *att,val1,val2,deltam,s1,v2,c1,c2,m1,m2,v1,r;
	float massetot,c3,c4,ra,c,rap,vvoxel,surf;
	double massetotale,vtot,v;
	double deltaneg;


	graphe = *graf;

	/* graphe contient le graphe des boules avec tous les attributs y compris les rayons et centres des boules */

	massetot = 0;

	vvoxel = pow(taillev,3);

	nboules = graphe->nel;

	natt = graphe->ncar - 4;

	narcs = graphe->nco;

	/* si opt = -1 diffusion instantan�e */

	if(opt == -1)
	{
		/* calcul de la masse totale et du volume total */
		vtot = 0;
		massetotale = 0;
		for(i=1;i<=nboules;i++)
		{
			massetotale += (double)(graphe->carac[numatt + 4][i]);
			vtot += (double)(4.0/3.0)*M_PI*(double)(pow((graphe)->carac[4][i],3))*(double)(vvoxel);
		}
		/* massetotale est la masse totale et vtot le volume total */
		for(i=1;i<=nboules;i++)
		{
			v = (double)(4.0/3.0)*M_PI*(double)(pow((graphe)->carac[4][i],3))*(double)(vvoxel);
			graphe->carac[numatt + 4][i] = (float)((v*massetotale)/vtot);
		}
		return;
	}

	/* att contient les valeurs finales de l'attribut complementaire numero "numatt" apres diffusion */
	att = (float*)mycalloc(nboules + 1,sizeof(float));

	/* remplissage des valeurs initiales pour att */
	for(i=1;i<=nboules;i++)
	{
		att[i] = graphe->carac[numatt + 4][i];
		massetot += att[i];
	}

	/* traitement de l'ensemble des arcs */



	surf = pow(taillev,2);

	for(i=1;i<=narcs;i++)
	{
		n1 = graphe->couple[1][i];
		n2 = graphe->couple[2][i];

		val1 = graphe->carac[numatt + 4][n1];
		val2 = graphe->carac[numatt + 4][n2];

		/* mise � jour de att[n1] et de att[n2] */

		/* calcul des concentrations dans les deux boules */

		v1 = (4.0/3.0)*M_PI*pow((graphe)->carac[4][n1],3)*vvoxel;
		v2 = (4.0/3.0)*M_PI*pow((graphe)->carac[4][n2],3)*vvoxel;
		m1 = graphe->carac[numatt + 4][n1];
		m2 = graphe->carac[numatt + 4][n2];
		c1 = m1/v1;
		c2 = m2/v2;


		/* calcul de la surface de contact */
		if( graphe->carac[4][n1] > graphe->carac[4][n2])
			{
				r= graphe->carac[4][n2];
			}
		else
			{
				r = graphe->carac[4][n1];
			};

		s1 = 4*M_PI*pow(r,2)*surf;

		/* la surface de contact exprim�e en micron metre est s1 */
		/* l'intervalle de temps exprime en jours est deltat */

		deltam = -cdif*deltat*s1*(c1-c2)/(graphe->carac[4][n1] + graphe->carac[4][n2]);

		/* METTRE LES GARDEFOUS AU MOMENT DU BILAN ET NON AU FUR ET A MESURE */

		att[n1] += deltam;
		att[n2] += -deltam;

	}

	/* le tableau att contient les nouvelles valeurs pour la masse de l'attribut diffus� mais cette masse
	 peut etre negative
	 */

	/* rectification des nouvelles valeurs dans le cas ou certaines sont negatives */

	nbneg = 0;
	deltaneg = 0;
	massetotale = 0;
	for(i=1;i<=nboules;i++)
	{
		massetotale += att[i];
		if(att[i] < 0)
		{
			nbneg++;
			deltaneg += -att[i];
			att[i] = 0;
		}
	}

	/* rectification des valeurs on enleve deltaneg */

	if(nbneg != 0)
	{
		rap = massetotale/(massetotale + deltaneg);
/*		fprintf(stderr,"\n rap = %f deltaneg = %f massetot = %f",rap,deltaneg,massetot);*/
		for(i=1;i<=nboules;i++)
		{
			if(att[i] != 0)
			{
				/* on multiplie chaque valeur non nulle par (deltaneg/(massetot + deltaneg) */

				att[i] = (float)((double)att[i] * rap);
			}
		}
	}


	/* calcul du nombre d'inversions et rectification sequentielle eventuelle */

	/* ninvers est le nombre d'inversion de l'order des valeurs de la concentration si le pas de temps
		 est suffisament petit ce nombre est nul
		 */

	/* on fait ncor iterations de la correction des inversions */
	/* si ncor = 1 et opt = 1 on corrige si ncor = 1 et opt = 0 on ne corrige pas */
	/* si ncor = 0 on ne fait rien */


	for(l = 0;l < ncor;l++)
	{
		ninvers = 0;
		for(i=1;i<=narcs;i++)
		{
			n1 = graphe->couple[1][i];
			n2 = graphe->couple[2][i];

			/* mise � jour de att[n1] et de att[n2] */

			/* calcul des concentrations dans les deux boules */

			v1 = (4.0/3.0)*M_PI*pow((graphe)->carac[4][n1],3)*vvoxel;
			v2 = (4.0/3.0)*M_PI*pow((graphe)->carac[4][n2],3)*vvoxel;
			m1 = graphe->carac[numatt + 4][n1];
			m2 = graphe->carac[numatt + 4][n2];
			c1 = m1/v1;
			c2 = m2/v2;

			c3 = (att[n1])/v1;
			c4 = (att[n2])/v2;

			/* si la masse transferee fait changer l'ordre des densites on transfere juste ce qu'il faut pour les egaler */

			if((c1-c2)*(c3-c4) < 0)
			{
				if(c3 < c4)
				{
					ra = c3/c4;
				}
				else
				{
					ra = c4/c3;
				}
				if(ra < 0.99)
				{
					ninvers++;
					/* on met les valeurs des densit�s � la m�me valeur si opt = 1 */
					if(opt == 1)
					{
						c = (att[n1] + att[n2])/(v1 + v2);
						att[n1] = v1*c;
						att[n2] = v2*c;
					}
				}
			}
		}
		if(ninvers == 0)break;
	}
	if(ncor == 0)ninvers = -1;
	/* ecriture des nouvelles valeurs dans le graphe */
/*	massetotf = 0;*/
	for(i=1;i<=nboules;i++)
	{
		graphe->carac[numatt + 4][i] = att[i] ;
/*		massetotf += att[i];*/
	}
	/* fprintf(stderr,"\n rapport des bilans de masse = %f",massetot/massetotf);*/
	*nbvalneg =nbneg;
	*sommeneg = deltaneg;
	*nbvalinv = ninvers;

	free(att);
}

void diffusiong4(graphe_s **graf,int numatt,int nbiter,float taillev)
{
	int natt,nboules,i,narcs,n1,n2,l;
	graphe_s *graphe;
	float v2,m1,m2,v1;
	float vvoxel;

	graphe = *graf;

	/* graphe contient le graphe des boules avec tous les attributs y compris les rayons et centres des boules */

	vvoxel = pow(taillev,3);

	nboules = graphe->nel;

	natt = graphe->ncar - 4;

	narcs = graphe->nco;

	for(l=0;l<nbiter;l++)
	{

		for(i=1;i<=narcs;i++)
		{
			n1 = graphe->couple[1][i];
			n2 = graphe->couple[2][i];

			/* mise � jour de att[n1] et de att[n2] */

			/* calcul des concentrations dans les deux boules */

				v1 = (4.0/3.0)*M_PI*pow((graphe)->carac[4][n1],3)*vvoxel;
				v2 = (4.0/3.0)*M_PI*pow((graphe)->carac[4][n2],3)*vvoxel;
				m1 = graphe->carac[numatt + 4][n1];
				m2 = graphe->carac[numatt + 4][n2];
				graphe->carac[numatt + 4][n1] = (v1/(v1+v2))*(m1+m2);
				graphe->carac[numatt + 4][n2] = (v2/(v1+v2))*(m1+m2);
		}

	}
}


/* void voisins(int n,graphe_s *graphe1,int nvmax,int *nvois,int **vois) */

/* cette fonction determine les composantes connexes d'un graphe */
/* entree : un pointeur sur le graphe */
/* sortie : un fichier contenant le nombre de composantes connexes, chaque composante connexe : nb de boules, volume total des boules,
   pourcentage du volume total des boules, liste des boules (centre, rayon) ainsi que le nombre de composantes connexes */

void compconnexes(graphe_s *graphe,char *fcompco,char *fcompcor,int *nbcompco)
{
	int *etiq,nboules,i,natraiter,*atraiter,*compco,ncompco,k,nvmax,nvois,*vois,p,l,numco, nbouleau;
	float vco,vtotal,somr;
	double vmoyen;
	FILE *ptcompco,*ptcompcor;

	nboules = graphe->nel;

	ptcompco = fopen(fcompco,"w");

	ptcompcor = fopen(fcompcor,"w");

	fprintf(stderr,"\n nb de boules = %d",nboules);

	etiq = (int*)mycalloc(nboules+1,sizeof(int));
	atraiter = (int*)mycalloc(nboules,sizeof(int));
	compco = (int*)mycalloc(nboules,sizeof(int));

	vtotal = 0;
	nvmax = 10000;

	vois = (int*)mycalloc(nvmax,sizeof(int));

	/* calcul du volume total en pixels cube */
	somr = 0;
	for(i=1;i<=nboules;i++)
	{
		vtotal += (4.0/3.0)*M_PI*(graphe->carac[4][i])*(graphe->carac[4][i])*(graphe->carac[4][i]);
		somr += graphe->carac[4][i];
	}
	somr = somr/nboules;


	fprintf(ptcompco,"\n volume total en voxels = %f",vtotal);
	fprintf(ptcompco,"\n");
	fprintf(ptcompco,"\n");

	numco = 0;
	/* numco contient le nombre de composantes connexes */

	/* initialisation de etiq a -1 ; les boules sont numerotees a partir de 1*/
	for(i=0;i<nboules+1;i++)etiq[i]=-1;
	vmoyen = 0;
	for(i=1;i<=nboules;i++)
	{
		/* determination de la composante connexe de la boule i si elle n'est pas deja incluse dans une composante connexe */
		if(etiq[i] < 0)
		{
			numco++;
			/* on determine la composante connexe numco */

			/* cas ou le point n'est pas deja inclu dans une composante connexe */
			/* initialisation a 0 du volume de la composante connexe */
			vco = 0;
			/* determination de la composante connexe du noeud i et stokage dans le fichier */
			/* initialisation a 0 du nombre de noeuds a traiter et du nombre de noeud de la composante connexe */
			natraiter = 0;
			ncompco = 0;
			atraiter[natraiter] = i;
			natraiter++;
			etiq[i] = numco;

			while(natraiter > 0)
			{
				natraiter--;
				k = atraiter[natraiter];
				/* on range k dans le tableau contenant la composante connexe et on met dans le tableau atraiter tous les voisins de k
				 qui ne sont pas deja dans la composante connexe courante ou dans une autre composante connexe (ce dernier cas etant
				 normalement impossible)
				 */
				compco[ncompco] = k;
				/* fprintf(stderr,"\n noeud numero %d inclu",k);*/
				ncompco++;
				/* fprintf(stderr,"\n k = %d",k);*/
				vco += (4.0/3.0)*M_PI*(graphe->carac[4][k])*(graphe->carac[4][k])*(graphe->carac[4][k]);

				voisins(k,graphe,nvmax,&nvois,&vois);
				/* fprintf(stderr,"\n nb de voisins = %d",nvois);*/

				/* on inclut dans le tableau atraiter tous les voisins qui n'ont jamais ete dans le tableau */
				for(p=0;p<nvois;p++)
				{
					l = vois[p];
					/* fprintf(stderr,"\n l = %d",l);*/
					if(etiq[l] < 0)
					{
						/* cas ou le point n'a pas ete traite alors on l'inclut dans la liste a traiter */
						atraiter[natraiter] = l;
						/* fprintf(stderr,"\n inclusion de %d dans la liste a traiter",l);*/
						natraiter++;
						etiq[l] = numco;
					}
				}

			}
			/* a la fin de la boucle natraiter est egal a 0 et le tableau compco contient la composante connexe de la boule numero i */

			/* la composante connexe comprend ncompco noeuds et a un volume total de vco en pixels cube */

			/* stockage des informations dans le fichier de sortie */
			fprintf(ptcompco,"\n");
			fprintf(ptcompco,"\n %d",ncompco);
			fprintf(ptcompco,"\n %f",vco);
			vmoyen += vco;
			fprintf(ptcompco,"\n %f",(vco/vtotal)*100);
			fprintf(ptcompco,"\n");

			fprintf(ptcompcor,"\n");
			fprintf(ptcompcor,"\n composante connexe numero : %d",numco);
			fprintf(ptcompcor,"\n nombre de boules = %d",ncompco);
			fprintf(ptcompcor,"\n volume total des boules = %f",vco);
			fprintf(ptcompcor,"\n pourcentage du volume total de l'ensemble des boules = %f",(vco/vtotal)*100);
			fprintf(ptcompcor,"\n");

			for(p=0;p<ncompco;p++)
			{
				k = compco[p];
				fprintf(ptcompco,"\n %f %f %f %f",graphe->carac[1][k],graphe->carac[2][k],graphe->carac[3][k],graphe->carac[4][k]);
			}
		}

	}
	fprintf(ptcompcor,"\n nombre total de composantes connexes = %d",numco);
	fprintf(ptcompcor,"\n volume moyen des composantes connexes en voxels = %f",(float)(vmoyen/numco));
	fprintf(ptcompcor,"\n rayon moyen des boules = %f",somr);
	*nbcompco = numco;
	free(vois);
	fclose(ptcompco);
	fclose(ptcompcor);
}


/* cette fonction prend un graphe en entree et retourne le bilan total des masses */

double bilanmasses(graphe_s *graphe)
{
	double massetot;
	int nboules,i,j,natt;

	massetot = 0;
	natt = graphe->ncar - 4;
	nboules = graphe->nel;

	for(i=1;i<=nboules;i++)
	{
		for(j=1;j<=natt;j++)
		{
			massetot += graphe->carac[4+j][i];
		}
	}
	return(massetot);
}

/* bilan detaill� des masses de matiere organique et de micro organismes */
void bilanmatmic(graphe_s *graphe)
{
	double massemic,massemat;
	int nboules,i,j,natt;

	massemic = 0;
	massemat = 0;
	natt = graphe->ncar - 4;
	nboules = graphe->nel;

	for(i=1;i<=nboules;i++)
	{
		massemic += graphe->carac[5][i];
		massemat += graphe->carac[6][i];
	}
	if(graphe->carac[6][i] != 0 && graphe->carac[5][i] == 0)fprintf(stderr,"\n ALERTE i = %d",i);
	fprintf(stderr,"\n masse totale de micro organismes = %lf ; masse totale de mati�re organique = %lf",massemic,massemat);
}


/* routines de drainage par vidage des boules */

/* cette proc�dure prend en entr�e un graphe de boules, ue distance seuil et un cadre d'image (dimx,dimy,dimz) ; elle
 donne en  sortie un tableau de boules formant le bord de l'image le premier �l�ment du tableau est le nombre d'�l�ments du tableau */

int *calbords(graphe_s *graphe,float dseuil,int dimx,int dimy,int dimz)
{
	int *trav,nboules,nbord,i,*trav2;
	float x,y,z,r,d,d1,d2,d3,d4,d5,d6;

	nboules = graphe->nel;
	nbord = 0;
	trav = (int*)mycalloc(nboules,sizeof(int));

	for(i=1;i<=nboules;i++)
	{
		x = graphe->carac[1][i];
		y = graphe->carac[2][i];
		z = graphe->carac[3][i];
		r = graphe->carac[4][i];
		d1 = dimx - (x+r);
		d2 = x-r;
		d3 = dimy - (y+r);
		d4 = y-r;
		d5 = dimz - (z+r);
		d6 = z - r;
		d = d1;
		if(d > d2)d=d2;
		if(d > d3)d=d3;
		if(d > d4)d=d4;
		if(d > d5)d=d5;
		if(d > d6)d=d6;
		if(d <= dseuil)
		{
			trav[nbord] = i;
			nbord++;
		}
	}
	trav2 = (int*)mycalloc(nbord+1,sizeof(int));
	trav2[0] = nbord;
	for(i=1;i<=nbord;i++)
	{
		trav2[i] = trav[i-1];
	}
	free(trav);
	fprintf(stderr,"\n nombre de boules du bord = %d \n",nbord);
	return(trav2);
}

/* cette proc�dure prend en entr�e un graphe de boules, un tableau contenant un l'ensemble des boules trait�es (tab(0)(i) = 0 si non trait� et non couronne,
 1 si couronne, 2 si trait� et pas couronne ,
 tab(1)(i) � 0 si air et 1 si eau , un seuil de drainage pour le rayon des boules et met � jour le tableau des boules trait�es en ajoutant le traitement des
 boules voisines le premier �l�ment du tableau des boules trait�es contient le nombre de boules du tableau, elle rend aussi le
 nombre de boules vid�es */

void draintrait(graphe_s *graphe,int ***trait,float seuil,int *nvid)
{
	int **trav,i,nvmax,nvois,*vois,k,n,nv,nboules;

	trav = *trait;
	nboules = graphe->nel;
	nvmax = nboules/100;
	nv = 0;

	vois = (int*)mycalloc(nvmax,sizeof(int));

	for(i=1;i<=nboules;i++)
	{

		/* on ne traite que les boules non trait�es de de la couronne */


		if(trav[0][i] == 1)
		{
			/* traitement de la boule de la couronne en regardant les voisins */

			voisins(i,graphe,nvmax,&nvois,&vois);

			/* vois contient les nvois voisins de la boule i dans le graphe */
			for(k=0;k<nvois;k++)
			{
				n = vois[k];
				if(trav[1][i] == 0)
				{
					if(graphe->carac[4][n] > seuil && trav[1][n] == 1)
					{
							/* cas d'une boule d'eau � vider */
							trav[1][n] = 0;
							nv++;
					}
				}
				if(trav[1][n] == 0)
				{
					if(graphe->carac[4][i] > seuil && trav[1][i] == 1)
					{
						/* cas d'une boule d'eau � vider */
						trav[1][i] = 0;
						nv++;
					}
				}

				trav[0][n] = -1;
			}
			trav[0][i] = 2;
		}
	}
	*nvid = nv;

	/* mise � jour de la nouvelle couronne */
	for(i=1;i<=nboules;i++)
	{
		if(trav[0][i] == -1 )trav[0][i] = 1;
	}
	free(vois);
}

/* drainage des boulee � partir du graphe des boules it�ration, retourne le tableau trait */
int **drainit(graphe_s *graphe,float seuil,float dseuil,int dimx,int dimy,int dimz)
{
	int **trait,*bord,nbord,i,nvid,nboules,nvtot;

	nboules = graphe->nel;
	trait = allotab2i(2,nboules+1);
	nvtot = 0;

	/* initialisation de trait on met � 1 les boules du bord et � 0 les autres */
	bord = calbords(graphe,dseuil,dimx,dimy,dimz);
	/* bord contient les num�ros des boules du bord dans le graphe */
	nbord = bord[0];
	/* le nombre de boules du bord est nbord */

	/* on initialise trait avec les boules du bord en vidant celles dont le rayon est plus grand que seuil */

	/* initialisation de trait */
	for(i=1;i<=nboules;i++)
	{
		trait[0][i] = 0;
		trait[1][i] = 1;
	}

	/* on met � 1 les boules du bord et on vide celles dont le rayon est plus grand que seuil */

	for(i=1;i<=nbord;i++)
	{
		trait[0][bord[i]] = 1;
		if(graphe->carac[4][bord[i]] <= seuil)
		{
			trait[1][bord[i]] = 1;
		}
		else
		{
			trait[1][bord[i]] = 0;
		}
	}

	/* le tableau trait a �t� initialis� et on lance le processus de vidage it�ratif */

	draintrait(graphe,&trait,seuil,&nvid);

	while(nvid != 0)
	{


		draintrait(graphe,&trait,seuil,&nvid);

		nvtot += nvid;
		i++;
	}

	/* le tableau trait contient le r�sultat du drainage */


	fprintf(stderr,"\n nb de boules total vid�es = %d",nvtot);

	nvtot = 0;
	/* on compte le nombre de boules d'eau dont le rayon est plus grand que le seuil */
	for(i=1;i<=nboules;i++)
	{
		/* traitement de la boule i */
		if(trait[1][i] == 1 && graphe->carac[4][i] > seuil)nvtot++;
		if(trait[1][i] == 0 && graphe->carac[4][i] <= seuil)
			{
				fprintf(stderr,"\n erreur dans drainage 1");
				exit(15);
			}
	}
	fprintf(stderr,"\n nombre de boules d'eau de rayon plus grand que le seuil = %d",nvtot);

	return(trait);
}

/* cette procedure cr�e un fichier de boules drain�es en fonction du tableau trait et du graphe */
void ficdrain(graphe_s *graphe,int **trait,char *fic,float seuil)
{
	int i,nboules,nbouleau,nvtot;
	FILE *ptfic;



	nboules = graphe->nel;

	ptfic = fopen(fic,"w");

	nbouleau = 0;
	nvtot = 0;
	/* on compte le nombre de boules d'eau */
	for(i=1;i<=nboules;i++)
	{
		/* traitement de la boule i */
		if(trait[1][i] == 1)
		{
			nbouleau++;
			if(graphe->carac[4][i] > seuil)nvtot++;
		}

	}

	fprintf(stderr,"\n nombre de boules d'eau = %d nb de boules d'eau avec rayon > seuil = %d",nbouleau,nvtot);

	/* on �crit le nombre de boules d'eau en tete du fichier */
	fprintf(ptfic,"%d \n",nbouleau);


	for(i=1;i<=nboules;i++)
	{
		/* traitement de la boule i */
		if(trait[1][i] == 1)
		{
			/* cas d'une boule remplie d'eau on l'ecrit dans le fichier pour la simulation biologique */
			fprintf(ptfic,"%f %f %f %f %f %f %f %f \n",(float)graphe->carac[1][i],(float)graphe->carac[2][i],(float)graphe->carac[3][i],(float)graphe->carac[4][i],0.0,0.0,0.0,0.0);
		}


	}
	fclose(ptfic);

}

/* v�rification drainage style Val�rie retourne 1 si OK et le nombre d'erreur sinon */

void verifdrain(graphe_s *graphe,int **trait,float seuil,int *res)
{
	int i,narcs,k,l,nban,nboules;

	narcs = graphe->nco;

	nban = 0;

	/* on parcourt les arcs pour voir si une boule de rayon > seuil entour� de boules d'eau n'a pas �t� vid�e ou si toutes les
	 boules de rayon > seuil voisines d'une boule d'air ont bien �t� vid�es */
	fprintf(stderr,"\n");
	for(i=1;i<=narcs;i++)
	{
		k = graphe->couple[1][i];
		l = graphe->couple[2][i];


		if(trait[1][k]==1 && graphe->carac[4][k] > seuil)
		{
			if(trait[1][l]==0)nban++;
		}
		if(trait[1][l]==1 && graphe->carac[4][l] > seuil)
		{
			if(trait[1][k]==0)nban++;
		}
	}
	/* on verifie que les boules de rayon plus petit que le seuil sont bien pleines d'eau */
	nboules = graphe->nbnoeuds;
	if(nban != 0)fprintf(stderr,"\n probleme 1");
	for(i=1;i<=nboules;i++)
	{
		if(trait[1][i]==0 && graphe->carac[4][i] <= seuil)nban++;
	}
	if(nban != 0)fprintf(stderr,"\n probleme 2");
	*res = nban;
}

/* drainage des boulee � partir du graphe des boules it�ration, retourne le tableau trait methode par iteration sur les noeuds du graphe 
   A la fin de la procedure trait[1][i] est a 0 si la boule a ete vide (air) et a 1 sinon (eau)
*/
int **drainit2(graphe_s *graphe,float seuil,float dseuil,int dimx,int dimy,int dimz)
{
	int **trait,*bord,nbord,i,nvid,nboules,nvtot,n,ns;

	nboules = graphe->nel;
	trait = allotab2i(2,nboules+1);

	/* initialisation de trait on met a 1 les boules du bord et a 0 les autres */
	bord = calbords(graphe,dseuil,dimx,dimy,dimz);
	/* bord contient les numeros des boules du bord dans le graphe */
	nbord = bord[0];
	/* le nombre de boules du bord est nbord */

	fprintf(stderr,"\n nombre de boules du bord = %d",nbord);

	/* on initialise trait avec les boules du bord en vidant celles dont le rayon est plus grand que seuil */

	/* initialisation de trait */
	for(i=1;i<=nboules;i++)
	{
		trait[0][i] = 0;
		trait[1][i] = 1;
	}

	/* on met a 1 les boules du bord et on vide celles dont le rayon est plus grand que seuil */

	n = 0; 
	ns = 0; 

	for(i=1;i<=nbord;i++)
	{
		trait[0][bord[i]] = 1;
		if(graphe->carac[4][bord[i]] <= seuil)
		{
			trait[1][bord[i]] = 1;
		}
		else
		{
			trait[1][bord[i]] = 0;
			n++; 
			if(graphe->liste[bord[i]] != 0)ns++; 
		}
	}

	fprintf(stderr,"\n nombre des boules du bord videes = %d",n); 

	fprintf(stderr,"\n nombre des boules du bord videes avec des voisins = %d",ns); 

	fprintf(stderr,"\n fin des pretraitements de drainit2");

	/* le tableau trait a ete initialise et on lance le processus de vidage par parcours iteratif des arcs */


	draintrait2(graphe,&trait,seuil,&nvid);

	nvtot = nvid; 

	while(nvid != 0)
	{

		draintrait2(graphe,&trait,seuil,&nvid);

		nvtot += nvid;
	}

	/* le tableau trait contient le resultat du drainage */


	fprintf(stderr,"\n nb de boules total en dehors du bord videes = %d",nvtot);

	nvtot = 0;
	/* on compte le nombre de boules d'eau dont le rayon est plus grand que le seuil */
	for(i=1;i<=nboules;i++)
	{
		/* traitement de la boule i */
		if(trait[1][i] == 1 && graphe->carac[4][i] > seuil)nvtot++;
		if(trait[1][i] == 0 && graphe->carac[4][i] <= seuil)
		{
			fprintf(stderr,"\n erreur dans drainage 1");
			exit(15);
		}
	}
	fprintf(stderr,"\n nombre de boules d'eau de rayon plus grand que le seuil = %d",nvtot);

	return(trait);
}

/* drainage par parcours des arcs du graphe en mettant trait � jour */

void draintrait2(graphe_s *graphe,int ***trait,float seuil,int *nvid)
{
	int **trav,i,narcs,nv,k,l,n;


	trav = *trait;

	narcs = graphe->nco;

	nv = 0;

	/* on parcourt les arcs  et on vide toute boule dont le rayon est > seuil voisine d'une boule d'air */


	for(i=1;i<=narcs;i++)
	{
		
		k = graphe->couple[1][i];
		l = graphe->couple[2][i];

		if(trav[1][k]==1 && graphe->carac[4][k] > seuil)
		{
			if(trav[1][l]==0)
			{
				trav[1][k]=0;
				nv++;
			}
		}
		if(trav[1][l]==1 && graphe->carac[4][l] > seuil)
		{
			if(trav[1][k]==0)
			{
				trav[1][l]=0;
				nv++;
			}
		}
	}
	*nvid = nv;
}

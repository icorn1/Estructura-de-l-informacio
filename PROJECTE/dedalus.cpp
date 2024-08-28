#include "dedalus.hpp"
using namespace std;

/*
Pràctica d'ESIN Q1 21-22. Ixent Cornella Vilana, Eric Gonzalez Duro
****** MODUL "dedalus": ******

OBJECTIUS DEL MODUL:
	Dèdal fou l'arquitecte que va dissenyar el laberint al mite del laberint del Minotaure. En aquesta
	mòdul, es defineix el namespace dedalus que inclou la funció construir. Aquesta funció rep 
	un laberint i excava el laberint fins obtenir un laberint perfecte, és a dir, un laberint que 
	no té àrees inaccessibles ni tampoc cicles, imitant així un laberint creat pel gran Dèdal.
	
IMPLEMENTACIÓ ESCOLLIDA:
	Hem observat que per a construir un laberint perfecte, ens fa falta (entre d'altres coses) aleatorietat. Això ho obtenim 
	amb un útil de la classe util::, util::Random. Ara bé, s'ens demana de crear un laberint sense àrees inaccesibles ni cicles.
	Això, en termes de codificació, vol dir que, si fem l'implementació amb partició (que és el més raonable*), els
	grups de la partició al final del procés han de ser 1 (que equival a dir que no hi han àares inaccesibles, cada
	posició es pot accedir desde qualsevol altra), i també no pot ser que a l'hora d'unir grups, les dos posicions a 
	unir siguin del mateix grup, ja que estariem creant un cicle. Això no és díficil d'implementar gràcies a la classe
	partició que hem creat anteriorment, tant sols cal comprovar que les cambres a unir no estiguin al mateix grup (contem
	amb la funció partició::mateix_grup() per fer-ho) i comprovar els grups de la partició (contem amb partició::size() per
	saber els grups d'aquesta).
	
	*Creiem que fer dedalus amb partició és el més assenyat no només perque ho fica a l'enunciat del projecte, sinó
	perquè l'implementació es simplifica moltissim amb aquesta classe.
*/


void dedalus::construir(laberint & M) throw(error){
//PRE: 
//POST: Excava el laberint M, deixant-lo sense cicles i amb cap area inaccesible (fent-lo perfecte). Tira un error si ja estava excavat d'alguna manera qualsevol.
//COST: El cost de construir és lineal, ja que afegim nfils*ncols elements a la particio, només amb això ja tenim cost lineal. D'altra banda, es crida unir i mateix_grup
//	a la funció, per tant, revisant el cost d'aquest, podem veure que és constant en els dos. Llavors, no es modifica el cost en la nostra funció. Cal remarcar que,
//	també fem part.size()-1 iteracions al bucle que hi ha a la funció, llavors el cost augmentaria a, minim, 2nfils*ncols. Θ(n).

	nat nfils = M.num_files();
	nat ncols = M.num_columnes();
	cambra tancada(0,0,0,0); // creem una cambra completament tancada per fer comparacions.
	posicio cam, adj; 	//representen la posicio de la cambra en la que iterem, i la adjacent que obtindrem.
	
	util::Random R; //creem un generador de nombres aleatoris
	
	particio<posicio> part(nfils*ncols);
	
	//Ara afegirem a la particio de posicions que hem creat cada una de les cambres de laberint,
	//a la vegada que comprovem que aquest no estigui ja excavat, cosa que fem amb una comparació
	//d'una cambra tancada amb la cambra de cada posició, que ha de ser inicialment tancada.
	for(nat i = 1; i <= nfils; ++i){
		for(nat j = 1; j <= ncols; ++j){
			posicio pos = make_pair(i,j);
			if(tancada != M(pos)){
				throw error(EstaExcavat);
			}
			else{
				part.afegir(pos);
			}
			
		}
	}
	
	//Hem de fer que no quedi cap cambra que no estigui connectada, que es equivalent a que només hi hagi un grup a la partició:
	while(part.size() > 1){
	
		nat i = R(1,nfils);	//R ens retornara un enter entre 1 i nfils, ideal per obtenir una fila i columna aleatoria.
		nat j = R(1,ncols);
		cam = make_pair(i,j);		
		
		paret p(R(0,3)); //Ara obtenim una paret entre 0[nord] i 3 [oest]
		
		//obtenim la cambra adjacent segons la paret p obtinguda
		if(p == paret::NORD){
			--i;
		}
		else if(p == paret::SUD){
			++i;
		}
		else if(p == paret::EST){
			++j;
		}
		else if(p == paret::OEST){
			--j;
		}
		
		//Comprovem que:
		//	a) la paret no doni a l'exterior
		if(i >= 1 && i <= nfils && j >= 1 && j <= ncols){
			adj = make_pair(i,j);
		//	b) Que les cambres adjacents no estiguin al mateix grup, per evitar cicles.
			if(!part.mateix_grup(cam,adj)){
				part.unir(cam, adj);
				M.obre_porta(p, cam);
			}
		} //en els casos en que no es dona a) i/o b), no es fa res.
		
	}
}

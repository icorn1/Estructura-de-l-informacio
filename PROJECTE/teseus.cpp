#include "teseus.hpp"
using namespace std;

/*
Pràctica d'ESIN Q1 21-22. Ixent Cornella Vilana, Eric Gonzalez Duro
****** MODUL "teseus": ******

OBJECTIUS DEL MODUL:
	Aquest mòdul defineix el namespace teseus que inclou la funció buscar que permet
	resoldre laberints del sistema MAZE. La funció rep un laberint i troba el camí
	més curt (el que passa per un menor nombre de cambres) entre la cambra inicial i la
	final. En cas que hi hagi més d’un camí amb la mateixa mida retornarem qualsevol dels
	camins. 
	
IMPLEMENTACIÓ ESCOLLIDA:
	Resumint els objectius del mòdul, cal trobar el camí més curt d'una posició a una altra d'un laberint.
	S'ha de tenir en compte que potser que no hi hagi solució (que no hi hagi camí) o que alguna posició
	sigui invàlida. Llavors, plantejem el problema de la seguent manera: Tenim un laberint, excavat (o no)
	en el que, si son valides, cal trobar el camí més curt entre dos posicions, tot i respectant el ordre
	segons el que es pot accedir a posicions (en termes del projecte, veure si la porta entre dos cambres
	està oberta). És moooolt més fàcil veure el problema no com un laberint, sinó com un graf. Cada cambra 
	del laberint és un vèrtex, i si la porta està oberta entre dos cambres, llavors hi ha un enllaç (no dirigit)
	entra aquell vertex i l'adjaçent. Llavors, el laberint es transforma en un rectangle de vèrtex (amb la
	mateixa forma que el laberint), amb un número màxim d'arestes n*(n-1)/2, amb n = nfils*ncols = vertexs.
	
	Ara que tenim aquesta visió del problema, és més fàcil resoldre'l. Hi han de fet diversos algoritmes que,
	donats dos vèrtexs, ens retornen el camí o si hi ha camí entre ells. Alguns d'aquests son:
	- Depth-first Search: Aquest algoritme troba un camí rapidament, però no té perquè ser el més curt.  
	- Dijkstra: Aquest algoritme és ideal, troba el camí més curt entre dos vertex. Ara bé, està pensat per
		    grafs dirigits, i el nostre és no dirigit. Massa complicat.
	- Breadth-First Search: Aquest últim algoritme, el que utilitzarem, trobarà el camí més curt entre dos vèrtexs, ja que,
				 si bé recorr tot el graf, primer explora el camí més curt, llavors només ens cal comprovar a cada
				 pas si el vertex al que estem és el final, i si és així, parar. 
	
	Finalment, farem una cerca en amplada, utilitzant la classe stl::list que s'ens ofereix al .hpp.
	Ara bé, hi han dos obstacles: 
	- El graf pot tenir cicles (s'ens especifica a l'enunciat), per tant, ens cal saber si hem visitat el vertex que estem mirant, per tal d'eviar cicliciatat.
		-> solució: array de booleans que ens indica si hem vist aquell vertex.
	- El graf és no dirigt. Ens fa falta una manera de saber els predecessors, per poder guardar el camí.
		-> solució: array de posicions, que anem omplentant amb els predecessors del vertex que s'obté a cada iteració.
	
*/


#define INT_MAX 2147483647 //Usarem INT_MAX per tenir un valor NULL pels nats, ens servirà per fer comparacions al final de la funció buscar.

void teseus::buscar(const laberint & M, const posicio & inici, const posicio & final, std::list<posicio> & L) throw(error){
//PRE: L es una llista buida
//POST: Modifica el contingut de L amb el camí entre inici i final del laberint M, si es que la posicio és vàlida i hi ha camí entre aquestes dos
//	En cas de que no es donguin aquestes condicions, tira error. 
//COST: El cost de les funcions que cridem a laberint son constants, i el cos de BFS és Θ(v+e), on v = nfils*ncols = n, i e son les arestes 
//	dels vertexs, en el pitjor cas seran 4n arestes. Llavors tenim Θ(n+4n) = O(n).
	
	//Per fer-ho mes net
	nat ncols = M.num_columnes();
	nat nfils = M.num_files();
	bool condicio1 = not (inici.first >= 1 && inici.first <= nfils && inici.second >= 1 && inici.second <= ncols);
	bool condicio2 = not (final.first >= 1 && final.first <= nfils && final.second >= 1 && final.second <= ncols);
	if(condicio1 or condicio2){
		throw error(IniciFinalNoValid);
	}
	
	//Hem decidit no crear memoria dinamica ni per a booleans ni per a les posicions, per evitar generar memoria brossa en cas d'errors.
	bool visitats[nfils][ncols];
	posicio pred[nfils][ncols];  
   	
	for(nat i = 0; i < nfils; ++i){
		for(nat j = 0; j < ncols; ++j){
			visitats[i][j] = false;
			pred[i][j] = make_pair(INT_MAX,INT_MAX); 	//Al principi inicialitzem a INT_MAX per fer comparacions posteriorment.
		}							//Hem triat INT_MAX perque treballem amb nats, i no podem usar valors com ara
									//-1 per representar posicions Nules. 
	}
	
	bool trobat = false;	//Ens dirà si té solució el laberint.
	
	//Algoritme BFS: Primer vertex que visitem es l'inicial, el marquem com vist.
	visitats[inici.first-1][inici.second-1] = true;
	if(inici == final)
		trobat = true;

	L.push_back(inici);
	//mentre que no trobem un cami o haguem acabat de cercar:
	while (!L.empty() && !trobat){
	
		// Desencuem per mirar el cami desde el vertex obtingut.
		posicio actual = L.front();
		L.pop_front();

		//Recorrem tots els adjacents del vertex que hem encuat, que son com a maxim 4 (nord, est, sud, oest)
		//i els marquem com visitats si no ho estan, i després encuem. Al ser només
		//4, podem fer if elses per comprovar les adjacencies, i obtenir el vertex adjacent corresponent.
		
		for(nat i = paret::NORD; i < paret::NO_DIR && !trobat; i++){
		
			nat fil = actual.first;
			nat col = actual.second;
			cambra cam = M(actual);
			
			bool valid = false; //Ens dirà si l'adjacencia és valida (és valida si no dona a l'exterior i si hi ha porta oberta entre les dos cambres).
			if(i == paret::NORD){
				--fil;
				if(fil >= 1 && cam.porta_oberta(i)){ //recordem que la i equival a la paret corresponent
					valid = true;
				}	
			}
			else if(i == paret::SUD){
				++fil;
				if(fil <= nfils && cam.porta_oberta(i)){
					valid = true;
				}
			}
			else if(i == paret::EST){
				++col;
				if(col <= ncols && cam.porta_oberta(i)){
					valid = true;
				}
			}
			else if(i == paret::OEST){
				--col;
				if(col >= 1 && cam.porta_oberta(i)){ //recordem que la i equival a la paret corresponent
					valid = true;
				}
			}
			
			if(valid){	//Si es valida l'adjacencia, seguim amb el BFS:
			
				posicio adj = make_pair(fil, col);
				if(final == adj){	//Estem al vertex final, llavors trobat = true i marquem predecessor.
					trobat = true;
					pred[fil-1][col-1] = actual;
				}
				
				 //ara treballem amb arrays, que indexen desde 0, mentre que les posicions començen a (1,1). Es important decrementar fil i col.
				--fil;
				--col;
				if(!trobat && !visitats[fil][col]){
					visitats[fil][col] = true;	//marquem com vist
					L.push_back(adj);		//el fiquem a la cua.
					pred[fil][col] = actual; 	//el predecessor del que hem obtingut es la cambra que fou l'actual.
				}
			}
		}
	}	//Fi del while
	
	if(not trobat){	//Si no hem trobat solució, tirem error.
		throw error(SenseSolucio);
	}
	else{			//En cas contrari, obtindrem el camí a partir dels predecessors.
		list <posicio> cami;
		posicio petjada = final; //recorrem el cami inversament, però encuem de manera frontal, llavors la llista mostrara el cami en ordre
		posicio invalida = make_pair(INT_MAX, INT_MAX);
		cami.push_front(petjada);
		
		while (pred[petjada.first-1][petjada.second-1] != invalida) {	//Si el predecessor es invalid, vol dir que hem arribat al final del cami.
			cami.push_front(pred[petjada.first-1][petjada.second-1]);
			petjada = pred[petjada.first-1][petjada.second-1];
		}
		L = cami; //"Retornem" L, cal actualitzar-la amb el camí obtingut.
	}	
}

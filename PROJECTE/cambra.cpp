#include "cambra.hpp"

cambra::cambra(bool n, bool s, bool e, bool o) throw(error){
// PRE: Els 4 parametres son explicitament booleans.
// POST: Crea una cambra amb les parets obertes en funcio dels parametres donats.
// COST: Cost constant. Θ(1)

	_nord = n;
	_sud = s;
	_est = e;
	_oest = o;
}

cambra::cambra(const cambra &c) throw(error){
// PRE: 
// POST: Crea una cambra copiant els valors dels atributs privats de c.
// COST: Cost constant. Θ(1)

	_nord = c._nord;
	_sud = c._sud;
	_est = c._est;
	_oest = c._oest;
}

cambra& cambra::operator=(const cambra & c) throw(error){
// PRE: 
// POST: Assigna una els valors privats del p.i. als de la cambra c
// COST: Cost constant. Θ(1)
	_nord = c._nord;
	_sud = c._sud;
	_est = c._est;
	_oest = c._oest;
	return *this;
}

cambra::~cambra() throw(){
// PRE: 
// POST: Esborra la memoria dinamica d'una cambra.
// COST: Cost constant. Θ(1)

//No usem memoria dinamica
}

// Retorna si l'habitació té una porta oberta a la paret indicada.
bool cambra::porta_oberta(paret p) const throw(){
// PRE: 
// POST: Retorna si la paret p del p.i. esta oberta (si el valor privat es igual a true).
// COST: Cost constant. Θ(1)
	if(p == paret::NORD) 		return _nord;	
	else if(p == paret::EST) 	return _est;
	else if(p == paret::SUD) 	return _sud;
	else if(p == paret::OEST) 	return _oest;
	
	return false; // Ultim cas: que la paret sigui NO_DIR -> no te porta oberta.
}

// Obre una nova porta a la paret indicada. Si la porta ja està oberta no
// fa res. Es produeix un error si la paret és NO_DIR.
void cambra::obre_porta(paret p) throw(error){
// PRE: 
// POST: Fica a TRUE el valor privat equivalent a la paret p, o tira un error si p es igual a NO_DIR.
// COST: Cost constant. Θ(1)
	if(p == paret::NORD) 		_nord = true;	
	else if(p == paret::EST) 	_est  = true;
	else if(p == paret::SUD) 	_sud  = true;
	else if(p == paret::OEST) 	_oest = true;
	
	else throw error(ParetInexistent);
}

// Tanca la porta a la paret indicada. Si la porta ja estava tancada no
// fa res. Es produeix un error si la paret és NO_DIR.
void cambra::tanca_porta(paret p) throw(error){
// PRE: 
// POST: Fica a FALSE el valor privat equivalent a la paret p, o tira un error si p es igual a NO_DIR.
// COST: Cost constant. Θ(1)
	if(p == paret::NORD) 		_nord = false;	
	else if(p == paret::EST) 	_est  = false;
	else if(p == paret::SUD) 	_sud  = false;
	else if(p == paret::OEST) 	_oest = false;
	
	else throw error(ParetInexistent);
}

// Igualtat i desigualtat entre cambres. Dues cambres es consideren iguals
// si tenen les mateixes portes obertes a les mateixes parets.
bool cambra::operator==(const cambra & c) const throw(){
// PRE: 
// POST: Retorna si la cambra c te els mateixos valors privats que el p.i.
// COST: Cost constant. Θ(1)
	if(_nord == c._nord && _sud == c._sud && _est == c._est && _oest == c._oest) return true;
	else return false;
	
}
bool cambra::operator!=(const cambra & c) const throw(){
// PRE: 
// POST: Retorna si la cambra c no te els mateixos valors privats que el p.i.
// COST: Cost constant. Θ(1)
	return not(*this == c);
}


// Operador "menor que" entre cambres. Una cambra és més petita que una
// altra si té més portes tancades que l'altra. Tenint en compte que una
// porta tancada és més petita que una porta oberta, en cas que tinguin el
// mateix número de portes tancades, per decidir qui és més petit es miraran
// les portes en aquest ordre NORD, EST, SUD i OEST. Per exemple:
//   cambra c1, c2(true,true), c3(true, false, true);
//   cout << (c1 < c2); // escriu 'true'
//   cout << (c2 < c3); // escriu 'true'
bool cambra::operator<(const cambra & c) const throw(){
// PRE: 
// POST: Retorna true si la cambra p.i. es "menor que" c, seguint els criteris mencionats anteriorment.
// COST: Cost constant. Θ(1)
	
	char valor1[] = "nnnn"; 
	int cont1 = 0;
	
	//El que farem es contar les portes obertes de cada cambra i a la vegada marcar-les.
	if(_nord) {
		++cont1;
		valor1[0] = 's';
	}
	if(_est) {
		++cont1;
		valor1[1] = 's';
	}
	if(_sud) {
		++cont1;
		valor1[2] = 's';
	}
	if(_oest) {
		++cont1;
		valor1[3] = 's';
	}
	
	char valor2[] = "nnnn";
	int cont2 = 0;
	if(c._nord) {
		++cont2;
		valor2[0] = 's';
	}
	if(c._est) {
		++cont2;
		valor2[1] = 's';
	}
	if(c._sud) {
		++cont2;
		valor2[2] = 's';         
	}
	if(c._oest) {
		++cont2;
		valor2[3] = 's';
	}
	
	//mirem resultats 
	if(cont1 < cont2){ // Casos bases: que no tinguin el mateix nombre de portes obertes
		return true;
	}
	else if (cont1 > cont2){
		return false;
	}
	else{	//cas en el que les dos cambres tenen les mateixes portes obertes
		int i = 0;
		while(valor1[i] == valor2[i]){ //comprovem els valors de cada orientacio, fins que difereixin (o no)
			i++;
		}
		if(i == 4){
			return false; //les cambres son exactament iguals.
		}
		else{
			if(valor1[i] == 's') return false;	//estem en el cas de que les portes obertes han diferit, per tant si la del parametre implicit es la que esta oberta,
								//vol dir que aquesta es mes gran, per tant l'operador "<" retornara fals.
			else return true; 			//cas contrari. 
		}	 
	}
}

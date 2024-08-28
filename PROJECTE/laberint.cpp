#include "laberint.hpp"
#include <iostream>

laberint::laberint(nat num_fil, nat num_col) throw(error){
// PRE: 
// POST: Crea una laberint amb num_fil files i num_col columnes, i tira error FilColIncorrecte si algun d'aquests dos parametres son 0. 
// COST: Cost constant. Θ(1)
	if(num_fil > 0 && num_col > 0){
		_nfils = num_fil;

		_ncols = num_col;
		
		_lab = new cambra[_nfils*_ncols]; 
	}
	else throw error(FilsColsIncorrecte);
}


// Constructora d'un laberint a partir d'un istream (canal d'entrada) is.
// El format de l'istream seguirà l'exposat a l'apartat 2.3. Es presuposa que
// el laberint és correcte.

  /* Exemple d'entrada:  	|   Moltes posicions son ignorades:

  3 11                    	| 'i' = ignorada.
  ***********************    	| iiiiiiiiiiiiiiiiiiiiiii       
  * *   * *   *     *   *    	| *i*i i*i*i i*i i i*i i*
  * *** * * * *** * * ***   	| i i*i i i i i*i i i i*i 
  * *   *   * *   *     *	| *i*i i*i i*i*i i*i i i*
  * * * ***** * * ***** *   	| i i i i*i*i i i i*i*i i
  *   *         * *     *  	| *i i*i i i i i*i*i i i*
  ***********************    	| iiiiiiiiiiiiiiiiiiiiiii
                         	|
                         	Com es pot veure, segueix un patró en funció
                         	de la fila i la columna. (També s'ignoren els
                         	bordes inferiors i superiors, seguint el format
                         	del apartat 2.3)
  */
laberint::laberint(std::istream & is) throw(error){
// PRE: El laberint introduït pel canal estandard es correcte.
// POST: Crea un laberint nou, a partir del canal istream (obrint les cambres que toquin en funcio d'aquest)
// COST: Cost lineal. Θ(n) (amb n = _tam, també es pot escriure com Θ(_nfils*_ncols)).

	nat ncols, nfils;
	is >> nfils >> ncols;	//llegim nfiles i ncols
	_ncols = ncols;
	_nfils = nfils;
	_lab = new cambra[_nfils*_ncols];
	
	string cfil;	//METODE PER LLEGIR: Usarem un getline per cada fila, i aixi simplificar la lectura.
	char inp;
	nat fila = 0;		//representa les files.
	nat col = 0; 		//indica la columna.
	int cambra = -1; 	//indica la cambra sobre la que estem.
	std::getline(is, cfil); //ignorem la primera fila
	 
	while(fila < _nfils*3 - (_nfils-1)){ 	//Mentres que no sigui l'ultima fila, llegim per pantalla
		std::getline(is, cfil);
		
		if(fila != 0 && fila != _nfils*3 - (_nfils-1) - 1){ //mirem que no estiguem a un borde superior/inferior
			cambra = ((fila-1)/2)*_ncols -1;
			
			while(col < _ncols*3 - (_ncols-1)){	//Mentres que no sigui l'ultima columna, llegim per pantalla
				inp = cfil[col];
				
				if(col != 0 && col != _ncols*3 - (_ncols-1)- 1){ //mirem que no estiguem a un borde lateral.
				
					// si la fila es parella i la columna imparella, vol dir que cal obrir una paret NORD/SUD si el caracter donat es ' '.
					if(fila%2 == 0){	
						if(col%2 == 1 && inp == ' '){
							                                    
							_lab[cambra].obre_porta(paret::SUD);           
							_lab[cambra+_ncols].obre_porta(paret::NORD);  
							                                                     
						}
					}
					else {
						if(col%2 == 0 && inp == ' '){	//altrament, si la fila es imparella i la col parella, cal mirar si caracter es ' ' per obrir cambra EST/OEST.
						                            
							_lab[cambra].obre_porta(paret::EST);          
							_lab[cambra+1].obre_porta(paret::OEST);                                                  
						}
					}
				}
				++col;
				if(col%2 == 1) ++cambra;	
			}
		}
		
		++fila;
		col = 0; 
	}
}


// Constructora per còpia, assignació i destructora.
laberint::laberint(const laberint & l) throw(error){
// PRE: 
// POST: Crea un laberint copiant els valors dels atributs privats de l.
// COST: Cost lineal. Θ(n) (amb n = _tam, també es pot escriure com Θ(_nfils*_ncols)).

	_nfils = l._nfils;
	_ncols = l._ncols;
	
	_lab = new cambra[_nfils*_ncols];
	for(nat i = 0; i < _nfils*_ncols; ++i){
		cambra c(l._lab[i]);
		_lab[i] = c;
	}
}


laberint & laberint::operator=(const laberint & l) throw(error){
// PRE: 
// POST: Assigna els valors privats de l al p.i., tot borrant la memoria dinamica anterior del p.i. Retorna el p.i.
// COST: Cost lineal. Θ(n) (amb n = _tam, també es pot escriure com Θ(_nfils*_ncols)).
	if(this != &l){
		_nfils = l._nfils;
		_ncols = l._ncols;

		//borrem memoria dinamica

		delete[] _lab;

		_lab = new cambra[_nfils*_ncols];

		for(nat i = 0; i < _nfils*_ncols; ++i){
			cambra c(l._lab[i]);
			_lab[i] = c;
		}
	}
	return *this;
}


laberint::~laberint() throw(){
// PRE: 
// POST: Esborra la memoria dinamica del objecte.
// COST: Cost lineal, l'operador delete[] fa nfils*ncols iteracions per borrar la memoria dinàmica. Θ(n)

	delete[] _lab;
}


// Retornen el número de files i columnes que té el laberint, respectivament.
nat laberint::num_files() const throw(){
// PRE: 
// POST: Retorna el nombre de files del laberint
// COST: Cost constant. Θ(1)

	return _nfils;
}


nat laberint::num_columnes() const throw(){
// PRE: 
// POST: Retorna el nombre de columnes del laberint
// COST: Cost constant. Θ(1)

	return _ncols;
}


cambra laberint::operator()(const posicio &pos) const throw(error){
// PRE: 
// POST: Retorna la cambra corresponent a la posicio pos, si aquesta existeix. En cas contrari, tira error PosicioInexistent. 
// COST: Cost constant. Θ(1)

	nat p1 = pos.first;
	nat p2 = pos.second;
	nat posi;

	if(p1 <= _nfils && p2 <= _ncols && p1 > 0 && p2 > 0){     
		posi = (p1-1) * _ncols + p2 - 1;      //calculem la posicio que li correspon a pos a l'array _lab.           
		return _lab[posi];                                    
	}

	else throw error(PosicioInexistent);

}

void laberint::obre_porta(paret p, const posicio & pos) throw(error){
// PRE: 
// POST: Obra la porta de la cambra a la posicio pos (si aquesta existeix, en cas contrari, tira error PosicioInexistent), i tambe l'adjacent corresponent. 
// POST: En cas de que la posicio sigui correcta pero sigui una porta exterior que es vol obrir, tira un error. Si la porta es invalida, tira un error tambe.
// COST: Cost constant. Θ(1)

	nat p1 = pos.first;
	nat p2 = pos.second;

	if(p == paret::NO_DIR){
		throw error(cambra::ParetInexistent);
	}

	else{

		if(p1 <= _nfils && p2 <= _ncols && p1 > 0 && p2 > 0){
			if(p1 == 1 && p == paret::NORD){	//casos que dona a l'exterior 
				throw error(PortaExterior);
			}
			else if(p2 == 1 && p == paret::OEST){
				throw error(PortaExterior);
			}
			else if(p1 == _nfils && p == paret::SUD){
				throw error(PortaExterior);
			}
			else if(p2 == _ncols && p == paret::EST){
				throw error(PortaExterior);
			}
			else{
				if(p == paret::NORD){
					//obrir_porta p posicio pos
					//obrir_porta SUD posicio pos.first-1
					nat posi1 = (p1-1) * _ncols + p2 - 1;
					_lab[posi1].obre_porta(p);
					
					nat posi2 = (p1-2)*_ncols+p2-1;
					_lab[posi2].obre_porta(paret::SUD);
				}
				else if(p == paret::SUD){
					//obrir_porta p posicio pos
					//obrir_porta NORD posicio pos.first+1
					nat posi1 = (p1-1) * _ncols + p2 - 1;
					_lab[posi1].obre_porta(p);
					
					nat posi2 = (p1)*_ncols+p2-1;
					_lab[posi2].obre_porta(paret::NORD);
				}
				else if(p == paret::EST){
					//obrir_porta p posicio pos
					//obrir_porta OEST posicio pos.second+1
					nat posi1 = (p1-1) * _ncols + p2 - 1;
					_lab[posi1].obre_porta(p);
					
					nat posi2 = (p1-1)*_ncols+p2;
					_lab[posi2].obre_porta(paret::OEST);
				}
				else if(p == paret::OEST){
					//obrir_porta p posicio pos
					//obrir_porta EST posicio pos.second-1
					nat posi1 = (p1-1) * _ncols + p2 - 1;
					_lab[posi1].obre_porta(p);
					
					nat posi2 = (p1-1)*_ncols+p2-2;
					_lab[posi2].obre_porta(paret::EST);
				}
			}
		}   
	
		else throw error(PosicioInexistent);
	}
}

// Tanca la porta en la paret donada de la cambra que està especificada per pos.
// També tanca la porta corresponent en la cambra adjacent. Es produeix un error
// si la posició no existeix.
void laberint::tanca_porta(paret p, const posicio & pos) throw(error){
// PRE: 
// POST: Tanca la porta de la cambra a la posicio pos (si aquesta existeix, en cas contrari, tira error PosicioInexistent), i tambe l'adjacent corresponent. 
// COST: Cost constant. Θ(1)

	nat p1 = pos.first;
	nat p2 = pos.second;

	if(p1 <= _nfils && p2 <= _ncols && p1 > 0 && p2 > 0){
		
		if(p == paret::NORD){
			//tancar_porta p posicio pos
			//tancar_porta SUD posicio pos.first-1
			nat posi1 = (p1-1) * _ncols + p2 - 1;
			_lab[posi1].tanca_porta(p);
			
			if(p1 != 1){
				nat posi2 = (p1-2)*_ncols+p2-1;
				_lab[posi2].tanca_porta(paret::SUD);
			}
		}
		else if(p == paret::SUD){
			//obrir_porta p posicio pos
			//obrir_porta NORD posicio pos.first+1
			nat posi1 = (p1-1) * _ncols + p2 - 1;
			_lab[posi1].tanca_porta(p);
			
			if(p1 != _nfils){
				nat posi2 = (p1)*_ncols+p2-1;
				_lab[posi2].tanca_porta(paret::NORD);
			}
		}
		else if(p == paret::EST){
			//obrir_porta p posicio pos
			//obrir_porta OEST posicio pos.second+1
			nat posi1 = (p1-1) * _ncols + p2 - 1;
			_lab[posi1].tanca_porta(p);
			
			if(p2 != _ncols){
				nat posi2 = (p1-1)*_ncols + p2;
				_lab[posi2].tanca_porta(paret::OEST);
			}
		}
		else if(p == paret::OEST){
			//obrir_porta p posicio pos
			//obrir_porta EST posicio pos.second-1
			nat posi1 = (p1-1) * _ncols + p2 - 1;
			_lab[posi1].tanca_porta(p);
			
			if(p2 != 1){
				nat posi2 = (p1-1)*_ncols + p2 - 2;
				_lab[posi2].tanca_porta(paret::EST);
			}
		}
		
		
	}   

	else throw error(PosicioInexistent);
}


// Escriu el laberint a l'ostream (canal de sortida) os. El format per escriure
// el laberint seguirà l'exposat a l'apartat 2.3.
void laberint::print(std::ostream & os) const throw(){
// PRE: 
// POST: Mostra el laberint corresponent al p.i. pel canal estandard de sortida. 
// COST: Cost lineal. Θ(n) (amb n = _tam, també es pot escriure com Θ(_nfils*_ncols)).
	os << _nfils << " " << _ncols << "\n";
	
	nat col = 0;
	int cambra = -1;
	for(nat i = 0; i < (_ncols*3 - (_ncols-1)); ++i){ //borde superior
		os << "*";
	}
	int fils = _nfils*3 - (_nfils-1) -2; //fem les files que toquen (menys els bordes, que son 2)
	int i = 0;
	while(i < fils){
		os << "\n";
		col = 0;
		
		cambra = (i/2)*_ncols -1;
	
		for(nat j = 0; j < (_ncols*3 - (_ncols-1)); ++j){
			if(j == 0 or j == _ncols*3 - _ncols){ //principi o final
				os << "*";
			}
			else{
				if(i%2 == 0){
					if(col%2 == 0){
						if(_lab[cambra].porta_oberta(paret::EST)) os << " ";
						else os << "*";
					}
					else{
						os << " "; 
						++cambra;
					}
				}
				else{
					if(col%2 == 0){
						os << "*"; 
					}
					else{	
						++cambra;
						if(_lab[cambra].porta_oberta(paret::SUD)) os << " "; // 
						else os << "*";
						
					}
				}
			}
			++col;
		}
		++i;
	}
	os << "\n";
	for(nat i = 0; i < (_ncols*3 - (_ncols-1)); ++i){ //borde inferior
		os << "*";
	}
	os << "\n";
	
}


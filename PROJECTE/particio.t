#include <iostream>
using namespace std;

template <typename T>
particio<T>::particio(nat n) throw(error){
// PRE: 
// POST: Crea una partició amb n elements. 
// COST: Cost lineal, inicialitzem n punters a llistes a NULL. Θ(n)
    _part = new node_hash*[n]; // Demanem n elements de tipus nat
    _nmax = n;
    _nelems = 0;
    _grups = 0;

    for(nat i = 0; i < _nmax; ++i){
        _part[i] = NULL;    // Inicialitzem a NULL les n llistes 
    }
}

//*****FUNCIÓ PRIVADA*****
template <typename T>
typename particio<T>::node_hash* particio<T>::copia_llista(node_hash *p){
//PRE: ja s'ha reservat espai per una taula _part de tamany _nmax (tamany maxim del p.i., que ha de ser igual a p._nmax).
//POST: Copia a la taula reservada cada llista de la taula de dispersió, incloent els atributs de cada node.
//COST: Cost lineal, es copien tots els atributs de cada node de la llista Θ(n) amb n = p._part[i].size(). 
	node_hash *desti = NULL;
	if(p  != NULL){
		desti = new node_hash;
		desti->_k = p->_k; 
		desti->_seg = copia_llista(p->_seg);
		desti->_tam = p->_tam;
	}
	return desti;
	/*
	 PROBLEMA DETECTAT:
		No es pot assignar el representant de un node a l'hora de copiar la llista, ja que
		pot ser que aquest node no existeixi encara (que es copii més tard).
	   SOLUCIÓ:
	   	No modifiquem el representant del node copiat quan estem copiant els demés atributs,
	   	sinó que ho fem més tard a la funció que ha cridat aquesta funció originalment, per evitar
	   	errors d'enllaços greus.
	*/
}

// Constructora per còpia, assignació i destructora.
template <typename T>
particio<T>::particio(const particio & p) throw(error){
// PRE: 
// POST: Crea una particio copiant els valors dels atributs privats de p.
// COST: Cost lineal, hem de copiar tots els atributs de cada node de la particio. Θ(n).

	_nelems = p._nelems;
	_grups = p._grups;
	_nmax = p._nmax;

	_part = new node_hash*[_nmax]; //reservem memoria per a crear la particio
	
	for(nat i = 0; i < _nmax; ++i){
		_part[i] = copia_llista(p._part[i]); 
		//Copiem els representants
		node_hash *n = _part[i];
		node_hash *n2 = p._part[i];
		while(n != NULL){
			if(n2->_rep != NULL) //Si el representant a buscar no s'apunta a ell mateix, cal buscar el node que correspondria al valor del representant.
				n->_rep = find(n2->_rep->_k);
			else 
				n->_rep = NULL; 	//Sinó, el fiquem a NULL.
			
			n = n->_seg;
			n2 = n2->_seg;		
		}
	}
}

template <typename T>
particio<T> & particio<T>::operator=(const particio & p) throw(error){
// PRE: 
// POST: Assigna els valors privats de p al p.i., tot borrant la memoria dinamica anterior del p.i. Retorna el p.i.
// COST: Cost lineal. Θ(n).

    if(this != &p){ // si son iguals no cal copiar.
    		//borrem memoria dinamica
    	    for(nat i = 0; i < _nmax; ++i)	
    	    	esborra_llista(_part[i]);	
	    delete[] _part;
	    
	    _nelems = p._nelems;
	    _grups = p._grups;
	    _nmax = p._nmax;
	    _part = new node_hash*[_nmax];
	    for(nat i = 0; i < _nmax; ++i){ 
		_part[i] = copia_llista(p._part[i]);
		// Copiem els representants al p.i.
		node_hash *n = _part[i];
		node_hash *n2 = p._part[i];
		while(n != NULL){
			if(n2->_rep != NULL) //Si el representant a buscar no s'apunta a ell mateix, cal buscar el node que correspondria al valor del representant.
				n->_rep = find(n2->_rep->_k);
			else 
				n->_rep = NULL; 	//Sinó, el fiquem a NULL.
			
			n = n->_seg;
			n2 = n2->_seg;		
		}
	}
    }

    return *this;
}

template <typename T>
void particio<T>::esborra_llista(node_hash *p){
// PRE:
// POST: Esborra la llista simplement encadenada corresponent a una entrada a la taula de dispersió.
// COST: Dèpen de les colisions que hi hagi, generalment seran poques. Θ(tamany de la llista).
	if(p != NULL){
		esborra_llista(p->_seg);
		delete p;
	}
}
template <typename T>
particio<T>::~particio() throw(){
// PRE: 
// POST: Esborra la memoria dinamica de l'objecte.
// COST: Cost lineal, segons complexitat teorica. Θ(n)
    for(nat i = 0; i < _nmax; ++i)
    	esborra_llista(_part[i]);	
    delete[] _part;
}


/*****FUNCIÓ PRIVADA******/
template <typename T>
typename particio<T>::node_hash* particio<T>::find(const T &x) const{
//PRE: es pot usar util::hash() sobre el tipus T.
//POST: Si x no es troba en la particio, retorna NULL. En cas contrari, retorna el node en el que està dins d'una de les llistes.
//COST: Cost constant. Θ(1)
	unsigned long i = util::hash(x)%_nmax; //obtenim hash
        node_hash *p = _part[i];
        
        while(p != NULL and p->_k != x){ //Pot ser que hi hagi colisions en la nostra taula de dispersió. Llavors, cal buscar l'element a la llista corresponent.
            p = p->_seg;
        }
        return p; //p pot ser un NULL 
}

template <typename T>
typename particio<T>::node_hash* particio<T>::find_rep(const T &x) const{
//PRE: 
//POST: 
//COST: Cost constant. Θ(1)
	
	node_hash *p = find(x);
	node_hash *res = NULL;
	while(p != NULL){
		res = p;
		p = p->_rep;
	}
	return res;
}


// Afegeix un nou element a la particio. Aquest nou element formarà ell sol
// un nou grup. En cas que l'element ja estigués present a la partició llavors
// no es modifica la partició. Es produeix un error si la particio ja té el
// número màxim d'elements abans d'afegir aquest nou.
template <typename T>
void particio<T>::afegir(const T &x) throw(error){
//PRE: 
//POST: l'element x ha estat afegit a la particio (si no estaba afegit ja, 
//      en aquest cas, la particio no es modifica). Llença un error si la particio ja esta plena 
//COST: Cost constant. Θ(1)

    if(_nmax > 0){ //la particio ha de tenir com a minim tamany 1
	node_hash *p = find(x);
	
        bool trobat = true; //trobat ens indica si ja estava l'element a la particio.
        if (p == NULL) 
        	trobat = false; 
        if(!trobat){ //inserim l'element donat x
            
            if(_nelems == _nmax) throw error(ParticioPlena);	//Si la particio està completa, tirem error.
            
            else{
		unsigned long i = util::hash(x)%_nmax; //obtenim hash
                p = _part[i]; //inserim al principi, cal guardar el node inicial
                _part[i] = new node_hash;
                _part[i]->_k = x;
                _part[i]->_rep = NULL;	//Sempre que afegim, el representant ha de ser NULL. (Ens facilita el codi)
                _part[i]->_tam = 1;		//El tamany es 1, ja que el node forma un grup propi on només està ell.
                _part[i]->_seg = p;		//Això ho fem per tal d'inserir al principi.
                ++_nelems;
                ++_grups;
            }
        }        
    }
    else throw error(ParticioPlena);
}


// Uneix els dos grups als quals pertanyen aquests dos elements. Si tots dos
// elements ja pertanyien al mateix grup no fa res.
// Es produeix un error si algun dels elements no pertany a la partició.

template <typename T>
void particio<T>::unir(const T & x, const T & y) throw(error){
//PRE: 
//POST: Les dues particions x i y donades, han estat unides en el mateix grup. Llença un error 
//      si un dels dos elements no pertany a alguna particio

//COST: Tot i que hi han molts bucles, els representants dels nodes que obtindrem, no estaran a més de dos nodes de diferencia
//	 entre ells. Podem veure els nodes de hash com un arbre, en el qual _rep ens indica el pare del node. Si _rep es NULL, llavors es l'arrel de l'arbre. 
//	Lo important és que, al fer quick-union (unim per pes els grups, és adir, el node amb menor tamany passarà a apuntar el de major tamany), l'alçada del arbre
//	pot augmentar molt, cosa que agumentar el cost temporal. Per contrarestar-ho, fem compressió total, és a dir, fem que el representant del node original sigui
//	el representant que obtenim, reduint així l'alçada de l'arbre a 2 (el pare i els fills).
//	Finalment, després de fer recerca, hem obtingut que el cost serà **Θ(α(n))**, on α(n) = factor de càrrega = (_nelems/_namx). Essencialment, podem arribar a costos
//	constants si el factor de càrrega es petit, i en el pitjor cas, logartimics. 

    if(_nmax > 0){ //la particio ha de tenir com a minim tamany 1
        
        node_hash *p1 = find(x);
        bool t1 = true;
        if (p1 == NULL){ 
        	t1 = false;
        }    
		node_hash *p2 = find(y);
        bool t2 = true;
        if (p2 == NULL) {
        	t2 = false;
        }
  
		if(t1 && t2){
		
			if(p1 != p2){	//Si son el mateix node no cal fer res
			 
				if(!mateix_grup(x, y)){    

					node_hash *rep1 = find_rep(p1->_k);
					node_hash *rep2 = find_rep(p2->_k);

					//MODIFIQUEM ENLLAÇOS/TAMANYS PER TAL D'UNIR GRUPS
					if(rep1->_tam > rep2->_tam){

						rep1->_tam += rep2->_tam;   	//incrementem grandaria, ja que ara sosté un element mes en el grup
						rep2->_rep = rep1;
						p2->_rep = rep1;	// COMPRESSIO: Fem que el node original ara apunti tambe al representant obtingut.
					}
					else{

						rep2->_tam += rep1->_tam; 		//incrementem grandaria, ja que ara sosté un element mes en el grup
						rep1->_rep = rep2;   
						p1->_rep = rep2; 	// COMPRESSIO: Fem que el node original ara apunti tambe al representant obtingut.
					}
					--_grups;        		// Si hem arribat fins aquest punt, hi ha un grup menys a la particio.            
		       	}
		    }         
		} 

        else throw error (ElemInexistent);
    } 

    else throw error(ElemInexistent);       
}

// Retorna si els elements x i y pertanyen al mateix grup.
// Es produeix un error si algun dels dos elements no pertany a la partició.
template <typename T>
bool particio<T>::mateix_grup(const T & x, const T & y) const throw(error){
//PRE: 
//POST: Retorna true si x i y pertanyen al mateix grup, i retorna false en cas contrari 
//COST: Igual que unió, depèn de l'alçada del "arbre" que es generaria amb els representants dels dos elements,
//	En el pitjor cas, l'alçada del arbre serà 2 (recordem que fem quick-union i compressió total), reduint així
//	els costos a casi constants. **Θ(α(n))**, on α(n) = factor de càrrega = (_nelems/_namx).
    
    if(_nmax > 0){ //la particio ha de tenir com a minim tamany 1
        
        node_hash *p1 = find(x);

        bool t1 = true;
        if (p1 == NULL) 
        	t1 = false;
            
        node_hash *p2 = find(y);

        bool t2 = true;
        if (p2 == NULL) 
        	t2 = false;
	
        if(t1 && t2){	//T1 i T2 ens indiquen si els elements x i y estan a la particio, per tant, cal comprobar que estiguin dins d'aquesta abans de fer res.

            if(p1 != p2){ //també cal comprovar que, un cop sabem que estan a la particio, no siguin el mateix element.
            
            	node_hash *rep1 = find_rep(x);
            	node_hash *rep2 = find_rep(y);
            	
                if(rep1 == NULL && rep2 == NULL){
                
                    return false;   //cas base: que els dos representants apuntin a NULL => grup diferent.
                }

                else{ 
                    //al final hem obtingut els representants de ambdos nodes. Comprovem si son iguals:
                    if(rep1 == rep2) return true;
                    else return false;
                }
                  
            }

            else return true;	//si son el mateix element retornem true
        }

        else throw error (ElemInexistent); //si no estan a la particio, tirem error.
    }

    else throw error (ElemInexistent); // cas en que nmax < 0
}

// Retorna el número de grups que té la particio.
template <typename T>
nat particio<T>::size() const throw(){
//PRE: 
//POST: Retorna el numero de grups que conté la partició
//COST: Cost constant. Θ(1)
    
    return _grups;
}

// Retorna el número d'elements que té la particio.
template <typename T>
nat particio<T>::num_elements() const throw(){
//PRE: 
//POST: Retorna el numero d'elements que conté la partició 
//COST: Cost constant. Θ(1)
    
    return _nelems;
}

// Retorna el número màxim d'elements que pot tenir la particio.
template <typename T>
nat particio<T>::num_maxim() const throw(){
//PRE: 
//POST: Retorna el numero màxim d'elements que pot arribar a tenir la partició
//COST: Cost constant. Θ(1)
    
    return _nmax;
}

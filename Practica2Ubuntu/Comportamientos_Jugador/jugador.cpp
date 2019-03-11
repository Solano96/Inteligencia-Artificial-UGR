#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <vector>
#include <set>

using namespace std;

// COGER OBJETO


bool ComportamientoJugador::puedoCogerObjeto(Sensores sensores){
	bool puedo = false;

	if(sensores.objetoActivo == '_'){
		if(sensores.superficie[2] == '1' && !bikini)
			puedo = bikini = true;
		else if(sensores.superficie[2] == '2' && !zapatillas)
			puedo = zapatillas = true;
		else if(sensores.superficie[2] == '3' && !llave)
			puedo = llave = true;
		else if(sensores.superficie[2] == '4' && !regalo && sensores.vida > 40){
			camino = false;

			for(int i = 0; i < 100; i++)
				for(int j = 0; j < 100; j++)
					if(mapaColina[i][j] < -500)
						mapaColina[i][j]+=1000;
	
			mapaObjetos[y_regalo][x_regalo] = -1;		
			puedo = regalo = true;
		}
	}
	
	return puedo;
}

bool ComportamientoJugador::puedoMeterObjeto(Sensores sensores){
	if(sensores.objetoActivo != '_' && sensores.terreno[0] != 'A' && sensores.terreno[0] != 'B')
		return true;
	return false;
}

bool ComportamientoJugador::puedePasar(Sensores sensores){
	if(sensores.terreno[2] == 'B' || sensores.terreno[2] == 'A' || sensores.terreno[2] == 'P' ||
	   sensores.terreno[2] == 'M' || sensores.terreno[2] == 'D' || sensores.superficie[2] == 'l' || sensores.superficie[2] == 'a'
	   || (sensores.superficie[2] == 'r' && !camino))
		return false;
	return true;
}

bool ComportamientoJugador::porAhiSePuede(int y, int x, int objetos[200][200], vector<vector<unsigned char> > mapa, int rey[200][200]){
	if(objetos[y][x] == 0 || objetos[y][x] == 1 || objetos[y][x] == 2 || objetos[y][x] == 3)
		return false;
	if(objetos[y][x] == 4 && regalo)
		return false;
	if(rey[y][x] == 1 && !regalo)
		return false;
	else if(mapa[y][x] == 'S' || mapa[y][x] == 'T' || mapa[y][x] == 'K')
		return true;
	else if(mapa[y][x] == 'B' && zapatillas)
		return true;
	else if(mapa[y][x] == 'A' && bikini)
		return true;
	return false;
}

bool ComportamientoJugador::hayObstaculos(int i, int j, Sensores sensores){
	if(mapaObjetos[i][j] == 0 || mapaObjetos[i][j] == 1 || mapaObjetos[i][j] == 2 || mapaObjetos[i][j] == 3)
		return true;
	if(mapaObjetos[i][j] == 4 && regalo)
		return true;
	if(mapaResultado[i][j] == 'P' || mapaResultado[i][j] == 'M')
		return true;
	else if(mapaResultado[i][j] == 'A' && !bikini)
		return true;
	else if(mapaResultado[i][j] == 'B' && !zapatillas)
		return true;
	else if(mapaResultado[i][j] == 'D' && !llave)
		return true;
	else if(mapaRey[i][j] == 1 && !regalo)
		return true;
	return false;
}

bool ComportamientoJugador::hayObstaculosAuxiliar(int i, int j){
	if(mapaObjetosAuxiliar[i][j] == 0 || mapaObjetosAuxiliar[i][j] == 1 || mapaObjetosAuxiliar[i][j] == 2 ||
	   mapaObjetosAuxiliar[i][j] == 3)
		return true;
	if(mapaObjetosAuxiliar[i][j] == 4 && regalo)
		return true;
	if(mapaAuxiliar[i][j] == 'P' || mapaAuxiliar[i][j] == 'M')
		return true;
	else if(mapaAuxiliar[i][j] == 'A' && !bikini)
		return true;
	else if(mapaAuxiliar[i][j] == 'B' && !zapatillas)
		return true;
	else if(mapaAuxiliar[i][j] == 'D' && !llave)
		return true;
	else if(mapaReyAuxiliar[i][j] == 1 && !regalo)
		return true;
	return false;
}

void ComportamientoJugador::actualizarPosAmarilla(int i, int j){
	switch (orientacion){
		case NORTE: posAmarilla.y = posRelativa.y-i;
				posAmarilla.x = posRelativa.x-i-i*i+j;
				break;
		case SUR:   posAmarilla.y = posRelativa.y+i;
				posAmarilla.x = posRelativa.x+i+i*i-j;
				break;
		case ESTE:  posAmarilla.y = posRelativa.y-i-i*i+j;
				posAmarilla.x = posRelativa.x+i;
				break;
		case OESTE: posAmarilla.y = posRelativa.y+i+i*i-j;
				posAmarilla.x = posRelativa.x-i;
				break;
	}
}

void ComportamientoJugador::rellenarCeldaAuxiliar(Sensores sensores, int i, int j){
	switch(orientacion){
		case NORTE: mapaAuxiliar[posRelativa.y-i][posRelativa.x-i-i*i+j] = sensores.terreno[j]; break;
		case SUR:   mapaAuxiliar[posRelativa.y+i][posRelativa.x+i+i*i-j] = sensores.terreno[j]; break;
		case ESTE:  mapaAuxiliar[posRelativa.y-i-i*i+j][posRelativa.x+i] = sensores.terreno[j]; break;
		case OESTE:   mapaAuxiliar[posRelativa.y+i+i*i-j][posRelativa.x-i] = sensores.terreno[j]; break;
	}

	if(sensores.superficie[j] == 'r'){
		switch(orientacion){
			case NORTE: mapaReyAuxiliar[posRelativa.y-i][posRelativa.x-i-i*i+j] = 1; break;
			case SUR:   mapaReyAuxiliar[posRelativa.y+i][posRelativa.x+i+i*i-j] = 1; break;
			case ESTE:  mapaReyAuxiliar[posRelativa.y-i-i*i+j][posRelativa.x+i] = 1; break;
			case OESTE: mapaReyAuxiliar[posRelativa.y+i+i*i-j][posRelativa.x-i] = 1; break;			
		}
	}

	switch(orientacion){
		case NORTE: mapaObjetosAuxiliar[posRelativa.y-i][posRelativa.x-i-i*i+j] = sensores.superficie[j]-'0'; break;
		case SUR:   mapaObjetosAuxiliar[posRelativa.y+i][posRelativa.x+i+i*i-j] = sensores.superficie[j]-'0'; break;
		case ESTE:  mapaObjetosAuxiliar[posRelativa.y-i-i*i+j][posRelativa.x+i] = sensores.superficie[j]-'0'; break;
		case OESTE: mapaObjetosAuxiliar[posRelativa.y+i+i*i-j][posRelativa.x-i] = sensores.superficie[j]-'0'; break;			
	}	
	
	if(sensores.terreno[j] == 'K' && i < 2)
		actualizarPosAmarilla(i,j);	
}

void ComportamientoJugador::rellenarMapaAuxiliar(Sensores sensores){	
	for(int i = 0; i < 4; i++){
		int n = i*i+i*2+1;
		for(int j = i*i; j < n; j++)
			rellenarCeldaAuxiliar(sensores, i, j);
	}

	if(accion_anterior == actFORWARD && !sensores.colision){
		mapaColinaAuxiliar[posRelativa.y][posRelativa.x]++;
	
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				mapaColinaAuxiliar[posRelativa.y-1+i][posRelativa.x-1+j]++;
	}
}

void ComportamientoJugador::rellenarCeldaResultado(Sensores sensores, int i, int j){
	switch(orientacion){
		case NORTE: mapaResultado[posicion.y-i][posicion.x-i-i*i+j] = sensores.terreno[j]; break;
		case SUR:   mapaResultado[posicion.y+i][posicion.x+i+i*i-j] = sensores.terreno[j]; break;
		case ESTE:  mapaResultado[posicion.y-i-i*i+j][posicion.x+i] = sensores.terreno[j]; break;
		case OESTE: mapaResultado[posicion.y+i+i*i-j][posicion.x-i] = sensores.terreno[j]; break;
	}

	if(sensores.superficie[j] == 'r'){
		switch(orientacion){
			case NORTE: mapaRey[posicion.y-i][posicion.x-i-i*i+j] = 1; break;
			case SUR:   mapaRey[posicion.y+i][posicion.x+i+i*i-j] = 1; break;
			case ESTE:  mapaRey[posicion.y-i-i*i+j][posicion.x+i] = 1; break;
			case OESTE:   mapaRey[posicion.y+i+i*i-j][posicion.x-i] = 1; break;			
		}
	}

	switch(orientacion){
		case NORTE: mapaObjetos[posicion.y-i][posicion.x-i-i*i+j] = sensores.superficie[j]-'0'; break;
		case SUR:   mapaObjetos[posicion.y+i][posicion.x+i+i*i-j] = sensores.superficie[j]-'0'; break;
		case ESTE:  mapaObjetos[posicion.y-i-i*i+j][posicion.x+i] = sensores.superficie[j]-'0'; break;
		case OESTE: mapaObjetos[posicion.y+i+i*i-j][posicion.x-i] = sensores.superficie[j]-'0'; break;			
	}	
}

void ComportamientoJugador::rellenarMapaResultado(Sensores sensores){	
	for(int i = 0; i < 4; i++){
		int n = i*i+i*2+1;
		for(int j = i*i; j < n; j++)
			rellenarCeldaResultado(sensores, i, j);
	}

	if(accion_anterior == actFORWARD && !sensores.colision){
		mapaColina[posicion.y][posicion.x]++;

		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
			    mapaColina[posicion.y-1+i][posicion.x-1+j]++;
	}
}


void ComportamientoJugador::initMapaOriginal(){

	int dif_y = posRelativa.y - posicion.y;
	int dif_x = posRelativa.x - posicion.x;
	for(int i = 0; i < tam_mapa; i++){
		for(int j = 0; j < tam_mapa; j++){
			if(mapaAuxiliar[i+dif_y][j+dif_x] != '?'){
				mapaResultado[i][j] = mapaAuxiliar[i+dif_y][j+dif_x];
				mapaColina[i][j] += mapaColinaAuxiliar[i+dif_y][j+dif_x];
				
			}
			if(mapaReyAuxiliar[i+dif_y][j+dif_x] == 1)
				mapaRey[i][j] = mapaReyAuxiliar[i+dif_y][j+dif_x];
			if(mapaObjetosAuxiliar[i+dif_y][j+dif_x]>=0 && mapaObjetosAuxiliar[i+dif_y][j+dif_x] < 5)
				mapaObjetos[i][j] = mapaObjetosAuxiliar[i+dif_y][j+dif_x];
		}
	}
}

Action ComportamientoJugador::destino(int dir){
	int rot = (dir-orientacion+4)%4;

	switch (rot){
		case 0: return actFORWARD; break;
		case 1: return actTURN_R;  break;
		case 2: return actTURN_L;  break;
		case 3: return actTURN_L;  break;	
	}
}

Action ComportamientoJugador::DecidirDireccionAuxiliar(Sensores sensores){
	int min = 9999999;
	int dir = -1;
	int i = posRelativa.y;
	int j = posRelativa.x;
	int inc = 1;

	if(mapaColinaAuxiliar[i-inc][j] <= min && !hayObstaculosAuxiliar(i-1, j)){
		if(orientacion == NORTE || mapaColinaAuxiliar[i-inc][j] < min){
			min = mapaColinaAuxiliar[i-inc][j];
			dir = NORTE;
		}
	}
	if(mapaColinaAuxiliar[i][j+inc] <= min && !hayObstaculosAuxiliar(i, j+1)){
		if(orientacion == ESTE || mapaColinaAuxiliar[i][j+inc] < min){
			min = mapaColinaAuxiliar[i][j+inc];
			dir = ESTE;
		}
	}
	if(mapaColinaAuxiliar[i+inc][j] <= min && !hayObstaculosAuxiliar(i+1, j)){
		if(orientacion == SUR || mapaColinaAuxiliar[i+inc][j] < min){
			min = mapaColinaAuxiliar[i+inc][j];
			dir = SUR;
		}
	}
	if(mapaColinaAuxiliar[i][j-inc] <= min && !hayObstaculosAuxiliar(i, j-1)){
		if(orientacion == OESTE || mapaColinaAuxiliar[i][j-inc] < min){
			min = mapaColinaAuxiliar[i][j-inc];
			dir = OESTE;
		}
	}
	
	if(sensores.superficie[2] == '0' || sensores.superficie[2] == '1' || 
	   sensores.superficie[2] == '2' || sensores.superficie[2] == '3'){
		switch (orientacion){
			case 0: mapaColinaAuxiliar[i-1][j] += 4; break;
			case 1: mapaColinaAuxiliar[i][j+1] += 4; break;
			case 2: mapaColinaAuxiliar[i+1][j] += 4; break;
			case 3: mapaColinaAuxiliar[i][j-1] += 4; break;				
		}
	}

	return destino(dir);
	
}

Action ComportamientoJugador::DecidirDireccion(Sensores sensores){
	int min = 9999999;
	int dir = -1;
	int i = posicion.y;
	int j = posicion.x;
	int inc = 1;

	if(mapaColina[i-inc][j] <= min && !hayObstaculos(i-1, j, sensores)){
		if(orientacion == NORTE || mapaColina[i-inc][j] < min){
			min = mapaColina[i-inc][j];
			dir = NORTE;
		}
	}
	if(mapaColina[i][j+inc] <= min && !hayObstaculos(i, j+1, sensores)){
		if(orientacion == ESTE || mapaColina[i][j+inc] < min){
			min = mapaColina[i][j+inc];
			dir = ESTE;
		}
	}
	if(mapaColina[i+inc][j] <= min && !hayObstaculos(i+1, j, sensores)){
		if(orientacion == SUR || mapaColina[i+inc][j] < min){
			min = mapaColina[i+inc][j];
			dir = SUR;
		}
	}
	if(mapaColina[i][j-inc] <= min && !hayObstaculos(i, j-1, sensores)){
		if(orientacion == OESTE || mapaColina[i][j-inc] < min){
			min = mapaColina[i][j-inc];
			dir = OESTE;
		}
	}

	if(sensores.superficie[2] == '0' || sensores.superficie[2] == '1' || 
	   sensores.superficie[2] == '2' || sensores.superficie[2] == '3'){
		switch (orientacion){
			case 0: mapaColina[i-1][j] += 4; break;
			case 1: mapaColina[i][j+1] += 4; break;
			case 2: mapaColina[i+1][j] += 4; break;
			case 3: mapaColina[i][j-1] += 4; break;	
		}
	}
	
	return destino(dir);
}

void ComportamientoJugador::actualizarPosRelativa(int avance){
	switch (orientacion){
		case NORTE: posRelativa.y-=avance; break;
		case SUR:   posRelativa.y+=avance; break;
		case ESTE:  posRelativa.x+=avance; break;
		case OESTE: posRelativa.x-=avance; break;
	}
}

void ComportamientoJugador::actualizarPosicion(int avance){
	switch (orientacion){
		case NORTE: posicion.y-=avance; break;
		case SUR:   posicion.y+=avance; break;
		case ESTE:  posicion.x+=avance; break;
		case OESTE: posicion.x-=avance; break;
	}
}

void ComportamientoJugador::reset(){	
	orientacion = NORTE;

	camino = regalo = bikini = zapatillas = llave = false;

	y_regalo = x_regalo = 199;
	esperar = 0;
	posicion.x = posicion.y = -1;
	posRelativa.x = posRelativa.y = tam_mapa;
	posAmarilla.x = posAmarilla.y = -1;

	for(int i = 0; i < tam_mapa*2; i++)
		for(int j = 0; j < tam_mapa*2; j++)
			mapaAuxiliar[i][j] = '?';

	for(int i = 0; i < tam_mapa*2; i++)
		for(int j = 0; j < tam_mapa*2; j++)
			mapaColinaAuxiliar[i][j] = 0;

	for(int i = 0; i < 100; i++)
		for(int j = 0; j < 100; j++)
			if(mapaColina[i][j] < -500)
				mapaColina[i][j]+=1000;
	
	num_vida++;
	
	ny = nx = -1;
}

void ComportamientoJugador::rellenarMapa(){	
	int n = 100;

	for(int i = 0; i < 100; i++)
		if(mapaResultado[i][15] == '?')
			n--;

	if(n > 50) n = 100;
	else if(n > 30) n = 50;
	else n = 30;

	for(int i = 0; i < tam_mapa; i++){
		for(int j = 0; j < 3; j++){
			mapaResultado[j][i] = 'P';
			mapaResultado[n-1-j][i] = 'P';
			mapaResultado[i][j] = 'P';
			mapaResultado[i][n-1-j] = 'P';
		}			
	}

	multiset<int> set;
	char l;

	for(int i = 0; i < 100; i++)
		for(int j = 0; j < 100; j++)
			if(mapaResultado[i][j] == '?'){
				if(j > 0){
					set.insert(mapaResultado[i][j-1]);
					if(i > 0)  set.insert(mapaResultado[i-1][j-1]);
					if(i < 99) set.insert(mapaResultado[i+1][j-1]);
				}
				if(j < 99){
					set.insert(mapaResultado[i][j+1]);
					if(i > 0)  set.insert(mapaResultado[i-1][j+1]);
					if(i < 99) set.insert(mapaResultado[i+1][j+1]);
				}
				if(i > 0) set.insert(mapaResultado[i-1][j]);
				if(i < 99) set.insert(mapaResultado[i+1][j]);

				l = 'x';
				if(set.count(l) < set.count('T')) l = 'T';
				if(set.count(l) < set.count('S')) l = 'S';
				if(set.count(l) < set.count('B')) l = 'B';
				if(set.count(l) < set.count('A')) l = 'A';				

				mapaResultado[i][j] = l;
				set.clear();
			}
}

inline int ComportamientoJugador::valor_absoluto(int n){
	if(n >= 0) return n;
	else	     return -n;
}

int ComportamientoJugador::CalcularDistancia(int y1, int x1, int y2, int x2){
	return valor_absoluto(y1-y2) + valor_absoluto(x1-x2);
}

list<nodo>::iterator ComportamientoJugador::hijoRepetidoAbiertos(list<nodo> &abiertos, nodo padre, int i, int j){
	list<nodo>::iterator it;

	for(it = abiertos.begin(); it != abiertos.end(); it++)
		if((*it).x == padre.x+j && (*it).y == padre.y+i)
			return it;

	return abiertos.end();			
}

list<nodo>::iterator ComportamientoJugador::hijoRepetidoCerrados(list<nodo> &cerrados, nodo padre, int i, int j){
	list<nodo>::iterator it;

	for(it = cerrados.begin(); it != cerrados.end(); it++)
		if((*it).x == padre.x+j && (*it).y == padre.y+i)
			return it;
	
	return cerrados.end();			
}



void ComportamientoJugador::ElaborarCamino(int y, int x, int posy, int posx, int objetos[200][200], 
				                   vector<vector<unsigned char> > mapa, int rey[200][200], int colina[200][200], int radio){
	list<nodo> abiertos, cerrados;
	list<nodo>::iterator it, mejor_nodo;
	vector<nodo> nodos;
	nodos.reserve(90000);
	int distancia = CalcularDistancia(posy, posx, y, x);

	nodo inicial(posy, posx, NULL, distancia);

	abiertos.push_back(inicial);

	int iteraciones = 0;
	
	while(!abiertos.empty()){	

		mejor_nodo = abiertos.begin();
		for(it = abiertos.begin(); it != abiertos.end(); it++){
			if((*it).distancia < (*mejor_nodo).distancia){
				mejor_nodo = it;
			}
		}
		
		abiertos.erase(mejor_nodo);
		nodo padre = *mejor_nodo;
		nodos.push_back(padre);
		cerrados.push_back(padre);

		if(padre.x == x && padre.y == y){
			nodo *nod = &padre;
			
			while(nod != NULL){
				colina[nod->y][nod->x]-=1000;
				nod = nod->padre;								
			}

			camino = true;
			return;
		}

		int dist_padre = CalcularDistancia(padre.y, padre.x, y, x);
		int _x = -1, _y = -1;

		if(padre.padre != NULL){
			_x = padre.padre->x;
			_y = padre.padre->y;
		}

		list<nodo>::iterator it1, it2;

		// Hijo Norte
		if(padre.y-1 >= 0 && valor_absoluto(padre.y-1-posy) <= radio){			
			it2 = hijoRepetidoCerrados(cerrados, padre, -1, 0);

			if(porAhiSePuede(padre.y-1, padre.x, objetos, mapa, rey) && (_x != padre.x || _y != padre.y-1) && it2 == cerrados.end()){
				int dist_hijo = CalcularDistancia(padre.y-1, padre.x, y, x);
				distancia = dist_hijo+padre.distancia-dist_padre+1;
				it1 = hijoRepetidoAbiertos(abiertos, padre, -1, 0);

				if(it1 == abiertos.end())
					abiertos.push_back(nodo(padre.y-1, padre.x, &nodos[nodos.size()-1], distancia));
				else if((*it1).distancia > distancia){
					abiertos.erase(it1);
					abiertos.push_back(nodo(padre.y-1, padre.x, &nodos[nodos.size()-1], distancia));
				}
			}			
		}
	
		// Hijo Sur
		if(padre.y+1 < 200 && valor_absoluto(padre.y+1-posy) <= radio){
			it2 = hijoRepetidoCerrados(cerrados, padre, 1, 0);

			if(porAhiSePuede(padre.y+1, padre.x, objetos, mapa, rey) && (_x != padre.x || _y != padre.y+1) && it2 == cerrados.end()){
				int dist_hijo = CalcularDistancia(padre.y+1, padre.x, y, x);
				distancia = dist_hijo+padre.distancia-dist_padre+1;	
				it1 = hijoRepetidoAbiertos(abiertos, padre, 1, 0);

				if(it1 == abiertos.end())
					abiertos.push_back(nodo(padre.y+1, padre.x, &nodos[nodos.size()-1], distancia));
				else if((*it1).distancia > distancia){
					abiertos.erase(it1);
					abiertos.push_back(nodo(padre.y+1, padre.x, &nodos[nodos.size()-1], distancia));
				}
			}
		}

		// Hijo Oeste
		if(padre.x-1 >= 0 && valor_absoluto(padre.x-1-posx) <= radio){
			it2 = hijoRepetidoCerrados(cerrados, padre, 0, -1);

			if(porAhiSePuede(padre.y, padre.x-1, objetos, mapa, rey) && (_x != padre.x-1 || _y != padre.y) && it2 == cerrados.end()){
				int dist_hijo = CalcularDistancia(padre.y, padre.x-1, y, x);
				distancia = dist_hijo+padre.distancia-dist_padre+1;
				it1 = hijoRepetidoAbiertos(abiertos, padre, 0, -1);
		
				if(it1 == abiertos.end())
					abiertos.push_back(nodo(padre.y, padre.x-1, &nodos[nodos.size()-1], distancia));
				else if((*it1).distancia > distancia){
					abiertos.erase(it1);
					abiertos.push_back(nodo(padre.y, padre.x-1, &nodos[nodos.size()-1], distancia));
				}
			}			
		}
		
		// Hijo Este
		if(padre.x+1 < 200 && valor_absoluto(padre.x+1-posx) <= radio){
			it2 = hijoRepetidoCerrados(cerrados, padre, 0, 1);

			if(porAhiSePuede(padre.y, padre.x+1, objetos, mapa, rey) && (_x != padre.x+1 || _y != padre.y) && it2 == cerrados.end()){
				int dist_hijo = CalcularDistancia(padre.y, padre.x+1, y, x);
				distancia = dist_hijo+padre.distancia-dist_padre+1;
				it1 = hijoRepetidoAbiertos(abiertos, padre, 0, 1);

				if(it1 == abiertos.end())
					abiertos.push_back(nodo(padre.y, padre.x+1, &nodos[nodos.size()-1], distancia));
				else if((*it1).distancia > distancia){
					abiertos.erase(it1);
					abiertos.push_back(nodo(padre.y, padre.x+1, &nodos[nodos.size()-1], distancia));
				}
			}			
		}
		
	}
}

void ComportamientoJugador::BuscarRey(int posy, int posx, int objetos[200][200], vector<vector<unsigned char> > mapa, 
						  int rey[200][200], int colina[200][200], Sensores sensores){
	bool seguir = true;
	
	int radio = 20;

	for(int i = -radio; i <= radio && seguir; i++)
		for(int j = -radio; j <= radio && seguir; j++)
			if(posy+i >= 0 && posy+i < 200 && posx+j >= 0 && posx+j < 200){

				if(rey[posy+i][posx+j] == 1){
					int desty = posy+i, destx = posx+j;
					bool found = false;
 
					if(desty-1 >= 0)
						if(!hayObstaculos(desty-1, destx, sensores)){
							desty = desty-1;
							found = true;
						}
					if(desty+1 < 200 && !found)
						if(!hayObstaculos(desty+1, destx, sensores)){
							desty = desty+1;
							found = true;
						}
					if(destx-1 >= 0 && !found)
						if(!hayObstaculos(desty, destx-1, sensores)){
							destx = destx-1;
							found = true;	
						}
					if(!found ) destx = destx+1;

					ElaborarCamino(desty, destx, posy, posx, objetos, mapa, rey, colina, radio);

					if(camino){
						colina[posy+i][posx+j]-=1000;
						seguir = false;
						esperar = 0;
					}
				}
			}
	if(!camino) esperar++;
}

void ComportamientoJugador::BuscarRegalo(int posy, int posx, int objetos[200][200], vector<vector<unsigned char> > mapa, 
						  int rey[200][200], int colina[200][200], Sensores sensores){
	bool seguir = true;

	int radio = 25;

	for(int i = 0; i < sensores.regalos.size(); i++){
		if(valor_absoluto(posy-sensores.regalos[i].first) <= radio && valor_absoluto(posx-sensores.regalos[i].second) <= radio && 
			mapaResultado[sensores.regalos[i].first][sensores.regalos[i].second] != '?'){

			int desty = sensores.regalos[i].first, destx = sensores.regalos[i].second;
			bool found = false;
 
			if(desty-1 >= 0)
				if(!hayObstaculos(desty-1, destx, sensores)){
					desty = desty-1;
					found = true;
				}
			if(desty+1 < 200 && !found)
				if(!hayObstaculos(desty+1, destx, sensores)){
					desty = desty+1;
					found = true;
				}
			if(destx-1 >= 0 && !found)
				if(!hayObstaculos(desty, destx-1, sensores)){
					destx = destx-1;
					found = true;	
				}
			if(!found ) destx = destx+1;

			ElaborarCamino(desty, destx, posy, posx, objetos, mapa, rey, colina, radio);
								
			if(camino){
				y_regalo = sensores.regalos[i].first;
				x_regalo = sensores.regalos[i].second;
				colina[y_regalo][x_regalo]-=1000;
				esperar = 0;
				seguir = false;
			}
		}
	}
	if(!camino) esperar++;
}

void ComportamientoJugador::BuscarNoExplorado(int posy, int posx, int objetos[200][200], vector<vector<unsigned char> > mapa, 
						  int rey[200][200], int colina[200][200], Sensores sensores){

	int radio = 10;

	int y_mas_cerca = 90000, x_mas_cerca = 90000;

	for(int i = -radio; i <= radio; i++)
		for(int j = -radio; j <= radio; j++)
			if(posy+i >= 0 && posy+i < 100 && posx+j >= 0 && posx+j < 100){

				if(mapa[posy+i][posx+j] == '?' && !hayObstaculos(posy+i, posx+j, sensores)){

				   if(CalcularDistancia(y_mas_cerca, x_mas_cerca, posy, posx) > CalcularDistancia(posy+i, posx+j, posy, posx)){
					y_mas_cerca = posy+i;
					x_mas_cerca = posx+j;
				   }
					
				}
			}
	if(y_mas_cerca != 90000){
		int desty = y_mas_cerca, destx = x_mas_cerca;
	
		ElaborarCamino(desty, destx, posy, posx, objetos, mapa, rey, colina, radio);

		if(camino){
			esperar = 0;
			ny = desty;
			nx = destx;
		}
		if(!camino) esperar++;
	}
}


void ComportamientoJugador::BuscarAmarillo(int posy, int posx, int objetos[200][200], vector<vector<unsigned char> > mapa, 
						  int rey[200][200], int colina[200][200], Sensores sensores){
	bool seguir = true;
	
	int radio = 10;

	for(int i = -radio; i <= radio && seguir; i++)
		for(int j = -radio; j <= radio && seguir; j++)
			if(posy+i >= 0 && posy+i < 200 && posx+j >= 0 && posx+j < 200){
				if(mapa[posy+i][posx+j] == 'K'){
					int desty = posy+i, destx = posx+j;

					ElaborarCamino(desty, destx, posy, posx, objetos, mapa, rey, colina, radio);
					
					if(camino) seguir = false;
				}
			}

}

Action ComportamientoJugador::think(Sensores sensores){
	
	Action accion;
	pasos++;

	if(pasos%2000 == 0) cout << "Pasos: " << pasos << endl;

	if(pasos == 19999) 
	   rellenarMapa();

	if(sensores.reset) 
	   reset();

	if(sensores.colision){
		if(posicion.x == -1)
			actualizarPosRelativa(-1);
		else
			actualizarPosicion(-1);
	}

	// Rellenar mapas
	if(posicion.x == -1)
		rellenarMapaAuxiliar(sensores);
	else 
		rellenarMapaResultado(sensores);

	// Tras encontrar punto amarillo actualizar mapa original
	if(posicion.x == -1 && sensores.terreno[0] == 'K'){
		for(int i = 0; i < 200; i++)
			for(int j = 0; j < 200; j++)
				if(mapaColinaAuxiliar[i][j] < -500)
					mapaColinaAuxiliar[i][j]+=1000;	

		camino = false;
		posicion.x = sensores.mensajeC;
		posicion.y = sensores.mensajeF;
		initMapaOriginal();
	}
	
	if(esperar != 0) esperar++;

	if(posicion.x == -1 && !camino)
		BuscarAmarillo(posRelativa.y, posRelativa.x, mapaObjetosAuxiliar, mapaAuxiliar, mapaReyAuxiliar, mapaColinaAuxiliar, sensores);

	if(!regalo && !camino && posicion.x != -1 && esperar%10 == 0)
		BuscarRegalo(posicion.y, posicion.x, mapaObjetos, mapaResultado, mapaRey, mapaColina, sensores);
	else if(!regalo && !camino && posicion.x == -1 && esperar%10 == 0)
		BuscarRegalo(posicion.y, posicion.x, mapaObjetosAuxiliar, mapaAuxiliar, mapaReyAuxiliar, mapaColinaAuxiliar, sensores);

	if(regalo && !camino && posicion.x != -1 && esperar%10 == 0)
		BuscarRey(posicion.y, posicion.x, mapaObjetos, mapaResultado, mapaRey, mapaColina, sensores);
	else if(regalo && !camino && posicion.x == -1 && esperar%10 == 0)
		BuscarRey(posicion.y, posicion.x, mapaObjetosAuxiliar, mapaAuxiliar, mapaReyAuxiliar, mapaColinaAuxiliar, sensores);

	/*if(posicion.x != -1 && !camino && nx == -1 && pasos%5==0)
		BuscarNoExplorado(posicion.y, posicion.x, mapaObjetos, mapaResultado, mapaRey, mapaColina, sensores);

	if(nx != -1 && nx == posicion.x && ny == posicion.y && posicion.x != -1){
		camino = false;
		nx = ny = -1;
	}*/
		

	if(camino && posicion.x != -1){
		if(mapaColina[posicion.y][posicion.x] < -500)
			mapaColina[posicion.y][posicion.x]+=1000;
	}
	else if(camino && mapaColinaAuxiliar[posRelativa.y][posRelativa.x] < -500)
			mapaColinaAuxiliar[posRelativa.y][posRelativa.x]+=1000;
	

	if(sensores.superficie[2] == 'r' && camino && regalo){
		if(sensores.objetoActivo != '4'){
			if(puedoMeterObjeto(sensores))
				accion = actPUSH;
			else 
				accion = actPOP;
		}
		else{
			accion = actGIVE;
			regalo = camino = false;
			for(int i = 0; i < 100; i++)
			    for(int j = 0; j < 100; j++)
				  if(mapaColina[i][j] < -500)
				     mapaColina[i][j]+=1000;
		}
	}
	else if(sensores.terreno[2] == 'A' && bikini && sensores.objetoActivo != '1' && sensores.terreno[0] != 'B'){
		if(puedoMeterObjeto(sensores))
			accion = actPUSH;
		else 
			accion = actPOP;
	}
	else if(sensores.terreno[2] == 'B' && zapatillas && sensores.objetoActivo != '2' && sensores.terreno[0] != 'A'){
		if(puedoMeterObjeto(sensores))
			accion = actPUSH;
		else 
			accion = actPOP;
	}
	else if(accion_anterior == actGIVE)
		accion = actFORWARD;
	else if(sensores.terreno[2] == 'D' && sensores.objetoActivo == '3'){
		accion = actGIVE;
		llave = true;
	}
	else if(sensores.terreno[2] == 'D' && llave){
		if(puedoMeterObjeto(sensores))
			accion = actPUSH;
		else 
			accion = actPOP;
	}
	else if(puedoCogerObjeto(sensores))
		accion = actPICKUP;
	else if(puedoMeterObjeto(sensores) && (sensores.superficie[2] == '0' || sensores.superficie[2] == '1' ||
		  sensores.superficie[2] == '2' || sensores.superficie[2] == '3' || sensores.superficie[2] == '4'))
		accion = actPUSH;
	else if(sensores.superficie[2] == 'a' || sensores.superficie[2] == 'l')
		accion = actIDLE;
	else if(posicion.x == -1)
		accion = DecidirDireccionAuxiliar(sensores);
	else
		accion = DecidirDireccion(sensores);


	if(accion == actTURN_R && accion_anterior == actTURN_L)
		accion = actTURN_L;
	else if(accion == actTURN_L && accion_anterior == actTURN_R)
		accion = actTURN_R;

	accion_anterior = accion;
	
	// Actualizar posición y orientación
	if(accion == actTURN_L)
		orientacion = (orientacion-1+4)%4;
	else if(accion == actTURN_R)
		orientacion = (orientacion+1)%4;
	else if(accion == actFORWARD && posicion.x != -1)
		actualizarPosicion(1);
	else if(accion == actFORWARD)
		actualizarPosRelativa(1);

	
	/*
	cout << "Pasos: " << pasos << endl;
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Mochila: " << sensores.mochila << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << "Fila: " << sensores.mensajeF << endl;
	cout << "Columna: " << sensores.mensajeC << endl;
	cout << "objetoActivo: " << sensores.objetoActivo << endl;
	cout << endl;*/

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

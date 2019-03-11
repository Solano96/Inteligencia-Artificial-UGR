#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <set>
#define NORTE 0
#define ESTE 1
#define SUR 2
#define OESTE 3

using namespace std;

struct Coord{
	int x, y;
};

struct nodo{
	int x,y;
	int distancia;
	nodo *padre;

	nodo(int _y, int _x, nodo *_padre, int d){
		y = _y;
		x = _x; 
		padre = _padre;
		distancia = d;	
	}
};	

class ComportamientoJugador : public Comportamiento{

   public:

	ComportamientoJugador(unsigned int size) : Comportamiento(size){
		// Inicializar Variables de Estado
		esperar = 0;
		x_regalo = 199;
		y_regalo = 199;	
		mision = 1;
		orientacion = NORTE;

		bikini = zapatillas = llave = regalo = false;

		tam_mapa = size;

		posicion.x = posicion.y = -1;
		posRelativa.x = posRelativa.y = size;
		posAmarilla.x = posAmarilla.y = -1;

		max = 15;	
		num_vida = 1;

		
		for(int i = 0; i < size*2; i++){
			vector<unsigned char> aux;
			for(int j = 0; j < size*2; j++){
				aux.push_back('?');
			}
			mapaAuxiliar.push_back(aux);
		}

		for(int i = 0; i < size*2; i++)
			for(int j = 0; j < size*2; j++)
				mapaColina[i][j] = 0;

		for(int i = 0; i < size*2; i++)
			for(int j = 0; j < size*2; j++){
				mapaColinaAuxiliar[i][j] = 0;
				mapaReyAuxiliar[i][j] = 0;
				mapaReyAuxiliar[i][j] = 0;
				mapaObjetosAuxiliar[i][j] = -1;
				mapaObjetos[i][j] = -1;
			}

		pasos = 0;

		camino = false;

		ny = nx = -1;
		
	}
	
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);


    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado

	int mision;
	int esperar;

	int ny, nx;

	int x_regalo, y_regalo;

	int tam_mapa;
	int max;
	
	int num_vida;

	int pasos;

	Action accion_anterior;
	int buscaAmarillo;

	Coord posicion;
	Coord posRelativa;
	Coord posAmarilla;
	
	int orientacion;
	
	vector<vector<unsigned char> > mapaAuxiliar;
	int mapaColina[200][200];
	int mapaColinaAuxiliar[200][200];
	int mapaReyAuxiliar[200][200];
	int mapaRey[200][200];
	int mapaObjetosAuxiliar[200][200];
	int mapaObjetos[200][200];

	bool bikini, zapatillas, llave, regalo;
	bool camino;

	bool puedoCogerObjeto(Sensores sensores);
	bool puedoMeterObjeto(Sensores sensores);
	void actualizarPosAmarilla(int i, int j);
	void rellenarCeldaAuxiliar(Sensores sensores, int i, int j);
	void rellenarMapaAuxiliar(Sensores sensores);
	void rellenarCeldaResultado(Sensores sensores, int i, int j);
	void rellenarMapaResultado(Sensores sensores);
	void initMapaOriginal();
	Action destino(int dir);
	Action DecidirDireccionAuxiliar(Sensores sensores);
	Action DecidirDireccion(Sensores sensores);
	bool hayObstaculos(int i, int j, Sensores sensores);
	bool hayObstaculosAuxiliar(int i, int j);
	bool puedePasar(Sensores sensores);
	void actualizarPosRelativa(int avance);
	void actualizarPosicion(int avance);
	void reset();
	void rellenarMapa();
	int valor_absoluto(int n);
	int CalcularDistancia(int y1, int x1, int y2, int x2);
	list<nodo>::iterator hijoRepetidoAbiertos(list<nodo> &abiertos, nodo padre, int i, int j);	
	list<nodo>::iterator hijoRepetidoCerrados(list<nodo> &cerrados, nodo padre, int i, int j);
	void ElaborarCamino(int y, int x, int posy, int posx, int objetos[200][200], vector<vector<unsigned char> > mapa, int rey[200][200], int colina[200][200], int radio);
	void BuscarRey(int posy, int posx, int objetos[200][200], vector<vector<unsigned char> > mapa, int rey[200][200], int colina[200][200], Sensores sensores);
	void BuscarRegalo(int posy, int posx, int objetos[200][200], vector<vector<unsigned char> > mapa, int rey[200][200], int colina[200][200], Sensores sensores);
	void BuscarAmarillo(int posy, int posx, int objetos[200][200], vector<vector<unsigned char> > mapa, int rey[200][200], int colina[200][200], Sensores sensores);
	void BuscarNoExplorado(int posy, int posx, int objetos[200][200], vector<vector<unsigned char> > mapa, int rey[200][200], int colina[200][200], Sensores sensores);
	bool porAhiSePuede(int y, int x, int objetos[200][200], vector<vector<unsigned char> > mapa, int rey[200][200]);
};


#endif

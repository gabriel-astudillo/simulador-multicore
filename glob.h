#ifndef GLOBAL_H
#define GLOBAL_H

#include <cppsim.hh>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <set>
#include <vector>
#include <list>
#include <assert.h>
#include <fstream>
#include <string>
#include <limits>
#include <queue>
#include <math.h>
#include <algorithm>
#include <unistd.h>
#include <random>
#include <chrono>

#define TIME_WIDTH 12


typedef enum {L1, L2} t_memoria;
typedef enum {DATA_OK, DATA_FAIL} t_dataStatus;

typedef enum {REG_VERBOSE, REG_QUIET} t_register;

/*
*	Variables globales para estadisticas
*/
extern counter*   g_tareasFinalizadas;
extern mean*      g_tiempoEsperaReady;   //tiempo de espera en fila ready (ingreso al procesador)
extern mean*      g_tiempoServicio;      //tiempo de servicio (inicio->fin de la ejecución en un core)
extern mean*      g_tiempoEsperaMemoria; //tiempo de espera debido a transferencia de datos entre memorias
extern statistic* g_tput;                //throughput

extern histogram* g_hist_tiempoServicio;
extern histogram* g_hist_tiempoEsperaReady;

extern double TR_L2_L1;
extern double TR_RAM_L2;

extern uint8_t g_size_L2;
extern uint8_t g_size_L1;

extern double *g_tiempoUtilizadoCore;
extern double *g_tiempoReposoCore;



class Registro {
private:
	t_register tipo;
	
public:
	Registro();
	Registro(t_register tipo);
	
	void print(double tiempo, string quien, string mensaje);
	
};

extern Registro *g_registro;

#endif


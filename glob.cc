#include "glob.h"
#include "check_args.h"

/*
*	Clase Registro
*/
Registro::Registro(){
	tipo = REG_VERBOSE;
}

Registro::Registro(t_register _tipo){
	tipo = _tipo;
}

void Registro::print(double tiempo, string quien, string mensaje){
	if( tipo == REG_VERBOSE){
		std::cout << std::setw(TIME_WIDTH) << std::fixed << tiempo << 
			": " << quien << ": " << mensaje << std::endl;
	}
	
}

/*
*	Indicadores
*/
counter*   g_tareasFinalizadas   = new counter("Tareas_finalizadas");
mean*      g_tiempoEsperaReady   = new mean("Espera_en_fila_Ready", .95);
mean*      g_tiempoServicio      = new mean("Tiempo_servicio", .95);    
mean*      g_tiempoEsperaMemoria = new mean("Espera_por_fallos_en_memoria", .95);
statistic* g_tput                = new mean("Troughput", .95);;               

histogram* g_hist_tiempoServicio    = new histogram("Tiempo Servicio", 0.0, 100.0, 20);
histogram* g_hist_tiempoEsperaReady = new histogram("Tiempo Espera Ready", 0.0, 100.0, 20);

/*
*	Tiempos de transferencia
*	entre las distintas memorias
*/
double TR_L2_L1;
double TR_RAM_L2;

/*
*	Tamaños L1 y L2
*	(Falta parametrizar con check_args)
*/
uint8_t g_size_L1;// = 20;
uint8_t g_size_L2;// = 40;



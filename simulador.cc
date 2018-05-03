#include "glob.h"
#include "check_args.h"

#include "generadorTareas.h"
#include "procesador.h"

Registro* g_registro;

double *g_tiempoUtilizadoCore;
double *g_tiempoReposoCore;

class sistema: public process {
	
private:
	double simLen;
	uint32_t totalTareas;
	uint32_t  totalCores;
	double tasaLlegada;
	
	handle<generadorTareas> generador;
	handle<Procesador>      procesador;

public:
	sistema( const string& _name, double _sl, uint32_t _totalTareas, uint32_t  _totalCores, double _tasaLlegada) : process( _name ) {
			 
		simLen      = _sl;
		totalTareas = _totalTareas;
		tasaLlegada = _tasaLlegada;
		
		totalCores  = _totalCores;
	
	}

protected:
  	void inner_body( void );
};


void sistema::inner_body( void ){
	
	generador  = new generadorTareas("Gen01", totalTareas, tasaLlegada);
	procesador = new Procesador("Proc0", totalCores);
	
	/* el procesador va asociado al generador */
	generador->asociarProcesador(procesador);
	
	generador->activate();
	procesador->activate();
	hold(simLen);
	generador->cancel();
	procesador->cancel();
	
	end_simulation();

}

int main( int argc, char* argv[] ){

	check_args(argc, argv);
	
	TR_L2_L1   = parametros.tr_L2_L1; //por omision: 0.30
	TR_RAM_L2  = parametros.tr_RAM_L2;//por omision: 0.50
	
	g_size_L1 = parametros.size_L1;   //por omision: 20
	g_size_L2 = parametros.size_L2;   //por omision: 40
	
	g_tiempoUtilizadoCore = (double*) calloc(parametros.totalCores, sizeof(double) );
	g_tiempoReposoCore    = (double*) calloc(parametros.totalCores, sizeof(double) );;
	
	std::cout << "Sistema" << std::endl;
	std::cout << "\tCores:\t\t\t" << std::to_string(parametros.totalCores) << std::endl;
	std::cout << "\tL1 size:\t\t" << std::to_string(g_size_L1) << std::endl;
	std::cout << "\tL2 size:\t\t" << std::to_string(g_size_L2) << std::endl;
	std::cout << "\tLatencia L2->L1:\t"  << TR_L2_L1 << std::endl;
	std::cout << "\tLatencia RAM->L2:\t" << TR_RAM_L2 << std::endl;
	
	std::cout << "\tTareas:\t\t\t"<< parametros.totalTareas << std::endl;	
	std::cout << "\tTasa de llegada:\t" << parametros.tasaLlegada << std::endl;	
	
	/*
	*	Objeto utilizado para realizar
	*	el log del sistema
	*/
	g_registro = new Registro(parametros.t_registro);
	
	g_tiempoServicio->reset();
	g_tiempoEsperaReady->reset();
	
	simulation::instance()->begin_simulation( new sqsDll( ) );

	handle<sistema> system = new sistema("System main", 
										1e100,                   /*Tiempo total de simulación*/
										parametros.totalTareas,  /*Cantidad de tareas        */
										parametros.totalCores,   /*Cantidad de cores         */
										parametros.tasaLlegada
										) ;

	system->activate();

	simulation::instance()->run();

	simulation::instance()->end_simulation();
	
	printf("\nEstadisticas\n");
	
	try{
		printf("%-15.15s%15s%15s\n", "", "Prom.", "desv." );
		printf("%-15.15s%15.2f%15.2f\n", "t. Servicio"    , g_tiempoServicio->m()   , sqrt(g_tiempoServicio->variance()) );
		printf("%-15.15s%15.2f%15.2f\n", "t. Espera Ready", g_tiempoEsperaReady->m(), sqrt(g_tiempoEsperaReady->variance()) );
		
		puts("");
		g_tput->report();
		puts("-----");

		//g_hist_tiempoServicio->report();
		//puts("-----");
		
		//g_hist_tiempoEsperaReady->report();
		//puts("-----");
		
		std::cout <<  "Core\tt. Utilizado\tt. Reposo\tutilizacion" << std::endl;
		for(uint32_t c=0; c<parametros.totalCores; c++){
			double utilizacion = g_tiempoUtilizadoCore[c] > 0 ? (1.0 - g_tiempoReposoCore[c]/g_tiempoUtilizadoCore[c]) : 0.0;
			printf("%4i%15f%15f%15f\n", c, g_tiempoUtilizadoCore[c], g_tiempoReposoCore[c], utilizacion);
			
			
			//std::cout << std::to_string(c) << "\t" << tiempoUtilizadoCore[c] << "\t\t\t" << tiempoReposoCore[c] << std::endl;	
		}
		puts("-----");
		
	} catch (runtime_error& e){
		std::cerr << "Error: " << e.what() <<std::endl;
	}
	

	return(EXIT_SUCCESS);
}

#include "glob.h"
#include "check_args.h"

#include "generadorTareas.h"
#include "procesador.h"

Registro* g_registro;

double *tiempoUtilizadoCore;
double *tiempoReposoCore;

class sistema: public process {
	
private:
	double simLen;
	uint32_t totalTareas;
	uint8_t  totalCores;
	double tasaLlegada;
	
	handle<generadorTareas> generador;
	handle<Procesador>      procesador;

public:
	sistema( const string& _name, double _sl, uint32_t _totalTareas, uint8_t  _totalCores, double _tasaLlegada) : process( _name ) {
			 
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
	

	//std::cout << this->time() << std::endl;
	end_simulation();

}

int main( int argc, char* argv[] ){

	check_args(argc, argv);
	
	tiempoUtilizadoCore = (double*) calloc(parametros.totalCores, sizeof(double) );
	tiempoReposoCore    = (double*) calloc(parametros.totalCores, sizeof(double) );;
	
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
	std::cout << std::endl << "done!" << std::endl;
	
	
	try{
		std::cout << "tiempo de Servicio\t" << \
		g_tiempoServicio->m() << "\t"  << \
		sqrt(g_tiempoServicio->variance()) << \
		"\n-----" << std::endl ;
		
		std::cout << "tiempo de Espera Ready\t" << \
		g_tiempoEsperaReady->m() << "\t"  << \
		sqrt(g_tiempoEsperaReady->variance()) << \
		"\n-----" << std::endl ;
		
		g_tput->report();
		puts("-----");

		//g_hist_tiempoServicio->report();
		//puts("-----");
		
		//g_hist_tiempoEsperaReady->report();
		//puts("-----");
		
		std::cout <<  "Core\tt. Utilizado\tt. Reposo" << std::endl;
		for(uint8_t c=0; c<parametros.totalCores; c++){
			double utilizacion = tiempoUtilizadoCore[c] > 0 ? (1.0 - tiempoReposoCore[c]/tiempoUtilizadoCore[c]) : 0.0;
			printf("%4i%15f%15f%15f\n", c, tiempoUtilizadoCore[c], tiempoReposoCore[c], utilizacion);
			
			
			//std::cout << std::to_string(c) << "\t" << tiempoUtilizadoCore[c] << "\t\t\t" << tiempoReposoCore[c] << std::endl;	
		}
		puts("-----");
		
	} catch (runtime_error& e){
		std::cerr << "Error: " << e.what() <<std::endl;
	}
	

	return(EXIT_SUCCESS);
}

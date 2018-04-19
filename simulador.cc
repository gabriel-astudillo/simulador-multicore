#include "glob.h"
#include "check_args.h"

#include "generadorTareas.h"
#include "procesador.h"

class sistema: public process {
	
private:
	double simLen;
	uint32_t totalTareas;
	uint8_t  totalCores;
	
	handle<generadorTareas> generador;
	handle<Procesador>      procesador;

public:
	sistema( const string& _name, double _sl, uint32_t _totalTareas, uint8_t  _totalCores) : process( _name ) {
			 
		simLen      = _sl;
		totalTareas = _totalTareas;
		
		totalCores  = _totalCores;
	
	}

protected:
  	void inner_body( void );
};


void sistema::inner_body( void ){
	
	generador  = new generadorTareas("Gen01", totalTareas);
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
	
	

	simulation::instance()->begin_simulation( new sqsDll( ) );

	handle<sistema> system = new sistema("System main", 
										1e100,                   /*Tiempo total de simulación*/
										parametros.totalTareas,  /*Cantidad de tareas        */
										parametros.totalCores    /*Cantidad de cores         */
										) ;

	system->activate();

	simulation::instance()->run();

	simulation::instance()->end_simulation();
	std::cout << std::endl << "done!" << std::endl;

	return 0;
}

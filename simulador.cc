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
		
		list< handle<coreSim> > cores;


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
	procesador = new Procesador("Proc0");
	
	for(uint8_t i=0; i < totalCores; i++){
		cores.push_back(new coreSim(string("Core") + string(std::to_string(i)) ));
	}
	
	
	/* Los cores van asociados al procesador... */
	procesador->asociarCore(cores);
	
	/* y el procesador está asociado a los cores... */
	
	std::list< handle<coreSim> >::iterator indexCore;
	indexCore = cores.begin();
	while( indexCore != cores.end() ){
		(*indexCore)->asociarProcesador(procesador);
		indexCore++;
	}
	
	
	/* ...y el procesador va asociado al generador */
	generador->asociarProcesador(procesador);
	
	generador->activate();
	
	indexCore = cores.begin();
	while( indexCore != cores.end() ){
		(*indexCore)->activate();
		indexCore++;
	}
	
	
	procesador->activate();
	hold(simLen);
	generador->cancel();
	
	indexCore = cores.begin();
	while( indexCore != cores.end() ){
		(*indexCore)->cancel();
		indexCore++;
	}
	
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

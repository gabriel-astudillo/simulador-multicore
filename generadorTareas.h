#ifndef _GENERADOR_TAREAS_H_
#define _GENERADOR_TAREAS_H_

#include "procesador.h"

class generadorTareas : public process {
private:
	
	rng<double>* tiempoArribo;
	uint32_t     totalTareas;
	string       name;
	
	//handle<coreSim>    core;
	handle<Procesador> procesador;
	
	Registro *registro;
	
protected:
	void inner_body();
	
public:
	generadorTareas(const string& _name, uint32_t totalTareas);
	
	virtual ~generadorTareas();

	
	void asociarProcesador(handle<Procesador> _procesador);
};


#endif


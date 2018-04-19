#ifndef _PROCESADOR_H_
#define _PROCESADOR_H_

#include "tarea.h"

class coreSim ;
class Procesador;

class Procesador : public process {
private:
	string name;
	list<Tarea*> filaTareas; /*Tareas a repartir entre los cores*/
	Registro *registro;
	 
	list< handle<coreSim> > cores;
	
	uint8_t L2; /*Tamaño memoria cache L2*/
	bool esperandoPorCore;

	
protected:
	void inner_body();
	
public:
	Procesador(const string& _name);
	
	virtual ~Procesador();
	
	void agregarTarea(Tarea* tarea);
	
	void asociarCore(list< handle<coreSim> > _cores);
	
	bool filaTareasEstaVacia();
	
	bool estaEsperandoPorCore();
	

};

class coreSim : public process {
private:
	list<Tarea*> filaTareas;
	uint8_t L1; /* tamaño memoria L1 */
	string name;
	Registro *registro;
	
	handle<Procesador> procesador;
	Tarea *tarea;
	
protected:
	void inner_body();
	
public:
	coreSim(const string& _name);
	
	virtual ~coreSim();
	
	void agregarTarea(Tarea* _tarea);
	
	void asociarProcesador(handle<Procesador> _procesador);
	
	bool tieneTareaAsignada();
	
	string getName();
};


#endif


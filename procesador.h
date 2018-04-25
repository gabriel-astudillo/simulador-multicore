#ifndef _PROCESADOR_H_
#define _PROCESADOR_H_

#include "tarea.h"
#include "memoria.h"

class coreSim ;
class Procesador;

class Procesador : public process {
private:
	string name;
	list<Tarea*> filaTareas; /*Tareas a repartir entre los cores*/
	Registro *registro;
	 
	list< handle<coreSim> > cores;
	
	uint8_t size_L2; /*Tamaño memoria cache L2*/
	Memoria *mem_L2;
	
	uint8_t totalCores;
	bool esperandoPorCore;
	
	void asociarCores();
	
	
protected:
	void inner_body();
	
public:
	Procesador(const string& _name, uint8_t _totalCores);	
	virtual ~Procesador();
	void agregarTarea(Tarea* tarea);
	bool filaTareasEstaVacia();
	bool estaEsperandoPorCore();
	t_dataStatus buscarDatoEnMemoria(char datoBuscar);

};

class coreSim : public process {
private:
	list<Tarea*> filaTareas;
	uint8_t size_L1; /* tamaño memoria L1 */
	
	Memoria *mem_L1;
	
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
	t_dataStatus buscarDatoEnMemoria(char datoBuscar);
	
};


#endif


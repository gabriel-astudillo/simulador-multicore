#ifndef _TAREA_H_
#define _TAREA_H_

#include <glob.h>

class Tarea {
private:
	uint32_t id;
	double tServicio;
	
	void asignarDatos();
	void agregarDato(char dato);
	
protected:
	
	
public:
	list<char> datos;
	
	Tarea(uint32_t _id);
	uint32_t getID();
	double getTservicio();
	void verDatos();
	
	
};



#endif


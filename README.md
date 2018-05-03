# simulador-multicore

Modo de uso:

./sim 
Uso: ./sim -t <nro de tareas> -c <nro de cores> -s <tasa llegada> [-l] [-L] [-m] [-M] [-v]

Descripción:
	-t   cantidad de Tareas a simular.
	-s   taSa de llegada: numero medio de tareas que llegan por unidad de tiempo.
	-c   cantidad de Cores del procesador.
	-l   Tiempo de transferencia de datos desde L2 a L1. Omision: 0.3
	-L   Tiempo de transferencia de datos desde RAM a L2. Omision: 0.5
	-m   Tamaño memoria L1. Omision: 20.
	-M   Tamaño memoria L2. Omision: 40.
	-v   Muestra en pantalla el registro de la simulacion.
  
 
 Core stuff is in `coresim/` 
---------------------------
Normally these files shouldn't change. This directory includes implementations of the following:
* Main event loop and related helper functions, global variables, main() function to determine which experiment to run: `main.cpp`
    * Note: deciding which experiment to run will eventually be moved to the `run/` directory, probably to `experiment.cpp`.
* Core event implementations (`Event`, `FlowArrivalEvent`, `FlowFinishedEvent`, etc): `event.cpp`.
* Representation of the topology: `node.cpp`, `topology.cpp`
* Queueing behavior. This is a basis for extension; the default implementation is FIFO-dropTail: `queue.cpp`.
* Flows and packets. This is also a basis for extension; default is TCP: `packet.cpp` and `flow.cpp`.
* Random variables used in flow generation. Used as a library by the flow generation code: `random_variable.cpp`.

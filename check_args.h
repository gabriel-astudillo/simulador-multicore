#ifndef _CHECK_ARGS_H_
#define _CHECK_ARGS_H_

struct args_t {
   //std::string archivoIndice;
   uint32_t totalTareas;
   uint8_t  totalCores;
};

extern struct args_t parametros;

void check_args(int argc , char **argv);

#endif

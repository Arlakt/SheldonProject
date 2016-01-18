#ifndef ALTITUDE_H
#define ALTITUDE_H

#include <navdata/navdata.h>

extern navdata_t navdata;

void thread_altitude(void * arg);

#endif

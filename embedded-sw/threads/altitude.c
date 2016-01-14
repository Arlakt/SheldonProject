#include <pthread.h>
#include <navdata/navdata.h>

extern int keepRunning;

navdata_t navdata;

void thread_altitude(void * arg)
{
	navdata_init();
	
	// Send start flag
	navdata_start();
	
	navdata_get(&navdata);
	// If in bootstrap mode, configure navdata
	if (navdata.header.state & ARDRONE_NAVDATA_BOOTSTRAP) {
		// Send AT_CONFIG command to enable demo options and wait for ACK
		// TODO
		// Send AT_CTRL command and wait for ACK
		// TODO
	}
	
	// Keep updating navdata
	while (keepRunning) {
		navdata_get(&navdata);
		
		// If no signal received and altitude too high, go down
		// TODO
	}
	
	pthread_exit(NULL);
}


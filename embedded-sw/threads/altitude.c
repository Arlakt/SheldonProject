#include <pthread.h>

#include <movement/at_commands_builder.h>
#include <movement/UDP_sender.h>
#include <navdata/navdata.h>

extern int keepRunning;

navdata_t navdata;

void thread_altitude(void * arg)
{
	navdata_init();
	navdata_start();
	
	do {
		navdata_get(&navdata);
		// If in bootstrap mode, configure navdata
		if (navdata.header.state & ARDRONE_NAVDATA_BOOTSTRAP) {
			char buffer[64];
			// Send AT_CONFIG command to enable demo options
			at_config(buffer, "general:navdata_demo", "TRUE");
			send_message(buffer, 1);
		}
	} while (navdata.header.state & ARDRONE_NAVDATA_BOOTSTRAP);
	
	// Keep updating navdata
	while (keepRunning) {
		navdata_get(&navdata);
		navdata_print(stdout, &navdata);
		// If no signal received and altitude too high, go down
		// TODO
	}
	
	pthread_exit(NULL);
}


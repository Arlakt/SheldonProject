#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include <movement/at_commands_builder.h>
#include <movement/UDP_sender.h>
#include <navdata/navdata.h>

extern int keepRunning;
extern pthread_mutex_t compute_pos_mux;
extern pthread_mutex_t track_pos_mux;
extern pthread_mutex_t at_cmd_mux;

navdata_t navdata;

static void sighandler(int sig)
{
    keepRunning=0;
    printf("CTRL+C signal in altitude thread\n");
}

void * altitude(void * arg)
{
	// Handle SIGINT
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = sighandler;
	sigaction(SIGINT, &act, NULL);
	
	// Start navdata
	navdata_init();
	navdata_start();
	
	// Configure navdata
	do {
		navdata_get(&navdata);
		if (navdata.header.state & ARDRONE_NAVDATA_BOOTSTRAP) {
			// Send AT_CONFIG command to enable demo options
			char buffer[64];
			printf("altitude: enabling demo options...\n");
			pthread_mutex_lock(&at_cmd_mux);
			at_config(buffer, "general:navdata_demo", "TRUE");
			send_message(buffer, 0);
			pthread_mutex_unlock(&at_cmd_mux);
			usleep(40000);
		}
	} while (navdata.header.state & ARDRONE_NAVDATA_BOOTSTRAP);
	printf("altitude: demo options enabled...\n");
	
	// Keep updating navdata
	while (keepRunning) {
		sleep(1);
		navdata_get(&navdata);
		navdata_print(stdout, &navdata);
		// If no signal received and altitude too high, go down
		// TODO
	}
	
	pthread_exit(NULL);
}


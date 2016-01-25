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
extern pthread_mutex_t navdata_mux;

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
	char buffer[64];
	pthread_mutex_lock(&at_cmd_mux);
	printf("altitude: enabling demo options...\n");
	navdata_get(&navdata);
	if (navdata.header.state & ARDRONE_NAVDATA_BOOTSTRAP) {
		// Send AT_CONFIG command to enable demo options
		printf("altitude: sending AT CONFIG...\n");
		at_config(buffer, "general:navdata_demo", "TRUE");
		send_message(buffer, 0);
		usleep(40000);
		do {
			navdata_get(&navdata);
		} while (! (navdata.header.state & ARDRONE_COMMAND_MASK));
		// Send AT_CTRL
		printf("altitude: sending AT CTRL...\n");
		at_ctrl(buffer, 0);
		send_message(buffer, 0);
		usleep(40000);
		do {
			navdata_get(&navdata);
		} while (! (navdata.header.state & ARDRONE_COMMAND_MASK));
	}
	pthread_mutex_unlock(&at_cmd_mux);
	printf("altitude: demo options enabled :-)\n");
	
	// Keep updating navdata
	while (keepRunning) {
		usleep(40000);
		pthread_mutex_lock(&navdata_mux);
		navdata_get(&navdata);
		navdata_print(stdout, &navdata);
		pthread_mutex_unlock(&navdata_mux);
		// If no signal received and altitude too high, go down
		// TODO
	}
	
	pthread_exit(NULL);
}


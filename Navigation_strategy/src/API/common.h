#include <signal.h>
#include <string.h>

extern int keepRunning;

void intHandlerThread1(int sig);
void intHandlerThread2(int sig);
void intHandlerThread3(int sig);

#ifndef SERIAL_H
#define SERIAL_H

int serial_init(char * device);
int serial_start(int fd);
void serial_stop(int fd);
int serial_get_data(int fd, unsigned int * data);

#endif


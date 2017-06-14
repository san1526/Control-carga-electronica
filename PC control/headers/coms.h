#ifndef COMS_H_INCLUDED
#define COMS_H_INCLUDED

char buf_data_in [100];

int set_interface_attribs (int fd, int speed, int parity);

void set_blocking (int fd, int should_block);

void open_port (char *portname,int *fd);

void send_data (int fd,char* buff_out,int nBytes);

void receive_data (int fd,char *data_in, int nbytes);

void send_command(int fd, char *com, char *dato);

void flush(int fd);

#endif // COMS_H_INCLUDED

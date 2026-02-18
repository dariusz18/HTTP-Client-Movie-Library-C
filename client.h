#ifndef CLIENT_H
#define CLIENT_H

void read_line(char *buffer, int size);
void read_data(char *user, char *pass, int size);
void read_login(char *admin, char *user, char *pass, int size);
void read_movie(char *title, char *year_str, char *description, char *rating_str, 
               int title_size, int year_size, int desc_size, int rating_size);

#endif
#ifndef HELPER_H
#define HELPER_H

#include <stdbool.h>

#define HOST "63.32.125.183"
#define PORT 8081
#define BUFLEN 1000

extern char *cookie;
extern char *token;

int conectare();
char *trimite(int sock, char *msg);
void trimite_req(int sock, const char *msg);
char *find(char *str);
char *receive(int sock);
void extrage_cookie(char *rasp);
void extrage_token(char *rasp);

#endif
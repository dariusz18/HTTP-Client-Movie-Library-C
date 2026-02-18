#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "parson.h"
#include "helper.h"

char *cookie = NULL;
char *token = NULL;

int conectare() {
   struct sockaddr_in adr;
   int sock = socket(AF_INET, SOCK_STREAM, 0);
   memset(&adr, 0, sizeof(adr));
   adr.sin_family = AF_INET;
   adr.sin_port = htons(PORT);
   
   inet_aton(HOST, &adr.sin_addr);
   connect(sock, (struct sockaddr*)&adr, sizeof(adr));
   
   return sock;
}

char *trimite(int sock, char *msg) {
   char *rasp = calloc(BUFLEN, sizeof(char));
   int trimis = 0;
   int len = strlen(msg);
   int ok = 0;
   
   for (int total = 0; total < len && (ok == 0); total += trimis) {
       char *buf = msg + total;
       int rest = len - total;
       
       trimis = send(sock, buf, rest, 0);
       if (total + trimis >= len) {
           ok = 1;
       }
   }
   int primit = recv(sock, rasp, BUFLEN - 1, 0);

   if (primit >= 0) {
       rasp[primit] = '\0';
   } else {
       rasp[0] = '\0';
   }
   
   return rasp;
}

void trimite_req(int sock, const char *msg) {
   int trimis = 0;
   int len = strlen(msg);
   int ok = 0;
   
   for (int total = 0; total < len && (ok == 0); total += trimis) {
       const char *buf = msg + total;
       int rest = len - total;
       
       trimis = send(sock, buf, rest, 0);
       if (total + trimis >= len) {
           ok = 1;
       }
   }
}

char *find(char *str) {
    char *p = str;
    while (*p != '\0') {
        if (p[0] == '\r' && p[1] == '\n' && p[2] == '\r' && p[3] == '\n')
            return p;
        p++;
    }
    return NULL;
}

char *receive(int sock) {
   char buf[BUFLEN];
   char *rasp = malloc(BUFLEN);
   
   rasp[0] = '\0';
   int len = 0;
   int bytes = recv(sock, buf, BUFLEN - 1, 0);
   
   while (bytes > 0) {
       buf[bytes] = '\0';
       int new_size = len + bytes + 1;
       char *temp = realloc(rasp, new_size);
       rasp = temp;
       strcat(rasp, buf);
       len += bytes;
     
       bytes = recv(sock, buf, BUFLEN - 1, 0);
   }
   
   return rasp;
}

void extrage_cookie(char *rasp) {
   char *start = strstr(rasp, "Set-Cookie:");
   start += strlen("Set-Cookie:"); 
   char *end = strchr(start, ';');
   cookie = calloc(end - start + 1, sizeof(char));
   strncpy(cookie, start, end - start);
       
}

void extrage_token(char *rasp) {
  char *body = find(rasp);
  if (body != NULL) body += 4;  //salt peste RNRN
  else if (rasp[0] == '{') body = rasp;
  
  JSON_Value *json = json_parse_string(body);
  const char *token_value = json_object_get_string(json_object(json), "token");
  
  if (token_value != NULL) {
      if (token != NULL) free(token);
      int len = strlen(token_value);
      token = calloc(len + 1, sizeof(char));
      memcpy(token, token_value, len);
      token[len] = '\0';
  }
  
  json_value_free(json);
}
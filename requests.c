#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "parson.h"
#include "helper.h"
#include "requests.h"

char *build(const char *url) {
   char *msg = calloc(BUFLEN, sizeof(char));
   sprintf(msg, "GET %s HTTP/1.1\r\nHost: %s\r\n", url, HOST);
   sprintf(msg + strlen(msg), "Cookie: %s\r\n", cookie);
   sprintf(msg + strlen(msg), "Authorization: Bearer %s\r\n", token);
   
   strcat(msg, "Connection: close\r\n\r\n");
   return msg;
}

char *build2(const char *url, const char *tip, const char *data) {
   char *msg = calloc(BUFLEN * 2, sizeof(char));
   
   sprintf(msg, "POST %s HTTP/1.1\r\nHost: %s\r\n", url, HOST);
   sprintf(msg + strlen(msg), "Content-Type: %s\r\n", tip);
   sprintf(msg + strlen(msg), "Content-Length: %ld\r\n", strlen(data));
   sprintf(msg + strlen(msg), "Cookie: %s\r\n", cookie);
   sprintf(msg + strlen(msg), "Authorization: Bearer %s\r\n", token);
   
   strcat(msg, "Connection: close\r\n\r\n");
   strcat(msg, data);
   
   return msg;
}

void login_admin(const char *user, const char *pass) {
   JSON_Value *json = json_value_init_object();
   JSON_Object *obj = json_value_get_object(json);
   json_object_set_string(obj, "username", user);
   json_object_set_string(obj, "password", pass);
   char *data = json_serialize_to_string(json);
   
   char *req = build2("/api/v1/tema/admin/login", "application/json", data);
   int sock = conectare();
   char *rasp = trimite(sock, req);
   free(req);
   close(sock);
   
   if (rasp != NULL) {
       bool eroare = strstr(rasp, "HTTP/1.1 4");
   
   if (eroare) {
       printf("ERROR: login admin\n");
   } else {
       extrage_cookie(rasp);
       printf("SUCCESS: Admin autentificat cu succes\n");
   }
   free(rasp);
   }
   json_value_free(json);
}

void add_user(const char *user, const char *pass) {
   JSON_Value *json = json_value_init_object();
   JSON_Object *obj = json_value_get_object(json);
   json_object_set_string(obj, "username", user);
   json_object_set_string(obj, "password", pass);
   char *data = json_serialize_to_string(json);
   
   char *req = build2("/api/v1/tema/admin/users", "application/json", data);
   int sock = conectare();
   char *rasp = trimite(sock, req);
   free(req);
   close(sock);
   
   if (rasp != NULL) {
       bool eroare = strstr(rasp, "HTTP/1.1 4");
       if (eroare) {
           printf("ERROR: add user\n");
       } else {
           printf("SUCCESS: Utilizator adaugat cu succes\n");
       }
       free(rasp);
   }
   json_value_free(json);
}

void print_users(const char *json_string) {
  printf("SUCCESS: Lista utilizatorilor\n");
  JSON_Value *root = json_parse_string(json_string);
  JSON_Object *obj = json_value_get_object(root);
  JSON_Array *users = json_object_get_array(obj, "users");

  int nr_users = json_array_get_count(users);
  for (int i = 0; i < nr_users; i++) {
      JSON_Object *user = json_array_get_object(users, i);
      const char *username = json_object_get_string(user, "username");
      const char *password = json_object_get_string(user, "password");
      printf("#%d %s:%s\n", i + 1, username, password);
  }
  json_value_free(root);
}

void get_users() {
   char *req = build("/api/v1/tema/admin/users");
   int sock = conectare();
   trimite_req(sock, req);
   free(req);

   char *rasp = receive(sock);
   close(sock);
   bool eroare = strstr(rasp, "HTTP/1.1 4");
   if (eroare) {
       printf("ERROR: utilizatori\n");
       free(rasp);
       return;
   }
   
   char *body = find(rasp);
   if (body != NULL) body += 4;  //salt peste header
   else if (rasp[0] == '{') body = rasp;

   print_users(body);
   free(rasp);
}

void login(const char *admin_username, const char *username, const char *password) {
  JSON_Value *json = json_value_init_object();
  JSON_Object *obj = json_value_get_object(json);
  json_object_set_string(obj, "admin_username", admin_username);
  json_object_set_string(obj, "username", username);
  json_object_set_string(obj, "password", password);
  char *data = json_serialize_to_string(json);

  char *req = build2("/api/v1/tema/user/login", "application/json", data);
  int sock = conectare();
  char *rasp = trimite(sock, req);
  free(req);
  close(sock);

  if (rasp != NULL) {
   bool eroare = strstr(rasp, "HTTP/1.1 4");
   if (eroare) {
          printf("ERROR: login\n");
      } else {
          extrage_cookie(rasp);
          printf("SUCCESS: Autentificare reusita\n");
      }
      free(rasp);
  }
  json_value_free(json);
}

void get_access() {
   char *req = build("/api/v1/tema/library/access");
   int sock = conectare();
   char *rasp = trimite(sock, req);
   free(req);
   close(sock);

   if (rasp != NULL) {
       bool eroare = strstr(rasp, "HTTP/1.1 4");
       if (eroare) {
           printf("ERROR: get_access\n");
       } else {
           extrage_token(rasp);
           printf("SUCCESS: Token JWT primit\n");
       }
       free(rasp);
   }  
 
}

void add_movie(const char *title, int year, const char *description, double rating) {
  JSON_Value *json = json_value_init_object();
  JSON_Object *obj = json_value_get_object(json);
  json_object_set_string(obj, "title", title);
  json_object_set_number(obj, "year", year);
  json_object_set_string(obj, "description", description);
  json_object_set_number(obj, "rating", rating);
  char *data = json_serialize_to_string(json);

  char *req = build2("/api/v1/tema/library/movies", "application/json", data);
  int sock = conectare();
  char *rasp = trimite(sock, req);
  free(req);
  close(sock);
  
  if (rasp != NULL) {
   bool eroare = strstr(rasp, "HTTP/1.1 4");
   
   if (eroare) {
       printf("ERROR: add_movie\n");
   } else {
       printf("SUCCESS: Film adaugat\n");
   }
   free(rasp);
   }
}

void logout_admin() {
   char *req = build("/api/v1/tema/admin/logout");
   int sock = conectare();
   char *rasp = trimite(sock, req);
   free(req);
   close(sock);
   
   if (rasp != NULL) {
   bool eroare = strstr(rasp, "HTTP/1.1 4");
   if (eroare) {
       printf("ERROR: logout_admin\n");
   } else {
       if (cookie != NULL) {
           free(cookie);
       }
       printf("SUCCESS: Admin delogat\n");
   }
   free(rasp);
   }
}

void logout() {
   char *req = build("/api/v1/tema/user/logout");
   int sock = conectare();
   char *rasp = trimite(sock, req);
   free(req);
   close(sock);
   
   if (rasp != NULL) {
    bool eroare = strstr(rasp, "HTTP/1.1 4");
   
   if (eroare) {
       printf("ERROR: logout\n");
   } else {
       free(cookie);
       cookie = NULL;
       free(token);
       token = NULL;
       printf("SUCCESS: Utilizator delogat\n");
   }
   free(rasp);
}
}
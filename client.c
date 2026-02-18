#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client.h"
#include "helper.h"
#include "requests.h"

void read_line(char *buffer, int size) {
   fgets(buffer, size, stdin);
   int i = 0;
   while (buffer[i] != '\n' && buffer[i] != '\0') {
       i++;
   }
   if (buffer[i] == '\n') {
       buffer[i] = '\0';
   }
}

void read_data(char *user, char *pass, int size) {
   printf("username=");
   read_line(user, size);
   
   printf("password=");
   read_line(pass, size);
}

void read_login(char *admin, char *user, char *pass, int size) {
   printf("admin_username=");
   read_line(admin, size);
   
   printf("username=");
   read_line(user, size);
   
   printf("password=");
   read_line(pass, size);
}

void read_movie(char *title, char *year_str, char *description, char *rating_str, 
               int title_size, int year_size, int desc_size, int rating_size) {
   printf("title=");
   read_line(title, title_size);
   
   printf("year=");
   read_line(year_str, year_size);
   
   printf("description=");
   read_line(description, desc_size);
   
   printf("rating=");
   read_line(rating_str, rating_size);
}

int main() {
   char cmd[50];
   setbuf(stdout, NULL);

   while (1) {
       read_line(cmd, sizeof(cmd));

       if (strcmp(cmd, "login_admin") == 0) {
           char user[50], pass[50];
           read_data(user, pass, sizeof(user));
           login_admin(user, pass);
       }
       else if (strcmp(cmd, "add_user") == 0) {
           char user[50], pass[50];
           read_data(user, pass, sizeof(user));
           add_user(user, pass);
       }
       else if (strcmp(cmd, "get_users") == 0) {
           get_users();
       }
       else if (strcmp(cmd, "logout_admin") == 0) {
           logout_admin();
       }
       else if (strcmp(cmd, "login") == 0) {
           char admin[50], user[50], pass[50];
           read_login(admin, user, pass, sizeof(admin));
           login(admin, user, pass);
       }
       else if (strcmp(cmd, "get_access") == 0) {
           get_access();
       }
       else if (strcmp(cmd, "logout") == 0) {
           logout();
       }
       else if (strcmp(cmd, "add_movie") == 0) {
           char title[100], year_str[10], description[100], rating_str[20];
           read_movie(title, year_str, description, rating_str, sizeof(title), sizeof(year_str), sizeof(description), sizeof(rating_str));
           
           int year = atoi(year_str);
           double rating = atof(rating_str);
           add_movie(title, year, description, rating);
       }
       else if (strcmp(cmd, "exit") == 0) {
           break;
       }
   }
   return 0;
}
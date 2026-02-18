#ifndef REQUESTS_H
#define REQUESTS_H

char *build(const char *url);
char *build2(const char *url, const char *tip, const char *data);
void login_admin(const char *user, const char *pass);
void add_user(const char *user, const char *pass);
void print_users(const char *json_string);
void get_users();
void login(const char *admin_username, const char *username, const char *password);
void get_access();
void add_movie(const char *title, int year, const char *description, double rating);
void logout_admin();
void logout();

#endif
#ifndef FUNCTIONSLIST_H
#define FUNCTIONSLIST_H

#include "functions.h"

// Function prototypes from functions.c
void root(const char *query_string, char *response);
void square(const char *query_string, char *response);
void cube(const char *query_string, char *response);
void hello(const char *query_string, char *response);
void prime(const char *query_string, char *response);
void pingpong(const char *query_string, char *response);
void fibonacci(const char *query_string, char *response);

// Array of function pointers (functions with arguments must be handled with extra logic)
void (*function_list[])(const char *, char *) = {
    root,  // Cast for functions that do not use int
    square,
    cube,
    hello,
    prime,
    pingpong,
    fibonacci
};

#endif // FUNCTIONSLIST_H

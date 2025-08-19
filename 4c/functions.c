#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "civetweb.h"

void root(const char *query_string, char *response) {
    if(query_string){
        sprintf(response, "Error!!, root URL shouldn't contains any arguments.\n");
    }
    else{
        sprintf(response ,"Hello World\n");
    }
}

void square(const char *query_string, char *response) {
    char query[1024] = "";
    if(query_string){
        mg_get_var(query_string, strlen(query_string), "num", query, 1024);
    }
    if(strcmp(query, "") == 0){
        sprintf(response, "Square is %d\n", 1);    
    }
    else{
        int num = atoi(query);
        if(num != 0 || (num == 0 && query[0] == '0')){
            sprintf(response, "Square of %d is %d\n", num, num * num);
        }
        else{
            sprintf(response, "Please pass the integer arguments to find the square\n");
        }
    }
}

void cube(const char *query_string, char *response) {
    char query[1024] = "";
    if(query_string){
        mg_get_var(query_string, strlen(query_string), "num", query, 1024);
    }
    if(strcmp(query, "") == 0){
        sprintf(response, "Cube is %d\n", 1);    
    }
    else{
        int num = atoi(query);
        if(num != 0 || (num == 0 && query[0] == '0')){
            sprintf(response, "Cube of %d is %d\n", num, num * num * num);
        }
        else{
            sprintf(response, "Please pass the integer arguments to find the cube\n");
        }
    }
}

void hello(const char *query_string, char *response) {
    char str[1024] = "";
    if(query_string){
        mg_get_var(query_string, strlen(query_string), "str", str, 1024);
    }
    if(strcmp(str, "") == 0){
        sprintf(response, "Hello\n");
    }
    else{
        sprintf(response, "Hello, %s\n", str);
    }
}

void prime(const char *query_string, char *response) {
    char query[1024] = "";
    if(query_string){
        mg_get_var(query_string, strlen(query_string), "num", query, 1024);
    }
    if(strcmp(query, "") == 0){
        sprintf(response, "False\n");
    }
    else{
        int num = atoi(query);
        if(num != 0 || (num == 0 && query[0] == '0')){            
            int is_prime = 1;
            for (int i = 2; i < num; i++) {
                if (num % i == 0) {
                    is_prime = 0;
                    break;
                }
            }
            sprintf(response, is_prime ? "True\n" : "False\n");
        }
        else{
            sprintf(response, "Please pass the integer argument for which we have to check for prime number.\n");
        }
    }
}
void fibonacci(const char *query_string, char *response) {
    char query[1024] = "";
    if(query_string){
        mg_get_var(query_string, strlen(query_string), "num", query, 1024);
    }
    if(strcmp(query, "") == 0){
        sprintf(response, "1\n");
    }
    else{
        int num = atoi(query);
        if(num > 0 || (num == 0 && query[0] == '0')){            
            int a = 0, b=1;
            for(int i=2; i<=num; i++){
                int temp = a;
                a = b;
                b = a+temp;
            }
            sprintf(response, "%d\n", num==0 ? 0 : b );
        }
        else{
            sprintf(response, "Please pass the integer greater than or equal to 0.\n");
        }
    }
}

void pingpong(const char *query_string, char *response) {
    char str[1024] = "";
    if(query_string){
        mg_get_var(query_string, strlen(query_string), "str", str, 1024);
    }
    if(strcmp(str, "") == 0){
        sprintf(response, "PingPong\n");
    }
    else{
        sprintf(response, "pp: %s\n", str);
    }
}

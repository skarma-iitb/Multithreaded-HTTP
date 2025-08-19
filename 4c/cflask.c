#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "civetweb.h"
#include "functions.h"
#include <stdbool.h>
#include "functionslist.h"


typedef struct TrieNode {
    struct TrieNode *children[128];
    int function_id;  // Store function ID or -1 if not an endpoint
    bool is_end_of_url;  // Marks if the node is the end of a valid URL
} TrieNode;

TrieNode* create_node() {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    node->is_end_of_url = false;
    node->function_id = -1;
    for (int i = 0; i < 128; i++) {
        node->children[i] = NULL;
    }
    return node;
}
void insert(TrieNode *treeroot, const char *url, int function_id) {
    TrieNode *current = treeroot;

    for (int i = 0; i < strlen(url); i++) {
        int index = (int)url[i];

        // Create a new node if the path does not exist
        if (current->children[index] == NULL) {
            current->children[index] = create_node();
        }
        current = current->children[index];
    }

    // Mark the end of the URL and set the function ID
    current->is_end_of_url = true;
    current->function_id = function_id;
}

int lookup(TrieNode *treeroot, const char *url) {
    TrieNode *current = treeroot;

    for (int i = 0; i < strlen(url); i++) {
        int index = (int)url[i];

        // If the path doesn't exist, the URL is not found
        if (current->children[index] == NULL) {
            return -1;  // URL not found
        }
        current = current->children[index];
    }

    // Check if it's the end of a valid URL
    if (current != NULL && current->is_end_of_url) {
        return current->function_id;
    }
    return -1;  // URL not found
}

void initialize_mappings(TrieNode *treeroot) {
    insert(treeroot, "/", ROOT);
    insert(treeroot, "/square", SQUARE);
    insert(treeroot, "/cube", CUBE);
    insert(treeroot, "/hello", HELLO);
    insert(treeroot, "/prime", PRIME);
    insert(treeroot, "/pingpong", PINGPONG);
    insert(treeroot, "/helloworld", HELLO);
    insert(treeroot, "/arithmetic/square", SQUARE);
    insert(treeroot, "/arithmetic/cube", CUBE);
    insert(treeroot, "/arithmetic/prime", PRIME);
    insert(treeroot, "/prime", PRIME);
    insert(treeroot, "/fibonacci", FIBONACCI);
    insert(treeroot, "/arithmetic/fibonacci", FIBONACCI);
}


    TrieNode *treeroot;

// Handler function for incoming requests
static int begin_request_handler(struct mg_connection *conn) {
    const struct mg_request_info *request_info = mg_get_request_info(conn);
    const char *uri = request_info->request_uri;
    int function_index = -1;
    char response[1024];

    // // Check for query parameters
    // if (request_info->query_string) {
    //     mg_get_var(request_info->query_string, strlen(request_info->query_string), "num", query, 1024);
    //         mg_get_var(request_info->query_string, strlen(request_info->query_string), "str", str, 1024);
    //         // printf("str: %s and %d\n", str, strcmp(str, ""));
    // }
    // printf("url: %s\n", uri);
    // Map URI to function index
    // if (!strcmp(uri, "/")) {
    //     function_index = ROOT;
    // } else if (!strcmp(uri, "/square")) {
    //     function_index = SQUARE;
    // } else if (!strcmp(uri, "/cube")) {
    //     function_index = CUBE;
    // } else if (!strcmp(uri, "/hello")) {
    //     function_index = HELLO;
    // } else if (!strcmp(uri, "/prime")) {
    //     function_index = PRIME;
    // } else if (!strcmp(uri, "/pingpong")) {
    //     function_index = PINGPONG;
    // } else if (!strcmp(uri, "/helloworld")) {
    //     function_index = HELLO;
    // } else if (!strcmp(uri, "/arithmetic/square")) {
    //     function_index = SQUARE;
    // } else if (!strcmp(uri, "/arithmetic/cube")) {
    //     function_index = CUBE;
    // } else if (!strcmp(uri, "/arithmetic/prime")) {
    //     function_index = PRIME;
    // } else if (!strcmp(uri, "/arithmetic/fibonacci")) {
    //     function_index = FIBONACCI;
    // }

    function_index = lookup(treeroot, uri);

    if (function_index >= 0) {
        // Call the corresponding function with or without arguments
        function_list[function_index](request_info->query_string, response);  // Call with num as an argument
        // } else {
        //     function_list
        // }

        // Send response back to client
        mg_printf(conn,
                  "HTTP/1.1 200 OK\r\n"
                  "Content-Type: text/plain\r\n"
                  "Content-Length: %d\r\n"
                  "\r\n"
                  "%s",
                  (int)strlen(response), response);
    } else {
        mg_printf(conn,
                  "HTTP/1.1 404 Not Found\r\n"
                  "Content-Type: text/plain\r\n"
                  "Content-Length: %d\r\n"
                  "\r\n"
                  "Error: URL not found",
                  (int)strlen("Error: URL not found"));
    }

    return 1; // Mark request as processed
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <port> <num_threads>\n", argv[0]);
        return 1;
    }

    const char *port = argv[1];
    const char *num_threads = argv[2];
    struct mg_context *ctx;
    struct mg_callbacks callbacks;
    // char num_threads_str[10];


    const char *options[] = {"listening_ports", port, "num_threads", num_threads , NULL};

    memset(&callbacks, 0, sizeof(callbacks));

    treeroot = create_node();
    initialize_mappings(treeroot);

    callbacks.begin_request = begin_request_handler;
    ctx = mg_start(&callbacks, NULL, options);

    printf("Server started on port %s with %s threads\n", port, num_threads);
    getchar(); // Wait for user input to stop the server

    mg_stop(ctx);

    return 0;
}

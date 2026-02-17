#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include "game.h"
#include "globals.h"

int main(void) {    
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    struct hostent* host;

    char msg_buffer[BUFF_SIZE];
    char fields[FIELDS];

    FILE* results_file;

    /*open server socket*/
    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        fprintf(stderr, "Error creating server socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*set server socket options*/
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (void*)ON, sizeof(int));

    /*set server address and port*/
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    /*bind server socket to address*/
    if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0) {
        fprintf(stderr, "Error in bind() function call: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*listen for connections*/
    if (listen(server_socket, LISTENQ) < 0) {
        fprintf(stderr, "Error in listen() function call: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on %d port!\n", PORT);

    /*main loop*/
    for (;;) {
        /*accept incoming connection from clients*/
        printf("Server is waiting for connections from clients...\n");
        if ((client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len)) < 0) {
            fprintf(stderr, "Error in accept() function call: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        /*get client information with DNS api*/
        if(NULL == (host = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, sizeof(struct in_addr), AF_INET))) {
            printf("Connected with client:\naddress: %s\n", inet_ntoa(client_addr.sin_addr));
        }
        else {
            printf("Connected with client:\naddress: %s\nname: %s\n", inet_ntoa(client_addr.sin_addr), host->h_name);
        }
        
        for (;;) {
            /*wait for message from client*/
            printf("Waiting for message from client %s...\n", host->h_name);
            if (recv(client_socket, msg_buffer, BUFF_SIZE, 0) < 0) {
                fprintf(stderr, "Error in recv() function call: %s \n", strerror(errno));
                close(client_socket);
                continue;  //wait for another incoming connection
            }

            /*send game history to client*/
            if (msg_buffer[0] == '1') {
                FILE* results_file;
                if ((results_file = fopen("results.txt", "r")) != NULL) {
                    printf("Game History:\n");
                    while (fgets(msg_buffer, BUFF_SIZE, results_file) != NULL) {
                        printf("%s", msg_buffer);
                        if ((send(client_socket, (char *) msg_buffer, BUFF_SIZE, 0)) < 0) {
                            fprintf(stderr, "Error in send() function call: %s \n", strerror(errno));
                            exit(EXIT_FAILURE);
                        }
                    }
                    fclose(results_file);
                }
                /*send End Of Transmission to client*/
                snprintf(msg_buffer, BUFF_SIZE, "EOT");
                if ((send(client_socket, (char *) msg_buffer, BUFF_SIZE, 0)) < 0) {
                    fprintf(stderr, "Error in send() function call: %s \n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
            /*game algorithm*/
            else if (msg_buffer[0] == '2') {
                /*display game instruction for server*/
                game_instruction();

                /*initial fill of board with '='*/
                fields_fill(fields);

                for(;;) {
                    printf("Your move!\n");
                    
                    fields_print(fields);
                    fields_write(fields, 'X');
                    fields_print(fields);

                    if ((send(client_socket, (char *) fields, FIELDS, 0)) < 0) {
                        fprintf(stderr, "Error in send() function call: %s \n", strerror(errno));
                        exit(EXIT_FAILURE);
                    }

                    if (game_summary(fields) != KEEP_PLAYING) {
                        break;
                    }
                    else {
                        printf("Wait for opponent's move!\n");
                    }

                    if ((recv(client_socket, (char *) fields, FIELDS, 0)) < 0) {
                        fprintf(stderr, "Error in recv() function call: %s \n", strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                    fields_print(fields);

                    if (game_summary(fields) != KEEP_PLAYING) {
                        break;
                    }
                } /*game for loop*/
                
                char game_result_str[4];
                switch(game_summary(fields)) {
                    case X_WINS:
                        printf("============\n");
                        printf("| You win! |\n");
                        printf("============\n\n");
                        snprintf(game_result_str, 4, "1 0");
                        break;
                    case O_WINS:
                        printf("=============\n");
                        printf("| You lose! |\n");
                        printf("=============\n\n");
                        snprintf(game_result_str, 4, "0 1");
                        break;
                    case DRAW:
                        printf("=========\n");
                        printf("| Draw! |\n");
                        printf("=========\n\n");
                        snprintf(game_result_str, 4, "1 1");
                        break;
                    default:
                        break;
                }

                if ((results_file = fopen("results.txt", "a")) != NULL) {
                    struct hostent* server; struct hostent* client;
                    server = gethostbyaddr((char*)&server_addr.sin_addr.s_addr, sizeof(struct in_addr), AF_INET);
                    client = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, sizeof(struct in_addr), AF_INET);
                    if (server != NULL && client != NULL) {
                        time_t now = time(NULL);
                        struct tm* time_info;
                        time_info = localtime(&now);
                        char time_str[BUFF_SIZE];
                        strftime(time_str, BUFF_SIZE, "%c", time_info);
                        fprintf(results_file, "%s | %s(server) vs %s(client) | %s\n", time_str, server->h_name, client->h_name, game_result_str);
                    }
                    fclose(results_file);
                }
            }
            else {
                fprintf(stderr, "Client %s disconnected.\n", host->h_name);
                close(client_socket);
                break;
            }
        }/*client for loop*/
    }/*accept for loop*/
    close(server_socket);
    return 0;
}/*main*/

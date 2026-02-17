#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <errno.h>

#include "game.h"
#include "globals.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Provide server address!\n");
        exit(EXIT_FAILURE);
    }

    int client_socket;
    struct sockaddr_in server_addr;
    char msg_buffer[BUFF_SIZE];
	char fields[FIELDS];

    /*open client socket*/
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Error creating server socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    /*set server address and port*/
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(PORT);

    /*connect to server socket*/
    if (-1 == (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))) {
        fprintf(stderr,"Error in connect() function call: %s \n", strerror(errno));
		exit(EXIT_FAILURE);
    }
    printf("Connected with server!\n");

    for (;;) {
        /*get user input*/
        game_menu();
        printf("User input: ");
        scanf("%s", msg_buffer);

        /*receive game history from server*/
        if (msg_buffer[0] == '1') {
            /*send client app user input to server*/
            if ((send(client_socket, msg_buffer, strlen(msg_buffer), 0) < 0 )) {
                fprintf(stderr,"Error in send() function call: %s \n", strerror(errno));
                exit(EXIT_FAILURE);
            }

            printf("Game History:\n");
            for(;;) {
                if ((recv(client_socket, (char *) msg_buffer, BUFF_SIZE, 0) < 0)) {
                    fprintf(stderr, "Error in recv() function call: %s \n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                if (0 == strcmp("EOT", msg_buffer)) {
                    break;
                }
                printf("%s", msg_buffer);
            }
        }
        /*game algorithm*/
        else if (msg_buffer[0] == '2') {
            /*send client app user input to server*/
            if ((send(client_socket, msg_buffer, strlen(msg_buffer), 0) < 0 )) {
                fprintf(stderr,"Error in send() function call: %s \n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            /*display game instruction for client*/
            game_instruction();
            for (;;) {
                printf("Wait for opponent's move!\n");

                if ((recv(client_socket, (char *) fields, FIELDS, 0) < 0)) {
                    fprintf(stderr, "Error in recv() function call: %s \n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
                fields_print(fields);

                if (game_summary(fields) != KEEP_PLAYING) {
                    break;
                }

                printf("Your move!\n");

                fields_write(fields, 'O');
                fields_print(fields);

                if ((send(client_socket, (char *) fields, FIELDS, 0)) < 0) {
                    fprintf(stderr, "Error in send() function call: %s \n", strerror(errno));
                    exit(EXIT_FAILURE);
                }

                if (game_summary(fields) != KEEP_PLAYING) {
                    break;
                }
            }

            switch(game_summary(fields)) {
                case O_WINS:
                    printf("============\n");
                    printf("| You win! |\n");
                    printf("============\n\n");
                    break;
                case X_WINS:
                    printf("=============\n");
                    printf("| You lose! |\n");
                    printf("=============\n\n");
                    break;
                case DRAW:
                    printf("=========\n");
                    printf("| Draw! |\n");
                    printf("=========\n\n");
                    break;
                default:
                    break;
            }
        }
        /*exit from app*/
        else if (msg_buffer[0] == '3') {
            if ((send(client_socket, msg_buffer, strlen(msg_buffer), 0) < 0 )) {
                fprintf(stderr,"Error in send() function call: %s \n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            close(client_socket);
            printf("Exitting...\n");
            exit(EXIT_SUCCESS);
        }
        else {
            fprintf(stderr, "Invalid argument!\n");
            game_menu();
        }
    }
    close(client_socket);
    return 0;
}//main

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "faculty.h"



// Forward declarations for functions you need to define


void handle_client(int client_socket) {
    
    int bytes_received;
    int flag = 1;

    while (flag) {
    char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        char sendMessage[] = "\n\n....................................Acadmia....................................\n"
                    "Please select your role:\n"
                    "1.) Admin\n"
                    "2.) Faculty\n"
                    "3.) Student\n"
                    "4.) Exit\n"
                    "Enter the corresponding number to proceed: ";
	write(client_socket, sendMessage, sizeof(sendMessage));

        bytes_received = read(client_socket, buffer, sizeof(buffer));


        if (bytes_received <= 0) {
            break;  // Client disconnected or error occurred.
        }
        buffer[bytes_received]='\0';
        int retIndex = atoi(buffer);

        switch (retIndex) {
            case 1:
                 	adminHandle(client_socket);
                break;
            case 2:
                	facultyHandle(client_socket);
                break;
           
            case 3:
                          studentHandle(client_socket);
                break;
            default:
                char errorMsg[] = " Invalid choice";
                write(client_socket, errorMsg, sizeof(errorMsg));

                break;
        }
    }

    close(client_socket);
}


int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Bind the socket to an address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080); // Use your desired port number
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        close(server_socket);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        close(server_socket);
        exit(1);
    }

    printf("Server is listening on port 8080...\n");

    while (1) {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        int pid = fork();
        if (pid == 0) {
            // Child process
            close(server_socket);  // Close the parent's socket
            handle_client(client_socket);
            exit(0);
        } else if (pid < 0) {
            perror("Fork failed");
            close(client_socket);
        } else {
            // Parent process
            close(client_socket);  // Close the child's socket
        }
    }

    close(server_socket);
    return 0;
}


// taken from: https://www.geeksforgeeks.org/socket-programming-cc/ and then modified
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

#include "loopbackprog.h"

int receive_clients(int server_fd, struct sockaddr *address, size_t addrlen, int num_clients) {
    char buffer[BUFF_LEN] = {0}; 
    int bytes_read = -1;
    int bytes_sent = -1;
    int new_socket = -1;
    int ret = EXIT_FAILURE;
    int num_messages = num_clients == 1 ? 2 : 1;

    for (int i = 0; i < num_clients; i++) {

	    // Listen for a connection
        if (listen(server_fd, 3) < 0) { 
            perror("listen"); 
            goto recv_cleanup;
        } 

	    // Accept the connection
        if ((new_socket = accept(server_fd, address, (socklen_t*) &addrlen)) < 0) { 
            perror("accept"); 
            goto recv_cleanup;
        }
        printf("Accepted client connection.\n");

        for (int j = 0; j < num_messages; j++) {
	        // Read the client message	
            if (0 >= (bytes_read = read(new_socket, buffer, BUFF_LEN))) {
                perror("read");
                goto recv_cleanup;
            } 
            buffer[bytes_read + 1] = '\0';  // Force null termination
            //printf("%d - %s\n", bytes_read, buffer); 

            // Send the server message
            bytes_sent = send(new_socket , HELLO_SERVER_STR , strlen(HELLO_SERVER_STR) + 1 , 0 ); 
            if (bytes_sent != strlen(HELLO_SERVER_STR) + 1) {
                perror("send");
                goto recv_cleanup;
            }
            //printf("Hello message sent\n"); 
        }
    }
    ret = EXIT_SUCCESS;

recv_cleanup:
    if (new_socket) {
        close(new_socket);    
    }
    return ret;
}

int main(int argc, char const *argv[]) 
{ 
    int server_fd;
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    int ret = EXIT_FAILURE;     
    int num_client_socks = 1;
 
    // Only argument is number of sockets to use - namely, either 1 or 2.
	if (SERVER_ARG_NUM != argc) {
		printf("ERROR: Wrong number of arguments\n");
		printf(SERVER_USAGE_STR);
		goto cleanup;
	}

    num_client_socks = atoi(argv[ARG_NUM_SOCKS]);
    if (num_client_socks != 1 && num_client_socks != 2) {
        printf("ERROR: invalid number of client socks. Should be 2 or 1, not %d\n", num_client_socks);
        goto cleanup;
    } 

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        goto cleanup;
    } 
       
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) { 
        perror("setsockopt"); 
        goto cleanup;
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) { 
        perror("bind failed"); 
        goto cleanup;
    } 
    
    if (EXIT_SUCCESS != receive_clients(server_fd, (struct sockaddr *) &address, addrlen, num_client_socks)) {
        perror("receive_clients failed");
        goto cleanup;
    }

    ret = EXIT_SUCCESS;

cleanup:
    if (server_fd != -1) {
        close(server_fd);
    }
    return ret; 
}

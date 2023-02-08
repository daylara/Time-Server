#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT_NUMBER 60000

int main(int argc, char *argv[])
{
    // Declare variables
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;
    char client_request[256], server_response[256];

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);// Create TCP socket based on internet protocols
    if (socket_desc == -1)// If socket creation fails
    {
        puts("Could not create socket"); // Print error message
        return 1;
    }
 // Set server address and port
    server.sin_family = AF_INET;// Set internet protocols
    server.sin_addr.s_addr = INADDR_ANY;// Set server address to any available address
    server.sin_port = htons(PORT_NUMBER);// Set server port to defined constant

  // Bind socket to address and port
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    // If bind operation fails
    {
        puts("Binding failed");
        return 1;
    }

    // Listen
    listen(socket_desc, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections..."); // Print message
    c = sizeof(struct sockaddr_in);// Get size of client address structure
    new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);// Accept incoming connection and assign new socket descriptor
    if (new_socket < 0)// If accept operation fails
    {
        puts("Accept failed");// Print error message
        return 1;
    }
    puts("Connection accepted");// Print message

    // Receive request from client
        // Process request and send response to client
    while (1)
    {
        // Receive request from client
        if (recv(new_socket, client_request, 256, 0) < 0)// If receive operation fails
        {
            puts("Receive failed");// Print error message
            return 1;
        }

        // Use the "date" command to get the current time and date in various formats
        char *request = strtok(client_request, "\n\r");// Split request string by line breaks
        if (strcmp(request, "GET_TIME") == 0)// If request is for time
        {
            FILE *fp = popen("date +%T", "r");// Execute "date" command and get time in HH:MM:SS format
            fgets(server_response, 256, fp);// Read command output and store in server response string
            pclose(fp);// Close command output stream
        }
        else if (strcmp(request, "GET_DATE") == 0)// If request is for date
        {
            FILE *fp = popen("date +%d.%m.%Y", "r");// Execute "date" command and get date in DD.MM.YYYY format
            fgets(server_response, 256, fp);// Read command output and store in server response string
            pclose(fp);// Close command output stream
        }
        else if (strcmp(request, "GET_TIME_DATE") == 0) // If request is for time and date
        {
            FILE *fp = popen("date +%T\\ %d.%m.%Y", "r");// Execute "date" command and get time and date in HH:MM:SS DD.MM.YYYY format
            fgets(server_response, 256, fp);// Read command output and store in server response string
            pclose(fp);
        }
        else if (strcmp(request, "GET_TIME_ZONE") == 0)// If request is for time zone
        {
            FILE *fp = popen("date +%:z", "r");// Execute "date" command and get time zone in +HHMM or -HHMM format
            fgets(server_response, 256, fp);// Read command output and store in server response string
            pclose(fp);
        }
        else if (strcmp(request, "GET_DAY_OF_WEEK") == 0)// If request is for day of week
        {
            FILE *fp = popen("date +%A", "r");// Execute "date" command and get day of week in full name format (e.g. Monday)
            fgets(server_response, 256, fp);
            pclose(fp);
        }
        else if (strcmp(request, "CLOSE_SERVER") == 0)// If request is to close server
        {
            // Close socket and end programs
            close(new_socket);// Close new socket descriptor
            close(socket_desc);// Close original socket descriptor
            puts("GOOD BYE");
            break;
        }
        else
        {
            strcpy(server_response, "INCORRECT REQUEST\n");// Set server response to error message
        }

        // Send response to client
        if (send(new_socket, server_response, strlen(server_response), 0) < 0)
        { // If send operation fails
            puts("Send failed");
            return 1;
        }

    }


    // Close socket and end program
    

    return 0;
}


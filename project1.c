/*************************************************************
 * Audrey Bergmann
 * Chat Client - Project 1
 * A client-server chat application
 * CS 372
 * Last Modified 7.28.19
 ************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/****************************************************************
 * GETADDRINFO
 * function used to iterate through results
 * code from Beej's guide 5.1
***************************************************************/

int getaddrinfo(const char *node,
                const char *service,
                const struct addrinfo *hints,
                struct addrinfo **);

/****************************************************************
 * CREATE ADDRESS
 * returns server info
 * code from Beej's guide 5.1
***************************************************************/

struct addrinfo* createAddress(char* address, char * port) {
      int status;
      struct addrinfo hints;
      struct addrinfo *servinfo; //points to results
      memset (&hints, 0, sizeof hints); //make sure struct empty
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;  //TCP
      if ((status = getaddrinfo(address, port, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
      }
      return servinfo;
    }

/****************************************************************
 * MAKE SOCKET
 * exchange user names with the server and set up connection
 * code from Beej's guide 5.2
***************************************************************/

int makeSocket(struct addrinfo* servinfo) {
    int s;
    if ((s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1){
        fprintf(stderr, "error creating socket");
        exit(1);
      }
    return s;
  }

/****************************************************************
 * BIND SOCKET
 * makes a connection
 * syntax taken from Beej's guide 5.4
 ***************************************************************/

void bindSocket(int s, struct addrinfo * servinfo) {
    connect(s, servinfo->ai_addr, servinfo->ai_addrlen);
  }

/****************************************************************
 * INITIATE CONTACT
 * exchange user names with the server and set up connection
 ***************************************************************/

  int initiateContact(char * clientName, char * serverName, char * address, char * port) {
    printf("enter username (max 10 characters):\n");
    scanf("%s", clientName);
    printf("username is:");
    printf("%s\n", clientName);
    struct addrinfo * currentAddress = createAddress(address, port);
    int currentSocket = makeSocket(currentAddress);
    bindSocket(currentSocket, currentAddress);
    int sending = send(currentSocket, clientName, strlen(clientName), 0);
    int receiving = recv(currentSocket, serverName, 10, 0);
    printf ("server name is:");
    printf("%s\n", serverName);
    return currentSocket;
  }

  /****************************************************************
   * SENDMESSAGE
   * Sends message from the client to the server
  ***************************************************************/
  void sendMessage (int s, char * clientName, char * input) {
    int bytes = 0;
    printf("%s> ", clientName);
    fgets(input, 500, stdin);
    if (strcmp(input, "\\quit\n") == 0) {
        printf("connection closed by client");
        exit(1);
    }
    bytes = send(s, input, strlen(input), 0);
  }

  /****************************************************************
   * RECEIVEMESSAGE
   * Receives message from the server to the client
  ***************************************************************/
  void receiveMessage (int s, char * server, char * output) {
      int state;
      state = recv(s, output, 500, 0);
      if (state == 0) {
          printf("connection closed by server\n");
          exit(1);
      }
      printf("%s > %s\n", server, output);
      memset(output, 0, sizeof(output));
  }

  /****************************************************************
   * MAIN
  ***************************************************************/
  main (int argc, char *argv[]) {
    char userName[10];
    char serverName[10];
    char input[500];
    char output[500];
    //initiate contact with server and exchange info
    int currentSocket = initiateContact(userName, serverName, argv[1], argv[2]);
    memset(input, 0, sizeof(input));
    memset(output, 0, sizeof(output));
    //clear stdin
    fgets(input, 500, stdin);
    //take turns sending messages while connection is current
    while (1) {
      sendMessage(currentSocket, userName, input);
      receiveMessage(currentSocket, serverName, output);
    }
    close(currentSocket);
  }

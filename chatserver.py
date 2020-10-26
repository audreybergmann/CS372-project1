#!/bin/python

#Audrey Bergmann
#CS 372
#Project 1

import sys
from socket import *


#this code was based off the python server in our textbook on page 168-169

#also https://docs.python.org/release/2.6.5/library/socketserver.html#socketserver-tcpserver-example
#section 21.17.4.1

if __name__ == "__main__":

    serverPort = sys.argv[1]
    #print "server port is"
    #print serverPort
    serverSocket = socket(AF_INET, SOCK_STREAM)
    serverSocket.bind(('', int(serverPort)))

    serverSocket.listen(1)

    #print "the server is ready to receive"

    serverName = ""
    while len(serverName) == 0 or len(serverName) > 10:
        serverName = raw_input("enter username (max 10 characters): ")

    while 1:
        #print "in the while loop"
        connectionSocket, address = serverSocket.accept()
        print "connected on address {}".format(address)
        #print "ready to receive messages"

        clientName = connectionSocket.recv(1024)
        print "ready to chat with {}".format(clientName)
        connectionSocket.send(serverName)
        #sendingMessage = ""

        while 1:
            ##clientName = connectionSocket.recv(1024)
            ##print "ready to chat with {}".format(clientName)
            ##connectionSocket.send(serverName)

            #receivedMessage = ""
            receivedMessage = connectionSocket.recv(501)[0:-1]
            if receivedMessage != "":
                print "{}> {}".format(clientName, receivedMessage)
            if receivedMessage == "":
                print "connection terminated by client. waiting for new connection..."
                break
            sendingMessage = ""
            while len(sendingMessage) == 0 or len(sendingMessage) > 500:
                sendingMessage = raw_input("{}> ".format(serverName))

            if sendingMessage == "\quit":
                print "connection terminated by server. waiting for new connection..."
                break

            connectionSocket.send(sendingMessage)

        connectionSocket.close()

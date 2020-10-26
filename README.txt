To run this chat server

make the server an executable: chmod +x chatserver.py

run the executable: python chatserver.py 55998

in a separate window, compile project1.c: gcc -o project1 project1.c

run client: ./project1 <server address> <server port>
*I tested this on flip3 so the command was: ./project1 flip3.engr.oregonstate.edu 55998

Enter both usernames and send first message from the client. The program should send messages back and forth until \quit command issued.
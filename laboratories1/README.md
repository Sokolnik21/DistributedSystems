# laboratories1
Application that allows users to send (broadcast and display) information without central server through token ring logic simulation.

## specification
Each client is created with 4 arguments:
- text-based client id
- port on which listens
- ip address and port address of the neighbour to whom messages will be send
- information whether user has token after start
- choosen protocol { TCP, UDP }

Messages are sent in one way only. <br>
In the network there is only one token and no application can broadcast until it gets the token (At the beginning the token is free to take) <br>
To send a message it is needed to remove the token and write a message into it (The token can be treated like an envelope) <br>
A receiver reads the message then releases the token (flags, clearing content) and sends the token to the next neighbour. <br>
(For this simulation the token are hold for 1 second by every client via sleep(1000)) <br>
No client is malicious and there is no possibility of having 2 tokens in the network (however, a method that prevents from it would be implemented) <br>
Program must provide:
- to add new users with full functionalities while system is working
- to prevent from situation in which message is sent in endless loop

In addition:
- each client must multicast information when receiving a token (at some group address, common for every client - can be hard-written in code)
- multicast receivers are only loggers - simple applications that saves sender ID and timestamp when client gets the token

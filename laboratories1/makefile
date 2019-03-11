compile:
	gcc -std=c99 Client.c Scanner.c Token.c -o client.out

run:
	./client.out r2d2 127.0.0.1 32 127.0.0.1 42 true TCP

bottle:
	./client.out Bottle 127.0.0.1 32002 127.0.0.1 32003 false TCP

tommy:
	./client.out Tommy 127.0.0.1 32003 127.0.0.1 32004 false TCP

mark:
	./client.out Mark 127.0.0.1 32004 127.0.0.1 32002 true TCP

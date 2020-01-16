all: game

deplace.o: deplaceTran.c
	gcc -Wall -c deplaceTran.c

clientAPI.o: clientAPI.c
	gcc -Wall -c clientAPI.c

snakeAPI.o: snakeAPI.c
	gcc -Wall -c snakeAPI.c

main.o: mainTran.c
	gcc -Wall -c mainTran.c

game: deplace.o clientAPI.o snakeAPI.o main.o
	gcc -Wall -o game deplace.o clientAPI.o snakeAPI.o main.o

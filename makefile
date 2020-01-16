all: game

deplace.o: deplace.c
	gcc -Wall -c deplace.c

clientAPI.o: clientAPI.c
	gcc -Wall -c clientAPI.c

snakeAPI.o: snakeAPI.c
	gcc -Wall -c snakeAPI.c

main.o: main.c
	gcc -Wall -c main.c

game: deplace.o clientAPI.o snakeAPI.o main.o
	gcc -Wall -o game deplace.o clientAPI.o snakeAPI.o main.o

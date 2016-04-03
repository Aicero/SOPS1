OBJ = demon.c

compile :
	gcc $(OBJ) -o dem

kill :
	killall dem

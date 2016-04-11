OBJ = demon.c

compile :
	gcc $(OBJ) -o dem

kill :
	killall dem

sig :
	killall -SIGUSR1 dem

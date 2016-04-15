OBJ = demon.c

compile :
	gcc $(OBJ) -o dem

kill :
	killall dem

sig :
	killall -SIGUSR1 dem

test :
	./dem TESTOWANIE/zrodlo TESTOWANIE/docel -T 3 -S 2
	
testrek :
	./dem TESTOWANIE/zrodlo TESTOWANIE/docel -R -T 3 -S 2

testverb :
	./dem TESTOWANIE/zrodlo TESTOWANIE/docel -r -t 3 -v -s 3
	clear

cleanprep :
	make clean
	make prep

clean :
	rm -rf TESTOWANIE
	
prep :
	mkdir TESTOWANIE
	mkdir TESTOWANIE/zrodlo TESTOWANIE/docel TESTOWANIE/zrodlo/zdir1 TESTOWANIE/zrodlo/zdir2 TESTOWANIE/docel/ddir TESTOWANIE/docel/ddir2
	touch TESTOWANIE/zrodlo/pl1 TESTOWANIE/zrodlo/pl2 TESTOWANIE/zrodlo/pl3 TESTOWANIE/docel/ddir/pliczekjakis TESTOWANIE/docel/ddir/ageagegeaageaggae
	touch TESTOWANIE/docel/pliczekdousuniecia TESTOWANIE/zrodlo/zdir1/pliczekwfolderze TESTOWANIE/docel/ddir2/aeggea
	clear
	ls -l TESTOWANIE/docel TESTOWANIE/zrodlo
